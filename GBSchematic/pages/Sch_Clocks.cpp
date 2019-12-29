#include "Sch_Clocks.h"

#include "Sch_Pins.h"
#include "Sch_Debug.h"

namespace Schematics {

//-----------------------------------------------------------------------------

ClockSignals1 ClockSignals1::tick_slow(const SystemSignals& sys_sig,
                                       const Clocks& clk,
                                       wire CLK) {
  ClockSignals1 sig = {
    /*p01.ABOL*/ .CPUCLK_REQn   = not(sys_sig.CLK_REQ),
    /*p01.BUTY*/ .CPUCLK_REQ    = not(sig.CPUCLK_REQn),
    /*p01.UCOB*/ .CLK_BAD1      = not(sys_sig.CLK_GOOD),
    /*p01.ATEZ*/ .CLK_BAD2      = not(sys_sig.CLK_GOOD),
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
    /*p01.AFUR*/ .PHAZ_xBCDExxx = and(clk.PHAZ_xBCDExxx, sys_sig.MODE_PROD),
    /*p01.ALEF*/ .PHAZ_xxCDEFxx = and(clk.PHAZ_xxCDEFxx, sys_sig.MODE_PROD),
    /*p01.APUK*/ .PHAZ_xxxDEFGx = and(clk.PHAZ_xxxDEFGx, sys_sig.MODE_PROD),
    /*p01.ADYK*/ .PHAZ_xxxxEFGH = and(clk.PHAZ_xxxxEFGH, sys_sig.MODE_PROD),

    /*p01.AFEP*/ .AFEP_ABxxxxGH = not( sig.PHAZ_xxCDEFxx),
    /*p01.ATYP*/ .ATYP_xBCDExxx = not(!sig.PHAZ_xBCDExxx),
    /*p01.ADAR*/ .ADAR_ABCxxxxH = not( sig.PHAZ_xxxxEFGH),
    /*p01.AROV*/ .AROV_xxxDEFGx = not(!sig.PHAZ_xxxDEFGx),
    /*p01.AFAS*/ .AFAS_xxxxxFGH = nor(sig.ADAR_ABCxxxxH, sig.ATYP_xBCDExxx),

    // gated on CPUCLK_REQ
    /*p01.NULE*/ .NULE_xxxxEFGH = nor(sig.CPUCLK_REQn, sig.ATYP_xBCDExxx),
    /*p01.BYRY*/ .BYRY_ABCDxxxx = not(sig.NULE_xxxxEFGH),
    /*p01.BUDE*/ .BUDE_xxxxEFGH = not(sig.BYRY_ABCDxxxx),
    /*p01.DOVA*/ .DOVA_xBCDExxx = not(sig.BUDE_xxxxEFGH),
    /*p01.UVYT*/ .UVYT_xBCDExxx = not(sig.BUDE_xxxxEFGH),
    /*p01.BEKO*/ .BEKO_ABCDxxxx = not(sig.BUDE_xxxxEFGH),
    /*p04.MOPA*/ .MOPA_AxxxxFGH = not(sig.UVYT_xBCDExxx),

    /*p01.BAPY*/ .BAPY_AxxxxxxH = nor(sig.CPUCLK_REQn, sig.AROV_xxxDEFGx, sig.ATYP_xBCDExxx),
    /*p01.BERU*/ .BERU_xBCDEFGx = not(sig.BAPY_AxxxxxxH),
    /*p01.BUFA*/ .BUFA_AxxxxxxH = not(sig.BERU_xBCDEFGx),
    /*p01.BOLO*/ .BOLO_xBCDEFGx = not(sig.BUFA_AxxxxxxH),
    /*p01.BEJA*/ .BEJA_xxxxEFGH = nand(sig.BOLO_xBCDEFGx, sig.BEKO_ABCDxxxx),
    /*p01.BANE*/ .BANE_ABCDxxxx = not(sig.BEJA_xxxxEFGH),
    /*p01.BELO*/ .BELO_xxxxEFGH = not(sig.BANE_ABCDxxxx),
    /*p01.BAZE*/ .BAZE_ABCDxxxx = not(sig.BELO_xxxxEFGH),
    /*p01.BUTO*/ .BUTO_xBCDEFGH = nand(sig.AFEP_ABxxxxGH, sig.ATYP_xBCDExxx, sig.BAZE_ABCDxxxx),
    /*p01.BELE*/ .BELE_Axxxxxxx = not(sig.BUTO_xBCDEFGH),

    // gated on CLK_GOOD
    /*p01.BYJU*/ .BYJU_xBCDEFGH = nor(sig.BELE_Axxxxxxx, sig.CLK_BAD2),
    /*p01.BALY*/ .BALY_Axxxxxxx = not(sig.BYJU_xBCDEFGH),
    /*p01.BOGA*/ .BOGA_AxCDEFGH = not(sig.BALY_Axxxxxxx),
    /*p01.BUVU*/ .BUVU_Axxxxxxx = and(sig.CPUCLK_REQ, sig.BALY_Axxxxxxx),
    /*p01.BYXO*/ .BYXO_xBCDEFGH = not(sig.BUVU_Axxxxxxx),
    /*p01.BEDO*/ .BEDO_xBxxxxxx = not(sig.BYXO_xBCDEFGH),
    /*p01.BOWA*/ .BOWA_AxCDEFGH = not(sig.BEDO_xBxxxxxx),
  };

  return sig;
}

//----------------------------------------

ClockSignals1 ClockSignals1::tick_fast(const SystemSignals& sys_sig,
                                       int phase) {

  ClockSignals1 sig;

  /*p01.ABOL*/ sig.CPUCLK_REQn   = not(sys_sig.CLK_REQ);
  /*p01.BUTY*/ sig.CPUCLK_REQ    = not(sig.CPUCLK_REQn);
  /*p01.UCOB*/ sig.CLK_BAD1      = not(sys_sig.CLK_GOOD);
  /*p01.ATEZ*/ sig.CLK_BAD2      = not(sys_sig.CLK_GOOD);

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
    sig.PHAZ_xBCDExxx = xBCDExxx;
    sig.PHAZ_xxCDEFxx = xxCDEFxx;
    sig.PHAZ_xxxDEFGx = xxxDEFGx;
    sig.PHAZ_xxxxEFGH = xxxxEFGH;

    sig.AFEP_ABxxxxGH = ABxxxxGH;
    sig.ATYP_xBCDExxx = xBCDExxx;
    sig.ADAR_ABCxxxxH = ABCDxxxx;
    sig.AROV_xxxDEFGx = xxxDEFGx;
    sig.AFAS_xxxxxFGH = xxxxxFGH;
    sig.BUTO_xBCDEFGH = AxCDEFGH;
    sig.BELE_Axxxxxxx = xBxxxxxx;
  }
  else {
    sig.PHAZ_xBCDExxx = 0;
    sig.PHAZ_xxCDEFxx = 0;
    sig.PHAZ_xxxDEFGx = 0;
    sig.PHAZ_xxxxEFGH = 0;

    sig.AFEP_ABxxxxGH = 1;
    sig.ATYP_xBCDExxx = 0;
    sig.ADAR_ABCxxxxH = 1;
    sig.AROV_xxxDEFGx = 0;
    sig.AFAS_xxxxxFGH = 0;
    sig.BUTO_xBCDEFGH = 1;
    sig.BELE_Axxxxxxx = 0;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_REQ) {
    sig.NULE_xxxxEFGH = AxxxxFGH;
    sig.BUDE_xxxxEFGH = AxxxxFGH;
    sig.MOPA_AxxxxFGH = AxxxxFGH;
    sig.BEJA_xxxxEFGH = AxxxxFGH;
    sig.BELO_xxxxEFGH = AxxxxFGH;

    sig.BYRY_ABCDxxxx = xBCDExxx;
    sig.DOVA_xBCDExxx = xBCDExxx;
    sig.UVYT_xBCDExxx = xBCDExxx;
    sig.BEKO_ABCDxxxx = xBCDExxx;
    sig.BANE_ABCDxxxx = xBCDExxx;
    sig.BAZE_ABCDxxxx = xBCDExxx;

    sig.BAPY_AxxxxxxH = AxxxxxxH;
    sig.BUFA_AxxxxxxH = AxxxxxxH;
    sig.BERU_xBCDEFGx = xBCDEFGx;
    sig.BOLO_xBCDEFGx = xBCDEFGx;
  }
  else {
    sig.NULE_xxxxEFGH = sys_sig.CLK_REQ;
    sig.BUDE_xxxxEFGH = sys_sig.CLK_REQ;
    sig.MOPA_AxxxxFGH = sys_sig.CLK_REQ;
    sig.BEJA_xxxxEFGH = sys_sig.CLK_REQ;
    sig.BELO_xxxxEFGH = sys_sig.CLK_REQ;

    sig.BYRY_ABCDxxxx = !sys_sig.CLK_REQ;
    sig.DOVA_xBCDExxx = !sys_sig.CLK_REQ;
    sig.UVYT_xBCDExxx = !sys_sig.CLK_REQ;
    sig.BEKO_ABCDxxxx = !sys_sig.CLK_REQ;
    sig.BANE_ABCDxxxx = !sys_sig.CLK_REQ;
    sig.BAZE_ABCDxxxx = !sys_sig.CLK_REQ;

    sig.BAPY_AxxxxxxH = 0;
    sig.BUFA_AxxxxxxH = 0;
    sig.BERU_xBCDEFGx = 1;
    sig.BOLO_xBCDEFGx = 1;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_GOOD) {
    sig.BYJU_xBCDEFGH = AxCDEFGH;
    sig.BOGA_AxCDEFGH = AxCDEFGH;
    sig.BALY_Axxxxxxx = xBxxxxxx;
  }
  else {
    sig.BYJU_xBCDEFGH = 0;
    sig.BOGA_AxCDEFGH = 0;
    sig.BALY_Axxxxxxx = 1;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_GOOD && sys_sig.CLK_REQ) {
    sig.BYXO_xBCDEFGH = AxCDEFGH;
    sig.BOWA_AxCDEFGH = AxCDEFGH;
    sig.BUVU_Axxxxxxx = xBxxxxxx;
    sig.BEDO_xBxxxxxx = xBxxxxxx;
  }
  else {
    sig.BUVU_Axxxxxxx = sys_sig.CLK_REQ;
    sig.BYXO_xBCDEFGH = !sys_sig.CLK_REQ;
    sig.BOWA_AxCDEFGH = !sys_sig.CLK_REQ;
    sig.BEDO_xBxxxxxx = sys_sig.CLK_REQ;
  }

