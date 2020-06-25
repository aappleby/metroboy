#include "Sch_Clocks.h"
#include "TestGB.h"

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


ClockSignals ClockRegisters::sig(const TestGB& test_gb) const {
  ///*p01.ARYS*/ wire ARYS_xBxDxFxH = not(ext_pins.CLK); // ignoring the deglitcher here
  ///*p01.AVET*/ wire AVET_AxCxExGx = ext_pins.CLK;

  /*p01.ANOS*/ wire _ANOS_AxCxExGx = not(test_gb.sys_pins.CLK_IN_xBxDxFxH);
  /*p01.ATEZ*/ wire _ATEZ_CLKBAD = not(test_gb.sys_pins.CLK_GOOD);
  /*p01.ABOL*/ wire _ABOL_CLKREQn = not(test_gb.cpu_pins.CLKREQ);
  /*p01.BUTY*/ wire _BUTY_CLKREQ = not(_ABOL_CLKREQn);

  /*p01.ATYP*/ wire _ATYP_xBCDExxx = not(test_gb.clk_reg.AFUR_PHAZ_xBCDExxx.qn());
  /*p01.AFEP*/ wire _AFEP_ABxxxxGH = not(test_gb.clk_reg.ALEF_PHAZ_xxCDEFxx.q());
  /*p01.AROV*/ wire _AROV_xxxDEFGx = not(test_gb.clk_reg.APUK_PHAZ_xxxDEFGx.qn());
  /*p01.ADAR*/ wire _ADAR_ABCDxxxx = not(test_gb.clk_reg.ADYK_PHAZ_xxxxEFGH.q());

  /*p01.ATAL*/ wire _ATAL_xBxDxFxH = not(_ANOS_AxCxExGx);
  /*p01.AZOF*/ wire _AZOF_AxCxExGx = not(_ATAL_xBxDxFxH);
  /*p01.ZAXY*/ wire _ZAXY_xBxDxFxH = not(_AZOF_AxCxExGx);
  /*p01.ZEME*/ wire _ZEME_AxCxExGx = not(_ZAXY_xBxDxFxH);
  /*p01.ALET*/ wire _ALET_xBxDxFxH = not(_ZEME_AxCxExGx);
  /*p27.MEHE*/ wire _MEHE_AxCxExGx = not(_ALET_xBxDxFxH);
  /*p27.MYVO*/ wire _MYVO_AxCxExGx = not(_ALET_xBxDxFxH);
  /*p01.LAPE*/ wire _LAPE_AxCxExGx = not(_ALET_xBxDxFxH);
  /*p27.TAVA*/ wire _TAVA_xBxDxFxH = not(_LAPE_AxCxExGx);
  /*p29.XYVA*/ wire _XYVA_AxCxExGx = not(_ZEME_AxCxExGx);
  /*p29.XOTA*/ wire _XOTA_xBxDxFxH = not(_XYVA_AxCxExGx);
  /*p29.XYFY*/ wire _XYFY_AxCxExGx = not(_XOTA_xBxDxFxH);

  ///*p01.ATAG*/ wire ATAG_xBxDxFxH = not(AZOF_AxCxExGx);
  ///*p01.AMUK*/ wire AMUK_AxCxExGx = not(ATAG_xBxDxFxH); // apu master 4m clock, but apu isn't hooked up yet
  ///*p01.DOVA*/ wire DOVA_xBCDExxx = not(BUDE_AxxxxFGH); // and then this goes to channel 1

  /*p29.XUPY*/ wire _XUPY_xBCxxFGx = not(test_gb.clk_reg.WUVU_AxxDExxH);
  /*p29.XOCE*/ wire _XOCE_ABxxEFxx = not(test_gb.clk_reg.WOSU_xxCDxxGH);
  /*p30.CYKE*/ wire _CYKE_AxxDExxH = not(_XUPY_xBCxxFGx);
  /*p30.WUDA*/ wire _WUDA_xBCxxFGx = not(_CYKE_AxxDExxH);
  /*p01.NULE*/ wire _NULE_AxxxxFGH = nor(_ABOL_CLKREQn, _ATYP_xBCDExxx);
  /*p01.BYRY*/ wire _BYRY_xBCDExxx = not(_NULE_AxxxxFGH);
  /*p01.BUDE*/ wire _BUDE_AxxxxFGH = not(_BYRY_xBCDExxx);
  /*p01.UVYT*/ wire _UVYT_xBCDExxx = not(_BUDE_AxxxxFGH);
  /*p04.MOPA*/ wire _MOPA_AxxxxFGH = not(_UVYT_xBCDExxx);
  /*p01.AFAS*/ wire _AFAS_xxxxxFGH = nor(_ADAR_ABCDxxxx, _ATYP_xBCDExxx);
  /*p01.BUGO*/ wire _BUGO_xxCDEFxx = not(_AFEP_ABxxxxGH);
  /*p01.BATE*/ wire _BATE_ABxxxxxH = nor(_ABOL_CLKREQn, _BUGO_xxCDEFxx, _AROV_xxxDEFGx);
  /*p01.BASU*/ wire _BASU_xxCDEFGx = not(_BATE_ABxxxxxH);
  /*p01.BUKE*/ wire _BUKE_ABxxxxxH = not(_BASU_xxCDEFGx);
  /*p01.BAPY*/ wire _BAPY_AxxxxxxH = nor(_ABOL_CLKREQn, _AROV_xxxDEFGx, _ATYP_xBCDExxx);
  /*p01.BERU*/ wire _BERU_xBCDEFGx = not(_BAPY_AxxxxxxH);
  /*p01.BUFA*/ wire _BUFA_AxxxxxxH = not(_BERU_xBCDEFGx);
  /*p01.BOLO*/ wire _BOLO_xBCDEFGx = not(_BUFA_AxxxxxxH);
  /*p01.BEKO*/ wire _BEKO_xBCDExxx = not(_BUDE_AxxxxFGH);
  /*p01.BEJA*/ wire _BEJA_AxxxxFGH = nand(_BOLO_xBCDEFGx, _BEKO_xBCDExxx);
  /*p01.BANE*/ wire _BANE_xBCDExxx = not(_BEJA_AxxxxFGH);
  /*p01.BELO*/ wire _BELO_AxxxxFGH = not(_BANE_xBCDExxx);
  /*p01.BAZE*/ wire _BAZE_xBCDExxx = not(_BELO_AxxxxFGH);
  /*p01.BUTO*/ wire _BUTO_AxCDEFGH = nand(_AFEP_ABxxxxGH, _ATYP_xBCDExxx, _BAZE_xBCDExxx);
  /*p01.BELE*/ wire _BELE_xBxxxxxx = not(_BUTO_AxCDEFGH);
  /*p01.BYJU*/ wire _BYJU_AxCDEFGH = nor(_BELE_xBxxxxxx, _ATEZ_CLKBAD);
  /*p01.BALY*/ wire _BALY_xBxxxxxx = not(_BYJU_AxCDEFGH);
  /*p01.BOGA*/ wire _BOGA_AxCDEFGH = not(_BALY_xBxxxxxx);

  /*p21.TALU*/ wire _TALU_xBCDExxx = not(!test_gb.clk_reg.VENA_xBCDExxx);
  /*p21.SONO*/ wire _SONO_AxxxxFGH = not(_TALU_xBCDExxx);
  /*p28.AWOH*/ wire _AWOH_AxxDExxH = not(_XUPY_xBCxxFGx);
  /*p28.XYNY*/ wire _XYNY_xBCDExxx = not(_MOPA_AxxxxFGH);
  /*p01.BUVU*/ wire _BUVU_xBxxxxxx = and(_BUTY_CLKREQ, _BALY_xBxxxxxx);
  /*p01.BYXO*/ wire _BYXO_AxCDEFGH = not(_BUVU_xBxxxxxx);
  /*p01.BEDO*/ wire _BEDO_xBxxxxxx = not(_BYXO_AxCDEFGH);

  /*p29.WOJO*/ wire _WOJO_xxxDxxxH = nor(!test_gb.clk_reg.WUVU_AxxDExxH, !test_gb.clk_reg.WOSU_xxCDxxGH);
  /*p29.XYSO*/ wire _XYSO_ABCxDEFx = not(_WOJO_xxxDxxxH);

  /*p17.ABUR*/ wire _ABUR_xxCDEFGx = not(_BUKE_ABxxxxxH);
  /*p17.BORY*/ wire _BORY_ABxxxxxH = not(_ABUR_xxCDEFGx);
  /*p01.BOMA*/ wire _BOMA_xBxxxxxx = not(_BOGA_AxCDEFGH);
  /*p01.BOWA*/ wire _BOWA_AxCDEFGH = not(_BEDO_xBxxxxxx);


  return {
    .ATAL_xBxDxFxH = _ATAL_xBxDxFxH,
    .AZOF_AxCxExGx = _AZOF_AxCxExGx,
    .ZAXY_xBxDxFxH = _ZAXY_xBxDxFxH,
    .ZEME_AxCxExGx = _ZEME_AxCxExGx,
    .ALET_xBxDxFxH = _ALET_xBxDxFxH,
    .MEHE_AxCxExGx = _MEHE_AxCxExGx,
    .MYVO_AxCxExGx = _MYVO_AxCxExGx,
    .LAPE_AxCxExGx = _LAPE_AxCxExGx,
    .TAVA_xBxDxFxH = _TAVA_xBxDxFxH,
    .XYVA_AxCxExGx = _XYVA_AxCxExGx,
    .XOTA_xBxDxFxH = _XOTA_xBxDxFxH,
    .XYFY_AxCxExGx = _XYFY_AxCxExGx,
    .XUPY_xBCxxFGx = _XUPY_xBCxxFGx,
    .XOCE_ABxxEFxx = _XOCE_ABxxEFxx,
    .CYKE_AxxDExxH = _CYKE_AxxDExxH,
    .WUDA_xBCxxFGx = _WUDA_xBCxxFGx,
    .NULE_AxxxxFGH = _NULE_AxxxxFGH,
    .BYRY_xBCDExxx = _BYRY_xBCDExxx,
    .BUDE_AxxxxFGH = _BUDE_AxxxxFGH,
    .UVYT_xBCDExxx = _UVYT_xBCDExxx,
    .MOPA_AxxxxFGH = _MOPA_AxxxxFGH,
    .AFAS_xxxxxFGH = _AFAS_xxxxxFGH,
    .BUGO_xxCDEFxx = _BUGO_xxCDEFxx,
    .BATE_ABxxxxxH = _BATE_ABxxxxxH,
    .BASU_xxCDEFGx = _BASU_xxCDEFGx,
    .BUKE_ABxxxxxH = _BUKE_ABxxxxxH,
    .BAPY_AxxxxxxH = _BAPY_AxxxxxxH,
    .BERU_xBCDEFGx = _BERU_xBCDEFGx,
    .BUFA_AxxxxxxH = _BUFA_AxxxxxxH,
    .BOLO_xBCDEFGx = _BOLO_xBCDEFGx,
    .BEKO_xBCDExxx = _BEKO_xBCDExxx,
    .BEJA_AxxxxFGH = _BEJA_AxxxxFGH,
    .BANE_xBCDExxx = _BANE_xBCDExxx,
    .BELO_AxxxxFGH = _BELO_AxxxxFGH,
    .BAZE_xBCDExxx = _BAZE_xBCDExxx,
    .BUTO_AxCDEFGH = _BUTO_AxCDEFGH,
    .BELE_xBxxxxxx = _BELE_xBxxxxxx,
    .BYJU_AxCDEFGH = _BYJU_AxCDEFGH,
    .BALY_xBxxxxxx = _BALY_xBxxxxxx,
    .BOGA_AxCDEFGH = _BOGA_AxCDEFGH,
    .TALU_xBCDExxx = _TALU_xBCDExxx,
    .SONO_AxxxxFGH = _SONO_AxxxxFGH,
    .AWOH_AxxDExxH = _AWOH_AxxDExxH,
    .XYNY_xBCDExxx = _XYNY_xBCDExxx,
    .BUVU_xBxxxxxx = _BUVU_xBxxxxxx,
    .BYXO_AxCDEFGH = _BYXO_AxCDEFGH,
    .BEDO_xBxxxxxx = _BEDO_xBxxxxxx,
    .WOJO_xxxDxxxH = _WOJO_xxxDxxxH,
    .XYSO_ABCxDEFx = _XYSO_ABCxDEFx,
    .ABUR_xxCDEFGx = _ABUR_xxCDEFGx,
    .BORY_ABxxxxxH = _BORY_ABxxxxxH,
    .BOMA_xBxxxxxx = _BOMA_xBxxxxxx,
    .BOWA_AxCDEFGH = _BOWA_AxCDEFGH,
  };
}


