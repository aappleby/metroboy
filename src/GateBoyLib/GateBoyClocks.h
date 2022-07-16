#pragma once
#include "GateBoyLib/Regs.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoyClock {
  void reset_to_cart();

  /*_p01.ABOL*/ wire ABOL_CLKREQn_new() const { return not1(SIG_CPU_CLKREQ.out_new()); }
  /*#p01.BUTY*/ wire BUTY_CLKREQp_new() const { return not1(ABOL_CLKREQn_new()); }

  wire AZOF_AxCxExGx_old() const {
    /*_p01.ATAL*/ wire ATAL_xBxDxFxH_old = not1(AVET_AxCxExGx.out_old());
    /*_p01.AZOF*/ wire AZOF_AxCxExGx_old = not1(ATAL_xBxDxFxH_old);
    return AZOF_AxCxExGx_old;
  }

  wire AZOF_AxCxExGx_new() const {
    /*_p01.ATAL*/ wire ATAL_xBxDxFxH_new = not1(AVET_AxCxExGx.out_new());
    /*_p01.AZOF*/ wire AZOF_AxCxExGx_new = not1(ATAL_xBxDxFxH_new);
    return AZOF_AxCxExGx_new;
  }


  /*_p01.ZAXY*/ wire ZAXY_xBxDxFxH_old() const { return not1(AZOF_AxCxExGx_old()); }
  /*#p01.ZEME*/ wire ZEME_AxCxExGx_old() const { return not1(ZAXY_xBxDxFxH_old()); }
  /*_p29.XYVA*/ wire XYVA_xBxDxFxH_old() const { return not1(ZEME_AxCxExGx_old()); }
  /*_p29.XOTA*/ wire XOTA_AxCxExGx_old() const { return not1(XYVA_xBxDxFxH_old()); }
  /*_p29.XYFY*/ wire XYFY_xBxDxFxH_old() const { return not1(XOTA_AxCxExGx_old()); }
  /*#p01.ALET*/ wire ALET_xBxDxFxH_old() const { return not1(ZEME_AxCxExGx_old()); }
  /*_p01.LAPE*/ wire LAPE_AxCxExGx_old() const { return not1(ALET_xBxDxFxH_old()); }
  /*#p27.MEHE*/ wire MEHE_AxCxExGx_old() const { return not1(ALET_xBxDxFxH_old()); }
  /*#p27.MYVO*/ wire MYVO_AxCxExGx_old() const { return not1(ALET_xBxDxFxH_old()); }
  /*_p27.MOXE*/ wire MOXE_AxCxExGx_old() const { return not1(ALET_xBxDxFxH_old()); }
  /*_p27.TAVA*/ wire TAVA_xBxDxFxH_old() const { return not1(LAPE_AxCxExGx_old()); }

  /*_p01.ZAXY*/ wire ZAXY_xBxDxFxH_new() const { return not1(AZOF_AxCxExGx_new()); }
  /*#p01.ZEME*/ wire ZEME_AxCxExGx_new() const { return not1(ZAXY_xBxDxFxH_new()); }
  /*_p29.XYVA*/ wire XYVA_xBxDxFxH_new() const { return not1(ZEME_AxCxExGx_new()); }
  /*_p29.XOTA*/ wire XOTA_AxCxExGx_new() const { return not1(XYVA_xBxDxFxH_new()); }
  /*_p29.XYFY*/ wire XYFY_xBxDxFxH_new() const { return not1(XOTA_AxCxExGx_new()); }
  /*#p01.ALET*/ wire ALET_xBxDxFxH_new() const { return not1(ZEME_AxCxExGx_new()); }
  /*_p01.LAPE*/ wire LAPE_AxCxExGx_new() const { return not1(ALET_xBxDxFxH_new()); }
  /*#p27.MEHE*/ wire MEHE_AxCxExGx_new() const { return not1(ALET_xBxDxFxH_new()); }
  /*#p27.MYVO*/ wire MYVO_AxCxExGx_new() const { return not1(ALET_xBxDxFxH_new()); }
  /*_p27.MOXE*/ wire MOXE_AxCxExGx_new() const { return not1(ALET_xBxDxFxH_new()); }
  /*_p27.TAVA*/ wire TAVA_xBxDxFxH_new() const { return not1(LAPE_AxCxExGx_new()); }

  /*#p01.ATYP*/ wire ATYP_ABCDxxxx_new() const { return not1(AFUR_ABCDxxxx.qn_newB()); }
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH_new() const { return not1(ALEF_xBCDExxx.qp_newB()); }
  /*#p01.AROV*/ wire AROV_xxCDEFxx_new() const { return not1(APUK_xxCDEFxx.qn_newB()); }
  /*#p01.ADAR*/ wire ADAR_ABCxxxxH_new() const { return not1(ADYK_xxxDEFGx.qp_newB()); }

  /*#p01.BEKO*/ wire BEKO_ABCDxxxx_new() const { return not1(BUDE_xxxxEFGH_new()); } // BEKO+BAVY parallel
  /*#p01.BAPY*/ wire BAPY_xxxxxxGH_new() const { return nor3(ABOL_CLKREQn_new(), AROV_xxCDEFxx_new(), ATYP_ABCDxxxx_new()); }
  /*#p01.BERU*/ wire BERU_ABCDEFxx_new() const { return not1(BAPY_xxxxxxGH_new()); }
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH_new() const { return not1(BERU_ABCDEFxx_new()); }
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx_new() const { return not1(BUFA_xxxxxxGH_new()); }
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH_new() const { return nand4(BOLO_ABCDEFxx_new(), BOLO_ABCDEFxx_new(), BEKO_ABCDxxxx_new(), BEKO_ABCDxxxx_new()); }
  /*#p01.BANE*/ wire BANE_ABCDxxxx_new() const { return not1(BEJA_xxxxEFGH_new()); }
  /*#p01.BELO*/ wire BELO_xxxxEFGH_new() const { return not1(BANE_ABCDxxxx_new()); }
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx_new() const { return not1(BELO_xxxxEFGH_new()); }
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH_new() const { return nand3(AFEP_AxxxxFGH_new(), ATYP_ABCDxxxx_new(), BAZE_ABCDxxxx_new()); }
  /*#p01.BELE*/ wire BELE_Axxxxxxx_new() const { return not1(BUTO_xBCDEFGH_new()); }
  /*#p01.BUGO*/ wire BUGO_xBCDExxx_new() const { return not1(AFEP_AxxxxFGH_new()); }
  /*#p01.BATE*/ wire BATE_AxxxxxGH_new() const { return nor3(BUGO_xBCDExxx_new(), AROV_xxCDEFxx_new(), ABOL_CLKREQn_new()); }
  /*#p01.BASU*/ wire BASU_xBCDEFxx_new() const { return not1(BATE_AxxxxxGH_new()); }
  /*#p01.BUKE*/ wire BUKE_AxxxxxGH_new() const { return not1(BASU_xBCDEFxx_new()); }
  /*#p01.BELU*/ wire BELU_xxxxEFGH_new() const { return nor2(ATYP_ABCDxxxx_new(), ABOL_CLKREQn_new()); }
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx_new() const { return not1(BELU_xxxxEFGH_new()); }
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH_new() const { return not1(BYRY_ABCDxxxx_new()); }
  /*_p01.UVYT*/ wire UVYT_ABCDxxxx_new() const { return not1(BUDE_xxxxEFGH_new()); }
  /*_p04.MOPA*/ wire MOPA_xxxxEFGH_new() const { return not1(UVYT_ABCDxxxx_new()); }
  /*_p28.XYNY*/ wire XYNY_ABCDxxxx_new() const { return not1(MOPA_xxxxEFGH_new()); }

  /*#p21.TALU*/ wire TALU_xxCDEFxx_new() const { return not1(VENA_xxCDEFxx.qn_new()); }
  /*#p29.XUPY*/ wire XUPY_ABxxEFxx_new() const { return not1(WUVU_ABxxEFxx.qn_new()); }
  /*#p29.XOCE*/ wire XOCE_xBCxxFGx_new() const { return not1(WOSU_AxxDExxH.qp_new()); }
  /*#p29.WOJO*/ wire WOJO_AxxxExxx_new() const { return nor2(WOSU_AxxDExxH.qn_new(), WUVU_ABxxEFxx.qn_new()); }
  /*#p21.SONO*/ wire SONO_ABxxxxGH_new() const { return not1(TALU_xxCDEFxx_new()); }
  /*_p29.XYSO*/ wire XYSO_xBCDxFGH_new() const { return not1(WOJO_AxxxExxx_new()); }
  /*#p30.CYKE*/ wire CYKE_ABxxEFxx_new() const { return not1(XUPY_ABxxEFxx_new()); }
  /*#p30.WUDA*/ wire WUDA_xxCDxxGH_new() const { return not1(CYKE_ABxxEFxx_new()); }
  /*#p28.AWOH*/ wire AWOH_xxCDxxGH_new() const { return not1(XUPY_ABxxEFxx_new()); }
  /*#p01.AJAX*/ wire AJAX_xxxxEFGH_new() const { return not1(ATYP_ABCDxxxx_new()); }


  /*#p17.ABUR*/ wire ABUR_xxCDEFGx() const { return not1(BUKE_AxxxxxGH_new()); }
  /*#p17.BORY*/ wire BORY_ABxxxxxH() const { return not1(ABUR_xxCDEFGx()); }

  /*_p01.ATAG*/ wire ATAG_AxCxExGx() const { return not1(AZOF_AxCxExGx_new()); }
  /*_p01.DOVA*/ wire DOVA_ABCDxxxx() const { return not1(BUDE_xxxxEFGH_new()); } // this goes to all the trigger regs

  /*_p01.AMUK*/ wire AMUK_xBxDxFxH() const { return not1(ATAG_AxCxExGx()); } // goes to clock generators and wave ram clock
  /*_p01.CYBO*/ wire CYBO_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }
  /*_p01.ARYF*/ wire ARYF_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }
  /*_p01.APUV*/ wire APUV_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }
  /*_p15.AZEG*/ wire AZEG_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }

  //----------------------------------------

  /*_p01.ANOS*/ Gate ANOS_xBxDxFxH; // NAND latch deglitcher
  /*_p01.AVET*/ Gate AVET_AxCxExGx; // NAND latch deglitcher

  /*_p01.AFUR*/ DFF9B AFUR_ABCDxxxx;
  /*_p01.ALEF*/ DFF9B ALEF_xBCDExxx;
  /*_p01.APUK*/ DFF9B APUK_xxCDEFxx;
  /*_p01.ADYK*/ DFF9B ADYK_xxxDEFGx;

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
