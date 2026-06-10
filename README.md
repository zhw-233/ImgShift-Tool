# ImgShift-Tool

ImgShift-Tool 一个简易图片处理工具。

是zhw同学的C语言期末大作业。

## 计划功能

- 图片格式转换
- 图片压缩
- 批量处理图片
- 命令行操作
- Qt/C++ 桌面界面

## 项目结构

```text
backend/      Python 后端，负责命令行入口和图片处理逻辑
frontend/     Qt/C++ 桌面应用
tests/        自动化测试
assets/       测试图片、图标和其他静态资源
docs/         项目文档和学习笔记
scripts/      开发、构建和打包辅助脚本
```

## 当前状态

项目目前处于初始化阶段，已经创建基础目录结构。下一步会先实现 Python 命令行版本的核心图片处理功能。

## 本地开发

建议在项目根目录创建 Python 虚拟环境：

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

运行当前命令行入口：

```bash
python3 backend/image_cli.py
```
