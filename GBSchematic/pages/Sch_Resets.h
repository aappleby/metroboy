#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Resets {

  /*p01.BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07
  /*p01.AFER*/ Reg  RESET_REG;

  /*p01.ALUR*/ bool SYS_RESETn;
  /*p01.XAPO*/ bool VID_RESETn;
};

//-----------------------------------------------------------------------------

};