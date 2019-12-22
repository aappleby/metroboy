#include "Sch_Clocks.h"

#include "Sch_Pins.h"
#include "Sch_Debug.h"

#pragma warning(disable:4458)

namespace Schematics {

//-----------------------------------------------------------------------------

void Clocks_tick(const Clocks& clk,
                 wire CLK,
                 wire CLK_GOOD,
                 wire CPUCLK_REQ_,
                 /*p07.UPOJ*/ wire MODE_PROD,
                 /*p01.XAPO*/ wire VID_RESETn,
                 Clocks& next) {
  next.CLKIN_A = CLK_GOOD;
  next.CLKIN_B = CLK;
  
  /*p07.UPOJ*/ next.MODE_PROD = MODE_PROD;
  /*p01.XAPO*/ next.VID_RESETn = VID_RESETn;

  /*p01.UCOB*/ wire CLK_BAD1 = not(CLK_GOOD);
  /*p01.ATEZ*/ wire CLK_BAD2 = not(CLK_GOOD);
  /*p01.ABOL*/ wire CPUCLK_REQn = not(CPUCLK_REQ_);
  /*p01.BUTY*/ wire CPUCLK_REQ  = not(clk.CPUCLK_REQn);

  //----------
  // Clock input and deglitcher

  /*p01.ARYS*/ wire ARYS_xBxDxFxH = not(CLK);
  /*p01.ANOS*/ wire ANOS_xBxDxFxH = nand(CLK, clk.ROOT_AxCxExGx);
  /*p01.AVET*/ wire ROOT_AxCxExGx = nand(clk.ANOS_xBxDxFxH, ARYS_xBxDxFxH);

  /*p01.ATAL*/ wire ATAL_AxCxExGx = not(ROOT_AxCxExGx);
  /*p01.AZOF*/ wire AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
  /*p01.ZAXY*/ wire ZAXY_AxCxExGx = not(AZOF_xBxDxFxH);
  /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_AxCxExGx);
  /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
  /*p27.MYVO*/ wire MYVO_AxCxExGx = not(ALET_xBxDxFxH);
  /*p29.XYVA*/ wire XYVA_AxCxExGx = not(ZEME_AxCxExGx);
  /*p29.XOTA*/ wire XOTA_AxCxExGx = not(XYVA_AxCxExGx);
  /*p27.MOXE*/ wire MOXE_AxCxExGx = not(ALET_xBxDxFxH);
  /*p27.MEHE*/ wire MEHE_AxCxExGx = not(ALET_xBxDxFxH);
  /*p01.LAPE*/ wire LAPE_AxCxExGx = not(ALET_xBxDxFxH);
  /*p27.TAVA*/ wire TAVA_xBxDxFxH = not(LAPE_AxCxExGx);
  /*p29.XYFY*/ wire XYFY_AxCxExGx = not(XOTA_AxCxExGx);

  {
    // gated on VID_RESETn
    /*p29.WUVU*/ wire WUVU_xBCxxFGx = clk.WUVU_xBCxxFGx;
    /*p21.VENA*/ wire VENA_xxxxEFGH = clk.VENA_xxxxEFGH;
    /*p29.WOSU*/ wire WOSU_xBCxxFGx = clk.WOSU_xBCxxFGx;
    /*p29.XUPY*/ wire XUPY_ABxxEFxx = not(WUVU_xBCxxFGx);
    /*p28.AWOH*/ wire AWOH_xxCDxxGH = not(XUPY_ABxxEFxx);
    /*p21.TALU*/ wire TALU_xxxxEFGH = not(!VENA_xxxxEFGH);
    /*p21.SONO*/ wire SONO_ABCDxxxx = not(TALU_xxxxEFGH);
    /*p29.XOCE*/ wire XOCE_AxxDExxH = not(WOSU_xBCxxFGx);

    /*p29.XUPY*/ next.XUPY_ABxxEFxx = XUPY_ABxxEFxx;
    /*p28.AWOH*/ next.AWOH_xxCDxxGH = AWOH_xxCDxxGH;
    /*p21.TALU*/ next.TALU_xxxxEFGH = TALU_xxxxEFGH;
    /*p21.SONO*/ next.SONO_ABCDxxxx = SONO_ABCDxxxx;
    /*p29.XOCE*/ next.XOCE_AxxDExxH = XOCE_AxxDExxH;

    /*p29.WUVU*/ next.WUVU_xBCxxFGx.tock( XOTA_AxCxExGx, VID_RESETn, !WUVU_xBCxxFGx);
    /*p21.VENA*/ next.VENA_xxxxEFGH.tock(!WUVU_xBCxxFGx, VID_RESETn, !VENA_xxxxEFGH);
    /*p29.WOSU*/ next.WOSU_xBCxxFGx.tock( XYFY_AxCxExGx, VID_RESETn, !WUVU_xBCxxFGx);
  }

