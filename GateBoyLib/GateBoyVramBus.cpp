#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyDMA.h"

#include "GateBoyLib/GateBoyTileFetcher.h"
#include "GateBoyLib/GateBoySpriteStore.h"
#include "GateBoyLib/GateBoyOamBus.h"
#include "GateBoyLib/GateBoyCpuBus.h"

//------------------------------------------------------------------------------------------------------------------------

void SpriteTempA::store_sprite(SpriteFlipX sprite, wire XADO_STORE_SPRITE_An) {
  /* p33.REWO*/ REWO_SPRITE_DA0n.dff8n(XADO_STORE_SPRITE_An, sprite.PUTE_FLIP0p);
  /* p33.PEBA*/ PEBA_SPRITE_DA1n.dff8n(XADO_STORE_SPRITE_An, sprite.PELO_FLIP1p);
  /* p33.MOFO*/ MOFO_SPRITE_DA2n.dff8n(XADO_STORE_SPRITE_An, sprite.PONO_FLIP2p);
  /* p33.PUDU*/ PUDU_SPRITE_DA3n.dff8n(XADO_STORE_SPRITE_An, sprite.POBE_FLIP3p);
  /* p33.SAJA*/ SAJA_SPRITE_DA4n.dff8n(XADO_STORE_SPRITE_An, sprite.PACY_FLIP4p);
  /* p33.SUNY*/ SUNY_SPRITE_DA5n.dff8n(XADO_STORE_SPRITE_An, sprite.PUGU_FLIP5p);
  /* p33.SEMO*/ SEMO_SPRITE_DA6n.dff8n(XADO_STORE_SPRITE_An, sprite.PAWE_FLIP6p);
  /* p33.SEGA*/ SEGA_SPRITE_DA7n.dff8n(XADO_STORE_SPRITE_An, sprite.PULY_FLIP7p);
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteTempB::store_sprite(SpriteFlipX sprite, wire PUCO_STORE_SPRITE_Bn) {
  /* p33.PEFO*/ PEFO_SPRITE_DB0n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PUTE_FLIP0p);
  /* p33.ROKA*/ ROKA_SPRITE_DB1n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PELO_FLIP1p);
  /* p33.MYTU*/ MYTU_SPRITE_DB2n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PONO_FLIP2p);
  /* p33.RAMU*/ RAMU_SPRITE_DB3n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.POBE_FLIP3p);
  /* p33.SELE*/ SELE_SPRITE_DB4n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PACY_FLIP4p);
  /* p33.SUTO*/ SUTO_SPRITE_DB5n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PUGU_FLIP5p);
  /* p33.RAMA*/ RAMA_SPRITE_DB6n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PAWE_FLIP6p);
  /* p33.RYDU*/ RYDU_SPRITE_DB7n.dff8n(PUCO_STORE_SPRITE_Bn, sprite.PULY_FLIP7p);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyVramBus::cpu_addr_to_vram_addr(Signal BUS_CPU_A[16], wire XYMU_RENDERINGp, wire LUFA_DMA_VRAMp) {
  // CPU to VRAM address driver
  /*#p25.XANE*/ wire _XANE_VRAM_LOCKn_new_evn = nor2(LUFA_DMA_VRAMp, XYMU_RENDERINGp);
  /* p25.XEDU*/ wire _XEDU_VRAM_LOCKp_new_evn = not1(_XANE_VRAM_LOCKn_new_evn);
  /* p25.XAKY*/ BUS_VRAM_An[ 0].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 0]);
  /* p25.XUXU*/ BUS_VRAM_An[ 1].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 1]);
  /* p25.XYNE*/ BUS_VRAM_An[ 2].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 2]);
  /* p25.XODY*/ BUS_VRAM_An[ 3].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 3]);
  /* p25.XECA*/ BUS_VRAM_An[ 4].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 4]);
  /* p25.XOBA*/ BUS_VRAM_An[ 5].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 5]);
  /* p25.XOPO*/ BUS_VRAM_An[ 6].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 6]);
  /* p25.XYBO*/ BUS_VRAM_An[ 7].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 7]);
  /* p25.RYSU*/ BUS_VRAM_An[ 8].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 8]);
  /* p25.RESE*/ BUS_VRAM_An[ 9].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 9]);
  /* p25.RUSE*/ BUS_VRAM_An[10].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[10]);
  /* p25.RYNA*/ BUS_VRAM_An[11].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[11]);
  /* p25.RUMO*/ BUS_VRAM_An[12].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[12]);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyVramBus::dma_addr_to_vram_addr(const GateBoyDMA& dma) {
  /* p04.NAKY*/ wire _NAKY_DMA_A00p_new = dma.NAKY_DMA_A00p.qp_new();
  /* p04.PYRO*/ wire _PYRO_DMA_A01p_new = dma.PYRO_DMA_A01p.qp_new();
  /* p04.NEFY*/ wire _NEFY_DMA_A02p_new = dma.NEFY_DMA_A02p.qp_new();
  /* p04.MUTY*/ wire _MUTY_DMA_A03p_new = dma.MUTY_DMA_A03p.qp_new();
  /* p04.NYKO*/ wire _NYKO_DMA_A04p_new = dma.NYKO_DMA_A04p.qp_new();
  /* p04.PYLO*/ wire _PYLO_DMA_A05p_new = dma.PYLO_DMA_A05p.qp_new();
  /* p04.NUTO*/ wire _NUTO_DMA_A06p_new = dma.NUTO_DMA_A06p.qp_new();
  /* p04.MUGU*/ wire _MUGU_DMA_A07p_new = dma.MUGU_DMA_A07p.qp_new();
  /* p04.NAFA*/ wire _NAFA_DMA_A08p_new = dma.NAFA_DMA_A08n.qn_new();
  /* p04.PYNE*/ wire _PYNE_DMA_A09p_new = dma.PYNE_DMA_A09n.qn_new();
  /* p04.PARA*/ wire _PARA_DMA_A10p_new = dma.PARA_DMA_A10n.qn_new();
  /* p04.NYDO*/ wire _NYDO_DMA_A11p_new = dma.NYDO_DMA_A11n.qn_new();
  /* p04.NYGY*/ wire _NYGY_DMA_A12p_new = dma.NYGY_DMA_A12n.qn_new();

  /* p04.AHOC*/ wire _AHOC_DMA_VRAMn_new = not1(dma.LUFA_DMA_VRAMp());
  /* p04.ECAL*/ BUS_VRAM_An[ 0].tri6_nn(_AHOC_DMA_VRAMn_new, _NAKY_DMA_A00p_new);
  /* p04.EGEZ*/ BUS_VRAM_An[ 1].tri6_nn(_AHOC_DMA_VRAMn_new, _PYRO_DMA_A01p_new);
  /* p04.FUHE*/ BUS_VRAM_An[ 2].tri6_nn(_AHOC_DMA_VRAMn_new, _NEFY_DMA_A02p_new);
  /* p04.FYZY*/ BUS_VRAM_An[ 3].tri6_nn(_AHOC_DMA_VRAMn_new, _MUTY_DMA_A03p_new);
  /* p04.DAMU*/ BUS_VRAM_An[ 4].tri6_nn(_AHOC_DMA_VRAMn_new, _NYKO_DMA_A04p_new);
  /* p04.DAVA*/ BUS_VRAM_An[ 5].tri6_nn(_AHOC_DMA_VRAMn_new, _PYLO_DMA_A05p_new);
  /* p04.ETEG*/ BUS_VRAM_An[ 6].tri6_nn(_AHOC_DMA_VRAMn_new, _NUTO_DMA_A06p_new);
  /*#p04.EREW*/ BUS_VRAM_An[ 7].tri6_nn(_AHOC_DMA_VRAMn_new, _MUGU_DMA_A07p_new);
  /*#p04.EVAX*/ BUS_VRAM_An[ 8].tri6_nn(_AHOC_DMA_VRAMn_new, _NAFA_DMA_A08p_new);
  /* p04.DUVE*/ BUS_VRAM_An[ 9].tri6_nn(_AHOC_DMA_VRAMn_new, _PYNE_DMA_A09p_new);
  /* p04.ERAF*/ BUS_VRAM_An[10].tri6_nn(_AHOC_DMA_VRAMn_new, _PARA_DMA_A10p_new);
  /* p04.FUSY*/ BUS_VRAM_An[11].tri6_nn(_AHOC_DMA_VRAMn_new, _NYDO_DMA_A11p_new);
  /* p04.EXYF*/ BUS_VRAM_An[12].tri6_nn(_AHOC_DMA_VRAMn_new, _NYGY_DMA_A12p_new);
}

