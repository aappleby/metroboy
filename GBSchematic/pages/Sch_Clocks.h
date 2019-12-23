#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// c.cpu.CLK_xBCDEFGH = BOWA_AxCDEFGH;

struct ClockSignals {
  void reset() {
    *this = {
      /*p01.ABOL*/ .CPUCLK_REQn   = 0,
      /*p01.BUTY*/ .CPUCLK_REQ    = 1,
      /*p01.UCOB*/ .CLK_BAD1      = 0,
      /*p01.ATEZ*/ .CLK_BAD2      = 0,
      /*p01.ARYS*/ .ARYS_xBxDxFxH = 1,
      /*p01.ANOS*/ .ANOS_xBxDxFxH = 1,
      /*p01.AVET*/ .ROOT_AxCxExGx = 0,
      /*p01.ATAL*/ .ATAL_AxCxExGx = 1,
      /*p01.AZOF*/ .AZOF_xBxDxFxH = 0,
      /*p01.ZAXY*/ .ZAXY_AxCxExGx = 1,
      /*p01.ZEME*/ .ZEME_AxCxExGx = 0,
      /*p01.ALET*/ .ALET_xBxDxFxH = 1,
      /*p27.MYVO*/ .MYVO_AxCxExGx = 0,
      /*p29.XYVA*/ .XYVA_AxCxExGx = 1,
      /*p29.XOTA*/ .XOTA_AxCxExGx = 0,
      /*p27.MOXE*/ .MOXE_AxCxExGx = 0,
      /*p27.MEHE*/ .MEHE_AxCxExGx = 0,
      /*p01.LAPE*/ .LAPE_AxCxExGx = 0,
      /*p27.TAVA*/ .TAVA_xBxDxFxH = 1,
      /*p29.XYFY*/ .XYFY_AxCxExGx = 1,
    };
  }

  /*p01.ABOL*/ bool CPUCLK_REQn;
  /*p01.BUTY*/ bool CPUCLK_REQ;
  /*p01.UCOB*/ bool CLK_BAD1;
  /*p01.ATEZ*/ bool CLK_BAD2;

  /*p01.ARYS*/ bool ARYS_xBxDxFxH;
  /*p01.ANOS*/ bool ANOS_xBxDxFxH;
  /*p01.AVET*/ bool ROOT_AxCxExGx;
  /*p01.ATAL*/ bool ATAL_AxCxExGx;
  /*p01.AZOF*/ bool AZOF_xBxDxFxH;
  /*p01.ZAXY*/ bool ZAXY_AxCxExGx;
  /*p01.ZEME*/ bool ZEME_AxCxExGx;
  /*p01.ALET*/ bool ALET_xBxDxFxH;
  /*p27.MYVO*/ bool MYVO_AxCxExGx;
  /*p29.XYVA*/ bool XYVA_AxCxExGx;
  /*p29.XOTA*/ bool XOTA_AxCxExGx;
  /*p27.MOXE*/ bool MOXE_AxCxExGx;
  /*p27.MEHE*/ bool MEHE_AxCxExGx;
  /*p01.LAPE*/ bool LAPE_AxCxExGx;
  /*p27.TAVA*/ bool TAVA_xBxDxFxH;
  /*p29.XYFY*/ bool XYFY_AxCxExGx;
};

//-----------------------------------------------------------------------------

struct Clocks {

  static ClockSignals tick_slow(const Clocks& clk,
                                wire CLK,
                                wire CLK_GOOD,
                                wire CPUCLK_REQ_,
                                /*p07.UPOJ*/ wire MODE_PROD,
                                /*p01.XAPO*/ wire VID_RESETn);

  static void tock_slow(const Clocks& clk,
                        wire CLK,
                        wire CLK_GOOD,
                        wire CPUCLK_REQ_,
                        /*p07.UPOJ*/ wire MODE_PROD,
                        /*p01.XAPO*/ wire VID_RESETn,
                        Clocks& next);

  static void tock_fast(const Clocks& clk,
                        wire CLK,
                        wire CLK_GOOD,
                        wire CPUCLK_REQ_,
                        /*p07.UPOJ*/ wire MODE_PROD,
                        /*p01.XAPO*/ wire VID_RESETn,
                        Clocks& next);


