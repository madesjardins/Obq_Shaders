import re
import sys
import glob
import excons
from excons.tools import maya
from excons.tools import arnold

excons.SetArgument("no-arch", 1)
env = excons.MakeBaseEnv()

Obq_version = "v304"

if sys.platform == "win32":
   env.Append(CPPDEFINES=["_CRT_SECURE_NO_WARNINGS"])
   # Get rid of:
   # - nameless struct/union warning (4201)
   # - unreferenced formal parameter (4100)
   # - local variable is initialized but not referenced (4189)
   # - conditional expression is constant (4127)
   env.Append(CPPFLAGS=" /wd4201 /wd4100 /wd4189 /wd4127")

else:
   env.Append(CCFLAGS=" -fmessage-length=0")
   env.Append(CPPFLAGS=" -Wno-unused-parameter -Wno-unused-variable -Wno-unused")
   
   if re.match("^linux", sys.platform):
      env.Append(LINKFLAGS=" -Wl,--no-undefined")

prjs = [
   {"name": "Obq_Shaders__Core__%s" % Obq_version,
    "alias": "shaders",
    "type": "dynamicmodule",
    "ext": arnold.PluginExt(),
    "incdirs": ["glm", "src", "src/pbrt"],
    "srcs": filter(lambda x: "Simbiont" not in x, glob.glob("src/*.cpp")) +
            glob.glob("src/pbrt/*.cpp"),
    "custom": [arnold.Require]
   }
]

excons.DeclareTargets(env, prjs)
