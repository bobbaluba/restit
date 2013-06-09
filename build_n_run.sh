#!/bin/sh
# build package and run restit-program

./build.sh

fileName="restit_$(date +%Y%m%d_%H%M%S)"

mkdir -p logs
./build/restit >> logs/$fileName.log
