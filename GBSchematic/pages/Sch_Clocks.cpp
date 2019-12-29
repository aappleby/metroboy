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
    /*p01.AFUR*/ .PHAZ_xxxDEFGx = and(clk_reg.PHAZ_xxxDEFGx, sys_sig.MODE_PROD),
    /*p01.ALEF*/ .PHAZ_xxxxEFGH = and(clk_reg.PHAZ_xxxxEFGH, sys_sig.MODE_PROD),
    /*p01.APUK*/ .PHAZ_AxxxxFGH = and(clk_reg.PHAZ_AxxxxFGH, sys_sig.MODE_PROD),
    /*p01.ADYK*/ .PHAZ_ABxxxxGH = and(clk_reg.PHAZ_ABxxxxGH, sys_sig.MODE_PROD),

    /*p01.AFEP*/ .AFEP_ABCDxxxx = not( sig.PHAZ_xxxxEFGH),
    /*p01.ATYP*/ .ATYP_xxxDEFGx = not(!sig.PHAZ_xxxDEFGx),
    /*p01.ADAR*/ .ADAR_xxCDEFxx = not( sig.PHAZ_ABxxxxGH),
    /*p01.AROV*/ .AROV_AxxxxFGH = not(!sig.PHAZ_AxxxxFGH),
    /*p01.AFAS*/ .AFAS_ABxxxxxH = nor(sig.ADAR_xxCDEFxx, sig.ATYP_xxxDEFGx),

    // gated on CPUCLK_REQ
    /*p01.NULE*/ .NULE_ABCxxxxH = nor(sys_sig.CPUCLK_REQn, sig.ATYP_xxxDEFGx),
    /*p01.BYRY*/ .BYRY_xxxDEFGx = not(sig.NULE_ABCxxxxH),
    /*p01.BUDE*/ .BUDE_ABCxxxxH = not(sig.BYRY_xxxDEFGx),
    /*p01.DOVA*/ .DOVA_xxxDEFGx = not(sig.BUDE_ABCxxxxH),
    /*p01.UVYT*/ .UVYT_xxxDEFGx = not(sig.BUDE_ABCxxxxH),
    /*p01.BEKO*/ .BEKO_xxxDEFGx = not(sig.BUDE_ABCxxxxH),
    /*p04.MOPA*/ .MOPA_ABCxxxxH = not(sig.UVYT_xxxDEFGx),

    /*p01.BAPY*/ .BAPY_xBCxxxxx = nor(sys_sig.CPUCLK_REQn, sig.AROV_AxxxxFGH, sig.ATYP_xxxDEFGx),
    /*p01.BERU*/ .BERU_AxxDEFGH = not(sig.BAPY_xBCxxxxx),
    /*p01.BUFA*/ .BUFA_xBCxxxxx = not(sig.BERU_AxxDEFGH),
    /*p01.BOLO*/ .BOLO_AxxDEFGH = not(sig.BUFA_xBCxxxxx),
    /*p01.BEJA*/ .BEJA_ABCxxxxH = nand(sig.BOLO_AxxDEFGH, sig.BEKO_xxxDEFGx),
    /*p01.BANE*/ .BANE_xxxDEFGx = not(sig.BEJA_ABCxxxxH),
    /*p01.BELO*/ .BELO_ABCxxxxH = not(sig.BANE_xxxDEFGx),
    /*p01.BAZE*/ .BAZE_xxxDEFGx = not(sig.BELO_ABCxxxxH),
    /*p01.BUTO*/ .BUTO_ABCxEFGH = nand(sig.AFEP_ABCDxxxx, sig.ATYP_xxxDEFGx, sig.BAZE_xxxDEFGx),
    /*p01.BELE*/ .BELE_xxxDxxxx = not(sig.BUTO_ABCxEFGH),

    // gated on CLK_GOOD
    /*p01.BYJU*/ .BYJU_ABCxEFGH = nor(sig.BELE_xxxDxxxx, sys_sig.CLK_BAD2),
    /*p01.BALY*/ .BALY_xxxDxxxx = not(sig.BYJU_ABCxEFGH),
    /*p01.BOGA*/ .BOGA_ABCxEFGH = not(sig.BALY_xxxDxxxx),
    /*p01.BUVU*/ .BUVU_xxxDxxxx = and(sys_sig.CPUCLK_REQ, sig.BALY_xxxDxxxx),
    /*p01.BYXO*/ .BYXO_ABCxEFGH = not(sig.BUVU_xxxDxxxx),
    /*p01.BEDO*/ .BEDO_xxxDxxxx = not(sig.BYXO_ABCxEFGH),
    /*p01.BOWA*/ .BOWA_ABCxEFGH = not(sig.BEDO_xxxDxxxx),
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
    sig.PHAZ_xxxDEFGx = xBCDExxx;
    sig.PHAZ_xxxxEFGH = xxCDEFxx;
    sig.PHAZ_AxxxxFGH = xxxDEFGx;
    sig.PHAZ_ABxxxxGH = xxxxEFGH;

    sig.AFEP_ABCDxxxx = ABxxxxGH;
    sig.ATYP_xxxDEFGx = xBCDExxx;
    sig.ADAR_xxCDEFxx = ABCDxxxx;
    sig.AROV_AxxxxFGH = xxxDEFGx;
    sig.AFAS_ABxxxxxH = xxxxxFGH;
    sig.BUTO_ABCxEFGH = AxCDEFGH;
    sig.BELE_xxxDxxxx = xBxxxxxx;
  }
  else {
    sig.PHAZ_xxxDEFGx = 0;
    sig.PHAZ_xxxxEFGH = 0;
    sig.PHAZ_AxxxxFGH = 0;
    sig.PHAZ_ABxxxxGH = 0;

    sig.AFEP_ABCDxxxx = 1;
    sig.ATYP_xxxDEFGx = 0;
    sig.ADAR_xxCDEFxx = 1;
    sig.AROV_AxxxxFGH = 0;
    sig.AFAS_ABxxxxxH = 0;
    sig.BUTO_ABCxEFGH = 1;
    sig.BELE_xxxDxxxx = 0;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_REQ) {
    sig.NULE_ABCxxxxH = AxxxxFGH;
    sig.BUDE_ABCxxxxH = AxxxxFGH;
    sig.MOPA_ABCxxxxH = AxxxxFGH;
    sig.BEJA_ABCxxxxH = AxxxxFGH;
    sig.BELO_ABCxxxxH = AxxxxFGH;

    sig.BYRY_xxxDEFGx = xBCDExxx;
    sig.DOVA_xxxDEFGx = xBCDExxx;
    sig.UVYT_xxxDEFGx = xBCDExxx;
    sig.BEKO_xxxDEFGx = xBCDExxx;
    sig.BANE_xxxDEFGx = xBCDExxx;
    sig.BAZE_xxxDEFGx = xBCDExxx;

    sig.BAPY_xBCxxxxx = AxxxxxxH;
    sig.BUFA_xBCxxxxx = AxxxxxxH;
    sig.BERU_AxxDEFGH = xBCDEFGx;
    sig.BOLO_AxxDEFGH = xBCDEFGx;
  }
  else {
    sig.NULE_ABCxxxxH = sys_sig.CLK_REQ;
    sig.BUDE_ABCxxxxH = sys_sig.CLK_REQ;
    sig.MOPA_ABCxxxxH = sys_sig.CLK_REQ;
    sig.BEJA_ABCxxxxH = sys_sig.CLK_REQ;
    sig.BELO_ABCxxxxH = sys_sig.CLK_REQ;

    sig.BYRY_xxxDEFGx = !sys_sig.CLK_REQ;
    sig.DOVA_xxxDEFGx = !sys_sig.CLK_REQ;
    sig.UVYT_xxxDEFGx = !sys_sig.CLK_REQ;
    sig.BEKO_xxxDEFGx = !sys_sig.CLK_REQ;
    sig.BANE_xxxDEFGx = !sys_sig.CLK_REQ;
    sig.BAZE_xxxDEFGx = !sys_sig.CLK_REQ;

    sig.BAPY_xBCxxxxx = 0;
    sig.BUFA_xBCxxxxx = 0;
    sig.BERU_AxxDEFGH = 1;
    sig.BOLO_AxxDEFGH = 1;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_GOOD) {
    sig.BYJU_ABCxEFGH = AxCDEFGH;
    sig.BOGA_ABCxEFGH = AxCDEFGH;
    sig.BALY_xxxDxxxx = xBxxxxxx;
  }
  else {
    sig.BYJU_ABCxEFGH = 0;
    sig.BOGA_ABCxEFGH = 0;
    sig.BALY_xxxDxxxx = 1;
  }

  if (sys_sig.MODE_PROD && sys_sig.CLK_GOOD && sys_sig.CLK_REQ) {
    sig.BYXO_ABCxEFGH = AxCDEFGH;
    sig.BOWA_ABCxEFGH = AxCDEFGH;
    sig.BUVU_xxxDxxxx = xBxxxxxx;
    sig.BEDO_xxxDxxxx = xBxxxxxx;
  }
  else {
    sig.BUVU_xxxDxxxx = sys_sig.CLK_REQ;
    sig.BYXO_ABCxEFGH = !sys_sig.CLK_REQ;
    sig.BOWA_ABCxEFGH = !sys_sig.CLK_REQ;
    sig.BEDO_xxxDxxxx = sys_sig.CLK_REQ;
  }

  return sig;
}

