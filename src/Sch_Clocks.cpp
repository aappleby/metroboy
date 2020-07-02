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
  ClockSignals sig;

  auto& cpu_bus = gb.cpu_bus;
  auto& clk_reg = gb.clk_reg;

  ///*p01.ARYS*/ wire ARYS_xBxDxFxH = not(ext_pins.CLK); // ignoring the deglitcher here
  ///*p01.AVET*/ wire AVET_AxCxExGx = ext_pins.CLK;

  /*p01.ANOS*/ wire ANOS_AxCxExGx = not(PIN_CLK_IN_xBxDxFxH);
  /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(gb.EXT_PIN_CLK_GOOD);
  /*p01.ABOL*/ wire ABOL_CLKREQn  = not(cpu_bus.PIN_CLKREQ);
  /*p01.BUTY*/ wire BUTY_CLKREQ   = not(ABOL_CLKREQn);

  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(clk_reg.AFUR_PHAZ_xBCDExxx.qn());
  /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(clk_reg.ALEF_PHAZ_xxCDEFxx.q());
  /*p01.AROV*/ wire AROV_xxxDEFGx = not(clk_reg.APUK_PHAZ_xxxDEFGx.qn());
  /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(clk_reg.ADYK_PHAZ_xxxxEFGH.q());

  /*p01.ATAL*/ sig.ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
  /*p01.AZOF*/ sig.AZOF_AxCxExGx = not(sig.ATAL_xBxDxFxH);
  /*p01.ZAXY*/ sig.ZAXY_xBxDxFxH = not(sig.AZOF_AxCxExGx);
  /*p01.ZEME*/ sig.ZEME_AxCxExGx = not(sig.ZAXY_xBxDxFxH);
  /*p01.ALET*/ sig.ALET_xBxDxFxH = not(sig.ZEME_AxCxExGx);
  /*p27.MEHE*/ sig.MEHE_AxCxExGx = not(sig.ALET_xBxDxFxH);
  /*p27.MYVO*/ sig.MYVO_AxCxExGx = not(sig.ALET_xBxDxFxH);
  /*p01.LAPE*/ sig.LAPE_AxCxExGx = not(sig.ALET_xBxDxFxH);
  /*p27.TAVA*/ sig.TAVA_xBxDxFxH = not(sig.LAPE_AxCxExGx);
  /*p29.XYVA*/ sig.XYVA_AxCxExGx = not(sig.ZEME_AxCxExGx);
  /*p29.XOTA*/ sig.XOTA_xBxDxFxH = not(sig.XYVA_AxCxExGx);
  /*p29.XYFY*/ sig.XYFY_AxCxExGx = not(sig.XOTA_xBxDxFxH);

  ///*p01.ATAG*/ wire ATAG_xBxDxFxH = not(AZOF_AxCxExGx);
  ///*p01.AMUK*/ wire AMUK_AxCxExGx = not(ATAG_xBxDxFxH); // apu master 4m clock, but apu isn't hooked up yet
  ///*p01.DOVA*/ wire DOVA_xBCDExxx = not(BUDE_AxxxxFGH); // and then this goes to channel 1

  /*p29.XUPY*/ sig.XUPY_xBCxxFGx = not(clk_reg.WUVU_AxxDExxH);
  /*p29.XOCE*/ sig.XOCE_ABxxEFxx = not(clk_reg.WOSU_xxCDxxGH);
  /*p30.CYKE*/ sig.CYKE_AxxDExxH = not(sig.XUPY_xBCxxFGx);
  /*p30.WUDA*/ sig.WUDA_xBCxxFGx = not(sig.CYKE_AxxDExxH);
  /*p01.NULE*/ sig.NULE_AxxxxFGH = nor(ABOL_CLKREQn, ATYP_xBCDExxx);
  /*p01.BYRY*/ sig.BYRY_xBCDExxx = not(sig.NULE_AxxxxFGH);
  /*p01.BUDE*/ sig.BUDE_AxxxxFGH = not(sig.BYRY_xBCDExxx);
  /*p01.UVYT*/ sig.UVYT_xBCDExxx = not(sig.BUDE_AxxxxFGH);
  /*p04.MOPA*/ sig.MOPA_AxxxxFGH = not(sig.UVYT_xBCDExxx);
  /*p01.AFAS*/ sig.AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
  /*p01.BUGO*/ sig.BUGO_xxCDEFxx = not(AFEP_ABxxxxGH);
  /*p01.BATE*/ sig.BATE_ABxxxxxH = nor(ABOL_CLKREQn, sig.BUGO_xxCDEFxx, AROV_xxxDEFGx);
  /*p01.BASU*/ sig.BASU_xxCDEFGx = not(sig.BATE_ABxxxxxH);
  /*p01.BUKE*/ sig.BUKE_ABxxxxxH = not(sig.BASU_xxCDEFGx);
  /*p01.BAPY*/ sig.BAPY_AxxxxxxH = nor(ABOL_CLKREQn, AROV_xxxDEFGx, ATYP_xBCDExxx);
  /*p01.BERU*/ sig.BERU_xBCDEFGx = not(sig.BAPY_AxxxxxxH);
  /*p01.BUFA*/ sig.BUFA_AxxxxxxH = not(sig.BERU_xBCDEFGx);
  /*p01.BOLO*/ sig.BOLO_xBCDEFGx = not(sig.BUFA_AxxxxxxH);
  /*p01.BEKO*/ sig.BEKO_xBCDExxx = not(sig.BUDE_AxxxxFGH);
  /*p01.BEJA*/ sig.BEJA_AxxxxFGH = nand(sig.BOLO_xBCDEFGx, sig.BEKO_xBCDExxx);
  /*p01.BANE*/ sig.BANE_xBCDExxx = not(sig.BEJA_AxxxxFGH);
  /*p01.BELO*/ sig.BELO_AxxxxFGH = not(sig.BANE_xBCDExxx);
  /*p01.BAZE*/ sig.BAZE_xBCDExxx = not(sig.BELO_AxxxxFGH);
  /*p01.BUTO*/ sig.BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, sig.BAZE_xBCDExxx);
  /*p01.BELE*/ sig.BELE_xBxxxxxx = not(sig.BUTO_AxCDEFGH);
  /*p01.BYJU*/ sig.BYJU_AxCDEFGH = nor(sig.BELE_xBxxxxxx, ATEZ_CLKBAD);
  /*p01.BALY*/ sig.BALY_xBxxxxxx = not(sig.BYJU_AxCDEFGH);
  /*p01.BOGA*/ sig.BOGA_AxCDEFGH = not(sig.BALY_xBxxxxxx);

  /*p21.TALU*/ sig.TALU_xBCDExxx = not(!clk_reg.VENA_xBCDExxx);
  /*p21.SONO*/ sig.SONO_AxxxxFGH = not(sig.TALU_xBCDExxx);
  /*p28.AWOH*/ sig.AWOH_AxxDExxH = not(sig.XUPY_xBCxxFGx);
  /*p28.XYNY*/ sig.XYNY_xBCDExxx = not(sig.MOPA_AxxxxFGH);
  /*p01.BUVU*/ sig.BUVU_xBxxxxxx = and(BUTY_CLKREQ, sig.BALY_xBxxxxxx);
  /*p01.BYXO*/ sig.BYXO_AxCDEFGH = not(sig.BUVU_xBxxxxxx);
  /*p01.BEDO*/ sig.BEDO_xBxxxxxx = not(sig.BYXO_AxCDEFGH);

  /*p29.WOJO*/ sig.WOJO_xxxDxxxH = nor(!clk_reg.WUVU_AxxDExxH, !clk_reg.WOSU_xxCDxxGH);
  /*p29.XYSO*/ sig.XYSO_ABCxDEFx = not(sig.WOJO_xxxDxxxH);

  /*p17.ABUR*/ sig.ABUR_xxCDEFGx = not(sig.BUKE_ABxxxxxH);
  /*p17.BORY*/ sig.BORY_ABxxxxxH = not(sig.ABUR_xxCDEFGx);
  /*p01.BOMA*/ sig.BOMA_xBxxxxxx = not(sig.BOGA_AxCDEFGH);
  /*p01.BOWA*/ sig.BOWA_AxCDEFGH = not(sig.BEDO_xBxxxxxx);

  return sig;
}

