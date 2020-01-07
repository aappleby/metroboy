#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void SysRegisters::pwron() {
  phase_count    = -1;

  PIN_RST        = true;
  PIN_CLK_GOOD   = false;
  PIN_T1         = false;
  PIN_T2         = false;

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
}

void SysRegisters::reset() {
  //phase_count = 140511;
  phase_count    = -1;

  PIN_RST        = false;
  PIN_CLK_GOOD   = true;
  PIN_T1         = false;
  PIN_T2         = false;

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
}

void SysRegisters::phase_begin() {
  phase_count++;
}

void SysRegisters::phase_end() {
}

void SysRegisters::pass_begin() {
}

bool SysRegisters::pass_end() {
  return false;
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

  /*p07.UBET*/ wire T1n = not(PIN_T1);
  /*p07.UVAR*/ wire T2n = not(PIN_T2);
  /*p07.UPOJ*/ wire MODE_PROD   = nand(T1n, T2n, PIN_RST);
  /*p07.UMUT*/ wire MODE_DBG1   = and(PIN_T1, T2n);
  /*p07.UNOR*/ wire MODE_DBG2   = and(PIN_T2, T1n);

  return {
    .phase          = phase         ,
    .PIN_RST        = PIN_RST       ,
    .PIN_CLK_GOOD   = PIN_CLK_GOOD  ,
    .PIN_T1         = PIN_T1        ,
    .PIN_T2         = PIN_T2        ,
    .PIN_RD_C       = PIN_RD_C      ,
    .PIN_WR_C       = PIN_WR_C      ,
    .PIN_P10_B      = PIN_P10_B     ,

    .CPU_CLK_REQ    = CPU_CLK_REQ   ,
    .CPU_ADDR_VALID = CPU_ADDR_VALID,
    .CPU_RAW_RD     = CPU_RAW_RD    ,
    .CPU_RAW_WR     = CPU_RAW_WR    ,
    .CPU_FROM_CPU5  = CPU_FROM_CPU5 ,

    .MODE_PROD      = MODE_PROD     ,
    .MODE_DBG1      = MODE_DBG1     ,
    .MODE_DBG2      = MODE_DBG2     ,
    .BOOT_BIT       = BOOT_BIT      ,
    .LCDC_EN        = LCDC_EN       ,
    .DIV_06n        = DIV_06n       ,
    .DIV_07n        = DIV_07n       ,
    .DIV_15         = DIV_15        ,

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