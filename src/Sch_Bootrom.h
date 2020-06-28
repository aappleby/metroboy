#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct Bootrom {
  void tick(TestGB& gb);
  bool commit();
  /*p07.TEPU*/ Reg BOOT_BITn; // Starts 0, set to 1 by bootrom which blocks reading 0x0000-0x00FF.
};

//-----------------------------------------------------------------------------

}; // namespace Schematics