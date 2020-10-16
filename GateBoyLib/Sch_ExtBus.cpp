#include "GateBoyLib/Sch_ExtBus.h"

#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void ExtBus::dump(Dumper& d) const {
  d("\002===== Ext Bus =====\001\n");
  d("PIN CLK    : %c\n", PIN_EXT_CLK.c());
  d("PIN RDn    : %c\n", PIN_EXT_RDn.c());
  d("PIN WRn    : %c\n", PIN_EXT_WRn.c());
  d("PIN CSn    : %c\n", PIN_EXT_CSn.c());
  d("\n");

  d("PIN ADDR   : %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    PIN_EXT_A15p.c(), PIN_EXT_A14p.c(), PIN_EXT_A13p.c(), PIN_EXT_A12p.c(),
    PIN_EXT_A11p.c(), PIN_EXT_A10p.c(), PIN_EXT_A09p.c(), PIN_EXT_A08p.c(),
    PIN_EXT_A07p.c(), PIN_EXT_A06p.c(), PIN_EXT_A05p.c(), PIN_EXT_A04p.c(),
    PIN_EXT_A03p.c(), PIN_EXT_A02p.c(), PIN_EXT_A01p.c(), PIN_EXT_A00p.c());
  d("PIN DATA   : %c%c%c%c%c%c%c%c\n",
    PIN_EXT_D07p.c(), PIN_EXT_D06p.c(), PIN_EXT_D05p.c(), PIN_EXT_D04p.c(),
    PIN_EXT_D03p.c(), PIN_EXT_D02p.c(), PIN_EXT_D01p.c(), PIN_EXT_D00p.c());
  d("\n");

  d("ADDR LATCH : _%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    NYRE_EXT_ADDR_LATCH_14p.c(), LONU_EXT_ADDR_LATCH_13p.c(), LOBU_EXT_ADDR_LATCH_12p.c(), LUMY_EXT_ADDR_LATCH_11p.c(),
    PATE_EXT_ADDR_LATCH_10p.c(), LYSA_EXT_ADDR_LATCH_09p.c(), LUNO_EXT_ADDR_LATCH_08p.c(), ARYM_EXT_ADDR_LATCH_07p.c(),
    AROS_EXT_ADDR_LATCH_06p.c(), ATEV_EXT_ADDR_LATCH_05p.c(), AVYS_EXT_ADDR_LATCH_04p.c(), ARET_EXT_ADDR_LATCH_03p.c(),
    ALYR_EXT_ADDR_LATCH_02p.c(), APUR_EXT_ADDR_LATCH_01p.c(), ALOR_EXT_ADDR_LATCH_00p.c());
  d("DATA LATCH : %c%c%c%c%c%c%c%c\n",
    SAZY_EXT_DATA_LATCH_D7n.c(), RUPA_EXT_DATA_LATCH_D6n.c(), SAGO_EXT_DATA_LATCH_D5n.c(), SODY_EXT_DATA_LATCH_D4n.c(),
    SELO_EXT_DATA_LATCH_D3n.c(), RAXY_EXT_DATA_LATCH_D2n.c(), RONY_EXT_DATA_LATCH_D1n.c(), SOMA_EXT_DATA_LATCH_D0n.c());
  d("\n");

  //d("ext_glitch.da %c\n", ext_glitch.da.c());
  //d("ext_glitch.db %c\n", ext_glitch.db.c());
  //d("ext_glitch.dc %c\n", ext_glitch.dc.c());
  //d("ext_glitch.dd %c\n", ext_glitch.dd.c());
  //d("ext_glitch.de %c\n", ext_glitch.de.c());
  //d("ext_glitch.df %c\n", ext_glitch.df.c());
  //d("ext_glitch.dg %c\n", ext_glitch.dg.c());
  //d("ext_glitch.dh %c\n", ext_glitch.dh.c());
  //d("ext_glitch.di %c\n", ext_glitch.di.c());
}

//------------------------------------------------------------------------------