//------------------------------------------------------------------------------------------------------------------------
// BG map read address

void GateBoyVramBus::scroll_to_addr(BGScrollX scroll_x, BGScrollY scroll_y, wire POTU_BGW_MAP_READp, wire AXAD_WIN_MODEn, wire XAFO_LCDC_BGMAPp) {
  /* p26.ACEN*/ wire _ACEN_BG_MAP_READp = and2(POTU_BGW_MAP_READp, AXAD_WIN_MODEn);
  /* p26.BAFY*/ wire _BAFY_BG_MAP_READn = not1(_ACEN_BG_MAP_READp);
  /* p26.AXEP*/ BUS_VRAM_An[ 0].tri6_nn(_BAFY_BG_MAP_READn, scroll_x.BABE_MAP_X0S);
  /* p26.AFEB*/ BUS_VRAM_An[ 1].tri6_nn(_BAFY_BG_MAP_READn, scroll_x.ABOD_MAP_X1S);
  /* p26.ALEL*/ BUS_VRAM_An[ 2].tri6_nn(_BAFY_BG_MAP_READn, scroll_x.BEWY_MAP_X2S);
  /* p26.COLY*/ BUS_VRAM_An[ 3].tri6_nn(_BAFY_BG_MAP_READn, scroll_x.BYCA_MAP_X3S);
  /* p26.AJAN*/ BUS_VRAM_An[ 4].tri6_nn(_BAFY_BG_MAP_READn, scroll_x.ACUL_MAP_X4S);
  /* p26.DUHO*/ BUS_VRAM_An[ 5].tri6_nn(_BAFY_BG_MAP_READn, scroll_y.ETAM_MAP_Y0S);
  /* p26.CASE*/ BUS_VRAM_An[ 6].tri6_nn(_BAFY_BG_MAP_READn, scroll_y.DOTO_MAP_Y1S);
  /* p26.CYPO*/ BUS_VRAM_An[ 7].tri6_nn(_BAFY_BG_MAP_READn, scroll_y.DABA_MAP_Y2S);
  /* p26.CETA*/ BUS_VRAM_An[ 8].tri6_nn(_BAFY_BG_MAP_READn, scroll_y.EFYK_MAP_Y3S);
  /* p26.DAFE*/ BUS_VRAM_An[ 9].tri6_nn(_BAFY_BG_MAP_READn, scroll_y.EJOK_MAP_Y4S);
  /*#p26.AMUV*/ BUS_VRAM_An[10].tri6_nn(_BAFY_BG_MAP_READn, XAFO_LCDC_BGMAPp);
  /* p26.COVE*/ BUS_VRAM_An[11].tri6_nn(_BAFY_BG_MAP_READn, 1);
  /* p26.COXO*/ BUS_VRAM_An[12].tri6_nn(_BAFY_BG_MAP_READn, 1);
}

