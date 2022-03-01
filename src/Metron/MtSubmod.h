#pragma once
#include "Platform.h"

#include "MtNode.h"

struct MtModule;

//------------------------------------------------------------------------------

struct MtSubmod : public MtNode {
  std::string name;
  MtModule* mod;
};

//------------------------------------------------------------------------------
