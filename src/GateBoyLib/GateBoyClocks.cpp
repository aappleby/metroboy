#include "GateBoyLib/GateBoyClocks.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

void GateBoyClock::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void GateBoyClock::reset_to_bootrom() {
  ANOS_DEGLITCH.state = 0b00011001;
  AVET_DEGLITCH.state = 0b00011000;
  AFUR_xxxxEFGH.state = 0b00011001;
  ALEF_AxxxxFGH.state = 0b00011011;
  APUK_ABxxxxGH.state = 0b00011001;
  ADYK_ABCxxxxH.state = 0b00011011;
  WUVU_ABxxEFxx.state = 0b00011000;
  VENA_xxCDEFxx.state = 0b00011010;
  WOSU_AxxDExxH.state = 0b00011010;

  SIG_CPU_CLKREQ.state        = 0b00011000;
  SIG_CPU_BOWA_Axxxxxxx.state = 0b00011001;
  SIG_CPU_BEDO_xBCDEFGH.state = 0b00011000;
  SIG_CPU_BEKO_ABCDxxxx.state = 0b00011001;
  SIG_CPU_BUDE_xxxxEFGH.state = 0b00011000;
  SIG_CPU_BOLO_ABCDEFxx.state = 0b00011001;
  SIG_CPU_BUKE_AxxxxxGH.state = 0b00011000;
  SIG_CPU_BOMA_xBCDEFGH.state = 0b00011001;
  SIG_CPU_BOGA_Axxxxxxx.state = 0b00011000;
}

