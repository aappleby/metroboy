#!/usr/bin/python3

import os

def swap_ext(name, new_ext):
    return os.path.splitext(name)[0] + new_ext

config = {
  "verbose"    : False,
  "swap_ext"   : swap_ext
}

################################################################################

def needs_rebuild(file_in, file_out):
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
    print(f"Rebuild caused by missing {file_out}")
    return True;

  if os.path.getmtime(file_in) > os.path.getmtime(file_out):
    print(f"Rebuild caused by changed {file_in}")
    return True

  deps_file = swap_ext(file_out, ".d")
  if os.path.exists(deps_file):
    for dep in open(deps_file).read().split():
      if os.path.exists(dep) and os.path.getmtime(dep) > os.path.getmtime(file_out):
        print(f"Rebuild caused by changed {dep}")
        return True

  return False

################################################################################

def run_command(files_in, files_out, arg_dict):
  arg_dict["file_in"]   = files_in[0]
  arg_dict["files_in"]  = files_in
  arg_dict["file_out"]  = files_out[0]
  arg_dict["files_out"] = files_out

  if not needs_rebuild(files_in, files_out):
    return

  for file_out in files_out:
    os.makedirs(os.path.dirname(file_out), exist_ok = True)

  if "desc" in arg_dict:
    print(eval("f\"" + arg_dict["desc"] + "\"", {}, arg_dict))

  formatted_command = eval("f\"" + arg_dict["command"] + "\"", {}, arg_dict)

  if arg_dict["verbose"]:
    print(formatted_command)

  if os.system(formatted_command):
    print(f"Failed command: \"{formatted_command}\"")

################################################################################

def command(**kwargs):
  top_kwargs = dict(config)
  top_kwargs.update(kwargs)
  top_kwargs["config"] = config;
  top_kwargs["rule"] = kwargs;

  def action(files_in, files_out, **kwargs):
    local_kwargs = dict(top_kwargs)
    local_kwargs.update(kwargs)

    files_in  = files_in  if type(files_in)  is list else [files_in]
    files_out = files_out if type(files_out) is list else [files_out]

    if len(files_in) == len(files_out):
      for i in range(len(files_in)):
        run_command([files_in[i]], [files_out[i]], local_kwargs)
    else:
      run_command(files_in, files_out, local_kwargs)

  return action

################################################################################
