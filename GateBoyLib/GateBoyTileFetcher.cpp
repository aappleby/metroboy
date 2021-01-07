#include "GateBoyLib/GateBoyTileFetcher.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyCpuBus.h"

//------------------------------------------------------------------------------------------------------------------------

void TileFetcher::tock(GateBoyClock& clk, const GateBoyVramBus& vram_bus, NorLatch XYMU_RENDERINGn, wire NYXU_BFETCH_RSTn, wire MOCE_BFETCH_DONEn_old)
{
  for (int feedback = 0; feedback < 2; feedback++) {
    /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn = nand3(_LAXU_BFETCH_S0p.qp_any(), _NYVA_BFETCH_S2p.qp_any(), NYXU_BFETCH_RSTn);
    /* p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(clk.ALET_xBxDxFxH(), _MOCE_BFETCH_DONEn);

    /* p27.LAXU*/ _LAXU_BFETCH_S0p.dff17_any(_LEBO_AxCxExGx,             NYXU_BFETCH_RSTn, _LAXU_BFETCH_S0p.qn_any());
    /* p27.MESU*/ _MESU_BFETCH_S1p.dff17_any(_LAXU_BFETCH_S0p.qn_any(), NYXU_BFETCH_RSTn, _MESU_BFETCH_S1p.qn_any());
    /* p27.NYVA*/ _NYVA_BFETCH_S2p.dff17_any(_MESU_BFETCH_S1p.qn_any(), NYXU_BFETCH_RSTn, _NYVA_BFETCH_S2p.qn_any());
  }

  /* p27.LYRY*/ wire _LYRY_BFETCH_DONEp_old = not1(MOCE_BFETCH_DONEn_old);
  /* p27.LOVY*/ LOVY_FETCH_DONEp.dff17(clk.MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, _LYRY_BFETCH_DONEp_old);
  /* p27.LURY*/ wire _LURY_BG_FETCH_DONEn = and2(LOVY_FETCH_DONEp.qn_new(), XYMU_RENDERINGn.qn_new());
  /* p27.LONY*/ LONY_FETCHINGp.nand_latch(NYXU_BFETCH_RSTn, _LURY_BG_FETCH_DONEn);
  /* p27.LYZU*/ _LYZU_BFETCH_S0p_D1.dff17(clk.ALET_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), _LAXU_BFETCH_S0p.qp_new());

  tile_temp_a.store_vram_data(vram_bus, LOMA_LATCH_TILE_DAn());
  tile_temp_b.store_vram_data(vram_bus, LABU_LATCH_TILE_DBn());
}

//------------------------------------------------------------------------------------------------------------------------

wire TileFetcher::LOMA_LATCH_TILE_DAn() const {
  /* p24.LOBY*/ wire _LOBY_RENDERINGn = not1(_XYMU_RENDERINGn.qn_new());

  /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n = not1(_LAXU_BFETCH_S0p.qp_new());
  /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n = not1(_NYVA_BFETCH_S2p.qp_new());

  /*#p27.MYSO*/ wire _MYSO_STORE_VRAM_DATA_TRIGp = nor3(_LOBY_RENDERINGn, _LAXE_BFETCH_S0n, _LYZU_BFETCH_S0p_D1.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /*#p27.NYDY*/ wire _NYDY_LATCH_TILE_DAn = nand3(_MYSO_STORE_VRAM_DATA_TRIGp, _MESU_BFETCH_S1p.qp_new(), _NOFU_BFETCH_S2n); // NYDY on fetch phase 6
  /*#p32.METE*/ wire _METE_LATCH_TILE_DAp = not1(_NYDY_LATCH_TILE_DAn);
  /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn = not1(_METE_LATCH_TILE_DAp);
  return _LOMA_LATCH_TILE_DAn;
}

//------------------------------------------------------------------------------------------------------------------------

wire TileFetcher::LABU_LATCH_TILE_DBn() const {
  /* p24.LOBY*/ wire _LOBY_RENDERINGn = not1(_XYMU_RENDERINGn.qn_new());

  /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n = not1(_LAXU_BFETCH_S0p.qp_new());
  /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n = not1(_MESU_BFETCH_S1p.qp_new());

  /*#p27.MYSO*/ wire _MYSO_STORE_VRAM_DATA_TRIGp = nor3(_LOBY_RENDERINGn, _LAXE_BFETCH_S0n, _LYZU_BFETCH_S0p_D1.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /* p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp = and2(_MYSO_STORE_VRAM_DATA_TRIGp, _NAKO_BFETCH_S1n); // MOFU fires on fetch phase 2 and 10
  /* p32.LESO*/ wire _LESO_LATCH_TILE_DBn = not1(_MOFU_LATCH_TILE_DBp);
  /* p32.LUVE*/ wire _LUVE_LATCH_TILE_DBp = not1(_LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
  /* p32.LABU*/ wire _LABU_LATCH_TILE_DBn = not1(_LUVE_LATCH_TILE_DBp);
  return _LABU_LATCH_TILE_DBn;
}

//------------------------------------------------------------------------------------------------------------------------
