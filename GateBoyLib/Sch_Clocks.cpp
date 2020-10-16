#include "GateBoyLib/Sch_Clocks.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

#pragma warning(disable:4100) // unreferenced parameter

//#define FAST_BOOT

//-----------------------------------------------------------------------------

void ClockRegisters::dump(Dumper& d, wire CLK) const {
  d("\002===== Clocks =====\001\n");
  d("PHASE %c%c%c%c\n", AFUR_xxxxEFGH.c(), ALEF_AxxxxFGH.c(), APUK_ABxxxxGH.c(), ADYK_ABCxxxxH.c());
  d("\n");
  d("AFUR_xxxxEFGH %c\n", AFUR_xxxxEFGH.c());
  d("ALEF_AxxxxFGH %c\n", ALEF_AxxxxFGH.c());
  d("APUK_ABxxxxGH %c\n", APUK_ABxxxxGH.c());
  d("ADYK_ABCxxxxH %c\n", ADYK_ABCxxxxH.c());
  d("ATYP_ABCDxxxx %c\n", ATYP_ABCDxxxx.c());
  d("AROV_xxCDEFxx %c\n", AROV_xxCDEFxx.c());
  d("AJAX_xxxxEFGH %c\n", AJAX_xxxxEFGH.c());
  d("VENA_ABCDxxxx %c\n", VENA_xxCDEFxx.c());
  d("TALU_ABCDxxxx %c\n", TALU_xxCDEFxx.c());
  d("XOCE_AxxDExxH %c\n", XOCE_xBCxxFGx.c());
  d("WOSU_xBCxxFGx %c\n", WOSU_AxxDExxH.c());
  d("WOJO_xxCxxxGx %c\n", WOJO_AxxxExxx.c());
  d("WUVU_xxCDxxGH %c\n", WUVU_ABxxEFxx.c());
  d("XUPY_xxCDxxGH %c\n", XUPY_ABxxEFxx.c());
  d("\n");

  d("\002===== Reset =====\001\n");
  d("TUBO_WAITINGp  %c\n", TUBO_WAITINGp.c());
  d("ASOL_POR_DONEn %c\n", ASOL_POR_DONEn.c());
  d("AFER_SYS_RSTp  %c\n", AFER_SYS_RSTp.c());
  d("\n");
}

//==============================================================================

