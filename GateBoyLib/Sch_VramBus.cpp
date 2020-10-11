#include "GateBoyLib/Sch_VramBus.h"

#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

#define DUMP(A) do { d("%-20s : %d\n", #A, wire(A)); } while(0);

//------------------------------------------------------------------------------

void VramBus::dump(Dumper& d, const SchematicTop& /*top*/) const {
  d("\002===== VRAM Bus =====\001\n");

  d("VRAM BUS ADDR : %04x %c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    get_bus_addr() | 0x8000,
    BUS_VRAM_A12n.c(), BUS_VRAM_A11n.c(), BUS_VRAM_A10n.c(), BUS_VRAM_A09n.c(),
    BUS_VRAM_A08n.c(), BUS_VRAM_A07n.c(), BUS_VRAM_A06n.c(), BUS_VRAM_A05n.c(),
    BUS_VRAM_A04n.c(), BUS_VRAM_A03n.c(), BUS_VRAM_A02n.c(), BUS_VRAM_A01n.c(),
    BUS_VRAM_A00n.c());
  d("VRAM BUS DATA : %c%c%c%c%c%c%c%c\n",
    BUS_VRAM_D7p.c(), BUS_VRAM_D6p.c(), BUS_VRAM_D5p.c(), BUS_VRAM_D4p.c(),
    BUS_VRAM_D3p.c(), BUS_VRAM_D2p.c(), BUS_VRAM_D1p.c(), BUS_VRAM_D0p.c());
  d("\n");

  d("VRAM PIN MCSn : %c\n", PIN_VRAM_CSn.c());
  d("VRAM PIN MOEn : %c\n", PIN_VRAM_OEn.c());
  d("VRAM PIN MWRn : %c\n", PIN_VRAM_WRn.c());
  d("VRAM PIN ADDR : 0x%04x\n", get_pin_addr() | 0x8000);
  d("VRAM PIN DATA : %02x %c%c%c%c%c%c%c%c\n",
    get_bus_data(),
    PIN_VRAM_D07p.c(), PIN_VRAM_D06p.c(), PIN_VRAM_D05p.c(), PIN_VRAM_D04p.c(),
    PIN_VRAM_D03p.c(), PIN_VRAM_D02p.c(), PIN_VRAM_D01p.c(), PIN_VRAM_D00p.c());
  d("\n");

  int TILE_DA = pack_p(LEGU_TILE_DA0n.q07n(), NUDU_TILE_DA1n.q07n(), MUKU_TILE_DA2n.q07n(), LUZO_TILE_DA3n.q07n(),
                       MEGU_TILE_DA4n.q07n(), MYJY_TILE_DA5n.q07n(), NASA_TILE_DA6n.q07n(), NEFO_TILE_DA7n.q07n());
  int TILE_DB = pack_p(RAWU_TILE_DB0p.q11p(), POZO_TILE_DB1p.q11p(), PYZO_TILE_DB2p.q11p(), POXA_TILE_DB3p.q11p(),
                       PULO_TILE_DB4p.q11p(), POJU_TILE_DB5p.q11p(), POWY_TILE_DB6p.q11p(), PYJU_TILE_DB7p.q11p());
  int SPRITE_DA = pack_p(PEFO_SPRITE_DB0n.q07n(), ROKA_SPRITE_DB1n.q07n(), MYTU_SPRITE_DB2n.q07n(), RAMU_SPRITE_DB3n.q07n(),
                         SELE_SPRITE_DB4n.q07n(), SUTO_SPRITE_DB5n.q07n(), RAMA_SPRITE_DB6n.q07n(), RYDU_SPRITE_DB7n.q07n());
  int SPRITE_DB = pack_p(REWO_SPRITE_DA0n.q07n(), PEBA_SPRITE_DA1n.q07n(), MOFO_SPRITE_DA2n.q07n(), PUDU_SPRITE_DA3n.q07n(),
                         SAJA_SPRITE_DA4n.q07n(), SUNY_SPRITE_DA5n.q07n(), SEMO_SPRITE_DA6n.q07n(), SEGA_SPRITE_DA7n.q07n());

  d("TILE_DA       : 0x%02x\n", TILE_DA);
  d("TILE_DB       : 0x%02x\n", TILE_DB);
  d("SPRITE_DA     : 0x%02x\n", SPRITE_DA);
  d("SPRITE_DB     : 0x%02x\n", SPRITE_DB);
  d("\n");
}

//------------------------------------------------------------------------------

