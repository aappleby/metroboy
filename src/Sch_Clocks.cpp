#include "Sch_Clocks.h"
#include "Sch_Top.h"

using namespace Schematics;

#pragma warning(disable:4100) // unreferenced parameter

#define FAST_BOOT
#define PHASE(A) ((A) & (1 << (7 - phase)))

void ClockRegisters::dump(Dumper& d, const SchematicTop& top) const {
  d("---------- Sys Pins ----------\n");
  d("SYS_PIN_CLK_A %d\n", _SYS_PIN_CLK_A.qp());
  d("SYS_PIN_CLK_B %d\n", _SYS_PIN_CLK_B.qp());
  d("SYS_PIN_RSTp  %d\n", _SYS_PIN_RSTp.qp());
  d("SYS_PIN_T2n   %d\n", _SYS_PIN_T2n.qp());
  d("SYS_PIN_T1n   %d\n", _SYS_PIN_T1n.qp());
  d("\n");

  d("---------- Phase Clock ----------\n");
  d("PHASE         %c%c%c%c\n", AFUR_ABCDxxxx.c(),
                                ALEF_AxxxxFGH.cn(),
                                APUK_xxCDEFxx.c(),
                                ADYK_ABCxxxxH.cn());
  d("ATEZ_CLKBAD   %d\n", ATEZ_CLKBAD.qp());
  d("ABOL_CLKREQn  %d\n", ABOL_CLKREQn.qp());
  d("AFUR_ABCDxxxx %d\n", AFUR_ABCDxxxx.qp());
  d("ALEF_AxxxxFGH %d\n", ALEF_AxxxxFGH.qp());
  d("APUK_xxCDEFxx %d\n", APUK_xxCDEFxx.qp());
  d("ADYK_ABCxxxxH %d\n", ADYK_ABCxxxxH.qp());
  d("AROV_ABxxxxGH %d\n", AROV_ABxxxxGH.qp());
  d("AFEP_AxxxxFGH %d\n", AFEP_AxxxxFGH.qp());
  d("ATYP_xxxxEFGH %d\n", ATYP_xxxxEFGH.qp());
  d("AJAX_ABCDxxxx %d\n", AJAX_ABCDxxxx.qp());
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
  d("CPU_PIN_STARTp        : %d\n", CPU_PIN_STARTp.qp());
  d("CPU_PIN_READYp        : %d\n", CPU_PIN_READYp.qp());
  d("CPU_PIN_SYS_RSTp      : %d\n", CPU_PIN_SYS_RSTp.qp());
  d("CPU_PIN_EXT_RST       : %d\n", CPU_PIN_EXT_RST.qp());
  d("CPU_PIN_UNOR_DBG      : %d\n", CPU_PIN_UNOR_DBG.qp());
  d("CPU_PIN_UMUT_DBG      : %d\n", CPU_PIN_UMUT_DBG.qp());
  d("CPU_PIN_EXT_CLKGOOD   : %d\n", CPU_PIN_EXT_CLKGOOD.qp());
  d("CPU_PIN_BOWA_ABCDExxx : %d\n", CPU_PIN_BOWA_ABCDExxx.qp());
  d("CPU_PIN_BEDO_xxxxxFGH : %d\n", CPU_PIN_BEDO_xxxxxFGH.qp());
  d("CPU_PIN_BEKO_xxxxEFGH : %d\n", CPU_PIN_BEKO_xxxxEFGH.qp());
  d("CPU_PIN_BUDE_ABCDxxxx : %d\n", CPU_PIN_BUDE_ABCDxxxx.qp());
  d("CPU_PIN_BOLO_ABxxEFGH : %d\n", CPU_PIN_BOLO_ABxxEFGH.qp());
  d("CPU_PIN_BUKE_xxxxxFxx : %d\n", CPU_PIN_BUKE_xxxxxFxx.qp());
  d("CPU_PIN_BOMA_xxxxxFGH : %d\n", CPU_PIN_BOMA_xxxxxFGH.qp());
  d("CPU_PIN_BOGA_ABCDExxx : %d\n", CPU_PIN_BOGA_ABCDExxx.qp());
  d("EXT_PIN_CLK_ABCDxxxx  : %d\n", EXT_PIN_CLK_ABCDxxxx.qp());
  d("\n");

  d("----------  Reset   ----------\n");
  /*p01.XEBE*/ wire _XEBE_SYS_RSTn = not1(XORE_SYS_RSTp());
  /*p01.XODO*/ wire _XODO_VID_RSTp = nand2(_XEBE_SYS_RSTn, top.pix_pipe.XONA_LCDC_EN.qp());
  /*p01.XAPO*/ wire _XAPO_VID_RSTn = not1(_XODO_VID_RSTp);
  d("TUBO %d\n", _TUBO_WAITINGp.qp());
  d("ASOL %d\n", _ASOL_POR_DONEn.qp());
  d("AFER %d\n", _AFER_SYS_RSTp.qp());
  d("SOTO %d\n", !_SOTO_DBG_VRAM.qn());
  d("_XEBE_SYS_RSTn %d\n", _XEBE_SYS_RSTn);
  d("XONA1 %d\n", _XONA_LCDC_EN.as_wire());
  d("XONA2 %d\n", top.pix_pipe.XONA_LCDC_EN.qp());
  d("_XODO_VID_RSTp %d\n", _XODO_VID_RSTp);
  d("_XAPO_VID_RSTn %d\n", _XAPO_VID_RSTn);
  d("\n");
}

