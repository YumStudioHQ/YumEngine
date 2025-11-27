#!/bin/sh

# Building YumEngine from macOS
#
# This build script compiles YumEngine and its Lua dependency for:
# - macOS (Universal: x86_64 + arm64)
# - Windows (x64, x86, ARM32, ARM64)
# - Linux (x86_64, x86, ARM32, ARM64)
#
# General build steps:
# 1. Compile Lua (submodule):
#      <C compiler> -fPIC -c lua/*.c [FLAGS_CC]
#
# 2. Compile YumEngine (or your C++ application):
#      <C++ compiler> src/*.cpp tmp/*.o -Ilua/ -std=c++23 -fPIC
#
# Notes:
# - Prefer the C++ API if building your own application using YumEngine.
# - If using the C++ API, define YUM_CXX_API_NO_C_EXPORT to disable C exports.
# - Lua is compiled with C17 and YumEngine with C++23.
# - Optimization flags are -O2.
#
# This script requires:
# - macOS: clang (gcc/g++)
# - Windows: mingw-w64 toolchains
# - Linux: Zig (for cross-compilation)
#
# Output files:
#   macOS:     libyum.dylib
#   Windows:   winyum*.dll
#   Linux:     libyum*.so


#
# COMMON FLAGS
#
FLAGS_CC="-std=c17 -Wall -Wextra -fPIC -O2 -c"
FLAGS_CXX="-std=c++23 -Wall -shared -Wextra -fPIC -O2 -Ilua/"
ZIG_CC="zig cc"
ZIG_CXX="zig c++"
OUTDIR="bin"

rm -rf $OUTDIR
mkdir $OUTDIR
rm -rf tmp
mkdir -p tmp

lua bump-version.lua "$@"

########################################
# macOS Universal (x86_64 + arm64)
########################################
echo "Building for MacOS..."
for file in lua/*.c; do
  gcc $FLAGS_CC -arch x86_64 -arch arm64 "$file" -o "tmp/$(basename "$file" .c).o"
  echo "Done $file -- (macos)"
done

g++ $FLAGS_CXX -dynamiclib -arch x86_64 -arch arm64 \
    src/*.cpp tmp/*.o -o $OUTDIR/libyum.dylib

echo "Done libyum.dylib"
rm tmp/*.o

########################################
# Windows x64
########################################
echo "Building for Windows x64..."
for file in lua/*.c; do
  x86_64-w64-mingw32-gcc $FLAGS_CC "$file" -o "tmp/$(basename "$file" .c).o"
  echo "Done $file -- (win64)"
done

x86_64-w64-mingw32-g++ $FLAGS_CXX \
  -static -static-libstdc++ -static-libgcc \
  src/*.cpp tmp/*.o -o $OUTDIR/winyum64.dll

echo "Done winyum64.dll"
rm tmp/*.o



########################################
# Windows x86 (32-bit)
########################################
echo "Building for Windows x86..."
for file in lua/*.c; do
  i686-w64-mingw32-gcc -m32 $FLAGS_CC "$file" -o "tmp/$(basename "$file" .c).o"
  echo "Done $file -- (win32)"
done

i686-w64-mingw32-g++ -m32 $FLAGS_CXX \
  -static -static-libstdc++ -static-libgcc -shared \
  src/*.cpp tmp/*.o -o $OUTDIR/winyum32.dll

echo "Done winyum32.dll"
rm tmp/*.o



########################################
# Windows ARM64
########################################
echo "WINDOWS ARM 64-bit (aarch64)"
for file in lua/*.c; do
  $ZIG_CC -target aarch64-windows-gnu $FLAGS_CC "$file" \
    -o "tmp/$(basename "$file" .c).o"
  echo "Done $file -- (win arm64)"
done

$ZIG_CXX -target aarch64-windows-gnu $FLAGS_CXX -shared \
  src/*.cpp tmp/*.o -o $OUTDIR/winyum_arm64.dll

echo "win arm64: Done!"
rm tmp/*.o



########################################
# Linux ARM64
########################################
echo "LINUX ARM 64-bit (aarch64)"
for file in lua/*.c; do
  $ZIG_CC -target aarch64-linux-gnu $FLAGS_CC "$file" \
    -o "tmp/$(basename "$file" .c).o"
  echo "Done $file -- (linux arm64)"
done

$ZIG_CXX -target aarch64-linux-gnu $FLAGS_CXX -shared \
  src/*.cpp tmp/*.o -o $OUTDIR/libyum_arm64.so

echo "linux arm64: Done!"
rm tmp/*.o



########################################
# Linux x86_64
########################################
echo "LINUX 64-bit (x86_64)"
for file in lua/*.c; do
  $ZIG_CC -target x86_64-linux-gnu $FLAGS_CC "$file" \
    -o "tmp/$(basename "$file" .c).o"
  echo "Done $file -- (linux64)"
done

$ZIG_CXX -target x86_64-linux-gnu $FLAGS_CXX -shared \
  src/*.cpp tmp/*.o -o $OUTDIR/libyum64.so

echo "linux64: Done!"
rm tmp/*.o



########################################
# Linux x86 (32-bit)
########################################
echo "LINUX 32-bit (i686)"
for file in lua/*.c; do
  $ZIG_CC -target x86-linux-gnu $FLAGS_CC "$file" \
    -o "tmp/$(basename "$file" .c).o"
  echo "Done $file -- (linux32)"
done

$ZIG_CXX -target x86-linux-gnu $FLAGS_CXX -shared \
  src/*.cpp tmp/*.o -o $OUTDIR/libyum32.so

echo "linux32: Done!"
rm tmp/*.o
rm -rf tmp