void ExtBus::tock(SchematicTop& top) {

  /*p01.AREV*/ wire AREV_CPU_WRn_ABCxEFGH = nand2(top.cpu_bus.PIN_CPU_WRp.qp(), top.clk_reg.AFAS_xxxxEFGx);
  /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCxEFGH);



  /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(top.TEXO_8000_9FFFn, top.UMUT_MODE_DBG1p);
  /*p08.LEVO*/ wire _LEVO_8000_9FFFp = not1(top.TEXO_8000_9FFFn);
  /*p08.LAGU*/ wire _LAGU = and_or3(top.cpu_bus.PIN_CPU_RDp.qp(), _LEVO_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.qp());
  /*p08.LYWE*/ wire _LYWE = not1(_LAGU);
  /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);

  /*p08.TOVA*/ wire _TOVA_MODE_DBG2n = not1(top.UNOR_MODE_DBG2p);

  //----------------------------------------
  // Ext pins

  {
    /*p08.TYMU*/ wire _TYMU_EXT_RDn = nor2(top.dma_reg.LUMA_DMA_READ_CARTp, _MOTY_CPU_EXT_RD);
    /*p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMU_EXT_RDn, _TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMU_EXT_RDn, top.UNOR_MODE_DBG2p);
    PIN_EXT_RDn.io_pin(_UGAC_RD_A, _URUN_RD_D);
  }

  {

    /*p08.MEXO*/ wire _MEXO_CPU_WRn_ABCDxxxH = not1(APOV_CPU_WRp_xxxxEFGx);
    /*p08.NEVY*/ wire _NEVY = or2(_MEXO_CPU_WRn_ABCDxxxH, _MOCA_DBG_EXT_RD);
    /*p08.PUVA*/ wire _PUVA_EXT_WRn = or2(_NEVY, top.dma_reg.LUMA_DMA_READ_CARTp);
    /*p08.UVER*/ wire _UVER_WR_A = nand2(_PUVA_EXT_WRn, _TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVA_EXT_WRn, top.UNOR_MODE_DBG2p);
    PIN_EXT_WRn.io_pin(_UVER_WR_A, _USUF_WR_D);
  }


  {
    /*p08.SOGY*/ wire _SOGY_A14n = not1(top.cpu_bus.BUS_CPU_A14.qp());
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and3(top.cpu_bus.BUS_CPU_A13.qp(), _SOGY_A14n, top.cpu_bus.BUS_CPU_A15.qp());
    /*p08.TYNU*/ wire _TYNU_ADDR_RAM = and_or3(top.cpu_bus.BUS_CPU_A15.qp(), top.cpu_bus.BUS_CPU_A14.qp(), _TUMA_CART_RAM);

    /*p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A_xxCDEFGH = and3(top.ABUZ_xxCDEFGH, _TYNU_ADDR_RAM, top.cpu_bus.TUNA_0000_FDFFp());
    /*p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A_xxCDEFGH = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.MARU_DMA_A15n.qn07(), _TOZA_PIN_EXT_CS_A_xxCDEFGH);
    PIN_EXT_CSn.io_pin(_TYHO_PIN_EXT_CS_A_xxCDEFGH, _TYHO_PIN_EXT_CS_A_xxCDEFGH);
  }


  //----------------------------------------
  // Ext address

  // DMA address / CPU address latch -> ext addr pins
  {
    /*p08.MULE*/ wire _MULE_MODE_DBG1n  = not1(top.UMUT_MODE_DBG1p);
    /*p08.LOXO*/ wire _LOXO_HOLDn = and_or3(_MULE_MODE_DBG1n, top.TEXO_8000_9FFFn, top.UMUT_MODE_DBG1p);
    /*p08.LASY*/ wire _LASY_HOLDn = not1(_LOXO_HOLDn);
    /*p08.MATE*/ wire _MATE_HOLDn = not1(_LASY_HOLDn);

    /* p08.ALOR*/ ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A00.qp());
    /* p08.APUR*/ APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A01.qp());
    /* p08.ALYR*/ ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A02.qp());
    /* p08.ARET*/ ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A03.qp());
    /* p08.AVYS*/ AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A04.qp());
    /* p08.ATEV*/ ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A05.qp());
    /* p08.AROS*/ AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A06.qp());
    /* p08.ARYM*/ ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A07.qp());
    /* p08.LUNO*/ LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A08.qp());
    /* p08.LYSA*/ LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A09.qp());
    /* p08.PATE*/ PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A10.qp());
    /* p08.LUMY*/ LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A11.qp());
    /* p08.LOBU*/ LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A12.qp());
    /* p08.LONU*/ LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A13.qp());
    /* p08.NYRE*/ NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A14.qp());


    /* p08.AMET*/ wire _AMET_A00p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.NAKY_DMA_A00p.qp17(),  ALOR_EXT_ADDR_LATCH_00p.qp08());
    /* p08.ATOL*/ wire _ATOL_A01p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.PYRO_DMA_A01p.qp17(),  APUR_EXT_ADDR_LATCH_01p.qp08());
    /* p08.APOK*/ wire _APOK_A02p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.NEFY_DMA_A02p.qp17(),  ALYR_EXT_ADDR_LATCH_02p.qp08());
    /* p08.AMER*/ wire _AMER_A03p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.MUTY_DMA_A03p.qp17(),  ARET_EXT_ADDR_LATCH_03p.qp08());
    /* p08.ATEM*/ wire _ATEM_A04p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.NYKO_DMA_A04p.qp17(),  AVYS_EXT_ADDR_LATCH_04p.qp08());
    /* p08.ATOV*/ wire _ATOV_A05p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.PYLO_DMA_A05p.qp17(),  ATEV_EXT_ADDR_LATCH_05p.qp08());
    /* p08.ATYR*/ wire _ATYR_A06p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.NUTO_DMA_A06p.qp17(),  AROS_EXT_ADDR_LATCH_06p.qp08());
    /*#p08.ASUR*/ wire _ASUR_A07p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.MUGU_DMA_A07p.qp17(),  ARYM_EXT_ADDR_LATCH_07p.qp08());
    /*#p08.MANO*/ wire _MANO_A08p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.NAFA_DMA_A08n.qn07(), LUNO_EXT_ADDR_LATCH_08p.qp08());
    /* p08.MASU*/ wire _MASU_A09p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.PYNE_DMA_A09n.qn07(), LYSA_EXT_ADDR_LATCH_09p.qp08());
    /* p08.PAMY*/ wire _PAMY_A10p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.PARA_DMA_A10n.qn07(), PATE_EXT_ADDR_LATCH_10p.qp08());
    /* p08.MALE*/ wire _MALE_A11p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.NYDO_DMA_A11n.qn07(), LUMY_EXT_ADDR_LATCH_11p.qp08());
    /* p08.MOJY*/ wire _MOJY_A12p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.NYGY_DMA_A12n.qn07(), LOBU_EXT_ADDR_LATCH_12p.qp08());
    /* p08.MUCE*/ wire _MUCE_A13p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.PULA_DMA_A13n.qn07(), LONU_EXT_ADDR_LATCH_13p.qp08());
    /* p08.PEGE*/ wire _PEGE_A14p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.POKU_DMA_A14n.qn07(), NYRE_EXT_ADDR_LATCH_14p.qp08());

    /*p08.KUPO*/ wire _KUPO = nand2(_AMET_A00p, _TOVA_MODE_DBG2n);
    /*p08.CABA*/ wire _CABA = nand2(_ATOL_A01p, _TOVA_MODE_DBG2n);
    /*p08.BOKU*/ wire _BOKU = nand2(_APOK_A02p, _TOVA_MODE_DBG2n);
    /*p08.BOTY*/ wire _BOTY = nand2(_AMER_A03p, _TOVA_MODE_DBG2n);
    /*p08.BYLA*/ wire _BYLA = nand2(_ATEM_A04p, _TOVA_MODE_DBG2n);
    /*p08.BADU*/ wire _BADU = nand2(_ATOV_A05p, _TOVA_MODE_DBG2n);
    /*p08.CEPU*/ wire _CEPU = nand2(_ATYR_A06p, _TOVA_MODE_DBG2n);
    /*p08.DEFY*/ wire _DEFY = nand2(_ASUR_A07p, _TOVA_MODE_DBG2n);
    /*p08.MYNY*/ wire _MYNY = nand2(_MANO_A08p, _TOVA_MODE_DBG2n);
    /*p08.MUNE*/ wire _MUNE = nand2(_MASU_A09p, _TOVA_MODE_DBG2n);
    /*p08.ROXU*/ wire _ROXU = nand2(_PAMY_A10p, _TOVA_MODE_DBG2n);
    /*p08.LEPY*/ wire _LEPY = nand2(_MALE_A11p, _TOVA_MODE_DBG2n);
    /*p08.LUCE*/ wire _LUCE = nand2(_MOJY_A12p, _TOVA_MODE_DBG2n);
    /*p08.LABE*/ wire _LABE = nand2(_MUCE_A13p, _TOVA_MODE_DBG2n);
    /*p08.PUHE*/ wire _PUHE = nand2(_PEGE_A14p, _TOVA_MODE_DBG2n);

    /*p08.KOTY*/ wire _KOTY = nor2 (_AMET_A00p, top.UNOR_MODE_DBG2p);
    /*p08.COTU*/ wire _COTU = nor2 (_ATOL_A01p, top.UNOR_MODE_DBG2p);
    /*p08.BAJO*/ wire _BAJO = nor2 (_APOK_A02p, top.UNOR_MODE_DBG2p);
    /*p08.BOLA*/ wire _BOLA = nor2 (_AMER_A03p, top.UNOR_MODE_DBG2p);
    /*p08.BEVO*/ wire _BEVO = nor2 (_ATEM_A04p, top.UNOR_MODE_DBG2p);
    /*p08.AJAV*/ wire _AJAV = nor2 (_ATOV_A05p, top.UNOR_MODE_DBG2p);
    /*p08.CYKA*/ wire _CYKA = nor2 (_ATYR_A06p, top.UNOR_MODE_DBG2p);
    /*p08.COLO*/ wire _COLO = nor2 (_ASUR_A07p, top.UNOR_MODE_DBG2p);
    /*p08.MEGO*/ wire _MEGO = nor2 (_MANO_A08p, top.UNOR_MODE_DBG2p);
    /*p08.MENY*/ wire _MENY = nor2 (_MASU_A09p, top.UNOR_MODE_DBG2p);
    /*p08.RORE*/ wire _RORE = nor2 (_PAMY_A10p, top.UNOR_MODE_DBG2p);
    /*p08.LYNY*/ wire _LYNY = nor2 (_MALE_A11p, top.UNOR_MODE_DBG2p);
    /*p08.LOSO*/ wire _LOSO = nor2 (_MOJY_A12p, top.UNOR_MODE_DBG2p);
    /*p08.LEVA*/ wire _LEVA = nor2 (_MUCE_A13p, top.UNOR_MODE_DBG2p);
    /*p08.PAHY*/ wire _PAHY = nor2 (_PEGE_A14p, top.UNOR_MODE_DBG2p);

    PIN_EXT_A00p.io_pin(_KUPO, _KOTY);
    PIN_EXT_A01p.io_pin(_CABA, _COTU);
    PIN_EXT_A02p.io_pin(_BOKU, _BAJO);
    PIN_EXT_A03p.io_pin(_BOTY, _BOLA);
    PIN_EXT_A04p.io_pin(_BYLA, _BEVO);
    PIN_EXT_A05p.io_pin(_BADU, _AJAV);
    PIN_EXT_A06p.io_pin(_CEPU, _CYKA);
    PIN_EXT_A07p.io_pin(_DEFY, _COLO);
    PIN_EXT_A08p.io_pin(_MYNY, _MEGO);
    PIN_EXT_A09p.io_pin(_MUNE, _MENY);
    PIN_EXT_A10p.io_pin(_ROXU, _RORE);
    PIN_EXT_A11p.io_pin(_LEPY, _LYNY);
    PIN_EXT_A12p.io_pin(_LUCE, _LOSO);
    PIN_EXT_A13p.io_pin(_LABE, _LEVA);
    PIN_EXT_A14p.io_pin(_PUHE, _PAHY);
  }


  {
    // A15 is "special"

    /* p08.RYCA*/ wire _RYCA_MODE_DBG2n = not1(top.UNOR_MODE_DBG2p);
    /* p08.SOBY*/ wire _SOBY_A15n = nor2(top.cpu_bus.BUS_CPU_A15.qp(), top.TUTU_ADDR_BOOTp);
    /* p08.SEPY*/ wire _SEPY_A15p_ABxxxxxx = nand2(top.ABUZ_xxCDEFGH, _SOBY_A15n);
    /* p08.TAZY*/ wire _TAZY_A15p = mux2p(top.dma_reg.LUMA_DMA_READ_CARTp, top.dma_reg.MARU_DMA_A15n.qn07(), _SEPY_A15p_ABxxxxxx);
    /* p08.SUZE*/ wire _SUZE_PIN_EXT_A15n = nand2(_TAZY_A15p, _RYCA_MODE_DBG2n);
    /* p08.RULO*/ wire _RULO_PIN_EXT_A15n = nor2 (_TAZY_A15p, top.UNOR_MODE_DBG2p);

    PIN_EXT_A15p.io_pin(_SUZE_PIN_EXT_A15n, _RULO_PIN_EXT_A15n);
  }

  //----------------------------------------
  // Ext data out


  // CPU data bus -> external data bus
  {
    // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /*p08.REDU*/ wire _REDU_CPU_RDn = not1(top.TEDO_CPU_RDp);
    /*p08.RORU*/ wire _RORU_CBD_TO_EPDn = mux2p(top.UNOR_MODE_DBG2p, _REDU_CPU_RDn, _MOTY_CPU_EXT_RD);
    /*p08.LULA*/ wire _LULA_CBD_TO_EPDp = not1(_RORU_CBD_TO_EPDn);

    /*p25.RUXA*/ wire _RUXA = nand2(top.cpu_bus.BUS_CPU_D0p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RUJA*/ wire _RUJA = nand2(top.cpu_bus.BUS_CPU_D1p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RABY*/ wire _RABY = nand2(top.cpu_bus.BUS_CPU_D2p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RERA*/ wire _RERA = nand2(top.cpu_bus.BUS_CPU_D3p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RORY*/ wire _RORY = nand2(top.cpu_bus.BUS_CPU_D4p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RYVO*/ wire _RYVO = nand2(top.cpu_bus.BUS_CPU_D5p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RAFY*/ wire _RAFY = nand2(top.cpu_bus.BUS_CPU_D6p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RAVU*/ wire _RAVU = nand2(top.cpu_bus.BUS_CPU_D7p.qp(), _LULA_CBD_TO_EPDp);

    /*p08.RUNE*/ wire _RUNE = nor2 (top.cpu_bus.BUS_CPU_D0p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RYPU*/ wire _RYPU = nor2 (top.cpu_bus.BUS_CPU_D1p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.SULY*/ wire _SULY = nor2 (top.cpu_bus.BUS_CPU_D2p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.SEZE*/ wire _SEZE = nor2 (top.cpu_bus.BUS_CPU_D3p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RESY*/ wire _RESY = nor2 (top.cpu_bus.BUS_CPU_D4p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.TAMU*/ wire _TAMU = nor2 (top.cpu_bus.BUS_CPU_D5p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.ROGY*/ wire _ROGY = nor2 (top.cpu_bus.BUS_CPU_D6p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RYDA*/ wire _RYDA = nor2 (top.cpu_bus.BUS_CPU_D7p.qp(), _RORU_CBD_TO_EPDn);

    PIN_EXT_D00p.io_pin(_RUXA, _RUNE, _LULA_CBD_TO_EPDp);
    PIN_EXT_D01p.io_pin(_RUJA, _RYPU, _LULA_CBD_TO_EPDp);
    PIN_EXT_D02p.io_pin(_RABY, _SULY, _LULA_CBD_TO_EPDp);
    PIN_EXT_D03p.io_pin(_RERA, _SEZE, _LULA_CBD_TO_EPDp);
    PIN_EXT_D04p.io_pin(_RORY, _RESY, _LULA_CBD_TO_EPDp);
    PIN_EXT_D05p.io_pin(_RYVO, _TAMU, _LULA_CBD_TO_EPDp);
    PIN_EXT_D06p.io_pin(_RAFY, _ROGY, _LULA_CBD_TO_EPDp);
    PIN_EXT_D07p.io_pin(_RAVU, _RYDA, _LULA_CBD_TO_EPDp);
  }

  //----------------------------------------
  // Ext data in
  {
    // -> ext bus
    /*p08.LAVO*/ wire _LAVO_HOLDn = nand3(top.cpu_bus.PIN_CPU_RDp.qp(), top.TEXO_8000_9FFFn, top.cpu_bus.PIN_CPU_LATCH_EXT.qp());

    // Ext pin -> Ext latch
    /*#p08.SOMA*/ SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn, PIN_EXT_D00p.qn());
    /* p08.RONY*/ RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn, PIN_EXT_D01p.qn());
    /* p08.RAXY*/ RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn, PIN_EXT_D02p.qn());
    /* p08.SELO*/ SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn, PIN_EXT_D03p.qn());
    /* p08.SODY*/ SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn, PIN_EXT_D04p.qn());
    /* p08.SAGO*/ SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn, PIN_EXT_D05p.qn());
    /* p08.RUPA*/ RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn, PIN_EXT_D06p.qn());
    /* p08.SAZY*/ SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn, PIN_EXT_D07p.qn());

    // Ext latch -> int bus
    /*#p08.RYMA*/ top.cpu_bus.BUS_CPU_D0p.tri_6nn(_LAVO_HOLDn, SOMA_EXT_DATA_LATCH_D0n.qp08());
    /* p08.RUVO*/ top.cpu_bus.BUS_CPU_D1p.tri_6nn(_LAVO_HOLDn, RONY_EXT_DATA_LATCH_D1n.qp08());
    /* p08.RYKO*/ top.cpu_bus.BUS_CPU_D2p.tri_6nn(_LAVO_HOLDn, RAXY_EXT_DATA_LATCH_D2n.qp08());
    /* p08.TAVO*/ top.cpu_bus.BUS_CPU_D3p.tri_6nn(_LAVO_HOLDn, SELO_EXT_DATA_LATCH_D3n.qp08());
    /* p08.TEPE*/ top.cpu_bus.BUS_CPU_D4p.tri_6nn(_LAVO_HOLDn, SODY_EXT_DATA_LATCH_D4n.qp08());
    /* p08.SAFO*/ top.cpu_bus.BUS_CPU_D5p.tri_6nn(_LAVO_HOLDn, SAGO_EXT_DATA_LATCH_D5n.qp08());
    /* p08.SEVU*/ top.cpu_bus.BUS_CPU_D6p.tri_6nn(_LAVO_HOLDn, RUPA_EXT_DATA_LATCH_D6n.qp08());
    /* p08.TAJU*/ top.cpu_bus.BUS_CPU_D7p.tri_6nn(_LAVO_HOLDn, SAZY_EXT_DATA_LATCH_D7n.qp08());
  }
}

//------------------------------------------------------------------------------
