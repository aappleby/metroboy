#include "Sch_VramBus.h"

#include "Sch_Top.h"
#include "Probe.h"

using namespace Schematics;

#define DUMP(A) do { d("%-20s : %d\n", #A, wire(A)); } while(0);

//------------------------------------------------------------------------------

void VramBus::dump(Dumper& d, const SchematicTop& top) const {
  d("---------- VRAM Bus ----------\n");
  d("MAP X                : %d\n", get_map_x(top));
  d("MAP Y                : %d\n", get_map_y(top));

  /*p25.XEDU*/ DUMP(XEDU_CPU_VRAM_RDn);
  /*p04.AHOC*/ DUMP(AHOC_DMA_VRAM_RDn);
  /*p29.ABON*/ DUMP(ABON_SPR_VRM_RDn);
  /*p26.BAFY*/ DUMP(BAFY_BG_MAP_READn);
  /*p25.WUKO*/ DUMP(WUKO_WIN_MAP_READn);
  /*p27.NETA*/ DUMP(NETA_TILE_READp);
  /*p26.ASUL*/ DUMP(ASUL_TILE_READp);
  /*p26.BEJE*/ DUMP(BEJE_BGD_TILE_READn);
  /*p25.XUCY*/ DUMP(XUCY_WIN_TILE_READn);
  /*p25.VUZA*/ DUMP(VUZA_TILE_BANKp);

  int TILE_DA = pack_p(LEGU_TILE_DA0n.qn(), NUDU_TILE_DA1n.qn(), MUKU_TILE_DA2n.qn(), LUZO_TILE_DA3n.qn(),
                   MEGU_TILE_DA4n.qn(), MYJY_TILE_DA5n.qn(), NASA_TILE_DA6n.qn(), NEFO_TILE_DA7n.qn());
  int TILE_DB = pack_p(RAWU_TILE_DB0n.qn(), POZO_TILE_DB1n.qn(), PYZO_TILE_DB2n.qn(), POXA_TILE_DB3n.qn(),
                   PULO_TILE_DB4n.qn(), POJU_TILE_DB5n.qn(), POWY_TILE_DB6n.qn(), PYJU_TILE_DB7n.qn());
  int SPRITE_DA = pack_p(PEFO_SPRITE_DB0n.qn(), ROKA_SPRITE_DB1n.qn(), MYTU_SPRITE_DB2n.qn(), RAMU_SPRITE_DB3n.qn(),
                   SELE_SPRITE_DB4n.qn(), SUTO_SPRITE_DB5n.qn(), RAMA_SPRITE_DB6n.qn(), RYDU_SPRITE_DB7n.qn());
  int SPRITE_DB = pack_p(REWO_SPRITE_DA0n.qn(), PEBA_SPRITE_DA1n.qn(), MOFO_SPRITE_DA2n.qn(), PUDU_SPRITE_DA3n.qn(),
                   SAJA_SPRITE_DA4n.qn(), SUNY_SPRITE_DA5n.qn(), SEMO_SPRITE_DA6n.qn(), SEGA_SPRITE_DA7n.qn());

  d("TILE_DA      : 0x%02x\n", TILE_DA);
  d("TILE_DB      : 0x%02x\n", TILE_DB);
  d("SPRITE_DA    : 0x%02x\n", SPRITE_DA);
  d("SPRITE_DB    : 0x%02x\n", SPRITE_DB);

  d("VRAM BUS ADDR    : %04x %c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    get_bus_addr() | 0x8000,
    _VRAM_BUS_A12.c(), _VRAM_BUS_A11.c(), _VRAM_BUS_A10.c(), _VRAM_BUS_A09.c(),
    _VRAM_BUS_A08.c(), _VRAM_BUS_A07.c(), _VRAM_BUS_A06.c(), _VRAM_BUS_A05.c(),
    _VRAM_BUS_A04.c(), _VRAM_BUS_A03.c(), _VRAM_BUS_A02.c(), _VRAM_BUS_A01.c(),
    _VRAM_BUS_A00.c());

  d("VRAM BUS DATA    : %c%c%c%c%c%c%c%c\n",
    _VRAM_BUS_D7n.c(), _VRAM_BUS_D6n.c(), _VRAM_BUS_D5n.c(), _VRAM_BUS_D4n.c(),
    _VRAM_BUS_D3n.c(), _VRAM_BUS_D2n.c(), _VRAM_BUS_D1n.c(), _VRAM_BUS_D0n.c());

  d("VRAM PIN MCS     : %c%c%c\n", _VRAM_PIN_CS_A.c(), _VRAM_PIN_CS_C.c(), _VRAM_PIN_CS_D.c());
  d("VRAM PIN MOE     : %c%c%c\n", _VRAM_PIN_OE_A.c(), _VRAM_PIN_OE_C.c(), _VRAM_PIN_OE_D.c());
  d("VRAM PIN MWR     : %c%c%c\n", _VRAM_PIN_WR_A.c(), _VRAM_PIN_WR_C.c(), _VRAM_PIN_WR_D.c());
  d("VRAM PIN ADDR    : 0x%04x\n", get_pin_addr() | 0x8000);
  d("VRAM PIN DATA A  : %c%c%c%c%c%c%c%c\n",
    _VRAM_PIN_D7_A.c(), _VRAM_PIN_D6_A.c(), _VRAM_PIN_D5_A.c(), _VRAM_PIN_D4_A.c(),
    _VRAM_PIN_D3_A.c(), _VRAM_PIN_D2_A.c(), _VRAM_PIN_D1_A.c(), _VRAM_PIN_D0_A.c());
  d("VRAM PIN DATA B  : %c%c%c%c%c%c%c%c\n",
    _VRAM_PIN_D7_B.c(), _VRAM_PIN_D6_B.c(), _VRAM_PIN_D5_B.c(), _VRAM_PIN_D4_B.c(),
    _VRAM_PIN_D3_B.c(), _VRAM_PIN_D2_B.c(), _VRAM_PIN_D1_B.c(), _VRAM_PIN_D0_B.c());
  d("VRAM PIN DATA C  : %c%c%c%c%c%c%c%c\n",
    _VRAM_PIN_D7n_C.c(), _VRAM_PIN_D6n_C.c(), _VRAM_PIN_D5n_C.c(), _VRAM_PIN_D4n_C.c(),
    _VRAM_PIN_D3n_C.c(), _VRAM_PIN_D2n_C.c(), _VRAM_PIN_D1n_C.c(), _VRAM_PIN_D0n_C.c());
  d("VRAM PIN DATA D  : %c%c%c%c%c%c%c%c\n",
    _VRAM_PIN_D7_D.c(), _VRAM_PIN_D6_D.c(), _VRAM_PIN_D5_D.c(), _VRAM_PIN_D4_D.c(),
    _VRAM_PIN_D3_D.c(), _VRAM_PIN_D2_D.c(), _VRAM_PIN_D1_D.c(), _VRAM_PIN_D0_D.c());
  d("\n");
}


int VramBus::get_map_x(const SchematicTop& /*top*/) const {
  return pack_p(_BABE_MAP_X0S, _ABOD_MAP_X1S, _BEWY_MAP_X2S, _BYCA_MAP_X3S, _ACUL_MAP_X4S, 0, 0, 0);
}

int VramBus::get_map_y(const SchematicTop& /*top*/) const {
  return pack_p(_ETAM_MAP_Y0S, _DOTO_MAP_Y1S, _DABA_MAP_Y2S, _EFYK_MAP_Y3S, _EJOK_MAP_Y4S, 0, 0, 0);
}

//------------------------------------------------------------------------------

