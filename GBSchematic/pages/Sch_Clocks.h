#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// c.cpu.CLK_xBCDEFGH = BOWA_xBCDEFGH;

struct ClocksIn {
  bool CPUCLK_REQ;
  bool VID_RESETn;
};

struct Clocks {

  void tick(const Pins& pins,
            const Debug& debug,
            const ClocksIn& in,
            const Clocks& b);

  /*p01.ABOL*/ bool CPUCLK_REQn;
  /*p01.BUTY*/ bool CPUCLK_REQ;
  /*p01.UCOB*/ bool CLK_BAD1;
  /*p01.ATEZ*/ bool CLK_BAD2;

  /*p01.ANOS*/ bool ROOTCLK_AxCxExGx;
  /*p01.AVET*/ bool ROOTCLK_xBxDxFxH;

  // Phase generator
  /*p01.AFUR*/ RegDuo PHASE_ABCDxxxx;
  /*p01.ALEF*/ RegDuo PHASE_xBCDExxx;
  /*p01.APUK*/ RegDuo PHASE_xxCDEFxx;
  /*p01.ADYK*/ RegDuo PHASE_xxxDEFGx;

  /*p01.AFAS*/ bool AFAS_xxxxEFGx;
  /*p01.UVYT*/ bool UVYT_ABCDxxxx;
  /*p01.DOVA*/ bool DOVA_ABCDxxxx;
  /*p04.MOPA*/ bool MOPA_xxxxEFGH;
  /*p01.BOGA*/ bool BOGA_xBCDEFGH;
  /*p01.BEDO*/ bool BEDO_Axxxxxxx;
  /*p01.BOWA*/ bool BOWA_xBCDEFGH;

  /*p01.ALET*/ bool ALET_AxCxExGx;
  /*p27.MYVO*/ bool MYVO_xBxDxFxH;

  /*p01.ZEME*/ bool ZEME_xBxDxFxH;

  /*p29.XOTA*/ bool XOTA_xBxDxFxH;
  /*p29.WUVU*/ Reg  WUVU_AxxDExxH;
  /*p29.XUPY*/ bool XUPY_xBCxxFGx;
  /*p28.AWOH*/ bool AWOH_AxxDExxH;
  /*p21.VENA*/ Reg  VENA_AxxxxFGH;
  /*p21.TALU*/ bool TALU_AxxxxFGH;
  /*p21.SONO*/ bool SONO_xBCDExxx;

  /*p27.MOXE*/ bool MOXE_xBxDxFxH;
  /*p27.MEHE*/ bool MEHE_xBxDxFxH;

  /*p01.LAPE*/ bool LAPE_xBxDxFxH;
  /*p27.TAVA*/ bool TAVA_AxCxExGx;

  /*p29.WOSU*/ Reg  WOSU_QQQQQQQQ;
  /*p29.XOCE*/ bool XOCE_QQQQQQQQ;
};
//-----------------------------------------------------------------------------

};