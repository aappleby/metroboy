#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyResetDebug;

//-----------------------------------------------------------------------------

struct GateBoyClock {

  void reset_to_cart() {
    SIG_CPU_BUKE_AxxxxxGH.reset(1);
    SIG_CPU_CLKREQ.reset(1);

    WUVU_ABxxEFxx.reset(1, 1);
    VENA_xxCDEFxx.reset(0, 0);
    WOSU_AxxDExxH.reset(0, 1);
  }

  void tock(const GateBoyResetDebug& rst);
  void tock_vid(const GateBoyResetDebug& rst);

  //----------------------------------------
  // Root clocks - ignoring the deglitcher here

  /* p01.UCOB*/ uint8_t UCOB_CLKBADp() const { return not1(PIN74_CLKGOOD.int_qp_new()); }
  /* p01.ATEZ*/ uint8_t ATEZ_CLKBADp() const { return not1(PIN74_CLKGOOD.int_qp_new()); }
  /* p01.ABOL*/ uint8_t ABOL_CLKREQn() const { return not1(SIG_CPU_CLKREQ.qp_new()); }
  /*#p01.BUTY*/ uint8_t BUTY_CLKREQp() const { return not1(ABOL_CLKREQn()); }

  /* p01.ATAL*/ uint8_t ATAL_xBxDxFxH() const { return PIN74_CLK_IN.int_qp_new(); }
  /* p01.AZOF*/ uint8_t AZOF_AxCxExGx() const { return not1(ATAL_xBxDxFxH()); }
  /* p01.ZAXY*/ uint8_t ZAXY_xBxDxFxH() const { return not1(AZOF_AxCxExGx()); }
  /*#p01.ZEME*/ uint8_t ZEME_AxCxExGx() const { return not1(ZAXY_xBxDxFxH()); }
  /* p29.XYVA*/ uint8_t XYVA_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); }
  /* p29.XOTA*/ uint8_t XOTA_AxCxExGx() const { return not1(XYVA_xBxDxFxH()); }
  /* p29.XYFY*/ uint8_t XYFY_xBxDxFxH() const { return not1(XOTA_AxCxExGx()); }
  /*#p01.ALET*/ uint8_t ALET_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); }
  /* p01.LAPE*/ uint8_t LAPE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*#p27.MEHE*/ uint8_t MEHE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*#p27.MYVO*/ uint8_t MYVO_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /* p27.MOXE*/ uint8_t MOXE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /* p27.TAVA*/ uint8_t TAVA_xBxDxFxH() const { return not1(LAPE_AxCxExGx()); }

  /*#p01.ATYP*/ uint8_t ATYP_ABCDxxxx() const { return not1(AFUR_xxxxEFGHp.qp_new()); }
  /*#p01.AFEP*/ uint8_t AFEP_AxxxxFGH() const { return not1(ALEF_AxxxxFGHp.qn_new()); }
  /*#p01.AROV*/ uint8_t AROV_xxCDEFxx() const { return not1(APUK_ABxxxxGHp.qp_new()); }
  /*#p01.ADAR*/ uint8_t ADAR_ABCxxxxH() const { return not1(ADYK_ABCxxxxHp.qn_new()); }

  /*#p01.BEKO*/ uint8_t BEKO_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); } // BEKO+BAVY parallel
  /*#p01.BAPY*/ uint8_t BAPY_xxxxxxGH() const { return nor3(ABOL_CLKREQn(), AROV_xxCDEFxx(), ATYP_ABCDxxxx()); }
  /*#p01.BERU*/ uint8_t BERU_ABCDEFxx() const { return not1(BAPY_xxxxxxGH()); }
  /*#p01.BUFA*/ uint8_t BUFA_xxxxxxGH() const { return not1(BERU_ABCDEFxx()); }
  /*#p01.BOLO*/ uint8_t BOLO_ABCDEFxx() const { return not1(BUFA_xxxxxxGH()); }
  /*#p01.BEJA*/ uint8_t BEJA_xxxxEFGH() const { return nand4(BOLO_ABCDEFxx(), BOLO_ABCDEFxx(), BEKO_ABCDxxxx(), BEKO_ABCDxxxx()); }
  /*#p01.BANE*/ uint8_t BANE_ABCDxxxx() const { return not1(BEJA_xxxxEFGH()); }
  /*#p01.BELO*/ uint8_t BELO_xxxxEFGH() const { return not1(BANE_ABCDxxxx()); }
  /*#p01.BAZE*/ uint8_t BAZE_ABCDxxxx() const { return not1(BELO_xxxxEFGH()); }
  /*#p01.BUTO*/ uint8_t BUTO_xBCDEFGH() const { return nand3(AFEP_AxxxxFGH(), ATYP_ABCDxxxx(), BAZE_ABCDxxxx()); }
  /*#p01.BELE*/ uint8_t BELE_Axxxxxxx() const { return not1(BUTO_xBCDEFGH()); }
  /*#p01.BYJU*/ uint8_t BYJU_Axxxxxxx() const { return or2(BELE_Axxxxxxx(), ATEZ_CLKBADp()); }
  /*#p01.BALY*/ uint8_t BALY_xBCDEFGH() const { return not1(BYJU_Axxxxxxx()); }
  /* p01.BOGA*/ uint8_t BOGA_Axxxxxxx() const { return not1(BALY_xBCDEFGH()); }
  /*#p01.BUVU*/ uint8_t BUVU_Axxxxxxx() const { return and2(BALY_xBCDEFGH(), BUTY_CLKREQp()); }
  /*#p01.BYXO*/ uint8_t BYXO_xBCDEFGH() const { return not1(BUVU_Axxxxxxx()); }
  /*#p01.BEDO*/ uint8_t BEDO_Axxxxxxx() const { return not1(BYXO_xBCDEFGH()); }
  /*#p01.BOWA*/ uint8_t BOWA_xBCDEFGH() const { return not1(BEDO_Axxxxxxx()); }
  /*#p01.BUGO*/ uint8_t BUGO_xBCDExxx() const { return not1(AFEP_AxxxxFGH()); }
  /*#p01.BATE*/ uint8_t BATE_AxxxxxGH() const { return nor3(BUGO_xBCDExxx(), AROV_xxCDEFxx(), ABOL_CLKREQn()); }
  /*#p01.BASU*/ uint8_t BASU_xBCDEFxx() const { return not1(BATE_AxxxxxGH()); }
  /*#p01.BUKE*/ uint8_t BUKE_AxxxxxGH() const { return not1(BASU_xBCDEFxx()); }
  /*#p01.BOMA*/ uint8_t BOMA_xBCDEFGH() const { return not1(BOGA_Axxxxxxx()); }
  /*#p01.BELU*/ uint8_t BELU_xxxxEFGH() const { return nor2(ATYP_ABCDxxxx(), ABOL_CLKREQn()); }
  /*#p01.BYRY*/ uint8_t BYRY_ABCDxxxx() const { return not1(BELU_xxxxEFGH()); }
  /*#p01.BUDE*/ uint8_t BUDE_xxxxEFGH() const { return not1(BYRY_ABCDxxxx()); }
  /* p01.UVYT*/ uint8_t UVYT_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); }
  /* p04.MOPA*/ uint8_t MOPA_xxxxEFGH() const { return not1(UVYT_ABCDxxxx()); }
  /* p28.XYNY*/ uint8_t XYNY_ABCDxxxx() const { return not1(MOPA_xxxxEFGH()); }

  /*#p21.TALU*/ uint8_t TALU_xxCDEFxx() const { return not1(VENA_xxCDEFxx.qn_new()); }
  /*#p29.XUPY*/ uint8_t XUPY_ABxxEFxx() const { return not1(WUVU_ABxxEFxx.qn_new()); }
  /*#p29.XOCE*/ uint8_t XOCE_xBCxxFGx() const { return not1(WOSU_AxxDExxH.qp_new()); }
  /*#p29.WOJO*/ uint8_t WOJO_AxxxExxx() const { return nor2(WOSU_AxxDExxH.qn_new(), WUVU_ABxxEFxx.qn_new()); }
  /*#p21.SONO*/ uint8_t SONO_ABxxxxGH() const { return not1(TALU_xxCDEFxx()); }
  /* p29.XYSO*/ uint8_t XYSO_xBCDxFGH() const { return not1(WOJO_AxxxExxx()); }
  /*#p30.CYKE*/ uint8_t CYKE_ABxxEFxx() const { return not1(XUPY_ABxxEFxx()); }
  /*#p30.WUDA*/ uint8_t WUDA_xxCDxxGH() const { return not1(CYKE_ABxxEFxx()); }
  /*#p28.AWOH*/ uint8_t AWOH_xxCDxxGH() const { return not1(XUPY_ABxxEFxx()); }
  /*#p01.AJAX*/ uint8_t AJAX_xxxxEFGH() const { return not1(ATYP_ABCDxxxx()); }

  void dump(Dumper& d) {
    d.dump_bitp("AFUR_xxxxEFGHp : ", AFUR_xxxxEFGHp.state);
    d.dump_bitp("ALEF_AxxxxFGHp : ", ALEF_AxxxxFGHp.state);
    d.dump_bitp("APUK_ABxxxxGHp : ", APUK_ABxxxxGHp.state);
    d.dump_bitp("ADYK_ABCxxxxHp : ", ADYK_ABCxxxxHp.state);
    d("\n");
    d.dump_bitp("WUVU_ABxxEFxxp : ", WUVU_ABxxEFxx.state);
    d.dump_bitp("VENA_xxCDEFxxp : ", VENA_xxCDEFxx.state);
    d.dump_bitp("WOSU_AxxDExxHp : ", WOSU_AxxDExxH.state);
  }

  /*p01.AFUR*/ DFF9 AFUR_xxxxEFGHp;
  /*p01.ALEF*/ DFF9 ALEF_AxxxxFGHp;
  /*p01.APUK*/ DFF9 APUK_ABxxxxGHp;
  /*p01.ADYK*/ DFF9 ADYK_ABCxxxxHp;

  /*p29.WUVU*/ DFF17 WUVU_ABxxEFxx;
  /*p21.VENA*/ DFF17 VENA_xxCDEFxx;
  /*p29.WOSU*/ DFF17 WOSU_AxxDExxH;

  PinIn  PIN74_CLKGOOD;
  PinIn  PIN74_CLK_IN;
  PinOut PIN75_CLK_OUT;

  Signal SIG_CPU_CLKREQ;
  Signal SIG_CPU_BOWA_Axxxxxxx; // top left port PORTD_01: <- this is the "put address on bus" clock
  Signal SIG_CPU_BEDO_xBCDEFGH; // top left port PORTD_02: <-
  Signal SIG_CPU_BEKO_ABCDxxxx; // top left port PORTD_03: <- this is the "reset for next cycle" clock
  Signal SIG_CPU_BUDE_xxxxEFGH; // top left port PORTD_04: <- this is the "put write data on bus" clock
  Signal SIG_CPU_BOLO_ABCDEFxx; // top left port PORTD_05: <-
  Signal SIG_CPU_BUKE_AxxxxxGH; // top left port PORTD_07: <- this is probably the "latch bus data" clock
  Signal SIG_CPU_BOMA_xBCDEFGH; // top left port PORTD_08: <- (RESET_CLK) // These two clocks are the only ones that run before SIG_CPU_READYp is asserted.
  Signal SIG_CPU_BOGA_Axxxxxxx; // top left port PORTD_09: <- test pad 3
};

//-----------------------------------------------------------------------------