//-----------------------------------------------------------------------------

ClockSignals2 ClockSignals2::tick_slow(const ResetSignals2& rst_sig2, const ClockRegisters& clk_reg) {
  ClockSignals2 sig = {
    // gated on VID_RESETn
    /*p29.WUVU*/ .WUVU_xxCDxxGH = and(clk_reg.WUVU_xxCDxxGH, rst_sig2.VID_RESETn),
    /*p21.VENA*/ .VENA_xxxxEFGH = and(clk_reg.VENA_xxxxEFGH, rst_sig2.VID_RESETn),
    /*p29.WOSU*/ .WOSU_xBCxxFGx = and(clk_reg.WOSU_xBCxxFGx, rst_sig2.VID_RESETn),
    /*p29.XUPY*/ .XUPY_ABxxEFxx = not(sig.WUVU_xxCDxxGH),
    /*p28.AWOH*/ .AWOH_xxCDxxGH = not(sig.XUPY_ABxxEFxx),
    /*p21.TALU*/ .TALU_xxxxEFGH = not(!sig.VENA_xxxxEFGH),
    /*p21.SONO*/ .SONO_ABCDxxxx = not(sig.TALU_xxxxEFGH),
    /*p29.XOCE*/ .XOCE_AxxDExxH = not(sig.WOSU_xBCxxFGx),
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
    /*p29.WUVU*/ .WUVU_xxCDxxGH = and(clk_reg.WUVU_xxCDxxGH, rst_sig2.VID_RESETn),
    /*p21.VENA*/ .VENA_xxxxEFGH = and(clk_reg.VENA_xxxxEFGH, rst_sig2.VID_RESETn),
    /*p29.WOSU*/ .WOSU_xBCxxFGx = and(clk_reg.WOSU_xBCxxFGx, rst_sig2.VID_RESETn),
    /*p29.XUPY*/ .XUPY_ABxxEFxx = not(sig.WUVU_xxCDxxGH),
    /*p28.AWOH*/ .AWOH_xxCDxxGH = not(sig.XUPY_ABxxEFxx),
    /*p21.TALU*/ .TALU_xxxxEFGH = not(!sig.VENA_xxxxEFGH),
    /*p21.SONO*/ .SONO_ABCDxxxx = not(sig.TALU_xxxxEFGH),
    /*p29.XOCE*/ .XOCE_AxxDExxH = not(sig.WOSU_xBCxxFGx),
  };

#if 0
  if (rst_sig2.VID_RESETn) {
    sig.XUPY_ABxxEFxx = ABxxEFxx;
    /*
    sig.AWOH_xxCDxxGH = xxCDxxGH;
    sig.WUVU_xxCDxxGH = xxCDxxGH;

    sig.XOCE_AxxDExxH = AxxDExxH;
    sig.WOSU_xBCxxFGx = xBCxxFGx;

    sig.SONO_ABCDxxxx = ABCDxxxx;
    sig.VENA_xxxxEFGH = xxxxEFGH;
    sig.TALU_xxxxEFGH = xxxxEFGH;
    */
  }
  else {
    sig.XUPY_ABxxEFxx = 1;
    /*
    sig.AWOH_xxCDxxGH = 0;
    sig.WUVU_xxCDxxGH = 0;

    sig.XOCE_AxxDExxH = 1;
    sig.WOSU_xBCxxFGx = 0;

    sig.SONO_ABCDxxxx = 1;
    sig.VENA_xxxxEFGH = 0;
    sig.TALU_xxxxEFGH = 0;
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
  /*p01.AFUR*/ next.PHAZ_xxxDEFGx.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD, !sig1.PHAZ_ABxxxxGH);
  /*p01.ALEF*/ next.PHAZ_xxxxEFGH.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD,  sig1.PHAZ_xxxDEFGx);
  /*p01.APUK*/ next.PHAZ_AxxxxFGH.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD,  sig1.PHAZ_xxxxEFGH);
  /*p01.ADYK*/ next.PHAZ_ABxxxxGH.duotock(sig1.ATAL_xBxDxFxH, sys_sig.MODE_PROD,  sig1.PHAZ_AxxxxFGH);
}

