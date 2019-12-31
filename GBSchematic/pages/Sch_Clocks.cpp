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

void ClockSignals1::tick_slow(const SystemSignals& sys_sig,
                              const ClockRegisters1& clk_reg) {
  wire CLK = sys_sig.clk();

  /*p01.ARYS*/ ARYS_AxCxExGx = CLK;
  /*p01.ANOS*/ ANOS_AxCxExGx = CLK;
  /*p01.AVET*/ AVET_xBxDxFxH = not(CLK);
  /*p01.ATAL*/ ATAL_AxCxExGx = not(AVET_xBxDxFxH);
  /*p01.AZOF*/ AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
  /*p01.ZAXY*/ ZAXY_AxCxExGx = not(AZOF_xBxDxFxH);
  /*p01.ZEME*/ ZEME_xBxDxFxH = not(ZAXY_AxCxExGx);
  /*p01.ALET*/ ALET_AxCxExGx = not(ZEME_xBxDxFxH);
  /*p27.MYVO*/ MYVO_xBxDxFxH = not(ALET_AxCxExGx);
  /*p29.XYVA*/ XYVA_AxCxExGx = not(ZEME_xBxDxFxH);
  /*p29.XOTA*/ XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
  /*p27.MOXE*/ MOXE_xBxDxFxH = not(ALET_AxCxExGx);
  /*p27.MEHE*/ MEHE_xBxDxFxH = not(ALET_AxCxExGx);
  /*p01.LAPE*/ LAPE_xBxDxFxH = not(ALET_AxCxExGx);
  /*p27.TAVA*/ TAVA_AxCxExGx = not(LAPE_xBxDxFxH);
  /*p29.XYFY*/ XYFY_AxCxExGx = not(XOTA_xBxDxFxH);

  // gated on MODE_PROD
  /*p01.AFUR*/ PHAZ_ABCDxxxx = and(clk_reg.PHAZ_ABCDxxxx, sys_sig.MODE_PROD);
  /*p01.ALEF*/ PHAZ_xBCDExxx = and(clk_reg.PHAZ_xBCDExxx, sys_sig.MODE_PROD);
  /*p01.APUK*/ PHAZ_xxCDEFxx = and(clk_reg.PHAZ_xxCDEFxx, sys_sig.MODE_PROD);
  /*p01.ADYK*/ PHAZ_xxxDEFGx = and(clk_reg.PHAZ_xxxDEFGx, sys_sig.MODE_PROD);

  /*p01.AFEP*/ AFEP_AxxxxFGH = not( PHAZ_xBCDExxx);
  /*p01.ATYP*/ ATYP_ABCDxxxx = not(!PHAZ_ABCDxxxx);
  /*p01.ADAR*/ ADAR_ABCxxxxH = not( PHAZ_xxxDEFGx);
  /*p01.AROV*/ AROV_xxCDEFxx = not(!PHAZ_xxCDEFxx);
  /*p01.AFAS*/ AFAS_xxxxEFGx = nor(ADAR_ABCxxxxH, ATYP_ABCDxxxx);

  // gated on CPUCLK_REQ
  /*p01.NULE*/ NULE_xxxxEFGH = nor(sys_sig.CPUCLK_REQn, ATYP_ABCDxxxx);
  /*p01.BYRY*/ BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
  /*p01.BUDE*/ BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
  /*p01.DOVA*/ DOVA_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.UVYT*/ UVYT_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.BEKO*/ BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p04.MOPA*/ MOPA_xxxxEFGH = not(UVYT_ABCDxxxx);

  /*p01.BAPY*/ BAPY_xxxxxxGH = nor(sys_sig.CPUCLK_REQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*p01.BERU*/ BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
  /*p01.BUFA*/ BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
  /*p01.BOLO*/ BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);
  /*p01.BEJA*/ BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
  /*p01.BANE*/ BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
  /*p01.BELO*/ BELO_xxxxEFGH = not(BANE_ABCDxxxx);
  /*p01.BAZE*/ BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
  /*p01.BUTO*/ BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*p01.BELE*/ BELE_Axxxxxxx = not(BUTO_xBCDEFGH);

  // gated on CLK_GOOD
  /*p01.BYJU*/ BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, sys_sig.CLK_BAD2);
  /*p01.BALY*/ BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
  /*p01.BOGA*/ BOGA_xBCDEFGH = not(BALY_Axxxxxxx);
  /*p01.BUVU*/ BUVU_Axxxxxxx = and(sys_sig.CPUCLK_REQ, BALY_Axxxxxxx);
  /*p01.BYXO*/ BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
  /*p01.BEDO*/ BEDO_Axxxxxxx = not(BYXO_xBCDEFGH);
  /*p01.BOWA*/ BOWA_xBCDEFGH = not(BEDO_Axxxxxxx);
}

