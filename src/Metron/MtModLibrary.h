#pragma once
#include <vector>
#include "MtModule.h"

//------------------------------------------------------------------------------

struct MtModLibrary {

  std::vector<MtModule*> modules;

  void reset();
  void load(const std::string& input_filename, const std::string& output_filename);
  MtModule* find_module(const std::string & module_name);
};

//------------------------------------------------------------------------------
