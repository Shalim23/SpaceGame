import os
from jinja2 import Environment, FileSystemLoader
from .._data import GAME_PATH, OBJ_TYPE_TO_FOLDER


def _object_exists(obj_type: str, name: str) -> bool:
    PATH = f"{GAME_PATH}/{OBJ_TYPE_TO_FOLDER[obj_type]}"

    objects = os.listdir(PATH)
    found_obj = [obj for obj in objects if name in obj]

    if found_obj:
        print(f"{name} {obj_type} already exists!")
        return True
    
    return False


def generate(obj_type: str, name: str) -> bool:
    if _object_exists(obj_type, name):
        return False
    
    path = f"{GAME_PATH}/{OBJ_TYPE_TO_FOLDER[obj_type]}"

    environment = Environment(loader=FileSystemLoader("Tools/templates/"))

    match obj_type:
        case "system":
            system_fullname = f"{name}System"

            template_h = environment.get_template("SystemTemplate.h")
            template_cpp = environment.get_template("SystemTemplate.cpp")

            content_h = template_h.render(name=system_fullname)
            content_cpp = template_cpp.render(name=system_fullname)

            with open(path + f"/{system_fullname}.h", "w") as f:
                f.write(content_h)
            with open(path + f"/{system_fullname}.cpp", "w") as f:
                f.write(content_cpp)

            print(f"Created {system_fullname}")

        case "component":
            component_fullname = f"{name}Component"

            template_h = environment.get_template("ComponentTemplate.h")
            content_h = template_h.render(name=component_fullname)
            with open(path + f"/{component_fullname}.h", "w") as f:
                f.write(content_h)
            print(f"Created {component_fullname}")

        case _:
            print(f"Unknown type {obj_type}!")
            return False

    return True
