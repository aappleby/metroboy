#include "GateBoyLib/GateBoyClocks.h"

#include "GateBoyLib/GateBoy.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_clocks_gates() {
  /*_PIN_73*/ pins.sys.PIN_73_CLK_DRIVE.pin_out(pins.sys.PIN_74_CLK.clk(), pins.sys.PIN_74_CLK.clk());

  /*_p01.ARYS*/ wire ARYS_CLKIN = not1(pins.sys.PIN_74_CLK.clk());
  /*_p01.AVET*/ gb_state.sys_clk.AVET_DEGLITCH <<= nand2(gb_state.sys_clk.ANOS_DEGLITCH.out_mid(), ARYS_CLKIN);
  /*_p01.ANOS*/ gb_state.sys_clk.ANOS_DEGLITCH <<= nand2(pins.sys.PIN_74_CLK.clk(), gb_state.sys_clk.AVET_DEGLITCH.out_mid());
  /*_p01.AVET*/ gb_state.sys_clk.AVET_DEGLITCH <<= nand2(gb_state.sys_clk.ANOS_DEGLITCH.out_mid(), ARYS_CLKIN);
  /*_p01.ANOS*/ gb_state.sys_clk.ANOS_DEGLITCH <<= nand2(pins.sys.PIN_74_CLK.clk(), gb_state.sys_clk.AVET_DEGLITCH.out_mid());

  /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(gb_state.sys_clk.AVET_DEGLITCH.out_new());
  /*_p01.ATAN*/ wire ATAN_AxCxExGx = not1(ATAL_xBxDxFxH); // cell not marked on die but it's next to ATAL

  DFF9 ADYK_ABCxxxxH_old = gb_state.sys_clk.ADYK_ABCxxxxH;
  DFF9 AFUR_xxxxEFGH_old = gb_state.sys_clk.AFUR_xxxxEFGH;
  DFF9 ALEF_AxxxxFGH_old = gb_state.sys_clk.ALEF_AxxxxFGH;
  DFF9 APUK_ABxxxxGH_old = gb_state.sys_clk.APUK_ABxxxxGH;

  /*_p01.AFUR*/ gb_state.sys_clk.AFUR_xxxxEFGH.dff9(ATAN_AxCxExGx, pins.sys.UPOJ_MODE_PRODn(), ADYK_ABCxxxxH_old.qp_old());
  /*_p01.ALEF*/ gb_state.sys_clk.ALEF_AxxxxFGH.dff9(ATAL_xBxDxFxH, pins.sys.UPOJ_MODE_PRODn(), AFUR_xxxxEFGH_old.qn_old());
  /*_p01.APUK*/ gb_state.sys_clk.APUK_ABxxxxGH.dff9(ATAN_AxCxExGx, pins.sys.UPOJ_MODE_PRODn(), ALEF_AxxxxFGH_old.qn_old());
  /*_p01.ADYK*/ gb_state.sys_clk.ADYK_ABCxxxxH.dff9(ATAL_xBxDxFxH, pins.sys.UPOJ_MODE_PRODn(), APUK_ABxxxxGH_old.qn_old());

  /*_PIN_75*/ pins.sys.PIN_75_CLK_OUT.pin_out(gb_state.sys_clk.BUDE_xxxxEFGH(), gb_state.sys_clk.BUDE_xxxxEFGH());

  /*_SIG_CPU_BOWA_Axxxxxxx*/ gb_state.sys_clk.SIG_CPU_BOWA_Axxxxxxx.sig_out(BOWA_xBCDEFGH());
  /*_SIG_CPU_BEDO_xBCDEFGH*/ gb_state.sys_clk.SIG_CPU_BEDO_xBCDEFGH.sig_out(BEDO_Axxxxxxx());
  /*_SIG_CPU_BEKO_ABCDxxxx*/ gb_state.sys_clk.SIG_CPU_BEKO_ABCDxxxx.sig_out(gb_state.sys_clk.BEKO_ABCDxxxx());
  /*_SIG_CPU_BUDE_xxxxEFGH*/ gb_state.sys_clk.SIG_CPU_BUDE_xxxxEFGH.sig_out(gb_state.sys_clk.BUDE_xxxxEFGH());
  /*_SIG_CPU_BOLO_ABCDEFxx*/ gb_state.sys_clk.SIG_CPU_BOLO_ABCDEFxx.sig_out(gb_state.sys_clk.BOLO_ABCDEFxx());
  /*_SIG_CPU_BUKE_AxxxxxGH*/ gb_state.sys_clk.SIG_CPU_BUKE_AxxxxxGH.sig_out(gb_state.sys_clk.BUKE_AxxxxxGH());
  /*_SIG_CPU_BOMA_xBCDEFGH*/ gb_state.sys_clk.SIG_CPU_BOMA_xBCDEFGH.sig_out(BOMA_xBCDEFGH());
  /*_SIG_CPU_BOGA_Axxxxxxx*/ gb_state.sys_clk.SIG_CPU_BOGA_Axxxxxxx.sig_out(BOGA_Axxxxxxx());
}

//-----------------------------------------------------------------------------

void GateBoy::tock_vid_clocks_gates() {
  /*_p29.WOSU*/ gb_state.sys_clk.WOSU_AxxDExxH.dff17(gb_state.sys_clk.XYFY_EVN(),            XAPO_VID_RSTn(), gb_state.sys_clk.WUVU_ABxxEFxx.qn_old());
  /*_p29.WUVU*/ gb_state.sys_clk.WUVU_ABxxEFxx.dff17(gb_state.sys_clk.XOTA_ODD(),            XAPO_VID_RSTn(), gb_state.sys_clk.WUVU_ABxxEFxx.qn_old());
  /*_p21.VENA*/ gb_state.sys_clk.VENA_xxCDEFxx.dff17(gb_state.sys_clk.WUVU_ABxxEFxx.qn_new(), XAPO_VID_RSTn(), gb_state.sys_clk.VENA_xxCDEFxx.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}

//-----------------------------------------------------------------------------
