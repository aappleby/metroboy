#include "GateBoyLib/GateBoyVramBus.h"
#include "GateBoyLib/GateBoyDMA.h"

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
  /* p04.NAKY*/ wire _NAKY_DMA_A00p_new = dma.NAKY_DMA_A00p_evn.qp_new();
  /* p04.PYRO*/ wire _PYRO_DMA_A01p_new = dma.PYRO_DMA_A01p_evn.qp_new();
  /* p04.NEFY*/ wire _NEFY_DMA_A02p_new = dma.NEFY_DMA_A02p_evn.qp_new();
  /* p04.MUTY*/ wire _MUTY_DMA_A03p_new = dma.MUTY_DMA_A03p_evn.qp_new();
  /* p04.NYKO*/ wire _NYKO_DMA_A04p_new = dma.NYKO_DMA_A04p_evn.qp_new();
  /* p04.PYLO*/ wire _PYLO_DMA_A05p_new = dma.PYLO_DMA_A05p_evn.qp_new();
  /* p04.NUTO*/ wire _NUTO_DMA_A06p_new = dma.NUTO_DMA_A06p_evn.qp_new();
  /* p04.MUGU*/ wire _MUGU_DMA_A07p_new = dma.MUGU_DMA_A07p_evn.qp_new();
  /* p04.NAFA*/ wire _NAFA_DMA_A08p_new = dma.NAFA_DMA_A08n_h.qn_new();
  /* p04.PYNE*/ wire _PYNE_DMA_A09p_new = dma.PYNE_DMA_A09n_h.qn_new();
  /* p04.PARA*/ wire _PARA_DMA_A10p_new = dma.PARA_DMA_A10n_h.qn_new();
  /* p04.NYDO*/ wire _NYDO_DMA_A11p_new = dma.NYDO_DMA_A11n_h.qn_new();
  /* p04.NYGY*/ wire _NYGY_DMA_A12p_new = dma.NYGY_DMA_A12n_h.qn_new();

  /* p04.AHOC*/ wire _AHOC_DMA_VRAMn_new = not1(dma.LUFA_DMA_VRAMp_new());
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
