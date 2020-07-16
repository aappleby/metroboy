#include "Sch_VramBus.h"

#include "Sch_Top.h"

using namespace Schematics;

VramBus::VramBus(){
  _VRM_TRI_A00.preset_a(0);
  _VRM_TRI_A01.preset_a(0);
  _VRM_TRI_A02.preset_a(0);
  _VRM_TRI_A03.preset_a(0);
  _VRM_TRI_A04.preset_a(0);
  _VRM_TRI_A05.preset_a(0);
  _VRM_TRI_A06.preset_a(0);
  _VRM_TRI_A07.preset_a(0);
  _VRM_TRI_A08.preset_a(0);
  _VRM_TRI_A09.preset_a(0);
  _VRM_TRI_A10.preset_a(0);
  _VRM_TRI_A11.preset_a(0);
  _VRM_TRI_A12.preset_a(0);

  _VRM_TRI_D0.preset_a(0);
  _VRM_TRI_D1.preset_a(0);
  _VRM_TRI_D2.preset_a(0);
  _VRM_TRI_D3.preset_a(0);
  _VRM_TRI_D4.preset_a(0);
  _VRM_TRI_D5.preset_a(0);
  _VRM_TRI_D6.preset_a(0);
  _VRM_TRI_D7.preset_a(0);
  
  _VRAM_PIN_MCSn_C.set(0);
  _VRAM_PIN_MOEn_C.set(0);
  _VRAM_PIN_MWRn_C.set(0);
  
  _VRAM_PIN_MD0_C.set(0);
  _VRAM_PIN_MD1_C.set(0);
  _VRAM_PIN_MD2_C.set(0);
  _VRAM_PIN_MD3_C.set(0);
  _VRAM_PIN_MD4_C.set(0);
  _VRAM_PIN_MD5_C.set(0);
  _VRAM_PIN_MD6_C.set(0);
  _VRAM_PIN_MD7_C.set(0);
}

//------------------------------------------------------------------------------

