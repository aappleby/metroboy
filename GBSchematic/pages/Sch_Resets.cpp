#include "Sch_Resets.h"

#include "Sch_Debug.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"

namespace Schematics {

//-----------------------------------------------------------------------------

ResetSignals ResetSignals::tick(const ResetRegisters& prev,
                                bool MODE_DBG1,
                                bool MODE_DBG2,
                                bool RST,
                                bool CLK_BAD1,
                                bool CPUCLK_REQn,
                                bool BOGA_AxCDEFGH,
                                bool DIV_15,
                                bool LCDC_EN) {

  /*p01.AFER*/ bool prev_RESET_REG = prev.RESET_REG;
  /*p01.TUBO*/ bool prev_BAD_CLOCK_LATCH = prev.BAD_CLOCK_LATCH;

  //----------

  /*p01.UPYF*/ bool UPYF = or(RST, CLK_BAD1);

  // weird latch
  /*p01.TUBO*/ bool BAD_CLOCK_LATCH = !UPYF ? 1 : !CPUCLK_REQn ? 0 : prev_BAD_CLOCK_LATCH;
  /*p01.BOMA*/ bool RESET_CLK   = not(BOGA_AxCDEFGH);
  /*p01.UNUT*/ bool TIMEOUT     = and(BAD_CLOCK_LATCH, DIV_15);
  /*p01.TABA*/ bool CPU_RESET   = or(MODE_DBG2, MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ bool CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ bool RESET_IN    = or (/*p01.AFAR*/ nor(CPU_RESETn, RST), RST);

  // Master system reset

  /*p01.AVOR*/ bool AVOR_RESET  = or(prev_RESET_REG, RESET_IN);
  /*p01.ALUR*/ bool SYS_RESETn  = not(AVOR_RESET);  // this goes all over the place
  /*p01.DULA*/ bool SYS_RESET   = not(SYS_RESETn);

  // Master video reset

  /*p01.CUNU*/ bool CUNU_RESETn = not(SYS_RESET);
  /*p01.XORE*/ bool XORE_RESET  = not(CUNU_RESETn);
  /*p01.XEBE*/ bool XEBE_RESET  = not(XORE_RESET);
  /*p01.XODO*/ bool XODO_RESET  = nand(XEBE_RESET, LCDC_EN);
  /*p01.XAPO*/ bool VID_RESETn  = not(XODO_RESET);

  /*p01.WALU*/ bool WALU_RESET  = not(XORE_RESET);
  /*p01.XARE*/ bool XARE_RESET  = not(XORE_RESET);

  /*p01.ROSY*/ bool VID_RESET5  = not(VID_RESETn);
  /*p01.ATAR*/ bool VID_RESET6  = not(VID_RESETn);
  /*p01.PYRY*/ bool VID_RESET4  = not(VID_RESETn);
  /*p01.TOFU*/ bool VID_RESET3  = not(VID_RESETn);
  /*p01.ABEZ*/ bool VID_RESETn3 = not(VID_RESET6);
  /*p01.WESY*/ bool WESY_RESET  = not(XORE_RESET); // video
  /*p01.SOTO*/ bool SOTO_RESET  = not(SYS_RESET);

  //----------

  ResetSignals rst_sig = {};

  /*p01.BOMA*/ rst_sig.RESET_CLK   = RESET_CLK;
  /*p01.ALUR*/ rst_sig.SYS_RESETn  = SYS_RESETn;
  /*p01.DULA*/ rst_sig.SYS_RESET   = SYS_RESET;

  /*p01.CUNU*/ rst_sig.CUNU_RESETn = CUNU_RESETn;
  /*p01.XORE*/ rst_sig.XORE_RESET  = XORE_RESET;
  /*p01.XEBE*/ rst_sig.XEBE_RESET  = XEBE_RESET;
  /*p01.XODO*/ rst_sig.XODO_RESET  = XODO_RESET;
  /*p01.XAPO*/ rst_sig.VID_RESETn  = VID_RESETn;

  /*p01.WALU*/ rst_sig.WALU_RESET  = WALU_RESET;
  /*p01.XARE*/ rst_sig.XARE_RESET  = XARE_RESET;

  /*p01.ROSY*/ rst_sig.VID_RESET5  = VID_RESET5;
  /*p01.ATAR*/ rst_sig.VID_RESET6  = VID_RESET6;
  /*p01.PYRY*/ rst_sig.VID_RESET4  = VID_RESET4;
  /*p01.TOFU*/ rst_sig.VID_RESET3  = VID_RESET3;
  /*p01.ABEZ*/ rst_sig.VID_RESETn3 = VID_RESETn3;
  /*p01.WESY*/ rst_sig.WESY_RESET  = WESY_RESET;
  /*p01.SOTO*/ rst_sig.SOTO_RESET  = SOTO_RESET;

  return rst_sig;
}

//-----------------------------------------------------------------------------

void ResetRegisters::tick(const ResetRegisters& prev,
                  bool MODE_PROD,
                  bool MODE_DBG1,
                  bool MODE_DBG2,
                  bool RST,
                  bool CLK_BAD1,
                  bool CPUCLK_REQn,
                  bool BOGA_AxCDEFGH,
                  bool DIV_15,
                  bool LCDC_EN) {
  ResetSignals rst_sig = ResetSignals::tick(prev, MODE_DBG1, MODE_DBG2, RST, CLK_BAD1, CPUCLK_REQn, BOGA_AxCDEFGH, DIV_15, LCDC_EN);

  ResetRegisters& next = *this;

  //----------

  /*p01.UPYF*/ bool UPYF = or(RST, CLK_BAD1);

  // weird latch
  /*p01.TUBO*/ bool BAD_CLOCK_LATCH2 = !UPYF ? 1 : !CPUCLK_REQn ? 0 : prev.BAD_CLOCK_LATCH;
  /*p01.BOMA*/ bool RESET_CLK   = not(BOGA_AxCDEFGH);
  /*p01.UNUT*/ bool TIMEOUT     = and(BAD_CLOCK_LATCH2, DIV_15);
  /*p01.TABA*/ bool CPU_RESET   = or(MODE_DBG2, MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ bool CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ bool RESET_IN    = or (/*p01.AFAR*/ nor(CPU_RESETn, RST), RST);

  // Master system reset

  // Master video reset

  /*p01.TUBO*/ next.BAD_CLOCK_LATCH = BAD_CLOCK_LATCH2;
  /*p01.AFER*/ next.RESET_REG.tock(RESET_CLK, MODE_PROD, RESET_IN);

  next.sig = rst_sig;
}

//-----------------------------------------------------------------------------

};