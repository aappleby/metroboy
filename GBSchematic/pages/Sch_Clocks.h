#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// c.cpu.CLK_xBCDEFGH = BOWA_xBCDEFGH;

struct ClockSignals1 {
  static ClockSignals1 tick_slow(const SystemSignals& sys_sig, const ClockRegisters& clk);
  static ClockSignals1 tick_fast(const SystemSignals& sys_sig);

  void reset() {
    *this = {
      // ungated
      /*p01.ARYS*/ .ARYS_xBxDxFxH = 0,
      /*p01.ANOS*/ .ANOS_xBxDxFxH = 0,
      /*p01.AVET*/ .ROOT_AxCxExGx = 1,
      /*p01.ATAL*/ .ATAL_xBxDxFxH = 0,
      /*p01.AZOF*/ .AZOF_AxCxExGx = 1,
      /*p01.ZAXY*/ .ZAXY_xBxDxFxH = 0,
      /*p01.ZEME*/ .ZEME_AxCxExGx = 1,
      /*p01.ALET*/ .ALET_xBxDxFxH = 0,
      /*p27.MYVO*/ .MYVO_AxCxExGx = 1,
      /*p29.XYVA*/ .XYVA_xBxDxFxH = 0,
      /*p29.XOTA*/ .XOTA_AxCxExGx = 1,
      /*p27.MOXE*/ .MOXE_AxCxExGx = 1,
      /*p27.MEHE*/ .MEHE_AxCxExGx = 1,
      /*p01.LAPE*/ .LAPE_AxCxExGx = 1,
      /*p27.TAVA*/ .TAVA_xBxDxFxH = 0,
      /*p29.XYFY*/ .XYFY_xBxDxFxH = 0,

      // gated on MODE_PROD
      /*p01.AFUR*/ .PHAZ_ABCDxxxx = 0,
      /*p01.ALEF*/ .PHAZ_xBCDExxx = 0,
      /*p01.APUK*/ .PHAZ_xxCDEFxx = 0,
      /*p01.ADYK*/ .PHAZ_xxxDEFGx = 0,
      /*p01.AFEP*/ .AFEP_AxxxxFGH = 1,
      /*p01.ATYP*/ .ATYP_ABCDxxxx = 0,
      /*p01.ADAR*/ .ADAR_ABCxxxxH = 1,
      /*p01.AROV*/ .AROV_xxCDEFxx = 0,
      /*p01.AFAS*/ .AFAS_xxxxxFGH = 0,

      // gated on CPUCLK_REQ
      /*p01.NULE*/ .NULE_xxxxEFGH = 1,
      /*p01.BYRY*/ .BYRY_ABCDxxxx = 0,
      /*p01.BUDE*/ .BUDE_xxxxEFGH = 1,
      /*p01.DOVA*/ .DOVA_ABCDxxxx = 0,
      /*p01.UVYT*/ .UVYT_ABCDxxxx = 0,
      /*p01.BEKO*/ .BEKO_ABCDxxxx = 0,
      /*p04.MOPA*/ .MOPA_xxxxEFGH = 1,

      /*p01.BAPY*/ .BAPY_xxxxxxGH = 1,
      /*p01.BERU*/ .BERU_ABCDEFxx = 0,
      /*p01.BUFA*/ .BUFA_xxxxxxGH = 1,
      /*p01.BOLO*/ .BOLO_ABCDEFxx = 0,

      /*p01.BEJA*/ .BEJA_xxxxEFGH = 1,
      /*p01.BANE*/ .BANE_ABCDxxxx = 0,
      /*p01.BELO*/ .BELO_xxxxEFGH = 1,
      /*p01.BAZE*/ .BAZE_ABCDxxxx = 0,
      /*p01.BUTO*/ .BUTO_xBCDEFGH = 1,
      /*p01.BELE*/ .BELE_Axxxxxxx = 0,

      // gated on CLK_GOOD
      /*p01.BYJU*/ .BYJU_xBCDEFGH = 1,
      /*p01.BALY*/ .BALY_Axxxxxxx = 0,
      /*p01.BOGA*/ .BOGA_xBCDEFGH = 1,
      /*p01.BUVU*/ .BUVU_Axxxxxxx = 0,
      /*p01.BYXO*/ .BYXO_xBCDEFGH = 1,
      /*p01.BEDO*/ .BEDO_Axxxxxxx = 0,
      /*p01.BOWA*/ .BOWA_xBCDEFGH = 1,
    };
  }

  // ungated
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
  /*p01.AFUR*/ bool PHAZ_ABCDxxxx;
  /*p01.ALEF*/ bool PHAZ_xBCDExxx;
  /*p01.APUK*/ bool PHAZ_xxCDEFxx;
  /*p01.ADYK*/ bool PHAZ_xxxDEFGx;
  /*p01.AFEP*/ bool AFEP_AxxxxFGH;
  /*p01.ATYP*/ bool ATYP_ABCDxxxx;
  /*p01.ADAR*/ bool ADAR_ABCxxxxH;
  /*p01.AROV*/ bool AROV_xxCDEFxx;
  /*p01.AFAS*/ bool AFAS_xxxxxFGH;

