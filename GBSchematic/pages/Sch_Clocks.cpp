#include "Sch_Clocks.h"

#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Resets.h"

namespace Schematics {

void check_phase_name(int phase, const bool val, char* name) {
  bool expected = name[phase + 5] != 'x';
  if (val != expected) {
    printf("Phase of %s FAIL - phase %d, expected %d, actual %d\n", name, phase, expected, val);
    __debugbreak();
  }
}

//-----------------------------------------------------------------------------

void ClockSignals1::check_phase(const SystemRegisters& sys_reg) const {
  int phase = sys_reg.phaseC();

  check_phase_name(phase, ANOS_AxCxExGx, "ANOS_AxCxExGx");
  check_phase_name(phase, AVET_xBxDxFxH, "AVET_xBxDxFxH");
  check_phase_name(phase, ZEME_xBxDxFxH, "ZEME_xBxDxFxH");
  check_phase_name(phase, ALET_AxCxExGx, "ALET_AxCxExGx");
  check_phase_name(phase, MYVO_xBxDxFxH, "MYVO_xBxDxFxH");
  check_phase_name(phase, MOXE_xBxDxFxH, "MOXE_xBxDxFxH");
  check_phase_name(phase, MEHE_xBxDxFxH, "MEHE_xBxDxFxH");
  check_phase_name(phase, LAPE_xBxDxFxH, "LAPE_xBxDxFxH");
  check_phase_name(phase, TAVA_AxCxExGx, "TAVA_AxCxExGx");

  check_phase_name(phase, AROV_xxCDEFxx, "AROV_xxCDEFxx");
  check_phase_name(phase, AFAS_xxxxEFGx, "AFAS_xxxxEFGx");

  if (sys_reg.CLK_GOOD) {
    check_phase_name(phase, BOGA_xBCDEFGH, "BOGA_xBCDEFGH");
  }

  if (sys_reg.CPUCLK_REQ) {
    check_phase_name(phase, DOVA_ABCDxxxx, "DOVA_ABCDxxxx");
    check_phase_name(phase, UVYT_ABCDxxxx, "UVYT_ABCDxxxx");
    check_phase_name(phase, MOPA_xxxxEFGH, "MOPA_xxxxEFGH");
    check_phase_name(phase, BEDO_Axxxxxxx, "BEDO_Axxxxxxx");
    check_phase_name(phase, BORY_AxxxxxGH, "BORY_AxxxxxGH");
  }
}

//----------------------------------------

ClockSignals1 ClockSignals1::tick_slow(const SystemRegisters& sys_reg, const ClockRegisters1& clk_reg) {
  ClockSignals1 sig;

  /*p01.ARYS*/ wire ARYS_AxCxExGx = sys_reg.clk();
  /*p01.ANOS*/ sig.ANOS_AxCxExGx = sys_reg.clk();
  /*p01.AVET*/ sig.AVET_xBxDxFxH = not(sys_reg.clk());
  /*p01.ATAL*/ wire ATAL_AxCxExGx = not(sig.AVET_xBxDxFxH);
  /*p01.AZOF*/ wire AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
  /*p01.ZAXY*/ wire ZAXY_AxCxExGx = not(AZOF_xBxDxFxH);
  /*p01.ZEME*/ sig.ZEME_xBxDxFxH = not(ZAXY_AxCxExGx);
  /*p01.ALET*/ sig.ALET_AxCxExGx = not(sig.ZEME_xBxDxFxH);
  /*p27.MYVO*/ sig.MYVO_xBxDxFxH = not(sig.ALET_AxCxExGx);
  /*p27.MOXE*/ sig.MOXE_xBxDxFxH = not(sig.ALET_AxCxExGx);
  /*p27.MEHE*/ sig.MEHE_xBxDxFxH = not(sig.ALET_AxCxExGx);
  /*p01.LAPE*/ sig.LAPE_xBxDxFxH = not(sig.ALET_AxCxExGx);
  /*p27.TAVA*/ sig.TAVA_AxCxExGx = not(sig.LAPE_xBxDxFxH);

  // gated on MODE_PROD
  /*p01.AFUR*/ wire PHAZ_ABCDxxxx = and(clk_reg.PHAZ_ABCDxxxx, sys_reg.MODE_PROD);
  /*p01.ALEF*/ wire PHAZ_xBCDExxx = and(clk_reg.PHAZ_xBCDExxx, sys_reg.MODE_PROD);
  /*p01.APUK*/ wire PHAZ_xxCDEFxx = and(clk_reg.PHAZ_xxCDEFxx, sys_reg.MODE_PROD);
  /*p01.ADYK*/ wire PHAZ_xxxDEFGx = and(clk_reg.PHAZ_xxxDEFGx, sys_reg.MODE_PROD);

  /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( PHAZ_xBCDExxx);
  /*p01.ATYP*/ wire ATYP_ABCDxxxx  = not(!PHAZ_ABCDxxxx);
  /*p01.ADAR*/ wire ADAR_ABCxxxxH = not( PHAZ_xxxDEFGx);
  /*p01.AROV*/ sig.AROV_xxCDEFxx  = not(!PHAZ_xxCDEFxx);
  /*p01.AJAX*/ sig.AJAX_xxxxEFGH = not(ATYP_ABCDxxxx);

  /*p01.AFAS*/ sig.AFAS_xxxxEFGx = nor(ADAR_ABCxxxxH, ATYP_ABCDxxxx);

  // gated on CPUCLK_REQ
  /*p01.NULE*/ wire NULE_xxxxEFGH = nor(sys_reg.CPUCLK_REQn, ATYP_ABCDxxxx);
  /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
  /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
  /*p01.DOVA*/ sig.DOVA_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.UVYT*/ sig.UVYT_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.BEKO*/ wire BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p04.MOPA*/ sig.MOPA_xxxxEFGH = not(sig.UVYT_ABCDxxxx);

  /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(sys_reg.CPUCLK_REQn, sig.AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*p01.BERU*/ wire BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
  /*p01.BUFA*/ wire BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
  /*p01.BOLO*/ wire BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);
  /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
  /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
  /*p01.BELO*/ wire BELO_xxxxEFGH = not(BANE_ABCDxxxx);
  /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BELO_xxxxEFGH);

