#include "GateBoyLib/GateBoyClocks.h"

#include "GateBoyLib/GateBoy.h"

void GateBoyClock::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void GateBoyClock::reset_to_bootrom() {
  ANOS_DEGLITCH.state = 0b00011000;
  AVET_DEGLITCH.state = 0b00011001;
  AFUR_xxxxEFGH.state = 0b00011010;
  ALEF_AxxxxFGH.state = 0b00011001;
  APUK_ABxxxxGH.state = 0b00011011;
  ADYK_ABCxxxxH.state = 0b00011001;

  WUVU_ABxxEFxx.state = 0b00011010;
  VENA_xxCDEFxx.state = 0b00011010;
  WOSU_AxxDExxH.state = 0b00011000;

  SIG_CPU_CLKREQ.state        = 0b00011000;
  SIG_CPU_BOWA_Axxxxxxx.state = 0b00011001;
  SIG_CPU_BEDO_xBCDEFGH.state = 0b00011000;
  SIG_CPU_BEKO_ABCDxxxx.state = 0b00011001;
  SIG_CPU_BUDE_xxxxEFGH.state = 0b00011000;
  SIG_CPU_BOLO_ABCDEFxx.state = 0b00011001;
  SIG_CPU_BUKE_AxxxxxGH.state = 0b00011000;
  SIG_CPU_BOMA_xBCDEFGH.state = 0b00011000;
  SIG_CPU_BOGA_Axxxxxxx.state = 0b00011001;
}

void GateBoyClock::reset_to_cart() {
  ANOS_DEGLITCH.state = 0b00011000;
  AVET_DEGLITCH.state = 0b00011001;

  AFUR_xxxxEFGH.state = 0b00011010;
  ALEF_AxxxxFGH.state = 0b00011001;
  APUK_ABxxxxGH.state = 0b00011011;
  ADYK_ABCxxxxH.state = 0b00011001;

  WUVU_ABxxEFxx.state = 0b00011011;
  VENA_xxCDEFxx.state = 0b00011000;
  WOSU_AxxDExxH.state = 0b00011001;

  SIG_CPU_CLKREQ.state = 0b00011001;
  SIG_CPU_BOWA_Axxxxxxx.state = 0b00011001;
  SIG_CPU_BEDO_xBCDEFGH.state = 0b00011000;
  SIG_CPU_BEKO_ABCDxxxx.state = 0b00011001;
  SIG_CPU_BUDE_xxxxEFGH.state = 0b00011000;
  SIG_CPU_BOLO_ABCDEFxx.state = 0b00011001;
  SIG_CPU_BUKE_AxxxxxGH.state = 0b00011001;
  SIG_CPU_BOMA_xBCDEFGH.state = 0b00011000;
  SIG_CPU_BOGA_Axxxxxxx.state = 0b00011001;
}

/*_p01.ABOL*/ wire GateBoyClock::ABOL_CLKREQn() const { return not1(SIG_CPU_CLKREQ.out_new()); }
/*#p01.BUTY*/ wire GateBoyClock::BUTY_CLKREQp() const { return not1(ABOL_CLKREQn()); }

wire GateBoyClock::AZOF_ODD() const {
  /*_p01.ATAL*/ wire ATAL_EVN = not1(AVET_DEGLITCH.out_mid());
  /*_p01.AZOF*/ wire AZOF_ODD = not1(ATAL_EVN);
  return AZOF_ODD;
}

/*_p01.ZAXY*/ wire GateBoyClock::ZAXY_EVN() const { return not1(AZOF_ODD()); }
/*#p01.ZEME*/ wire GateBoyClock::ZEME_ODD() const { return not1(ZAXY_EVN()); }
/*_p29.XYVA*/ wire GateBoyClock::XYVA_EVN() const { return not1(ZEME_ODD()); }
/*_p29.XOTA*/ wire GateBoyClock::XOTA_ODD() const { return not1(XYVA_EVN()); }
/*_p29.XYFY*/ wire GateBoyClock::XYFY_EVN() const { return not1(XOTA_ODD()); }
/*#p01.ALET*/ wire GateBoyClock::ALET_evn() const { return not1(ZEME_ODD()); }
/*_p01.LAPE*/ wire GateBoyClock::LAPE_odd() const { return not1(ALET_evn()); }
/*#p27.MEHE*/ wire GateBoyClock::MEHE_odd() const { return not1(ALET_evn()); }
/*#p27.MYVO*/ wire GateBoyClock::MYVO_odd() const { return not1(ALET_evn()); }
/*_p27.MOXE*/ wire GateBoyClock::MOXE_ODD() const { return not1(ALET_evn()); }
/*_p27.TAVA*/ wire GateBoyClock::TAVA_evn() const { return not1(LAPE_odd()); }

/*#p01.ATYP*/ wire GateBoyClock::ATYP_ABCDxxxx() const { return not1(AFUR_xxxxEFGH.qp_new()); }
/*#p01.AFEP*/ wire GateBoyClock::AFEP_AxxxxFGH() const { return not1(ALEF_AxxxxFGH.qn_new()); }
/*#p01.AROV*/ wire GateBoyClock::AROV_xxCDEFxx() const { return not1(APUK_ABxxxxGH.qp_new()); }
/*#p01.ADAR*/ wire GateBoyClock::ADAR_ABCxxxxH() const { return not1(ADYK_ABCxxxxH.qn_new()); }

