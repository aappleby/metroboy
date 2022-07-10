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

/*_p01.ABOL*/ wire GateBoyClock::ABOL_CLKREQn_new() const { return not1(SIG_CPU_CLKREQ.out_new()); }
/*#p01.BUTY*/ wire GateBoyClock::BUTY_CLKREQp_new() const { return not1(ABOL_CLKREQn_new()); }

wire GateBoyClock::AZOF_AxCxExGx_old() const {
  /*_p01.ATAL*/ wire ATAL_xBxDxFxH_old = not1(AVET_AxCxExGx.out_old());
  /*_p01.AZOF*/ wire AZOF_AxCxExGx_old = not1(ATAL_xBxDxFxH_old);
  return AZOF_AxCxExGx_old;
}

wire GateBoyClock::AZOF_AxCxExGx_new() const {
  /*_p01.ATAL*/ wire ATAL_xBxDxFxH_new = not1(AVET_AxCxExGx.out_new());
  /*_p01.AZOF*/ wire AZOF_AxCxExGx_new = not1(ATAL_xBxDxFxH_new);
  return AZOF_AxCxExGx_new;
}

/*_p01.ZAXY*/ wire GateBoyClock::ZAXY_xBxDxFxH_old() const { return not1(AZOF_AxCxExGx_old()); }
/*#p01.ZEME*/ wire GateBoyClock::ZEME_AxCxExGx_old() const { return not1(ZAXY_xBxDxFxH_old()); }
/*_p29.XYVA*/ wire GateBoyClock::XYVA_xBxDxFxH_old() const { return not1(ZEME_AxCxExGx_old()); }
/*_p29.XOTA*/ wire GateBoyClock::XOTA_AxCxExGx_old() const { return not1(XYVA_xBxDxFxH_old()); }
/*_p29.XYFY*/ wire GateBoyClock::XYFY_xBxDxFxH_old() const { return not1(XOTA_AxCxExGx_old()); }
/*#p01.ALET*/ wire GateBoyClock::ALET_xBxDxFxH_old() const { return not1(ZEME_AxCxExGx_old()); }
/*_p01.LAPE*/ wire GateBoyClock::LAPE_AxCxExGx_old() const { return not1(ALET_xBxDxFxH_old()); }
/*#p27.MEHE*/ wire GateBoyClock::MEHE_AxCxExGx_old() const { return not1(ALET_xBxDxFxH_old()); }
/*#p27.MYVO*/ wire GateBoyClock::MYVO_AxCxExGx_old() const { return not1(ALET_xBxDxFxH_old()); }
/*_p27.MOXE*/ wire GateBoyClock::MOXE_AxCxExGx_old() const { return not1(ALET_xBxDxFxH_old()); }
/*_p27.TAVA*/ wire GateBoyClock::TAVA_xBxDxFxH_old() const { return not1(LAPE_AxCxExGx_old()); }

/*_p01.ZAXY*/ wire GateBoyClock::ZAXY_xBxDxFxH_new() const { return not1(AZOF_AxCxExGx_new()); }
/*#p01.ZEME*/ wire GateBoyClock::ZEME_AxCxExGx_new() const { return not1(ZAXY_xBxDxFxH_new()); }
/*_p29.XYVA*/ wire GateBoyClock::XYVA_xBxDxFxH_new() const { return not1(ZEME_AxCxExGx_new()); }
/*_p29.XOTA*/ wire GateBoyClock::XOTA_AxCxExGx_new() const { return not1(XYVA_xBxDxFxH_new()); }
/*_p29.XYFY*/ wire GateBoyClock::XYFY_xBxDxFxH_new() const { return not1(XOTA_AxCxExGx_new()); }
/*#p01.ALET*/ wire GateBoyClock::ALET_xBxDxFxH_new() const { return not1(ZEME_AxCxExGx_new()); }
/*_p01.LAPE*/ wire GateBoyClock::LAPE_AxCxExGx_new() const { return not1(ALET_xBxDxFxH_new()); }
/*#p27.MEHE*/ wire GateBoyClock::MEHE_AxCxExGx_new() const { return not1(ALET_xBxDxFxH_new()); }
/*#p27.MYVO*/ wire GateBoyClock::MYVO_AxCxExGx_new() const { return not1(ALET_xBxDxFxH_new()); }
/*_p27.MOXE*/ wire GateBoyClock::MOXE_AxCxExGx_new() const { return not1(ALET_xBxDxFxH_new()); }
/*_p27.TAVA*/ wire GateBoyClock::TAVA_xBxDxFxH_new() const { return not1(LAPE_AxCxExGx_new()); }