  // BAZE here seems incongruous
  /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
  /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, sys_reg.CLK_BAD2);
  /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
  /*p01.BOGA*/ sig.BOGA_xBCDEFGH = not(BALY_Axxxxxxx);


  /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(sys_reg.CPUCLK_REQ, BALY_Axxxxxxx);
  /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
  /*p01.BEDO*/ sig.BEDO_Axxxxxxx = not(BYXO_xBCDEFGH);
  /*p01.BOWA*/ sig.BOWA_xBCDEFGH = not(sig.BEDO_Axxxxxxx);

  /*p01.BUGO*/ wire BUGO_xBCDExxx = not(AFEP_AxxxxFGH);
  /*p01.BATE*/ wire BATE_AxxxxxGH = nor(sys_reg.CPUCLK_REQn, BUGO_xBCDExxx, sig.AROV_xxCDEFxx);
  /*p01.BASU*/ wire BASU_xBCDEFxx = not(BATE_AxxxxxGH);
  /*p01.BUKE*/ wire BUKE_AxxxxxGH = not(BASU_xBCDEFxx);
  /*p17.ABUR*/ wire ABUR_xBCDEFxx = not(BUKE_AxxxxxGH);
  /*p17.BORY*/ sig.BORY_AxxxxxGH = not(ABUR_xBCDEFxx);


  return sig;
}

//----------------------------------------

