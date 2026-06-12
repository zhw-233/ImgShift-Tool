# ImgShift-Tool

ImgShift-Tool 一个简易图片处理工具。

是zhw同学的C语言期末大作业。

## 功能（ 期望 ）

- 图片格式转换
- 图片压缩
- 批量处理图片
- 命令行操作
- Qt/C++ 桌面界面
- 支持更多格式（ .gif / .pdf …… ）

## 当前状态

已实现：

- 图片格式转换
- 图片压缩
- 命令行功能
- Qt/C++ 桌面界面

其余部分正在加紧赶工……

## 项目结构

```text
backend/      Python 后端，负责命令行入口和图片处理逻辑
frontend/     Qt/C++ 桌面应用
```

## 安装依赖

### 运行环境

- Python 3.10 或更高版本
- Qt6 6.2 或更高版本 包含 Widget 模块
- C++20
- CMake 3.16 或更高版本

项目依赖部分 Python 包，安装方式：

```bash
pip install -r requirements.txt
```

## 命令行使用

支持功能：

- `trans`: 转换图片格式
- `zip`: 压缩图片

支持的图片格式：

```text
JPG / JPEG / PNG / WEBP
```

### 查看帮助

macOS / Linux

```bash
python3 backend/image_cli.py -h
python3 backend/image_cli.py trans -h
python3 backend/image_cli.py zip -h
```

Windows

```powershell
python backend\image_cli.py -h
python backend\image_cli.py trans -h
python backend\image_cli.py zip -h
```

### 转换图片格式

基本用法：

```bash
python3 backend/image_cli.py trans 输入图片路径 -f 目标格式
```

示例：将 `example.jpg` 转为 `png`

```bash
python3 backend/image_cli.py trans tests/example.jpg -f png
```

额外参数说明：

- `-o` / `--output`：输出目录，默认输入目录，输出目录必须已经存在
- `-f` / `--format`：目标格式，必填项
- `-q` / `--quality`：JPG / JPEG / WEBP 压缩质量，0-100（100图像质量最高），默认95
- `-no-fast` / `--no-fast`：关闭快速模式（可优化文件空间），默认开启快速模式
- `-progressive` / `--progressive`：是否生成渐进式图片，默认关闭

### 压缩图片

基本用法：

```bash
python3 backend/image_cli.py zip 输入图片路径 
```

额外参数说明：

- `-o` / `--output`：输出目录，默认输入目录，输出目录必须已经存在

压缩 JPG / JPEG：

- `-q` / `--quality`：JPG / JPEG / WEBP 压缩质量，0-100（100图像质量最高），默认75
- `-progressive` / `--progressive`：是否生成渐进式图片，默认关闭

压缩 PNG：

- `-level` / `--level`：PNG 压缩等级，0-9（0为不压缩），默认3

压缩 WEBP：

- `-q` / `--quality`：JPG / JPEG / WEBP 压缩质量，0-100（100图像质量最高），默认75
- `-lossless` / `--lossless`：WEBP 是否启用无损压缩，默认关闭

### 输出文件名规则

JPG 格式文件统一以 JPEG 格式输出

执行 `trans` 部分会生成 `trans_...`

执行 `zip` 部分会生成 `zip_...`

注意：会直接在输出目录下覆盖已存在的同文件名图片

## 桌面端使用

### 项目构建

#### 前端
```bash
cmake -S frontend -B frontend/build
cmake --build frontend/build
```

#### 后端
```bash
pip install pyinstaller
pyinstaller --clean --onefile --name image_cli --paths backend backend/image_cli.py
```

在后端生成可执行程序image_cli后，将其移动至：

`macOS` ： `.app` 的 `ImgShift-Tool.app/Contents/MacOS/image_cli`

`Windows` ： 前端的可执行文件同目录下即可

### 项目运行

点击可执行程序即可

## 打包说明

项目提供 GitHub Actions 工作流，可在手动触发或推送 `v*` 标签时构建 macOS / Windows 桌面端压缩包。

## 提示

命令行指令默认值与桌面界面控件默认值可能不同，请以界面实际显示为准。

由于缺失苹果的 `Developer ID Application` 证书，因此 macOS 需要在 APP 文件夹终端窗口执行：

```bash
xattr -dr com.apple.quarantine ImgShift-Tool.app
```

后才可运行

## 许可证

本项目基于 MIT License 开源

用到的组件 `Pillow` 、 `Qt` 各自遵循其对应的许可证。

