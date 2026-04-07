#!/bin/sh

BOOST_DIR="external/boost"

echo "Checking Boost..."

if [ ! -f "$BOOST_DIR/boost/version.hpp" ]; then
    echo "Bootstrapping Boost..."
    cd "$BOOST_DIR" || exit 1
    ./bootstrap.sh || exit 1
    ./b2 headers || exit 1
    echo "Boost bootstrapped"
else
    echo "Boost already bootstrapped"
fi
