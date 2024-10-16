#!/bin/sh -e

BASEDIR=$(realpath "$(dirname "$0")")
ROOTDIR=$(realpath "$BASEDIR/..")

cmake -S . -B "$ROOTDIR/build" -DCMAKE_C_COMPILER=clang
cmake --build "$ROOTDIR/build"