void VramBus::tock(SchematicTop& top) {

  /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXT.qp(), top.cpu_bus.TEVY_8000_9FFFn());
  /*p25.TEFA*/ wire TEFA_8000_9FFFp = nor2(top.cpu_bus.SYRO_FE00_FFFFp(), TEXO_8000_9FFFn);
  /*p25.SOSE*/ wire SOSE_8000_9FFFp = and2(top.cpu_bus.CPU_BUS_A15.qp(), TEFA_8000_9FFFp);

  /*p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY = nor2(top.sprite_fetcher._TULY_SFETCH_S1.qp(), top.sprite_fetcher._VONU_SFETCH_S1_D4.qp());
  /*p29.TYSO*/ wire TYSO_SPRITE_READn = or2(_SAKY_SFETCH_MYSTERY, top.pix_pipe.TEPA_RENDERINGn()); // def or
  /*p29.TEXY*/ wire TEXY_SPR_READ_VRAMp = not1(TYSO_SPRITE_READn);

  /*p29.ABON*/ ABON_SPR_VRM_RDn = not1(TEXY_SPR_READ_VRAMp);

  /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not1(top.tile_fetcher._LONY_BG_FETCH_RUNNINGp.qp());
  /*p27.LENA*/ wire LENA_BGW_VRM_RDp = not1(LUSU_BGW_VRAM_RDn);

  //----------------------------------------
  // VRAM pins

  {
    /*p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp = and2(SOSE_8000_9FFFp, top.APOV_CPU_WRp_xxxDxxxx());
    /*p25.SUDO*/ wire _SUDO_MWRp_C = not1(_VRAM_PIN_WR_C.qp());
    /*p25.TYJY*/ wire _TYJY_DBG_VRAM_WRp = mux2_p(_SUDO_MWRp_C, _TUJA_CPU_VRAM_WRp, top.clk_reg.TUTO_DBG_VRAMp());

    /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn());
    /*p25.TEFY*/ wire _TEFY_VRAM_MCSp = not1(_VRAM_PIN_CS_C.qp());
    /*p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2_p(_TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SERE*/ wire _SERE_CPU_VRM_RDp = and2(_TOLE_VRAM_RDp, top.pix_pipe.ROPY_RENDERINGn());

    /*p25.SOHY*/ wire _SOHY_MWRn = nand2(_TYJY_DBG_VRAM_WRp, _SERE_CPU_VRM_RDp);
    /*p25.TAXY*/ wire _TAXY_MWRn_A = and2(_SOHY_MWRn, top.clk_reg.RACO_DBG_VRAMn());
    /*p25.SOFY*/ wire _SOFY_MWRn_D = or2(_SOHY_MWRn, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SYSY*/ wire _SYSY_MWRp_A = not1(_TAXY_MWRn_A);
    /*p25.RAGU*/ wire _RAGU_MWRp_D = not1(_SOFY_MWRn_D);
    _VRAM_PIN_WR_A = _SYSY_MWRp_A;
    _VRAM_PIN_WR_D = _RAGU_MWRp_D;
  }

  {
    /*p25.XANE*/ wire _XANE_VRAM_LOCKn = nor2(top.dma_reg.LUFA_DMA_VRM_RDp(), top.pix_pipe.XYMU_RENDERINGp()); // def nor4
    /*p25.TAVY*/ wire _TAVY_MOEp = not1(_VRAM_PIN_OE_C.qp());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.CPU_PIN_WRp.qp());  // Schematic wrong, second input is CPU_PIN_WRp
    /*p25.SALE*/ wire _SALE_VRAM_WRn = mux2_p(_TAVY_MOEp, TEGU_CPU_VRAM_WRn, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.RYLU*/ wire _RYLU_CPU_VRAM_RDn = nand2(_SALE_VRAM_WRn, _XANE_VRAM_LOCKn);

    /*p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(top.sprite_fetcher._TOXE_SFETCH_S0.qp());
    /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(top.sprite_fetcher._TYFO_SFETCH_S0_D1.qp(), TYTU_SFETCH_S0n);

    /*p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp = and2(TACU_SPR_SEQ_5_TRIG, TEXY_SPR_READ_VRAMp);
    /*p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn = not1(_SOHO_SPR_VRAM_RDp);
    /*p25.APAM*/ wire _APAM_DMA_VRAM_RDn = not1(top.dma_reg.LUFA_DMA_VRM_RDp());

    /*p27.MYMA*/ wire MYMA_BGW_VRAM_RDn = not1(top.tile_fetcher._LONY_BG_FETCH_RUNNINGp.qp());


    /*p25.RACU*/ wire _RACU_MOEn = and4(_RYLU_CPU_VRAM_RDn, _RAWA_SPR_VRAM_RDn, MYMA_BGW_VRAM_RDn, _APAM_DMA_VRAM_RDn); // def and

    /*p25.SEMA*/ wire _SEMA_MOEn_A = and2(_RACU_MOEn, top.clk_reg.RACO_DBG_VRAMn());
    /*p25.RUTE*/ wire _RUTE_MOEn_D = or2(_RACU_MOEn, top.clk_reg.TUTO_DBG_VRAMp()); // schematic wrong, second input is RACU
    /*p25.REFO*/ wire _REFO_MOEn_A = not1(_SEMA_MOEn_A);
    /*p25.SAHA*/ wire _SAHA_MOEn_D = not1(_RUTE_MOEn_D);
    _VRAM_PIN_OE_A = _REFO_MOEn_A;
    _VRAM_PIN_OE_D = _SAHA_MOEn_D;
  }

  {
    wire LUFA = top.dma_reg.LUFA_DMA_VRM_RDp();

    /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn());
    /*p25.TEFY*/ wire _TEFY_VRAM_MCSp = not1(_VRAM_PIN_CS_C.qp());
    /*p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2_p(_TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SERE*/ wire _SERE_CPU_VRM_RDp = and2(_TOLE_VRAM_RDp, top.pix_pipe.ROPY_RENDERINGn());
  
    // SUTU := nor4(LENA, LUFA, TEXY, SERE)
    /*p25.SUTU*/ wire _SUTU_MCSn = nor4(LENA_BGW_VRM_RDp, LUFA, TEXY_SPR_READ_VRAMp, _SERE_CPU_VRM_RDp);

    wire RACO = top.clk_reg.RACO_DBG_VRAMn();
    wire TUTU = top.clk_reg.TUTO_DBG_VRAMp();

    /*p25.TODE*/ wire _TODE_MCSn_A = and2(_SUTU_MCSn, RACO);
    /*p25.SEWO*/ wire _SEWO_MCSn_D = or2(_SUTU_MCSn, TUTU);
    /*p25.SOKY*/ wire _SOKY_MCSp_A = not1(_TODE_MCSn_A);
    /*p25.SETY*/ wire _SETY_MCSp_D = not1(_SEWO_MCSn_D);
    _VRAM_PIN_CS_A = _SOKY_MCSp_A;
    _VRAM_PIN_CS_D = _SETY_MCSp_D;
  }

  //----------------------------------------
  // VRAM address

  // CPU address -> vram address
  {
    /*p25.XANE*/ wire _XANE_VRAM_LOCKn = nor2(top.dma_reg.LUFA_DMA_VRM_RDp(), top.pix_pipe.XYMU_RENDERINGp());
    /*p25.XEDU*/ XEDU_CPU_VRAM_RDn = not1(_XANE_VRAM_LOCKn);
    /*p25.XAKY*/ _VRAM_BUS_A00 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A00.qp());
    /*p25.XUXU*/ _VRAM_BUS_A01 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A01.qp());
    /*p25.XYNE*/ _VRAM_BUS_A02 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A02.qp());
    /*p25.XODY*/ _VRAM_BUS_A03 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A03.qp());
    /*p25.XECA*/ _VRAM_BUS_A04 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A04.qp());
    /*p25.XOBA*/ _VRAM_BUS_A05 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A05.qp());
    /*p25.XOPO*/ _VRAM_BUS_A06 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A06.qp());
    /*p25.XYBO*/ _VRAM_BUS_A07 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A07.qp());
    /*p25.RYSU*/ _VRAM_BUS_A08 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A08.qp());
    /*p25.RESE*/ _VRAM_BUS_A09 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A09.qp());
    /*p25.RUSE*/ _VRAM_BUS_A10 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A10.qp());
    /*p25.RYNA*/ _VRAM_BUS_A11 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A11.qp());
    /*p25.RUMO*/ _VRAM_BUS_A12 = tribuf_6n(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A12.qp());
  }

  // DMA address -> vram address
  {
     // FIXME proooobabaly need to check qn/qp here

    // EVAX / EREW look the same

    /*p04.AHOC*/ AHOC_DMA_VRAM_RDn = not1(top.dma_reg.LUFA_DMA_VRM_RDp());
    /*p04.ECAL*/ _VRAM_BUS_A00 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp());
    /*p04.EGEZ*/ _VRAM_BUS_A01 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp());
    /*p04.FUHE*/ _VRAM_BUS_A02 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp());
    /*p04.FYZY*/ _VRAM_BUS_A03 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp());
    /*p04.DAMU*/ _VRAM_BUS_A04 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp());
    /*p04.DAVA*/ _VRAM_BUS_A05 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp());
    /*p04.ETEG*/ _VRAM_BUS_A06 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp());
    /*#p04.EREW*/ _VRAM_BUS_A07 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp());

    /*#p04.EVAX*/ _VRAM_BUS_A08 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.NAFA_DMA_A08.qn());
    /*p04.DUVE*/ _VRAM_BUS_A09 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.PYNE_DMA_A09.qn());
    /*p04.ERAF*/ _VRAM_BUS_A10 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.PARA_DMA_A10.qn());
    /*p04.FUSY*/ _VRAM_BUS_A11 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.NYDO_DMA_A11.qn());
    /*p04.EXYF*/ _VRAM_BUS_A12 = tribuf_6n(AHOC_DMA_VRAM_RDn, top.dma_reg.NYGY_DMA_A12.qn());
  }

  // Sprite fetcher read
  {
    /*#p29.WUKY*/ wire _WUKY_FLIP_Yp = not1(top.oam_bus.YZOS_OAM_DB6n.qp());
    
    /*#p29.CYVU*/ wire _CYVU_L0 = xor2_gnd(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L0.qp());
    /*#p29.BORE*/ wire _BORE_L1 = xor2_gnd(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L1.qp());
    /*#p29.BUVY*/ wire _BUVY_L2 = xor2_gnd(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L2.qp());
    /*#p29.WAGO*/ wire _WAGO_L3 = xor2_gnd(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L3.qp());
    
    /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEp = not1(top.pix_pipe.XYMO_LCDC_SPSIZEn.qn());
    /*#p29.GEJY*/ wire _GEJY_L3 = amux2(top.oam_bus.XUSO_OAM_DA0n.qp(), _FUFO_LCDC_SPSIZEp,
                                        top.pix_pipe.XYMO_LCDC_SPSIZEn.qn(), _WAGO_L3);

    /*#p29.XUQU*/ wire XUQU_SPRITE_AB = not1(top.sprite_fetcher._VONU_SFETCH_S1_D4.qn());

    /*p29.ABEM*/ _VRAM_BUS_A00 = tribuf_6n(ABON_SPR_VRM_RDn, XUQU_SPRITE_AB);
    /*p29.BAXE*/ _VRAM_BUS_A01 = tribuf_6n(ABON_SPR_VRM_RDn, _CYVU_L0);
    /*p29.ARAS*/ _VRAM_BUS_A02 = tribuf_6n(ABON_SPR_VRM_RDn, _BORE_L1);
    /*p29.AGAG*/ _VRAM_BUS_A03 = tribuf_6n(ABON_SPR_VRM_RDn, _BUVY_L2);
    /*p29.FAMU*/ _VRAM_BUS_A04 = tribuf_6n(ABON_SPR_VRM_RDn, _GEJY_L3);

    /*#p29.FUGY*/ _VRAM_BUS_A05 = tribuf_6n(ABON_SPR_VRM_RDn, top.oam_bus.XEGU_OAM_DA1n.qp());
    /*p29.GAVO*/ _VRAM_BUS_A06 = tribuf_6n(ABON_SPR_VRM_RDn, top.oam_bus.YJEX_OAM_DA2n.qp());
    /*p29.WYGA*/ _VRAM_BUS_A07 = tribuf_6n(ABON_SPR_VRM_RDn, top.oam_bus.XYJU_OAM_DA3n.qp());
    /*p29.WUNE*/ _VRAM_BUS_A08 = tribuf_6n(ABON_SPR_VRM_RDn, top.oam_bus.YBOG_OAM_DA4n.qp());
    /*p29.GOTU*/ _VRAM_BUS_A09 = tribuf_6n(ABON_SPR_VRM_RDn, top.oam_bus.WYSO_OAM_DA5n.qp());
    /*p29.GEGU*/ _VRAM_BUS_A10 = tribuf_6n(ABON_SPR_VRM_RDn, top.oam_bus.XOTE_OAM_DA6n.qp());
    /*p29.XEHE*/ _VRAM_BUS_A11 = tribuf_6n(ABON_SPR_VRM_RDn, top.oam_bus.YZAB_OAM_DA7n.qp());
    /*p29.DYSO*/ _VRAM_BUS_A12 = tribuf_6n(ABON_SPR_VRM_RDn, 0);   // sprites always in low half of tile store
  }

  {
    /*p26.FAFO*/ _FAFO_TILE_Y0S = add_s(top.lcd_reg.MUWY_Y0p.qp(), top.pix_pipe.GAVE_SCY0.qp(), 0);
    /*p26.FAFO*/ _FAFO_TILE_Y0C = add_c(top.lcd_reg.MUWY_Y0p.qp(), top.pix_pipe.GAVE_SCY0.qp(), 0);
    /*p26.EMUX*/ _EMUX_TILE_Y1S = add_s(top.lcd_reg.MYRO_Y1p.qp(), top.pix_pipe.FYMO_SCY1.qp(), _FAFO_TILE_Y0C);
    /*p26.EMUX*/ _EMUX_TILE_Y1C = add_c(top.lcd_reg.MYRO_Y1p.qp(), top.pix_pipe.FYMO_SCY1.qp(), _FAFO_TILE_Y0C);
    /*p26.ECAB*/ _ECAB_TILE_Y2S = add_s(top.lcd_reg.LEXA_Y2p.qp(), top.pix_pipe.FEZU_SCY2.qp(), _EMUX_TILE_Y1C);
    /*p26.ECAB*/ _ECAB_TILE_Y2C = add_c(top.lcd_reg.LEXA_Y2p.qp(), top.pix_pipe.FEZU_SCY2.qp(), _EMUX_TILE_Y1C);
    /*p26.ETAM*/ _ETAM_MAP_Y0S  = add_s(top.lcd_reg.LYDO_Y3p.qp(), top.pix_pipe.FUJO_SCY3.qp(), _ECAB_TILE_Y2C);
    /*p26.ETAM*/ _ETAM_MAP_Y0C  = add_c(top.lcd_reg.LYDO_Y3p.qp(), top.pix_pipe.FUJO_SCY3.qp(), _ECAB_TILE_Y2C);
    /*p26.DOTO*/ _DOTO_MAP_Y1S  = add_s(top.lcd_reg.LOVU_Y4p.qp(), top.pix_pipe.DEDE_SCY4.qp(), _ETAM_MAP_Y0C);
    /*p26.DOTO*/ _DOTO_MAP_Y1C  = add_c(top.lcd_reg.LOVU_Y4p.qp(), top.pix_pipe.DEDE_SCY4.qp(), _ETAM_MAP_Y0C);
    /*p26.DABA*/ _DABA_MAP_Y2S  = add_s(top.lcd_reg.LEMA_Y5p.qp(), top.pix_pipe.FOTY_SCY5.qp(), _DOTO_MAP_Y1C);
    /*p26.DABA*/ _DABA_MAP_Y2C  = add_c(top.lcd_reg.LEMA_Y5p.qp(), top.pix_pipe.FOTY_SCY5.qp(), _DOTO_MAP_Y1C);
    /*p26.EFYK*/ _EFYK_MAP_Y3S  = add_s(top.lcd_reg.MATO_Y6p.qp(), top.pix_pipe.FOHA_SCY6.qp(), _DABA_MAP_Y2C);
    /*p26.EFYK*/ _EFYK_MAP_Y3C  = add_c(top.lcd_reg.MATO_Y6p.qp(), top.pix_pipe.FOHA_SCY6.qp(), _DABA_MAP_Y2C);
    /*p26.EJOK*/ _EJOK_MAP_Y4S  = add_s(top.lcd_reg.LAFO_Y7p.qp(), top.pix_pipe.FUNY_SCY7.qp(), _EFYK_MAP_Y3C);
    /*p26.EJOK*/ _EJOK_MAP_Y4C  = add_c(top.lcd_reg.LAFO_Y7p.qp(), top.pix_pipe.FUNY_SCY7.qp(), _EFYK_MAP_Y3C);

    /*p26.ATAD*/ _ATAD_TILE_X0S = add_s(top.pix_pipe.XEHO_X0p.qp(), top.pix_pipe.DATY_SCX0.qp(), 0);
    /*p26.ATAD*/ _ATAD_TILE_X0C = add_c(top.pix_pipe.XEHO_X0p.qp(), top.pix_pipe.DATY_SCX0.qp(), 0);
    /*p26.BEHU*/ _BEHU_TILE_X1S = add_s(top.pix_pipe.SAVY_X1p.qp(), top.pix_pipe.DUZU_SCX1.qp(), _ATAD_TILE_X0C);
    /*p26.BEHU*/ _BEHU_TILE_X1C = add_c(top.pix_pipe.SAVY_X1p.qp(), top.pix_pipe.DUZU_SCX1.qp(), _ATAD_TILE_X0C);
    /*p26.APYH*/ _APYH_TILE_X2S = add_s(top.pix_pipe.XODU_X2p.qp(), top.pix_pipe.CYXU_SCX2.qp(), _BEHU_TILE_X1C);
    /*p26.APYH*/ _APYH_TILE_X2C = add_c(top.pix_pipe.XODU_X2p.qp(), top.pix_pipe.CYXU_SCX2.qp(), _BEHU_TILE_X1C);
    /*p26.BABE*/ _BABE_MAP_X0S  = add_s(top.pix_pipe.XYDO_X3p.qp(), top.pix_pipe.GUBO_SCX3.qp(), _APYH_TILE_X2C);
    /*p26.BABE*/ _BABE_MAP_X0C  = add_c(top.pix_pipe.XYDO_X3p.qp(), top.pix_pipe.GUBO_SCX3.qp(), _APYH_TILE_X2C);
    /*p26.ABOD*/ _ABOD_MAP_X1S  = add_s(top.pix_pipe.TUHU_X4p.qp(), top.pix_pipe.BEMY_SCX4.qp(), _BABE_MAP_X0C);
    /*p26.ABOD*/ _ABOD_MAP_X1C  = add_c(top.pix_pipe.TUHU_X4p.qp(), top.pix_pipe.BEMY_SCX4.qp(), _BABE_MAP_X0C);
    /*p26.BEWY*/ _BEWY_MAP_X2S  = add_s(top.pix_pipe.TUKY_X5p.qp(), top.pix_pipe.CUZY_SCX5.qp(), _ABOD_MAP_X1C);
    /*p26.BEWY*/ _BEWY_MAP_X2C  = add_c(top.pix_pipe.TUKY_X5p.qp(), top.pix_pipe.CUZY_SCX5.qp(), _ABOD_MAP_X1C);
    /*p26.BYCA*/ _BYCA_MAP_X3S  = add_s(top.pix_pipe.TAKO_X6p.qp(), top.pix_pipe.CABU_SCX6.qp(), _BEWY_MAP_X2C);
    /*p26.BYCA*/ _BYCA_MAP_X3C  = add_c(top.pix_pipe.TAKO_X6p.qp(), top.pix_pipe.CABU_SCX6.qp(), _BEWY_MAP_X2C);
    /*p26.ACUL*/ _ACUL_MAP_X4S  = add_s(top.pix_pipe.SYBE_X7p.qp(), top.pix_pipe.BAKE_SCX7.qp(), _BYCA_MAP_X3C);
    /*p26.ACUL*/ _ACUL_MAP_X4C  = add_c(top.pix_pipe.SYBE_X7p.qp(), top.pix_pipe.BAKE_SCX7.qp(), _BYCA_MAP_X3C);

    // Background map read
    {
      /*p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(top.tile_fetcher._MESU_BFETCH_S1.qp());
      /*p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(top.tile_fetcher._NYVA_BFETCH_S2.qp());

      /*p26.AXAD*/ wire _AXAD_WIN_MODEn = not1(top.pix_pipe.PORE_WIN_MODEp());
      /*p27.NOGU*/ wire _NOGU_FETCH_01p = nand2(NAKO_BFETCH_S1n, NOFU_BFETCH_S2n);
      /*p27.NENY*/ wire _NENY_FETCH_01n = not1(_NOGU_FETCH_01p);
      /*p27.POTU*/ wire _POTU_BG_MAP_READp = and2(LENA_BGW_VRM_RDp, _NENY_FETCH_01n);
      /*p26.ACEN*/ wire _ACEN_BG_MAP_READp = and2(_POTU_BG_MAP_READp, _AXAD_WIN_MODEn);

      // Fires on phases 0-3 of tile read
      /*p26.BAFY*/ BAFY_BG_MAP_READn = not1(_ACEN_BG_MAP_READp);

      /*p26.AXEP*/ _VRAM_BUS_A00 = tribuf_6n(BAFY_BG_MAP_READn, _BABE_MAP_X0S);
      /*p26.AFEB*/ _VRAM_BUS_A01 = tribuf_6n(BAFY_BG_MAP_READn, _ABOD_MAP_X1S);
      /*p26.ALEL*/ _VRAM_BUS_A02 = tribuf_6n(BAFY_BG_MAP_READn, _BEWY_MAP_X2S);
      /*p26.COLY*/ _VRAM_BUS_A03 = tribuf_6n(BAFY_BG_MAP_READn, _BYCA_MAP_X3S);
      /*p26.AJAN*/ _VRAM_BUS_A04 = tribuf_6n(BAFY_BG_MAP_READn, _ACUL_MAP_X4S);
      /*p26.DUHO*/ _VRAM_BUS_A05 = tribuf_6n(BAFY_BG_MAP_READn, _ETAM_MAP_Y0S);
      /*p26.CASE*/ _VRAM_BUS_A06 = tribuf_6n(BAFY_BG_MAP_READn, _DOTO_MAP_Y1S);
      /*p26.CYPO*/ _VRAM_BUS_A07 = tribuf_6n(BAFY_BG_MAP_READn, _DABA_MAP_Y2S);
      /*p26.CETA*/ _VRAM_BUS_A08 = tribuf_6n(BAFY_BG_MAP_READn, _EFYK_MAP_Y3S);
      /*p26.DAFE*/ _VRAM_BUS_A09 = tribuf_6n(BAFY_BG_MAP_READn, _EJOK_MAP_Y4S);
      /*#p26.AMUV*/ _VRAM_BUS_A10 = tribuf_6n(BAFY_BG_MAP_READn, top.pix_pipe.XAFO_LCDC_BGMAPn.qn());
      /*p26.COVE*/ _VRAM_BUS_A11 = tribuf_6n(BAFY_BG_MAP_READn, 1);
      /*p26.COXO*/ _VRAM_BUS_A12 = tribuf_6n(BAFY_BG_MAP_READn, 1);
    }

    // Window map read
    {
      // CHECK QP/QN

      /*#p27.NOCU*/ wire NOCU_WIN_MODEn = not1(top.pix_pipe.PYNU_WIN_MODE_A.qp());
      /*#p27.PORE*/ wire PORE_WIN_MODEp = not1(NOCU_WIN_MODEn);
      /*#p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(top.tile_fetcher._MESU_BFETCH_S1.qp());
      /*#p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(top.tile_fetcher._NYVA_BFETCH_S2.qp());

      /*#p27.NOGU*/ wire NOGU_FETCH_01p = nand2(NAKO_BFETCH_S1n, NOFU_BFETCH_S2n);
      /*#p27.NENY*/ wire NENY_FETCH_01n = not1(NOGU_FETCH_01p);

      /*#p27.POTU*/ wire POTU_BG_MAP_READp = and2(NENY_FETCH_01n, LENA_BGW_VRM_RDp);

      /*#p25.XEZE*/ wire XEZE_WIN_MAP_READp = and2(POTU_BG_MAP_READp, PORE_WIN_MODEp);
      /*#p25.WUKO*/ WUKO_WIN_MAP_READn = not1(XEZE_WIN_MAP_READp);
      /*#p27.XEJA*/ _VRAM_BUS_A00 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.WYKA_WIN_X3.qp());
      /*p27.XAMO*/ _VRAM_BUS_A01 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.WODY_WIN_X4.qp());
      /*p27.XAHE*/ _VRAM_BUS_A02 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.WOBO_WIN_X5.qp());
      /*p27.XULO*/ _VRAM_BUS_A03 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.WYKO_WIN_X6.qp());
      /*p27.WUJU*/ _VRAM_BUS_A04 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.XOLO_WIN_X7.qp());
      /*#p27.VYTO*/ _VRAM_BUS_A05 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.TUFU_WIN_Y3.qp());
      /*p27.VEHA*/ _VRAM_BUS_A06 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.TAXA_WIN_Y4.qp());
      /*p27.VACE*/ _VRAM_BUS_A07 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.TOZO_WIN_Y5.qp());
      /*p27.VOVO*/ _VRAM_BUS_A08 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.TATE_WIN_Y6.qp());
      /*p27.VULO*/ _VRAM_BUS_A09 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.TEKE_WIN_Y7.qp());
      /*#p27.VEVY*/ _VRAM_BUS_A10 = tribuf_6n(WUKO_WIN_MAP_READn, top.pix_pipe.WOKY_LCDC_WINMAP.qn());
      /*p27.VEZA*/ _VRAM_BUS_A11 = tribuf_6n(WUKO_WIN_MAP_READn, 1);
      /*p27.VOGU*/ _VRAM_BUS_A12 = tribuf_6n(WUKO_WIN_MAP_READn, 1);
    }

    // Background/window tile read
    {
      // CHECK QP/QN

      /*#p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(top.tile_fetcher._MESU_BFETCH_S1.qp());
      /*#p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(top.tile_fetcher._NYVA_BFETCH_S2.qp());
      /*p27.NOCU*/ wire NOCU_WIN_MODEn = not1(top.pix_pipe.PYNU_WIN_MODE_A.qp()); // need to check pynu first
      /*#p27.PORE*/ wire PORE_WIN_MODEp = not1(NOCU_WIN_MODEn);
      /*#p26.AXAD*/ wire AXAD_WIN_MODEn = not1(PORE_WIN_MODEp);

      /*#p27.NOGU*/ wire NOGU_FETCH_01p = nand2(NAKO_BFETCH_S1n, NOFU_BFETCH_S2n);
      /*#p27.XUHA*/ wire XUHA_FETCH_S2p  = not1(NOFU_BFETCH_S2n);

      // Fires on phases 4-11 of tile read
      /*p27.NETA*/ NETA_TILE_READp = and2(LENA_BGW_VRM_RDp, NOGU_FETCH_01p);
      /*p26.ASUL*/ ASUL_TILE_READp = and2(NETA_TILE_READp, AXAD_WIN_MODEn);
      /*p26.BEJE*/ BEJE_BGD_TILE_READn = not1(ASUL_TILE_READp);

      /*p25.XUCY*/ XUCY_WIN_TILE_READn = nand2(NETA_TILE_READp, PORE_WIN_MODEp);

      /*p25.VUZA*/ VUZA_TILE_BANKp = nor2(PYJU_TILE_DB7n.qn(), top.pix_pipe.WEXU_LCDC_BGTILEn.qn());
      
      /*#p26.ASUM*/ _VRAM_BUS_A00 = tribuf_6n(BEJE_BGD_TILE_READn, XUHA_FETCH_S2p);
      /*p26.EVAD*/ _VRAM_BUS_A01 = tribuf_6n(BEJE_BGD_TILE_READn, _FAFO_TILE_Y0S);
      /*p26.DAHU*/ _VRAM_BUS_A02 = tribuf_6n(BEJE_BGD_TILE_READn, _EMUX_TILE_Y1S);
      /*p26.DODE*/ _VRAM_BUS_A03 = tribuf_6n(BEJE_BGD_TILE_READn, _ECAB_TILE_Y2S); // check outputs of ECAB

      /*#p25.XONU*/ _VRAM_BUS_A00 = tribuf_6n(XUCY_WIN_TILE_READn, XUHA_FETCH_S2p);
      /*#p25.WUDO*/ _VRAM_BUS_A01 = tribuf_6n(XUCY_WIN_TILE_READn, top.pix_pipe.VYNO_WIN_Y0.qp());
      /*#p25.WAWE*/ _VRAM_BUS_A02 = tribuf_6n(XUCY_WIN_TILE_READn, top.pix_pipe.VUJO_WIN_Y1.qp());
      /*#p25.WOLU*/ _VRAM_BUS_A03 = tribuf_6n(XUCY_WIN_TILE_READn, top.pix_pipe.VYMU_WIN_Y2.qp());
      /*#p25.VAPY*/ _VRAM_BUS_A04 = tribuf_6p(NETA_TILE_READp, RAWU_TILE_DB0n.qn());
      /*#p25.SEZU*/ _VRAM_BUS_A05 = tribuf_6p(NETA_TILE_READp, POZO_TILE_DB1n.qn());
      /*#p25.VEJY*/ _VRAM_BUS_A06 = tribuf_6p(NETA_TILE_READp, PYZO_TILE_DB2n.qn());
      /*#p25.RUSA*/ _VRAM_BUS_A07 = tribuf_6p(NETA_TILE_READp, POXA_TILE_DB3n.qn());
      /*#p25.ROHA*/ _VRAM_BUS_A08 = tribuf_6p(NETA_TILE_READp, PULO_TILE_DB4n.qn());
      /*#p25.RESO*/ _VRAM_BUS_A09 = tribuf_6p(NETA_TILE_READp, POJU_TILE_DB5n.qn());
      /*#p25.SUVO*/ _VRAM_BUS_A10 = tribuf_6p(NETA_TILE_READp, POWY_TILE_DB6n.qn());
      /*#p25.TOBO*/ _VRAM_BUS_A11 = tribuf_6p(NETA_TILE_READp, PYJU_TILE_DB7n.qn());


      /*#p25.VURY*/ _VRAM_BUS_A12 = tribuf_6p(NETA_TILE_READp, VUZA_TILE_BANKp);
    }
  }

  // VRAM addr bus -> VRAM addr pin
  {
    /*p25.MYFU*/ wire _MYFU = not1(_VRAM_BUS_A00.qp());
    /*p25.MASA*/ wire _MASA = not1(_VRAM_BUS_A01.qp());
    /*p25.MYRE*/ wire _MYRE = not1(_VRAM_BUS_A02.qp());
    /*p25.MAVU*/ wire _MAVU = not1(_VRAM_BUS_A03.qp());
    /*p25.MEPA*/ wire _MEPA = not1(_VRAM_BUS_A04.qp());
    /*p25.MYSA*/ wire _MYSA = not1(_VRAM_BUS_A05.qp());
    /*p25.MEWY*/ wire _MEWY = not1(_VRAM_BUS_A06.qp());
    /*p25.MUME*/ wire _MUME = not1(_VRAM_BUS_A07.qp());
    /*p25.VOVA*/ wire _VOVA = not1(_VRAM_BUS_A08.qp());
    /*p25.VODE*/ wire _VODE = not1(_VRAM_BUS_A09.qp());
    /*p25.RUKY*/ wire _RUKY = not1(_VRAM_BUS_A10.qp());
    /*p25.RUMA*/ wire _RUMA = not1(_VRAM_BUS_A11.qp());
    /*p25.REHO*/ wire _REHO = not1(_VRAM_BUS_A12.qp());

    /*p25.LEXE*/ _VRAM_PIN_A00 = _MYFU;
    /*p25.LOZU*/ _VRAM_PIN_A01 = _MASA;
    /*p25.LACA*/ _VRAM_PIN_A02 = _MYRE;
    /*p25.LUVO*/ _VRAM_PIN_A03 = _MAVU;
    /*p25.LOLY*/ _VRAM_PIN_A04 = _MEPA;
    /*p25.LALO*/ _VRAM_PIN_A05 = _MYSA;
    /*p25.LEFA*/ _VRAM_PIN_A06 = _MEWY;
    /*p25.LUBY*/ _VRAM_PIN_A07 = _MUME;
    /*p25.TUJY*/ _VRAM_PIN_A08 = _VOVA;
    /*p25.TAGO*/ _VRAM_PIN_A09 = _VODE;
    /*p25.NUVA*/ _VRAM_PIN_A10 = _RUKY;
    /*p25.PEDU*/ _VRAM_PIN_A11 = _RUMA;
    /*p25.PONY*/ _VRAM_PIN_A12 = _REHO;
  }

  //----------------------------------------
  // VRAM data out

  // CPU bus -> VRAM bus -> VRAM pin. VRAM bus is _not_ _inverted_ during writes.
  {
    /*p25.TAVY*/ wire _TAVY_MOEp = not1(_VRAM_PIN_OE_C.qp());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.CPU_PIN_WRp.qp());  // Schematic wrong, second input is CPU_PIN_WRp
    /*p25.SALE*/ wire _SALE_CPU_VRAM_WRn = mux2_p(_TAVY_MOEp, TEGU_CPU_VRAM_WRn, top.clk_reg.TUTO_DBG_VRAMp());

    /*p25.TUCA*/ wire _TUCA_CPU_VRAM_AVn = and2(SOSE_8000_9FFFp, top.ABUZ_AVn());
    /*p25.TEFY*/ wire _TEFY_VRAM_MCSp = not1(_VRAM_PIN_CS_C.qp());
    /*p25.TOLE*/ wire _TOLE_VRAM_AVn     = mux2_p(_TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_AVn, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SERE*/ wire _SERE = and2(_TOLE_VRAM_AVn, top.pix_pipe.ROPY_RENDERINGn());
    /*p25.RUVY*/ wire _RUVY_VRAM_WRp = not1(_SALE_CPU_VRAM_WRn);
    /*p25.SAZO*/ wire _SAZO_CBD_TO_VPDp = and2(_RUVY_VRAM_WRp, _SERE);
    /*p25.RYJE*/ wire _RYJE_CBD_TO_VPDn = not1(_SAZO_CBD_TO_VPDp);
    /*p25.REVO*/ wire _REVO_CBD_TO_VPDp = not1(_RYJE_CBD_TO_VPDn);
    /*p25.ROCY*/ wire _ROCY_CBD_TO_VPDp = and2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
    /*p25.RAHU*/ wire _RAHU_CBD_TO_VPDn = not1(_ROCY_CBD_TO_VPDp);

    /*p25.TEME*/ _VRAM_BUS_D0n = tribuf_10n(_RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D0.qp());
    /*p25.TEWU*/ _VRAM_BUS_D1n = tribuf_10n(_RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D1.qp());
    /*p25.TYGO*/ _VRAM_BUS_D2n = tribuf_10n(_RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D2.qp());
    /*p25.SOTE*/ _VRAM_BUS_D3n = tribuf_10n(_RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D3.qp());
    /*p25.SEKE*/ _VRAM_BUS_D4n = tribuf_10n(_RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D4.qp());
    /*p25.RUJO*/ _VRAM_BUS_D5n = tribuf_10n(_RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D5.qp());
    /*p25.TOFA*/ _VRAM_BUS_D6n = tribuf_10n(_RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D6.qp());
    /*p25.SUZA*/ _VRAM_BUS_D7n = tribuf_10n(_RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D7.qp());

    {
      /*p25.ROVE*/ wire _ROVE_CBUS_TO_VPINp = not1(_RAHU_CBD_TO_VPDn);
      /*p25.SEFA*/ wire _SEFA = and2(_VRAM_BUS_D0n.qp(), _ROVE_CBUS_TO_VPINp);
      /*p25.SOGO*/ wire _SOGO = and2(_VRAM_BUS_D1n.qp(), _ROVE_CBUS_TO_VPINp);
      /*p25.SEFU*/ wire _SEFU = and2(_VRAM_BUS_D2n.qp(), _ROVE_CBUS_TO_VPINp);
      /*p25.SUNA*/ wire _SUNA = and2(_VRAM_BUS_D3n.qp(), _ROVE_CBUS_TO_VPINp);
      /*p25.SUMO*/ wire _SUMO = and2(_VRAM_BUS_D4n.qp(), _ROVE_CBUS_TO_VPINp);
      /*p25.SAZU*/ wire _SAZU = and2(_VRAM_BUS_D5n.qp(), _ROVE_CBUS_TO_VPINp);
      /*p25.SAMO*/ wire _SAMO = and2(_VRAM_BUS_D6n.qp(), _ROVE_CBUS_TO_VPINp);
      /*p25.SUKE*/ wire _SUKE = and2(_VRAM_BUS_D7n.qp(), _ROVE_CBUS_TO_VPINp);

      /*p25.REGE*/ wire _REGE = not1(_SEFA);
      /*p25.RYKY*/ wire _RYKY = not1(_SOGO);
      /*p25.RAZO*/ wire _RAZO = not1(_SEFU);
      /*p25.RADA*/ wire _RADA = not1(_SUNA);
      /*p25.RYRO*/ wire _RYRO = not1(_SUMO);
      /*p25.REVU*/ wire _REVU = not1(_SAZU);
      /*p25.REKU*/ wire _REKU = not1(_SAMO);
      /*p25.RYZE*/ wire _RYZE = not1(_SUKE);

      _VRAM_PIN_D0_A = _REGE;
      _VRAM_PIN_D1_A = _RYKY;
      _VRAM_PIN_D2_A = _RAZO;
      _VRAM_PIN_D3_A = _RADA;
      _VRAM_PIN_D4_A = _RYRO;
      _VRAM_PIN_D5_A = _REVU;
      _VRAM_PIN_D6_A = _REKU;
      _VRAM_PIN_D7_A = _RYZE;
    }

    {
      /*p25.RELA*/ wire _RELA_VRM_TO_CPUn = or2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
      /*p25.RENA*/ wire _RENA_VRM_TO_CPUp = not1(_RELA_VRM_TO_CPUn);
      /*p25.ROFA*/ wire _ROFA_VRM_TO_CPUn = not1(_RENA_VRM_TO_CPUp);
      _VRAM_PIN_D0_B = _ROFA_VRM_TO_CPUn;
      _VRAM_PIN_D1_B = _ROFA_VRM_TO_CPUn;
      _VRAM_PIN_D2_B = _ROFA_VRM_TO_CPUn;
      _VRAM_PIN_D3_B = _ROFA_VRM_TO_CPUn;
      _VRAM_PIN_D4_B = _ROFA_VRM_TO_CPUn;
      _VRAM_PIN_D5_B = _ROFA_VRM_TO_CPUn;
      _VRAM_PIN_D6_B = _ROFA_VRM_TO_CPUn;
      _VRAM_PIN_D7_B = _ROFA_VRM_TO_CPUn;
    }

    {
      /*p25.SYNU*/ wire _SYNU = or2(_VRAM_BUS_D0n.qp(), _RAHU_CBD_TO_VPDn);
      /*p25.SYMA*/ wire _SYMA = or2(_VRAM_BUS_D1n.qp(), _RAHU_CBD_TO_VPDn);
      /*p25.ROKO*/ wire _ROKO = or2(_VRAM_BUS_D2n.qp(), _RAHU_CBD_TO_VPDn);
      /*p25.SYBU*/ wire _SYBU = or2(_VRAM_BUS_D3n.qp(), _RAHU_CBD_TO_VPDn);
      /*p25.SAKO*/ wire _SAKO = or2(_VRAM_BUS_D4n.qp(), _RAHU_CBD_TO_VPDn);
      /*p25.SEJY*/ wire _SEJY = or2(_VRAM_BUS_D5n.qp(), _RAHU_CBD_TO_VPDn);
      /*p25.SEDO*/ wire _SEDO = or2(_VRAM_BUS_D6n.qp(), _RAHU_CBD_TO_VPDn);
      /*p25.SAWU*/ wire _SAWU = or2(_VRAM_BUS_D7n.qp(), _RAHU_CBD_TO_VPDn);

      /*p25.RURA*/ wire _RURA = not1(_SYNU);
      /*p25.RULY*/ wire _RULY = not1(_SYMA);
      /*p25.RARE*/ wire _RARE = not1(_ROKO);
      /*p25.RODU*/ wire _RODU = not1(_SYBU);
      /*p25.RUBE*/ wire _RUBE = not1(_SAKO);
      /*p25.RUMU*/ wire _RUMU = not1(_SEJY);
      /*p25.RYTY*/ wire _RYTY = not1(_SEDO);
      /*p25.RADY*/ wire _RADY = not1(_SAWU);

      _VRAM_PIN_D0_D = _RURA;
      _VRAM_PIN_D1_D = _RULY;
      _VRAM_PIN_D2_D = _RARE;
      _VRAM_PIN_D3_D = _RODU;
      _VRAM_PIN_D4_D = _RUBE;
      _VRAM_PIN_D5_D = _RUMU;
      _VRAM_PIN_D6_D = _RYTY;
      _VRAM_PIN_D7_D = _RADY;
    }
  }

  //----------------------------------------
  // VRAM data in

  // VPD -> VBD
  {
    /*p25.TAVY*/ wire _TAVY_MOEp = not1(_VRAM_PIN_OE_C.qp());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.CPU_PIN_WRp.qp());  // Schematic wrong, second input is CPU_PIN_WRp
    /*p25.SALE*/ wire _SALE_VRAM_WRn = mux2_p(_TAVY_MOEp, TEGU_CPU_VRAM_WRn, top.clk_reg.TUTO_DBG_VRAMp());

    /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn());
    /*p25.TEFY*/ wire _TEFY_VRAM_MCSp = not1(_VRAM_PIN_CS_C.qp());
    /*p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2_p(_TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SERE*/ wire _SERE_CPU_VRM_RDp = and2(_TOLE_VRAM_RDp, top.pix_pipe.ROPY_RENDERINGn());
    /*p25.RUVY*/ wire _RUVY_VRAM_WR = not1(_SALE_VRAM_WRn);
    /*p25.SAZO*/ wire _SAZO_VRAM_RD = and2(_RUVY_VRAM_WR, _SERE_CPU_VRM_RDp);
    /*p25.RYJE*/ wire _RYJE_VRAM_RDn = not1(_SAZO_VRAM_RD);
    /*p25.REVO*/ wire _REVO_VRAM_RDp = not1(_RYJE_VRAM_RDn);
    /*p25.RELA*/ wire _RELA_VPD_TO_VBDn = or2(_REVO_VRAM_RDp, _SAZO_VRAM_RD);
    /*p25.RENA*/ wire _RENA_VPD_TO_VBDp = not1(_RELA_VPD_TO_VBDn);

    // But if tri6p is inverting, then _VRAM_PIN_D0n_C is _not_ inverting?
    // Or is vram bus _not_ inverting on read?

    /*p25.RODY*/ _VRAM_BUS_D0n = tribuf_6p(_RENA_VPD_TO_VBDp, _VRAM_PIN_D0n_C.qp());
    /*p25.REBA*/ _VRAM_BUS_D1n = tribuf_6p(_RENA_VPD_TO_VBDp, _VRAM_PIN_D1n_C.qp());
    /*p25.RYDO*/ _VRAM_BUS_D2n = tribuf_6p(_RENA_VPD_TO_VBDp, _VRAM_PIN_D2n_C.qp());
    /*p25.REMO*/ _VRAM_BUS_D3n = tribuf_6p(_RENA_VPD_TO_VBDp, _VRAM_PIN_D3n_C.qp());
    /*p25.ROCE*/ _VRAM_BUS_D4n = tribuf_6p(_RENA_VPD_TO_VBDp, _VRAM_PIN_D4n_C.qp());
    /*p25.ROPU*/ _VRAM_BUS_D5n = tribuf_6p(_RENA_VPD_TO_VBDp, _VRAM_PIN_D5n_C.qp());
    /*p25.RETA*/ _VRAM_BUS_D6n = tribuf_6p(_RENA_VPD_TO_VBDp, _VRAM_PIN_D6n_C.qp());
    /*p25.RAKU*/ _VRAM_BUS_D7n = tribuf_6p(_RENA_VPD_TO_VBDp, _VRAM_PIN_D7n_C.qp());
  }

  // VBD -> CBD
  {
    /*p04.DECY*/ wire DECY = not1(top.cpu_bus.CPU_PIN_HOLD_MEM.qp());
    /*p04.CATY*/ wire CATY = not1(DECY);

    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(top.ext_bus.EXT_PIN_RD_C.qp(), top.cpu_bus.CPU_PIN_RDp.qp(), top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /*p07.AJAS*/ wire AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);
    /*p28.MYNU*/ wire MYNU_CPU_RDn = nand2(ASOT_CPU_RDp, CATY);
    /*p28.LEKO*/ wire LEKO_CPU_RDp = not1(MYNU_CPU_RDn);

    /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn());
    /*p25.TEFY*/ wire _TEFY_VRAM_MCSp = not1(_VRAM_PIN_CS_C.qp());
    /*p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2_p(_TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SERE*/ wire _SERE_CPU_VRM_RDp = and2(_TOLE_VRAM_RDp, top.pix_pipe.ROPY_RENDERINGn());
    /*p25.TYVY*/ wire _TYVY_VBD_TO_CBDn = nand2(_SERE_CPU_VRM_RDp, LEKO_CPU_RDp);
    /*p25.SEBY*/ wire _SEBY_VBD_TO_CBDp = not1(_TYVY_VBD_TO_CBDn);

    /*p25.RERY*/ wire _RERY_VBUS_D0p = not1(_VRAM_BUS_D0n.qp());
    /*p25.RUNA*/ wire _RUNA_VBUS_D1p = not1(_VRAM_BUS_D1n.qp());
    /*p25.RONA*/ wire _RONA_VBUS_D2p = not1(_VRAM_BUS_D2n.qp());
    /*p25.RUNO*/ wire _RUNO_VBUS_D3p = not1(_VRAM_BUS_D3n.qp());
    /*p25.SANA*/ wire _SANA_VBUS_D4p = not1(_VRAM_BUS_D4n.qp());
    /*p25.RORO*/ wire _RORO_VBUS_D5p = not1(_VRAM_BUS_D5n.qp());
    /*p25.RABO*/ wire _RABO_VBUS_D6p = not1(_VRAM_BUS_D6n.qp());
    /*p25.SAME*/ wire _SAME_VBUS_D7p = not1(_VRAM_BUS_D7n.qp());

    /*p25.RUGA*/ top.cpu_bus.CPU_BUS_D0 = tribuf_6p(_SEBY_VBD_TO_CBDp, _RERY_VBUS_D0p);
    /*p25.ROTA*/ top.cpu_bus.CPU_BUS_D1 = tribuf_6p(_SEBY_VBD_TO_CBDp, _RUNA_VBUS_D1p);
    /*p25.RYBU*/ top.cpu_bus.CPU_BUS_D2 = tribuf_6p(_SEBY_VBD_TO_CBDp, _RONA_VBUS_D2p);
    /*p25.RAJU*/ top.cpu_bus.CPU_BUS_D3 = tribuf_6p(_SEBY_VBD_TO_CBDp, _RUNO_VBUS_D3p);
    /*p25.TYJA*/ top.cpu_bus.CPU_BUS_D4 = tribuf_6p(_SEBY_VBD_TO_CBDp, _SANA_VBUS_D4p);
    /*p25.REXU*/ top.cpu_bus.CPU_BUS_D5 = tribuf_6p(_SEBY_VBD_TO_CBDp, _RORO_VBUS_D5p);
    /*p25.RUPY*/ top.cpu_bus.CPU_BUS_D6 = tribuf_6p(_SEBY_VBD_TO_CBDp, _RABO_VBUS_D6p);
    /*p25.TOKU*/ top.cpu_bus.CPU_BUS_D7 = tribuf_6p(_SEBY_VBD_TO_CBDp, _SAME_VBUS_D7p);
  }

  // VBD -> tile pix temp
  {
    /*#p27.LAXE*/ wire LAXE_BFETCH_S0n = not1(top.tile_fetcher._LAXU_BFETCH_S0.qp());
    /*#p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(top.tile_fetcher._MESU_BFETCH_S1.qp());
    /*#p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(top.tile_fetcher._NYVA_BFETCH_S2.qp());

    // MYSO fires on fetch phase 2, 6, 10
    /*#p27.MYSO*/ wire MYSO_BG_TRIGp   = nor3(top.pix_pipe.LOBY_RENDERINGn(), LAXE_BFETCH_S0n, top.tile_fetcher._LYZU_BFETCH_S0_D1.qp());

    // NYDY on fetch phase 6
    /*#p27.NYDY*/ wire NYDY_LATCH_TILE_DAn   = nand3(MYSO_BG_TRIGp, top.tile_fetcher._MESU_BFETCH_S1.qp(), NOFU_BFETCH_S2n);
    /*#p32.METE*/ wire METE_LATCH_TILE_DAp   = not1(NYDY_LATCH_TILE_DAn);
    /*#p32.LOMA*/ wire LOMA_LATCH_TILE_DAn   = not1(METE_LATCH_TILE_DAp);

    /*p32.LEGU*/ LEGU_TILE_DA0n = dff8_A(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, _VRAM_BUS_D0n.qp());
    /*p32.NUDU*/ NUDU_TILE_DA1n = dff8_A(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, _VRAM_BUS_D1n.qp());
    /*p32.MUKU*/ MUKU_TILE_DA2n = dff8_A(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, _VRAM_BUS_D2n.qp());
    /*p32.LUZO*/ LUZO_TILE_DA3n = dff8_A(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, _VRAM_BUS_D3n.qp());
    /*p32.MEGU*/ MEGU_TILE_DA4n = dff8_A(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, _VRAM_BUS_D4n.qp());
    /*p32.MYJY*/ MYJY_TILE_DA5n = dff8_A(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, _VRAM_BUS_D5n.qp());
    /*p32.NASA*/ NASA_TILE_DA6n = dff8_A(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, _VRAM_BUS_D6n.qp());
    /*p32.NEFO*/ NEFO_TILE_DA7n = dff8_A(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, _VRAM_BUS_D7n.qp());

    wire VYPO = 1;

    // MOFU fires on fetch phase 2 and 10
    /*p27.MOFU*/ wire MOFU_LATCH_TILE_DBp   = and2(MYSO_BG_TRIGp, NAKO_BFETCH_S1n);
    /*p32.LESO*/ wire LESO_LATCH_TILE_DBn   = not1(MOFU_LATCH_TILE_DBp);
    /*p??.LUVE*/ wire LUVE_MATCH_TILE_DBp   = not1(LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /*p32.LABU*/ wire LABU_LATCH_TILE_DBn   = not1(LUVE_MATCH_TILE_DBp);

    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /*p32.RAWU*/ RAWU_TILE_DB0n = dff11_A(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, _VRAM_BUS_D0n.qp());
    /*p32.POZO*/ POZO_TILE_DB1n = dff11_A(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, _VRAM_BUS_D1n.qp());
    /*p32.PYZO*/ PYZO_TILE_DB2n = dff11_A(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, _VRAM_BUS_D2n.qp());
    /*p32.POXA*/ POXA_TILE_DB3n = dff11_A(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, _VRAM_BUS_D3n.qp());
    /*p32.PULO*/ PULO_TILE_DB4n = dff11_A(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, _VRAM_BUS_D4n.qp());
    /*p32.POJU*/ POJU_TILE_DB5n = dff11_A(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, _VRAM_BUS_D5n.qp());
    /*p32.POWY*/ POWY_TILE_DB6n = dff11_A(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, _VRAM_BUS_D6n.qp());
    /*p32.PYJU*/ PYJU_TILE_DB7n = dff11_A(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, _VRAM_BUS_D7n.qp());
  }

  // VRAM tri -> sprite pix temp + x flip
  {
    /*#p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(top.sprite_fetcher._TOXE_SFETCH_S0.qp());
    /*p29.SYCU*/ wire SYCU_SFETCH_S0pe = nor3(TYTU_SFETCH_S0n, top.pix_pipe.LOBY_RENDERINGn(), top.sprite_fetcher._TYFO_SFETCH_S0_D1.qp());
    /*#p29.RACA*/ wire RACA_LATCH_SPPIXB = and2(top.sprite_fetcher._VONU_SFETCH_S1_D4.qp(), SYCU_SFETCH_S0pe);
    /*#p29.TOPU*/ wire TOPU_LATCH_SPPIXA = and2(top.sprite_fetcher._TULY_SFETCH_S1.qp(), SYCU_SFETCH_S0pe);

    /*#p29.XONO*/ wire _XONO_FLIP_X = and2(top.oam_bus.BAXO_OAM_DB5n.qp(), TEXY_SPR_READ_VRAMp);
    /*p33.POBE*/ wire _POBE_FLIP0n = mux2_p(_VRAM_BUS_D7n.qp(), _VRAM_BUS_D0n.qp(), _XONO_FLIP_X);
    /*p33.PACY*/ wire _PACY_FLIP1n = mux2_p(_VRAM_BUS_D6n.qp(), _VRAM_BUS_D1n.qp(), _XONO_FLIP_X);
    /*p33.PONO*/ wire _PONO_FLIP2n = mux2_p(_VRAM_BUS_D5n.qp(), _VRAM_BUS_D2n.qp(), _XONO_FLIP_X);
    /*p33.PUGU*/ wire _PUGU_FLIP3n = mux2_p(_VRAM_BUS_D4n.qp(), _VRAM_BUS_D3n.qp(), _XONO_FLIP_X);
    /*p33.PUTE*/ wire _PUTE_FLIP4n = mux2_p(_VRAM_BUS_D3n.qp(), _VRAM_BUS_D4n.qp(), _XONO_FLIP_X);
    /*p33.PULY*/ wire _PULY_FLIP5n = mux2_p(_VRAM_BUS_D2n.qp(), _VRAM_BUS_D5n.qp(), _XONO_FLIP_X);
    /*p33.PELO*/ wire _PELO_FLIP6n = mux2_p(_VRAM_BUS_D1n.qp(), _VRAM_BUS_D6n.qp(), _XONO_FLIP_X);
    /*p33.PAWE*/ wire _PAWE_FLIP7n = mux2_p(_VRAM_BUS_D0n.qp(), _VRAM_BUS_D7n.qp(), _XONO_FLIP_X);

    /*#p29.PEBY*/ wire _PEBY_CLKp = not1(RACA_LATCH_SPPIXB);
    /*#p29.NYBE*/ wire _NYBE_CLKn = not1(_PEBY_CLKp);
    /*#p29.PUCO*/ wire _PUCO_CLKp = not1(_NYBE_CLKn);

    /*p33.PEFO*/ PEFO_SPRITE_DB0n = dff8_A(_PUCO_CLKp, !_PUCO_CLKp, _POBE_FLIP0n);
    /*p33.ROKA*/ ROKA_SPRITE_DB1n = dff8_A(_PUCO_CLKp, !_PUCO_CLKp, _PACY_FLIP1n);
    /*p33.MYTU*/ MYTU_SPRITE_DB2n = dff8_A(_PUCO_CLKp, !_PUCO_CLKp, _PONO_FLIP2n);
    /*p33.RAMU*/ RAMU_SPRITE_DB3n = dff8_A(_PUCO_CLKp, !_PUCO_CLKp, _PUGU_FLIP3n);
    /*p33.SELE*/ SELE_SPRITE_DB4n = dff8_A(_PUCO_CLKp, !_PUCO_CLKp, _PUTE_FLIP4n);
    /*p33.SUTO*/ SUTO_SPRITE_DB5n = dff8_A(_PUCO_CLKp, !_PUCO_CLKp, _PULY_FLIP5n);
    /*p33.RAMA*/ RAMA_SPRITE_DB6n = dff8_A(_PUCO_CLKp, !_PUCO_CLKp, _PELO_FLIP6n);
    /*p33.RYDU*/ RYDU_SPRITE_DB7n = dff8_A(_PUCO_CLKp, !_PUCO_CLKp, _PAWE_FLIP7n);

    /*#p29.VYWA*/ wire _VYWA_CLKp = not1(TOPU_LATCH_SPPIXA);
    /*#p29.WENY*/ wire _WENY_CLKn = not1(_VYWA_CLKp);
    /*#p29.XADO*/ wire _XADO_CLKp = not1(_WENY_CLKn);

    /*p33.REWO*/ REWO_SPRITE_DA0n = dff8_A(_XADO_CLKp, !_XADO_CLKp, _POBE_FLIP0n);
    /*p33.PEBA*/ PEBA_SPRITE_DA1n = dff8_A(_XADO_CLKp, !_XADO_CLKp, _PACY_FLIP1n);
    /*p33.MOFO*/ MOFO_SPRITE_DA2n = dff8_A(_XADO_CLKp, !_XADO_CLKp, _PONO_FLIP2n);
    /*p33.PUDU*/ PUDU_SPRITE_DA3n = dff8_A(_XADO_CLKp, !_XADO_CLKp, _PUGU_FLIP3n);
    /*p33.SAJA*/ SAJA_SPRITE_DA4n = dff8_A(_XADO_CLKp, !_XADO_CLKp, _PUTE_FLIP4n);
    /*p33.SUNY*/ SUNY_SPRITE_DA5n = dff8_A(_XADO_CLKp, !_XADO_CLKp, _PULY_FLIP5n);
    /*p33.SEMO*/ SEMO_SPRITE_DA6n = dff8_A(_XADO_CLKp, !_XADO_CLKp, _PELO_FLIP6n);
    /*p33.SEGA*/ SEGA_SPRITE_DA7n = dff8_A(_XADO_CLKp, !_XADO_CLKp, _PAWE_FLIP7n);
  }
}

//------------------------------------------------------------------------------
