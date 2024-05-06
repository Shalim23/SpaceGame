import subprocess
import argparse

CONFIGURATIONS = "Debug;MinSizeRel;DebugServer;MinSizeRelServer"


def _main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--open", action="store_true", help="Open solution")
    args = parser.parse_args()

    subprocess.check_call(["cmake",
        f"-DCMAKE_CONFIGURATION_TYPES={CONFIGURATIONS}", "-S", ".", "-B", "./bin"])

    if args.open:
        subprocess.Popen(["start", "bin/SpaceGame.sln"], shell=True)


if __name__ == "__main__":
    _main()
