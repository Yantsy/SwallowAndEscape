# 游戏发行完整指南

这是一份从零开始的游戏发行教程，包含所有基础知识和详细步骤。

---

## 目录

1. [Shell脚本基础](#1-shell脚本基础)
2. [CMake构建系统](#2-cmake构建系统)
3. [Linux打包流程](#3-linux打包流程)
4. [Windows打包流程](#4-windows打包流程)
5. [GitHub Actions自动化](#5-github-actions自动化)
6. [完整发行流程](#6-完整发行流程)

---

## 1. Shell脚本基础

### 1.1 什么是Shell脚本？

Shell脚本是一个文本文件，包含一系列命令，可以自动执行任务。

### 1.2 基本结构

```bash
#!/bin/bash              # Shebang：告诉系统用bash执行这个脚本
set -e                   # 遇到错误立即退出

# 这是注释
echo "Hello World"       # 打印文本
```

### 1.3 常用命令解释

#### 变量
```bash
APP_NAME="SwallowAndEscape"    # 定义变量（注意=两边不能有空格）
echo $APP_NAME                  # 使用变量（加$符号）
echo ${APP_NAME}                # 推荐写法（更清晰）
```

#### 命令替换
```bash
ARCH=$(uname -m)               # 执行命令并将结果赋值给变量
# 结果：x86_64

NUM_CORES=$(nproc)             # 获取CPU核心数
# 结果：16
```

#### 文件操作
```bash
mkdir -p build                 # 创建目录（-p表示如果已存在不报错）
rm -rf build                   # 删除目录（-r递归，-f强制）
cp file.txt backup.txt         # 复制文件
cp -r dir1 dir2                # 复制目录
```

#### 条件判断
```bash
if [ -f "file.txt" ]; then     # 如果文件存在
    echo "文件存在"
fi

if [ ! -d "build" ]; then      # 如果目录不存在
    mkdir build
fi
```

#### 循环
```bash
for file in *.txt; do          # 遍历所有txt文件
    echo $file
done
```

### 1.4 我们的构建脚本解析

让我们逐行分析 `build-simple-package.sh`：

```bash
#!/bin/bash
# 第1行：Shebang，指定用bash执行

set -e
# 第2行：如果任何命令失败，立即退出脚本

APP_NAME="SwallowAndEscape"
VERSION="1.0.0"
# 第4-5行：定义变量，方便后续使用和修改

echo "Building ${APP_NAME}..."
# 第7行：打印提示信息，${APP_NAME}会被替换为实际值

rm -rf build dist
mkdir -p build dist
# 第9-10行：删除旧的构建目录，创建新的

cd build
# 第11行：进入build目录

cmake .. -DCMAKE_BUILD_TYPE=Release
# 第13行：运行CMake配置
#   .. 表示上级目录（CMakeLists.txt所在位置）
#   -DCMAKE_BUILD_TYPE=Release 表示发布版本（优化编译）

make -j$(nproc)
# 第14行：编译项目
#   -j$(nproc) 表示使用所有CPU核心并行编译（加快速度）

cd ..
# 第16行：返回项目根目录

DIST_DIR="dist/${APP_NAME}-${VERSION}-Linux"
mkdir -p "${DIST_DIR}"
# 第18-19行：创建发行包目录

cp build/SwallowAndEscape "${DIST_DIR}/"
# 第21行：复制可执行文件到发行包

cp -r assets "${DIST_DIR}/"
# 第24行：复制资源文件夹

mkdir -p "${DIST_DIR}/lib"
# 第27行：创建lib目录存放依赖库

for lib in libSDL2-2.0.so.0 libSDL2_image-2.0.so.0 libSDL2_mixer-2.0.so.0; do
    LIB_PATH=$(ldconfig -p | grep "$lib" | awk '{print $NF}' | head -1)
    if [ -n "$LIB_PATH" ]; then
        cp "$LIB_PATH" "${DIST_DIR}/lib/"
        echo "  Copied: $lib"
    fi
done
# 第30-36行：查找并复制SDL2库文件
#   ldconfig -p：列出系统所有共享库
#   grep "$lib"：过滤出我们需要的库
#   awk '{print $NF}'：提取路径
#   head -1：取第一个结果

cat > "${DIST_DIR}/run.sh" << 'RUNEOF'
#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export LD_LIBRARY_PATH="${SCRIPT_DIR}/lib:${LD_LIBRARY_PATH}"
cd "${SCRIPT_DIR}"
exec ./SwallowAndEscape "$@"
RUNEOF
# 第39-45行：创建启动脚本（Heredoc语法）
#   << 'RUNEOF' 表示多行文本输入，直到遇到RUNEOF
#   SCRIPT_DIR：获取脚本所在目录的绝对路径
#   LD_LIBRARY_PATH：告诉系统在哪里找共享库
#   exec：替换当前进程执行游戏

chmod +x "${DIST_DIR}/run.sh"
# 第47行：给启动脚本添加执行权限

tar -czf "${APP_NAME}-${VERSION}-Linux.tar.gz" "${APP_NAME}-${VERSION}-Linux"
# 第70行：打包成tar.gz压缩文件
#   -c：创建归档
#   -z：使用gzip压缩
#   -f：指定文件名

ls -lh "${APP_NAME}-${VERSION}-Linux.tar.gz"
# 第73行：显示生成的文件大小
```

---

## 2. CMake构建系统

### 2.1 什么是CMake？

CMake是一个跨平台的构建系统生成器。它读取`CMakeLists.txt`文件，生成适合你平台的构建文件（Linux上是Makefile，Windows上是Visual Studio项目等）。

### 2.2 CMakeLists.txt逐行解析

```cmake
cmake_minimum_required(VERSION 3.20)
# 要求CMake最低版本为3.20

project(SwallowAndEscape VERSION 1.0.0)
# 定义项目名称和版本号
# 这会创建变量：
#   ${PROJECT_NAME} = "SwallowAndEscape"
#   ${PROJECT_VERSION} = "1.0.0"

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置C++标准为C++17，并要求必须支持

find_package(PkgConfig REQUIRED)
# 查找pkg-config工具（用于查找库）

pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2MIXER REQUIRED SDL2_mixer)
# 使用pkg-config查找SDL2相关库
# 这会创建变量：
#   ${SDL2_INCLUDE_DIRS}：头文件路径
#   ${SDL2_LIBRARIES}：库文件

add_executable(${PROJECT_NAME}
    src/main.cc
    src/quote.h
    src/quote.cc
)
# 创建可执行文件，指定源文件

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2IMAGE_INCLUDE_DIRS}
    ${SDL2MIXER_INCLUDE_DIRS}
)
# 告诉编译器在哪里找头文件

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2IMAGE_LIBRARIES}
    ${SDL2MIXER_LIBRARIES}
)
# 链接SDL2库到可执行文件

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
# 安装规则：将可执行文件安装到bin目录

install(DIRECTORY assets DESTINATION share/${PROJECT_NAME})
# 安装规则：将assets目录安装到share/SwallowAndEscape

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
# 构建后自动复制assets到可执行文件旁边（用于本地测试）
# POST_BUILD：构建完成后执行
# ${CMAKE_COMMAND} -E copy_directory：跨平台的复制命令
# ${CMAKE_SOURCE_DIR}：源代码根目录
# $<TARGET_FILE_DIR:${PROJECT_NAME}>：可执行文件所在目录
```

### 2.3 CMake工作流程

```
1. 配置阶段（cmake ..）
   ↓
   读取CMakeLists.txt
   ↓
   查找依赖库
   ↓
   生成Makefile

2. 编译阶段（make）
   ↓
   编译源文件
   ↓
   链接库
   ↓
   生成可执行文件
```

---

## 3. Linux打包流程

### 3.1 为什么需要打包？

直接分发可执行文件会遇到问题：
- 用户系统可能没有SDL2库
- 库版本可能不兼容
- 路径问题导致找不到资源文件

### 3.2 打包策略

我们采用"半静态"打包：
1. 将SDL2库打包进去（因为版本差异大）
2. 依赖系统的基础库（如libc，所有Linux都有）
3. 提供启动脚本设置库路径

### 3.3 目录结构

```
SwallowAndEscape-1.0.0-Linux/
├── SwallowAndEscape          # 可执行文件
├── run.sh                    # 启动脚本
├── lib/                      # 依赖库
│   ├── libSDL2-2.0.so.0
│   ├── libSDL2_image-2.0.so.0
│   └── libSDL2_mixer-2.0.so.0
├── assets/                   # 游戏资源
│   ├── food.png
│   ├── A.mp3
│   └── ...
└── README.txt                # 使用说明
```

### 3.4 启动脚本详解

```bash
#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# 获取脚本所在目录的绝对路径
# 分解：
#   ${BASH_SOURCE[0]}：脚本文件路径
#   dirname：获取目录部分
#   cd ... && pwd：进入目录并打印绝对路径

export LD_LIBRARY_PATH="${SCRIPT_DIR}/lib:${LD_LIBRARY_PATH}"
# 设置库搜索路径
# LD_LIBRARY_PATH：Linux系统查找共享库的环境变量
# ${SCRIPT_DIR}/lib：我们打包的库
# :${LD_LIBRARY_PATH}：保留原有路径

cd "${SCRIPT_DIR}"
# 进入游戏目录（确保能找到assets）

exec ./SwallowAndEscape "$@"
# 执行游戏
# exec：替换当前进程（不是创建子进程）
# "$@"：传递所有命令行参数
```

### 3.5 查找库文件的命令

```bash
ldconfig -p | grep libSDL2
# ldconfig -p：列出系统所有共享库缓存
# grep：过滤出SDL2相关的

# 输出示例：
# libSDL2-2.0.so.0 (libc6,x86-64) => /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0
```

---

## 4. Windows打包流程

### 4.1 交叉编译概念

交叉编译：在Linux上编译Windows程序

```
Linux系统（开发环境）
    ↓
MinGW工具链（交叉编译器）
    ↓
Windows可执行文件（.exe）
```

### 4.2 MinGW是什么？

MinGW = Minimalist GNU for Windows
- 提供GCC编译器的Windows版本
- 可以在Linux上编译Windows程序
- 包名：`mingw-w64`

### 4.3 Windows打包结构

```
SwallowAndEscape-1.0.0-Windows/
├── SwallowAndEscape.exe      # 可执行文件
├── SDL2.dll                  # SDL2动态库
├── SDL2_image.dll
├── SDL2_mixer.dll
├── assets/                   # 游戏资源
└── README.txt
```

### 4.4 DLL是什么？

DLL = Dynamic Link Library（动态链接库）
- Windows的共享库（类似Linux的.so文件）
- 必须和.exe放在同一目录或系统目录

### 4.5 为什么Windows更简单？

Windows程序查找DLL的顺序：
1. 程序所在目录 ← 我们把DLL放这里
2. 系统目录
3. PATH环境变量

所以只需把DLL和exe放一起即可！

---

## 5. GitHub Actions自动化

### 5.1 什么是GitHub Actions？

GitHub提供的CI/CD服务，可以自动执行任务：
- 代码推送时自动测试
- 创建tag时自动构建发布包
- 完全免费（公开仓库）

### 5.2 工作流程文件结构

文件位置：`.github/workflows/release.yml`

```yaml
name: Build and Release
# 工作流名称

on:
  push:
    tags:
      - 'v*'
  workflow_dispatch:
# 触发条件：
#   - 推送v开头的tag（如v1.0.0）
#   - 手动触发

jobs:
  build-linux:
    runs-on: ubuntu-latest
    # 在Ubuntu虚拟机上运行
    
    steps:
      - uses: actions/checkout@v3
        # 检出代码
      
      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y cmake libsdl2-dev ...
        # 安装依赖
      
      - name: Build Linux package
        run: |
          chmod +x build-simple-package.sh
          ./build-simple-package.sh
        # 执行构建脚本
      
      - name: Upload Linux package
        uses: actions/upload-artifact@v3
        with:
          name: SwallowAndEscape-Linux
          path: dist/SwallowAndEscape-*-Linux.tar.gz
        # 上传构建产物
```

### 5.3 YAML语法基础

```yaml
key: value              # 键值对
list:                   # 列表
  - item1
  - item2
nested:                 # 嵌套
  child: value
multiline: |            # 多行文本
  line 1
  line 2
```

### 5.4 工作流程图

```
推送tag (v1.0.0)
    ↓
GitHub Actions触发
    ↓
    ├─→ build-linux任务
    │   ├─ 创建Ubuntu虚拟机
    │   ├─ 安装依赖
    │   ├─ 执行构建脚本
    │   └─ 上传tar.gz
    │
    └─→ build-windows任务
        ├─ 创建Windows虚拟机
        ├─ 安装MSYS2
        ├─ 编译Windows版本
        └─ 上传zip
    ↓
create-release任务
    ├─ 下载所有构建产物
    └─ 创建GitHub Release
```

---

## 6. 完整发行流程

### 6.1 本地测试（第一次发布前）

```bash
# 1. 测试Linux构建
./build-simple-package.sh

# 2. 验证生成的包
cd dist
tar -xzf SwallowAndEscape-1.0.0-Linux.tar.gz
cd SwallowAndEscape-1.0.0-Linux
./run.sh

# 3. 测试游戏是否正常运行
```

### 6.2 发布新版本

```bash
# 1. 确保所有更改已提交
git status
git add .
git commit -m "准备发布v1.0.0"
git push

# 2. 创建并推送tag
git tag v1.0.0
git push origin v1.0.0

# 3. 等待GitHub Actions完成（5-10分钟）
# 访问：https://github.com/你的用户名/SwallowAndEscape/actions

# 4. 检查Release页面
# 访问：https://github.com/你的用户名/SwallowAndEscape/releases
```

### 6.3 版本号规范

使用语义化版本：`主版本.次版本.修订号`

```
v1.0.0 → v1.0.1  # 修复bug
v1.0.1 → v1.1.0  # 添加新功能
v1.1.0 → v2.0.0  # 重大更新（不兼容旧版本）
```

### 6.4 发布检查清单

- [ ] 代码已测试无bug
- [ ] 版本号已更新（CMakeLists.txt和脚本中）
- [ ] README已更新
- [ ] 所有更改已提交
- [ ] 本地构建测试通过
- [ ] 创建tag并推送
- [ ] GitHub Actions构建成功
- [ ] 下载Release包测试

### 6.5 常见问题排查

#### 构建失败
```bash
# 查看详细错误
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make VERBOSE=1
```

#### 库找不到
```bash
# 检查SDL2是否安装
pkg-config --modversion sdl2

# 检查库路径
ldconfig -p | grep SDL2
```

#### 游戏运行时找不到资源
```bash
# 检查assets是否复制
ls -la build/assets

# 检查工作目录
pwd
ls assets
```

---

## 7. 快速参考

### 常用命令

```bash
# 清理构建
rm -rf build dist

# 本地构建
./build-simple-package.sh

# 查看生成的文件
ls -lh dist/

# 测试打包
cd dist
tar -xzf SwallowAndEscape-1.0.0-Linux.tar.gz
cd SwallowAndEscape-1.0.0-Linux
./run.sh

# 发布新版本
git tag v1.0.1
git push origin v1.0.1
```

### 文件说明

| 文件 | 作用 |
|------|------|
| CMakeLists.txt | CMake配置文件 |
| build-simple-package.sh | Linux打包脚本 |
| build-windows.sh | Windows交叉编译脚本 |
| .github/workflows/release.yml | GitHub Actions配置 |
| SwallowAndEscape.desktop | Linux桌面入口 |
| installer.nsi | Windows安装程序配置 |

---

## 8. 进阶主题

### 8.1 添加图标

Linux需要PNG格式：
```bash
# 转换BMP到PNG
convert logo.bmp logo.png
```

### 8.2 代码签名（可选）

Windows可执行文件签名可以避免安全警告，但需要购买证书。

### 8.3 自动更新

可以集成更新检查功能：
- 检查GitHub API获取最新版本
- 提示用户下载更新

---

## 总结

发行流程本质上就是：

1. **编译**：将源代码变成可执行文件
2. **打包**：收集可执行文件、依赖库、资源文件
3. **分发**：上传到GitHub Release供用户下载

关键点：
- Shell脚本自动化重复任务
- CMake管理跨平台构建
- 打包时包含所有依赖
- GitHub Actions实现自动化

现在你可以：
- 修改版本号
- 运行`./build-simple-package.sh`测试
- 推送tag发布新版本

有任何问题随时问我！
