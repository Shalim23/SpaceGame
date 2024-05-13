from pathlib import Path
import ctypes
from dataclasses import dataclass, field
import struct
from jinja2 import Environment, FileSystemLoader
from data import BIN_DATA_PATH, GENERATED_PATH


@dataclass
class TextureDescriptor:
    texture_id: ctypes.c_uint32 = field(default=ctypes.c_uint32)
    start: ctypes.c_uint32 = field(default=ctypes.c_uint32)
    size: ctypes.c_uint32 = field(default=ctypes.c_uint32)

ENUM_NAME = "TextureType"
TEXTURES_PATH = "Data/Textures"

textures = []

def gather_textures(path: Path):
    for obj in path.iterdir():
        if obj.is_file() and obj.name.endswith(".png"):
            textures.append(obj)
        elif obj.is_dir():
            gather_textures(obj)

gather_textures(Path(TEXTURES_PATH))

enum_entries = []

next_start: ctypes.c_uint32 = 0
with open(f"{BIN_DATA_PATH}/textures.bin", "wb") as t_bin, \
     open (f"{BIN_DATA_PATH}/texturesDescriptors.bin", "wb") as t_desc:

    for count, t in enumerate(textures):
        with open(t.as_posix(), "rb") as t_file:
            data = t_file.read()
            write_size = t_bin.write(data)

            assert write_size == t.lstat().st_size

            desc = TextureDescriptor(count, next_start, write_size)
            t_desc.write(struct.pack("iii", desc.texture_id, desc.start, desc.size))

            next_start += write_size

            texture_name = t.as_posix().split(f"{TEXTURES_PATH}/")[-1].replace("/", "_").replace(".png", "")
            enum_entries.append(f"\t{texture_name} = {count}")

            print(f"Processed {t.as_posix()}")

environment = Environment(loader=FileSystemLoader("Tools/templates/"))
template = environment.get_template("EnumTemplate.h")
content = template.render(name=ENUM_NAME, entries=enum_entries)
with open(f"{GENERATED_PATH}/{ENUM_NAME}.h", "w") as f:
    f.write(content)
