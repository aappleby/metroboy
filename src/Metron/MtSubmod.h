#pragma once
#include "MtNode.h"
#include "Platform.h"

struct MtModule;

//------------------------------------------------------------------------------

struct MtSubmod : public MtNode {
  MtSubmod(const MtNode& n) : MtNode(n) {}

  std::string name;
  MtModule* mod;
};

//------------------------------------------------------------------------------
