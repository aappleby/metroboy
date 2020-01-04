#include "Sch_Resets.h"

#include "Sch_Debug.h"
#include "Sch_Pins.h"
#include "Sch_Clocks.h"
#include "Sch_System.h"

#pragma warning(disable:4458)

namespace Schematics {

//-----------------------------------------------------------------------------

void RstRegisters::pwron() {
  WAITING_FOR_CLKREQ = false;
  RESET_REG.pwron();
  changed = false;
}

void RstRegisters::reset() {
  WAITING_FOR_CLKREQ = true;
  RESET_REG.reset(0, 0);
  changed = false;
}

void RstRegisters::phase_begin() {
}

void RstRegisters::phase_end() {
}

void RstRegisters::pass_begin() {
}

bool RstRegisters::pass_end() {
  changed |= RESET_REG.commit();

  bool ret = changed;
  changed = false;
  return ret;
}

//-----------------------------------------------------------------------------

RstSignals RstRegisters::tick_slow(const SysSignals& sys_sig, const ClkSignals& clk_sig) {
  /*p01.UPYF*/ wire UPYF = or(sys_sig.PIN_RST, sys_sig.UCOB_CLKBAD);

  /*p01.TUBO*/ wire WAITING_FOR_CLKREQ2 = !UPYF ? 1 : !sys_sig.ABOL_CLKREQn ? 0 : WAITING_FOR_CLKREQ;


  /*p01.BOMA*/ wire RESET_CLK   = not(clk_sig.BOGA_xBCDEFGH);
  /*p01.UNUT*/ wire TIMEOUT     = and(WAITING_FOR_CLKREQ2, sys_sig.DIV_15);
  /*p01.TABA*/ wire CPU_RESET   = or(sys_sig.PIN_MODE_DBG2, sys_sig.PIN_MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ wire CPU_RESETn  = not(CPU_RESET);
  /*p01.ASOL*/ wire RESET_IN    = or (/*p01.AFAR*/ nor(CPU_RESETn, sys_sig.PIN_RST), sys_sig.PIN_RST);

  if (WAITING_FOR_CLKREQ != WAITING_FOR_CLKREQ2) {
    changed = true;
  }

  /*p01.TUBO*/ WAITING_FOR_CLKREQ = WAITING_FOR_CLKREQ2;
  /*p01.AFER*/ RESET_REG.set(RESET_CLK, sys_sig.MODE_PROD, RESET_IN);

  return rst_signals(sys_sig, clk_sig);
}

//-----------------------------------------------------------------------------

RstSignals RstRegisters::rst_signals(const SysSignals& sys_sig, const ClkSignals& clk_sig) {


  /*p01.UPYF*/ wire UPYF = or(sys_sig.PIN_RST, sys_sig.UCOB_CLKBAD);
  /*p01.TUBO*/ wire WAITING_FOR_CLKREQ2 = !UPYF ? 1 : !sys_sig.ABOL_CLKREQn ? 0 : WAITING_FOR_CLKREQ;

  /*p01.UNUT*/ wire TIMEOUT     = and(WAITING_FOR_CLKREQ2, sys_sig.DIV_15);
  /*p01.TABA*/ wire CPU_RESET   = or(sys_sig.PIN_MODE_DBG2, sys_sig.PIN_MODE_DBG1, TIMEOUT);
  /*p01.ALYP*/ wire CPU_RESETn  = not(CPU_RESET);
  /*p01.AFAR*/ wire AFAR        = nor(CPU_RESETn, sys_sig.PIN_RST);
  /*p01.ASOL*/ wire RESET_IN    = or (AFAR, sys_sig.PIN_RST);

  /*p01.BOMA*/ wire RESET_CLK   = not(clk_sig.BOGA_xBCDEFGH);

  /*p01.AVOR*/ wire AVOR_RESET  = or(RESET_REG, RESET_IN);
  /*p01.ALUR*/ wire SYS_RESETn  = not(AVOR_RESET);   // this goes all over the place
  /*p01.DULA*/ wire SYS_RESET   = not(SYS_RESETn);
  /*p01.CUNU*/ wire CUNU_RESETn = not(SYS_RESET);
  /*p01.XORE*/ wire XORE_RESET  = not(CUNU_RESETn);
  /*p01.WESY*/ wire WESY_RESET  = not(XORE_RESET);
  /*p01.WALU*/ wire WALU_RESET  = not(XORE_RESET);
  /*p01.XARE*/ wire XARE_RESET  = not(XORE_RESET);

  RstSignals sig;

  /*p01.BOMA*/ sig.RESET_CLK   = RESET_CLK;
  /*p01.ALUR*/ sig.SYS_RESETn  = SYS_RESETn;
  /*p01.DULA*/ sig.SYS_RESET   = SYS_RESET;
  /*p01.CUNU*/ sig.CUNU_RESETn = CUNU_RESETn;
  /*p01.WESY*/ sig.WESY_RESET  = WESY_RESET;
  /*p01.WALU*/ sig.WALU_RESET  = WALU_RESET;
  /*p01.XARE*/ sig.XARE_RESET  = XARE_RESET;

  return sig;
}

//-----------------------------------------------------------------------------

VrstSignals RstRegisters::vrst_signals(const SysSignals& sys_sig, const RstSignals& rst_sig) {
  /*p01.XORE*/ wire XORE_RESET  = not(rst_sig.CUNU_RESETn);
  /*p01.XEBE*/ wire XEBE_RESET  = not(XORE_RESET);
  /*p01.XODO*/ wire XODO_RESET  = nand(XEBE_RESET, sys_sig.LCDC_EN);
  /*p01.XAPO*/ wire VID_RESETn  = not(XODO_RESET);
  /*p01.TOFU*/ wire VID_RESET3  = not(VID_RESETn);
  /*p01.PYRY*/ wire VID_RESET4  = not(VID_RESETn);
  /*p01.ROSY*/ wire VID_RESET5  = not(VID_RESETn);
  /*p01.ATAR*/ wire VID_RESET6  = not(VID_RESETn);
  /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);

  VrstSignals sig;

  /*p01.XAPO*/ sig.VID_RESETn  = VID_RESETn;
  /*p01.TOFU*/ sig.VID_RESET3  = VID_RESET3;
  /*p01.PYRY*/ sig.VID_RESET4  = VID_RESET4;
  /*p01.ROSY*/ sig.VID_RESET5  = VID_RESET5;
  /*p01.ATAR*/ sig.VID_RESET6  = VID_RESET6;
  /*p01.ABEZ*/ sig.VID_RESETn3 = VID_RESETn3;

  return sig;
}

//-----------------------------------------------------------------------------

};