  // gated on CPUCLK_REQ
  /*p01.NULE*/ bool NULE_xxxxEFGH;
  /*p01.BYRY*/ bool BYRY_ABCDxxxx;
  /*p01.BUDE*/ bool BUDE_xxxxEFGH;
  /*p01.DOVA*/ bool DOVA_ABCDxxxx;
  /*p01.UVYT*/ bool UVYT_ABCDxxxx;
  /*p01.BEKO*/ bool BEKO_ABCDxxxx;
  /*p04.MOPA*/ bool MOPA_xxxxEFGH;

  /*p01.BAPY*/ bool BAPY_xxxxxxGH;
  /*p01.BERU*/ bool BERU_ABCDEFxx;
  /*p01.BUFA*/ bool BUFA_xxxxxxGH;
  /*p01.BOLO*/ bool BOLO_ABCDEFxx;
  /*p01.BEJA*/ bool BEJA_xxxxEFGH;
  /*p01.BANE*/ bool BANE_ABCDxxxx;
  /*p01.BELO*/ bool BELO_xxxxEFGH;
  /*p01.BAZE*/ bool BAZE_ABCDxxxx;
  /*p01.BUTO*/ bool BUTO_xBCDEFGH;
  /*p01.BELE*/ bool BELE_Axxxxxxx;

  // gated on CLK_GOOD
  /*p01.BYJU*/ bool BYJU_xBCDEFGH;
  /*p01.BALY*/ bool BALY_Axxxxxxx;
  /*p01.BOGA*/ bool BOGA_xBCDEFGH;
  /*p01.BUVU*/ bool BUVU_Axxxxxxx;
  /*p01.BYXO*/ bool BYXO_xBCDEFGH;
  /*p01.BEDO*/ bool BEDO_Axxxxxxx;
  /*p01.BOWA*/ bool BOWA_xBCDEFGH;
};

//-----------------------------------------------------------------------------
// Video clocks

struct ClockSignals2 {
  static ClockSignals2 tick_slow(const ResetSignals2& rst_sig2, const ClockRegisters& clk);
  static ClockSignals2 tick_fast(const SystemSignals& sys_sig, const ResetSignals2& rst_sig2, const ClockRegisters& clk_reg);

  void reset() {
    /*p29.WUVU*/ WUVU_AxxDExxH = 1;
    /*p21.VENA*/ VENA_xBCDExxx = 0;
    /*p29.WOSU*/ WOSU_xxCDxxGH = 1;
    /*p29.XUPY*/ XUPY_xBCxxFGx = 0;
    /*p28.AWOH*/ AWOH_AxxDExxH = 1;
    /*p21.TALU*/ TALU_xBCDExxx = 0;
    /*p21.SONO*/ SONO_AxxxxFGH = 1;
    /*p29.XOCE*/ XOCE_ABxxEFxx = 0;
  }

  /*p29.WUVU*/ bool WUVU_AxxDExxH;
  /*p21.VENA*/ bool VENA_xBCDExxx;
  /*p29.WOSU*/ bool WOSU_xxCDxxGH;
  /*p29.XUPY*/ bool XUPY_xBCxxFGx;
  /*p28.AWOH*/ bool AWOH_AxxDExxH;
  /*p21.TALU*/ bool TALU_xBCDExxx;
  /*p21.SONO*/ bool SONO_AxxxxFGH;
  /*p29.XOCE*/ bool XOCE_ABxxEFxx;
};

//-----------------------------------------------------------------------------

struct ClockRegisters {

  static void tock_slow1(const SystemSignals& sys_sig,
                         const ClockSignals1& clk_sig1,
                         ClockRegisters& next);

  static void tock_slow2(const SystemSignals& sys_sig,
                         const ClockSignals1& clk_sig1,
                         const ClockSignals2& clk_sig2,
                         const ResetSignals2& rst_sig2,
                         ClockRegisters& next);

  static void tock_fast1(const SystemSignals& sys_sig,
                         ClockRegisters& next);

  static void tock_fast2(const SystemSignals& sys_sig,
                         const ResetSignals2& rst_sig2,
                         ClockRegisters& next);

  // ResetRegisters to immediately before the first phase on the first line
  void reset() {
    PHAZ_ABCDxxxx.val = 0; PHAZ_ABCDxxxx.clk = 0;
    PHAZ_xBCDExxx.val = 0; PHAZ_xBCDExxx.clk = 0;
    PHAZ_xxCDEFxx.val = 0; PHAZ_xxCDEFxx.clk = 0;
    PHAZ_xxxDEFGx.val = 0; PHAZ_xxxDEFGx.clk = 0;

    WUVU_AxxDExxH.val = 1; WUVU_AxxDExxH.clk = 1;
    VENA_xBCDExxx.val = 0; VENA_xBCDExxx.clk = 0;
    WOSU_xxCDxxGH.val = 1; WOSU_xxCDxxGH.clk = 0;
  }

private:

  friend struct ClockSignals1;
  friend struct ClockSignals2;

  /*p01.AFUR*/ Reg PHAZ_ABCDxxxx;
  /*p01.ALEF*/ Reg PHAZ_xBCDExxx;
  /*p01.APUK*/ Reg PHAZ_xxCDEFxx;
  /*p01.ADYK*/ Reg PHAZ_xxxDEFGx;

  /*p29.WUVU*/ Reg WUVU_AxxDExxH;
  /*p21.VENA*/ Reg VENA_xBCDExxx;
  /*p29.WOSU*/ Reg WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

};