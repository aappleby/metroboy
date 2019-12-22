#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ResetSignals {
  /*p01.BOMA*/ wire RESET_CLK; // _____fgh -> PORTD_07
  /*p01.ALUR*/ wire SYS_RESETn;
  /*p01.DULA*/ wire SYS_RESET;

  /*p01.CUNU*/ wire CUNU_RESETn;
  /*p01.XORE*/ wire XORE_RESET;
  /*p01.XEBE*/ wire XEBE_RESET;
  /*p01.XODO*/ wire XODO_RESET;
  /*p01.XAPO*/ wire VID_RESETn;

  /*p01.WESY*/ wire WESY_RESET;
  /*p01.WALU*/ wire WALU_RESET;
  /*p01.XARE*/ wire XARE_RESET;
  /*p01.SOTO*/ wire SOTO_RESET;

  /*p01.ROSY*/ wire VID_RESET5;
  /*p01.ATAR*/ wire VID_RESET6;
  /*p01.ABEZ*/ wire VID_RESETn3;
  /*p01.PYRY*/ wire VID_RESET4;
  /*p01.TOFU*/ wire VID_RESET3;
};

struct Resets {

  void reset() {
    BAD_CLOCK_LATCH = true;
    RESET_CLK = false;
    RESET_REG.val = false;
    RESET_REG.clk = false;
    SYS_RESETn = true;
    SYS_RESET = false;
    CUNU_RESETn = true;
    VID_RESETn = true;
    WESY_RESET = true;
    VID_RESET5 = false;
    VID_RESET6 = false;
    VID_RESETn3 = true;
    VID_RESET4 = false;
    VID_RESET3 = false;
    XORE_RESET = false;
    XEBE_RESET = true;
    XODO_RESET = false;
    WALU_RESET = true;
    XARE_RESET = true;
    SOTO_RESET = true;
  };

  void tick(const Resets& prev,
            bool MODE_PROD,
            bool MODE_DBG1,
            bool MODE_DBG2,
            bool RST,
            bool CLK_BAD1,
            bool CPUCLK_REQn,
            bool BOGA_AxCDEFGH,
            bool DIV_15,
            bool LCDC_EN);

  /*p01.BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07

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

  /*p01.TUBO*/ bool BAD_CLOCK_LATCH;
  /*p01.AFER*/ Reg  RESET_REG;
};

//-----------------------------------------------------------------------------

};