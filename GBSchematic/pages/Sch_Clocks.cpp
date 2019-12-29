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
    /*p01.ARYS*/ .ARYS_xBxDxFxH = not(CLK),
    /*p01.ANOS*/ .ANOS_xBxDxFxH = not(CLK),
    /*p01.AVET*/ .ROOT_AxCxExGx = CLK,
    /*p01.ATAL*/ .ATAL_xBxDxFxH = not(sig.ROOT_AxCxExGx),
    /*p01.AZOF*/ .AZOF_AxCxExGx = not(sig.ATAL_xBxDxFxH),
    /*p01.ZAXY*/ .ZAXY_xBxDxFxH = not(sig.AZOF_AxCxExGx),
    /*p01.ZEME*/ .ZEME_AxCxExGx = not(sig.ZAXY_xBxDxFxH),
    /*p01.ALET*/ .ALET_xBxDxFxH = not(sig.ZEME_AxCxExGx),
    /*p27.MYVO*/ .MYVO_AxCxExGx = not(sig.ALET_xBxDxFxH),
    /*p29.XYVA*/ .XYVA_xBxDxFxH = not(sig.ZEME_AxCxExGx),
    /*p29.XOTA*/ .XOTA_AxCxExGx = not(sig.XYVA_xBxDxFxH),
    /*p27.MOXE*/ .MOXE_AxCxExGx = not(sig.ALET_xBxDxFxH),
    /*p27.MEHE*/ .MEHE_AxCxExGx = not(sig.ALET_xBxDxFxH),
    /*p01.LAPE*/ .LAPE_AxCxExGx = not(sig.ALET_xBxDxFxH),
    /*p27.TAVA*/ .TAVA_xBxDxFxH = not(sig.LAPE_AxCxExGx),
    /*p29.XYFY*/ .XYFY_xBxDxFxH = not(sig.XOTA_AxCxExGx),

    // gated on MODE_PROD
    /*p01.AFUR*/ .PHAZ_ABCDxxxx = and(clk_reg.PHAZ_ABCDxxxx, sys_sig.MODE_PROD),
    /*p01.ALEF*/ .PHAZ_xBCDExxx = and(clk_reg.PHAZ_xBCDExxx, sys_sig.MODE_PROD),
    /*p01.APUK*/ .PHAZ_xxCDEFxx = and(clk_reg.PHAZ_xxCDEFxx, sys_sig.MODE_PROD),
    /*p01.ADYK*/ .PHAZ_xxxDEFGx = and(clk_reg.PHAZ_xxxDEFGx, sys_sig.MODE_PROD),

    /*p01.AFEP*/ .AFEP_AxxxxFGH = not( sig.PHAZ_xBCDExxx),
    /*p01.ATYP*/ .ATYP_ABCDxxxx = not(!sig.PHAZ_ABCDxxxx),
    /*p01.ADAR*/ .ADAR_ABCxxxxH = not( sig.PHAZ_xxxDEFGx),
    /*p01.AROV*/ .AROV_xxCDEFxx = not(!sig.PHAZ_xxCDEFxx),
    /*p01.AFAS*/ .AFAS_xxxxxFGH = nor(sig.ADAR_ABCxxxxH, sig.ATYP_ABCDxxxx),

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

