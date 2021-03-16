/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */

/*
 *  THIS FILE IS AUTOGENERATED. DO NOT MODIFY IT; ALL CHANGES WILL BE LOST IN
 *  VAIN.
 *
 *  @generated
 */
#include <folly/init/Init.h>

#include <mcrouter/lib/carbon/CmdLineClient.h>
#include <mcrouter/lib/carbon/JsonClient.h>

#include "mcrouter/lib/network/gen/MemcacheMessages.h"

using carbon::JsonClient;
using carbon::tools::CmdLineClient;

namespace {

class MemcacheJsonClient : public JsonClient {
 public:
  explicit MemcacheJsonClient(
      JsonClient::Options opts,
      std::function<void(const std::string&)> onError)
      : JsonClient(std::move(opts), std::move(onError)) {}

 protected:
  bool sendRequestByName(
      const std::string& requestName,
      const folly::dynamic& requestJson,
      folly::dynamic& replyJson) override final {
    if (requestName == facebook::memcache::McAddRequest::name) {
      return sendRequest<facebook::memcache::McAddRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McAppendRequest::name) {
      return sendRequest<facebook::memcache::McAppendRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McCasRequest::name) {
      return sendRequest<facebook::memcache::McCasRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McDecrRequest::name) {
      return sendRequest<facebook::memcache::McDecrRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McDeleteRequest::name) {
      return sendRequest<facebook::memcache::McDeleteRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McExecRequest::name) {
      return sendRequest<facebook::memcache::McExecRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McFlushAllRequest::name) {
      return sendRequest<facebook::memcache::McFlushAllRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McFlushReRequest::name) {
      return sendRequest<facebook::memcache::McFlushReRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McGetRequest::name) {
      return sendRequest<facebook::memcache::McGetRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McGetsRequest::name) {
      return sendRequest<facebook::memcache::McGetsRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McIncrRequest::name) {
      return sendRequest<facebook::memcache::McIncrRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McLeaseGetRequest::name) {
      return sendRequest<facebook::memcache::McLeaseGetRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McLeaseSetRequest::name) {
      return sendRequest<facebook::memcache::McLeaseSetRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McMetagetRequest::name) {
      return sendRequest<facebook::memcache::McMetagetRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McPrependRequest::name) {
      return sendRequest<facebook::memcache::McPrependRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McQuitRequest::name) {
      return sendRequest<facebook::memcache::McQuitRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McReplaceRequest::name) {
      return sendRequest<facebook::memcache::McReplaceRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McSetRequest::name) {
      return sendRequest<facebook::memcache::McSetRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McShutdownRequest::name) {
      return sendRequest<facebook::memcache::McShutdownRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McStatsRequest::name) {
      return sendRequest<facebook::memcache::McStatsRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McTouchRequest::name) {
      return sendRequest<facebook::memcache::McTouchRequest>(
          requestJson, replyJson);
    }
    if (requestName == facebook::memcache::McVersionRequest::name) {
      return sendRequest<facebook::memcache::McVersionRequest>(
          requestJson, replyJson);
    }
    return false;
  }
};
} // anonymous namespace

int main(int argc, char** argv) {
  int tmpArgc = 1;
  folly::init(&tmpArgc, &argv, /* removeFlags */ false);
  CmdLineClient client;
  client.sendRequests<MemcacheJsonClient>(argc, const_cast<const char**>(argv));
  return 0;
}
