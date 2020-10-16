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

  int TILE_DA = pack_p(LEGU_TILE_DA0n.qn07(), NUDU_TILE_DA1n.qn07(), MUKU_TILE_DA2n.qn07(), LUZO_TILE_DA3n.qn07(),
                       MEGU_TILE_DA4n.qn07(), MYJY_TILE_DA5n.qn07(), NASA_TILE_DA6n.qn07(), NEFO_TILE_DA7n.qn07());
  int TILE_DB = pack_p(RAWU_TILE_DB0p.q11p(), POZO_TILE_DB1p.q11p(), PYZO_TILE_DB2p.q11p(), POXA_TILE_DB3p.q11p(),
                       PULO_TILE_DB4p.q11p(), POJU_TILE_DB5p.q11p(), POWY_TILE_DB6p.q11p(), PYJU_TILE_DB7p.q11p());
  int SPRITE_DA = pack_p(PEFO_SPRITE_DB0n.qn07(), ROKA_SPRITE_DB1n.qn07(), MYTU_SPRITE_DB2n.qn07(), RAMU_SPRITE_DB3n.qn07(),
                         SELE_SPRITE_DB4n.qn07(), SUTO_SPRITE_DB5n.qn07(), RAMA_SPRITE_DB6n.qn07(), RYDU_SPRITE_DB7n.qn07());
  int SPRITE_DB = pack_p(REWO_SPRITE_DA0n.qn07(), PEBA_SPRITE_DA1n.qn07(), MOFO_SPRITE_DA2n.qn07(), PUDU_SPRITE_DA3n.qn07(),
                         SAJA_SPRITE_DA4n.qn07(), SUNY_SPRITE_DA5n.qn07(), SEMO_SPRITE_DA6n.qn07(), SEGA_SPRITE_DA7n.qn07());

  d("TILE_DA       : 0x%02x\n", TILE_DA);
  d("TILE_DB       : 0x%02x\n", TILE_DB);
  d("SPRITE_DA     : 0x%02x\n", SPRITE_DA);
  d("SPRITE_DB     : 0x%02x\n", SPRITE_DB);
  d("\n");
}

//------------------------------------------------------------------------------

