import os
from pathlib import Path
from jinja2 import Environment, FileSystemLoader
from .._data import GAME_PATH, GENERATED_PATH, OBJ_TYPE_TO_FOLDER


def _find_system_from_path(path: str, obj: str) -> tuple[str, str]:
    obj_path = Path(path) / obj

    if obj_path.is_dir():
        objects = os.listdir(obj_path.as_posix())
        if not objects:
            return None
        
        for child_obj in objects:
            if result := _find_system_from_path(path, f"{obj}/{child_obj}"):
                return result
    
    if obj.endswith("System.h"):
        return (obj.replace(".h", ""), obj.split("/")[-1].replace(".h", ""))
    
    return None

def _build_systems_data(path: str, objects: list[str]) -> tuple[list[str], list[str]]:
    include_paths = []
    systems = []

    for obj in objects:
        if result := _find_system_from_path(path, obj):
            include_path, system = result
            include_paths.append(include_path)
            systems.append(system)

    return (include_paths, systems)


def update_registry(obj_type: str):
    folder = OBJ_TYPE_TO_FOLDER[obj_type]
    objects_path = f"{GAME_PATH}/ECS/{folder}"
    registry_path = f"{GENERATED_PATH}/{folder}Registry.h"
    blob_path = f"{GENERATED_PATH}/All{folder}.h"

    environment = Environment(loader=FileSystemLoader("Tools/templates/"))
    blob_template = environment.get_template("BlobTemplate.h")

    objects = os.listdir(objects_path)

    match obj_type:
        case "system":
            include_paths, systems = _build_systems_data(objects_path, objects)
            blob_content = blob_template.render(type=folder, objects=include_paths)
            with open(blob_path, "w") as f:
                f.write(blob_content)

            template = environment.get_template("SystemsRegistryTemplate.h")
            content = template.render(systems=systems)
        
        case "component":
            objects = [obj.replace(".h", "") for obj in objects if obj.endswith(".h")]

            blob_content = blob_template.render(type=folder, objects=objects)
            with open(blob_path, "w") as f:
                f.write(blob_content)

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
