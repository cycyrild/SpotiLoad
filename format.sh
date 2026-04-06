#!/bin/sh

set -eu

CLANG_FORMAT="${CLANG_FORMAT:-clang-format}"
BUILD_DIR="${BUILD_DIR:-build}"

COMPILE_COMMANDS="$BUILD_DIR/compile_commands.json"

[ -f "$COMPILE_COMMANDS" ] || {
    echo "compile_commands.json not found"
    exit 1
}

sed -n 's/.*"file": "\(.*\)".*/\1/p' \
    "$COMPILE_COMMANDS" \
| grep -E '\.(cc|hh)$' \
| sort -u \
| xargs -r "$CLANG_FORMAT" -i
