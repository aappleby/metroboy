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
    VRAM_BUS_A12n.c(), VRAM_BUS_A11n.c(), VRAM_BUS_A10n.c(), VRAM_BUS_A09n.c(),
    VRAM_BUS_A08n.c(), VRAM_BUS_A07n.c(), VRAM_BUS_A06n.c(), VRAM_BUS_A05n.c(),
    VRAM_BUS_A04n.c(), VRAM_BUS_A03n.c(), VRAM_BUS_A02n.c(), VRAM_BUS_A01n.c(),
    VRAM_BUS_A00n.c());

  d("VRAM BUS DATA    : %c%c%c%c%c%c%c%c\n",
    VRAM_BUS_D7p.c(), VRAM_BUS_D6p.c(), VRAM_BUS_D5p.c(), VRAM_BUS_D4p.c(),
    VRAM_BUS_D3p.c(), VRAM_BUS_D2p.c(), VRAM_BUS_D1p.c(), VRAM_BUS_D0p.c());

  d("VRAM PIN MCS     : %c%c%c\n", _VRAM_PIN_CS_A.c(), _VRAM_PIN_CS_C.c(), _VRAM_PIN_CS_D.c());
  d("VRAM PIN MOE     : %c%c%c\n", _VRAM_PIN_OE_A.c(), _VRAM_PIN_OE_C.c(), _VRAM_PIN_OE_D.c());
  d("VRAM PIN MWR     : %c%c%c\n", _VRAM_PIN_WR_A.c(), _VRAM_PIN_WR_C.c(), _VRAM_PIN_WR_D.c());
  d("VRAM PIN ADDR    : 0x%04x\n", get_pin_addr() | 0x8000);
  d("VRAM PIN DATA A  : %c%c%c%c%c%c%c%c\n",
    VRAM_PIN_D7n_A.c(), VRAM_PIN_D6n_A.c(), VRAM_PIN_D5n_A.c(), VRAM_PIN_D4n_A.c(),
    VRAM_PIN_D3n_A.c(), VRAM_PIN_D2n_A.c(), VRAM_PIN_D1n_A.c(), VRAM_PIN_D0n_A.c());
  d("VRAM PIN DATA B  : %c%c%c%c%c%c%c%c\n",
    VRAM_PIN_D7_B.c(), VRAM_PIN_D6_B.c(), VRAM_PIN_D5_B.c(), VRAM_PIN_D4_B.c(),
    VRAM_PIN_D3_B.c(), VRAM_PIN_D2_B.c(), VRAM_PIN_D1_B.c(), VRAM_PIN_D0_B.c());
  d("VRAM PIN DATA C  : %c%c%c%c%c%c%c%c\n",
    VRAM_PIN_D7n_C.c(), VRAM_PIN_D6n_C.c(), VRAM_PIN_D5n_C.c(), VRAM_PIN_D4n_C.c(),
    VRAM_PIN_D3n_C.c(), VRAM_PIN_D2n_C.c(), VRAM_PIN_D1n_C.c(), VRAM_PIN_D0n_C.c());
  d("VRAM PIN DATA D  : %c%c%c%c%c%c%c%c\n",
    VRAM_PIN_D7n_D.c(), VRAM_PIN_D6n_D.c(), VRAM_PIN_D5n_D.c(), VRAM_PIN_D4n_D.c(),
    VRAM_PIN_D3n_D.c(), VRAM_PIN_D2n_D.c(), VRAM_PIN_D1n_D.c(), VRAM_PIN_D0n_D.c());
  d("\n");
}


int VramBus::get_map_x(const SchematicTop& /*top*/) const {
  return pack_p(BABE_MAP_X0S, ABOD_MAP_X1S, BEWY_MAP_X2S, BYCA_MAP_X3S, ACUL_MAP_X4S, 0, 0, 0);
}

int VramBus::get_map_y(const SchematicTop& /*top*/) const {
  return pack_p(ETAM_MAP_Y0S, DOTO_MAP_Y1S, DABA_MAP_Y2S, EFYK_MAP_Y3S, EJOK_MAP_Y4S, 0, 0, 0);
}

//------------------------------------------------------------------------------

