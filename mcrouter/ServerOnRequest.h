/*
 *  Copyright (c) 2015-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */
#pragma once

#include "mcrouter/CarbonRouterClient.h"
#include "mcrouter/config.h"
#include "mcrouter/lib/network/AsyncMcServer.h"
#include "mcrouter/lib/network/AsyncMcServerWorker.h"
#include "mcrouter/lib/network/CaretHeader.h"
#include "mcrouter/lib/network/McServerRequestContext.h"
#include "mcrouter/lib/network/gen/Memcache.h"

namespace facebook {
namespace memcache {
namespace mcrouter {

template <class Request>
struct ServerRequestContext {
  McServerRequestContext ctx;
  Request req;
  folly::IOBuf reqBuffer;

  ServerRequestContext(
      McServerRequestContext&& ctx_,
      Request&& req_,
      const folly::IOBuf* reqBuffer_)
      : ctx(std::move(ctx_)),
        req(std::move(req_)),
        reqBuffer(reqBuffer_ ? reqBuffer_->cloneAsValue() : folly::IOBuf()) {}
};

template <class RouterInfo>
class ServerOnRequest {
 public:
  template <class Request>
  using ReplyFunction =
      void (*)(McServerRequestContext&& ctx, ReplyT<Request>&& reply);

  ServerOnRequest(
      CarbonRouterClient<RouterInfo>& client,
      bool retainSourceIp,
      bool enablePassThroughMode)
      : client_(client),
        retainSourceIp_(retainSourceIp),
        enablePassThroughMode_(enablePassThroughMode) {}

  template <class Request>
  void onRequest(
      McServerRequestContext&& ctx,
      Request&& req,
      const UmbrellaMessageInfo* headerInfo,
      const folly::IOBuf* reqBuffer) {
    using Reply = ReplyT<Request>;
    send(
        std::move(ctx),
        std::move(req),
        &McServerRequestContext::reply<Reply>,
        headerInfo,
        reqBuffer);
  }
  template <class Request>
  void onRequest(McServerRequestContext&& ctx, Request&& req) {
    using Reply = ReplyT<Request>;
    send(std::move(ctx), std::move(req), &McServerRequestContext::reply<Reply>);
  }

  void onRequest(McServerRequestContext&& ctx, McVersionRequest&&) {
    McVersionReply reply(mc_res_ok);
    reply.value() =
        folly::IOBuf(folly::IOBuf::COPY_BUFFER, MCROUTER_PACKAGE_STRING);

    McServerRequestContext::reply(std::move(ctx), std::move(reply));
  }

  void onRequest(McServerRequestContext&& ctx, McQuitRequest&&) {
    McServerRequestContext::reply(std::move(ctx), McQuitReply(mc_res_ok));
  }

  void onRequest(McServerRequestContext&& ctx, McShutdownRequest&&) {
    McServerRequestContext::reply(std::move(ctx), McShutdownReply(mc_res_ok));
  }

  template <class Request>
  void send(
      McServerRequestContext&& ctx,
      Request&& req,
      ReplyFunction<Request> replyFn,
      const UmbrellaMessageInfo* headerInfo = nullptr,
      const folly::IOBuf* reqBuffer = nullptr) {
    // We just reuse buffers iff:
    //  1) enablePassThroughMode_ is true.
    //  2) headerInfo is not NULL.
    //  3) reqBuffer is not NULL.
    const folly::IOBuf* reusableRequestBuffer =
        (enablePassThroughMode_ && headerInfo) ? reqBuffer : nullptr;

    auto rctx = std::make_unique<ServerRequestContext<Request>>(
        std::move(ctx), std::move(req), reusableRequestBuffer);
    auto& reqRef = rctx->req;
    auto& sessionRef = rctx->ctx.session();

    // if we are reusing the request buffer, adjust the start offset and set
    // it to the request.
    if (reusableRequestBuffer) {
      auto& reqBufferRef = rctx->reqBuffer;
      reqBufferRef.trimStart(headerInfo->headerSize);
      reqRef.setSerializedBuffer(reqBufferRef);
    }

    auto cb = [ sctx = std::move(rctx), replyFn ](
        const Request&, ReplyT<Request>&& reply) {
      replyFn(std::move(sctx->ctx), std::move(reply));
    };

    if (retainSourceIp_) {
      auto peerIp = sessionRef.getSocketAddress().getAddressStr();
      client_.send(reqRef, std::move(cb), peerIp);
    } else {
      client_.send(reqRef, std::move(cb));
    }
  }

 private:
  CarbonRouterClient<RouterInfo>& client_;
  bool retainSourceIp_{false};
  bool enablePassThroughMode_{false};
};
} // mcrouter
} // memcache
} // facebook
