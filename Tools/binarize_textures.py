from pathlib import Path
import ctypes
import zlib
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


crcs = set()
def get_crc32(path):
    crc32_hash = 0
    buffer_size = 65536  # Read in chunks of 64KB

    with open(path, "rb") as t_file:
        while chunk := t_file.read(buffer_size):
            crc32_hash = zlib.crc32(chunk, crc32_hash)
    
    # Ensure the result is an unsigned 32-bit integer
    crc32_hash = crc32_hash & 0xFFFFFFFF
    assert crc32_hash not in crcs
    crcs.add(crc32_hash)
    return crc32_hash


gather_textures(Path(TEXTURES_PATH))

enum_entries = []

next_start: ctypes.c_uint32 = 0
with open(f"{BIN_DATA_PATH}/textures.bin", "wb") as t_bin, \
     open (f"{BIN_DATA_PATH}/texturesDescriptors.bin", "wb") as t_desc:

    for t in textures:
        with open(t.as_posix(), "rb") as t_file:
            data = t_file.read()
            write_size = t_bin.write(data)

            assert write_size == t.lstat().st_size

            crc32_hash = get_crc32(t.as_posix())
            desc = TextureDescriptor(crc32_hash, next_start, write_size)
            t_desc.write(struct.pack("III", desc.texture_id, desc.start, desc.size))

            next_start += write_size

            texture_name = t.as_posix().split(f"{TEXTURES_PATH}/")[-1].replace("/", "_").replace(".png", "")
            enum_entries.append(f"{texture_name} = {crc32_hash}")

            print(f"Processed {t.as_posix()}")

environment = Environment(loader=FileSystemLoader("Tools/templates/"))
template = environment.get_template("EnumTemplate.h")
content = template.render(name=ENUM_NAME, entries=enum_entries)
with open(f"{GENERATED_PATH}/{ENUM_NAME}.h", "w") as f:
    f.write(content)
