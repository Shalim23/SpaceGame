import os
from jinja2 import Environment, FileSystemLoader
from .._data import GAME_PATH, GENERATED_PATH, OBJ_TYPE_TO_FOLDER


def update_registry(obj_type: str):
    folder = OBJ_TYPE_TO_FOLDER[obj_type]
    objects_path = f"{GAME_PATH}/{folder}"
    registry_path = f"{GENERATED_PATH}/{folder}Registry.h"
    blob_path = f"{GENERATED_PATH}/All{folder}.h"
    environment = Environment(loader=FileSystemLoader("Tools/templates/"))

    objects = os.listdir(objects_path)
    objects = [obj.replace(".h", "") for obj in objects if obj.endswith(".h")]

    blob_template = environment.get_template("BlobTemplate.h")
    blob_content = blob_template.render(type=folder, objects=objects)
    with open(blob_path, "w") as f:
        f.write(blob_content)

    match obj_type:
        case "system":
            template = environment.get_template("SystemsRegistryTemplate.h")
            content = template.render(systems=objects)
        
        case "component":
            enum_name = "ComponentType"
            enum_template = environment.get_template("EnumTemplate.h")
            components = [comp.replace("Component", "") for comp in objects]
            enum_content = enum_template.render(name=enum_name, entries=components)
            with open(f"{GENERATED_PATH}/{enum_name}.h", "w") as f:
                f.write(enum_content)
            
            comps_dict = {}
            for i in range(0, len(objects)):
                comps_dict[f"{enum_name}::{components[i]}"] = objects[i]
            template = environment.get_template("ComponentsRegistryTemplate.h")
            content = template.render(components=comps_dict)
            
            info_template = environment.get_template("ComponentsInfoTemplate.h")
            info_content = info_template.render(components=comps_dict)
            with open(f"{GENERATED_PATH}/ComponentsInfo.h", "w") as f:
                f.write(info_content)

        case _:
            print(f"Unknown type {obj_type}!")
            return

    with open(registry_path, "w") as f:
        f.write(content)
    print(f"Updated {registry_path}")
