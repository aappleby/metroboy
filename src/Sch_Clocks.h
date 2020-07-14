#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ClockRegisters {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top);
  SignalHash commit();

  void dump(Dumper& d);

  wire get_clk_a() const { return SYS_PIN_CLK_A.get(); }
  wire get_clk_b() const { return SYS_PIN_CLK_B.get(); }

  void set_clk_a(wire clk_a) { SYS_PIN_CLK_A.set(clk_a); }
  void set_clk_b(wire clk_b) { SYS_PIN_CLK_B.set(clk_b); }

  //----------------------------------------

  /*p01.UCOB*/ wire UCOB_CLKBADp()  const { return not(SYS_PIN_CLK_A); }

  /*p01.AFAS*/ wire AFAS_xxxxEFGx() const {  // AREV_CPU_WRn
    /*p01.ADAR*/ wire _ADAR_ABCxxxxH = not( _ADYK_xxxDEFGx.q());
    return nor(_ADAR_ABCxxxxH, ATYP_ABCDxxxx());
  }

  /*p21.TALU*/ wire TALU_ABCDxxxx() const { return not(VENA_xxxxEFGH()); } // lcd
  /*p21.SONO*/ wire SONO_xxxxEFGH() const { return not(TALU_ABCDxxxx()); } // lcd
  /*p28.AWOH*/ wire AWOH_xxCDxxGH() const { return not(XUPY_ABxxEFxx()); } // lcd
  
  /*p01.AZOF*/ wire AZOF_AxCxExGx() const { return not(ATAL_xBxDxFxH()); } // apu control

  /*p01.ZEME*/ wire ZEME_AxCxExGx() const { return not(ZAXY_xBxDxFxH()); } // bus mux, sprite store
  /*p01.ALET*/ wire ALET_xBxDxFxH() const { return not(ZEME_AxCxExGx()); } // ppu, sprite fetcher, sprite scanner, tile fetcher, window
  /*p27.MOXE*/ wire MOXE_AxCxExGx() const { return not(ALET_xBxDxFxH()); } // ppu
  /*p27.MEHE*/ wire MEHE_AxCxExGx() const { return not(ALET_xBxDxFxH()); } // window
  /*p27.MYVO*/ wire MYVO_AxCxExGx() const { return not(ALET_xBxDxFxH()); } // tile fetcher

  /*p29.XOCE*/ wire XOCE_AxxDExxH() const { return not(WOSU_xBCxxFGx()); } // bus mux
  /*p29.XUPY*/ wire XUPY_ABxxEFxx() const { return not(WUVU_xxCDxxGH()); } // bus mux, lcd, sprite scanner

  /*p01.UVYT*/ wire UVYT_ABCDxxxx() const { return not(BUDE_xxxxEFGH()); } // bus mux, dma
  /*p04.MOPA*/ wire MOPA_xxxxEFGH() const { return not(UVYT_ABCDxxxx()); } // bus mux, dma
  /*p28.XYNY*/ wire XYNY_ABCDxxxx() const { return not(MOPA_xxxxEFGH()); } // bus mux
  /*p29.XYSO*/ wire XYSO_ABCxDEFx() const { return not(WOJO_xxxDxxxH()); } // bus mux

  /*p01.BOGA*/ wire BOGA_xBCDEFGH() const { return not(BALY_Axxxxxxx()); } // joy rst tim
  /*p01.BOMA*/ wire BOMA_Axxxxxxx() const { return not(BOGA_xBCDEFGH()); } // rst

  // Exposed for debugging
  /*p01.AFUR*/ wire AFUR_ABCDxxxx() const { return _AFUR_ABCDxxxx.q(); }
  /*p01.ALEF*/ wire ALEF_xBCDExxx() const { return _ALEF_xBCDExxx.q(); }
  /*p01.APUK*/ wire APUK_xxCDEFxx() const { return _APUK_xxCDEFxx.q(); }
  /*p01.ADYK*/ wire ADYK_xxxDEFGx() const { return _ADYK_xxxDEFGx.q(); }

  /*p29.WUVU*/ wire WUVU_xxCDxxGH() const { return _WUVU_xxCDxxGH.q(); }
  /*p21.VENA*/ wire VENA_xxxxEFGH() const { return _VENA_xxxxEFGH.q(); }
  /*p29.WOSU*/ wire WOSU_xBCxxFGx() const { return _WOSU_xBCxxFGx.q(); }

  //----------------------------------------

