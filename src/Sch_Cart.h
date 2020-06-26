#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct CartSignals {
};

struct CartRegisters {
  CartSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();
};

//-----------------------------------------------------------------------------

}; // namespace Schematics