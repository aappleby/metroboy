#include "Sch_Clocks.h"
#include "Sch_Top.h"

using namespace Schematics;

#define FAST_BOOT

#define FAST_IMPL

#define PHASE(A) (((A) >> (7 - phase)) & 1)

//-----------------------------------------------------------------------------

void ClockRegisters::tick(const SchematicTop& top) {
  _XONA_LCDC_EN = top.pix_pipe.XONA_LCDC_EN.q();
}

//-----------------------------------------------------------------------------

#ifdef FAST_IMPL

void ClockRegisters::tock(const SchematicTop& top) {
  const int phase = top.phase_count & 7;
  // ignoring the deglitcher here

  _AFUR_ABCDxxxx.hold_d(PHASE(0b11110000));
  _ALEF_xBCDExxx.hold_d(PHASE(0b01111000));
  _APUK_xxCDEFxx.hold_d(PHASE(0b00111100));
  _ADYK_xxxDEFGx.hold_d(PHASE(0b00011110));

  CPU_PIN_BEKO_ABCDxxxx.hold(PHASE(0b11110000));
  CPU_PIN_BUDE_xxxxEFGH.hold(PHASE(0b00001111));
  CPU_PIN_BOLO_ABCDEFxx.hold(PHASE(0b11111100));
  CPU_PIN_BUKE_AxxxxxGH.hold(PHASE(0b10000011));

  CPU_PIN_BOWA_xBCDEFGH.hold(!PHASE_A);
  CPU_PIN_BEDO_Axxxxxxx.hold( PHASE_A);
  CPU_PIN_BOMA_Axxxxxxx.hold( PHASE_A);

  wire upoj_run = _SYS_PIN_T1n || _SYS_PIN_T2n || !_SYS_PIN_RSTp;
  wire unor_dbg = _SYS_PIN_T2n && !_SYS_PIN_T1n;
  wire umut_dbg = _SYS_PIN_T1n && !_SYS_PIN_T2n;

  if (!upoj_run) {
    _AFUR_ABCDxxxx.hold_d(0);
    _ALEF_xBCDExxx.hold_d(0);
    _APUK_xxCDEFxx.hold_d(0);
    _ADYK_xxxDEFGx.hold_d(0);

    CPU_PIN_BEKO_ABCDxxxx.hold(0);
    CPU_PIN_BUDE_xxxxEFGH.hold(1);
    CPU_PIN_BOLO_ABCDEFxx.hold(0);
    CPU_PIN_BUKE_AxxxxxGH.hold(1);

    CPU_PIN_BOWA_xBCDEFGH.hold(1);
    CPU_PIN_BEDO_Axxxxxxx.hold(0);
    CPU_PIN_BOMA_Axxxxxxx.hold(0);
  }

  if (!SYS_PIN_CLK_A) {
    CPU_PIN_BOWA_xBCDEFGH.hold(0);
    CPU_PIN_BEDO_Axxxxxxx.hold(1);
    CPU_PIN_BOMA_Axxxxxxx.hold(1);
  }

  if (!_CPU_PIN_READYp) {
    CPU_PIN_BEKO_ABCDxxxx.hold(1);
    CPU_PIN_BUDE_xxxxEFGH.hold(0);
    CPU_PIN_BOLO_ABCDEFxx.hold(1);
    CPU_PIN_BUKE_AxxxxxGH.hold(0);
    CPU_PIN_BOWA_xBCDEFGH.hold(1);
    CPU_PIN_BEDO_Axxxxxxx.hold(0);
  }

  CPU_PIN_BOGA_xBCDEFGH.hold((!PHASE_A | !upoj_run) & SYS_PIN_CLK_A);

  EXT_PIN_CLK.hold(PHASE(0b11110000));
  if (!_CPU_PIN_READYp) EXT_PIN_CLK.hold(0);

  CPU_PIN_EXT_CLKGOOD = SYS_PIN_CLK_A.as_wire();

  if (_SYS_PIN_RSTp || !SYS_PIN_CLK_A) _TUBO_CPU_READYn.hold(1);
  if (_CPU_PIN_READYp) _TUBO_CPU_READYn.hold(0);


  if (unor_dbg || umut_dbg) {
    _CPU_PIN_STARTp = 1;
    _ASOL_POR_DONEn.hold(_SYS_PIN_RSTp);
  }
  else {
    wire start = _TUBO_CPU_READYn.q() && top.tim_reg.TERO_DIV_03();
    _CPU_PIN_STARTp = start;

    if ( _SYS_PIN_RSTp)          _ASOL_POR_DONEn.hold(1);
    if (!_SYS_PIN_RSTp && start) _ASOL_POR_DONEn.hold(0);
  }


  _CPU_PIN_SYS_RSTp = _AFER_SYS_RSTp.q();
  _CPU_PIN_EXT_RST  = _SYS_PIN_RSTp.as_wire();


  if (_AFER_SYS_RSTp.q() || _ASOL_POR_DONEn.q()) {
    /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not(unor_dbg);
    /*p25.SOTO*/ SOTO_DBG = ff17_r2(_SYCY_DBG_CLOCKn, 0, SOTO_DBG.qn());
  }
  else {
    /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not(unor_dbg);
    /*p25.SOTO*/ SOTO_DBG = ff17_r2(_SYCY_DBG_CLOCKn, 1, SOTO_DBG.qn());
  }


  {
    if (upoj_run) {
      if (!SYS_PIN_CLK_A) {
        _AFER_SYS_RSTp = ff13_r2(0, 1, 1, _ASOL_POR_DONEn.q());
        //_AFER_SYS_RSTp.hold_clk(0);
      }
      else {
        wire clk1 = _AFER_SYS_RSTp.clk();
        wire clk2 = !PHASE_A;

        if (!clk1 && clk2) {
          _AFER_SYS_RSTp = ff13_r2(clk2, !clk2, 1, _ASOL_POR_DONEn.q());
        }
        else {
          _AFER_SYS_RSTp = ff13_r2(clk2, !clk2, 1, _ASOL_POR_DONEn.q());
        }

        //if (PHASE_B) _AFER_SYS_RSTp.hold_d(_ASOL_POR_DONEn.q());
        //_AFER_SYS_RSTp.hold_clk(!PHASE_A);
      }
    }
    else {
      if (!SYS_PIN_CLK_A) {
        _AFER_SYS_RSTp = ff13_r2(0, 1, 0, _ASOL_POR_DONEn.q());
        //_AFER_SYS_RSTp.hold_d(0);
        //_AFER_SYS_RSTp.hold_clk(0);
      }
      else {
        //wire clk = _AFER_SYS_RSTp.clk();
        _AFER_SYS_RSTp = ff13_r2(1, 0, 0, _ASOL_POR_DONEn.q());

        //_AFER_SYS_RSTp.hold_d(0);
        //_AFER_SYS_RSTp.hold_clk(1);
      }
    }


  }


  _CPU_PIN_UNOR_DBG = _SYS_PIN_T2n && !_SYS_PIN_T1n;
  _CPU_PIN_UMUT_DBG = _SYS_PIN_T1n && !_SYS_PIN_T2n;

  // can't do fast mode for this until vid clock is running
  {
    /*p29.XYVA*/ wire _XYVA_xBxDxFxH = not(ZEME_AxCxExGx());
    /*p29.XOTA*/ wire _XOTA_AxCxExGx = not(_XYVA_xBxDxFxH);
    /*p29.XYFY*/ wire _XYFY_xBxDxFxH = not(_XOTA_AxCxExGx);

    wire WUVU_xxCDxxGH_ = _WUVU_xxCDxxGH.q();
    wire VENA_xxxxEFGH_ = _VENA_xxxxEFGH.q();

    /*p29.WUVU*/ _WUVU_xxCDxxGH = ff17_r2( _XOTA_AxCxExGx, XAPO_VID_RSTn(), !WUVU_xxCDxxGH_);
    /*p21.VENA*/ _VENA_xxxxEFGH = ff17_r2(!WUVU_xxCDxxGH_, XAPO_VID_RSTn(), !VENA_xxxxEFGH_);
    /*p29.WOSU*/ _WOSU_xBCxxFGx = ff17_r2( _XYFY_xBxDxFxH, XAPO_VID_RSTn(), !WUVU_xxCDxxGH_);
  }
}

#endif


//-----------------------------------------------------------------------------

#ifndef FAST_IMPL

void ClockRegisters::tock(const SchematicTop& top) {
  const int phase = top.phase_count & 7;
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
    /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(SYS_PIN_CLK_A);
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(_CPU_PIN_READYp);

    /*p01.AROV*/ wire AROV_xxCDEFxx = not(_APUK_xxCDEFxx.qn());
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( _ALEF_xBCDExxx.q());
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

    CPU_PIN_BOWA_xBCDEFGH = BOWA_xBCDEFGH;
    CPU_PIN_BEDO_Axxxxxxx = BEDO_Axxxxxxx;
    CPU_PIN_BEKO_ABCDxxxx = BEKO_ABCDxxxx;
    CPU_PIN_BUDE_xxxxEFGH = BUDE_xxxxEFGH;
    CPU_PIN_BOLO_ABCDEFxx = BOLO_ABCDEFxx;
    CPU_PIN_BUKE_AxxxxxGH = BUKE_AxxxxxGH;
    CPU_PIN_BOMA_Axxxxxxx = BOMA_Axxxxxxx;
    CPU_PIN_BOGA_xBCDEFGH = BOGA_xBCDEFGH;
  }

  // can't do fast mode for this until vid clock is running
  {
    /*p29.XYVA*/ wire _XYVA_xBxDxFxH = not(ZEME_AxCxExGx());
    /*p29.XOTA*/ wire _XOTA_AxCxExGx = not(_XYVA_xBxDxFxH);
    /*p29.XYFY*/ wire _XYFY_xBxDxFxH = not(_XOTA_AxCxExGx);

    wire WUVU_xxCDxxGH_ = _WUVU_xxCDxxGH.q();
    wire VENA_xxxxEFGH_ = _VENA_xxxxEFGH.q();

    /*p29.WUVU*/ _WUVU_xxCDxxGH = ff17_r2( _XOTA_AxCxExGx, XAPO_VID_RSTn(), !WUVU_xxCDxxGH_);
    /*p21.VENA*/ _VENA_xxxxEFGH = ff17_r2(!WUVU_xxCDxxGH_, XAPO_VID_RSTn(), !VENA_xxxxEFGH_);
    /*p29.WOSU*/ _WOSU_xBCxxFGx = ff17_r2( _XYFY_xBxDxFxH, XAPO_VID_RSTn(), !WUVU_xxCDxxGH_);
  }

  {
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(AFUR_ABCDxxxx());
    /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn(), ATYP_xBCDExxx);
    /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
    /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
    /* PIN_75 */ EXT_PIN_CLK = BUDE_AxxxxFGH;
  }

  CPU_PIN_EXT_CLKGOOD = SYS_PIN_CLK_A.as_wire();
  /*p01.UPYF*/ wire _UPYF = or(_SYS_PIN_RSTp, UCOB_CLKBADp());

  /*p01.TUBO*/ _TUBO_CPU_READYn = nor_latch_r2(_UPYF, CPU_PIN_READYp());


#ifdef FAST_BOOT
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and (_TUBO_CPU_READYn.q(), top.tim_reg.TERO_DIV_03());
#else
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and (_TUBO_CPU_READYn.q(), top.tim_reg.UPOF_DIV_15());
#endif

  /*p01.TABA*/ wire _TABA_POR_TRIGn = or(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  _CPU_PIN_STARTp   = _TABA_POR_TRIGn;

  /*p01.ALYP*/ wire _ALYP_RSTn = not(_TABA_POR_TRIGn);
  /*p01.AFAR*/ wire _AFAR_RST  = nor(_ALYP_RSTn, _SYS_PIN_RSTp);

  /*p01.ASOL*/ _ASOL_POR_DONEn = nor_latch_r2(_SYS_PIN_RSTp, _AFAR_RST); // Schematic wrong, this is a latch.


  /*p01.AFER*/ _AFER_SYS_RSTp = ff13_r2(BOGA_xBCDEFGH(), BOMA_Axxxxxxx(), UPOJ_MODE_PRODn(), _ASOL_POR_DONEn.q());

  _CPU_PIN_SYS_RSTp = AFER_SYS_RSTp();
  _CPU_PIN_EXT_RST  = _SYS_PIN_RSTp.as_wire();

  /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not(UNOR_MODE_DBG2p());
  /*p25.SOTO*/ SOTO_DBG = ff17_r2(_SYCY_DBG_CLOCKn, CUNU_SYS_RSTn(), SOTO_DBG.qn());

  _CPU_PIN_UNOR_DBG = UNOR_MODE_DBG2p();
  _CPU_PIN_UMUT_DBG = UMUT_MODE_DBG1p();
}


#endif

//-----------------------------------------------------------------------------

uint64_t ClockRegisters::commit() {
  uint64_t ret = commit_and_hash((uint8_t*)this, sizeof(*this));
  return {ret};
}

//-----------------------------------------------------------------------------

void ClockRegisters::dump(Dumper& d) {
  d("----------ClockRegisters----------\n");
  d("AFUR_ABCDxxxx %d\n", _AFUR_ABCDxxxx);
  d("ALEF_xBCDExxx %d\n", _ALEF_xBCDExxx);
  d("APUK_xxCDEFxx %d\n", _APUK_xxCDEFxx);
  d("ADYK_xxxDEFGx %d\n", _ADYK_xxxDEFGx);
}

//-----------------------------------------------------------------------------