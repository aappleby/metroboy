#pragma once
#include "Platform.h"

//------------------------------------------------------------------------------

struct MtModLibrary {
  void reset();
  void load(const std::string& input_filename, const std::string& output_filename);
  MtModule* find_module(const std::string & module_name);

  //----------

  std::vector<MtModule*> modules;
};

//------------------------------------------------------------------------------
