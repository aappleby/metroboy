#include "Sch_Clocks.h"
#include "Sch_Top.h"

using namespace Schematics;

#pragma warning(disable:4100) // unreferenced parameter

#define FAST_BOOT
#define PHASE(A) ((A) & (1 << (7 - phase)))

void ClockRegisters::dump(Dumper& d, const SchematicTop& top) const {
  d("----------  Clock   ----------\n");
  d("SYS_PIN_CLK_A %d\n", (wire)_SYS_PIN_CLK_A);
  d("SYS_PIN_CLK_B %d\n", (wire)_SYS_PIN_CLK_B);
  d("SYS_PIN_RSTp  %d\n", (wire)_SYS_PIN_RSTp);
  d("SYS_PIN_T2n   %d\n", (wire)_SYS_PIN_T2n);
  d("SYS_PIN_T1n   %d\n", (wire)_SYS_PIN_T1n);

  d("TUBO %d\n", (wire)_TUBO_WAITINGp);
  d("ASOL %d\n", (wire)_ASOL_POR_DONEn);
  d("AFER %d\n", (wire)_AFER_SYS_RSTp.q());
  d("SOTO %d\n", (wire)_SOTO_DBG_VRAM.q());

  d("AFUR_ABCDxxxx %d\n", _AFUR_ABCDxxxx.q());
  d("ALEF_xBCDExxx %d\n", _ALEF_xBCDExxx.q());
  d("APUK_xxCDEFxx %d\n", _APUK_xxCDEFxx.q());
  d("ADYK_xxxDEFGx %d\n", _ADYK_xxxDEFGx.q());

  d("WUVU_ABxxEFxx %d\n", WUVU_ABxxEFxx.q());
  d("VENA_xxCDEFxx %d\n", VENA_xxCDEFxx.q());
  d("WOSU_AxxDExxH %d\n", WOSU_AxxDExxH.q());
  
  d("CPU_PIN_STARTp........%d\n", (wire)CPU_PIN_STARTp);
  d("CPU_PIN_READYp........%d\n", (wire)CPU_PIN_READYp);
  d("CPU_PIN_SYS_RSTp......%d\n", (wire)CPU_PIN_SYS_RSTp);
  d("CPU_PIN_EXT_RST.......%d\n", (wire)CPU_PIN_EXT_RST);
  d("CPU_PIN_UNOR_DBG......%d\n", (wire)CPU_PIN_UNOR_DBG);
  d("CPU_PIN_UMUT_DBG......%d\n", (wire)CPU_PIN_UMUT_DBG);
  d("CPU_PIN_EXT_CLKGOOD...%d\n", (wire)CPU_PIN_EXT_CLKGOOD);
  d("CPU_PIN_BOWA_xBCDEFGH.%d\n", (wire)CPU_PIN_BOWA_xBCDEFGH);
  d("CPU_PIN_BEDO_Axxxxxxx.%d\n", (wire)CPU_PIN_BEDO_Axxxxxxx);
  d("CPU_PIN_BEKO_ABCDxxxx.%d\n", (wire)CPU_PIN_BEKO_ABCDxxxx);
  d("CPU_PIN_BUDE_xxxxEFGH.%d\n", (wire)CPU_PIN_BUDE_xxxxEFGH);
  d("CPU_PIN_BOLO_ABCDEFxx.%d\n", (wire)CPU_PIN_BOLO_ABCDEFxx);
  d("CPU_PIN_BUKE_AxxxxxGH.%d\n", (wire)CPU_PIN_BUKE_AxxxxxGH);
  d("CPU_PIN_BOMA_Axxxxxxx.%d\n", (wire)CPU_PIN_BOMA_Axxxxxxx);
  d("CPU_PIN_BOGA_xBCDEFGH.%d\n", (wire)CPU_PIN_BOGA_xBCDEFGH);
  d("EXT_PIN_CLK_xxxxEFGH..%d\n", (wire)EXT_PIN_CLK_xxxxEFGH);

  d("----------  Reset   ----------\n");
  /*p01.XEBE*/ wire _XEBE_SYS_RSTn = not1(XORE_SYS_RSTp());
  /*p01.XODO*/ wire _XODO_VID_RSTp = nand2(_XEBE_SYS_RSTn, top.pix_pipe.XONA_LCDC_EN.q());
  /*p01.XAPO*/ wire _XAPO_VID_RSTn = not1(_XODO_VID_RSTp);
  d("_XEBE_SYS_RSTn %d\n", _XEBE_SYS_RSTn);
  d("XONA1 %d\n", _XONA_LCDC_EN.as_wire());
  d("XONA2 %d\n", top.pix_pipe.XONA_LCDC_EN.q());
  d("_XODO_VID_RSTp %d\n", _XODO_VID_RSTp);
  d("_XAPO_VID_RSTn %d\n", _XAPO_VID_RSTn);

  d("\n");
}


