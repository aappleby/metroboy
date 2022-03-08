#pragma once
#include "MtNode.h"
#include "Platform.h"

struct MtSubmod;
struct MtMethod;

//------------------------------------------------------------------------------

struct MtCall : public MtNode {
  MtCall(const MtNode& n) : MtNode(n) { assert(sym == sym_call_expression); }

  MtSubmod* submod = nullptr;
  MtMethod* method = nullptr;
  std::vector<std::string> args;
};

//------------------------------------------------------------------------------