//------------------------------------------------------------------------------------------------------------------------
// Win map read address


void GateBoyVramBus::win_to_addr(const WinMapX& win_map_x, const WinLineY& win_line_y, wire POTU_BGW_MAP_READp, wire PORE_WIN_MODEp, wire WOKY_LCDC_WINMAPp) {
  /*#p25.XEZE*/ wire _XEZE_WIN_MAP_READp = and2(POTU_BGW_MAP_READp, PORE_WIN_MODEp);
  /*#p25.WUKO*/ wire _WUKO_WIN_MAP_READn = not1(_XEZE_WIN_MAP_READp);
  /*#p27.XEJA*/ BUS_VRAM_An[ 0].tri6_nn(_WUKO_WIN_MAP_READn, win_map_x.WYKA_WIN_X3.qp_new());
  /* p27.XAMO*/ BUS_VRAM_An[ 1].tri6_nn(_WUKO_WIN_MAP_READn, win_map_x.WODY_WIN_X4.qp_new());
  /* p27.XAHE*/ BUS_VRAM_An[ 2].tri6_nn(_WUKO_WIN_MAP_READn, win_map_x.WOBO_WIN_X5.qp_new());
  /* p27.XULO*/ BUS_VRAM_An[ 3].tri6_nn(_WUKO_WIN_MAP_READn, win_map_x.WYKO_WIN_X6.qp_new());
  /* p27.WUJU*/ BUS_VRAM_An[ 4].tri6_nn(_WUKO_WIN_MAP_READn, win_map_x.XOLO_WIN_X7.qp_new());
  /*#p27.VYTO*/ BUS_VRAM_An[ 5].tri6_nn(_WUKO_WIN_MAP_READn, win_line_y.TUFU_WIN_Y3.qp_new());
  /* p27.VEHA*/ BUS_VRAM_An[ 6].tri6_nn(_WUKO_WIN_MAP_READn, win_line_y.TAXA_WIN_Y4.qp_new());
  /* p27.VACE*/ BUS_VRAM_An[ 7].tri6_nn(_WUKO_WIN_MAP_READn, win_line_y.TOZO_WIN_Y5.qp_new());
  /* p27.VOVO*/ BUS_VRAM_An[ 8].tri6_nn(_WUKO_WIN_MAP_READn, win_line_y.TATE_WIN_Y6.qp_new());
  /* p27.VULO*/ BUS_VRAM_An[ 9].tri6_nn(_WUKO_WIN_MAP_READn, win_line_y.TEKE_WIN_Y7.qp_new());
  /*#p27.VEVY*/ BUS_VRAM_An[10].tri6_nn(_WUKO_WIN_MAP_READn, WOKY_LCDC_WINMAPp);
  /* p27.VEZA*/ BUS_VRAM_An[11].tri6_nn(_WUKO_WIN_MAP_READn, 1);
  /* p27.VOGU*/ BUS_VRAM_An[12].tri6_nn(_WUKO_WIN_MAP_READn, 1);
}

