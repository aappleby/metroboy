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
  // Root clocks

  /* p01.UCOB*/ wire UCOB_CLKBADp() const { return not1b(PIN74_CLKGOOD.int_qp_new()); }
  /* p01.ATEZ*/ wire ATEZ_CLKBADp() const { return not1b(PIN74_CLKGOOD.int_qp_new()); }
  /* p01.ABOL*/ wire ABOL_CLKREQn() const { return not1b(SIG_CPU_CLKREQ.qp_new2()); }
  /*#p01.BUTY*/ wire BUTY_CLKREQp() const { return not1b(ABOL_CLKREQn()); }

  wire AZOF_AxCxExGx() const {
    /* p01.ATAL*/ wire _ATAL_xBxDxFxH = not1b(AVET.qp_new2());
    /* p01.AZOF*/ wire _AZOF_AxCxExGx = not1b(_ATAL_xBxDxFxH);
    return _AZOF_AxCxExGx;
  }

  /* p01.ZAXY*/ wire ZAXY_xBxDxFxH() const { return not1b(AZOF_AxCxExGx()); }
  /*#p01.ZEME*/ wire ZEME_AxCxExGx() const { return not1b(ZAXY_xBxDxFxH()); }
  /* p29.XYVA*/ wire XYVA_xBxDxFxH() const { return not1b(ZEME_AxCxExGx()); }
  /* p29.XOTA*/ wire XOTA_AxCxExGx() const { return not1b(XYVA_xBxDxFxH()); }
  /* p29.XYFY*/ wire XYFY_xBxDxFxH() const { return not1b(XOTA_AxCxExGx()); }
  /*#p01.ALET*/ wire ALET_xBxDxFxH() const { return not1b(ZEME_AxCxExGx()); }
  /* p01.LAPE*/ wire LAPE_AxCxExGx() const { return not1b(ALET_xBxDxFxH()); }
  /*#p27.MEHE*/ wire MEHE_AxCxExGx() const { return not1b(ALET_xBxDxFxH()); }
  /*#p27.MYVO*/ wire MYVO_AxCxExGx() const { return not1b(ALET_xBxDxFxH()); }
  /* p27.MOXE*/ wire MOXE_AxCxExGx() const { return not1b(ALET_xBxDxFxH()); }
  /* p27.TAVA*/ wire TAVA_xBxDxFxH() const { return not1b(LAPE_AxCxExGx()); }

  /*#p01.ATYP*/ wire ATYP_ABCDxxxx() const { return not1b(AFUR_xxxxEFGHp.qp_new2()); }
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH() const { return not1b(ALEF_AxxxxFGHp.qn_new2()); }
  /*#p01.AROV*/ wire AROV_xxCDEFxx() const { return not1b(APUK_ABxxxxGHp.qp_new2()); }
  /*#p01.ADAR*/ wire ADAR_ABCxxxxH() const { return not1b(ADYK_ABCxxxxHp.qn_new2()); }

  /*#p01.BEKO*/ wire BEKO_ABCDxxxx() const { return not1b(BUDE_xxxxEFGH()); } // BEKO+BAVY parallel
  /*#p01.BAPY*/ wire BAPY_xxxxxxGH() const { return nor3b(ABOL_CLKREQn(), AROV_xxCDEFxx(), ATYP_ABCDxxxx()); }
  /*#p01.BERU*/ wire BERU_ABCDEFxx() const { return not1b(BAPY_xxxxxxGH()); }
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH() const { return not1b(BERU_ABCDEFxx()); }
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx() const { return not1b(BUFA_xxxxxxGH()); }
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH() const { return nand4b(BOLO_ABCDEFxx(), BOLO_ABCDEFxx(), BEKO_ABCDxxxx(), BEKO_ABCDxxxx()); }
  /*#p01.BANE*/ wire BANE_ABCDxxxx() const { return not1b(BEJA_xxxxEFGH()); }
  /*#p01.BELO*/ wire BELO_xxxxEFGH() const { return not1b(BANE_ABCDxxxx()); }
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx() const { return not1b(BELO_xxxxEFGH()); }
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH() const { return nand3b(AFEP_AxxxxFGH(), ATYP_ABCDxxxx(), BAZE_ABCDxxxx()); }
  /*#p01.BELE*/ wire BELE_Axxxxxxx() const { return not1b(BUTO_xBCDEFGH()); }
  /*#p01.BYJU*/ wire BYJU_Axxxxxxx() const { return or2(BELE_Axxxxxxx(), ATEZ_CLKBADp()); }
  /*#p01.BALY*/ wire BALY_xBCDEFGH() const { return not1b(BYJU_Axxxxxxx()); }
  /* p01.BOGA*/ wire BOGA_Axxxxxxx() const { return not1b(BALY_xBCDEFGH()); }
  /*#p01.BUVU*/ wire BUVU_Axxxxxxx() const { return and2(BALY_xBCDEFGH(), BUTY_CLKREQp()); }
  /*#p01.BYXO*/ wire BYXO_xBCDEFGH() const { return not1b(BUVU_Axxxxxxx()); }
  /*#p01.BEDO*/ wire BEDO_Axxxxxxx() const { return not1b(BYXO_xBCDEFGH()); }
  /*#p01.BOWA*/ wire BOWA_xBCDEFGH() const { return not1b(BEDO_Axxxxxxx()); }
  /*#p01.BUGO*/ wire BUGO_xBCDExxx() const { return not1b(AFEP_AxxxxFGH()); }
  /*#p01.BATE*/ wire BATE_AxxxxxGH() const { return nor3b(BUGO_xBCDExxx(), AROV_xxCDEFxx(), ABOL_CLKREQn()); }
  /*#p01.BASU*/ wire BASU_xBCDEFxx() const { return not1b(BATE_AxxxxxGH()); }
  /*#p01.BUKE*/ wire BUKE_AxxxxxGH() const { return not1b(BASU_xBCDEFxx()); }
  /*#p01.BOMA*/ wire BOMA_xBCDEFGH() const { return not1b(BOGA_Axxxxxxx()); }
  /*#p01.BELU*/ wire BELU_xxxxEFGH() const { return nor2b(ATYP_ABCDxxxx(), ABOL_CLKREQn()); }
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx() const { return not1b(BELU_xxxxEFGH()); }
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH() const { return not1b(BYRY_ABCDxxxx()); }
  /* p01.UVYT*/ wire UVYT_ABCDxxxx() const { return not1b(BUDE_xxxxEFGH()); }
  /* p04.MOPA*/ wire MOPA_xxxxEFGH() const { return not1b(UVYT_ABCDxxxx()); }
  /* p28.XYNY*/ wire XYNY_ABCDxxxx() const { return not1b(MOPA_xxxxEFGH()); }

  /*#p21.TALU*/ wire TALU_xxCDEFxx() const { return not1b(VENA_xxCDEFxx.qn_new2()); }
  /*#p29.XUPY*/ wire XUPY_ABxxEFxx() const { return not1b(WUVU_ABxxEFxx.qn_new2()); }
  /*#p29.XOCE*/ wire XOCE_xBCxxFGx() const { return not1b(WOSU_AxxDExxH.qp_new2()); }
  /*#p29.WOJO*/ wire WOJO_AxxxExxx() const { return nor2b(WOSU_AxxDExxH.qn_new2(), WUVU_ABxxEFxx.qn_new2()); }
  /*#p21.SONO*/ wire SONO_ABxxxxGH() const { return not1b(TALU_xxCDEFxx()); }
  /* p29.XYSO*/ wire XYSO_xBCDxFGH() const { return not1b(WOJO_AxxxExxx()); }
  /*#p30.CYKE*/ wire CYKE_ABxxEFxx() const { return not1b(XUPY_ABxxEFxx()); }
  /*#p30.WUDA*/ wire WUDA_xxCDxxGH() const { return not1b(CYKE_ABxxEFxx()); }
  /*#p28.AWOH*/ wire AWOH_xxCDxxGH() const { return not1b(XUPY_ABxxEFxx()); }
  /*#p01.AJAX*/ wire AJAX_xxxxEFGH() const { return not1b(ATYP_ABCDxxxx()); }

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

  /* p01.ANOS*/ Gate ANOS; // NAND latch deglitcher
  /* p01.AVET*/ Gate AVET; // NAND latch deglitcher

  /* p01.AFUR*/ DFF9 AFUR_xxxxEFGHp;
  /* p01.ALEF*/ DFF9 ALEF_AxxxxFGHp;
  /* p01.APUK*/ DFF9 APUK_ABxxxxGHp;
  /* p01.ADYK*/ DFF9 ADYK_ABCxxxxHp;

  /* p29.WUVU*/ DFF17 WUVU_ABxxEFxx;
  /* p21.VENA*/ DFF17 VENA_xxCDEFxx;
  /* p29.WOSU*/ DFF17 WOSU_AxxDExxH;

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