private:
  Signal _ABOL_CLKREQn;

  /*p01.ATYP*/ wire ATYP_ABCDxxxx() const { return not(!_AFUR_ABCDxxxx.q()); }
  /*p01.ATAL*/ wire ATAL_xBxDxFxH() const { return SYS_PIN_CLK_B; } // ignoring the deglitcher here
  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH() const { return not(AZOF_AxCxExGx()); }

  /*p01.NULE*/ wire NULE_xxxxEFGH() const { return nor(_ABOL_CLKREQn,  ATYP_ABCDxxxx()); }
  /*p01.BYRY*/ wire BYRY_ABCDxxxx() const { return not(NULE_xxxxEFGH()); }
  /*p01.BUDE*/ wire BUDE_xxxxEFGH() const { return not(BYRY_ABCDxxxx()); }
  /*p01.BEKO*/ wire BEKO_ABCDxxxx() const { return not(BUDE_xxxxEFGH()); }
  
  /*p01.BAPY*/ wire BAPY_xxxxxxGH() const {
    /*p01.AROV*/ wire _AROV_xxCDEFxx = not(!_APUK_xxCDEFxx.q());
    return nor(_ABOL_CLKREQn, _AROV_xxCDEFxx, ATYP_ABCDxxxx());
  }
  /*p01.BERU*/ wire BERU_ABCDEFxx() const { return not(BAPY_xxxxxxGH()); }
  /*p01.BUFA*/ wire BUFA_xxxxxxGH() const { return not(BERU_ABCDEFxx()); }
  /*p01.BOLO*/ wire BOLO_ABCDEFxx() const { return not(BUFA_xxxxxxGH()); }
  /*p01.BEJA*/ wire BEJA_xxxxEFGH() const { return nand(BOLO_ABCDEFxx(), BEKO_ABCDxxxx()); }
  /*p01.BANE*/ wire BANE_ABCDxxxx() const { return not(BEJA_xxxxEFGH()); }
  /*p01.BELO*/ wire BELO_xxxxEFGH() const { return not(BANE_ABCDxxxx()); }
  /*p01.BAZE*/ wire BAZE_ABCDxxxx() const { return not(BELO_xxxxEFGH()); }
  /*p01.BUTO*/ wire BUTO_xBCDEFGH() const {
    /*p01.AFEP*/ wire _AFEP_AxxxxFGH = not( _ALEF_xBCDExxx.q());
    return nand(_AFEP_AxxxxFGH, ATYP_ABCDxxxx(), BAZE_ABCDxxxx());
  }
  /*p01.BELE*/ wire BELE_Axxxxxxx() const { return not(BUTO_xBCDEFGH()); }
  /*p01.BYJU*/ wire BYJU_xBCDEFGH() const {
    /*p01.ATEZ*/ wire _ATEZ_CLKBAD   = not(SYS_PIN_CLK_A);
    return nor(BELE_Axxxxxxx(), _ATEZ_CLKBAD);
  }
  /*p01.BALY*/ wire BALY_Axxxxxxx() const { return not(BYJU_xBCDEFGH()); }
  /*p29.WOJO*/ wire WOJO_xxxDxxxH() const { return nor(_WUVU_xxCDxxGH.qn(), _WOSU_xBCxxFGx.qn()); }

  /*p01.AFUR*/ Reg9 _AFUR_ABCDxxxx;
  /*p01.ALEF*/ Reg9 _ALEF_xBCDExxx;
  /*p01.APUK*/ Reg9 _APUK_xxCDEFxx;
  /*p01.ADYK*/ Reg9 _ADYK_xxxDEFGx;

  /*p29.WUVU*/ Reg17 _WUVU_xxCDxxGH;
  /*p21.VENA*/ Reg17 _VENA_xxxxEFGH;
  /*p29.WOSU*/ Reg17 _WOSU_xBCxxFGx;

  ExtPinIn  SYS_PIN_CLK_A;  // PIN_74 -> ATEZ, UCOB. Basically "clock good".
  ExtPinIn  SYS_PIN_CLK_B;  // PIN_74 

  CpuPinOut CPU_PIN_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  CpuPinOut CPU_PIN_BOWA_xBCDEFGH; // top left port PORTD_01: <- BOWA_AxCDEFGH // Blue clock - decoders, alu, some reset stuff
  CpuPinOut CPU_PIN_BEDO_Axxxxxxx; // top left port PORTD_02: <- BEDO_xBxxxxxx
  CpuPinOut CPU_PIN_BEKO_ABCDxxxx; // top left port PORTD_03: <- BEKO_ABCDxxxx + BAVY connection not indicated on P01 - test pad 1
  CpuPinOut CPU_PIN_BUDE_xxxxEFGH; // top left port PORTD_04: <- BUDE_AxxxxFGH + BEVA
  CpuPinOut CPU_PIN_BOLO_ABCDEFxx; // top left port PORTD_05: <- BOLO_ABCDEFxx + BYDA? - test pad 2
  CpuPinOut CPU_PIN_BUKE_AxxxxxGH; // top left port PORTD_07: <- BUKE_ABxxxxxH
  CpuPinOut CPU_PIN_BOMA_Axxxxxxx; // top left port PORTD_08: <- BOMA_xBxxxxxx (RESET_CLK)
  CpuPinOut CPU_PIN_BOGA_xBCDEFGH; // top left port PORTD_09: <- BOGA_AxCDEFGH - test pad 3
  ExtPinOut EXT_PIN_CLK;     // PIN_75 <- P01.BUDE/BEVA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics