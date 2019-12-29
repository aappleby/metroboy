#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// c.cpu.CLK_xBCDEFGH = BOWA_ABCxEFGH;

struct ClockSignals1 {
  static ClockSignals1 tick_slow(const SystemSignals& sys_sig, const ClockRegisters& clk);
  static ClockSignals1 tick_fast(const SystemSignals& sys_sig);

  void reset() {
    *this = {
      // ungated
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
      /*p01.AFUR*/ .PHAZ_xxxDEFGx = 0,
      /*p01.ALEF*/ .PHAZ_xxxxEFGH = 1,
      /*p01.APUK*/ .PHAZ_AxxxxFGH = 1,
      /*p01.ADYK*/ .PHAZ_ABxxxxGH = 1,
      /*p01.AFEP*/ .AFEP_ABCDxxxx = 0,
      /*p01.ATYP*/ .ATYP_xxxDEFGx = 0,
      /*p01.ADAR*/ .ADAR_xxCDEFxx = 0,
      /*p01.AROV*/ .AROV_AxxxxFGH = 1,
      /*p01.AFAS*/ .AFAS_ABxxxxxH = 1,

      // gated on CPUCLK_REQ
      /*p01.NULE*/ .NULE_ABCxxxxH = 1,
      /*p01.BYRY*/ .BYRY_xxxDEFGx = 0,
      /*p01.BUDE*/ .BUDE_ABCxxxxH = 1,
      /*p01.DOVA*/ .DOVA_xxxDEFGx = 0,
      /*p01.UVYT*/ .UVYT_xxxDEFGx = 0,
      /*p01.BEKO*/ .BEKO_xxxDEFGx = 0,
      /*p04.MOPA*/ .MOPA_ABCxxxxH = 1,

      /*p01.BAPY*/ .BAPY_xBCxxxxx = 0,
      /*p01.BERU*/ .BERU_AxxDEFGH = 1,
      /*p01.BUFA*/ .BUFA_xBCxxxxx = 0,
      /*p01.BOLO*/ .BOLO_AxxDEFGH = 1,

      /*p01.BEJA*/ .BEJA_ABCxxxxH = 1,
      /*p01.BANE*/ .BANE_xxxDEFGx = 0,
      /*p01.BELO*/ .BELO_ABCxxxxH = 1,
      /*p01.BAZE*/ .BAZE_xxxDEFGx = 0,
      /*p01.BUTO*/ .BUTO_ABCxEFGH = 1,
      /*p01.BELE*/ .BELE_xxxDxxxx = 0,

      // gated on CLK_GOOD
      /*p01.BYJU*/ .BYJU_ABCxEFGH = 1,
      /*p01.BALY*/ .BALY_xxxDxxxx = 0,
      /*p01.BOGA*/ .BOGA_ABCxEFGH = 1,
      /*p01.BUVU*/ .BUVU_xxxDxxxx = 0,
      /*p01.BYXO*/ .BYXO_ABCxEFGH = 1,
      /*p01.BEDO*/ .BEDO_xxxDxxxx = 0,
      /*p01.BOWA*/ .BOWA_ABCxEFGH = 1,
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
  /*p01.AFUR*/ bool PHAZ_xxxDEFGx;
  /*p01.ALEF*/ bool PHAZ_xxxxEFGH;
  /*p01.APUK*/ bool PHAZ_AxxxxFGH;
  /*p01.ADYK*/ bool PHAZ_ABxxxxGH;
  /*p01.AFEP*/ bool AFEP_ABCDxxxx;
  /*p01.ATYP*/ bool ATYP_xxxDEFGx;
  /*p01.ADAR*/ bool ADAR_xxCDEFxx;
  /*p01.AROV*/ bool AROV_AxxxxFGH;
  /*p01.AFAS*/ bool AFAS_ABxxxxxH;

  // gated on CPUCLK_REQ
  /*p01.NULE*/ bool NULE_ABCxxxxH;
  /*p01.BYRY*/ bool BYRY_xxxDEFGx;
  /*p01.BUDE*/ bool BUDE_ABCxxxxH;
  /*p01.DOVA*/ bool DOVA_xxxDEFGx;
  /*p01.UVYT*/ bool UVYT_xxxDEFGx;
  /*p01.BEKO*/ bool BEKO_xxxDEFGx;
  /*p04.MOPA*/ bool MOPA_ABCxxxxH;

  /*p01.BAPY*/ bool BAPY_xBCxxxxx;
  /*p01.BERU*/ bool BERU_AxxDEFGH;
  /*p01.BUFA*/ bool BUFA_xBCxxxxx;
  /*p01.BOLO*/ bool BOLO_AxxDEFGH;
  /*p01.BEJA*/ bool BEJA_ABCxxxxH;
  /*p01.BANE*/ bool BANE_xxxDEFGx;
  /*p01.BELO*/ bool BELO_ABCxxxxH;
  /*p01.BAZE*/ bool BAZE_xxxDEFGx;
  /*p01.BUTO*/ bool BUTO_ABCxEFGH;
  /*p01.BELE*/ bool BELE_xxxDxxxx;

  // gated on CLK_GOOD
  /*p01.BYJU*/ bool BYJU_ABCxEFGH;
  /*p01.BALY*/ bool BALY_xxxDxxxx;
  /*p01.BOGA*/ bool BOGA_ABCxEFGH;
  /*p01.BUVU*/ bool BUVU_xxxDxxxx;
  /*p01.BYXO*/ bool BYXO_ABCxEFGH;
  /*p01.BEDO*/ bool BEDO_xxxDxxxx;
  /*p01.BOWA*/ bool BOWA_ABCxEFGH;
};

//-----------------------------------------------------------------------------
// Video clocks

struct ClockSignals2 {
  static ClockSignals2 tick_slow(const ResetSignals2& rst_sig2, const ClockRegisters& clk);
  static ClockSignals2 tick_fast(const SystemSignals& sys_sig, const ResetSignals2& rst_sig2, const ClockRegisters& clk_reg);

  void reset() {
    /*p29.WUVU*/ WUVU_xxCDxxGH = 1;
    /*p21.VENA*/ VENA_xxxxEFGH = 1;
    /*p29.WOSU*/ WOSU_xBCxxFGx = 0;
    /*p29.XUPY*/ XUPY_ABxxEFxx = 0;
    /*p28.AWOH*/ AWOH_xxCDxxGH = 1;
    /*p21.TALU*/ TALU_xxxxEFGH = 1;
    /*p21.SONO*/ SONO_ABCDxxxx = 0;
    /*p29.XOCE*/ XOCE_AxxDExxH = 1;
  }

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
    PHAZ_xxxDEFGx.val = 0; PHAZ_xxxDEFGx.clk = 1;
    PHAZ_xxxxEFGH.val = 1; PHAZ_xxxxEFGH.clk = 1;
    PHAZ_AxxxxFGH.val = 1; PHAZ_AxxxxFGH.clk = 1;
    PHAZ_ABxxxxGH.val = 1; PHAZ_ABxxxxGH.clk = 1;

    WUVU_xxCDxxGH.val = 1; WUVU_xxCDxxGH.clk = 0;
    VENA_xxxxEFGH.val = 1; VENA_xxxxEFGH.clk = 0;
    WOSU_xBCxxFGx.val = 0; WOSU_xBCxxFGx.clk = 1;
  }

private:

  friend struct ClockSignals1;
  friend struct ClockSignals2;

  /*p01.AFUR*/ Reg PHAZ_xxxDEFGx;
  /*p01.ALEF*/ Reg PHAZ_xxxxEFGH;
  /*p01.APUK*/ Reg PHAZ_AxxxxFGH;
  /*p01.ADYK*/ Reg PHAZ_ABxxxxGH;

  /*p29.WUVU*/ Reg WUVU_xxCDxxGH;
  /*p21.VENA*/ Reg VENA_xxxxEFGH;
  /*p29.WOSU*/ Reg WOSU_xBCxxFGx;
};

//-----------------------------------------------------------------------------

};