#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Resets {

  /*p01.BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07
  /*p01.AFER*/ Reg  RESET_REG;

  /*p01.ALUR*/ bool SYS_RESETn;
  /*p01.DULA*/ bool SYS_RESET;
  /*p01.CUNU*/ bool CUNU_RESETn;


  /*p01.XAPO*/ bool VID_RESETn;

  /*p01.WESY*/ bool WESY_RESET;

  /*p01.ROSY*/ bool VID_RESET5;
  /*p01.ATAR*/ bool VID_RESET6;
  /*p01.ABEZ*/ bool VID_RESETn3;
  /*p01.PYRY*/ bool VID_RESET4;
  /*p01.TOFU*/ bool VID_RESET3;

  /*p01.XORE*/ bool XORE_RESET;
  /*p01.XEBE*/ bool XEBE_RESET;
  /*p01.XODO*/ bool XODO_RESET;

  /*p01.WALU*/ bool WALU_RESET;
  /*p01.XARE*/ bool XARE_RESET;
  /*p01.SOTO*/ bool SOTO_RESET;
};

//-----------------------------------------------------------------------------

};