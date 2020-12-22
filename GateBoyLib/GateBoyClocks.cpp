#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyResetDebug.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyClock::tock(const GateBoyResetDebug& rst) {
  PIN75_CLK_OUT.reset_for_pass();

  wire2 ADYK_ABCxxxxHp_old = ADYK_ABCxxxxHp.qp_old2();
  wire2 AFUR_xxxxEFGHp_old = AFUR_xxxxEFGHp.qn_old2();
  wire2 ALEF_AxxxxFGHp_old = ALEF_AxxxxFGHp.qn_old2();
  wire2 APUK_ABxxxxGHp_old = APUK_ABxxxxGHp.qn_old2();

  /* p01.AFUR*/ AFUR_xxxxEFGHp.dff9(~ATAL_xBxDxFxH(), rst.UPOJ_MODE_PRODn(), ADYK_ABCxxxxHp_old);
  /* p01.ALEF*/ ALEF_AxxxxFGHp.dff9( ATAL_xBxDxFxH(), rst.UPOJ_MODE_PRODn(), AFUR_xxxxEFGHp_old);
  /* p01.APUK*/ APUK_ABxxxxGHp.dff9(~ATAL_xBxDxFxH(), rst.UPOJ_MODE_PRODn(), ALEF_AxxxxFGHp_old);
  /* p01.ADYK*/ ADYK_ABCxxxxHp.dff9( ATAL_xBxDxFxH(), rst.UPOJ_MODE_PRODn(), APUK_ABxxxxGHp_old);

  PIN75_CLK_OUT.pin_out_dp(BUDE_xxxxEFGH());
  SIG_CPU_BOWA_Axxxxxxx.set(BOWA_xBCDEFGH());
  SIG_CPU_BEDO_xBCDEFGH.set(BEDO_Axxxxxxx());
  SIG_CPU_BEKO_ABCDxxxx.set(BEKO_ABCDxxxx());
  SIG_CPU_BUDE_xxxxEFGH.set(BUDE_xxxxEFGH());
  SIG_CPU_BOLO_ABCDEFxx.set(BOLO_ABCDEFxx());
  SIG_CPU_BUKE_AxxxxxGH.set(BUKE_AxxxxxGH());
  SIG_CPU_BOMA_xBCDEFGH.set(BOMA_xBCDEFGH());
  SIG_CPU_BOGA_Axxxxxxx.set(BOGA_Axxxxxxx());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyClock::tock_vid(const GateBoyResetDebug& rst) {
  /* p29.WOSU*/ WOSU_AxxDExxH.dff17(XYFY_xBxDxFxH(),         rst.XAPO_VID_RSTn(), WUVU_ABxxEFxx.qn_old2());
  /* p29.WUVU*/ WUVU_ABxxEFxx.dff17(XOTA_AxCxExGx(),         rst.XAPO_VID_RSTn(), WUVU_ABxxEFxx.qn_old2());
  /* p21.VENA*/ VENA_xxCDEFxx.dff17(WUVU_ABxxEFxx.qn_new2(), rst.XAPO_VID_RSTn(), VENA_xxCDEFxx.qn_old2()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}

//------------------------------------------------------------------------------------------------------------------------
