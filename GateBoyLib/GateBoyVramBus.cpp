#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyDMA.h"

#include "GateBoyLib/GateBoyTileFetcher.h"
#include "GateBoyLib/GateBoySpriteStore.h"
#include "GateBoyLib/GateBoyOamBus.h"
#include "GateBoyLib/GateBoyCpuBus.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyVramBus::cpu_addr_to_vram_addr(Signal BUS_CPU_A[16], wire2 XYMU_RENDERINGp, wire2 LUFA_DMA_VRAMp) {
  // CPU to VRAM address driver
  /*#p25.XANE*/ wire2 _XANE_VRAM_LOCKn_new_evn = nor2b(LUFA_DMA_VRAMp, XYMU_RENDERINGp);
  /* p25.XEDU*/ wire2 _XEDU_VRAM_LOCKp_new_evn = not1b(_XANE_VRAM_LOCKn_new_evn);
  /* p25.XAKY*/ BUS_VRAM_An[ 0].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 0].qp_new());
  /* p25.XUXU*/ BUS_VRAM_An[ 1].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 1].qp_new());
  /* p25.XYNE*/ BUS_VRAM_An[ 2].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 2].qp_new());
  /* p25.XODY*/ BUS_VRAM_An[ 3].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 3].qp_new());
  /* p25.XECA*/ BUS_VRAM_An[ 4].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 4].qp_new());
  /* p25.XOBA*/ BUS_VRAM_An[ 5].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 5].qp_new());
  /* p25.XOPO*/ BUS_VRAM_An[ 6].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 6].qp_new());
  /* p25.XYBO*/ BUS_VRAM_An[ 7].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 7].qp_new());
  /* p25.RYSU*/ BUS_VRAM_An[ 8].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 8].qp_new());
  /* p25.RESE*/ BUS_VRAM_An[ 9].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[ 9].qp_new());
  /* p25.RUSE*/ BUS_VRAM_An[10].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[10].qp_new());
  /* p25.RYNA*/ BUS_VRAM_An[11].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[11].qp_new());
  /* p25.RUMO*/ BUS_VRAM_An[12].tri6_nn(_XEDU_VRAM_LOCKp_new_evn, BUS_CPU_A[12].qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyVramBus::dma_addr_to_vram_addr(const GateBoyDMA& dma) {
  /* p04.NAKY*/ wire2 _NAKY_DMA_A00p_new = dma.NAKY_DMA_A00p.qp_new();
  /* p04.PYRO*/ wire2 _PYRO_DMA_A01p_new = dma.PYRO_DMA_A01p.qp_new();
  /* p04.NEFY*/ wire2 _NEFY_DMA_A02p_new = dma.NEFY_DMA_A02p.qp_new();
  /* p04.MUTY*/ wire2 _MUTY_DMA_A03p_new = dma.MUTY_DMA_A03p.qp_new();
  /* p04.NYKO*/ wire2 _NYKO_DMA_A04p_new = dma.NYKO_DMA_A04p.qp_new();
  /* p04.PYLO*/ wire2 _PYLO_DMA_A05p_new = dma.PYLO_DMA_A05p.qp_new();
  /* p04.NUTO*/ wire2 _NUTO_DMA_A06p_new = dma.NUTO_DMA_A06p.qp_new();
  /* p04.MUGU*/ wire2 _MUGU_DMA_A07p_new = dma.MUGU_DMA_A07p.qp_new();
  /* p04.NAFA*/ wire2 _NAFA_DMA_A08p_new = dma.NAFA_DMA_A08n.qn_new();
  /* p04.PYNE*/ wire2 _PYNE_DMA_A09p_new = dma.PYNE_DMA_A09n.qn_new();
  /* p04.PARA*/ wire2 _PARA_DMA_A10p_new = dma.PARA_DMA_A10n.qn_new();
  /* p04.NYDO*/ wire2 _NYDO_DMA_A11p_new = dma.NYDO_DMA_A11n.qn_new();
  /* p04.NYGY*/ wire2 _NYGY_DMA_A12p_new = dma.NYGY_DMA_A12n.qn_new();

  /* p04.AHOC*/ wire2 _AHOC_DMA_VRAMn_new = not1b(dma.LUFA_DMA_VRAMp());
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

void GateBoyVramBus::scroll_to_addr(BGScrollX scroll_x, BGScrollY scroll_y, wire2 POTU_BGW_MAP_READp, wire2 AXAD_WIN_MODEn, wire2 XAFO_LCDC_BGMAPp) {
  /* p26.ACEN*/ wire2 _ACEN_BG_MAP_READp = and2(POTU_BGW_MAP_READp, AXAD_WIN_MODEn);
  /* p26.BAFY*/ wire2 _BAFY_BG_MAP_READn = not1b(_ACEN_BG_MAP_READp);
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


void GateBoyVramBus::win_to_addr(const WinMapX& win_map_x, const WinLineY& win_line_y, wire2 POTU_BGW_MAP_READp, wire2 PORE_WIN_MODEp, wire2 WOKY_LCDC_WINMAPp) {
  /*#p25.XEZE*/ wire2 _XEZE_WIN_MAP_READp = and2(POTU_BGW_MAP_READp, PORE_WIN_MODEp);
  /*#p25.WUKO*/ wire2 _WUKO_WIN_MAP_READn = not1b(_XEZE_WIN_MAP_READp);
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

void GateBoyVramBus::tile_to_addr(const BGScrollY scroll_y, const WinLineY win_line_y, const TileTempB tile_temp_b, wire2 NETA_BGW_TILE_READp, wire2 XUHA_FETCH_HILOp, wire2 WEXU_LCDC_BGTILEp, wire2 PORE_WIN_MODEp, wire2 AXAD_WIN_MODEn) {
  /* p26.ASUL*/ wire2 _ASUL_BG_TILE_READp =  and2(NETA_BGW_TILE_READp, AXAD_WIN_MODEn);
  /* p26.BEJE*/ wire2 _BEJE_BG_TILE_READn = not1b(_ASUL_BG_TILE_READp);
  /* p25.XUCY*/ wire2 _XUCY_WIN_TILE_READn = nand2b(NETA_BGW_TILE_READp, PORE_WIN_MODEp);

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

  /*#p25.VUZA*/ wire2 _VUZA_TILE_BANKp = nor2(tile_temp_b.PYJU_TILE_DB7p.qp_new(), WEXU_LCDC_BGTILEp);
  /*#p25.VURY*/ BUS_VRAM_An[12].tri6_pn(NETA_BGW_TILE_READp, _VUZA_TILE_BANKp);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyVramBus::sprite_to_addr(Bus SPR_TRI_L[4], const OamTempA& oam_temp_a, const OamTempB& oam_temp_b, wire2 XUQU_SPRITE_AB, wire2 SAKY_SFETCHn, wire2 XYMU_RENDERINGp, wire2 XYMO_LCDC_SPSIZEp) {
  /*#p29.WUKY*/ wire2 _WUKY_FLIP_Yp = not1b(oam_temp_b.YZOS_OAM_DB6p.qp_new());
  /*#p29.FUFO*/ wire2 _FUFO_LCDC_SPSIZEn = not1b(XYMO_LCDC_SPSIZEp);

  /*#p29.CYVU*/ wire2 _CYVU_L0 = xor2(_WUKY_FLIP_Yp, SPR_TRI_L[0].qp_new());
  /*#p29.BORE*/ wire2 _BORE_L1 = xor2(_WUKY_FLIP_Yp, SPR_TRI_L[1].qp_new());
  /*#p29.BUVY*/ wire2 _BUVY_L2 = xor2(_WUKY_FLIP_Yp, SPR_TRI_L[2].qp_new());
  /*#p29.WAGO*/ wire2 _WAGO_L3 = xor2(_WUKY_FLIP_Yp, SPR_TRI_L[3].qp_new());
  /*#p29.GEJY*/ wire2 _GEJY_L3 = amux2(oam_temp_a.XUSO_OAM_DA0p.qp_new(), _FUFO_LCDC_SPSIZEn, XYMO_LCDC_SPSIZEp, _WAGO_L3);

  /* p29.TEPA*/ wire2 _TEPA_RENDERINGp = not1b(XYMU_RENDERINGp);
  /* p29.TYSO*/ wire2 _TYSO_SFETCHINGn = or2(SAKY_SFETCHn, _TEPA_RENDERINGp); // def or
  /* p29.TEXY*/ wire2 _TEXY_SFETCHINGp = not1b(_TYSO_SFETCHINGn);
  /* p29.ABON*/ wire2 _ABON_SFETCHINGn = not1b(_TEXY_SFETCHINGp);
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
  /* p25.MYFU*/ wire2 _MYFUp = not1b(BUS_VRAM_An[ 0].qp_new());
  /* p25.MASA*/ wire2 _MASAp = not1b(BUS_VRAM_An[ 1].qp_new());
  /* p25.MYRE*/ wire2 _MYREp = not1b(BUS_VRAM_An[ 2].qp_new());
  /* p25.MAVU*/ wire2 _MAVUp = not1b(BUS_VRAM_An[ 3].qp_new());
  /* p25.MEPA*/ wire2 _MEPAp = not1b(BUS_VRAM_An[ 4].qp_new());
  /* p25.MYSA*/ wire2 _MYSAp = not1b(BUS_VRAM_An[ 5].qp_new());
  /* p25.MEWY*/ wire2 _MEWYp = not1b(BUS_VRAM_An[ 6].qp_new());
  /* p25.MUME*/ wire2 _MUMEp = not1b(BUS_VRAM_An[ 7].qp_new());
  /* p25.VOVA*/ wire2 _VOVAp = not1b(BUS_VRAM_An[ 8].qp_new());
  /* p25.VODE*/ wire2 _VODEp = not1b(BUS_VRAM_An[ 9].qp_new());
  /* p25.RUKY*/ wire2 _RUKYp = not1b(BUS_VRAM_An[10].qp_new());
  /* p25.RUMA*/ wire2 _RUMAp = not1b(BUS_VRAM_An[11].qp_new());
  /* p25.REHO*/ wire2 _REHOp = not1b(BUS_VRAM_An[12].qp_new());

  /* p25.LEXE*/ wire2 _LEXEn = not1b(_MYFUp);
  /* p25.LOZU*/ wire2 _LOZUn = not1b(_MASAp);
  /* p25.LACA*/ wire2 _LACAn = not1b(_MYREp);
  /* p25.LUVO*/ wire2 _LUVOn = not1b(_MAVUp);
  /* p25.LOLY*/ wire2 _LOLYn = not1b(_MEPAp);
  /* p25.LALO*/ wire2 _LALOn = not1b(_MYSAp);
  /* p25.LEFA*/ wire2 _LEFAn = not1b(_MEWYp);
  /* p25.LUBY*/ wire2 _LUBYn = not1b(_MUMEp);
  /* p25.TUJY*/ wire2 _TUJYn = not1b(_VOVAp);
  /* p25.TAGO*/ wire2 _TAGOn = not1b(_VODEp);
  /* p25.NUVA*/ wire2 _NUVAn = not1b(_RUKYp);
  /* p25.PEDU*/ wire2 _PEDUn = not1b(_RUMAp);
  /* p25.PONY*/ wire2 _PONYn = not1b(_REHOp);

  PIN34_VRAM_ADDR[ 0].pin_out_dp(_LEXEn);
  PIN34_VRAM_ADDR[ 1].pin_out_dp(_LOZUn);
  PIN34_VRAM_ADDR[ 2].pin_out_dp(_LACAn);
  PIN34_VRAM_ADDR[ 3].pin_out_dp(_LUVOn);
  PIN34_VRAM_ADDR[ 4].pin_out_dp(_LOLYn);
  PIN34_VRAM_ADDR[ 5].pin_out_dp(_LALOn);
  PIN34_VRAM_ADDR[ 6].pin_out_dp(_LEFAn);
  PIN34_VRAM_ADDR[ 7].pin_out_dp(_LUBYn);
  PIN34_VRAM_ADDR[ 8].pin_out_dp(_TUJYn);
  PIN34_VRAM_ADDR[ 9].pin_out_dp(_TAGOn);
  PIN34_VRAM_ADDR[10].pin_out_dp(_NUVAn);
  PIN34_VRAM_ADDR[11].pin_out_dp(_PEDUn);
  PIN34_VRAM_ADDR[12].pin_out_dp(_PONYn);
}

//------------------------------------------------------------------------------------------------------------------------
// PIN_VRAM_D driver

void GateBoyVramBus::cpu_data_to_bus_data(const GateBoyCpuBus& cpu_bus, wire2 SERE_CPU_VRAM_RDp, wire2 SALE_CPU_VRAM_WRn) {
  /*#p25.RUVY*/ wire2 _RUVY_CPU_VRAM_WRp = not1b(SALE_CPU_VRAM_WRn);
  /*#p25.SAZO*/ wire2 _SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, _RUVY_CPU_VRAM_WRp);

  /*#p25.RYJE*/ wire2 _RYJE_CBD_TO_VPDn = not1b(_SAZO_CBD_TO_VPDp);
  /*#p25.REVO*/ wire2 _REVO_CBD_TO_VPDp = not1b(_RYJE_CBD_TO_VPDn);
  /* p25.ROCY*/ wire2 _ROCY_CBD_TO_VPDp = and2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
  /* p25.RAHU*/ wire2 _RAHU_CBD_TO_VPDn = not1b(_ROCY_CBD_TO_VPDp);

  /* p25.TEME*/ BUS_VRAM_Dp[0].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[0].qp_new());
  /* p25.TEWU*/ BUS_VRAM_Dp[1].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[1].qp_new());
  /*#p25.TYGO*/ BUS_VRAM_Dp[2].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[2].qp_new());
  /* p25.SOTE*/ BUS_VRAM_Dp[3].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[3].qp_new());
  /* p25.SEKE*/ BUS_VRAM_Dp[4].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[4].qp_new());
  /* p25.RUJO*/ BUS_VRAM_Dp[5].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[5].qp_new());
  /* p25.TOFA*/ BUS_VRAM_Dp[6].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[6].qp_new());
  /* p25.SUZA*/ BUS_VRAM_Dp[7].tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D[7].qp_new());
}

