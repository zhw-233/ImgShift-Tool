from PIL import Image,ImageSequence
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

def gifMaker(input_file,out_path,name,time=300) :
    out_path=Path(out_path)/f"gifmaker_{Path(name).name}.gif"

    tmp_files=[]
    max_height=0
    max_width=0

    for file in input_file :
        with Image.open(file) as im :
            max_height=max(max_height,im.height)
            max_width=max(max_width,im.width)
            tmp_files.append(im.convert("RGBA").copy())

    frames=[]
    for file in tmp_files :
        tmp=Image.new("RGBA",(max_width,max_height),(0,0,0,0))
        tmp.paste(file,(0,0),file)
        frames.append(tmp)
    
    first=frames[0]
    rest=frames[1:]
    first.save(
        out_path,
        format="GIF",
        save_all=True,
        append_images=rest,
        duration=time,
        loop=0,
        disposal=2,
        optimize=True
    )

    return out_path

def gifSpliter(in_path,name,out_path) :
    in_path=Path(in_path)/name
    out_path=Path(out_path)

    with Image.open(in_path) as im :
        for index,img in enumerate(ImageSequence.Iterator(im),start=1) :
            img_path=out_path/f"gifspliter_{Path(in_path).stem}_{index:03d}.png"
            img.copy().save(
                img_path,
                format="PNG",
                optimize=True
            )

    return out_path
