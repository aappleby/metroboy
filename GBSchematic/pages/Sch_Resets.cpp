#include "Sch_Resets.h"

#include "Sch_Debug.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"

#pragma warning(disable:4458)

namespace Schematics {

//-----------------------------------------------------------------------------

ResetSignals1 ResetSignals1::tick_slow(const SystemRegisters& sys_reg,
                                       const ClockSignals1& clk_sig1,
                                       const ResetRegisters& rst_reg) {


  /*p01.UPYF*/ wire UPYF = or(sys_reg.RST, sys_reg.CLK_BAD1);
  /*p01.TUBO*/ bool WAITING_FOR_CLKREQ = rst_reg.WAITING_FOR_CLKREQ;
  /*p01.TUBO*/ if (!sys_reg.CPUCLK_REQn) WAITING_FOR_CLKREQ = 0;
  /*p01.TUBO*/ if (!UPYF)                WAITING_FOR_CLKREQ = 1;

  /*p01.UNUT*/ wire TIMEOUT     = and(WAITING_FOR_CLKREQ, sys_reg.DIV_15);
  /*p01.TABA*/ wire CPU_RESET   = or(sys_reg.MODE_DBG2, sys_reg.MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ wire CPU_RESETn  = not(CPU_RESET);
  /*p01.AFAR*/ wire AFAR        = nor(CPU_RESETn, sys_reg.RST);
  /*p01.ASOL*/ wire RESET_IN    = or (AFAR, sys_reg.RST);

  /*p01.AVOR*/ wire AVOR_RESET  = or(rst_reg.RESET_REG, RESET_IN);
  /*p01.ALUR*/ wire SYS_RESETn  = not(AVOR_RESET);   // this goes all over the place
  /*p01.DULA*/ wire SYS_RESET   = not(SYS_RESETn);
  /*p01.SOTO*/ wire SOTO_RESET  = not(SYS_RESET);
  /*p01.CUNU*/ wire CUNU_RESETn = not(SYS_RESET);
  /*p01.XORE*/ wire XORE_RESET  = not(CUNU_RESETn);
  /*p01.BOMA*/ wire RESET_CLK   = not(clk_sig1.BOGA_xBCDEFGH);
  /*p01.WESY*/ wire WESY_RESET  = not(XORE_RESET);
  /*p01.WALU*/ wire WALU_RESET  = not(XORE_RESET);
  /*p01.XARE*/ wire XARE_RESET  = not(XORE_RESET);

  ResetSignals1 sig;

  /*p01.BOMA*/ sig.RESET_CLK   = RESET_CLK;
  /*p01.ALUR*/ sig.SYS_RESETn  = SYS_RESETn;
  /*p01.DULA*/ sig.SYS_RESET   = SYS_RESET;
  /*p01.CUNU*/ sig.CUNU_RESETn = CUNU_RESETn;
  /*p01.WESY*/ sig.WESY_RESET  = WESY_RESET;
  /*p01.WALU*/ sig.WALU_RESET  = WALU_RESET;
  /*p01.XARE*/ sig.XARE_RESET  = XARE_RESET;
  /*p01.SOTO*/ sig.SOTO_RESET  = SOTO_RESET;

  return sig;
}

//----------------------------------------

ResetSignals1 ResetSignals1::tick_fast(const SystemRegisters& sys_reg, const ResetRegisters& rst_reg) {

  bool RESET = false;

  // polarity here seems weird
  bool WAITING_FOR_CLKREQ = rst_reg.WAITING_FOR_CLKREQ;
  if (sys_reg.CPUCLK_REQ) WAITING_FOR_CLKREQ = 0;
  if (!sys_reg.RST && sys_reg.CLK_GOOD) WAITING_FOR_CLKREQ = 1;

  bool TIMEOUT = and(WAITING_FOR_CLKREQ, sys_reg.DIV_15);

  RESET = TIMEOUT || rst_reg.RESET_REG || sys_reg.RST || sys_reg.MODE_DBG1 || sys_reg.MODE_DBG2;

  ResetSignals1 sig;

  sig.RESET_CLK = sys_reg.phaseC() == 0;
  if (!sys_reg.MODE_PROD) sig.RESET_CLK = 0;
  if (!sys_reg.CLK_GOOD)  sig.RESET_CLK = 1;

  sig.SYS_RESETn  = !RESET;
  sig.SYS_RESET   =  RESET;
  sig.CUNU_RESETn = !RESET;
  sig.WESY_RESET  = !RESET;
  sig.WALU_RESET  = !RESET;
  sig.XARE_RESET  = !RESET;
  sig.SOTO_RESET  = !RESET;

  return sig;
}

//-----------------------------------------------------------------------------

ResetSignals2 ResetSignals2::tick_slow(const SystemRegisters& sys_reg,
                              const ResetSignals1& rst_sig1) {
  /*p01.XORE*/ wire XORE_RESET  = not(rst_sig1.CUNU_RESETn);
  /*p01.XEBE*/ wire XEBE_RESET  = not(XORE_RESET);
  /*p01.XODO*/ wire XODO_RESET  = nand(XEBE_RESET, sys_reg.LCDC_EN);
  /*p01.XAPO*/ wire VID_RESETn  = not(XODO_RESET);
  /*p01.TOFU*/ wire VID_RESET3  = not(VID_RESETn);
  /*p01.PYRY*/ wire VID_RESET4  = not(VID_RESETn);
  /*p01.ROSY*/ wire VID_RESET5  = not(VID_RESETn);
  /*p01.ATAR*/ wire VID_RESET6  = not(VID_RESETn);
  /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);

  ResetSignals2 sig;

  /*p01.XAPO*/ sig.VID_RESETn  = VID_RESETn;
  /*p01.TOFU*/ sig.VID_RESET3  = VID_RESET3;
  /*p01.PYRY*/ sig.VID_RESET4  = VID_RESET4;
  /*p01.ROSY*/ sig.VID_RESET5  = VID_RESET5;
  /*p01.ATAR*/ sig.VID_RESET6  = VID_RESET6;
  /*p01.ABEZ*/ sig.VID_RESETn3 = VID_RESETn3;

  return sig;
}

//----------------------------------------

ResetSignals2 ResetSignals2::tick_fast(const SystemRegisters& sys_reg,
                                       const ResetSignals1& rst_sig1) {
  wire VID_RESET = rst_sig1.SYS_RESET || !sys_reg.LCDC_EN;

  ResetSignals2 sig;

  sig.VID_RESETn  = !VID_RESET;
  sig.VID_RESET3  =  VID_RESET;
  sig.VID_RESET4  =  VID_RESET;
  sig.VID_RESET5  =  VID_RESET;
  sig.VID_RESET6  =  VID_RESET;
  sig.VID_RESETn3 = !VID_RESET;

  return sig;
}

//-----------------------------------------------------------------------------

void ResetRegisters::pwron() {
  WAITING_FOR_CLKREQ = false;
  RESET_REG.val = false;
  RESET_REG.clk = false;
}

void ResetRegisters::reset() {
  WAITING_FOR_CLKREQ = true;
  RESET_REG.val = false;
  RESET_REG.clk = false;
}

void ResetRegisters::check_match(const ResetRegisters& a, const ResetRegisters& b) {
  check(a.WAITING_FOR_CLKREQ == b.WAITING_FOR_CLKREQ);
  check(a.RESET_REG.val == b.RESET_REG.val);
}

//----------------------------------------

void ResetRegisters::tock_slow(const SystemRegisters& sys_reg,
                               const ClockSignals1& clk_sig1) {
  ResetRegisters prev = *this;
  ResetRegisters& next = *this;

  /*p01.UPYF*/ bool UPYF = or(sys_reg.RST, sys_reg.CLK_BAD1);
  /*p01.TUBO*/ bool WAITING_FOR_CLKREQ2 = !UPYF ? 1 : !sys_reg.CPUCLK_REQn ? 0 : prev.WAITING_FOR_CLKREQ;
  /*p01.BOMA*/ bool RESET_CLK   = not(clk_sig1.BOGA_xBCDEFGH);
  /*p01.UNUT*/ bool TIMEOUT     = and(WAITING_FOR_CLKREQ2, sys_reg.DIV_15);
  /*p01.TABA*/ bool CPU_RESET   = or(sys_reg.MODE_DBG2, sys_reg.MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ bool CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ bool RESET_IN    = or (/*p01.AFAR*/ nor(CPU_RESETn, sys_reg.RST), sys_reg.RST);

  /*p01.TUBO*/ next.WAITING_FOR_CLKREQ = WAITING_FOR_CLKREQ2;
  /*p01.AFER*/ next.RESET_REG.tock(RESET_CLK, sys_reg.MODE_PROD, RESET_IN);
}

//----------------------------------------

void ResetRegisters::tock_fast(const SystemRegisters& sys_reg) {

  int phase = sys_reg.phaseC();
  bool BOGA_xBCDEFGH = ((phase != 0) || !sys_reg.MODE_PROD) && sys_reg.CLK_GOOD;

  if (sys_reg.CPUCLK_REQ) WAITING_FOR_CLKREQ = 0;
  if (!sys_reg.RST && !sys_reg.CLK_BAD1) WAITING_FOR_CLKREQ = 1;
  bool TIMEOUT = and(WAITING_FOR_CLKREQ, sys_reg.DIV_15);
  
  bool RESET     = TIMEOUT || sys_reg.RST || sys_reg.MODE_DBG2 || sys_reg.MODE_DBG1;

  RESET_REG.tock(!BOGA_xBCDEFGH, sys_reg.MODE_PROD, RESET);
}

//-----------------------------------------------------------------------------

#pragma warning(disable:4100)

void reset_fast(const SystemRegisters& sys_sig_b,
                const SystemRegisters& sys_reg,
                ResetRegisters& rst_reg,
                ResetSignals1& rst_sig1,
                ResetSignals2& rst_sig2) {

  bool reset_clk_b = sys_sig_b.phaseC() == 0;
  if (!sys_sig_b.MODE_PROD) reset_clk_b = 0;
  if (!sys_sig_b.CLK_GOOD)  reset_clk_b = 1;

  bool reset_clk_c = sys_reg.phaseC() == 0;
  if (!sys_reg.MODE_PROD) reset_clk_c = 0;
  if (!sys_reg.CLK_GOOD)  reset_clk_c = 1;

  if (reset_clk_b != rst_sig1.RESET_CLK) {
    printf("x");
  }

  if (sys_reg.CPUCLK_REQ) rst_reg.WAITING_FOR_CLKREQ = 0;
  if (!sys_reg.RST && !sys_reg.CLK_BAD1) rst_reg.WAITING_FOR_CLKREQ = 1;
  bool TIMEOUT = and(rst_reg.WAITING_FOR_CLKREQ, sys_reg.DIV_15);
  bool RESET = TIMEOUT || sys_reg.RST || sys_reg.MODE_DBG2 || sys_reg.MODE_DBG1;

  if (!rst_sig1.RESET_CLK && reset_clk_c) {
    rst_reg.RESET_REG.val = RESET;
  }

  rst_sig1.RESET_CLK = reset_clk_c;

  if (!sys_reg.MODE_PROD) {
    rst_reg.RESET_REG.val = 0;
  }

  RESET |= rst_reg.RESET_REG;


  rst_sig1.SYS_RESETn  = !RESET;
  rst_sig1.SYS_RESET   =  RESET;
  rst_sig1.CUNU_RESETn = !RESET;
  rst_sig1.WESY_RESET  = !RESET;
  rst_sig1.WALU_RESET  = !RESET;
  rst_sig1.XARE_RESET  = !RESET;
  rst_sig1.SOTO_RESET  = !RESET;

  wire VID_RESET = RESET || !sys_reg.LCDC_EN;

  rst_sig2.VID_RESETn  = !VID_RESET;
  rst_sig2.VID_RESET3  =  VID_RESET;
  rst_sig2.VID_RESET4  =  VID_RESET;
  rst_sig2.VID_RESET5  =  VID_RESET;
  rst_sig2.VID_RESET6  =  VID_RESET;
  rst_sig2.VID_RESETn3 = !VID_RESET;
}

//-----------------------------------------------------------------------------

};