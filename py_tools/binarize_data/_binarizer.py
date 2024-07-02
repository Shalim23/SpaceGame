import struct
import ctypes
import hashlib
from dataclasses import dataclass, field
from jinja2 import Environment, FileSystemLoader
from . import DataTypeBase
from ._data_types import FontDataType, TextDataType, TextureDataType
from .._data import GENERATED_PATH, BIN_DATA_PATH

class Binarizer():

    _ENUM_NAME = "DataType"

    @dataclass
    class _DataDescriptor:
        data_id: ctypes.c_uint32 = field(default=ctypes.c_uint32)
        start: ctypes.c_uint32 = field(default=ctypes.c_uint32)
        size: ctypes.c_uint32 = field(default=ctypes.c_uint32)

    def __init__(self) -> None:
        self._data_types = [FontDataType, TextDataType, TextureDataType]

    #uint -> data types amount
    #struct * data types amount -> data types info
    #rest of the data -> can be reached using data types info
    def binarize(self):
        data_to_write: list[tuple[str, bytes]] = []
        enum_entries = []
        for t in self._data_types:
            data_type: DataTypeBase = t()
            data_to_write.append((data_type.get_data_type_name(), data_type.binarize()))

        with open(f"{BIN_DATA_PATH}/data.bin", "wb") as data_file:

            data_types_count = len(self._data_types) & 0xFFFFFFFF
            data_file.write(struct.pack("I", data_types_count))
            next_start: ctypes.c_uint32 = 0

            for name, data in data_to_write:
                hash_obj = hashlib.sha256(name.encode())
                name_hash = int.from_bytes(hash_obj.digest())
                name_hash = name_hash & 0xFFFFFFFF
                data_size = len(data)
                enum_entries.append(f"{name} = {name_hash}")
                data_desc = self._DataDescriptor(name_hash, next_start, data_size)
                data_file.write(struct.pack("III", data_desc.data_id, data_desc.start, data_desc.size))
                next_start += data_size

            for _, data in data_to_write:
                data_file.write(data)

        environment = Environment(loader=FileSystemLoader("Tools/templates/"))
        template = environment.get_template("EnumTemplate.h")
        content = template.render(name=self._ENUM_NAME, entries=enum_entries)
        with open(f"{GENERATED_PATH}/{self._ENUM_NAME}.h", "w") as f:
            f.write(content)