void VramBus::tock(SchematicTop& top) {
  wire VYPO = 1;

  /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.PIN_CPU_ADDR_EXTp.qp(), top.cpu_bus.TEVY_8000_9FFFn());
  /*p25.TEFA*/ wire TEFA_8000_9FFFp = nor2(top.cpu_bus.SYRO_FE00_FFFFp(), TEXO_8000_9FFFn);
  /*p25.SOSE*/ wire SOSE_8000_9FFFp = and2(top.cpu_bus.BUS_CPU_A15.qp(), TEFA_8000_9FFFp);

  /*p25.ROPY*/ wire ROPY_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGp.qp());

  //----------------------------------------
  // VRAM pins

  {
    /*p25.TUJA*/ wire TUJA_CPU_VRAM_WRp = and2(SOSE_8000_9FFFp, top.APOV_CPU_WRp_xxxxEFGx);
    /*p25.SUDO*/ wire SUDO_MWRp_C = not1(PIN_VRAM_WRn.qn());
    /*p25.TYJY*/ wire TYJY_DBG_VRAM_WRp = mux2p(top.TUTO_DBG_VRAMp, SUDO_MWRp_C, TUJA_CPU_VRAM_WRp);

    /*p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn);
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(PIN_VRAM_CSn.qn());
    /*p25.TOLE*/ wire TOLE_VRAM_RDp     = mux2p(top.TUTO_DBG_VRAMp, TEFY_VRAM_MCSp, TUCA_CPU_VRAM_RDp);
    /*p25.SERE*/ wire SERE_CPU_VRM_RDp = and2(TOLE_VRAM_RDp, ROPY_RENDERINGn);

    /*p25.SOHY*/ wire SOHY_MWRn = nand2(TYJY_DBG_VRAM_WRp, SERE_CPU_VRM_RDp);
    /*p25.RACO*/ wire RACO_DBG_VRAMn = not1(top.TUTO_DBG_VRAMp);
    /*p25.TAXY*/ wire TAXY_MWRn_A = and2(SOHY_MWRn, RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire SOFY_MWRn_D = or2(SOHY_MWRn, top.TUTO_DBG_VRAMp);
    /*p25.SYSY*/ wire SYSY_MWRp_A = not1(TAXY_MWRn_A);
    /*p25.RAGU*/ wire RAGU_MWRp_D = not1(SOFY_MWRn_D);
    PIN_VRAM_WRn.io_pin(SYSY_MWRp_A, RAGU_MWRp_D);
  }

  {
    /*p25.XANE*/ wire XANE_VRAM_LOCKn = nor2(top.dma_reg.LUFA_DMA_READ_VRAMp, top.pix_pipe.XYMU_RENDERINGp.qp()); // def nor4
    /*p25.TAVY*/ wire TAVY_MOEp = not1(PIN_VRAM_OEn.qn());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.qp());  // Schematic wrong, second input is PIN_CPU_WRp
    /*p25.SALE*/ wire SALE_VRAM_WRn = mux2p(top.TUTO_DBG_VRAMp, TAVY_MOEp, TEGU_CPU_VRAM_WRn);
    /*p25.RYLU*/ wire RYLU_CPU_VRAM_RDn = nand2(SALE_VRAM_WRn, XANE_VRAM_LOCKn);

    /*p25.RAWA*/ wire RAWA_SPR_VRAM_RDn = not1(top.sprite_fetcher.SOHO_SPR_VRAM_RDp);
    /*p25.APAM*/ wire APAM_DMA_VRAM_RDn = not1(top.dma_reg.LUFA_DMA_READ_VRAMp);

    /*p27.MYMA*/ wire MYMA_BGW_VRAM_RDn = not1(top.tile_fetcher.LONY_BG_FETCH_RUNNINGp.qp());


    /*p25.RACU*/ wire RACU_MOEn = and4(RYLU_CPU_VRAM_RDn, RAWA_SPR_VRAM_RDn, MYMA_BGW_VRAM_RDn, APAM_DMA_VRAM_RDn); // def and

    /*p25.RACO*/ wire RACO_DBG_VRAMn = not1(top.TUTO_DBG_VRAMp);
    /*p25.SEMA*/ wire SEMA_MOEn_A = and2(RACU_MOEn, RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire RUTE_MOEn_D = or2(RACU_MOEn, top.TUTO_DBG_VRAMp); // schematic wrong, second input is RACU
    /*p25.REFO*/ wire REFO_MOEn_A = not1(SEMA_MOEn_A);
    /*p25.SAHA*/ wire SAHA_MOEn_D = not1(RUTE_MOEn_D);
    PIN_VRAM_OEn.io_pin(REFO_MOEn_A, SAHA_MOEn_D);
  }

  {
    /*p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn);
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(PIN_VRAM_CSn.qn());
    /*p25.TOLE*/ wire TOLE_VRAM_RDp     = mux2p(top.TUTO_DBG_VRAMp, TEFY_VRAM_MCSp, TUCA_CPU_VRAM_RDp);
    /*p25.SERE*/ wire SERE_CPU_VRM_RDp = and2(TOLE_VRAM_RDp, ROPY_RENDERINGn);

    // SUTU := nor4(LENA, LUFA, TEXY, SERE)
    /*p25.SUTU*/ wire SUTU_MCSn = nor4(top.tile_fetcher.LENA_BGW_VRM_RDp,
                                        top.dma_reg.LUFA_DMA_READ_VRAMp,
                                        top.sprite_fetcher.TEXY_SPR_READ_VRAMp,
                                        SERE_CPU_VRM_RDp);

    /*p25.RACO*/ wire RACO_DBG_VRAMn = not1(top.TUTO_DBG_VRAMp);
    /*p25.TODE*/ wire TODE_MCSn_A = and2(SUTU_MCSn, RACO_DBG_VRAMn);
    /*p25.SEWO*/ wire SEWO_MCSn_D = or2(SUTU_MCSn, top.TUTO_DBG_VRAMp);
    /*p25.SOKY*/ wire SOKY_MCSp_A = not1(TODE_MCSn_A);
    /*p25.SETY*/ wire SETY_MCSp_D = not1(SEWO_MCSn_D);
    PIN_VRAM_CSn.io_pin(SOKY_MCSp_A, SETY_MCSp_D);
  }

  //----------------------------------------
  // VRAM address

  // CPU address -> vram address
  {
    /* p25.XANE*/ wire XANE_VRAM_LOCKn = nor2(top.dma_reg.LUFA_DMA_READ_VRAMp, top.pix_pipe.XYMU_RENDERINGp.qp());
    /* p25.XEDU*/ wire XEDU_CPU_VRAM_RDn = not1(XANE_VRAM_LOCKn);
    /* p25.XAKY*/ BUS_VRAM_A00n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A00.qp());
    /* p25.XUXU*/ BUS_VRAM_A01n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A01.qp());
    /* p25.XYNE*/ BUS_VRAM_A02n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A02.qp());
    /* p25.XODY*/ BUS_VRAM_A03n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A03.qp());
    /* p25.XECA*/ BUS_VRAM_A04n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A04.qp());
    /* p25.XOBA*/ BUS_VRAM_A05n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A05.qp());
    /* p25.XOPO*/ BUS_VRAM_A06n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A06.qp());
    /* p25.XYBO*/ BUS_VRAM_A07n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A07.qp());
    /* p25.RYSU*/ BUS_VRAM_A08n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A08.qp());
    /* p25.RESE*/ BUS_VRAM_A09n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A09.qp());
    /* p25.RUSE*/ BUS_VRAM_A10n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A10.qp());
    /* p25.RYNA*/ BUS_VRAM_A11n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A11.qp());
    /* p25.RUMO*/ BUS_VRAM_A12n.tri_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A12.qp());
  }

  // DMA address -> vram address
  {
    /* p04.AHOC*/ wire AHOC_DMA_VRAM_RDn = not1(top.dma_reg.LUFA_DMA_READ_VRAMp);
    /* p04.ECAL*/ BUS_VRAM_A00n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp());
    /* p04.EGEZ*/ BUS_VRAM_A01n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp());
    /* p04.FUHE*/ BUS_VRAM_A02n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp());
    /* p04.FYZY*/ BUS_VRAM_A03n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp());
    /* p04.DAMU*/ BUS_VRAM_A04n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp());
    /* p04.DAVA*/ BUS_VRAM_A05n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp());
    /* p04.ETEG*/ BUS_VRAM_A06n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp());
    /*#p04.EREW*/ BUS_VRAM_A07n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp());
    /*#p04.EVAX*/ BUS_VRAM_A08n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NAFA_DMA_A08n.q07n());
    /* p04.DUVE*/ BUS_VRAM_A09n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PYNE_DMA_A09n.q07n());
    /* p04.ERAF*/ BUS_VRAM_A10n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PARA_DMA_A10n.q07n());
    /* p04.FUSY*/ BUS_VRAM_A11n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NYDO_DMA_A11n.q07n());
    /* p04.EXYF*/ BUS_VRAM_A12n.tri_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NYGY_DMA_A12n.q07n());
  }

  // Sprite fetcher read
  {
    /*#p29.WUKY*/ wire WUKY_FLIP_Yp = not1(top.sprite_scanner.YZOS_OAM_DB6p.q08p());

    /*#p29.CYVU*/ wire CYVU_L0 = xor2(WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L0.qp());
    /*#p29.BORE*/ wire BORE_L1 = xor2(WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L1.qp());
    /*#p29.BUVY*/ wire BUVY_L2 = xor2(WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L2.qp());
    /*#p29.WAGO*/ wire WAGO_L3 = xor2(WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L3.qp());

    /*#p29.FUFO*/ wire FUFO_LCDC_SPSIZEn = not1(top.pix_pipe.XYMO_LCDC_SPSIZEn.q08());
    /*#p29.GEJY*/ wire GEJY_L3 = amux2(top.sprite_scanner.XUSO_OAM_DA0p.q08p(), FUFO_LCDC_SPSIZEn,
                                       top.pix_pipe.XYMO_LCDC_SPSIZEn.q08(), WAGO_L3);

    /* p29.ABEM*/ BUS_VRAM_A00n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_fetcher.XUQU_SPRITE_AB);
    /* p29.BAXE*/ BUS_VRAM_A01n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, CYVU_L0);
    /* p29.ARAS*/ BUS_VRAM_A02n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, BORE_L1);
    /* p29.AGAG*/ BUS_VRAM_A03n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, BUVY_L2);
    /* p29.FAMU*/ BUS_VRAM_A04n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, GEJY_L3);
    /*#p29.FUGY*/ BUS_VRAM_A05n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_scanner.XEGU_OAM_DA1p.q08p());
    /* p29.GAVO*/ BUS_VRAM_A06n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_scanner.YJEX_OAM_DA2p.q08p());
    /* p29.WYGA*/ BUS_VRAM_A07n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_scanner.XYJU_OAM_DA3p.q08p());
    /* p29.WUNE*/ BUS_VRAM_A08n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_scanner.YBOG_OAM_DA4p.q08p());
    /* p29.GOTU*/ BUS_VRAM_A09n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_scanner.WYSO_OAM_DA5p.q08p());
    /* p29.GEGU*/ BUS_VRAM_A10n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_scanner.XOTE_OAM_DA6p.q08p());
    /* p29.XEHE*/ BUS_VRAM_A11n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_scanner.YZAB_OAM_DA7p.q08p());
    /* p29.DYSO*/ BUS_VRAM_A12n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, 0);   // sprites always in low half of tile store
  }

  {
    // Map scroll adder
#pragma warning(push)
#pragma warning(disable:4189)
    /*#p26.FAFO*/ wire FAFO_TILE_Y0S = add_s(top.lcd_reg.MUWY_Y0p.qp(), top.pix_pipe.GAVE_SCY0n.q08(), 0);
    /*#p26.FAFO*/ wire FAFO_TILE_Y0C = add_c(top.lcd_reg.MUWY_Y0p.qp(), top.pix_pipe.GAVE_SCY0n.q08(), 0);
    /* p26.EMUX*/ wire EMUX_TILE_Y1S = add_s(top.lcd_reg.MYRO_Y1p.qp(), top.pix_pipe.FYMO_SCY1n.q08(), FAFO_TILE_Y0C);
    /* p26.EMUX*/ wire EMUX_TILE_Y1C = add_c(top.lcd_reg.MYRO_Y1p.qp(), top.pix_pipe.FYMO_SCY1n.q08(), FAFO_TILE_Y0C);
    /* p26.ECAB*/ wire ECAB_TILE_Y2S = add_s(top.lcd_reg.LEXA_Y2p.qp(), top.pix_pipe.FEZU_SCY2n.q08(), EMUX_TILE_Y1C);
    /* p26.ECAB*/ wire ECAB_TILE_Y2C = add_c(top.lcd_reg.LEXA_Y2p.qp(), top.pix_pipe.FEZU_SCY2n.q08(), EMUX_TILE_Y1C);
    /* p26.ETAM*/ wire ETAM_MAP_Y0S  = add_s(top.lcd_reg.LYDO_Y3p.qp(), top.pix_pipe.FUJO_SCY3n.q08(), ECAB_TILE_Y2C);
    /* p26.ETAM*/ wire ETAM_MAP_Y0C  = add_c(top.lcd_reg.LYDO_Y3p.qp(), top.pix_pipe.FUJO_SCY3n.q08(), ECAB_TILE_Y2C);
    /* p26.DOTO*/ wire DOTO_MAP_Y1S  = add_s(top.lcd_reg.LOVU_Y4p.qp(), top.pix_pipe.DEDE_SCY4n.q08(), ETAM_MAP_Y0C);
    /* p26.DOTO*/ wire DOTO_MAP_Y1C  = add_c(top.lcd_reg.LOVU_Y4p.qp(), top.pix_pipe.DEDE_SCY4n.q08(), ETAM_MAP_Y0C);
    /* p26.DABA*/ wire DABA_MAP_Y2S  = add_s(top.lcd_reg.LEMA_Y5p.qp(), top.pix_pipe.FOTY_SCY5n.q08(), DOTO_MAP_Y1C);
    /* p26.DABA*/ wire DABA_MAP_Y2C  = add_c(top.lcd_reg.LEMA_Y5p.qp(), top.pix_pipe.FOTY_SCY5n.q08(), DOTO_MAP_Y1C);
    /* p26.EFYK*/ wire EFYK_MAP_Y3S  = add_s(top.lcd_reg.MATO_Y6p.qp(), top.pix_pipe.FOHA_SCY6n.q08(), DABA_MAP_Y2C);
    /* p26.EFYK*/ wire EFYK_MAP_Y3C  = add_c(top.lcd_reg.MATO_Y6p.qp(), top.pix_pipe.FOHA_SCY6n.q08(), DABA_MAP_Y2C);
    /* p26.EJOK*/ wire EJOK_MAP_Y4S  = add_s(top.lcd_reg.LAFO_Y7p.qp(), top.pix_pipe.FUNY_SCY7n.q08(), EFYK_MAP_Y3C);
    /* p26.EJOK*/ wire EJOK_MAP_Y4C  = add_c(top.lcd_reg.LAFO_Y7p.qp(), top.pix_pipe.FUNY_SCY7n.q08(), EFYK_MAP_Y3C);

    /*#p26.ATAD*/ wire ATAD_TILE_X0S = add_s(top.pix_pipe.XEHO_X0p.qp(), top.pix_pipe.DATY_SCX0n.q08(), 0);
    /*#p26.ATAD*/ wire ATAD_TILE_X0C = add_c(top.pix_pipe.XEHO_X0p.qp(), top.pix_pipe.DATY_SCX0n.q08(), 0);
    /* p26.BEHU*/ wire BEHU_TILE_X1S = add_s(top.pix_pipe.SAVY_X1p.qp(), top.pix_pipe.DUZU_SCX1n.q08(), ATAD_TILE_X0C);
    /* p26.BEHU*/ wire BEHU_TILE_X1C = add_c(top.pix_pipe.SAVY_X1p.qp(), top.pix_pipe.DUZU_SCX1n.q08(), ATAD_TILE_X0C);
    /* p26.APYH*/ wire APYH_TILE_X2S = add_s(top.pix_pipe.XODU_X2p.qp(), top.pix_pipe.CYXU_SCX2n.q08(), BEHU_TILE_X1C);
    /* p26.APYH*/ wire APYH_TILE_X2C = add_c(top.pix_pipe.XODU_X2p.qp(), top.pix_pipe.CYXU_SCX2n.q08(), BEHU_TILE_X1C);
    /* p26.BABE*/ wire BABE_MAP_X0S  = add_s(top.pix_pipe.XYDO_X3p.qp(), top.pix_pipe.GUBO_SCX3n.q08(), APYH_TILE_X2C);
    /* p26.BABE*/ wire BABE_MAP_X0C  = add_c(top.pix_pipe.XYDO_X3p.qp(), top.pix_pipe.GUBO_SCX3n.q08(), APYH_TILE_X2C);
    /* p26.ABOD*/ wire ABOD_MAP_X1S  = add_s(top.pix_pipe.TUHU_X4p.qp(), top.pix_pipe.BEMY_SCX4n.q08(), BABE_MAP_X0C);
    /* p26.ABOD*/ wire ABOD_MAP_X1C  = add_c(top.pix_pipe.TUHU_X4p.qp(), top.pix_pipe.BEMY_SCX4n.q08(), BABE_MAP_X0C);
    /* p26.BEWY*/ wire BEWY_MAP_X2S  = add_s(top.pix_pipe.TUKY_X5p.qp(), top.pix_pipe.CUZY_SCX5n.q08(), ABOD_MAP_X1C);
    /* p26.BEWY*/ wire BEWY_MAP_X2C  = add_c(top.pix_pipe.TUKY_X5p.qp(), top.pix_pipe.CUZY_SCX5n.q08(), ABOD_MAP_X1C);
    /* p26.BYCA*/ wire BYCA_MAP_X3S  = add_s(top.pix_pipe.TAKO_X6p.qp(), top.pix_pipe.CABU_SCX6n.q08(), BEWY_MAP_X2C);
    /* p26.BYCA*/ wire BYCA_MAP_X3C  = add_c(top.pix_pipe.TAKO_X6p.qp(), top.pix_pipe.CABU_SCX6n.q08(), BEWY_MAP_X2C);
    /* p26.ACUL*/ wire ACUL_MAP_X4S  = add_s(top.pix_pipe.SYBE_X7p.qp(), top.pix_pipe.BAKE_SCX7n.q08(), BYCA_MAP_X3C);
    /* p26.ACUL*/ wire ACUL_MAP_X4C  = add_c(top.pix_pipe.SYBE_X7p.qp(), top.pix_pipe.BAKE_SCX7n.q08(), BYCA_MAP_X3C);
#pragma warning(pop)

    // Background map read
    {
      /* p26.AXEP*/ BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, BABE_MAP_X0S);
      /* p26.AFEB*/ BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, ABOD_MAP_X1S);
      /* p26.ALEL*/ BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, BEWY_MAP_X2S);
      /* p26.COLY*/ BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, BYCA_MAP_X3S);
      /* p26.AJAN*/ BUS_VRAM_A04n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, ACUL_MAP_X4S);
      /* p26.DUHO*/ BUS_VRAM_A05n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, ETAM_MAP_Y0S);
      /* p26.CASE*/ BUS_VRAM_A06n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, DOTO_MAP_Y1S);
      /* p26.CYPO*/ BUS_VRAM_A07n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, DABA_MAP_Y2S);
      /* p26.CETA*/ BUS_VRAM_A08n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, EFYK_MAP_Y3S);
      /* p26.DAFE*/ BUS_VRAM_A09n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, EJOK_MAP_Y4S);
      /*#p26.AMUV*/ BUS_VRAM_A10n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, top.pix_pipe.XAFO_LCDC_BGMAPn.q08());
      /* p26.COVE*/ BUS_VRAM_A11n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, 1);
      /* p26.COXO*/ BUS_VRAM_A12n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, 1);
    }

    // Window map read
    {
      /*#p27.XEJA*/ BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WYKA_WIN_X3.qp());
      /* p27.XAMO*/ BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WODY_WIN_X4.qp());
      /* p27.XAHE*/ BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WOBO_WIN_X5.qp());
      /* p27.XULO*/ BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WYKO_WIN_X6.qp());
      /* p27.WUJU*/ BUS_VRAM_A04n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.XOLO_WIN_X7.qp());
      /*#p27.VYTO*/ BUS_VRAM_A05n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TUFU_WIN_Y3.qp());
      /* p27.VEHA*/ BUS_VRAM_A06n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TAXA_WIN_Y4.qp());
      /* p27.VACE*/ BUS_VRAM_A07n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TOZO_WIN_Y5.qp());
      /* p27.VOVO*/ BUS_VRAM_A08n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TATE_WIN_Y6.qp());
      /* p27.VULO*/ BUS_VRAM_A09n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TEKE_WIN_Y7.qp());
      /*#p27.VEVY*/ BUS_VRAM_A10n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WOKY_LCDC_WINMAPn.q08());
      /* p27.VEZA*/ BUS_VRAM_A11n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, 1);
      /* p27.VOGU*/ BUS_VRAM_A12n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, 1);
    }

    // Background/window tile read
    {
      /*#p26.ASUM*/ BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  top.tile_fetcher.XUHA_FETCH_S2p);
      /* p26.EVAD*/ BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  FAFO_TILE_Y0S);
      /* p26.DAHU*/ BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  EMUX_TILE_Y1S);
      /* p26.DODE*/ BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  ECAB_TILE_Y2S); // check outputs of ECAB

      /*#p25.XONU*/ BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.tile_fetcher.XUHA_FETCH_S2p);
      /*#p25.WUDO*/ BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VYNO_WIN_Y0.qp());
      /*#p25.WAWE*/ BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VUJO_WIN_Y1.qp());
      /*#p25.WOLU*/ BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VYMU_WIN_Y2.qp());

      /*#p25.VAPY*/ BUS_VRAM_A04n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     RAWU_TILE_DB0p.q11p());
      /*#p25.SEZU*/ BUS_VRAM_A05n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     POZO_TILE_DB1p.q11p());
      /*#p25.VEJY*/ BUS_VRAM_A06n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     PYZO_TILE_DB2p.q11p());
      /*#p25.RUSA*/ BUS_VRAM_A07n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     POXA_TILE_DB3p.q11p());
      /*#p25.ROHA*/ BUS_VRAM_A08n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     PULO_TILE_DB4p.q11p());
      /*#p25.RESO*/ BUS_VRAM_A09n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     POJU_TILE_DB5p.q11p());
      /*#p25.SUVO*/ BUS_VRAM_A10n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     POWY_TILE_DB6p.q11p());
      /*#p25.TOBO*/ BUS_VRAM_A11n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     PYJU_TILE_DB7p.q11p());

      /*#p25.VUZA*/ wire VUZA_TILE_BANKp = nor2(PYJU_TILE_DB7p.q11p(), top.pix_pipe.WEXU_LCDC_BGTILEn.q08());
      /*#p25.VURY*/ BUS_VRAM_A12n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     VUZA_TILE_BANKp);
    }
  }

  // VRAM addr bus -> VRAM addr pin
  {
    /*p25.MYFU*/ wire MYFUp = not1(BUS_VRAM_A00n.qp());
    /*p25.MASA*/ wire MASAp = not1(BUS_VRAM_A01n.qp());
    /*p25.MYRE*/ wire MYREp = not1(BUS_VRAM_A02n.qp());
    /*p25.MAVU*/ wire MAVUp = not1(BUS_VRAM_A03n.qp());
    /*p25.MEPA*/ wire MEPAp = not1(BUS_VRAM_A04n.qp());
    /*p25.MYSA*/ wire MYSAp = not1(BUS_VRAM_A05n.qp());
    /*p25.MEWY*/ wire MEWYp = not1(BUS_VRAM_A06n.qp());
    /*p25.MUME*/ wire MUMEp = not1(BUS_VRAM_A07n.qp());
    /*p25.VOVA*/ wire VOVAp = not1(BUS_VRAM_A08n.qp());
    /*p25.VODE*/ wire VODEp = not1(BUS_VRAM_A09n.qp());
    /*p25.RUKY*/ wire RUKYp = not1(BUS_VRAM_A10n.qp());
    /*p25.RUMA*/ wire RUMAp = not1(BUS_VRAM_A11n.qp());
    /*p25.REHO*/ wire REHOp = not1(BUS_VRAM_A12n.qp());

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

    PIN_VRAM_A00p.io_pin(LEXEn, LEXEn);
    PIN_VRAM_A01p.io_pin(LOZUn, LOZUn);
    PIN_VRAM_A02p.io_pin(LACAn, LACAn);
    PIN_VRAM_A03p.io_pin(LUVOn, LUVOn);
    PIN_VRAM_A04p.io_pin(LOLYn, LOLYn);
    PIN_VRAM_A05p.io_pin(LALOn, LALOn);
    PIN_VRAM_A06p.io_pin(LEFAn, LEFAn);
    PIN_VRAM_A07p.io_pin(LUBYn, LUBYn);
    PIN_VRAM_A08p.io_pin(TUJYn, TUJYn);
    PIN_VRAM_A09p.io_pin(TAGOn, TAGOn);
    PIN_VRAM_A10p.io_pin(NUVAn, NUVAn);
    PIN_VRAM_A11p.io_pin(PEDUn, PEDUn);
    PIN_VRAM_A12p.io_pin(PONYn, PONYn);
  }

  //----------------------------------------
  // VRAM data out

  // CBD->VBD->VPD
  {
    /*p25.TAVY*/ wire TAVY_MOEp = not1(PIN_VRAM_OEn.qn());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.qp());  // Schematic wrong, second input is PIN_CPU_WRp
    /*p25.SALE*/ wire SALE_CPU_VRAM_WRn = mux2p(top.TUTO_DBG_VRAMp, TAVY_MOEp, TEGU_CPU_VRAM_WRn);

    /*p25.TUCA*/ wire TUCA_CPU_VRAM_AVn = and2(SOSE_8000_9FFFp, top.ABUZ_AVn);
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(PIN_VRAM_CSn.qn());
    /*p25.TOLE*/ wire TOLE_VRAM_AVn     = mux2p(top.TUTO_DBG_VRAMp, TEFY_VRAM_MCSp, TUCA_CPU_VRAM_AVn);
    /*p25.SERE*/ wire SERE = and2(TOLE_VRAM_AVn, ROPY_RENDERINGn);
    /*p25.RUVY*/ wire RUVY_VRAM_WRp = not1(SALE_CPU_VRAM_WRn);
    /*p25.SAZO*/ wire SAZO_CBD_TO_VPDp = and2(RUVY_VRAM_WRp, SERE);
    /*p25.RYJE*/ wire RYJE_CBD_TO_VPDn = not1(SAZO_CBD_TO_VPDp);
    /*p25.REVO*/ wire REVO_CBD_TO_VPDp = not1(RYJE_CBD_TO_VPDn);
    /*p25.ROCY*/ wire ROCY_CBD_TO_VPDp = and2(REVO_CBD_TO_VPDp, SAZO_CBD_TO_VPDp);
    /*p25.RAHU*/ wire RAHU_CBD_TO_VPDn = not1(ROCY_CBD_TO_VPDp);

    /* p25.TEME*/ BUS_VRAM_D0p.tri10_np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /* p25.TEWU*/ BUS_VRAM_D1p.tri10_np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*#p25.TYGO*/ BUS_VRAM_D2p.tri10_np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /* p25.SOTE*/ BUS_VRAM_D3p.tri10_np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /* p25.SEKE*/ BUS_VRAM_D4p.tri10_np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /* p25.RUJO*/ BUS_VRAM_D5p.tri10_np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /* p25.TOFA*/ BUS_VRAM_D6p.tri10_np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /* p25.SUZA*/ BUS_VRAM_D7p.tri10_np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*#p25.ROVE*/ wire ROVE_CBD_TO_VPDp = not1(RAHU_CBD_TO_VPDn);
    /*#p25.SEFA*/ wire SEFA_D0p = and2(BUS_VRAM_D0p.qp(), ROVE_CBD_TO_VPDp);
    /* p25.SOGO*/ wire SOGO_D1p = and2(BUS_VRAM_D1p.qp(), ROVE_CBD_TO_VPDp);
    /* p25.SEFU*/ wire SEFU_D2p = and2(BUS_VRAM_D2p.qp(), ROVE_CBD_TO_VPDp);
    /* p25.SUNA*/ wire SUNA_D3p = and2(BUS_VRAM_D3p.qp(), ROVE_CBD_TO_VPDp);
    /* p25.SUMO*/ wire SUMO_D4p = and2(BUS_VRAM_D4p.qp(), ROVE_CBD_TO_VPDp);
    /* p25.SAZU*/ wire SAZU_D5p = and2(BUS_VRAM_D5p.qp(), ROVE_CBD_TO_VPDp);
    /* p25.SAMO*/ wire SAMO_D6p = and2(BUS_VRAM_D6p.qp(), ROVE_CBD_TO_VPDp);
    /* p25.SUKE*/ wire SUKE_D7p = and2(BUS_VRAM_D7p.qp(), ROVE_CBD_TO_VPDp);

    /*#p25.REGE*/ wire REGE_D0n = not1(SEFA_D0p);
    /* p25.RYKY*/ wire RYKY_D1n = not1(SOGO_D1p);
    /* p25.RAZO*/ wire RAZO_D2n = not1(SEFU_D2p);
    /* p25.RADA*/ wire RADA_D3n = not1(SUNA_D3p);
    /* p25.RYRO*/ wire RYRO_D4n = not1(SUMO_D4p);
    /* p25.REVU*/ wire REVU_D5n = not1(SAZU_D5p);
    /* p25.REKU*/ wire REKU_D6n = not1(SAMO_D6p);
    /* p25.RYZE*/ wire RYZE_D7n = not1(SUKE_D7p);

    /*#p25.SYNU*/ wire SYNU_D0p = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D0p.qp());
    /* p25.SYMA*/ wire SYMA_D1p = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D1p.qp());
    /* p25.ROKO*/ wire ROKO_D2p = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D2p.qp());
    /* p25.SYBU*/ wire SYBU_D3p = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D3p.qp());
    /* p25.SAKO*/ wire SAKO_D4p = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D4p.qp());
    /* p25.SEJY*/ wire SEJY_D5p = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D5p.qp());
    /* p25.SEDO*/ wire SEDO_D6p = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D6p.qp());
    /* p25.SAWU*/ wire SAWU_D7p = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D7p.qp());

    /*#p25.RURA*/ wire RURA_D0n = not1(SYNU_D0p);
    /* p25.RULY*/ wire RULY_D1n = not1(SYMA_D1p);
    /* p25.RARE*/ wire RARE_D2n = not1(ROKO_D2p);
    /* p25.RODU*/ wire RODU_D3n = not1(SYBU_D3p);
    /* p25.RUBE*/ wire RUBE_D4n = not1(SAKO_D4p);
    /* p25.RUMU*/ wire RUMU_D5n = not1(SEJY_D5p);
    /* p25.RYTY*/ wire RYTY_D6n = not1(SEDO_D6p);
    /* p25.RADY*/ wire RADY_D7n = not1(SAWU_D7p);

    /*#p25.RELA*/ wire RELA_CBD_TO_VPDp = or2(REVO_CBD_TO_VPDp, SAZO_CBD_TO_VPDp);
    /*#p25.RENA*/ wire RENA_CBD_TO_VPDn = not1(RELA_CBD_TO_VPDp);
    /*#p25.ROFA*/ wire ROFA_CBD_TO_VPDp = not1(RENA_CBD_TO_VPDn);

    /*
    if (ROFA_CBD_TO_VPDp) {
      printf("! 0x%02x\n", top.cpu_bus.get_bus_data());

      printf("$ 0x%02x\n", pack_p(BUS_VRAM_D0p.tp(),
                                  BUS_VRAM_D1p.tp(),
                                  BUS_VRAM_D2p.tp(),
                                  BUS_VRAM_D3p.tp(),
                                  BUS_VRAM_D4p.tp(),
                                  BUS_VRAM_D5p.tp(),
                                  BUS_VRAM_D6p.tp(),
                                  BUS_VRAM_D7p.tp()));


    }
    */

    /*#*/ PIN_VRAM_D00p.io_pin(REGE_D0n, RURA_D0n, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D01p.io_pin(RYKY_D1n, RULY_D1n, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D02p.io_pin(RAZO_D2n, RARE_D2n, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D03p.io_pin(RADA_D3n, RODU_D3n, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D04p.io_pin(RYRO_D4n, RUBE_D4n, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D05p.io_pin(REVU_D5n, RUMU_D5n, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D06p.io_pin(REKU_D6n, RYTY_D6n, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D07p.io_pin(RYZE_D7n, RADY_D7n, ROFA_CBD_TO_VPDp);
  }

  //----------------------------------------
  // VRAM data in

  // VPD -> VBD
  {
    /*p25.TAVY*/ wire TAVY_MOEp = not1(PIN_VRAM_OEn.qn());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.qp());  // Schematic wrong, second input is PIN_CPU_WRp
    /*p25.SALE*/ wire SALE_VRAM_WRn = mux2p(top.TUTO_DBG_VRAMp, TAVY_MOEp, TEGU_CPU_VRAM_WRn);

    /*p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn);
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(PIN_VRAM_CSn.qn());
    /*p25.TOLE*/ wire TOLE_VRAM_RDp     = mux2p(top.TUTO_DBG_VRAMp, TEFY_VRAM_MCSp, TUCA_CPU_VRAM_RDp);
    /*p25.SERE*/ wire SERE_CPU_VRM_RDp = and2(TOLE_VRAM_RDp, ROPY_RENDERINGn);
    /*p25.RUVY*/ wire RUVY_VRAM_WR = not1(SALE_VRAM_WRn);
    /*p25.SAZO*/ wire SAZO_VRAM_RD = and2(RUVY_VRAM_WR, SERE_CPU_VRM_RDp);
    /*p25.RYJE*/ wire RYJE_VRAM_RDn = not1(SAZO_VRAM_RD);
    /*p25.REVO*/ wire REVO_VRAM_RDp = not1(RYJE_VRAM_RDn);
    /*p25.RELA*/ wire RELA_VPD_TO_VBDn = or2(REVO_VRAM_RDp, SAZO_VRAM_RD);
    /*p25.RENA*/ wire RENA_VPD_TO_VBDp = not1(RELA_VPD_TO_VBDn);

    /*p25.RODY*/ BUS_VRAM_D0p.tri_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D00p.qn());
    /*p25.REBA*/ BUS_VRAM_D1p.tri_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D01p.qn());
    /*p25.RYDO*/ BUS_VRAM_D2p.tri_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D02p.qn());
    /*p25.REMO*/ BUS_VRAM_D3p.tri_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D03p.qn());
    /*p25.ROCE*/ BUS_VRAM_D4p.tri_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D04p.qn());
    /*p25.ROPU*/ BUS_VRAM_D5p.tri_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D05p.qn());
    /*p25.RETA*/ BUS_VRAM_D6p.tri_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D06p.qn());
    /*p25.RAKU*/ BUS_VRAM_D7p.tri_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D07p.qn());
  }

  // VBD -> CBD
  {
#if 0
    wire SEBY_VBD_TO_CBDp = and2(SOSE_8000_9FFFp,
                                 !PIN_CPU_ADDR_EXTp,
                                 ROPY_RENDERINGn,
                                 PIN_CPU_RDp,
                                 PIN_CPU_LATCH_EXT);

#endif

    /*p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn);
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(PIN_VRAM_CSn.qn());
    /*p25.TOLE*/ wire TOLE_VRAM_RDp     = mux2p(top.TUTO_DBG_VRAMp, TEFY_VRAM_MCSp, TUCA_CPU_VRAM_RDp);
    /*p25.SERE*/ wire SERE_CPU_VRM_RDp = and2(TOLE_VRAM_RDp, ROPY_RENDERINGn);
    /*p25.TYVY*/ wire TYVY_VBD_TO_CBDn = nand2(SERE_CPU_VRM_RDp, top.LEKO_CPU_RDp);
    /*p25.SEBY*/ wire SEBY_VBD_TO_CBDp = not1(TYVY_VBD_TO_CBDn);

    /*p25.RERY*/ wire RERY_VBUS_D0n = not1(BUS_VRAM_D0p.qp());
    /*p25.RUNA*/ wire RUNA_VBUS_D1n = not1(BUS_VRAM_D1p.qp());
    /*p25.RONA*/ wire RONA_VBUS_D2n = not1(BUS_VRAM_D2p.qp());
    /*p25.RUNO*/ wire RUNO_VBUS_D3n = not1(BUS_VRAM_D3p.qp());
    /*p25.SANA*/ wire SANA_VBUS_D4n = not1(BUS_VRAM_D4p.qp());
    /*p25.RORO*/ wire RORO_VBUS_D5n = not1(BUS_VRAM_D5p.qp());
    /*p25.RABO*/ wire RABO_VBUS_D6n = not1(BUS_VRAM_D6p.qp());
    /*p25.SAME*/ wire SAME_VBUS_D7n = not1(BUS_VRAM_D7p.qp());

    /*#p25.RUGA*/ top.cpu_bus.BUS_CPU_D0p.tri_6pn(SEBY_VBD_TO_CBDp, RERY_VBUS_D0n);
    /* p25.ROTA*/ top.cpu_bus.BUS_CPU_D1p.tri_6pn(SEBY_VBD_TO_CBDp, RUNA_VBUS_D1n);
    /* p25.RYBU*/ top.cpu_bus.BUS_CPU_D2p.tri_6pn(SEBY_VBD_TO_CBDp, RONA_VBUS_D2n);
    /* p25.RAJU*/ top.cpu_bus.BUS_CPU_D3p.tri_6pn(SEBY_VBD_TO_CBDp, RUNO_VBUS_D3n);
    /* p25.TYJA*/ top.cpu_bus.BUS_CPU_D4p.tri_6pn(SEBY_VBD_TO_CBDp, SANA_VBUS_D4n);
    /* p25.REXU*/ top.cpu_bus.BUS_CPU_D5p.tri_6pn(SEBY_VBD_TO_CBDp, RORO_VBUS_D5n);
    /* p25.RUPY*/ top.cpu_bus.BUS_CPU_D6p.tri_6pn(SEBY_VBD_TO_CBDp, RABO_VBUS_D6n);
    /* p25.TOKU*/ top.cpu_bus.BUS_CPU_D7p.tri_6pn(SEBY_VBD_TO_CBDp, SAME_VBUS_D7n);
  }

  // VBD -> tile pix temp
  {
    /*#p32.LOMA*/ wire LOMA_LATCH_TILE_DAn = not1(top.tile_fetcher.METE_LATCH_TILE_DAp);

    /*p32.LEGU*/ LEGU_TILE_DA0n.dff8n(LOMA_LATCH_TILE_DAn, BUS_VRAM_D0p.qp());
    /*p32.NUDU*/ NUDU_TILE_DA1n.dff8n(LOMA_LATCH_TILE_DAn, BUS_VRAM_D1p.qp());
    /*p32.MUKU*/ MUKU_TILE_DA2n.dff8n(LOMA_LATCH_TILE_DAn, BUS_VRAM_D2p.qp());
    /*p32.LUZO*/ LUZO_TILE_DA3n.dff8n(LOMA_LATCH_TILE_DAn, BUS_VRAM_D3p.qp());
    /*p32.MEGU*/ MEGU_TILE_DA4n.dff8n(LOMA_LATCH_TILE_DAn, BUS_VRAM_D4p.qp());
    /*p32.MYJY*/ MYJY_TILE_DA5n.dff8n(LOMA_LATCH_TILE_DAn, BUS_VRAM_D5p.qp());
    /*p32.NASA*/ NASA_TILE_DA6n.dff8n(LOMA_LATCH_TILE_DAn, BUS_VRAM_D6p.qp());
    /*p32.NEFO*/ NEFO_TILE_DA7n.dff8n(LOMA_LATCH_TILE_DAn, BUS_VRAM_D7p.qp());

    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /*p??.LUVE*/ wire LUVE_MATCH_TILE_DBp = not1(top.tile_fetcher.LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /*p32.LABU*/ wire LABU_LATCH_TILE_DBn = not1(LUVE_MATCH_TILE_DBp);


    /*p32.RAWU*/ RAWU_TILE_DB0p.dff11(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D0p.qp());
    /*p32.POZO*/ POZO_TILE_DB1p.dff11(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D1p.qp());
    /*p32.PYZO*/ PYZO_TILE_DB2p.dff11(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D2p.qp());
    /*p32.POXA*/ POXA_TILE_DB3p.dff11(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D3p.qp());
    /*p32.PULO*/ PULO_TILE_DB4p.dff11(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D4p.qp());
    /*p32.POJU*/ POJU_TILE_DB5p.dff11(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D5p.qp());
    /*p32.POWY*/ POWY_TILE_DB6p.dff11(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D6p.qp());
    /*p32.PYJU*/ PYJU_TILE_DB7p.dff11(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D7p.qp());
  }

  // VRAM tri -> sprite pix temp + x flip
  {
    /*#p29.XONO*/ wire XONO_FLIP_X = and2(top.sprite_scanner.BAXO_OAM_DB5p.q08p(), top.sprite_fetcher.TEXY_SPR_READ_VRAMp);

    /*p33.PUTE*/ wire PUTE_FLIP0p = mux2p(XONO_FLIP_X, BUS_VRAM_D7p.qp(), BUS_VRAM_D0p.qp());
    /*p33.PELO*/ wire PELO_FLIP1p = mux2p(XONO_FLIP_X, BUS_VRAM_D6p.qp(), BUS_VRAM_D1p.qp());
    /*p33.PONO*/ wire PONO_FLIP2p = mux2p(XONO_FLIP_X, BUS_VRAM_D5p.qp(), BUS_VRAM_D2p.qp());
    /*p33.POBE*/ wire POBE_FLIP3p = mux2p(XONO_FLIP_X, BUS_VRAM_D4p.qp(), BUS_VRAM_D3p.qp());
    /*p33.PACY*/ wire PACY_FLIP4p = mux2p(XONO_FLIP_X, BUS_VRAM_D3p.qp(), BUS_VRAM_D4p.qp());
    /*p33.PUGU*/ wire PUGU_FLIP5p = mux2p(XONO_FLIP_X, BUS_VRAM_D2p.qp(), BUS_VRAM_D5p.qp());
    /*p33.PAWE*/ wire PAWE_FLIP6p = mux2p(XONO_FLIP_X, BUS_VRAM_D1p.qp(), BUS_VRAM_D6p.qp());
    /*p33.PULY*/ wire PULY_FLIP7p = mux2p(XONO_FLIP_X, BUS_VRAM_D0p.qp(), BUS_VRAM_D7p.qp());

    /*#p29.PEBY*/ wire PEBY_CLKp = not1(top.sprite_fetcher.RACA_LATCH_SPPIXB);
    /*#p29.NYBE*/ wire NYBE_CLKn = not1(PEBY_CLKp);
    /*#p29.PUCO*/ wire PUCO_CLKp = not1(NYBE_CLKn);
    /*#p29.PUCO*/ wire PUCO_CLKn = not1(PUCO_CLKp);

    /*p33.PEFO*/ PEFO_SPRITE_DB0n.dff8(PUCO_CLKp, PUCO_CLKn, PUTE_FLIP0p);
    /*p33.ROKA*/ ROKA_SPRITE_DB1n.dff8(PUCO_CLKp, PUCO_CLKn, PELO_FLIP1p);
    /*p33.MYTU*/ MYTU_SPRITE_DB2n.dff8(PUCO_CLKp, PUCO_CLKn, PONO_FLIP2p);
    /*p33.RAMU*/ RAMU_SPRITE_DB3n.dff8(PUCO_CLKp, PUCO_CLKn, POBE_FLIP3p);
    /*p33.SELE*/ SELE_SPRITE_DB4n.dff8(PUCO_CLKp, PUCO_CLKn, PACY_FLIP4p);
    /*p33.SUTO*/ SUTO_SPRITE_DB5n.dff8(PUCO_CLKp, PUCO_CLKn, PUGU_FLIP5p);
    /*p33.RAMA*/ RAMA_SPRITE_DB6n.dff8(PUCO_CLKp, PUCO_CLKn, PAWE_FLIP6p);
    /*p33.RYDU*/ RYDU_SPRITE_DB7n.dff8(PUCO_CLKp, PUCO_CLKn, PULY_FLIP7p);

    /*#p29.VYWA*/ wire VYWA_CLKp = not1(top.sprite_fetcher.TOPU_LATCH_SPPIXA);
    /*#p29.WENY*/ wire WENY_CLKn = not1(VYWA_CLKp);
    /*#p29.XADO*/ wire XADO_CLKp = not1(WENY_CLKn);
    /*#p29.????*/ wire XADO_CLKn = not1(XADO_CLKp);

    /*p33.REWO*/ REWO_SPRITE_DA0n.dff8(XADO_CLKp, XADO_CLKn, PUTE_FLIP0p);
    /*p33.PEBA*/ PEBA_SPRITE_DA1n.dff8(XADO_CLKp, XADO_CLKn, PELO_FLIP1p);
    /*p33.MOFO*/ MOFO_SPRITE_DA2n.dff8(XADO_CLKp, XADO_CLKn, PONO_FLIP2p);
    /*p33.PUDU*/ PUDU_SPRITE_DA3n.dff8(XADO_CLKp, XADO_CLKn, POBE_FLIP3p);
    /*p33.SAJA*/ SAJA_SPRITE_DA4n.dff8(XADO_CLKp, XADO_CLKn, PACY_FLIP4p);
    /*p33.SUNY*/ SUNY_SPRITE_DA5n.dff8(XADO_CLKp, XADO_CLKn, PUGU_FLIP5p);
    /*p33.SEMO*/ SEMO_SPRITE_DA6n.dff8(XADO_CLKp, XADO_CLKn, PAWE_FLIP6p);
    /*p33.SEGA*/ SEGA_SPRITE_DA7n.dff8(XADO_CLKp, XADO_CLKn, PULY_FLIP7p);
  }
}

//------------------------------------------------------------------------------
