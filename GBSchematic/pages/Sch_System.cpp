#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void SystemRegisters::pwron() {
  phase_count = -1;

  RST        = true;
  CLK_GOOD   = false;
  MODE_PROD  = true;
  MODE_DBG1  = false;
  MODE_DBG2  = false;

  CLK_REQ    = false;
  ADDR_VALID = false;
  CPU_RAW_RD = false;
  CPU_RAW_WR = false;
  FROM_CPU5  = false;

  BOOT_BIT   = true;
  LCDC_EN    = false;
  DIV_06n    = true;
  DIV_07n    = true;
  DIV_15     = false;
  FF60_1     = false;
}

void SystemRegisters::reset() {
  //phase_count = 140511;
  phase_count = -1;

  RST        = false;
  CLK_GOOD   = true;
  MODE_PROD  = true;
  MODE_DBG1  = false;
  MODE_DBG2  = false;

  CLK_REQ    = true;
  ADDR_VALID = false;
  CPU_RAW_RD = false;
  CPU_RAW_WR = false;
  FROM_CPU5  = false;

  BOOT_BIT   = true;
  LCDC_EN    = true;
  DIV_06n    = true;
  DIV_07n    = true;
  DIV_15     = false;
  FF60_1     = false;
}

void SystemRegisters::next_phase() {
  phase_count++;

  /*p01.ABOL*/ ABOL_CLKREQn = not(CLK_REQ);
  /*p01.BUTY*/ BUTY_CLKREQ  = not(ABOL_CLKREQn);
  /*p01.UCOB*/ UCOB_CLKBAD  = not(CLK_GOOD);
  /*p01.ATEZ*/ ATEZ_CLKBAD  = not(CLK_GOOD);

  // ignoring the deglitcher here
  bool clk = !(phase_count & 1);
  /*p01.ARYS*/ wire ARYS_xBxDxFxH = not(clk);
  /*p01.AVET*/ wire AVET_AxCxExGx = clk;
  /*p01.ANOS*/ wire ANOS_xBxDxFxH = not(clk);

  /*p01.ATAL*/ ATAL_AxCxExGx = not(ANOS_xBxDxFxH);
  /*p01.AZOF*/ AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
}

int SystemRegisters::phase() const {
  return (phase_count & 7);
}

//-----------------------------------------------------------------------------

};