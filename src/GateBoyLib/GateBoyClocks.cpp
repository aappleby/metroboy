#include "GateBoyLib/GateBoyClocks.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

void GateBoyClock::reset() {
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

  /*_p01.ABOL*/ wire ABOL_CLKREQn  = not1(reg_new.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*#p01.BUTY*/ wire BUTY_CLKREQp = not1(ABOL_CLKREQn);

  /*#p01.AROV*/ wire AROV_xxCDEFxx = not1(reg_new.sys_clk.APUK_xxCDEFxx.qn_newB());
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH = not1(reg_new.sys_clk.ALEF_xBCDExxx.qp_newB());
  /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_newB());

  /*#p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);

  /*#p01.BUGO*/ wire BUGO_xBCDExxx = not1(AFEP_AxxxxFGH);
  /*#p01.BATE*/ wire BATE_AxxxxxGH = nor3(BUGO_xBCDExxx, AROV_xxCDEFxx, ABOL_CLKREQn);
  /*#p01.BASU*/ wire BASU_xBCDEFxx = not1(BATE_AxxxxxGH);
  /*#p01.BUKE*/ wire BUKE_AxxxxxGH = not1(BASU_xBCDEFxx);
  /*#p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx);
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
  /*#p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH); // BEKO+BAVY parallel

  /*#p01.BEJA*/ wire BEJA_xxxxEFGH = nand4(BOLO_ABCDEFxx, BOLO_ABCDEFxx, BEKO_ABCDxxxx, BEKO_ABCDxxxx);
  /*#p01.BANE*/ wire BANE_ABCDxxxx = not1(BEJA_xxxxEFGH);
  /*#p01.BELO*/ wire BELO_xxxxEFGH = not1(BANE_ABCDxxxx);
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx = not1(BELO_xxxxEFGH);
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH = nand3(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*#p01.BELE*/ wire BELE_Axxxxxxx = not1(BUTO_xBCDEFGH);
  /*#p01.BYJU*/ wire BYJU_Axxxxxxx =  or2(BELE_Axxxxxxx, pins.sys.ATEZ_CLKBADp_new());
  /*#p01.BALY*/ wire BALY_xBCDEFGH = not1(BYJU_Axxxxxxx);

  /*#p01.BUVU*/ wire BUVU_Axxxxxxx = and2(BALY_xBCDEFGH, BUTY_CLKREQp);
  /*#p01.BYXO*/ wire BYXO_xBCDEFGH = not1(BUVU_Axxxxxxx);
  /*#p01.BEDO*/ wire BEDO_Axxxxxxx = not1(BYXO_xBCDEFGH);
  /*#p01.BOWA*/ wire BOWA_xBCDEFGH = not1(BEDO_Axxxxxxx);
  /*_p01.BOGA*/ wire BOGA_Axxxxxxx = not1(BALY_xBCDEFGH);
  /*#p01.BOMA*/ wire BOMA_xBCDEFGH = not1(BOGA_Axxxxxxx);

  /*_SIG_CPU_BOWA_Axxxxxxx*/ reg_new.sys_clk.SIG_CPU_BOWA_Axxxxxxx.sig_out(BOWA_xBCDEFGH);
  /*_SIG_CPU_BEDO_xBCDEFGH*/ reg_new.sys_clk.SIG_CPU_BEDO_xBCDEFGH.sig_out(BEDO_Axxxxxxx);
  /*_SIG_CPU_BEKO_ABCDxxxx*/ reg_new.sys_clk.SIG_CPU_BEKO_ABCDxxxx.sig_out(BEKO_ABCDxxxx);
  /*_SIG_CPU_BUDE_xxxxEFGH*/ reg_new.sys_clk.SIG_CPU_BUDE_xxxxEFGH.sig_out(BUDE_xxxxEFGH);
  /*_SIG_CPU_BOLO_ABCDEFxx*/ reg_new.sys_clk.SIG_CPU_BOLO_ABCDEFxx.sig_out(BOLO_ABCDEFxx);
  /*_SIG_CPU_BUKE_AxxxxxGH*/ reg_new.sys_clk.SIG_CPU_BUKE_AxxxxxGH.sig_out(BUKE_AxxxxxGH);
  /*_SIG_CPU_BOMA_xBCDEFGH*/ reg_new.sys_clk.SIG_CPU_BOMA_xBCDEFGH.sig_out(BOMA_xBCDEFGH);
  /*_SIG_CPU_BOGA_Axxxxxxx*/ reg_new.sys_clk.SIG_CPU_BOGA_Axxxxxxx.sig_out(BOGA_Axxxxxxx);

  /*_PIN_75*/ pins.sys.PIN_75_CLK_OUT.pin_out(BUDE_xxxxEFGH, BUDE_xxxxEFGH);
}

//-----------------------------------------------------------------------------

void GateBoy::tock_vid_clocks_gates(const GateBoyState& reg_old) {
  auto& reg_new = gb_state;
  /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(reg_new.sys_clk.AVET_AxCxExGx.out_new());
  /*_p01.AZOF*/ wire AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
  /*_p01.ZAXY*/ wire ZAXY_xBxDxFxH = not1(AZOF_AxCxExGx);
  /*#p01.ZEME*/ wire ZEME_AxCxExGx = not1(ZAXY_xBxDxFxH);
  /*_p29.XYVA*/ wire XYVA_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*_p29.XOTA*/ wire XOTA_AxCxExGx = not1(XYVA_xBxDxFxH);
  /*_p29.XYFY*/ wire XYFY_xBxDxFxH = not1(XOTA_AxCxExGx);
  /*_p29.WOSU*/ reg_new.sys_clk.WOSU_AxxDExxH.dff17(XYFY_xBxDxFxH, reg_new.XAPO_VID_RSTn_new(), reg_old.sys_clk.WUVU_ABxxEFxx.qn_old());
  /*_p29.WUVU*/ reg_new.sys_clk.WUVU_ABxxEFxx.dff17(XOTA_AxCxExGx, reg_new.XAPO_VID_RSTn_new(), reg_old.sys_clk.WUVU_ABxxEFxx.qn_old());
  /*_p21.VENA*/ reg_new.sys_clk.VENA_xxCDEFxx.dff17(reg_new.sys_clk.WUVU_ABxxEFxx.qn_new(), reg_new.XAPO_VID_RSTn_new(), reg_old.sys_clk.VENA_xxCDEFxx.qn_old()); // inverting the clock to VENA doesn't seem to break anything, which is really weird
}

//-----------------------------------------------------------------------------
