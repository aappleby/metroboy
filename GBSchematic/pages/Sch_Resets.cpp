#include "Sch_Resets.h"

#include "Sch_Debug.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"

#pragma warning(disable:4458)

namespace Schematics {

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
  ResetRegisters& next = *this;

  /*p01.AFER*/ wire prev_RESET_REG = prev.RESET_REG;
  /*p01.TUBO*/ wire prev_BAD_CLOCK_LATCH = prev.BAD_CLOCK_LATCH;

  //----------

  /*p01.UPYF*/ wire UPYF = or(RST, CLK_BAD1);

  // weird latch
  /*p01.TUBO*/ wire BAD_CLOCK_LATCH = !UPYF ? 1 : !CPUCLK_REQn ? 0 : prev_BAD_CLOCK_LATCH;
  /*p01.BOMA*/ wire RESET_CLK   = not(BOGA_AxCDEFGH);
  /*p01.UNUT*/ wire TIMEOUT     = and(BAD_CLOCK_LATCH, DIV_15);
  /*p01.TABA*/ wire CPU_RESET   = or(MODE_DBG2, MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ wire CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ wire RESET_IN    = or (/*p01.AFAR*/ nor(CPU_RESETn, RST), RST);

  // Master system reset

  /*p01.AVOR*/ wire AVOR_RESET  = or(prev_RESET_REG, RESET_IN);
  /*p01.ALUR*/ wire SYS_RESETn  = not(AVOR_RESET);  // this goes all over the place
  /*p01.DULA*/ wire SYS_RESET   = not(SYS_RESETn);

  // Master video reset

  /*p01.CUNU*/ wire CUNU_RESETn = not(SYS_RESET);
  /*p01.XORE*/ wire XORE_RESET  = not(CUNU_RESETn);
  /*p01.XEBE*/ wire XEBE_RESET  = not(XORE_RESET);
  /*p01.XODO*/ wire XODO_RESET  = nand(XEBE_RESET, LCDC_EN);
  /*p01.XAPO*/ wire VID_RESETn  = not(XODO_RESET);

  /*p01.WALU*/ wire WALU_RESET  = not(XORE_RESET);
  /*p01.XARE*/ wire XARE_RESET  = not(XORE_RESET);

  /*p01.ROSY*/ wire VID_RESET5  = not(VID_RESETn);
  /*p01.ATAR*/ wire VID_RESET6  = not(VID_RESETn);
  /*p01.PYRY*/ wire VID_RESET4  = not(VID_RESETn);
  /*p01.TOFU*/ wire VID_RESET3  = not(VID_RESETn);
  /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);
  /*p01.WESY*/ wire WESY_RESET  = not(XORE_RESET); // video
  /*p01.SOTO*/ wire SOTO_RESET  = not(SYS_RESET);

  //----------

  /*p01.TUBO*/ next.BAD_CLOCK_LATCH = BAD_CLOCK_LATCH;
  /*p01.BOMA*/ next.sig.RESET_CLK   = RESET_CLK;
  /*p01.ALUR*/ next.sig.SYS_RESETn  = SYS_RESETn;
  /*p01.DULA*/ next.sig.SYS_RESET   = SYS_RESET;

  /*p01.CUNU*/ next.sig.CUNU_RESETn = CUNU_RESETn;
  /*p01.XORE*/ next.sig.XORE_RESET  = XORE_RESET;
  /*p01.XEBE*/ next.sig.XEBE_RESET  = XEBE_RESET;
  /*p01.XODO*/ next.sig.XODO_RESET  = XODO_RESET;
  /*p01.XAPO*/ next.sig.VID_RESETn  = VID_RESETn;

  /*p01.WALU*/ next.sig.WALU_RESET  = WALU_RESET;
  /*p01.XARE*/ next.sig.XARE_RESET  = XARE_RESET;

  /*p01.ROSY*/ next.sig.VID_RESET5  = VID_RESET5;
  /*p01.ATAR*/ next.sig.VID_RESET6  = VID_RESET6;
  /*p01.PYRY*/ next.sig.VID_RESET4  = VID_RESET4;
  /*p01.TOFU*/ next.sig.VID_RESET3  = VID_RESET3;
  /*p01.ABEZ*/ next.sig.VID_RESETn3 = VID_RESETn3;
  /*p01.WESY*/ next.sig.WESY_RESET  = WESY_RESET;
  /*p01.SOTO*/ next.sig.SOTO_RESET  = SOTO_RESET;

  /*p01.AFER*/ next.RESET_REG.tock(RESET_CLK, MODE_PROD, RESET_IN);
}

//-----------------------------------------------------------------------------

};