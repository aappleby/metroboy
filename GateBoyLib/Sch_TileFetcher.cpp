#include "GateBoyLib/Sch_TileFetcher.h"
#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void TileFetcher::dump(Dumper& d, const SchematicTop& /*top*/) const {
  d("----------TileFetcher---------\n");
  d("LAXU_BFETCH_S0           %c\n", _LAXU_BFETCH_S0.c());
  d("MESU_BFETCH_S1           %c\n", _MESU_BFETCH_S1.c());
  d("NYVA_BFETCH_S2           %c\n", _NYVA_BFETCH_S2.c());
  d("LYZU_BFETCH_S0_D1        %c\n", _LYZU_BFETCH_S0_D1.c());
  d("\n");
  d("NYKA_FETCH_DONE_P11      %c\n", _NYKA_FETCH_DONE_P11.c());
  d("PORY_FETCH_DONE_P12      %c\n", _PORY_FETCH_DONE_P12.c());
  d("PYGO_FETCH_DONE_P13      %c\n", _PYGO_FETCH_DONE_P13.c());
  d("POKY_PRELOAD_DONEp       %c\n", _POKY_PRELOAD_LATCHp.c());
  d("\n");
  d("LONY_FETCH_RUNNINGp      %c\n", _LONY_BG_FETCH_RUNNINGp.c()); // 1 for phases 0-11, 0 for 12-15
  d("LOVY_FETCH_DONEp         %c\n", _LOVY_BG_FETCH_DONEp.c());    // 0 for phases 0-11, 1 for 12-15
  d("\n");
}

//------------------------------------------------------------------------------

void TileFetcher::tock(SchematicTop& top) {
  /*p01.ZEME*/ wire ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /*p01.ALET*/ wire ALET_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*p27.MYVO*/ wire MYVO_AxCxExGx = not1(ALET_xBxDxFxH);

  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(_LAXU_BFETCH_S0.qp(), _NYVA_BFETCH_S2.qp(), top.NYXU_FETCH_TRIGn);
  /*p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(ALET_xBxDxFxH, MOCE_BFETCH_DONEn);

  /*p27.LAXU*/ _LAXU_BFETCH_S0    = dff17_AB(_LEBO_AxCxExGx,             top.NYXU_FETCH_TRIGn,   _LAXU_BFETCH_S0.qn());
  /*p27.MESU*/ _MESU_BFETCH_S1    = dff17_AB(_LAXU_BFETCH_S0.qn(),       top.NYXU_FETCH_TRIGn,   _MESU_BFETCH_S1.qn());
  /*p27.NYVA*/ _NYVA_BFETCH_S2    = dff17_AB(_MESU_BFETCH_S1.qn(),       top.NYXU_FETCH_TRIGn,   _NYVA_BFETCH_S2.qn());
  /*p27.LYZU*/ _LYZU_BFETCH_S0_D1 = dff17_B(ALET_xBxDxFxH, top.pix_pipe.XYMU_RENDERINGp.tp(), _LAXU_BFETCH_S0.qp());

  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not1(MOCE_BFETCH_DONEn);

  /*p27.NYFO*/ wire NYFO_WIN_FETCH_TRIGn = not1(top.pix_pipe.NUNY_WX_MATCH_TRIGp);
  /*p27.MOSU*/ wire MOSU_WIN_FETCH_TRIGp = not1(NYFO_WIN_FETCH_TRIGn);
  /*p24.LOBY*/ wire LOBY_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGp.tp());
  /*p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG = nor2(MOSU_WIN_FETCH_TRIGp, LOBY_RENDERINGn);

  /*p24.NYKA*/ _NYKA_FETCH_DONE_P11 = dff17_B(ALET_xBxDxFxH, _NAFY_RENDERING_AND_NOT_WIN_TRIG, LYRY_BFETCH_DONEp);
  /*p24.PORY*/ _PORY_FETCH_DONE_P12 = dff17_B(MYVO_AxCxExGx, _NAFY_RENDERING_AND_NOT_WIN_TRIG, _NYKA_FETCH_DONE_P11.qp());
  /*p24.PYGO*/ _PYGO_FETCH_DONE_P13 = dff17_B(ALET_xBxDxFxH, top.pix_pipe.XYMU_RENDERINGp.tp(),   _PORY_FETCH_DONE_P12.qp());

  /*p24.POKY*/ _POKY_PRELOAD_LATCHp = nor_latch(_PYGO_FETCH_DONE_P13.qp(), LOBY_RENDERINGn);

  /*p27.LOVY*/ _LOVY_BG_FETCH_DONEp   = dff17_A(MYVO_AxCxExGx, top.NYXU_FETCH_TRIGn,     LYRY_BFETCH_DONEp);
  /*p27.LURY*/ wire _LURY_BG_FETCH_DONEn = and2(_LOVY_BG_FETCH_DONEp.qn(), top.pix_pipe.XYMU_RENDERINGp.tp());
  /*p27.LONY*/ _LONY_BG_FETCH_RUNNINGp = nand_latch(top.NYXU_FETCH_TRIGn, _LURY_BG_FETCH_DONEn);
}

//------------------------------------------------------------------------------