/*#p01.ATYP*/ wire GateBoyClock::ATYP_ABCDxxxx_new() const { return not1(AFUR_ABCDxxxx.qn_newB()); }
/*#p01.AFEP*/ wire GateBoyClock::AFEP_AxxxxFGH_new() const { return not1(ALEF_xBCDExxx.qp_newB()); }
/*#p01.AROV*/ wire GateBoyClock::AROV_xxCDEFxx_new() const { return not1(APUK_xxCDEFxx.qn_newB()); }
/*#p01.ADAR*/ wire GateBoyClock::ADAR_ABCxxxxH_new() const { return not1(ADYK_xxxDEFGx.qp_newB()); }

/*#p01.BEKO*/ wire GateBoyClock::BEKO_ABCDxxxx_new() const { return not1(BUDE_xxxxEFGH_new()); } // BEKO+BAVY parallel
/*#p01.BAPY*/ wire GateBoyClock::BAPY_xxxxxxGH_new() const { return nor3(ABOL_CLKREQn_new(), AROV_xxCDEFxx_new(), ATYP_ABCDxxxx_new()); }
/*#p01.BERU*/ wire GateBoyClock::BERU_ABCDEFxx_new() const { return not1(BAPY_xxxxxxGH_new()); }
/*#p01.BUFA*/ wire GateBoyClock::BUFA_xxxxxxGH_new() const { return not1(BERU_ABCDEFxx_new()); }
/*#p01.BOLO*/ wire GateBoyClock::BOLO_ABCDEFxx_new() const { return not1(BUFA_xxxxxxGH_new()); }
/*#p01.BEJA*/ wire GateBoyClock::BEJA_xxxxEFGH_new() const { return nand4(BOLO_ABCDEFxx_new(), BOLO_ABCDEFxx_new(), BEKO_ABCDxxxx_new(), BEKO_ABCDxxxx_new()); }
/*#p01.BANE*/ wire GateBoyClock::BANE_ABCDxxxx_new() const { return not1(BEJA_xxxxEFGH_new()); }
/*#p01.BELO*/ wire GateBoyClock::BELO_xxxxEFGH_new() const { return not1(BANE_ABCDxxxx_new()); }
/*#p01.BAZE*/ wire GateBoyClock::BAZE_ABCDxxxx_new() const { return not1(BELO_xxxxEFGH_new()); }
/*#p01.BUTO*/ wire GateBoyClock::BUTO_xBCDEFGH_new() const { return nand3(AFEP_AxxxxFGH_new(), ATYP_ABCDxxxx_new(), BAZE_ABCDxxxx_new()); }
/*#p01.BELE*/ wire GateBoyClock::BELE_Axxxxxxx_new() const { return not1(BUTO_xBCDEFGH_new()); }
/*#p01.BUGO*/ wire GateBoyClock::BUGO_xBCDExxx_new() const { return not1(AFEP_AxxxxFGH_new()); }
/*#p01.BATE*/ wire GateBoyClock::BATE_AxxxxxGH_new() const { return nor3(BUGO_xBCDExxx_new(), AROV_xxCDEFxx_new(), ABOL_CLKREQn_new()); }
/*#p01.BASU*/ wire GateBoyClock::BASU_xBCDEFxx_new() const { return not1(BATE_AxxxxxGH_new()); }
/*#p01.BUKE*/ wire GateBoyClock::BUKE_AxxxxxGH_new() const { return not1(BASU_xBCDEFxx_new()); }
/*#p01.BELU*/ wire GateBoyClock::BELU_xxxxEFGH_new() const { return nor2(ATYP_ABCDxxxx_new(), ABOL_CLKREQn_new()); }
/*#p01.BYRY*/ wire GateBoyClock::BYRY_ABCDxxxx_new() const { return not1(BELU_xxxxEFGH_new()); }
/*#p01.BUDE*/ wire GateBoyClock::BUDE_xxxxEFGH_new() const { return not1(BYRY_ABCDxxxx_new()); }
/*_p01.UVYT*/ wire GateBoyClock::UVYT_ABCDxxxx_new() const { return not1(BUDE_xxxxEFGH_new()); }
/*_p04.MOPA*/ wire GateBoyClock::MOPA_xxxxEFGH_new() const { return not1(UVYT_ABCDxxxx_new()); }
/*_p28.XYNY*/ wire GateBoyClock::XYNY_ABCDxxxx_new() const { return not1(MOPA_xxxxEFGH_new()); }