//======================================================================================================================

void ClockRegisters::tick_slow(const SchematicTop& top) {
  _XONA_LCDC_EN = top.pix_pipe.XONA_LCDC_EN.qp();
}

void ClockRegisters::tock_clk_slow(int phase, const SchematicTop& top) {
  // ignoring the deglitcher here

  {
    wire AFUR_ABCDxxxx_ = AFUR_ABCDxxxx.qp();
    wire ALEF_AxxxxFGH_ = ALEF_AxxxxFGH.qp();
    wire APUK_xxCDEFxx_ = APUK_xxCDEFxx.qp();
    wire ADYK_ABCxxxxH_ = ADYK_ABCxxxxH.qp();

    // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

    /*p07.UBET*/ wire UBET_T1p        = not1(_SYS_PIN_T1n.qp());
    /*p07.UVAR*/ wire UVAR_T2p        = not1(_SYS_PIN_T2n.qp());
    /*p07.UPOJ*/ wire UPOJ_MODE_PRODn = nand3(UBET_T1p, UVAR_T2p, _SYS_PIN_RSTp.qp());

    // AFUR_01 NC
    // AFUR_02 << !ATAL_03: CLKp
    // AFUR_03 << ADYK_09 : D
    // AFUR_04 << ATAL_03 : CLKn
    // AFUR_05 NC
    // AFUR_06 << UPOJ_04 : RSTp
    // AFUR_07 NC
    // AFUR_08 >> ALEF_03 : Qn?
    // AFUR_09 >> ATYP_01 : Q?

    // ALEF_01 NC
    // ALEF_02 << ATAL_03 : CLKp
    // ALEF_03 << AFUR_08 : D
    // ALEF_04 << !ATAL_03: CLKn
    // ALEF_05 NC
    // ALEF_06 << UPOJ_04 : RSTp
    // ALEF_07 NC
    // ALEF_08 >> APUK_03, AFEP_01 : Qn?
    // ALEF_09 >> NC      : Q?

    // APUK_01 NC
    // APUK_02 << !ATAL_03: CLKp
    // APUK_03 << ALEF_08 : D
    // APUK_04 << ATAL_03 : CLKn
    // APUK_05 NC
    // APUK_06 << UPOJ_04 : RSTp
    // APUK_07 NC
    // APUK_08 >> ADYK_03 : Qn?
    // APUK_09 >> AROV_01 : Q?

    // ADYK_01 NC
    // ADYK_02 << ATAL_03 : CLKp
    // ADYK_03 << APUK_08 : D
    // ADYK_04 << !ATAL_03: CLKn
    // ADYK_05 NC
    // ADYK_06 << UPOJ_04 : RSTp
    // ADYK_07 NC
    // ADYK_08 >> ADAR_01 : Qn?
    // ADYK_09 >> AFUR_03 : Q?

    /*p01.AFUR*/ AFUR_ABCDxxxx = dff9(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), UPOJ_MODE_PRODn,  ADYK_ABCxxxxH_);
    /*p01.ALEF*/ ALEF_AxxxxFGH = dff9( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, !AFUR_ABCDxxxx_);
    /*p01.APUK*/ APUK_xxCDEFxx = dff9(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, !ALEF_AxxxxFGH_);
    /*p01.ADYK*/ ADYK_ABCxxxxH = dff9( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, !APUK_xxCDEFxx_);
  }

  {
    /*p01.ATEZ*/ ATEZ_CLKBAD   = not1(_SYS_PIN_CLK_A.qp());
    /*p01.ABOL*/ ABOL_CLKREQn  = not1(CPU_PIN_READYp.qp());
    /*p01.BUTY*/ BUTY_CLKREQ   = not1(ABOL_CLKREQn.qp());

    /*p01.AROV*/ AROV_ABxxxxGH = not1(APUK_xxCDEFxx.qp());
    /*p01.AFEP*/ AFEP_AxxxxFGH = not1(ALEF_AxxxxFGH.qn());
    /*p01.ATYP*/ ATYP_xxxxEFGH = not1(AFUR_ABCDxxxx.qp());
    /*p01.AJAX*/ AJAX_ABCDxxxx = not1(ATYP_xxxxEFGH.qp());


    /*p01.BAPY*/ wire BAPY_xxCDxxxx = nor3(ABOL_CLKREQn,
                                           AROV_ABxxxxGH,
                                           ATYP_xxxxEFGH);

    /*p01.BERU*/ wire BERU_ABxxEFGH = not1(BAPY_xxCDxxxx);
    /*p01.BUFA*/ wire BUFA_xxCDxxxx = not1(BERU_ABxxEFGH);
    /*p01.BOLO*/ wire BOLO_ABxxEFGH = not1(BUFA_xxCDxxxx);


    /*p01.BUDE*/ wire BUDE_ABCDxxxx = not1(BYRY_xxxxEFGH());
    /*p01.BEKO*/ wire BEKO_xxxxEFGH = not1(BUDE_ABCDxxxx);
    /*p01.BEJA*/ wire BEJA_ABCDxxxx = nand2(BOLO_ABxxEFGH,
                                            BEKO_xxxxEFGH);
    /*p01.BANE*/ wire BANE_xxxxEFGH = not1(BEJA_ABCDxxxx);
    /*p01.BELO*/ wire BELO_ABCDxxxx = not1(BANE_xxxxEFGH);
    /*p01.BAZE*/ wire BAZE_xxxxEFGH = not1(BELO_ABCDxxxx);
    /*p01.BUTO*/ wire BUTO_ABCDExxx = nand3(AFEP_AxxxxFGH,
                                            ATYP_xxxxEFGH,
                                            BAZE_xxxxEFGH);
    /*p01.BELE*/ wire BELE_xxxxxFGH = not1(BUTO_ABCDExxx);
    /*p01.BYJU*/ wire BYJU_ABCDExxx = nor2(BELE_xxxxxFGH, ATEZ_CLKBAD);
    /*p01.BALY*/ wire BALY_xxxxxFGH = not1(BYJU_ABCDExxx);
    /*p01.BUVU*/ wire BUVU_xxxxxFGH = and2(BUTY_CLKREQ, BALY_xxxxxFGH);

    /*p01.BYXO*/ wire BYXO_ABCDExxx = not1(BUVU_xxxxxFGH);
    /*p01.BEDO*/ wire BEDO_xxxxxFGH = not1(BYXO_ABCDExxx);
    /*p01.BOWA*/ wire BOWA_ABCDExxx = not1(BEDO_xxxxxFGH);

    /*p01.BUGO*/ wire BUGO_xBCDExxx = not1(AFEP_AxxxxFGH);
    /*p01.BATE*/ wire BATE_xxxxxFxx = nor3(ABOL_CLKREQn,
                                           BUGO_xBCDExxx,
                                           AROV_ABxxxxGH);
    /*p01.BASU*/ wire BASU_ABCDExGH = not1(BATE_xxxxxFxx);

    /*p01.BUKE*/ wire BUKE_xxxxxFxx = not1(BASU_ABCDExGH);
    /*p01.BOGA*/ wire BOGA_ABCDExxx = not1(BALY_xxxxxFGH);
    /*p01.BOMA*/ wire BOMA_xxxxxFGH = not1(BOGA_ABCDExxx);

    CPU_PIN_BOWA_ABCDExxx = BOWA_ABCDExxx;
    CPU_PIN_BEDO_xxxxxFGH = BEDO_xxxxxFGH;

    CPU_PIN_BEKO_xxxxEFGH = BEKO_xxxxEFGH;
    CPU_PIN_BUDE_ABCDxxxx = BUDE_ABCDxxxx;

    CPU_PIN_BOLO_ABxxEFGH = BOLO_ABxxEFGH;
    CPU_PIN_BUKE_xxxxxFxx = BUKE_xxxxxFxx;
    
    CPU_PIN_BOMA_xxxxxFGH = BOMA_xxxxxFGH;
    CPU_PIN_BOGA_ABCDExxx = BOGA_ABCDExxx;

    /* PIN_75 */ EXT_PIN_CLK_ABCDxxxx = BUDE_ABCDxxxx;
  }

  CPU_PIN_EXT_CLKGOOD = (wire)_SYS_PIN_CLK_A.qp();
}


