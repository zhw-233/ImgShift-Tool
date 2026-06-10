from image_core import trans,supported_formats,zip_jpeg,zip_png,zip_webp
from pathlib import Path
import argparse

def build_parser() :
    parser=argparse.ArgumentParser(
        description="ImgShift-Tool 命令行图片处理工具",
        add_help=False
    )
    parser.add_argument(
        "-h",
        "--help",
        action="help",
        help="帮助"
    )
    subparsers=parser.add_subparsers(dest="command")
    trans_parser=subparsers.add_parser(
        "trans",
        help="转换图片格式",
        description="转换图片格式，可设置输出目录，图片质量等参数",
        add_help=False
    )
    trans_parser.add_argument(
        "-h",
        "--help",
        action="help",
        help="帮助"
    )
    trans_parser.add_argument(
        "-no-fast",
        "--no-fast",
        action="store_false",
        dest="fast",
        help="关闭快速模式（可优化文件空间），默认开启快速模式"
    )
    trans_parser.add_argument(
        "-progressive",
        "--progressive",
        action="store_true",
        help="是否生成渐进式图片，默认关闭"
    )
    trans_parser.add_argument(
        "input",
        help="输入图片路径"
    )
    trans_parser.add_argument(
        "-o",
        "--output",
        default=None,
        help="图片输出目录，默认输入目录"
    )
    trans_parser.add_argument(
        "-f",
        "--format",
        required=True,
        help="目标格式，支持： "+" , ".join(supported_formats)
    )
    trans_parser.add_argument(
        "-q",
        "--quality",
        type=int,
        default=75,
        help="图片质量( JPG / JPEG )有损压缩，0-100，默认75"
    )
    zip_parser=subparsers.add_parser(
        "zip",
        help="压缩图片",
        description="压缩图片，可以设置压缩程度等相关信息，多余参数会自动忽略",
        add_help=False
    )
    zip_parser.add_argument(
        "-h",
        "--help",
        action="help",
        help="帮助"
    )
    zip_parser.add_argument(
        "-progressive",
        "--progressive",
        action="store_true",
        help="JPG / JPEG 是否生成渐进式图片，默认关闭"
    )
    zip_parser.add_argument(
        "-lossless",
        "--lossless",
        action="store_true",
        help="WEBP 是否启用无损压缩，默认关闭"
    )
    zip_parser.add_argument(
        "input",
        help="输入图片路径"
    )
    zip_parser.add_argument(
        "-o",
        "--output",
        default=None,
        help="图片输出目录，默认输入目录"
    )
    zip_parser.add_argument(
        "-q",
        "--quality",
        type=int,
        default=75,
        help="JPG / JPEG / WEBP 压缩质量，0-100（100质量最高），默认75"
    )
    zip_parser.add_argument(
        "-level",
        "--level",
        type=int,
        default=3,
        help="PNG 压缩等级，0-9（0为不压缩），默认3"
    )
    return parser

def run_trans(args) :
    in_path=Path(args.input)
    out_path=Path(args.output) if args.output else in_path.parent
    target_format=args.format.upper()
    quality=args.quality

    if not in_path.is_file() :
        print(f"错误：输入文件不存在：{in_path}")
        return 1
    if not out_path.is_dir() :
        print(f"错误：输出路径不存在：{out_path}")
        return 1
    if target_format not in supported_formats :
        print(f"错误：{target_format} 格式暂不支持")
        return 1
    if not 0<=quality<=100 :
        print("错误：图片质量值超出限制")
        return 1
    result=trans(
        in_path.parent,
        in_path.name,
        out_path,
        target_format,
        quality,
        args.fast,
        args.progressive
    )
    print(f"成功，图片已输出至{result}")
    return 0

def run_zip(args) :
    in_path=Path(args.input)
    out_path=Path(args.output) if args.output else in_path.parent
    if not in_path.is_file() :
        print(f"错误：输入文件不存在：{in_path}")
        return 1
    if not out_path.is_dir() :
        print(f"错误：输出路径不存在：{out_path}")
        return 1
    format=Path(in_path).suffix[1:].upper()
    if format not in supported_formats :
        print(f"错误：{format} 格式暂不支持")
        return 1
    if format in {"JPG","JPEG"} :
        if not 0<=args.quality<=100 :
            print("错误：图片质量值超出限制")
            return 1
        result=zip_jpeg(
            in_path.parent,
            in_path.name,
            out_path,
            args.quality,
            args.progressive
        )
    elif format=="PNG" :
        if not 0<=args.level<=9 :
            print("错误：图片压缩值超出限制")
            return 1
        result=zip_png(
            in_path.parent,
            in_path.name,
            out_path,
            args.level
        )
    elif format=="WEBP" :
        if not 0<=args.quality<=100 :
            print("错误：图片质量值超出限制")
            return 1
        result=zip_webp(
            in_path.parent,
            in_path.name,
            out_path,
            args.lossless,
            args.quality
        )
    print(f"成功，图片已输出至{result}")
    return 0

def main() :
    parser=build_parser()
    args=parser.parse_args()
    if args.command=="trans" :
        return run_trans(args)
    if args.command=="zip" :
        return run_zip(args)
    parser.print_help()
    return 0

if __name__=="__main__" :
    raise SystemExit(main())
