#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void SysRegisters::pwron() {
  phase_count    = -1;

  PIN_RST        = true;
  PIN_CLK_GOOD   = false;
  PIN_MODE_DBG1  = false;
  PIN_MODE_DBG2  = false;

  CPU_CLK_REQ    = false;
  CPU_ADDR_VALID = false;
  CPU_RAW_RD     = false;
  CPU_RAW_WR     = false;
  CPU_FROM_CPU5  = false;

  BOOT_BIT       = true;
  LCDC_EN        = false;
  DIV_06n        = true;
  DIV_07n        = true;
  DIV_15         = false;
  FF60_1         = false;
}

void SysRegisters::reset() {
  //phase_count = 140511;
  phase_count    = -1;

  PIN_RST        = false;
  PIN_CLK_GOOD   = true;
  PIN_MODE_DBG1  = false;
  PIN_MODE_DBG2  = false;

  CPU_CLK_REQ    = true;
  CPU_ADDR_VALID = false;
  CPU_RAW_RD     = false;
  CPU_RAW_WR     = false;
  CPU_FROM_CPU5  = false;

  BOOT_BIT       = true;
  LCDC_EN        = true;
  DIV_06n        = true;
  DIV_07n        = true;
  DIV_15         = false;
  FF60_1         = false;
}

void SysRegisters::commit() {
}

SysSignals SysRegisters::next_phase() {
  phase_count++;
  return signals();
}

int SysRegisters::phase() const {
  return (phase_count & 7);
}

//-----------------------------------------------------------------------------

SysSignals SysRegisters::signals() const {

  // ignoring the deglitcher here
  bool clk = !(phase_count & 1);

  int phase = phase_count & 7;
  /*p01.ARYS*/ wire ARYS_xBxDxFxH = not(clk);
  /*p01.AVET*/ wire AVET_AxCxExGx = clk;
  /*p01.ANOS*/ wire ANOS_xBxDxFxH = not(clk);
  /*p01.ATAL*/ wire ATAL_AxCxExGx = not(ANOS_xBxDxFxH);
  /*p01.AZOF*/ wire AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
  /*p01.ABOL*/ wire ABOL_CLKREQn  = not(CPU_CLK_REQ);
  /*p01.BUTY*/ wire BUTY_CLKREQ   = not(ABOL_CLKREQn);
  /*p01.UCOB*/ wire UCOB_CLKBAD   = not(PIN_CLK_GOOD);
  /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(PIN_CLK_GOOD);
  wire MODE_PROD = !PIN_MODE_DBG1 && !PIN_MODE_DBG2;

  return {
    .phase          = phase         ,
    .PIN_RST        = PIN_RST       ,
    .PIN_CLK_GOOD   = PIN_CLK_GOOD  ,
    .PIN_MODE_DBG1  = PIN_MODE_DBG1 ,
    .PIN_MODE_DBG2  = PIN_MODE_DBG2 ,
    .PIN_RD_C       = PIN_RD_C      ,
    .PIN_WR_C       = PIN_WR_C      ,
    .CPU_CLK_REQ    = CPU_CLK_REQ   ,
    .CPU_ADDR_VALID = CPU_ADDR_VALID,
    .CPU_RAW_RD     = CPU_RAW_RD    ,
    .CPU_RAW_WR     = CPU_RAW_WR    ,
    .CPU_FROM_CPU5  = CPU_FROM_CPU5 ,
    .MODE_PROD      = MODE_PROD     ,
    .BOOT_BIT       = BOOT_BIT      ,
    .LCDC_EN        = LCDC_EN       ,
    .DIV_06n        = DIV_06n       ,
    .DIV_07n        = DIV_07n       ,
    .DIV_15         = DIV_15        ,
    .FF60_1         = FF60_1        ,
    .ATAL_AxCxExGx  = ATAL_AxCxExGx ,
    .AZOF_xBxDxFxH  = AZOF_xBxDxFxH ,
    .ABOL_CLKREQn   = ABOL_CLKREQn  ,
    .BUTY_CLKREQ    = BUTY_CLKREQ   ,
    .UCOB_CLKBAD    = UCOB_CLKBAD   ,
    .ATEZ_CLKBAD    = ATEZ_CLKBAD   ,
  };
}

//-----------------------------------------------------------------------------

};