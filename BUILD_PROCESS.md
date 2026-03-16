# 构建流程图解

## 整体流程

```
源代码 (src/*.cc)
    ↓
CMake配置 (cmake ..)
    ↓
编译 (make)
    ↓
可执行文件 (SwallowAndEscape)
    ↓
打包 (build-simple-package.sh)
    ↓
发行包 (.tar.gz / .zip)
    ↓
GitHub Release
    ↓
用户下载
```

---

## 详细流程：本地构建

### 步骤1：CMake配置

```
你运行：cmake ..

CMake做什么：
1. 读取 CMakeLists.txt
2. 查找 SDL2 库
   ├─ 在 /usr/lib 找到 libSDL2.so
   ├─ 在 /usr/include 找到 SDL2 头文件
   └─ 记录路径
3. 生成 Makefile
```

**生成的文件：**
```
build/
├── CMakeCache.txt        # CMake缓存
├── Makefile              # 构建规则
└── CMakeFiles/           # 临时文件
```

### 步骤2：编译

```
你运行：make -j16

Make做什么：
1. 编译 main.cc
   gcc -c src/main.cc -o main.o
   
2. 编译 quote.cc
   gcc -c src/quote.cc -o quote.o
   
3. 链接
   gcc main.o quote.o -lSDL2 -lSDL2_image -lSDL2_mixer -o SwallowAndEscape
```

**生成的文件：**
```
build/
├── SwallowAndEscape      # 可执行文件 ✓
├── main.o                # 目标文件
└── quote.o               # 目标文件
```

### 步骤3：打包

```
你运行：./build-simple-package.sh

脚本做什么：
1. 创建目录结构
   dist/SwallowAndEscape-1.0.0-Linux/
   
2. 复制可执行文件
   cp build/SwallowAndEscape dist/.../
   
3. 复制资源
   cp -r assets dist/.../
   
4. 查找并复制SDL2库
   ldconfig -p | grep libSDL2
   → 找到 /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0
   → 复制到 dist/.../lib/
   
5. 创建启动脚本
   生成 run.sh
   
6. 打包
   tar -czf SwallowAndEscape-1.0.0-Linux.tar.gz ...
```

**最终结构：**
```
dist/
└── SwallowAndEscape-1.0.0-Linux.tar.gz
    └── SwallowAndEscape-1.0.0-Linux/
        ├── SwallowAndEscape      # 可执行文件
        ├── run.sh                # 启动脚本
        ├── lib/                  # 依赖库
        │   ├── libSDL2-2.0.so.0
        │   ├── libSDL2_image-2.0.so.0
        │   └── libSDL2_mixer-2.0.so.0
        ├── assets/               # 游戏资源
        └── README.txt
```

---

## 详细流程：GitHub Actions自动构建

### 触发条件

```
你执行：git push origin v1.0.0

GitHub检测到：
- 推送了一个tag
- tag名称匹配 'v*'
- 触发 .github/workflows/release.yml
```

### 工作流程

```
GitHub Actions启动
    ↓
创建虚拟机
    ├─ Ubuntu虚拟机 (build-linux)
    └─ Windows虚拟机 (build-windows)
    ↓
并行执行两个任务
```

#### Linux构建任务

```
1. 检出代码
   git clone 你的仓库
   
2. 安装依赖
   apt-get install cmake libsdl2-dev ...
   
3. 运行构建脚本
   ./build-simple-package.sh
   
4. 上传构建产物
   上传 dist/SwallowAndEscape-1.0.0-Linux.tar.gz
```

#### Windows构建任务

```
1. 检出代码
   git clone 你的仓库
   
2. 安装MSYS2
   下载并安装MinGW工具链
   
3. 编译Windows版本
   mingw-gcc 编译 .exe
   
4. 复制DLL
   cp SDL2.dll ...
   
5. 打包
   7z a SwallowAndEscape-1.0.0-Windows.zip
   
6. 上传构建产物
   上传 .zip 文件
```

#### 创建Release

```
等待两个构建任务完成
    ↓
下载所有构建产物
    ├─ SwallowAndEscape-1.0.0-Linux.tar.gz
    └─ SwallowAndEscape-1.0.0-Windows.zip
    ↓
创建GitHub Release
    ├─ 标题：v1.0.0
    ├─ 附件：Linux tar.gz
    └─ 附件：Windows zip
```