//------------------------------------------------------------------------------------------------------------------------
// BG/Win tile read address

void GateBoyVramBus::tile_to_addr(const BGScrollY scroll_y, const WinLineY win_line_y, const TileTempB tile_temp_b, wire NETA_BGW_TILE_READp, wire XUHA_FETCH_HILOp, wire WEXU_LCDC_BGTILEp, wire PORE_WIN_MODEp, wire AXAD_WIN_MODEn) {
  /* p26.ASUL*/ wire _ASUL_BG_TILE_READp =  and2(NETA_BGW_TILE_READp, AXAD_WIN_MODEn);
  /* p26.BEJE*/ wire _BEJE_BG_TILE_READn = not1(_ASUL_BG_TILE_READp);
  /* p25.XUCY*/ wire _XUCY_WIN_TILE_READn = nand2(NETA_BGW_TILE_READp, PORE_WIN_MODEp);

  /*#p26.ASUM*/ BUS_VRAM_An[ 0].tri6_nn(_BEJE_BG_TILE_READn, XUHA_FETCH_HILOp);
  /* p26.EVAD*/ BUS_VRAM_An[ 1].tri6_nn(_BEJE_BG_TILE_READn, scroll_y.FAFO_TILE_Y0S);
  /* p26.DAHU*/ BUS_VRAM_An[ 2].tri6_nn(_BEJE_BG_TILE_READn, scroll_y.EMUX_TILE_Y1S);
  /* p26.DODE*/ BUS_VRAM_An[ 3].tri6_nn(_BEJE_BG_TILE_READn, scroll_y.ECAB_TILE_Y2S);

  /*#p25.XONU*/ BUS_VRAM_An[ 0].tri6_nn(_XUCY_WIN_TILE_READn, XUHA_FETCH_HILOp);
  /*#p25.WUDO*/ BUS_VRAM_An[ 1].tri6_nn(_XUCY_WIN_TILE_READn, win_line_y.VYNO_WIN_Y0.qp_new());
  /*#p25.WAWE*/ BUS_VRAM_An[ 2].tri6_nn(_XUCY_WIN_TILE_READn, win_line_y.VUJO_WIN_Y1.qp_new());
  /*#p25.WOLU*/ BUS_VRAM_An[ 3].tri6_nn(_XUCY_WIN_TILE_READn, win_line_y.VYMU_WIN_Y2.qp_new());

  /*#p25.VAPY*/ BUS_VRAM_An[ 4].tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.RAWU_TILE_DB0p.qp_new());
  /*#p25.SEZU*/ BUS_VRAM_An[ 5].tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POZO_TILE_DB1p.qp_new());
  /*#p25.VEJY*/ BUS_VRAM_An[ 6].tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PYZO_TILE_DB2p.qp_new());
  /*#p25.RUSA*/ BUS_VRAM_An[ 7].tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POXA_TILE_DB3p.qp_new());
  /*#p25.ROHA*/ BUS_VRAM_An[ 8].tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PULO_TILE_DB4p.qp_new());
  /*#p25.RESO*/ BUS_VRAM_An[ 9].tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POJU_TILE_DB5p.qp_new());
  /*#p25.SUVO*/ BUS_VRAM_An[10].tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.POWY_TILE_DB6p.qp_new());
  /*#p25.TOBO*/ BUS_VRAM_An[11].tri6_pn(NETA_BGW_TILE_READp, tile_temp_b.PYJU_TILE_DB7p.qp_new());

  /*#p25.VUZA*/ wire _VUZA_TILE_BANKp = nor2(tile_temp_b.PYJU_TILE_DB7p.qp_new(), WEXU_LCDC_BGTILEp);
  /*#p25.VURY*/ BUS_VRAM_An[12].tri6_pn(NETA_BGW_TILE_READp, _VUZA_TILE_BANKp);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyVramBus::sprite_to_addr(const SpriteStore& sprite_store, const OamTempA& oam_temp_a, const OamTempB& oam_temp_b, wire XUQU_SPRITE_AB, wire SAKY_SFETCHn, wire XYMU_RENDERINGp, wire XYMO_LCDC_SPSIZEp) {
  /*#p29.WUKY*/ wire _WUKY_FLIP_Yp = not1(oam_temp_b.YZOS_OAM_DB6p.qp_new());
  /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not1(XYMO_LCDC_SPSIZEp);

  /*#p29.CYVU*/ wire _CYVU_L0 = xor2(_WUKY_FLIP_Yp, sprite_store.SPR_TRI_L[0].qp_new());
  /*#p29.BORE*/ wire _BORE_L1 = xor2(_WUKY_FLIP_Yp, sprite_store.SPR_TRI_L[1].qp_new());
  /*#p29.BUVY*/ wire _BUVY_L2 = xor2(_WUKY_FLIP_Yp, sprite_store.SPR_TRI_L[2].qp_new());
  /*#p29.WAGO*/ wire _WAGO_L3 = xor2(_WUKY_FLIP_Yp, sprite_store.SPR_TRI_L[3].qp_new());
  /*#p29.GEJY*/ wire _GEJY_L3 = amux2(oam_temp_a.XUSO_OAM_DA0p.qp_new(), _FUFO_LCDC_SPSIZEn, XYMO_LCDC_SPSIZEp, _WAGO_L3);

  /* p29.TEPA*/ wire _TEPA_RENDERINGp = not1(XYMU_RENDERINGp);
  /* p29.TYSO*/ wire _TYSO_SFETCHINGn = or2(SAKY_SFETCHn, _TEPA_RENDERINGp); // def or
  /* p29.TEXY*/ wire _TEXY_SFETCHINGp = not1(_TYSO_SFETCHINGn);
  /* p29.ABON*/ wire _ABON_SFETCHINGn = not1(_TEXY_SFETCHINGp);
  /* p29.ABEM*/ BUS_VRAM_An[ 0].tri6_nn(_ABON_SFETCHINGn, XUQU_SPRITE_AB);
  /* p29.BAXE*/ BUS_VRAM_An[ 1].tri6_nn(_ABON_SFETCHINGn, _CYVU_L0);
  /* p29.ARAS*/ BUS_VRAM_An[ 2].tri6_nn(_ABON_SFETCHINGn, _BORE_L1);
  /* p29.AGAG*/ BUS_VRAM_An[ 3].tri6_nn(_ABON_SFETCHINGn, _BUVY_L2);
  /* p29.FAMU*/ BUS_VRAM_An[ 4].tri6_nn(_ABON_SFETCHINGn, _GEJY_L3);
  /*#p29.FUGY*/ BUS_VRAM_An[ 5].tri6_nn(_ABON_SFETCHINGn, oam_temp_a.XEGU_OAM_DA1p.qp_new());
  /* p29.GAVO*/ BUS_VRAM_An[ 6].tri6_nn(_ABON_SFETCHINGn, oam_temp_a.YJEX_OAM_DA2p.qp_new());
  /* p29.WYGA*/ BUS_VRAM_An[ 7].tri6_nn(_ABON_SFETCHINGn, oam_temp_a.XYJU_OAM_DA3p.qp_new());
  /* p29.WUNE*/ BUS_VRAM_An[ 8].tri6_nn(_ABON_SFETCHINGn, oam_temp_a.YBOG_OAM_DA4p.qp_new());
  /* p29.GOTU*/ BUS_VRAM_An[ 9].tri6_nn(_ABON_SFETCHINGn, oam_temp_a.WYSO_OAM_DA5p.qp_new());
  /* p29.GEGU*/ BUS_VRAM_An[10].tri6_nn(_ABON_SFETCHINGn, oam_temp_a.XOTE_OAM_DA6p.qp_new());
  /* p29.XEHE*/ BUS_VRAM_An[11].tri6_nn(_ABON_SFETCHINGn, oam_temp_a.YZAB_OAM_DA7p.qp_new());
  /* p29.DYSO*/ BUS_VRAM_An[12].tri6_nn(_ABON_SFETCHINGn, 0);   // sprites always in low half of tile store
}

