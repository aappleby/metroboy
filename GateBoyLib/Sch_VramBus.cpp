#include "GateBoyLib/Sch_VramBus.h"

#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

#define DUMP(A) do { d("%-20s : %d\n", #A, wire(A)); } while(0);

//------------------------------------------------------------------------------

void VramBus::dump(Dumper& d, const SchematicTop& /*top*/) const {
  d("---------- VRAM Bus ----------\n");

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

  int TILE_DA = pack_p(LEGU_TILE_DA0n.qn(), NUDU_TILE_DA1n.qn(), MUKU_TILE_DA2n.qn(), LUZO_TILE_DA3n.qn(),
                       MEGU_TILE_DA4n.qn(), MYJY_TILE_DA5n.qn(), NASA_TILE_DA6n.qn(), NEFO_TILE_DA7n.qn());
  int TILE_DB = pack_p(RAWU_TILE_DB0n.qn(), POZO_TILE_DB1n.qn(), PYZO_TILE_DB2n.qn(), POXA_TILE_DB3n.qn(),
                       PULO_TILE_DB4n.qn(), POJU_TILE_DB5n.qn(), POWY_TILE_DB6n.qn(), PYJU_TILE_DB7n.qn());
  int SPRITE_DA = pack_p(PEFO_SPRITE_DB0n.qn(), ROKA_SPRITE_DB1n.qn(), MYTU_SPRITE_DB2n.qn(), RAMU_SPRITE_DB3n.qn(),
                         SELE_SPRITE_DB4n.qn(), SUTO_SPRITE_DB5n.qn(), RAMA_SPRITE_DB6n.qn(), RYDU_SPRITE_DB7n.qn());
  int SPRITE_DB = pack_p(REWO_SPRITE_DA0n.qn(), PEBA_SPRITE_DA1n.qn(), MOFO_SPRITE_DA2n.qn(), PUDU_SPRITE_DA3n.qn(),
                         SAJA_SPRITE_DA4n.qn(), SUNY_SPRITE_DA5n.qn(), SEMO_SPRITE_DA6n.qn(), SEGA_SPRITE_DA7n.qn());

  d("TILE_DA       : 0x%02x\n", TILE_DA);
  d("TILE_DB       : 0x%02x\n", TILE_DB);
  d("SPRITE_DA     : 0x%02x\n", SPRITE_DA);
  d("SPRITE_DB     : 0x%02x\n", SPRITE_DB);
  d("\n");
}

//------------------------------------------------------------------------------

