import os
import argparse
from jinja2 import Environment, FileSystemLoader
from data import GAME_PATH, OBJ_TYPE_TO_FOLDER
from update_registry import update_registry


def _object_exists(obj_type: str, name: str) -> bool:
    PATH = f"{GAME_PATH}/{OBJ_TYPE_TO_FOLDER[obj_type]}"

    objects = os.listdir(PATH)
    found_obj = [obj for obj in objects if name in obj]

    if found_obj:
        print(f"{name} {obj_type} already exists!")
        return True
    
    return False


def _generate(obj_type: str, name: str) -> bool:
    if _object_exists(obj_type, name):
        return False
    
    path = f"{GAME_PATH}/{OBJ_TYPE_TO_FOLDER[obj_type]}"

    environment = Environment(loader=FileSystemLoader("Tools/templates/"))

    match obj_type:
        case "system":
            system_fullname = f"{name.capitalize()}System"

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
            component_fullname = f"{name.capitalize()}Component"

            template_h = environment.get_template("ComponentTemplate.h")
            content_h = template_h.render(name=component_fullname)
            with open(path + f"/{component_fullname}.h", "w") as f:
                f.write(content_h)
            print(f"Created {component_fullname}")

        case _:
            print(f"Unknown type {obj_type}!")
            return False

    return True


def _main():
    parser = argparse.ArgumentParser()
    parser.add_argument("obj_type", type=str,
                        choices=["system", "component"], help="New type")
    parser.add_argument("obj_name", type=str, help="New type name")
    args = parser.parse_args()

    if _generate(args.obj_type, args.obj_name):
        update_registry(args.obj_type)


if __name__ == "__main__":
    _main()