/*#p21.TALU*/ wire GateBoyClock::TALU_xxCDEFxx_new() const { return not1(VENA_xxCDEFxx.qn_new()); }
/*#p29.XUPY*/ wire GateBoyClock::XUPY_ABxxEFxx_new() const { return not1(WUVU_ABxxEFxx.qn_new()); }
/*#p29.XOCE*/ wire GateBoyClock::XOCE_xBCxxFGx_new() const { return not1(WOSU_AxxDExxH.qp_new()); }
/*#p29.WOJO*/ wire GateBoyClock::WOJO_AxxxExxx_new() const { return nor2(WOSU_AxxDExxH.qn_new(), WUVU_ABxxEFxx.qn_new()); }
/*#p21.SONO*/ wire GateBoyClock::SONO_ABxxxxGH_new() const { return not1(TALU_xxCDEFxx_new()); }
/*_p29.XYSO*/ wire GateBoyClock::XYSO_xBCDxFGH_new() const { return not1(WOJO_AxxxExxx_new()); }
/*#p30.CYKE*/ wire GateBoyClock::CYKE_ABxxEFxx_new() const { return not1(XUPY_ABxxEFxx_new()); }
/*#p30.WUDA*/ wire GateBoyClock::WUDA_xxCDxxGH_new() const { return not1(CYKE_ABxxEFxx_new()); }
/*#p28.AWOH*/ wire GateBoyClock::AWOH_xxCDxxGH_new() const { return not1(XUPY_ABxxEFxx_new()); }
/*#p01.AJAX*/ wire GateBoyClock::AJAX_xxxxEFGH_new() const { return not1(ATYP_ABCDxxxx_new()); }

/*#p01.BYJU*/ wire GateBoy::BYJU_Axxxxxxx_new() const { return or2(gb_state.sys_clk.BELE_Axxxxxxx_new(), pins.sys.ATEZ_CLKBADp_new()); }
/*#p01.BALY*/ wire GateBoy::BALY_xBCDEFGH_new() const { return not1(BYJU_Axxxxxxx_new()); }
/*_p01.BOGA*/ wire GateBoy::BOGA_Axxxxxxx_new() const { return not1(BALY_xBCDEFGH_new()); }

/*#p01.BUVU*/ wire GateBoy::BUVU_Axxxxxxx_new() const { return and2(BALY_xBCDEFGH_new(), gb_state.sys_clk.BUTY_CLKREQp_new()); }
/*#p01.BYXO*/ wire GateBoy::BYXO_xBCDEFGH_new() const { return not1(BUVU_Axxxxxxx_new()); }
/*#p01.BEDO*/ wire GateBoy::BEDO_Axxxxxxx_new() const { return not1(BYXO_xBCDEFGH_new()); }
/*#p01.BOWA*/ wire GateBoy::BOWA_xBCDEFGH_new() const { return not1(BEDO_Axxxxxxx_new()); }
/*#p01.BOMA*/ wire GateBoy::BOMA_xBCDEFGH_new() const { return not1(BOGA_Axxxxxxx_new()); }

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
