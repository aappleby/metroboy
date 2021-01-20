#include "GateBoyLib/GateBoyClocks.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_clocks() {
  pins.PIN_73_CLK_DRIVE.reset_for_pass();
  pins.PIN_75_CLK_OUT.reset_for_pass();

  /*PIN_73*/ pins.PIN_73_CLK_DRIVE.pin_out_dp(pins.PIN_74_CLK.clock());

  /* p01.ARYS*/ wire ARYS = not1(pins.PIN_74_CLK.clock());
  /* p01.AVET*/ clk.AVET = nand2(clk.ANOS, ARYS);
  /* p01.ANOS*/ clk.ANOS = nand2(pins.PIN_74_CLK.clock(), clk.AVET);
  /* p01.AVET*/ clk.AVET = nand2(clk.ANOS, ARYS);
  /* p01.ANOS*/ clk.ANOS = nand2(pins.PIN_74_CLK.clock(), clk.AVET);

  /* p01.ATAL*/ wire _ATAL_xBxDxFxH = not1(clk.AVET);
  /* p01.ATAN*/ wire _ATAN_AxCxExGx = not1(_ATAL_xBxDxFxH); // cell not marked on die but it's next to ATAL

  DFF9 _ADYK_ABCxxxxHp_old = clk.ADYK_ABCxxxxHp;
  DFF9 _AFUR_xxxxEFGHp_old = clk.AFUR_xxxxEFGHp;
  DFF9 _ALEF_AxxxxFGHp_old = clk.ALEF_AxxxxFGHp;
  DFF9 _APUK_ABxxxxGHp_old = clk.APUK_ABxxxxGHp;

  /* p01.AFUR*/ clk.AFUR_xxxxEFGHp.dff9(_ATAN_AxCxExGx, rst.UPOJ_MODE_PRODn(), _ADYK_ABCxxxxHp_old.qp_old());
  /* p01.ALEF*/ clk.ALEF_AxxxxFGHp.dff9(_ATAL_xBxDxFxH, rst.UPOJ_MODE_PRODn(), _AFUR_xxxxEFGHp_old.qn_old());
  /* p01.APUK*/ clk.APUK_ABxxxxGHp.dff9(_ATAN_AxCxExGx, rst.UPOJ_MODE_PRODn(), _ALEF_AxxxxFGHp_old.qn_old());
  /* p01.ADYK*/ clk.ADYK_ABCxxxxHp.dff9(_ATAL_xBxDxFxH, rst.UPOJ_MODE_PRODn(), _APUK_ABxxxxGHp_old.qn_old());

  /*PIN_75*/ pins.PIN_75_CLK_OUT.pin_out_dp(BUDE_xxxxEFGH());
  /*SIG_CPU_BOWA_Axxxxxxx*/ clk.SIG_CPU_BOWA_Axxxxxxx.sig_out(BOWA_xBCDEFGH());
  /*SIG_CPU_BEDO_xBCDEFGH*/ clk.SIG_CPU_BEDO_xBCDEFGH.sig_out(BEDO_Axxxxxxx());
  /*SIG_CPU_BEKO_ABCDxxxx*/ clk.SIG_CPU_BEKO_ABCDxxxx.sig_out(BEKO_ABCDxxxx());
  /*SIG_CPU_BUDE_xxxxEFGH*/ clk.SIG_CPU_BUDE_xxxxEFGH.sig_out(BUDE_xxxxEFGH());
  /*SIG_CPU_BOLO_ABCDEFxx*/ clk.SIG_CPU_BOLO_ABCDEFxx.sig_out(BOLO_ABCDEFxx());
  /*SIG_CPU_BUKE_AxxxxxGH*/ clk.SIG_CPU_BUKE_AxxxxxGH.sig_out(BUKE_AxxxxxGH());
  /*SIG_CPU_BOMA_xBCDEFGH*/ clk.SIG_CPU_BOMA_xBCDEFGH.sig_out(BOMA_xBCDEFGH());
  /*SIG_CPU_BOGA_Axxxxxxx*/ clk.SIG_CPU_BOGA_Axxxxxxx.sig_out(BOGA_Axxxxxxx());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_vid_clocks() {
  /* p29.WOSU*/ clk.WOSU_AxxDExxH.dff17(XYFY_xBxDxFxH(),            rst.XAPO_VID_RSTn(), clk.WUVU_ABxxEFxx.qn_old());
  /* p29.WUVU*/ clk.WUVU_ABxxEFxx.dff17(XOTA_AxCxExGx(),            rst.XAPO_VID_RSTn(), clk.WUVU_ABxxEFxx.qn_old());
  /* p21.VENA*/ clk.VENA_xxCDEFxx.dff17(clk.WUVU_ABxxEFxx.qn_new(), rst.XAPO_VID_RSTn(), clk.VENA_xxCDEFxx.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}

//------------------------------------------------------------------------------------------------------------------------
