from . import update_registry
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("obj_type", type=str,
                    choices=["system", "component"], help="Type")
args = parser.parse_args()

update_registry(args.obj_type)
