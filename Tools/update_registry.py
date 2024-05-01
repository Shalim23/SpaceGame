import argparse
from py_impl.impl import update_registry

parser = argparse.ArgumentParser()
parser.add_argument("obj_type", type=str,
                     choices=["system", "component"], help="New type")
args = parser.parse_args()

update_registry(args.obj_type)