void GateBoyVramBus::bus_data_to_pins(wire2 SERE_CPU_VRAM_RDp, wire2 SALE_CPU_VRAM_WRn) {
  /*#p25.RUVY*/ wire2 _RUVY_CPU_VRAM_WRp = not1b(SALE_CPU_VRAM_WRn);
  /*#p25.SAZO*/ wire2 _SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, _RUVY_CPU_VRAM_WRp);

  /*#p25.RYJE*/ wire2 _RYJE_CBD_TO_VPDn = not1b(_SAZO_CBD_TO_VPDp);
  /*#p25.REVO*/ wire2 _REVO_CBD_TO_VPDp = not1b(_RYJE_CBD_TO_VPDn);

  /* p25.ROCY*/ wire2 _ROCY_CBD_TO_VPDp = and2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
  /*#p25.RELA*/ wire2 _RELA_CBD_TO_VPDp =  or2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);

  /* p25.RAHU*/ wire2 _RAHU_CBD_TO_VPDn = not1b(_ROCY_CBD_TO_VPDp);
  /*#p25.RENA*/ wire2 _RENA_CBD_TO_VPDn = not1b(_RELA_CBD_TO_VPDp);

  /*#p25.ROVE*/ wire2 _ROVE_CBD_TO_VPDp = not1b(_RAHU_CBD_TO_VPDn);
  /*#p25.ROFA*/ wire2 _ROFA_CBD_TO_VPDp = not1b(_RENA_CBD_TO_VPDn);

  /*#p25.SEFA*/ wire2 _SEFA_D0p = and2(BUS_VRAM_Dp[0].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SOGO*/ wire2 _SOGO_D1p = and2(BUS_VRAM_Dp[1].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SEFU*/ wire2 _SEFU_D2p = and2(BUS_VRAM_Dp[2].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SUNA*/ wire2 _SUNA_D3p = and2(BUS_VRAM_Dp[3].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SUMO*/ wire2 _SUMO_D4p = and2(BUS_VRAM_Dp[4].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SAZU*/ wire2 _SAZU_D5p = and2(BUS_VRAM_Dp[5].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SAMO*/ wire2 _SAMO_D6p = and2(BUS_VRAM_Dp[6].qp_new(), _ROVE_CBD_TO_VPDp);
  /* p25.SUKE*/ wire2 _SUKE_D7p = and2(BUS_VRAM_Dp[7].qp_new(), _ROVE_CBD_TO_VPDp);

  /*#p25.SYNU*/ wire2 _SYNU_D0p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[0].qp_new());
  /* p25.SYMA*/ wire2 _SYMA_D1p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[1].qp_new());
  /* p25.ROKO*/ wire2 _ROKO_D2p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[2].qp_new());
  /* p25.SYBU*/ wire2 _SYBU_D3p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[3].qp_new());
  /* p25.SAKO*/ wire2 _SAKO_D4p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[4].qp_new());
  /* p25.SEJY*/ wire2 _SEJY_D5p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[5].qp_new());
  /* p25.SEDO*/ wire2 _SEDO_D6p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[6].qp_new());
  /* p25.SAWU*/ wire2 _SAWU_D7p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_Dp[7].qp_new());

  /*#p25.REGE*/ wire2 _REGE_D0n = not1b(_SEFA_D0p);
  /* p25.RYKY*/ wire2 _RYKY_D1n = not1b(_SOGO_D1p);
  /* p25.RAZO*/ wire2 _RAZO_D2n = not1b(_SEFU_D2p);
  /* p25.RADA*/ wire2 _RADA_D3n = not1b(_SUNA_D3p);
  /* p25.RYRO*/ wire2 _RYRO_D4n = not1b(_SUMO_D4p);
  /* p25.REVU*/ wire2 _REVU_D5n = not1b(_SAZU_D5p);
  /* p25.REKU*/ wire2 _REKU_D6n = not1b(_SAMO_D6p);
  /* p25.RYZE*/ wire2 _RYZE_D7n = not1b(_SUKE_D7p);

  /*#p25.RURA*/ wire2 _RURA_D0n = not1b(_SYNU_D0p);
  /* p25.RULY*/ wire2 _RULY_D1n = not1b(_SYMA_D1p);
  /* p25.RARE*/ wire2 _RARE_D2n = not1b(_ROKO_D2p);
  /* p25.RODU*/ wire2 _RODU_D3n = not1b(_SYBU_D3p);
  /* p25.RUBE*/ wire2 _RUBE_D4n = not1b(_SAKO_D4p);
  /* p25.RUMU*/ wire2 _RUMU_D5n = not1b(_SEJY_D5p);
  /* p25.RYTY*/ wire2 _RYTY_D6n = not1b(_SEDO_D6p);
  /* p25.RADY*/ wire2 _RADY_D7n = not1b(_SAWU_D7p);

  PIN25_VRAM_DATA[0].pin_out_pull_hilo(_ROFA_CBD_TO_VPDp, _REGE_D0n, _RURA_D0n);
  PIN25_VRAM_DATA[1].pin_out_pull_hilo(_ROFA_CBD_TO_VPDp, _RYKY_D1n, _RULY_D1n);
  PIN25_VRAM_DATA[2].pin_out_pull_hilo(_ROFA_CBD_TO_VPDp, _RAZO_D2n, _RARE_D2n);
  PIN25_VRAM_DATA[3].pin_out_pull_hilo(_ROFA_CBD_TO_VPDp, _RADA_D3n, _RODU_D3n);
  PIN25_VRAM_DATA[4].pin_out_pull_hilo(_ROFA_CBD_TO_VPDp, _RYRO_D4n, _RUBE_D4n);
  PIN25_VRAM_DATA[5].pin_out_pull_hilo(_ROFA_CBD_TO_VPDp, _REVU_D5n, _RUMU_D5n);
  PIN25_VRAM_DATA[6].pin_out_pull_hilo(_ROFA_CBD_TO_VPDp, _REKU_D6n, _RYTY_D6n);
  PIN25_VRAM_DATA[7].pin_out_pull_hilo(_ROFA_CBD_TO_VPDp, _RYZE_D7n, _RADY_D7n);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyVramBus::set_pin_cs(wire2 TUTO_VRAM_DBGp, wire2 SERE_CPU_VRAM_RDp, wire2 LUFA_DMA_VRAMp, wire2 LENA_BFETCHINGp, wire2 TEXY_SFETCHINGp) {
  /*#p25.SUTU*/ wire2 _SUTU_MCSn = nor4b(LENA_BFETCHINGp, LUFA_DMA_VRAMp, TEXY_SFETCHINGp, SERE_CPU_VRAM_RDp);

  /*#p25.RACO*/ wire2 _RACO_DBG_VRAMn = not1b(TUTO_VRAM_DBGp);
  /* p25.TODE*/ wire2 _TODE_MCSn_A = and2(_SUTU_MCSn, _RACO_DBG_VRAMn);
  /* p25.SEWO*/ wire2 _SEWO_MCSn_D =  or2(_SUTU_MCSn, TUTO_VRAM_DBGp);
  /* p25.SOKY*/ wire2 _SOKY_MCSp_A = not1b(_TODE_MCSn_A);
  /* p25.SETY*/ wire2 _SETY_MCSp_D = not1b(_SEWO_MCSn_D);

  PIN43_VRAM_CSn.pin_out_hilo(_SOKY_MCSp_A, _SETY_MCSp_D); // FIXME not actually using this pin, but we should
}

