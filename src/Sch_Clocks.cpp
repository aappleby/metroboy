#include "Sch_Clocks.h"
#include "Sch_Top.h"

using namespace Schematics;

#define FAST_BOOT

//-----------------------------------------------------------------------------
void ClockRegisters::tick(const SchematicTop& top) {
  _XONA_LCDC_EN = top.pix_pipe.XONA_LCDC_EN.q();
}

void ClockRegisters::tock(const SchematicTop& top) {
  // ignoring the deglitcher here

  {
    wire AFUR_xBCDExxx_ = _AFUR_ABCDxxxx.q();
    wire ALEF_xxCDEFxx_ = _ALEF_xBCDExxx.q();
    wire APUK_xxxDEFGx_ = _APUK_xxCDEFxx.q();
    wire ADYK_xxxxEFGH_ = _ADYK_xxxDEFGx.q();

    // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

    /*p01.AFUR*/ _AFUR_ABCDxxxx = ff9(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), top.clk_reg.UPOJ_MODE_PRODn(), !ADYK_xxxxEFGH_);
    /*p01.ALEF*/ _ALEF_xBCDExxx = ff9( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), top.clk_reg.UPOJ_MODE_PRODn(),  AFUR_xBCDExxx_);
    /*p01.APUK*/ _APUK_xxCDEFxx = ff9(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), top.clk_reg.UPOJ_MODE_PRODn(),  ALEF_xxCDEFxx_);
    /*p01.ADYK*/ _ADYK_xxxDEFGx = ff9( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), top.clk_reg.UPOJ_MODE_PRODn(),  APUK_xxxDEFGx_);
  }

  {
    /*p29.XYVA*/ wire _XYVA_xBxDxFxH = not(ZEME_AxCxExGx());
    /*p29.XOTA*/ wire _XOTA_AxCxExGx = not(_XYVA_xBxDxFxH);
    /*p29.XYFY*/ wire _XYFY_xBxDxFxH = not(_XOTA_AxCxExGx);

    wire WUVU_xxCDxxGH_ = _WUVU_xxCDxxGH.q();
    wire VENA_xxxxEFGH_ = _VENA_xxxxEFGH.q();

    /*p29.WUVU*/ _WUVU_xxCDxxGH = ff17( _XOTA_AxCxExGx, top.clk_reg.XAPO_VID_RSTn(), !WUVU_xxCDxxGH_);
    /*p21.VENA*/ _VENA_xxxxEFGH = ff17(!WUVU_xxCDxxGH_, top.clk_reg.XAPO_VID_RSTn(), !VENA_xxxxEFGH_);
    /*p29.WOSU*/ _WOSU_xBCxxFGx = ff17( _XYFY_xBxDxFxH, top.clk_reg.XAPO_VID_RSTn(), !WUVU_xxCDxxGH_);
  }

  {
    /*p01.BUTY*/ wire BUTY_CLKREQ   = not(ABOL_CLKREQn());
    /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(BUTY_CLKREQ, BALY_Axxxxxxx());
    /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
    /*p01.BEDO*/ wire BEDO_Axxxxxxx = not(BYXO_xBCDEFGH);
    /*p01.BOWA*/ wire BOWA_xBCDEFGH = not(BEDO_Axxxxxxx);
    CPU_PIN_BOWA_xBCDEFGH = SignalState::from_wire(BOWA_xBCDEFGH);
    CPU_PIN_BEDO_Axxxxxxx = SignalState::from_wire(BEDO_Axxxxxxx);
  }
    
  CPU_PIN_BEKO_ABCDxxxx = SignalState::from_wire(BEKO_ABCDxxxx());
  CPU_PIN_BUDE_xxxxEFGH = SignalState::from_wire(BUDE_xxxxEFGH());
  CPU_PIN_BOLO_ABCDEFxx = SignalState::from_wire(BOLO_ABCDEFxx());
    
  {
    /*p01.AROV*/ wire AROV_xxCDEFxx = not(!APUK_xxCDEFxx());
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not(ALEF_xBCDExxx());
    /*p01.BUGO*/ wire BUGO_xBCDExxx = not(AFEP_AxxxxFGH);
    /*p01.BATE*/ wire BATE_AxxxxxGH = nor(ABOL_CLKREQn(),
                                          BUGO_xBCDExxx,
                                          AROV_xxCDEFxx);
    /*p01.BASU*/ wire BASU_xBCDEFxx = not(BATE_AxxxxxGH);
    /*p01.BUKE*/ wire BUKE_AxxxxxGH = not(BASU_xBCDEFxx);
    CPU_PIN_BUKE_AxxxxxGH = SignalState::from_wire(BUKE_AxxxxxGH);
  }

  CPU_PIN_BOMA_Axxxxxxx = SignalState::from_wire(BOMA_Axxxxxxx());
  CPU_PIN_BOGA_xBCDEFGH = SignalState::from_wire(BOGA_xBCDEFGH());

  {
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(AFUR_ABCDxxxx());
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn(), ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /* PIN_75 */ EXT_PIN_CLK = SignalState::from_wire(BUDE_AxxxxFGH);
  }

  CPU_PIN_EXT_CLKGOOD = SignalState::from_wire(SYS_PIN_CLK_A);

  /*p01.UPYF*/ wire _UPYF = or(_SYS_PIN_RSTp, top.clk_reg.UCOB_CLKBADp());

  /*p01.TUBO*/ _TUBO_CPU_READYn = nor_latch(_UPYF, CPU_PIN_READYp());

