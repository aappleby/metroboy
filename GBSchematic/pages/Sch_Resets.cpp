#include "Sch_Resets.h"

#include "Sch_Debug.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"

namespace Schematics {

//-----------------------------------------------------------------------------

ResetSignals1 ResetSignals1::tick(const ResetRegisters& rst_reg,
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
  /*p01.AFAR*/ wire AFAR        = nor(CPU_RESETn, RST);
  /*p01.ASOL*/ bool RESET_IN    = or (AFAR, RST);

  /*p01.AVOR*/ bool AVOR_RESET  = or(rst_reg.RESET_REG, RESET_IN);
  /*p01.ALUR*/ wire SYS_RESETn  = not(AVOR_RESET);   // this goes all over the place
  /*p01.DULA*/ wire SYS_RESET   = not(SYS_RESETn);
  /*p01.SOTO*/ wire SOTO_RESET  = not(SYS_RESET);
  /*p01.CUNU*/ wire CUNU_RESETn = not(SYS_RESET);
  /*p01.XORE*/ wire XORE_RESET  = not(CUNU_RESETn);

  ResetSignals1 rst_sig;

  {
    /*p01.BOMA*/ wire RESET_CLK    = not(BOGA_AxCDEFGH);
    /*p01.BOMA*/ rst_sig.RESET_CLK = RESET_CLK;
  }

  {
    /*p01.WESY*/ wire WESY_RESET  = not(XORE_RESET);
    /*p01.WALU*/ wire WALU_RESET  = not(XORE_RESET);
    /*p01.XARE*/ wire XARE_RESET  = not(XORE_RESET);

    /*p01.ALUR*/ rst_sig.SYS_RESETn  = SYS_RESETn;
    /*p01.DULA*/ rst_sig.SYS_RESET   = SYS_RESET;
    /*p01.CUNU*/ rst_sig.CUNU_RESETn = CUNU_RESETn;
    /*p01.WESY*/ rst_sig.WESY_RESET  = WESY_RESET;
    /*p01.WALU*/ rst_sig.WALU_RESET  = WALU_RESET;
    /*p01.XARE*/ rst_sig.XARE_RESET  = XARE_RESET;
    /*p01.SOTO*/ rst_sig.SOTO_RESET  = SOTO_RESET;
  }

  {
    /*p01.XEBE*/ wire XEBE_RESET  = not(XORE_RESET);
    /*p01.XODO*/ wire XODO_RESET  = nand(XEBE_RESET, LCDC_EN);
    /*p01.XAPO*/ wire VID_RESETn  = not(XODO_RESET);
    /*p01.TOFU*/ wire VID_RESET3  = not(VID_RESETn);
    /*p01.PYRY*/ wire VID_RESET4  = not(VID_RESETn);
    /*p01.ROSY*/ wire VID_RESET5  = not(VID_RESETn);
    /*p01.ATAR*/ wire VID_RESET6  = not(VID_RESETn);
    /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);

    /*p01.XAPO*/ rst_sig.VID_RESETn  = VID_RESETn;
    /*p01.TOFU*/ rst_sig.VID_RESET3  = VID_RESET3;
    /*p01.PYRY*/ rst_sig.VID_RESET4  = VID_RESET4;
    /*p01.ROSY*/ rst_sig.VID_RESET5  = VID_RESET5;
    /*p01.ATAR*/ rst_sig.VID_RESET6  = VID_RESET6;
    /*p01.ABEZ*/ rst_sig.VID_RESETn3 = VID_RESETn3;
  }

  return rst_sig;
}

//-----------------------------------------------------------------------------

