#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyReset;

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoyClock {
  void reset_to_poweron() {
    memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
  }

  void reset_to_bootrom() {
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

  void reset_to_cart() {
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

  /*_p01.ABOL*/ wire ABOL_CLKREQn() const { return not1(SIG_CPU_CLKREQ.out_new()); }
  /*#p01.BUTY*/ wire BUTY_CLKREQp() const { return not1(ABOL_CLKREQn()); }

  wire AZOF_ODD() const {
    /*_p01.ATAL*/ wire ATAL_EVN = not1(AVET_DEGLITCH.out_mid());
    /*_p01.AZOF*/ wire AZOF_ODD = not1(ATAL_EVN);
    return AZOF_ODD;
  }

  /*_p01.ZAXY*/ wire ZAXY_EVN() const { return not1(AZOF_ODD()); }
  /*#p01.ZEME*/ wire ZEME_ODD() const { return not1(ZAXY_EVN()); }
  /*_p29.XYVA*/ wire XYVA_EVN() const { return not1(ZEME_ODD()); }
  /*_p29.XOTA*/ wire XOTA_ODD() const { return not1(XYVA_EVN()); }
  /*_p29.XYFY*/ wire XYFY_EVN() const { return not1(XOTA_ODD()); }
  /*#p01.ALET*/ wire ALET_evn() const { return not1(ZEME_ODD()); }
  /*_p01.LAPE*/ wire LAPE_odd() const { return not1(ALET_evn()); }
  /*#p27.MEHE*/ wire MEHE_odd() const { return not1(ALET_evn()); }
  /*#p27.MYVO*/ wire MYVO_odd() const { return not1(ALET_evn()); }
  /*_p27.MOXE*/ wire MOXE_ODD() const { return not1(ALET_evn()); }
  /*_p27.TAVA*/ wire TAVA_evn() const { return not1(LAPE_odd()); }

  /*#p01.ATYP*/ wire ATYP_ABCDxxxx() const { return not1(AFUR_xxxxEFGH.qp_new()); }
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH() const { return not1(ALEF_AxxxxFGH.qn_new()); }
  /*#p01.AROV*/ wire AROV_xxCDEFxx() const { return not1(APUK_ABxxxxGH.qp_new()); }
  /*#p01.ADAR*/ wire ADAR_ABCxxxxH() const { return not1(ADYK_ABCxxxxH.qn_new()); }

  /*#p01.BEKO*/ wire BEKO_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); } // BEKO+BAVY parallel
  /*#p01.BAPY*/ wire BAPY_xxxxxxGH() const { return nor3(ABOL_CLKREQn(), AROV_xxCDEFxx(), ATYP_ABCDxxxx()); }
  /*#p01.BERU*/ wire BERU_ABCDEFxx() const { return not1(BAPY_xxxxxxGH()); }
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH() const { return not1(BERU_ABCDEFxx()); }
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx() const { return not1(BUFA_xxxxxxGH()); }
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH() const { return nand4(BOLO_ABCDEFxx(), BOLO_ABCDEFxx(), BEKO_ABCDxxxx(), BEKO_ABCDxxxx()); }
  /*#p01.BANE*/ wire BANE_ABCDxxxx() const { return not1(BEJA_xxxxEFGH()); }
  /*#p01.BELO*/ wire BELO_xxxxEFGH() const { return not1(BANE_ABCDxxxx()); }
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx() const { return not1(BELO_xxxxEFGH()); }
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH() const { return nand3(AFEP_AxxxxFGH(), ATYP_ABCDxxxx(), BAZE_ABCDxxxx()); }
  /*#p01.BELE*/ wire BELE_Axxxxxxx() const { return not1(BUTO_xBCDEFGH()); }
  /*#p01.BUGO*/ wire BUGO_xBCDExxx() const { return not1(AFEP_AxxxxFGH()); }
  /*#p01.BATE*/ wire BATE_AxxxxxGH() const { return nor3(BUGO_xBCDExxx(), AROV_xxCDEFxx(), ABOL_CLKREQn()); }
  /*#p01.BASU*/ wire BASU_xBCDEFxx() const { return not1(BATE_AxxxxxGH()); }
  /*#p01.BUKE*/ wire BUKE_AxxxxxGH() const { return not1(BASU_xBCDEFxx()); }
  /*#p01.BELU*/ wire BELU_xxxxEFGH() const { return nor2(ATYP_ABCDxxxx(), ABOL_CLKREQn()); }
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx() const { return not1(BELU_xxxxEFGH()); }
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH() const { return not1(BYRY_ABCDxxxx()); }
  /*_p01.UVYT*/ wire UVYT_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); }
  /*_p04.MOPA*/ wire MOPA_xxxxEFGH() const { return not1(UVYT_ABCDxxxx()); }
  /*_p28.XYNY*/ wire XYNY_ABCDxxxx() const { return not1(MOPA_xxxxEFGH()); }

  /*#p21.TALU*/ wire TALU_xxCDEFxx() const { return not1(VENA_xxCDEFxx.qn_new()); }
  /*#p29.XUPY*/ wire XUPY_ABxxEFxx() const { return not1(WUVU_ABxxEFxx.qn_new()); }
  /*#p29.XOCE*/ wire XOCE_xBCxxFGx() const { return not1(WOSU_AxxDExxH.qp_new()); }
  /*#p29.WOJO*/ wire WOJO_AxxxExxx() const { return nor2(WOSU_AxxDExxH.qn_new(), WUVU_ABxxEFxx.qn_new()); }
  /*#p21.SONO*/ wire SONO_ABxxxxGH() const { return not1(TALU_xxCDEFxx()); }
  /*_p29.XYSO*/ wire XYSO_xBCDxFGH() const { return not1(WOJO_AxxxExxx()); }
  /*#p30.CYKE*/ wire CYKE_ABxxEFxx() const { return not1(XUPY_ABxxEFxx()); }
  /*#p30.WUDA*/ wire WUDA_xxCDxxGH() const { return not1(CYKE_ABxxEFxx()); }
  /*#p28.AWOH*/ wire AWOH_xxCDxxGH() const { return not1(XUPY_ABxxEFxx()); }
  /*#p01.AJAX*/ wire AJAX_xxxxEFGH() const { return not1(ATYP_ABCDxxxx()); }

  /*_p01.ANOS*/ Gate ANOS_DEGLITCH; // NAND latch deglitcher
  /*_p01.AVET*/ Gate AVET_DEGLITCH; // NAND latch deglitcher

  /*_p01.AFUR*/ DFF9 AFUR_xxxxEFGH;
  /*_p01.ALEF*/ DFF9 ALEF_AxxxxFGH;
  /*_p01.APUK*/ DFF9 APUK_ABxxxxGH;
  /*_p01.ADYK*/ DFF9 ADYK_ABCxxxxH;

  /*_p29.WUVU*/ DFF17 WUVU_ABxxEFxx;
  /*_p21.VENA*/ DFF17 VENA_xxCDEFxx;
  /*_p29.WOSU*/ DFF17 WOSU_AxxDExxH;

  /*_SIG_CPU_CLKREQ       */ SigIn  SIG_CPU_CLKREQ;
  /*_SIG_CPU_BOWA_Axxxxxxx*/ SigOut SIG_CPU_BOWA_Axxxxxxx; // top left port PORTD_01: <- this is the "put address on bus" clock
  /*_SIG_CPU_BEDO_xBCDEFGH*/ SigOut SIG_CPU_BEDO_xBCDEFGH; // top left port PORTD_02: <-
  /*_SIG_CPU_BEKO_ABCDxxxx*/ SigOut SIG_CPU_BEKO_ABCDxxxx; // top left port PORTD_03: <- this is the "reset for next cycle" clock
  /*_SIG_CPU_BUDE_xxxxEFGH*/ SigOut SIG_CPU_BUDE_xxxxEFGH; // top left port PORTD_04: <- this is the "put write data on bus" clock
  /*_SIG_CPU_BOLO_ABCDEFxx*/ SigOut SIG_CPU_BOLO_ABCDEFxx; // top left port PORTD_05: <-
  /*_SIG_CPU_BUKE_AxxxxxGH*/ SigOut SIG_CPU_BUKE_AxxxxxGH; // top left port PORTD_07: <- this is probably the "latch bus data" clock
  /*_SIG_CPU_BOMA_xBCDEFGH*/ SigOut SIG_CPU_BOMA_xBCDEFGH; // top left port PORTD_08: <- (RESET_CLK) // These two clocks are the only ones that run before SIG_CPU_READYp is asserted.
  /*_SIG_CPU_BOGA_Axxxxxxx*/ SigOut SIG_CPU_BOGA_Axxxxxxx; // top left port PORTD_09: <- test pad 3
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
