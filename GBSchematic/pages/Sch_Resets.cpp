#include "Sch_Resets.h"

#include "Sch_Debug.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ResetsIn {
  bool CPU_RAW_RD;
  bool CPU_RAW_WR;
  bool ADDR_VALID;

  bool DIV_15;
  bool LCDC_EN;
};

// PORTD_07 = RESET_CLK
//c.cpu.CPU_RESET = CPU_RESET;

void Resets_tick(const Debug& debug,
                 const Pins& pins,
                 const Clocks& clocks,
                 const ResetsIn& in,
                 const Resets& prev,
                 Resets& next) {

  // Clock timeout reset thing

  /*p01.TUBO*/ wire NO_CLOCK  = or(clocks.CPUCLK_REQn, /*p01.UPYF*/ or(pins.RST, clocks.CLK_BAD1));
  /*p01.UNUT*/ wire TIMEOUT   = and(NO_CLOCK, in.DIV_15);
  /*p01.TABA*/ wire CPU_RESET = or(debug.MODE_DBG2, debug.MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ wire CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ wire RESET_IN  = or (/*p01.AFAR*/ nor(CPU_RESETn, pins.RST), pins.RST);
  /*p01.BOMA*/ next.RESET_CLK = not(clocks.BOGA_xBCDEFGH);
  /*p01.AFER*/ next.RESET_REG.tock(prev.RESET_CLK, debug.MODE_PROD, RESET_IN);

  // Master system reset

  /*p01.AVOR*/ wire AVOR_RESET  = or(prev.RESET_REG, RESET_IN);
  /*p01.ALUR*/ next.SYS_RESETn = not(AVOR_RESET);  // this goes all over the place

  // Master video reset

  /*p01.DULA*/ wire DULA_RESET  = not(prev.SYS_RESETn);
  /*p01.CUNU*/ wire CUNU_RESET  = not(DULA_RESET);
  /*p01.XORE*/ wire XORE_RESET  = not(CUNU_RESET);
  /*p01.XEBE*/ wire XEBE_RESET  = not(XORE_RESET);
  /*p01.XODO*/ wire XODO_RESET  = nand(XEBE_RESET, in.LCDC_EN);
  /*p01.XAPO*/ next.VID_RESETn = not(XODO_RESET);
}

//-----------------------------------------------------------------------------

};