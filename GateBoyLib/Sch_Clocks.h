#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct ResetRegisters {
  /*p01.TUBO*/ NorLatch TUBO_WAITINGp;  // Must be 0 in run mode, otherwise we'd ping PIN_CPU_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ DFF13 AFER_SYS_RSTp_evn;     // AFER should keep clocking even if PIN_CPU_CLKREQ = 0
};

struct PhaseClockRegisters {
  /*p01.AFUR*/ DFF9 AFUR_xxxxEFGHp;
  /*p01.ALEF*/ DFF9 ALEF_AxxxxFGHp;
  /*p01.APUK*/ DFF9 APUK_ABxxxxGHp;
  /*p01.ADYK*/ DFF9 ADYK_ABCxxxxHp;
};

struct VideoClockRegisters {
  /*p29.WUVU*/ DFF17 WUVU_ABxxEFxxp;
  /*p21.VENA*/ DFF17 VENA_xxCDEFxxp;
  /*p29.WOSU*/ DFF17 WOSU_AxxDExxHp;
};

struct DebugRegisters {
  /*p25.SOTO*/ DFF17 SOTO_DBG_VRAMp;
};

//-----------------------------------------------------------------------------
