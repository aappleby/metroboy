#pragma once
#include "Sch_Common.h"

namespace Schematics {

inline void check_phase_name(int phase, const bool val, char* name) {
  bool expected = name[phase + 5] != 'x';
  if (val != expected) {
    printf("Phase of %s FAIL\n", name);
    __debugbreak();
  }
}
//-----------------------------------------------------------------------------
// c.cpu.CLK_xBCDEFGH = BOWA_xBCDEFGH;

struct ClockSignals1 {
  static ClockSignals1 tick_slow(const SystemSignals& sys_sig, const ClockRegisters& clk_reg);
  static ClockSignals1 tick_fast(const SystemSignals& sys_sig, const ClockRegisters& clk_reg);

  void reset() {
    *this = {
      // ungated
      /*p01.ARYS*/ .ARYS_AxCxExGx = 0,
      /*p01.ANOS*/ .ANOS_AxCxExGx = 0,
      /*p01.AVET*/ .AVET_xBxDxFxH = 1,
      /*p01.ATAL*/ .ATAL_AxCxExGx = 0,
      /*p01.AZOF*/ .AZOF_xBxDxFxH = 1,
      /*p01.ZAXY*/ .ZAXY_AxCxExGx = 0,
      /*p01.ZEME*/ .ZEME_xBxDxFxH = 1,
      /*p01.ALET*/ .ALET_AxCxExGx = 0,
      /*p27.MYVO*/ .MYVO_xBxDxFxH = 1,
      /*p29.XYVA*/ .XYVA_AxCxExGx = 0,
      /*p29.XOTA*/ .XOTA_xBxDxFxH = 1,
      /*p27.MOXE*/ .MOXE_xBxDxFxH = 1,
      /*p27.MEHE*/ .MEHE_xBxDxFxH = 1,
      /*p01.LAPE*/ .LAPE_xBxDxFxH = 1,
      /*p27.TAVA*/ .TAVA_AxCxExGx = 0,
      /*p29.XYFY*/ .XYFY_AxCxExGx = 0,

      // gated on MODE_PROD
      /*p01.AFUR*/ .PHAZ_ABCDxxxx = 0,
      /*p01.ALEF*/ .PHAZ_xBCDExxx = 0,
      /*p01.APUK*/ .PHAZ_xxCDEFxx = 0,
      /*p01.ADYK*/ .PHAZ_xxxDEFGx = 0,
      /*p01.AFEP*/ .AFEP_AxxxxFGH = 1,
      /*p01.ATYP*/ .ATYP_ABCDxxxx = 0,
      /*p01.ADAR*/ .ADAR_ABCxxxxH = 1,
      /*p01.AROV*/ .AROV_xxCDEFxx = 0,
      /*p01.AFAS*/ .AFAS_xxxxEFGx = 0,

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

  void check_phase(int phase) {
    check_phase_name(phase, ARYS_AxCxExGx, "ARYS_AxCxExGx");
    check_phase_name(phase, ANOS_AxCxExGx, "ANOS_AxCxExGx");
    check_phase_name(phase, AVET_xBxDxFxH, "AVET_xBxDxFxH");
    check_phase_name(phase, ATAL_AxCxExGx, "ATAL_AxCxExGx");
    check_phase_name(phase, AZOF_xBxDxFxH, "AZOF_xBxDxFxH");
    check_phase_name(phase, ZAXY_AxCxExGx, "ZAXY_AxCxExGx");
    check_phase_name(phase, ZEME_xBxDxFxH, "ZEME_xBxDxFxH");
    check_phase_name(phase, ALET_AxCxExGx, "ALET_AxCxExGx");
    check_phase_name(phase, MYVO_xBxDxFxH, "MYVO_xBxDxFxH");
    check_phase_name(phase, XYVA_AxCxExGx, "XYVA_AxCxExGx");
    check_phase_name(phase, XOTA_xBxDxFxH, "XOTA_xBxDxFxH");
    check_phase_name(phase, MOXE_xBxDxFxH, "MOXE_xBxDxFxH");
    check_phase_name(phase, MEHE_xBxDxFxH, "MEHE_xBxDxFxH");
    check_phase_name(phase, LAPE_xBxDxFxH, "LAPE_xBxDxFxH");
    check_phase_name(phase, TAVA_AxCxExGx, "TAVA_AxCxExGx");
    check_phase_name(phase, XYFY_AxCxExGx, "XYFY_AxCxExGx");

    check_phase_name(phase, PHAZ_ABCDxxxx, "PHAZ_ABCDxxxx");
    check_phase_name(phase, PHAZ_xBCDExxx, "PHAZ_xBCDExxx");
    check_phase_name(phase, PHAZ_xxCDEFxx, "PHAZ_xxCDEFxx");
    check_phase_name(phase, PHAZ_xxxDEFGx, "PHAZ_xxxDEFGx");
    check_phase_name(phase, AFEP_AxxxxFGH, "AFEP_AxxxxFGH");
    check_phase_name(phase, ATYP_ABCDxxxx, "ATYP_ABCDxxxx");
    check_phase_name(phase, ADAR_ABCxxxxH, "ADAR_ABCxxxxH");
    check_phase_name(phase, AROV_xxCDEFxx, "AROV_xxCDEFxx");
    check_phase_name(phase, AFAS_xxxxEFGx, "AFAS_xxxxEFGx");

    check_phase_name(phase, NULE_xxxxEFGH, "NULE_xxxxEFGH");
    check_phase_name(phase, BYRY_ABCDxxxx, "BYRY_ABCDxxxx");
    check_phase_name(phase, BUDE_xxxxEFGH, "BUDE_xxxxEFGH");
    check_phase_name(phase, DOVA_ABCDxxxx, "DOVA_ABCDxxxx");
    check_phase_name(phase, UVYT_ABCDxxxx, "UVYT_ABCDxxxx");
    check_phase_name(phase, BEKO_ABCDxxxx, "BEKO_ABCDxxxx");
    check_phase_name(phase, MOPA_xxxxEFGH, "MOPA_xxxxEFGH");

    check_phase_name(phase, BAPY_xxxxxxGH, "BAPY_xxxxxxGH");
    check_phase_name(phase, BERU_ABCDEFxx, "BERU_ABCDEFxx");
    check_phase_name(phase, BUFA_xxxxxxGH, "BUFA_xxxxxxGH");
    check_phase_name(phase, BOLO_ABCDEFxx, "BOLO_ABCDEFxx");
    check_phase_name(phase, BEJA_xxxxEFGH, "BEJA_xxxxEFGH");
    check_phase_name(phase, BANE_ABCDxxxx, "BANE_ABCDxxxx");
    check_phase_name(phase, BELO_xxxxEFGH, "BELO_xxxxEFGH");
    check_phase_name(phase, BAZE_ABCDxxxx, "BAZE_ABCDxxxx");
    check_phase_name(phase, BUTO_xBCDEFGH, "BUTO_xBCDEFGH");
    check_phase_name(phase, BELE_Axxxxxxx, "BELE_Axxxxxxx");

    check_phase_name(phase, BYJU_xBCDEFGH, "BYJU_xBCDEFGH");
    check_phase_name(phase, BALY_Axxxxxxx, "BALY_Axxxxxxx");
    check_phase_name(phase, BOGA_xBCDEFGH, "BOGA_xBCDEFGH");
    check_phase_name(phase, BUVU_Axxxxxxx, "BUVU_Axxxxxxx");
    check_phase_name(phase, BYXO_xBCDEFGH, "BYXO_xBCDEFGH");
    check_phase_name(phase, BEDO_Axxxxxxx, "BEDO_Axxxxxxx");
    check_phase_name(phase, BOWA_xBCDEFGH, "BOWA_xBCDEFGH");
  }

  // ungated
  /*p01.ARYS*/ bool ARYS_AxCxExGx;
  /*p01.ANOS*/ bool ANOS_AxCxExGx;
  /*p01.AVET*/ bool AVET_xBxDxFxH;
  /*p01.ATAL*/ bool ATAL_AxCxExGx;
  /*p01.AZOF*/ bool AZOF_xBxDxFxH;
  /*p01.ZAXY*/ bool ZAXY_AxCxExGx;
  /*p01.ZEME*/ bool ZEME_xBxDxFxH;
  /*p01.ALET*/ bool ALET_AxCxExGx;
  /*p27.MYVO*/ bool MYVO_xBxDxFxH;
  /*p29.XYVA*/ bool XYVA_AxCxExGx;
  /*p29.XOTA*/ bool XOTA_xBxDxFxH;
  /*p27.MOXE*/ bool MOXE_xBxDxFxH;
  /*p27.MEHE*/ bool MEHE_xBxDxFxH;
  /*p01.LAPE*/ bool LAPE_xBxDxFxH;
  /*p27.TAVA*/ bool TAVA_AxCxExGx;
  /*p29.XYFY*/ bool XYFY_AxCxExGx;

  // gated on MODE_PROD
  /*p01.AFUR*/ bool PHAZ_ABCDxxxx;
  /*p01.ALEF*/ bool PHAZ_xBCDExxx;
  /*p01.APUK*/ bool PHAZ_xxCDEFxx;
  /*p01.ADYK*/ bool PHAZ_xxxDEFGx;
  /*p01.AFEP*/ bool AFEP_AxxxxFGH;
  /*p01.ATYP*/ bool ATYP_ABCDxxxx;
  /*p01.ADAR*/ bool ADAR_ABCxxxxH;
  /*p01.AROV*/ bool AROV_xxCDEFxx;
  /*p01.AFAS*/ bool AFAS_xxxxEFGx;

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

  void check_phase(int phase) {
    check_phase_name(phase, WUVU_AxxDExxH, "WUVU_AxxDExxH");
    check_phase_name(phase, VENA_xBCDExxx, "VENA_xBCDExxx");
    check_phase_name(phase, WOSU_xxCDxxGH, "WOSU_xxCDxxGH");
    check_phase_name(phase, XUPY_xBCxxFGx, "XUPY_xBCxxFGx");
    check_phase_name(phase, AWOH_AxxDExxH, "AWOH_AxxDExxH");
    check_phase_name(phase, TALU_xBCDExxx, "TALU_xBCDExxx");
    check_phase_name(phase, SONO_AxxxxFGH, "SONO_AxxxxFGH");
    check_phase_name(phase, XOCE_ABxxEFxx, "XOCE_ABxxEFxx");
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

  static void tock_fast1(const SystemSignals& sys_sig,
                         const ClockSignals1& clk_sig1,
                         ClockRegisters& next);

  static void tock_slow2(const SystemSignals& sys_sig,
                         const ClockSignals1& clk_sig1,
                         const ClockSignals2& clk_sig2,
                         const ResetSignals2& rst_sig2,
                         ClockRegisters& next);

  static void tock_fast2(const SystemSignals& sys_sig,
                         const ClockSignals1& clk_sig1,
                         const ClockSignals2& clk_sig2,
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

  void check_phase(int phase) {
    check_phase_name(phase, PHAZ_ABCDxxxx, "PHAZ_ABCDxxxx");
    check_phase_name(phase, PHAZ_xBCDExxx, "PHAZ_xBCDExxx");
    check_phase_name(phase, PHAZ_xxCDEFxx, "PHAZ_xxCDEFxx");
    check_phase_name(phase, PHAZ_xxxDEFGx, "PHAZ_xxxDEFGx");
    check_phase_name(phase, WUVU_AxxDExxH, "WUVU_AxxDExxH");
    check_phase_name(phase, VENA_xBCDExxx, "VENA_xBCDExxx");
    check_phase_name(phase, WOSU_xxCDxxGH, "WOSU_xxCDxxGH");
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