  // Resets to immediately before the first phase on the first line
  void reset() {
    sig.reset();

    ARYS_xBxDxFxH	= true;
    ANOS_xBxDxFxH	= true;
    ALET_xBxDxFxH	= true;
    TAVA_xBxDxFxH	= true;

    ROOT_AxCxExGx	= false;
    MYVO_AxCxExGx	= false;
    ZEME_AxCxExGx	= false;
    XOTA_AxCxExGx	= false;
    MOXE_AxCxExGx	= false;
    MEHE_AxCxExGx	= false;
    LAPE_AxCxExGx	= false;

    AFAS_xxxxxFGH	= true;
    UVYT_xBCDExxx	= false;
    DOVA_xBCDExxx	= false;
    MOPA_AxxxxFGH	= true;
    BOGA_AxCDEFGH	= true;
    BEDO_xBxxxxxx	= false;	
    BOWA_AxCDEFGH	= true;
    XUPY_ABxxEFxx	= false;
    AWOH_xxCDxxGH	= true;
    TALU_xxxxEFGH	= true;
    SONO_ABCDxxxx	= false;
    XOCE_AxxDExxH	= true;

    PHAZ_xBCDExxx.val=false;
    PHAZ_xBCDExxx.clk=true;
    PHAZ_xxCDEFxx.val=false;
    PHAZ_xxCDEFxx.clk=true;
    PHAZ_xxxDEFGx.val=false;
    PHAZ_xxxDEFGx.clk=true;
    PHAZ_xxxxEFGH.val=true;
    PHAZ_xxxxEFGH.clk=true;

    WUVU_xBCxxFGx.val=true;
    WUVU_xBCxxFGx.clk=false;
    VENA_xxxxEFGH.val=true;
    VENA_xxxxEFGH.clk=false;
    WOSU_xBCxxFGx.val=false;
    WOSU_xBCxxFGx.clk=true;
  }

  ClockSignals sig;

  /*p01.ARYS*/ bool ARYS_xBxDxFxH;
  /*p01.ANOS*/ bool ANOS_xBxDxFxH;
  /*p01.ALET*/ bool ALET_xBxDxFxH;
  /*p27.TAVA*/ bool TAVA_xBxDxFxH;

  /*p01.AVET*/ bool ROOT_AxCxExGx;
  /*p27.MYVO*/ bool MYVO_AxCxExGx;
  /*p01.ZEME*/ bool ZEME_AxCxExGx;
  /*p29.XOTA*/ bool XOTA_AxCxExGx;
  /*p27.MOXE*/ bool MOXE_AxCxExGx;
  /*p27.MEHE*/ bool MEHE_AxCxExGx;
  /*p01.LAPE*/ bool LAPE_AxCxExGx;

  /*p01.AFAS*/ bool AFAS_xxxxxFGH;
  /*p01.UVYT*/ bool UVYT_xBCDExxx;
  /*p01.DOVA*/ bool DOVA_xBCDExxx;
  /*p04.MOPA*/ bool MOPA_AxxxxFGH;
  /*p01.BOGA*/ bool BOGA_AxCDEFGH;
  /*p01.BEDO*/ bool BEDO_xBxxxxxx;
  /*p01.BOWA*/ bool BOWA_AxCDEFGH;
  /*p29.XUPY*/ bool XUPY_ABxxEFxx;
  /*p28.AWOH*/ bool AWOH_xxCDxxGH;
  /*p21.TALU*/ bool TALU_xxxxEFGH;
  /*p21.SONO*/ bool SONO_ABCDxxxx;
  /*p29.XOCE*/ bool XOCE_AxxDExxH;

  /*p01.AFUR*/ Reg  PHAZ_xBCDExxx;
  /*p01.ALEF*/ Reg  PHAZ_xxCDEFxx;
  /*p01.APUK*/ Reg  PHAZ_xxxDEFGx;
  /*p01.ADYK*/ Reg  PHAZ_xxxxEFGH;

  /*p29.WUVU*/ Reg  WUVU_xBCxxFGx;
  /*p21.VENA*/ Reg  VENA_xxxxEFGH;
  /*p29.WOSU*/ Reg  WOSU_xBCxxFGx;
};

//-----------------------------------------------------------------------------

};