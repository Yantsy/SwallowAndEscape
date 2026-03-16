#!/bin/bash
set -e

APP_NAME="SwallowAndEscape"
VERSION="1.0.0"

echo "Building ${APP_NAME}..."

# 清理并构建
rm -rf build dist
mkdir -p build dist
cd build

cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

cd ..

# 创建发行包目录
DIST_DIR="dist/${APP_NAME}-${VERSION}-Linux"
mkdir -p "${DIST_DIR}"

# 复制可执行文件
cp build/SwallowAndEscape "${DIST_DIR}/"

# 复制资源
cp -r assets "${DIST_DIR}/"

# 复制依赖的共享库
echo "Copying SDL2 libraries..."
mkdir -p "${DIST_DIR}/lib"

# 查找并复制SDL2相关库
for lib in libSDL2-2.0.so.0 libSDL2_image-2.0.so.0 libSDL2_mixer-2.0.so.0; do
    LIB_PATH=$(ldconfig -p | grep "$lib" | awk '{print $NF}' | head -1)
    if [ -n "$LIB_PATH" ]; then
        cp "$LIB_PATH" "${DIST_DIR}/lib/"
        echo "  Copied: $lib"
    fi
done

# 创建启动脚本
cat > "${DIST_DIR}/run.sh" << 'RUNEOF'
#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export LD_LIBRARY_PATH="${SCRIPT_DIR}/lib:${LD_LIBRARY_PATH}"
cd "${SCRIPT_DIR}"
exec ./SwallowAndEscape "$@"
RUNEOF

chmod +x "${DIST_DIR}/run.sh"

# 创建README
cat > "${DIST_DIR}/README.txt" << 'READMEEOF'
Swallow And Escape - Linux版本

运行方法：
  ./run.sh

或者如果系统已安装SDL2库：
  ./SwallowAndEscape

系统要求：
  - SDL2 2.0+
  - SDL2_image 2.0+
  - SDL2_mixer 2.0+

如果遇到库依赖问题，请安装：
  Ubuntu/Debian: sudo apt-get install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-mixer-2.0-0
  Fedora: sudo dnf install SDL2 SDL2_image SDL2_mixer
  Arch: sudo pacman -S sdl2 sdl2_image sdl2_mixer
READMEEOF

# 打包
cd dist
tar -czf "${APP_NAME}-${VERSION}-Linux.tar.gz" "${APP_NAME}-${VERSION}-Linux"

echo "✓ Package created: dist/${APP_NAME}-${VERSION}-Linux.tar.gz"
ls -lh "${APP_NAME}-${VERSION}-Linux.tar.gz"
