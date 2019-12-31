#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ClockSignals1 {
  void check_phase(int phase) const;

  static ClockSignals1 tick_slow(const SystemRegisters& sys_reg, const ClockRegisters1& clk_reg);
  static ClockSignals1 tick_fast(const SystemRegisters& sys_reg, const ClockRegisters1& clk_reg);

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

struct ClockRegisters1 {

  void tock_slow(const SystemRegisters& sys_reg);
  void tock_fast(const SystemRegisters& sys_reg);

  void pwron();
  void reset();
  void check_phase(int phase) const;
  static void check_match(const ClockRegisters1& a, const ClockRegisters1& b);
  void commit();

//private:

  friend struct ClockSignals1;
  friend struct ClockSignals2;

  /*p01.AFUR*/ Reg PHAZ_ABCDxxxx;
  /*p01.ALEF*/ Reg PHAZ_xBCDExxx;
  /*p01.APUK*/ Reg PHAZ_xxCDEFxx;
  /*p01.ADYK*/ Reg PHAZ_xxxDEFGx;

  /*p01.AFUR*/ Reg2 PHAZ_ABCDxxxx2;
  /*p01.ALEF*/ Reg2 PHAZ_xBCDExxx2;
  /*p01.APUK*/ Reg2 PHAZ_xxCDEFxx2;
  /*p01.ADYK*/ Reg2 PHAZ_xxxDEFGx2;
};

//-----------------------------------------------------------------------------
// Video clocks

struct ClockSignals2 {
  void check_phase(int phase) const;

  static ClockSignals2 tick_slow(const SystemRegisters& sys_reg, const ResetSignals2& rst_sig2, const ClockRegisters2& clk_reg);
  static ClockSignals2 tick_fast(const SystemRegisters& sys_reg, const ResetSignals2& rst_sig2, const ClockRegisters2& clk_reg);

  /*p29.WUVU*/ bool WUVU_AxxDExxH;
  /*p21.VENA*/ bool VENA_xBCDExxx;
  /*p29.WOSU*/ bool WOSU_xxCDxxGH;
  /*p29.XUPY*/ bool XUPY_xBCxxFGx;
  /*p28.AWOH*/ bool AWOH_AxxDExxH;
  /*p21.TALU*/ bool TALU_xBCDExxx; // this drives the LCD xy counter
  /*p21.SONO*/ bool SONO_AxxxxFGH;
  /*p29.XOCE*/ bool XOCE_ABxxEFxx;
};

//-----------------------------------------------------------------------------

struct ClockRegisters2 {

  void tock_slow(const SystemRegisters& sys_reg,
                 const ClockSignals1& clk_sig1,
                 const ClockSignals2& clk_sig2,
                 const ResetSignals2& rst_sig2);

  void tock_fast(const SystemRegisters& sys_reg,
                 const ResetSignals2& rst_sig2);

  void pwron();
  void reset();
  void check_phase(int phase) const;
  static void check_match(const ClockRegisters2& a, const ClockRegisters2& b);

//private:

  friend struct ClockSignals1;
  friend struct ClockSignals2;

  /*p29.WUVU*/ Reg WUVU_AxxDExxH;
  /*p21.VENA*/ Reg VENA_xBCDExxx;
  /*p29.WOSU*/ Reg WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

};