void VramBus::tock(SchematicTop& top) {
  wire VYPO = 1;

  /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.PIN_CPU_ADDR_EXTp.tp(), top.cpu_bus.TEVY_8000_9FFFn());
  /*p25.TEFA*/ wire TEFA_8000_9FFFp = nor2(top.cpu_bus.SYRO_FE00_FFFFp(), TEXO_8000_9FFFn);
  /*p25.SOSE*/ wire SOSE_8000_9FFFp = and2(top.cpu_bus.BUS_CPU_A15.tp(), TEFA_8000_9FFFp);

  /*p25.ROPY*/ wire ROPY_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGp.tp());

  //----------------------------------------
  // VRAM pins

  {
    /*p25.TUJA*/ wire TUJA_CPU_VRAM_WRp = and2(SOSE_8000_9FFFp, top.APOV_CPU_WRp_xxxxEFGx);
    /*p25.SUDO*/ wire SUDO_MWRp_C = not1(PIN_VRAM_WRn.qn());
    /*p25.TYJY*/ wire TYJY_DBG_VRAM_WRp = mux2_p(SUDO_MWRp_C, TUJA_CPU_VRAM_WRp, top.TUTO_DBG_VRAMp);

    /*p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn);
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(PIN_VRAM_CSn.qn());
    /*p25.TOLE*/ wire TOLE_VRAM_RDp     = mux2_p(TEFY_VRAM_MCSp, TUCA_CPU_VRAM_RDp, top.TUTO_DBG_VRAMp);
    /*p25.SERE*/ wire SERE_CPU_VRM_RDp = and2(TOLE_VRAM_RDp, ROPY_RENDERINGn);

    /*p25.SOHY*/ wire SOHY_MWRn = nand2(TYJY_DBG_VRAM_WRp, SERE_CPU_VRM_RDp);
    /*p25.RACO*/ wire RACO_DBG_VRAMn = not1(top.TUTO_DBG_VRAMp);
    /*p25.TAXY*/ wire TAXY_MWRn_A = and2(SOHY_MWRn, RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire SOFY_MWRn_D = or2(SOHY_MWRn, top.TUTO_DBG_VRAMp);
    /*p25.SYSY*/ wire SYSY_MWRp_A = not1(TAXY_MWRn_A);
    /*p25.RAGU*/ wire RAGU_MWRp_D = not1(SOFY_MWRn_D);
    PIN_VRAM_WRn = io_pin(SYSY_MWRp_A, RAGU_MWRp_D);
  }

  {
    /*p25.XANE*/ wire XANE_VRAM_LOCKn = nor2(top.dma_reg.LUFA_DMA_READ_VRAMp, top.pix_pipe.XYMU_RENDERINGp.tp()); // def nor4
    /*p25.TAVY*/ wire TAVY_MOEp = not1(PIN_VRAM_OEn.qn());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.tp());  // Schematic wrong, second input is PIN_CPU_WRp
    /*p25.SALE*/ wire SALE_VRAM_WRn = mux2_p(TAVY_MOEp, TEGU_CPU_VRAM_WRn, top.TUTO_DBG_VRAMp);
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
    PIN_VRAM_OEn = io_pin(REFO_MOEn_A, SAHA_MOEn_D);
  }

  {
    /*p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn);
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(PIN_VRAM_CSn.qn());
    /*p25.TOLE*/ wire TOLE_VRAM_RDp     = mux2_p(TEFY_VRAM_MCSp, TUCA_CPU_VRAM_RDp, top.TUTO_DBG_VRAMp);
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
    PIN_VRAM_CSn = io_pin(SOKY_MCSp_A, SETY_MCSp_D);
  }

  //----------------------------------------
  // VRAM address

  // CPU address -> vram address
  {
    /* p25.XANE*/ wire XANE_VRAM_LOCKn = nor2(top.dma_reg.LUFA_DMA_READ_VRAMp, top.pix_pipe.XYMU_RENDERINGp.tp());
    /* p25.XEDU*/ wire XEDU_CPU_VRAM_RDn = not1(XANE_VRAM_LOCKn);
    /* p25.XAKY*/ BUS_VRAM_A00n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A00.tp());
    /* p25.XUXU*/ BUS_VRAM_A01n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A01.tp());
    /* p25.XYNE*/ BUS_VRAM_A02n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A02.tp());
    /* p25.XODY*/ BUS_VRAM_A03n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A03.tp());
    /* p25.XECA*/ BUS_VRAM_A04n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A04.tp());
    /* p25.XOBA*/ BUS_VRAM_A05n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A05.tp());
    /* p25.XOPO*/ BUS_VRAM_A06n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A06.tp());
    /* p25.XYBO*/ BUS_VRAM_A07n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A07.tp());
    /* p25.RYSU*/ BUS_VRAM_A08n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A08.tp());
    /* p25.RESE*/ BUS_VRAM_A09n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A09.tp());
    /* p25.RUSE*/ BUS_VRAM_A10n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A10.tp());
    /* p25.RYNA*/ BUS_VRAM_A11n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A11.tp());
    /* p25.RUMO*/ BUS_VRAM_A12n = tribuf_6nn(XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A12.tp());
  }

  // DMA address -> vram address
  {
    /* p04.AHOC*/ wire AHOC_DMA_VRAM_RDn = not1(top.dma_reg.LUFA_DMA_READ_VRAMp);
    /* p04.ECAL*/ BUS_VRAM_A00n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp());
    /* p04.EGEZ*/ BUS_VRAM_A01n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp());
    /* p04.FUHE*/ BUS_VRAM_A02n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp());
    /* p04.FYZY*/ BUS_VRAM_A03n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp());
    /* p04.DAMU*/ BUS_VRAM_A04n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp());
    /* p04.DAVA*/ BUS_VRAM_A05n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp());
    /* p04.ETEG*/ BUS_VRAM_A06n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp());
    /*#p04.EREW*/ BUS_VRAM_A07n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp());
    /*#p04.EVAX*/ BUS_VRAM_A08n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NAFA_DMA_A08n.qn());
    /* p04.DUVE*/ BUS_VRAM_A09n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PYNE_DMA_A09n.qn());
    /* p04.ERAF*/ BUS_VRAM_A10n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.PARA_DMA_A10n.qn());
    /* p04.FUSY*/ BUS_VRAM_A11n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NYDO_DMA_A11n.qn());
    /* p04.EXYF*/ BUS_VRAM_A12n = tribuf_6nn(AHOC_DMA_VRAM_RDn, top.dma_reg.NYGY_DMA_A12n.qn());
  }

  // Sprite fetcher read
  {
    /*#p29.WUKY*/ wire WUKY_FLIP_Yp = not1(top.oam_bus.YZOS_OAM_DB6p.qp());
    
    /*#p29.CYVU*/ wire CYVU_L0 = xor2(WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L0.tp());
    /*#p29.BORE*/ wire BORE_L1 = xor2(WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L1.tp());
    /*#p29.BUVY*/ wire BUVY_L2 = xor2(WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L2.tp());
    /*#p29.WAGO*/ wire WAGO_L3 = xor2(WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L3.tp());
    
    /*#p29.FUFO*/ wire FUFO_LCDC_SPSIZEn = not1(top.pix_pipe.XYMO_LCDC_SPSIZEn.qn());
    /*#p29.GEJY*/ wire GEJY_L3 = amux2(top.oam_bus.XUSO_OAM_DA0p.qp(), FUFO_LCDC_SPSIZEn,
                                       top.pix_pipe.XYMO_LCDC_SPSIZEn.qn(), WAGO_L3);

    /* p29.ABEM*/ BUS_VRAM_A00n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_fetcher.XUQU_SPRITE_AB);
    /* p29.BAXE*/ BUS_VRAM_A01n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, CYVU_L0);
    /* p29.ARAS*/ BUS_VRAM_A02n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, BORE_L1);
    /* p29.AGAG*/ BUS_VRAM_A03n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, BUVY_L2);
    /* p29.FAMU*/ BUS_VRAM_A04n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, GEJY_L3);
    /*#p29.FUGY*/ BUS_VRAM_A05n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.XEGU_OAM_DA1p.qp());
    /* p29.GAVO*/ BUS_VRAM_A06n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.YJEX_OAM_DA2p.qp());
    /* p29.WYGA*/ BUS_VRAM_A07n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.XYJU_OAM_DA3p.qp());
    /* p29.WUNE*/ BUS_VRAM_A08n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.YBOG_OAM_DA4p.qp());
    /* p29.GOTU*/ BUS_VRAM_A09n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.WYSO_OAM_DA5p.qp());
    /* p29.GEGU*/ BUS_VRAM_A10n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.XOTE_OAM_DA6p.qp());
    /* p29.XEHE*/ BUS_VRAM_A11n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.YZAB_OAM_DA7p.qp());
    /* p29.DYSO*/ BUS_VRAM_A12n = tribuf_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, 0);   // sprites always in low half of tile store
  }

  {
    // Map scroll adder
#pragma warning(push)
#pragma warning(disable:4189)
    /*#p26.FAFO*/ wire FAFO_TILE_Y0S = add_s(top.lcd_reg.MUWY_Y0p.qp(), top.pix_pipe.GAVE_SCY0n.qn(), 0);
    /*#p26.FAFO*/ wire FAFO_TILE_Y0C = add_c(top.lcd_reg.MUWY_Y0p.qp(), top.pix_pipe.GAVE_SCY0n.qn(), 0);
    /* p26.EMUX*/ wire EMUX_TILE_Y1S = add_s(top.lcd_reg.MYRO_Y1p.qp(), top.pix_pipe.FYMO_SCY1n.qn(), FAFO_TILE_Y0C);
    /* p26.EMUX*/ wire EMUX_TILE_Y1C = add_c(top.lcd_reg.MYRO_Y1p.qp(), top.pix_pipe.FYMO_SCY1n.qn(), FAFO_TILE_Y0C);
    /* p26.ECAB*/ wire ECAB_TILE_Y2S = add_s(top.lcd_reg.LEXA_Y2p.qp(), top.pix_pipe.FEZU_SCY2n.qn(), EMUX_TILE_Y1C);
    /* p26.ECAB*/ wire ECAB_TILE_Y2C = add_c(top.lcd_reg.LEXA_Y2p.qp(), top.pix_pipe.FEZU_SCY2n.qn(), EMUX_TILE_Y1C);
    /* p26.ETAM*/ wire ETAM_MAP_Y0S  = add_s(top.lcd_reg.LYDO_Y3p.qp(), top.pix_pipe.FUJO_SCY3n.qn(), ECAB_TILE_Y2C);
    /* p26.ETAM*/ wire ETAM_MAP_Y0C  = add_c(top.lcd_reg.LYDO_Y3p.qp(), top.pix_pipe.FUJO_SCY3n.qn(), ECAB_TILE_Y2C);
    /* p26.DOTO*/ wire DOTO_MAP_Y1S  = add_s(top.lcd_reg.LOVU_Y4p.qp(), top.pix_pipe.DEDE_SCY4n.qn(), ETAM_MAP_Y0C);
    /* p26.DOTO*/ wire DOTO_MAP_Y1C  = add_c(top.lcd_reg.LOVU_Y4p.qp(), top.pix_pipe.DEDE_SCY4n.qn(), ETAM_MAP_Y0C);
    /* p26.DABA*/ wire DABA_MAP_Y2S  = add_s(top.lcd_reg.LEMA_Y5p.qp(), top.pix_pipe.FOTY_SCY5n.qn(), DOTO_MAP_Y1C);
    /* p26.DABA*/ wire DABA_MAP_Y2C  = add_c(top.lcd_reg.LEMA_Y5p.qp(), top.pix_pipe.FOTY_SCY5n.qn(), DOTO_MAP_Y1C);
    /* p26.EFYK*/ wire EFYK_MAP_Y3S  = add_s(top.lcd_reg.MATO_Y6p.qp(), top.pix_pipe.FOHA_SCY6n.qn(), DABA_MAP_Y2C);
    /* p26.EFYK*/ wire EFYK_MAP_Y3C  = add_c(top.lcd_reg.MATO_Y6p.qp(), top.pix_pipe.FOHA_SCY6n.qn(), DABA_MAP_Y2C);
    /* p26.EJOK*/ wire EJOK_MAP_Y4S  = add_s(top.lcd_reg.LAFO_Y7p.qp(), top.pix_pipe.FUNY_SCY7n.qn(), EFYK_MAP_Y3C);
    /* p26.EJOK*/ wire EJOK_MAP_Y4C  = add_c(top.lcd_reg.LAFO_Y7p.qp(), top.pix_pipe.FUNY_SCY7n.qn(), EFYK_MAP_Y3C);

    /*#p26.ATAD*/ wire ATAD_TILE_X0S = add_s(top.pix_pipe.XEHO_X0p.qp(), top.pix_pipe.DATY_SCX0n.qn(), 0);
    /*#p26.ATAD*/ wire ATAD_TILE_X0C = add_c(top.pix_pipe.XEHO_X0p.qp(), top.pix_pipe.DATY_SCX0n.qn(), 0);
    /* p26.BEHU*/ wire BEHU_TILE_X1S = add_s(top.pix_pipe.SAVY_X1p.qp(), top.pix_pipe.DUZU_SCX1n.qn(), ATAD_TILE_X0C);
    /* p26.BEHU*/ wire BEHU_TILE_X1C = add_c(top.pix_pipe.SAVY_X1p.qp(), top.pix_pipe.DUZU_SCX1n.qn(), ATAD_TILE_X0C);
    /* p26.APYH*/ wire APYH_TILE_X2S = add_s(top.pix_pipe.XODU_X2p.qp(), top.pix_pipe.CYXU_SCX2n.qn(), BEHU_TILE_X1C);
    /* p26.APYH*/ wire APYH_TILE_X2C = add_c(top.pix_pipe.XODU_X2p.qp(), top.pix_pipe.CYXU_SCX2n.qn(), BEHU_TILE_X1C);
    /* p26.BABE*/ wire BABE_MAP_X0S  = add_s(top.pix_pipe.XYDO_X3p.qp(), top.pix_pipe.GUBO_SCX3n.qn(), APYH_TILE_X2C);
    /* p26.BABE*/ wire BABE_MAP_X0C  = add_c(top.pix_pipe.XYDO_X3p.qp(), top.pix_pipe.GUBO_SCX3n.qn(), APYH_TILE_X2C);
    /* p26.ABOD*/ wire ABOD_MAP_X1S  = add_s(top.pix_pipe.TUHU_X4p.qp(), top.pix_pipe.BEMY_SCX4n.qn(), BABE_MAP_X0C);
    /* p26.ABOD*/ wire ABOD_MAP_X1C  = add_c(top.pix_pipe.TUHU_X4p.qp(), top.pix_pipe.BEMY_SCX4n.qn(), BABE_MAP_X0C);
    /* p26.BEWY*/ wire BEWY_MAP_X2S  = add_s(top.pix_pipe.TUKY_X5p.qp(), top.pix_pipe.CUZY_SCX5n.qn(), ABOD_MAP_X1C);
    /* p26.BEWY*/ wire BEWY_MAP_X2C  = add_c(top.pix_pipe.TUKY_X5p.qp(), top.pix_pipe.CUZY_SCX5n.qn(), ABOD_MAP_X1C);
    /* p26.BYCA*/ wire BYCA_MAP_X3S  = add_s(top.pix_pipe.TAKO_X6p.qp(), top.pix_pipe.CABU_SCX6n.qn(), BEWY_MAP_X2C);
    /* p26.BYCA*/ wire BYCA_MAP_X3C  = add_c(top.pix_pipe.TAKO_X6p.qp(), top.pix_pipe.CABU_SCX6n.qn(), BEWY_MAP_X2C);
    /* p26.ACUL*/ wire ACUL_MAP_X4S  = add_s(top.pix_pipe.SYBE_X7p.qp(), top.pix_pipe.BAKE_SCX7n.qn(), BYCA_MAP_X3C);
    /* p26.ACUL*/ wire ACUL_MAP_X4C  = add_c(top.pix_pipe.SYBE_X7p.qp(), top.pix_pipe.BAKE_SCX7n.qn(), BYCA_MAP_X3C);
#pragma warning(pop)

    // Background map read
    {
      /* p26.AXEP*/ BUS_VRAM_A00n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, BABE_MAP_X0S);
      /* p26.AFEB*/ BUS_VRAM_A01n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, ABOD_MAP_X1S);
      /* p26.ALEL*/ BUS_VRAM_A02n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, BEWY_MAP_X2S);
      /* p26.COLY*/ BUS_VRAM_A03n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, BYCA_MAP_X3S);
      /* p26.AJAN*/ BUS_VRAM_A04n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, ACUL_MAP_X4S);
      /* p26.DUHO*/ BUS_VRAM_A05n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, ETAM_MAP_Y0S);
      /* p26.CASE*/ BUS_VRAM_A06n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, DOTO_MAP_Y1S);
      /* p26.CYPO*/ BUS_VRAM_A07n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, DABA_MAP_Y2S);
      /* p26.CETA*/ BUS_VRAM_A08n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, EFYK_MAP_Y3S);
      /* p26.DAFE*/ BUS_VRAM_A09n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, EJOK_MAP_Y4S);
      /*#p26.AMUV*/ BUS_VRAM_A10n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, top.pix_pipe.XAFO_LCDC_BGMAPn.qn());
      /* p26.COVE*/ BUS_VRAM_A11n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, 1);
      /* p26.COXO*/ BUS_VRAM_A12n = tribuf_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, 1);
    }

    // Window map read
    {
      /*#p27.XEJA*/ BUS_VRAM_A00n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WYKA_WIN_X3.qp());
      /* p27.XAMO*/ BUS_VRAM_A01n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WODY_WIN_X4.qp());
      /* p27.XAHE*/ BUS_VRAM_A02n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WOBO_WIN_X5.qp());
      /* p27.XULO*/ BUS_VRAM_A03n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WYKO_WIN_X6.qp());
      /* p27.WUJU*/ BUS_VRAM_A04n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.XOLO_WIN_X7.qp());
      /*#p27.VYTO*/ BUS_VRAM_A05n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TUFU_WIN_Y3.qp());
      /* p27.VEHA*/ BUS_VRAM_A06n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TAXA_WIN_Y4.qp());
      /* p27.VACE*/ BUS_VRAM_A07n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TOZO_WIN_Y5.qp());
      /* p27.VOVO*/ BUS_VRAM_A08n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TATE_WIN_Y6.qp());
      /* p27.VULO*/ BUS_VRAM_A09n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TEKE_WIN_Y7.qp());
      /*#p27.VEVY*/ BUS_VRAM_A10n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WOKY_LCDC_WINMAPn.qn());
      /* p27.VEZA*/ BUS_VRAM_A11n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, 1);
      /* p27.VOGU*/ BUS_VRAM_A12n = tribuf_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, 1);
    }

    // Background/window tile read
    {
      /*#p26.ASUM*/ BUS_VRAM_A00n = tribuf_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  top.tile_fetcher.XUHA_FETCH_S2p);
      /* p26.EVAD*/ BUS_VRAM_A01n = tribuf_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  FAFO_TILE_Y0S);
      /* p26.DAHU*/ BUS_VRAM_A02n = tribuf_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  EMUX_TILE_Y1S);
      /* p26.DODE*/ BUS_VRAM_A03n = tribuf_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  ECAB_TILE_Y2S); // check outputs of ECAB

      /*#p25.XONU*/ BUS_VRAM_A00n = tribuf_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.tile_fetcher.XUHA_FETCH_S2p);
      /*#p25.WUDO*/ BUS_VRAM_A01n = tribuf_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VYNO_WIN_Y0.qp());
      /*#p25.WAWE*/ BUS_VRAM_A02n = tribuf_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VUJO_WIN_Y1.qp());
      /*#p25.WOLU*/ BUS_VRAM_A03n = tribuf_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VYMU_WIN_Y2.qp());

      /*#p25.VAPY*/ BUS_VRAM_A04n = tribuf_6pn(top.tile_fetcher.NETA_TILE_READp,     RAWU_TILE_DB0n.qn());
      /*#p25.SEZU*/ BUS_VRAM_A05n = tribuf_6pn(top.tile_fetcher.NETA_TILE_READp,     POZO_TILE_DB1n.qn());
      /*#p25.VEJY*/ BUS_VRAM_A06n = tribuf_6pn(top.tile_fetcher.NETA_TILE_READp,     PYZO_TILE_DB2n.qn());
      /*#p25.RUSA*/ BUS_VRAM_A07n = tribuf_6pn(top.tile_fetcher.NETA_TILE_READp,     POXA_TILE_DB3n.qn());
      /*#p25.ROHA*/ BUS_VRAM_A08n = tribuf_6pn(top.tile_fetcher.NETA_TILE_READp,     PULO_TILE_DB4n.qn());
      /*#p25.RESO*/ BUS_VRAM_A09n = tribuf_6pn(top.tile_fetcher.NETA_TILE_READp,     POJU_TILE_DB5n.qn());
      /*#p25.SUVO*/ BUS_VRAM_A10n = tribuf_6pn(top.tile_fetcher.NETA_TILE_READp,     POWY_TILE_DB6n.qn());
      /*#p25.TOBO*/ BUS_VRAM_A11n = tribuf_6pn(top.tile_fetcher.NETA_TILE_READp,     PYJU_TILE_DB7n.qn());

      /*#p25.VUZA*/ wire VUZA_TILE_BANKp = nor2(PYJU_TILE_DB7n.qn(), top.pix_pipe.WEXU_LCDC_BGTILEn.qn());
      /*#p25.VURY*/ BUS_VRAM_A12n = tribuf_6pn(top.tile_fetcher.NETA_TILE_READp,     VUZA_TILE_BANKp);
    }
  }

  // VRAM addr bus -> VRAM addr pin
  {
    /*p25.MYFU*/ wire MYFUp = not1(BUS_VRAM_A00n.tp());
    /*p25.MASA*/ wire MASAp = not1(BUS_VRAM_A01n.tp());
    /*p25.MYRE*/ wire MYREp = not1(BUS_VRAM_A02n.tp());
    /*p25.MAVU*/ wire MAVUp = not1(BUS_VRAM_A03n.tp());
    /*p25.MEPA*/ wire MEPAp = not1(BUS_VRAM_A04n.tp());
    /*p25.MYSA*/ wire MYSAp = not1(BUS_VRAM_A05n.tp());
    /*p25.MEWY*/ wire MEWYp = not1(BUS_VRAM_A06n.tp());
    /*p25.MUME*/ wire MUMEp = not1(BUS_VRAM_A07n.tp());
    /*p25.VOVA*/ wire VOVAp = not1(BUS_VRAM_A08n.tp());
    /*p25.VODE*/ wire VODEp = not1(BUS_VRAM_A09n.tp());
    /*p25.RUKY*/ wire RUKYp = not1(BUS_VRAM_A10n.tp());
    /*p25.RUMA*/ wire RUMAp = not1(BUS_VRAM_A11n.tp());
    /*p25.REHO*/ wire REHOp = not1(BUS_VRAM_A12n.tp());

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

    PIN_VRAM_A00p = io_pin(LEXEn, LEXEn);
    PIN_VRAM_A01p = io_pin(LOZUn, LOZUn);
    PIN_VRAM_A02p = io_pin(LACAn, LACAn);
    PIN_VRAM_A03p = io_pin(LUVOn, LUVOn);
    PIN_VRAM_A04p = io_pin(LOLYn, LOLYn);
    PIN_VRAM_A05p = io_pin(LALOn, LALOn);
    PIN_VRAM_A06p = io_pin(LEFAn, LEFAn);
    PIN_VRAM_A07p = io_pin(LUBYn, LUBYn);
    PIN_VRAM_A08p = io_pin(TUJYn, TUJYn);
    PIN_VRAM_A09p = io_pin(TAGOn, TAGOn);
    PIN_VRAM_A10p = io_pin(NUVAn, NUVAn);
    PIN_VRAM_A11p = io_pin(PEDUn, PEDUn);
    PIN_VRAM_A12p = io_pin(PONYn, PONYn);
  }

  //----------------------------------------
  // VRAM data out

  // CBD->VBD->VPD
  {
    /*p25.TAVY*/ wire TAVY_MOEp = not1(PIN_VRAM_OEn.qn());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.tp());  // Schematic wrong, second input is PIN_CPU_WRp
    /*p25.SALE*/ wire SALE_CPU_VRAM_WRn = mux2_p(TAVY_MOEp, TEGU_CPU_VRAM_WRn, top.TUTO_DBG_VRAMp);

    /*p25.TUCA*/ wire TUCA_CPU_VRAM_AVn = and2(SOSE_8000_9FFFp, top.ABUZ_AVn);
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(PIN_VRAM_CSn.qn());
    /*p25.TOLE*/ wire TOLE_VRAM_AVn     = mux2_p(TEFY_VRAM_MCSp, TUCA_CPU_VRAM_AVn, top.TUTO_DBG_VRAMp);
    /*p25.SERE*/ wire SERE = and2(TOLE_VRAM_AVn, ROPY_RENDERINGn);
    /*p25.RUVY*/ wire RUVY_VRAM_WRp = not1(SALE_CPU_VRAM_WRn);
    /*p25.SAZO*/ wire SAZO_CBD_TO_VPDp = and2(RUVY_VRAM_WRp, SERE);
    /*p25.RYJE*/ wire RYJE_CBD_TO_VPDn = not1(SAZO_CBD_TO_VPDp);
    /*p25.REVO*/ wire REVO_CBD_TO_VPDp = not1(RYJE_CBD_TO_VPDn);
    /*p25.ROCY*/ wire ROCY_CBD_TO_VPDp = and2(REVO_CBD_TO_VPDp, SAZO_CBD_TO_VPDp);
    /*p25.RAHU*/ wire RAHU_CBD_TO_VPDn = not1(ROCY_CBD_TO_VPDp);

    /* p25.TEME*/ BUS_VRAM_D0p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D0p.tp());
    /* p25.TEWU*/ BUS_VRAM_D1p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D1p.tp());
    /*#p25.TYGO*/ BUS_VRAM_D2p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D2p.tp());
    /* p25.SOTE*/ BUS_VRAM_D3p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D3p.tp());
    /* p25.SEKE*/ BUS_VRAM_D4p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D4p.tp());
    /* p25.RUJO*/ BUS_VRAM_D5p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D5p.tp());
    /* p25.TOFA*/ BUS_VRAM_D6p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D6p.tp());
    /* p25.SUZA*/ BUS_VRAM_D7p = tribuf_10np(RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D7p.tp());

    /*#p25.ROVE*/ wire ROVE_CBD_TO_VPDp = not1(RAHU_CBD_TO_VPDn);
    /*#p25.SEFA*/ wire SEFAp = and2(BUS_VRAM_D0p.tp(), ROVE_CBD_TO_VPDp);
    /* p25.SOGO*/ wire SOGOp = and2(BUS_VRAM_D1p.tp(), ROVE_CBD_TO_VPDp);
    /* p25.SEFU*/ wire SEFUp = and2(BUS_VRAM_D2p.tp(), ROVE_CBD_TO_VPDp);
    /* p25.SUNA*/ wire SUNAp = and2(BUS_VRAM_D3p.tp(), ROVE_CBD_TO_VPDp);
    /* p25.SUMO*/ wire SUMOp = and2(BUS_VRAM_D4p.tp(), ROVE_CBD_TO_VPDp);
    /* p25.SAZU*/ wire SAZUp = and2(BUS_VRAM_D5p.tp(), ROVE_CBD_TO_VPDp);
    /* p25.SAMO*/ wire SAMOp = and2(BUS_VRAM_D6p.tp(), ROVE_CBD_TO_VPDp);
    /* p25.SUKE*/ wire SUKEp = and2(BUS_VRAM_D7p.tp(), ROVE_CBD_TO_VPDp);

    /*#p25.REGE*/ wire REGEn = not1(SEFAp);
    /* p25.RYKY*/ wire RYKYn = not1(SOGOp);
    /* p25.RAZO*/ wire RAZOn = not1(SEFUp);
    /* p25.RADA*/ wire RADAn = not1(SUNAp);
    /* p25.RYRO*/ wire RYROn = not1(SUMOp);
    /* p25.REVU*/ wire REVUn = not1(SAZUp);
    /* p25.REKU*/ wire REKUn = not1(SAMOp);
    /* p25.RYZE*/ wire RYZEn = not1(SUKEp);

    /*#p25.SYNU*/ wire SYNUp = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D0p.tp());
    /* p25.SYMA*/ wire SYMAp = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D1p.tp());
    /* p25.ROKO*/ wire ROKOp = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D2p.tp());
    /* p25.SYBU*/ wire SYBUp = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D3p.tp());
    /* p25.SAKO*/ wire SAKOp = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D4p.tp());
    /* p25.SEJY*/ wire SEJYp = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D5p.tp());
    /* p25.SEDO*/ wire SEDOp = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D6p.tp());
    /* p25.SAWU*/ wire SAWUp = or2(RAHU_CBD_TO_VPDn, BUS_VRAM_D7p.tp());

    /*#p25.RURA*/ wire RURAn = not1(SYNUp);
    /* p25.RULY*/ wire RULYn = not1(SYMAp);
    /* p25.RARE*/ wire RAREn = not1(ROKOp);
    /* p25.RODU*/ wire RODUn = not1(SYBUp);
    /* p25.RUBE*/ wire RUBEn = not1(SAKOp);
    /* p25.RUMU*/ wire RUMUn = not1(SEJYp);
    /* p25.RYTY*/ wire RYTYn = not1(SEDOp);
    /* p25.RADY*/ wire RADYn = not1(SAWUp);

    /*#p25.RELA*/ wire RELA_CBD_TO_VPDp = or2(REVO_CBD_TO_VPDp, SAZO_CBD_TO_VPDp);
    /*#p25.RENA*/ wire RENA_CBD_TO_VPDn = not1(RELA_CBD_TO_VPDp);
    /*#p25.ROFA*/ wire ROFA_CBD_TO_VPDp = not1(RENA_CBD_TO_VPDn);

    /*#*/ PIN_VRAM_D00p = io_pin(REGEn, RURAn, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D01p = io_pin(RYKYn, RULYn, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D02p = io_pin(RAZOn, RAREn, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D03p = io_pin(RADAn, RODUn, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D04p = io_pin(RYROn, RUBEn, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D05p = io_pin(REVUn, RUMUn, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D06p = io_pin(REKUn, RYTYn, ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D07p = io_pin(RYZEn, RADYn, ROFA_CBD_TO_VPDp);
  }

  //----------------------------------------
  // VRAM data in

  // VPD -> VBD
  {
    /*p25.TAVY*/ wire TAVY_MOEp = not1(PIN_VRAM_OEn.qn());
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand2(SOSE_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.tp());  // Schematic wrong, second input is PIN_CPU_WRp
    /*p25.SALE*/ wire SALE_VRAM_WRn = mux2_p(TAVY_MOEp, TEGU_CPU_VRAM_WRn, top.TUTO_DBG_VRAMp);

    /*p25.TUCA*/ wire TUCA_CPU_VRAM_RDp = and2(SOSE_8000_9FFFp, top.ABUZ_AVn);
    /*p25.TEFY*/ wire TEFY_VRAM_MCSp = not1(PIN_VRAM_CSn.qn());
    /*p25.TOLE*/ wire TOLE_VRAM_RDp     = mux2_p(TEFY_VRAM_MCSp, TUCA_CPU_VRAM_RDp, top.TUTO_DBG_VRAMp);
    /*p25.SERE*/ wire SERE_CPU_VRM_RDp = and2(TOLE_VRAM_RDp, ROPY_RENDERINGn);
    /*p25.RUVY*/ wire RUVY_VRAM_WR = not1(SALE_VRAM_WRn);
    /*p25.SAZO*/ wire SAZO_VRAM_RD = and2(RUVY_VRAM_WR, SERE_CPU_VRM_RDp);
    /*p25.RYJE*/ wire RYJE_VRAM_RDn = not1(SAZO_VRAM_RD);
    /*p25.REVO*/ wire REVO_VRAM_RDp = not1(RYJE_VRAM_RDn);
    /*p25.RELA*/ wire RELA_VPD_TO_VBDn = or2(REVO_VRAM_RDp, SAZO_VRAM_RD);
    /*p25.RENA*/ wire RENA_VPD_TO_VBDp = not1(RELA_VPD_TO_VBDn);

    /*p25.RODY*/ BUS_VRAM_D0p = tribuf_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D00p.qn());
    /*p25.REBA*/ BUS_VRAM_D1p = tribuf_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D01p.qn());
    /*p25.RYDO*/ BUS_VRAM_D2p = tribuf_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D02p.qn());
    /*p25.REMO*/ BUS_VRAM_D3p = tribuf_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D03p.qn());
    /*p25.ROCE*/ BUS_VRAM_D4p = tribuf_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D04p.qn());
    /*p25.ROPU*/ BUS_VRAM_D5p = tribuf_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D05p.qn());
    /*p25.RETA*/ BUS_VRAM_D6p = tribuf_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D06p.qn());
    /*p25.RAKU*/ BUS_VRAM_D7p = tribuf_6pn(RENA_VPD_TO_VBDp, PIN_VRAM_D07p.qn());
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
    /*p25.TOLE*/ wire TOLE_VRAM_RDp     = mux2_p(TEFY_VRAM_MCSp, TUCA_CPU_VRAM_RDp, top.TUTO_DBG_VRAMp);
    /*p25.SERE*/ wire SERE_CPU_VRM_RDp = and2(TOLE_VRAM_RDp, ROPY_RENDERINGn);
    /*p25.TYVY*/ wire TYVY_VBD_TO_CBDn = nand2(SERE_CPU_VRM_RDp, top.LEKO_CPU_RDp);
    /*p25.SEBY*/ wire SEBY_VBD_TO_CBDp = not1(TYVY_VBD_TO_CBDn);

    /*p25.RERY*/ wire RERY_VBUS_D0n = not1(BUS_VRAM_D0p.tp());
    /*p25.RUNA*/ wire RUNA_VBUS_D1n = not1(BUS_VRAM_D1p.tp());
    /*p25.RONA*/ wire RONA_VBUS_D2n = not1(BUS_VRAM_D2p.tp());
    /*p25.RUNO*/ wire RUNO_VBUS_D3n = not1(BUS_VRAM_D3p.tp());
    /*p25.SANA*/ wire SANA_VBUS_D4n = not1(BUS_VRAM_D4p.tp());
    /*p25.RORO*/ wire RORO_VBUS_D5n = not1(BUS_VRAM_D5p.tp());
    /*p25.RABO*/ wire RABO_VBUS_D6n = not1(BUS_VRAM_D6p.tp());
    /*p25.SAME*/ wire SAME_VBUS_D7n = not1(BUS_VRAM_D7p.tp());

    /*#p25.RUGA*/ top.cpu_bus.BUS_CPU_D0p = tribuf_6pn(SEBY_VBD_TO_CBDp, RERY_VBUS_D0n);
    /* p25.ROTA*/ top.cpu_bus.BUS_CPU_D1p = tribuf_6pn(SEBY_VBD_TO_CBDp, RUNA_VBUS_D1n);
    /* p25.RYBU*/ top.cpu_bus.BUS_CPU_D2p = tribuf_6pn(SEBY_VBD_TO_CBDp, RONA_VBUS_D2n);
    /* p25.RAJU*/ top.cpu_bus.BUS_CPU_D3p = tribuf_6pn(SEBY_VBD_TO_CBDp, RUNO_VBUS_D3n);
    /* p25.TYJA*/ top.cpu_bus.BUS_CPU_D4p = tribuf_6pn(SEBY_VBD_TO_CBDp, SANA_VBUS_D4n);
    /* p25.REXU*/ top.cpu_bus.BUS_CPU_D5p = tribuf_6pn(SEBY_VBD_TO_CBDp, RORO_VBUS_D5n);
    /* p25.RUPY*/ top.cpu_bus.BUS_CPU_D6p = tribuf_6pn(SEBY_VBD_TO_CBDp, RABO_VBUS_D6n);
    /* p25.TOKU*/ top.cpu_bus.BUS_CPU_D7p = tribuf_6pn(SEBY_VBD_TO_CBDp, SAME_VBUS_D7n);
  }

  // VBD -> tile pix temp
  {
    /*#p32.LOMA*/ wire LOMA_LATCH_TILE_DAn = not1(top.tile_fetcher.METE_LATCH_TILE_DAp);
    /*p32.LEGU*/ LEGU_TILE_DA0n = dff8_A_inv(LOMA_LATCH_TILE_DAn, BUS_VRAM_D0p.tp());
    /*p32.NUDU*/ NUDU_TILE_DA1n = dff8_A_inv(LOMA_LATCH_TILE_DAn, BUS_VRAM_D1p.tp());
    /*p32.MUKU*/ MUKU_TILE_DA2n = dff8_A_inv(LOMA_LATCH_TILE_DAn, BUS_VRAM_D2p.tp());
    /*p32.LUZO*/ LUZO_TILE_DA3n = dff8_A_inv(LOMA_LATCH_TILE_DAn, BUS_VRAM_D3p.tp());
    /*p32.MEGU*/ MEGU_TILE_DA4n = dff8_A_inv(LOMA_LATCH_TILE_DAn, BUS_VRAM_D4p.tp());
    /*p32.MYJY*/ MYJY_TILE_DA5n = dff8_A_inv(LOMA_LATCH_TILE_DAn, BUS_VRAM_D5p.tp());
    /*p32.NASA*/ NASA_TILE_DA6n = dff8_A_inv(LOMA_LATCH_TILE_DAn, BUS_VRAM_D6p.tp());
    /*p32.NEFO*/ NEFO_TILE_DA7n = dff8_A_inv(LOMA_LATCH_TILE_DAn, BUS_VRAM_D7p.tp());

    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /*p??.LUVE*/ wire LUVE_MATCH_TILE_DBp = not1(top.tile_fetcher.LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /*p32.LABU*/ wire LABU_LATCH_TILE_DBn = not1(LUVE_MATCH_TILE_DBp);
    /*p32.RAWU*/ RAWU_TILE_DB0n = dff11_A_inv(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D0p.tp());
    /*p32.POZO*/ POZO_TILE_DB1n = dff11_A_inv(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D1p.tp());
    /*p32.PYZO*/ PYZO_TILE_DB2n = dff11_A_inv(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D2p.tp());
    /*p32.POXA*/ POXA_TILE_DB3n = dff11_A_inv(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D3p.tp());
    /*p32.PULO*/ PULO_TILE_DB4n = dff11_A_inv(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D4p.tp());
    /*p32.POJU*/ POJU_TILE_DB5n = dff11_A_inv(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D5p.tp());
    /*p32.POWY*/ POWY_TILE_DB6n = dff11_A_inv(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D6p.tp());
    /*p32.PYJU*/ PYJU_TILE_DB7n = dff11_A_inv(LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D7p.tp());
  }

  // VRAM tri -> sprite pix temp + x flip
  {
    /*#p29.XONO*/ wire XONO_FLIP_X = and2(top.oam_bus.BAXO_OAM_DB5p.qp(), top.sprite_fetcher.TEXY_SPR_READ_VRAMp);

    /*p33.PUTE*/ wire PUTE_FLIP0n = mux2_p2(XONO_FLIP_X, BUS_VRAM_D7p.tp(), BUS_VRAM_D0p.tp());
    /*p33.PELO*/ wire PELO_FLIP1n = mux2_p2(XONO_FLIP_X, BUS_VRAM_D6p.tp(), BUS_VRAM_D1p.tp());
    /*p33.PONO*/ wire PONO_FLIP2n = mux2_p2(XONO_FLIP_X, BUS_VRAM_D5p.tp(), BUS_VRAM_D2p.tp());
    /*p33.POBE*/ wire POBE_FLIP3n = mux2_p2(XONO_FLIP_X, BUS_VRAM_D4p.tp(), BUS_VRAM_D3p.tp());
    /*p33.PACY*/ wire PACY_FLIP4n = mux2_p2(XONO_FLIP_X, BUS_VRAM_D3p.tp(), BUS_VRAM_D4p.tp());
    /*p33.PUGU*/ wire PUGU_FLIP5n = mux2_p2(XONO_FLIP_X, BUS_VRAM_D2p.tp(), BUS_VRAM_D5p.tp());
    /*p33.PAWE*/ wire PAWE_FLIP6n = mux2_p2(XONO_FLIP_X, BUS_VRAM_D1p.tp(), BUS_VRAM_D6p.tp());
    /*p33.PULY*/ wire PULY_FLIP7n = mux2_p2(XONO_FLIP_X, BUS_VRAM_D0p.tp(), BUS_VRAM_D7p.tp());

    /*#p29.PEBY*/ wire PEBY_CLKp = not1(top.sprite_fetcher.RACA_LATCH_SPPIXB);
    /*#p29.NYBE*/ wire NYBE_CLKn = not1(PEBY_CLKp);
    /*#p29.PUCO*/ wire PUCO_CLKp = not1(NYBE_CLKn);

    /*p33.PEFO*/ PEFO_SPRITE_DB0n = dff8_A_inv(PUCO_CLKp, PUTE_FLIP0n);
    /*p33.ROKA*/ ROKA_SPRITE_DB1n = dff8_A_inv(PUCO_CLKp, PELO_FLIP1n);
    /*p33.MYTU*/ MYTU_SPRITE_DB2n = dff8_A_inv(PUCO_CLKp, PONO_FLIP2n);
    /*p33.RAMU*/ RAMU_SPRITE_DB3n = dff8_A_inv(PUCO_CLKp, POBE_FLIP3n);
    /*p33.SELE*/ SELE_SPRITE_DB4n = dff8_A_inv(PUCO_CLKp, PACY_FLIP4n);
    /*p33.SUTO*/ SUTO_SPRITE_DB5n = dff8_A_inv(PUCO_CLKp, PUGU_FLIP5n);
    /*p33.RAMA*/ RAMA_SPRITE_DB6n = dff8_A_inv(PUCO_CLKp, PAWE_FLIP6n);
    /*p33.RYDU*/ RYDU_SPRITE_DB7n = dff8_A_inv(PUCO_CLKp, PULY_FLIP7n);

    /*#p29.VYWA*/ wire VYWA_CLKp = not1(top.sprite_fetcher.TOPU_LATCH_SPPIXA);
    /*#p29.WENY*/ wire WENY_CLKn = not1(VYWA_CLKp);
    /*#p29.XADO*/ wire XADO_CLKp = not1(WENY_CLKn);

    /*p33.REWO*/ REWO_SPRITE_DA0n = dff8_A_inv(XADO_CLKp, PUTE_FLIP0n);
    /*p33.PEBA*/ PEBA_SPRITE_DA1n = dff8_A_inv(XADO_CLKp, PELO_FLIP1n);
    /*p33.MOFO*/ MOFO_SPRITE_DA2n = dff8_A_inv(XADO_CLKp, PONO_FLIP2n);
    /*p33.PUDU*/ PUDU_SPRITE_DA3n = dff8_A_inv(XADO_CLKp, POBE_FLIP3n);
    /*p33.SAJA*/ SAJA_SPRITE_DA4n = dff8_A_inv(XADO_CLKp, PACY_FLIP4n);
    /*p33.SUNY*/ SUNY_SPRITE_DA5n = dff8_A_inv(XADO_CLKp, PUGU_FLIP5n);
    /*p33.SEMO*/ SEMO_SPRITE_DA6n = dff8_A_inv(XADO_CLKp, PAWE_FLIP6n);
    /*p33.SEGA*/ SEGA_SPRITE_DA7n = dff8_A_inv(XADO_CLKp, PULY_FLIP7n);
  }
}

//------------------------------------------------------------------------------