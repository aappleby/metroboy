#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct ClockRegisters {
  void reset_cart() {
    TUBO_WAITINGp_t0.reset(0);
    ASOL_POR_DONEn.reset(0);
    AFER_SYS_RSTp.reset(REG_D0C1);
    AFUR_xxxxEFGH_t1.reset(REG_D0C1);
    ALEF_AxxxxFGH_t1.reset(REG_D1C0);
    APUK_ABxxxxGH_t1.reset(REG_D1C1);
    ADYK_ABCxxxxH_t1.reset(REG_D1C0);
    WUVU_ABxxEFxx_t1.reset(REG_D1C1);
    VENA_xxCDEFxx_t1.reset(REG_D0C0);
    WOSU_AxxDExxH_t1.reset(REG_D1C0);
  }

  void reset_boot() {
    TUBO_WAITINGp_t0.reset(0);
    ASOL_POR_DONEn.reset(0);
    AFER_SYS_RSTp.reset(REG_D0C0);
    AFUR_xxxxEFGH_t1.reset(REG_D0C0);
    ALEF_AxxxxFGH_t1.reset(REG_D0C0);
    APUK_ABxxxxGH_t1.reset(REG_D0C0);
    ADYK_ABCxxxxH_t1.reset(REG_D0C0);
    WUVU_ABxxEFxx_t1.reset(REG_D0C0);
    VENA_xxCDEFxx_t1.reset(REG_D0C0);
    WOSU_AxxDExxH_t1.reset(REG_D0C0);
  }

  /*p01.TUBO*/ NorLatch TUBO_WAITINGp_t0;  // Must be 0 in run mode, otherwise we'd ping PIN_CPU_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ DFF13 AFER_SYS_RSTp; // AFER should keep clocking even if PIN_CPU_CLKREQ = 0

  /*p01.AFUR*/ DFF9 AFUR_xxxxEFGH_t1;
  /*p01.ALEF*/ DFF9 ALEF_AxxxxFGH_t1;
  /*p01.APUK*/ DFF9 APUK_ABxxxxGH_t1;
  /*p01.ADYK*/ DFF9 ADYK_ABCxxxxH_t1;

  /*p29.WUVU*/ DFF17 WUVU_ABxxEFxx_t1;
  /*p21.VENA*/ DFF17 VENA_xxCDEFxx_t1;
  /*p29.WOSU*/ DFF17 WOSU_AxxDExxH_t1;
};

//-----------------------------------------------------------------------------
