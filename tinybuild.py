import os
import re
import sys
import multiprocessing

def swap_ext(name, new_ext):
  return os.path.splitext(name)[0] + new_ext

def join(names, divider = ' '):
  return divider.join(names)

def expand(text, arg_dict):
  if text is not None:
    while re.search("{[^}]+}", text) is not None:
      text = eval("f\"" + text + "\"", {}, arg_dict)
  return text

def listify(x):
  return x if type(x) is list else [x]

global_config = {
  "verbose"    : False,
  "clean"      : False,
  "serial"     : False,
  "dry_run"    : False,
  "swap_ext"   : swap_ext,
  "join"       : join
}

################################################################################

def check_deps(files_in, file_out, file_kwargs):
  file_out = expand(file_out, file_kwargs)
  if not os.path.exists(file_out):
    if file_kwargs['verbose']:
      print(f"Rebuilding {file_out} because it's missing")
    return True;

  for file_in in files_in:
    file_in = expand(file_in, file_kwargs)
    if os.path.exists(file_in):
      if os.path.getmtime(file_in) > os.path.getmtime(file_out):
        if file_kwargs['verbose']:
          print(f"Rebuilding {file_out} because it's older than dependency {file_in}")
        return True
    else:
      print(f"Dependency {file_in} missing, aborting build!")
      # Is there a better way to handle this?
      sys.exit(-1)
  return False

################################################################################

def needs_rebuild(files_in, files_out, file_kwargs):
  if file_kwargs.get("force", False):
    return True

  files_in  = listify(files_in)
  files_out = listify(files_out)

  for file_out in files_out:
    file_out = expand(file_out, file_kwargs)

    # Check user-specified deps
    if check_deps(file_kwargs.get("deps", []), file_out, file_kwargs):
      return True

    # Check depfile, if present
    if os.path.exists(file_out + ".d"):
      deps = open(file_out + ".d").read().split()
      deps = [d for d in deps[1:] if d != '\\']
      if check_deps(deps, file_out, file_kwargs):
        return True

    # Check input files
    if check_deps(files_in, file_out, file_kwargs):
      return True

    # All checks passed, don't need to rebuild this output
    if file_kwargs.get("verbose", False):
      print(f"File {file_out} is up to date")

  return False

################################################################################

def run_command(file_kwargs):

  if desc := file_kwargs.get("desc", None):
    print(expand(desc, file_kwargs))

  command = expand(file_kwargs["command"], file_kwargs)
  result = -1

  if file_kwargs.get("dry_run", False):
    print(f"Dry run: \"{command}\"")
  elif result := os.system(command):
    print(f"Command failed: \"{command}\"")
  elif file_kwargs["verbose"]:
    print(f"Command done: \"{command}\"")

  return result

################################################################################

def create_action(do_map, do_reduce, kwargs):
  # Take a snapshot of the global config at the time the rule is defined
  config_kwargs = dict(global_config)

  # Take a snapshot of the config kwargs and patch in our rule kwargs
  rule_kwargs = dict(config_kwargs)
  rule_kwargs.update(kwargs)
  rule_kwargs["config"] = config_kwargs;

  def action(files_in, files_out, **kwargs):
    files_in  = listify(files_in)
    files_out = listify(files_out)

    # Make sure our output directories exist
    for file_out in files_out:
      if dirname := os.path.dirname(file_out):
        os.makedirs(dirname, exist_ok = True)

    if do_map:
      assert len(files_in) == len(files_out)

    # Take a snapshot of the rule kwargs and patch in our action kwargs
    action_kwargs = dict(rule_kwargs)
    action_kwargs.update(kwargs)
    action_kwargs["rule"]      = rule_kwargs
    action_kwargs["files_in"]  = files_in
    action_kwargs["files_out"] = files_out

    ########################################
    # Clean files if requested

    if action_kwargs.get("clean", None):
      for file_out in action_kwargs["files_out"]:
        file_out = expand(file_out, action_kwargs)
        if action_kwargs.get("verbose", False):
          print(f"rm -f {file_out}")
        os.system(f"rm -f {file_out}")
      return

    ########################################
    # Dispatch the action as a map

    if do_map:
      results = []
      for i in range(len(files_in)):
        file_in  = files_in[i]
        file_out = files_out[i]

        file_kwargs = dict(action_kwargs)
        file_kwargs["action"]   = action_kwargs
        file_kwargs["file_in"]  = file_in
        file_kwargs["file_out"] = file_out

        if needs_rebuild(file_in, file_out, file_kwargs):
          if file_kwargs["serial"]:
            run_command(file_kwargs)
          else:
            result = pool.apply_async(run_command, [file_kwargs])
            results.append(result)

      # Block until all tasks done
      sum = 0
      for result in results:
        sum = sum + result.get()
      if sum:
        print("Command failed, aborting build")
        sys.exit(-1)

    ########################################
    # Dispatch the action as a reduce

    if do_reduce:
      file_kwargs = dict(action_kwargs)
      file_kwargs["action"]   = action_kwargs
      file_kwargs["file_in"]  = files_in[0]
      file_kwargs["file_out"] = files_out[0]
      if needs_rebuild(files_in, files_out, file_kwargs):
        run_command(file_kwargs)

  return action

################################################################################

def map(**kwargs):
  return create_action(do_map = True, do_reduce = False, kwargs = kwargs)

def reduce(**kwargs):
  return create_action(do_map = False, do_reduce = True, kwargs = kwargs)

################################################################################

# Warning - this _must_ be done _after_ the rest of the module is initialized
pool = multiprocessing.Pool(multiprocessing.cpu_count())