void VramBus::tock(SchematicTop& top) {

  /*p25.TAVY*/ wire _TAVY_MOEp = not(_VRAM_PIN_MOEn_C);
  /*p25.SALE*/ wire _SALE_VRAM_WRn = mux2_p(_TAVY_MOEp, top.cpu_bus.TEGU_CPU_VRAM_WRn(), top.clk_reg.TUTO_DBG_VRAMp());

  /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and (top.cpu_bus.SOSE_8000_9FFFp(), top.ABUZ_CPU_ADDR_VALIDp());
  /*p25.TEFY*/ wire _TEFY_VRAM_MCSp = not(_VRAM_PIN_MCSn_C);
  /*p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2_p(_TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp, top.clk_reg.TUTO_DBG_VRAMp());
  /*p25.SERE*/ wire _SERE_CPU_VRM_RDp = and (_TOLE_VRAM_RDp, top.pix_pipe.ROPY_RENDERINGn());
  /*p25.RUVY*/ wire _RUVY_VRAM_WR = not(_SALE_VRAM_WRn);
  /*p25.SAZO*/ wire _SAZO_VRAM_RD = and (_RUVY_VRAM_WR, _SERE_CPU_VRM_RDp);
  /*p25.RYJE*/ wire _RYJE_VRAM_RDn = not(_SAZO_VRAM_RD);
  /*p25.REVO*/ wire _REVO_VRAM_RDp = not(_RYJE_VRAM_RDn);
  /*p25.RELA*/ wire _RELA_VRM_TO_CPUn = or (_REVO_VRAM_RDp, _SAZO_VRAM_RD);
  /*p25.RENA*/ wire _RENA_VRM_TO_CPUp = not(_RELA_VRM_TO_CPUn);

  //----------------------------------------
  // VRAM pins

  {
    /*p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp = and(top.cpu_bus.SOSE_8000_9FFFp(), top.APOV_CPU_WRp_xxxxEFGx());
    /*p25.SUDO*/ wire _SUDO_MWRp_C = not(_VRAM_PIN_MWRn_C);
    /*p25.TYJY*/ wire _TYJY_DBG_VRAM_WRp = mux2_p(_SUDO_MWRp_C, _TUJA_CPU_VRAM_WRp, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SOHY*/ wire _SOHY_MWRn = nand(_TYJY_DBG_VRAM_WRp, _SERE_CPU_VRM_RDp);
    /*p25.TAXY*/ wire _TAXY_MWRn_A = and(_SOHY_MWRn, top.clk_reg.RACO_DBG_VRAMn());
    /*p25.SOFY*/ wire _SOFY_MWRn_D = or (_SOHY_MWRn, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SYSY*/ wire _SYSY_MWRp_A = not(_TAXY_MWRn_A);
    /*p25.RAGU*/ wire _RAGU_MWRp_D = not(_SOFY_MWRn_D);
    _VRAM_PIN_MWRn_A.set(_SYSY_MWRp_A);
    _VRAM_PIN_MWRn_D.set(_RAGU_MWRp_D);
  }


  {
    /*p25.XANE*/ wire _XANE_VRAM_LOCKn = nor(top.dma_reg.LUFA_DMA_VRM_RDp(), top.pix_pipe.XYMU_RENDERINGp()); // def nor
    /*p25.RYLU*/ wire _RYLU_DBG_VRAM_RDn = nand(_SALE_VRAM_WRn, _XANE_VRAM_LOCKn);
    /*p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp = and(top.sprite_fetcher.TACU_SPR_SEQ_5_TRIG(), top.sprite_fetcher.ABON_SPR_VRM_RDn());
    /*p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn = not(_SOHO_SPR_VRAM_RDp);
    /*p25.APAM*/ wire _APAM_DMA_VRAM_RDn = not(top.dma_reg.LUFA_DMA_VRM_RDp());
    /*p25.RACU*/ wire _RACU_MOEn = and(_RYLU_DBG_VRAM_RDn, _RAWA_SPR_VRAM_RDn, top.tile_fetcher.MYMA_BGW_VRAM_RDn(), _APAM_DMA_VRAM_RDn); // def and
    /*p25.SEMA*/ wire _SEMA_MOEn_A = and(_RACU_MOEn, top.clk_reg.RACO_DBG_VRAMn());
    /*p25.RUTE*/ wire _RUTE_MOEn_D = or (_RACU_MOEn, top.clk_reg.TUTO_DBG_VRAMp()); // schematic wrong, second input is RACU
    /*p25.REFO*/ wire _REFO_MOEn_A = not(_SEMA_MOEn_A);
    /*p25.SAHA*/ wire _SAHA_MOEn_D = not(_RUTE_MOEn_D);
    _VRAM_PIN_MOEn_A.set(_REFO_MOEn_A);
    _VRAM_PIN_MOEn_D.set(_SAHA_MOEn_D);
  }

  {
    // Polarity issues here, ABON should be P
    // ABON = not(TEXY)
    // SUTU = nor(LENA, LUFA, ABON, SERE);
    /*p25.SUTU*/ wire _SUTU_MCSn = nor(top.tile_fetcher.LENA_BGW_VRM_RDp(),
                                       top.dma_reg.LUFA_DMA_VRM_RDp(),
                                       top.sprite_fetcher.ABON_SPR_VRM_RDn(),
                                       _SERE_CPU_VRM_RDp);
    /*p25.TODE*/ wire _TODE_MCSn_A = and(_SUTU_MCSn, top.clk_reg.RACO_DBG_VRAMn());
    /*p25.SEWO*/ wire _SEWO_MCSn_D = or (_SUTU_MCSn, top.clk_reg.TUTO_DBG_VRAMp());
    /*p25.SOKY*/ wire _SOKY_MCSp_A = not(_TODE_MCSn_A);
    /*p25.SETY*/ wire _SETY_MCSp_D = not(_SEWO_MCSn_D);
    _VRAM_PIN_MCSn_A.set(_SOKY_MCSp_A);
    _VRAM_PIN_MCSn_D.set(_SETY_MCSp_D);
  }

  //----------------------------------------
  // VRAM address

  // CPU address -> vram address
  {
    /*p25.XANE*/ wire _XANE_VRAM_LOCKn = nor(top.dma_reg.LUFA_DMA_VRM_RDp(), top.pix_pipe.XYMU_RENDERINGp()); // def nor
    /*p25.XEDU*/ wire _XEDU_VRAM_UNLOCKEDn = not(_XANE_VRAM_LOCKn);
    /*p25.XAKY*/ _VRM_TRI_A00.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A00);
    /*p25.XUXU*/ _VRM_TRI_A01.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A01);
    /*p25.XYNE*/ _VRM_TRI_A02.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A02);
    /*p25.XODY*/ _VRM_TRI_A03.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A03);
    /*p25.XECA*/ _VRM_TRI_A04.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A04);
    /*p25.XOBA*/ _VRM_TRI_A05.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A05);
    /*p25.XOPO*/ _VRM_TRI_A06.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A06);
    /*p25.XYBO*/ _VRM_TRI_A07.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A07);
    /*p25.RYSU*/ _VRM_TRI_A08.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A08);
    /*p25.RESE*/ _VRM_TRI_A09.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A09);
    /*p25.RUSE*/ _VRM_TRI_A10.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A10);
    /*p25.RYNA*/ _VRM_TRI_A11.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A11);
    /*p25.RUMO*/ _VRM_TRI_A12.set_tribuf_6n(_XEDU_VRAM_UNLOCKEDn, top.cpu_bus.CPU_PIN_A12);
  }

  // DMA address -> vram address
  {
    /*p04.AHOC*/ wire _AHOC_DMA_VRAM_RDn = not(top.dma_reg.LUFA_DMA_VRM_RDp());
    /*p04.ECAL*/ _VRM_TRI_A00.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A00.q());
    /*p04.EGEZ*/ _VRM_TRI_A01.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A01.q());
    /*p04.FUHE*/ _VRM_TRI_A02.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A02.q());
    /*p04.FYZY*/ _VRM_TRI_A03.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A03.q());
    /*p04.DAMU*/ _VRM_TRI_A04.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A04.q());
    /*p04.DAVA*/ _VRM_TRI_A05.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A05.q());
    /*p04.ETEG*/ _VRM_TRI_A06.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A06.q());
    /*p04.EREW*/ _VRM_TRI_A07.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A07.q());
    /*p04.EVAX*/ _VRM_TRI_A08.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A08.q());
    /*p04.DUVE*/ _VRM_TRI_A09.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A09.q());
    /*p04.ERAF*/ _VRM_TRI_A10.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A10.q());
    /*p04.FUSY*/ _VRM_TRI_A11.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A11.q());
    /*p04.EXYF*/ _VRM_TRI_A12.set_tribuf_6n(_AHOC_DMA_VRAM_RDn, top.dma_reg.DMA_A12.q());
  }

  // Sprite fetcher read
  {
    /*p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not(top.pix_pipe.XYMO_LCDC_SPSIZE.q());
    /*p29.WUKY*/ wire _WUKY_FLIP_Y = not(top.YZOS_OAM_DA6.q());
    /*p29.CYVU*/ wire _CYVU_OAM_DB0 = xor (_WUKY_FLIP_Y, top.sprite_store.SPR_TRI_LINE_1.q());
    /*p29.BORE*/ wire _BORE_OAM_DB1 = xor (_WUKY_FLIP_Y, top.sprite_store.SPR_TRI_LINE_2.q());
    /*p29.BUVY*/ wire _BUVY_OAM_DB2 = xor (_WUKY_FLIP_Y, top.sprite_store.SPR_TRI_LINE_3.q());
    /*p29.WAGO*/ wire _WAGO = xor (_WUKY_FLIP_Y, top.sprite_store.SPR_TRI_LINE_0.q());
    /*p29.GEJY*/ wire _GEJY_OAM_DB3 = amux2(_FUFO_LCDC_SPSIZEn, !top.XUSO_OAM_DB0.q(), top.pix_pipe.XYMO_LCDC_SPSIZE.q(), _WAGO);

    /*p29.ABEM*/ _VRM_TRI_A00.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), top.sprite_fetcher.XUQU_SPRITE_AB());
    /*p29.BAXE*/ _VRM_TRI_A01.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), _CYVU_OAM_DB0);
    /*p29.ARAS*/ _VRM_TRI_A02.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), _BORE_OAM_DB1);
    /*p29.AGAG*/ _VRM_TRI_A03.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), _BUVY_OAM_DB2);
    /*p29.FAMU*/ _VRM_TRI_A04.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), _GEJY_OAM_DB3);
    /*p29.FUGY*/ _VRM_TRI_A05.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), top.XEGU_OAM_DB1.q());
    /*p29.GAVO*/ _VRM_TRI_A06.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), top.YJEX_OAM_DB2.q());
    /*p29.WYGA*/ _VRM_TRI_A07.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), top.XYJU_OAM_DB3.q());
    /*p29.WUNE*/ _VRM_TRI_A08.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), top.YBOG_OAM_DB4.q());
    /*p29.GOTU*/ _VRM_TRI_A09.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), top.WYSO_OAM_DB5.q());
    /*p29.GEGU*/ _VRM_TRI_A10.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), top.XOTE_OAM_DB6.q());
    /*p29.XEHE*/ _VRM_TRI_A11.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), top.YZAB_OAM_DB7.q());
    /*p29.DYSO*/ _VRM_TRI_A12.set_tribuf_6n(top.sprite_fetcher.ABON_SPR_VRM_RDn(), 0);   // sprites always in low half of tile store
  }

  {
    /*p26.FAFO*/ wire _FAFO_TILE_Y0S = add_s(top.lcd_reg.MUWY_Y0.q(), top.tile_fetcher.GAVE_SCY0.q(), 0);
    /*p26.FAFO*/ wire _FAFO_TILE_Y0C = add_c(top.lcd_reg.MUWY_Y0.q(), top.tile_fetcher.GAVE_SCY0.q(), 0);
    /*p26.EMUX*/ wire _EMUX_TILE_Y1S = add_s(top.lcd_reg.MYRO_Y1.q(), top.tile_fetcher.FYMO_SCY1.q(), _FAFO_TILE_Y0C);
    /*p26.EMUX*/ wire _EMUX_TILE_Y1C = add_c(top.lcd_reg.MYRO_Y1.q(), top.tile_fetcher.FYMO_SCY1.q(), _FAFO_TILE_Y0C);
    /*p26.ECAB*/ wire _ECAB_TILE_Y2S = add_s(top.lcd_reg.LEXA_Y2.q(), top.tile_fetcher.FEZU_SCY2.q(), _EMUX_TILE_Y1C);
    /*p26.ECAB*/ wire _ECAB_TILE_Y2C = add_c(top.lcd_reg.LEXA_Y2.q(), top.tile_fetcher.FEZU_SCY2.q(), _EMUX_TILE_Y1C);
    /*p26.ETAM*/ wire _ETAM_MAP_Y0S  = add_s(top.lcd_reg.LYDO_Y3.q(), top.tile_fetcher.FUJO_SCY3.q(), _ECAB_TILE_Y2C);
    /*p26.ETAM*/ wire _ETAM_MAP_Y0C  = add_c(top.lcd_reg.LYDO_Y3.q(), top.tile_fetcher.FUJO_SCY3.q(), _ECAB_TILE_Y2C);
    /*p26.DOTO*/ wire _DOTO_MAP_Y1S  = add_s(top.lcd_reg.LOVU_Y4.q(), top.tile_fetcher.DEDE_SCY4.q(), _ETAM_MAP_Y0C);
    /*p26.DOTO*/ wire _DOTO_MAP_Y1C  = add_c(top.lcd_reg.LOVU_Y4.q(), top.tile_fetcher.DEDE_SCY4.q(), _ETAM_MAP_Y0C);
    /*p26.DABA*/ wire _DABA_MAP_Y2S  = add_s(top.lcd_reg.LEMA_Y5.q(), top.tile_fetcher.FOTY_SCY5.q(), _DOTO_MAP_Y1C);
    /*p26.DABA*/ wire _DABA_MAP_Y2C  = add_c(top.lcd_reg.LEMA_Y5.q(), top.tile_fetcher.FOTY_SCY5.q(), _DOTO_MAP_Y1C);
    /*p26.EFYK*/ wire _EFYK_MAP_Y3S  = add_s(top.lcd_reg.MATO_Y6.q(), top.tile_fetcher.FOHA_SCY6.q(), _DABA_MAP_Y2C);
    /*p26.EFYK*/ wire _EFYK_MAP_Y3C  = add_c(top.lcd_reg.MATO_Y6.q(), top.tile_fetcher.FOHA_SCY6.q(), _DABA_MAP_Y2C);
    /*p26.EJOK*/ wire _EJOK_MAP_Y4S  = add_s(top.lcd_reg.LAFO_Y7.q(), top.tile_fetcher.FUNY_SCY7.q(), _EFYK_MAP_Y3C);
    /*p26.EJOK*/ wire _EJOK_MAP_Y4C  = add_c(top.lcd_reg.LAFO_Y7.q(), top.tile_fetcher.FUNY_SCY7.q(), _EFYK_MAP_Y3C);

    (void)_EJOK_MAP_Y4C;

    /*p26.ATAD*/ wire _ATAD_TILE_X0S = add_s(top.pix_pipe.XEHO_X0.q(), top.tile_fetcher.DATY_SCX0.q(), 0);
    /*p26.ATAD*/ wire _ATAD_TILE_X0C = add_c(top.pix_pipe.XEHO_X0.q(), top.tile_fetcher.DATY_SCX0.q(), 0);
    /*p26.BEHU*/ wire _BEHU_TILE_X1S = add_s(top.pix_pipe.SAVY_X1.q(), top.tile_fetcher.DUZU_SCX1.q(), _ATAD_TILE_X0C);
    /*p26.BEHU*/ wire _BEHU_TILE_X1C = add_c(top.pix_pipe.SAVY_X1.q(), top.tile_fetcher.DUZU_SCX1.q(), _ATAD_TILE_X0C);
    /*p26.APYH*/ wire _APYH_TILE_X2S = add_s(top.pix_pipe.XODU_X2.q(), top.tile_fetcher.CYXU_SCX2.q(), _BEHU_TILE_X1C);
    /*p26.APYH*/ wire _APYH_TILE_X2C = add_c(top.pix_pipe.XODU_X2.q(), top.tile_fetcher.CYXU_SCX2.q(), _BEHU_TILE_X1C);
    /*p26.BABE*/ wire _BABE_MAP_X0S  = add_s(top.pix_pipe.XYDO_X3.q(), top.tile_fetcher.GUBO_SCX3.q(), _APYH_TILE_X2C);
    /*p26.BABE*/ wire _BABE_MAP_X0C  = add_c(top.pix_pipe.XYDO_X3.q(), top.tile_fetcher.GUBO_SCX3.q(), _APYH_TILE_X2C);
    /*p26.ABOD*/ wire _ABOD_MAP_X1S  = add_s(top.pix_pipe.TUHU_X4.q(), top.tile_fetcher.BEMY_SCX4.q(), _BABE_MAP_X0C);
    /*p26.ABOD*/ wire _ABOD_MAP_X1C  = add_c(top.pix_pipe.TUHU_X4.q(), top.tile_fetcher.BEMY_SCX4.q(), _BABE_MAP_X0C);
    /*p26.BEWY*/ wire _BEWY_MAP_X2S  = add_s(top.pix_pipe.TUKY_X5.q(), top.tile_fetcher.CUZY_SCX5.q(), _ABOD_MAP_X1C);
    /*p26.BEWY*/ wire _BEWY_MAP_X2C  = add_c(top.pix_pipe.TUKY_X5.q(), top.tile_fetcher.CUZY_SCX5.q(), _ABOD_MAP_X1C);
    /*p26.BYCA*/ wire _BYCA_MAP_X3S  = add_s(top.pix_pipe.TAKO_X6.q(), top.tile_fetcher.CABU_SCX6.q(), _BEWY_MAP_X2C);
    /*p26.BYCA*/ wire _BYCA_MAP_X3C  = add_c(top.pix_pipe.TAKO_X6.q(), top.tile_fetcher.CABU_SCX6.q(), _BEWY_MAP_X2C);
    /*p26.ACUL*/ wire _ACUL_MAP_X4S  = add_s(top.pix_pipe.SYBE_X7.q(), top.tile_fetcher.BAKE_SCX7.q(), _BYCA_MAP_X3C);
    /*p26.ACUL*/ wire _ACUL_MAP_X4C  = add_c(top.pix_pipe.SYBE_X7.q(), top.tile_fetcher.BAKE_SCX7.q(), _BYCA_MAP_X3C);

    (void)_ATAD_TILE_X0S;
    (void)_BEHU_TILE_X1S;
    (void)_APYH_TILE_X2S;
    (void)_ACUL_MAP_X4C;

    /*p27.NOGU*/ wire _NOGU_FETCH_01p = nand(top.tile_fetcher.NAKO_BFETCH_S1n(), top.tile_fetcher.NOFU_BFETCH_S2n());
    /*p27.NENY*/ wire _NENY_FETCH_01n = not(_NOGU_FETCH_01p);
    /*p27.POTU*/ wire _POTU_BG_MAP_READp = and (top.tile_fetcher.LENA_BGW_VRM_RDp(), _NENY_FETCH_01n);
    /*p26.AXAD*/ wire _AXAD_WIN_MODEn = not(top.pix_pipe.PORE_WIN_MODEp());

    // Background map read
    {
      /*p26.ACEN*/ wire _ACEN_BG_MAP_READp = and (_POTU_BG_MAP_READp, _AXAD_WIN_MODEn);
      /*p26.BAFY*/ wire _BAFY_BG_MAP_READn = not(_ACEN_BG_MAP_READp);
      /*p26.AXEP*/ _VRM_TRI_A00.set_tribuf_6n(_BAFY_BG_MAP_READn, _BABE_MAP_X0S);
      /*p26.AFEB*/ _VRM_TRI_A01.set_tribuf_6n(_BAFY_BG_MAP_READn, _ABOD_MAP_X1S);
      /*p26.ALEL*/ _VRM_TRI_A02.set_tribuf_6n(_BAFY_BG_MAP_READn, _BEWY_MAP_X2S);
      /*p26.COLY*/ _VRM_TRI_A03.set_tribuf_6n(_BAFY_BG_MAP_READn, _BYCA_MAP_X3S);
      /*p26.AJAN*/ _VRM_TRI_A04.set_tribuf_6n(_BAFY_BG_MAP_READn, _ACUL_MAP_X4S);
      /*p26.DUHO*/ _VRM_TRI_A05.set_tribuf_6n(_BAFY_BG_MAP_READn, _ETAM_MAP_Y0S);
      /*p26.CASE*/ _VRM_TRI_A06.set_tribuf_6n(_BAFY_BG_MAP_READn, _DOTO_MAP_Y1S);
      /*p26.CYPO*/ _VRM_TRI_A07.set_tribuf_6n(_BAFY_BG_MAP_READn, _DABA_MAP_Y2S);
      /*p26.CETA*/ _VRM_TRI_A08.set_tribuf_6n(_BAFY_BG_MAP_READn, _EFYK_MAP_Y3S);
      /*p26.DAFE*/ _VRM_TRI_A09.set_tribuf_6n(_BAFY_BG_MAP_READn, _EJOK_MAP_Y4S);
      /*p26.AMUV*/ _VRM_TRI_A10.set_tribuf_6n(_BAFY_BG_MAP_READn, top.pix_pipe.XAFO_LCDC_BGMAP.q());
      /*p26.COVE*/ _VRM_TRI_A11.set_tribuf_6n(_BAFY_BG_MAP_READn, 0);
      /*p26.COXO*/ _VRM_TRI_A12.set_tribuf_6n(_BAFY_BG_MAP_READn, 0);
    }

    // Window map read
    {
      /*p25.XEZE*/ wire _XEZE_WIN_MAP_READp = and (_POTU_BG_MAP_READp, top.pix_pipe.PORE_WIN_MODEp());
      /*p25.WUKO*/ wire _WUKO_WIN_MAP_READn = not(_XEZE_WIN_MAP_READp);
      /*p27.XEJA*/ _VRM_TRI_A00.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.WYKA_WIN_X3());
      /*p27.XAMO*/ _VRM_TRI_A01.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.WODY_WIN_X4());
      /*p27.XAHE*/ _VRM_TRI_A02.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.WOBO_WIN_X5());
      /*p27.XULO*/ _VRM_TRI_A03.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.WYKO_WIN_X6());
      /*p27.WUJU*/ _VRM_TRI_A04.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.XOLO_WIN_X7());
      /*p27.VYTO*/ _VRM_TRI_A05.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.TUFU_WIN_Y3());
      /*p27.VEHA*/ _VRM_TRI_A06.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.TAXA_WIN_Y4());
      /*p27.VACE*/ _VRM_TRI_A07.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.TOZO_WIN_Y5());
      /*p27.VOVO*/ _VRM_TRI_A08.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.TATE_WIN_Y6());
      /*p27.VULO*/ _VRM_TRI_A09.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.TEKE_WIN_Y7());
      /*p27.VEVY*/ _VRM_TRI_A10.set_tribuf_6n(_WUKO_WIN_MAP_READn, top.pix_pipe.WOKY_LCDC_WINMAP.q());
      /*p27.VEZA*/ _VRM_TRI_A11.set_tribuf_6n(_WUKO_WIN_MAP_READn, 0);
      /*p27.VOGU*/ _VRM_TRI_A12.set_tribuf_6n(_WUKO_WIN_MAP_READn, 0);
    }

    // Background/window tile read
    {
      /*p27.XUHA*/ wire _XUHA_FETCH_S2p  = not (top.tile_fetcher.NOFU_BFETCH_S2n());
      /*p27.NETA*/ wire _NETA_TILE_READp = and (top.tile_fetcher.LENA_BGW_VRM_RDp(), _NOGU_FETCH_01p);
      /*p26.ASUL*/ wire _ASUL_TILE_READp = and (_NETA_TILE_READp, _AXAD_WIN_MODEn);
      /*p26.BEJE*/ wire _BEJE_BGD_TILE_READn = not (_ASUL_TILE_READp);
      /*p25.XUCY*/ wire _XUCY_WIN_TILE_READn = nand(_NETA_TILE_READp, top.pix_pipe.PORE_WIN_MODEp());
      /*p25.VUZA*/ wire _VUZA_TILE_BANKp = nor(top.pix_pipe.WEXU_LCDC_BGTILE.q(), top.PYJU_TILE_DB7.q()); // register reused
      
      /*p26.ASUM*/ _VRM_TRI_A00.set_tribuf_6n(_BEJE_BGD_TILE_READn, _XUHA_FETCH_S2p);
      /*p26.EVAD*/ _VRM_TRI_A01.set_tribuf_6n(_BEJE_BGD_TILE_READn, _FAFO_TILE_Y0S);
      /*p26.DAHU*/ _VRM_TRI_A02.set_tribuf_6n(_BEJE_BGD_TILE_READn, _EMUX_TILE_Y1S);
      /*p26.DODE*/ _VRM_TRI_A03.set_tribuf_6n(_BEJE_BGD_TILE_READn, _ECAB_TILE_Y2S);

      /*p25.XONU*/ _VRM_TRI_A00.set_tribuf_6n(_XUCY_WIN_TILE_READn, _XUHA_FETCH_S2p);
      /*p25.WUDO*/ _VRM_TRI_A01.set_tribuf_6n(_XUCY_WIN_TILE_READn, top.pix_pipe.VYNO_WIN_Y0());
      /*p25.WAWE*/ _VRM_TRI_A02.set_tribuf_6n(_XUCY_WIN_TILE_READn, top.pix_pipe.VUJO_WIN_Y1());
      /*p25.WOLU*/ _VRM_TRI_A03.set_tribuf_6n(_XUCY_WIN_TILE_READn, top.pix_pipe.VYMU_WIN_Y2());

      /*p25.VAPY*/ _VRM_TRI_A04.set_tribuf_6p(_NETA_TILE_READp, top.RAWU_TILE_DB0.q());
      /*p25.SEZU*/ _VRM_TRI_A05.set_tribuf_6p(_NETA_TILE_READp, top.POZO_TILE_DB1.q());
      /*p25.VEJY*/ _VRM_TRI_A06.set_tribuf_6p(_NETA_TILE_READp, top.PYZO_TILE_DB2.q());
      /*p25.RUSA*/ _VRM_TRI_A07.set_tribuf_6p(_NETA_TILE_READp, top.POXA_TILE_DB3.q());
      /*p25.ROHA*/ _VRM_TRI_A08.set_tribuf_6p(_NETA_TILE_READp, top.PULO_TILE_DB4.q());
      /*p25.RESO*/ _VRM_TRI_A09.set_tribuf_6p(_NETA_TILE_READp, top.POJU_TILE_DB5.q());
      /*p25.SUVO*/ _VRM_TRI_A10.set_tribuf_6p(_NETA_TILE_READp, top.POWY_TILE_DB6.q());
      /*p25.TOBO*/ _VRM_TRI_A11.set_tribuf_6p(_NETA_TILE_READp, top.PYJU_TILE_DB7.q());
      /*p25.VURY*/ _VRM_TRI_A12.set_tribuf_6p(_NETA_TILE_READp, _VUZA_TILE_BANKp);
    }
  }

  // VRAM atri -> VRAM pin
  {
    /*p25.MYFU*/ wire _MYFU = not(_VRM_TRI_A00.q());
    /*p25.MASA*/ wire _MASA = not(_VRM_TRI_A01.q());
    /*p25.MYRE*/ wire _MYRE = not(_VRM_TRI_A02.q());
    /*p25.MAVU*/ wire _MAVU = not(_VRM_TRI_A03.q());
    /*p25.MEPA*/ wire _MEPA = not(_VRM_TRI_A04.q());
    /*p25.MYSA*/ wire _MYSA = not(_VRM_TRI_A05.q());
    /*p25.MEWY*/ wire _MEWY = not(_VRM_TRI_A06.q());
    /*p25.MUME*/ wire _MUME = not(_VRM_TRI_A07.q());
    /*p25.VOVA*/ wire _VOVA = not(_VRM_TRI_A08.q());
    /*p25.VODE*/ wire _VODE = not(_VRM_TRI_A09.q());
    /*p25.RUKY*/ wire _RUKY = not(_VRM_TRI_A10.q());
    /*p25.RUMA*/ wire _RUMA = not(_VRM_TRI_A11.q());
    /*p25.REHO*/ wire _REHO = not(_VRM_TRI_A12.q());

    /*p25.LEXE*/ _VRAM_PIN_MA00_AD.set(_MYFU);
    /*p25.LOZU*/ _VRAM_PIN_MA01_AD.set(_MASA);
    /*p25.LACA*/ _VRAM_PIN_MA02_AD.set(_MYRE);
    /*p25.LUVO*/ _VRAM_PIN_MA03_AD.set(_MAVU);
    /*p25.LOLY*/ _VRAM_PIN_MA04_AD.set(_MEPA);
    /*p25.LALO*/ _VRAM_PIN_MA05_AD.set(_MYSA);
    /*p25.LEFA*/ _VRAM_PIN_MA06_AD.set(_MEWY);
    /*p25.LUBY*/ _VRAM_PIN_MA07_AD.set(_MUME);
    /*p25.TUJY*/ _VRAM_PIN_MA08_AD.set(_VOVA);
    /*p25.TAGO*/ _VRAM_PIN_MA09_AD.set(_VODE);
    /*p25.NUVA*/ _VRAM_PIN_MA10_AD.set(_RUKY);
    /*p25.PEDU*/ _VRAM_PIN_MA11_AD.set(_RUMA);
    /*p25.PONY*/ _VRAM_PIN_MA12_AD.set(_REHO);
  }

  //----------------------------------------
  // VRAM data out

  // Int bus -> VRAM dtri
  {
    /*p25.ROCY*/ wire _ROCY_CPU_TO_VRMp = and (_REVO_VRAM_RDp, _SAZO_VRAM_RD);
    /*p25.RAHU*/ wire _RAHU_CPU_TO_VRMn = not(_ROCY_CPU_TO_VRMp);
    /*p25.TEME*/ _VRM_TRI_D0.set_tribuf_10n(_RAHU_CPU_TO_VRMn, top.cpu_bus.CPU_TRI_D0.q());
    /*p25.TEWU*/ _VRM_TRI_D1.set_tribuf_10n(_RAHU_CPU_TO_VRMn, top.cpu_bus.CPU_TRI_D1.q());
    /*p25.TYGO*/ _VRM_TRI_D2.set_tribuf_10n(_RAHU_CPU_TO_VRMn, top.cpu_bus.CPU_TRI_D2.q());
    /*p25.SOTE*/ _VRM_TRI_D3.set_tribuf_10n(_RAHU_CPU_TO_VRMn, top.cpu_bus.CPU_TRI_D3.q());
    /*p25.SEKE*/ _VRM_TRI_D4.set_tribuf_10n(_RAHU_CPU_TO_VRMn, top.cpu_bus.CPU_TRI_D4.q());
    /*p25.RUJO*/ _VRM_TRI_D5.set_tribuf_10n(_RAHU_CPU_TO_VRMn, top.cpu_bus.CPU_TRI_D5.q());
    /*p25.TOFA*/ _VRM_TRI_D6.set_tribuf_10n(_RAHU_CPU_TO_VRMn, top.cpu_bus.CPU_TRI_D6.q());
    /*p25.SUZA*/ _VRM_TRI_D7.set_tribuf_10n(_RAHU_CPU_TO_VRMn, top.cpu_bus.CPU_TRI_D7.q());
  }

  // VRAM dtri -> VRAM pin
  {
    /*p25.RYJE*/ wire _RYJE_VRAM_RDn = not(_SAZO_VRAM_RD);
    /*p25.REVO*/ wire _REVO_VRAM_RDp = not(_RYJE_VRAM_RDn);
    /*p25.ROCY*/ wire _ROCY_CPU_TO_VRMp = and (_REVO_VRAM_RDp, _SAZO_VRAM_RD);
    /*p25.RAHU*/ wire _RAHU_CPU_TO_VRMn = not(_ROCY_CPU_TO_VRMp);
    /*p25.ROVE*/ wire _ROVE_CPU_TO_VRMp = not(_RAHU_CPU_TO_VRMn);
    /*p25.SEFA*/ wire _SEFA = and(_VRM_TRI_D0.q(), _ROVE_CPU_TO_VRMp);
    /*p25.SOGO*/ wire _SOGO = and(_VRM_TRI_D1.q(), _ROVE_CPU_TO_VRMp);
    /*p25.SEFU*/ wire _SEFU = and(_VRM_TRI_D2.q(), _ROVE_CPU_TO_VRMp);
    /*p25.SUNA*/ wire _SUNA = and(_VRM_TRI_D3.q(), _ROVE_CPU_TO_VRMp);
    /*p25.SUMO*/ wire _SUMO = and(_VRM_TRI_D4.q(), _ROVE_CPU_TO_VRMp);
    /*p25.SAZU*/ wire _SAZU = and(_VRM_TRI_D5.q(), _ROVE_CPU_TO_VRMp);
    /*p25.SAMO*/ wire _SAMO = and(_VRM_TRI_D6.q(), _ROVE_CPU_TO_VRMp);
    /*p25.SUKE*/ wire _SUKE = and(_VRM_TRI_D7.q(), _ROVE_CPU_TO_VRMp);

    /*p25.REGE*/ wire _REGE = not(_SEFA);
    /*p25.RYKY*/ wire _RYKY = not(_SOGO);
    /*p25.RAZO*/ wire _RAZO = not(_SEFU);
    /*p25.RADA*/ wire _RADA = not(_SUNA);
    /*p25.RYRO*/ wire _RYRO = not(_SUMO);
    /*p25.REVU*/ wire _REVU = not(_SAZU);
    /*p25.REKU*/ wire _REKU = not(_SAMO);
    /*p25.RYZE*/ wire _RYZE = not(_SUKE);

    _VRAM_PIN_MD0_A.set(_REGE);
    _VRAM_PIN_MD1_A.set(_RYKY);
    _VRAM_PIN_MD2_A.set(_RAZO);
    _VRAM_PIN_MD3_A.set(_RADA);
    _VRAM_PIN_MD4_A.set(_RYRO);
    _VRAM_PIN_MD5_A.set(_REVU);
    _VRAM_PIN_MD6_A.set(_REKU);
    _VRAM_PIN_MD7_A.set(_RYZE);

    /*p25.ROFA*/ wire _ROFA_VRM_TO_CPUn = not(_RENA_VRM_TO_CPUp);
    _VRAM_PIN_MD0_B.set(_ROFA_VRM_TO_CPUn);
    _VRAM_PIN_MD1_B.set(_ROFA_VRM_TO_CPUn);
    _VRAM_PIN_MD2_B.set(_ROFA_VRM_TO_CPUn);
    _VRAM_PIN_MD3_B.set(_ROFA_VRM_TO_CPUn);
    _VRAM_PIN_MD4_B.set(_ROFA_VRM_TO_CPUn);
    _VRAM_PIN_MD5_B.set(_ROFA_VRM_TO_CPUn);
    _VRAM_PIN_MD6_B.set(_ROFA_VRM_TO_CPUn);
    _VRAM_PIN_MD7_B.set(_ROFA_VRM_TO_CPUn);

    /*p25.SYNU*/ wire _SYNU = or (_VRM_TRI_D0.q(), _RAHU_CPU_TO_VRMn);
    /*p25.SYMA*/ wire _SYMA = or (_VRM_TRI_D1.q(), _RAHU_CPU_TO_VRMn);
    /*p25.ROKO*/ wire _ROKO = or (_VRM_TRI_D2.q(), _RAHU_CPU_TO_VRMn);
    /*p25.SYBU*/ wire _SYBU = or (_VRM_TRI_D3.q(), _RAHU_CPU_TO_VRMn);
    /*p25.SAKO*/ wire _SAKO = or (_VRM_TRI_D4.q(), _RAHU_CPU_TO_VRMn);
    /*p25.SEJY*/ wire _SEJY = or (_VRM_TRI_D5.q(), _RAHU_CPU_TO_VRMn);
    /*p25.SEDO*/ wire _SEDO = or (_VRM_TRI_D6.q(), _RAHU_CPU_TO_VRMn);
    /*p25.SAWU*/ wire _SAWU = or (_VRM_TRI_D7.q(), _RAHU_CPU_TO_VRMn);

    /*p25.RURA*/ wire _RURA = not(_SYNU);
    /*p25.RULY*/ wire _RULY = not(_SYMA);
    /*p25.RARE*/ wire _RARE = not(_ROKO);
    /*p25.RODU*/ wire _RODU = not(_SYBU);
    /*p25.RUBE*/ wire _RUBE = not(_SAKO);
    /*p25.RUMU*/ wire _RUMU = not(_SEJY);
    /*p25.RYTY*/ wire _RYTY = not(_SEDO);
    /*p25.RADY*/ wire _RADY = not(_SAWU);

    _VRAM_PIN_MD0_D.set(_RURA);
    _VRAM_PIN_MD1_D.set(_RULY);
    _VRAM_PIN_MD2_D.set(_RARE);
    _VRAM_PIN_MD3_D.set(_RODU);
    _VRAM_PIN_MD4_D.set(_RUBE);
    _VRAM_PIN_MD5_D.set(_RUMU);
    _VRAM_PIN_MD6_D.set(_RYTY);
    _VRAM_PIN_MD7_D.set(_RADY);
  }

  //----------------------------------------
  // VRAM data in

  // VRAM pin -> VRAM tri
  {
    /*p25.RODY*/ _VRM_TRI_D0.set_tribuf_6p(_RENA_VRM_TO_CPUp, _VRAM_PIN_MD0_C);
    /*p25.REBA*/ _VRM_TRI_D1.set_tribuf_6p(_RENA_VRM_TO_CPUp, _VRAM_PIN_MD1_C);
    /*p25.RYDO*/ _VRM_TRI_D2.set_tribuf_6p(_RENA_VRM_TO_CPUp, _VRAM_PIN_MD2_C);
    /*p25.REMO*/ _VRM_TRI_D3.set_tribuf_6p(_RENA_VRM_TO_CPUp, _VRAM_PIN_MD3_C);
    /*p25.ROCE*/ _VRM_TRI_D4.set_tribuf_6p(_RENA_VRM_TO_CPUp, _VRAM_PIN_MD4_C);
    /*p25.ROPU*/ _VRM_TRI_D5.set_tribuf_6p(_RENA_VRM_TO_CPUp, _VRAM_PIN_MD5_C);
    /*p25.RETA*/ _VRM_TRI_D6.set_tribuf_6p(_RENA_VRM_TO_CPUp, _VRAM_PIN_MD6_C);
    /*p25.RAKU*/ _VRM_TRI_D7.set_tribuf_6p(_RENA_VRM_TO_CPUp, _VRAM_PIN_MD7_C);
  }

  // VRAM tri -> int bus
  {
    /*p25.TYVY*/ wire _TYVY_VRM_TO_CPUn = nand(_SERE_CPU_VRM_RDp, top.LEKO_CPU_RDp());
    /*p25.SEBY*/ wire _SEBY_VRM_TO_CPUp = not(_TYVY_VRM_TO_CPUn);

    /*p25.RERY*/ wire _RERY_VBUS_D0 = not(_VRM_TRI_D0.q());
    /*p25.RUNA*/ wire _RUNA_VBUS_D1 = not(_VRM_TRI_D1.q());
    /*p25.RONA*/ wire _RONA_VBUS_D2 = not(_VRM_TRI_D2.q());
    /*p25.RUNO*/ wire _RUNO_VBUS_D3 = not(_VRM_TRI_D3.q());
    /*p25.SANA*/ wire _SANA_VBUS_D4 = not(_VRM_TRI_D4.q());
    /*p25.RORO*/ wire _RORO_VBUS_D5 = not(_VRM_TRI_D5.q());
    /*p25.RABO*/ wire _RABO_VBUS_D6 = not(_VRM_TRI_D6.q());
    /*p25.SAME*/ wire _SAME_VBUS_D7 = not(_VRM_TRI_D7.q());

    /*p25.RUGA*/ top.cpu_bus.CPU_TRI_D0.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RERY_VBUS_D0);
    /*p25.ROTA*/ top.cpu_bus.CPU_TRI_D1.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RUNA_VBUS_D1);
    /*p25.RYBU*/ top.cpu_bus.CPU_TRI_D2.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RONA_VBUS_D2);
    /*p25.RAJU*/ top.cpu_bus.CPU_TRI_D3.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RUNO_VBUS_D3);
    /*p25.TYJA*/ top.cpu_bus.CPU_TRI_D4.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _SANA_VBUS_D4);
    /*p25.REXU*/ top.cpu_bus.CPU_TRI_D5.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RORO_VBUS_D5);
    /*p25.RUPY*/ top.cpu_bus.CPU_TRI_D6.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _RABO_VBUS_D6);
    /*p25.TOKU*/ top.cpu_bus.CPU_TRI_D7.set_tribuf_6p(_SEBY_VRM_TO_CPUp, _SAME_VBUS_D7);
  }

  // VRAM tri -> tile pix temp
  {
    /*p32.METE*/ wire _METE_LATCH_TILE_DAn = not(top.tile_fetcher.NYDY_LATCH_TILE_DAp());
    /*p32.LOMA*/ wire _LOMA_LATCH_TILE_DAp = not(_METE_LATCH_TILE_DAn);
    /*p32.LEGU*/ top.LEGU_TILE_DA0 = ff8(_LOMA_LATCH_TILE_DAp, !_LOMA_LATCH_TILE_DAp, _VRM_TRI_D0.q());
    /*p32.NUDU*/ top.NUDU_TILE_DA1 = ff8(_LOMA_LATCH_TILE_DAp, !_LOMA_LATCH_TILE_DAp, _VRM_TRI_D1.q());
    /*p32.MUKU*/ top.MUKU_TILE_DA2 = ff8(_LOMA_LATCH_TILE_DAp, !_LOMA_LATCH_TILE_DAp, _VRM_TRI_D2.q());
    /*p32.LUZO*/ top.LUZO_TILE_DA3 = ff8(_LOMA_LATCH_TILE_DAp, !_LOMA_LATCH_TILE_DAp, _VRM_TRI_D3.q());
    /*p32.MEGU*/ top.MEGU_TILE_DA4 = ff8(_LOMA_LATCH_TILE_DAp, !_LOMA_LATCH_TILE_DAp, _VRM_TRI_D4.q());
    /*p32.MYJY*/ top.MYJY_TILE_DA5 = ff8(_LOMA_LATCH_TILE_DAp, !_LOMA_LATCH_TILE_DAp, _VRM_TRI_D5.q());
    /*p32.NASA*/ top.NASA_TILE_DA6 = ff8(_LOMA_LATCH_TILE_DAp, !_LOMA_LATCH_TILE_DAp, _VRM_TRI_D6.q());
    /*p32.NEFO*/ top.NEFO_TILE_DA7 = ff8(_LOMA_LATCH_TILE_DAp, !_LOMA_LATCH_TILE_DAp, _VRM_TRI_D7.q());

    // Why are the clocks swapped?
    /*p32.LESO*/ wire _LESO_LATCH_TILE_DBp = not(top.tile_fetcher.MOFU_LATCH_TILE_DBn());
    /*p??.LUVE*/ wire _LUVE_MATCH_TILE_DBn = not(_LESO_LATCH_TILE_DBp); // Schematic wrong, was labeled AJAR
    /*p32.LABU*/ wire _LABU_LATCH_TILE_DBp = not(_LUVE_MATCH_TILE_DBn);
    /*p32.RAWU*/ top.RAWU_TILE_DB0 = ff11(_LABU_LATCH_TILE_DBp, _LUVE_MATCH_TILE_DBn, 0, _VRM_TRI_D0.q());
    /*p32.POZO*/ top.POZO_TILE_DB1 = ff11(_LABU_LATCH_TILE_DBp, _LUVE_MATCH_TILE_DBn, 0, _VRM_TRI_D1.q());
    /*p32.PYZO*/ top.PYZO_TILE_DB2 = ff11(_LABU_LATCH_TILE_DBp, _LUVE_MATCH_TILE_DBn, 0, _VRM_TRI_D2.q());
    /*p32.POXA*/ top.POXA_TILE_DB3 = ff11(_LABU_LATCH_TILE_DBp, _LUVE_MATCH_TILE_DBn, 0, _VRM_TRI_D3.q());
    /*p32.PULO*/ top.PULO_TILE_DB4 = ff11(_LABU_LATCH_TILE_DBp, _LUVE_MATCH_TILE_DBn, 0, _VRM_TRI_D4.q());
    /*p32.POJU*/ top.POJU_TILE_DB5 = ff11(_LABU_LATCH_TILE_DBp, _LUVE_MATCH_TILE_DBn, 0, _VRM_TRI_D5.q());
    /*p32.POWY*/ top.POWY_TILE_DB6 = ff11(_LABU_LATCH_TILE_DBp, _LUVE_MATCH_TILE_DBn, 0, _VRM_TRI_D6.q());
    /*p32.PYJU*/ top.PYJU_TILE_DB7 = ff11(_LABU_LATCH_TILE_DBp, _LUVE_MATCH_TILE_DBn, 0, _VRM_TRI_D7.q());
  }

  // VRAM tri -> sprite pix temp + x flip
  {
    /*p29.XONO*/ wire _XONO_FLIP_X = and(top.BAXO_OAM_DA5.q(), top.sprite_fetcher.TEXY_SPR_READ_VRAMp());
    /*p33.POBE*/ wire _POBE_FLIP0 = mux2_p(_VRM_TRI_D7.q(), _VRM_TRI_D0.q(), _XONO_FLIP_X);
    /*p33.PACY*/ wire _PACY_FLIP1 = mux2_p(_VRM_TRI_D6.q(), _VRM_TRI_D1.q(), _XONO_FLIP_X);
    /*p33.PONO*/ wire _PONO_FLIP2 = mux2_p(_VRM_TRI_D5.q(), _VRM_TRI_D2.q(), _XONO_FLIP_X);
    /*p33.PUGU*/ wire _PUGU_FLIP3 = mux2_p(_VRM_TRI_D4.q(), _VRM_TRI_D3.q(), _XONO_FLIP_X);
    /*p33.PUTE*/ wire _PUTE_FLIP4 = mux2_p(_VRM_TRI_D3.q(), _VRM_TRI_D4.q(), _XONO_FLIP_X);
    /*p33.PULY*/ wire _PULY_FLIP5 = mux2_p(_VRM_TRI_D2.q(), _VRM_TRI_D5.q(), _XONO_FLIP_X);
    /*p33.PELO*/ wire _PELO_FLIP6 = mux2_p(_VRM_TRI_D1.q(), _VRM_TRI_D6.q(), _XONO_FLIP_X);
    /*p33.PAWE*/ wire _PAWE_FLIP7 = mux2_p(_VRM_TRI_D0.q(), _VRM_TRI_D7.q(), _XONO_FLIP_X);

    /*p29.PEBY*/ wire _PEBY_CLKp = not(top.sprite_fetcher.RACA_LATCH_SPPIXB());
    /*p29.NYBE*/ wire _NYBE_CLKn = not(_PEBY_CLKp);
    /*p29.PUCO*/ wire _PUCO_CLKp = not(_NYBE_CLKn);

    /*p33.PEFO*/ top.PEFO_SPRITE_DA0 = ff8(_PUCO_CLKp, !_PUCO_CLKp, _POBE_FLIP0);
    /*p33.ROKA*/ top.ROKA_SPRITE_DA1 = ff8(_PUCO_CLKp, !_PUCO_CLKp, _PACY_FLIP1);
    /*p33.MYTU*/ top.MYTU_SPRITE_DA2 = ff8(_PUCO_CLKp, !_PUCO_CLKp, _PONO_FLIP2);
    /*p33.RAMU*/ top.RAMU_SPRITE_DA3 = ff8(_PUCO_CLKp, !_PUCO_CLKp, _PUGU_FLIP3);
    /*p33.SELE*/ top.SELE_SPRITE_DA4 = ff8(_PUCO_CLKp, !_PUCO_CLKp, _PUTE_FLIP4);
    /*p33.SUTO*/ top.SUTO_SPRITE_DA5 = ff8(_PUCO_CLKp, !_PUCO_CLKp, _PULY_FLIP5);
    /*p33.RAMA*/ top.RAMA_SPRITE_DA6 = ff8(_PUCO_CLKp, !_PUCO_CLKp, _PELO_FLIP6);
    /*p33.RYDU*/ top.RYDU_SPRITE_DA7 = ff8(_PUCO_CLKp, !_PUCO_CLKp, _PAWE_FLIP7);

    /*p29.VYWA*/ wire _VYWA_CLKp = not(top.sprite_fetcher.TOPU_LATCH_SPPIXA());
    /*p29.WENY*/ wire _WENY_CLKn = not(_VYWA_CLKp);
    /*p29.XADO*/ wire _XADO_CLKp = not(_WENY_CLKn);

    /*p33.REWO*/ top.REWO_SPRITE_DB0 = ff8(_XADO_CLKp, !_XADO_CLKp, _POBE_FLIP0);
    /*p33.PEBA*/ top.PEBA_SPRITE_DB1 = ff8(_XADO_CLKp, !_XADO_CLKp, _PACY_FLIP1);
    /*p33.MOFO*/ top.MOFO_SPRITE_DB2 = ff8(_XADO_CLKp, !_XADO_CLKp, _PONO_FLIP2);
    /*p33.PUDU*/ top.PUDU_SPRITE_DB3 = ff8(_XADO_CLKp, !_XADO_CLKp, _PUGU_FLIP3);
    /*p33.SAJA*/ top.SAJA_SPRITE_DB4 = ff8(_XADO_CLKp, !_XADO_CLKp, _PUTE_FLIP4);
    /*p33.SUNY*/ top.SUNY_SPRITE_DB5 = ff8(_XADO_CLKp, !_XADO_CLKp, _PULY_FLIP5);
    /*p33.SEMO*/ top.SEMO_SPRITE_DB6 = ff8(_XADO_CLKp, !_XADO_CLKp, _PELO_FLIP6);
    /*p33.SEGA*/ top.SEGA_SPRITE_DB7 = ff8(_XADO_CLKp, !_XADO_CLKp, _PAWE_FLIP7);
  }
}