  return sig;
}

//-----------------------------------------------------------------------------

ClockSignals2 ClockSignals2::tick_slow(const Clocks& clk) {
  ClockSignals2 sig = {
    // gated on VID_RESETn
    /*p29.WUVU*/ .WUVU_xxCDxxGH = clk.WUVU_xxCDxxGH,
    /*p21.VENA*/ .VENA_xxxxEFGH = clk.VENA_xxxxEFGH,
    /*p29.WOSU*/ .WOSU_xBCxxFGx = clk.WOSU_xBCxxFGx,
    /*p29.XUPY*/ .XUPY_ABxxEFxx = not(sig.WUVU_xxCDxxGH),
    /*p28.AWOH*/ .AWOH_xxCDxxGH = not(sig.XUPY_ABxxEFxx),
    /*p21.TALU*/ .TALU_xxxxEFGH = not(!sig.VENA_xxxxEFGH),
    /*p21.SONO*/ .SONO_ABCDxxxx = not(sig.TALU_xxxxEFGH),
    /*p29.XOCE*/ .XOCE_AxxDExxH = not(sig.WOSU_xBCxxFGx),
  };

  return sig;
}

//----------------------------------------

ClockSignals2 ClockSignals2::tick_fast(int phase, wire VID_RESETn) {

  ClockSignals2 sig;

  bool xxxxEFGH = (phase == 4) || (phase == 5) || (phase == 6) || (phase == 7);
  bool xxCDxxGH = (phase == 2) || (phase == 3) || (phase == 6) || (phase == 7);
  bool xBCxxFGx = (phase == 1) || (phase == 2) || (phase == 5) || (phase == 6);
  bool ABxxEFxx = !xxCDxxGH;
  bool ABCDxxxx = !xxxxEFGH;
  bool AxxDExxH = !xBCxxFGx;


  if (VID_RESETn) {
    sig.XUPY_ABxxEFxx = ABxxEFxx;
    sig.AWOH_xxCDxxGH = xxCDxxGH;
    sig.WUVU_xxCDxxGH = xxCDxxGH;

    sig.XOCE_AxxDExxH = AxxDExxH;
    sig.WOSU_xBCxxFGx = xBCxxFGx;

    sig.SONO_ABCDxxxx = ABCDxxxx;
    sig.VENA_xxxxEFGH = xxxxEFGH;
    sig.TALU_xxxxEFGH = xxxxEFGH;
  }
  else {
    sig.XUPY_ABxxEFxx = 1;
    sig.AWOH_xxCDxxGH = 0;
    sig.WUVU_xxCDxxGH = 0;

    sig.XOCE_AxxDExxH = 1;
    sig.WOSU_xBCxxFGx = 0;

    sig.SONO_ABCDxxxx = 1;
    sig.VENA_xxxxEFGH = 0;
    sig.TALU_xxxxEFGH = 0;
  }

  return sig;
}

