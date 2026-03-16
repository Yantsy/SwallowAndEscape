# 实战演示：从零到发布

这是一个真实的操作演示，跟着做一遍你就完全理解了。

---

## 场景：你想发布游戏的第一个版本

### 第1步：检查当前状态

```bash
# 进入项目目录
cd ~/workspace/mine/SwallowAndEscape

# 查看当前文件
ls -la

# 你会看到：
# src/          - 源代码
# assets/       - 游戏资源
# CMakeLists.txt - 构建配置
# build-simple-package.sh - 打包脚本
```

**理解：** 这些是你的项目文件，接下来要把它们变成用户可以运行的程序。

---

### 第2步：本地构建测试

```bash
# 运行构建脚本
./build-simple-package.sh
```

**你会看到什么：**

```
Building SwallowAndEscape...
-- The C compiler identification is GNU 14.2.0
-- The CXX compiler identification is GNU 14.2.0
-- Found PkgConfig: /usr/bin/pkg-config
-- Checking for module 'sdl2'
--   Found sdl2, version 2.30.0
[ 66%] Building CXX object CMakeFiles/SwallowAndEscape.dir/src/main.cc.o
[100%] Linking CXX executable SwallowAndEscape
[100%] Built target SwallowAndEscape
Copying SDL2 libraries...
  Copied: libSDL2-2.0.so.0
  Copied: libSDL2_image-2.0.so.0
  Copied: libSDL2_mixer-2.0.so.0
✓ Package created: dist/SwallowAndEscape-1.0.0-Linux.tar.gz
-rw-rw-r-- 1 user user 2.9M Mar 16 11:43 SwallowAndEscape-1.0.0-Linux.tar.gz
```

**每一行的含义：**

1. `Building SwallowAndEscape...` - 开始构建
2. `The C compiler identification` - CMake检测编译器
3. `Found sdl2, version 2.30.0` - 找到SDL2库
4. `Building CXX object` - 编译C++文件
5. `Linking CXX executable` - 链接生成可执行文件
6. `Copying SDL2 libraries` - 复制依赖库
7. `Package created` - 打包完成

**现在你有了：**
```
dist/SwallowAndEscape-1.0.0-Linux.tar.gz  (2.9MB)
```

---

### 第3步：测试打包结果

```bash
# 进入dist目录
cd dist

# 解压
tar -xzf SwallowAndEscape-1.0.0-Linux.tar.gz

# 查看内容
ls -la SwallowAndEscape-1.0.0-Linux/
```

**你会看到：**
```
drwxrwxr-x 4 user user 4096 Mar 16 11:43 .
drwxrwxr-x 3 user user 4096 Mar 16 11:43 ..
-rwxrwxr-x 1 user user  55K Mar 16 11:43 SwallowAndEscape
-rwxrwxr-x 1 user user  234 Mar 16 11:43 run.sh
drwxrwxr-x 2 user user 4096 Mar 16 11:43 lib
drwxrwxr-x 2 user user 4096 Mar 16 11:43 assets
-rw-rw-r-- 1 user user  512 Mar 16 11:43 README.txt
```

**理解：**
- `SwallowAndEscape` - 你的游戏程序
- `run.sh` - 启动脚本
- `lib/` - SDL2库文件
- `assets/` - 游戏资源（图片、音乐等）

```bash
# 运行游戏测试
cd SwallowAndEscape-1.0.0-Linux
./run.sh
```

**如果游戏窗口打开了，恭喜！打包成功！**

按Ctrl+C退出游戏。

---

### 第4步：查看启动脚本内容

```bash
# 查看run.sh做了什么
cat run.sh
```

**你会看到：**
```bash
#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export LD_LIBRARY_PATH="${SCRIPT_DIR}/lib:${LD_LIBRARY_PATH}"
cd "${SCRIPT_DIR}"
exec ./SwallowAndEscape "$@"
```

**逐行解释：**

