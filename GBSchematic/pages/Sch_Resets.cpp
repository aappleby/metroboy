#include "Sch_Resets.h"

#include "Sch_Debug.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"

namespace Schematics {

//-----------------------------------------------------------------------------

ResetSignals1 ResetSignals1::tick_slow(const SystemSignals& sys_sig,
                                       const ClockSignals1& clk_sig1,
                                       const ResetRegisters& rst_reg) {

  /*p01.UPYF*/ bool UPYF = or(sys_sig.RST, sys_sig.CLK_BAD1);
  /*p01.TUBO*/ bool BAD_CLOCK_LATCH = !UPYF ? 1 : !sys_sig.CPUCLK_REQn ? 0 : rst_reg.BAD_CLOCK_LATCH;
  /*p01.UNUT*/ bool TIMEOUT     = and(BAD_CLOCK_LATCH, sys_sig.DIV_15);
  /*p01.TABA*/ bool CPU_RESET   = or(sys_sig.MODE_DBG2, sys_sig.MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ bool CPU_RESETn  = not(CPU_RESET);
  /*p01.AFAR*/ wire AFAR        = nor(CPU_RESETn, sys_sig.RST);
  /*p01.ASOL*/ bool RESET_IN    = or (AFAR, sys_sig.RST);

  /*p01.AVOR*/ bool AVOR_RESET  = or(rst_reg.RESET_REG, RESET_IN);
  /*p01.ALUR*/ wire SYS_RESETn  = not(AVOR_RESET);   // this goes all over the place
  /*p01.DULA*/ wire SYS_RESET   = not(SYS_RESETn);
  /*p01.SOTO*/ wire SOTO_RESET  = not(SYS_RESET);
  /*p01.CUNU*/ wire CUNU_RESETn = not(SYS_RESET);
  /*p01.XORE*/ wire XORE_RESET  = not(CUNU_RESETn);

  ResetSignals1 rst_sig;

  {
    /*p01.BOMA*/ wire RESET_CLK    = not(clk_sig1.BOGA_AxCDEFGH);
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

  return rst_sig;
}

//-----------------------------------------------------------------------------

ResetSignals2 ResetSignals2::tick_slow(const SystemSignals& sys_sig,
                                       const ResetRegisters& rst_reg) {

  /*p01.UPYF*/ bool UPYF = or(sys_sig.RST, sys_sig.CLK_BAD1);
  /*p01.TUBO*/ bool BAD_CLOCK_LATCH = !UPYF ? 1 : !sys_sig.CPUCLK_REQn ? 0 : rst_reg.BAD_CLOCK_LATCH;
  /*p01.UNUT*/ bool TIMEOUT     = and(BAD_CLOCK_LATCH, sys_sig.DIV_15);
  /*p01.TABA*/ bool CPU_RESET   = or(sys_sig.MODE_DBG2, sys_sig.MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ bool CPU_RESETn  = not(CPU_RESET);
  /*p01.AFAR*/ wire AFAR        = nor(CPU_RESETn, sys_sig.RST);
  /*p01.ASOL*/ bool RESET_IN    = or (AFAR, sys_sig.RST);

  /*p01.AVOR*/ bool AVOR_RESET  = or(rst_reg.RESET_REG, RESET_IN);
  /*p01.ALUR*/ wire SYS_RESETn  = not(AVOR_RESET);   // this goes all over the place
  /*p01.DULA*/ wire SYS_RESET   = not(SYS_RESETn);
  /*p01.SOTO*/ wire SOTO_RESET  = not(SYS_RESET);
  /*p01.CUNU*/ wire CUNU_RESETn = not(SYS_RESET);
  /*p01.XORE*/ wire XORE_RESET  = not(CUNU_RESETn);

  ResetSignals2 rst_sig;

  {
    /*p01.XEBE*/ wire XEBE_RESET  = not(XORE_RESET);
    /*p01.XODO*/ wire XODO_RESET  = nand(XEBE_RESET, sys_sig.LCDC_EN);
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

void ResetRegisters::tock_slow(const SystemSignals& sys_sig,
                               const ClockSignals1& clk_sig1,
                               const ResetRegisters& rst_reg,
                               ResetRegisters& next) {
  /*p01.UPYF*/ bool UPYF = or(sys_sig.RST, sys_sig.CLK_BAD1);
  /*p01.TUBO*/ bool BAD_CLOCK_LATCH2 = !UPYF ? 1 : !sys_sig.CPUCLK_REQn ? 0 : rst_reg.BAD_CLOCK_LATCH;
  /*p01.BOMA*/ bool RESET_CLK   = not(clk_sig1.BOGA_AxCDEFGH);
  /*p01.UNUT*/ bool TIMEOUT     = and(BAD_CLOCK_LATCH2, sys_sig.DIV_15);
  /*p01.TABA*/ bool CPU_RESET   = or(sys_sig.MODE_DBG2, sys_sig.MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ bool CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ bool RESET_IN    = or (/*p01.AFAR*/ nor(CPU_RESETn, sys_sig.RST), sys_sig.RST);

  /*p01.TUBO*/ next.BAD_CLOCK_LATCH = BAD_CLOCK_LATCH2;
  /*p01.AFER*/ next.RESET_REG.tock(RESET_CLK, sys_sig.MODE_PROD, RESET_IN);
}

//-----------------------------------------------------------------------------

};