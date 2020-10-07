#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoy;
struct GateBoyTests;

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ClockRegisters {

  void reset() {}
  void reset_bootrom() {
    UCOB_CLKBADp.reset(TRI_HZNP);

    ZAXY_xBxDxFxH.reset(TRI_HZNP);
    BUDE_xxxxEFGH.reset(TRI_HZNP);
    ATYP_ABCDxxxx.reset(TRI_HZNP);
    AROV_xxCDEFxx.reset(TRI_HZNP);
    AJAX_xxxxEFGH.reset(TRI_HZNP);
    ADAR_ABCxxxxH.reset(TRI_HZNP);
    BALY_xBCDEFGH.reset(TRI_HZNP);
    XUPY_ABxxEFxx.reset(TRI_HZNP);
    TALU_xxCDEFxx.reset(TRI_HZNP);
    XOCE_xBCxxFGx.reset(TRI_HZNP);
    WOJO_AxxxExxx.reset(TRI_HZNP);
    ALUR_SYS_RSTn.reset(TRI_HZNP);
    XAPO_VID_RSTn.reset(TRI_HZNP);

    TUBO_WAITINGp.reset(TRI_D0NP);
    ASOL_POR_DONEn.reset(TRI_D0NP);
    AFER_SYS_RSTp.reset(REG_D0C0);

    AFUR_xxxxEFGH.reset(REG_D0C0);
    ALEF_AxxxxFGH.reset(REG_D0C0);
    APUK_ABxxxxGH.reset(REG_D0C0);
    ADYK_ABCxxxxH.reset(REG_D0C0);

    WUVU_ABxxEFxx.reset(REG_D0C0);
    VENA_xxCDEFxx.reset(REG_D0C0);
    WOSU_AxxDExxH.reset(REG_D0C0);
  }

  void dump(Dumper& d, wire CLK) const;

  void tick_slow(wire CLK, wire CLKGOOD, wire CPUREADY, SchematicTop& top);
  void tock_clk_slow(wire RST, wire CLK, wire CLKGOOD, wire CPUREADY, SchematicTop& top);
  void tock_rst_slow(wire RST, wire CLKGOOD, wire CPUREADY, SchematicTop& top);
  void tock_dbg_slow(SchematicTop& top);
  void tock_vid_slow(wire CLK, SchematicTop& top);

  /* p01.UCOB*/ Sig UCOB_CLKBADp = ERR_XXXX;  // timer
  /* p01.ZAXY*/ Sig ZAXY_xBxDxFxH = ERR_XXXX; // various
  /* p01.BUDE*/ Sig BUDE_xxxxEFGH = ERR_XXXX; // uvyt, beko

  /* p01.ATYP*/ Sig ATYP_ABCDxxxx = ERR_XXXX; // afas, etc
  /* p01.AROV*/ Sig AROV_xxCDEFxx = ERR_XXXX; // ext bus
  /* p01.AJAX*/ Sig AJAX_xxxxEFGH = ERR_XXXX; // ext bus

  /*#p01.ADAR*/ Sig ADAR_ABCxxxxH = ERR_XXXX; // ext bus, oam bus
  /* p01.BALY*/ Sig BALY_xBCDEFGH = ERR_XXXX; // joypad, timer

  /* p29.XUPY*/ Sig XUPY_ABxxEFxx = ERR_XXXX; // lcd, sprite scanner
  /* p21.TALU*/ Sig TALU_xxCDEFxx = ERR_XXXX; // lcd, pix pipe (wy match should probably move to lcd)
  /* p29.XOCE*/ Sig XOCE_xBCxxFGx = ERR_XXXX; // oam bus, sprite scanner
  /* p29.WOJO*/ Sig WOJO_AxxxExxx = ERR_XXXX; // oam bus

  /* p01.ALUR*/ Sig ALUR_SYS_RSTn = ERR_XXXX; // everywhere
  /* p01.XAPO*/ Sig XAPO_VID_RSTn = ERR_XXXX; // everywhere

private:
  friend struct ::GateBoy;
  friend struct ::GateBoyTests;

  /*p01.TUBO*/ NorLatch TUBO_WAITINGp = ERR_XXXX;  // Must be 0 in run mode, otherwise we'd ping PIN_CPU_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn = ERR_XXXX; // Schematic wrong, this is a latch.
  /*p01.AFER*/ DFF13 AFER_SYS_RSTp = ERR_XXXX; // AFER should keep clocking even if PIN_CPU_CLKREQ = 0

  /*p01.AFUR*/ DFF9 AFUR_xxxxEFGH = ERR_XXXX;
  /*p01.ALEF*/ DFF9 ALEF_AxxxxFGH = ERR_XXXX;
  /*p01.APUK*/ DFF9 APUK_ABxxxxGH = ERR_XXXX;
  /*p01.ADYK*/ DFF9 ADYK_ABCxxxxH = ERR_XXXX;

  /*p29.WUVU*/ DFF17 WUVU_ABxxEFxx = ERR_XXXX;
  /*p21.VENA*/ DFF17 VENA_xxCDEFxx = ERR_XXXX;
  /*p29.WOSU*/ DFF17 WOSU_AxxDExxH = ERR_XXXX;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics