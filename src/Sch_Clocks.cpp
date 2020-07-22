#include "Sch_Clocks.h"
#include "Sch_Top.h"

using namespace Schematics;

#pragma warning(disable:4100) // unreferenced parameter

#define FAST_BOOT
#define PHASE(A) ((A) & (1 << (7 - phase)))

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

  wire clk_xxxxxxGH = and(clk_ABxxxxGH, clk_xxxxEFGH);
  wire clk_AxxxxxGH = and(clk_AxxxxFGH, clk_ABxxxxGH);
  wire clk_xBCDEFGH = or (clk_xBCDExxx, clk_xxxxEFGH, clk_xxxxxxGH);

  _CPU_PIN_BEDO_Axxxxxxx = !clk_xBCDEFGH;
  _CPU_PIN_BOWA_xBCDEFGH =  clk_xBCDEFGH;

  _CPU_PIN_BEKO_ABCDxxxx = !clk_xxxxEFGH;
  _CPU_PIN_BUDE_xxxxEFGH =  clk_xxxxEFGH;

  _CPU_PIN_BOLO_ABCDEFxx = !clk_xxxxxxGH;
  _CPU_PIN_BUKE_AxxxxxGH =  clk_AxxxxxGH;

  _CPU_PIN_BOMA_Axxxxxxx = !clk_xBCDEFGH;
  _CPU_PIN_BOGA_xBCDEFGH =  clk_xBCDEFGH;

  if (!_SYS_PIN_CLK_A) {
    _CPU_PIN_BEDO_Axxxxxxx.delta = DELTA_SIG1;
    _CPU_PIN_BOWA_xBCDEFGH.delta = DELTA_SIG0;
    _CPU_PIN_BOMA_Axxxxxxx.delta = DELTA_SIG1;
    _CPU_PIN_BOGA_xBCDEFGH.delta = DELTA_SIG0;
  }

  if (!_CPU_PIN_READYp) {
    _CPU_PIN_BEDO_Axxxxxxx.delta = DELTA_SIG0;
    _CPU_PIN_BOWA_xBCDEFGH.delta = DELTA_SIG1;
    _CPU_PIN_BEKO_ABCDxxxx.delta = DELTA_SIG1;
    _CPU_PIN_BUDE_xxxxEFGH.delta = DELTA_SIG0;
    _CPU_PIN_BOLO_ABCDEFxx.delta = DELTA_SIG1;
    _CPU_PIN_BUKE_AxxxxxGH.delta = DELTA_SIG0;
  }

  _EXT_PIN_CLK_xxxxEFGH = clk_xxxxEFGH & _CPU_PIN_READYp;

  _CPU_PIN_EXT_CLKGOOD = _SYS_PIN_CLK_A;

  _AFUR_ABCDxxxx.delta = DELTA_D0C0 | (!clk_xxxDEFGx << 0);
  _ALEF_xBCDExxx.delta = DELTA_D0C0 | ( clk_ABCDxxxx << 0);
  _APUK_xxCDEFxx.delta = DELTA_D0C0 | ( clk_xBCDExxx << 0);
  _ADYK_xxxDEFGx.delta = DELTA_D0C0 | ( clk_xxCDEFxx << 0);

  if (!upoj_run) {
    _AFUR_ABCDxxxx.delta = DELTA_A0C0;
    _ALEF_xBCDExxx.delta = DELTA_A0C0;
    _APUK_xxCDEFxx.delta = DELTA_A0C0;
    _ADYK_xxxDEFGx.delta = DELTA_A0C0;
  }

  _AFUR_ABCDxxxx.delta |= (!_SYS_PIN_CLK_B << 1);
  _ALEF_xBCDExxx.delta |= ( _SYS_PIN_CLK_B << 1);
  _APUK_xxCDEFxx.delta |= (!_SYS_PIN_CLK_B << 1);
  _ADYK_xxxDEFGx.delta |= ( _SYS_PIN_CLK_B << 1);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_rst_fast(int phase, const SchematicTop& top) {
  _TUBO_WAITINGp.delta = DELTA_PASS;
  if (_SYS_PIN_RSTp || !_SYS_PIN_CLK_A) _TUBO_WAITINGp.delta = DELTA_SIG1;
  if (_CPU_PIN_READYp)                  _TUBO_WAITINGp.delta = DELTA_SIG0;

#ifdef FAST_BOOT
  wire _UNUT_POR_TRIGn = and (_TUBO_WAITINGp.q(), top.tim_reg.TERO_DIV_03());
#else
  wire _UNUT_POR_TRIGn = and (_TUBO_WAITINGp.q(), top.tim_reg.UPOF_DIV_15());
#endif

  wire _TABA_POR_TRIGn = or(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  _CPU_PIN_STARTp = _TABA_POR_TRIGn;

  _ASOL_POR_DONEn.delta = DELTA_PASS;
  if (_SYS_PIN_RSTp) _ASOL_POR_DONEn.delta = DELTA_SIG1;
  if (and(_TABA_POR_TRIGn, !_SYS_PIN_RSTp))     _ASOL_POR_DONEn.delta = DELTA_SIG0;

  _AFER_SYS_RSTp = ff13_r2(BOGA_xBCDEFGH(), BOMA_Axxxxxxx(), UPOJ_MODE_PRODn(), _ASOL_POR_DONEn.q());

  _CPU_PIN_SYS_RSTp = AFER_SYS_RSTp();
  _CPU_PIN_EXT_RST  = _SYS_PIN_RSTp.as_wire();
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_dbg_fast(int phase, const SchematicTop& top) {
  /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not(UNOR_MODE_DBG2p());
  /*p25.SOTO*/ _SOTO_DBG_VRAM = ff17_r2(_SYCY_DBG_CLOCKn, CUNU_SYS_RSTn(), _SOTO_DBG_VRAM.qn());

  _CPU_PIN_UNOR_DBG = UNOR_MODE_DBG2p();
  _CPU_PIN_UMUT_DBG = UMUT_MODE_DBG1p();
}

//-----------------------------------------------------------------------------
// can't do fast mode for this until vid clock is running

void ClockRegisters::tock_vid_fast(int phase, const SchematicTop& top) {
  wire vid_reset = or(_AFER_SYS_RSTp.q(), _ASOL_POR_DONEn.q(), !_XONA_LCDC_EN);

  wire WUVU_xxCDxxGH_ = _WUVU_xxCDxxGH.q();
  wire VENA_xxxxEFGH_ = _VENA_xxxxEFGH.q();

  _WUVU_xxCDxxGH = ff17_r2(!_SYS_PIN_CLK_B,  !vid_reset, !WUVU_xxCDxxGH_);
  _WOSU_xBCxxFGx = ff17_r2( _SYS_PIN_CLK_B,  !vid_reset, !WUVU_xxCDxxGH_);
  _VENA_xxxxEFGH = ff17_r2(!WUVU_xxCDxxGH_,  !vid_reset, !VENA_xxxxEFGH_);
}










//======================================================================================================================

void ClockRegisters::tick_slow(const SchematicTop& top) {
  _XONA_LCDC_EN = top.pix_pipe.XONA_LCDC_EN.q();
}

void ClockRegisters::tock_clk_slow(int phase, const SchematicTop& top) {
  // ignoring the deglitcher here

  {
    wire AFUR_xBCDExxx_ = _AFUR_ABCDxxxx.q();
    wire ALEF_xxCDEFxx_ = _ALEF_xBCDExxx.q();
    wire APUK_xxxDEFGx_ = _APUK_xxCDEFxx.q();
    wire ADYK_xxxxEFGH_ = _ADYK_xxxDEFGx.q();

    // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

    /*p07.UBET*/ wire UBET_T1p        = not(_SYS_PIN_T1n);
    /*p07.UVAR*/ wire UVAR_T2p        = not(_SYS_PIN_T2n);
    /*p07.UPOJ*/ wire UPOJ_MODE_PRODn = nand(UBET_T1p, UVAR_T2p, _SYS_PIN_RSTp);

    /*p01.AFUR*/ _AFUR_ABCDxxxx = ff9_r2(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), UPOJ_MODE_PRODn, !ADYK_xxxxEFGH_);
    /*p01.ALEF*/ _ALEF_xBCDExxx = ff9_r2( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), UPOJ_MODE_PRODn,  AFUR_xBCDExxx_);
    /*p01.APUK*/ _APUK_xxCDEFxx = ff9_r2(!ATAL_xBxDxFxH(),  ATAL_xBxDxFxH(), UPOJ_MODE_PRODn,  ALEF_xxCDEFxx_);
    /*p01.ADYK*/ _ADYK_xxxDEFGx = ff9_r2( ATAL_xBxDxFxH(), !ATAL_xBxDxFxH(), UPOJ_MODE_PRODn,  APUK_xxxDEFGx_);

  }

  {
    /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(_SYS_PIN_CLK_A);
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(_CPU_PIN_READYp);
    /*p01.AROV*/ wire AROV_xxCDEFxx = not(_APUK_xxCDEFxx.qn());
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not(_ALEF_xBCDExxx.q());
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(_AFUR_ABCDxxxx.qn());

    /*p01.BUTY*/ wire BUTY_CLKREQ   = not(ABOL_CLKREQn);

    /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);

    /*p01.BERU*/ wire BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
    /*p01.BUFA*/ wire BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
    /*p01.BOLO*/ wire BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor(ABOL_CLKREQn,  ATYP_ABCDxxxx);
    /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
    /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
    /*p01.BEKO*/ wire BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
    /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
    /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
    /*p01.BELO*/ wire BELO_xxxxEFGH = not(BANE_ABCDxxxx);
    /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
    /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
    /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
    /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, ATEZ_CLKBAD);
    /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
    /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(BUTY_CLKREQ, BALY_Axxxxxxx);

    /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
    /*p01.BEDO*/ wire BEDO_Axxxxxxx = not(BYXO_xBCDEFGH);
    /*p01.BOWA*/ wire BOWA_xBCDEFGH = not(BEDO_Axxxxxxx);

    /*p01.BUGO*/ wire BUGO_xBCDExxx = not(AFEP_AxxxxFGH);
    /*p01.BATE*/ wire BATE_AxxxxxGH = nor(ABOL_CLKREQn, BUGO_xBCDExxx, AROV_xxCDEFxx);
    /*p01.BASU*/ wire BASU_xBCDEFxx = not(BATE_AxxxxxGH);

    /*p01.BUKE*/ wire BUKE_AxxxxxGH = not(BASU_xBCDEFxx);
    /*p01.BOGA*/ wire BOGA_xBCDEFGH = not(BALY_Axxxxxxx);
    /*p01.BOMA*/ wire BOMA_Axxxxxxx = not(BOGA_xBCDEFGH);

    _CPU_PIN_BOWA_xBCDEFGH = BOWA_xBCDEFGH;
    _CPU_PIN_BEDO_Axxxxxxx = BEDO_Axxxxxxx;
    _CPU_PIN_BEKO_ABCDxxxx = BEKO_ABCDxxxx;
    _CPU_PIN_BUDE_xxxxEFGH = BUDE_xxxxEFGH;
    _CPU_PIN_BOLO_ABCDEFxx = BOLO_ABCDEFxx;
    _CPU_PIN_BUKE_AxxxxxGH = BUKE_AxxxxxGH;
    _CPU_PIN_BOMA_Axxxxxxx = BOMA_Axxxxxxx;
    _CPU_PIN_BOGA_xBCDEFGH = BOGA_xBCDEFGH;

    /* PIN_75 */ _EXT_PIN_CLK_xxxxEFGH = BUDE_xxxxEFGH;
  }

  _CPU_PIN_EXT_CLKGOOD = _SYS_PIN_CLK_A.as_wire();
}