ClockSignals1 ClockSignals1::tick_fast(const SystemRegisters& sys_reg, const ClockRegisters1& clk_reg) {
  return tick_slow(sys_reg, clk_reg);

#if 0
  ClockSignals1 sig;

  wire CLK = sys_reg.clk();

  /*p01.ARYS*/ sig.ARYS_AxCxExGx = CLK;
  /*p01.ANOS*/ sig.ANOS_AxCxExGx = CLK;
  /*p01.ATAL*/ sig.ATAL_AxCxExGx = CLK;
  /*p01.ZAXY*/ sig.ZAXY_AxCxExGx = CLK;
  /*p01.ALET*/ sig.ALET_AxCxExGx = CLK;
  /*p29.XYVA*/ sig.XYVA_AxCxExGx = CLK;
  /*p27.TAVA*/ sig.TAVA_AxCxExGx = CLK;
  /*p29.XYFY*/ sig.XYFY_AxCxExGx = CLK;

  /*p01.AVET*/ sig.AVET_xBxDxFxH = !CLK;
  /*p01.AZOF*/ sig.AZOF_xBxDxFxH = !CLK;
  /*p01.ZEME*/ sig.ZEME_xBxDxFxH = !CLK;
  /*p27.MYVO*/ sig.MYVO_xBxDxFxH = !CLK;
  /*p29.XOTA*/ sig.XOTA_xBxDxFxH = !CLK;
  /*p27.MOXE*/ sig.MOXE_xBxDxFxH = !CLK;
  /*p27.MEHE*/ sig.MEHE_xBxDxFxH = !CLK;
  /*p01.LAPE*/ sig.LAPE_xBxDxFxH = !CLK;

  /*p01.AFUR*/ sig.PHAZ_ABCDxxxx = and(clk_reg.PHAZ_ABCDxxxx, sys_reg.MODE_PROD);
  /*p01.ALEF*/ sig.PHAZ_xBCDExxx = and(clk_reg.PHAZ_xBCDExxx, sys_reg.MODE_PROD);
  /*p01.APUK*/ sig.PHAZ_xxCDEFxx = and(clk_reg.PHAZ_xxCDEFxx, sys_reg.MODE_PROD);
  /*p01.ADYK*/ sig.PHAZ_xxxDEFGx = and(clk_reg.PHAZ_xxxDEFGx, sys_reg.MODE_PROD);


  /*p01.AFEP*/ sig.AFEP_AxxxxFGH = !sig.PHAZ_xBCDExxx;
  /*p01.ATYP*/ sig.ATYP_ABCDxxxx =  sig.PHAZ_ABCDxxxx;
  /*p01.ADAR*/ sig.ADAR_ABCxxxxH = !sig.PHAZ_xxxDEFGx;
  /*p01.AROV*/ sig.AROV_xxCDEFxx =  sig.PHAZ_xxCDEFxx;
  /*p01.AFAS*/ sig.AFAS_xxxxEFGx = nor(!sig.PHAZ_xxxDEFGx, sig.PHAZ_ABCDxxxx);

  if (sys_reg.CPUCLK_REQ) {
    // gated on CPUCLK_REQ
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor(0, sig.ATYP_ABCDxxxx);
    /*p01.BUDE*/ wire BUDE_xxxxEFGH = NULE_xxxxEFGH;
    /*p04.MOPA*/ sig.MOPA_xxxxEFGH = NULE_xxxxEFGH;
    /*p01.BYRY*/ wire BYRY_ABCDxxxx = !NULE_xxxxEFGH;
    /*p01.DOVA*/ sig.DOVA_ABCDxxxx = !NULE_xxxxEFGH;
    /*p01.UVYT*/ sig.UVYT_ABCDxxxx = !NULE_xxxxEFGH;
    /*p01.BEKO*/ wire BEKO_ABCDxxxx = !NULE_xxxxEFGH;

    /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(0, sig.AROV_xxCDEFxx, sig.ATYP_ABCDxxxx);
    /*p01.BUFA*/ wire BUFA_xxxxxxGH = BAPY_xxxxxxGH;
    /*p01.BERU*/ wire BERU_ABCDEFxx = !BAPY_xxxxxxGH;
    /*p01.BOLO*/ wire BOLO_ABCDEFxx = !BAPY_xxxxxxGH;

    /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
    /*p01.BELO*/ wire BELO_xxxxEFGH = BEJA_xxxxEFGH;
    /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
    /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BEJA_xxxxEFGH);

    /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(sig.AFEP_AxxxxFGH, sig.ATYP_ABCDxxxx, BAZE_ABCDxxxx);
    /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
    /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, sys_reg.CLK_BAD2);
    /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
    /*p01.BOGA*/ sig.BOGA_xBCDEFGH = BYJU_xBCDEFGH;

    /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(1, BALY_Axxxxxxx);
    /*p01.BEDO*/ sig.BEDO_Axxxxxxx = BUVU_Axxxxxxx;
    /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
    /*p01.BOWA*/ sig.BOWA_xBCDEFGH = not(BUVU_Axxxxxxx);
  }
  else {
    // gated on CPUCLK_REQ
    /*p01.NULE*/ wire NULE_xxxxEFGH = 0;
    /*p01.BUDE*/ wire BUDE_xxxxEFGH = 0;
    /*p04.MOPA*/ sig.MOPA_xxxxEFGH = 0;
    /*p01.BYRY*/ wire BYRY_ABCDxxxx = 1;
    /*p01.DOVA*/ sig.DOVA_ABCDxxxx = 1;
    /*p01.UVYT*/ sig.UVYT_ABCDxxxx = 1;
    /*p01.BEKO*/ wire BEKO_ABCDxxxx = 1;

    /*p01.BAPY*/ wire BAPY_xxxxxxGH = 0;
    /*p01.BUFA*/ wire BUFA_xxxxxxGH = 0;
    /*p01.BERU*/ wire BERU_ABCDEFxx = 1;
    /*p01.BOLO*/ wire BOLO_ABCDEFxx = 1;

    /*p01.BEJA*/ wire BEJA_xxxxEFGH = 0;
    /*p01.BELO*/ wire BELO_xxxxEFGH = 0;
    /*p01.BANE*/ wire BANE_ABCDxxxx = 1;
    /*p01.BAZE*/ wire BAZE_ABCDxxxx = 1;

    /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(sig.AFEP_AxxxxFGH, sig.ATYP_ABCDxxxx);
    /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
    /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, sys_reg.CLK_BAD2);
    /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
    /*p01.BOGA*/ sig.BOGA_xBCDEFGH = BYJU_xBCDEFGH;

    /*p01.BUVU*/ wire BUVU_Axxxxxxx = 0;
    /*p01.BEDO*/ sig.BEDO_Axxxxxxx = 0;
    /*p01.BYXO*/ wire BYXO_xBCDEFGH = 1;
    /*p01.BOWA*/ sig.BOWA_xBCDEFGH = 1;
  }


  return sig;
