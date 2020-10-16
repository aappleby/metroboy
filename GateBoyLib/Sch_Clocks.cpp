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

//-----------------------------------------------------------------------------

void ClockRegisters::tick_slow(wire CLK, wire CLKGOOD, wire CPUREADY, SchematicTop& top) {

  /*p01.AVOR*/ wire AVOR_SYS_RSTp = or2(AFER_SYS_RSTp.qp13(), ASOL_POR_DONEn.qp04());
  /*#p01.ALUR*/ ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*#p01.DULA*/ wire DULA_SYS_RSTp = not1(top.clk_reg.ALUR_SYS_RSTn);
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
  /*p01.XORE*/ wire XORE_SYS_RSTp = not1(CUNU_SYS_RSTn);
  /*p01.XEBE*/ wire XEBE_SYS_RSTn = not1(XORE_SYS_RSTp);
  /*p01.XODO*/ wire XODO_VID_RSTp = nand2(XEBE_SYS_RSTn, top.pix_pipe.XONA_LCDC_LCDENn.qn08());
  /*p01.XAPO*/ XAPO_VID_RSTn = not1(XODO_VID_RSTp);

  /*p01.ATEZ*/ wire ATEZ_CLKBAD = not1(CLKGOOD);
  /*p01.UCOB*/ UCOB_CLKBADp = not1(CLKGOOD);
  /*p01.ABOL*/ wire ABOL_CLKREQn = not1(CPUREADY);

  /*p01.ATAL*/ wire ATAL_xBxDxFxH = CLK; // ignoring the deglitcher here
  /*p01.AZOF*/ wire AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
  /*p01.ZAXY*/ ZAXY_xBxDxFxH = not1(AZOF_AxCxExGx);

  /*#p01.AROV*/ AROV_xxCDEFxx = not1(APUK_ABxxxxGH.qp09());
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH = not1(ALEF_AxxxxFGH.qn08());
  /*#p01.ATYP*/ ATYP_ABCDxxxx = not1(AFUR_xxxxEFGH.qp09());
  /*#p01.AJAX*/ AJAX_xxxxEFGH = not1(ATYP_ABCDxxxx.qp());

  /*#p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);

  /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*#p01.BUDE*/ BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);

  /*#p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx);
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);

  /*#p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH);
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH = nand4(BOLO_ABCDEFxx, BOLO_ABCDEFxx, BEKO_ABCDxxxx, BEKO_ABCDxxxx);
  /*#p01.BANE*/ wire BANE_ABCDxxxx = not1(BEJA_xxxxEFGH);
  /*#p01.BELO*/ wire BELO_xxxxEFGH = not1(BANE_ABCDxxxx);
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx = not1(BELO_xxxxEFGH);

  /*#p01.BUTO*/ wire BUTO_xBCDEFGH = nand3(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*#p01.BELE*/ wire BELE_Axxxxxxx = not1(BUTO_xBCDEFGH);
  /*#p01.BYJU*/ wire BYJU_Axxxxxxx = or2(BELE_Axxxxxxx, ATEZ_CLKBAD);

  /*#p01.BALY*/ BALY_xBCDEFGH = not1(BYJU_Axxxxxxx);
  /*#p01.ADAR*/ ADAR_ABCxxxxH = not1(ADYK_ABCxxxxH.qn08());
  /*#p29.XUPY*/ XUPY_ABxxEFxx = not1(WUVU_ABxxEFxx.qn16());
  /*#p21.TALU*/ TALU_xxCDEFxx = not1(VENA_xxCDEFxx.qn16());
  /*#p29.XOCE*/ XOCE_xBCxxFGx = not1(WOSU_AxxDExxH.qp17());
  /*#p29.WOJO*/ WOJO_AxxxExxx = nor2(WOSU_AxxDExxH.qn16(), WUVU_ABxxEFxx.qn16());
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_clk_slow(wire RST, wire CLK, wire CLKGOOD, wire CPUREADY, SchematicTop& top) {
  // ignoring the deglitcher here
  // the comp clock is unmarked on the die trace but it's directly to the left of ATAL
  /*p01.ATAL*/ wire ATAL_xBxDxFxH = CLK;
  /*p01.ATAL*/ wire ATAL_AxCxExGx = not1(ATAL_xBxDxFxH);

  /*p01.AFUR*/ AFUR_xxxxEFGH.dff9(ATAL_AxCxExGx, top.UPOJ_MODE_PRODn, ADYK_ABCxxxxH.qp09());
  /*p01.ALEF*/ ALEF_AxxxxFGH.dff9(ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, AFUR_xxxxEFGH.qn08());
  /*p01.APUK*/ APUK_ABxxxxGH.dff9(ATAL_AxCxExGx, top.UPOJ_MODE_PRODn, ALEF_AxxxxFGH.qn08());
  /*p01.ADYK*/ ADYK_ABCxxxxH.dff9(ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, APUK_ABxxxxGH.qn08());

  /*#p01.ABOL*/ wire ABOL_CLKREQn = not1(CPUREADY);
  /*#p01.BUTY*/ wire BUTY_CLKREQ = not1(ABOL_CLKREQn);

  /*#p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*#p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx);
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);

  /*#p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH); // BEKO+BAVY parallel
  /*#p01.BUVU*/ wire BUVU_Axxxxxxx = and2(BALY_xBCDEFGH, BUTY_CLKREQ);

  /*#p01.BYXO*/ wire BYXO_xBCDEFGH = not1(BUVU_Axxxxxxx);
  /*#p01.BEDO*/ wire BEDO_Axxxxxxx = not1(BYXO_xBCDEFGH);
  /*#p01.BOWA*/ wire BOWA_xBCDEFGH = not1(BEDO_Axxxxxxx);

  /*#p01.AFEP*/ wire AFEP_AxxxxFGH = not1(ALEF_AxxxxFGH.qn08());
  /*#p01.BUGO*/ wire BUGO_xBCDExxx = not1(AFEP_AxxxxFGH);
  /*#p01.BATE*/ wire BATE_AxxxxxGH = nor3(BUGO_xBCDExxx,
                                          AROV_xxCDEFxx,
                                          ABOL_CLKREQn);
  /*#p01.BASU*/ wire BASU_xBCDEFxx = not1(BATE_AxxxxxGH);

  /*#p01.BUKE*/ wire BUKE_AxxxxxGH = not1(BASU_xBCDEFxx);
  /*#p01.BOGA*/ wire BOGA_xBCDEFGH = not1(BALY_xBCDEFGH);
  /*#p01.BOMA*/ wire BOMA_Axxxxxxx = not1(BOGA_xBCDEFGH);

  top.cpu_bus.PIN_CPU_EXT_CLKGOOD.set(CLKGOOD);

  top.cpu_bus.PIN_CPU_BOWA_Axxxxxxx.set(BOWA_xBCDEFGH);
  top.cpu_bus.PIN_CPU_BEDO_xBCDEFGH.set(BEDO_Axxxxxxx);

  top.cpu_bus.PIN_CPU_BEKO_ABCDxxxx.set(BEKO_ABCDxxxx);
  top.cpu_bus.PIN_CPU_BUDE_xxxxEFGH.set(BUDE_xxxxEFGH);

  top.cpu_bus.PIN_CPU_BOLO_ABCDEFxx.set(BOLO_ABCDEFxx);
  top.cpu_bus.PIN_CPU_BUKE_AxxxxxGH.set(BUKE_AxxxxxGH);

  top.cpu_bus.PIN_CPU_BOMA_xBCDEFGH.set(BOMA_Axxxxxxx);
  top.cpu_bus.PIN_CPU_BOGA_Axxxxxxx.set(BOGA_xBCDEFGH);

  top.ext_bus.PIN_EXT_CLK.io_pin(BUDE_xxxxEFGH, BUDE_xxxxEFGH);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_rst_slow(wire RST, wire CLKGOOD, wire CPUREADY, SchematicTop& top) {
  /*p01.UPYF*/ wire UPYF = or2(RST, UCOB_CLKBADp);
  /*p01.TUBO*/ TUBO_WAITINGp.nor_latch(UPYF, CPUREADY);

#ifdef FAST_BOOT
  /*p01.UNUT*/ wire UNUT_POR_TRIGn = and2(TUBO_WAITINGp.qp(), top.tim_reg.TERO_DIV_03.qp());
#else
  /*p01.UNUT*/ wire UNUT_POR_TRIGn = and2(TUBO_WAITINGp.qp04(), top.tim_reg.UPOF_DIV_15.qp17());
#endif

  /*p01.TABA*/ wire TABA_POR_TRIGn = or3(top.UNOR_MODE_DBG2p, top.UMUT_MODE_DBG1p, UNUT_POR_TRIGn);
  top.cpu_bus.PIN_CPU_STARTp.set(TABA_POR_TRIGn);

  /*#p01.ALYP*/ wire ALYP_RSTn = not1(TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire AFAR_RST  = nor2(RST, ALYP_RSTn);
  /*p01.ASOL*/ ASOL_POR_DONEn.nor_latch(RST, AFAR_RST); // Schematic wrong, this is a latch.

  /*#p01.BOGA*/ wire BOGA_xBCDEFGH = not1(BALY_xBCDEFGH);
  /*p01.AFER*/ AFER_SYS_RSTp.dff13(BOGA_xBCDEFGH, top.UPOJ_MODE_PRODn, ASOL_POR_DONEn.qp04());

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
  /*p01.ZEME*/ wire ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /*p29.XYVA*/ wire XYVA_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*p29.XOTA*/ wire XOTA_AxCxExGx = not1(XYVA_xBxDxFxH);
  /*p29.XYFY*/ wire XYFY_xBxDxFxH = not1(XOTA_AxCxExGx);

  /*p29.WUVU*/ WUVU_ABxxEFxx.dff17(XOTA_AxCxExGx,      XAPO_VID_RSTn, WUVU_ABxxEFxx.qn16());
  /*p21.VENA*/ VENA_xxCDEFxx.dff17(WUVU_ABxxEFxx.qn16(), XAPO_VID_RSTn, VENA_xxCDEFxx.qn16()); // inverting the clock to this reg doesn't seem to break anything, which is really weird
  /*p29.WOSU*/ WOSU_AxxDExxH.dff17(XYFY_xBxDxFxH,      XAPO_VID_RSTn, WUVU_ABxxEFxx.qn16());
}

//-----------------------------------------------------------------------------