//------------------------------------------------------------------------------------------------------------------------
// PIN_VRAM_A driver

void GateBoyVramBus::addr_to_pins() {
  /* p25.MYFU*/ wire _MYFUp = not1(BUS_VRAM_An[ 0].qp_new());
  /* p25.MASA*/ wire _MASAp = not1(BUS_VRAM_An[ 1].qp_new());
  /* p25.MYRE*/ wire _MYREp = not1(BUS_VRAM_An[ 2].qp_new());
  /* p25.MAVU*/ wire _MAVUp = not1(BUS_VRAM_An[ 3].qp_new());
  /* p25.MEPA*/ wire _MEPAp = not1(BUS_VRAM_An[ 4].qp_new());
  /* p25.MYSA*/ wire _MYSAp = not1(BUS_VRAM_An[ 5].qp_new());
  /* p25.MEWY*/ wire _MEWYp = not1(BUS_VRAM_An[ 6].qp_new());
  /* p25.MUME*/ wire _MUMEp = not1(BUS_VRAM_An[ 7].qp_new());
  /* p25.VOVA*/ wire _VOVAp = not1(BUS_VRAM_An[ 8].qp_new());
  /* p25.VODE*/ wire _VODEp = not1(BUS_VRAM_An[ 9].qp_new());
  /* p25.RUKY*/ wire _RUKYp = not1(BUS_VRAM_An[10].qp_new());
  /* p25.RUMA*/ wire _RUMAp = not1(BUS_VRAM_An[11].qp_new());
  /* p25.REHO*/ wire _REHOp = not1(BUS_VRAM_An[12].qp_new());

  /* p25.LEXE*/ wire _LEXEn = not1(_MYFUp);
  /* p25.LOZU*/ wire _LOZUn = not1(_MASAp);
  /* p25.LACA*/ wire _LACAn = not1(_MYREp);
  /* p25.LUVO*/ wire _LUVOn = not1(_MAVUp);
  /* p25.LOLY*/ wire _LOLYn = not1(_MEPAp);
  /* p25.LALO*/ wire _LALOn = not1(_MYSAp);
  /* p25.LEFA*/ wire _LEFAn = not1(_MEWYp);
  /* p25.LUBY*/ wire _LUBYn = not1(_MUMEp);
  /* p25.TUJY*/ wire _TUJYn = not1(_VOVAp);
  /* p25.TAGO*/ wire _TAGOn = not1(_VODEp);
  /* p25.NUVA*/ wire _NUVAn = not1(_RUKYp);
  /* p25.PEDU*/ wire _PEDUn = not1(_RUMAp);
  /* p25.PONY*/ wire _PONYn = not1(_REHOp);

  PIN_VRAM_Ap[ 0].pin_out(_LEXEn, _LEXEn);
  PIN_VRAM_Ap[ 1].pin_out(_LOZUn, _LOZUn);
  PIN_VRAM_Ap[ 2].pin_out(_LACAn, _LACAn);
  PIN_VRAM_Ap[ 3].pin_out(_LUVOn, _LUVOn);
  PIN_VRAM_Ap[ 4].pin_out(_LOLYn, _LOLYn);
  PIN_VRAM_Ap[ 5].pin_out(_LALOn, _LALOn);
  PIN_VRAM_Ap[ 6].pin_out(_LEFAn, _LEFAn);
  PIN_VRAM_Ap[ 7].pin_out(_LUBYn, _LUBYn);
  PIN_VRAM_Ap[ 8].pin_out(_TUJYn, _TUJYn);
  PIN_VRAM_Ap[ 9].pin_out(_TAGOn, _TAGOn);
  PIN_VRAM_Ap[10].pin_out(_NUVAn, _NUVAn);
  PIN_VRAM_Ap[11].pin_out(_PEDUn, _PEDUn);
  PIN_VRAM_Ap[12].pin_out(_PONYn, _PONYn);
}

