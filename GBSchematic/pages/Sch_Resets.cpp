#include "Sch_Resets.h"

#include "Sch_Debug.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"

namespace Schematics {

//-----------------------------------------------------------------------------

ResetSignals ResetSignals::tick(const ResetRegisters& rst_reg,
                                bool MODE_DBG1,
                                bool MODE_DBG2,
                                bool RST,
                                bool CLK_BAD1,
                                bool CPUCLK_REQn,
                                bool BOGA_AxCDEFGH,
                                bool DIV_15,
                                bool LCDC_EN) {

  /*p01.UPYF*/ bool UPYF = or(RST, CLK_BAD1);
  /*p01.TUBO*/ bool BAD_CLOCK_LATCH = !UPYF ? 1 : !CPUCLK_REQn ? 0 : rst_reg.BAD_CLOCK_LATCH;
  /*p01.UNUT*/ bool TIMEOUT     = and(BAD_CLOCK_LATCH, DIV_15);
  /*p01.TABA*/ bool CPU_RESET   = or(MODE_DBG2, MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ bool CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ bool RESET_IN    = or (/*p01.AFAR*/ nor(CPU_RESETn, RST), RST);
  /*p01.AVOR*/ bool AVOR_RESET  = or(rst_reg.RESET_REG, RESET_IN);

  ResetSignals rst_sig = {
    /*p01.BOMA*/ .RESET_CLK   = not(BOGA_AxCDEFGH),
    /*p01.ALUR*/ .SYS_RESETn  = not(AVOR_RESET),   // this goes all over the place
    /*p01.DULA*/ .SYS_RESET   = not(rst_sig.SYS_RESETn),
    /*p01.CUNU*/ .CUNU_RESETn = not(rst_sig.SYS_RESET),
    /*p01.XORE*/ .XORE_RESET  = not(rst_sig.CUNU_RESETn),
    /*p01.XEBE*/ .XEBE_RESET  = not(rst_sig.XORE_RESET),
    /*p01.XODO*/ .XODO_RESET  = nand(rst_sig.XEBE_RESET, LCDC_EN),
    /*p01.XAPO*/ .VID_RESETn  = not(rst_sig.XODO_RESET),
    /*p01.WESY*/ .WESY_RESET  = not(rst_sig.XORE_RESET),
    /*p01.ROSY*/ .VID_RESET5  = not(rst_sig.VID_RESETn),
    /*p01.ATAR*/ .VID_RESET6  = not(rst_sig.VID_RESETn),
    /*p01.ABEZ*/ .VID_RESETn3 = not(rst_sig.VID_RESET6),
    /*p01.PYRY*/ .VID_RESET4  = not(rst_sig.VID_RESETn),
    /*p01.TOFU*/ .VID_RESET3  = not(rst_sig.VID_RESETn),
    /*p01.WALU*/ .WALU_RESET  = not(rst_sig.XORE_RESET),
    /*p01.XARE*/ .XARE_RESET  = not(rst_sig.XORE_RESET),
    /*p01.SOTO*/ .SOTO_RESET  = not(rst_sig.SYS_RESET),
  };

  return rst_sig;
}

//-----------------------------------------------------------------------------

void ResetRegisters::tock(const ResetSignals& /*rst_sig*/,
                          const ResetRegisters& rst_reg,
                          bool MODE_PROD,
                          bool MODE_DBG1,
                          bool MODE_DBG2,
                          bool RST,
                          bool CLK_BAD1,
                          bool CPUCLK_REQn,
                          bool BOGA_AxCDEFGH,
                          bool DIV_15,
                          bool LCDC_EN,
                          ResetRegisters& next) {
  ResetSignals sig = ResetSignals::tick(rst_reg, MODE_DBG1, MODE_DBG2, RST, CLK_BAD1, CPUCLK_REQn, BOGA_AxCDEFGH, DIV_15, LCDC_EN);

  /*p01.UPYF*/ bool UPYF = or(RST, CLK_BAD1);
  /*p01.TUBO*/ bool BAD_CLOCK_LATCH2 = !UPYF ? 1 : !CPUCLK_REQn ? 0 : rst_reg.BAD_CLOCK_LATCH;
  /*p01.BOMA*/ bool RESET_CLK   = not(BOGA_AxCDEFGH);
  /*p01.UNUT*/ bool TIMEOUT     = and(BAD_CLOCK_LATCH2, DIV_15);
  /*p01.TABA*/ bool CPU_RESET   = or(MODE_DBG2, MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ bool CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ bool RESET_IN    = or (/*p01.AFAR*/ nor(CPU_RESETn, RST), RST);

  /*p01.TUBO*/ next.BAD_CLOCK_LATCH = BAD_CLOCK_LATCH2;
  /*p01.AFER*/ next.RESET_REG.tock(RESET_CLK, MODE_PROD, RESET_IN);

  next.sig = sig;
}

//-----------------------------------------------------------------------------

};