//----------------------------------------

void ClockSignals1::tick_fast(const SystemSignals& sys_sig, const ClockRegisters1& clk_reg) {
  wire CLK = sys_sig.clk();

  /*p01.ARYS*/ ARYS_AxCxExGx = CLK;
  /*p01.ANOS*/ ANOS_AxCxExGx = CLK;
  /*p01.ATAL*/ ATAL_AxCxExGx = CLK;
  /*p01.ZAXY*/ ZAXY_AxCxExGx = CLK;
  /*p01.ALET*/ ALET_AxCxExGx = CLK;
  /*p29.XYVA*/ XYVA_AxCxExGx = CLK;
  /*p27.TAVA*/ TAVA_AxCxExGx = CLK;
  /*p29.XYFY*/ XYFY_AxCxExGx = CLK;

  /*p01.AVET*/ AVET_xBxDxFxH = !CLK;
  /*p01.AZOF*/ AZOF_xBxDxFxH = !CLK;
  /*p01.ZEME*/ ZEME_xBxDxFxH = !CLK;
  /*p27.MYVO*/ MYVO_xBxDxFxH = !CLK;
  /*p29.XOTA*/ XOTA_xBxDxFxH = !CLK;
  /*p27.MOXE*/ MOXE_xBxDxFxH = !CLK;
  /*p27.MEHE*/ MEHE_xBxDxFxH = !CLK;
  /*p01.LAPE*/ LAPE_xBxDxFxH = !CLK;

  /*p01.AFUR*/ PHAZ_ABCDxxxx = and(clk_reg.PHAZ_ABCDxxxx, sys_sig.MODE_PROD);
  /*p01.ALEF*/ PHAZ_xBCDExxx = and(clk_reg.PHAZ_xBCDExxx, sys_sig.MODE_PROD);
  /*p01.APUK*/ PHAZ_xxCDEFxx = and(clk_reg.PHAZ_xxCDEFxx, sys_sig.MODE_PROD);
  /*p01.ADYK*/ PHAZ_xxxDEFGx = and(clk_reg.PHAZ_xxxDEFGx, sys_sig.MODE_PROD);


  /*p01.AFEP*/ AFEP_AxxxxFGH = !PHAZ_xBCDExxx;
  /*p01.ATYP*/ ATYP_ABCDxxxx =  PHAZ_ABCDxxxx;
  /*p01.ADAR*/ ADAR_ABCxxxxH = !PHAZ_xxxDEFGx;
  /*p01.AROV*/ AROV_xxCDEFxx =  PHAZ_xxCDEFxx;
  /*p01.AFAS*/ AFAS_xxxxEFGx = nor(!PHAZ_xxxDEFGx, PHAZ_ABCDxxxx);

  // gated on CPUCLK_REQ
  /*p01.NULE*/ NULE_xxxxEFGH = nor(sys_sig.CPUCLK_REQn, ATYP_ABCDxxxx);
  /*p01.BYRY*/ BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
  /*p01.BUDE*/ BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
  /*p01.DOVA*/ DOVA_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.UVYT*/ UVYT_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.BEKO*/ BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p04.MOPA*/ MOPA_xxxxEFGH = not(UVYT_ABCDxxxx);

  /*p01.BAPY*/ BAPY_xxxxxxGH = nor(sys_sig.CPUCLK_REQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*p01.BERU*/ BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
  /*p01.BUFA*/ BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
  /*p01.BOLO*/ BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);
  /*p01.BEJA*/ BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
  /*p01.BANE*/ BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
  /*p01.BELO*/ BELO_xxxxEFGH = not(BANE_ABCDxxxx);
  /*p01.BAZE*/ BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
  /*p01.BUTO*/ BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*p01.BELE*/ BELE_Axxxxxxx = not(BUTO_xBCDEFGH);

  // gated on CLK_GOOD
  /*p01.BYJU*/ BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, sys_sig.CLK_BAD2);
  /*p01.BALY*/ BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
  /*p01.BOGA*/ BOGA_xBCDEFGH = not(BALY_Axxxxxxx);
  /*p01.BUVU*/ BUVU_Axxxxxxx = and(sys_sig.CPUCLK_REQ, BALY_Axxxxxxx);
  /*p01.BYXO*/ BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
  /*p01.BEDO*/ BEDO_Axxxxxxx = not(BYXO_xBCDEFGH);
  /*p01.BOWA*/ BOWA_xBCDEFGH = not(BEDO_Axxxxxxx);
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

