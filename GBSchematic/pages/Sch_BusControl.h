#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct BusControl {

  /*p07.TEDO*/ bool CPU_RD;
  /*p07.TAPU*/ bool CPU_WR;
  /*p07.ASOT*/ bool CPU_RD2;
  /*p07.CUPA*/ bool CPU_WR2;
  /*p01.ABUZ*/ bool ADDR_VALID_ABxxxxxx;

  /*p08.RORU*/ bool CBUS_TO_CEXTn;
  /*p08.LULA*/ bool CBUS_TO_CEXT;
};

//-----------------------------------------------------------------------------

};