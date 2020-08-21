#include "Sch_Clocks.h"
#include "Sch_Top.h"

using namespace Schematics;

#pragma warning(disable:4100) // unreferenced parameter

#define FAST_BOOT

void ClockRegisters::dump(Dumper& d) const {
  d("---------- Sys Pins ----------\n");
  d("SYS_PIN_CLK_A %d\n", SYS_PIN_CLK_A.tp());
  d("SYS_PIN_CLK_B %d\n", SYS_PIN_CLK_B.tp());
  d("SYS_PIN_RSTp  %d\n", SYS_PIN_RSTp.tp());
  d("SYS_PIN_T2n   %d\n", SYS_PIN_T2n.tp());
  d("SYS_PIN_T1n   %d\n", SYS_PIN_T1n.tp());
  d("\n");

  d("---------- Phase Clock ----------\n");
  d("PHASE         %c%c%c%c\n", AFUR_xxxxEFGH.c(), ALEF_AxxxxFGH.cn(), APUK_ABxxxxGH.c(), ADYK_ABCxxxxH.cn());
  d("ATEZ_CLKBAD   %d\n", ATEZ_CLKBAD.qp());
  d("ABOL_CLKREQn  %d\n", ABOL_CLKREQn.qp());
  d("\n");
  d("AFUR_xxxxEFGH %d\n", AFUR_xxxxEFGH.qp());
  d("ALEF_AxxxxFGH %d\n", ALEF_AxxxxFGH.qp());
  d("APUK_ABxxxxGH %d\n", APUK_ABxxxxGH.qp());
  d("ADYK_ABCxxxxH %d\n", ADYK_ABCxxxxH.qp());
  d("\n");
  d("AROV_xxCDEFxx %d\n", AROV_xxCDEFxx.qp());
  d("AFEP_AxxxxFGH %d\n", AFEP_AxxxxFGH.qp());
  d("ATYP_ABCDxxxx %d\n", ATYP_ABCDxxxx.qp());
  d("AJAX_xxxxEFGH %d\n", AJAX_xxxxEFGH.qp());
  d("\n");

  d("---------- PPU Clock ----------\n");
  d("MYVO_AxCxExGx %d\n", MYVO_AxCxExGx());
  d("WUVU_xxCDxxGH %d\n", !WUVU_xxCDxxGH.qn());
  d("VENA_ABCDxxxx %d\n", !VENA_ABCDxxxx.qn());
  d("WOSU_xBCxxFGx %d\n",  WOSU_xBCxxFGx.qp());
  d("XUPY_xxCDxxGH %d\n", XUPY_xxCDxxGH());
  d("TALU_ABCDxxxx %d\n", TALU_ABCDxxxx());
  d("XOCE_AxxDExxH %d\n", XOCE_AxxDExxH());
  d("AWOH_ABxxEFxx %d\n", AWOH_ABxxEFxx());
  d("SONO_xxxxEFGH %d\n", SONO_xxxxEFGH());
  d("WOJO_xxCxxxGx %d\n", WOJO_xxCxxxGx());
  d("\n");

  d("---------- CPU Pins ----------\n");
  d("CPU_PIN_STARTp        : %d\n", CPU_PIN_STARTp.tp());
  d("CPU_PIN_READYp        : %d\n", CPU_PIN_READYp.tp());
  d("CPU_PIN_SYS_RSTp      : %d\n", CPU_PIN_SYS_RSTp.tp());
  d("CPU_PIN_EXT_RST       : %d\n", CPU_PIN_EXT_RST.tp());
  d("CPU_PIN_UNOR_DBG      : %d\n", CPU_PIN_UNOR_DBG.tp());
  d("CPU_PIN_UMUT_DBG      : %d\n", CPU_PIN_UMUT_DBG.tp());
  d("CPU_PIN_EXT_CLKGOOD   : %d\n", CPU_PIN_EXT_CLKGOOD.tp());
  d("CPU_PIN_BOWA_xBCDEFGH : %d\n", CPU_PIN_BOWA_xBCDEFGH.tp());
  d("CPU_PIN_BEDO_Axxxxxxx : %d\n", CPU_PIN_BEDO_Axxxxxxx.tp());
  d("CPU_PIN_BEKO_ABCDxxxx : %d\n", CPU_PIN_BEKO_ABCDxxxx.tp());
  d("CPU_PIN_BUDE_xxxxEFGH : %d\n", CPU_PIN_BUDE_xxxxEFGH.tp());
  d("CPU_PIN_BOLO_ABCDEFxx : %d\n", CPU_PIN_BOLO_ABCDEFxx.tp());
  d("CPU_PIN_BUKE_AxxxxxGH : %d\n", CPU_PIN_BUKE_AxxxxxGH.tp());
  d("CPU_PIN_BOMA_Axxxxxxx : %d\n", CPU_PIN_BOMA_Axxxxxxx.tp());
  d("CPU_PIN_BOGA_xBCDEFGH : %d\n", CPU_PIN_BOGA_xBCDEFGH.tp());
  d("EXT_PIN_CLK_xxxxEFGH  : %d\n", EXT_PIN_CLK_xxxxEFGH.tp());
  d("\n");

  d("----------  Reset   ----------\n");
  d("TUBO %d\n",  _TUBO_WAITINGp.tp());
  d("ASOL %d\n",  _ASOL_POR_DONEn.tp());
  d("AFER %d\n",  _AFER_SYS_RSTp.qp());
  d("SOTO %d\n",  !_SOTO_DBG_VRAM.qn());
  d("\n");
}

