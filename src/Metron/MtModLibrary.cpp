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

MtModule* MtModLibrary::find_mod(const std::string& name) {
  for (auto mod : modules) {
    if (mod->mod_name == name) return mod;
  }
  return nullptr;
}

bool MtModLibrary::has_mod(const std::string& name) {
  return find_mod(name) != nullptr;
}

//------------------------------------------------------------------------------
