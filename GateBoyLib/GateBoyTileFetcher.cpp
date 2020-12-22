#include "GateBoyLib/GateBoyTileFetcher.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyCpuBus.h"

//------------------------------------------------------------------------------------------------------------------------

void TileFetcher::tock(GateBoyClock& clk, Bus BUS_VRAM_Dp[8], wire2 XYMU_RENDERINGp, wire2 NYXU_BFETCH_RSTn, wire2 MOCE_BFETCH_DONEn_old)
{
  for (int feedback = 0; feedback < 2; feedback++) {
    /* p27.MOCE*/ wire2 _MOCE_BFETCH_DONEn = MOCE_BFETCH_DONEn_any(NYXU_BFETCH_RSTn);
    /* p27.LEBO*/ wire2 _LEBO_AxCxExGx = nand2b(clk.ALET_xBxDxFxH(), _MOCE_BFETCH_DONEn);

    /* p27.LAXU*/ _LAXU_BFETCH_S0p.dff17_any(_LEBO_AxCxExGx,             NYXU_BFETCH_RSTn, _LAXU_BFETCH_S0p.qn_any2());
    /* p27.MESU*/ _MESU_BFETCH_S1p.dff17_any(_LAXU_BFETCH_S0p.qn_any2(), NYXU_BFETCH_RSTn, _MESU_BFETCH_S1p.qn_any2());
    /* p27.NYVA*/ _NYVA_BFETCH_S2p.dff17_any(_MESU_BFETCH_S1p.qn_any2(), NYXU_BFETCH_RSTn, _NYVA_BFETCH_S2p.qn_any2());
  }

  /* p27.LYRY*/ wire2 _LYRY_BFETCH_DONEp_old = not1b(MOCE_BFETCH_DONEn_old);
  /* p27.LOVY*/ LOVY_FETCH_DONEp.dff17(clk.MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, _LYRY_BFETCH_DONEp_old);
  /* p27.LURY*/ wire2 _LURY_BG_FETCH_DONEn = and2(LOVY_FETCH_DONEp.qn_new(), XYMU_RENDERINGp);
  /* p27.LONY*/ LONY_FETCHINGp.nand_latch(NYXU_BFETCH_RSTn, _LURY_BG_FETCH_DONEn);
  /* p27.LYZU*/ _LYZU_BFETCH_S0p_D1.dff17(clk.ALET_xBxDxFxH(), XYMU_RENDERINGp, _LAXU_BFETCH_S0p.qp_new());

  tile_temp_a.tock(BUS_VRAM_Dp, LOMA_LATCH_TILE_DAn());
  tile_temp_b.tock(BUS_VRAM_Dp, LABU_LATCH_TILE_DBn());
}

//------------------------------------------------------------------------------------------------------------------------

wire2 TileFetcher::LOMA_LATCH_TILE_DAn() const {
  /* p24.LOBY*/ wire2 _LOBY_RENDERINGn_new_xxx = not1b(_XYMU_RENDERINGp.qp_new());

  /* p27.LAXU*/ wire2 LAXU_BFETCH_S0p = _LAXU_BFETCH_S0p.qp_new();
  /* p27.MESU*/ wire2 MESU_BFETCH_S1p = _MESU_BFETCH_S1p.qp_new();
  /* p27.NYVA*/ wire2 NYVA_BFETCH_S2p = _NYVA_BFETCH_S2p.qp_new();

  /*#p27.LAXE*/ wire2 _LAXE_BFETCH_S0n = not1b(LAXU_BFETCH_S0p);
  /*#p27.NOFU*/ wire2 _NOFU_BFETCH_S2n = not1b(NYVA_BFETCH_S2p);

  /*#p27.MYSO*/ wire2 _MYSO_STORE_VRAM_DATA_TRIGp = nor3b(_LOBY_RENDERINGn_new_xxx, _LAXE_BFETCH_S0n, _LYZU_BFETCH_S0p_D1.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /*#p27.NYDY*/ wire2 _NYDY_LATCH_TILE_DAn = nand3b(_MYSO_STORE_VRAM_DATA_TRIGp, MESU_BFETCH_S1p, _NOFU_BFETCH_S2n); // NYDY on fetch phase 6
  /*#p32.METE*/ wire2 _METE_LATCH_TILE_DAp = not1b(_NYDY_LATCH_TILE_DAn);
  /*#p32.LOMA*/ wire2 _LOMA_LATCH_TILE_DAn = not1b(_METE_LATCH_TILE_DAp);
  return _LOMA_LATCH_TILE_DAn;
}

//------------------------------------------------------------------------------------------------------------------------

wire2 TileFetcher::LABU_LATCH_TILE_DBn() const {
  /* p24.LOBY*/ wire2 _LOBY_RENDERINGn = not1b(_XYMU_RENDERINGp.qp_new());

  /* p27.LAXU*/ wire2 LAXU_BFETCH_S0p = _LAXU_BFETCH_S0p.qp_new();
  /* p27.MESU*/ wire2 MESU_BFETCH_S1p = _MESU_BFETCH_S1p.qp_new();

  /*#p27.LAXE*/ wire2 _LAXE_BFETCH_S0n = not1b(LAXU_BFETCH_S0p);
  /*#p27.NAKO*/ wire2 _NAKO_BFETCH_S1n = not1b(MESU_BFETCH_S1p);

  /*#p27.MYSO*/ wire2 _MYSO_STORE_VRAM_DATA_TRIGp = nor3b(_LOBY_RENDERINGn, _LAXE_BFETCH_S0n, _LYZU_BFETCH_S0p_D1.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /* p27.MOFU*/ wire2 _MOFU_LATCH_TILE_DBp = and2(_MYSO_STORE_VRAM_DATA_TRIGp, _NAKO_BFETCH_S1n); // MOFU fires on fetch phase 2 and 10
  /* p32.LESO*/ wire2 _LESO_LATCH_TILE_DBn = not1b(_MOFU_LATCH_TILE_DBp);
  /* p??.LUVE*/ wire2 _LUVE_LATCH_TILE_DBp = not1b(_LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
  /* p32.LABU*/ wire2 _LABU_LATCH_TILE_DBn = not1b(_LUVE_LATCH_TILE_DBp);
  return _LABU_LATCH_TILE_DBn;
}

//------------------------------------------------------------------------------------------------------------------------
