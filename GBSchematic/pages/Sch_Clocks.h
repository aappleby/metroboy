#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// c.cpu.CLK_xBCDEFGH = BOWA_AxCDEFGH;

struct ClockSignals1 {
  static ClockSignals1 tick_slow(const SystemSignals& sys_sig,
                                 const Clocks& clk,
                                 wire CLK);

  static ClockSignals1 tick_fast(int phase,
                                 wire CLK_GOOD,
                                 wire CPUCLK_REQ_,
                                 wire MODE_PROD);

  void reset() {
    *this = {
      /*p01.ABOL*/ .CPUCLK_REQn   = 0,
      /*p01.BUTY*/ .CPUCLK_REQ    = 1,
      /*p01.UCOB*/ .CLK_BAD1      = 0,
      /*p01.ATEZ*/ .CLK_BAD2      = 0,
      /*p01.ARYS*/ .ARYS_xBxDxFxH = 1,
      /*p01.ANOS*/ .ANOS_xBxDxFxH = 1,
      /*p01.AVET*/ .ROOT_AxCxExGx = 0,
      /*p01.ATAL*/ .ATAL_xBxDxFxH = 1,
      /*p01.AZOF*/ .AZOF_AxCxExGx = 0,
      /*p01.ZAXY*/ .ZAXY_xBxDxFxH = 1,
      /*p01.ZEME*/ .ZEME_AxCxExGx = 0,
      /*p01.ALET*/ .ALET_xBxDxFxH = 1,
      /*p27.MYVO*/ .MYVO_AxCxExGx = 0,
      /*p29.XYVA*/ .XYVA_xBxDxFxH = 1,
      /*p29.XOTA*/ .XOTA_AxCxExGx = 0,
      /*p27.MOXE*/ .MOXE_AxCxExGx = 0,
      /*p27.MEHE*/ .MEHE_AxCxExGx = 0,
      /*p01.LAPE*/ .LAPE_AxCxExGx = 0,
      /*p27.TAVA*/ .TAVA_xBxDxFxH = 1,
      /*p29.XYFY*/ .XYFY_xBxDxFxH = 1,

      // gated on MODE_PROD
      /*p01.AFUR*/ .PHAZ_xBCDExxx = 0,
      /*p01.ALEF*/ .PHAZ_xxCDEFxx = 1,
      /*p01.APUK*/ .PHAZ_xxxDEFGx = 1,
      /*p01.ADYK*/ .PHAZ_xxxxEFGH = 1,
      /*p01.AFEP*/ .AFEP_ABxxxxGH = 0,
      /*p01.ATYP*/ .ATYP_xBCDExxx = 0,
      /*p01.ADAR*/ .ADAR_ABCxxxxH = 0,
      /*p01.AROV*/ .AROV_xxxDEFGx = 1,
      /*p01.AFAS*/ .AFAS_xxxxxFGH = 1,

      // gated on CPUCLK_REQ
      /*p01.NULE*/ .NULE_xxxxEFGH = 1,
      /*p01.BYRY*/ .BYRY_ABCDxxxx = 0,
      /*p01.BUDE*/ .BUDE_xxxxEFGH = 1,
      /*p01.DOVA*/ .DOVA_xBCDExxx = 0,
      /*p01.UVYT*/ .UVYT_xBCDExxx = 0,
      /*p01.BEKO*/ .BEKO_ABCDxxxx = 0,
      /*p04.MOPA*/ .MOPA_AxxxxFGH = 1,

      /*p01.BAPY*/ .BAPY_AxxxxxxH = 0,
      /*p01.BERU*/ .BERU_xBCDEFGx = 1,
      /*p01.BUFA*/ .BUFA_AxxxxxxH = 0,
      /*p01.BOLO*/ .BOLO_xBCDEFGx = 1,
      /*p01.BEJA*/ .BEJA_xxxxEFGH = 1,
      /*p01.BANE*/ .BANE_ABCDxxxx = 0,
      /*p01.BELO*/ .BELO_xxxxEFGH = 1,
      /*p01.BAZE*/ .BAZE_ABCDxxxx = 0,
      /*p01.BUTO*/ .BUTO_xBCDEFGH = 1,
      /*p01.BELE*/ .BELE_Axxxxxxx = 0,

      // gated on CLK_GOOD
      /*p01.BYJU*/ .BYJU_xBCDEFGH = 1,
      /*p01.BALY*/ .BALY_Axxxxxxx = 0,
      /*p01.BOGA*/ .BOGA_AxCDEFGH = 1,
      /*p01.BUVU*/ .BUVU_Axxxxxxx = 0,
      /*p01.BYXO*/ .BYXO_xBCDEFGH = 1,
      /*p01.BEDO*/ .BEDO_xBxxxxxx = 0,
      /*p01.BOWA*/ .BOWA_AxCDEFGH = 1,
    };
  }

