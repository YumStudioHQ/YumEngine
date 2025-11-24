#!/bin/bash
set -e

OUT="out"

echo "LINUX 32-bit"
docker run --rm -it \
  -e FORCE_COLOR=1 \
  -v "./":/src:ro \
  -v "$OUT":/out \
  yumengine-builder \
  bash -c "
    mkdir -p /tmp/build32 &&
    cd /tmp/build32 &&
    cp -r /src/* . &&
    echo 'Compiling Lua...'  &&
    gcc -m32 lua/*.c -std=c17 -O2 -fPIC -c -fdiagnostics-color=always &&
    echo 'Compiling YumEngine core...'  &&
    g++ -m32 src/*.cpp *.o -I lua/ -std=c++23 -O2 -fPIC -shared -fdiagnostics-color=always -o /out/libyum32.so
  "

echo "LINUX32:Done!"