```bash
#!/bin/bash
# 告诉系统用bash执行这个脚本

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# 获取脚本所在目录的绝对路径
# 例如：/home/user/Downloads/SwallowAndEscape-1.0.0-Linux

export LD_LIBRARY_PATH="${SCRIPT_DIR}/lib:${LD_LIBRARY_PATH}"
# 设置库搜索路径
# 告诉系统：在 ./lib 目录找SDL2库
# 例如：LD_LIBRARY_PATH=/home/user/Downloads/SwallowAndEscape-1.0.0-Linux/lib

cd "${SCRIPT_DIR}"
# 进入游戏目录
# 这样程序才能找到 assets/ 文件夹

exec ./SwallowAndEscape "$@"
# 运行游戏
# "$@" 表示传递所有命令行参数
```

**为什么需要这个脚本？**

如果直接运行 `./SwallowAndEscape`，会报错：
```
error while loading shared libraries: libSDL2-2.0.so.0: cannot open shared object file
```

因为系统不知道去 `lib/` 目录找SDL2库。

`run.sh` 通过设置 `LD_LIBRARY_PATH` 解决了这个问题。

---

### 第5步：返回项目准备发布

```bash
# 返回项目根目录
cd ~/workspace/mine/SwallowAndEscape

# 查看git状态
git status
```

**你会看到：**
```
位于分支 main
您的分支与上游分支 'origin/main' 一致。

未跟踪的文件:
  build/
  dist/
```

**理解：**
- `build/` 和 `dist/` 是构建产物
- 已经在 `.gitignore` 中，不会提交到git

```bash
# 确认所有代码已提交
git log --oneline -5
```

**你会看到最近的提交：**
```
73e2095 Add comprehensive documentation
df1c5aa Remove unnecessary files
61bc9f6 Add cross-platform build and release system
```

---

### 第6步：创建版本标签

```bash
# 创建v1.0.0标签
git tag v1.0.0

# 查看标签
git tag -l
```

**你会看到：**
```
v1.0.0
```

**理解：**
- tag是git的版本标记
- 类似书签，标记某个提交点
- GitHub Actions会监听tag推送

```bash
# 查看tag详情
git show v1.0.0
```

**你会看到：**
```
tag v1.0.0
commit 73e2095...
Author: Your Name
Date: ...

Add comprehensive documentation
...
```

---

### 第7步：推送标签触发自动构建

```bash
# 推送标签到GitHub
git push origin v1.0.0
```

**你会看到：**
```
Enumerating objects: 1, done.
Counting objects: 100% (1/1), done.
Writing objects: 100% (1/1), 160 bytes | 160.00 KiB/s, done.
Total 1 (delta 0), reused 0 (delta 0)
To github.com:Yantsy/SwallowAndEscape.git
 * [new tag]         v1.0.0 -> v1.0.0
```

**这时发生了什么：**

1. GitHub收到tag推送
2. 检查 `.github/workflows/release.yml`
3. 发现触发条件匹配：`tags: - 'v*'`
4. 启动自动构建

---

### 第8步：观察GitHub Actions

打开浏览器，访问：
```
https://github.com/Yantsy/SwallowAndEscape/actions
```

**你会看到：**

```
┌─────────────────────────────────────────┐
│ Build and Release                       │
│ ● 运行中                                │
│ v1.0.0 · 刚刚                           │
└─────────────────────────────────────────┘
```

点击进去，你会看到3个任务：

```
build-linux     ● 运行中
build-windows   ● 运行中
create-release  ⏸ 等待中
```

**点击 `build-linux` 查看详细日志：**

```
Run actions/checkout@v3
  Syncing repository: Yantsy/SwallowAndEscape
  
Run sudo apt-get update
  Hit:1 http://azure.archive.ubuntu.com/ubuntu jammy InRelease
  Get:2 http://azure.archive.ubuntu.com/ubuntu jammy-updates InRelease
  
Run chmod +x build-simple-package.sh
  Building SwallowAndEscape...
  [ 66%] Building CXX object CMakeFiles/SwallowAndEscape.dir/src/main.cc.o
  [100%] Linking CXX executable SwallowAndEscape
  ✓ Package created: dist/SwallowAndEscape-1.0.0-Linux.tar.gz
  
Run actions/upload-artifact@v3
  Uploading SwallowAndEscape-1.0.0-Linux.tar.gz
```

**等待5-10分钟，直到所有任务变成绿色 ✓**

---

### 第9步：查看Release

访问：
```
https://github.com/Yantsy/SwallowAndEscape/releases
```

