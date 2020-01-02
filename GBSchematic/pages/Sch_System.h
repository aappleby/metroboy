#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// Fake registers used for puppeteering the sim while we get things working.

struct SystemRegisters {
  void pwron();
  void reset();
  void next_phase();
  int phase() const;

  //----------------------------------------
  // master clock

  int phase_count;
  /*p01.ATAL*/ bool ATAL_AxCxExGx;
  /*p01.AZOF*/ bool AZOF_xBxDxFxH;

  // input pins
  bool RST;
  bool CLK_GOOD;
  bool MODE_PROD;
  bool MODE_DBG1;
  bool MODE_DBG2;
  bool RD_C;
  bool WR_C;

  // signals from cpu
  bool CLK_REQ;
  bool ADDR_VALID;
  bool CPU_RAW_RD;
  bool CPU_RAW_WR;
  bool FROM_CPU5;

  // other random stuff for convenience
  bool BOOT_BIT;
  bool LCDC_EN;
  bool DIV_06n;
  bool DIV_07n;
  bool DIV_15;
  bool FF60_1;

  // signals trivially derived from the above
  /*p01.ABOL*/ bool ABOL_CLKREQn;
  /*p01.BUTY*/ bool BUTY_CLKREQ;
  /*p01.UCOB*/ bool UCOB_CLKBAD;
  /*p01.ATEZ*/ bool ATEZ_CLKBAD;
};

//-----------------------------------------------------------------------------

};