//-----------------------------------------------------------------------------

void Clocks::tock_slow1(const SystemSignals& sys_sig,
                        const ClockSignals1& sig1,
                        Clocks& next) {
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ next.PHAZ_xBCDExxx.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD, !sig1.PHAZ_xxxxEFGH);
  /*p01.ALEF*/ next.PHAZ_xxCDEFxx.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD,  sig1.PHAZ_xBCDExxx);
  /*p01.APUK*/ next.PHAZ_xxxDEFGx.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD,  sig1.PHAZ_xxCDEFxx);
  /*p01.ADYK*/ next.PHAZ_xxxxEFGH.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD,  sig1.PHAZ_xxxDEFGx);
}

//----------------------------------------

void Clocks::tock_slow2(const SystemSignals& /*sys_sig*/,
                        const ClockSignals1& sig1,
                        const ClockSignals2& sig2,
                        wire VID_RESETn,
                        Clocks& next) {
  /*p29.WUVU*/ next.WUVU_xxCDxxGH.tock( sig1.XOTA_AxCxExGx, VID_RESETn, !sig2.WUVU_xxCDxxGH);
  /*p21.VENA*/ next.VENA_xxxxEFGH.tock(!sig2.WUVU_xxCDxxGH, VID_RESETn, !sig2.VENA_xxxxEFGH);
  /*p29.WOSU*/ next.WOSU_xBCxxFGx.tock( sig1.XYFY_xBxDxFxH, VID_RESETn, !sig2.WUVU_xxCDxxGH);
}