ClockSignals1 ClockSignals1::tick_fast(const SystemSignals& sys_sig) {
  int phase = sys_sig.phase();

  ClockSignals1 sig;

  bool xBxDxFxH = (phase & 1);
  bool AxCxExGx = !xBxDxFxH;

  /*p01.AVET*/ sig.ROOT_AxCxExGx = AxCxExGx;
  /*p01.AZOF*/ sig.AZOF_AxCxExGx = AxCxExGx;
  /*p01.ZEME*/ sig.ZEME_AxCxExGx = AxCxExGx;
  /*p27.MYVO*/ sig.MYVO_AxCxExGx = AxCxExGx;
  /*p29.XOTA*/ sig.XOTA_AxCxExGx = AxCxExGx;
  /*p27.MOXE*/ sig.MOXE_AxCxExGx = AxCxExGx;
  /*p27.MEHE*/ sig.MEHE_AxCxExGx = AxCxExGx;
  /*p01.LAPE*/ sig.LAPE_AxCxExGx = AxCxExGx;

  /*p01.ARYS*/ sig.ARYS_xBxDxFxH = xBxDxFxH;
  /*p01.ANOS*/ sig.ANOS_xBxDxFxH = xBxDxFxH;
  /*p01.ATAL*/ sig.ATAL_xBxDxFxH = xBxDxFxH;
  /*p01.ZAXY*/ sig.ZAXY_xBxDxFxH = xBxDxFxH;
  /*p01.ALET*/ sig.ALET_xBxDxFxH = xBxDxFxH;
  /*p29.XYVA*/ sig.XYVA_xBxDxFxH = xBxDxFxH;
  /*p27.TAVA*/ sig.TAVA_xBxDxFxH = xBxDxFxH;
  /*p29.XYFY*/ sig.XYFY_xBxDxFxH = xBxDxFxH;


  bool xBCDExxx = (phase == 1) || (phase == 2) || (phase == 3) || (phase == 4);
  bool xxCDEFxx = (phase == 2) || (phase == 3) || (phase == 4) || (phase == 5);
  bool xxxDEFGx = (phase == 3) || (phase == 4) || (phase == 5) || (phase == 6);
  bool xxxxEFGH = (phase == 4) || (phase == 5) || (phase == 6) || (phase == 7);
  bool ABCDxxxx = !xxxxEFGH;
  bool xxxxxFGH = (phase == 5) || (phase == 6) || (phase == 7);
  bool ABxxxxGH = !xxCDEFxx;

  bool AxxxxxxH = (phase == 0) || (phase == 7);
  bool xBCDEFGx = !AxxxxxxH;
  bool AxxxxFGH = !xBCDExxx;
  bool xBxxxxxx = (phase == 1);
  bool AxCDEFGH = !xBxxxxxx;

  if (sys_sig.MODE_PROD) {
    sig.PHAZ_ABCDxxxx = xBCDExxx;
    sig.PHAZ_xBCDExxx = xxCDEFxx;
    sig.PHAZ_xxCDEFxx = xxxDEFGx;
    sig.PHAZ_xxxDEFGx = xxxxEFGH;

    sig.AFEP_AxxxxFGH = ABxxxxGH;
    sig.ATYP_ABCDxxxx = xBCDExxx;
    sig.ADAR_ABCxxxxH = ABCDxxxx;
    sig.AROV_xxCDEFxx = xxxDEFGx;
    sig.AFAS_xxxxxFGH = xxxxxFGH;
    sig.BUTO_xBCDEFGH = AxCDEFGH;
    sig.BELE_Axxxxxxx = xBxxxxxx;
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
    sig.AFAS_xxxxxFGH = 0;
    sig.BUTO_xBCDEFGH = 1;
    sig.BELE_Axxxxxxx = 0;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_REQ) {
    sig.NULE_xxxxEFGH = AxxxxFGH;
    sig.BUDE_xxxxEFGH = AxxxxFGH;
    sig.MOPA_xxxxEFGH = AxxxxFGH;
    sig.BEJA_xxxxEFGH = AxxxxFGH;
    sig.BELO_xxxxEFGH = AxxxxFGH;

    sig.BYRY_ABCDxxxx = xBCDExxx;
    sig.DOVA_ABCDxxxx = xBCDExxx;
    sig.UVYT_ABCDxxxx = xBCDExxx;
    sig.BEKO_ABCDxxxx = xBCDExxx;
    sig.BANE_ABCDxxxx = xBCDExxx;
    sig.BAZE_ABCDxxxx = xBCDExxx;

    sig.BAPY_xxxxxxGH = AxxxxxxH;
    sig.BUFA_xxxxxxGH = AxxxxxxH;
    sig.BERU_ABCDEFxx = xBCDEFGx;
    sig.BOLO_ABCDEFxx = xBCDEFGx;
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
    sig.BYJU_xBCDEFGH = AxCDEFGH;
    sig.BOGA_xBCDEFGH = AxCDEFGH;
    sig.BALY_Axxxxxxx = xBxxxxxx;
  }
  else {
    sig.BYJU_xBCDEFGH = 0;
    sig.BOGA_xBCDEFGH = 0;
    sig.BALY_Axxxxxxx = 1;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_GOOD && sys_sig.CLK_REQ) {
    sig.BYXO_xBCDEFGH = AxCDEFGH;
    sig.BOWA_xBCDEFGH = AxCDEFGH;
    sig.BUVU_Axxxxxxx = xBxxxxxx;
    sig.BEDO_Axxxxxxx = xBxxxxxx;
  }
  else {
    sig.BUVU_Axxxxxxx = sys_sig.CLK_REQ;
    sig.BYXO_xBCDEFGH = !sys_sig.CLK_REQ;
    sig.BOWA_xBCDEFGH = !sys_sig.CLK_REQ;
    sig.BEDO_Axxxxxxx = sys_sig.CLK_REQ;
  }

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

#pragma warning(disable : 4189)

ClockSignals2 ClockSignals2::tick_fast(const SystemSignals& sys_sig, const ResetSignals2& rst_sig2, const ClockRegisters& clk_reg) {
  int phase = sys_sig.phase();

  bool xxxxEFGH = (phase == 4) || (phase == 5) || (phase == 6) || (phase == 7);
  bool xxCDxxGH = (phase == 2) || (phase == 3) || (phase == 6) || (phase == 7);
  bool xBCxxFGx = (phase == 1) || (phase == 2) || (phase == 5) || (phase == 6);
  bool ABxxEFxx = !xxCDxxGH;
  bool ABCDxxxx = !xxxxEFGH;
  bool AxxDExxH = !xBCxxFGx;

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

#if 0
  if (rst_sig2.VID_RESETn) {
    sig.XUPY_xBCxxFGx = ABxxEFxx;
    /*
    sig.AWOH_AxxDExxH = xxCDxxGH;
    sig.WUVU_AxxDExxH = xxCDxxGH;

    sig.XOCE_ABxxEFxx = AxxDExxH;
    sig.WOSU_xxCDxxGH = xBCxxFGx;

    sig.SONO_AxxxxFGH = ABCDxxxx;
    sig.VENA_xBCDExxx = xxxxEFGH;
    sig.TALU_xBCDExxx = xxxxEFGH;
    */
  }
  else {
    sig.XUPY_xBCxxFGx = 1;
    /*
    sig.AWOH_AxxDExxH = 0;
    sig.WUVU_AxxDExxH = 0;

    sig.XOCE_ABxxEFxx = 1;
    sig.WOSU_xxCDxxGH = 0;

    sig.SONO_AxxxxFGH = 1;
    sig.VENA_xBCDExxx = 0;
    sig.TALU_xBCDExxx = 0;
    */
  }
#endif


  return sig;
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_slow1(const SystemSignals& sys_sig,
                                const ClockSignals1& sig1,
                                ClockRegisters& next) {
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ next.PHAZ_ABCDxxxx.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD, !sig1.PHAZ_xxxDEFGx);
  /*p01.ALEF*/ next.PHAZ_xBCDExxx.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD,  sig1.PHAZ_ABCDxxxx);
  /*p01.APUK*/ next.PHAZ_xxCDEFxx.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD,  sig1.PHAZ_xBCDExxx);
  /*p01.ADYK*/ next.PHAZ_xxxDEFGx.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD,  sig1.PHAZ_xxCDEFxx);
}