//-----------------------------------------------------------------------------

void ClockRegisters::tick_slow(const SchematicTop& top) {
  _XONA_LCDC_ENn_qn = top.pix_pipe.XONA_LCDC_ENn.qn();
}

void ClockRegisters::tock_clk_slow(const SchematicTop& top) {
  // ignoring the deglitcher here

  // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

  /*p07.UBET*/ wire UBET_T1p        = not1(SYS_PIN_T1n.tp());
  /*p07.UVAR*/ wire UVAR_T2p        = not1(SYS_PIN_T2n.tp());
  /*p07.UPOJ*/ wire UPOJ_MODE_PRODn = nand3(UBET_T1p, UVAR_T2p, SYS_PIN_RSTp.tp());

  /*p01.AFUR*/ AFUR_xxxxEFGH = dff9_inv(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, ADYK_ABCxxxxH.qp());
  /*p01.ALEF*/ ALEF_AxxxxFGH = dff9_inv( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, AFUR_xxxxEFGH.qn());
  /*p01.APUK*/ APUK_ABxxxxGH = dff9_inv(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, ALEF_AxxxxFGH.qn());
  /*p01.ADYK*/ ADYK_ABCxxxxH = dff9_inv( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, APUK_ABxxxxGH.qn());

  /* p01.ATEZ*/ ATEZ_CLKBAD   = not1(SYS_PIN_CLK_A.tp());
  /* p01.ABOL*/ ABOL_CLKREQn  = not1(CPU_PIN_READYp.tp());
  /*#p01.BUTY*/ wire BUTY_CLKREQ = not1(ABOL_CLKREQn.qp());

  /*#p01.AROV*/ AROV_xxCDEFxx = not1(APUK_ABxxxxGH.qp());
  /*#p01.AFEP*/ AFEP_AxxxxFGH = not1(ALEF_AxxxxFGH.qn());
  /*#p01.ATYP*/ ATYP_ABCDxxxx = not1(AFUR_xxxxEFGH.qp());
  /*#p01.AJAX*/ AJAX_xxxxEFGH = not1(ATYP_ABCDxxxx.qp());

  /*#p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn,
                                          AROV_xxCDEFxx,
                                          ATYP_ABCDxxxx);

  /*#p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx); // BUFA+BYLY parallel
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);


  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx()); // BUDE+BEVA parallel
  /*#p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH); // BEKO+BAVY parallel
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH = nand2(BOLO_ABCDEFxx,
                                           BEKO_ABCDxxxx);
  /*#p01.BANE*/ wire BANE_ABCDxxxx = not1(BEJA_xxxxEFGH);
  /*#p01.BELO*/ wire BELO_xxxxEFGH = not1(BANE_ABCDxxxx);
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx = not1(BELO_xxxxEFGH);
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH = nand3(AFEP_AxxxxFGH,
                                           ATYP_ABCDxxxx,
                                           BAZE_ABCDxxxx);
  /*#p01.BELE*/ wire BELE_Axxxxxxx = not1(BUTO_xBCDEFGH);
  /*#p01.BYJU*/ wire BYJU_xBCDEFGH = nor2(BELE_Axxxxxxx, ATEZ_CLKBAD);
  /*#p01.BALY*/ wire BALY_Axxxxxxx = not1(BYJU_xBCDEFGH);
  /*#p01.BUVU*/ wire BUVU_Axxxxxxx = and2(BALY_Axxxxxxx, BUTY_CLKREQ);

  /*#p01.BYXO*/ wire BYXO_xBCDEFGH = not1(BUVU_Axxxxxxx);
  /*#p01.BEDO*/ wire BEDO_Axxxxxxx = not1(BYXO_xBCDEFGH);
  /*#p01.BOWA*/ wire BOWA_xBCDEFGH = not1(BEDO_Axxxxxxx);

  /*#p01.BUGO*/ wire BUGO_xBCDExxx = not1(AFEP_AxxxxFGH);
  /*#p01.BATE*/ wire BATE_AxxxxxGH = nor3(BUGO_xBCDExxx,
                                          AROV_xxCDEFxx,
                                          ABOL_CLKREQn);
  /*#p01.BASU*/ wire BASU_xBCDEFxx = not1(BATE_AxxxxxGH);

  /*#p01.BUKE*/ wire BUKE_AxxxxxGH = not1(BASU_xBCDEFxx);
  /*#p01.BOGA*/ wire BOGA_xBCDEFGH = not1(BALY_Axxxxxxx);
  /*#p01.BOMA*/ wire BOMA_Axxxxxxx = not1(BOGA_xBCDEFGH);

  CPU_PIN_BOWA_xBCDEFGH = BOWA_xBCDEFGH;
  CPU_PIN_BEDO_Axxxxxxx = BEDO_Axxxxxxx;

  CPU_PIN_BEKO_ABCDxxxx = BEKO_ABCDxxxx;
  CPU_PIN_BUDE_xxxxEFGH = BUDE_xxxxEFGH;

  CPU_PIN_BOLO_ABCDEFxx = BOLO_ABCDEFxx;
  CPU_PIN_BUKE_AxxxxxGH = BUKE_AxxxxxGH;
    
  CPU_PIN_BOMA_Axxxxxxx = BOMA_Axxxxxxx;
  CPU_PIN_BOGA_xBCDEFGH = BOGA_xBCDEFGH;

  /* PIN_75 */ EXT_PIN_CLK_xxxxEFGH = BUDE_xxxxEFGH;

  CPU_PIN_EXT_CLKGOOD = (wire)SYS_PIN_CLK_A.tp();
}


