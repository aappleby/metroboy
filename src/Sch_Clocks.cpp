#include "Sch_Clocks.h"
#include "Sch_Top.h"

using namespace Schematics;

#if 0
// if rung 6 of AFUR/ALEF/APUK/ADYK is QN and not Q...

ADYK = !APUK
APUK = !ALEF
ALEF = !AFUR
AFUR = ADYK

ADYK APUK ALEF AFUR
0    0    0    0
1    1    1    0
0    0    1    1
1    0    0    0
1    1    1    0

// yeah doesn't work or make sense.
#endif

//-----------------------------------------------------------------------------

ClockSignals ClockRegisters::sig(const SchematicTop& gb) const {
  return sig(gb.cpu_bus, gb.EXT_PIN_CLK_GOOD);
}

ClockSignals ClockRegisters::sig(const CpuBus& cpu_bus, wire EXT_PIN_CLK_GOOD) const {
  ClockSignals clk_sig;

  ///*p01.ARYS*/ wire ARYS_xBxDxFxH = not(ext_pins.CLK); // ignoring the deglitcher here
  ///*p01.AVET*/ wire AVET_AxCxExGx = ext_pins.CLK;

  {
    /*p01.ANOS*/ clk_sig.ANOS_AxCxExGx = not(PIN_CLK_IN_xBxDxFxH);
    /*p29.WUVU*/ clk_sig.WUVU_AxxDExxH = WUVU_AxxDExxH;
    /*p21.VENA*/ clk_sig.VENA_xBCDExxx = VENA_xBCDExxx;
    /*p29.WOSU*/ clk_sig.WOSU_xxCDxxGH = WOSU_xxCDxxGH;
  }

  ///*p01.ATAG*/ wire ATAG_xBxDxFxH = not(AZOF_AxCxExGx);
  ///*p01.AMUK*/ wire AMUK_AxCxExGx = not(ATAG_xBxDxFxH); // apu master 4m clock, but apu isn't hooked up yet
  ///*p01.DOVA*/ wire DOVA_xBCDExxx = not(BUDE_AxxxxFGH); // and then this goes to channel 1

  {
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(cpu_bus.CPU_PIN_CLKREQ);
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!AFUR_xBCDExxx);
    /*p01.AROV*/ wire AROV_xxxDEFGx = not(!APUK_xxxDEFGx);
    /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(ADYK_xxxxEFGH);
    /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(ALEF_xxCDEFxx);
    /*p01.BUGO*/ wire BUGO_xxCDEFxx = not(AFEP_ABxxxxGH);
    /*p01.NULE*/ clk_sig.NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
    /*p01.AFAS*/ clk_sig.AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
    /*p01.BATE*/ clk_sig.BATE_ABxxxxxH = nor(ABOL_CLKREQn,  BUGO_xxCDEFxx, AROV_xxxDEFGx);
    /*p01.BAPY*/ clk_sig.BAPY_AxxxxxxH = nor(ABOL_CLKREQn,  AROV_xxxDEFGx, ATYP_xBCDExxx);
  }



  {
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!AFUR_xBCDExxx);
    /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(ALEF_xxCDEFxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(clk_sig.NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /*p01.BERU*/ wire BERU_xBCDEFGx = not(clk_sig.BAPY_AxxxxxxH);
    /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
    /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);
    /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH);
    /*p01.BEJA*/ wire BEJA_AxxxxFGH = nand(BOLO_xBCDEFGx, BEKO_xBCDExxx);
    /*p01.BANE*/ wire BANE_xBCDExxx = not(BEJA_AxxxxFGH);
    /*p01.BELO*/ wire BELO_AxxxxFGH = not(BANE_xBCDExxx);
    /*p01.BAZE*/ wire BAZE_xBCDExxx = not(BELO_AxxxxFGH);
    /*p01.BUTO*/ wire BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, BAZE_xBCDExxx);
    /*p01.BELE*/ wire BELE_xBxxxxxx = not(BUTO_AxCDEFGH);
    /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(EXT_PIN_CLK_GOOD);
    /*p01.BYJU*/ clk_sig.BYJU_AxCDEFGH = nor(BELE_xBxxxxxx, ATEZ_CLKBAD);
  }


  {
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(cpu_bus.CPU_PIN_CLKREQ);
    /*p01.BUTY*/ wire BUTY_CLKREQ   = not(ABOL_CLKREQn);
    /*p01.BALY*/ wire BALY_xBxxxxxx = not(clk_sig.BYJU_AxCDEFGH);
    /*p01.BUVU*/ clk_sig.BUVU_xBxxxxxx = and(BUTY_CLKREQ, BALY_xBxxxxxx);
  }

  return clk_sig;
}

