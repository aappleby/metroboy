#include "Sch_Clocks.h"

#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Resets.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void ClockSignals1::reset() {
  // ungated
  /*p01.ARYS*/ ARYS_AxCxExGx = 0;
  /*p01.ANOS*/ ANOS_AxCxExGx = 0;
  /*p01.AVET*/ AVET_xBxDxFxH = 1;
  /*p01.ATAL*/ ATAL_AxCxExGx = 0;
  /*p01.AZOF*/ AZOF_xBxDxFxH = 1;
  /*p01.ZAXY*/ ZAXY_AxCxExGx = 0;
  /*p01.ZEME*/ ZEME_xBxDxFxH = 1;
  /*p01.ALET*/ ALET_AxCxExGx = 0;
  /*p27.MYVO*/ MYVO_xBxDxFxH = 1;
  /*p29.XYVA*/ XYVA_AxCxExGx = 0;
  /*p29.XOTA*/ XOTA_xBxDxFxH = 1;
  /*p27.MOXE*/ MOXE_xBxDxFxH = 1;
  /*p27.MEHE*/ MEHE_xBxDxFxH = 1;
  /*p01.LAPE*/ LAPE_xBxDxFxH = 1;
  /*p27.TAVA*/ TAVA_AxCxExGx = 0;
  /*p29.XYFY*/ XYFY_AxCxExGx = 0;

  // gated on MODE_PROD
  /*p01.AFUR*/ PHAZ_ABCDxxxx = 0;
  /*p01.ALEF*/ PHAZ_xBCDExxx = 0;
  /*p01.APUK*/ PHAZ_xxCDEFxx = 0;
  /*p01.ADYK*/ PHAZ_xxxDEFGx = 0;
  /*p01.AFEP*/ AFEP_AxxxxFGH = 1;
  /*p01.ATYP*/ ATYP_ABCDxxxx = 0;
  /*p01.ADAR*/ ADAR_ABCxxxxH = 1;
  /*p01.AROV*/ AROV_xxCDEFxx = 0;
  /*p01.AFAS*/ AFAS_xxxxEFGx = 0;

  // gated on CPUCLK_REQ
  /*p01.NULE*/ NULE_xxxxEFGH = 1;
  /*p01.BYRY*/ BYRY_ABCDxxxx = 0;
  /*p01.BUDE*/ BUDE_xxxxEFGH = 1;
  /*p01.DOVA*/ DOVA_ABCDxxxx = 0;
  /*p01.UVYT*/ UVYT_ABCDxxxx = 0;
  /*p01.BEKO*/ BEKO_ABCDxxxx = 0;
  /*p04.MOPA*/ MOPA_xxxxEFGH = 1;

  /*p01.BAPY*/ BAPY_xxxxxxGH = 1;
  /*p01.BERU*/ BERU_ABCDEFxx = 0;
  /*p01.BUFA*/ BUFA_xxxxxxGH = 1;
  /*p01.BOLO*/ BOLO_ABCDEFxx = 0;

  /*p01.BEJA*/ BEJA_xxxxEFGH = 1;
  /*p01.BANE*/ BANE_ABCDxxxx = 0;
  /*p01.BELO*/ BELO_xxxxEFGH = 1;
  /*p01.BAZE*/ BAZE_ABCDxxxx = 0;
  /*p01.BUTO*/ BUTO_xBCDEFGH = 1;
  /*p01.BELE*/ BELE_Axxxxxxx = 0;

  // gated on CLK_GOOD
  /*p01.BYJU*/ BYJU_xBCDEFGH = 1;
  /*p01.BALY*/ BALY_Axxxxxxx = 0;
  /*p01.BOGA*/ BOGA_xBCDEFGH = 1;
  /*p01.BUVU*/ BUVU_Axxxxxxx = 0;
  /*p01.BYXO*/ BYXO_xBCDEFGH = 1;
  /*p01.BEDO*/ BEDO_Axxxxxxx = 0;
  /*p01.BOWA*/ BOWA_xBCDEFGH = 1;
}

