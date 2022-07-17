#pragma once
#include "GateBoyLib/Regs.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoyClock {
  void reset_to_cart();

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
