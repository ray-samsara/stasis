#!/usr/bin/env bash

set -xe

cmake -B ./build/stasis -S . -G "Unix Makefiles"
cmake --build ./build/stasis -j "$(nproc)"