void ClockRegisters1::tock_slow(const SystemSignals& sys_sig) {
  ClockRegisters1 prev = *this;
  ClockRegisters1& next = *this;

  wire CLK = sys_sig.clk();

  /*p01.AVET*/ wire AVET_xBxDxFxH = not(CLK);
  /*p01.ATAL*/ wire ATAL_AxCxExGx = not(AVET_xBxDxFxH);

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ next.PHAZ_ABCDxxxx.duotock(ATAL_AxCxExGx, sys_sig.MODE_PROD, !prev.PHAZ_xxxDEFGx);
  /*p01.ALEF*/ next.PHAZ_xBCDExxx.duotock(ATAL_AxCxExGx, sys_sig.MODE_PROD,  prev.PHAZ_ABCDxxxx);
  /*p01.APUK*/ next.PHAZ_xxCDEFxx.duotock(ATAL_AxCxExGx, sys_sig.MODE_PROD,  prev.PHAZ_xBCDExxx);
  /*p01.ADYK*/ next.PHAZ_xxxDEFGx.duotock(ATAL_AxCxExGx, sys_sig.MODE_PROD,  prev.PHAZ_xxCDEFxx);
}

//----------------------------------------

void ClockRegisters1::tock_fast(const SystemSignals& sys_sig) {
  int phase = sys_sig.phaseC();

  PHAZ_ABCDxxxx.val = (phase == 0) || (phase == 1) || (phase == 2) || (phase == 3);
  PHAZ_xBCDExxx.val = (phase == 1) || (phase == 2) || (phase == 3) || (phase == 4);
  PHAZ_xxCDEFxx.val = (phase == 2) || (phase == 3) || (phase == 4) || (phase == 5);
  PHAZ_xxxDEFGx.val = (phase == 3) || (phase == 4) || (phase == 5) || (phase == 6);

  if (!sys_sig.MODE_PROD) {
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

void ClockSignals2::tick_slow(const SystemSignals& /*sys_sig*/, const ResetSignals2& rst_sig2, const ClockRegisters2& clk_reg) {
  // gated on VID_RESETn
  /*p29.WUVU*/ WUVU_AxxDExxH = and(clk_reg.WUVU_AxxDExxH, rst_sig2.VID_RESETn);
  /*p21.VENA*/ VENA_xBCDExxx = and(clk_reg.VENA_xBCDExxx, rst_sig2.VID_RESETn);
  /*p29.WOSU*/ WOSU_xxCDxxGH = and(clk_reg.WOSU_xxCDxxGH, rst_sig2.VID_RESETn);
  /*p29.XUPY*/ XUPY_xBCxxFGx = not(WUVU_AxxDExxH);
  /*p28.AWOH*/ AWOH_AxxDExxH = not(XUPY_xBCxxFGx);
  /*p21.TALU*/ TALU_xBCDExxx = not(!VENA_xBCDExxx);
  /*p21.SONO*/ SONO_AxxxxFGH = not(TALU_xBCDExxx);
  /*p29.XOCE*/ XOCE_ABxxEFxx = not(WOSU_xxCDxxGH);
}

//----------------------------------------

void ClockSignals2::tick_fast(const SystemSignals& /*sys_sig*/, const ResetSignals2& rst_sig2, const ClockRegisters2& clk_reg) {
  // gated on VID_RESETn
  WUVU_AxxDExxH =  clk_reg.WUVU_AxxDExxH;
  VENA_xBCDExxx =  clk_reg.VENA_xBCDExxx;
  WOSU_xxCDxxGH =  clk_reg.WOSU_xxCDxxGH;
  XUPY_xBCxxFGx = !clk_reg.WUVU_AxxDExxH;
  AWOH_AxxDExxH =  clk_reg.WUVU_AxxDExxH;
  TALU_xBCDExxx =  clk_reg.VENA_xBCDExxx;
  SONO_AxxxxFGH = !clk_reg.VENA_xBCDExxx;
  XOCE_ABxxEFxx = !clk_reg.WOSU_xxCDxxGH;

  if (!rst_sig2.VID_RESETn) {
    WUVU_AxxDExxH = 0;
    VENA_xBCDExxx = 0;
    WOSU_xxCDxxGH = 0;
  }
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

void ClockRegisters2::tock_slow(const SystemSignals& /*sys_sig*/,
                                const ClockSignals1& clk_sig1,
                                const ClockSignals2& clk_sig2,
                                const ResetSignals2& rst_sig2) {
  /*p29.WUVU*/ WUVU_AxxDExxH.tock( clk_sig1.XOTA_xBxDxFxH, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
  /*p21.VENA*/ VENA_xBCDExxx.tock(!clk_sig2.WUVU_AxxDExxH, rst_sig2.VID_RESETn, !clk_sig2.VENA_xBCDExxx);
  /*p29.WOSU*/ WOSU_xxCDxxGH.tock( clk_sig1.XYFY_AxCxExGx, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
}

//----------------------------------------

void ClockRegisters2::tock_fast(const SystemSignals& sys_sig,
                                const ResetSignals2& rst_sig2) {
  wire CLK = sys_sig.clk();
  
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