#!/usr/bin/python3
# Experimental use of metrolib/tinybuild.py, beware

import sys
import glob

sys.path.append("symlinks/metrolib")
import tinybuild

################################################################################

tinybuild.global_config["toolchain"]  = "x86_64-linux-gnu"
tinybuild.global_config["build_type"] = "-g -O0"
tinybuild.global_config["warnings"]   = "-Wunused-variable -Werror"
tinybuild.global_config["depfile"]    = "-MMD -MF {file_out}.d"
tinybuild.global_config["defines"]    = "-DCONFIG_DEBUG"
tinybuild.global_config["cpp_std"]    = "-std=gnu++2a"

compile_cpp = tinybuild.map(
  desc      = "Compiling C++ {file_in} => {file_out}",
  command   = "{toolchain}-g++ {opts} {includes} {defines} -c {file_in} -o {file_out}",
  opts      = "{cpp_std} {warnings} {depfile} {build_type}",
  includes  = "-Isymlinks/metrolib -Isrc -I. -Isymlinks",
)

compile_c   = tinybuild.map(
  desc      = "Compiling C {file_in} => {file_out}",
  command   = "{toolchain}-gcc {opts} {includes} {defines} -c {file_in} -o {file_out}",
  opts      = "{warnings} {depfile} {build_type}",
  includes  = "-Isymlinks/metrolib -Isrc -I. -Isymlinks",
)

link_c_lib = tinybuild.reduce(
  desc      = "Bundling {file_out}",
  command   = "ar rcs {file_out} {join(files_in)}",
)

link_c_bin  = tinybuild.reduce(
  desc      = "Linking {file_out}",
  command   = "{toolchain}-g++ {opts} {join(files_in)} {join(deps)} {libraries} -o {file_out}",
  opts      = "{build_type}",
  libraries = ""
)

def obj_name(x):
  return "obj/" + tinybuild.swap_ext(x, ".o")

################################################################################

print("########## Building ImGui")

imgui_srcs = glob.glob("symlinks/imgui/*.cpp")
imgui_objs = [obj_name(f) for f in imgui_srcs]

compile_cpp(imgui_srcs, imgui_objs)
link_c_lib(imgui_objs, "bin/imgui.a")

print("########## Building glad")

compile_c("symlinks/glad/glad.c", "obj/glad.o")

print("########## Building GateBoyLib")

GateBoyLib_srcs = glob.glob("src/GateBoyLib/*.cpp")
GateBoyLib_objs = [obj_name(f) for f in GateBoyLib_srcs]

compile_cpp(GateBoyLib_srcs, GateBoyLib_objs)
link_c_lib(GateBoyLib_objs, "obj/src/GateBoyLib/GateBoyLib.a")

print("########## Building MetroBoyLib")

MetroBoyLib_srcs = glob.glob("src/MetroBoyLib/*.cpp")
MetroBoyLib_objs = [obj_name(f) for f in MetroBoyLib_srcs]

compile_cpp(MetroBoyLib_srcs, MetroBoyLib_objs)

link_c_lib(MetroBoyLib_objs, "obj/src/MetroBoyLib/MetroBoyLib.a")

print("########## Building GateBoyApp")

compile_cpp("src/GateBoyApp/GateBoyApp.cpp", "obj/src/GateBoyApp/GateBoyApp.o")

link_c_bin(
  files_in = [
    "obj/src/GateBoyApp/GateBoyApp.o",
    "obj/glad.o"
  ],
  files_out = "bin/GateBoyApp",
  deps = [
    "obj/src/GateBoyLib/GateBoyLib.a",
    "symlinks/metrolib/bin/metrolib/libappbase.a",
    "symlinks/metrolib/bin/metrolib/libaudio.a",
    "symlinks/metrolib/bin/metrolib/libcore.a",
    "symlinks/metrolib/bin/metrolib/libgameboy.a",
    "bin/imgui.a",
  ],
  libraries="-lSDL2 -ldl -lpthread"
)

################################################################################
