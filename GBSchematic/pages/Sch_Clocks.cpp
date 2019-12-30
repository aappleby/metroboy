#include "Sch_Clocks.h"

#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Resets.h"

namespace Schematics {

//-----------------------------------------------------------------------------

ClockSignals1 ClockSignals1::tick_slow(const SystemSignals& sys_sig,
                                       const ClockRegisters& clk_reg) {
  wire CLK = sys_sig.clk();

  ClockSignals1 sig = {
    /*p01.ARYS*/ .ARYS_AxCxExGx = CLK,
    /*p01.ANOS*/ .ANOS_AxCxExGx = CLK,
    /*p01.AVET*/ .AVET_xBxDxFxH = not(CLK),
    /*p01.ATAL*/ .ATAL_AxCxExGx = not(sig.AVET_xBxDxFxH),
    /*p01.AZOF*/ .AZOF_xBxDxFxH = not(sig.ATAL_AxCxExGx),
    /*p01.ZAXY*/ .ZAXY_AxCxExGx = not(sig.AZOF_xBxDxFxH),
    /*p01.ZEME*/ .ZEME_xBxDxFxH = not(sig.ZAXY_AxCxExGx),
    /*p01.ALET*/ .ALET_AxCxExGx = not(sig.ZEME_xBxDxFxH),
    /*p27.MYVO*/ .MYVO_xBxDxFxH = not(sig.ALET_AxCxExGx),
    /*p29.XYVA*/ .XYVA_AxCxExGx = not(sig.ZEME_xBxDxFxH),
    /*p29.XOTA*/ .XOTA_xBxDxFxH = not(sig.XYVA_AxCxExGx),
    /*p27.MOXE*/ .MOXE_xBxDxFxH = not(sig.ALET_AxCxExGx),
    /*p27.MEHE*/ .MEHE_xBxDxFxH = not(sig.ALET_AxCxExGx),
    /*p01.LAPE*/ .LAPE_xBxDxFxH = not(sig.ALET_AxCxExGx),
    /*p27.TAVA*/ .TAVA_AxCxExGx = not(sig.LAPE_xBxDxFxH),
    /*p29.XYFY*/ .XYFY_AxCxExGx = not(sig.XOTA_xBxDxFxH),

    // gated on MODE_PROD
    /*p01.AFUR*/ .PHAZ_ABCDxxxx = and(clk_reg.PHAZ_ABCDxxxx, sys_sig.MODE_PROD),
    /*p01.ALEF*/ .PHAZ_xBCDExxx = and(clk_reg.PHAZ_xBCDExxx, sys_sig.MODE_PROD),
    /*p01.APUK*/ .PHAZ_xxCDEFxx = and(clk_reg.PHAZ_xxCDEFxx, sys_sig.MODE_PROD),
    /*p01.ADYK*/ .PHAZ_xxxDEFGx = and(clk_reg.PHAZ_xxxDEFGx, sys_sig.MODE_PROD),

    /*p01.AFEP*/ .AFEP_AxxxxFGH = not( sig.PHAZ_xBCDExxx),
    /*p01.ATYP*/ .ATYP_ABCDxxxx = not(!sig.PHAZ_ABCDxxxx),
    /*p01.ADAR*/ .ADAR_ABCxxxxH = not( sig.PHAZ_xxxDEFGx),
    /*p01.AROV*/ .AROV_xxCDEFxx = not(!sig.PHAZ_xxCDEFxx),
    /*p01.AFAS*/ .AFAS_xxxxEFGx = nor(sig.ADAR_ABCxxxxH, sig.ATYP_ABCDxxxx),

    // gated on CPUCLK_REQ
    /*p01.NULE*/ .NULE_xxxxEFGH = nor(sys_sig.CPUCLK_REQn, sig.ATYP_ABCDxxxx),
    /*p01.BYRY*/ .BYRY_ABCDxxxx = not(sig.NULE_xxxxEFGH),
    /*p01.BUDE*/ .BUDE_xxxxEFGH = not(sig.BYRY_ABCDxxxx),
    /*p01.DOVA*/ .DOVA_ABCDxxxx = not(sig.BUDE_xxxxEFGH),
    /*p01.UVYT*/ .UVYT_ABCDxxxx = not(sig.BUDE_xxxxEFGH),
    /*p01.BEKO*/ .BEKO_ABCDxxxx = not(sig.BUDE_xxxxEFGH),
    /*p04.MOPA*/ .MOPA_xxxxEFGH = not(sig.UVYT_ABCDxxxx),

    /*p01.BAPY*/ .BAPY_xxxxxxGH = nor(sys_sig.CPUCLK_REQn, sig.AROV_xxCDEFxx, sig.ATYP_ABCDxxxx),
    /*p01.BERU*/ .BERU_ABCDEFxx = not(sig.BAPY_xxxxxxGH),
    /*p01.BUFA*/ .BUFA_xxxxxxGH = not(sig.BERU_ABCDEFxx),
    /*p01.BOLO*/ .BOLO_ABCDEFxx = not(sig.BUFA_xxxxxxGH),
    /*p01.BEJA*/ .BEJA_xxxxEFGH = nand(sig.BOLO_ABCDEFxx, sig.BEKO_ABCDxxxx),
    /*p01.BANE*/ .BANE_ABCDxxxx = not(sig.BEJA_xxxxEFGH),
    /*p01.BELO*/ .BELO_xxxxEFGH = not(sig.BANE_ABCDxxxx),
    /*p01.BAZE*/ .BAZE_ABCDxxxx = not(sig.BELO_xxxxEFGH),
    /*p01.BUTO*/ .BUTO_xBCDEFGH = nand(sig.AFEP_AxxxxFGH, sig.ATYP_ABCDxxxx, sig.BAZE_ABCDxxxx),
    /*p01.BELE*/ .BELE_Axxxxxxx = not(sig.BUTO_xBCDEFGH),

    // gated on CLK_GOOD
    /*p01.BYJU*/ .BYJU_xBCDEFGH = nor(sig.BELE_Axxxxxxx, sys_sig.CLK_BAD2),
    /*p01.BALY*/ .BALY_Axxxxxxx = not(sig.BYJU_xBCDEFGH),
    /*p01.BOGA*/ .BOGA_xBCDEFGH = not(sig.BALY_Axxxxxxx),
    /*p01.BUVU*/ .BUVU_Axxxxxxx = and(sys_sig.CPUCLK_REQ, sig.BALY_Axxxxxxx),
    /*p01.BYXO*/ .BYXO_xBCDEFGH = not(sig.BUVU_Axxxxxxx),
    /*p01.BEDO*/ .BEDO_Axxxxxxx = not(sig.BYXO_xBCDEFGH),
    /*p01.BOWA*/ .BOWA_xBCDEFGH = not(sig.BEDO_Axxxxxxx),
  };

  return sig;
}

//----------------------------------------

#pragma warning(disable : 4189)

ClockSignals1 ClockSignals1::tick_fast(const SystemSignals& sys_sig, const ClockRegisters& clk_reg) {
  wire CLK = sys_sig.clk();

  ClockSignals1 sig = {
    /*p01.ARYS*/ .ARYS_AxCxExGx = CLK,
    /*p01.ANOS*/ .ANOS_AxCxExGx = CLK,
    /*p01.AVET*/ .AVET_xBxDxFxH = not(CLK),
    /*p01.ATAL*/ .ATAL_AxCxExGx = not(sig.AVET_xBxDxFxH),
    /*p01.AZOF*/ .AZOF_xBxDxFxH = not(sig.ATAL_AxCxExGx),
    /*p01.ZAXY*/ .ZAXY_AxCxExGx = not(sig.AZOF_xBxDxFxH),
    /*p01.ZEME*/ .ZEME_xBxDxFxH = not(sig.ZAXY_AxCxExGx),
    /*p01.ALET*/ .ALET_AxCxExGx = not(sig.ZEME_xBxDxFxH),
    /*p27.MYVO*/ .MYVO_xBxDxFxH = not(sig.ALET_AxCxExGx),
    /*p29.XYVA*/ .XYVA_AxCxExGx = not(sig.ZEME_xBxDxFxH),
    /*p29.XOTA*/ .XOTA_xBxDxFxH = not(sig.XYVA_AxCxExGx),
    /*p27.MOXE*/ .MOXE_xBxDxFxH = not(sig.ALET_AxCxExGx),
    /*p27.MEHE*/ .MEHE_xBxDxFxH = not(sig.ALET_AxCxExGx),
    /*p01.LAPE*/ .LAPE_xBxDxFxH = not(sig.ALET_AxCxExGx),
    /*p27.TAVA*/ .TAVA_AxCxExGx = not(sig.LAPE_xBxDxFxH),
    /*p29.XYFY*/ .XYFY_AxCxExGx = not(sig.XOTA_xBxDxFxH),

    // gated on MODE_PROD
    /*p01.AFUR*/ .PHAZ_ABCDxxxx = and(clk_reg.PHAZ_ABCDxxxx, sys_sig.MODE_PROD),
    /*p01.ALEF*/ .PHAZ_xBCDExxx = and(clk_reg.PHAZ_xBCDExxx, sys_sig.MODE_PROD),
    /*p01.APUK*/ .PHAZ_xxCDEFxx = and(clk_reg.PHAZ_xxCDEFxx, sys_sig.MODE_PROD),
    /*p01.ADYK*/ .PHAZ_xxxDEFGx = and(clk_reg.PHAZ_xxxDEFGx, sys_sig.MODE_PROD),

    /*p01.AFEP*/ .AFEP_AxxxxFGH = not( sig.PHAZ_xBCDExxx),
    /*p01.ATYP*/ .ATYP_ABCDxxxx = not(!sig.PHAZ_ABCDxxxx),
    /*p01.ADAR*/ .ADAR_ABCxxxxH = not( sig.PHAZ_xxxDEFGx),
    /*p01.AROV*/ .AROV_xxCDEFxx = not(!sig.PHAZ_xxCDEFxx),
    /*p01.AFAS*/ .AFAS_xxxxEFGx = nor(sig.ADAR_ABCxxxxH, sig.ATYP_ABCDxxxx),

    // gated on CPUCLK_REQ
    /*p01.NULE*/ .NULE_xxxxEFGH = nor(sys_sig.CPUCLK_REQn, sig.ATYP_ABCDxxxx),
    /*p01.BYRY*/ .BYRY_ABCDxxxx = not(sig.NULE_xxxxEFGH),
    /*p01.BUDE*/ .BUDE_xxxxEFGH = not(sig.BYRY_ABCDxxxx),
    /*p01.DOVA*/ .DOVA_ABCDxxxx = not(sig.BUDE_xxxxEFGH),
    /*p01.UVYT*/ .UVYT_ABCDxxxx = not(sig.BUDE_xxxxEFGH),
    /*p01.BEKO*/ .BEKO_ABCDxxxx = not(sig.BUDE_xxxxEFGH),
    /*p04.MOPA*/ .MOPA_xxxxEFGH = not(sig.UVYT_ABCDxxxx),

    /*p01.BAPY*/ .BAPY_xxxxxxGH = nor(sys_sig.CPUCLK_REQn, sig.AROV_xxCDEFxx, sig.ATYP_ABCDxxxx),
    /*p01.BERU*/ .BERU_ABCDEFxx = not(sig.BAPY_xxxxxxGH),
    /*p01.BUFA*/ .BUFA_xxxxxxGH = not(sig.BERU_ABCDEFxx),
    /*p01.BOLO*/ .BOLO_ABCDEFxx = not(sig.BUFA_xxxxxxGH),
    /*p01.BEJA*/ .BEJA_xxxxEFGH = nand(sig.BOLO_ABCDEFxx, sig.BEKO_ABCDxxxx),
    /*p01.BANE*/ .BANE_ABCDxxxx = not(sig.BEJA_xxxxEFGH),
    /*p01.BELO*/ .BELO_xxxxEFGH = not(sig.BANE_ABCDxxxx),
    /*p01.BAZE*/ .BAZE_ABCDxxxx = not(sig.BELO_xxxxEFGH),
    /*p01.BUTO*/ .BUTO_xBCDEFGH = nand(sig.AFEP_AxxxxFGH, sig.ATYP_ABCDxxxx, sig.BAZE_ABCDxxxx),
    /*p01.BELE*/ .BELE_Axxxxxxx = not(sig.BUTO_xBCDEFGH),

    // gated on CLK_GOOD
    /*p01.BYJU*/ .BYJU_xBCDEFGH = nor(sig.BELE_Axxxxxxx, sys_sig.CLK_BAD2),
    /*p01.BALY*/ .BALY_Axxxxxxx = not(sig.BYJU_xBCDEFGH),
    /*p01.BOGA*/ .BOGA_xBCDEFGH = not(sig.BALY_Axxxxxxx),
    /*p01.BUVU*/ .BUVU_Axxxxxxx = and(sys_sig.CPUCLK_REQ, sig.BALY_Axxxxxxx),
    /*p01.BYXO*/ .BYXO_xBCDEFGH = not(sig.BUVU_Axxxxxxx),
    /*p01.BEDO*/ .BEDO_Axxxxxxx = not(sig.BYXO_xBCDEFGH),
    /*p01.BOWA*/ .BOWA_xBCDEFGH = not(sig.BEDO_Axxxxxxx),
  };

  //----------

#if 0
  int phase = sys_sig.phaseC();
  bool xBxDxFxH = (phase & 1);
  bool AxCxExGx = !xBxDxFxH;

  /*p01.AVET*/ sig.AVET_xBxDxFxH = xBxDxFxH;
  /*p01.AZOF*/ sig.AZOF_xBxDxFxH = xBxDxFxH;
  /*p01.ZEME*/ sig.ZEME_xBxDxFxH = xBxDxFxH;
  /*p27.MYVO*/ sig.MYVO_xBxDxFxH = xBxDxFxH;
  /*p29.XOTA*/ sig.XOTA_xBxDxFxH = xBxDxFxH;
  /*p27.MOXE*/ sig.MOXE_xBxDxFxH = xBxDxFxH;
  /*p27.MEHE*/ sig.MEHE_xBxDxFxH = xBxDxFxH;
  /*p01.LAPE*/ sig.LAPE_xBxDxFxH = xBxDxFxH;

  /*p01.ARYS*/ sig.ARYS_AxCxExGx = AxCxExGx;
  /*p01.ANOS*/ sig.ANOS_AxCxExGx = AxCxExGx;
  /*p01.ATAL*/ sig.ATAL_AxCxExGx = AxCxExGx;
  /*p01.ZAXY*/ sig.ZAXY_AxCxExGx = AxCxExGx;
  /*p01.ALET*/ sig.ALET_AxCxExGx = AxCxExGx;
  /*p29.XYVA*/ sig.XYVA_AxCxExGx = AxCxExGx;
  /*p27.TAVA*/ sig.TAVA_AxCxExGx = AxCxExGx;
  /*p29.XYFY*/ sig.XYFY_AxCxExGx = AxCxExGx;

  bool ABCDxxxx = (phase == 0) || (phase == 1) || (phase == 2) || (phase == 3);
  bool xBCDExxx = (phase == 1) || (phase == 2) || (phase == 3) || (phase == 4);
  bool xxCDEFxx = (phase == 2) || (phase == 3) || (phase == 4) || (phase == 5);
  bool xxxDEFGx = (phase == 3) || (phase == 4) || (phase == 5) || (phase == 6);
  bool xxxxEFGH = !ABCDxxxx;
  bool AxxxxFGH = !xBCDExxx;
  bool ABCxxxxH = !xxxDEFGx;

  bool ABCDEFxx = ABCDxxxx | xxCDEFxx;
  bool xxxxxxGH = !ABCDEFxx;

  bool Axxxxxxx = (phase == 0);
  bool xBCDEFGH = !Axxxxxxx;

  bool xxxxEFGx = (phase == 4) || (phase == 5) || (phase == 6);

  if (sys_sig.MODE_PROD) {
    sig.PHAZ_ABCDxxxx = ABCDxxxx;
    sig.PHAZ_xBCDExxx = xBCDExxx;
    sig.PHAZ_xxCDEFxx = xxCDEFxx;
    sig.PHAZ_xxxDEFGx = xxxDEFGx;

    sig.AFEP_AxxxxFGH = AxxxxFGH;
    sig.ATYP_ABCDxxxx = ABCDxxxx;
    sig.ADAR_ABCxxxxH = ABCxxxxH;
    sig.AROV_xxCDEFxx = xxCDEFxx;
    sig.AFAS_xxxxEFGx = xxxxEFGx;
    sig.BUTO_xBCDEFGH = xBCDEFGH;
    sig.BELE_Axxxxxxx = Axxxxxxx;
  }
  else {
    sig.PHAZ_ABCDxxxx = 0;
    sig.PHAZ_xBCDExxx = 0;
    sig.PHAZ_xxCDEFxx = 0;
    sig.PHAZ_xxxDEFGx = 0;

    sig.AFEP_AxxxxFGH = 1;
    sig.ATYP_ABCDxxxx = 0;
    sig.ADAR_ABCxxxxH = 1;
    sig.AROV_xxCDEFxx = 0;
    sig.AFAS_xxxxEFGx = 0;
    sig.BUTO_xBCDEFGH = 1;
    sig.BELE_Axxxxxxx = 0;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_REQ) {
    sig.NULE_xxxxEFGH = xxxxEFGH;
    sig.BUDE_xxxxEFGH = xxxxEFGH;
    sig.MOPA_xxxxEFGH = xxxxEFGH;
    sig.BEJA_xxxxEFGH = xxxxEFGH;
    sig.BELO_xxxxEFGH = xxxxEFGH;

    sig.BYRY_ABCDxxxx = ABCDxxxx;
    sig.DOVA_ABCDxxxx = ABCDxxxx;
    sig.UVYT_ABCDxxxx = ABCDxxxx;
    sig.BEKO_ABCDxxxx = ABCDxxxx;
    sig.BANE_ABCDxxxx = ABCDxxxx;
    sig.BAZE_ABCDxxxx = ABCDxxxx;

    sig.BAPY_xxxxxxGH = xxxxxxGH;
    sig.BUFA_xxxxxxGH = xxxxxxGH;
    sig.BERU_ABCDEFxx = ABCDEFxx;
    sig.BOLO_ABCDEFxx = ABCDEFxx;
  }
  else {
    sig.NULE_xxxxEFGH = sys_sig.CLK_REQ;
    sig.BUDE_xxxxEFGH = sys_sig.CLK_REQ;
    sig.MOPA_xxxxEFGH = sys_sig.CLK_REQ;
    sig.BEJA_xxxxEFGH = sys_sig.CLK_REQ;
    sig.BELO_xxxxEFGH = sys_sig.CLK_REQ;

    sig.BYRY_ABCDxxxx = !sys_sig.CLK_REQ;
    sig.DOVA_ABCDxxxx = !sys_sig.CLK_REQ;
    sig.UVYT_ABCDxxxx = !sys_sig.CLK_REQ;
    sig.BEKO_ABCDxxxx = !sys_sig.CLK_REQ;
    sig.BANE_ABCDxxxx = !sys_sig.CLK_REQ;
    sig.BAZE_ABCDxxxx = !sys_sig.CLK_REQ;

    sig.BAPY_xxxxxxGH = 0;
    sig.BUFA_xxxxxxGH = 0;
    sig.BERU_ABCDEFxx = 1;
    sig.BOLO_ABCDEFxx = 1;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_GOOD) {
    sig.BYJU_xBCDEFGH = xBCDEFGH;
    sig.BOGA_xBCDEFGH = xBCDEFGH;
    sig.BALY_Axxxxxxx = Axxxxxxx;
  }
  else {
    sig.BYJU_xBCDEFGH = 0;
    sig.BOGA_xBCDEFGH = 0;
    sig.BALY_Axxxxxxx = 1;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_GOOD && sys_sig.CLK_REQ) {
    sig.BYXO_xBCDEFGH = xBCDEFGH;
    sig.BOWA_xBCDEFGH = xBCDEFGH;
    sig.BUVU_Axxxxxxx = Axxxxxxx;
    sig.BEDO_Axxxxxxx = Axxxxxxx;
  }
  else {
    sig.BUVU_Axxxxxxx = sys_sig.CLK_REQ;
    sig.BYXO_xBCDEFGH = !sys_sig.CLK_REQ;
    sig.BOWA_xBCDEFGH = !sys_sig.CLK_REQ;
    sig.BEDO_Axxxxxxx = sys_sig.CLK_REQ;
  }
#endif

  return sig;
}

//-----------------------------------------------------------------------------

ClockSignals2 ClockSignals2::tick_slow(const ResetSignals2& rst_sig2, const ClockRegisters& clk_reg) {
  ClockSignals2 sig = {
    // gated on VID_RESETn
    /*p29.WUVU*/ .WUVU_AxxDExxH = and(clk_reg.WUVU_AxxDExxH, rst_sig2.VID_RESETn),
    /*p21.VENA*/ .VENA_xBCDExxx = and(clk_reg.VENA_xBCDExxx, rst_sig2.VID_RESETn),
    /*p29.WOSU*/ .WOSU_xxCDxxGH = and(clk_reg.WOSU_xxCDxxGH, rst_sig2.VID_RESETn),
    /*p29.XUPY*/ .XUPY_xBCxxFGx = not(sig.WUVU_AxxDExxH),
    /*p28.AWOH*/ .AWOH_AxxDExxH = not(sig.XUPY_xBCxxFGx),
    /*p21.TALU*/ .TALU_xBCDExxx = not(!sig.VENA_xBCDExxx),
    /*p21.SONO*/ .SONO_AxxxxFGH = not(sig.TALU_xBCDExxx),
    /*p29.XOCE*/ .XOCE_ABxxEFxx = not(sig.WOSU_xxCDxxGH),
  };

  return sig;
}

//----------------------------------------

ClockSignals2 ClockSignals2::tick_fast(const SystemSignals& /*sys_sig*/, const ResetSignals2& rst_sig2, const ClockRegisters& clk_reg) {
  ClockSignals2 sig = {
    // gated on VID_RESETn
    /*p29.WUVU*/ .WUVU_AxxDExxH = and(clk_reg.WUVU_AxxDExxH, rst_sig2.VID_RESETn),
    /*p21.VENA*/ .VENA_xBCDExxx = and(clk_reg.VENA_xBCDExxx, rst_sig2.VID_RESETn),
    /*p29.WOSU*/ .WOSU_xxCDxxGH = and(clk_reg.WOSU_xxCDxxGH, rst_sig2.VID_RESETn),
    /*p29.XUPY*/ .XUPY_xBCxxFGx = not(sig.WUVU_AxxDExxH),
    /*p28.AWOH*/ .AWOH_AxxDExxH = not(sig.XUPY_xBCxxFGx),
    /*p21.TALU*/ .TALU_xBCDExxx = not(!sig.VENA_xBCDExxx),
    /*p21.SONO*/ .SONO_AxxxxFGH = not(sig.TALU_xBCDExxx),
    /*p29.XOCE*/ .XOCE_ABxxEFxx = not(sig.WOSU_xxCDxxGH),
  };

  return sig;
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_slow1(const SystemSignals& sys_sig,
                                const ClockSignals1& sig1,
                                ClockRegisters& next) {
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ next.PHAZ_ABCDxxxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD, !sig1.PHAZ_xxxDEFGx);
  /*p01.ALEF*/ next.PHAZ_xBCDExxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_ABCDxxxx);
  /*p01.APUK*/ next.PHAZ_xxCDEFxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_xBCDExxx);
  /*p01.ADYK*/ next.PHAZ_xxxDEFGx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_xxCDEFxx);
}

