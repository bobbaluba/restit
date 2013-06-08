#!/bin/sh
# build package

mkdir -p build
cd build/
qmake ../src
make
