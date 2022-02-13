#pragma once
#include "Platform.h"

//------------------------------------------------------------------------------

struct MtModLibrary {
  void reset();
  void load(const std::string& input_filename);
  void load(const std::string& input_filename, const std::string& output_filename);
  MtModule* find_module(const std::string & module_name);

  void add_search_path(const std::string& path) {
    search_paths.push_back(path.ends_with("/") ? path : path + "/");
  }

  //----------

  std::vector<MtModule*> modules;
  std::vector<std::string> search_paths;
};

//------------------------------------------------------------------------------
