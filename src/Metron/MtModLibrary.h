#pragma once
#include "Platform.h"

//------------------------------------------------------------------------------

struct MtModLibrary {
  void reset();
  void load(const std::string& input_filename);
  MtModule* find_module(const std::string & module_name);
  bool has_mod(const std::string& name);
  void add_search_path(const std::string& path);

  //----------

  std::vector<MtModule*> modules;
  std::vector<std::string> search_paths;
};

//------------------------------------------------------------------------------
