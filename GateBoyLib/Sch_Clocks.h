#pragma once
#include "GateBoyLib/Cells.h"

struct GateBoy;
struct GateBoyTests;

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ClockRegisters {

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
  /* p01.BALY*/ Sig BALY_Axxxxxxx; // joypad, timer

  /* p29.XUPY*/ Sig XUPY_xxCDxxGH; // lcd, sprite scanner
  /* p21.TALU*/ Sig TALU_ABCDxxxx; // lcd, pix pipe (wy match should probably move to lcd)
  /* p29.XOCE*/ Sig XOCE_AxxDExxH; // oam bus, sprite scanner
  /* p29.WOJO*/ Sig WOJO_xxCxxxGx; // oam bus

  /* p01.ALUR*/ Sig ALUR_SYS_RSTn; // everywhere
  /* p01.XAPO*/ Sig XAPO_VID_RSTn; // everywhere

private:
  friend struct ::GateBoy;
  friend struct ::GateBoyTests;

  /*p01.TUBO*/ Tri   TUBO_WAITINGp  = TRI_D1NP; // Must be 0 in run mode, otherwise we'd ping PIN_CPU_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ Tri   ASOL_POR_DONEn = TRI_D1NP; // Schematic wrong, this is a latch.
  /*p01.AFER*/ RegQP AFER_SYS_RSTp  = REG_D0C0; // AFER should keep clocking even if PIN_CPU_CLKREQ = 0

  /*p01.AFUR*/ Reg AFUR_xxxxEFGH = REG_D0C0;
  /*p01.ALEF*/ Reg ALEF_AxxxxFGH = REG_D0C0;
  /*p01.APUK*/ Reg APUK_ABxxxxGH = REG_D0C0;
  /*p01.ADYK*/ Reg ADYK_ABCxxxxH = REG_D0C0;

  /*p29.WUVU*/ RegQN  WUVU_xxCDxxGH = REG_D0C0;
  /*p21.VENA*/ RegQN  VENA_ABCDxxxx = REG_D0C0;
  /*p29.WOSU*/ RegQPN WOSU_xBCxxFGx = REG_D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics