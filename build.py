#!/usr/bin/python3

import argparse
import glob
import tinybuild

parser = argparse.ArgumentParser()
parser.add_argument('--verbose',  action='store_true', help='Print verbose build info')
parser.add_argument('--clean',    action='store_true', help='Delete intermediate files')
parser.add_argument('--serial',   action='store_true', help='Do not parallelize actions')
options = parser.parse_args()

tinybuild.config["toolchain"] = "x86_64-linux-gnu"
tinybuild.config["verbose"]   = options.verbose
tinybuild.config["clean"]     = options.clean
tinybuild.config["serial"]    = options.serial

################################################################################

compile_cpp = tinybuild.command(
  command   = "{toolchain}-g++ {opts} {includes} {defines} -c {file_in} -o {file_out}",
  desc      = "Compiling C++ {file_in} => {file_out}",
  opts      = "-std=gnu++2a -Wunused-variable -Werror -MD -MF {file_out}.d -g -O0",
  includes  = "-Isymlinks/metrolib -Isrc -I. -Isymlinks",
  defines   = "-DCONFIG_DEBUG"
)

compile_c   = tinybuild.command(
  command   = "{toolchain}-gcc {opts} {includes} {defines} -c {file_in} -o {file_out}",
  desc      = "Compiling C {file_in} => {file_out}",
  opts      = "-Wunused-variable -Werror -MD -MF {file_out}.d -g -O0",
  includes  = "-Isymlinks/metrolib -Isrc -I. -Isymlinks",
  defines   = "-DCONFIG_DEBUG",
)

c_library = tinybuild.command(
  command = "ar rcs {file_out} {' '.join(files_in)}",
  desc    = "Bundling {file_out}",
)

c_binary    = tinybuild.command(
  command   = "{toolchain}-g++ {opts} {' '.join(files_in)} {libraries} -o {file_out}",
  desc      = "Linking {file_out}",
  opts      = "-g",
  libraries = ""
)

################################################################################

imgui_srcs = glob.glob("symlinks/imgui/*.cpp")
imgui_objs = ["obj/" + tinybuild.swap_ext(f, ".o") for f in imgui_srcs]
imgui_lib  = "bin/imgui.a"

compile_cpp(imgui_srcs, imgui_objs)
c_library  (imgui_objs, imgui_lib)

compile_c  ("symlinks/glad/glad.c", "obj/glad.o")

################################################################################

GateBoyLib_srcs = glob.glob("src/GateBoyLib/*.cpp")
GateBoyLib_objs = ["obj/" + tinybuild.swap_ext(f, ".o") for f in GateBoyLib_srcs]
GateBoyLib_lib  = "obj/src/GateBoyLib/GateBoyLib.a"

compile_cpp(GateBoyLib_srcs, GateBoyLib_objs)
c_library  (GateBoyLib_objs, GateBoyLib_lib)

################################################################################

MetroBoyLib_srcs = glob.glob("src/MetroBoyLib/*.cpp")
MetroBoyLib_objs = ["obj/" + tinybuild.swap_ext(f, ".o") for f in MetroBoyLib_srcs]
MetroBoyLib_lib  = "obj/src/MetroBoyLib/MetroBoyLib.a"

compile_cpp(MetroBoyLib_srcs, MetroBoyLib_objs)
c_library  (MetroBoyLib_objs, MetroBoyLib_lib)

################################################################################

compile_cpp("src/GateBoyApp/GateBoyApp.cpp", "obj/src/GateBoyApp/GateBoyApp.o")

GateBoyApp_objs = [
  "obj/src/GateBoyApp/GateBoyApp.o",
  "obj/src/GateBoyLib/GateBoyLib.a",
  "obj/glad.o",
  "symlinks/metrolib/bin/metrolib/libappbase.a",
  "symlinks/metrolib/bin/metrolib/libaudio.a",
  "symlinks/metrolib/bin/metrolib/libcore.a",
  "symlinks/metrolib/bin/metrolib/libgameboy.a",
  "bin/imgui.a",
]

c_binary(
  GateBoyApp_objs,
  "bin/GateBoyApp",
  libraries="-lSDL2 -ldl -lpthread"
)
