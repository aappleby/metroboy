#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct SysSignals {
  // master clock
  const int phase;

  // input pins
  wire PIN_RST;
  wire PIN_CLK_GOOD;
  wire PIN_MODE_DBG1;
  wire PIN_MODE_DBG2;
  wire PIN_RD_C;
  wire PIN_WR_C;

  // signals from cpu
  wire CPU_CLK_REQ;
  wire CPU_ADDR_VALID;
  wire CPU_RAW_RD;
  wire CPU_RAW_WR;
  wire CPU_FROM_CPU5;

  // other random stuff for convenience
  wire MODE_PROD;
  wire BOOT_BIT;
  wire LCDC_EN;
  wire DIV_06n;
  wire DIV_07n;
  wire DIV_15;
  wire FF60_1;

  // signals trivially derived from the above
  /*p01.ATAL*/ wire ATAL_AxCxExGx;
  /*p01.AZOF*/ wire AZOF_xBxDxFxH;
  /*p01.ABOL*/ wire ABOL_CLKREQn;
  /*p01.BUTY*/ wire BUTY_CLKREQ;
  /*p01.UCOB*/ wire UCOB_CLKBAD;
  /*p01.ATEZ*/ wire ATEZ_CLKBAD;
};

//-----------------------------------------------------------------------------
// Fake registers used for puppeteering the sim while we get things working.

struct SysRegisters {
  void pwron();
  void reset();
  void commit();
  SysSignals next_phase();
  int phase() const;

  SysSignals signals() const;

  //----------------------------------------
  // master clock

  int phase_count;

  // input pins
  bool PIN_RST;
  bool PIN_CLK_GOOD;
  bool PIN_MODE_DBG1;
  bool PIN_MODE_DBG2;
  bool PIN_RD_C;
  bool PIN_WR_C;

  // signals from cpu
  bool CPU_CLK_REQ;
  bool CPU_ADDR_VALID;
  bool CPU_RAW_RD;
  bool CPU_RAW_WR;
  bool CPU_FROM_CPU5;

  // other random stuff for convenience
  bool BOOT_BIT;
  bool LCDC_EN;
  bool DIV_06n;
  bool DIV_07n;
  bool DIV_15;
  bool FF60_1;
};

//-----------------------------------------------------------------------------

};