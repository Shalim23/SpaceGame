import ctypes
from dataclasses import dataclass, field
import struct
from jinja2 import Environment, FileSystemLoader
from data import BIN_DATA_PATH, GENERATED_PATH


@dataclass
class TextDescriptor:
    text_id: ctypes.c_uint32 = field(default=ctypes.c_uint32)
    text_len: ctypes.c_uint32 = field(default=ctypes.c_uint32)
    text: bytes = field(default=bytes)

ENUM_NAME = "TextType"
TEXT_PATH = "Data/staticText.txt"

lines = []
with open(TEXT_PATH, "r") as f:
    lines = f.readlines()

enum_entries = []

with open (f"{BIN_DATA_PATH}/textDescriptors.bin", "wb") as t_desc:
    for count, line in enumerate(lines):
        line = line.rstrip()
        desc = TextDescriptor(count, len(line), line.encode())
        format_string = "ii" + str(len(desc.text)) + "s"
        t_desc.write(struct.pack(format_string, desc.text_id, desc.text_len, desc.text))
        formatted_line = line.replace(" ", "_")
        formatted_line = "".join(filter(str.isalpha, formatted_line))
        enum_entries.append(f"{formatted_line} = {count}")

environment = Environment(loader=FileSystemLoader("Tools/templates/"))
template = environment.get_template("EnumTemplate.h")
content = template.render(name=ENUM_NAME, entries=enum_entries)
with open(f"{GENERATED_PATH}/{ENUM_NAME}.h", "w") as f:
    f.write(content)

print(f"Text binarized!")
