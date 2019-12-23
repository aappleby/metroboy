#include "Sch_Clocks.h"

#include "Sch_Pins.h"
#include "Sch_Debug.h"

#pragma warning(disable:4458)
#pragma warning(disable:4100)

namespace Schematics {

//-----------------------------------------------------------------------------

ClockSignals Clocks::tick_slow(const Clocks& clk,
                               wire CLK,
                               wire CLK_GOOD,
                               wire CPUCLK_REQ_,
                               /*p07.UPOJ*/ wire MODE_PROD,
                               /*p01.XAPO*/ wire VID_RESETn) {
  ClockSignals sig = {
    /*p01.ABOL*/ .CPUCLK_REQn   = not(CPUCLK_REQ_),
    /*p01.BUTY*/ .CPUCLK_REQ    = not(sig.CPUCLK_REQn),
    /*p01.UCOB*/ .CLK_BAD1      = not(CLK_GOOD),
    /*p01.ATEZ*/ .CLK_BAD2      = not(CLK_GOOD),
    /*p01.ARYS*/ .ARYS_xBxDxFxH = not(CLK),
    /*p01.ANOS*/ .ANOS_xBxDxFxH = not(CLK),
    /*p01.AVET*/ .ROOT_AxCxExGx = CLK,
    /*p01.ATAL*/ .ATAL_AxCxExGx = not(sig.ROOT_AxCxExGx),
    /*p01.AZOF*/ .AZOF_xBxDxFxH = not(sig.ATAL_AxCxExGx),
    /*p01.ZAXY*/ .ZAXY_AxCxExGx = not(sig.AZOF_xBxDxFxH),
    /*p01.ZEME*/ .ZEME_AxCxExGx = not(sig.ZAXY_AxCxExGx),
    /*p01.ALET*/ .ALET_xBxDxFxH = not(sig.ZEME_AxCxExGx),
    /*p27.MYVO*/ .MYVO_AxCxExGx = not(sig.ALET_xBxDxFxH),
    /*p29.XYVA*/ .XYVA_AxCxExGx = not(sig.ZEME_AxCxExGx),
    /*p29.XOTA*/ .XOTA_AxCxExGx = not(sig.XYVA_AxCxExGx),
    /*p27.MOXE*/ .MOXE_AxCxExGx = not(sig.ALET_xBxDxFxH),
    /*p27.MEHE*/ .MEHE_AxCxExGx = not(sig.ALET_xBxDxFxH),
    /*p01.LAPE*/ .LAPE_AxCxExGx = not(sig.ALET_xBxDxFxH),
    /*p27.TAVA*/ .TAVA_xBxDxFxH = not(sig.LAPE_AxCxExGx),
    /*p29.XYFY*/ .XYFY_AxCxExGx = not(sig.XOTA_AxCxExGx),

    // gated on VID_RESETn
    /*p29.WUVU*/ .WUVU_xxCDxxGH = clk.WUVU_xxCDxxGH,
    /*p21.VENA*/ .VENA_xxxxEFGH = clk.VENA_xxxxEFGH,
    /*p29.WOSU*/ .WOSU_xBCxxFGx = clk.WOSU_xBCxxFGx,
    /*p29.XUPY*/ .XUPY_ABxxEFxx = not(sig.WUVU_xxCDxxGH),
    /*p28.AWOH*/ .AWOH_xxCDxxGH = not(sig.XUPY_ABxxEFxx),
    /*p21.TALU*/ .TALU_xxxxEFGH = not(!sig.VENA_xxxxEFGH),
    /*p21.SONO*/ .SONO_ABCDxxxx = not(sig.TALU_xxxxEFGH),
    /*p29.XOCE*/ .XOCE_AxxDExxH = not(sig.WOSU_xBCxxFGx),

    // gated on MODE_PROD
    /*p01.AFUR*/ .PHAZ_xBCDExxx = clk.PHAZ_xBCDExxx,
    /*p01.ALEF*/ .PHAZ_xxCDEFxx = clk.PHAZ_xxCDEFxx,
    /*p01.APUK*/ .PHAZ_xxxDEFGx = clk.PHAZ_xxxDEFGx,
    /*p01.ADYK*/ .PHAZ_xxxxEFGH = clk.PHAZ_xxxxEFGH,
    /*p01.AFEP*/ .AFEP_AxxxxFGH = not( sig.PHAZ_xxCDEFxx),
    /*p01.ATYP*/ .ATYP_ABCDxxxx = not(!sig.PHAZ_xBCDExxx),
    /*p01.ADAR*/ .ADAR_ABCxxxxH = not( sig.PHAZ_xxxxEFGH),
    /*p01.AROV*/ .AROV_xxCDEFxx = not(!sig.PHAZ_xxxDEFGx),
    /*p01.AFAS*/ .AFAS_xxxxxFGH = nor(sig.ADAR_ABCxxxxH, sig.ATYP_ABCDxxxx),

    // gated on CPUCLK_REQ
    /*p01.NULE*/ .NULE_xxxxEFGH = nor(sig.CPUCLK_REQn, sig.ATYP_ABCDxxxx),
    /*p01.BYRY*/ .BYRY_ABCDxxxx = not(sig.NULE_xxxxEFGH),
    /*p01.BUDE*/ .BUDE_xxxxEFGH = not(sig.BYRY_ABCDxxxx),
    /*p01.DOVA*/ .DOVA_xBCDExxx = not(sig.BUDE_xxxxEFGH),
    /*p01.UVYT*/ .UVYT_xBCDExxx = not(sig.BUDE_xxxxEFGH),
    /*p01.BEKO*/ .BEKO_ABCDxxxx = not(sig.BUDE_xxxxEFGH),
    /*p04.MOPA*/ .MOPA_AxxxxFGH = not(sig.UVYT_xBCDExxx),

    /*p01.BAPY*/ .BAPY_xxxxxxGH = nor(sig.CPUCLK_REQn, sig.AROV_xxCDEFxx, sig.ATYP_ABCDxxxx),
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

//-----------------------------------------------------------------------------

void Clocks::tock_slow(const Clocks& clk,
                       wire CLK,
                       wire CLK_GOOD,
                       wire CPUCLK_REQ_,
                       /*p07.UPOJ*/ wire MODE_PROD,
                       /*p01.XAPO*/ wire VID_RESETn,
                       Clocks& next) {
  ClockSignals sig = tick_slow(clk, CLK, CLK_GOOD, CPUCLK_REQ_, MODE_PROD, VID_RESETn);

  //----------
  // Clock input and deglitcher

  //----------

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ next.PHAZ_xBCDExxx.duotock(sig.ATAL_AxCxExGx, MODE_PROD, !sig.PHAZ_xxxxEFGH);
  /*p01.ALEF*/ next.PHAZ_xxCDEFxx.duotock(sig.ATAL_AxCxExGx, MODE_PROD,  sig.PHAZ_xBCDExxx);
  /*p01.APUK*/ next.PHAZ_xxxDEFGx.duotock(sig.ATAL_AxCxExGx, MODE_PROD,  sig.PHAZ_xxCDEFxx);
  /*p01.ADYK*/ next.PHAZ_xxxxEFGH.duotock(sig.ATAL_AxCxExGx, MODE_PROD,  sig.PHAZ_xxxDEFGx);

  /*p29.WUVU*/ next.WUVU_xxCDxxGH.tock( sig.XOTA_AxCxExGx, VID_RESETn, !sig.WUVU_xxCDxxGH);
  /*p21.VENA*/ next.VENA_xxxxEFGH.tock(!sig.WUVU_xxCDxxGH, VID_RESETn, !sig.VENA_xxxxEFGH);
  /*p29.WOSU*/ next.WOSU_xBCxxFGx.tock( sig.XYFY_AxCxExGx, VID_RESETn, !sig.WUVU_xxCDxxGH);

  next.sig = sig;
}

//-----------------------------------------------------------------------------

void Clocks::tock_fast(const Clocks& clk,
                       wire CLK,
                       wire CLK_GOOD,
                       wire CPUCLK_REQ_,
                       /*p07.UPOJ*/ wire MODE_PROD,
                       /*p01.XAPO*/ wire VID_RESETn,
                       Clocks& next) {
  ClockSignals sig = tick_slow(clk, CLK, CLK_GOOD, CPUCLK_REQ_, MODE_PROD, VID_RESETn);

  //----------

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ next.PHAZ_xBCDExxx.duotock(sig.ATAL_AxCxExGx, MODE_PROD, !sig.PHAZ_xxxxEFGH);
  /*p01.ALEF*/ next.PHAZ_xxCDEFxx.duotock(sig.ATAL_AxCxExGx, MODE_PROD,  sig.PHAZ_xBCDExxx);
  /*p01.APUK*/ next.PHAZ_xxxDEFGx.duotock(sig.ATAL_AxCxExGx, MODE_PROD,  sig.PHAZ_xxCDEFxx);
  /*p01.ADYK*/ next.PHAZ_xxxxEFGH.duotock(sig.ATAL_AxCxExGx, MODE_PROD,  sig.PHAZ_xxxDEFGx);

  /*p29.WUVU*/ next.WUVU_xxCDxxGH.tock( sig.XOTA_AxCxExGx, VID_RESETn, !sig.WUVU_xxCDxxGH);
  /*p21.VENA*/ next.VENA_xxxxEFGH.tock(!sig.WUVU_xxCDxxGH, VID_RESETn, !sig.VENA_xxxxEFGH);
  /*p29.WOSU*/ next.WOSU_xBCxxFGx.tock( sig.XYFY_AxCxExGx, VID_RESETn, !sig.WUVU_xxCDxxGH);

  next.sig = sig;
}

//-----------------------------------------------------------------------------

};