cd /Users/wys/Documents/Yum\ Studio/YumEngine/src/build/windows/

echo "Compiling lua4yum for WIN64"
/opt/homebrew/bin/x86_64-w64-mingw32-gcc /Users/wys/Documents/Yum\ Studio/YumEngine/lua/*.c \
    -std=c17 -O2 -fPIC -c

echo "Compiling YumEngine for WIN64"
/opt/homebrew/bin/x86_64-w64-mingw32-g++ /Users/wys/Documents/Yum\ Studio/YumEngine/src/*.cpp *.o \
    -I/Users/wys/Documents/Yum\ Studio/YumEngine/lua/ \
    -std=c++23 -O2 -fPIC -o '/Users/wys/Documents/Yum Studio/YumEngine/src/build/out/yum64.dll' \
    -static -static-libstdc++ -static-libgcc -shared \
    -fdiagnostics-color=always

echo "Done"
rm -r *.o
