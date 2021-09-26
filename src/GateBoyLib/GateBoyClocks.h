#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoyClock {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  wire ABOL_CLKREQn() const;
  wire BUTY_CLKREQp() const;

  wire AZOF_ODD() const;

  wire ZAXY_EVN() const;
  wire ZEME_ODD() const;
  wire XYVA_EVN() const;
  wire XOTA_ODD() const;
  wire XYFY_EVN() const;
  wire ALET_evn() const;
  wire LAPE_odd() const;
  wire MEHE_odd() const;
  wire MYVO_odd() const;
  wire MOXE_ODD() const;
  wire TAVA_evn() const;

  wire ATYP_ABCDxxxx() const;
  wire AFEP_AxxxxFGH() const;
  wire AROV_xxCDEFxx() const;
  wire ADAR_ABCxxxxH() const;

  wire BEKO_ABCDxxxx() const;
  wire BAPY_xxxxxxGH() const;
  wire BERU_ABCDEFxx() const;
  wire BUFA_xxxxxxGH() const;
  wire BOLO_ABCDEFxx() const;
  wire BEJA_xxxxEFGH() const;
  wire BANE_ABCDxxxx() const;
  wire BELO_xxxxEFGH() const;
  wire BAZE_ABCDxxxx() const;
  wire BUTO_xBCDEFGH() const;
  wire BELE_Axxxxxxx() const;
  wire BUGO_xBCDExxx() const;
  wire BATE_AxxxxxGH() const;
  wire BASU_xBCDEFxx() const;
  wire BUKE_AxxxxxGH() const;
  wire BELU_xxxxEFGH() const;
  wire BYRY_ABCDxxxx() const;
  wire BUDE_xxxxEFGH() const;
  wire UVYT_ABCDxxxx() const;
  wire MOPA_xxxxEFGH() const;
  wire XYNY_ABCDxxxx() const;

  wire TALU_xxCDEFxx() const;
  wire XUPY_ABxxEFxx() const;
  wire XOCE_xBCxxFGx() const;
  wire WOJO_AxxxExxx() const;
  wire SONO_ABxxxxGH() const;
  wire XYSO_xBCDxFGH() const;
  wire CYKE_ABxxEFxx() const;
  wire WUDA_xxCDxxGH() const;
  wire AWOH_xxCDxxGH() const;
  wire AJAX_xxxxEFGH() const;

  //----------------------------------------

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
