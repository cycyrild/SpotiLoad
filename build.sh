#!/bin/sh
set -eu

cmake --preset default
cmake --build --preset default --clean-first -v
