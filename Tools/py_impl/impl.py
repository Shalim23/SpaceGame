import os


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


def _get_component_generation_contents(name: str) -> list[str]:
    return ["#pragma once\n\n", f"struct {name}\n", "{};\n"]


def _get_system_generation_contents(name: str) -> list[str]:
    return ["#pragma once\n\n",
            f"class {name}",
            "{",
            "};\n"]


def _get_registry_main_headers() -> list[str]:
    return ["//GENERATED FILE! DO NOT MODIFY DIRECTLY!\n",
            "#pragma once\n",
            "#include \"../Types/TypesList.h\"\n"]


def generate(obj_type: str, name: str) -> bool:
    if _object_exists(obj_type, name):
        return False
    
    path = f"{_GAME_PATH}/{_OBJ_TYPE_TO_FOLDER[obj_type]}"
    contents = []

    match obj_type:
        case "system":
            system_fullname = f"{name.capitalize()}System"
            contents = _get_system_generation_contents(system_fullname)
            path += f"/{system_fullname}.h"
        case "component":
            component_fullname = f"{name.capitalize()}Component"
            contents = _get_component_generation_contents(component_fullname)
            path += f"/{component_fullname}.h"
        case _:
            print(f"Unknown type {obj_type}!")
            return False

    with open(path, "w") as f:
        f.writelines(contents)

    print(f"Created {path}")
    return True


def update_registry(obj_type: str):
    main_headers = _get_registry_main_headers()
    folder = _OBJ_TYPE_TO_FOLDER[obj_type]
    objects_path = f"{_GAME_PATH}/{folder}"
    registry_path = f"{_GENERATED_PATH}/{folder}Registry.h"
    obj_type_header = ""

    match obj_type:
        case "system":
            obj_type_header = "#include \"../Types/System.h\"\n"
        case "component":
            obj_type_header = "#include \"../Types/Component.h\"\n"
        case _:
            print(f"Unknown type {obj_type}!")
            return False
        
    objects = os.listdir(objects_path)

    with open(registry_path, "w") as f:
        f.writelines(main_headers)
        f.write(obj_type_header)

        for obj in objects:
            f.write(f"#include \"../{folder}/{obj}\"\n")

        f.write("\n\n")
        f.write(f"using Registered{folder} = TypesList\n<\n")

        objs = [obj.split(".")[0] + "," for obj in objects]
        if objs:
            last_obj = objs.pop()
            objs.append(last_obj.rstrip(",") + "\n")
            f.writelines(objs)

        f.write(">;\n\n")
        f.write(f"using {folder} = {folder}List<Registered{folder}>::type;\n")

        print(f"Updated {registry_path}")