void GateBoyClock::reset_to_cart() {
  ANOS_DEGLITCH.state = 0b00011001;
  AVET_DEGLITCH.state = 0b00011000;
  AFUR_xxxxEFGH.state = 0b00011001;
  ALEF_AxxxxFGH.state = 0b00011011;
  APUK_ABxxxxGH.state = 0b00011001;
  ADYK_ABCxxxxH.state = 0b00011011;
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

wire GateBoyClock::ABOL_CLKREQn_new() const { /*_p01.ABOL*/ return not1(SIG_CPU_CLKREQ.out_new()); }
wire GateBoyClock::BUTY_CLKREQp_new() const { /*#p01.BUTY*/ return not1(ABOL_CLKREQn_new()); }

wire GateBoyClock::AZOF_odd_old() const {
  /*_p01.ATAL*/ wire ATAL_EVN_old = not1(AVET_DEGLITCH.out_old());
  /*_p01.AZOF*/ wire AZOF_odd_old = not1(ATAL_EVN_old);
  return AZOF_odd_old;
}

wire GateBoyClock::AZOF_odd_new() const {
  /*_p01.ATAL*/ wire ATAL_EVN_new = not1(AVET_DEGLITCH.out_new());
  /*_p01.AZOF*/ wire AZOF_odd_new = not1(ATAL_EVN_new);
  return AZOF_odd_new;
}

wire GateBoyClock::ZAXY_evn_old() const { /*_p01.ZAXY*/ return not1(AZOF_odd_old()); }
wire GateBoyClock::ZEME_odd_old() const { /*#p01.ZEME*/ return not1(ZAXY_evn_old()); }
wire GateBoyClock::XYVA_evn_old() const { /*_p29.XYVA*/ return not1(ZEME_odd_old()); }
wire GateBoyClock::XOTA_odd_old() const { /*_p29.XOTA*/ return not1(XYVA_evn_old()); }
wire GateBoyClock::XYFY_evn_old() const { /*_p29.XYFY*/ return not1(XOTA_odd_old()); }
wire GateBoyClock::ALET_evn_old() const { /*#p01.ALET*/ return not1(ZEME_odd_old()); }
wire GateBoyClock::LAPE_odd_old() const { /*_p01.LAPE*/ return not1(ALET_evn_old()); }
wire GateBoyClock::MEHE_odd_old() const { /*#p27.MEHE*/ return not1(ALET_evn_old()); }
wire GateBoyClock::MYVO_odd_old() const { /*#p27.MYVO*/ return not1(ALET_evn_old()); }
wire GateBoyClock::MOXE_odd_old() const { /*_p27.MOXE*/ return not1(ALET_evn_old()); }
wire GateBoyClock::TAVA_evn_old() const { /*_p27.TAVA*/ return not1(LAPE_odd_old()); }

wire GateBoyClock::ZAXY_evn_new() const { /*_p01.ZAXY*/ return not1(AZOF_odd_new()); }
wire GateBoyClock::ZEME_odd_new() const { /*#p01.ZEME*/ return not1(ZAXY_evn_new()); }
wire GateBoyClock::XYVA_evn_new() const { /*_p29.XYVA*/ return not1(ZEME_odd_new()); }
wire GateBoyClock::XOTA_odd_new() const { /*_p29.XOTA*/ return not1(XYVA_evn_new()); }
wire GateBoyClock::XYFY_evn_new() const { /*_p29.XYFY*/ return not1(XOTA_odd_new()); }
wire GateBoyClock::ALET_evn_new() const { /*#p01.ALET*/ return not1(ZEME_odd_new()); }
wire GateBoyClock::LAPE_odd_new() const { /*_p01.LAPE*/ return not1(ALET_evn_new()); }
wire GateBoyClock::MEHE_odd_new() const { /*#p27.MEHE*/ return not1(ALET_evn_new()); }
wire GateBoyClock::MYVO_odd_new() const { /*#p27.MYVO*/ return not1(ALET_evn_new()); }
wire GateBoyClock::MOXE_odd_new() const { /*_p27.MOXE*/ return not1(ALET_evn_new()); }
wire GateBoyClock::TAVA_evn_new() const { /*_p27.TAVA*/ return not1(LAPE_odd_new()); }

wire GateBoyClock::ATYP_ABCDxxxx_new() const { /*#p01.ATYP*/ return not1(AFUR_xxxxEFGH.qp_new()); }
wire GateBoyClock::AFEP_AxxxxFGH_new() const { /*#p01.AFEP*/ return not1(ALEF_AxxxxFGH.qn_new()); }
wire GateBoyClock::AROV_xxCDEFxx_new() const { /*#p01.AROV*/ return not1(APUK_ABxxxxGH.qp_new()); }
wire GateBoyClock::ADAR_ABCxxxxH_new() const { /*#p01.ADAR*/ return not1(ADYK_ABCxxxxH.qn_new()); }

wire GateBoyClock::BEKO_ABCDxxxx_new() const { /*#p01.BEKO*/ return not1(BUDE_xxxxEFGH_new()); } // BEKO+BAVY parallel
wire GateBoyClock::BAPY_xxxxxxGH_new() const { /*#p01.BAPY*/ return nor3(ABOL_CLKREQn_new(), AROV_xxCDEFxx_new(), ATYP_ABCDxxxx_new()); }
wire GateBoyClock::BERU_ABCDEFxx_new() const { /*#p01.BERU*/ return not1(BAPY_xxxxxxGH_new()); }
wire GateBoyClock::BUFA_xxxxxxGH_new() const { /*#p01.BUFA*/ return not1(BERU_ABCDEFxx_new()); }
wire GateBoyClock::BOLO_ABCDEFxx_new() const { /*#p01.BOLO*/ return not1(BUFA_xxxxxxGH_new()); }
wire GateBoyClock::BEJA_xxxxEFGH_new() const { /*#p01.BEJA*/ return nand4(BOLO_ABCDEFxx_new(), BOLO_ABCDEFxx_new(), BEKO_ABCDxxxx_new(), BEKO_ABCDxxxx_new()); }
wire GateBoyClock::BANE_ABCDxxxx_new() const { /*#p01.BANE*/ return not1(BEJA_xxxxEFGH_new()); }
wire GateBoyClock::BELO_xxxxEFGH_new() const { /*#p01.BELO*/ return not1(BANE_ABCDxxxx_new()); }
wire GateBoyClock::BAZE_ABCDxxxx_new() const { /*#p01.BAZE*/ return not1(BELO_xxxxEFGH_new()); }
wire GateBoyClock::BUTO_xBCDEFGH_new() const { /*#p01.BUTO*/ return nand3(AFEP_AxxxxFGH_new(), ATYP_ABCDxxxx_new(), BAZE_ABCDxxxx_new()); }
wire GateBoyClock::BELE_Axxxxxxx_new() const { /*#p01.BELE*/ return not1(BUTO_xBCDEFGH_new()); }
wire GateBoyClock::BUGO_xBCDExxx_new() const { /*#p01.BUGO*/ return not1(AFEP_AxxxxFGH_new()); }
wire GateBoyClock::BATE_AxxxxxGH_new() const { /*#p01.BATE*/ return nor3(BUGO_xBCDExxx_new(), AROV_xxCDEFxx_new(), ABOL_CLKREQn_new()); }
wire GateBoyClock::BASU_xBCDEFxx_new() const { /*#p01.BASU*/ return not1(BATE_AxxxxxGH_new()); }
wire GateBoyClock::BUKE_AxxxxxGH_new() const { /*#p01.BUKE*/ return not1(BASU_xBCDEFxx_new()); }
wire GateBoyClock::BELU_xxxxEFGH_new() const { /*#p01.BELU*/ return nor2(ATYP_ABCDxxxx_new(), ABOL_CLKREQn_new()); }
wire GateBoyClock::BYRY_ABCDxxxx_new() const { /*#p01.BYRY*/ return not1(BELU_xxxxEFGH_new()); }
wire GateBoyClock::BUDE_xxxxEFGH_new() const { /*#p01.BUDE*/ return not1(BYRY_ABCDxxxx_new()); }
wire GateBoyClock::UVYT_ABCDxxxx_new() const { /*_p01.UVYT*/ return not1(BUDE_xxxxEFGH_new()); }
wire GateBoyClock::MOPA_xxxxEFGH_new() const { /*_p04.MOPA*/ return not1(UVYT_ABCDxxxx_new()); }
wire GateBoyClock::XYNY_ABCDxxxx_new() const { /*_p28.XYNY*/ return not1(MOPA_xxxxEFGH_new()); }

wire GateBoyClock::TALU_xxCDEFxx_new() const { /*#p21.TALU*/ return not1(VENA_xxCDEFxx.qn_new()); }
wire GateBoyClock::XUPY_ABxxEFxx_new() const { /*#p29.XUPY*/ return not1(WUVU_ABxxEFxx.qn_new()); }
wire GateBoyClock::XOCE_xBCxxFGx_new() const { /*#p29.XOCE*/ return not1(WOSU_AxxDExxH.qp_new()); }
wire GateBoyClock::WOJO_AxxxExxx_new() const { /*#p29.WOJO*/ return nor2(WOSU_AxxDExxH.qn_new(), WUVU_ABxxEFxx.qn_new()); }
wire GateBoyClock::SONO_ABxxxxGH_new() const { /*#p21.SONO*/ return not1(TALU_xxCDEFxx_new()); }
wire GateBoyClock::XYSO_xBCDxFGH_new() const { /*_p29.XYSO*/ return not1(WOJO_AxxxExxx_new()); }
wire GateBoyClock::CYKE_ABxxEFxx_new() const { /*#p30.CYKE*/ return not1(XUPY_ABxxEFxx_new()); }
wire GateBoyClock::WUDA_xxCDxxGH_new() const { /*#p30.WUDA*/ return not1(CYKE_ABxxEFxx_new()); }
wire GateBoyClock::AWOH_xxCDxxGH_new() const { /*#p28.AWOH*/ return not1(XUPY_ABxxEFxx_new()); }
wire GateBoyClock::AJAX_xxxxEFGH_new() const { /*#p01.AJAX*/ return not1(ATYP_ABCDxxxx_new()); }

wire GateBoy::BYJU_Axxxxxxx_new() const { /*#p01.BYJU*/ return or2(gb_state.sys_clk.BELE_Axxxxxxx_new(), pins.sys.ATEZ_CLKBADp_new()); }
wire GateBoy::BALY_xBCDEFGH_new() const { /*#p01.BALY*/ return not1(BYJU_Axxxxxxx_new()); }
wire GateBoy::BOGA_Axxxxxxx_new() const { /*_p01.BOGA*/ return not1(BALY_xBCDEFGH_new()); }

wire GateBoy::BUVU_Axxxxxxx_new() const { /*#p01.BUVU*/ return and2(BALY_xBCDEFGH_new(), gb_state.sys_clk.BUTY_CLKREQp_new()); }
wire GateBoy::BYXO_xBCDEFGH_new() const { /*#p01.BYXO*/ return not1(BUVU_Axxxxxxx_new()); }
wire GateBoy::BEDO_Axxxxxxx_new() const { /*#p01.BEDO*/ return not1(BYXO_xBCDEFGH_new()); }
wire GateBoy::BOWA_xBCDEFGH_new() const { /*#p01.BOWA*/ return not1(BEDO_Axxxxxxx_new()); }
wire GateBoy::BOMA_xBCDEFGH_new() const { /*#p01.BOMA*/ return not1(BOGA_Axxxxxxx_new()); }

//-----------------------------------------------------------------------------

void GateBoy::tock_clocks_gates(const GateBoyState& reg_old) {
  auto& reg_new = gb_state;

  /*_PIN_73*/ pins.sys.PIN_73_CLK_DRIVE.pin_out(pins.sys.PIN_74_CLK.clk_new(), pins.sys.PIN_74_CLK.clk_new());

  /*_p01.ARYS*/ wire ARYS_CLKIN = not1(pins.sys.PIN_74_CLK.clk_new());
  /*_p01.AVET*/ reg_new.sys_clk.AVET_DEGLITCH <<= nand2(reg_new.sys_clk.ANOS_DEGLITCH.out_mid(), ARYS_CLKIN);
  /*_p01.ANOS*/ reg_new.sys_clk.ANOS_DEGLITCH <<= nand2(pins.sys.PIN_74_CLK.clk_new(), reg_new.sys_clk.AVET_DEGLITCH.out_mid());
  /*_p01.AVET*/ reg_new.sys_clk.AVET_DEGLITCH <<= nand2(reg_new.sys_clk.ANOS_DEGLITCH.out_mid(), ARYS_CLKIN);
  /*_p01.ANOS*/ reg_new.sys_clk.ANOS_DEGLITCH <<= nand2(pins.sys.PIN_74_CLK.clk_new(), reg_new.sys_clk.AVET_DEGLITCH.out_mid());

  /*_p01.ATAL*/ wire ATAL_EVN = not1(reg_new.sys_clk.AVET_DEGLITCH.out_new());
  /*_p01.ATAN*/ wire ATAN_OLD = not1(ATAL_EVN); // cell not marked on die but it's next to ATAL

  /*_p01.AFUR*/ reg_new.sys_clk.AFUR_xxxxEFGH.dff9(ATAN_OLD, pins.sys.UPOJ_MODE_PRODn_new(), reg_old.sys_clk.ADYK_ABCxxxxH.qp_old());
  /*_p01.ALEF*/ reg_new.sys_clk.ALEF_AxxxxFGH.dff9(ATAL_EVN, pins.sys.UPOJ_MODE_PRODn_new(), reg_old.sys_clk.AFUR_xxxxEFGH.qn_old());
  /*_p01.APUK*/ reg_new.sys_clk.APUK_ABxxxxGH.dff9(ATAN_OLD, pins.sys.UPOJ_MODE_PRODn_new(), reg_old.sys_clk.ALEF_AxxxxFGH.qn_old());
  /*_p01.ADYK*/ reg_new.sys_clk.ADYK_ABCxxxxH.dff9(ATAL_EVN, pins.sys.UPOJ_MODE_PRODn_new(), reg_old.sys_clk.APUK_ABxxxxGH.qn_old());

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
  /*_p29.WOSU*/ reg_new.sys_clk.WOSU_AxxDExxH.dff17(reg_new.sys_clk.XYFY_evn_new(),         reg_new.XAPO_VID_RSTn_new(), reg_old.sys_clk.WUVU_ABxxEFxx.qn_old());
  /*_p29.WUVU*/ reg_new.sys_clk.WUVU_ABxxEFxx.dff17(reg_new.sys_clk.XOTA_odd_new(),         reg_new.XAPO_VID_RSTn_new(), reg_old.sys_clk.WUVU_ABxxEFxx.qn_old());
  /*_p21.VENA*/ reg_new.sys_clk.VENA_xxCDEFxx.dff17(reg_new.sys_clk.WUVU_ABxxEFxx.qn_new(), reg_new.XAPO_VID_RSTn_new(), reg_old.sys_clk.VENA_xxCDEFxx.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}

//-----------------------------------------------------------------------------
