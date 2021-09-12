#include "GateBoyLib/GateBoyClocks.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_clocks_gates() {
  /*_PIN_73*/ gbs.clk.PIN_73_CLK_DRIVE.pin_out(gbs.clk.PIN_74_CLK.clk(), gbs.clk.PIN_74_CLK.clk());

  /*_p01.ARYS*/ wire ARYS_CLKIN = not1(gbs.clk.PIN_74_CLK.clk());
  /*_p01.AVET*/ gbs.clk.AVET_DEGLITCH = nand2(gbs.clk.ANOS_DEGLITCH.out_mid(), ARYS_CLKIN);
  /*_p01.ANOS*/ gbs.clk.ANOS_DEGLITCH = nand2(gbs.clk.PIN_74_CLK.clk(), gbs.clk.AVET_DEGLITCH.out_mid());
  /*_p01.AVET*/ gbs.clk.AVET_DEGLITCH = nand2(gbs.clk.ANOS_DEGLITCH.out_mid(), ARYS_CLKIN);
  /*_p01.ANOS*/ gbs.clk.ANOS_DEGLITCH = nand2(gbs.clk.PIN_74_CLK.clk(), gbs.clk.AVET_DEGLITCH.out_mid());

  /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(gbs.clk.AVET_DEGLITCH.out_new());
  /*_p01.ATAN*/ wire ATAN_AxCxExGx = not1(ATAL_xBxDxFxH); // cell not marked on die but it's next to ATAL

  DFF9 ADYK_ABCxxxxH_old = gbs.clk.ADYK_ABCxxxxH;
  DFF9 AFUR_xxxxEFGH_old = gbs.clk.AFUR_xxxxEFGH;
  DFF9 ALEF_AxxxxFGH_old = gbs.clk.ALEF_AxxxxFGH;
  DFF9 APUK_ABxxxxGH_old = gbs.clk.APUK_ABxxxxGH;

  /*_p01.AFUR*/ gbs.clk.AFUR_xxxxEFGH.dff9(ATAN_AxCxExGx, UPOJ_MODE_PRODn(), ADYK_ABCxxxxH_old.qp_old());
  /*_p01.ALEF*/ gbs.clk.ALEF_AxxxxFGH.dff9(ATAL_xBxDxFxH, UPOJ_MODE_PRODn(), AFUR_xxxxEFGH_old.qn_old());
  /*_p01.APUK*/ gbs.clk.APUK_ABxxxxGH.dff9(ATAN_AxCxExGx, UPOJ_MODE_PRODn(), ALEF_AxxxxFGH_old.qn_old());
  /*_p01.ADYK*/ gbs.clk.ADYK_ABCxxxxH.dff9(ATAL_xBxDxFxH, UPOJ_MODE_PRODn(), APUK_ABxxxxGH_old.qn_old());

  /*_PIN_75*/ gbs.clk.PIN_75_CLK_OUT.pin_out(BUDE_xxxxEFGH(), BUDE_xxxxEFGH());

  /*_SIG_CPU_BOWA_Axxxxxxx*/ gbs.clk.SIG_CPU_BOWA_Axxxxxxx.sig_out(BOWA_xBCDEFGH());
  /*_SIG_CPU_BEDO_xBCDEFGH*/ gbs.clk.SIG_CPU_BEDO_xBCDEFGH.sig_out(BEDO_Axxxxxxx());
  /*_SIG_CPU_BEKO_ABCDxxxx*/ gbs.clk.SIG_CPU_BEKO_ABCDxxxx.sig_out(BEKO_ABCDxxxx());
  /*_SIG_CPU_BUDE_xxxxEFGH*/ gbs.clk.SIG_CPU_BUDE_xxxxEFGH.sig_out(BUDE_xxxxEFGH());
  /*_SIG_CPU_BOLO_ABCDEFxx*/ gbs.clk.SIG_CPU_BOLO_ABCDEFxx.sig_out(BOLO_ABCDEFxx());
  /*_SIG_CPU_BUKE_AxxxxxGH*/ gbs.clk.SIG_CPU_BUKE_AxxxxxGH.sig_out(BUKE_AxxxxxGH());
  /*_SIG_CPU_BOMA_xBCDEFGH*/ gbs.clk.SIG_CPU_BOMA_xBCDEFGH.sig_out(BOMA_xBCDEFGH());
  /*_SIG_CPU_BOGA_Axxxxxxx*/ gbs.clk.SIG_CPU_BOGA_Axxxxxxx.sig_out(BOGA_Axxxxxxx());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_vid_clocks_gates() {
  /*_p29.WOSU*/ gbs.clk.WOSU_AxxDExxH.dff17(XYFY_xBxDxFxH(),            XAPO_VID_RSTn(), gbs.clk.WUVU_ABxxEFxx.qn_old());
  /*_p29.WUVU*/ gbs.clk.WUVU_ABxxEFxx.dff17(XOTA_AxCxExGx(),            XAPO_VID_RSTn(), gbs.clk.WUVU_ABxxEFxx.qn_old());
  /*_p21.VENA*/ gbs.clk.VENA_xxCDEFxx.dff17(gbs.clk.WUVU_ABxxEFxx.qn_new(), XAPO_VID_RSTn(), gbs.clk.VENA_xxCDEFxx.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}

//------------------------------------------------------------------------------------------------------------------------