//-----------------------------------------------------------------------------

void ClockRegisters::tock_rst_slow(const SchematicTop& top) {
  /*p01.UPYF*/ wire _UPYF = or2(SYS_PIN_RSTp.tp(), UCOB_CLKBADp());

  /*p01.TUBO*/ _TUBO_WAITINGp = nor_latch(_UPYF, CPU_PIN_READYp.tp());


#ifdef FAST_BOOT
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(_TUBO_WAITINGp.tp(), top.tim_reg.TERO_DIV_03.qp());
#else
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(_TUBO_WAITINGp.qp(), top.tim_reg.UPOF_DIV_15());
#endif

  /*p01.TABA*/ wire _TABA_POR_TRIGn = or3(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  CPU_PIN_STARTp   = _TABA_POR_TRIGn;

  /*#p01.ALYP*/ wire _ALYP_RSTn = not1(_TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire _AFAR_RST  = nor2(SYS_PIN_RSTp.tp(), _ALYP_RSTn);

  /*p01.ASOL*/ _ASOL_POR_DONEn = nor_latch(SYS_PIN_RSTp.tp(), _AFAR_RST); // Schematic wrong, this is a latch.
  /*p01.AFER*/ _AFER_SYS_RSTp = dff13_B(BOGA_xBCDEFGH(), BOMA_Axxxxxxx(), UPOJ_MODE_PRODn(), _ASOL_POR_DONEn.tp());

  CPU_PIN_SYS_RSTp = AFER_SYS_RSTp();
  CPU_PIN_EXT_RST  = SYS_PIN_RSTp.tp();
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_dbg_slow(const SchematicTop& top) {
  /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not1(UNOR_MODE_DBG2p());
  /*p25.SOTO*/ _SOTO_DBG_VRAM = dff17_A(_SYCY_DBG_CLOCKn, CUNU_SYS_RSTn(), _SOTO_DBG_VRAM.qn());

  CPU_PIN_UNOR_DBG = UNOR_MODE_DBG2p();
  CPU_PIN_UMUT_DBG = UMUT_MODE_DBG1p();
}


//-----------------------------------------------------------------------------

void ClockRegisters::tock_vid_slow(const SchematicTop& top) {
  /*p29.XYVA*/ wire XYVA_xBxDxFxH = not1(ZEME_AxCxExGx());
  /*p29.XOTA*/ wire XOTA_AxCxExGx = not1(XYVA_xBxDxFxH);
  /*p29.XYFY*/ wire XYFY_xBxDxFxH = not1(XOTA_AxCxExGx);

  /*p29.WUVU*/ WUVU_xxCDxxGH = dff17_A (XOTA_AxCxExGx,      XAPO_VID_RSTn(), WUVU_xxCDxxGH.qn());
  /*p21.VENA*/ VENA_ABCDxxxx = dff17_A (WUVU_xxCDxxGH.qn(), XAPO_VID_RSTn(), VENA_ABCDxxxx.qn());
  /*p29.WOSU*/ WOSU_xBCxxFGx = dff17_AB(XYFY_xBxDxFxH,      XAPO_VID_RSTn(), WUVU_xxCDxxGH.qn());
}

//-----------------------------------------------------------------------------
