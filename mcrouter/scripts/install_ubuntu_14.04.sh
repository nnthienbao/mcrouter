#!/usr/bin/env bash

cd "$(dirname "$0")" || ( echo "cd fail"; exit 1 )

export CC="gcc-5"
export CXX="g++-5"

./get_and_build_everything.sh ubuntu-14.04 "$@"