ResetSignals2 ResetSignals2::tick(const ResetRegisters& rst_reg,
                                bool MODE_DBG1,
                                bool MODE_DBG2,
                                bool RST,
                                bool CLK_BAD1,
                                bool CPUCLK_REQn,
                                bool DIV_15,
                                bool LCDC_EN) {

  /*p01.UPYF*/ bool UPYF = or(RST, CLK_BAD1);
  /*p01.TUBO*/ bool BAD_CLOCK_LATCH = !UPYF ? 1 : !CPUCLK_REQn ? 0 : rst_reg.BAD_CLOCK_LATCH;
  /*p01.UNUT*/ bool TIMEOUT     = and(BAD_CLOCK_LATCH, DIV_15);
  /*p01.TABA*/ bool CPU_RESET   = or(MODE_DBG2, MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ bool CPU_RESETn  = not(CPU_RESET);
  /*p01.AFAR*/ wire AFAR        = nor(CPU_RESETn, RST);
  /*p01.ASOL*/ bool RESET_IN    = or (AFAR, RST);

  /*p01.AVOR*/ bool AVOR_RESET  = or(rst_reg.RESET_REG, RESET_IN);
  /*p01.ALUR*/ wire SYS_RESETn  = not(AVOR_RESET);   // this goes all over the place
  /*p01.DULA*/ wire SYS_RESET   = not(SYS_RESETn);
  /*p01.SOTO*/ wire SOTO_RESET  = not(SYS_RESET);
  /*p01.CUNU*/ wire CUNU_RESETn = not(SYS_RESET);
  /*p01.XORE*/ wire XORE_RESET  = not(CUNU_RESETn);

  ResetSignals2 rst_sig;

  {
    /*p01.XEBE*/ wire XEBE_RESET  = not(XORE_RESET);
    /*p01.XODO*/ wire XODO_RESET  = nand(XEBE_RESET, LCDC_EN);
    /*p01.XAPO*/ wire VID_RESETn  = not(XODO_RESET);
    /*p01.TOFU*/ wire VID_RESET3  = not(VID_RESETn);
    /*p01.PYRY*/ wire VID_RESET4  = not(VID_RESETn);
    /*p01.ROSY*/ wire VID_RESET5  = not(VID_RESETn);
    /*p01.ATAR*/ wire VID_RESET6  = not(VID_RESETn);
    /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);

    /*p01.XAPO*/ rst_sig.VID_RESETn  = VID_RESETn;
    /*p01.TOFU*/ rst_sig.VID_RESET3  = VID_RESET3;
    /*p01.PYRY*/ rst_sig.VID_RESET4  = VID_RESET4;
    /*p01.ROSY*/ rst_sig.VID_RESET5  = VID_RESET5;
    /*p01.ATAR*/ rst_sig.VID_RESET6  = VID_RESET6;
    /*p01.ABEZ*/ rst_sig.VID_RESETn3 = VID_RESETn3;
  }

  return rst_sig;
}

//-----------------------------------------------------------------------------

void ResetRegisters::tock(const ResetSignals1& rst_sig1,
                          const ResetSignals2& /*rst_sig2*/,
                          const ResetRegisters& rst_reg,
                          bool MODE_PROD,
                          bool MODE_DBG1,
                          bool MODE_DBG2,
                          bool RST,
                          bool CLK_BAD1,
                          bool CPUCLK_REQn,
                          bool BOGA_AxCDEFGH,
                          bool DIV_15,
                          ResetRegisters& next) {
  /*p01.UPYF*/ bool UPYF = or(RST, CLK_BAD1);
  /*p01.TUBO*/ bool BAD_CLOCK_LATCH2 = !UPYF ? 1 : !CPUCLK_REQn ? 0 : rst_reg.BAD_CLOCK_LATCH;
  /*p01.BOMA*/ bool RESET_CLK   = not(BOGA_AxCDEFGH);
  /*p01.UNUT*/ bool TIMEOUT     = and(BAD_CLOCK_LATCH2, DIV_15);
  /*p01.TABA*/ bool CPU_RESET   = or(MODE_DBG2, MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ bool CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ bool RESET_IN    = or (/*p01.AFAR*/ nor(CPU_RESETn, RST), RST);

  /*p01.TUBO*/ next.BAD_CLOCK_LATCH = BAD_CLOCK_LATCH2;
  /*p01.AFER*/ next.RESET_REG.tock(RESET_CLK, MODE_PROD, RESET_IN);

  next.sig = rst_sig1;
}

//-----------------------------------------------------------------------------

};