#ifdef FAST_BOOT
  // Just wait until DIV = 4 instead of DIV = 32768
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and (_TUBO_CPU_READYn.q(), top.tim_reg.UFOR_DIV_01());
#else
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and (_TUBO_CPU_READYn.q(), top.tim_reg.UPOF_DIV_15());
#endif
  /*p01.TABA*/ wire _TABA_POR_TRIGn = or(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);


  /*p01.ALYP*/ wire _ALYP_RSTn = not(_TABA_POR_TRIGn);
  /*p01.AFAR*/ wire _AFAR_RST  = nor(_ALYP_RSTn, _SYS_PIN_RSTp);

  /*p01.ASOL*/ _ASOL_POR_DONEn = nor_latch(_SYS_PIN_RSTp, _AFAR_RST); // Schematic wrong, this is a latch.

  wire boga = top.clk_reg.BOGA_xBCDEFGH();

  /*p01.AFER*/ _AFER_SYS_RSTp = ff13(boga, top.clk_reg.BOMA_Axxxxxxx(), UPOJ_MODE_PRODn(), _ASOL_POR_DONEn.q());

  _CPU_PIN_STARTp = SignalState::from_wire(_TABA_POR_TRIGn);
  _CPU_PIN_SYS_RSTp = SignalState::from_wire(AFER_SYS_RSTp());
  _CPU_PIN_EXT_RST = SignalState::from_wire(_SYS_PIN_RSTp);

  /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not(UNOR_MODE_DBG2p());
  /*p25.SOTO*/ SOTO_DBG = ff17(_SYCY_DBG_CLOCKn, CUNU_SYS_RSTn(), SOTO_DBG.qn());

  //cpu_pins.UMUT_MODE_DBG1.set(dbg_sig.UMUT_MODE_DBG1);
  //cpu_pins.UNOR_MODE_DBG2.set(dbg_sig.UNOR_MODE_DBG2n);

  CPU_PIN_UNOR_DBG = SignalState::from_wire(UNOR_MODE_DBG2p());
  CPU_PIN_UMUT_DBG = SignalState::from_wire(UMUT_MODE_DBG1p());

  //cpu_pins.UPOJ_MODE_PRODn = UPOJ_MODE_PRODn;
  //cpu_pins.TOVA_MODE_DBG2n = TOVA_MODE_DBG2n;
  //cpu_pins.RYCA_MODE_DBG2n = RYCA_MODE_DBG2n;
}