  // gated on MODE_PROD
  /*p01.AFUR*/ wire PHAZ_xBCDExxx = clk.PHAZ_xBCDExxx;
  /*p01.ALEF*/ wire PHAZ_xxCDEFxx = clk.PHAZ_xxCDEFxx;
  /*p01.APUK*/ wire PHAZ_xxxDEFGx = clk.PHAZ_xxxDEFGx;
  /*p01.ADYK*/ wire PHAZ_xxxxEFGH = clk.PHAZ_xxxxEFGH;
  /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( PHAZ_xxCDEFxx);
  /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!PHAZ_xBCDExxx);
  /*p01.ADAR*/ wire ADAR_ABCxxxxH = not( PHAZ_xxxxEFGH);
  /*p01.AROV*/ wire AROV_xxCDEFxx = not(!PHAZ_xxxDEFGx);
  /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCxxxxH, ATYP_ABCDxxxx);

  // gated on CPUCLK_REQ
  /*p01.NULE*/ wire NULE_xxxxEFGH = nor(CPUCLK_REQn, ATYP_ABCDxxxx);
  /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
  /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
  /*p01.DOVA*/ wire DOVA_xBCDExxx = not(BUDE_xxxxEFGH);
  /*p01.UVYT*/ wire UVYT_xBCDExxx = not(BUDE_xxxxEFGH);
  /*p01.BEKO*/ wire BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p04.MOPA*/ wire MOPA_AxxxxFGH = not(UVYT_xBCDExxx);

  /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(CPUCLK_REQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*p01.BERU*/ wire BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
  /*p01.BUFA*/ wire BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
  /*p01.BOLO*/ wire BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);
  /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
  /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
  /*p01.BELO*/ wire BELO_xxxxEFGH = not(BANE_ABCDxxxx);
  /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
  /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);

  // gated on CLK_GOOD
  /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, CLK_BAD2);
  /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);
  /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_Axxxxxxx);
  /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(CPUCLK_REQ, BALY_Axxxxxxx);
  /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
  /*p01.BEDO*/ wire BEDO_xBxxxxxx = not(BYXO_xBCDEFGH);
  /*p01.BOWA*/ wire BOWA_AxCDEFGH = not(BEDO_xBxxxxxx);

  //----------
  // Commit

  /*p01.ABOL*/ next.CPUCLK_REQn = CPUCLK_REQn;
  /*p01.BUTY*/ next.CPUCLK_REQ  = CPUCLK_REQ;
  /*p01.UCOB*/ next.CLK_BAD1    = CLK_BAD1;
  /*p01.ATEZ*/ next.CLK_BAD2    = CLK_BAD2;  

  /*p01.ARYS*/ next.ARYS_xBxDxFxH = ARYS_xBxDxFxH;
  /*p01.ANOS*/ next.ANOS_xBxDxFxH = ANOS_xBxDxFxH;
  /*p01.AVET*/ next.ROOT_AxCxExGx = ROOT_AxCxExGx;

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ next.PHAZ_xBCDExxx.duotock(ATAL_AxCxExGx, MODE_PROD, !PHAZ_xxxxEFGH);
  /*p01.ALEF*/ next.PHAZ_xxCDEFxx.duotock(ATAL_AxCxExGx, MODE_PROD,  PHAZ_xBCDExxx);
  /*p01.APUK*/ next.PHAZ_xxxDEFGx.duotock(ATAL_AxCxExGx, MODE_PROD,  PHAZ_xxCDEFxx);
  /*p01.ADYK*/ next.PHAZ_xxxxEFGH.duotock(ATAL_AxCxExGx, MODE_PROD,  PHAZ_xxxDEFGx);

  /*p01.AFAS*/ next.AFAS_xxxxxFGH = AFAS_xxxxxFGH;
  /*p01.UVYT*/ next.UVYT_xBCDExxx = UVYT_xBCDExxx;
  /*p04.MOPA*/ next.MOPA_AxxxxFGH = MOPA_AxxxxFGH;
  /*p01.DOVA*/ next.DOVA_xBCDExxx = DOVA_xBCDExxx;
  /*p01.BOGA*/ next.BOGA_AxCDEFGH = BOGA_AxCDEFGH;
  /*p01.BEDO*/ next.BEDO_xBxxxxxx = BEDO_xBxxxxxx;
  /*p01.BOWA*/ next.BOWA_AxCDEFGH = BOWA_AxCDEFGH;
  /*p01.ZEME*/ next.ZEME_AxCxExGx = ZEME_AxCxExGx;
  /*p01.ALET*/ next.ALET_xBxDxFxH = ALET_xBxDxFxH;
  /*p27.MYVO*/ next.MYVO_AxCxExGx = MYVO_AxCxExGx;
  /*p29.XOTA*/ next.XOTA_AxCxExGx = XOTA_AxCxExGx;
  /*p27.MOXE*/ next.MOXE_AxCxExGx = MOXE_AxCxExGx;
  /*p27.MEHE*/ next.MEHE_AxCxExGx = MEHE_AxCxExGx;
  /*p01.LAPE*/ next.LAPE_AxCxExGx = LAPE_AxCxExGx;
  /*p27.TAVA*/ next.TAVA_xBxDxFxH = TAVA_xBxDxFxH;
}

//-----------------------------------------------------------------------------

};