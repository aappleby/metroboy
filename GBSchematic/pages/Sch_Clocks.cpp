#include "Sch_Clocks.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Clocks::tick(const ClocksIn& in, const Clocks& b) {
  
  Clocks& next = *this;

  //----------
  // Clock input and deglitcher

  /*p01.ABOL*/ next.CPUCLK_REQn = not(in.CPUCLK_REQ);
  /*p01.BUTY*/ next.CPUCLK_REQ  = not(b.CPUCLK_REQn);

  /*p01.UCOB*/ next.CLK_BAD1 = not(in.CLKIN_A);
  /*p01.ATEZ*/ next.CLK_BAD2 = not(in.CLKIN_A);
  /*p01.ARYS*/ wire CLKIN_Bn = not(in.CLKIN_B);

  /*p01.ANOS*/ next.ROOTCLK_AxCxExGx = nand(in.CLKIN_B, b.ROOTCLK_xBxDxFxH);
  /*p01.AVET*/ next.ROOTCLK_xBxDxFxH = nand(b.ROOTCLK_AxCxExGx, CLKIN_Bn);

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  /*p01.ATAL*/ wire PHASE_CLK = not(b.ROOTCLK_xBxDxFxH); // apu, phase generator
  /*p01.AFUR*/ next.PHASE_ABCDxxxx.tock(PHASE_CLK, in.MODE_PROD, !b.PHASE_xxxDEFGx);
  /*p01.ALEF*/ next.PHASE_xBCDExxx.tock(PHASE_CLK, in.MODE_PROD, b.PHASE_ABCDxxxx);
  /*p01.APUK*/ next.PHASE_xxCDEFxx.tock(PHASE_CLK, in.MODE_PROD, b.PHASE_xBCDExxx);
  /*p01.ADYK*/ next.PHASE_xxxDEFGx.tock(PHASE_CLK, in.MODE_PROD, b.PHASE_xxCDEFxx);

  //----------
  // Major branches of the clock tree.

  /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!b.PHASE_ABCDxxxx);  
  /*p01.ADAR*/ wire ADAR_ABCxxxxH = not( b.PHASE_xxxDEFGx);
  /*p01.AFAS*/ next.AFAS_xxxxEFGx = nor(ADAR_ABCxxxxH, ATYP_ABCDxxxx);

  /*p01.NULE*/ wire NULE_xxxxEFGH = nor(b.CPUCLK_REQn, ATYP_ABCDxxxx);
  /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
  /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
  /*p01.UVYT*/ next.UVYT_ABCDxxxx = not(BUDE_xxxxEFGH); // dma
  /*p04.MOPA*/ next.MOPA_xxxxEFGH = not(b.UVYT_ABCDxxxx); // -> sprites, dma

  /*p01.DOVA*/ next.DOVA_ABCDxxxx = not(BUDE_xxxxEFGH); // apu

  /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( b.PHASE_xBCDExxx);
  /*p01.AROV*/ wire AROV_xxCDEFxx = not(!b.PHASE_xxCDEFxx);

  /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(b.CPUCLK_REQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*p01.BERU*/ wire BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
  /*p01.BUFA*/ wire BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
  /*p01.BOLO*/ wire BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);
  /*p01.BEKO*/ wire BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
  /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
  /*p01.BELO*/ wire BELO_xxxxEFGH = not(BANE_ABCDxxxx);
  /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
  /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
  /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, b.CLK_BAD2);
  /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);

  /*p01.BOGA*/ next.BOGA_xBCDEFGH = not(BALY_Axxxxxxx); // joypad, timer, reset
  /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(b.CPUCLK_REQ, BALY_Axxxxxxx);
  /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
  /*p01.BEDO*/ next.BEDO_Axxxxxxx = not(BYXO_xBCDEFGH); // debug

  /*p01.BOWA*/ next.BOWA_xBCDEFGH = not(b.BEDO_Axxxxxxx);

  /*p01.ATAL*/ wire ATAL_AxCxExGx = not(b.ROOTCLK_xBxDxFxH);
  /*p01.AZOF*/ wire AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
  /*p01.ZAXY*/ wire ZAXY_AxCxExGx = not(AZOF_xBxDxFxH);
  /*p01.ZEME*/ next.ZEME_xBxDxFxH = not(ZAXY_AxCxExGx);
  /*p01.ALET*/ next.ALET_AxCxExGx = not(b.ZEME_xBxDxFxH);
  /*p27.MYVO*/ next.MYVO_xBxDxFxH = not(b.ALET_AxCxExGx);

  /*p29.XYVA*/ wire XYVA_AxCxExGx = not(ZEME_xBxDxFxH);

  /*p29.XOTA*/ next.XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
  /*p29.WUVU*/ next.WUVU_AxxDExxH.tock(b.XOTA_xBxDxFxH, in.VID_RESETn, !b.WUVU_AxxDExxH);

  /*p29.XUPY*/ next.XUPY_xBCxxFGx = not(b.WUVU_AxxDExxH);

  /*p28.AWOH*/ next.AWOH_AxxDExxH = not(b.XUPY_xBCxxFGx);
  /*p21.VENA*/ next.VENA_AxxxxFGH.tock(!b.WUVU_AxxDExxH, in.VID_RESETn, !b.VENA_AxxxxFGH);

  /*p21.TALU*/ next.TALU_AxxxxFGH = not(!b.VENA_AxxxxFGH);
  /*p21.SONO*/ next.SONO_xBCDExxx = not(b.TALU_AxxxxFGH);

  /*p27.MOXE*/ next.MOXE_xBxDxFxH = not(b.ALET_AxCxExGx);

  /*p27.MEHE*/ next.MEHE_xBxDxFxH = not(b.ALET_AxCxExGx);

  /*p01.LAPE*/ next.LAPE_xBxDxFxH = not(b.ALET_AxCxExGx);
  /*p27.TAVA*/ next.TAVA_AxCxExGx = not(b.LAPE_xBxDxFxH);

  /*p29.XYFY*/ wire XYFY_AxCxExGx = not(b.XOTA_xBxDxFxH);
  /*p29.WOSU*/ next.WOSU_QQQQQQQQ.tock(XYFY_AxCxExGx, in.VID_RESETn, !b.WUVU_AxxDExxH);
  /*p29.XOCE*/ next.XOCE_QQQQQQQQ = not(b.WOSU_QQQQQQQQ);

}

//-----------------------------------------------------------------------------

};