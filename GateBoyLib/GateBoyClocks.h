#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct GateBoyPhaseClock {

  //----------------------------------------
  // Root clocks - ignoring the deglitcher here

  /* p01.UCOB*/ wire UCOB_CLKBADp()  const { return  not1(PIN_SYS_CLKGOOD.qp_any()); }
  /* p01.ATEZ*/ wire ATEZ_CLKBADp()  const { return  not1(PIN_SYS_CLKGOOD.qp_any()); }
  /* p01.ABOL*/ wire ABOL_CLKREQn()  const { return  not1(PIN_SYS_CLKREQ.qp_any()); }

  /* p01.ATAL*/ wire ATAL_xBxDxFxH() const { return PIN_SYS_CLK.qp_new(); }
  /* p01.AZOF*/ wire AZOF_AxCxExGx() const { return not1(ATAL_xBxDxFxH()); }
  /* p01.ZAXY*/ wire ZAXY_xBxDxFxH() const { return not1(AZOF_AxCxExGx()); }
  /*#p01.ZEME*/ wire ZEME_AxCxExGx() const { return not1(ZAXY_xBxDxFxH()); }
  /* p29.XYVA*/ wire XYVA_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); }
  /* p29.XOTA*/ wire XOTA_AxCxExGx() const { return not1(XYVA_xBxDxFxH()); }
  /* p29.XYFY*/ wire XYFY_xBxDxFxH() const { return not1(XOTA_AxCxExGx()); }
  /*#p01.ALET*/ wire ALET_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); }
  /* p01.LAPE*/ wire LAPE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*#p27.MEHE*/ wire MEHE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /*#p27.MYVO*/ wire MYVO_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /* p27.MOXE*/ wire MOXE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); }
  /* p27.TAVA*/ wire TAVA_xBxDxFxH() const { return not1(LAPE_AxCxExGx()); }

  /*#p01.ATYP*/ wire ATYP_ABCDxxxx() const { return not1(AFUR_xxxxEFGHp.qp_new()); }
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH() const { return not1(ALEF_AxxxxFGHp.qn_new()); }
  /*#p01.AROV*/ wire AROV_xxCDEFxx() const { return not1(APUK_ABxxxxGHp.qp_new()); }
  /*#p01.ADAR*/ wire ADAR_ABCxxxxH() const { return not1(ADYK_ABCxxxxHp.qn_new()); }

  /*p01.AFUR*/ DFF9 AFUR_xxxxEFGHp;
  /*p01.ALEF*/ DFF9 ALEF_AxxxxFGHp;
  /*p01.APUK*/ DFF9 APUK_ABxxxxGHp;
  /*p01.ADYK*/ DFF9 ADYK_ABCxxxxHp;

  PinIn PIN_SYS_CLKGOOD;
  PinIn PIN_SYS_CLK;
  PinIn PIN_SYS_CLKREQ;

  PinOut PIN_CPU_BOWA_Axxxxxxx; // top left port PORTD_01: <- this is the "put address on bus" clock
  PinOut PIN_CPU_BEDO_xBCDEFGH; // top left port PORTD_02: <-
  PinOut PIN_CPU_BEKO_ABCDxxxx; // top left port PORTD_03: <- this is the "reset for next cycle" clock
  PinOut PIN_CPU_BUDE_xxxxEFGH; // top left port PORTD_04: <- this is the "put write data on bus" clock
  PinOut PIN_CPU_BOLO_ABCDEFxx; // top left port PORTD_05: <-
  PinOut PIN_CPU_BUKE_AxxxxxGH; // top left port PORTD_07: <- this is probably the "latch bus data" clock
  PinOut PIN_CPU_BOMA_xBCDEFGH; // top left port PORTD_08: <- (RESET_CLK) // These two clocks are the only ones that run before PIN_CPU_READYp is asserted.
  PinOut PIN_CPU_BOGA_Axxxxxxx; // top left port PORTD_09: <- test pad 3
};

//-----------------------------------------------------------------------------

struct GateBoyVideoClock {
  /*#p21.TALU*/ wire TALU_xxCDEFxx() const { return not1(VENA_xxCDEFxx.qn_new()); }
  /*#p29.XUPY*/ wire XUPY_ABxxEFxx() const { return not1(WUVU_ABxxEFxx.qn_new()); }
  /*#p29.XOCE*/ wire XOCE_xBCxxFGx() const { return not1(WOSU_AxxDExxH.qp_new()); }
  /*#p29.WOJO*/ wire WOJO_AxxxExxx() const { return nor2(WOSU_AxxDExxH.qn_new(), WUVU_ABxxEFxx.qn_new()); }
  /* p29.XYSO*/ wire XYSO_xBCDxFGH() const { return not1(WOJO_AxxxExxx()); }
  /*#p30.CYKE*/ wire CYKE_ABxxEFxx() const { return not1(XUPY_ABxxEFxx()); }
  /*#p30.WUDA*/ wire WUDA_xxCDxxGH() const { return not1(CYKE_ABxxEFxx()); }

  /*p29.WUVU*/ DFF17 WUVU_ABxxEFxx;
  /*p21.VENA*/ DFF17 VENA_xxCDEFxx;
  /*p29.WOSU*/ DFF17 WOSU_AxxDExxH;
};

//-----------------------------------------------------------------------------