//-----------------------------------------------------------------------------

void ClockRegisters::tick_fast(const SchematicTop& top) {
  _XONA_LCDC_EN = top.pix_pipe.XONA_LCDC_EN.q();
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_clk_fast(int phase, const SchematicTop& top) {
  wire upoj_run = _SYS_PIN_T1n || _SYS_PIN_T2n || !_SYS_PIN_RSTp;

  wire clk_ABCDxxxx = _AFUR_ABCDxxxx.q() & upoj_run;
  wire clk_xBCDExxx = _ALEF_xBCDExxx.q() & upoj_run;
  wire clk_xxCDEFxx = _APUK_xxCDEFxx.q() & upoj_run;
  wire clk_xxxDEFGx = _ADYK_xxxDEFGx.q() & upoj_run;

  wire clk_xxxxEFGH = !clk_ABCDxxxx;
  wire clk_AxxxxFGH = !clk_xBCDExxx;
  wire clk_ABxxxxGH = !clk_xxCDEFxx;

  wire clk_xxxxxxGH = and2(clk_ABxxxxGH, clk_xxxxEFGH);
  wire clk_AxxxxxGH = and2(clk_AxxxxFGH, clk_ABxxxxGH);
  wire clk_xBCDEFGH = or3(clk_xBCDExxx, clk_xxxxEFGH, clk_xxxxxxGH);

  CPU_PIN_BEDO_Axxxxxxx = !CPU_PIN_READYp ? DELTA_TRI0 : !_SYS_PIN_CLK_A ? DELTA_TRI1 : !clk_xBCDEFGH;
  CPU_PIN_BOWA_xBCDEFGH = !CPU_PIN_READYp ? DELTA_TRI1 : !_SYS_PIN_CLK_A ? DELTA_TRI0 :  clk_xBCDEFGH;

  CPU_PIN_BEKO_ABCDxxxx = !CPU_PIN_READYp ? DELTA_TRI1 : !clk_xxxxEFGH;
  CPU_PIN_BUDE_xxxxEFGH = !CPU_PIN_READYp ? DELTA_TRI0 :  clk_xxxxEFGH;

  CPU_PIN_BOLO_ABCDEFxx = !CPU_PIN_READYp ? DELTA_TRI1 : !clk_xxxxxxGH;
  CPU_PIN_BUKE_AxxxxxGH = !CPU_PIN_READYp ? DELTA_TRI0 :  clk_AxxxxxGH;

  CPU_PIN_BOMA_Axxxxxxx = !_SYS_PIN_CLK_A ? DELTA_TRI1 : !clk_xBCDEFGH;
  CPU_PIN_BOGA_xBCDEFGH = !_SYS_PIN_CLK_A ? DELTA_TRI0 :  clk_xBCDEFGH;

  EXT_PIN_CLK_xxxxEFGH = clk_xxxxEFGH & CPU_PIN_READYp;

  CPU_PIN_EXT_CLKGOOD = (wire)_SYS_PIN_CLK_A;

  _AFUR_ABCDxxxx = dff(!_SYS_PIN_CLK_B, upoj_run, !clk_xxxDEFGx);
  _ALEF_xBCDExxx = dff( _SYS_PIN_CLK_B, upoj_run,  clk_ABCDxxxx);
  _APUK_xxCDEFxx = dff(!_SYS_PIN_CLK_B, upoj_run,  clk_xBCDExxx);
  _ADYK_xxxDEFGx = dff( _SYS_PIN_CLK_B, upoj_run,  clk_xxCDEFxx);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_rst_fast(int phase, const SchematicTop& top) {
  wire unor = and2(!_SYS_PIN_T1n,  _SYS_PIN_T2n);
  wire umut = and2( _SYS_PIN_T1n, !_SYS_PIN_T2n);
  wire upoj = _SYS_PIN_T1n || _SYS_PIN_T2n || !_SYS_PIN_RSTp;


  if (CPU_PIN_READYp) {
    _TUBO_WAITINGp = DELTA_TRI0;
  }
  else if (_SYS_PIN_RSTp || !_SYS_PIN_CLK_A) {
    _TUBO_WAITINGp = DELTA_TRI1;
  }
  else {
    _TUBO_WAITINGp = DELTA_HOLD;
  }

#ifdef FAST_BOOT
  wire _UNUT_POR_TRIGn = and2(_TUBO_WAITINGp, top.tim_reg.TERO_DIV_03());
#else
  wire _UNUT_POR_TRIGn = and2(_TUBO_WAITINGp, top.tim_reg.UPOF_DIV_15());
#endif

  wire _TABA_POR_TRIGn = or3(unor, umut, _UNUT_POR_TRIGn);
  CPU_PIN_STARTp = _TABA_POR_TRIGn;
 
  if (and2(_TABA_POR_TRIGn, !_SYS_PIN_RSTp)) {
    _ASOL_POR_DONEn = DELTA_TRI0;
  }
  else if (_SYS_PIN_RSTp) {
    _ASOL_POR_DONEn = DELTA_TRI1;
  }
  else {
    _ASOL_POR_DONEn = DELTA_HOLD;
  }

  _AFER_SYS_RSTp = dff13(BOGA_xBCDEFGH(), BOMA_Axxxxxxx(), upoj, _ASOL_POR_DONEn);

  CPU_PIN_SYS_RSTp = _AFER_SYS_RSTp.q();
  CPU_PIN_EXT_RST  = (wire)_SYS_PIN_RSTp;
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_dbg_fast(int phase, const SchematicTop& top) {
  wire unor = and2(!_SYS_PIN_T1n,  _SYS_PIN_T2n);
  wire umut = and2( _SYS_PIN_T1n, !_SYS_PIN_T2n);

  wire soto_rst = or2(_AFER_SYS_RSTp.q(), _ASOL_POR_DONEn);
  _SOTO_DBG_VRAM = dff17(!unor, !soto_rst, _SOTO_DBG_VRAM.qn());

  CPU_PIN_UNOR_DBG = unor;
  CPU_PIN_UMUT_DBG = umut;
}

//-----------------------------------------------------------------------------
// can't do fast mode for this until vid clock is running

void ClockRegisters::tock_vid_fast(int phase, const SchematicTop& top) {
  wire vid_reset = or3(_AFER_SYS_RSTp.q(), _ASOL_POR_DONEn, !_XONA_LCDC_EN);

  wire WUVU = WUVU_ABxxEFxx.q();
  wire VENA = VENA_xxCDEFxx.q();

  WUVU_ABxxEFxx = dff17(!_SYS_PIN_CLK_B,  !vid_reset, !WUVU);
  WOSU_AxxDExxH = dff17( _SYS_PIN_CLK_B,  !vid_reset, !WUVU);
  VENA_xxCDEFxx = dff17(!WUVU,            !vid_reset, !VENA);
}

//-----------------------------------------------------------------------------









//======================================================================================================================

void ClockRegisters::tick_slow(const SchematicTop& top) {
  _XONA_LCDC_EN = top.pix_pipe.XONA_LCDC_EN.q();
}

void ClockRegisters::tock_clk_slow(int phase, const SchematicTop& top) {
  // ignoring the deglitcher here

  {
    wire AFUR_ABCDxxxx_ = _AFUR_ABCDxxxx.q();
    wire ALEF_xBCDExxx_ = _ALEF_xBCDExxx.q();
    wire APUK_xxCDEFxx_ = _APUK_xxCDEFxx.q();
    wire ADYK_xxxDEFGx_ = _ADYK_xxxDEFGx.q();

    // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

    /*p07.UBET*/ wire UBET_T1p        = not1(_SYS_PIN_T1n);
    /*p07.UVAR*/ wire UVAR_T2p        = not1(_SYS_PIN_T2n);
    /*p07.UPOJ*/ wire UPOJ_MODE_PRODn = nand3(UBET_T1p, UVAR_T2p, _SYS_PIN_RSTp);

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
    // ALEF_08 >> APUK_03 : Qn?
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

    // does this phase clock actually work?
    ///*p01.AFUR*/ _AFUR_ABCDxxxx = dff9(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), UPOJ_MODE_PRODn,  ADYK_xxxDEFGx_);
    ///*p01.ALEF*/ _ALEF_xBCDExxx = dff9( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, !AFUR_ABCDxxxx_);
    ///*p01.APUK*/ _APUK_xxCDEFxx = dff9(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, !ALEF_xBCDExxx_);
    ///*p01.ADYK*/ _ADYK_xxxDEFGx = dff9( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, !APUK_xxCDEFxx_);

    /*p01.AFUR*/ _AFUR_ABCDxxxx = dff9(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, !ADYK_xxxDEFGx_);
    /*p01.ALEF*/ _ALEF_xBCDExxx = dff9( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), UPOJ_MODE_PRODn,  AFUR_ABCDxxxx_);
    /*p01.APUK*/ _APUK_xxCDEFxx = dff9(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), UPOJ_MODE_PRODn,  ALEF_xBCDExxx_);
    /*p01.ADYK*/ _ADYK_xxxDEFGx = dff9( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), UPOJ_MODE_PRODn,  APUK_xxCDEFxx_);
  }

  {
    /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not1(_SYS_PIN_CLK_A);
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not1(CPU_PIN_READYp);
    /*p01.AROV*/ wire AROV_xxCDEFxx = not1(_APUK_xxCDEFxx.qn());
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not1(_ALEF_xBCDExxx.q());
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not1(_AFUR_ABCDxxxx.qn());

    /*p01.BUTY*/ wire BUTY_CLKREQ   = not1(ABOL_CLKREQn);

    /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);

    /*p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
    /*p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx);
    /*p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor2(ABOL_CLKREQn,  ATYP_ABCDxxxx);
    /*p01.BYRY*/ wire BYRY_ABCDxxxx = not1(NULE_xxxxEFGH);
    /*p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
    /*p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH);
    /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand2(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
    /*p01.BANE*/ wire BANE_ABCDxxxx = not1(BEJA_xxxxEFGH);
    /*p01.BELO*/ wire BELO_xxxxEFGH = not1(BANE_ABCDxxxx);
    /*p01.BAZE*/ wire BAZE_ABCDxxxx = not1(BELO_xxxxEFGH);
    /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand3(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
    /*p01.BELE*/ wire BELE_Axxxxxxx = not1(BUTO_xBCDEFGH);
    /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor2(BELE_Axxxxxxx, ATEZ_CLKBAD);
    /*p01.BALY*/ wire BALY_Axxxxxxx = not1(BYJU_xBCDEFGH);
    /*p01.BUVU*/ wire BUVU_Axxxxxxx = and2(BUTY_CLKREQ, BALY_Axxxxxxx);

    /*p01.BYXO*/ wire BYXO_xBCDEFGH = not1(BUVU_Axxxxxxx);
    /*p01.BEDO*/ wire BEDO_Axxxxxxx = not1(BYXO_xBCDEFGH);
    /*p01.BOWA*/ wire BOWA_xBCDEFGH = not1(BEDO_Axxxxxxx);

    /*p01.BUGO*/ wire BUGO_xBCDExxx = not1(AFEP_AxxxxFGH);
    /*p01.BATE*/ wire BATE_AxxxxxGH = nor3(ABOL_CLKREQn, BUGO_xBCDExxx, AROV_xxCDEFxx);
    /*p01.BASU*/ wire BASU_xBCDEFxx = not1(BATE_AxxxxxGH);

    /*p01.BUKE*/ wire BUKE_AxxxxxGH = not1(BASU_xBCDEFxx);
    /*p01.BOGA*/ wire BOGA_xBCDEFGH = not1(BALY_Axxxxxxx);
    /*p01.BOMA*/ wire BOMA_Axxxxxxx = not1(BOGA_xBCDEFGH);

    CPU_PIN_BOWA_xBCDEFGH = BOWA_xBCDEFGH;
    CPU_PIN_BEDO_Axxxxxxx = BEDO_Axxxxxxx;
    CPU_PIN_BEKO_ABCDxxxx = BEKO_ABCDxxxx;
    CPU_PIN_BUDE_xxxxEFGH = BUDE_xxxxEFGH;
    CPU_PIN_BOLO_ABCDEFxx = BOLO_ABCDEFxx;
    CPU_PIN_BUKE_AxxxxxGH = BUKE_AxxxxxGH;
    CPU_PIN_BOMA_Axxxxxxx = BOMA_Axxxxxxx;
    CPU_PIN_BOGA_xBCDEFGH = BOGA_xBCDEFGH;

    /* PIN_75 */ EXT_PIN_CLK_xxxxEFGH = BUDE_xxxxEFGH;
  }

  CPU_PIN_EXT_CLKGOOD = (wire)_SYS_PIN_CLK_A;
}