#endif
}

//-----------------------------------------------------------------------------

void ClockRegisters1::pwron() {
  PHAZ_ABCDxxxx.pwron();
  PHAZ_xBCDExxx.pwron();
  PHAZ_xxCDEFxx.pwron();
  PHAZ_xxxDEFGx.pwron();
}

void ClockRegisters1::reset() {
  PHAZ_ABCDxxxx.reset(0, 1, 0);
  PHAZ_xBCDExxx.reset(0, 1, 0);
  PHAZ_xxCDEFxx.reset(0, 1, 0);
  PHAZ_xxxDEFGx.reset(0, 1, 0);
}

void ClockRegisters1::check_phase(const SystemRegisters& sys_reg) const {
  int phase = sys_reg.phaseC();

  check_phase_name(phase, PHAZ_ABCDxxxx, "PHAZ_ABCDxxxx");
  check_phase_name(phase, PHAZ_xBCDExxx, "PHAZ_xBCDExxx");
  check_phase_name(phase, PHAZ_xxCDEFxx, "PHAZ_xxCDEFxx");
  check_phase_name(phase, PHAZ_xxxDEFGx, "PHAZ_xxxDEFGx");
}

//----------------------------------------

void ClockRegisters1::tick_slow(const SystemRegisters& sys_reg) {
  ClockRegisters1 prev = *this;
  ClockRegisters1& next = *this;

  wire CLK = sys_reg.clk();
  /*p01.AVET*/ wire AVET_xBxDxFxH = not(CLK);
  /*p01.ATAL*/ wire ATAL_AxCxExGx = not(AVET_xBxDxFxH);

  /*p01.AFUR*/ next.PHAZ_ABCDxxxx.set(ATAL_AxCxExGx, sys_reg.MODE_PROD, !prev.PHAZ_xxxDEFGx);
  /*p01.ALEF*/ next.PHAZ_xBCDExxx.set(ATAL_AxCxExGx, sys_reg.MODE_PROD,  prev.PHAZ_ABCDxxxx);
  /*p01.APUK*/ next.PHAZ_xxCDEFxx.set(ATAL_AxCxExGx, sys_reg.MODE_PROD,  prev.PHAZ_xBCDExxx);
  /*p01.ADYK*/ next.PHAZ_xxxDEFGx.set(ATAL_AxCxExGx, sys_reg.MODE_PROD,  prev.PHAZ_xxCDEFxx);
}

//----------------------------------------

void ClockRegisters1::tick_fast(const SystemRegisters& sys_reg) {
  PHAZ_ABCDxxxx.set(sys_reg.clk(), sys_reg.MODE_PROD, !PHAZ_xxxDEFGx);
  PHAZ_xBCDExxx.set(sys_reg.clk(), sys_reg.MODE_PROD,  PHAZ_ABCDxxxx);
  PHAZ_xxCDEFxx.set(sys_reg.clk(), sys_reg.MODE_PROD,  PHAZ_xBCDExxx);
  PHAZ_xxxDEFGx.set(sys_reg.clk(), sys_reg.MODE_PROD,  PHAZ_xxCDEFxx);
}

//----------------------------------------

void ClockRegisters1::commit() {
  PHAZ_ABCDxxxx.commit_duo();
  PHAZ_xBCDExxx.commit_duo();
  PHAZ_xxCDEFxx.commit_duo();
  PHAZ_xxxDEFGx.commit_duo();
}

//-----------------------------------------------------------------------------

void ClockSignals2::check_phase(const SystemRegisters& sys_reg) const {
  int phase = sys_reg.phaseC();

  //check_phase_name(phase, WUVU_AxxDExxH, "WUVU_AxxDExxH");
  //check_phase_name(phase, VENA_xBCDExxx, "VENA_xBCDExxx");
  //check_phase_name(phase, WOSU_xxCDxxGH, "WOSU_xxCDxxGH");
  check_phase_name(phase, XUPY_xBCxxFGx, "XUPY_xBCxxFGx");
  check_phase_name(phase, AWOH_AxxDExxH, "AWOH_AxxDExxH");
  check_phase_name(phase, TALU_xBCDExxx, "TALU_xBCDExxx");
  check_phase_name(phase, SONO_AxxxxFGH, "SONO_AxxxxFGH");
  check_phase_name(phase, XOCE_ABxxEFxx, "XOCE_ABxxEFxx");
}

//----------------------------------------

ClockSignals2 ClockSignals2::tick_slow(const SystemRegisters& /*sys_reg*/, const ResetSignals2& rst_sig2, const ClockRegisters2& clk_reg) {
  ClockSignals2 sig;

  // gated on VID_RESETn
  /*p29.WUVU*/ wire WUVU_AxxDExxH = and(clk_reg.WUVU_AxxDExxH, rst_sig2.VID_RESETn);
  /*p21.VENA*/ wire VENA_xBCDExxx = and(clk_reg.VENA_xBCDExxx, rst_sig2.VID_RESETn);
  /*p29.WOSU*/ wire WOSU_xxCDxxGH = and(clk_reg.WOSU_xxCDxxGH, rst_sig2.VID_RESETn);
  /*p29.WOJO*/ wire WOJO = nor(!WUVU_AxxDExxH, !WOSU_xxCDxxGH);
  /*p29.XUPY*/ sig.XUPY_xBCxxFGx = not(WUVU_AxxDExxH);
  /*p28.AWOH*/ sig.AWOH_AxxDExxH = not(sig.XUPY_xBCxxFGx);
  /*p21.TALU*/ sig.TALU_xBCDExxx = not(!VENA_xBCDExxx);
  /*p21.SONO*/ sig.SONO_AxxxxFGH = not(sig.TALU_xBCDExxx);
  /*p29.XOCE*/ sig.XOCE_ABxxEFxx = not(WOSU_xxCDxxGH);

  /*p29.XYSO*/ sig.XYSO = not(WOJO);

  return sig;
}

//----------------------------------------

ClockSignals2 ClockSignals2::tick_fast(const SystemRegisters& /*sys_reg*/, const ResetSignals2& /*rst_sig2*/, const ClockRegisters2& clk_reg) {
  ClockSignals2 sig;

  // gated on VID_RESETn
  sig.XUPY_xBCxxFGx = !clk_reg.WUVU_AxxDExxH;
  sig.AWOH_AxxDExxH =  clk_reg.WUVU_AxxDExxH;
  sig.TALU_xBCDExxx =  clk_reg.VENA_xBCDExxx;
  sig.SONO_AxxxxFGH = !clk_reg.VENA_xBCDExxx;
  sig.XOCE_ABxxEFxx = !clk_reg.WOSU_xxCDxxGH;

  return sig;
}

//-----------------------------------------------------------------------------

void ClockRegisters2::pwron() {
  WUVU_AxxDExxH.pwron();
  VENA_xBCDExxx.pwron();
  WOSU_xxCDxxGH.pwron();

  WUVU_AxxDExxH2.pwron();
  VENA_xBCDExxx2.pwron();
  WOSU_xxCDxxGH2.pwron();
}

void ClockRegisters2::reset() {
  WUVU_AxxDExxH.val = 1; WUVU_AxxDExxH.clk = 1;
  VENA_xBCDExxx.val = 0; VENA_xBCDExxx.clk = 0;
  WOSU_xxCDxxGH.val = 1; WOSU_xxCDxxGH.clk = 0;

  WUVU_AxxDExxH2.reset(1, 1, 1);
  VENA_xBCDExxx2.reset(0, 1, 0);
  WOSU_xxCDxxGH2.reset(0, 1, 1);
}

void ClockRegisters2::check_phase(const SystemRegisters& sys_reg) const {
  int phase = sys_reg.phaseC();

  check_phase_name(phase, WUVU_AxxDExxH, "WUVU_AxxDExxH");
  check_phase_name(phase, VENA_xBCDExxx, "VENA_xBCDExxx");
  check_phase_name(phase, WOSU_xxCDxxGH, "WOSU_xxCDxxGH");
}

//----------------------------------------

void ClockRegisters2::tock_slow(const ClockSignals1& clk_sig1, const ResetSignals2& rst_sig2) {
  ClockRegisters2 prev = *this;
  ClockRegisters2& next = *this;

  /*p29.XYVA*/ wire XYVA_AxCxExGx = not(clk_sig1.ZEME_xBxDxFxH);
  /*p29.XOTA*/ wire XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
  /*p29.XYFY*/ wire XYFY_AxCxExGx = not(XOTA_xBxDxFxH);

  /*p29.WUVU*/ next.WUVU_AxxDExxH.tock( XOTA_xBxDxFxH, rst_sig2.VID_RESETn, !prev.WUVU_AxxDExxH);
  /*p21.VENA*/ next.VENA_xBCDExxx.tock(!prev.WUVU_AxxDExxH,     rst_sig2.VID_RESETn, !prev.VENA_xBCDExxx);
  /*p29.WOSU*/ next.WOSU_xxCDxxGH.tock( XYFY_AxCxExGx, rst_sig2.VID_RESETn, !prev.WUVU_AxxDExxH);

  /*p29.WUVU*/ next.WUVU_AxxDExxH2.set( XOTA_xBxDxFxH, rst_sig2.VID_RESETn, !prev.WUVU_AxxDExxH);
  /*p21.VENA*/ next.VENA_xBCDExxx2.set(!prev.WUVU_AxxDExxH,     rst_sig2.VID_RESETn, !prev.VENA_xBCDExxx);
  /*p29.WOSU*/ next.WOSU_xxCDxxGH2.set( XYFY_AxCxExGx, rst_sig2.VID_RESETn, !prev.WUVU_AxxDExxH);
}

//----------------------------------------

void ClockRegisters2::tock_fast(const ClockSignals1& clk_sig1, const ResetSignals2& rst_sig2) {

  /*p29.XYVA*/ wire XYVA_AxCxExGx = not(clk_sig1.ZEME_xBxDxFxH);
  /*p29.XOTA*/ wire XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
  /*p29.XYFY*/ wire XYFY_AxCxExGx = not(XOTA_xBxDxFxH);

  /*p29.WUVU*/ WUVU_AxxDExxH.tock( XOTA_xBxDxFxH, rst_sig2.VID_RESETn, !WUVU_AxxDExxH);
  /*p21.VENA*/ VENA_xBCDExxx.tock(!WUVU_AxxDExxH,          rst_sig2.VID_RESETn, !VENA_xBCDExxx);
  /*p29.WOSU*/ WOSU_xxCDxxGH.tock( XYFY_AxCxExGx, rst_sig2.VID_RESETn, !WUVU_AxxDExxH);

  /*p29.WUVU*/ WUVU_AxxDExxH2.commit();
  /*p21.VENA*/ VENA_xBCDExxx2.commit();
  /*p29.WOSU*/ WOSU_xxCDxxGH2.commit();

  /*p29.WUVU*/ WUVU_AxxDExxH2.set( XOTA_xBxDxFxH, rst_sig2.VID_RESETn, !WUVU_AxxDExxH);
  /*p21.VENA*/ VENA_xBCDExxx2.set(!WUVU_AxxDExxH,          rst_sig2.VID_RESETn, !VENA_xBCDExxx);
  /*p29.WOSU*/ WOSU_xxCDxxGH2.set( XYFY_AxCxExGx, rst_sig2.VID_RESETn, !WUVU_AxxDExxH);

  /*p29.WUVU*/ WUVU_AxxDExxH2.commit();
  /*p21.VENA*/ VENA_xBCDExxx2.commit();
  /*p29.WOSU*/ WOSU_xxCDxxGH2.commit();
}

//----------------------------------------

void ClockRegisters2::commit() {
  /*p29.WUVU*/ WUVU_AxxDExxH2.commit();
  /*p21.VENA*/ VENA_xBCDExxx2.commit();
  /*p29.WOSU*/ WOSU_xxCDxxGH2.commit();

  check(WUVU_AxxDExxH2 == WUVU_AxxDExxH);
  check(VENA_xBCDExxx2 == VENA_xBCDExxx);
  check(WOSU_xxCDxxGH2 == WOSU_xxCDxxGH);
}

//-----------------------------------------------------------------------------

};