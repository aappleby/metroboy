#pragma once
#include "MtNode.h"
#include "Platform.h"

struct MtModule;

//------------------------------------------------------------------------------

struct MtSubmod : public MtNode {
  std::string name;
  MtModule* mod;
};

//------------------------------------------------------------------------------
