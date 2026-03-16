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

## 📚 文档

### 新手入门
- **[实战演示](HANDS_ON_TUTORIAL.md)** ⭐ 推荐！跟着做一遍，完全理解整个流程
- **[快速入门](QUICK_START.md)** - 5分钟学会发布游戏

### 深入学习
- **[完整指南](RELEASE_GUIDE.md)** - 详细教程，包含：
  - Shell脚本基础（逐行解释）
  - CMake构建系统（每个命令的含义）
  - Linux打包原理（为什么这样做）
  - Windows交叉编译（MinGW是什么）
  - GitHub Actions自动化（YAML语法）
  
- **[构建流程图解](BUILD_PROCESS.md)** - 可视化流程图：
  - 从源代码到可执行文件
  - 依赖关系图
  - 用户使用流程
  - 调试技巧

### 文档导航

```
不知道从哪开始？
    ↓
想快速上手？ → HANDS_ON_TUTORIAL.md (实战演示)
    ↓
想了解原理？ → RELEASE_GUIDE.md (完整指南)
    ↓
想看流程图？ → BUILD_PROCESS.md (图解)
    ↓
只想发布？ → QUICK_START.md (快速入门)
```

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
- SDL2 (图形、音频、输入)
- CMake (构建系统)
- GitHub Actions (CI/CD)

## 项目结构

```
SwallowAndEscape/
├── src/                      # 源代码
│   ├── main.cc              # 主程序
│   ├── quote.cc/h           # 辅助函数
├── assets/                   # 游戏资源
│   ├── food.png             # 食物贴图
│   ├── A.mp3                # 背景音乐
│   └── gamecontrollerdb.txt # 手柄映射
├── build-simple-package.sh   # Linux打包脚本
├── build-windows.sh          # Windows交叉编译脚本
├── CMakeLists.txt            # CMake配置
└── .github/workflows/        # GitHub Actions配置
    └── release.yml
```

## 发布流程

```
1. 修改代码
2. 本地测试: ./build-simple-package.sh
3. 提交代码: git commit && git push
4. 创建标签: git tag v1.0.0 && git push origin v1.0.0
5. GitHub Actions自动构建Linux和Windows版本
6. 自动发布到Releases页面
```

## 许可证

MIT License
