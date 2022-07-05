#pragma once
#include "GateBoyLib/Regs.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoyClock {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  wire ABOL_CLKREQn_new() const;
  wire BUTY_CLKREQp_new() const;

  wire AZOF_odd_old() const;
  wire ZAXY_evn_old() const;
  wire ZEME_odd_old() const;
  wire XYVA_evn_old() const;
  wire XOTA_odd_old() const;
  wire XYFY_evn_old() const;
  wire ALET_evn_old() const;
  wire LAPE_odd_old() const;
  wire MEHE_odd_old() const;
  wire MYVO_odd_old() const;
  wire MOXE_odd_old() const;
  wire TAVA_evn_old() const;

  wire AZOF_odd_new() const;
  wire ZAXY_evn_new() const;
  wire ZEME_odd_new() const;
  wire XYVA_evn_new() const;
  wire XOTA_odd_new() const;
  wire XYFY_evn_new() const;
  wire ALET_evn_new() const;
  wire LAPE_odd_new() const;
  wire MEHE_odd_new() const;
  wire MYVO_odd_new() const;
  wire MOXE_odd_new() const;
  wire TAVA_evn_new() const;

  wire ATYP_ABCDxxxx_new() const;
  wire AFEP_AxxxxFGH_new() const;
  wire AROV_xxCDEFxx_new() const;
  wire ADAR_ABCxxxxH_new() const;

  wire BEKO_ABCDxxxx_new() const;
  wire BAPY_xxxxxxGH_new() const;
  wire BERU_ABCDEFxx_new() const;
  wire BUFA_xxxxxxGH_new() const;
  wire BOLO_ABCDEFxx_new() const;
  wire BEJA_xxxxEFGH_new() const;
  wire BANE_ABCDxxxx_new() const;
  wire BELO_xxxxEFGH_new() const;
  wire BAZE_ABCDxxxx_new() const;
  wire BUTO_xBCDEFGH_new() const;
  wire BELE_Axxxxxxx_new() const;
  wire BUGO_xBCDExxx_new() const;
  wire BATE_AxxxxxGH_new() const;
  wire BASU_xBCDEFxx_new() const;
  wire BUKE_AxxxxxGH_new() const;
  wire BELU_xxxxEFGH_new() const;
  wire BYRY_ABCDxxxx_new() const;
  wire BUDE_xxxxEFGH_new() const;
  wire UVYT_ABCDxxxx_new() const;
  wire MOPA_xxxxEFGH_new() const;
  wire XYNY_ABCDxxxx_new() const;

  wire TALU_xxCDEFxx_new() const;
  wire XUPY_ABxxEFxx_new() const;
  wire XOCE_xBCxxFGx_new() const;
  wire WOJO_AxxxExxx_new() const;
  wire SONO_ABxxxxGH_new() const;
  wire XYSO_xBCDxFGH_new() const;
  wire CYKE_ABxxEFxx_new() const;
  wire WUDA_xxCDxxGH_new() const;
  wire AWOH_xxCDxxGH_new() const;
  wire AJAX_xxxxEFGH_new() const;

  /*#p17.ABUR*/ wire ABUR_xxCDEFGx() const { return not1(BUKE_AxxxxxGH_new()); }
  /*#p17.BORY*/ wire BORY_ABxxxxxH() const { return not1(ABUR_xxCDEFGx()); }

  /*_p01.ATAG*/ wire ATAG_AxCxExGx() const { return not1(AZOF_odd_new()); }
  /*_p01.DOVA*/ wire DOVA_ABCDxxxx() const { return not1(BUDE_xxxxEFGH_new()); } // this goes to all the trigger regs

  /*_p01.AMUK*/ wire AMUK_xBxDxFxH() const { return not1(ATAG_AxCxExGx()); } // goes to clock generators and wave ram clock
  /*_p01.CYBO*/ wire CYBO_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }
  /*_p01.ARYF*/ wire ARYF_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }
  /*_p01.APUV*/ wire APUV_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }
  /*_p15.AZEG*/ wire AZEG_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }

  //----------------------------------------

  /*_p01.ANOS*/ Gate ANOS_DEGLITCH; // NAND latch deglitcher
  /*_p01.AVET*/ Gate AVET_DEGLITCH; // NAND latch deglitcher

  /*_p01.AFUR*/ DFF9B AFUR_xxxxEFGH;
  /*_p01.ALEF*/ DFF9B ALEF_AxxxxFGH;
  /*_p01.APUK*/ DFF9B APUK_ABxxxxGH;
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