void GateBoyVramBus::set_pin_wr(wire2 TUTO_VRAM_DBGp, wire2 SERE_CPU_VRAM_RDp, wire2 TUJA_CPU_VRAM_WRp) {
  /* p25.SUDO*/ wire2 _SUDO_MWRp = not1b(/*vram_bus.PIN_VRAM_WRn.qn_new()*/ 1); // Ignoring debug stuff for now
  /* p25.TYJY*/ wire2 _TYJY_VRAM_WRp = mux2p(TUTO_VRAM_DBGp, _SUDO_MWRp, TUJA_CPU_VRAM_WRp);
  /* p25.SOHY*/ wire2 _SOHY_MWRn   = nand2b(_TYJY_VRAM_WRp, SERE_CPU_VRAM_RDp);

  /*#p25.RACO*/ wire2 _RACO_DBG_VRAMn = not1b(TUTO_VRAM_DBGp);
  /* p25.TAXY*/ wire2 _TAXY_MWRn_A = and2(_SOHY_MWRn, _RACO_DBG_VRAMn);
  /* p25.SOFY*/ wire2 _SOFY_MWRn_D =  or2(_SOHY_MWRn, TUTO_VRAM_DBGp);
  /* p25.SYSY*/ wire2 _SYSY_MWRp_A = not1b(_TAXY_MWRn_A);
  /* p25.RAGU*/ wire2 _RAGU_MWRp_D = not1b(_SOFY_MWRn_D);

  PIN49_VRAM_WRn.pin_out_hilo(_SYSY_MWRp_A, _RAGU_MWRp_D);
}