void ClockRegisters::tick_slow(wire CLK, wire CLKGOOD, wire CPUREADY, const SchematicTop& top) {

  /*#p01.AVOR*/ wire _AVOR_SYS_RSTp = or2(AFER_SYS_RSTp.qp13(), ASOL_POR_DONEn.qp04());
  /*#p01.ALUR*/ ALUR_SYS_RSTn = not1(_AVOR_SYS_RSTp);
  /*#p01.DULA*/ wire DULA_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*#p01.CUNU*/ CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
  /*#p01.XORE*/ XORE_SYS_RSTp = not1(CUNU_SYS_RSTn);
  /* p01.WESY*/ WESY_SYS_RSTn = not1(XORE_SYS_RSTp);

  /* p01.XEBE*/ wire _XEBE_SYS_RSTn = not1(XORE_SYS_RSTp);
  /* p01.XODO*/ wire _XODO_VID_RSTp = nand2(_XEBE_SYS_RSTn, top.pix_pipe.XONA_LCDC_LCDENn.qn08());
  /* p01.XAPO*/ XAPO_VID_RSTn = not1(_XODO_VID_RSTp);
  /*#p01.ATAR*/ ATAR_VID_RSTp = not1(XAPO_VID_RSTn);
  /*#p01.ABEZ*/ ABEZ_VID_RSTn = not1(ATAR_VID_RSTp);


  /* p01.ATEZ*/ wire _ATEZ_CLKBAD = not1(CLKGOOD);
  /* p01.UCOB*/ UCOB_CLKBADp = not1(CLKGOOD);
  /* p01.ABOL*/ wire _ABOL_CLKREQn = not1(CPUREADY);
  /*#p01.BUTY*/ wire _BUTY_CLKREQ = not1(_ABOL_CLKREQn);

  // ignoring the deglitcher here
  /*p01.ATAL*/ ATAL_xBxDxFxH = CLK;
  /*p01.AZOF*/ wire _AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
  /*p01.ZAXY*/ ZAXY_xBxDxFxH = not1(_AZOF_AxCxExGx);

  /*#p01.AROV*/ AROV_xxCDEFxx = not1(APUK_ABxxxxGH.qp09());
  /*#p01.AFEP*/ wire _AFEP_AxxxxFGH = not1(ALEF_AxxxxFGH.qn08());
  /*#p01.ATYP*/ ATYP_ABCDxxxx = not1(AFUR_xxxxEFGH.qp09());
  /*#p01.AJAX*/ AJAX_xxxxEFGH = not1(ATYP_ABCDxxxx.qp());

  /*#p01.BAPY*/ wire _BAPY_xxxxxxGH = nor3(_ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);

  /*#p01.BELU*/ wire _BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, _ABOL_CLKREQn);
  /*#p01.BYRY*/ wire _BYRY_ABCDxxxx = not1(_BELU_xxxxEFGH);
  /*#p01.BUDE*/ BUDE_xxxxEFGH = not1(_BYRY_ABCDxxxx);

  /*#p01.BERU*/ wire _BERU_ABCDEFxx = not1(_BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire _BUFA_xxxxxxGH = not1(_BERU_ABCDEFxx);
  /*#p01.BOLO*/ BOLO_ABCDEFxx = not1(_BUFA_xxxxxxGH);

  /*#p01.BEKO*/ BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH); // BEKO+BAVY parallel
  /*#p01.BEJA*/ wire _BEJA_xxxxEFGH = nand4(BOLO_ABCDEFxx, BOLO_ABCDEFxx, BEKO_ABCDxxxx, BEKO_ABCDxxxx);
  /*#p01.BANE*/ wire _BANE_ABCDxxxx = not1(_BEJA_xxxxEFGH);
  /*#p01.BELO*/ wire _BELO_xxxxEFGH = not1(_BANE_ABCDxxxx);
  /*#p01.BAZE*/ wire _BAZE_ABCDxxxx = not1(_BELO_xxxxEFGH);

  /*#p01.BUTO*/ wire _BUTO_xBCDEFGH = nand3(_AFEP_AxxxxFGH, ATYP_ABCDxxxx, _BAZE_ABCDxxxx);
  /*#p01.BELE*/ wire _BELE_Axxxxxxx = not1(_BUTO_xBCDEFGH);
  /*#p01.BYJU*/ wire _BYJU_Axxxxxxx = or2(_BELE_Axxxxxxx, _ATEZ_CLKBAD);

  /*#p01.BALY*/ BALY_xBCDEFGH = not1(_BYJU_Axxxxxxx);
  /* p01.BOGA*/ BOGA_Axxxxxxx = not1(BALY_xBCDEFGH);

  /*#p01.ADAR*/ ADAR_ABCxxxxH = not1(ADYK_ABCxxxxH.qn08());
  /*#p29.XUPY*/ XUPY_ABxxEFxx = not1(WUVU_ABxxEFxx.qn16());
  /*#p21.TALU*/ TALU_xxCDEFxx = not1(VENA_xxCDEFxx.qn16());
  /*#p29.XOCE*/ XOCE_xBCxxFGx = not1(WOSU_AxxDExxH.qp17());
  /*#p29.WOJO*/ WOJO_AxxxExxx = nor2(WOSU_AxxDExxH.qn16(), WUVU_ABxxEFxx.qn16());
  /*#p01.AFAS*/ AFAS_xxxxEFGx = nor2(ADAR_ABCxxxxH, ATYP_ABCDxxxx);

  /*#p01.ZEME*/ ZEME_AxCxExGx = not1(ZAXY_xBxDxFxH);
  /* p01.UVYT*/ UVYT_ABCDxxxx = not1(BUDE_xxxxEFGH);
  /* p04.MOPA*/ MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx);
  /*#p01.ALET*/ ALET_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*#p27.MYVO*/ MYVO_AxCxExGx = not1(ALET_xBxDxFxH);

  /*#p01.BUGO*/ wire _BUGO_xBCDExxx = not1(_AFEP_AxxxxFGH);
  /*#p01.BATE*/ wire _BATE_AxxxxxGH = nor3(_BUGO_xBCDExxx, AROV_xxCDEFxx, _ABOL_CLKREQn);
  /*#p01.BASU*/ wire _BASU_xBCDEFxx = not1(_BATE_AxxxxxGH);

  /*#p01.BUKE*/ BUKE_AxxxxxGH = not1(_BASU_xBCDEFxx);
  /*#p01.BOMA*/ BOMA_xBCDEFGH = not1(BOGA_Axxxxxxx);

  /*#p01.BUVU*/ wire _BUVU_Axxxxxxx = and2(BALY_xBCDEFGH, _BUTY_CLKREQ);
  /*#p01.BYXO*/ wire _BYXO_xBCDEFGH = not1(_BUVU_Axxxxxxx);
  /*#p01.BEDO*/ BEDO_Axxxxxxx = not1(_BYXO_xBCDEFGH);
  /*#p01.BOWA*/ BOWA_xBCDEFGH = not1(BEDO_Axxxxxxx);

}

//==============================================================================