**你会看到：**

```
┌─────────────────────────────────────────┐
│ v1.0.0                                  │
│ Latest                                  │
│                                         │
│ Assets                                  │
│ 📦 SwallowAndEscape-1.0.0-Linux.tar.gz │
│    2.9 MB                               │
│ 📦 SwallowAndEscape-1.0.0-Windows.zip  │
│    5.2 MB                               │
└─────────────────────────────────────────┘
```

**恭喜！你的游戏已经发布了！** 🎉

---

### 第10步：测试下载

```bash
# 下载Linux版本
wget https://github.com/Yantsy/SwallowAndEscape/releases/download/v1.0.0/SwallowAndEscape-1.0.0-Linux.tar.gz

# 解压
tar -xzf SwallowAndEscape-1.0.0-Linux.tar.gz

# 运行
cd SwallowAndEscape-1.0.0-Linux
./run.sh
```

**如果游戏正常运行，说明发布成功！**

---

## 场景2：发布更新版本

假设你修复了一个bug，想发布v1.0.1。

### 第1步：修改代码

```bash
# 编辑源代码
vim src/main.cc

# 修复bug...
```

### 第2步：更新版本号

需要修改3个文件：

**1. CMakeLists.txt (第3行)**
```cmake
project(SwallowAndEscape VERSION 1.0.1)  # 改这里
```

**2. build-simple-package.sh (第5行)**
```bash
VERSION="1.0.1"  # 改这里
```

**3. build-windows.sh (第5行)**
```bash
VERSION="1.0.1"  # 改这里
```

### 第3步：本地测试

```bash
# 清理旧构建
rm -rf build dist

# 重新构建
./build-simple-package.sh

# 测试
cd dist
tar -xzf SwallowAndEscape-1.0.1-Linux.tar.gz
cd SwallowAndEscape-1.0.1-Linux
./run.sh
```

### 第4步：提交并发布

```bash
# 返回项目根目录
cd ~/workspace/mine/SwallowAndEscape

# 提交更改
git add .
git commit -m "Fix bug and bump version to 1.0.1"
git push

# 创建新tag
git tag v1.0.1
git push origin v1.0.1
```

**完成！GitHub Actions会自动构建v1.0.1。**

---

## 常见问题实战

### Q1: 构建失败，如何调试？

```bash
# 手动运行构建步骤
cd ~/workspace/mine/SwallowAndEscape
rm -rf build
mkdir build
cd build

# 配置（查看详细输出）
cmake .. -DCMAKE_BUILD_TYPE=Release

# 如果这步失败，说明CMakeLists.txt有问题或缺少依赖

# 编译（查看详细命令）
make VERBOSE=1

# 如果这步失败，说明代码有编译错误
```

### Q2: 游戏运行时找不到资源文件

```bash
# 检查assets是否存在
ls -la build/assets

# 如果不存在，检查CMakeLists.txt的POST_BUILD命令
# 应该有：
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
# )
```

### Q3: 如何删除错误的tag？

```bash
# 删除本地tag
git tag -d v1.0.0

# 删除远程tag
git push origin :refs/tags/v1.0.0

# 重新创建
git tag v1.0.0
git push origin v1.0.0
```

### Q4: GitHub Actions失败了怎么办？

1. 访问 Actions 页面
2. 点击失败的工作流
3. 查看红色 ✗ 的步骤
4. 展开查看错误信息
5. 根据错误修复代码
6. 删除旧tag，重新推送

---

## 总结

**你学会了：**

1. ✅ 运行构建脚本打包游戏
2. ✅ 理解打包后的目录结构
3. ✅ 理解启动脚本的作用
4. ✅ 创建git tag
5. ✅ 推送tag触发自动构建
6. ✅ 在GitHub查看构建过程
7. ✅ 下载和测试发布包
8. ✅ 发布更新版本
9. ✅ 调试常见问题

**核心流程：**
```
修改代码 → 本地测试 → 提交git → 创建tag → 推送tag → 自动构建 → 发布完成
```

**记住：**
- 本地测试用 `./build-simple-package.sh`
- 发布用 `git push origin v1.0.0`
- 出问题看GitHub Actions日志

现在你可以独立发布游戏了！🚀
