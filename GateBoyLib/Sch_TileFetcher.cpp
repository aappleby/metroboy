#include "GateBoyLib/Sch_TileFetcher.h"
#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

#pragma warning(disable:4189)

//------------------------------------------------------------------------------

void TileFetcher::dump(Dumper& d, const SchematicTop& /*top*/) const {
  d("\002=====TileFetcher=====\001\n");
  d("LAXU_BFETCH_S0           %c\n", LAXU_BFETCH_S0.c());
  d("MESU_BFETCH_S1           %c\n", MESU_BFETCH_S1.c());
  d("NYVA_BFETCH_S2           %c\n", NYVA_BFETCH_S2.c());
  d("LYZU_BFETCH_S0_D1        %c\n", LYZU_BFETCH_S0_D1.c());
  d("\n");
  d("NYKA_FETCH_DONE_P11      %c\n", NYKA_FETCH_DONE_P11.c());
  d("PORY_FETCH_DONE_P12      %c\n", PORY_FETCH_DONE_P12.c());
  d("PYGO_FETCH_DONE_P13      %c\n", PYGO_FETCH_DONE_P13.c());
  d("POKY_PRELOAD_DONEp       %c\n", POKY_PRELOAD_LATCHp.c());
  d("\n");
  d("LONY_FETCH_RUNNINGp      %c\n", LONY_BG_FETCH_RUNNINGp.c()); // 1 for phases 0-11, 0 for 12-15
  d("LOVY_FETCH_DONEp         %c\n", LOVY_BG_FETCH_DONEp.c());    // 0 for phases 0-11, 1 for 12-15
  d("\n");
}

//------------------------------------------------------------------------------

void TileFetcher::tock(SchematicTop& top) {
  /*p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(top.clk_reg.ALET_xBxDxFxH, MOCE_BFETCH_DONEn);

  /*p27.LAXU*/ LAXU_BFETCH_S0   .dff17(_LEBO_AxCxExGx,            top.pix_pipe.NYXU_FETCH_TRIGn,       LAXU_BFETCH_S0.qn16());
  /*p27.MESU*/ MESU_BFETCH_S1   .dff17(LAXU_BFETCH_S0.qn16(),     top.pix_pipe.NYXU_FETCH_TRIGn,       MESU_BFETCH_S1.qn16());
  /*p27.NYVA*/ NYVA_BFETCH_S2   .dff17(MESU_BFETCH_S1.qn16(),     top.pix_pipe.NYXU_FETCH_TRIGn,       NYVA_BFETCH_S2.qn16());
  /*p27.LYZU*/ LYZU_BFETCH_S0_D1.dff17(top.clk_reg.ALET_xBxDxFxH, top.pix_pipe.XYMU_RENDERINGn.qn03(), LAXU_BFETCH_S0.qp17());

  /*p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG = nor2(top.pix_pipe.MOSU_WIN_FETCH_TRIGp, top.pix_pipe.LOBY_RENDERINGn);

  /*p24.NYKA*/ NYKA_FETCH_DONE_P11.dff17(top.clk_reg.ALET_xBxDxFxH, _NAFY_RENDERING_AND_NOT_WIN_TRIG, LYRY_BFETCH_DONEp);
  /*p24.PORY*/ PORY_FETCH_DONE_P12.dff17(top.clk_reg.MYVO_AxCxExGx, _NAFY_RENDERING_AND_NOT_WIN_TRIG, NYKA_FETCH_DONE_P11.qp17());
  /*p24.PYGO*/ PYGO_FETCH_DONE_P13.dff17(top.clk_reg.ALET_xBxDxFxH, top.pix_pipe.XYMU_RENDERINGn.qn03(),   PORY_FETCH_DONE_P12.qp17());

  /*p24.POKY*/ POKY_PRELOAD_LATCHp.nor_latch(PYGO_FETCH_DONE_P13.qp17(), top.pix_pipe.LOBY_RENDERINGn);

  /*p27.LOVY*/ LOVY_BG_FETCH_DONEp  .dff17(top.clk_reg.MYVO_AxCxExGx, top.pix_pipe.NYXU_FETCH_TRIGn,     LYRY_BFETCH_DONEp);
  /*p27.LURY*/ wire _LURY_BG_FETCH_DONEn = and2(LOVY_BG_FETCH_DONEp.qn16(), top.pix_pipe.XYMU_RENDERINGn.qn03());
  /*p27.LONY*/ LONY_BG_FETCH_RUNNINGp.nand_latch(top.pix_pipe.NYXU_FETCH_TRIGn, _LURY_BG_FETCH_DONEn);
}

//------------------------------------------------------------------------------
