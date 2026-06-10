from PIL import Image
from pathlib import Path

supported_formats={"JPG","JPEG","PNG","WEBP"}

def trans(in_path,name,out_path,target_format,target_quality=75,fast_mode=True,whether_progressive=False) :
    in_path=Path(in_path)/name
    out_path=Path(out_path)/str("trans_"+Path(name).with_suffix('.'+target_format.lower()).name)
    target_format=target_format.upper()
    if target_format=="JPG" :
        target_format="JPEG"
    with Image.open(in_path) as im :
        if target_format=="JPEG" and im.mode in {"RGBA","LA","P"} :
            im=im.convert("RGB")
        im.save(
            out_path,
            format=target_format,
            quality=target_quality,
            optimize=(not fast_mode),
            progressive=whether_progressive
        )
    return out_path

def zip_jpeg(in_path,name,out_path,target_quality,whether_progressive) :
    in_path=Path(in_path)/name
    out_path=Path(out_path)/str("zip_"+name)
    with Image.open(in_path) as im :
        im.save(
            out_path,
            format="JPEG",
            quality=target_quality,
            progressive=whether_progressive
        )
    return out_path

def zip_png(in_path,name,out_path,level) :
    in_path=Path(in_path)/name
    out_path=Path(out_path)/str("zip_"+name)
    with Image.open(in_path) as im :
        im.save(
            out_path,
            format="PNG",
            compress_level=level
        )
    return out_path

def zip_webp(in_path,name,out_path,whether_lossless,target_quality) :
    in_path=Path(in_path)/name
    out_path=Path(out_path)/str("zip_"+name)
    with Image.open(in_path) as im :
        im.save(
            out_path,
            format="WEBP",
            lossless=whether_lossless,
            quality=target_quality
        )
    return out_path