void VramBus::tock(SchematicTop& top) {

  /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXTp.tp(), top.cpu_bus.TEVY_8000_9FFFn());
  /*p25.TEFA*/ wire TEFA_8000_9FFFp = nor2(top.cpu_bus.SYRO_FE00_FFFFp(), TEXO_8000_9FFFn);
  /*p25.SOSE*/ wire SOSE_8000_9FFFp = and2(top.cpu_bus.CPU_BUS_A15.tp(), TEFA_8000_9FFFp);

  /*p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY = nor2(top.sprite_fetcher._TULY_SFETCH_S1.qp(), top.sprite_fetcher._VONU_SFETCH_S1_D4.qp());
  /*p29.TYSO*/ wire TYSO_SPRITE_READn = or2(_SAKY_SFETCH_MYSTERY, top.pix_pipe.TEPA_RENDERINGn()); // def or
  /*p29.TEXY*/ wire TEXY_SPR_READ_VRAMp = not1(TYSO_SPRITE_READn);

  /*p29.ABON*/ ABON_SPR_VRM_RDn = not1(TEXY_SPR_READ_VRAMp);

  /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not1(top.tile_fetcher._LONY_BG_FETCH_RUNNINGp.qp());
  /*p27.LENA*/ wire LENA_BGW_VRM_RDp = not1(LUSU_BGW_VRAM_RDn);

  //----------------------------------------
  // VRAM pins

  {
    /*p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp = and2(SOSE_8000_9FFFp, top.APOV_CPU_WRp_xxxxEFGx());
    /*p25.SUDO*/ wire _SUDO_MWRp_C = not1(_VRAM_PIN_WR_C.tp());
    /*p25.TYJY*/ wire _TYJY_DBG_VRAM_WRp = mux2_p(_SUDO_MWRp_C, _TUJA_CPU_VRAM_WRp, top.clk_reg.TUTO_DBG_VRAMp());

    /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn());
    /*p25.TEFY*/ wire _TEFY_VRAM_MCSp = not1(_VRAM_PIN_CS_C.tp());
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
    /*p25.TAVY*/ wire _TAVY_MOEp = not1(_VRAM_PIN_OE_C.tp());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.CPU_PIN_WRp.tp());  // Schematic wrong, second input is CPU_PIN_WRp
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
    /*p25.TEFY*/ wire _TEFY_VRAM_MCSp = not1(_VRAM_PIN_CS_C.tp());
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
    /* p25.XANE*/ wire _XANE_VRAM_LOCKn = nor2(top.dma_reg.LUFA_DMA_VRM_RDp(), top.pix_pipe.XYMU_RENDERINGp());
    /* p25.XEDU*/ XEDU_CPU_VRAM_RDn = not1(_XANE_VRAM_LOCKn);
    /* p25.XAKY*/ VRAM_BUS_A00n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A00.tp());
    /* p25.XUXU*/ VRAM_BUS_A01n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A01.tp());
    /* p25.XYNE*/ VRAM_BUS_A02n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A02.tp());
    /* p25.XODY*/ VRAM_BUS_A03n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A03.tp());
    /* p25.XECA*/ VRAM_BUS_A04n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A04.tp());
    /* p25.XOBA*/ VRAM_BUS_A05n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A05.tp());
    /* p25.XOPO*/ VRAM_BUS_A06n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A06.tp());
    /* p25.XYBO*/ VRAM_BUS_A07n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A07.tp());
    /* p25.RYSU*/ VRAM_BUS_A08n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A08.tp());
    /* p25.RESE*/ VRAM_BUS_A09n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A09.tp());
    /* p25.RUSE*/ VRAM_BUS_A10n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A10.tp());
    /* p25.RYNA*/ VRAM_BUS_A11n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A11.tp());
    /* p25.RUMO*/ VRAM_BUS_A12n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.CPU_BUS_A12.tp());
  }

  // DMA address -> vram address
  {
    /* p04.AHOC*/ AHOC_DMA_VRAM_RDn = not1(top.dma_reg.LUFA_DMA_VRM_RDp());
    /* p04.ECAL*/ VRAM_BUS_A00n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp());
    /* p04.EGEZ*/ VRAM_BUS_A01n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp());
    /* p04.FUHE*/ VRAM_BUS_A02n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp());
    /* p04.FYZY*/ VRAM_BUS_A03n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp());
    /* p04.DAMU*/ VRAM_BUS_A04n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp());
    /* p04.DAVA*/ VRAM_BUS_A05n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp());
    /* p04.ETEG*/ VRAM_BUS_A06n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp());
    /*#p04.EREW*/ VRAM_BUS_A07n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp());
    /*#p04.EVAX*/ VRAM_BUS_A08n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NAFA_DMA_A08n.qn());
    /* p04.DUVE*/ VRAM_BUS_A09n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PYNE_DMA_A09n.qn());
    /* p04.ERAF*/ VRAM_BUS_A10n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PARA_DMA_A10n.qn());
    /* p04.FUSY*/ VRAM_BUS_A11n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NYDO_DMA_A11n.qn());
    /* p04.EXYF*/ VRAM_BUS_A12n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NYGY_DMA_A12n.qn());
  }

  // Sprite fetcher read
  {
    /*#p29.WUKY*/ wire _WUKY_FLIP_Yp = not1(top.oam_bus.YZOS_OAM_DB6p.qp());
    
    /*#p29.CYVU*/ wire _CYVU_L0 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L0.tp());
    /*#p29.BORE*/ wire _BORE_L1 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L1.tp());
    /*#p29.BUVY*/ wire _BUVY_L2 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L2.tp());
    /*#p29.WAGO*/ wire _WAGO_L3 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L3.tp());
    
    /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not1(top.pix_pipe.XYMO_LCDC_SPSIZEn.qn());
    /*#p29.GEJY*/ wire _GEJY_L3 = amux2(top.oam_bus.XUSO_OAM_DA0p.qp(), _FUFO_LCDC_SPSIZEn,
                                        top.pix_pipe.XYMO_LCDC_SPSIZEn.qn(), _WAGO_L3);

    /*#p29.XUQU*/ wire XUQU_SPRITE_AB = not1(top.sprite_fetcher._VONU_SFETCH_S1_D4.qn());

    /* p29.ABEM*/ VRAM_BUS_A00n = tribuf_6nn(ABON_SPR_VRM_RDn, XUQU_SPRITE_AB);
    /* p29.BAXE*/ VRAM_BUS_A01n = tribuf_6nn(ABON_SPR_VRM_RDn, _CYVU_L0);
    /* p29.ARAS*/ VRAM_BUS_A02n = tribuf_6nn(ABON_SPR_VRM_RDn, _BORE_L1);
    /* p29.AGAG*/ VRAM_BUS_A03n = tribuf_6nn(ABON_SPR_VRM_RDn, _BUVY_L2);
    /* p29.FAMU*/ VRAM_BUS_A04n = tribuf_6nn(ABON_SPR_VRM_RDn, _GEJY_L3);
    /*#p29.FUGY*/ VRAM_BUS_A05n = tribuf_6nn(ABON_SPR_VRM_RDn, top.oam_bus.XEGU_OAM_DA1p.qp());
    /* p29.GAVO*/ VRAM_BUS_A06n = tribuf_6nn(ABON_SPR_VRM_RDn, top.oam_bus.YJEX_OAM_DA2p.qp());
    /* p29.WYGA*/ VRAM_BUS_A07n = tribuf_6nn(ABON_SPR_VRM_RDn, top.oam_bus.XYJU_OAM_DA3p.qp());
    /* p29.WUNE*/ VRAM_BUS_A08n = tribuf_6nn(ABON_SPR_VRM_RDn, top.oam_bus.YBOG_OAM_DA4p.qp());
    /* p29.GOTU*/ VRAM_BUS_A09n = tribuf_6nn(ABON_SPR_VRM_RDn, top.oam_bus.WYSO_OAM_DA5p.qp());
    /* p29.GEGU*/ VRAM_BUS_A10n = tribuf_6nn(ABON_SPR_VRM_RDn, top.oam_bus.XOTE_OAM_DA6p.qp());
    /* p29.XEHE*/ VRAM_BUS_A11n = tribuf_6nn(ABON_SPR_VRM_RDn, top.oam_bus.YZAB_OAM_DA7p.qp());
    /* p29.DYSO*/ VRAM_BUS_A12n = tribuf_6nn(ABON_SPR_VRM_RDn, 0);   // sprites always in low half of tile store
  }

  {
    /*#p26.FAFO*/ FAFO_TILE_Y0S = add_s(top.lcd_reg.MUWY_Y0p.qp(), top.pix_pipe.GAVE_SCY0n.qn(), 0);
    /*#p26.FAFO*/ FAFO_TILE_Y0C = add_c(top.lcd_reg.MUWY_Y0p.qp(), top.pix_pipe.GAVE_SCY0n.qn(), 0);
    /* p26.EMUX*/ EMUX_TILE_Y1S = add_s(top.lcd_reg.MYRO_Y1p.qp(), top.pix_pipe.FYMO_SCY1n.qn(), FAFO_TILE_Y0C);
    /* p26.EMUX*/ EMUX_TILE_Y1C = add_c(top.lcd_reg.MYRO_Y1p.qp(), top.pix_pipe.FYMO_SCY1n.qn(), FAFO_TILE_Y0C);
    /* p26.ECAB*/ ECAB_TILE_Y2S = add_s(top.lcd_reg.LEXA_Y2p.qp(), top.pix_pipe.FEZU_SCY2n.qn(), EMUX_TILE_Y1C);
    /* p26.ECAB*/ ECAB_TILE_Y2C = add_c(top.lcd_reg.LEXA_Y2p.qp(), top.pix_pipe.FEZU_SCY2n.qn(), EMUX_TILE_Y1C);
    /* p26.ETAM*/ ETAM_MAP_Y0S  = add_s(top.lcd_reg.LYDO_Y3p.qp(), top.pix_pipe.FUJO_SCY3n.qn(), ECAB_TILE_Y2C);
    /* p26.ETAM*/ ETAM_MAP_Y0C  = add_c(top.lcd_reg.LYDO_Y3p.qp(), top.pix_pipe.FUJO_SCY3n.qn(), ECAB_TILE_Y2C);
    /* p26.DOTO*/ DOTO_MAP_Y1S  = add_s(top.lcd_reg.LOVU_Y4p.qp(), top.pix_pipe.DEDE_SCY4n.qn(), ETAM_MAP_Y0C);
    /* p26.DOTO*/ DOTO_MAP_Y1C  = add_c(top.lcd_reg.LOVU_Y4p.qp(), top.pix_pipe.DEDE_SCY4n.qn(), ETAM_MAP_Y0C);
    /* p26.DABA*/ DABA_MAP_Y2S  = add_s(top.lcd_reg.LEMA_Y5p.qp(), top.pix_pipe.FOTY_SCY5n.qn(), DOTO_MAP_Y1C);
    /* p26.DABA*/ DABA_MAP_Y2C  = add_c(top.lcd_reg.LEMA_Y5p.qp(), top.pix_pipe.FOTY_SCY5n.qn(), DOTO_MAP_Y1C);
    /* p26.EFYK*/ EFYK_MAP_Y3S  = add_s(top.lcd_reg.MATO_Y6p.qp(), top.pix_pipe.FOHA_SCY6n.qn(), DABA_MAP_Y2C);
    /* p26.EFYK*/ EFYK_MAP_Y3C  = add_c(top.lcd_reg.MATO_Y6p.qp(), top.pix_pipe.FOHA_SCY6n.qn(), DABA_MAP_Y2C);
    /* p26.EJOK*/ EJOK_MAP_Y4S  = add_s(top.lcd_reg.LAFO_Y7p.qp(), top.pix_pipe.FUNY_SCY7n.qn(), EFYK_MAP_Y3C);
    /* p26.EJOK*/ EJOK_MAP_Y4C  = add_c(top.lcd_reg.LAFO_Y7p.qp(), top.pix_pipe.FUNY_SCY7n.qn(), EFYK_MAP_Y3C);

    /*#p26.ATAD*/ ATAD_TILE_X0S = add_s(top.pix_pipe.XEHO_X0p.qp(), top.pix_pipe.DATY_SCX0n.qn(), 0);
    /*#p26.ATAD*/ ATAD_TILE_X0C = add_c(top.pix_pipe.XEHO_X0p.qp(), top.pix_pipe.DATY_SCX0n.qn(), 0);
    /* p26.BEHU*/ BEHU_TILE_X1S = add_s(top.pix_pipe.SAVY_X1p.qp(), top.pix_pipe.DUZU_SCX1n.qn(), ATAD_TILE_X0C);
    /* p26.BEHU*/ BEHU_TILE_X1C = add_c(top.pix_pipe.SAVY_X1p.qp(), top.pix_pipe.DUZU_SCX1n.qn(), ATAD_TILE_X0C);
    /* p26.APYH*/ APYH_TILE_X2S = add_s(top.pix_pipe.XODU_X2p.qp(), top.pix_pipe.CYXU_SCX2n.qn(), BEHU_TILE_X1C);
    /* p26.APYH*/ APYH_TILE_X2C = add_c(top.pix_pipe.XODU_X2p.qp(), top.pix_pipe.CYXU_SCX2n.qn(), BEHU_TILE_X1C);
    /* p26.BABE*/ BABE_MAP_X0S  = add_s(top.pix_pipe.XYDO_X3p.qp(), top.pix_pipe.GUBO_SCX3n.qn(), APYH_TILE_X2C);
    /* p26.BABE*/ BABE_MAP_X0C  = add_c(top.pix_pipe.XYDO_X3p.qp(), top.pix_pipe.GUBO_SCX3n.qn(), APYH_TILE_X2C);
    /* p26.ABOD*/ ABOD_MAP_X1S  = add_s(top.pix_pipe.TUHU_X4p.qp(), top.pix_pipe.BEMY_SCX4n.qn(), BABE_MAP_X0C);
    /* p26.ABOD*/ ABOD_MAP_X1C  = add_c(top.pix_pipe.TUHU_X4p.qp(), top.pix_pipe.BEMY_SCX4n.qn(), BABE_MAP_X0C);
    /* p26.BEWY*/ BEWY_MAP_X2S  = add_s(top.pix_pipe.TUKY_X5p.qp(), top.pix_pipe.CUZY_SCX5n.qn(), ABOD_MAP_X1C);
    /* p26.BEWY*/ BEWY_MAP_X2C  = add_c(top.pix_pipe.TUKY_X5p.qp(), top.pix_pipe.CUZY_SCX5n.qn(), ABOD_MAP_X1C);
    /* p26.BYCA*/ BYCA_MAP_X3S  = add_s(top.pix_pipe.TAKO_X6p.qp(), top.pix_pipe.CABU_SCX6n.qn(), BEWY_MAP_X2C);
    /* p26.BYCA*/ BYCA_MAP_X3C  = add_c(top.pix_pipe.TAKO_X6p.qp(), top.pix_pipe.CABU_SCX6n.qn(), BEWY_MAP_X2C);
    /* p26.ACUL*/ ACUL_MAP_X4S  = add_s(top.pix_pipe.SYBE_X7p.qp(), top.pix_pipe.BAKE_SCX7n.qn(), BYCA_MAP_X3C);
    /* p26.ACUL*/ ACUL_MAP_X4C  = add_c(top.pix_pipe.SYBE_X7p.qp(), top.pix_pipe.BAKE_SCX7n.qn(), BYCA_MAP_X3C);

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

      // 0x9b00 = 0b1001100000000000
      // 0x9c00 = 0b1001110000000000

      /* p26.AXEP*/ VRAM_BUS_A00n = tribuf_6nn(BAFY_BG_MAP_READn, BABE_MAP_X0S);
      /* p26.AFEB*/ VRAM_BUS_A01n = tribuf_6nn(BAFY_BG_MAP_READn, ABOD_MAP_X1S);
      /* p26.ALEL*/ VRAM_BUS_A02n = tribuf_6nn(BAFY_BG_MAP_READn, BEWY_MAP_X2S);
      /* p26.COLY*/ VRAM_BUS_A03n = tribuf_6nn(BAFY_BG_MAP_READn, BYCA_MAP_X3S);
      /* p26.AJAN*/ VRAM_BUS_A04n = tribuf_6nn(BAFY_BG_MAP_READn, ACUL_MAP_X4S);
      /* p26.DUHO*/ VRAM_BUS_A05n = tribuf_6nn(BAFY_BG_MAP_READn, ETAM_MAP_Y0S);
      /* p26.CASE*/ VRAM_BUS_A06n = tribuf_6nn(BAFY_BG_MAP_READn, DOTO_MAP_Y1S);
      /* p26.CYPO*/ VRAM_BUS_A07n = tribuf_6nn(BAFY_BG_MAP_READn, DABA_MAP_Y2S);
      /* p26.CETA*/ VRAM_BUS_A08n = tribuf_6nn(BAFY_BG_MAP_READn, EFYK_MAP_Y3S);
      /* p26.DAFE*/ VRAM_BUS_A09n = tribuf_6nn(BAFY_BG_MAP_READn, EJOK_MAP_Y4S);
      /*#p26.AMUV*/ VRAM_BUS_A10n = tribuf_6nn(BAFY_BG_MAP_READn, top.pix_pipe.XAFO_LCDC_BGMAPn.qn());
      /* p26.COVE*/ VRAM_BUS_A11n = tribuf_6nn(BAFY_BG_MAP_READn, 1);
      /* p26.COXO*/ VRAM_BUS_A12n = tribuf_6nn(BAFY_BG_MAP_READn, 1);
    }

    // Window map read
    {
      // CHECK QP/QN

      /*#p27.NOCU*/ wire NOCU_WIN_MODEn = not1(top.pix_pipe.PYNU_WIN_MODE_A.tp());
      /*#p27.PORE*/ wire PORE_WIN_MODEp = not1(NOCU_WIN_MODEn);
      /*#p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(top.tile_fetcher._MESU_BFETCH_S1.qp());
      /*#p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(top.tile_fetcher._NYVA_BFETCH_S2.qp());

      /*#p27.NOGU*/ wire NOGU_FETCH_01p = nand2(NAKO_BFETCH_S1n, NOFU_BFETCH_S2n);
      /*#p27.NENY*/ wire NENY_FETCH_01n = not1(NOGU_FETCH_01p);

      /*#p27.POTU*/ wire POTU_BG_MAP_READp = and2(NENY_FETCH_01n, LENA_BGW_VRM_RDp);

      /*#p25.XEZE*/ wire XEZE_WIN_MAP_READp = and2(POTU_BG_MAP_READp, PORE_WIN_MODEp);
      /*#p25.WUKO*/ WUKO_WIN_MAP_READn = not1(XEZE_WIN_MAP_READp);
      /*#p27.XEJA*/ VRAM_BUS_A00n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.WYKA_WIN_X3.qp());
      /* p27.XAMO*/ VRAM_BUS_A01n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.WODY_WIN_X4.qp());
      /* p27.XAHE*/ VRAM_BUS_A02n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.WOBO_WIN_X5.qp());
      /* p27.XULO*/ VRAM_BUS_A03n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.WYKO_WIN_X6.qp());
      /* p27.WUJU*/ VRAM_BUS_A04n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.XOLO_WIN_X7.qp());
      /*#p27.VYTO*/ VRAM_BUS_A05n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.TUFU_WIN_Y3.qp());
      /* p27.VEHA*/ VRAM_BUS_A06n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.TAXA_WIN_Y4.qp());
      /* p27.VACE*/ VRAM_BUS_A07n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.TOZO_WIN_Y5.qp());
      /* p27.VOVO*/ VRAM_BUS_A08n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.TATE_WIN_Y6.qp());
      /* p27.VULO*/ VRAM_BUS_A09n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.TEKE_WIN_Y7.qp());
      /*#p27.VEVY*/ VRAM_BUS_A10n = tribuf_6nn(WUKO_WIN_MAP_READn, top.pix_pipe.WOKY_LCDC_WINMAPn.qn());
      /* p27.VEZA*/ VRAM_BUS_A11n = tribuf_6nn(WUKO_WIN_MAP_READn, 1);
      /* p27.VOGU*/ VRAM_BUS_A12n = tribuf_6nn(WUKO_WIN_MAP_READn, 1);
    }

    // Background/window tile read
    {
      // CHECK QP/QN

      /*#p27.NAKO*/ wire NAKO_BFETCH_S1n = not1(top.tile_fetcher._MESU_BFETCH_S1.qp());
      /*#p27.NOFU*/ wire NOFU_BFETCH_S2n = not1(top.tile_fetcher._NYVA_BFETCH_S2.qp());
      /* p27.NOCU*/ wire NOCU_WIN_MODEn = not1(top.pix_pipe.PYNU_WIN_MODE_A.tp()); // need to check pynu first
      /*#p27.PORE*/ wire PORE_WIN_MODEp = not1(NOCU_WIN_MODEn);
      /*#p26.AXAD*/ wire AXAD_WIN_MODEn = not1(PORE_WIN_MODEp);

      /*#p27.NOGU*/ wire NOGU_FETCH_01p = nand2(NAKO_BFETCH_S1n, NOFU_BFETCH_S2n);
      /*#p27.XUHA*/ wire XUHA_FETCH_S2p  = not1(NOFU_BFETCH_S2n);

      // Fires on phases 4-11 of tile read
      /* p27.NETA*/ NETA_TILE_READp = and2(LENA_BGW_VRM_RDp, NOGU_FETCH_01p);
      /* p26.ASUL*/ ASUL_TILE_READp = and2(NETA_TILE_READp, AXAD_WIN_MODEn);
      /* p26.BEJE*/ BEJE_BGD_TILE_READn = not1(ASUL_TILE_READp);

      /* p25.XUCY*/ XUCY_WIN_TILE_READn = nand2(NETA_TILE_READp, PORE_WIN_MODEp);

      // VUZA := nor(PYJU_11, WEXU_QN)
      /* p25.VUZA*/ VUZA_TILE_BANKp = nor2(PYJU_TILE_DB7n.qn(), top.pix_pipe.WEXU_LCDC_BGTILEn.qn());
      
      /*#p26.ASUM*/ VRAM_BUS_A00n = tribuf_6nn(BEJE_BGD_TILE_READn, XUHA_FETCH_S2p);
      /* p26.EVAD*/ VRAM_BUS_A01n = tribuf_6nn(BEJE_BGD_TILE_READn, FAFO_TILE_Y0S);
      /* p26.DAHU*/ VRAM_BUS_A02n = tribuf_6nn(BEJE_BGD_TILE_READn, EMUX_TILE_Y1S);
      /* p26.DODE*/ VRAM_BUS_A03n = tribuf_6nn(BEJE_BGD_TILE_READn, ECAB_TILE_Y2S); // check outputs of ECAB

      /*#p25.XONU*/ VRAM_BUS_A00n = tribuf_6nn(XUCY_WIN_TILE_READn, XUHA_FETCH_S2p);
      /*#p25.WUDO*/ VRAM_BUS_A01n = tribuf_6nn(XUCY_WIN_TILE_READn, top.pix_pipe.VYNO_WIN_Y0.qp());
      /*#p25.WAWE*/ VRAM_BUS_A02n = tribuf_6nn(XUCY_WIN_TILE_READn, top.pix_pipe.VUJO_WIN_Y1.qp());
      /*#p25.WOLU*/ VRAM_BUS_A03n = tribuf_6nn(XUCY_WIN_TILE_READn, top.pix_pipe.VYMU_WIN_Y2.qp());

      /*#p25.VAPY*/ VRAM_BUS_A04n = tribuf_6pn(NETA_TILE_READp,     RAWU_TILE_DB0n.qn());
      /*#p25.SEZU*/ VRAM_BUS_A05n = tribuf_6pn(NETA_TILE_READp,     POZO_TILE_DB1n.qn());
      /*#p25.VEJY*/ VRAM_BUS_A06n = tribuf_6pn(NETA_TILE_READp,     PYZO_TILE_DB2n.qn());
      /*#p25.RUSA*/ VRAM_BUS_A07n = tribuf_6pn(NETA_TILE_READp,     POXA_TILE_DB3n.qn());
      /*#p25.ROHA*/ VRAM_BUS_A08n = tribuf_6pn(NETA_TILE_READp,     PULO_TILE_DB4n.qn());
      /*#p25.RESO*/ VRAM_BUS_A09n = tribuf_6pn(NETA_TILE_READp,     POJU_TILE_DB5n.qn());
      /*#p25.SUVO*/ VRAM_BUS_A10n = tribuf_6pn(NETA_TILE_READp,     POWY_TILE_DB6n.qn());
      /*#p25.TOBO*/ VRAM_BUS_A11n = tribuf_6pn(NETA_TILE_READp,     PYJU_TILE_DB7n.qn());
      /*#p25.VURY*/ VRAM_BUS_A12n = tribuf_6pn(NETA_TILE_READp,     VUZA_TILE_BANKp);
    }
  }

  // VRAM addr bus -> VRAM addr pin
  {
    /*p25.MYFU*/ wire MYFUp = not1(VRAM_BUS_A00n.tp());
    /*p25.MASA*/ wire MASAp = not1(VRAM_BUS_A01n.tp());
    /*p25.MYRE*/ wire MYREp = not1(VRAM_BUS_A02n.tp());
    /*p25.MAVU*/ wire MAVUp = not1(VRAM_BUS_A03n.tp());
    /*p25.MEPA*/ wire MEPAp = not1(VRAM_BUS_A04n.tp());
    /*p25.MYSA*/ wire MYSAp = not1(VRAM_BUS_A05n.tp());
    /*p25.MEWY*/ wire MEWYp = not1(VRAM_BUS_A06n.tp());
    /*p25.MUME*/ wire MUMEp = not1(VRAM_BUS_A07n.tp());
    /*p25.VOVA*/ wire VOVAp = not1(VRAM_BUS_A08n.tp());
    /*p25.VODE*/ wire VODEp = not1(VRAM_BUS_A09n.tp());
    /*p25.RUKY*/ wire RUKYp = not1(VRAM_BUS_A10n.tp());
    /*p25.RUMA*/ wire RUMAp = not1(VRAM_BUS_A11n.tp());
    /*p25.REHO*/ wire REHOp = not1(VRAM_BUS_A12n.tp());

    /*p25.LEXE*/ wire LEXEn = not1(MYFUp);
    /*p25.LOZU*/ wire LOZUn = not1(MASAp);
    /*p25.LACA*/ wire LACAn = not1(MYREp);
    /*p25.LUVO*/ wire LUVOn = not1(MAVUp);
    /*p25.LOLY*/ wire LOLYn = not1(MEPAp);
    /*p25.LALO*/ wire LALOn = not1(MYSAp);
    /*p25.LEFA*/ wire LEFAn = not1(MEWYp);
    /*p25.LUBY*/ wire LUBYn = not1(MUMEp);
    /*p25.TUJY*/ wire TUJYn = not1(VOVAp);
    /*p25.TAGO*/ wire TAGOn = not1(VODEp);
    /*p25.NUVA*/ wire NUVAn = not1(RUKYp);
    /*p25.PEDU*/ wire PEDUn = not1(RUMAp);
    /*p25.PONY*/ wire PONYn = not1(REHOp);

    /*p25.LEXE*/ VRAM_PIN_A00n = LEXEn;
    /*p25.LOZU*/ VRAM_PIN_A01n = LOZUn;
    /*p25.LACA*/ VRAM_PIN_A02n = LACAn;
    /*p25.LUVO*/ VRAM_PIN_A03n = LUVOn;
    /*p25.LOLY*/ VRAM_PIN_A04n = LOLYn;
    /*p25.LALO*/ VRAM_PIN_A05n = LALOn;
    /*p25.LEFA*/ VRAM_PIN_A06n = LEFAn;
    /*p25.LUBY*/ VRAM_PIN_A07n = LUBYn;
    /*p25.TUJY*/ VRAM_PIN_A08n = TUJYn;
    /*p25.TAGO*/ VRAM_PIN_A09n = TAGOn;
    /*p25.NUVA*/ VRAM_PIN_A10n = NUVAn;
    /*p25.PEDU*/ VRAM_PIN_A11n = PEDUn;
    /*p25.PONY*/ VRAM_PIN_A12n = PONYn;
  }

  //----------------------------------------
  // VRAM data out

  // CBD->VBD->VPD
  {
    /*p25.TAVY*/ wire TAVY_MOEp = not1(_VRAM_PIN_OE_C.tp());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.CPU_PIN_WRp.tp());  // Schematic wrong, second input is CPU_PIN_WRp
    /*p25.SALE*/ wire SALE_CPU_VRAM_WRn = mux2_p(TAVY_MOEp, TEGU_CPU_VRAM_WRn, top.clk_reg.TUTO_DBG_VRAMp());

    /*p25.TUCA*/ wire TUCA_CPU_VRAM_AVn = and2(SOSE_8000_9FFFp, top.ABUZ_AVn());
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(_VRAM_PIN_CS_C.tp());
    /*p25.TOLE*/ wire TOLE_VRAM_AVn     = mux2_p(TEFY_VRAM_MCSp, TUCA_CPU_VRAM_AVn, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SERE*/ wire SERE = and2(TOLE_VRAM_AVn, top.pix_pipe.ROPY_RENDERINGn());
    /*p25.RUVY*/ wire RUVY_VRAM_WRp = not1(SALE_CPU_VRAM_WRn);
    /*p25.SAZO*/ wire SAZO_CBD_TO_VPDp = and2(RUVY_VRAM_WRp, SERE);
    /*p25.RYJE*/ wire RYJE_CBD_TO_VPDn = not1(SAZO_CBD_TO_VPDp);
    /*p25.REVO*/ wire REVO_CBD_TO_VPDp = not1(RYJE_CBD_TO_VPDn);
    /*p25.ROCY*/ wire ROCY_CBD_TO_VPDp = and2(REVO_CBD_TO_VPDp, SAZO_CBD_TO_VPDp);
    /*p25.RAHU*/ wire RAHU_CBD_TO_VPDn = not1(ROCY_CBD_TO_VPDp);

    // TEME_01 << CPU_BUS_D0p
    // TEME_02 nc
    // TEME_03 nc
    // TEME_04 nc
    // TEME_05 << RAHU
    // TEME_06 << CPU_BUS_D0p
    // TEME_07 nc
    // TEME_08 nc 
    // TEME_09 >> VRAM_BUS_D0p
    // TEME_10 nc

    /*p25.TEME*/ VRAM_BUS_D0p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D0p.tp());
    /*p25.TEWU*/ VRAM_BUS_D1p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D1p.tp());
    /*p25.TYGO*/ VRAM_BUS_D2p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D2p.tp());
    /*p25.SOTE*/ VRAM_BUS_D3p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D3p.tp());
    /*p25.SEKE*/ VRAM_BUS_D4p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D4p.tp());
    /*p25.RUJO*/ VRAM_BUS_D5p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D5p.tp());
    /*p25.TOFA*/ VRAM_BUS_D6p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D6p.tp());
    /*p25.SUZA*/ VRAM_BUS_D7p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.CPU_BUS_D7p.tp());

    /*p25.ROVE*/ wire ROVE_CBD_TO_VPDp = not1(RAHU_CBD_TO_VPDn);
    /*p25.SEFA*/ wire SEFAp = and2(VRAM_BUS_D0p.tp(), ROVE_CBD_TO_VPDp);
    /*p25.SOGO*/ wire SOGOp = and2(VRAM_BUS_D1p.tp(), ROVE_CBD_TO_VPDp);
    /*p25.SEFU*/ wire SEFUp = and2(VRAM_BUS_D2p.tp(), ROVE_CBD_TO_VPDp);
    /*p25.SUNA*/ wire SUNAp = and2(VRAM_BUS_D3p.tp(), ROVE_CBD_TO_VPDp);
    /*p25.SUMO*/ wire SUMOp = and2(VRAM_BUS_D4p.tp(), ROVE_CBD_TO_VPDp);
    /*p25.SAZU*/ wire SAZUp = and2(VRAM_BUS_D5p.tp(), ROVE_CBD_TO_VPDp);
    /*p25.SAMO*/ wire SAMOp = and2(VRAM_BUS_D6p.tp(), ROVE_CBD_TO_VPDp);
    /*p25.SUKE*/ wire SUKEp = and2(VRAM_BUS_D7p.tp(), ROVE_CBD_TO_VPDp);

    /*p25.REGE*/ wire REGEn = not1(SEFAp);
    /*p25.RYKY*/ wire RYKYn = not1(SOGOp);
    /*p25.RAZO*/ wire RAZOn = not1(SEFUp);
    /*p25.RADA*/ wire RADAn = not1(SUNAp);
    /*p25.RYRO*/ wire RYROn = not1(SUMOp);
    /*p25.REVU*/ wire REVUn = not1(SAZUp);
    /*p25.REKU*/ wire REKUn = not1(SAMOp);
    /*p25.RYZE*/ wire RYZEn = not1(SUKEp);

    VRAM_PIN_D0n_A = REGEn;
    VRAM_PIN_D1n_A = RYKYn;
    VRAM_PIN_D2n_A = RAZOn;
    VRAM_PIN_D3n_A = RADAn;
    VRAM_PIN_D4n_A = RYROn;
    VRAM_PIN_D5n_A = REVUn;
    VRAM_PIN_D6n_A = REKUn;
    VRAM_PIN_D7n_A = RYZEn;

    /*p25.SYNU*/ wire SYNUp = or2(RAHU_CBD_TO_VPDn, VRAM_BUS_D0p.tp());
    /*p25.SYMA*/ wire SYMAp = or2(RAHU_CBD_TO_VPDn, VRAM_BUS_D1p.tp());
    /*p25.ROKO*/ wire ROKOp = or2(RAHU_CBD_TO_VPDn, VRAM_BUS_D2p.tp());
    /*p25.SYBU*/ wire SYBUp = or2(RAHU_CBD_TO_VPDn, VRAM_BUS_D3p.tp());
    /*p25.SAKO*/ wire SAKOp = or2(RAHU_CBD_TO_VPDn, VRAM_BUS_D4p.tp());
    /*p25.SEJY*/ wire SEJYp = or2(RAHU_CBD_TO_VPDn, VRAM_BUS_D5p.tp());
    /*p25.SEDO*/ wire SEDOp = or2(RAHU_CBD_TO_VPDn, VRAM_BUS_D6p.tp());
    /*p25.SAWU*/ wire SAWUp = or2(RAHU_CBD_TO_VPDn, VRAM_BUS_D7p.tp());

    /*p25.RURA*/ wire RURAn = not1(SYNUp);
    /*p25.RULY*/ wire RULYn = not1(SYMAp);
    /*p25.RARE*/ wire RAREn = not1(ROKOp);
    /*p25.RODU*/ wire RODUn = not1(SYBUp);
    /*p25.RUBE*/ wire RUBEn = not1(SAKOp);
    /*p25.RUMU*/ wire RUMUn = not1(SEJYp);
    /*p25.RYTY*/ wire RYTYn = not1(SEDOp);
    /*p25.RADY*/ wire RADYn = not1(SAWUp);

    VRAM_PIN_D0n_D = RURAn;
    VRAM_PIN_D1n_D = RULYn;
    VRAM_PIN_D2n_D = RAREn;
    VRAM_PIN_D3n_D = RODUn;
    VRAM_PIN_D4n_D = RUBEn;
    VRAM_PIN_D5n_D = RUMUn;
    VRAM_PIN_D6n_D = RYTYn;
    VRAM_PIN_D7n_D = RADYn;

    /*p25.RELA*/ wire RELA_CBD_TO_VPDp = or2(REVO_CBD_TO_VPDp, SAZO_CBD_TO_VPDp);
    /*p25.RENA*/ wire RENA_CBD_TO_VPDn = not1(RELA_CBD_TO_VPDp);
    /*p25.ROFA*/ wire ROFA_CBD_TO_VPDp = not1(RENA_CBD_TO_VPDn);
    VRAM_PIN_D0_B = ROFA_CBD_TO_VPDp;
    VRAM_PIN_D1_B = ROFA_CBD_TO_VPDp;
    VRAM_PIN_D2_B = ROFA_CBD_TO_VPDp;
    VRAM_PIN_D3_B = ROFA_CBD_TO_VPDp;
    VRAM_PIN_D4_B = ROFA_CBD_TO_VPDp;
    VRAM_PIN_D5_B = ROFA_CBD_TO_VPDp;
    VRAM_PIN_D6_B = ROFA_CBD_TO_VPDp;
    VRAM_PIN_D7_B = ROFA_CBD_TO_VPDp;
  }

  //----------------------------------------
  // VRAM data in

  // VPD -> VBD
  {
    /*p25.TAVY*/ wire TAVY_MOEp = not1(_VRAM_PIN_OE_C.tp());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.CPU_PIN_WRp.tp());  // Schematic wrong, second input is CPU_PIN_WRp
    /*p25.SALE*/ wire SALE_VRAM_WRn = mux2_p(TAVY_MOEp, TEGU_CPU_VRAM_WRn, top.clk_reg.TUTO_DBG_VRAMp());

    /*p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn());
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(_VRAM_PIN_CS_C.tp());
    /*p25.TOLE*/ wire TOLE_VRAM_RDp     = mux2_p(TEFY_VRAM_MCSp, TUCA_CPU_VRAM_RDp, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SERE*/ wire SERE_CPU_VRM_RDp = and2(TOLE_VRAM_RDp, top.pix_pipe.ROPY_RENDERINGn());
    /*p25.RUVY*/ wire RUVY_VRAM_WR = not1(SALE_VRAM_WRn);
    /*p25.SAZO*/ wire SAZO_VRAM_RD = and2(RUVY_VRAM_WR, SERE_CPU_VRM_RDp);
    /*p25.RYJE*/ wire RYJE_VRAM_RDn = not1(SAZO_VRAM_RD);
    /*p25.REVO*/ wire REVO_VRAM_RDp = not1(RYJE_VRAM_RDn);
    /*p25.RELA*/ wire RELA_VPD_TO_VBDn = or2(REVO_VRAM_RDp, SAZO_VRAM_RD);
    /*p25.RENA*/ wire RENA_VPD_TO_VBDp = not1(RELA_VPD_TO_VBDn);

    // But if tri6p is inverting, then _VRAM_PIN_D0n_C is _not_ inverting?
    // Or is vram bus _not_ inverting on read?

    /*p25.RODY*/ VRAM_BUS_D0p = tribuf_6pn(RENA_VPD_TO_VBDp, VRAM_PIN_D0n_C.tp());
    /*p25.REBA*/ VRAM_BUS_D1p = tribuf_6pn(RENA_VPD_TO_VBDp, VRAM_PIN_D1n_C.tp());
    /*p25.RYDO*/ VRAM_BUS_D2p = tribuf_6pn(RENA_VPD_TO_VBDp, VRAM_PIN_D2n_C.tp());
    /*p25.REMO*/ VRAM_BUS_D3p = tribuf_6pn(RENA_VPD_TO_VBDp, VRAM_PIN_D3n_C.tp());
    /*p25.ROCE*/ VRAM_BUS_D4p = tribuf_6pn(RENA_VPD_TO_VBDp, VRAM_PIN_D4n_C.tp());
    /*p25.ROPU*/ VRAM_BUS_D5p = tribuf_6pn(RENA_VPD_TO_VBDp, VRAM_PIN_D5n_C.tp());
    /*p25.RETA*/ VRAM_BUS_D6p = tribuf_6pn(RENA_VPD_TO_VBDp, VRAM_PIN_D6n_C.tp());
    /*p25.RAKU*/ VRAM_BUS_D7p = tribuf_6pn(RENA_VPD_TO_VBDp, VRAM_PIN_D7n_C.tp());
  }

  // VBD -> CBD
  {
    /*p04.DECY*/ wire DECY = not1(top.cpu_bus.CPU_PIN_HOLD_MEM.tp());
    /*p04.CATY*/ wire CATY = not1(DECY);

    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(top.ext_bus.EXT_PIN_RD_C.tp(), top.cpu_bus.CPU_PIN_RDp.tp(), top.clk_reg.UNOR_MODE_DBG2p());
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /*p07.AJAS*/ wire AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);
    /*p28.MYNU*/ wire MYNU_CPU_RDn = nand2(ASOT_CPU_RDp, CATY);
    /*p28.LEKO*/ wire LEKO_CPU_RDp = not1(MYNU_CPU_RDn);

    /*p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn());
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(_VRAM_PIN_CS_C.tp());
    /*p25.TOLE*/ wire TOLE_VRAM_RDp     = mux2_p(TEFY_VRAM_MCSp, TUCA_CPU_VRAM_RDp, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SERE*/ wire SERE_CPU_VRM_RDp = and2(TOLE_VRAM_RDp, top.pix_pipe.ROPY_RENDERINGn());
    /*p25.TYVY*/ wire TYVY_VBD_TO_CBDn = nand2(SERE_CPU_VRM_RDp, LEKO_CPU_RDp);
    /*p25.SEBY*/ wire SEBY_VBD_TO_CBDp = not1(TYVY_VBD_TO_CBDn);

    /*p25.RERY*/ wire RERY_VBUS_D0n = not1(VRAM_BUS_D0p.tp());
    /*p25.RUNA*/ wire RUNA_VBUS_D1n = not1(VRAM_BUS_D1p.tp());
    /*p25.RONA*/ wire RONA_VBUS_D2n = not1(VRAM_BUS_D2p.tp());
    /*p25.RUNO*/ wire RUNO_VBUS_D3n = not1(VRAM_BUS_D3p.tp());
    /*p25.SANA*/ wire SANA_VBUS_D4n = not1(VRAM_BUS_D4p.tp());
    /*p25.RORO*/ wire RORO_VBUS_D5n = not1(VRAM_BUS_D5p.tp());
    /*p25.RABO*/ wire RABO_VBUS_D6n = not1(VRAM_BUS_D6p.tp());
    /*p25.SAME*/ wire SAME_VBUS_D7n = not1(VRAM_BUS_D7p.tp());

    // FIXME I'm missing an inverter here or something.

    // RERY := not(VRAM_BUS_D0p)
    // RUGA := tri(SEBY, RERY);
    // ........hmm

    /*p25.RUGA*/ top.cpu_bus.CPU_BUS_D0p = tribuf_6pn(SEBY_VBD_TO_CBDp, !RERY_VBUS_D0n);
    /*p25.ROTA*/ top.cpu_bus.CPU_BUS_D1p = tribuf_6pn(SEBY_VBD_TO_CBDp, !RUNA_VBUS_D1n);
    /*p25.RYBU*/ top.cpu_bus.CPU_BUS_D2p = tribuf_6pn(SEBY_VBD_TO_CBDp, !RONA_VBUS_D2n);
    /*p25.RAJU*/ top.cpu_bus.CPU_BUS_D3p = tribuf_6pn(SEBY_VBD_TO_CBDp, !RUNO_VBUS_D3n);
    /*p25.TYJA*/ top.cpu_bus.CPU_BUS_D4p = tribuf_6pn(SEBY_VBD_TO_CBDp, !SANA_VBUS_D4n);
    /*p25.REXU*/ top.cpu_bus.CPU_BUS_D5p = tribuf_6pn(SEBY_VBD_TO_CBDp, !RORO_VBUS_D5n);
    /*p25.RUPY*/ top.cpu_bus.CPU_BUS_D6p = tribuf_6pn(SEBY_VBD_TO_CBDp, !RABO_VBUS_D6n);
    /*p25.TOKU*/ top.cpu_bus.CPU_BUS_D7p = tribuf_6pn(SEBY_VBD_TO_CBDp, !SAME_VBUS_D7n);
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

    /*p32.LEGU*/ LEGU_TILE_DA0n = dff8_A_inv(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, VRAM_BUS_D0p.tp());
    /*p32.NUDU*/ NUDU_TILE_DA1n = dff8_A_inv(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, VRAM_BUS_D1p.tp());
    /*p32.MUKU*/ MUKU_TILE_DA2n = dff8_A_inv(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, VRAM_BUS_D2p.tp());
    /*p32.LUZO*/ LUZO_TILE_DA3n = dff8_A_inv(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, VRAM_BUS_D3p.tp());
    /*p32.MEGU*/ MEGU_TILE_DA4n = dff8_A_inv(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, VRAM_BUS_D4p.tp());
    /*p32.MYJY*/ MYJY_TILE_DA5n = dff8_A_inv(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, VRAM_BUS_D5p.tp());
    /*p32.NASA*/ NASA_TILE_DA6n = dff8_A_inv(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, VRAM_BUS_D6p.tp());
    /*p32.NEFO*/ NEFO_TILE_DA7n = dff8_A_inv(LOMA_LATCH_TILE_DAn, !LOMA_LATCH_TILE_DAn, VRAM_BUS_D7p.tp());

    wire VYPO = 1;

    // MOFU fires on fetch phase 2 and 10
    /*p27.MOFU*/ wire MOFU_LATCH_TILE_DBp   = and2(MYSO_BG_TRIGp, NAKO_BFETCH_S1n);
    /*p32.LESO*/ wire LESO_LATCH_TILE_DBn   = not1(MOFU_LATCH_TILE_DBp);
    /*p??.LUVE*/ wire LUVE_MATCH_TILE_DBp   = not1(LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /*p32.LABU*/ wire LABU_LATCH_TILE_DBn   = not1(LUVE_MATCH_TILE_DBp);

    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /*p32.RAWU*/ RAWU_TILE_DB0n = dff11_A_inv(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, VRAM_BUS_D0p.tp());
    /*p32.POZO*/ POZO_TILE_DB1n = dff11_A_inv(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, VRAM_BUS_D1p.tp());
    /*p32.PYZO*/ PYZO_TILE_DB2n = dff11_A_inv(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, VRAM_BUS_D2p.tp());
    /*p32.POXA*/ POXA_TILE_DB3n = dff11_A_inv(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, VRAM_BUS_D3p.tp());
    /*p32.PULO*/ PULO_TILE_DB4n = dff11_A_inv(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, VRAM_BUS_D4p.tp());
    /*p32.POJU*/ POJU_TILE_DB5n = dff11_A_inv(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, VRAM_BUS_D5p.tp());
    /*p32.POWY*/ POWY_TILE_DB6n = dff11_A_inv(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, VRAM_BUS_D6p.tp());
    /*p32.PYJU*/ PYJU_TILE_DB7n = dff11_A_inv(LABU_LATCH_TILE_DBn, LUVE_MATCH_TILE_DBp, VYPO, VRAM_BUS_D7p.tp());
  }

  // VRAM tri -> sprite pix temp + x flip
  {
    /*#p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(top.sprite_fetcher._TOXE_SFETCH_S0.qp());
    /*p29.SYCU*/ wire SYCU_SFETCH_S0pe = nor3(TYTU_SFETCH_S0n, top.pix_pipe.LOBY_RENDERINGn(), top.sprite_fetcher._TYFO_SFETCH_S0_D1.qp());
    /*#p29.RACA*/ wire RACA_LATCH_SPPIXB = and2(top.sprite_fetcher._VONU_SFETCH_S1_D4.qp(), SYCU_SFETCH_S0pe);
    /*#p29.TOPU*/ wire TOPU_LATCH_SPPIXA = and2(top.sprite_fetcher._TULY_SFETCH_S1.qp(), SYCU_SFETCH_S0pe);

    /*#p29.XONO*/ wire _XONO_FLIP_X = and2(top.oam_bus.BAXO_OAM_DB5p.qp(), TEXY_SPR_READ_VRAMp);

    /*p33.POBE*/ wire _POBE_FLIP0n = mux2_p(VRAM_BUS_D7p.tp(), VRAM_BUS_D0p.tp(), _XONO_FLIP_X);
    /*p33.PACY*/ wire _PACY_FLIP1n = mux2_p(VRAM_BUS_D6p.tp(), VRAM_BUS_D1p.tp(), _XONO_FLIP_X);
    /*p33.PONO*/ wire _PONO_FLIP2n = mux2_p(VRAM_BUS_D5p.tp(), VRAM_BUS_D2p.tp(), _XONO_FLIP_X);
    /*p33.PUGU*/ wire _PUGU_FLIP3n = mux2_p(VRAM_BUS_D4p.tp(), VRAM_BUS_D3p.tp(), _XONO_FLIP_X);
    /*p33.PUTE*/ wire _PUTE_FLIP4n = mux2_p(VRAM_BUS_D3p.tp(), VRAM_BUS_D4p.tp(), _XONO_FLIP_X);
    /*p33.PULY*/ wire _PULY_FLIP5n = mux2_p(VRAM_BUS_D2p.tp(), VRAM_BUS_D5p.tp(), _XONO_FLIP_X);
    /*p33.PELO*/ wire _PELO_FLIP6n = mux2_p(VRAM_BUS_D1p.tp(), VRAM_BUS_D6p.tp(), _XONO_FLIP_X);
    /*p33.PAWE*/ wire _PAWE_FLIP7n = mux2_p(VRAM_BUS_D0p.tp(), VRAM_BUS_D7p.tp(), _XONO_FLIP_X);

    /*#p29.PEBY*/ wire _PEBY_CLKp = not1(RACA_LATCH_SPPIXB);
    /*#p29.NYBE*/ wire _NYBE_CLKn = not1(_PEBY_CLKp);
    /*#p29.PUCO*/ wire _PUCO_CLKp = not1(_NYBE_CLKn);

    /*p33.PEFO*/ PEFO_SPRITE_DB0n = dff8_A_inv(_PUCO_CLKp, !_PUCO_CLKp, _POBE_FLIP0n);
    /*p33.ROKA*/ ROKA_SPRITE_DB1n = dff8_A_inv(_PUCO_CLKp, !_PUCO_CLKp, _PACY_FLIP1n);
    /*p33.MYTU*/ MYTU_SPRITE_DB2n = dff8_A_inv(_PUCO_CLKp, !_PUCO_CLKp, _PONO_FLIP2n);
    /*p33.RAMU*/ RAMU_SPRITE_DB3n = dff8_A_inv(_PUCO_CLKp, !_PUCO_CLKp, _PUGU_FLIP3n);
    /*p33.SELE*/ SELE_SPRITE_DB4n = dff8_A_inv(_PUCO_CLKp, !_PUCO_CLKp, _PUTE_FLIP4n);
    /*p33.SUTO*/ SUTO_SPRITE_DB5n = dff8_A_inv(_PUCO_CLKp, !_PUCO_CLKp, _PULY_FLIP5n);
    /*p33.RAMA*/ RAMA_SPRITE_DB6n = dff8_A_inv(_PUCO_CLKp, !_PUCO_CLKp, _PELO_FLIP6n);
    /*p33.RYDU*/ RYDU_SPRITE_DB7n = dff8_A_inv(_PUCO_CLKp, !_PUCO_CLKp, _PAWE_FLIP7n);

    /*#p29.VYWA*/ wire _VYWA_CLKp = not1(TOPU_LATCH_SPPIXA);
    /*#p29.WENY*/ wire _WENY_CLKn = not1(_VYWA_CLKp);
    /*#p29.XADO*/ wire _XADO_CLKp = not1(_WENY_CLKn);

    /*p33.REWO*/ REWO_SPRITE_DA0n = dff8_A_inv(_XADO_CLKp, !_XADO_CLKp, _POBE_FLIP0n);
    /*p33.PEBA*/ PEBA_SPRITE_DA1n = dff8_A_inv(_XADO_CLKp, !_XADO_CLKp, _PACY_FLIP1n);
    /*p33.MOFO*/ MOFO_SPRITE_DA2n = dff8_A_inv(_XADO_CLKp, !_XADO_CLKp, _PONO_FLIP2n);
    /*p33.PUDU*/ PUDU_SPRITE_DA3n = dff8_A_inv(_XADO_CLKp, !_XADO_CLKp, _PUGU_FLIP3n);
    /*p33.SAJA*/ SAJA_SPRITE_DA4n = dff8_A_inv(_XADO_CLKp, !_XADO_CLKp, _PUTE_FLIP4n);
    /*p33.SUNY*/ SUNY_SPRITE_DA5n = dff8_A_inv(_XADO_CLKp, !_XADO_CLKp, _PULY_FLIP5n);
    /*p33.SEMO*/ SEMO_SPRITE_DA6n = dff8_A_inv(_XADO_CLKp, !_XADO_CLKp, _PELO_FLIP6n);
    /*p33.SEGA*/ SEGA_SPRITE_DA7n = dff8_A_inv(_XADO_CLKp, !_XADO_CLKp, _PAWE_FLIP7n);
  }
}

//------------------------------------------------------------------------------