void VramBus::tock(SchematicTop& top) {
  wire VYPO = 1;

  /* p25.TEFA*/ wire _TEFA_8000_9FFFp = nor2(top.cpu_bus.SYRO_FE00_FFFFp(), top.TEXO_8000_9FFFn);
  /* p25.SOSE*/ wire _SOSE_8000_9FFFp = and2(top.cpu_bus.BUS_CPU_A15.qp(), _TEFA_8000_9FFFp);

  /* p25.ROPY*/ wire _ROPY_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGn.qn03());

  /* p25.TEFY*/ wire _TEFY_VRAM_MCSp = not1(PIN_VRAM_CSn.qn());
  /* p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and2(_SOSE_8000_9FFFp, top.ABUZ_AVn);
  /* p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2p(top.TUTO_DBG_VRAMp, _TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp);
  /* p25.SERE*/ wire _SERE_CPU_VRM_RDp = and2(_TOLE_VRAM_RDp, _ROPY_RENDERINGn);

  /* p25.TAVY*/ wire _TAVY_MOEp = not1(PIN_VRAM_OEn.qn());
  /* p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = nand2(_SOSE_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.qp());  // Schematic wrong, second input is PIN_CPU_WRp
  /* p25.SALE*/ wire _SALE_CPU_VRAM_WRn = mux2p(top.TUTO_DBG_VRAMp, _TAVY_MOEp, _TEGU_CPU_VRAM_WRn);
  /* p25.RUVY*/ wire _RUVY_VRAM_WRp    = not1(_SALE_CPU_VRAM_WRn);
  /* p25.SAZO*/ wire _SAZO_CBD_TO_VPDp = and2(_RUVY_VRAM_WRp, _SERE_CPU_VRM_RDp);
  /* p25.RYJE*/ wire _RYJE_CBD_TO_VPDn = not1(_SAZO_CBD_TO_VPDp);
  /* p25.REVO*/ wire _REVO_CBD_TO_VPDp = not1(_RYJE_CBD_TO_VPDn);
  /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp = or2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
  /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn = not1(_RELA_CBD_TO_VPDp);

  /* p25.XANE*/ wire _XANE_VRAM_LOCKn = nor2(top.dma_reg.LUFA_DMA_VRAMp, top.pix_pipe.XYMU_RENDERINGn.qn03());

  /* p25.RACO*/ wire _RACO_DBG_VRAMn = not1(top.TUTO_DBG_VRAMp);

  //----------------------------------------
  // VRAM pins

  {
    /*p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp = and2(_SOSE_8000_9FFFp, top.APOV_CPU_WRp_xxxxEFGx);
    /*p25.SUDO*/ wire _SUDO_MWRp_C = not1(PIN_VRAM_WRn.qn());
    /*p25.TYJY*/ wire _TYJY_DBG_VRAM_WRp = mux2p(top.TUTO_DBG_VRAMp, _SUDO_MWRp_C, _TUJA_CPU_VRAM_WRp);

    /*p25.SOHY*/ wire _SOHY_MWRn = nand2(_TYJY_DBG_VRAM_WRp, _SERE_CPU_VRM_RDp);

    /*p25.TAXY*/ wire _TAXY_MWRn_A = and2(_SOHY_MWRn, _RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire _SOFY_MWRn_D = or2(_SOHY_MWRn, top.TUTO_DBG_VRAMp);
    /*p25.SYSY*/ wire _SYSY_MWRp_A = not1(_TAXY_MWRn_A);
    /*p25.RAGU*/ wire _RAGU_MWRp_D = not1(_SOFY_MWRn_D);

    PIN_VRAM_WRn.io_pin(_SYSY_MWRp_A, _RAGU_MWRp_D);
  }

  {
    /*p25.RYLU*/ wire _RYLU_CPU_VRAM_RDn = nand2(_SALE_CPU_VRAM_WRn, _XANE_VRAM_LOCKn);

    /*p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn = not1(top.sprite_fetcher.SOHO_SPR_VRAM_RDp);
    /*p25.APAM*/ wire _APAM_DMA_VRAM_RDn = not1(top.dma_reg.LUFA_DMA_VRAMp);
    /*p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn = not1(top.tile_fetcher.LONY_BG_FETCH_RUNNINGp.qp03());

    /*p25.RACU*/ wire _RACU_MOEn = and4(_RYLU_CPU_VRAM_RDn, _RAWA_SPR_VRAM_RDn, _MYMA_BGW_VRAM_RDn, _APAM_DMA_VRAM_RDn); // def and

    /*p25.SEMA*/ wire _SEMA_MOEn_A = and2(_RACU_MOEn, _RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire _RUTE_MOEn_D = or2(_RACU_MOEn, top.TUTO_DBG_VRAMp); // schematic wrong, second input is RACU
    /*p25.REFO*/ wire _REFO_MOEn_A = not1(_SEMA_MOEn_A);
    /*p25.SAHA*/ wire _SAHA_MOEn_D = not1(_RUTE_MOEn_D);

    PIN_VRAM_OEn.io_pin(_REFO_MOEn_A, _SAHA_MOEn_D);
  }

  {
    /*#p25.SUTU*/ wire _SUTU_MCSn = nor4(top.tile_fetcher.LENA_BGW_VRM_RDp,
                                         top.dma_reg.LUFA_DMA_VRAMp,
                                         top.sprite_fetcher.TEXY_SPR_READ_VRAMp,
                                         _SERE_CPU_VRM_RDp);

    /*p25.TODE*/ wire _TODE_MCSn_A = and2(_SUTU_MCSn, _RACO_DBG_VRAMn);
    /*p25.SEWO*/ wire _SEWO_MCSn_D = or2(_SUTU_MCSn, top.TUTO_DBG_VRAMp);
    /*p25.SOKY*/ wire _SOKY_MCSp_A = not1(_TODE_MCSn_A);
    /*p25.SETY*/ wire _SETY_MCSp_D = not1(_SEWO_MCSn_D);

    PIN_VRAM_CSn.io_pin(_SOKY_MCSp_A, _SETY_MCSp_D);
  }

  //----------------------------------------
  // VRAM address

  // CPU address -> vram address
  {
    /* p25.XEDU*/ wire _XEDU_CPU_VRAM_RDn = not1(_XANE_VRAM_LOCKn);
    /* p25.XAKY*/ BUS_VRAM_A00n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A00.qp());
    /* p25.XUXU*/ BUS_VRAM_A01n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A01.qp());
    /* p25.XYNE*/ BUS_VRAM_A02n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A02.qp());
    /* p25.XODY*/ BUS_VRAM_A03n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A03.qp());
    /* p25.XECA*/ BUS_VRAM_A04n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A04.qp());
    /* p25.XOBA*/ BUS_VRAM_A05n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A05.qp());
    /* p25.XOPO*/ BUS_VRAM_A06n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A06.qp());
    /* p25.XYBO*/ BUS_VRAM_A07n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A07.qp());
    /* p25.RYSU*/ BUS_VRAM_A08n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A08.qp());
    /* p25.RESE*/ BUS_VRAM_A09n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A09.qp());
    /* p25.RUSE*/ BUS_VRAM_A10n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A10.qp());
    /* p25.RYNA*/ BUS_VRAM_A11n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A11.qp());
    /* p25.RUMO*/ BUS_VRAM_A12n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A12.qp());
  }

  // DMA address -> vram address
  {
    /* p04.AHOC*/ wire _AHOC_DMA_VRAM_RDn = not1(top.dma_reg.LUFA_DMA_VRAMp);
    /* p04.ECAL*/ BUS_VRAM_A00n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp17());
    /* p04.EGEZ*/ BUS_VRAM_A01n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp17());
    /* p04.FUHE*/ BUS_VRAM_A02n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp17());
    /* p04.FYZY*/ BUS_VRAM_A03n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp17());
    /* p04.DAMU*/ BUS_VRAM_A04n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp17());
    /* p04.DAVA*/ BUS_VRAM_A05n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp17());
    /* p04.ETEG*/ BUS_VRAM_A06n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp17());
    /*#p04.EREW*/ BUS_VRAM_A07n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp17());
    /*#p04.EVAX*/ BUS_VRAM_A08n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NAFA_DMA_A08n.qn07());
    /* p04.DUVE*/ BUS_VRAM_A09n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.PYNE_DMA_A09n.qn07());
    /* p04.ERAF*/ BUS_VRAM_A10n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.PARA_DMA_A10n.qn07());
    /* p04.FUSY*/ BUS_VRAM_A11n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NYDO_DMA_A11n.qn07());
    /* p04.EXYF*/ BUS_VRAM_A12n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NYGY_DMA_A12n.qn07());
  }

  // Sprite fetcher read
  {
    /*#p29.WUKY*/ wire _WUKY_FLIP_Yp = not1(top.oam_bus.YZOS_OAM_DB6p.qp08());
    /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not1(top.pix_pipe.XYMO_LCDC_SPSIZEn.qn08());

    /*#p29.CYVU*/ wire _CYVU_L0 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L0.qp());
    /*#p29.BORE*/ wire _BORE_L1 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L1.qp());
    /*#p29.BUVY*/ wire _BUVY_L2 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L2.qp());
    /*#p29.WAGO*/ wire _WAGO_L3 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L3.qp());
    /*#p29.GEJY*/ wire _GEJY_L3 = amux2(top.oam_bus.XUSO_OAM_DA0p.qp08(), _FUFO_LCDC_SPSIZEn,
                                        top.pix_pipe.XYMO_LCDC_SPSIZEn.qn08(), _WAGO_L3);

    /* p29.ABEM*/ BUS_VRAM_A00n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_fetcher.XUQU_SPRITE_AB);
    /* p29.BAXE*/ BUS_VRAM_A01n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, _CYVU_L0);
    /* p29.ARAS*/ BUS_VRAM_A02n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, _BORE_L1);
    /* p29.AGAG*/ BUS_VRAM_A03n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, _BUVY_L2);
    /* p29.FAMU*/ BUS_VRAM_A04n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, _GEJY_L3);
    /*#p29.FUGY*/ BUS_VRAM_A05n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.XEGU_OAM_DA1p.qp08());
    /* p29.GAVO*/ BUS_VRAM_A06n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.YJEX_OAM_DA2p.qp08());
    /* p29.WYGA*/ BUS_VRAM_A07n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.XYJU_OAM_DA3p.qp08());
    /* p29.WUNE*/ BUS_VRAM_A08n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.YBOG_OAM_DA4p.qp08());
    /* p29.GOTU*/ BUS_VRAM_A09n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.WYSO_OAM_DA5p.qp08());
    /* p29.GEGU*/ BUS_VRAM_A10n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.XOTE_OAM_DA6p.qp08());
    /* p29.XEHE*/ BUS_VRAM_A11n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.YZAB_OAM_DA7p.qp08());
    /* p29.DYSO*/ BUS_VRAM_A12n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, 0);   // sprites always in low half of tile store
  }

  {
    // Map scroll adder
#pragma warning(push)
#pragma warning(disable:4189)
    /*#p26.FAFO*/ wire _FAFO_TILE_Y0S = add_s(top.lcd_reg.MUWY_Y0p.qp17(), top.pix_pipe.GAVE_SCY0n.qn08(), 0);
    /*#p26.FAFO*/ wire _FAFO_TILE_Y0C = add_c(top.lcd_reg.MUWY_Y0p.qp17(), top.pix_pipe.GAVE_SCY0n.qn08(), 0);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1S = add_s(top.lcd_reg.MYRO_Y1p.qp17(), top.pix_pipe.FYMO_SCY1n.qn08(), _FAFO_TILE_Y0C);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1C = add_c(top.lcd_reg.MYRO_Y1p.qp17(), top.pix_pipe.FYMO_SCY1n.qn08(), _FAFO_TILE_Y0C);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2S = add_s(top.lcd_reg.LEXA_Y2p.qp17(), top.pix_pipe.FEZU_SCY2n.qn08(), _EMUX_TILE_Y1C);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2C = add_c(top.lcd_reg.LEXA_Y2p.qp17(), top.pix_pipe.FEZU_SCY2n.qn08(), _EMUX_TILE_Y1C);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0S  = add_s(top.lcd_reg.LYDO_Y3p.qp17(), top.pix_pipe.FUJO_SCY3n.qn08(), _ECAB_TILE_Y2C);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0C  = add_c(top.lcd_reg.LYDO_Y3p.qp17(), top.pix_pipe.FUJO_SCY3n.qn08(), _ECAB_TILE_Y2C);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1S  = add_s(top.lcd_reg.LOVU_Y4p.qp17(), top.pix_pipe.DEDE_SCY4n.qn08(), _ETAM_MAP_Y0C);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1C  = add_c(top.lcd_reg.LOVU_Y4p.qp17(), top.pix_pipe.DEDE_SCY4n.qn08(), _ETAM_MAP_Y0C);
    /* p26.DABA*/ wire _DABA_MAP_Y2S  = add_s(top.lcd_reg.LEMA_Y5p.qp17(), top.pix_pipe.FOTY_SCY5n.qn08(), _DOTO_MAP_Y1C);
    /* p26.DABA*/ wire _DABA_MAP_Y2C  = add_c(top.lcd_reg.LEMA_Y5p.qp17(), top.pix_pipe.FOTY_SCY5n.qn08(), _DOTO_MAP_Y1C);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3S  = add_s(top.lcd_reg.MATO_Y6p.qp17(), top.pix_pipe.FOHA_SCY6n.qn08(), _DABA_MAP_Y2C);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3C  = add_c(top.lcd_reg.MATO_Y6p.qp17(), top.pix_pipe.FOHA_SCY6n.qn08(), _DABA_MAP_Y2C);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4S  = add_s(top.lcd_reg.LAFO_Y7p.qp17(), top.pix_pipe.FUNY_SCY7n.qn08(), _EFYK_MAP_Y3C);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4C  = add_c(top.lcd_reg.LAFO_Y7p.qp17(), top.pix_pipe.FUNY_SCY7n.qn08(), _EFYK_MAP_Y3C);

    /*#p26.ATAD*/ wire _ATAD_TILE_X0S = add_s(top.pix_pipe.XEHO_X0p.qp17(), top.pix_pipe.DATY_SCX0n.qn08(), 0);
    /*#p26.ATAD*/ wire _ATAD_TILE_X0C = add_c(top.pix_pipe.XEHO_X0p.qp17(), top.pix_pipe.DATY_SCX0n.qn08(), 0);
    /* p26.BEHU*/ wire _BEHU_TILE_X1S = add_s(top.pix_pipe.SAVY_X1p.qp17(), top.pix_pipe.DUZU_SCX1n.qn08(), _ATAD_TILE_X0C);
    /* p26.BEHU*/ wire _BEHU_TILE_X1C = add_c(top.pix_pipe.SAVY_X1p.qp17(), top.pix_pipe.DUZU_SCX1n.qn08(), _ATAD_TILE_X0C);
    /* p26.APYH*/ wire _APYH_TILE_X2S = add_s(top.pix_pipe.XODU_X2p.qp17(), top.pix_pipe.CYXU_SCX2n.qn08(), _BEHU_TILE_X1C);
    /* p26.APYH*/ wire _APYH_TILE_X2C = add_c(top.pix_pipe.XODU_X2p.qp17(), top.pix_pipe.CYXU_SCX2n.qn08(), _BEHU_TILE_X1C);
    /* p26.BABE*/ wire _BABE_MAP_X0S  = add_s(top.pix_pipe.XYDO_X3p.qp17(), top.pix_pipe.GUBO_SCX3n.qn08(), _APYH_TILE_X2C);
    /* p26.BABE*/ wire _BABE_MAP_X0C  = add_c(top.pix_pipe.XYDO_X3p.qp17(), top.pix_pipe.GUBO_SCX3n.qn08(), _APYH_TILE_X2C);
    /* p26.ABOD*/ wire _ABOD_MAP_X1S  = add_s(top.pix_pipe.TUHU_X4p.qp17(), top.pix_pipe.BEMY_SCX4n.qn08(), _BABE_MAP_X0C);
    /* p26.ABOD*/ wire _ABOD_MAP_X1C  = add_c(top.pix_pipe.TUHU_X4p.qp17(), top.pix_pipe.BEMY_SCX4n.qn08(), _BABE_MAP_X0C);
    /* p26.BEWY*/ wire _BEWY_MAP_X2S  = add_s(top.pix_pipe.TUKY_X5p.qp17(), top.pix_pipe.CUZY_SCX5n.qn08(), _ABOD_MAP_X1C);
    /* p26.BEWY*/ wire _BEWY_MAP_X2C  = add_c(top.pix_pipe.TUKY_X5p.qp17(), top.pix_pipe.CUZY_SCX5n.qn08(), _ABOD_MAP_X1C);
    /* p26.BYCA*/ wire _BYCA_MAP_X3S  = add_s(top.pix_pipe.TAKO_X6p.qp17(), top.pix_pipe.CABU_SCX6n.qn08(), _BEWY_MAP_X2C);
    /* p26.BYCA*/ wire _BYCA_MAP_X3C  = add_c(top.pix_pipe.TAKO_X6p.qp17(), top.pix_pipe.CABU_SCX6n.qn08(), _BEWY_MAP_X2C);
    /* p26.ACUL*/ wire _ACUL_MAP_X4S  = add_s(top.pix_pipe.SYBE_X7p.qp17(), top.pix_pipe.BAKE_SCX7n.qn08(), _BYCA_MAP_X3C);
    /* p26.ACUL*/ wire _ACUL_MAP_X4C  = add_c(top.pix_pipe.SYBE_X7p.qp17(), top.pix_pipe.BAKE_SCX7n.qn08(), _BYCA_MAP_X3C);
#pragma warning(pop)

    // Background map read
    {
      /* p26.AXEP*/ BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _BABE_MAP_X0S);
      /* p26.AFEB*/ BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _ABOD_MAP_X1S);
      /* p26.ALEL*/ BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _BEWY_MAP_X2S);
      /* p26.COLY*/ BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _BYCA_MAP_X3S);
      /* p26.AJAN*/ BUS_VRAM_A04n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _ACUL_MAP_X4S);
      /* p26.DUHO*/ BUS_VRAM_A05n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _ETAM_MAP_Y0S);
      /* p26.CASE*/ BUS_VRAM_A06n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _DOTO_MAP_Y1S);
      /* p26.CYPO*/ BUS_VRAM_A07n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _DABA_MAP_Y2S);
      /* p26.CETA*/ BUS_VRAM_A08n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _EFYK_MAP_Y3S);
      /* p26.DAFE*/ BUS_VRAM_A09n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _EJOK_MAP_Y4S);
      /*#p26.AMUV*/ BUS_VRAM_A10n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, top.pix_pipe.XAFO_LCDC_BGMAPn.qn08());
      /* p26.COVE*/ BUS_VRAM_A11n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, 1);
      /* p26.COXO*/ BUS_VRAM_A12n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, 1);
    }

    // Window map read
    {
      /*#p27.XEJA*/ BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WYKA_WIN_X3.qp17());
      /* p27.XAMO*/ BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WODY_WIN_X4.qp17());
      /* p27.XAHE*/ BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WOBO_WIN_X5.qp17());
      /* p27.XULO*/ BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WYKO_WIN_X6.qp17());
      /* p27.WUJU*/ BUS_VRAM_A04n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.XOLO_WIN_X7.qp17());
      /*#p27.VYTO*/ BUS_VRAM_A05n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TUFU_WIN_Y3.qp17());
      /* p27.VEHA*/ BUS_VRAM_A06n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TAXA_WIN_Y4.qp17());
      /* p27.VACE*/ BUS_VRAM_A07n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TOZO_WIN_Y5.qp17());
      /* p27.VOVO*/ BUS_VRAM_A08n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TATE_WIN_Y6.qp17());
      /* p27.VULO*/ BUS_VRAM_A09n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TEKE_WIN_Y7.qp17());
      /*#p27.VEVY*/ BUS_VRAM_A10n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WOKY_LCDC_WINMAPn.qn08());
      /* p27.VEZA*/ BUS_VRAM_A11n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, 1);
      /* p27.VOGU*/ BUS_VRAM_A12n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, 1);
    }

    // Background/window tile read
    {
      /*#p26.ASUM*/ BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  top.tile_fetcher.XUHA_FETCH_S2p);
      /* p26.EVAD*/ BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  _FAFO_TILE_Y0S);
      /* p26.DAHU*/ BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  _EMUX_TILE_Y1S);
      /* p26.DODE*/ BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  _ECAB_TILE_Y2S); // check outputs of ECAB

      /*#p25.XONU*/ BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.tile_fetcher.XUHA_FETCH_S2p);
      /*#p25.WUDO*/ BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VYNO_WIN_Y0.qp17());
      /*#p25.WAWE*/ BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VUJO_WIN_Y1.qp17());
      /*#p25.WOLU*/ BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VYMU_WIN_Y2.qp17());

      /*#p25.VUZA*/ wire _VUZA_TILE_BANKp = nor2(PYJU_TILE_DB7p.q11p(), top.pix_pipe.WEXU_LCDC_BGTILEn.qn08());

      /*#p25.VAPY*/ BUS_VRAM_A04n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     RAWU_TILE_DB0p.q11p());
      /*#p25.SEZU*/ BUS_VRAM_A05n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     POZO_TILE_DB1p.q11p());
      /*#p25.VEJY*/ BUS_VRAM_A06n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     PYZO_TILE_DB2p.q11p());
      /*#p25.RUSA*/ BUS_VRAM_A07n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     POXA_TILE_DB3p.q11p());
      /*#p25.ROHA*/ BUS_VRAM_A08n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     PULO_TILE_DB4p.q11p());
      /*#p25.RESO*/ BUS_VRAM_A09n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     POJU_TILE_DB5p.q11p());
      /*#p25.SUVO*/ BUS_VRAM_A10n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     POWY_TILE_DB6p.q11p());
      /*#p25.TOBO*/ BUS_VRAM_A11n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     PYJU_TILE_DB7p.q11p());
      /*#p25.VURY*/ BUS_VRAM_A12n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     _VUZA_TILE_BANKp);
    }
  }

  // VRAM addr bus -> VRAM addr pin
  {
    /*p25.MYFU*/ wire _MYFUp = not1(BUS_VRAM_A00n.qp());
    /*p25.MASA*/ wire _MASAp = not1(BUS_VRAM_A01n.qp());
    /*p25.MYRE*/ wire _MYREp = not1(BUS_VRAM_A02n.qp());
    /*p25.MAVU*/ wire _MAVUp = not1(BUS_VRAM_A03n.qp());
    /*p25.MEPA*/ wire _MEPAp = not1(BUS_VRAM_A04n.qp());
    /*p25.MYSA*/ wire _MYSAp = not1(BUS_VRAM_A05n.qp());
    /*p25.MEWY*/ wire _MEWYp = not1(BUS_VRAM_A06n.qp());
    /*p25.MUME*/ wire _MUMEp = not1(BUS_VRAM_A07n.qp());
    /*p25.VOVA*/ wire _VOVAp = not1(BUS_VRAM_A08n.qp());
    /*p25.VODE*/ wire _VODEp = not1(BUS_VRAM_A09n.qp());
    /*p25.RUKY*/ wire _RUKYp = not1(BUS_VRAM_A10n.qp());
    /*p25.RUMA*/ wire _RUMAp = not1(BUS_VRAM_A11n.qp());
    /*p25.REHO*/ wire _REHOp = not1(BUS_VRAM_A12n.qp());

    /*p25.LEXE*/ wire _LEXEn = not1(_MYFUp);
    /*p25.LOZU*/ wire _LOZUn = not1(_MASAp);
    /*p25.LACA*/ wire _LACAn = not1(_MYREp);
    /*p25.LUVO*/ wire _LUVOn = not1(_MAVUp);
    /*p25.LOLY*/ wire _LOLYn = not1(_MEPAp);
    /*p25.LALO*/ wire _LALOn = not1(_MYSAp);
    /*p25.LEFA*/ wire _LEFAn = not1(_MEWYp);
    /*p25.LUBY*/ wire _LUBYn = not1(_MUMEp);
    /*p25.TUJY*/ wire _TUJYn = not1(_VOVAp);
    /*p25.TAGO*/ wire _TAGOn = not1(_VODEp);
    /*p25.NUVA*/ wire _NUVAn = not1(_RUKYp);
    /*p25.PEDU*/ wire _PEDUn = not1(_RUMAp);
    /*p25.PONY*/ wire _PONYn = not1(_REHOp);

    PIN_VRAM_A00p.io_pin(_LEXEn, _LEXEn);
    PIN_VRAM_A01p.io_pin(_LOZUn, _LOZUn);
    PIN_VRAM_A02p.io_pin(_LACAn, _LACAn);
    PIN_VRAM_A03p.io_pin(_LUVOn, _LUVOn);
    PIN_VRAM_A04p.io_pin(_LOLYn, _LOLYn);
    PIN_VRAM_A05p.io_pin(_LALOn, _LALOn);
    PIN_VRAM_A06p.io_pin(_LEFAn, _LEFAn);
    PIN_VRAM_A07p.io_pin(_LUBYn, _LUBYn);
    PIN_VRAM_A08p.io_pin(_TUJYn, _TUJYn);
    PIN_VRAM_A09p.io_pin(_TAGOn, _TAGOn);
    PIN_VRAM_A10p.io_pin(_NUVAn, _NUVAn);
    PIN_VRAM_A11p.io_pin(_PEDUn, _PEDUn);
    PIN_VRAM_A12p.io_pin(_PONYn, _PONYn);
  }

  //----------------------------------------
  // VRAM data out

  // CBD->VBD->VPD
  {
    /*p25.ROCY*/ wire _ROCY_CBD_TO_VPDp = and2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
    /*p25.RAHU*/ wire _RAHU_CBD_TO_VPDn = not1(_ROCY_CBD_TO_VPDp);

    /* p25.TEME*/ BUS_VRAM_D0p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /* p25.TEWU*/ BUS_VRAM_D1p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*#p25.TYGO*/ BUS_VRAM_D2p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /* p25.SOTE*/ BUS_VRAM_D3p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /* p25.SEKE*/ BUS_VRAM_D4p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /* p25.RUJO*/ BUS_VRAM_D5p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /* p25.TOFA*/ BUS_VRAM_D6p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /* p25.SUZA*/ BUS_VRAM_D7p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*#p25.ROVE*/ wire _ROVE_CBD_TO_VPDp = not1(_RAHU_CBD_TO_VPDn);
    /*#p25.SEFA*/ wire _SEFA_D0p = and2(BUS_VRAM_D0p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SOGO*/ wire _SOGO_D1p = and2(BUS_VRAM_D1p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SEFU*/ wire _SEFU_D2p = and2(BUS_VRAM_D2p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SUNA*/ wire _SUNA_D3p = and2(BUS_VRAM_D3p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SUMO*/ wire _SUMO_D4p = and2(BUS_VRAM_D4p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SAZU*/ wire _SAZU_D5p = and2(BUS_VRAM_D5p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SAMO*/ wire _SAMO_D6p = and2(BUS_VRAM_D6p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SUKE*/ wire _SUKE_D7p = and2(BUS_VRAM_D7p.qp(), _ROVE_CBD_TO_VPDp);

    /*#p25.REGE*/ wire _REGE_D0n = not1(_SEFA_D0p);
    /* p25.RYKY*/ wire _RYKY_D1n = not1(_SOGO_D1p);
    /* p25.RAZO*/ wire _RAZO_D2n = not1(_SEFU_D2p);
    /* p25.RADA*/ wire _RADA_D3n = not1(_SUNA_D3p);
    /* p25.RYRO*/ wire _RYRO_D4n = not1(_SUMO_D4p);
    /* p25.REVU*/ wire _REVU_D5n = not1(_SAZU_D5p);
    /* p25.REKU*/ wire _REKU_D6n = not1(_SAMO_D6p);
    /* p25.RYZE*/ wire _RYZE_D7n = not1(_SUKE_D7p);

    /*#p25.SYNU*/ wire _SYNU_D0p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_D0p.qp());
    /* p25.SYMA*/ wire _SYMA_D1p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_D1p.qp());
    /* p25.ROKO*/ wire _ROKO_D2p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_D2p.qp());
    /* p25.SYBU*/ wire _SYBU_D3p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_D3p.qp());
    /* p25.SAKO*/ wire _SAKO_D4p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_D4p.qp());
    /* p25.SEJY*/ wire _SEJY_D5p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_D5p.qp());
    /* p25.SEDO*/ wire _SEDO_D6p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_D6p.qp());
    /* p25.SAWU*/ wire _SAWU_D7p = or2(_RAHU_CBD_TO_VPDn, BUS_VRAM_D7p.qp());

    /*#p25.RURA*/ wire _RURA_D0n = not1(_SYNU_D0p);
    /* p25.RULY*/ wire _RULY_D1n = not1(_SYMA_D1p);
    /* p25.RARE*/ wire _RARE_D2n = not1(_ROKO_D2p);
    /* p25.RODU*/ wire _RODU_D3n = not1(_SYBU_D3p);
    /* p25.RUBE*/ wire _RUBE_D4n = not1(_SAKO_D4p);
    /* p25.RUMU*/ wire _RUMU_D5n = not1(_SEJY_D5p);
    /* p25.RYTY*/ wire _RYTY_D6n = not1(_SEDO_D6p);
    /* p25.RADY*/ wire _RADY_D7n = not1(_SAWU_D7p);

    /*#p25.ROFA*/ wire _ROFA_CBD_TO_VPDp = not1(_RENA_CBD_TO_VPDn);

    /*#*/ PIN_VRAM_D00p.io_pin(_REGE_D0n, _RURA_D0n, _ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D01p.io_pin(_RYKY_D1n, _RULY_D1n, _ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D02p.io_pin(_RAZO_D2n, _RARE_D2n, _ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D03p.io_pin(_RADA_D3n, _RODU_D3n, _ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D04p.io_pin(_RYRO_D4n, _RUBE_D4n, _ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D05p.io_pin(_REVU_D5n, _RUMU_D5n, _ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D06p.io_pin(_REKU_D6n, _RYTY_D6n, _ROFA_CBD_TO_VPDp);
    /* */ PIN_VRAM_D07p.io_pin(_RYZE_D7n, _RADY_D7n, _ROFA_CBD_TO_VPDp);
  }

  //----------------------------------------
  // VRAM data in

  // VPD -> VBD
  {
    /*p25.RODY*/ BUS_VRAM_D0p.tri_6pn(_RENA_CBD_TO_VPDn, PIN_VRAM_D00p.qn());
    /*p25.REBA*/ BUS_VRAM_D1p.tri_6pn(_RENA_CBD_TO_VPDn, PIN_VRAM_D01p.qn());
    /*p25.RYDO*/ BUS_VRAM_D2p.tri_6pn(_RENA_CBD_TO_VPDn, PIN_VRAM_D02p.qn());
    /*p25.REMO*/ BUS_VRAM_D3p.tri_6pn(_RENA_CBD_TO_VPDn, PIN_VRAM_D03p.qn());
    /*p25.ROCE*/ BUS_VRAM_D4p.tri_6pn(_RENA_CBD_TO_VPDn, PIN_VRAM_D04p.qn());
    /*p25.ROPU*/ BUS_VRAM_D5p.tri_6pn(_RENA_CBD_TO_VPDn, PIN_VRAM_D05p.qn());
    /*p25.RETA*/ BUS_VRAM_D6p.tri_6pn(_RENA_CBD_TO_VPDn, PIN_VRAM_D06p.qn());
    /*p25.RAKU*/ BUS_VRAM_D7p.tri_6pn(_RENA_CBD_TO_VPDn, PIN_VRAM_D07p.qn());
  }

  // VBD -> CBD
  {
    /* p25.TYVY*/ wire _TYVY_VBD_TO_CBDn = nand2(_SERE_CPU_VRM_RDp, top.LEKO_CPU_RDp);
    /* p25.SEBY*/ wire _SEBY_VBD_TO_CBDp = not1(_TYVY_VBD_TO_CBDn);

    /* p25.RERY*/ wire _RERY_VBUS_D0n = not1(BUS_VRAM_D0p.qp());
    /* p25.RUNA*/ wire _RUNA_VBUS_D1n = not1(BUS_VRAM_D1p.qp());
    /* p25.RONA*/ wire _RONA_VBUS_D2n = not1(BUS_VRAM_D2p.qp());
    /* p25.RUNO*/ wire _RUNO_VBUS_D3n = not1(BUS_VRAM_D3p.qp());
    /* p25.SANA*/ wire _SANA_VBUS_D4n = not1(BUS_VRAM_D4p.qp());
    /* p25.RORO*/ wire _RORO_VBUS_D5n = not1(BUS_VRAM_D5p.qp());
    /* p25.RABO*/ wire _RABO_VBUS_D6n = not1(BUS_VRAM_D6p.qp());
    /* p25.SAME*/ wire _SAME_VBUS_D7n = not1(BUS_VRAM_D7p.qp());

    /*#p25.RUGA*/ top.cpu_bus.BUS_CPU_D0p.tri_6pn(_SEBY_VBD_TO_CBDp, _RERY_VBUS_D0n);
    /* p25.ROTA*/ top.cpu_bus.BUS_CPU_D1p.tri_6pn(_SEBY_VBD_TO_CBDp, _RUNA_VBUS_D1n);
    /* p25.RYBU*/ top.cpu_bus.BUS_CPU_D2p.tri_6pn(_SEBY_VBD_TO_CBDp, _RONA_VBUS_D2n);
    /* p25.RAJU*/ top.cpu_bus.BUS_CPU_D3p.tri_6pn(_SEBY_VBD_TO_CBDp, _RUNO_VBUS_D3n);
    /* p25.TYJA*/ top.cpu_bus.BUS_CPU_D4p.tri_6pn(_SEBY_VBD_TO_CBDp, _SANA_VBUS_D4n);
    /* p25.REXU*/ top.cpu_bus.BUS_CPU_D5p.tri_6pn(_SEBY_VBD_TO_CBDp, _RORO_VBUS_D5n);
    /* p25.RUPY*/ top.cpu_bus.BUS_CPU_D6p.tri_6pn(_SEBY_VBD_TO_CBDp, _RABO_VBUS_D6n);
    /* p25.TOKU*/ top.cpu_bus.BUS_CPU_D7p.tri_6pn(_SEBY_VBD_TO_CBDp, _SAME_VBUS_D7n);
  }

  // VBD -> tile pix temp
  {
    /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn = not1(top.tile_fetcher.METE_LATCH_TILE_DAp);

    /*p32.LEGU*/ LEGU_TILE_DA0n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_D0p.qp());
    /*p32.NUDU*/ NUDU_TILE_DA1n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_D1p.qp());
    /*p32.MUKU*/ MUKU_TILE_DA2n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_D2p.qp());
    /*p32.LUZO*/ LUZO_TILE_DA3n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_D3p.qp());
    /*p32.MEGU*/ MEGU_TILE_DA4n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_D4p.qp());
    /*p32.MYJY*/ MYJY_TILE_DA5n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_D5p.qp());
    /*p32.NASA*/ NASA_TILE_DA6n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_D6p.qp());
    /*p32.NEFO*/ NEFO_TILE_DA7n.dff8p(_LOMA_LATCH_TILE_DAn, BUS_VRAM_D7p.qp());

    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /*p??.LUVE*/ wire _LUVE_MATCH_TILE_DBp = not1(top.tile_fetcher.LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /*p32.LABU*/ wire _LABU_LATCH_TILE_DBn = not1(_LUVE_MATCH_TILE_DBp);

    /*p32.RAWU*/ RAWU_TILE_DB0p.dff11(_LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D0p.qp());
    /*p32.POZO*/ POZO_TILE_DB1p.dff11(_LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D1p.qp());
    /*p32.PYZO*/ PYZO_TILE_DB2p.dff11(_LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D2p.qp());
    /*p32.POXA*/ POXA_TILE_DB3p.dff11(_LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D3p.qp());
    /*p32.PULO*/ PULO_TILE_DB4p.dff11(_LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D4p.qp());
    /*p32.POJU*/ POJU_TILE_DB5p.dff11(_LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D5p.qp());
    /*p32.POWY*/ POWY_TILE_DB6p.dff11(_LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D6p.qp());
    /*p32.PYJU*/ PYJU_TILE_DB7p.dff11(_LABU_LATCH_TILE_DBn, VYPO, BUS_VRAM_D7p.qp());
  }

  // VRAM tri -> sprite pix temp + x flip
  {
    /*#p29.XONO*/ wire _XONO_FLIP_X = and2(top.oam_bus.BAXO_OAM_DB5p.qp08(), top.sprite_fetcher.TEXY_SPR_READ_VRAMp);

    /*p33.PUTE*/ wire _PUTE_FLIP0p = mux2p(_XONO_FLIP_X, BUS_VRAM_D7p.qp(), BUS_VRAM_D0p.qp());
    /*p33.PELO*/ wire _PELO_FLIP1p = mux2p(_XONO_FLIP_X, BUS_VRAM_D6p.qp(), BUS_VRAM_D1p.qp());
    /*p33.PONO*/ wire _PONO_FLIP2p = mux2p(_XONO_FLIP_X, BUS_VRAM_D5p.qp(), BUS_VRAM_D2p.qp());
    /*p33.POBE*/ wire _POBE_FLIP3p = mux2p(_XONO_FLIP_X, BUS_VRAM_D4p.qp(), BUS_VRAM_D3p.qp());
    /*p33.PACY*/ wire _PACY_FLIP4p = mux2p(_XONO_FLIP_X, BUS_VRAM_D3p.qp(), BUS_VRAM_D4p.qp());
    /*p33.PUGU*/ wire _PUGU_FLIP5p = mux2p(_XONO_FLIP_X, BUS_VRAM_D2p.qp(), BUS_VRAM_D5p.qp());
    /*p33.PAWE*/ wire _PAWE_FLIP6p = mux2p(_XONO_FLIP_X, BUS_VRAM_D1p.qp(), BUS_VRAM_D6p.qp());
    /*p33.PULY*/ wire _PULY_FLIP7p = mux2p(_XONO_FLIP_X, BUS_VRAM_D0p.qp(), BUS_VRAM_D7p.qp());

    /*#p29.PEBY*/ wire _PEBY_CLKp = not1(top.sprite_fetcher.RACA_LATCH_SPPIXB);
    /*#p29.NYBE*/ wire _NYBE_CLKn = not1(_PEBY_CLKp);
    /*#p29.PUCO*/ wire _PUCO_CLKp = not1(_NYBE_CLKn);

    latch_sprite_a_delay.set(_PUCO_CLKp);

    /*p33.PEFO*/ PEFO_SPRITE_DB0n.dff8n(latch_sprite_a_delay.q6(), _PUTE_FLIP0p);
    /*p33.ROKA*/ ROKA_SPRITE_DB1n.dff8n(latch_sprite_a_delay.q6(), _PELO_FLIP1p);
    /*p33.MYTU*/ MYTU_SPRITE_DB2n.dff8n(latch_sprite_a_delay.q6(), _PONO_FLIP2p);
    /*p33.RAMU*/ RAMU_SPRITE_DB3n.dff8n(latch_sprite_a_delay.q6(), _POBE_FLIP3p);
    /*p33.SELE*/ SELE_SPRITE_DB4n.dff8n(latch_sprite_a_delay.q6(), _PACY_FLIP4p);
    /*p33.SUTO*/ SUTO_SPRITE_DB5n.dff8n(latch_sprite_a_delay.q6(), _PUGU_FLIP5p);
    /*p33.RAMA*/ RAMA_SPRITE_DB6n.dff8n(latch_sprite_a_delay.q6(), _PAWE_FLIP6p);
    /*p33.RYDU*/ RYDU_SPRITE_DB7n.dff8n(latch_sprite_a_delay.q6(), _PULY_FLIP7p);

    /*#p29.VYWA*/ wire _VYWA_CLKp = not1(top.sprite_fetcher.TOPU_LATCH_SPPIXA);
    /*#p29.WENY*/ wire _WENY_CLKn = not1(_VYWA_CLKp);
    /*#p29.XADO*/ wire _XADO_CLKp = not1(_WENY_CLKn);

    latch_sprite_b_delay.set(_XADO_CLKp);

    /*p33.REWO*/ REWO_SPRITE_DA0n.dff8n(latch_sprite_b_delay.q6(), _PUTE_FLIP0p);
    /*p33.PEBA*/ PEBA_SPRITE_DA1n.dff8n(latch_sprite_b_delay.q6(), _PELO_FLIP1p);
    /*p33.MOFO*/ MOFO_SPRITE_DA2n.dff8n(latch_sprite_b_delay.q6(), _PONO_FLIP2p);
    /*p33.PUDU*/ PUDU_SPRITE_DA3n.dff8n(latch_sprite_b_delay.q6(), _POBE_FLIP3p);
    /*p33.SAJA*/ SAJA_SPRITE_DA4n.dff8n(latch_sprite_b_delay.q6(), _PACY_FLIP4p);
    /*p33.SUNY*/ SUNY_SPRITE_DA5n.dff8n(latch_sprite_b_delay.q6(), _PUGU_FLIP5p);
    /*p33.SEMO*/ SEMO_SPRITE_DA6n.dff8n(latch_sprite_b_delay.q6(), _PAWE_FLIP6p);
    /*p33.SEGA*/ SEGA_SPRITE_DA7n.dff8n(latch_sprite_b_delay.q6(), _PULY_FLIP7p);
  }
}

//------------------------------------------------------------------------------
