#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyResetDebug.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyClock::tock(const GateBoyResetDebug& rst) {
  PIN_73_CLK_DRIVE.reset_for_pass();
  PIN_75_CLK_OUT.reset_for_pass();

  /*PIN_73*/ PIN_73_CLK_DRIVE.pin_out_dp(PIN_74_CLK.ck());

  /* p01.ARYS*/ wire ARYS = not1(PIN_74_CLK.ck());
  /* p01.AVET*/ AVET = nand2(ANOS, ARYS);
  /* p01.ANOS*/ ANOS = nand2(PIN_74_CLK.ck(), AVET);
  /* p01.AVET*/ AVET = nand2(ANOS, ARYS);
  /* p01.ANOS*/ ANOS = nand2(PIN_74_CLK.ck(), AVET);

  /* p01.ATAL*/ wire _ATAL_xBxDxFxH = not1(AVET);
  /* p01.ATAN*/ wire _ATAN_AxCxExGx = not1(_ATAL_xBxDxFxH); // cell not marked on die but it's next to ATAL

  DFF9 _ADYK_ABCxxxxHp_old = ADYK_ABCxxxxHp;
  DFF9 _AFUR_xxxxEFGHp_old = AFUR_xxxxEFGHp;
  DFF9 _ALEF_AxxxxFGHp_old = ALEF_AxxxxFGHp;
  DFF9 _APUK_ABxxxxGHp_old = APUK_ABxxxxGHp;

  /* p01.AFUR*/ AFUR_xxxxEFGHp.dff9(_ATAN_AxCxExGx, rst.UPOJ_MODE_PRODn(), _ADYK_ABCxxxxHp_old.qp_old());
  /* p01.ALEF*/ ALEF_AxxxxFGHp.dff9(_ATAL_xBxDxFxH, rst.UPOJ_MODE_PRODn(), _AFUR_xxxxEFGHp_old.qn_old());
  /* p01.APUK*/ APUK_ABxxxxGHp.dff9(_ATAN_AxCxExGx, rst.UPOJ_MODE_PRODn(), _ALEF_AxxxxFGHp_old.qn_old());
  /* p01.ADYK*/ ADYK_ABCxxxxHp.dff9(_ATAL_xBxDxFxH, rst.UPOJ_MODE_PRODn(), _APUK_ABxxxxGHp_old.qn_old());

  /*PIN_75*/ PIN_75_CLK_OUT.pin_out_dp(BUDE_xxxxEFGH());
  /*SIG_CPU_BOWA_Axxxxxxx*/ SIG_CPU_BOWA_Axxxxxxx.sig_out(BOWA_xBCDEFGH());
  /*SIG_CPU_BEDO_xBCDEFGH*/ SIG_CPU_BEDO_xBCDEFGH.sig_out(BEDO_Axxxxxxx());
  /*SIG_CPU_BEKO_ABCDxxxx*/ SIG_CPU_BEKO_ABCDxxxx.sig_out(BEKO_ABCDxxxx());
  /*SIG_CPU_BUDE_xxxxEFGH*/ SIG_CPU_BUDE_xxxxEFGH.sig_out(BUDE_xxxxEFGH());
  /*SIG_CPU_BOLO_ABCDEFxx*/ SIG_CPU_BOLO_ABCDEFxx.sig_out(BOLO_ABCDEFxx());
  /*SIG_CPU_BUKE_AxxxxxGH*/ SIG_CPU_BUKE_AxxxxxGH.sig_out(BUKE_AxxxxxGH());
  /*SIG_CPU_BOMA_xBCDEFGH*/ SIG_CPU_BOMA_xBCDEFGH.sig_out(BOMA_xBCDEFGH());
  /*SIG_CPU_BOGA_Axxxxxxx*/ SIG_CPU_BOGA_Axxxxxxx.sig_out(BOGA_Axxxxxxx());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyClock::tock_vid(const GateBoyResetDebug& rst) {
  /* p29.WOSU*/ WOSU_AxxDExxH.dff17(XYFY_xBxDxFxH(),         rst.XAPO_VID_RSTn(), WUVU_ABxxEFxx.qn_old());
  /* p29.WUVU*/ WUVU_ABxxEFxx.dff17(XOTA_AxCxExGx(),         rst.XAPO_VID_RSTn(), WUVU_ABxxEFxx.qn_old());
  /* p21.VENA*/ VENA_xxCDEFxx.dff17(WUVU_ABxxEFxx.qn_new(), rst.XAPO_VID_RSTn(), VENA_xxCDEFxx.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}

//------------------------------------------------------------------------------------------------------------------------