//----------------------------------------

void ClockRegisters::tock_slow2(const SystemSignals& /*sys_sig*/,
                                const ClockSignals1& sig1,
                                const ClockSignals2& sig2,
                                const ResetSignals2& rst_sig2,
                                ClockRegisters& next) {
  /*p29.WUVU*/ next.WUVU_AxxDExxH.tock( sig1.XOTA_AxCxExGx, rst_sig2.VID_RESETn, !sig2.WUVU_AxxDExxH);
  /*p21.VENA*/ next.VENA_xBCDExxx.tock(!sig2.WUVU_AxxDExxH, rst_sig2.VID_RESETn, !sig2.VENA_xBCDExxx);
  /*p29.WOSU*/ next.WOSU_xxCDxxGH.tock( sig1.XYFY_xBxDxFxH, rst_sig2.VID_RESETn, !sig2.WUVU_AxxDExxH);
}

//-----------------------------------------------------------------------------

void ClockRegisters::tock_fast1(const SystemSignals& sys_sig, ClockRegisters& next) {
  int phase = sys_sig.phase();

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
}

//----------------------------------------

void ClockRegisters::tock_fast2(const SystemSignals& sys_sig, const ResetSignals2& rst_sig2, ClockRegisters& next) {

  int phase = sys_sig.phase();

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
}

//-----------------------------------------------------------------------------

};