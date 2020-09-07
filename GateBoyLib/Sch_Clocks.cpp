#include "GateBoyLib/Sch_Clocks.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

#pragma warning(disable:4100) // unreferenced parameter

#define FAST_BOOT

//-----------------------------------------------------------------------------

void ClockRegisters::dump(Dumper& d, wire CLK) const {
  d("---------- Phase Clock ----------\n");
  d("PHASE         %c%c%c%c\n", AFUR_xxxxEFGH.c(), ALEF_AxxxxFGH.c(), APUK_ABxxxxGH.c(), ADYK_ABCxxxxH.c());
  d("\n");
  d("AFUR_xxxxEFGH %d\n", AFUR_xxxxEFGH.qp());
  d("ALEF_AxxxxFGH %d\n", ALEF_AxxxxFGH.qp());
  d("APUK_ABxxxxGH %d\n", APUK_ABxxxxGH.qp());
  d("ADYK_ABCxxxxH %d\n", ADYK_ABCxxxxH.qp());
  d("\n");
  d("ATYP_ABCDxxxx %d\n", ATYP_ABCDxxxx.qp());
  d("AROV_xxCDEFxx %d\n", AROV_xxCDEFxx.qp());
  d("AJAX_xxxxEFGH %d\n", AJAX_xxxxEFGH.qp());
  d("\n");

  d("---------- PPU Clock ----------\n");
  d("VENA_ABCDxxxx %d\n", !VENA_ABCDxxxx.qn());
  d("TALU_ABCDxxxx %c\n", TALU_ABCDxxxx.c());
  d("XOCE_AxxDExxH %c\n", XOCE_AxxDExxH.c());
  d("WOSU_xBCxxFGx %d\n",  WOSU_xBCxxFGx.qp());
  d("WOJO_xxCxxxGx %c\n", WOJO_xxCxxxGx.c());
  d("WUVU_xxCDxxGH %d\n", !WUVU_xxCDxxGH.qn());
  d("XUPY_xxCDxxGH %c\n", XUPY_xxCDxxGH.c());
  d("\n");

  d("----------  Reset   ----------\n");
  d("TUBO %d\n",  TUBO_WAITINGp.tp());
  d("ASOL %d\n",  ASOL_POR_DONEn.tp());
  d("AFER %d\n",  AFER_SYS_RSTp.qp());
  d("\n");
}

//-----------------------------------------------------------------------------

