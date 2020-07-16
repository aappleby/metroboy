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

  wire get_rst() const {
    return _SYS_PIN_RSTp.get();
  }

  void set_rst(bool rst) {
    _SYS_PIN_RSTp.set(rst);
  }

  void set_cpu_ready(bool ready) {
    _CPU_PIN_READYp.set(ready);
  }

  void set_t1t2(bool t1, bool t2) {
    _SYS_PIN_T1n.set(t1);
    _SYS_PIN_T2n.set(t2);
  }

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

  //-----------------------------------------------------------------------------
  // Resets

  wire SYS_PIN_RSTp() const { return _SYS_PIN_RSTp.get(); }

  wire ALUR_SYS_RSTn() const { // used everywhere
    /*p01.AVOR*/ wire AVOR_SYS_RSTp = or(_AFER_SYS_RSTp.q(), _ASOL_POR_DONEn.q());
    /*p01.ALUR*/ wire ALUR_SYS_RSTn = not(AVOR_SYS_RSTp);
    return ALUR_SYS_RSTn;
  }

  wire CUNU_SYS_RSTn() const { // tile fetcher, dma, maka, soto
    /*p01.DULA*/ wire DULA_SYS_RSTp = not(ALUR_SYS_RSTn());
    /*p01.CUNU*/ wire CUNU_SYS_RSTn = not(DULA_SYS_RSTp);
    return CUNU_SYS_RSTn;
  }

  /*p01.XORE*/ wire XORE_SYS_RSTp() const { return not(CUNU_SYS_RSTn()); }
  /*p01.WALU*/ wire WALU_SYS_RSTn() const { return not(XORE_SYS_RSTp()); }
  /*p01.WESY*/ wire WESY_SYS_RSTn() const { return not(XORE_SYS_RSTp()); }
  /*p03.MULO*/ wire MULO_SYS_RSTn() const { return not(ALUR_SYS_RSTn()); }

  wire XAPO_VID_RSTn() const {
    /*p01.XEBE*/ wire _XEBE_SYS_RSTn = not(XORE_SYS_RSTp());
    /*p01.XODO*/ wire _XODO_VID_RSTp = nand(_XEBE_SYS_RSTn, _XONA_LCDC_EN);
    /*p01.XAPO*/ wire _XAPO_VID_RSTn = not(_XODO_VID_RSTp);
    return _XAPO_VID_RSTn;
  }
  /*p01.ATAR*/ wire ATAR_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not(ATAR_VID_RSTp()); }
  /*p01.TOFU*/ wire TOFU_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.ROSY*/ wire ROSY_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.AMYG*/ wire AMYG_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.LYHA*/ wire LYHA_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.LYFE*/ wire LYFE_VID_RSTn() const { return not(LYHA_VID_RSTp()); }
  /*p01.PYRY*/ wire PYRY_VID_RSTp() const { return not(XAPO_VID_RSTn()); }

  /*p01.AFER*/ wire AFER_SYS_RSTp() const { return _AFER_SYS_RSTp.q(); }

  //-----------------------------------------------------------------------------
  // Debug stuff

  wire CPU_PIN_STARTp() const { return _CPU_PIN_STARTp.get(); }
  wire CPU_PIN_READYp() const { return _CPU_PIN_READYp.get(); }

  /*p01.ABOL*/ wire ABOL_CLKREQn() const { return not(_CPU_PIN_READYp.get()); }

  /*p25.TUTO*/ wire TUTO_DBG_VRAMp() const { return and (UNOR_MODE_DBG2p(), SOTO_DBG.qn()); }
  /*p25.RACO*/ wire RACO_DBG_VRAMn() const { return not(TUTO_DBG_VRAMp()); }

  /*p07.UBET*/ wire UBET_T1p()        const { return not(_SYS_PIN_T1n); }
  /*p07.UVAR*/ wire UVAR_T2p()        const { return not(_SYS_PIN_T2n); }
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p() const {
    wire ret = and(_SYS_PIN_T1n, UVAR_T2p());
    return ret;
  }
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p() const {
    wire ret = and(_SYS_PIN_T2n, UBET_T1p());
    return ret;
  }
  /*p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand(UBET_T1p(), UVAR_T2p(), _SYS_PIN_RSTp); }
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not(UNOR_MODE_DBG2p()); }

  //-----------------------------------------------------------------------------

//private:
  Signal _XONA_LCDC_EN;

  ExtPinIn  _SYS_PIN_RSTp;   // PIN_71 -> UPOJ, UPYF, AFAR, ASOL, UFOL
  ExtPinIn  _SYS_PIN_T2n;    // PIN_76, tied to 0 on board - but there's probably an implicit inverter
  ExtPinIn  _SYS_PIN_T1n;    // PIN_77, tied to 0 on board - but there's probably an implicit inverter

  // Must be 0 in run mode, otherwise we'd ping CPU_PIN_DBG_RST when UPOF_DIV_15 changed
  /*p01.TUBO*/ Reg _TUBO_CPU_READYn;

  // If AVOR_RSTp was 1 in run mode
  // then ALUR_RSTn = 0
  // then MULO_TIMA_RST = 1
  // then PUXY/NERO = 0 -> stuck
  // Therefore AVOR_RSTp = 0 in run mode
  // Therefore ASOL|AFER = 0 in run mode

  /*p01.ASOL*/ Reg _ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ Reg    _AFER_SYS_RSTp; // AFER should keep clocking even if CPU_PIN_CLKREQ = 0

  CpuPinOut _CPU_PIN_STARTp;    // top center port PORTC_04: <- P01.CPU_RESET
  CpuPinIn  _CPU_PIN_READYp;    // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?

  CpuPinOut _CPU_PIN_SYS_RSTp;  // top center port PORTC_01: <- P01.AFER , reset related reg
  CpuPinOut _CPU_PIN_EXT_RST;   // top center port PORTC_02: <- PIN_RESET directly connected to the pad 

  /*p25.SOTO*/ Reg SOTO_DBG;

  CpuPinOut CPU_PIN_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  CpuPinOut CPU_PIN_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1


  /*p01.ATYP*/ wire ATYP_ABCDxxxx() const { return not(!_AFUR_ABCDxxxx.q()); }
  /*p01.ATAL*/ wire ATAL_xBxDxFxH() const { return SYS_PIN_CLK_B; } // ignoring the deglitcher here
  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH() const { return not(AZOF_AxCxExGx()); }

  /*p01.NULE*/ wire NULE_xxxxEFGH() const { return nor(ABOL_CLKREQn(),  ATYP_ABCDxxxx()); }
  /*p01.BYRY*/ wire BYRY_ABCDxxxx() const { return not(NULE_xxxxEFGH()); }
  /*p01.BUDE*/ wire BUDE_xxxxEFGH() const { return not(BYRY_ABCDxxxx()); }
  /*p01.BEKO*/ wire BEKO_ABCDxxxx() const { return not(BUDE_xxxxEFGH()); }
  
  /*p01.BAPY*/ wire BAPY_xxxxxxGH() const {
    /*p01.AROV*/ wire _AROV_xxCDEFxx = not(!_APUK_xxCDEFxx.q());
    return nor(ABOL_CLKREQn(), _AROV_xxCDEFxx, ATYP_ABCDxxxx());
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

  /*p01.AFUR*/ Reg _AFUR_ABCDxxxx;
  /*p01.ALEF*/ Reg _ALEF_xBCDExxx;
  /*p01.APUK*/ Reg _APUK_xxCDEFxx;
  /*p01.ADYK*/ Reg _ADYK_xxxDEFGx;

  /*p29.WUVU*/ Reg _WUVU_xxCDxxGH;
  /*p21.VENA*/ Reg _VENA_xxxxEFGH;
  /*p29.WOSU*/ Reg _WOSU_xBCxxFGx;

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