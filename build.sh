#!/bin/sh
set -eu

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --clean-first