//----------------------------------------


void ClockRegisters::tock_fast1(const SystemSignals& sys_sig,
                                const ClockSignals1& sig1,
                                ClockRegisters& next) {

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ next.PHAZ_ABCDxxxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD, !sig1.PHAZ_xxxDEFGx);
  /*p01.ALEF*/ next.PHAZ_xBCDExxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_ABCDxxxx);
  /*p01.APUK*/ next.PHAZ_xxCDEFxx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_xBCDExxx);
  /*p01.ADYK*/ next.PHAZ_xxxDEFGx.duotock(sig1.ATAL_AxCxExGx, sys_sig.MODE_PROD,  sig1.PHAZ_xxCDEFxx);

#if 0
  int phase = sys_sig.phaseC();

  bool xBxDxFxH = (phase & 1);

  bool xBCDExxx = (phase == 1) || (phase == 2) || (phase == 3) || (phase == 4);
  bool xxCDEFxx = (phase == 2) || (phase == 3) || (phase == 4) || (phase == 5);
  bool xxxDEFGx = (phase == 3) || (phase == 4) || (phase == 5) || (phase == 6);
  bool xxxxEFGH = (phase == 4) || (phase == 5) || (phase == 6) || (phase == 7);

  if (sys_sig.MODE_PROD) {
    next.PHAZ_ABCDxxxx.val = xBCDExxx;
    next.PHAZ_xBCDExxx.val = xxCDEFxx;
    next.PHAZ_xxCDEFxx.val = xxxDEFGx;
    next.PHAZ_xxxDEFGx.val = xxxxEFGH;
  }
  else {
    next.PHAZ_ABCDxxxx.val = 0;
    next.PHAZ_xBCDExxx.val = 0;
    next.PHAZ_xxCDEFxx.val = 0;
    next.PHAZ_xxxDEFGx.val = 0;
  }

  next.PHAZ_ABCDxxxx.clk = xBxDxFxH;
  next.PHAZ_xBCDExxx.clk = xBxDxFxH;
  next.PHAZ_xxCDEFxx.clk = xBxDxFxH;
  next.PHAZ_xxxDEFGx.clk = xBxDxFxH;
