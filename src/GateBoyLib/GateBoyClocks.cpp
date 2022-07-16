#include "GateBoyLib/GateBoyClocks.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

void GateBoyClock::reset_to_cart() {
  ANOS_xBxDxFxH.state = 0b00011001;
  AVET_AxCxExGx.state = 0b00011000;
  
  AFUR_ABCDxxxx.set_state(0b00011001);
  ALEF_xBCDExxx.set_state(0b00011011);
  APUK_xxCDEFxx.set_state(0b00011001);
  ADYK_xxxDEFGx.set_state(0b00011011);

  WUVU_ABxxEFxx.state = 0b00011000;
  VENA_xxCDEFxx.state = 0b00011010;
  WOSU_AxxDExxH.state = 0b00011011;

  SIG_CPU_CLKREQ.state        = 0b00011001;
  SIG_CPU_BOWA_Axxxxxxx.state = 0b00011000;
  SIG_CPU_BEDO_xBCDEFGH.state = 0b00011001;
  SIG_CPU_BEKO_ABCDxxxx.state = 0b00011000;
  SIG_CPU_BUDE_xxxxEFGH.state = 0b00011001;
  SIG_CPU_BOLO_ABCDEFxx.state = 0b00011000;
  SIG_CPU_BUKE_AxxxxxGH.state = 0b00011001;
  SIG_CPU_BOMA_xBCDEFGH.state = 0b00011001;
  SIG_CPU_BOGA_Axxxxxxx.state = 0b00011000;
}

//-----------------------------------------------------------------------------

void GateBoy::tock_clocks_gates(const GateBoyState& reg_old) {
  auto& reg_new = gb_state;

  /*_PIN_73*/ pins.sys.PIN_73_CLK_DRIVE.pin_out(pins.sys.PIN_74_CLK.clk_new(), pins.sys.PIN_74_CLK.clk_new());

  /*_p01.ARYS*/ wire ARYS_xBxDxFxH = not1(pins.sys.PIN_74_CLK.clk_new());
  /*_p01.AVET*/ reg_new.sys_clk.AVET_AxCxExGx <<= nand2(reg_new.sys_clk.ANOS_xBxDxFxH.out_mid(), ARYS_xBxDxFxH);
  /*_p01.ANOS*/ reg_new.sys_clk.ANOS_xBxDxFxH <<= nand2(pins.sys.PIN_74_CLK.clk_new(), reg_new.sys_clk.AVET_AxCxExGx.out_mid());
  /*_p01.AVET*/ reg_new.sys_clk.AVET_AxCxExGx <<= nand2(reg_new.sys_clk.ANOS_xBxDxFxH.out_mid(), ARYS_xBxDxFxH);
  /*_p01.ANOS*/ reg_new.sys_clk.ANOS_xBxDxFxH <<= nand2(pins.sys.PIN_74_CLK.clk_new(), reg_new.sys_clk.AVET_AxCxExGx.out_mid());

  /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(reg_new.sys_clk.AVET_AxCxExGx.out_new());
  /*_p01.ATAN*/ wire ATAN_AxCxExGx = not1(ATAL_xBxDxFxH); // cell not marked on die but it's next to ATAL

  /*_p01.AFUR*/ reg_new.sys_clk.AFUR_ABCDxxxx.dff9b(ATAN_AxCxExGx, pins.sys.UPOJ_MODE_PRODn_new(), reg_old.sys_clk.ADYK_xxxDEFGx.qn_oldB());
  /*_p01.ALEF*/ reg_new.sys_clk.ALEF_xBCDExxx.dff9b(ATAL_xBxDxFxH, pins.sys.UPOJ_MODE_PRODn_new(), reg_old.sys_clk.AFUR_ABCDxxxx.qp_oldB());
  /*_p01.APUK*/ reg_new.sys_clk.APUK_xxCDEFxx.dff9b(ATAN_AxCxExGx, pins.sys.UPOJ_MODE_PRODn_new(), reg_old.sys_clk.ALEF_xBCDExxx.qp_oldB());
  /*_p01.ADYK*/ reg_new.sys_clk.ADYK_xxxDEFGx.dff9b(ATAL_xBxDxFxH, pins.sys.UPOJ_MODE_PRODn_new(), reg_old.sys_clk.APUK_xxCDEFxx.qp_oldB());

  /*_PIN_75*/ pins.sys.PIN_75_CLK_OUT.pin_out(reg_new.sys_clk.BUDE_xxxxEFGH_new(), reg_new.sys_clk.BUDE_xxxxEFGH_new());

  /*_SIG_CPU_BOWA_Axxxxxxx*/ reg_new.sys_clk.SIG_CPU_BOWA_Axxxxxxx.sig_out(BOWA_xBCDEFGH_new());
  /*_SIG_CPU_BEDO_xBCDEFGH*/ reg_new.sys_clk.SIG_CPU_BEDO_xBCDEFGH.sig_out(BEDO_Axxxxxxx_new());
  /*_SIG_CPU_BEKO_ABCDxxxx*/ reg_new.sys_clk.SIG_CPU_BEKO_ABCDxxxx.sig_out(reg_new.sys_clk.BEKO_ABCDxxxx_new());
  /*_SIG_CPU_BUDE_xxxxEFGH*/ reg_new.sys_clk.SIG_CPU_BUDE_xxxxEFGH.sig_out(reg_new.sys_clk.BUDE_xxxxEFGH_new());
  /*_SIG_CPU_BOLO_ABCDEFxx*/ reg_new.sys_clk.SIG_CPU_BOLO_ABCDEFxx.sig_out(reg_new.sys_clk.BOLO_ABCDEFxx_new());
  /*_SIG_CPU_BUKE_AxxxxxGH*/ reg_new.sys_clk.SIG_CPU_BUKE_AxxxxxGH.sig_out(reg_new.sys_clk.BUKE_AxxxxxGH_new());
  /*_SIG_CPU_BOMA_xBCDEFGH*/ reg_new.sys_clk.SIG_CPU_BOMA_xBCDEFGH.sig_out(BOMA_xBCDEFGH_new());
  /*_SIG_CPU_BOGA_Axxxxxxx*/ reg_new.sys_clk.SIG_CPU_BOGA_Axxxxxxx.sig_out(BOGA_Axxxxxxx_new());
}

//-----------------------------------------------------------------------------

void GateBoy::tock_vid_clocks_gates(const GateBoyState& reg_old) {
  auto& reg_new = gb_state;
  /*_p29.WOSU*/ reg_new.sys_clk.WOSU_AxxDExxH.dff17(reg_new.sys_clk.XYFY_xBxDxFxH_new(),         reg_new.XAPO_VID_RSTn_new(), reg_old.sys_clk.WUVU_ABxxEFxx.qn_old());
  /*_p29.WUVU*/ reg_new.sys_clk.WUVU_ABxxEFxx.dff17(reg_new.sys_clk.XOTA_AxCxExGx_new(),         reg_new.XAPO_VID_RSTn_new(), reg_old.sys_clk.WUVU_ABxxEFxx.qn_old());
  /*_p21.VENA*/ reg_new.sys_clk.VENA_xxCDEFxx.dff17(reg_new.sys_clk.WUVU_ABxxEFxx.qn_new(), reg_new.XAPO_VID_RSTn_new(), reg_old.sys_clk.VENA_xxCDEFxx.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}

//-----------------------------------------------------------------------------
