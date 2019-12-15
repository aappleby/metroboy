#include "Sch_Resets.h"

#include "Sch_Debug.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ResetsIn {
  bool DIV_15;
  bool LCDC_EN;
};

// PORTD_07 = RESET_CLK
//c.cpu.CPU_RESET = CPU_RESET;

void Resets_tick(const Debug& dbg,
                 const Pins& pins,
                 const Clocks& clocks,
                 const ResetsIn& in,
                 const Resets& prev,
                 Resets& next) {

  // Clock timeout reset thing

  /*p01.TUBO*/ wire NO_CLOCK  = or(clocks.CPUCLK_REQn, /*p01.UPYF*/ or(pins.RST, clocks.CLK_BAD1));
  /*p01.UNUT*/ wire TIMEOUT   = and(NO_CLOCK, in.DIV_15);
  /*p01.TABA*/ wire CPU_RESET = or(dbg.MODE_DBG2, dbg.MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ wire CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ wire RESET_IN  = or (/*p01.AFAR*/ nor(CPU_RESETn, pins.RST), pins.RST);
  /*p01.BOMA*/ next.RESET_CLK = not(clocks.BOGA_xBCDEFGH);
  /*p01.AFER*/ next.RESET_REG.tock(prev.RESET_CLK, dbg.MODE_PROD, RESET_IN);

  // Master system reset

  /*p01.AVOR*/ wire AVOR_RESET  = or(prev.RESET_REG, RESET_IN);
  /*p01.ALUR*/ next.SYS_RESETn = not(AVOR_RESET);  // this goes all over the place
  /*p01.DULA*/ next.SYS_RESET  = not(prev.SYS_RESETn);

  // Master video reset

  /*p01.CUNU*/ next.CUNU_RESETn = not(prev.SYS_RESET);
  /*p01.XORE*/ next.XORE_RESET  = not(prev.CUNU_RESETn);
  /*p01.XEBE*/ next.XEBE_RESET  = not(prev.XORE_RESET);
  /*p01.XODO*/ next.XODO_RESET  = nand(prev.XEBE_RESET, in.LCDC_EN);
  /*p01.XAPO*/ next.VID_RESETn  = not(prev.XODO_RESET);

  /*p01.WALU*/ next.WALU_RESET  = not(prev.XORE_RESET);
  /*p01.XARE*/ next.XARE_RESET  = not(prev.XORE_RESET);

  /*p01.ROSY*/ next.VID_RESET5  = not(prev.VID_RESETn);
  /*p01.ATAR*/ next.VID_RESET6  = not(prev.VID_RESETn);
  /*p01.PYRY*/ next.VID_RESET4  = not(prev.VID_RESETn);
  /*p01.TOFU*/ next.VID_RESET3  = not(prev.VID_RESETn);
  /*p01.ABEZ*/ next.VID_RESETn3 = not(prev.VID_RESET6);

  /*p01.WESY*/ next.WESY_RESET  = not(prev.XORE_RESET); // video

  /*p01.SOTO*/ next.SOTO_RESET  = not(prev.SYS_RESET);
}

//-----------------------------------------------------------------------------

};