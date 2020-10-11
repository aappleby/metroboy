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

void TileFetcher::tick(const SchematicTop& top) {
  /*p24.LOBY*/ wire LOBY_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGp.qp());

  /*#p27.NOCU*/ wire NOCU_WIN_MODEn = not1(top.pix_pipe.PYNU_WIN_MODE_A.qp());
  /* p27.PORE*/ wire PORE_WIN_MODEp = not1(NOCU_WIN_MODEn);
  /* p26.AXAD*/ wire AXAD_WIN_MODEn = not1(PORE_WIN_MODEp);

  /*#p27.LAXE*/ wire LAXE_BFETCH_S0n = not1(LAXU_BFETCH_S0.qp());
  /*#p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(MESU_BFETCH_S1.qp());
  /*#p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(NYVA_BFETCH_S2.qp());

  /* p27.NOGU*/ wire NOGU_FETCH_01p = nand2(NAKO_BFETCH_S1n, NOFU_BFETCH_S2n);
  /* p27.NENY*/ wire NENY_FETCH_01n = not1(NOGU_FETCH_01p);

  /* p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not1(LONY_BG_FETCH_RUNNINGp.qp());
  /* p27.LENA*/ LENA_BGW_VRM_RDp = not1(LUSU_BGW_VRAM_RDn);

  /* p27.POTU*/ wire POTU_BG_MAP_READp  = and2(LENA_BGW_VRM_RDp, NENY_FETCH_01n);
  /*#p25.XEZE*/ wire XEZE_WIN_MAP_READp = and2(POTU_BG_MAP_READp, PORE_WIN_MODEp);
  /* p26.ACEN*/ wire ACEN_BG_MAP_READp  = and2(POTU_BG_MAP_READp, AXAD_WIN_MODEn);

  /* p27.NETA*/ NETA_TILE_READp = and2(LENA_BGW_VRM_RDp, NOGU_FETCH_01p);
  /* p26.ASUL*/ wire ASUL_BG_TILE_READp  = and2(NETA_TILE_READp, AXAD_WIN_MODEn);
  /* p25.XUCY*/ XUCY_WIN_TILE_READn = nand2(NETA_TILE_READp, PORE_WIN_MODEp);
  /*#p25.WUKO*/ WUKO_WIN_MAP_READn  = not1(XEZE_WIN_MAP_READp);
  /* p26.BAFY*/ BAFY_BG_MAP_READn   = not1(ACEN_BG_MAP_READp);
  /* p26.BEJE*/ BEJE_BG_TILE_READn = not1(ASUL_BG_TILE_READp);

  /* p27.MOCE*/ MOCE_BFETCH_DONEn = nand3(LAXU_BFETCH_S0.qp(), NYVA_BFETCH_S2.qp(), top.NYXU_FETCH_TRIGn);

  /*#p27.XUHA*/ XUHA_FETCH_S2p  = not1(NOFU_BFETCH_S2n);

  // MYSO fires on fetch phase 2, 6, 10
  /*#p27.MYSO*/ wire MYSO_BG_TRIGp   = nor3(LOBY_RENDERINGn, LAXE_BFETCH_S0n, LYZU_BFETCH_S0_D1.qp());

  // NYDY on fetch phase 6
  /*#p27.NYDY*/ wire NYDY_LATCH_TILE_DAn = nand3(MYSO_BG_TRIGp, MESU_BFETCH_S1.qp(), NOFU_BFETCH_S2n);
  /*#p32.METE*/ METE_LATCH_TILE_DAp   = not1(NYDY_LATCH_TILE_DAn);

  // MOFU fires on fetch phase 2 and 10
  /*p27.MOFU*/ wire MOFU_LATCH_TILE_DBp = and2(MYSO_BG_TRIGp, NAKO_BFETCH_S1n);
  /*p32.LESO*/ LESO_LATCH_TILE_DBn   = not1(MOFU_LATCH_TILE_DBp);
}

//------------------------------------------------------------------------------

void TileFetcher::tock(SchematicTop& top) {
  wire XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGp.qp();
  /*p24.LOBY*/ wire LOBY_RENDERINGn = not1(XYMU_RENDERINGp);

  /*p01.ZEME*/ wire ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /*p01.ALET*/ wire ALET_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*p27.MYVO*/ wire MYVO_AxCxExGx = not1(ALET_xBxDxFxH);

  /*p27.LEBO*/ wire LEBO_AxCxExGx = nand2(ALET_xBxDxFxH, MOCE_BFETCH_DONEn);

  /*p27.LAXU*/ LAXU_BFETCH_S0   .dfff17(LEBO_AxCxExGx,             top.NYXU_FETCH_TRIGn,   LAXU_BFETCH_S0.qn());
  /*p27.MESU*/ MESU_BFETCH_S1   .dfff17(LAXU_BFETCH_S0.qn(),       top.NYXU_FETCH_TRIGn,   MESU_BFETCH_S1.qn());
  /*p27.NYVA*/ NYVA_BFETCH_S2   .dfff17(MESU_BFETCH_S1.qn(),       top.NYXU_FETCH_TRIGn,   NYVA_BFETCH_S2.qn());
  /*p27.LYZU*/ LYZU_BFETCH_S0_D1.dfff17(ALET_xBxDxFxH, XYMU_RENDERINGp, LAXU_BFETCH_S0.qp());

  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not1(MOCE_BFETCH_DONEn);

  /*p27.NYFO*/ wire NYFO_WIN_FETCH_TRIGn = not1(top.pix_pipe.NUNY_WX_MATCH_TRIGp);
  /*p27.MOSU*/ wire MOSU_WIN_FETCH_TRIGp = not1(NYFO_WIN_FETCH_TRIGn);
  /*p24.NAFY*/ wire NAFY_RENDERING_AND_NOT_WIN_TRIG = nor2(MOSU_WIN_FETCH_TRIGp, LOBY_RENDERINGn);

  /*p24.NYKA*/ NYKA_FETCH_DONE_P11.dfff17(ALET_xBxDxFxH, NAFY_RENDERING_AND_NOT_WIN_TRIG, LYRY_BFETCH_DONEp);
  /*p24.PORY*/ PORY_FETCH_DONE_P12.dfff17(MYVO_AxCxExGx, NAFY_RENDERING_AND_NOT_WIN_TRIG, NYKA_FETCH_DONE_P11.qp());
  /*p24.PYGO*/ PYGO_FETCH_DONE_P13.dfff17(ALET_xBxDxFxH, XYMU_RENDERINGp,   PORY_FETCH_DONE_P12.qp());

  /*p24.POKY*/ POKY_PRELOAD_LATCHp.nor_latch(PYGO_FETCH_DONE_P13.qp(), LOBY_RENDERINGn);

  /*p27.LOVY*/ LOVY_BG_FETCH_DONEp  .dfff17(MYVO_AxCxExGx, top.NYXU_FETCH_TRIGn,     LYRY_BFETCH_DONEp);
  /*p27.LURY*/ wire LURY_BG_FETCH_DONEn = and2(LOVY_BG_FETCH_DONEp.qn(), XYMU_RENDERINGp);
  /*p27.LONY*/ LONY_BG_FETCH_RUNNINGp.nand_latch(top.NYXU_FETCH_TRIGn, LURY_BG_FETCH_DONEn);
}

//------------------------------------------------------------------------------
