#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ClockRegisters {

  void dump(Dumper& d, const SchematicTop& top) const;

  void tick_slow(const SchematicTop& top);
  void tock_clk_slow(int phase, const SchematicTop& top);
  void tock_rst_slow(int phase, const SchematicTop& top);
  void tock_dbg_slow(int phase, const SchematicTop& top);
  void tock_vid_slow(int phase, const SchematicTop& top);

  wire get_clk_a() const { return _SYS_PIN_CLK_A.qp(); }
  wire get_clk_b() const { return _SYS_PIN_CLK_B.qp(); }

  void preset_clk_a(wire clk_a) { _SYS_PIN_CLK_A.preset(clk_a); }
  void preset_clk_b(wire clk_b) { _SYS_PIN_CLK_B.preset(clk_b); }

  wire get_rst() const {
    return _SYS_PIN_RSTp.qp();
  }

  void preset_rst(wire rst) {
    _SYS_PIN_RSTp.preset(rst);
  }

  void preset_cpu_ready(wire ready) {
    CPU_PIN_READYp.preset(ready);
  }

  void preset_t1t2(wire t1, wire t2) {
    _SYS_PIN_T1n.preset(t1);
    _SYS_PIN_T2n.preset(t2);
  }

  uint8_t get_phase_clock() {
    return (uint8_t)pack_p(AFUR_ABCDxxxx.qp(), ALEF_AxxxxFGH.qn(), APUK_xxCDEFxx.qp(), ADYK_ABCxxxxH.qn());
  }

  //-----------------------------------------------------------------------------
  // Clock input & 4 mhz clocks

  /*p01.UCOB*/ wire UCOB_CLKBADp()  const { return not1(_SYS_PIN_CLK_A.qp()); }
  /*p01.ATAL*/ wire ATAL_xBxDxFxH() const { return _SYS_PIN_CLK_B.qp(); } // ignoring the deglitcher here
  /*p01.AZOF*/ wire AZOF_AxCxExGx() const { return not1(ATAL_xBxDxFxH()); } // apu control
  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH() const { return not1(AZOF_AxCxExGx()); }
  /*p01.ZEME*/ wire ZEME_AxCxExGx() const { return not1(ZAXY_xBxDxFxH()); } // bus mux, sprite store
  /*p01.ALET*/ wire ALET_xBxDxFxH() const { return not1(ZEME_AxCxExGx()); } // ppu, sprite fetcher, sprite scanner, tile fetcher, window
  /*p27.MEHE*/ wire MEHE_AxCxExGx() const { return not1(ALET_xBxDxFxH()); } // window
  /*p27.MYVO*/ wire MYVO_AxCxExGx() const { return not1(ALET_xBxDxFxH()); } // tile fetcher

  //-----------------------------------------------------------------------------
  // Phase clocks

  /*p01.AFUR*/ Reg AFUR_ABCDxxxx = REG_D0C0;
  /*p01.ALEF*/ Reg ALEF_AxxxxFGH = REG_D0C0;
  /*p01.APUK*/ Reg APUK_xxCDEFxx = REG_D0C0;
  /*p01.ADYK*/ Reg ADYK_ABCxxxxH = REG_D0C0;

  /*p01.BELU*/ wire BELU_ABCDxxxx() const {
    return nor2(ABOL_CLKREQn,  ATYP_xxxxEFGH);
  }
  
  /*p01.BYRY*/ wire BYRY_xxxxEFGH() const {
    return not1(BELU_ABCDxxxx());
  }

  /*p01.BUDE*/ wire BUDE_ABCDxxxx() const { return not1(BYRY_xxxxEFGH()); }
  /*p01.UVYT*/ wire UVYT_xxxxEFGH() const { return not1(BUDE_ABCDxxxx()); } // bus mux, dma
  /*p01.BEKO*/ wire BEKO_xxxxEFGH() const { return not1(BUDE_ABCDxxxx()); }
  /*p04.MOPA*/ wire MOPA_ABCDxxxx() const { return not1(UVYT_xxxxEFGH()); } // bus mux, dma
  /*p28.XYNY*/ wire XYNY_xxxxEFGH() const { return not1(MOPA_ABCDxxxx()); } // bus mux

  /*p01.BAPY*/ wire BAPY_xxCDxxxx() const {
    // BAPY := ???(ABOL_02, AROV_02, ATYP_02);

    return nor3(ABOL_CLKREQn, AROV_ABxxxxGH, ATYP_xxxxEFGH);
  }

  /*p01.BERU*/ wire BERU_ABxxEFGH() const { return not1(BAPY_xxCDxxxx()); }
  /*p01.BUFA*/ wire BUFA_xxCDxxxx() const { return not1(BERU_ABxxEFGH()); }
  /*p01.BOLO*/ wire BOLO_ABxxEFGH() const { return not1(BUFA_xxCDxxxx()); }

  /*p01.BEJA*/ wire BEJA_ABCDxxxx() const { return nand2(BOLO_ABxxEFGH(), BEKO_xxxxEFGH()); }
  /*p01.BANE*/ wire BANE_xxxxEFGH() const { return not1(BEJA_ABCDxxxx()); }
  /*p01.BELO*/ wire BELO_ABCDxxxx() const { return not1(BANE_xxxxEFGH()); }
  /*p01.BAZE*/ wire BAZE_xxxxEFGH() const { return not1(BELO_ABCDxxxx()); }

  /*p01.BUTO*/ wire BUTO_ABCDExxx() const { return nand3(AFEP_AxxxxFGH, ATYP_xxxxEFGH, BAZE_xxxxEFGH()); }
  /*p01.BELE*/ wire BELE_xxxxxFGH() const { return not1(BUTO_ABCDExxx()); }
  /*p01.BYJU*/ wire BYJU_ABCDExxx() const { return nor2(BELE_xxxxxFGH(), ATEZ_CLKBAD); }
  /*p01.BALY*/ wire BALY_xxxxxFGH() const { return not1(BYJU_ABCDExxx()); }
  /*p01.BOGA*/ wire BOGA_ABCDExxx() const { return not1(BALY_xxxxxFGH()); } // joy rst tim
  /*p01.BOMA*/ wire BOMA_xxxxxFGH() const { return not1(BOGA_ABCDExxx()); } // rst

  /*p01.ADAR*/ wire ADAR_ABCxxxxH() const { return not1(ADYK_ABCxxxxH.qn()); }

  // ext_pin_wr, arev_cpu_wrn
  /*p01.AFAS*/ wire AFAS_xxxDxxxx() const {

    // AFAS := ???(ADAR_02, ATYP_02);

    // if this gate were (!A && B), it would produce xxxxEFGx, which looks
    // like the inverse of the WRn signal on the cart...

    return nor2(ADAR_ABCxxxxH(),
                ATYP_xxxxEFGH);
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

  /*p01.TUBO*/ wire TUBO_CPU_READYn() const { return _TUBO_WAITINGp.qp(); }
  /*p01.ASOL*/ wire ASOL_POR_DONEn()  const { return _ASOL_POR_DONEn.qp(); }
  /*p01.AFER*/ wire AFER_SYS_RSTp()   const { return _AFER_SYS_RSTp.qp(); }

  //-----------------------------------------------------------------------------
  // Resets

  wire SYS_PIN_RSTp() const { return _SYS_PIN_RSTp.qp(); }

  wire ALUR_SYS_RSTn() const { // used everywhere
    /*p01.AVOR*/ wire AVOR_SYS_RSTp = or2(_AFER_SYS_RSTp.qp(), _ASOL_POR_DONEn.qp());
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
    /*p01.XODO*/ wire _XODO_VID_RSTp = nand2(_XEBE_SYS_RSTn, _XONA_LCDC_EN);
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
  // Debug stuff

  /*p25.TUTO*/ wire TUTO_DBG_VRAMp()  const { return and2(UNOR_MODE_DBG2p(), _SOTO_DBG_VRAM.qn()); }
  /*p25.RACO*/ wire RACO_DBG_VRAMn()  const { return not1(TUTO_DBG_VRAMp()); }

  /*p07.UBET*/ wire UBET_T1p()        const { return not1(_SYS_PIN_T1n.qp()); }
  /*p07.UVAR*/ wire UVAR_T2p()        const { return not1(_SYS_PIN_T2n.qp()); }
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p() const { return and2(_SYS_PIN_T1n.qp(), UVAR_T2p()); }
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and2(_SYS_PIN_T2n.qp(), UBET_T1p()); }
  /*p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand3(UBET_T1p(), UVAR_T2p(), _SYS_PIN_RSTp.qp()); }
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }

  //-----------------------------------------------------------------------------

  Sig _XONA_LCDC_EN;

  Tri _SYS_PIN_CLK_A = TRI_D0NP; // PIN_74 -> ATEZ, UCOB. Basically "clock good".
  Tri _SYS_PIN_CLK_B = TRI_D0NP; // PIN_74 
  Tri _SYS_PIN_RSTp  = TRI_D1NP; // PIN_71 -> UPOJ, UPYF, AFAR, ASOL, UFOL
  Tri _SYS_PIN_T2n   = TRI_D1NP; // PIN_76, tied to 0 on board - but there's probably an implicit inverter
  Tri _SYS_PIN_T1n   = TRI_D1NP; // PIN_77, tied to 0 on board - but there's probably an implicit inverter

  // If AVOR_RSTp was 1 in run mode
  // then ALUR_RSTn = 0
  // then MULO_TIMA_RST = 1
  // then PUXY/NERO = 0 -> stuck
  // Therefore AVOR_RSTp = 0 in run mode
  // Therefore ASOL|AFER = 0 in run mode

  /*p01.TUBO*/ Tri _TUBO_WAITINGp  = TRI_D1NP; // Must be 0 in run mode, otherwise we'd ping CPU_PIN_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ Tri _ASOL_POR_DONEn = TRI_D1NP; // Schematic wrong, this is a latch.
  /*p01.AFER*/ RegQP _AFER_SYS_RSTp  = REG_D0C0; // AFER should keep clocking even if CPU_PIN_CLKREQ = 0
  /*p25.SOTO*/ RegQN _SOTO_DBG_VRAM  = REG_D0C0;

  /*p01.ATEZ*/ Sig ATEZ_CLKBAD;
  /*p01.ABOL*/ Sig ABOL_CLKREQn;
  /*p01.BUTY*/ Sig BUTY_CLKREQ;

  /*p01.AROV*/ Sig AROV_ABxxxxGH;
  /*p01.AFEP*/ Sig AFEP_AxxxxFGH;
  /*p01.ATYP*/ Sig ATYP_xxxxEFGH;
  /*p01.AJAX*/ Sig AJAX_ABCDxxxx;

  Tri CPU_PIN_STARTp   = TRI_HZNP;   // top center port PORTC_04: <- P01.CPU_RESET
  Tri CPU_PIN_READYp   = TRI_D0NP;   // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?
  Tri CPU_PIN_SYS_RSTp = TRI_HZNP;   // top center port PORTC_01: <- P01.AFER , reset related state
  Tri CPU_PIN_EXT_RST  = TRI_HZNP;   // top center port PORTC_02: <- PIN_RESET directly connected to the pad 
  Tri CPU_PIN_UNOR_DBG = TRI_HZNP;   // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  Tri CPU_PIN_UMUT_DBG = TRI_HZNP;   // top right port PORTA_05: <- P07.UMUT_MODE_DBG1

  Tri CPU_PIN_EXT_CLKGOOD = TRI_HZNP;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  
  Tri CPU_PIN_BOWA_ABCDExxx = TRI_HZNP; // top left port PORTD_01: // Blue clock - decoders, alu, some reset stuff
  Tri CPU_PIN_BEDO_xxxxxFGH = TRI_HZNP; // top left port PORTD_02:

  Tri CPU_PIN_BEKO_xxxxEFGH = TRI_HZNP; // top left port PORTD_03:
  Tri CPU_PIN_BUDE_ABCDxxxx = TRI_HZNP; // top left port PORTD_04: 

  Tri CPU_PIN_BOLO_ABxxEFGH = TRI_HZNP; // top left port PORTD_05: // CPU OEn? Would make sense with AFAS_xxxDxxxx as "WRen" I guess
  Tri CPU_PIN_BUKE_xxxxxFxx = TRI_HZNP; // top left port PORTD_07: // this is probably the "latch bus data" clock

  // These two clocks are the only ones that run before CPU_PIN_READYp is asserted.
  Tri CPU_PIN_BOMA_xxxxxFGH = TRI_HZNP; // top left port PORTD_08: (RESET_CLK)
  Tri CPU_PIN_BOGA_ABCDExxx = TRI_HZNP; // top left port PORTD_09: - test pad 3
  
  Tri EXT_PIN_CLK_ABCDxxxx = TRI_HZNP;     // PIN_75 <- P01.BUDE/BEVA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics