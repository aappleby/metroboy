#include "GateBoyLib/GateBoyTileFetcher.h"

#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyClocks.h"

void TileTempA::tock(const GateBoyVramBus& vram_bus, wire LOMA_LATCH_TILE_DAn)
{
  /* p32.LEGU*/ LEGU_TILE_DA0n_odd.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[0].qp_old());
  /* p32.NUDU*/ NUDU_TILE_DA1n_odd.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[1].qp_old());
  /* p32.MUKU*/ MUKU_TILE_DA2n_odd.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[2].qp_old());
  /* p32.LUZO*/ LUZO_TILE_DA3n_odd.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[3].qp_old());
  /* p32.MEGU*/ MEGU_TILE_DA4n_odd.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[4].qp_old());
  /* p32.MYJY*/ MYJY_TILE_DA5n_odd.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[5].qp_old());
  /* p32.NASA*/ NASA_TILE_DA6n_odd.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[6].qp_old());
  /* p32.NEFO*/ NEFO_TILE_DA7n_odd.dff8p(LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_Dp[7].qp_old());
}

void TileTempB::tock(const GateBoyVramBus& vram_bus, wire _LABU_LATCH_TILE_DBn)
{
  wire _VYPO_VCC = 1;
  // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
  /* p32.RAWU*/ RAWU_TILE_DB0p_odd.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[0].qp_old());
  /* p32.POZO*/ POZO_TILE_DB1p_odd.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[1].qp_old());
  /* p32.PYZO*/ PYZO_TILE_DB2p_odd.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[2].qp_old());
  /* p32.POXA*/ POXA_TILE_DB3p_odd.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[3].qp_old());
  /* p32.PULO*/ PULO_TILE_DB4p_odd.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[4].qp_old());
  /* p32.POJU*/ POJU_TILE_DB5p_odd.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[5].qp_old());
  /* p32.POWY*/ POWY_TILE_DB6p_odd.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[6].qp_old());
  /* p32.PYJU*/ PYJU_TILE_DB7p_odd.dff11(_LABU_LATCH_TILE_DBn, _VYPO_VCC, vram_bus.BUS_VRAM_Dp[7].qp_old());
}

void TileFetcher::tock2(GateBoyPhaseClock& pclk, wire XYMU_RENDERINGp, wire NYXU_BFETCH_RSTn, wire MOCE_BFETCH_DONEn_old)
{
  /* p27.LAXU*/ LAXU_BFETCH_S0p_evn.RSTn(NYXU_BFETCH_RSTn);
  /* p27.MESU*/ MESU_BFETCH_S1p_evn.RSTn(NYXU_BFETCH_RSTn);
  /* p27.NYVA*/ NYVA_BFETCH_S2p_evn.RSTn(NYXU_BFETCH_RSTn);

  /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn_mid_any = MOCE_BFETCH_DONEn(NYXU_BFETCH_RSTn);
  /* p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(pclk.ALET_xBxDxFxH(), _MOCE_BFETCH_DONEn_mid_any);

  /* p27.LAXU*/ LAXU_BFETCH_S0p_evn.dff17(_LEBO_AxCxExGx,               NYXU_BFETCH_RSTn, LAXU_BFETCH_S0p_evn.qn_new());
  /* p27.MESU*/ MESU_BFETCH_S1p_evn.dff17(LAXU_BFETCH_S0p_evn.qn_new(), NYXU_BFETCH_RSTn, MESU_BFETCH_S1p_evn.qn_new());
  /* p27.NYVA*/ NYVA_BFETCH_S2p_evn.dff17(MESU_BFETCH_S1p_evn.qn_new(), NYXU_BFETCH_RSTn, NYVA_BFETCH_S2p_evn.qn_new());

  /* p27.LYRY*/ wire _LYRY_BFETCH_DONEp_old = not1(MOCE_BFETCH_DONEn_old);
  /* p27.LOVY*/ LOVY_FETCH_DONEp_evn.dff17(pclk.MYVO_AxCxExGx(), NYXU_BFETCH_RSTn, _LYRY_BFETCH_DONEp_old);
  /* p27.LURY*/ wire _LURY_BG_FETCH_DONEn_new_evn = and2(LOVY_FETCH_DONEp_evn.qn_new(), XYMU_RENDERINGp);
  /* p27.LONY*/ LONY_FETCHINGp_xxx.nand_latch(NYXU_BFETCH_RSTn, _LURY_BG_FETCH_DONEn_new_evn);
  /* p27.LYZU*/ LYZU_BFETCH_S0p_D1_odd.dff17(pclk.ALET_xBxDxFxH(), XYMU_RENDERINGp, LAXU_BFETCH_S0p_evn.qp_new());
}


