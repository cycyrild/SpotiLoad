#!/bin/sh
set -eu

./bootstrap-boost.sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --clean-first
