#pragma once
#include "Platform.h"
#include <string>
#include <vector>

struct MtModule;
struct MtSourceFile;

//------------------------------------------------------------------------------

struct MtModLibrary {
  void reset();
  MtModule* find_mod(const std::string & module_name);
  bool has_mod(const std::string& name);
  void add_search_path(const std::string& path);

  //----------

  std::vector<std::string>   search_paths;
  std::vector<MtSourceFile*> source_files;
  std::vector<MtModule*>     modules;
};

//------------------------------------------------------------------------------
