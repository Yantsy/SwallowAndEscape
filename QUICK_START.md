# 快速入门：5分钟发布你的游戏

## 第一次发布（完整步骤）

### 1. 本地测试构建

```bash
# 进入项目目录
cd SwallowAndEscape

# 运行构建脚本
./build-simple-package.sh

# 你会看到：
# Building SwallowAndEscape...
# [编译过程...]
# ✓ Package created: dist/SwallowAndEscape-1.0.0-Linux.tar.gz
```

**这个脚本做了什么？**
1. 清理旧文件
2. 用CMake配置项目
3. 编译代码
4. 复制SDL2库
5. 打包成tar.gz

### 2. 测试打包结果

```bash
# 解压测试
cd dist
tar -xzf SwallowAndEscape-1.0.0-Linux.tar.gz
cd SwallowAndEscape-1.0.0-Linux

# 运行游戏
./run.sh

# 如果游戏正常运行，说明打包成功！
```

### 3. 发布到GitHub

```bash
# 返回项目根目录
cd ../..

# 提交所有更改
git add .
git commit -m "准备发布v1.0.0"
git push

# 创建版本标签
git tag v1.0.0
git push origin v1.0.0
```

### 4. 等待自动构建

1. 打开浏览器访问：`https://github.com/你的用户名/SwallowAndEscape/actions`
2. 你会看到一个正在运行的工作流
3. 等待5-10分钟，直到显示绿色✓

### 5. 下载发布包

访问：`https://github.com/你的用户名/SwallowAndEscape/releases`

你会看到：
- `SwallowAndEscape-1.0.0-Linux.tar.gz` (Linux版本)
- `SwallowAndEscape-1.0.0-Windows.zip` (Windows版本)

**完成！** 🎉

---

## 后续发布（更新版本）

### 修改版本号

编辑3个地方的版本号：

1. **CMakeLists.txt** (第3行)
```cmake
project(SwallowAndEscape VERSION 1.0.1)  # 改这里
```

2. **build-simple-package.sh** (第5行)
```bash
VERSION="1.0.1"  # 改这里
```

3. **build-windows.sh** (第5行)
```bash
VERSION="1.0.1"  # 改这里
```

### 发布新版本

```bash
# 1. 测试构建
./build-simple-package.sh

# 2. 提交更改
git add .
git commit -m "发布v1.0.1"
git push

# 3. 创建新tag
git tag v1.0.1
git push origin v1.0.1

# 完成！
```

---

## 常见问题

### Q: 构建失败怎么办？

```bash
# 查看详细错误信息
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make VERBOSE=1
```

### Q: 如何只构建不发布？

```bash
# 只运行构建脚本，不推送tag
./build-simple-package.sh
```

### Q: 如何删除错误的tag？

```bash
# 删除本地tag
git tag -d v1.0.0

# 删除远程tag
git push origin :refs/tags/v1.0.0
```

### Q: GitHub Actions在哪里看日志？

1. 访问：`https://github.com/你的用户名/SwallowAndEscape/actions`
2. 点击工作流名称
3. 点击具体的任务查看详细日志

---

## 命令速查表

```bash
# 本地构建
./build-simple-package.sh

# 清理构建文件
rm -rf build dist

# 查看git状态
git status

# 创建tag
git tag v1.0.0
git push origin v1.0.0

# 查看所有tag
git tag -l

# 删除tag
git tag -d v1.0.0                    # 本地
git push origin :refs/tags/v1.0.0    # 远程
```

---

## 文件结构说明

```
SwallowAndEscape/
├── src/                          # 源代码
├── assets/                       # 游戏资源
├── build-simple-package.sh       # 👈 运行这个构建Linux版本
├── CMakeLists.txt                # 构建配置
└── .github/workflows/            # 自动化配置
    └── release.yml
```

---

## 下一步

- 阅读 `RELEASE_GUIDE.md` 了解详细原理
- 修改游戏代码
- 发布新版本

有问题随时查看文档或提问！
