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
  /* p01.AROV*/ Sig AROV_xxCDEFxx; // ext bus
  /* p01.ATYP*/ Sig ATYP_ABCDxxxx; // afas, etc
  /* p01.AJAX*/ Sig AJAX_xxxxEFGH; // ext bus
  /*#p01.ADAR*/ Sig ADAR_ABCxxxxH;
  /* p01.BALY*/ Sig BALY_Axxxxxxx;

  /* p29.XUPY*/ Sig XUPY_xxCDxxGH;
  /* p21.TALU*/ Sig TALU_ABCDxxxx;
  /* p29.XOCE*/ Sig XOCE_AxxDExxH;
  /* p29.WOJO*/ Sig WOJO_xxCxxxGx;

  /* p01.ALUR*/ Sig ALUR_SYS_RSTn;
  /* p01.XAPO*/ Sig XAPO_VID_RSTn;

private:
  friend struct ::GateBoy;
  friend struct ::GateBoyTests;

  /*p01.TUBO*/ Tri TUBO_WAITINGp  = TRI_D1NP; // Must be 0 in run mode, otherwise we'd ping CPU_PIN_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ Tri _ASOL_POR_DONEn = TRI_D1NP; // Schematic wrong, this is a latch.
  /*p01.AFER*/ RegQP _AFER_SYS_RSTp  = REG_D0C0; // AFER should keep clocking even if CPU_PIN_CLKREQ = 0

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