#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoy;
struct GateBoyTests;

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ClockRegisters {
  void dump(Dumper& d) const {
    d("\002===== Clocks =====\001\n");
    d("PHASE %c%c%c%c\n", AFUR_xxxxEFGH.c(), ALEF_AxxxxFGH.c(), APUK_ABxxxxGH.c(), ADYK_ABCxxxxH.c());
    d("\n");
    d("AFUR_xxxxEFGH %c\n", AFUR_xxxxEFGH.c());
    d("ALEF_AxxxxFGH %c\n", ALEF_AxxxxFGH.c());
    d("APUK_ABxxxxGH %c\n", APUK_ABxxxxGH.c());
    d("ADYK_ABCxxxxH %c\n", ADYK_ABCxxxxH.c());
    d("VENA_ABCDxxxx %c\n", VENA_xxCDEFxx.c());
    d("WOSU_xBCxxFGx %c\n", WOSU_AxxDExxH.c());
    d("WUVU_xxCDxxGH %c\n", WUVU_ABxxEFxx.c());
    d("\n");

    d("\002===== Reset =====\001\n");
    d("TUBO_WAITINGp  %c\n", TUBO_WAITINGp.c());
    d("ASOL_POR_DONEn %c\n", ASOL_POR_DONEn.c());
    d("AFER_SYS_RSTp  %c\n", AFER_SYS_RSTp.c());
    d("\n");
  }

  void reset_cart() {
    TUBO_WAITINGp.reset(TRI_D0NP);
    ASOL_POR_DONEn.reset(TRI_D0NP);
    AFER_SYS_RSTp.reset(REG_D0C1);
    AFUR_xxxxEFGH.reset(REG_D0C1);
    ALEF_AxxxxFGH.reset(REG_D1C0);
    APUK_ABxxxxGH.reset(REG_D1C1);
    ADYK_ABCxxxxH.reset(REG_D1C0);
    WUVU_ABxxEFxx.reset(REG_D1C1);
    VENA_xxCDEFxx.reset(REG_D0C0);
    WOSU_AxxDExxH.reset(REG_D1C0);
  }

  void reset_boot() {
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

  void tock_clk_slow(SchematicTop& top, wire CLKGOOD);
  void tock_rst_slow(SchematicTop& top, wire RST, wire CLKGOOD, wire CPUREADY);
  void tock_dbg_slow(SchematicTop& top);
  void tock_vid_slow(SchematicTop& top, wire CLK);

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