void ClockSignals1::check_phase(int phase) {
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

//-----------------------------------------------------------------------------

void ClockSignals2::reset() {
  /*p29.WUVU*/ WUVU_AxxDExxH = 1;
  /*p21.VENA*/ VENA_xBCDExxx = 0;
  /*p29.WOSU*/ WOSU_xxCDxxGH = 1;
  /*p29.XUPY*/ XUPY_xBCxxFGx = 0;
  /*p28.AWOH*/ AWOH_AxxDExxH = 1;
  /*p21.TALU*/ TALU_xBCDExxx = 0;
  /*p21.SONO*/ SONO_AxxxxFGH = 1;
  /*p29.XOCE*/ XOCE_ABxxEFxx = 0;
}

void ClockSignals2::check_phase(int phase) {
  check_phase_name(phase, WUVU_AxxDExxH, "WUVU_AxxDExxH");
  check_phase_name(phase, VENA_xBCDExxx, "VENA_xBCDExxx");
  check_phase_name(phase, WOSU_xxCDxxGH, "WOSU_xxCDxxGH");
  check_phase_name(phase, XUPY_xBCxxFGx, "XUPY_xBCxxFGx");
  check_phase_name(phase, AWOH_AxxDExxH, "AWOH_AxxDExxH");
  check_phase_name(phase, TALU_xBCDExxx, "TALU_xBCDExxx");
  check_phase_name(phase, SONO_AxxxxFGH, "SONO_AxxxxFGH");
  check_phase_name(phase, XOCE_ABxxEFxx, "XOCE_ABxxEFxx");
}

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

void ClockSignals2::tick_fast(const SystemSignals& /*sys_sig*/, const ResetSignals2& rst_sig2, const ClockRegisters2& clk_reg) {
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

//-----------------------------------------------------------------------------

void ClockRegisters1::tock_slow(const SystemSignals& sys_sig,
                                const ClockSignals1& sig1) {
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ PHAZ_ABCDxxxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD, !sig1.PHAZ_xxxDEFGx);
  /*p01.ALEF*/ PHAZ_xBCDExxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_ABCDxxxx);
  /*p01.APUK*/ PHAZ_xxCDEFxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_xBCDExxx);
  /*p01.ADYK*/ PHAZ_xxxDEFGx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_xxCDEFxx);
}

//----------------------------------------


void ClockRegisters1::tock_fast(const SystemSignals& sys_sig,
                                const ClockSignals1& sig1) {

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ PHAZ_ABCDxxxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD, !sig1.PHAZ_xxxDEFGx);
  /*p01.ALEF*/ PHAZ_xBCDExxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_ABCDxxxx);
  /*p01.APUK*/ PHAZ_xxCDEFxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_xBCDExxx);
  /*p01.ADYK*/ PHAZ_xxxDEFGx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_xxCDEFxx);
}

//-----------------------------------------------------------------------------

void ClockRegisters2::tock_slow(const SystemSignals& /*sys_sig*/,
                                const ClockSignals1& clk_sig1,
                                const ClockSignals2& clk_sig2,
                                const ResetSignals2& rst_sig2) {
  /*p29.WUVU*/ WUVU_AxxDExxH.tock( clk_sig1.XOTA_xBxDxFxH, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
  /*p21.VENA*/ VENA_xBCDExxx.tock(!clk_sig2.WUVU_AxxDExxH, rst_sig2.VID_RESETn, !clk_sig2.VENA_xBCDExxx);
  /*p29.WOSU*/ WOSU_xxCDxxGH.tock( clk_sig1.XYFY_AxCxExGx, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
}

//----------------------------------------

void ClockRegisters2::tock_fast(const SystemSignals& /*sys_sig*/,
                                const ClockSignals1& clk_sig1,
                                const ClockSignals2& clk_sig2,
                                const ResetSignals2& rst_sig2) {

  /*p29.WUVU*/ WUVU_AxxDExxH.tock( clk_sig1.XOTA_xBxDxFxH, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
  /*p21.VENA*/ VENA_xBCDExxx.tock(!clk_sig2.WUVU_AxxDExxH, rst_sig2.VID_RESETn, !clk_sig2.VENA_xBCDExxx);
  /*p29.WOSU*/ WOSU_xxCDxxGH.tock( clk_sig1.XYFY_AxCxExGx, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
}

//-----------------------------------------------------------------------------

};