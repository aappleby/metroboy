#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ClockRegisters {

  void dump(Dumper& d, wire CLK) const;

  void tick_slow(const wire CLK, SchematicTop& top);
  void tock_clk_slow(wire RST, const wire CLK, wire CLKGOOD, SchematicTop& top);
  void tock_rst_slow(wire RST, wire CLKGOOD, SchematicTop& top);
  void tock_dbg_slow(SchematicTop& top);
  void tock_vid_slow(const wire CLK, SchematicTop& top);

  //-----------------------------------------------------------------------------
  // Clock input & 4 mhz clocks

  /*p01.UCOB*/ wire UCOB_CLKBADp(wire CLKGOOD)  const { return not1(CLKGOOD); }

  /*p01.ATAL*/ wire ATAL_xBxDxFxH(wire CLK) const { return CLK; } // ignoring the deglitcher here
  /*p01.AZOF*/ wire AZOF_AxCxExGx(wire CLK) const { return not1(ATAL_xBxDxFxH(CLK)); } // apu control
  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH(wire CLK) const { return not1(AZOF_AxCxExGx(CLK)); }
  /*p01.ZEME*/ wire ZEME_AxCxExGx(wire CLK) const { return not1(ZAXY_xBxDxFxH(CLK)); } // bus mux, sprite store
  /*p01.ALET*/ wire ALET_xBxDxFxH(wire CLK) const { return not1(ZEME_AxCxExGx(CLK)); } // ppu, sprite fetcher, sprite scanner, tile fetcher, window
  /*p27.MEHE*/ wire MEHE_AxCxExGx(wire CLK) const { return not1(ALET_xBxDxFxH(CLK)); } // window
  /*p27.MYVO*/ wire MYVO_AxCxExGx(wire CLK) const { return not1(ALET_xBxDxFxH(CLK)); } // tile fetcher

  //-----------------------------------------------------------------------------
  // Phase clocks

  /*p01.BYRY*/ wire BYRY_ABCDxxxx() const { return not1(BELU_xxxxEFGH); }
  /*p01.BUDE*/ wire BUDE_xxxxEFGH() const { return not1(BYRY_ABCDxxxx()); }
  /*p01.UVYT*/ wire UVYT_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); } // bus mux, dma
  /*p01.BEKO*/ wire BEKO_ABCDxxxx() const { return not1(BUDE_xxxxEFGH()); }
  /*p04.MOPA*/ wire MOPA_xxxxEFGH() const { return not1(UVYT_ABCDxxxx()); } // bus mux, dma
  /*p28.XYNY*/ wire XYNY_ABCDxxxx() const { return not1(MOPA_xxxxEFGH()); } // bus mux

  /*p01.BERU*/ wire BERU_ABCDEFxx() const { return not1(BAPY_xxxxxxGH); }
  /*p01.BUFA*/ wire BUFA_xxxxxxGH() const { return not1(BERU_ABCDEFxx()); }
  /*p01.BOLO*/ wire BOLO_ABCDEFxx() const { return not1(BUFA_xxxxxxGH()); }

  /*p01.BEJA*/ wire BEJA_xxxxEFGH() const { return nand2(BOLO_ABCDEFxx(),
                                                         BEKO_ABCDxxxx()); }
  /*p01.BANE*/ wire BANE_ABCDxxxx() const { return not1(BEJA_xxxxEFGH()); }
  /*p01.BELO*/ wire BELO_xxxxEFGH() const { return not1(BANE_ABCDxxxx()); }
  /*p01.BAZE*/ wire BAZE_ABCDxxxx() const { return not1(BELO_xxxxEFGH()); }

  /*p01.BUTO*/ wire BUTO_xBCDEFGH() const { return nand3(AFEP_AxxxxFGH,
                                                         ATYP_ABCDxxxx,
                                                         BAZE_ABCDxxxx()); }
  /*p01.BELE*/ wire BELE_Axxxxxxx() const { return not1(BUTO_xBCDEFGH()); }
  /*p01.BYJU*/ wire BYJU_xBCDEFGH(wire CLKGOOD) const {
    /* p01.ATEZ*/ wire ATEZ_CLKBAD  = not1(CLKGOOD);
    return nor2(BELE_Axxxxxxx(), ATEZ_CLKBAD);
  }
  /*p01.BALY*/ wire BALY_Axxxxxxx(wire CLKGOOD) const { return not1(BYJU_xBCDEFGH(CLKGOOD)); }
  /*p01.BOGA*/ wire BOGA_xBCDEFGH(wire CLKGOOD) const { return not1(BALY_Axxxxxxx(CLKGOOD)); } // joy rst tim
  /*p01.BOMA*/ wire BOMA_Axxxxxxx(wire CLKGOOD) const { return not1(BOGA_xBCDEFGH(CLKGOOD)); } // rst

  /*#p01.ADAR*/ wire ADAR_ABCxxxxH() const { return not1(ADYK_ABCxxxxH.qn()); }

  // ext_pin_wr, arev_cpu_wrn
  /*p01.AFAS*/ wire AFAS_xxxxEFGx() const {

    // AFAS := ???(ADAR_02, ATYP_02);

    // if this gate were (!A && B), it would produce xxxxEFGx, which looks
    // like the inverse of the WRn signal on the cart...

    return nor2(ADAR_ABCxxxxH(),
                ATYP_ABCDxxxx);
  }

  //-----------------------------------------------------------------------------
  // Vid clocks

  /*p29.WUVU*/ RegQN  WUVU_xxCDxxGH = REG_D0C0;
  /*p21.VENA*/ RegQN  VENA_ABCDxxxx = REG_D0C0;
  /*p29.WOSU*/ RegQPN WOSU_xBCxxFGx = REG_D0C0;

  /*p29.XUPY*/ wire XUPY_xxCDxxGH() const { return not1(WUVU_xxCDxxGH.qn()); } // def qn, bus mux, lcd, sprite scanner
  /*p21.TALU*/ wire TALU_ABCDxxxx() const { return not1(VENA_ABCDxxxx.qn()); } // def qn, lcd
  /*p29.XOCE*/ wire XOCE_AxxDExxH() const { return not1(WOSU_xBCxxFGx.qp());  } // def qp, bus mux
  /*p28.AWOH*/ wire AWOH_ABxxEFxx() const { return not1(XUPY_xxCDxxGH()); } // lcd
  /*p21.SONO*/ wire SONO_xxxxEFGH() const { return not1(TALU_ABCDxxxx()); } // lcd
  
  /*p29.WOJO*/ wire WOJO_xxCxxxGx() const {
    return nor2(WUVU_xxCDxxGH.qn(),  // def qn
                WOSU_xBCxxFGx.qn()); // def qn
  }

  //-----------------------------------------------------------------------------
  // POR state

  /*p01.TUBO*/ wire TUBO_CPU_READYn() const { return _TUBO_WAITINGp.tp(); }
  /*p01.ASOL*/ wire ASOL_POR_DONEn()  const { return _ASOL_POR_DONEn.tp(); }
  /*p01.AFER*/ wire AFER_SYS_RSTp()   const { return _AFER_SYS_RSTp.qp(); }

  //-----------------------------------------------------------------------------
  // Resets

  wire ALUR_SYS_RSTn() const { // used everywhere
    /*p01.AVOR*/ wire AVOR_SYS_RSTp = or2(_AFER_SYS_RSTp.qp(), _ASOL_POR_DONEn.tp());
    /*p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
    return ALUR_SYS_RSTn;
  }

  wire CUNU_SYS_RSTn() const { // tile fetcher, dma, maka, soto
    /*p01.DULA*/ wire DULA_SYS_RSTp = not1(ALUR_SYS_RSTn());
    /*p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
    return CUNU_SYS_RSTn;
  }

  /*p01.XORE*/ wire XORE_SYS_RSTp() const { return not1(CUNU_SYS_RSTn()); }
  /*p01.WALU*/ wire WALU_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*p01.WESY*/ wire WESY_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*p03.MULO*/ wire MULO_SYS_RSTn() const { return not1(ALUR_SYS_RSTn()); }

  wire XAPO_VID_RSTn() const {
    /*p01.XEBE*/ wire _XEBE_SYS_RSTn = not1(XORE_SYS_RSTp());

    // XODO := nand2(XONA_QN, XEBE) !!!
    /*p01.XODO*/ wire _XODO_VID_RSTp = nand2(_XEBE_SYS_RSTn, _XONA_LCDC_ENn_qn);
    /*p01.XAPO*/ wire _XAPO_VID_RSTn = not1(_XODO_VID_RSTp);
    return _XAPO_VID_RSTn;
  }
  /*p01.ATAR*/ wire ATAR_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not1(ATAR_VID_RSTp()); }
  /*p01.TOFU*/ wire TOFU_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*p01.ROSY*/ wire ROSY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*p01.AMYG*/ wire AMYG_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*p01.LYHA*/ wire LYHA_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*p01.LYFE*/ wire LYFE_VID_RSTn() const { return not1(LYHA_VID_RSTp()); }
  /*p01.PYRY*/ wire PYRY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }

  //-----------------------------------------------------------------------------

  Sig _XONA_LCDC_ENn_qn;
  /*p01.BELU*/ Sig BELU_xxxxEFGH;
  /*p01.BAPY*/ Sig BAPY_xxxxxxGH;

  /*p01.TUBO*/ Tri _TUBO_WAITINGp  = TRI_D1NP; // Must be 0 in run mode, otherwise we'd ping CPU_PIN_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ Tri _ASOL_POR_DONEn = TRI_D1NP; // Schematic wrong, this is a latch.
  /*p01.AFER*/ RegQP _AFER_SYS_RSTp  = REG_D0C0; // AFER should keep clocking even if CPU_PIN_CLKREQ = 0

  /*p01.AFUR*/ Reg AFUR_xxxxEFGH = REG_D0C0;
  /*p01.ALEF*/ Reg ALEF_AxxxxFGH = REG_D0C0;
  /*p01.APUK*/ Reg APUK_ABxxxxGH = REG_D0C0;
  /*p01.ADYK*/ Reg ADYK_ABCxxxxH = REG_D0C0;

  /*p01.AROV*/ Sig AROV_xxCDEFxx;
  /*p01.AFEP*/ Sig AFEP_AxxxxFGH;
  /*p01.ATYP*/ Sig ATYP_ABCDxxxx;
  /*p01.AJAX*/ Sig AJAX_xxxxEFGH;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics