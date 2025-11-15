cd /Users/wys/Documents/Yum\ Studio/Yum\ Engine/src/build/windows32/

echo "WIN32: Compiling Lua..."
/opt/homebrew/bin/i686-w64-mingw32-gcc /Users/wys/Documents/Yum\ Studio/Yum\ Engine/lua/*.c \
    -std=c17 -O2 -fPIC -c -shared

echo "WIN32: Compiling YumEngine..."
/opt/homebrew/bin/i686-w64-mingw32-g++ /Users/wys/Documents/Yum\ Studio/Yum\ Engine/src/*.cpp *.o \
    -I/Users/wys/Documents/Yum\ Studio/Yum\ Engine/lua/ \
    -std=c++23 -O2 -fPIC \
    -m32 \
    -o '/Users/wys/Documents/Yum Studio/Yum Engine/src/build/out/yum32.dll' \
    -static -static-libstdc++ -static-libgcc -shared \
    -fdiagnostics-color=always

echo "Done"
rm -f *.o