cd /Users/wys/Documents/Yum\ Studio/Yum\ Engine/src/build/macos

echo "MACOS: Compiling Lua..."
gcc -arch x86_64 -arch arm64 /Users/wys/Documents/Yum\ Studio/Yum\ Engine/lua/*.c -std=c17 -O2 -fPIC -c

echo "MACOS: Compiling YumEngine..."
g++ \
    -arch x86_64 -arch arm64 \
    /Users/wys/Documents/Yum\ Studio/Yum\ Engine/src/*.cpp *.o \
    -I/Users/wys/Documents/Yum\ Studio/Yum\ Engine/lua/ \
    -std=c++23 -O2 -fPIC -dynamiclib \
    -o '/Users/wys/Documents/Yum Studio/Yum Engine/src/build/out/libyum.dylib' \
    -fdiagnostics-color=always


echo "Done"
rm -r *.o
