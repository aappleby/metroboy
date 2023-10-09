#!/usr/bin/python3

import os
import glob
from collections.abc import Iterable

def swap_ext(name, new_ext):
    return os.path.splitext(name)[0] + new_ext

def join_space(list):
  return " ".join(list)

################################################################################

def needs_rebuild(file_in, file_out):
  if type(file_in) is list:
    for f in file_in:
      if needs_rebuild(f, file_out):
        return True
    return False

  if type(file_out) is list:
    for f in file_out:
      if needs_rebuild(file_in, f):
        return True
    return False

  if not os.path.exists(file_in):
    print(f"File {file_in} not found!")
    return False;
  if not os.path.exists(file_out):
    return True;

  result = os.path.getmtime(file_in) > os.path.getmtime(file_out)
  #if result is True:
  #  print(f"File {file_in} is newer than {file_out}")
  #else:
  #  print(f"File {file_out} up to date")
  return result

################################################################################

def do_command(command, **kwargs):
  formatted_command = eval(f"f\"{command}\"", globals(), kwargs)
  return os.system(formatted_command)

def gen_command(command, **kwargs):
  def action(srcs):
    files_out = []
    for src in srcs:
      file_in = src
      file_out = "obj/" + swap_ext(file_in, ".o")

      kwargs["file_in"] = file_in
      kwargs["file_out"] = file_out

      os.makedirs(os.path.dirname(file_out), exist_ok = True)
      if needs_rebuild(file_in, file_out):
        print(f"Compiling {file_in} ==> {file_out}")
        do_command(command, **kwargs)
      files_out.append(file_out)
    return files_out
  return action

################################################################################

def c_library(files_in, file_out):
  files_out = [file_out]
  if needs_rebuild(files_in, file_out):
    print(f"Linking {file_out}")
    os.system(f"ar rcs {file_out} {join_space(files_in)}")
  return files_out

################################################################################

compile_cpp = gen_command(
  command   = "{toolchain}-g++ {join_space(opts)} {join_space(includes)} {join_space(defines)} -c {file_in} -o {file_out}",
  toolchain = "x86_64-linux-gnu",
  opts      = ["-std=gnu++2a", "-Wunused-variable", "-Werror", "-MMD", "-g", "-O0"],
  includes  = ["-Isymlinks/metrolib", "-Isrc", "-I.", "-Isymlinks"],
  defines   = ["-DCONFIG_DEBUG"]
)

################################################################################

MetroBoyLib_srcs = glob.glob("src/MetroBoyLib/*.cpp")
MetroBoyLib_objs = compile_cpp(MetroBoyLib_srcs)
MetroBoyLib_lib  = c_library(MetroBoyLib_objs, "obj/src/MetroBoyLib/MetroBoyLib.a")

#def format_thing(command, _globals, _locals):
#  return eval(f"f\"{command}\"", _globals, _locals)
#
#foo = ["foo", "bar", "baz"]
#print(format_thing("Hello {join_space(foo)}", globals(), locals()))