/*#p01.BEKO*/ wire GateBoyClock::BEKO_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); } // BEKO+BAVY parallel
/*#p01.BAPY*/ wire GateBoyClock::BAPY_xxxxxxGH() const { return nor3(ABOL_CLKREQn(), AROV_xxCDEFxx(), ATYP_ABCDxxxx()); }
/*#p01.BERU*/ wire GateBoyClock::BERU_ABCDEFxx() const { return not1(BAPY_xxxxxxGH()); }
/*#p01.BUFA*/ wire GateBoyClock::BUFA_xxxxxxGH() const { return not1(BERU_ABCDEFxx()); }
/*#p01.BOLO*/ wire GateBoyClock::BOLO_ABCDEFxx() const { return not1(BUFA_xxxxxxGH()); }
/*#p01.BEJA*/ wire GateBoyClock::BEJA_xxxxEFGH() const { return nand4(BOLO_ABCDEFxx(), BOLO_ABCDEFxx(), BEKO_ABCDxxxx(), BEKO_ABCDxxxx()); }
/*#p01.BANE*/ wire GateBoyClock::BANE_ABCDxxxx() const { return not1(BEJA_xxxxEFGH()); }
/*#p01.BELO*/ wire GateBoyClock::BELO_xxxxEFGH() const { return not1(BANE_ABCDxxxx()); }
/*#p01.BAZE*/ wire GateBoyClock::BAZE_ABCDxxxx() const { return not1(BELO_xxxxEFGH()); }
/*#p01.BUTO*/ wire GateBoyClock::BUTO_xBCDEFGH() const { return nand3(AFEP_AxxxxFGH(), ATYP_ABCDxxxx(), BAZE_ABCDxxxx()); }
/*#p01.BELE*/ wire GateBoyClock::BELE_Axxxxxxx() const { return not1(BUTO_xBCDEFGH()); }
/*#p01.BUGO*/ wire GateBoyClock::BUGO_xBCDExxx() const { return not1(AFEP_AxxxxFGH()); }
/*#p01.BATE*/ wire GateBoyClock::BATE_AxxxxxGH() const { return nor3(BUGO_xBCDExxx(), AROV_xxCDEFxx(), ABOL_CLKREQn()); }
/*#p01.BASU*/ wire GateBoyClock::BASU_xBCDEFxx() const { return not1(BATE_AxxxxxGH()); }
/*#p01.BUKE*/ wire GateBoyClock::BUKE_AxxxxxGH() const { return not1(BASU_xBCDEFxx()); }
/*#p01.BELU*/ wire GateBoyClock::BELU_xxxxEFGH() const { return nor2(ATYP_ABCDxxxx(), ABOL_CLKREQn()); }
/*#p01.BYRY*/ wire GateBoyClock::BYRY_ABCDxxxx() const { return not1(BELU_xxxxEFGH()); }
/*#p01.BUDE*/ wire GateBoyClock::BUDE_xxxxEFGH() const { return not1(BYRY_ABCDxxxx()); }
/*_p01.UVYT*/ wire GateBoyClock::UVYT_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); }
/*_p04.MOPA*/ wire GateBoyClock::MOPA_xxxxEFGH() const { return not1(UVYT_ABCDxxxx()); }
/*_p28.XYNY*/ wire GateBoyClock::XYNY_ABCDxxxx() const { return not1(MOPA_xxxxEFGH()); }

/*#p21.TALU*/ wire GateBoyClock::TALU_xxCDEFxx() const { return not1(VENA_xxCDEFxx.qn_new()); }
/*#p29.XUPY*/ wire GateBoyClock::XUPY_ABxxEFxx() const { return not1(WUVU_ABxxEFxx.qn_new()); }
/*#p29.XOCE*/ wire GateBoyClock::XOCE_xBCxxFGx() const { return not1(WOSU_AxxDExxH.qp_new()); }
/*#p29.WOJO*/ wire GateBoyClock::WOJO_AxxxExxx() const { return nor2(WOSU_AxxDExxH.qn_new(), WUVU_ABxxEFxx.qn_new()); }
/*#p21.SONO*/ wire GateBoyClock::SONO_ABxxxxGH() const { return not1(TALU_xxCDEFxx()); }
/*_p29.XYSO*/ wire GateBoyClock::XYSO_xBCDxFGH() const { return not1(WOJO_AxxxExxx()); }
/*#p30.CYKE*/ wire GateBoyClock::CYKE_ABxxEFxx() const { return not1(XUPY_ABxxEFxx()); }
/*#p30.WUDA*/ wire GateBoyClock::WUDA_xxCDxxGH() const { return not1(CYKE_ABxxEFxx()); }
/*#p28.AWOH*/ wire GateBoyClock::AWOH_xxCDxxGH() const { return not1(XUPY_ABxxEFxx()); }
/*#p01.AJAX*/ wire GateBoyClock::AJAX_xxxxEFGH() const { return not1(ATYP_ABCDxxxx()); }

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