//-----------------------------------------------------------------------------

SignalHash ClockRegisters::commit() {
  SignalHash hash;

  hash << _XONA_LCDC_EN.commit();

  /*p01.TUBO*/ hash << _TUBO_CPU_READYn.commit();
  /*p01.ASOL*/ hash << _ASOL_POR_DONEn.commit(); // Schematic wrong, this is a latch.
  /*p01.AFER*/ hash << _AFER_SYS_RSTp.commit();

  hash << _CPU_PIN_STARTp.commit();
  hash << _CPU_PIN_READYp.commit_input();
  hash << _CPU_PIN_SYS_RSTp.commit();
  hash << _CPU_PIN_EXT_RST.commit();

  /*p25.SOTO*/ hash << SOTO_DBG.commit();
  ///*p07.BURO*/ hash << BURO_FF60_0.commit();
  ///*p07.AMUT*/ hash << AMUT_FF60_1.commit();

  hash << CPU_PIN_UNOR_DBG.commit();      // PORTA_02: <- P07.UNOR_MODE_DBG2
  hash << CPU_PIN_UMUT_DBG.commit();      // PORTA_05: <- P07.UMUT_MODE_DBG1

  hash << _SYS_PIN_RSTp.commit_input();
  hash << _SYS_PIN_T2n.commit_input();
  hash << _SYS_PIN_T1n.commit_input();

  hash << SYS_PIN_CLK_B.commit_input();
  hash << SYS_PIN_CLK_A.commit_input();

  hash << _AFUR_ABCDxxxx.commit();
  hash << _ALEF_xBCDExxx.commit();
  hash << _APUK_xxCDEFxx.commit();
  hash << _ADYK_xxxDEFGx.commit();

  hash << _WUVU_xxCDxxGH.commit();
  hash << _VENA_xxxxEFGH.commit();
  hash << _WOSU_xBCxxFGx.commit();

  // Edges on ABxxExGx

  // If pairs are differential and same sense
  // Pos edges on AB
  // Neg edges on ABEG

  hash << CPU_PIN_EXT_CLKGOOD.commit();   // PORTC_03: <- CLKIN_A
  hash << CPU_PIN_BOWA_xBCDEFGH.commit(); // PORTD_01: <- BOWA
  hash << CPU_PIN_BEDO_Axxxxxxx.commit(); // PORTD_02: <- BEDO

  hash << CPU_PIN_BEKO_ABCDxxxx.commit(); // PORTD_03: <- BEKO connection not indicated on P01
  hash << CPU_PIN_BUDE_xxxxEFGH.commit(); // PORTD_04: <- BUDE

  hash << CPU_PIN_BOLO_ABCDEFxx.commit(); // PORTD_05: <- BOLO
  hash << CPU_PIN_BUKE_AxxxxxGH.commit(); // PORTD_07: <- BUKE

  hash << CPU_PIN_BOMA_Axxxxxxx.commit(); // PORTD_08: <- BOMA
  hash << CPU_PIN_BOGA_xBCDEFGH.commit(); // PORTD_09: <- BOGA

  hash << EXT_PIN_CLK.commit();      // PIN_75 <- BUDE/BEVA

  return hash;
}

//-----------------------------------------------------------------------------

void ClockRegisters::dump(Dumper& d) {
  d("----------ClockRegisters----------\n");
  d("AFUR_ABCDxxxx %d\n", _AFUR_ABCDxxxx);
  d("ALEF_xBCDExxx %d\n", _ALEF_xBCDExxx);
  d("APUK_xxCDEFxx %d\n", _APUK_xxCDEFxx);
  d("ADYK_xxxDEFGx %d\n", _ADYK_xxxDEFGx);
}

//-----------------------------------------------------------------------------