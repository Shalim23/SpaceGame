import subprocess
import sys

CONFIGURATIONS = "Debug;MinSizeRel;DebugServer;MinSizeRelServer"

subprocess.check_call(["cmake",
    f"-DCMAKE_CONFIGURATION_TYPES={CONFIGURATIONS}", "-S", ".", "-B", "./bin"])

if "--open" in sys.argv:
    subprocess.Popen(["start", "bin/SpaceGame.sln"], shell=True)
