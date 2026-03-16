#!/bin/bash
set -e

# 安装MinGW交叉编译工具链（首次运行需要）
# sudo apt-get install -y mingw-w64 cmake

APP_NAME="SwallowAndEscape"
VERSION="1.0.0"

# 清理构建目录
rm -rf build-windows
mkdir -p build-windows
cd build-windows

# 下载Windows版SDL2库（如果还没有）
if [ ! -d "../SDL2-mingw" ]; then
    echo "Downloading SDL2 for Windows..."
    cd ..
    wget https://github.com/libsdl-org/SDL/releases/download/release-2.28.5/SDL2-devel-2.28.5-mingw.tar.gz
    tar -xzf SDL2-devel-2.28.5-mingw.tar.gz
    mv SDL2-2.28.5 SDL2-mingw
    
    wget https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-devel-2.8.2-mingw.tar.gz
    tar -xzf SDL2_image-devel-2.8.2-mingw.tar.gz
    mv SDL2_image-2.8.2 SDL2_image-mingw
    
    wget https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.0/SDL2_mixer-devel-2.8.0-mingw.tar.gz
    tar -xzf SDL2_mixer-devel-2.8.0-mingw.tar.gz
    mv SDL2_mixer-2.8.0 SDL2_mixer-mingw
    
    cd build-windows
fi

# 使用MinGW工具链编译
x86_64-w64-mingw32-g++ \
    -o ${APP_NAME}.exe \
    ../src/main.cc ../src/quote.cc \
    -I../SDL2-mingw/x86_64-w64-mingw32/include/SDL2 \
    -I../SDL2_image-mingw/x86_64-w64-mingw32/include/SDL2 \
    -I../SDL2_mixer-mingw/x86_64-w64-mingw32/include/SDL2 \
    -L../SDL2-mingw/x86_64-w64-mingw32/lib \
    -L../SDL2_image-mingw/x86_64-w64-mingw32/lib \
    -L../SDL2_mixer-mingw/x86_64-w64-mingw32/lib \
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer \
    -mwindows -static-libgcc -static-libstdc++

# 复制DLL文件
cp ../SDL2-mingw/x86_64-w64-mingw32/bin/SDL2.dll .
cp ../SDL2_image-mingw/x86_64-w64-mingw32/bin/*.dll .
cp ../SDL2_mixer-mingw/x86_64-w64-mingw32/bin/*.dll .

# 复制资源文件
cp -r ../assets .

# 创建发行包
mkdir -p ${APP_NAME}-${VERSION}-Windows
cp ${APP_NAME}.exe ${APP_NAME}-${VERSION}-Windows/
cp *.dll ${APP_NAME}-${VERSION}-Windows/
cp -r assets ${APP_NAME}-${VERSION}-Windows/
cp ../README.md ${APP_NAME}-${VERSION}-Windows/ 2>/dev/null || true

# 打包成ZIP
zip -r ${APP_NAME}-${VERSION}-Windows.zip ${APP_NAME}-${VERSION}-Windows

echo "Windows build created: ${APP_NAME}-${VERSION}-Windows.zip"
