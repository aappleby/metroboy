#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyResetDebug.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyClock::tock(const GateBoyResetDebug& rst) {

  wire ADYK_ABCxxxxHp_old = ADYK_ABCxxxxHp.qp_old();
  wire AFUR_xxxxEFGHp_old = AFUR_xxxxEFGHp.qn_old();
  wire ALEF_AxxxxFGHp_old = ALEF_AxxxxFGHp.qn_old();
  wire APUK_ABxxxxGHp_old = APUK_ABxxxxGHp.qn_old();

  /* p01.AFUR*/ AFUR_xxxxEFGHp.dff9(!ATAL_xBxDxFxH(), rst.UPOJ_MODE_PRODn(), ADYK_ABCxxxxHp_old);
  /* p01.ALEF*/ ALEF_AxxxxFGHp.dff9( ATAL_xBxDxFxH(), rst.UPOJ_MODE_PRODn(), AFUR_xxxxEFGHp_old);
  /* p01.APUK*/ APUK_ABxxxxGHp.dff9(!ATAL_xBxDxFxH(), rst.UPOJ_MODE_PRODn(), ALEF_AxxxxFGHp_old);
  /* p01.ADYK*/ ADYK_ABCxxxxHp.dff9( ATAL_xBxDxFxH(), rst.UPOJ_MODE_PRODn(), APUK_ABxxxxGHp_old);

  PIN75_CLK_OUT.pin_out_dp(BUDE_xxxxEFGH());
  SIG_CPU_BOWA_Axxxxxxx.set_new(BOWA_xBCDEFGH());
  SIG_CPU_BEDO_xBCDEFGH.set_new(BEDO_Axxxxxxx());
  SIG_CPU_BEKO_ABCDxxxx.set_new(BEKO_ABCDxxxx());
  SIG_CPU_BUDE_xxxxEFGH.set_new(BUDE_xxxxEFGH());
  SIG_CPU_BOLO_ABCDEFxx.set_new(BOLO_ABCDEFxx());
  SIG_CPU_BUKE_AxxxxxGH.set_new(BUKE_AxxxxxGH());
  SIG_CPU_BOMA_xBCDEFGH.set_new(BOMA_xBCDEFGH());
  SIG_CPU_BOGA_Axxxxxxx.set_new(BOGA_Axxxxxxx());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyClock::tock_vid(const GateBoyResetDebug& rst) {
  /* p29.WOSU*/ WOSU_AxxDExxH.dff17(XYFY_xBxDxFxH(),        rst.XAPO_VID_RSTn(), WUVU_ABxxEFxx.qn_old());
  /* p29.WUVU*/ WUVU_ABxxEFxx.dff17(XOTA_AxCxExGx(),        rst.XAPO_VID_RSTn(), WUVU_ABxxEFxx.qn_old());
  /* p21.VENA*/ VENA_xxCDEFxx.dff17(WUVU_ABxxEFxx.qn_new(), rst.XAPO_VID_RSTn(), VENA_xxCDEFxx.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}

//------------------------------------------------------------------------------------------------------------------------
