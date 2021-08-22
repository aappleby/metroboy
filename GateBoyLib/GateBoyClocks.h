#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyResetDebug;

//-----------------------------------------------------------------------------

struct GateBoyClock {

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

  /*_PIN_73*/ PinOut   PIN_73_CLK_DRIVE;
  /*_PIN_74*/ PinClock PIN_74_CLK;
  /*_PIN_75*/ PinOut   PIN_75_CLK_OUT;

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

//-----------------------------------------------------------------------------