void GateBoyVramBus::set_pin_oe(wire2 TUTO_VRAM_DBGp, wire2 SALE_CPU_VRAM_WRn, wire2 LUFA_DMA_VRAMp, wire2 XYMU_RENDERINGp, wire2 LONY_BFETCHINGp, wire2 SOHO_SPR_VRAM_RDp) {
  /*#p25.XANE*/ wire2 _XANE_VRAM_LOCKn = nor2b(LUFA_DMA_VRAMp, XYMU_RENDERINGp);
  /* p25.RYLU*/ wire2 _RYLU_CPU_VRAM_RDn = nand2b(SALE_CPU_VRAM_WRn, _XANE_VRAM_LOCKn);
  /* p25.RAWA*/ wire2 _RAWA_SPR_VRAM_RDn = not1b(SOHO_SPR_VRAM_RDp);
  /* p27.MYMA*/ wire2 _MYMA_BGW_VRAM_RDn = not1b(LONY_BFETCHINGp);
  /* p25.APAM*/ wire2 _APAM_DMA_VRAMn    = not1b(LUFA_DMA_VRAMp);

  /* p25.RACU*/ wire2 _RACU_MOEn   = and4(_RYLU_CPU_VRAM_RDn, _RAWA_SPR_VRAM_RDn, _MYMA_BGW_VRAM_RDn, _APAM_DMA_VRAMn); // def and

  /*#p25.RACO*/ wire2 _RACO_DBG_VRAMn = not1b(TUTO_VRAM_DBGp);
  /* p25.SEMA*/ wire2 _SEMA_MOEn_A = and2(_RACU_MOEn, _RACO_DBG_VRAMn);
  /* p25.RUTE*/ wire2 _RUTE_MOEn_D =  or2(_RACU_MOEn, TUTO_VRAM_DBGp); // schematic wrong, second input is RACU
  /* p25.REFO*/ wire2 _REFO_MOEn_A = not1b(_SEMA_MOEn_A);
  /* p25.SAHA*/ wire2 _SAHA_MOEn_D = not1b(_RUTE_MOEn_D);
  PIN45_VRAM_OEn.pin_out_hilo(_REFO_MOEn_A, _SAHA_MOEn_D);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyVramBus::read_vram(const uint8_t* vid_ram) {
  uint16_t addr = (uint16_t)BitBase::pack_ext_new(13, PIN34_VRAM_ADDR);
  uint8_t data = 0xFF;

  if (bit(~PIN45_VRAM_OEn.ext_qp_new())) {
    data = vid_ram[addr];
  }

  PIN25_VRAM_DATA[0].pin_in_oedp(~PIN45_VRAM_OEn.ext_qp_new(), bit(data, 0));
  PIN25_VRAM_DATA[1].pin_in_oedp(~PIN45_VRAM_OEn.ext_qp_new(), bit(data, 1));
  PIN25_VRAM_DATA[2].pin_in_oedp(~PIN45_VRAM_OEn.ext_qp_new(), bit(data, 2));
  PIN25_VRAM_DATA[3].pin_in_oedp(~PIN45_VRAM_OEn.ext_qp_new(), bit(data, 3));
  PIN25_VRAM_DATA[4].pin_in_oedp(~PIN45_VRAM_OEn.ext_qp_new(), bit(data, 4));
  PIN25_VRAM_DATA[5].pin_in_oedp(~PIN45_VRAM_OEn.ext_qp_new(), bit(data, 5));
  PIN25_VRAM_DATA[6].pin_in_oedp(~PIN45_VRAM_OEn.ext_qp_new(), bit(data, 6));
  PIN25_VRAM_DATA[7].pin_in_oedp(~PIN45_VRAM_OEn.ext_qp_new(), bit(data, 7));
}

void GateBoyVramBus::write_vram(uint8_t* vid_ram) {
  uint16_t addr= (uint16_t)BitBase::pack_ext_new(13, PIN34_VRAM_ADDR);
  uint8_t data = (uint8_t)BitBase::pack_ext_new(8, PIN25_VRAM_DATA);

  if (bit(~PIN49_VRAM_WRn.ext_qp_new())) {
    vid_ram[addr] = data;
  }
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyVramBus::pins_to_data_bus(wire2 SERE_CPU_VRAM_RDp, wire2 SALE_CPU_VRAM_WRn) {
  /*#p25.RUVY*/ wire2 _RUVY_CPU_VRAM_WRp = not1b(SALE_CPU_VRAM_WRn);
  /*#p25.SAZO*/ wire2 _SAZO_CBD_TO_VPDp = and2(SERE_CPU_VRAM_RDp, _RUVY_CPU_VRAM_WRp);

  /*#p25.RYJE*/ wire2 _RYJE_CBD_TO_VPDn = not1b(_SAZO_CBD_TO_VPDp);
  /*#p25.REVO*/ wire2 _REVO_CBD_TO_VPDp = not1b(_RYJE_CBD_TO_VPDn);
  /*#p25.RELA*/ wire2 _RELA_CBD_TO_VPDp =  or2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
  /*#p25.RENA*/ wire2 _RENA_CBD_TO_VPDn = not1b(_RELA_CBD_TO_VPDp);

  /* p25.RODY*/ BUS_VRAM_Dp[0].tri6_pn(_RENA_CBD_TO_VPDn, PIN25_VRAM_DATA[0].int_qp_new());
  /* p25.REBA*/ BUS_VRAM_Dp[1].tri6_pn(_RENA_CBD_TO_VPDn, PIN25_VRAM_DATA[1].int_qp_new());
  /* p25.RYDO*/ BUS_VRAM_Dp[2].tri6_pn(_RENA_CBD_TO_VPDn, PIN25_VRAM_DATA[2].int_qp_new());
  /* p25.REMO*/ BUS_VRAM_Dp[3].tri6_pn(_RENA_CBD_TO_VPDn, PIN25_VRAM_DATA[3].int_qp_new());
  /* p25.ROCE*/ BUS_VRAM_Dp[4].tri6_pn(_RENA_CBD_TO_VPDn, PIN25_VRAM_DATA[4].int_qp_new());
  /* p25.ROPU*/ BUS_VRAM_Dp[5].tri6_pn(_RENA_CBD_TO_VPDn, PIN25_VRAM_DATA[5].int_qp_new());
  /* p25.RETA*/ BUS_VRAM_Dp[6].tri6_pn(_RENA_CBD_TO_VPDn, PIN25_VRAM_DATA[6].int_qp_new());
  /* p25.RAKU*/ BUS_VRAM_Dp[7].tri6_pn(_RENA_CBD_TO_VPDn, PIN25_VRAM_DATA[7].int_qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyVramBus::data_bus_to_cpu_bus(GateBoyCpuBus& cpu_bus, wire2 SERE_CPU_VRAM_RDp) {
  /* p25.RERY*/ wire2 _RERY_VBUS_D0n = not1b(BUS_VRAM_Dp[0].qp_new());
  /* p25.RUNA*/ wire2 _RUNA_VBUS_D1n = not1b(BUS_VRAM_Dp[1].qp_new());
  /* p25.RONA*/ wire2 _RONA_VBUS_D2n = not1b(BUS_VRAM_Dp[2].qp_new());
  /* p25.RUNO*/ wire2 _RUNO_VBUS_D3n = not1b(BUS_VRAM_Dp[3].qp_new());
  /* p25.SANA*/ wire2 _SANA_VBUS_D4n = not1b(BUS_VRAM_Dp[4].qp_new());
  /* p25.RORO*/ wire2 _RORO_VBUS_D5n = not1b(BUS_VRAM_Dp[5].qp_new());
  /* p25.RABO*/ wire2 _RABO_VBUS_D6n = not1b(BUS_VRAM_Dp[6].qp_new());
  /* p25.SAME*/ wire2 _SAME_VBUS_D7n = not1b(BUS_VRAM_Dp[7].qp_new());

  /* p25.TYVY*/ wire2 _TYVY_VBD_TO_CBDn = nand2b(SERE_CPU_VRAM_RDp, cpu_bus.LEKO_CPU_RDp());
  /* p25.SEBY*/ wire2 _SEBY_VBD_TO_CBDp = not1b(_TYVY_VBD_TO_CBDn);

  /*#p25.RUGA*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_SEBY_VBD_TO_CBDp, _RERY_VBUS_D0n);
  /* p25.ROTA*/ cpu_bus.BUS_CPU_D[1].tri6_pn(_SEBY_VBD_TO_CBDp, _RUNA_VBUS_D1n);
  /* p25.RYBU*/ cpu_bus.BUS_CPU_D[2].tri6_pn(_SEBY_VBD_TO_CBDp, _RONA_VBUS_D2n);
  /* p25.RAJU*/ cpu_bus.BUS_CPU_D[3].tri6_pn(_SEBY_VBD_TO_CBDp, _RUNO_VBUS_D3n);
  /* p25.TYJA*/ cpu_bus.BUS_CPU_D[4].tri6_pn(_SEBY_VBD_TO_CBDp, _SANA_VBUS_D4n);
  /* p25.REXU*/ cpu_bus.BUS_CPU_D[5].tri6_pn(_SEBY_VBD_TO_CBDp, _RORO_VBUS_D5n);
  /* p25.RUPY*/ cpu_bus.BUS_CPU_D[6].tri6_pn(_SEBY_VBD_TO_CBDp, _RABO_VBUS_D6n);
  /* p25.TOKU*/ cpu_bus.BUS_CPU_D[7].tri6_pn(_SEBY_VBD_TO_CBDp, _SAME_VBUS_D7n);
}

//------------------------------------------------------------------------------------------------------------------------
