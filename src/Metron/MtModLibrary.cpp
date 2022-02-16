#include "MtModLibrary.h"

#include "MtModule.h"

#include <sys/stat.h>

//------------------------------------------------------------------------------

void MtModLibrary::reset() {
  for (auto module : modules) delete module;
  modules.clear();
}

void MtModLibrary::load(const std::string& input_filename) {
  load(input_filename, input_filename + ".sv");
}

void MtModLibrary::load(const std::string& input_filename, const std::string& output_filename) {
  printf("loading %s\n", input_filename.c_str());
  for (auto& prefix : search_paths) {
    auto input_path  = prefix + input_filename;
    auto output_path = prefix + output_filename;

    struct stat s;
    if (stat(input_path.c_str(), &s) == 0) {
      auto mod = new MtModule();
      mod->load(input_path, output_path);
      mod->lib = this;
      modules.push_back(mod);
      printf("loading %s done\n", input_filename.c_str());
      return;
    }
  }
  printf("loading %s failed\n", input_filename.c_str());
}

MtModule* MtModLibrary::find_module(const std::string& module_name) {
  for (auto mod : modules) {
    if (mod->mod_name == module_name) return mod;
  }
  return nullptr;
}

//------------------------------------------------------------------------------