//------------------------------------------------------------------------------------------------------------------------
// PIN_VRAM_D driver

void GateBoyVramBus::cpu_data_to_bus_data(const GateBoyCpuBus& cpu_bus, wire SERE_CPU_VRAM_RDp, wire SALE_CPU_VRAM_WRn) {
  /*#p25.RUVY*/ wire _RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn);
  /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, _RUVY_CPU_VRAM_WRp);

  /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn = not1(_SAZO_CBD_TO_VPDp);
  /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp = not1(_RYJE_CBD_TO_VPDn);
  /* p25.ROCY*/ wire _ROCY_CBD_TO_VPDp = and2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
  /* p25.RAHU*/ wire _RAHU_CBD_TO_VPDn = not1(_ROCY_CBD_TO_VPDp);

  /* p25.TEME*/ BUS_VRAM_Dp[0].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[0]);
  /* p25.TEWU*/ BUS_VRAM_Dp[1].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[1]);
  /*#p25.TYGO*/ BUS_VRAM_Dp[2].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[2]);
  /* p25.SOTE*/ BUS_VRAM_Dp[3].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[3]);
  /* p25.SEKE*/ BUS_VRAM_Dp[4].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[4]);
  /* p25.RUJO*/ BUS_VRAM_Dp[5].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[5]);
  /* p25.TOFA*/ BUS_VRAM_Dp[6].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[6]);
  /* p25.SUZA*/ BUS_VRAM_Dp[7].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[7]);
}

