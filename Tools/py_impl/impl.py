import os
from jinja2 import Environment, FileSystemLoader


_GAME_PATH = "Source/Game"
_GENERATED_PATH = f"{_GAME_PATH}/Generated"
_OBJ_TYPE_TO_FOLDER = {"system" : "Systems", "component" : "Components"}


def _object_exists(obj_type: str, name: str) -> bool:
    PATH = f"{_GAME_PATH}/{_OBJ_TYPE_TO_FOLDER[obj_type]}"

    objects = os.listdir(PATH)
    found_obj = [obj for obj in objects if name in obj]

    if found_obj:
        print(f"{name} {obj_type} already exists!")
        return True
    
    return False


def generate(obj_type: str, name: str) -> bool:
    if _object_exists(obj_type, name):
        return False
    
    path = f"{_GAME_PATH}/{_OBJ_TYPE_TO_FOLDER[obj_type]}"

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


def update_registry(obj_type: str):
    folder = _OBJ_TYPE_TO_FOLDER[obj_type]
    objects_path = f"{_GAME_PATH}/{folder}"
    registry_path = f"{_GENERATED_PATH}/{folder}Registry.h"
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