  /*p01.ABOL*/ bool CPUCLK_REQn;
  /*p01.BUTY*/ bool CPUCLK_REQ;
  /*p01.UCOB*/ bool CLK_BAD1;
  /*p01.ATEZ*/ bool CLK_BAD2;

  /*p01.ARYS*/ bool ARYS_xBxDxFxH;
  /*p01.ANOS*/ bool ANOS_xBxDxFxH;
  /*p01.AVET*/ bool ROOT_AxCxExGx;
  /*p01.ATAL*/ bool ATAL_xBxDxFxH;
  /*p01.AZOF*/ bool AZOF_AxCxExGx;
  /*p01.ZAXY*/ bool ZAXY_xBxDxFxH;
  /*p01.ZEME*/ bool ZEME_AxCxExGx;
  /*p01.ALET*/ bool ALET_xBxDxFxH;
  /*p27.MYVO*/ bool MYVO_AxCxExGx;
  /*p29.XYVA*/ bool XYVA_xBxDxFxH;
  /*p29.XOTA*/ bool XOTA_AxCxExGx;
  /*p27.MOXE*/ bool MOXE_AxCxExGx;
  /*p27.MEHE*/ bool MEHE_AxCxExGx;
  /*p01.LAPE*/ bool LAPE_AxCxExGx;
  /*p27.TAVA*/ bool TAVA_xBxDxFxH;
  /*p29.XYFY*/ bool XYFY_xBxDxFxH;

  // gated on MODE_PROD
  /*p01.AFUR*/ bool PHAZ_xBCDExxx;
  /*p01.ALEF*/ bool PHAZ_xxCDEFxx;
  /*p01.APUK*/ bool PHAZ_xxxDEFGx;
  /*p01.ADYK*/ bool PHAZ_xxxxEFGH;
  /*p01.AFEP*/ bool AFEP_ABxxxxGH;
  /*p01.ATYP*/ bool ATYP_xBCDExxx;
  /*p01.ADAR*/ bool ADAR_ABCxxxxH;
  /*p01.AROV*/ bool AROV_xxxDEFGx;
  /*p01.AFAS*/ bool AFAS_xxxxxFGH;

  // gated on CPUCLK_REQ
  /*p01.NULE*/ bool NULE_xxxxEFGH;
  /*p01.BYRY*/ bool BYRY_ABCDxxxx;
  /*p01.BUDE*/ bool BUDE_xxxxEFGH;
  /*p01.DOVA*/ bool DOVA_xBCDExxx;
  /*p01.UVYT*/ bool UVYT_xBCDExxx;
  /*p01.BEKO*/ bool BEKO_ABCDxxxx;
  /*p04.MOPA*/ bool MOPA_AxxxxFGH;

  /*p01.BAPY*/ bool BAPY_AxxxxxxH;
  /*p01.BERU*/ bool BERU_xBCDEFGx;
  /*p01.BUFA*/ bool BUFA_AxxxxxxH;
  /*p01.BOLO*/ bool BOLO_xBCDEFGx;
  /*p01.BEJA*/ bool BEJA_xxxxEFGH;
  /*p01.BANE*/ bool BANE_ABCDxxxx;
  /*p01.BELO*/ bool BELO_xxxxEFGH;
  /*p01.BAZE*/ bool BAZE_ABCDxxxx;
  /*p01.BUTO*/ bool BUTO_xBCDEFGH;
  /*p01.BELE*/ bool BELE_Axxxxxxx;