//-----------------------------------------------------------------------------

void ClockRegisters::tock_rst_slow(int phase, const SchematicTop& top) {
  /*p01.UPYF*/ wire _UPYF = or2(_SYS_PIN_RSTp, UCOB_CLKBADp());

  /*p01.TUBO*/ _TUBO_WAITINGp = nor_latch(_UPYF, CPU_PIN_READYp);


#ifdef FAST_BOOT
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(_TUBO_WAITINGp, top.tim_reg.TERO_DIV_03());
#else
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(_TUBO_WAITINGp.q(), top.tim_reg.UPOF_DIV_15());
#endif

  /*p01.TABA*/ wire _TABA_POR_TRIGn = or3(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  CPU_PIN_STARTp   = _TABA_POR_TRIGn;

  /*p01.ALYP*/ wire _ALYP_RSTn = not1(_TABA_POR_TRIGn);
  /*p01.AFAR*/ wire _AFAR_RST  = nor2(_ALYP_RSTn, _SYS_PIN_RSTp);

  /*p01.ASOL*/ _ASOL_POR_DONEn = nor_latch(_SYS_PIN_RSTp, _AFAR_RST); // Schematic wrong, this is a latch.

  _ASOL_POR_DONEn.dump_edge("_ASOL_POR_DONEn");

  /*p01.AFER*/ _AFER_SYS_RSTp = dff13(BOGA_xBCDEFGH(), BOMA_Axxxxxxx(), UPOJ_MODE_PRODn(), _ASOL_POR_DONEn);

  _AFER_SYS_RSTp.dump_edge("_AFER_SYS_RSTp");

  CPU_PIN_SYS_RSTp = AFER_SYS_RSTp();
  CPU_PIN_EXT_RST  = (wire)_SYS_PIN_RSTp;
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_dbg_slow(int phase, const SchematicTop& top) {
  /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not1(UNOR_MODE_DBG2p());
  /*p25.SOTO*/ _SOTO_DBG_VRAM = dff17(_SYCY_DBG_CLOCKn, CUNU_SYS_RSTn(), _SOTO_DBG_VRAM.qn());

  CPU_PIN_UNOR_DBG = UNOR_MODE_DBG2p();
  CPU_PIN_UMUT_DBG = UMUT_MODE_DBG1p();
}


//-----------------------------------------------------------------------------

// can't do fast mode for this until vid clock is running
void ClockRegisters::tock_vid_slow(int phase, const SchematicTop& top) {
  /*p29.XYVA*/ wire XYVA_xBxDxFxH = not1(ZEME_AxCxExGx());
  /*p29.XOTA*/ wire XOTA_AxCxExGx = not1(XYVA_xBxDxFxH);
  /*p29.XYFY*/ wire XYFY_xBxDxFxH = not1(XOTA_AxCxExGx);

  wire WUVU = WUVU_ABxxEFxx.q();
  wire VENA = VENA_xxCDEFxx.q();

  /*p29.WUVU*/ WUVU_ABxxEFxx = dff17( XOTA_AxCxExGx, XAPO_VID_RSTn(), !WUVU);
  /*p21.VENA*/ VENA_xxCDEFxx = dff17(!WUVU,          XAPO_VID_RSTn(), !VENA);
  /*p29.WOSU*/ WOSU_AxxDExxH = dff17( XYFY_xBxDxFxH, XAPO_VID_RSTn(), !WUVU);
}

//-----------------------------------------------------------------------------