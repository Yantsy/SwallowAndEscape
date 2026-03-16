#!/bin/bash
set -e

# 配置
APP_NAME="SwallowAndEscape"
VERSION="1.0.0"
ARCH=$(uname -m)

# 清理并创建构建目录
rm -rf build AppDir
mkdir -p build
cd build

# 构建项目
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# 安装到AppDir
DESTDIR=../AppDir make install

cd ..

# 创建AppDir结构
mkdir -p AppDir/usr/share/applications
mkdir -p AppDir/usr/share/icons/hicolor/256x256/apps

# 复制desktop文件
cp ${APP_NAME}.desktop AppDir/usr/share/applications/

# 复制图标
if [ -f assets/logo.png ]; then
    cp assets/logo.png AppDir/usr/share/icons/hicolor/256x256/apps/swallowandscape.png
elif [ -f assets/logo.bmp ]; then
    cp assets/logo.bmp AppDir/usr/share/icons/hicolor/256x256/apps/swallowandscape.png
else
    # 创建一个简单的占位图标
    echo "Warning: No icon found, creating placeholder"
fi

# 创建AppRun脚本
cat > AppDir/AppRun << 'APPRUN_EOF'
#!/bin/bash
SELF=$(readlink -f "$0")
HERE=${SELF%/*}
export PATH="${HERE}/usr/bin:${PATH}"
export LD_LIBRARY_PATH="${HERE}/usr/lib:${LD_LIBRARY_PATH}"
cd "${HERE}/usr/share/SwallowAndEscape"
exec "${HERE}/usr/bin/SwallowAndEscape" "$@"
APPRUN_EOF

chmod +x AppDir/AppRun

# 下载linuxdeploy
if [ ! -f linuxdeploy-${ARCH}.AppImage ]; then
    echo "Downloading linuxdeploy..."
    wget -q https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-${ARCH}.AppImage
    chmod +x linuxdeploy-${ARCH}.AppImage
fi

# 使用linuxdeploy打包依赖
echo "Building AppImage..."
./linuxdeploy-${ARCH}.AppImage --appdir AppDir --output appimage

# 查找并重命名生成的AppImage
GENERATED_APPIMAGE=$(ls ${APP_NAME}-*.AppImage 2>/dev/null | head -n1)
if [ -n "$GENERATED_APPIMAGE" ]; then
    mv "$GENERATED_APPIMAGE" ${APP_NAME}-${VERSION}-${ARCH}.AppImage
    echo "✓ AppImage created: ${APP_NAME}-${VERSION}-${ARCH}.AppImage"
    ls -lh ${APP_NAME}-${VERSION}-${ARCH}.AppImage
else
    echo "✗ Failed to create AppImage"
    exit 1
fi