wire TileFetcher::LOMA_LATCH_TILE_DAn(wire XYMU_RENDERINGp) const {
    /* p24.LOBY*/ wire _LOBY_RENDERINGn_new_xxx = not1(XYMU_RENDERINGp);

  /* p27.LAXU*/ wire LAXU_BFETCH_S0p_evn_new = LAXU_BFETCH_S0p_evn.qp_new();
  /* p27.MESU*/ wire MESU_BFETCH_S1p_evn_new = MESU_BFETCH_S1p_evn.qp_new();
  /* p27.NYVA*/ wire NYVA_BFETCH_S2p_evn_new = NYVA_BFETCH_S2p_evn.qp_new();

  /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n_new_evn = not1(LAXU_BFETCH_S0p_evn_new);
  /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n_new_evn = not1(NYVA_BFETCH_S2p_evn_new);

  /*#p27.MYSO*/ wire _MYSO_STORE_VRAM_DATA_TRIGp_new = nor3(_LOBY_RENDERINGn_new_xxx, _LAXE_BFETCH_S0n_new_evn, LYZU_BFETCH_S0p_D1_odd.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /*#p27.NYDY*/ wire _NYDY_LATCH_TILE_DAn_new = nand3(_MYSO_STORE_VRAM_DATA_TRIGp_new, MESU_BFETCH_S1p_evn_new, _NOFU_BFETCH_S2n_new_evn); // NYDY on fetch phase 6
  /*#p32.METE*/ wire _METE_LATCH_TILE_DAp_new = not1(_NYDY_LATCH_TILE_DAn_new);
  /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn_new = not1(_METE_LATCH_TILE_DAp_new);
  return _LOMA_LATCH_TILE_DAn_new;
}

wire TileFetcher::LABU_LATCH_TILE_DBn(wire XYMU_RENDERINGp) const {
  /* p24.LOBY*/ wire _LOBY_RENDERINGn_new_xxx = not1(XYMU_RENDERINGp);

  /* p27.LAXU*/ wire LAXU_BFETCH_S0p_evn_new = LAXU_BFETCH_S0p_evn.qp_new();
  /* p27.MESU*/ wire MESU_BFETCH_S1p_evn_new = MESU_BFETCH_S1p_evn.qp_new();

  /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n_new_evn = not1(LAXU_BFETCH_S0p_evn_new);
  /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n_new_evn = not1(MESU_BFETCH_S1p_evn_new);

  /*#p27.MYSO*/ wire _MYSO_STORE_VRAM_DATA_TRIGp_new = nor3(_LOBY_RENDERINGn_new_xxx, _LAXE_BFETCH_S0n_new_evn, LYZU_BFETCH_S0p_D1_odd.qp_new()); // MYSO fires on fetch phase 2, 6, 10

  /* p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp_new = and2(_MYSO_STORE_VRAM_DATA_TRIGp_new, _NAKO_BFETCH_S1n_new_evn); // MOFU fires on fetch phase 2 and 10
  /* p32.LESO*/ wire _LESO_LATCH_TILE_DBn_new = not1(_MOFU_LATCH_TILE_DBp_new);
  /* p??.LUVE*/ wire _LUVE_LATCH_TILE_DBp_new = not1(_LESO_LATCH_TILE_DBn_new); // Schematic wrong, was labeled AJAR
  /* p32.LABU*/ wire _LABU_LATCH_TILE_DBn_new = not1(_LUVE_LATCH_TILE_DBp_new);
  return _LABU_LATCH_TILE_DBn_new;
}
