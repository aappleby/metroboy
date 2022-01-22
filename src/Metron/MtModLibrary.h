#pragma once
#include <vector>
#include "MtModule.h"

//------------------------------------------------------------------------------

struct ModLibrary {

  std::vector<Module*> modules;

  void reset();
  void load(const std::string& input_filename, const std::string& output_filename);
  Module* find_module(const std::string & module_name);
};

//------------------------------------------------------------------------------