void ClockRegisters::tock_clk_slow(wire CLKGOOD, SchematicTop& top) {
  // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

  /*p01.AFUR*/ AFUR_xxxxEFGH.dff9(!ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, ADYK_ABCxxxxH.qp09());
  /*p01.ALEF*/ ALEF_AxxxxFGH.dff9( ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, AFUR_xxxxEFGH.qn08());
  /*p01.APUK*/ APUK_ABxxxxGH.dff9(!ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, ALEF_AxxxxFGH.qn08());
  /*p01.ADYK*/ ADYK_ABCxxxxH.dff9( ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, APUK_ABxxxxGH.qn08());

  top.cpu_bus.PIN_CPU_EXT_CLKGOOD.set(CLKGOOD);

  top.cpu_bus.PIN_CPU_BOWA_Axxxxxxx.set(BOWA_xBCDEFGH);
  top.cpu_bus.PIN_CPU_BEDO_xBCDEFGH.set(BEDO_Axxxxxxx);

  top.cpu_bus.PIN_CPU_BEKO_ABCDxxxx.set(BEKO_ABCDxxxx);
  top.cpu_bus.PIN_CPU_BUDE_xxxxEFGH.set(BUDE_xxxxEFGH);

  top.cpu_bus.PIN_CPU_BOLO_ABCDEFxx.set(BOLO_ABCDEFxx);
  top.cpu_bus.PIN_CPU_BUKE_AxxxxxGH.set(BUKE_AxxxxxGH);

  top.cpu_bus.PIN_CPU_BOMA_xBCDEFGH.set(BOMA_xBCDEFGH);
  top.cpu_bus.PIN_CPU_BOGA_Axxxxxxx.set(BOGA_Axxxxxxx);

  top.ext_bus.PIN_EXT_CLK.io_pin(BUDE_xxxxEFGH, BUDE_xxxxEFGH);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_rst_slow(wire RST, wire CLKGOOD, wire CPUREADY, SchematicTop& top) {
  /*p01.UPYF*/ wire _UPYF = or2(RST, UCOB_CLKBADp);
  /*p01.TUBO*/ TUBO_WAITINGp.nor_latch(_UPYF, CPUREADY);

#ifdef FAST_BOOT
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(TUBO_WAITINGp.qp(), top.tim_reg.TERO_DIV_03.qp());
#else
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(TUBO_WAITINGp.qp04(), top.tim_reg.UPOF_DIV_15.qp17());
#endif

  /*p01.TABA*/ wire _TABA_POR_TRIGn = or3(top.UNOR_MODE_DBG2p, top.UMUT_MODE_DBG1p, _UNUT_POR_TRIGn);
  top.cpu_bus.PIN_CPU_STARTp.set(_TABA_POR_TRIGn);

  /*#p01.ALYP*/ wire _ALYP_RSTn = not1(_TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire _AFAR_RSTp  = nor2(RST, _ALYP_RSTn);
  /* p01.ASOL*/ ASOL_POR_DONEn.nor_latch(RST, _AFAR_RSTp); // Schematic wrong, this is a latch.

  /* p01.AFER*/ AFER_SYS_RSTp.dff13(BOGA_Axxxxxxx, top.UPOJ_MODE_PRODn, ASOL_POR_DONEn.qp04());

  top.cpu_bus.PIN_CPU_SYS_RSTp.set(AFER_SYS_RSTp.qp13());
  top.cpu_bus.PIN_CPU_EXT_RST.set(RST);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_dbg_slow(SchematicTop& top) {
  top.cpu_bus.PIN_CPU_UNOR_DBG.set(top.UNOR_MODE_DBG2p);
  top.cpu_bus.PIN_CPU_UMUT_DBG.set(top.UMUT_MODE_DBG1p);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_vid_slow(wire CLK, SchematicTop& top) {
  /*p29.XYVA*/ wire _XYVA_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*p29.XOTA*/ wire _XOTA_AxCxExGx = not1(_XYVA_xBxDxFxH);
  /*p29.XYFY*/ wire _XYFY_xBxDxFxH = not1(_XOTA_AxCxExGx);

  /*p29.WUVU*/ WUVU_ABxxEFxx.dff17(_XOTA_AxCxExGx,       XAPO_VID_RSTn, WUVU_ABxxEFxx.qn16());
  /*p21.VENA*/ VENA_xxCDEFxx.dff17(WUVU_ABxxEFxx.qn16(), XAPO_VID_RSTn, VENA_xxCDEFxx.qn16()); // inverting the clock to this reg doesn't seem to break anything, which is really weird
  /*p29.WOSU*/ WOSU_AxxDExxH.dff17(_XYFY_xBxDxFxH,       XAPO_VID_RSTn, WUVU_ABxxEFxx.qn16());
}

//-----------------------------------------------------------------------------