//-----------------------------------------------------------------------------

void Clocks::tock_fast1(const SystemSignals& sys_sig, int phase, Clocks& next) {

  bool xBxDxFxH = (phase & 1);

  bool xBCDExxx = (phase == 1) || (phase == 2) || (phase == 3) || (phase == 4);
  bool xxCDEFxx = (phase == 2) || (phase == 3) || (phase == 4) || (phase == 5);
  bool xxxDEFGx = (phase == 3) || (phase == 4) || (phase == 5) || (phase == 6);
  bool xxxxEFGH = (phase == 4) || (phase == 5) || (phase == 6) || (phase == 7);

  if (sys_sig.MODE_PROD) {
    next.PHAZ_xBCDExxx.val = xBCDExxx;
    next.PHAZ_xxCDEFxx.val = xxCDEFxx;
    next.PHAZ_xxxDEFGx.val = xxxDEFGx;
    next.PHAZ_xxxxEFGH.val = xxxxEFGH;
  }
  else {
    next.PHAZ_xBCDExxx.val = 0;
    next.PHAZ_xxCDEFxx.val = 0;
    next.PHAZ_xxxDEFGx.val = 0;
    next.PHAZ_xxxxEFGH.val = 0;
  }

  next.PHAZ_xBCDExxx.clk = xBxDxFxH;
  next.PHAZ_xxCDEFxx.clk = xBxDxFxH;
  next.PHAZ_xxxDEFGx.clk = xBxDxFxH;
  next.PHAZ_xxxxEFGH.clk = xBxDxFxH;
}

//----------------------------------------

void Clocks::tock_fast2(const SystemSignals& /*sys_sig*/, int phase, wire VID_RESETn, Clocks& next) {

  bool xBxDxFxH = (phase & 1);
  bool AxCxExGx = !xBxDxFxH;

  bool xxxxEFGH = (phase == 4) || (phase == 5) || (phase == 6) || (phase == 7);
  bool xxCDxxGH = (phase == 2) || (phase == 3) || (phase == 6) || (phase == 7);
  bool xBCxxFGx = (phase == 1) || (phase == 2) || (phase == 5) || (phase == 6);
  bool ABxxEFxx = !xxCDxxGH;

  if (VID_RESETn) {
    next.WUVU_xxCDxxGH.val = xxCDxxGH;
    next.WUVU_xxCDxxGH.clk = AxCxExGx;

    next.VENA_xxxxEFGH.val = xxxxEFGH;
    next.VENA_xxxxEFGH.clk = ABxxEFxx;

    next.WOSU_xBCxxFGx.val = xBCxxFGx;
    next.WOSU_xBCxxFGx.clk = xBxDxFxH;
  }
  else {
    next.WUVU_xxCDxxGH.val = 0;
    next.WUVU_xxCDxxGH.clk = AxCxExGx;

    next.VENA_xxxxEFGH.val = 0;
    next.VENA_xxxxEFGH.clk = 1;

    next.WOSU_xBCxxFGx.val = 0;
    next.WOSU_xBCxxFGx.clk = xBxDxFxH;
  }
}

//-----------------------------------------------------------------------------

};