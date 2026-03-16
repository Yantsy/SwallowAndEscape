# Swallow And Escape

一个双人贪吃蛇游戏，玩家对抗AI。

## 快速开始

### 玩家

下载最新版本：[Releases](https://github.com/Yantsy/SwallowAndEscape/releases)

- **Linux**: 下载 `.tar.gz`，解压后运行 `./run.sh`
- **Windows**: 下载 `.zip`，解压后运行 `SwallowAndEscape.exe`

### 开发者

#### 构建

```bash
# 安装依赖（Ubuntu/Debian）
sudo apt-get install cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev

# 构建
mkdir build && cd build
cmake ..
make

# 运行
./SwallowAndEscape
```

#### 打包发布

```bash
# 本地打包测试
./build-simple-package.sh

# 发布新版本
git tag v1.0.0
git push origin v1.0.0
```

## 文档

- **[快速入门](QUICK_START.md)** - 5分钟学会发布游戏
- **[完整指南](RELEASE_GUIDE.md)** - 详细的发行教程（包含Shell、CMake、打包等基础知识）
- **[构建流程](BUILD_PROCESS.md)** - 图解构建和发布流程

## 游戏控制

### 键盘
- `W/A/S/D` 或 `方向键` - 移动
- `Q` - 停止
- `E` - 向右移动

### 手柄
- `D-Pad` - 移动
- `Start` - 停止

## 技术栈

- C++17
- SDL2
- CMake

## 许可证

MIT License
