#include "MtModLibrary.h"

#include "MtModule.h"
#include <sys/stat.h>

//------------------------------------------------------------------------------

void MtModLibrary::reset() {
  for (auto module : modules) delete module;
  modules.clear();
}

void MtModLibrary::add_search_path(const std::string& path) {
  search_paths.push_back(path);
}

void MtModLibrary::load(const std::string& full_path) {
  auto mod = new MtModule();
  mod->load(full_path);
  mod->lib = this;
  modules.push_back(mod);
}

MtModule* MtModLibrary::find_module(const std::string& module_name) {
  for (auto mod : modules) {
    if (mod->mod_name == module_name) return mod;
  }
  return nullptr;
}

bool MtModLibrary::has_mod(const std::string& name) {
  for (auto mod : modules) if (mod->mod_name == name) return true;
  return false;
}

//------------------------------------------------------------------------------
