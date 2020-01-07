#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct SysPins {
  /* PIN_71 */ bool RST;     // -> TUBO,ASOL,UFOL,UPOJ
  /* PIN_74 */ bool CLKIN_A; // clock good signal
  /* PIN_74 */ bool CLKIN_B; // clock signal

  /* PIN_76 */ bool T2;      // -> P07.UVAR
  /* PIN_77 */ bool T1;      // -> P07.UBET
};

//-----------------------------------------------------------------------------

struct SysSignals {
  // master clock
  const int phase;

  // input pins
  wire PIN_RST;
  wire PIN_CLK_GOOD;
  bool PIN_T1;
  bool PIN_T2;
  wire PIN_RD_C;
  wire PIN_WR_C;
  bool PIN_P10_B;

  // signals from cpu
  wire CPU_CLK_REQ;
  wire CPU_ADDR_VALID;
  wire CPU_RAW_RD;
  wire CPU_RAW_WR;
  wire CPU_FROM_CPU5;

  // other random stuff for convenience
  wire MODE_PROD;
  wire MODE_DBG1;
  wire MODE_DBG2;
  wire BOOT_BIT;
  wire LCDC_EN;
  wire DIV_06n;
  wire DIV_07n;
  wire DIV_15;

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
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  int phase() const;
  SysSignals signals() const;

  //----------------------------------------
  // master clock

  int phase_count;

  // input pins
  bool PIN_RST;
  bool PIN_CLK_GOOD;
  bool PIN_T1;
  bool PIN_T2;
  bool PIN_RD_C;
  bool PIN_WR_C;
  bool PIN_P10_B;

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
};

//-----------------------------------------------------------------------------

};