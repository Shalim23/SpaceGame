from pathlib import Path
import ctypes
from dataclasses import dataclass, field
import struct

@dataclass
class TextureDescriptor:
    texture_id: ctypes.c_uint32 = field(default=ctypes.c_uint32)
    start: ctypes.c_uint32 = field(default=ctypes.c_uint32)
    size: ctypes.c_uint32 = field(default=ctypes.c_uint32)


TEXTURES_PATH = "Data/Textures"
TEXTURES_ENUM_PATH = "Source/Game/Generated/TextureType.h"

textures = []

def gather_textures(path: Path):
    for obj in path.iterdir():
        if obj.is_file() and obj.name.endswith(".png"):
            textures.append(obj)
        elif obj.is_dir():
            gather_textures(obj)

gather_textures(Path(TEXTURES_PATH))

next_start: ctypes.c_uint32 = 0
with open(TEXTURES_ENUM_PATH, "w") as t_enum, \
     open("bin/textures.bin", "wb") as t_bin, \
     open ("bin/textures_descriptors.bin", "wb") as t_desc:
    
    t_enum.write("//GENERATED FILE! DO NOT MODIFY DIRECTLY!\n")
    t_enum.write("#pragma once\n\n")
    t_enum.write("enum class TextureType\n")
    t_enum.write("{\n")

    for count, t in enumerate(textures):
        with open(t.as_posix(), "rb") as t_file:
            data = t_file.read()
            write_size = t_bin.write(data)

            assert write_size == t.lstat().st_size

            desc = TextureDescriptor(count, next_start, write_size)
            t_desc.write(struct.pack("iii", desc.texture_id, desc.start, desc.size))

            next_start += write_size

            texture_name = t.as_posix().split(f"{TEXTURES_PATH}/")[-1].replace("/", "_").replace(".png", "")
            t_enum.write(f"\t{texture_name} = {count},\n")

            print(f"Processed {t.as_posix()}")

    t_enum.write("};\n")
    