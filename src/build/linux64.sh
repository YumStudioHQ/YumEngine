#!/bin/bash
set -e

OUT="out"

echo "LINUX 64-bit"
docker run --rm -it \
  -e FORCE_COLOR=1 \
  -v "./":/src:ro \
  -v "$OUT":/out \
  yumengine-builder \
  bash -c "
    mkdir -p /tmp/build64 &&
    cd /tmp/build64 &&
    cp -r /src/* . &&
    echo 'Compiling Lua...'  &&
    gcc lua/*.c -std=c17 -O2 -fPIC -c -fdiagnostics-color=always &&
    echo 'Compiling YumEngine core...'  &&
    g++ src/*.cpp *.o -I lua/ -std=c++23 -O2 -fPIC -shared -fdiagnostics-color=always -o /out/libyum64.so
  "

echo "LINUX64: Done!"
