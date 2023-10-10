#!/usr/bin/python3

import glob
import tinybuild

tinybuild.config["toolchain"] = "x86_64-linux-gnu"
tinybuild.config["verbose"]   = False

################################################################################

compile_cpp = tinybuild.command(
  command   = "{toolchain}-g++ {opts} {includes} {defines} -c {file_in} -o {file_out}",
  desc      = "Compiling C++ {file_in} => {file_out}",
  opts      = "-std=gnu++2a -Wunused-variable -Werror -MMD -g -O0",
  includes  = "-Isymlinks/metrolib -Isrc -I. -Isymlinks",
  defines   = "-DCONFIG_DEBUG"
)

compile_c   = tinybuild.command(
  command   = "{toolchain}-gcc {opts} {includes} {defines} -c {file_in} -o {file_out}",
  desc      = "Compiling C {file_in} => {file_out}",
  opts      = "-Wunused-variable -Werror -MMD -g -O0",
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

GateBoyLib_srcs = glob.glob("src/GateBoyLib/*.cpp")
GateBoyLib_objs = ["obj/" + tinybuild.swap_ext(f, ".o") for f in GateBoyLib_srcs]
GateBoyLib_lib  = "obj/src/GateBoyLib/GateBoyLib.a"

compile_cpp(GateBoyLib_srcs, GateBoyLib_objs)
c_library(GateBoyLib_objs, GateBoyLib_lib)

MetroBoyLib_srcs = glob.glob("src/MetroBoyLib/*.cpp")
MetroBoyLib_objs = ["obj/" + tinybuild.swap_ext(f, ".o") for f in MetroBoyLib_srcs]
MetroBoyLib_lib  = "obj/src/MetroBoyLib/MetroBoyLib.a"

compile_cpp(MetroBoyLib_srcs, MetroBoyLib_objs)
c_library(MetroBoyLib_objs, MetroBoyLib_lib)

compile_cpp("src/GateBoyApp/GateBoyApp.cpp", "obj/src/GateBoyApp/GateBoyApp.o")

compile_c("symlinks/glad/glad.c", "obj/glad.o")

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

#c_binary(
#  GateBoyApp_objs,
#  "obj/src/GateBoyApp/GateBoyApp",
#  libraries="-lSDL2 -ldl -lpthread"
#)