void GateBoyVramBus::bus_data_to_pins(wire SERE_CPU_VRAM_RDp, wire SALE_CPU_VRAM_WRn) {
  /*#p25.RUVY*/ wire _RUVY_CPU_VRAM_WRp = not1(SALE_CPU_VRAM_WRn);
  /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, _RUVY_CPU_VRAM_WRp);

  /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn = not1(_SAZO_CBD_TO_VPDp);
  /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp = not1(_RYJE_CBD_TO_VPDn);

  /* p25.ROCY*/ wire _ROCY_CBD_TO_VPDp = and2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
  /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp =  or2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);

  /* p25.RAHU*/ wire _RAHU_CBD_TO_VPDn = not1(_ROCY_CBD_TO_VPDp);
  /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn = not1(_RELA_CBD_TO_VPDp);

  /*#p25.ROVE*/ wire _ROVE_CBD_TO_VPDp = not1(_RAHU_CBD_TO_VPDn);
  /*#p25.ROFA*/ wire _ROFA_CBD_TO_VPDp = not1(_RENA_CBD_TO_VPDn);

  /*#p25.SEFA*/ wire _SEFA_D0p = and2(BUS_VRAM_Dp[0].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SOGO*/ wire _SOGO_D1p = and2(BUS_VRAM_Dp[1].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SEFU*/ wire _SEFU_D2p = and2(BUS_VRAM_Dp[2].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SUNA*/ wire _SUNA_D3p = and2(BUS_VRAM_Dp[3].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SUMO*/ wire _SUMO_D4p = and2(BUS_VRAM_Dp[4].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SAZU*/ wire _SAZU_D5p = and2(BUS_VRAM_Dp[5].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SAMO*/ wire _SAMO_D6p = and2(BUS_VRAM_Dp[6].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SUKE*/ wire _SUKE_D7p = and2(BUS_VRAM_Dp[7].qp_new(), _ROVE_CBD_TO_VPDp);

  /*#p25.SYNU*/ wire _SYNU_D0p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[0].qp_new());
  /* p25.SYMA*/ wire _SYMA_D1p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[1].qp_new());
  /* p25.ROKO*/ wire _ROKO_D2p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[2].qp_new());
  /* p25.SYBU*/ wire _SYBU_D3p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[3].qp_new());
  /* p25.SAKO*/ wire _SAKO_D4p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[4].qp_new());
  /* p25.SEJY*/ wire _SEJY_D5p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[5].qp_new());
  /* p25.SEDO*/ wire _SEDO_D6p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[6].qp_new());
  /* p25.SAWU*/ wire _SAWU_D7p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[7].qp_new());

  /*#p25.REGE*/ wire _REGE_D0n = not1(_SEFA_D0p);
  /* p25.RYKY*/ wire _RYKY_D1n = not1(_SOGO_D1p);
  /* p25.RAZO*/ wire _RAZO_D2n = not1(_SEFU_D2p);
  /* p25.RADA*/ wire _RADA_D3n = not1(_SUNA_D3p);
  /* p25.RYRO*/ wire _RYRO_D4n = not1(_SUMO_D4p);
  /* p25.REVU*/ wire _REVU_D5n = not1(_SAZU_D5p);
  /* p25.REKU*/ wire _REKU_D6n = not1(_SAMO_D6p);
  /* p25.RYZE*/ wire _RYZE_D7n = not1(_SUKE_D7p);

  /*#p25.RURA*/ wire _RURA_D0n = not1(_SYNU_D0p);
  /* p25.RULY*/ wire _RULY_D1n = not1(_SYMA_D1p);
  /* p25.RARE*/ wire _RARE_D2n = not1(_ROKO_D2p);
  /* p25.RODU*/ wire _RODU_D3n = not1(_SYBU_D3p);
  /* p25.RUBE*/ wire _RUBE_D4n = not1(_SAKO_D4p);
  /* p25.RUMU*/ wire _RUMU_D5n = not1(_SEJY_D5p);
  /* p25.RYTY*/ wire _RYTY_D6n = not1(_SEDO_D6p);
  /* p25.RADY*/ wire _RADY_D7n = not1(_SAWU_D7p);

  PIN_VRAM_Dp[0].pin_out(_ROFA_CBD_TO_VPDp, _REGE_D0n, _RURA_D0n);
  PIN_VRAM_Dp[1].pin_out(_ROFA_CBD_TO_VPDp, _RYKY_D1n, _RULY_D1n);
  PIN_VRAM_Dp[2].pin_out(_ROFA_CBD_TO_VPDp, _RAZO_D2n, _RARE_D2n);
  PIN_VRAM_Dp[3].pin_out(_ROFA_CBD_TO_VPDp, _RADA_D3n, _RODU_D3n);
  PIN_VRAM_Dp[4].pin_out(_ROFA_CBD_TO_VPDp, _RYRO_D4n, _RUBE_D4n);
  PIN_VRAM_Dp[5].pin_out(_ROFA_CBD_TO_VPDp, _REVU_D5n, _RUMU_D5n);
  PIN_VRAM_Dp[6].pin_out(_ROFA_CBD_TO_VPDp, _REKU_D6n, _RYTY_D6n);
  PIN_VRAM_Dp[7].pin_out(_ROFA_CBD_TO_VPDp, _RYZE_D7n, _RADY_D7n);
}
