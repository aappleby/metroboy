#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct ClockRegisters {
  void reset_cart() {
    TUBO_WAITINGp_s.reset(0);
    ASOL_POR_DONEn_s.reset(0);
    AFER_SYS_RSTp_s.reset(REG_D0C1);
    AFUR_xxxxEFGHp_s.reset(REG_D0C1);
    ALEF_AxxxxFGHp_s.reset(REG_D1C0);
    APUK_ABxxxxGHp_s.reset(REG_D1C1);
    ADYK_ABCxxxxHp_s.reset(REG_D1C0);
    WUVU_ABxxEFxxp.reset(REG_D1C1);
    VENA_xxCDEFxxp.reset(REG_D0C0);
    WOSU_AxxDExxHp.reset(REG_D1C0);
  }

  void reset_boot() {
    TUBO_WAITINGp_s.reset(0);
    ASOL_POR_DONEn_s.reset(0);
    AFER_SYS_RSTp_s.reset(REG_D0C0);
    AFUR_xxxxEFGHp_s.reset(REG_D0C0);
    ALEF_AxxxxFGHp_s.reset(REG_D0C0);
    APUK_ABxxxxGHp_s.reset(REG_D0C0);
    ADYK_ABCxxxxHp_s.reset(REG_D0C0);
    WUVU_ABxxEFxxp.reset(REG_D0C0);
    VENA_xxCDEFxxp.reset(REG_D0C0);
    WOSU_AxxDExxHp.reset(REG_D0C0);
  }

  /*p01.TUBO*/ NorLatch TUBO_WAITINGp_s;  // Must be 0 in run mode, otherwise we'd ping PIN_CPU_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn_s; // Schematic wrong, this is a latch.
  /*p01.AFER*/ DFF13 AFER_SYS_RSTp_s; // AFER should keep clocking even if PIN_CPU_CLKREQ = 0

  /*p01.AFUR*/ DFF9 AFUR_xxxxEFGHp_s;
  /*p01.ALEF*/ DFF9 ALEF_AxxxxFGHp_s;
  /*p01.APUK*/ DFF9 APUK_ABxxxxGHp_s;
  /*p01.ADYK*/ DFF9 ADYK_ABCxxxxHp_s;

  /*p29.WUVU*/ DFF17 WUVU_ABxxEFxxp;
  /*p21.VENA*/ DFF17 VENA_xxCDEFxxp;
  /*p29.WOSU*/ DFF17 WOSU_AxxDExxHp;
};

//-----------------------------------------------------------------------------
