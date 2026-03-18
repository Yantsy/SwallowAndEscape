# Swallow And Escape

一个单人贪吃蛇游戏，人类玩家对抗电脑玩家。

## 快速开始

### 玩家


### 开发者

#### 构建

```bash
# 安装依赖（Ubuntu/Debian）
sudo apt-get install cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev

# 构建
git clone https://github.com/Yantsy/SwallowAndEscape.git
cd SwallowAndEscape
mkdir build
cmake -B build
cmake --build build

# 运行
./build/SwallowAndEscape
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
- SDL2 (图形、音频、输入)
- CMake (构建系统)

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
├── CMakeLists.txt            # CMake配置
```
## 许可证

MIT License
