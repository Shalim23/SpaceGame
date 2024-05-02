from pathlib import Path
import ctypes

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

            typed_count: ctypes.c_uint32 = count
            next_start += write_size
            typed_size: ctypes.c_uint32 = write_size

            t_desc.write(typed_count.to_bytes(8, "little"))
            t_desc.write(next_start.to_bytes(8, "little"))
            t_desc.write(typed_size.to_bytes(8, "little"))

            texture_name = t.as_posix().split(f"{TEXTURES_PATH}/")[-1].replace("/", "_").replace(".png", "")
            t_enum.write(f"\t{texture_name} = {count},\n")

            print(f"Processed {t.as_posix()}")

    t_enum.write("};\n")
    