//-----------------------------------------------------------------------------

void ClockRegisters::tick(const SchematicTop& gb) {
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  /*p01.ABOL*/ wire ABOL_CLKREQn  = not(gb.cpu_bus.CPU_PIN_CLKREQ);

  tick(ABOL_CLKREQn, rst_sig.XAPO_VID_RSTn, dbg_sig.UPOJ_MODE_PROD);
}

void ClockRegisters::tick(wire ABOL_CLKREQn, wire XAPO_VID_RSTn, wire UPOJ_MODE_PROD) {
  {
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(PIN_CLK_IN_xBxDxFxH);
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
    /*p01.AFUR*/ AFUR_xBCDExxx.set_duo(ATAL_xBxDxFxH, UPOJ_MODE_PROD, !ADYK_xxxxEFGH);
    /*p01.ALEF*/ ALEF_xxCDEFxx.set_duo(ATAL_xBxDxFxH, UPOJ_MODE_PROD,  AFUR_xBCDExxx);
    /*p01.APUK*/ APUK_xxxDEFGx.set_duo(ATAL_xBxDxFxH, UPOJ_MODE_PROD,  ALEF_xxCDEFxx);
    /*p01.ADYK*/ ADYK_xxxxEFGH.set_duo(ATAL_xBxDxFxH, UPOJ_MODE_PROD,  APUK_xxxDEFGx);
  }

  {
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(PIN_CLK_IN_xBxDxFxH);
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
    /*p29.XYVA*/ wire XYVA_AxCxExGx = not(ZEME_AxCxExGx);
    /*p29.XOTA*/ wire XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
    /*p29.XYFY*/ wire XYFY_AxCxExGx = not(XOTA_xBxDxFxH);

    /*p29.WUVU*/ WUVU_AxxDExxH.set(XOTA_xBxDxFxH,  XAPO_VID_RSTn, !WUVU_AxxDExxH);
    /*p21.VENA*/ VENA_xBCDExxx.set(!WUVU_AxxDExxH, XAPO_VID_RSTn, !VENA_xBCDExxx);
    /*p29.WOSU*/ WOSU_xxCDxxGH.set(XYFY_AxCxExGx,  XAPO_VID_RSTn, !WUVU_AxxDExxH);
  }

  //cpu_pins.CLK_GOOD.set(sys_pins.CLK_GOOD);

  // phases checked and ok
  // cpu_pins.BEDO_xBxxxxxx.set(clk_sig.BEDO_xBxxxxxx);
  // cpu_pins.BOWA_AxCDEFGH.set(clk_sig.BOWA_AxCDEFGH);
  // cpu_pins.BEKO_xBCDExxx.set(clk_sig.BEKO_xBCDExxx);
  // cpu_pins.BUDE_AxxxxFGH.set(clk_sig.BUDE_AxxxxFGH);
  // cpu_pins.BOLO_xBCDEFGx.set(clk_sig.BOLO_xBCDEFGx);
  // cpu_pins.BUKE_ABxxxxxH.set(clk_sig.BUKE_ABxxxxxH);
  // cpu_pins.BOGA_AxCDEFGH.set(clk_sig.BOGA_AxCDEFGH);
  // cpu_pins.BOMA_xBxxxxxx.set(clk_sig.BOMA_xBxxxxxx);

  {
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(AFUR_xBCDExxx.qn());
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn, ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /* PIN_75 */ PIN_PHI.set(BUDE_AxxxxFGH);
  }
}

//-----------------------------------------------------------------------------

SignalHash ClockRegisters::commit() {
  SignalHash hash;
  /* PIN_74 */ hash << PIN_CLK_IN_xBxDxFxH.clear_preset();
  /* PIN_75 */ hash << PIN_PHI.commit_pinout(); // <- BUDE/BEVA

  hash << AFUR_xBCDExxx.commit_duo();
  hash << ALEF_xxCDEFxx.commit_duo();
  hash << APUK_xxxDEFGx.commit_duo();
  hash << ADYK_xxxxEFGH.commit_duo();

  hash << WUVU_AxxDExxH.commit_reg();
  hash << VENA_xBCDExxx.commit_reg();
  hash << WOSU_xxCDxxGH.commit_reg();
  return hash;
}

//-----------------------------------------------------------------------------