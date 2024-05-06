import os
import argparse
from jinja2 import Environment, FileSystemLoader
from data import GAME_PATH, GENERATED_PATH, OBJ_TYPE_TO_FOLDER


def update_registry(obj_type: str):
    folder = OBJ_TYPE_TO_FOLDER[obj_type]
    objects_path = f"{GAME_PATH}/{folder}"
    registry_path = f"{GENERATED_PATH}/{folder}Registry.h"
    environment = Environment(loader=FileSystemLoader("Tools/templates/"))
    content = None

    objects = os.listdir(objects_path)
    objects = [obj.replace(".h", "") for obj in objects if obj.endswith(".h")]

    match obj_type:
        case "system":
            template = environment.get_template("SystemsRegistryTemplate.h")
            content = template.render(systems=objects)
        case "component":
            template = environment.get_template("ComponentsRegistryTemplate.h")
            content = template.render(components=objects)
        case _:
            print(f"Unknown type {obj_type}!")
            return
        
    with open(registry_path, "w") as f:
        f.write(content)

    print(f"Updated {registry_path}")


def _main():
    parser = argparse.ArgumentParser()
    parser.add_argument("obj_type", type=str,
                        choices=["system", "component"], help="New type")
    args = parser.parse_args()

    update_registry(args.obj_type)


if __name__ == "__main__":
    _main()
