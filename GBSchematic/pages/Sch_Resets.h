#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ResetSignals {
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
};

//-----------------------------------------------------------------------------

struct ResetRegisters {

  void reset() {
    sig.RESET_CLK = false;

    sig.SYS_RESETn = true;
    sig.SYS_RESET = false;
    sig.CUNU_RESETn = true;

    sig.VID_RESETn = true;
    sig.WESY_RESET = true;
    sig.VID_RESET5 = false;
    sig.VID_RESET6 = false;
    sig.VID_RESETn3 = true;
    sig.VID_RESET4 = false;
    sig.VID_RESET3 = false;
    sig.XORE_RESET = false;
    sig.XEBE_RESET = true;
    sig.XODO_RESET = false;
    sig.WALU_RESET = true;
    sig.XARE_RESET = true;
    sig.SOTO_RESET = true;

    BAD_CLOCK_LATCH = true;
    RESET_REG.val = false;
    RESET_REG.clk = false;
  };

  void tick(const ResetRegisters& prev,
            bool MODE_PROD,
            bool MODE_DBG1,
            bool MODE_DBG2,
            bool RST,
            bool CLK_BAD1,
            bool CPUCLK_REQn,
            bool BOGA_AxCDEFGH,
            bool DIV_15,
            bool LCDC_EN);

  ResetSignals sig;

  /*p01.TUBO*/ bool BAD_CLOCK_LATCH;
  /*p01.AFER*/ Reg  RESET_REG;
};

//-----------------------------------------------------------------------------

};