#!/usr/bin/python3

import os
import re
import sys
import multiprocessing

def swap_ext(name, new_ext):
    return os.path.splitext(name)[0] + new_ext

def expand(text, arg_dict):
  if text is not None:
    while re.search("{[^}]*?}", text) is not None:
      text = eval("f\"" + text + "\"", {}, arg_dict)
  return text

config = {
  "verbose"    : False,
  "clean"      : False,
  "swap_ext"   : swap_ext
}

pool = multiprocessing.Pool(multiprocessing.cpu_count())

################################################################################

def needs_rebuild(file_in, file_out):
  global config
  if type(file_out) is list:
    for f in file_out:
      if needs_rebuild(file_in, f):
        return True
    return False

  if type(file_in) is list:
    for f in file_in:
      if needs_rebuild(f, file_out):
        return True
    return False

  if not os.path.exists(file_in):
    print(f"File {file_in} not found!")
    return False;

  if not os.path.exists(file_out):
    if config['verbose']:
      print(f"Rebuild caused by missing output {file_out}")
    return True;

  if os.path.getmtime(file_in) > os.path.getmtime(file_out):
    if config['verbose']:
      print(f"Rebuild caused by changed input {file_in}")
    return True

  deps_file = swap_ext(file_out, ".d")
  if os.path.exists(deps_file):
    for dep in open(deps_file).read().split():
      if os.path.exists(dep) and os.path.getmtime(dep) > os.path.getmtime(file_out):
        if config['verbose']:
          print(f"Rebuild caused by changed dep {dep}")
        return True

  return False

################################################################################

def clean(files_in, files_out, **kwargs):
  if type(files_out) is list:
    for file in files_out:
      clean(files_in, file)
    return
  if os.path.exists(files_out):
    print(f"Cleaning {files_out}")
    os.system(f"rm {files_out}")

################################################################################

def run_something(arg_dict):
  if "desc" in arg_dict:
    print(expand(arg_dict["desc"], arg_dict))

  command = 
  formatted_command = expand(arg_dict["command"], arg_dict)

  if arg_dict["verbose"]:
    print(formatted_command)

  if os.system(formatted_command):
    print(f"Command failed: \"{formatted_command}\"")
    print("Aborting build")
    sys.exit(result)

  return 0

################################################################################

def run_batch(files_in, files_out, arg_dict):
  arg_dict["files_in"]  = files_in
  arg_dict["files_out"] = files_out
  run_something(arg_dict)

################################################################################

def run_map(file_in, file_out, arg_dict):
  arg_dict["file_in"]   = file_in
  arg_dict["file_out"]  = file_out
  run_something(arg_dict)

################################################################################

def run_parallel(files_in, files_out, arg_dict):
  global pool
  results = []
  for i in range(len(files_in)):
    result = pool.apply_async(run_map, [files_in[i], files_out[i], arg_dict])
    results.append(result)
  for result in results:
    result.get()

################################################################################

def command(**kwargs):
  if config["clean"] is True:
    return clean

  top_kwargs = dict(config)
  top_kwargs.update(kwargs)
  top_kwargs["config"] = config;
  top_kwargs["rule"] = kwargs;

  def action(files_in, files_out, **kwargs):
    local_kwargs = dict(top_kwargs)
    local_kwargs.update(kwargs)

    files_in  = files_in  if type(files_in)  is list else [files_in]
    files_out = files_out if type(files_out) is list else [files_out]

    if not needs_rebuild(files_in, files_out):
      return

    for file_out in files_out:
      dirname = os.path.dirname(file_out)
      if dirname:
        os.makedirs(dirname, exist_ok = True)


    if len(files_in) == len(files_out) and not config["serial"]:
      run_parallel(files_in, files_out, local_kwargs)
    else:
      for i in range(len(files_in)):
        run_command([files_in[i]], [files_out[i]], local_kwargs)

  return action

################################################################################

def batch(**kwargs):
  if config["clean"] is True:
    return clean

  top_kwargs = dict(config)
  top_kwargs.update(kwargs)
  top_kwargs["config"] = config;
  top_kwargs["rule"] = kwargs;

  def action(files_in, files_out, **kwargs):
    local_kwargs = dict(top_kwargs)
    local_kwargs.update(kwargs)

    if not needs_rebuild(files_in, files_out):
      return

    for file_out in files_out:
      dirname = os.path.dirname(file_out)
      if dirname:
        os.makedirs(dirname, exist_ok = True)

    run_command(files_in, files_out, local_kwargs)

  return action


################################################################################
