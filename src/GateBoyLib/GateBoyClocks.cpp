#include "GateBoyLib/GateBoyClocks.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_clocks_gates() {
  /*_PIN_73*/ clk.PIN_73_CLK_DRIVE.pin_out(clk.PIN_74_CLK.clk(), clk.PIN_74_CLK.clk());

  /*_p01.ARYS*/ wire ARYS_CLKIN = not1(clk.PIN_74_CLK.clk());
  /*_p01.AVET*/ clk.AVET_DEGLITCH = nand2(clk.ANOS_DEGLITCH.out_mid(), ARYS_CLKIN);
  /*_p01.ANOS*/ clk.ANOS_DEGLITCH = nand2(clk.PIN_74_CLK.clk(), clk.AVET_DEGLITCH.out_mid());
  /*_p01.AVET*/ clk.AVET_DEGLITCH = nand2(clk.ANOS_DEGLITCH.out_mid(), ARYS_CLKIN);
  /*_p01.ANOS*/ clk.ANOS_DEGLITCH = nand2(clk.PIN_74_CLK.clk(), clk.AVET_DEGLITCH.out_mid());

  /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(clk.AVET_DEGLITCH.out_new());
  /*_p01.ATAN*/ wire ATAN_AxCxExGx = not1(ATAL_xBxDxFxH); // cell not marked on die but it's next to ATAL

  DFF9 ADYK_ABCxxxxH_old = clk.ADYK_ABCxxxxH;
  DFF9 AFUR_xxxxEFGH_old = clk.AFUR_xxxxEFGH;
  DFF9 ALEF_AxxxxFGH_old = clk.ALEF_AxxxxFGH;
  DFF9 APUK_ABxxxxGH_old = clk.APUK_ABxxxxGH;

  /*_p01.AFUR*/ clk.AFUR_xxxxEFGH.dff9(ATAN_AxCxExGx, UPOJ_MODE_PRODn(), ADYK_ABCxxxxH_old.qp_old());
  /*_p01.ALEF*/ clk.ALEF_AxxxxFGH.dff9(ATAL_xBxDxFxH, UPOJ_MODE_PRODn(), AFUR_xxxxEFGH_old.qn_old());
  /*_p01.APUK*/ clk.APUK_ABxxxxGH.dff9(ATAN_AxCxExGx, UPOJ_MODE_PRODn(), ALEF_AxxxxFGH_old.qn_old());
  /*_p01.ADYK*/ clk.ADYK_ABCxxxxH.dff9(ATAL_xBxDxFxH, UPOJ_MODE_PRODn(), APUK_ABxxxxGH_old.qn_old());

  /*_PIN_75*/ clk.PIN_75_CLK_OUT.pin_out(BUDE_xxxxEFGH(), BUDE_xxxxEFGH());

  /*_SIG_CPU_BOWA_Axxxxxxx*/ clk.SIG_CPU_BOWA_Axxxxxxx.sig_out(BOWA_xBCDEFGH());
  /*_SIG_CPU_BEDO_xBCDEFGH*/ clk.SIG_CPU_BEDO_xBCDEFGH.sig_out(BEDO_Axxxxxxx());
  /*_SIG_CPU_BEKO_ABCDxxxx*/ clk.SIG_CPU_BEKO_ABCDxxxx.sig_out(BEKO_ABCDxxxx());
  /*_SIG_CPU_BUDE_xxxxEFGH*/ clk.SIG_CPU_BUDE_xxxxEFGH.sig_out(BUDE_xxxxEFGH());
  /*_SIG_CPU_BOLO_ABCDEFxx*/ clk.SIG_CPU_BOLO_ABCDEFxx.sig_out(BOLO_ABCDEFxx());
  /*_SIG_CPU_BUKE_AxxxxxGH*/ clk.SIG_CPU_BUKE_AxxxxxGH.sig_out(BUKE_AxxxxxGH());
  /*_SIG_CPU_BOMA_xBCDEFGH*/ clk.SIG_CPU_BOMA_xBCDEFGH.sig_out(BOMA_xBCDEFGH());
  /*_SIG_CPU_BOGA_Axxxxxxx*/ clk.SIG_CPU_BOGA_Axxxxxxx.sig_out(BOGA_Axxxxxxx());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_vid_clocks_gates() {
  /*_p29.WOSU*/ clk.WOSU_AxxDExxH.dff17(XYFY_xBxDxFxH(),            XAPO_VID_RSTn(), clk.WUVU_ABxxEFxx.qn_old());
  /*_p29.WUVU*/ clk.WUVU_ABxxEFxx.dff17(XOTA_AxCxExGx(),            XAPO_VID_RSTn(), clk.WUVU_ABxxEFxx.qn_old());
  /*_p21.VENA*/ clk.VENA_xxCDEFxx.dff17(clk.WUVU_ABxxEFxx.qn_new(), XAPO_VID_RSTn(), clk.VENA_xxCDEFxx.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}

//------------------------------------------------------------------------------------------------------------------------