//-----------------------------------------------------------------------------

void ClockRegisters::tick(const SchematicTop& gb) {
  {
    auto clk_sig = gb.clk_reg.sig(gb);
    auto dbg_sig = gb.dbg_reg.sig(gb);

    /*p01.AFUR*/ AFUR_PHAZ_xBCDExxx.set_duo(clk_sig.ATAL_xBxDxFxH, dbg_sig.UPOJ_MODE_PROD, !ADYK_PHAZ_xxxxEFGH);
    /*p01.ALEF*/ ALEF_PHAZ_xxCDEFxx.set_duo(clk_sig.ATAL_xBxDxFxH, dbg_sig.UPOJ_MODE_PROD,  AFUR_PHAZ_xBCDExxx);
    /*p01.APUK*/ APUK_PHAZ_xxxDEFGx.set_duo(clk_sig.ATAL_xBxDxFxH, dbg_sig.UPOJ_MODE_PROD,  ALEF_PHAZ_xxCDEFxx);
    /*p01.ADYK*/ ADYK_PHAZ_xxxxEFGH.set_duo(clk_sig.ATAL_xBxDxFxH, dbg_sig.UPOJ_MODE_PROD,  APUK_PHAZ_xxxDEFGx);
  }

  {
    auto clk_sig = gb.clk_reg.sig(gb);
    auto rst_sig = gb.rst_reg.sig(gb);

    /*p29.WUVU*/ WUVU_AxxDExxH.set(clk_sig.XOTA_xBxDxFxH, rst_sig.XAPO_VID_RSTn, !WUVU_AxxDExxH);
    /*p21.VENA*/ VENA_xBCDExxx.set(!WUVU_AxxDExxH, rst_sig.XAPO_VID_RSTn, !VENA_xBCDExxx);
    /*p29.WOSU*/ WOSU_xxCDxxGH.set(clk_sig.XYFY_AxCxExGx, rst_sig.XAPO_VID_RSTn, !WUVU_AxxDExxH);
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
    auto clk_sig = gb.clk_reg.sig(gb);
    /* PIN_75 */ PIN_PHI.set(clk_sig.BUDE_AxxxxFGH);
  }

}

//-----------------------------------------------------------------------------

SignalHash ClockRegisters::commit() {
  SignalHash hash;
  /* PIN_74 */ hash << PIN_CLK_IN_xBxDxFxH.clear_preset();
  /* PIN_75 */ hash << PIN_PHI.commit_pinout(); // <- BUDE/BEVA

  hash << AFUR_PHAZ_xBCDExxx.commit_duo();
  hash << ALEF_PHAZ_xxCDEFxx.commit_duo();
  hash << APUK_PHAZ_xxxDEFGx.commit_duo();
  hash << ADYK_PHAZ_xxxxEFGH.commit_duo();

  hash << WUVU_AxxDExxH.commit_reg();
  hash << VENA_xBCDExxx.commit_reg();
  hash << WOSU_xxCDxxGH.commit_reg();
  return hash;
}

//-----------------------------------------------------------------------------