//----------------------------------------

void ClockRegisters::tock_slow2(const SystemSignals& /*sys_sig*/,
                                const ClockSignals1& sig1,
                                const ClockSignals2& sig2,
                                const ResetSignals2& rst_sig2,
                                ClockRegisters& next) {
  /*p29.WUVU*/ next.WUVU_xxCDxxGH.tock( sig1.XOTA_AxCxExGx, rst_sig2.VID_RESETn, !sig2.WUVU_xxCDxxGH);
  /*p21.VENA*/ next.VENA_xxxxEFGH.tock(!sig2.WUVU_xxCDxxGH, rst_sig2.VID_RESETn, !sig2.VENA_xxxxEFGH);
  /*p29.WOSU*/ next.WOSU_xBCxxFGx.tock( sig1.XYFY_xBxDxFxH, rst_sig2.VID_RESETn, !sig2.WUVU_xxCDxxGH);
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
    next.PHAZ_xxxDEFGx.val = xBCDExxx;
    next.PHAZ_xxxxEFGH.val = xxCDEFxx;
    next.PHAZ_AxxxxFGH.val = xxxDEFGx;
    next.PHAZ_ABxxxxGH.val = xxxxEFGH;
  }
  else {
    next.PHAZ_xxxDEFGx.val = 0;
    next.PHAZ_xxxxEFGH.val = 0;
    next.PHAZ_AxxxxFGH.val = 0;
    next.PHAZ_ABxxxxGH.val = 0;
  }

  next.PHAZ_xxxDEFGx.clk = xBxDxFxH;
  next.PHAZ_xxxxEFGH.clk = xBxDxFxH;
  next.PHAZ_AxxxxFGH.clk = xBxDxFxH;
  next.PHAZ_ABxxxxGH.clk = xBxDxFxH;
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