#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct VramRegisters {
  void tick(TestGB& gb);
  bool commit();
};

//-----------------------------------------------------------------------------

}; // namespace Schematics