void ClockRegisters::tick_slow(wire CLK, wire CLKGOOD, wire CPUREADY, SchematicTop& top) {

  /*p01.AVOR*/ wire AVOR_SYS_RSTp = or2(AFER_SYS_RSTp.qp(), ASOL_POR_DONEn.tp());
  /*p01.ALUR*/ ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*p01.DULA*/ wire DULA_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
  /*p01.XORE*/ wire XORE_SYS_RSTp = not1(CUNU_SYS_RSTn);
  /*p01.XEBE*/ wire XEBE_SYS_RSTn = not1(XORE_SYS_RSTp);
  /*p01.XODO*/ wire XODO_VID_RSTp = nand2(XEBE_SYS_RSTn, top.pix_pipe.XONA_LCDC_LCDENn.qn());
  /*p01.XAPO*/ XAPO_VID_RSTn = not1(XODO_VID_RSTp);

  /*p01.ATEZ*/ wire ATEZ_CLKBAD = not1(CLKGOOD);
  /*p01.UCOB*/ UCOB_CLKBADp = not1(CLKGOOD);
  /*p01.ABOL*/ wire ABOL_CLKREQn = not1(CPUREADY);
  
  /*p01.ATAL*/ wire ATAL_xBxDxFxH = CLK; // ignoring the deglitcher here
  /*p01.AZOF*/ wire AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
  /*p01.ZAXY*/ ZAXY_xBxDxFxH = not1(AZOF_AxCxExGx);

  /*#p01.AROV*/ AROV_xxCDEFxx = not1(APUK_ABxxxxGH.qp());
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH = not1(ALEF_AxxxxFGH.qn());
  /*#p01.ATYP*/ ATYP_ABCDxxxx = not1(AFUR_xxxxEFGH.qp());
  /*#p01.AJAX*/ AJAX_xxxxEFGH = not1(ATYP_ABCDxxxx.qp());

  /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*p01.BELU*/ wire BELU_xxxxEFGH = nor2(ABOL_CLKREQn, ATYP_ABCDxxxx);
  /*p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*p01.BUDE*/ BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);

  /*p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
  /*p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx);
  /*p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);

  /*p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH);
  /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand2(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
  /*p01.BANE*/ wire BANE_ABCDxxxx = not1(BEJA_xxxxEFGH);
  /*p01.BELO*/ wire BELO_xxxxEFGH = not1(BANE_ABCDxxxx);
  /*p01.BAZE*/ wire BAZE_ABCDxxxx = not1(BELO_xxxxEFGH);
  /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand3(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*p01.BELE*/ wire BELE_Axxxxxxx = not1(BUTO_xBCDEFGH);
  /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor2(BELE_Axxxxxxx, ATEZ_CLKBAD);
  /*p01.BALY*/ BALY_Axxxxxxx = not1(BYJU_xBCDEFGH);

  /*#p01.ADAR*/ ADAR_ABCxxxxH = not1(ADYK_ABCxxxxH.qn());

  /*p29.XUPY*/ XUPY_xxCDxxGH = not1(WUVU_xxCDxxGH.qn());
  /*p21.TALU*/ TALU_ABCDxxxx = not1(VENA_ABCDxxxx.qn());
  /*p29.XOCE*/ XOCE_AxxDExxH = not1(WOSU_xBCxxFGx.qp());
  /*p29.WOJO*/ WOJO_xxCxxxGx = nor2(WUVU_xxCDxxGH.qn(), WOSU_xBCxxFGx.qn());
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_clk_slow(wire RST, wire CLK, wire CLKGOOD, wire CPUREADY, SchematicTop& top) {
  // ignoring the deglitcher here
  // the comp clock is unmarked on the die trace but it's directly to the left of ATAL
  /*p01.ATAL*/ wire ATAL_xBxDxFxH = CLK;
  /*p01.ATAL*/ wire ATAL_AxCxExGx = not1(ATAL_xBxDxFxH);

  /*p01.AFUR*/ AFUR_xxxxEFGH.tock(ATAL_AxCxExGx, top.UPOJ_MODE_PRODn, ADYK_ABCxxxxH.qp());
  /*p01.ALEF*/ ALEF_AxxxxFGH.tock(ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, AFUR_xxxxEFGH.qn());
  /*p01.APUK*/ APUK_ABxxxxGH.tock(ATAL_AxCxExGx, top.UPOJ_MODE_PRODn, ALEF_AxxxxFGH.qn());
  /*p01.ADYK*/ ADYK_ABCxxxxH.tock(ATAL_xBxDxFxH, top.UPOJ_MODE_PRODn, APUK_ABxxxxGH.qn());

  /* p01.ABOL*/ wire ABOL_CLKREQn = not1(CPUREADY);
  /*#p01.BUTY*/ wire BUTY_CLKREQ = not1(ABOL_CLKREQn);

  /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
  /*p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx);
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);

  /*#p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH); // BEKO+BAVY parallel
  /*#p01.BUVU*/ wire BUVU_Axxxxxxx = and2(BALY_Axxxxxxx, BUTY_CLKREQ);

  /*#p01.BYXO*/ wire BYXO_xBCDEFGH = not1(BUVU_Axxxxxxx);
  /*#p01.BEDO*/ wire BEDO_Axxxxxxx = not1(BYXO_xBCDEFGH);
  /*#p01.BOWA*/ wire BOWA_xBCDEFGH = not1(BEDO_Axxxxxxx);

  /*#p01.AFEP*/ wire AFEP_AxxxxFGH = not1(ALEF_AxxxxFGH.qn());
  /*#p01.BUGO*/ wire BUGO_xBCDExxx = not1(AFEP_AxxxxFGH);
  /*#p01.BATE*/ wire BATE_AxxxxxGH = nor3(BUGO_xBCDExxx,
                                          AROV_xxCDEFxx,
                                          ABOL_CLKREQn);
  /*#p01.BASU*/ wire BASU_xBCDEFxx = not1(BATE_AxxxxxGH);

  /*#p01.BUKE*/ wire BUKE_AxxxxxGH = not1(BASU_xBCDEFxx);
  /*#p01.BOGA*/ wire BOGA_xBCDEFGH = not1(BALY_Axxxxxxx);
  /*#p01.BOMA*/ wire BOMA_Axxxxxxx = not1(BOGA_xBCDEFGH);

  top.cpu_bus.PIN_CPU_EXT_CLKGOOD = CLKGOOD;

  top.cpu_bus.PIN_CPU_BOWA_xBCDEFGH = BOWA_xBCDEFGH;
  top.cpu_bus.PIN_CPU_BEDO_Axxxxxxx = BEDO_Axxxxxxx;

  top.cpu_bus.PIN_CPU_BEKO_ABCDxxxx = BEKO_ABCDxxxx;
  top.cpu_bus.PIN_CPU_BUDE_xxxxEFGH = BUDE_xxxxEFGH;

  top.cpu_bus.PIN_CPU_BOLO_ABCDEFxx = BOLO_ABCDEFxx;
  top.cpu_bus.PIN_CPU_BUKE_AxxxxxGH = BUKE_AxxxxxGH;
    
  top.cpu_bus.PIN_CPU_BOMA_Axxxxxxx = BOMA_Axxxxxxx;
  top.cpu_bus.PIN_CPU_BOGA_xBCDEFGH = BOGA_xBCDEFGH;

  top.ext_bus.PIN_EXT_CLK = io_pin(BUDE_xxxxEFGH, BUDE_xxxxEFGH);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_rst_slow(wire RST, wire CLKGOOD, wire CPUREADY, SchematicTop& top) {
  /*p01.UPYF*/ wire UPYF = or2(RST, UCOB_CLKBADp);
  /*p01.TUBO*/ TUBO_WAITINGp = nor_latch(UPYF, CPUREADY);

#ifdef FAST_BOOT
  /*p01.UNUT*/ wire UNUT_POR_TRIGn = and2(TUBO_WAITINGp.tp(), top.tim_reg.TERO_DIV_03.qp());
#else
  /*p01.UNUT*/ wire UNUT_POR_TRIGn = and2(TUBO_WAITINGp.qp(), top.tim_reg.UPOF_DIV_15());
#endif

  /*p01.TABA*/ wire TABA_POR_TRIGn = or3(top.UNOR_MODE_DBG2p, top.UMUT_MODE_DBG1p, UNUT_POR_TRIGn);
  top.cpu_bus.PIN_CPU_STARTp   = TABA_POR_TRIGn;

  /*#p01.ALYP*/ wire ALYP_RSTn = not1(TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire AFAR_RST  = nor2(RST, ALYP_RSTn);
  /*p01.ASOL*/ ASOL_POR_DONEn = nor_latch(RST, AFAR_RST); // Schematic wrong, this is a latch.

  /*p01.BOGA*/ wire BOGA_xBCDEFGH = not1(BALY_Axxxxxxx);
  /*p01.AFER*/ AFER_SYS_RSTp.tock(BOGA_xBCDEFGH, top.UPOJ_MODE_PRODn, ASOL_POR_DONEn.tp());

  top.cpu_bus.PIN_CPU_SYS_RSTp = AFER_SYS_RSTp.qp();
  top.cpu_bus.PIN_CPU_EXT_RST  = RST;
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_dbg_slow(SchematicTop& top) {
  top.cpu_bus.PIN_CPU_UNOR_DBG = top.UNOR_MODE_DBG2p;
  top.cpu_bus.PIN_CPU_UMUT_DBG = top.UMUT_MODE_DBG1p;
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_vid_slow(wire CLK, SchematicTop& top) {
  /*p01.ZEME*/ wire ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /*p29.XYVA*/ wire XYVA_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*p29.XOTA*/ wire XOTA_AxCxExGx = not1(XYVA_xBxDxFxH);
  /*p29.XYFY*/ wire XYFY_xBxDxFxH = not1(XOTA_AxCxExGx);

  /*p29.WUVU*/ WUVU_xxCDxxGH = dff17 (XOTA_AxCxExGx,      XAPO_VID_RSTn, WUVU_xxCDxxGH.qn());
  /*p21.VENA*/ VENA_ABCDxxxx = dff17 (WUVU_xxCDxxGH.qn(), XAPO_VID_RSTn, VENA_ABCDxxxx.qn());
  /*p29.WOSU*/ WOSU_xBCxxFGx = dff17(XYFY_xBxDxFxH,      XAPO_VID_RSTn, WUVU_xxCDxxGH.qn());
}

//-----------------------------------------------------------------------------
