import ctypes
import struct
import tempfile
import zlib
from pathlib import Path
from dataclasses import dataclass, field
from jinja2 import Environment, FileSystemLoader
from .._data import GENERATED_PATH

from . import DataTypeBase


DATA_PATH = "Data"


class FontDataType(DataTypeBase):

    """
    Workaround:
    Font data appears to be corrupted when converted from .ttf to binary by simple reading from .ttf file.
    Current font data is already converted to .bin by https://convertio.co/ttf-bin/.
    """
    def binarize(self) -> bytes:
        with open(f"{DATA_PATH}/Fonts/DinaRemaster.bin", "rb") as file:
            return file.read()

    def get_data_type_name(self) -> str:
        return "Font"
    

class TextDataType(DataTypeBase):

    _ENUM_NAME = "TextType"

    @dataclass
    class _TextDescriptor:
        text_id: ctypes.c_uint32 = field(default=ctypes.c_uint32)
        text_len: ctypes.c_uint32 = field(default=ctypes.c_uint32)
        text: bytes = field(default=bytes)

    def _read_text(self) -> list[str]:
        with open(f"{DATA_PATH}/text.txt", "r") as f:
            return f.readlines()

    def binarize(self) -> bytes:
        data = bytearray()
        lines = self._read_text()
        enum_entries = []

        text_count = len(lines)
        text_count = text_count & 0xFFFFFFFF
        data.extend(struct.pack("i", text_count))
        for count, line in enumerate(lines):
            line = line.rstrip()
            desc = self._TextDescriptor(count, len(line), line.encode())
            format_string = "ii" + str(len(desc.text)) + "s"
            
            data.extend(struct.pack(format_string, desc.text_id, desc.text_len, desc.text))
            formatted_line = line.replace(" ", "_")
            formatted_line = "".join(filter(str.isalpha, formatted_line))
            enum_entries.append(f"{formatted_line} = {count}")

        environment = Environment(loader=FileSystemLoader("Tools/templates/"))
        template = environment.get_template("EnumTemplate.h")
        content = template.render(name=self._ENUM_NAME, entries=enum_entries)
        with open(f"{GENERATED_PATH}/{self._ENUM_NAME}.h", "w") as f:
            f.write(content)

        return data
    
    def get_data_type_name(self) -> str:
        return "Text"


class TextureDataType(DataTypeBase):

    _TEXTURES_PATH = f"{DATA_PATH}/Textures"
    _ENUM_NAME = "TextureType"
    
    @dataclass
    class _TextureDescriptor:
        texture_id: ctypes.c_uint32 = field(default=ctypes.c_uint32)
        start: ctypes.c_uint32 = field(default=ctypes.c_uint32)
        size: ctypes.c_uint32 = field(default=ctypes.c_uint32)

    def __init__(self) -> None:
        self._crcs = set()
        self._textures = []

    def _get_crc32(self, path):
        crc32_hash = 0
        buffer_size = 65536  # Read in chunks of 64KB

        with open(path, "rb") as t_file:
            while chunk := t_file.read(buffer_size):
                crc32_hash = zlib.crc32(chunk, crc32_hash)
        
        # Ensure the result is an unsigned 32-bit integer
        crc32_hash = crc32_hash & 0xFFFFFFFF
        assert crc32_hash not in self._crcs
        self._crcs.add(crc32_hash)
        return crc32_hash
    
    def _gather_textures(self, path: Path):
        for obj in path.iterdir():
            if obj.is_file() and obj.name.endswith(".png"):
                self._textures.append(obj)
            elif obj.is_dir():
                self._gather_textures(obj)

    
    #uint -> amount
    #struct * amount -> descriptors
    #rest of the data -> textures
    def binarize(self) -> bytes:
        self._gather_textures(Path(self._TEXTURES_PATH))
        data = None
        enum_entries = []

        next_start: ctypes.c_uint32 = 0
        with tempfile.NamedTemporaryFile(mode="rb+") as desc_file:
            with tempfile.NamedTemporaryFile(mode="rb+") as textures_file:

                for t in self._textures:
                    with open(t.as_posix(), "rb") as t_file:
                        data = t_file.read()
                        write_size = textures_file.write(data)

                        assert write_size == t.lstat().st_size

                        crc32_hash = self._get_crc32(t.as_posix())
                        desc = self._TextureDescriptor(crc32_hash, next_start, write_size)
                        desc_file.write(struct.pack("III", desc.texture_id, desc.start, desc.size))

                        next_start += write_size

                        texture_name = t.as_posix().split(f"{self._TEXTURES_PATH}/")[-1].replace("/", "_").replace(".png", "")
                        enum_entries.append(f"{texture_name} = {crc32_hash}")

                        print(f"Processed {t.as_posix()}")
                
                desc_file.flush()
                textures_file.flush()
                desc_file.seek(0)
                textures_file.seek(0)

                with tempfile.NamedTemporaryFile(mode="rb+") as all_data_file:
                    all_data_file.write(struct.pack("I", len(enum_entries)))
                    all_data_file.write(desc_file.read())
                    all_data_file.write(textures_file.read())

                    all_data_file.flush()
                    all_data_file.seek(0)
                    data = all_data_file.read()

        environment = Environment(loader=FileSystemLoader("Tools/templates/"))
        template = environment.get_template("EnumTemplate.h")
        content = template.render(name=self._ENUM_NAME, entries=enum_entries)
        with open(f"{GENERATED_PATH}/{self._ENUM_NAME}.h", "w") as f:
            f.write(content)

        assert data
        return data

    def get_data_type_name(self) -> str:
        return "Texture"