---

## 用户使用流程

### Linux用户

```
1. 下载
   wget https://github.com/.../SwallowAndEscape-1.0.0-Linux.tar.gz
   
2. 解压
   tar -xzf SwallowAndEscape-1.0.0-Linux.tar.gz
   
3. 运行
   cd SwallowAndEscape-1.0.0-Linux
   ./run.sh
   
运行时发生什么：
   ├─ run.sh 设置 LD_LIBRARY_PATH
   ├─ 系统在 lib/ 目录找到SDL2库
   ├─ 加载 SwallowAndEscape 可执行文件
   ├─ 程序在 assets/ 找到资源文件
   └─ 游戏启动 ✓
```

### Windows用户

```
1. 下载
   下载 SwallowAndEscape-1.0.0-Windows.zip
   
2. 解压
   右键 → 解压到当前文件夹
   
3. 运行
   双击 SwallowAndEscape.exe
   
运行时发生什么：
   ├─ Windows在同目录找到 SDL2.dll
   ├─ 加载所有DLL
   ├─ 程序在 assets/ 找到资源文件
   └─ 游戏启动 ✓
```

---

## 依赖关系图

### 编译时依赖

```
SwallowAndEscape (可执行文件)
    ├─ 依赖 → libSDL2.so
    ├─ 依赖 → libSDL2_image.so
    ├─ 依赖 → libSDL2_mixer.so
    └─ 依赖 → libc.so (系统库)
```

### 运行时依赖

```
用户系统
    ├─ 有 libc.so ✓ (所有Linux都有)
    ├─ 没有 libSDL2.so ✗
    └─ 解决方案：我们打包SDL2库
```

---

## 文件类型说明

### 源代码阶段

| 文件 | 说明 |
|------|------|
| .cc / .cpp | C++源代码 |
| .h | 头文件 |
| CMakeLists.txt | 构建配置 |

### 编译阶段

| 文件 | 说明 |
|------|------|
| .o | 目标文件（编译后的二进制） |
| Makefile | 构建规则 |

### 链接阶段

| 文件 | 说明 |
|------|------|
| SwallowAndEscape | 可执行文件（Linux） |
| SwallowAndEscape.exe | 可执行文件（Windows） |

### 运行时

| 文件 | 说明 |
|------|------|
| .so | 共享库（Linux） |
| .dll | 动态链接库（Windows） |

---

## 命令对应关系

### CMake命令

```bash
cmake ..                          # 配置项目
cmake .. -DCMAKE_BUILD_TYPE=Release  # 发布版本（优化）
cmake .. -DCMAKE_BUILD_TYPE=Debug    # 调试版本（带符号）
```

### Make命令

```bash
make                    # 编译（单线程）
make -j16               # 编译（16线程并行）
make clean              # 清理编译产物
make install            # 安装到系统
make VERBOSE=1          # 显示详细编译命令
```

### Git命令

```bash
git tag v1.0.0          # 创建本地tag
git push origin v1.0.0  # 推送tag到远程
git tag -l              # 列出所有tag
git tag -d v1.0.0       # 删除本地tag
```

---

## 调试技巧

### 查看可执行文件依赖

```bash
# Linux
ldd build/SwallowAndEscape

# 输出：
# libSDL2-2.0.so.0 => /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0
# libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6
```

### 查看库搜索路径

```bash
# 当前库路径
echo $LD_LIBRARY_PATH

# 系统库缓存
ldconfig -p
```

### 测试启动脚本

```bash
# 手动设置库路径
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./SwallowAndEscape
```

---

## 总结

**核心概念：**
1. CMake生成构建规则
2. Make执行编译
3. 打包脚本收集所有文件
4. GitHub Actions自动化整个流程

**关键文件：**
- `CMakeLists.txt` - 告诉CMake如何构建
- `build-simple-package.sh` - 告诉脚本如何打包
- `.github/workflows/release.yml` - 告诉GitHub如何自动化

**记住：**
- 本地测试用 `./build-simple-package.sh`
- 发布用 `git push origin v1.0.0`
- 一切都是自动的！