//-----------------------------------------------------------------------------

void ClockRegisters::tock_rst_slow(int phase, const SchematicTop& top) {
  /*p01.UPYF*/ wire _UPYF = or2(_SYS_PIN_RSTp.qp(), UCOB_CLKBADp());

  /*p01.TUBO*/ _TUBO_WAITINGp = nor_latch(_UPYF, CPU_PIN_READYp.qp());


#ifdef FAST_BOOT
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(_TUBO_WAITINGp.qp(), top.tim_reg.TERO_DIV_03());
#else
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(_TUBO_WAITINGp.qp(), top.tim_reg.UPOF_DIV_15());
#endif

  /*p01.TABA*/ wire _TABA_POR_TRIGn = or3(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  CPU_PIN_STARTp   = _TABA_POR_TRIGn;

  /*p01.ALYP*/ wire _ALYP_RSTn = not1(_TABA_POR_TRIGn);
  /*p01.AFAR*/ wire _AFAR_RST  = nor2(_ALYP_RSTn, _SYS_PIN_RSTp.qp());

  /*p01.ASOL*/ _ASOL_POR_DONEn = nor_latch(_SYS_PIN_RSTp.qp(), _AFAR_RST); // Schematic wrong, this is a latch.
  /*p01.AFER*/ _AFER_SYS_RSTp = dff13_B(BOGA_ABCDExxx(), BOMA_xxxxxFGH(), UPOJ_MODE_PRODn(), _ASOL_POR_DONEn.qp());

  CPU_PIN_SYS_RSTp = AFER_SYS_RSTp();
  CPU_PIN_EXT_RST  = _SYS_PIN_RSTp.qp();
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_dbg_slow(int phase, const SchematicTop& top) {
  /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not1(UNOR_MODE_DBG2p());
  /*p25.SOTO*/ _SOTO_DBG_VRAM = dff17_A(_SYCY_DBG_CLOCKn, CUNU_SYS_RSTn(), _SOTO_DBG_VRAM.qn());

  CPU_PIN_UNOR_DBG = UNOR_MODE_DBG2p();
  CPU_PIN_UMUT_DBG = UMUT_MODE_DBG1p();
}


//-----------------------------------------------------------------------------

void ClockRegisters::tock_vid_slow(int phase, const SchematicTop& top) {
  /*p29.XYVA*/ wire XYVA_xBxDxFxH = not1(ZEME_AxCxExGx());
  /*p29.XOTA*/ wire XOTA_AxCxExGx = not1(XYVA_xBxDxFxH);
  /*p29.XYFY*/ wire XYFY_xBxDxFxH = not1(XOTA_AxCxExGx);

  wire WUVUn = WUVU_xxCDxxGH.qn();
  wire VENAn = VENA_ABCDxxxx.qn();

  /*p29.WUVU*/ WUVU_xxCDxxGH = dff17_A (XOTA_AxCxExGx, XAPO_VID_RSTn(), WUVUn);
  /*p21.VENA*/ VENA_ABCDxxxx = dff17_A (WUVUn,          XAPO_VID_RSTn(), VENAn);
  /*p29.WOSU*/ WOSU_xBCxxFGx = dff17_AB(XYFY_xBxDxFxH, XAPO_VID_RSTn(), WUVUn);
}

//-----------------------------------------------------------------------------