//-----------------------------------------------------------------------------

void ClockRegisters::tock_rst_slow(int phase, const SchematicTop& top) {
  /*p01.UPYF*/ wire _UPYF = or(_SYS_PIN_RSTp, UCOB_CLKBADp());

  /*p01.TUBO*/ _TUBO_WAITINGp = nor_latch_r2(_UPYF, CPU_PIN_READYp());


#ifdef FAST_BOOT
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and (_TUBO_WAITINGp.q(), top.tim_reg.TERO_DIV_03());
#else
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and (_TUBO_WAITINGp.q(), top.tim_reg.UPOF_DIV_15());
#endif

  /*p01.TABA*/ wire _TABA_POR_TRIGn = or(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  _CPU_PIN_STARTp   = _TABA_POR_TRIGn;

  /*p01.ALYP*/ wire _ALYP_RSTn = not(_TABA_POR_TRIGn);
  /*p01.AFAR*/ wire _AFAR_RST  = nor(_ALYP_RSTn, _SYS_PIN_RSTp);

  /*p01.ASOL*/ _ASOL_POR_DONEn = nor_latch_r2(_SYS_PIN_RSTp, _AFAR_RST); // Schematic wrong, this is a latch.


  /*p01.AFER*/ _AFER_SYS_RSTp = ff13_r2(BOGA_xBCDEFGH(), BOMA_Axxxxxxx(), UPOJ_MODE_PRODn(), _ASOL_POR_DONEn.q());

  _CPU_PIN_SYS_RSTp = AFER_SYS_RSTp();
  _CPU_PIN_EXT_RST  = _SYS_PIN_RSTp.as_wire();
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_dbg_slow(int phase, const SchematicTop& top) {
  /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not(UNOR_MODE_DBG2p());
  /*p25.SOTO*/ _SOTO_DBG_VRAM = ff17_r2(_SYCY_DBG_CLOCKn, CUNU_SYS_RSTn(), _SOTO_DBG_VRAM.qn());

  _CPU_PIN_UNOR_DBG = UNOR_MODE_DBG2p();
  _CPU_PIN_UMUT_DBG = UMUT_MODE_DBG1p();
}


//-----------------------------------------------------------------------------

// can't do fast mode for this until vid clock is running
void ClockRegisters::tock_vid_slow(int phase, const SchematicTop& top) {
  /*p29.XYVA*/ wire XYVA_xBxDxFxH = not(ZEME_AxCxExGx());
  /*p29.XOTA*/ wire _XOTA_AxCxExGx = not(XYVA_xBxDxFxH);
  /*p29.XYFY*/ wire _XYFY_xBxDxFxH = not(_XOTA_AxCxExGx);

  wire WUVU_xxCDxxGH_ = _WUVU_xxCDxxGH.q();
  wire VENA_xxxxEFGH_ = _VENA_xxxxEFGH.q();

  /*p29.WUVU*/ _WUVU_xxCDxxGH = ff17_r2( _XOTA_AxCxExGx, XAPO_VID_RSTn(), !WUVU_xxCDxxGH_);
  /*p21.VENA*/ _VENA_xxxxEFGH = ff17_r2(!WUVU_xxCDxxGH_, XAPO_VID_RSTn(), !VENA_xxxxEFGH_);
  /*p29.WOSU*/ _WOSU_xBCxxFGx = ff17_r2( _XYFY_xBxDxFxH, XAPO_VID_RSTn(), !WUVU_xxCDxxGH_);
}

//-----------------------------------------------------------------------------