//------------------------------------------------------------------------------

SignalHash VramBus::commit() {
  SignalHash hash;

  hash << _VRM_TRI_A00.commit();
  hash << _VRM_TRI_A01.commit();
  hash << _VRM_TRI_A02.commit();
  hash << _VRM_TRI_A03.commit();
  hash << _VRM_TRI_A04.commit();
  hash << _VRM_TRI_A05.commit();
  hash << _VRM_TRI_A06.commit();
  hash << _VRM_TRI_A07.commit();
  hash << _VRM_TRI_A08.commit();
  hash << _VRM_TRI_A09.commit();
  hash << _VRM_TRI_A10.commit();
  hash << _VRM_TRI_A11.commit();
  hash << _VRM_TRI_A12.commit();

  hash << _VRM_TRI_D0.commit();
  hash << _VRM_TRI_D1.commit();
  hash << _VRM_TRI_D2.commit();
  hash << _VRM_TRI_D3.commit();
  hash << _VRM_TRI_D4.commit();
  hash << _VRM_TRI_D5.commit();
  hash << _VRM_TRI_D6.commit();
  hash << _VRM_TRI_D7.commit();

  hash << _VRAM_PIN_MCSn_A.commit();   // PIN_43 <- SOKY
  hash << _VRAM_PIN_MCSn_C.commit_input();   // PIN_43 -> TEFY
  hash << _VRAM_PIN_MCSn_D.commit();   // PIN_43 <- SETY

  hash << _VRAM_PIN_MOEn_A.commit();   // PIN_45 <- REFO
  hash << _VRAM_PIN_MOEn_C.commit_input();   // PIN_45 -> TAVY
  hash << _VRAM_PIN_MOEn_D.commit();   // PIN_45 <- SAHA

  hash << _VRAM_PIN_MWRn_A.commit();   // PIN_49 <- SYSY
  hash << _VRAM_PIN_MWRn_C.commit_input();   // PIN_49 -> SUDOs
  hash << _VRAM_PIN_MWRn_D.commit();   // PIN_49 <- RAGU

  hash << _VRAM_PIN_MA00_AD.commit();  // PIN_34 <- ECAL
  hash << _VRAM_PIN_MA01_AD.commit();  // PIN_35 <- EGEZ
  hash << _VRAM_PIN_MA02_AD.commit();  // PIN_36 <- FUHE
  hash << _VRAM_PIN_MA03_AD.commit();  // PIN_37 <- FYZY
  hash << _VRAM_PIN_MA04_AD.commit();  // PIN_38 <- DAMU
  hash << _VRAM_PIN_MA05_AD.commit();  // PIN_39 <- DAVA
  hash << _VRAM_PIN_MA06_AD.commit();  // PIN_40 <- ETEG
  hash << _VRAM_PIN_MA07_AD.commit();  // PIN_41 <- EREW
  hash << _VRAM_PIN_MA08_AD.commit();  // PIN_48 <- EVAX
  hash << _VRAM_PIN_MA09_AD.commit();  // PIN_47 <- DUVE
  hash << _VRAM_PIN_MA10_AD.commit();  // PIN_44 <- ERAF
  hash << _VRAM_PIN_MA11_AD.commit();  // PIN_46 <- FUSY
  hash << _VRAM_PIN_MA12_AD.commit();  // PIN_42 <- EXYF

  hash << _VRAM_PIN_MD0_A.commit();    // PIN_33 <- REGE
  hash << _VRAM_PIN_MD0_B.commit();    // PIN_33 <- ROFA
  hash << _VRAM_PIN_MD0_D.commit();    // PIN_33 <- RURA
  hash << _VRAM_PIN_MD1_A.commit();    // PIN_31 <- RYKY
  hash << _VRAM_PIN_MD1_B.commit();    // PIN_31 <- ROFA
  hash << _VRAM_PIN_MD1_D.commit();    // PIN_31 <- RULY
  hash << _VRAM_PIN_MD2_A.commit();    // PIN_30 <- RAZO
  hash << _VRAM_PIN_MD2_B.commit();    // PIN_30 <- ROFA
  hash << _VRAM_PIN_MD2_D.commit();    // PIN_30 <- RARE
  hash << _VRAM_PIN_MD3_A.commit();    // PIN_29 <- RADA
  hash << _VRAM_PIN_MD3_B.commit();    // PIN_29 <- ROFA
  hash << _VRAM_PIN_MD3_D.commit();    // PIN_29 <- RODU
  hash << _VRAM_PIN_MD4_A.commit();    // PIN_28 <- RYRO
  hash << _VRAM_PIN_MD4_B.commit();    // PIN_28 <- ROFA
  hash << _VRAM_PIN_MD4_D.commit();    // PIN_28 <- RUBE
  hash << _VRAM_PIN_MD5_A.commit();    // PIN_27 <- REVU
  hash << _VRAM_PIN_MD5_B.commit();    // PIN_27 <- ROFA
  hash << _VRAM_PIN_MD5_D.commit();    // PIN_27 <- RUMU
  hash << _VRAM_PIN_MD6_A.commit();    // PIN_26 <- REKU
  hash << _VRAM_PIN_MD6_B.commit();    // PIN_26 <- ROFA
  hash << _VRAM_PIN_MD6_D.commit();    // PIN_26 <- RYTY
  hash << _VRAM_PIN_MD7_A.commit();    // PIN_25 <- RYZE
  hash << _VRAM_PIN_MD7_B.commit();    // PIN_25 <- ROFA
  hash << _VRAM_PIN_MD7_D.commit();    // PIN_25 <- RADY

  hash << _VRAM_PIN_MD0_C.commit_input();    // PIN_33 -> RODY
  hash << _VRAM_PIN_MD1_C.commit_input();    // PIN_31 -> REBA
  hash << _VRAM_PIN_MD2_C.commit_input();    // PIN_30 -> RYDO
  hash << _VRAM_PIN_MD3_C.commit_input();    // PIN_29 -> REMO
  hash << _VRAM_PIN_MD4_C.commit_input();    // PIN_28 -> ROCE
  hash << _VRAM_PIN_MD5_C.commit_input();    // PIN_27 -> ROPU
  hash << _VRAM_PIN_MD6_C.commit_input();    // PIN_26 -> RETA
  hash << _VRAM_PIN_MD7_C.commit_input();    // PIN_25 -> RAKU


  return hash;
}


//------------------------------------------------------------------------------
