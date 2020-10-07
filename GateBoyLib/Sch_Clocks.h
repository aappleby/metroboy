#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoy;
struct GateBoyTests;

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ClockRegisters {

  void reset() {}
  void reset_bootrom() {}
  void dump(Dumper& d, wire CLK) const;

  void tick_slow(wire CLK, wire CLKGOOD, wire CPUREADY, SchematicTop& top);
  void tock_clk_slow(wire RST, wire CLK, wire CLKGOOD, wire CPUREADY, SchematicTop& top);
  void tock_rst_slow(wire RST, wire CLKGOOD, wire CPUREADY, SchematicTop& top);
  void tock_dbg_slow(SchematicTop& top);
  void tock_vid_slow(wire CLK, SchematicTop& top);

  /* p01.UCOB*/ Sig UCOB_CLKBADp;  // timer
  /* p01.ZAXY*/ Sig ZAXY_xBxDxFxH; // various
  /* p01.BUDE*/ Sig BUDE_xxxxEFGH; // uvyt, beko

  /* p01.ATYP*/ Sig ATYP_ABCDxxxx; // afas, etc
  /* p01.AROV*/ Sig AROV_xxCDEFxx; // ext bus
  /* p01.AJAX*/ Sig AJAX_xxxxEFGH; // ext bus

  /*#p01.ADAR*/ Sig ADAR_ABCxxxxH; // ext bus, oam bus
  /* p01.BALY*/ Sig BALY_xBCDEFGH; // joypad, timer

  /* p29.XUPY*/ Sig XUPY_ABxxEFxx; // lcd, sprite scanner
  /* p21.TALU*/ Sig TALU_xxCDEFxx; // lcd, pix pipe (wy match should probably move to lcd)
  /* p29.XOCE*/ Sig XOCE_xBCxxFGx; // oam bus, sprite scanner
  /* p29.WOJO*/ Sig WOJO_AxxxExxx; // oam bus

  /* p01.ALUR*/ Sig ALUR_SYS_RSTn; // everywhere
  /* p01.XAPO*/ Sig XAPO_VID_RSTn; // everywhere

private:
  friend struct ::GateBoy;
  friend struct ::GateBoyTests;

  /*p01.TUBO*/ NorLatch TUBO_WAITINGp;  // Must be 0 in run mode, otherwise we'd ping PIN_CPU_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ DFF13 AFER_SYS_RSTp; // AFER should keep clocking even if PIN_CPU_CLKREQ = 0

  /*p01.AFUR*/ DFF9 AFUR_xxxxEFGH;
  /*p01.ALEF*/ DFF9 ALEF_AxxxxFGH;
  /*p01.APUK*/ DFF9 APUK_ABxxxxGH;
  /*p01.ADYK*/ DFF9 ADYK_ABCxxxxH;

  /*p29.WUVU*/ DFF17 WUVU_ABxxEFxx;
  /*p21.VENA*/ DFF17 VENA_xxCDEFxx;
  /*p29.WOSU*/ DFF17 WOSU_AxxDExxH;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics