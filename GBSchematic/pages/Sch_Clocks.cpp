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

void ClockSignals1::check_phase(int phase) const {
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

//----------------------------------------

ClockSignals1 ClockSignals1::tick_slow(const SystemRegisters& sys_reg,
                              const ClockRegisters1& clk_reg) {
  ClockSignals1 sig;

  /*p01.ARYS*/ sig.ARYS_AxCxExGx = sys_reg.clk();
  /*p01.ANOS*/ sig.ANOS_AxCxExGx = sys_reg.clk();
  /*p01.AVET*/ sig.AVET_xBxDxFxH = not(sys_reg.clk());
  /*p01.ATAL*/ sig.ATAL_AxCxExGx = not(sig.AVET_xBxDxFxH);
  /*p01.AZOF*/ sig.AZOF_xBxDxFxH = not(sig.ATAL_AxCxExGx);
  /*p01.ZAXY*/ sig.ZAXY_AxCxExGx = not(sig.AZOF_xBxDxFxH);
  /*p01.ZEME*/ sig.ZEME_xBxDxFxH = not(sig.ZAXY_AxCxExGx);
  /*p01.ALET*/ sig.ALET_AxCxExGx = not(sig.ZEME_xBxDxFxH);
  /*p27.MYVO*/ sig.MYVO_xBxDxFxH = not(sig.ALET_AxCxExGx);
  /*p29.XYVA*/ sig.XYVA_AxCxExGx = not(sig.ZEME_xBxDxFxH);
  /*p29.XOTA*/ sig.XOTA_xBxDxFxH = not(sig.XYVA_AxCxExGx);
  /*p27.MOXE*/ sig.MOXE_xBxDxFxH = not(sig.ALET_AxCxExGx);
  /*p27.MEHE*/ sig.MEHE_xBxDxFxH = not(sig.ALET_AxCxExGx);
  /*p01.LAPE*/ sig.LAPE_xBxDxFxH = not(sig.ALET_AxCxExGx);
  /*p27.TAVA*/ sig.TAVA_AxCxExGx = not(sig.LAPE_xBxDxFxH);
  /*p29.XYFY*/ sig.XYFY_AxCxExGx = not(sig.XOTA_xBxDxFxH);

  // gated on MODE_PROD
  /*p01.AFUR*/ sig.PHAZ_ABCDxxxx = and(clk_reg.PHAZ_ABCDxxxx, sys_reg.MODE_PROD);
  /*p01.ALEF*/ sig.PHAZ_xBCDExxx = and(clk_reg.PHAZ_xBCDExxx, sys_reg.MODE_PROD);
  /*p01.APUK*/ sig.PHAZ_xxCDEFxx = and(clk_reg.PHAZ_xxCDEFxx, sys_reg.MODE_PROD);
  /*p01.ADYK*/ sig.PHAZ_xxxDEFGx = and(clk_reg.PHAZ_xxxDEFGx, sys_reg.MODE_PROD);

  /*p01.AFEP*/ sig.AFEP_AxxxxFGH = not( sig.PHAZ_xBCDExxx);
  /*p01.ATYP*/ sig.ATYP_ABCDxxxx = not(!sig.PHAZ_ABCDxxxx);
  /*p01.ADAR*/ sig.ADAR_ABCxxxxH = not( sig.PHAZ_xxxDEFGx);
  /*p01.AROV*/ sig.AROV_xxCDEFxx = not(!sig.PHAZ_xxCDEFxx);
  /*p01.AFAS*/ sig.AFAS_xxxxEFGx = nor(sig.ADAR_ABCxxxxH, sig.ATYP_ABCDxxxx);

  // gated on CPUCLK_REQ
  /*p01.NULE*/ sig.NULE_xxxxEFGH = nor(sys_reg.CPUCLK_REQn, sig.ATYP_ABCDxxxx);
  /*p01.BYRY*/ sig.BYRY_ABCDxxxx = not(sig.NULE_xxxxEFGH);
  /*p01.BUDE*/ sig.BUDE_xxxxEFGH = not(sig.BYRY_ABCDxxxx);
  /*p01.DOVA*/ sig.DOVA_ABCDxxxx = not(sig.BUDE_xxxxEFGH);
  /*p01.UVYT*/ sig.UVYT_ABCDxxxx = not(sig.BUDE_xxxxEFGH);
  /*p01.BEKO*/ sig.BEKO_ABCDxxxx = not(sig.BUDE_xxxxEFGH);
  /*p04.MOPA*/ sig.MOPA_xxxxEFGH = not(sig.UVYT_ABCDxxxx);

  /*p01.BAPY*/ sig.BAPY_xxxxxxGH = nor(sys_reg.CPUCLK_REQn, sig.AROV_xxCDEFxx, sig.ATYP_ABCDxxxx);
  /*p01.BERU*/ sig.BERU_ABCDEFxx = not(sig.BAPY_xxxxxxGH);
  /*p01.BUFA*/ sig.BUFA_xxxxxxGH = not(sig.BERU_ABCDEFxx);
  /*p01.BOLO*/ sig.BOLO_ABCDEFxx = not(sig.BUFA_xxxxxxGH);
  /*p01.BEJA*/ sig.BEJA_xxxxEFGH = nand(sig.BOLO_ABCDEFxx, sig.BEKO_ABCDxxxx);
  /*p01.BANE*/ sig.BANE_ABCDxxxx = not(sig.BEJA_xxxxEFGH);
  /*p01.BELO*/ sig.BELO_xxxxEFGH = not(sig.BANE_ABCDxxxx);
  /*p01.BAZE*/ sig.BAZE_ABCDxxxx = not(sig.BELO_xxxxEFGH);
  /*p01.BUTO*/ sig.BUTO_xBCDEFGH = nand(sig.AFEP_AxxxxFGH, sig.ATYP_ABCDxxxx, sig.BAZE_ABCDxxxx);
  /*p01.BELE*/ sig.BELE_Axxxxxxx = not(sig.BUTO_xBCDEFGH);

  // gated on CLK_GOOD
  /*p01.BYJU*/ sig.BYJU_xBCDEFGH = nor(sig.BELE_Axxxxxxx, sys_reg.CLK_BAD2);
  /*p01.BALY*/ sig.BALY_Axxxxxxx = not(sig.BYJU_xBCDEFGH);
  /*p01.BOGA*/ sig.BOGA_xBCDEFGH = not(sig.BALY_Axxxxxxx);
  /*p01.BUVU*/ sig.BUVU_Axxxxxxx = and(sys_reg.CPUCLK_REQ, sig.BALY_Axxxxxxx);
  /*p01.BYXO*/ sig.BYXO_xBCDEFGH = not(sig.BUVU_Axxxxxxx);
  /*p01.BEDO*/ sig.BEDO_Axxxxxxx = not(sig.BYXO_xBCDEFGH);
  /*p01.BOWA*/ sig.BOWA_xBCDEFGH = not(sig.BEDO_Axxxxxxx);

  return sig;
}

//----------------------------------------

ClockSignals1 ClockSignals1::tick_fast(const SystemRegisters& sys_reg, const ClockRegisters1& clk_reg) {
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

  // gated on CPUCLK_REQ
  /*p01.NULE*/ sig.NULE_xxxxEFGH = nor(sys_reg.CPUCLK_REQn, sig.ATYP_ABCDxxxx);
  /*p01.BYRY*/ sig.BYRY_ABCDxxxx = not(sig.NULE_xxxxEFGH);
  /*p01.BUDE*/ sig.BUDE_xxxxEFGH = not(sig.BYRY_ABCDxxxx);
  /*p01.DOVA*/ sig.DOVA_ABCDxxxx = not(sig.BUDE_xxxxEFGH);
  /*p01.UVYT*/ sig.UVYT_ABCDxxxx = not(sig.BUDE_xxxxEFGH);
  /*p01.BEKO*/ sig.BEKO_ABCDxxxx = not(sig.BUDE_xxxxEFGH);
  /*p04.MOPA*/ sig.MOPA_xxxxEFGH = not(sig.UVYT_ABCDxxxx);

  /*p01.BAPY*/ sig.BAPY_xxxxxxGH = nor(sys_reg.CPUCLK_REQn, sig.AROV_xxCDEFxx, sig.ATYP_ABCDxxxx);
  /*p01.BERU*/ sig.BERU_ABCDEFxx = not(sig.BAPY_xxxxxxGH);
  /*p01.BUFA*/ sig.BUFA_xxxxxxGH = not(sig.BERU_ABCDEFxx);
  /*p01.BOLO*/ sig.BOLO_ABCDEFxx = not(sig.BUFA_xxxxxxGH);
  /*p01.BEJA*/ sig.BEJA_xxxxEFGH = nand(sig.BOLO_ABCDEFxx, sig.BEKO_ABCDxxxx);
  /*p01.BANE*/ sig.BANE_ABCDxxxx = not(sig.BEJA_xxxxEFGH);
  /*p01.BELO*/ sig.BELO_xxxxEFGH = not(sig.BANE_ABCDxxxx);
  /*p01.BAZE*/ sig.BAZE_ABCDxxxx = not(sig.BELO_xxxxEFGH);
  /*p01.BUTO*/ sig.BUTO_xBCDEFGH = nand(sig.AFEP_AxxxxFGH, sig.ATYP_ABCDxxxx, sig.BAZE_ABCDxxxx);
  /*p01.BELE*/ sig.BELE_Axxxxxxx = not(sig.BUTO_xBCDEFGH);

  // gated on CLK_GOOD
  /*p01.BYJU*/ sig.BYJU_xBCDEFGH = nor(sig.BELE_Axxxxxxx, sys_reg.CLK_BAD2);
  /*p01.BALY*/ sig.BALY_Axxxxxxx = not(sig.BYJU_xBCDEFGH);
  /*p01.BOGA*/ sig.BOGA_xBCDEFGH = not(sig.BALY_Axxxxxxx);
  /*p01.BUVU*/ sig.BUVU_Axxxxxxx = and(sys_reg.CPUCLK_REQ, sig.BALY_Axxxxxxx);
  /*p01.BYXO*/ sig.BYXO_xBCDEFGH = not(sig.BUVU_Axxxxxxx);
  /*p01.BEDO*/ sig.BEDO_Axxxxxxx = not(sig.BYXO_xBCDEFGH);
  /*p01.BOWA*/ sig.BOWA_xBCDEFGH = not(sig.BEDO_Axxxxxxx);

  return sig;
}

//-----------------------------------------------------------------------------

void ClockRegisters1::pwron() {
  PHAZ_ABCDxxxx.val = 0; PHAZ_ABCDxxxx.clk = 0;
  PHAZ_xBCDExxx.val = 0; PHAZ_xBCDExxx.clk = 0;
  PHAZ_xxCDEFxx.val = 0; PHAZ_xxCDEFxx.clk = 0;
  PHAZ_xxxDEFGx.val = 0; PHAZ_xxxDEFGx.clk = 0;
}
void ClockRegisters1::reset() {
  PHAZ_ABCDxxxx.val = 0; PHAZ_ABCDxxxx.clk = 0;
  PHAZ_xBCDExxx.val = 0; PHAZ_xBCDExxx.clk = 0;
  PHAZ_xxCDEFxx.val = 0; PHAZ_xxCDEFxx.clk = 0;
  PHAZ_xxxDEFGx.val = 0; PHAZ_xxxDEFGx.clk = 0;
}

void ClockRegisters1::check_phase(int phase) const {
  check_phase_name(phase, PHAZ_ABCDxxxx, "PHAZ_ABCDxxxx");
  check_phase_name(phase, PHAZ_xBCDExxx, "PHAZ_xBCDExxx");
  check_phase_name(phase, PHAZ_xxCDEFxx, "PHAZ_xxCDEFxx");
  check_phase_name(phase, PHAZ_xxxDEFGx, "PHAZ_xxxDEFGx");
}

void ClockRegisters1::check_match(const ClockRegisters1& a, const ClockRegisters1& b) {
  check(a.PHAZ_ABCDxxxx.val == b.PHAZ_ABCDxxxx.val);
  check(a.PHAZ_xBCDExxx.val == b.PHAZ_xBCDExxx.val);
  check(a.PHAZ_xxCDEFxx.val == b.PHAZ_xxCDEFxx.val);
  check(a.PHAZ_xxxDEFGx.val == b.PHAZ_xxxDEFGx.val);
}

//----------------------------------------

void ClockRegisters1::tock_slow(const SystemRegisters& sys_reg) {
  ClockRegisters1 prev = *this;
  ClockRegisters1& next = *this;

  wire CLK = sys_reg.clk();

  /*p01.AVET*/ wire AVET_xBxDxFxH = not(CLK);
  /*p01.ATAL*/ wire ATAL_AxCxExGx = not(AVET_xBxDxFxH);

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ next.PHAZ_ABCDxxxx.duotock(ATAL_AxCxExGx, sys_reg.MODE_PROD, !prev.PHAZ_xxxDEFGx);
  /*p01.ALEF*/ next.PHAZ_xBCDExxx.duotock(ATAL_AxCxExGx, sys_reg.MODE_PROD,  prev.PHAZ_ABCDxxxx);
  /*p01.APUK*/ next.PHAZ_xxCDEFxx.duotock(ATAL_AxCxExGx, sys_reg.MODE_PROD,  prev.PHAZ_xBCDExxx);
  /*p01.ADYK*/ next.PHAZ_xxxDEFGx.duotock(ATAL_AxCxExGx, sys_reg.MODE_PROD,  prev.PHAZ_xxCDEFxx);
}

//----------------------------------------

void ClockRegisters1::tock_fast(const SystemRegisters& sys_reg) {
  int phase = sys_reg.phaseC();

  PHAZ_ABCDxxxx.val = (phase == 0) || (phase == 1) || (phase == 2) || (phase == 3);
  PHAZ_xBCDExxx.val = (phase == 1) || (phase == 2) || (phase == 3) || (phase == 4);
  PHAZ_xxCDEFxx.val = (phase == 2) || (phase == 3) || (phase == 4) || (phase == 5);
  PHAZ_xxxDEFGx.val = (phase == 3) || (phase == 4) || (phase == 5) || (phase == 6);

  if (!sys_reg.MODE_PROD) {
    PHAZ_ABCDxxxx.val = 0;
    PHAZ_xBCDExxx.val = 0;
    PHAZ_xxCDEFxx.val = 0;
    PHAZ_xxxDEFGx.val = 0;
  }
}

//-----------------------------------------------------------------------------

void ClockSignals2::check_phase(int phase) const {
  check_phase_name(phase, WUVU_AxxDExxH, "WUVU_AxxDExxH");
  check_phase_name(phase, VENA_xBCDExxx, "VENA_xBCDExxx");
  check_phase_name(phase, WOSU_xxCDxxGH, "WOSU_xxCDxxGH");
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
  /*p29.WUVU*/ sig.WUVU_AxxDExxH = and(clk_reg.WUVU_AxxDExxH, rst_sig2.VID_RESETn);
  /*p21.VENA*/ sig.VENA_xBCDExxx = and(clk_reg.VENA_xBCDExxx, rst_sig2.VID_RESETn);
  /*p29.WOSU*/ sig.WOSU_xxCDxxGH = and(clk_reg.WOSU_xxCDxxGH, rst_sig2.VID_RESETn);
  /*p29.XUPY*/ sig.XUPY_xBCxxFGx = not(sig.WUVU_AxxDExxH);
  /*p28.AWOH*/ sig.AWOH_AxxDExxH = not(sig.XUPY_xBCxxFGx);
  /*p21.TALU*/ sig.TALU_xBCDExxx = not(!sig.VENA_xBCDExxx);
  /*p21.SONO*/ sig.SONO_AxxxxFGH = not(sig.TALU_xBCDExxx);
  /*p29.XOCE*/ sig.XOCE_ABxxEFxx = not(sig.WOSU_xxCDxxGH);

  return sig;
}

//----------------------------------------

ClockSignals2 ClockSignals2::tick_fast(const SystemRegisters& /*sys_reg*/, const ResetSignals2& rst_sig2, const ClockRegisters2& clk_reg) {
  ClockSignals2 sig;

  // gated on VID_RESETn
  sig.WUVU_AxxDExxH =  clk_reg.WUVU_AxxDExxH;
  sig.VENA_xBCDExxx =  clk_reg.VENA_xBCDExxx;
  sig.WOSU_xxCDxxGH =  clk_reg.WOSU_xxCDxxGH;
  sig.XUPY_xBCxxFGx = !clk_reg.WUVU_AxxDExxH;
  sig.AWOH_AxxDExxH =  clk_reg.WUVU_AxxDExxH;
  sig.TALU_xBCDExxx =  clk_reg.VENA_xBCDExxx;
  sig.SONO_AxxxxFGH = !clk_reg.VENA_xBCDExxx;
  sig.XOCE_ABxxEFxx = !clk_reg.WOSU_xxCDxxGH;

  if (!rst_sig2.VID_RESETn) {
    sig.WUVU_AxxDExxH = 0;
    sig.VENA_xBCDExxx = 0;
    sig.WOSU_xxCDxxGH = 0;
  }

  return sig;
}

//-----------------------------------------------------------------------------

void ClockRegisters2::pwron() {
  WUVU_AxxDExxH.val = 0; WUVU_AxxDExxH.clk = 0;
  VENA_xBCDExxx.val = 0; VENA_xBCDExxx.clk = 0;
  WOSU_xxCDxxGH.val = 0; WOSU_xxCDxxGH.clk = 0;
}

void ClockRegisters2::reset() {
  WUVU_AxxDExxH.val = 1; WUVU_AxxDExxH.clk = 1;
  VENA_xBCDExxx.val = 0; VENA_xBCDExxx.clk = 0;
  WOSU_xxCDxxGH.val = 1; WOSU_xxCDxxGH.clk = 0;
}

void ClockRegisters2::check_phase(int phase) const {
  check_phase_name(phase, WUVU_AxxDExxH, "WUVU_AxxDExxH");
  check_phase_name(phase, VENA_xBCDExxx, "VENA_xBCDExxx");
  check_phase_name(phase, WOSU_xxCDxxGH, "WOSU_xxCDxxGH");
}

void ClockRegisters2::check_match(const ClockRegisters2& a, const ClockRegisters2& b) {
  check(a.WUVU_AxxDExxH.val == b.WUVU_AxxDExxH.val);
  check(a.VENA_xBCDExxx.val == b.VENA_xBCDExxx.val);
  check(a.WOSU_xxCDxxGH.val == b.WOSU_xxCDxxGH.val);
}

//----------------------------------------

void ClockRegisters2::tock_slow(const SystemRegisters& /*sys_reg*/,
                                const ClockSignals1& clk_sig1,
                                const ClockSignals2& clk_sig2,
                                const ResetSignals2& rst_sig2) {
  /*p29.WUVU*/ WUVU_AxxDExxH.tock( clk_sig1.XOTA_xBxDxFxH, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
  /*p21.VENA*/ VENA_xBCDExxx.tock(!clk_sig2.WUVU_AxxDExxH, rst_sig2.VID_RESETn, !clk_sig2.VENA_xBCDExxx);
  /*p29.WOSU*/ WOSU_xxCDxxGH.tock( clk_sig1.XYFY_AxCxExGx, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
}

//----------------------------------------

void ClockRegisters2::tock_fast(const SystemRegisters& sys_reg,
                                const ResetSignals2& rst_sig2) {
  wire CLK = sys_reg.clk();
  
  /*p01.AVET*/ bool AVET_xBxDxFxH = not(CLK);
  /*p01.ATAL*/ bool ATAL_AxCxExGx = not(AVET_xBxDxFxH);
  /*p01.AZOF*/ bool AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
  /*p01.ZAXY*/ bool ZAXY_AxCxExGx = not(AZOF_xBxDxFxH);
  /*p01.ZEME*/ bool ZEME_xBxDxFxH = not(ZAXY_AxCxExGx);
  /*p29.XYVA*/ bool XYVA_AxCxExGx = not(ZEME_xBxDxFxH);
  /*p29.XOTA*/ bool XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
  /*p29.XYFY*/ bool XYFY_AxCxExGx = not(XOTA_xBxDxFxH);

  /*p21.VENA*/ VENA_xBCDExxx.tock(!WUVU_AxxDExxH, rst_sig2.VID_RESETn, !VENA_xBCDExxx);
  /*p29.WUVU*/ WUVU_AxxDExxH.tock(XOTA_xBxDxFxH,  rst_sig2.VID_RESETn, !WUVU_AxxDExxH);
  /*p29.WOSU*/ WOSU_xxCDxxGH.tock(XYFY_AxCxExGx,  rst_sig2.VID_RESETn, !WUVU_AxxDExxH);
}

//-----------------------------------------------------------------------------

};