#endif
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_slow2(const SystemSignals& /*sys_sig*/,
                                const ClockSignals1& clk_sig1,
                                const ClockSignals2& clk_sig2,
                                const ResetSignals2& rst_sig2,
                                ClockRegisters& next) {
  /*p29.WUVU*/ next.WUVU_AxxDExxH.tock( clk_sig1.XOTA_xBxDxFxH, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
  /*p21.VENA*/ next.VENA_xBCDExxx.tock(!clk_sig2.WUVU_AxxDExxH, rst_sig2.VID_RESETn, !clk_sig2.VENA_xBCDExxx);
  /*p29.WOSU*/ next.WOSU_xxCDxxGH.tock( clk_sig1.XYFY_AxCxExGx, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
}

//----------------------------------------

void ClockRegisters::tock_fast2(const SystemSignals& /*sys_sig*/,
                                const ClockSignals1& clk_sig1,
                                const ClockSignals2& clk_sig2,
                                const ResetSignals2& rst_sig2,
                                ClockRegisters& next) {

  /*p29.WUVU*/ next.WUVU_AxxDExxH.tock( clk_sig1.XOTA_xBxDxFxH, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);
  /*p21.VENA*/ next.VENA_xBCDExxx.tock(!clk_sig2.WUVU_AxxDExxH, rst_sig2.VID_RESETn, !clk_sig2.VENA_xBCDExxx);
  /*p29.WOSU*/ next.WOSU_xxCDxxGH.tock( clk_sig1.XYFY_AxCxExGx, rst_sig2.VID_RESETn, !clk_sig2.WUVU_AxxDExxH);

#if 0
  int phase = sys_sig.phaseC();

  bool xBxDxFxH = (phase & 1);
  bool AxCxExGx = !xBxDxFxH;

  bool xxxxEFGH = (phase == 4) || (phase == 5) || (phase == 6) || (phase == 7);
  bool xxCDxxGH = (phase == 2) || (phase == 3) || (phase == 6) || (phase == 7);
  bool xBCxxFGx = (phase == 1) || (phase == 2) || (phase == 5) || (phase == 6);
  bool ABxxEFxx = !xxCDxxGH;

  if (rst_sig2.VID_RESETn) {
    next.WUVU_AxxDExxH.val = xxCDxxGH;
    next.WUVU_AxxDExxH.clk = AxCxExGx;

    next.VENA_xBCDExxx.val = xxxxEFGH;
    next.VENA_xBCDExxx.clk = ABxxEFxx;

    next.WOSU_xxCDxxGH.val = xBCxxFGx;
    next.WOSU_xxCDxxGH.clk = xBxDxFxH;
  }
  else {
    next.WUVU_AxxDExxH.val = 0;
    next.WUVU_AxxDExxH.clk = AxCxExGx;

    next.VENA_xBCDExxx.val = 0;
    next.VENA_xBCDExxx.clk = 1;

    next.WOSU_xxCDxxGH.val = 0;
    next.WOSU_xxCDxxGH.clk = xBxDxFxH;
  }
#endif
}

//-----------------------------------------------------------------------------

};