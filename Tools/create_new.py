import argparse
from py_impl.impl import generate, update_registry

parser = argparse.ArgumentParser()
parser.add_argument("obj_type", type=str,
                     choices=["system", "component"], help="New type")
parser.add_argument("obj_name", type=str, help="New type name")
args = parser.parse_args()

if generate(args.obj_type, args.obj_name):
    update_registry(args.obj_type)
