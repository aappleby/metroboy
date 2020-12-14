#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyResetDebug.h"

void GateBoyPhaseClock::tock(const GateBoyResetDebug& rstdbg) {
  /* p01.AFUR*/ AFUR_xxxxEFGHp.dff9(!ATAL_xBxDxFxH(), rstdbg.UPOJ_MODE_PRODn_ext(), ADYK_ABCxxxxHp.qp_old());
  /* p01.ALEF*/ ALEF_AxxxxFGHp.dff9( ATAL_xBxDxFxH(), rstdbg.UPOJ_MODE_PRODn_ext(), AFUR_xxxxEFGHp.qn_old());
  /* p01.APUK*/ APUK_ABxxxxGHp.dff9(!ATAL_xBxDxFxH(), rstdbg.UPOJ_MODE_PRODn_ext(), ALEF_AxxxxFGHp.qn_old());
  /* p01.ADYK*/ ADYK_ABCxxxxHp.dff9( ATAL_xBxDxFxH(), rstdbg.UPOJ_MODE_PRODn_ext(), APUK_ABxxxxGHp.qn_old());

  PIN_EXT_CLK.pin_out(BUDE_xxxxEFGH(), BUDE_xxxxEFGH());
  PIN_CPU_BOWA_Axxxxxxx.setp(BOWA_xBCDEFGH());
  PIN_CPU_BEDO_xBCDEFGH.setp(BEDO_Axxxxxxx());
  PIN_CPU_BEKO_ABCDxxxx.setp(BEKO_ABCDxxxx());
  PIN_CPU_BUDE_xxxxEFGH.setp(BUDE_xxxxEFGH());
  PIN_CPU_BOLO_ABCDEFxx.setp(BOLO_ABCDEFxx());
  PIN_CPU_BUKE_AxxxxxGH.setp(BUKE_AxxxxxGH());
  PIN_CPU_BOMA_xBCDEFGH.setp(BOMA_xBCDEFGH());
  PIN_CPU_BOGA_Axxxxxxx.setp(BOGA_Axxxxxxx());
}

void GateBoyVideoClock::tock(const GateBoyResetDebug& rstdbg, const GateBoyPhaseClock& pclk) {
  /* p29.WOSU*/ WOSU_AxxDExxH.dff17(pclk.XYFY_xBxDxFxH(),   rstdbg.XAPO_VID_RSTn(), WUVU_ABxxEFxx.qn_any());
  /* p29.WUVU*/ WUVU_ABxxEFxx.dff17(pclk.XOTA_AxCxExGx(),   rstdbg.XAPO_VID_RSTn(), WUVU_ABxxEFxx.qn_any());
  /* p21.VENA*/ VENA_xxCDEFxx.dff17(WUVU_ABxxEFxx.qn_new(), rstdbg.XAPO_VID_RSTn(), VENA_xxCDEFxx.qn_any()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}