void ClockRegisters::tick(const TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto rst_sig = ResetSignals::get(gb);

  /*p01.AFUR*/ AFUR_PHAZ_xBCDExxx.set_duo(clk_sig.ATAL_xBxDxFxH, dbg_sig.UPOJ_MODE_PRODn, !ADYK_PHAZ_xxxxEFGH.a);
  /*p01.ALEF*/ ALEF_PHAZ_xxCDEFxx.set_duo(clk_sig.ATAL_xBxDxFxH, dbg_sig.UPOJ_MODE_PRODn,  AFUR_PHAZ_xBCDExxx.a);
  /*p01.APUK*/ APUK_PHAZ_xxxDEFGx.set_duo(clk_sig.ATAL_xBxDxFxH, dbg_sig.UPOJ_MODE_PRODn,  ALEF_PHAZ_xxCDEFxx.a);
  /*p01.ADYK*/ ADYK_PHAZ_xxxxEFGH.set_duo(clk_sig.ATAL_xBxDxFxH, dbg_sig.UPOJ_MODE_PRODn,  APUK_PHAZ_xxxDEFGx.a);

  /*p29.WUVU*/ WUVU_AxxDExxH.set(clk_sig.XOTA_xBxDxFxH, rst_sig.XAPO_VID_RSTn, !WUVU_AxxDExxH);
  /*p21.VENA*/ VENA_xBCDExxx.set(!WUVU_AxxDExxH, rst_sig.XAPO_VID_RSTn, !VENA_xBCDExxx);
  /*p29.WOSU*/ WOSU_xxCDxxGH.set(clk_sig.XYFY_AxCxExGx, rst_sig.XAPO_VID_RSTn, !WUVU_AxxDExxH);

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
}

bool ClockRegisters::commit() {
  bool changed = false;

  /*p01.AFUR*/ changed |= AFUR_PHAZ_xBCDExxx.commit_duo();
  /*p01.ALEF*/ changed |= ALEF_PHAZ_xxCDEFxx.commit_duo();
  /*p01.APUK*/ changed |= APUK_PHAZ_xxxDEFGx.commit_duo();
  /*p01.ADYK*/ changed |= ADYK_PHAZ_xxxxEFGH.commit_duo();

  /*p29.WUVU*/ changed |= WUVU_AxxDExxH.commit_reg();
  /*p21.VENA*/ changed |= VENA_xBCDExxx.commit_reg();
  /*p29.WOSU*/ changed |= WOSU_xxCDxxGH.commit_reg();


  return changed;
}