  // gated on CLK_GOOD
  /*p01.BYJU*/ bool BYJU_xBCDEFGH;
  /*p01.BALY*/ bool BALY_Axxxxxxx;
  /*p01.BOGA*/ bool BOGA_AxCDEFGH;
  /*p01.BUVU*/ bool BUVU_Axxxxxxx;
  /*p01.BYXO*/ bool BYXO_xBCDEFGH;
  /*p01.BEDO*/ bool BEDO_xBxxxxxx;
  /*p01.BOWA*/ bool BOWA_AxCDEFGH;
};

//-----------------------------------------------------------------------------

struct ClockSignals2 {
  static ClockSignals2 tick_slow(const Clocks& clk);

  static ClockSignals2 tick_fast(int phase, wire VID_RESETn);

  void reset() {
    *this = {
      // gated on VID_RESETn
      /*p29.WUVU*/ .WUVU_xxCDxxGH = 1,
      /*p21.VENA*/ .VENA_xxxxEFGH = 1,
      /*p29.WOSU*/ .WOSU_xBCxxFGx = 0,
      /*p29.XUPY*/ .XUPY_ABxxEFxx = 0,
      /*p28.AWOH*/ .AWOH_xxCDxxGH = 1,
      /*p21.TALU*/ .TALU_xxxxEFGH = 1,
      /*p21.SONO*/ .SONO_ABCDxxxx = 0,
      /*p29.XOCE*/ .XOCE_AxxDExxH = 1,
    };
  }

  // gated on VID_RESETn
  /*p29.WUVU*/ bool WUVU_xxCDxxGH;
  /*p21.VENA*/ bool VENA_xxxxEFGH;
  /*p29.WOSU*/ bool WOSU_xBCxxFGx;
  /*p29.XUPY*/ bool XUPY_ABxxEFxx;
  /*p28.AWOH*/ bool AWOH_xxCDxxGH;
  /*p21.TALU*/ bool TALU_xxxxEFGH;
  /*p21.SONO*/ bool SONO_ABCDxxxx;
  /*p29.XOCE*/ bool XOCE_AxxDExxH;
};

//-----------------------------------------------------------------------------

struct Clocks {

  static void tock_slow1(const ClockSignals1& sig,
                         /*p07.UPOJ*/ wire MODE_PROD,
                         Clocks& next);

  static void tock_slow2(const ClockSignals1& sig1,
                         const ClockSignals2& sig2,
                         /*p01.XAPO*/ wire VID_RESETn,
                         Clocks& next);

  static void tock_fast1(int phase,
                         /*p07.UPOJ*/ wire MODE_PROD,
                         Clocks& next);

  static void tock_fast2(int phase,
                         /*p01.XAPO*/ wire VID_RESETn,
                         Clocks& next);

  // ResetRegisters to immediately before the first phase on the first line
  void reset() {
    PHAZ_xBCDExxx.val = 0; PHAZ_xBCDExxx.clk = 1;
    PHAZ_xxCDEFxx.val = 1; PHAZ_xxCDEFxx.clk = 1;
    PHAZ_xxxDEFGx.val = 1; PHAZ_xxxDEFGx.clk = 1;
    PHAZ_xxxxEFGH.val = 1; PHAZ_xxxxEFGH.clk = 1;

    WUVU_xxCDxxGH.val = 1; WUVU_xxCDxxGH.clk = 0;
    VENA_xxxxEFGH.val = 1; VENA_xxxxEFGH.clk = 0;
    WOSU_xBCxxFGx.val = 0; WOSU_xBCxxFGx.clk = 1;
  }

  /*p01.AFUR*/ Reg PHAZ_xBCDExxx;
  /*p01.ALEF*/ Reg PHAZ_xxCDEFxx;
  /*p01.APUK*/ Reg PHAZ_xxxDEFGx;
  /*p01.ADYK*/ Reg PHAZ_xxxxEFGH;

  /*p29.WUVU*/ Reg WUVU_xxCDxxGH;
  /*p21.VENA*/ Reg VENA_xxxxEFGH;
  /*p29.WOSU*/ Reg WOSU_xBCxxFGx;
};

//-----------------------------------------------------------------------------

};