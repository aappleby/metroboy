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

  d("ext_glitch.da %c\n", ext_glitch.da.c());
  d("ext_glitch.db %c\n", ext_glitch.db.c());
  d("ext_glitch.dc %c\n", ext_glitch.dc.c());
  d("ext_glitch.dd %c\n", ext_glitch.dd.c());
  d("ext_glitch.de %c\n", ext_glitch.de.c());
  d("ext_glitch.df %c\n", ext_glitch.df.c());
  d("ext_glitch.dg %c\n", ext_glitch.dg.c());
  d("ext_glitch.dh %c\n", ext_glitch.dh.c());
  d("ext_glitch.di %c\n", ext_glitch.di.c());
}

//------------------------------------------------------------------------------

void ExtBus::tock(SchematicTop& top) {

  wire LUMA_DMA_READ_CARTp = top.dma_reg.LUMA_DMA_READ_CARTp;
  wire TUNA_0000_FDFFp = top.cpu_bus.TUNA_0000_FDFFp();
  wire TUTU_ADDR_BOOTp = top.TUTU_ADDR_BOOTp;
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n = not1(top.UNOR_MODE_DBG2p);

  //----------------------------------------
  // Ext pins



  {
#if 0
    PIN_EXT_RDn = (PIN_CPU_WR && PIN_CPU_ADDR_EXTp);
    if (ADDR_VRAM) {
      PIN_EXT_RDn = PIN_CPU_RDp || PIN_CPU_WRp;
    }
#endif

    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.BUS_CPU_A15.qp());
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.BUS_CPU_A13.qp(), top.cpu_bus.BUS_CPU_A14.qp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.PIN_CPU_ADDR_EXTp.qp(), TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, top.UMUT_MODE_DBG1p);
    /*p08.LEVO*/ wire LEVO_8000_9FFFp = not1(TEXO_8000_9FFFn);
    /*p08.LAGU*/ wire LAGU = and_or3(top.cpu_bus.PIN_CPU_RDp.qp(), LEVO_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.qp());
    /*p08.LYWE*/ wire LYWE = not1(LAGU);
    /*p08.MOTY*/ wire MOTY_CPU_EXT_RD = or2(MOCA_DBG_EXT_RD, LYWE);
    /*p08.TYMU*/ wire TYMU_EXT_RDn = nor2(LUMA_DMA_READ_CARTp, MOTY_CPU_EXT_RD);
    /*p08.UGAC*/ wire UGAC_RD_A = nand2(TYMU_EXT_RDn, TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire URUN_RD_D = nor2 (TYMU_EXT_RDn, top.UNOR_MODE_DBG2p);
    PIN_EXT_RDn.io_pin(UGAC_RD_A, URUN_RD_D);
  }

  {
#if 0
    PIN_EXT_WRn = nand(PIN_CPU_WRp, xxxxEFGx, PIN_CPU_ADDR_EXTp, !ADDR_VRAM);
#endif

    /*p01.AFAS*/ wire AFAS_xxxxEFGx = nor2(top.clk_reg.ADAR_ABCxxxxH, top.clk_reg.ATYP_ABCDxxxx);
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCxEFGH = nand2(top.cpu_bus.PIN_CPU_WRp.qp(), AFAS_xxxxEFGx);
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCxEFGH);
    /*p08.MEXO*/ wire MEXO_CPU_WRn_ABCDxxxH = not1(APOV_CPU_WRp_xxxxEFGx);

    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.BUS_CPU_A15.qp());
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.BUS_CPU_A13.qp(), top.cpu_bus.BUS_CPU_A14.qp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.PIN_CPU_ADDR_EXTp.qp(), TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, top.UMUT_MODE_DBG1p);
    /*p08.NEVY*/ wire NEVY = or2(MEXO_CPU_WRn_ABCDxxxH, MOCA_DBG_EXT_RD);
    /*p08.PUVA*/ wire PUVA_EXT_WRn = or2(NEVY, LUMA_DMA_READ_CARTp);
    /*p08.UVER*/ wire UVER_WR_A = nand2(PUVA_EXT_WRn, TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire USUF_WR_D = nor2 (PUVA_EXT_WRn, top.UNOR_MODE_DBG2p);
    PIN_EXT_WRn.io_pin(UVER_WR_A, USUF_WR_D);
  }


  {
#if 0
    /*p08.TYNU*/ wire TYNU_ADDR_RAM = (BUS_CPU_A15 && BUS_CPU_A14) || (BUS_CPU_A13 && !BUS_CPU_A14 && BUS_CPU_A15);
    PIN_EXT_CSn = nand(xxCDEFGH, PIN_CPU_ADDR_EXTp, TYNU_ADDR_RAM, 0000_FDFFp);
#endif

    /*#p01.AGUT*/ wire AGUT_xxCDEFGH = or_and3(top.clk_reg.AROV_xxCDEFxx.qp(), top.clk_reg.AJAX_xxxxEFGH.qp(), top.cpu_bus.PIN_CPU_ADDR_EXTp.qp());
    /*#p01.AWOD*/ wire AWOD_ABxxxxxx = nor2(top.UNOR_MODE_DBG2p, AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ wire ABUZ_xxCDEFGH = not1(AWOD_ABxxxxxx);

    /*p08.SOGY*/ wire SOGY_A14n = not1(top.cpu_bus.BUS_CPU_A14.qp());
    /*p08.TUMA*/ wire TUMA_CART_RAM = and3(top.cpu_bus.BUS_CPU_A13.qp(), SOGY_A14n, top.cpu_bus.BUS_CPU_A15.qp());
    /*p08.TYNU*/ wire TYNU_ADDR_RAM = and_or3(top.cpu_bus.BUS_CPU_A15.qp(), top.cpu_bus.BUS_CPU_A14.qp(), TUMA_CART_RAM);

    /*p08.TOZA*/ wire TOZA_PIN_EXT_CS_A_xxCDEFGH = and3(ABUZ_xxCDEFGH, TYNU_ADDR_RAM, TUNA_0000_FDFFp); // suggests ABUZp
    /*p08.TYHO*/ wire TYHO_PIN_EXT_CS_A_xxCDEFGH = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.MARU_DMA_A15n.q07(), TOZA_PIN_EXT_CS_A_xxCDEFGH);
    PIN_EXT_CSn.io_pin(TYHO_PIN_EXT_CS_A_xxCDEFGH, TYHO_PIN_EXT_CS_A_xxCDEFGH);
  }


  //----------------------------------------
  // Ext address

  // DMA address / CPU address latch -> ext addr pins
  {
#if 0
    PIN_EXT_A14p = tp_latch(and2(PIN_CPU_ADDR_EXTp, !ADDR_VRAM), BUS_CPU_A14);
#endif

    /*p08.MULE*/ wire MULE_MODE_DBG1n  = not1(top.UMUT_MODE_DBG1p);
    /*p08.SORE*/ wire SORE_0000_7FFFp  = not1(top.cpu_bus.BUS_CPU_A15.qp());
    /*p08.TEVY*/ wire TEVY_8000_9FFFn  = or3(top.cpu_bus.BUS_CPU_A13.qp(), top.cpu_bus.BUS_CPU_A14.qp(), SORE_0000_7FFFp);

    //wire blah = top.cpu_bus.PIN_CPU_ADDR_EXTp.qp();
    //ext_glitch.set(blah);

    /*p08.TEXO*/ wire TEXO_8000_9FFFn  = and2(top.cpu_bus.PIN_CPU_ADDR_EXTp.qp(), TEVY_8000_9FFFn);
    /*p08.LOXO*/ wire LOXO_HOLDn = and_or3(MULE_MODE_DBG1n, TEXO_8000_9FFFn, top.UMUT_MODE_DBG1p);
    /*p08.LASY*/ wire LASY_HOLDn = not1(LOXO_HOLDn);
    /*p08.MATE*/ wire MATE_HOLDn = not1(LASY_HOLDn);


    // Is this acutally a pass gate?

    /* p08.ALOR*/ ALOR_EXT_ADDR_LATCH_00p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A00.qp());
    /* p08.APUR*/ APUR_EXT_ADDR_LATCH_01p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A01.qp());
    /* p08.ALYR*/ ALYR_EXT_ADDR_LATCH_02p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A02.qp());
    /* p08.ARET*/ ARET_EXT_ADDR_LATCH_03p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A03.qp());
    /* p08.AVYS*/ AVYS_EXT_ADDR_LATCH_04p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A04.qp());
    /* p08.ATEV*/ ATEV_EXT_ADDR_LATCH_05p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A05.qp());
    /* p08.AROS*/ AROS_EXT_ADDR_LATCH_06p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A06.qp());
    /* p08.ARYM*/ ARYM_EXT_ADDR_LATCH_07p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A07.qp());
    /* p08.LUNO*/ LUNO_EXT_ADDR_LATCH_08p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A08.qp());
    /* p08.LYSA*/ LYSA_EXT_ADDR_LATCH_09p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A09.qp());
    /* p08.PATE*/ PATE_EXT_ADDR_LATCH_10p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A10.qp());
    /* p08.LUMY*/ LUMY_EXT_ADDR_LATCH_11p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A11.qp());
    /* p08.LOBU*/ LOBU_EXT_ADDR_LATCH_12p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A12.qp());
    /* p08.LONU*/ LONU_EXT_ADDR_LATCH_13p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A13.qp());
    /* p08.NYRE*/ NYRE_EXT_ADDR_LATCH_14p.tp_latch(MATE_HOLDn, top.cpu_bus.BUS_CPU_A14.qp());


    /* p08.AMET*/ wire AMET_A00p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.NAKY_DMA_A00p.qp(),  ALOR_EXT_ADDR_LATCH_00p.qp());
    /* p08.ATOL*/ wire ATOL_A01p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.PYRO_DMA_A01p.qp(),  APUR_EXT_ADDR_LATCH_01p.qp());
    /* p08.APOK*/ wire APOK_A02p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.NEFY_DMA_A02p.qp(),  ALYR_EXT_ADDR_LATCH_02p.qp());
    /* p08.AMER*/ wire AMER_A03p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.MUTY_DMA_A03p.qp(),  ARET_EXT_ADDR_LATCH_03p.qp());
    /* p08.ATEM*/ wire ATEM_A04p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.NYKO_DMA_A04p.qp(),  AVYS_EXT_ADDR_LATCH_04p.qp());
    /* p08.ATOV*/ wire ATOV_A05p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.PYLO_DMA_A05p.qp(),  ATEV_EXT_ADDR_LATCH_05p.qp());
    /* p08.ATYR*/ wire ATYR_A06p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.NUTO_DMA_A06p.qp(),  AROS_EXT_ADDR_LATCH_06p.qp());
    /*#p08.ASUR*/ wire ASUR_A07p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.MUGU_DMA_A07p.qp(),  ARYM_EXT_ADDR_LATCH_07p.qp());
    /*#p08.MANO*/ wire MANO_A08p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.NAFA_DMA_A08n.q07(), LUNO_EXT_ADDR_LATCH_08p.qp());
    /* p08.MASU*/ wire MASU_A09p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.PYNE_DMA_A09n.q07(), LYSA_EXT_ADDR_LATCH_09p.qp());
    /* p08.PAMY*/ wire PAMY_A10p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.PARA_DMA_A10n.q07(), PATE_EXT_ADDR_LATCH_10p.qp());
    /* p08.MALE*/ wire MALE_A11p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.NYDO_DMA_A11n.q07(), LUMY_EXT_ADDR_LATCH_11p.qp());
    /* p08.MOJY*/ wire MOJY_A12p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.NYGY_DMA_A12n.q07(), LOBU_EXT_ADDR_LATCH_12p.qp());
    /* p08.MUCE*/ wire MUCE_A13p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.PULA_DMA_A13n.q07(), LONU_EXT_ADDR_LATCH_13p.qp());
    /* p08.PEGE*/ wire PEGE_A14p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.POKU_DMA_A14n.q07(), NYRE_EXT_ADDR_LATCH_14p.qp());

    /*p08.KUPO*/ wire KUPO = nand2(AMET_A00p, TOVA_MODE_DBG2n);
    /*p08.CABA*/ wire CABA = nand2(ATOL_A01p, TOVA_MODE_DBG2n);
    /*p08.BOKU*/ wire BOKU = nand2(APOK_A02p, TOVA_MODE_DBG2n);
    /*p08.BOTY*/ wire BOTY = nand2(AMER_A03p, TOVA_MODE_DBG2n);
    /*p08.BYLA*/ wire BYLA = nand2(ATEM_A04p, TOVA_MODE_DBG2n);
    /*p08.BADU*/ wire BADU = nand2(ATOV_A05p, TOVA_MODE_DBG2n);
    /*p08.CEPU*/ wire CEPU = nand2(ATYR_A06p, TOVA_MODE_DBG2n);
    /*p08.DEFY*/ wire DEFY = nand2(ASUR_A07p, TOVA_MODE_DBG2n);
    /*p08.MYNY*/ wire MYNY = nand2(MANO_A08p, TOVA_MODE_DBG2n);
    /*p08.MUNE*/ wire MUNE = nand2(MASU_A09p, TOVA_MODE_DBG2n);
    /*p08.ROXU*/ wire ROXU = nand2(PAMY_A10p, TOVA_MODE_DBG2n);
    /*p08.LEPY*/ wire LEPY = nand2(MALE_A11p, TOVA_MODE_DBG2n);
    /*p08.LUCE*/ wire LUCE = nand2(MOJY_A12p, TOVA_MODE_DBG2n);
    /*p08.LABE*/ wire LABE = nand2(MUCE_A13p, TOVA_MODE_DBG2n);
    /*p08.PUHE*/ wire PUHE = nand2(PEGE_A14p, TOVA_MODE_DBG2n);

    /*p08.KOTY*/ wire KOTY = nor2 (AMET_A00p, top.UNOR_MODE_DBG2p);
    /*p08.COTU*/ wire COTU = nor2 (ATOL_A01p, top.UNOR_MODE_DBG2p);
    /*p08.BAJO*/ wire BAJO = nor2 (APOK_A02p, top.UNOR_MODE_DBG2p);
    /*p08.BOLA*/ wire BOLA = nor2 (AMER_A03p, top.UNOR_MODE_DBG2p);
    /*p08.BEVO*/ wire BEVO = nor2 (ATEM_A04p, top.UNOR_MODE_DBG2p);
    /*p08.AJAV*/ wire AJAV = nor2 (ATOV_A05p, top.UNOR_MODE_DBG2p);
    /*p08.CYKA*/ wire CYKA = nor2 (ATYR_A06p, top.UNOR_MODE_DBG2p);
    /*p08.COLO*/ wire COLO = nor2 (ASUR_A07p, top.UNOR_MODE_DBG2p);
    /*p08.MEGO*/ wire MEGO = nor2 (MANO_A08p, top.UNOR_MODE_DBG2p);
    /*p08.MENY*/ wire MENY = nor2 (MASU_A09p, top.UNOR_MODE_DBG2p);
    /*p08.RORE*/ wire RORE = nor2 (PAMY_A10p, top.UNOR_MODE_DBG2p);
    /*p08.LYNY*/ wire LYNY = nor2 (MALE_A11p, top.UNOR_MODE_DBG2p);
    /*p08.LOSO*/ wire LOSO = nor2 (MOJY_A12p, top.UNOR_MODE_DBG2p);
    /*p08.LEVA*/ wire LEVA = nor2 (MUCE_A13p, top.UNOR_MODE_DBG2p);
    /*p08.PAHY*/ wire PAHY = nor2 (PEGE_A14p, top.UNOR_MODE_DBG2p);

    PIN_EXT_A00p.io_pin(KUPO, KOTY);
    PIN_EXT_A01p.io_pin(CABA, COTU);
    PIN_EXT_A02p.io_pin(BOKU, BAJO);
    PIN_EXT_A03p.io_pin(BOTY, BOLA);
    PIN_EXT_A04p.io_pin(BYLA, BEVO);
    PIN_EXT_A05p.io_pin(BADU, AJAV);
    PIN_EXT_A06p.io_pin(CEPU, CYKA);
    PIN_EXT_A07p.io_pin(DEFY, COLO);
    PIN_EXT_A08p.io_pin(MYNY, MEGO);
    PIN_EXT_A09p.io_pin(MUNE, MENY);
    PIN_EXT_A10p.io_pin(ROXU, RORE);
    PIN_EXT_A11p.io_pin(LEPY, LYNY);
    PIN_EXT_A12p.io_pin(LUCE, LOSO);
    PIN_EXT_A13p.io_pin(LABE, LEVA);
    PIN_EXT_A14p.io_pin(PUHE, PAHY);
  }


  {
    // A15 is "special"

    /*#p01.AGUT*/ wire AGUT_xxCDEFGH = or_and3(top.clk_reg.AROV_xxCDEFxx.qp(), top.clk_reg.AJAX_xxxxEFGH.qp(), top.cpu_bus.PIN_CPU_ADDR_EXTp.qp());
    /*#p01.AWOD*/ wire AWOD_ABxxxxxx = nor2(top.UNOR_MODE_DBG2p, AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ wire ABUZ_xxCDEFGH = not1(AWOD_ABxxxxxx);

    /* p08.RYCA*/ wire RYCA_MODE_DBG2n = not1(top.UNOR_MODE_DBG2p);
    /* p08.SOBY*/ wire SOBY_A15n = nor2(top.cpu_bus.BUS_CPU_A15.qp(), TUTU_ADDR_BOOTp);
    /* p08.SEPY*/ wire SEPY_A15p_ABxxxxxx = nand2(ABUZ_xxCDEFGH, SOBY_A15n);
    /* p08.TAZY*/ wire TAZY_A15p = mux2p(LUMA_DMA_READ_CARTp, top.dma_reg.MARU_DMA_A15n.q07(), SEPY_A15p_ABxxxxxx);

    /* p08.SUZE*/ wire SUZE_PIN_EXT_A15n = nand2(TAZY_A15p, RYCA_MODE_DBG2n);
    /* p08.RULO*/ wire RULO_PIN_EXT_A15n = nor2 (TAZY_A15p, top.UNOR_MODE_DBG2p);

    PIN_EXT_A15p.io_pin(SUZE_PIN_EXT_A15n, RULO_PIN_EXT_A15n);
  }

  //----------------------------------------
  // Ext data out


  // CPU data bus -> external data bus
  {
    // So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.BUS_CPU_A15.qp());
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.BUS_CPU_A13.qp(), top.cpu_bus.BUS_CPU_A14.qp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.PIN_CPU_ADDR_EXTp.qp(), TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, top.UMUT_MODE_DBG1p);
    /*p08.LEVO*/ wire LEVO_8000_9FFFp = not1(TEXO_8000_9FFFn);
    /*p08.LAGU*/ wire LAGU = and_or3(top.cpu_bus.PIN_CPU_RDp.qp(), LEVO_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.qp());
    /*p08.LYWE*/ wire LYWE = not1(LAGU);
    /*p08.MOTY*/ wire MOTY_CPU_EXT_RD = or2(MOCA_DBG_EXT_RD, LYWE);

    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2n(top.UNOR_MODE_DBG2p, PIN_EXT_RDn.qn(), top.cpu_bus.PIN_CPU_RDp.qp());
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /*p08.REDU*/ wire REDU_CPU_RDn = not1(TEDO_CPU_RDp);
    /*p08.RORU*/ wire RORU_CBD_TO_EPDn = mux2p(top.UNOR_MODE_DBG2p, REDU_CPU_RDn, MOTY_CPU_EXT_RD);
    /*p08.LULA*/ wire LULA_CBD_TO_EPDp = not1(RORU_CBD_TO_EPDn);

    /*p25.RUXA*/ wire RUXA = nand2(top.cpu_bus.BUS_CPU_D0p.qp(), LULA_CBD_TO_EPDp);
    /*p25.RUJA*/ wire RUJA = nand2(top.cpu_bus.BUS_CPU_D1p.qp(), LULA_CBD_TO_EPDp);
    /*p25.RABY*/ wire RABY = nand2(top.cpu_bus.BUS_CPU_D2p.qp(), LULA_CBD_TO_EPDp);
    /*p25.RERA*/ wire RERA = nand2(top.cpu_bus.BUS_CPU_D3p.qp(), LULA_CBD_TO_EPDp);
    /*p25.RORY*/ wire RORY = nand2(top.cpu_bus.BUS_CPU_D4p.qp(), LULA_CBD_TO_EPDp);
    /*p25.RYVO*/ wire RYVO = nand2(top.cpu_bus.BUS_CPU_D5p.qp(), LULA_CBD_TO_EPDp);
    /*p25.RAFY*/ wire RAFY = nand2(top.cpu_bus.BUS_CPU_D6p.qp(), LULA_CBD_TO_EPDp);
    /*p25.RAVU*/ wire RAVU = nand2(top.cpu_bus.BUS_CPU_D7p.qp(), LULA_CBD_TO_EPDp);

    /*p08.RUNE*/ wire RUNE = nor2 (top.cpu_bus.BUS_CPU_D0p.qp(), RORU_CBD_TO_EPDn);
    /*p08.RYPU*/ wire RYPU = nor2 (top.cpu_bus.BUS_CPU_D1p.qp(), RORU_CBD_TO_EPDn);
    /*p08.SULY*/ wire SULY = nor2 (top.cpu_bus.BUS_CPU_D2p.qp(), RORU_CBD_TO_EPDn);
    /*p08.SEZE*/ wire SEZE = nor2 (top.cpu_bus.BUS_CPU_D3p.qp(), RORU_CBD_TO_EPDn);
    /*p08.RESY*/ wire RESY = nor2 (top.cpu_bus.BUS_CPU_D4p.qp(), RORU_CBD_TO_EPDn);
    /*p08.TAMU*/ wire TAMU = nor2 (top.cpu_bus.BUS_CPU_D5p.qp(), RORU_CBD_TO_EPDn);
    /*p08.ROGY*/ wire ROGY = nor2 (top.cpu_bus.BUS_CPU_D6p.qp(), RORU_CBD_TO_EPDn);
    /*p08.RYDA*/ wire RYDA = nor2 (top.cpu_bus.BUS_CPU_D7p.qp(), RORU_CBD_TO_EPDn);

    PIN_EXT_D00p.io_pin(RUXA, RUNE, LULA_CBD_TO_EPDp);
    PIN_EXT_D01p.io_pin(RUJA, RYPU, LULA_CBD_TO_EPDp);
    PIN_EXT_D02p.io_pin(RABY, SULY, LULA_CBD_TO_EPDp);
    PIN_EXT_D03p.io_pin(RERA, SEZE, LULA_CBD_TO_EPDp);
    PIN_EXT_D04p.io_pin(RORY, RESY, LULA_CBD_TO_EPDp);
    PIN_EXT_D05p.io_pin(RYVO, TAMU, LULA_CBD_TO_EPDp);
    PIN_EXT_D06p.io_pin(RAFY, ROGY, LULA_CBD_TO_EPDp);
    PIN_EXT_D07p.io_pin(RAVU, RYDA, LULA_CBD_TO_EPDp);
  }

  //----------------------------------------
  // Ext data in
  {
    // -> ext bus
    /*p08.SORE*/ wire SORE_0000_7FFFp  = not1(top.cpu_bus.BUS_CPU_A15.qp());
    /*p08.TEVY*/ wire TEVY_8000_9FFFn  = or3(top.cpu_bus.BUS_CPU_A13.qp(), top.cpu_bus.BUS_CPU_A14.qp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn  = and2(top.cpu_bus.PIN_CPU_ADDR_EXTp.qp(), TEVY_8000_9FFFn);
    /*p08.LAVO*/ wire LAVO_HOLDn = nand3(top.cpu_bus.PIN_CPU_RDp.qp(), TEXO_8000_9FFFn, top.cpu_bus.PIN_CPU_LATCH_EXT.qp());

    // Ext pin -> Ext latch
    /*#p08.SOMA*/ SOMA_EXT_DATA_LATCH_D0n.tp_latch(LAVO_HOLDn, PIN_EXT_D00p.qn());
    /* p08.RONY*/ RONY_EXT_DATA_LATCH_D1n.tp_latch(LAVO_HOLDn, PIN_EXT_D01p.qn());
    /* p08.RAXY*/ RAXY_EXT_DATA_LATCH_D2n.tp_latch(LAVO_HOLDn, PIN_EXT_D02p.qn());
    /* p08.SELO*/ SELO_EXT_DATA_LATCH_D3n.tp_latch(LAVO_HOLDn, PIN_EXT_D03p.qn());
    /* p08.SODY*/ SODY_EXT_DATA_LATCH_D4n.tp_latch(LAVO_HOLDn, PIN_EXT_D04p.qn());
    /* p08.SAGO*/ SAGO_EXT_DATA_LATCH_D5n.tp_latch(LAVO_HOLDn, PIN_EXT_D05p.qn());
    /* p08.RUPA*/ RUPA_EXT_DATA_LATCH_D6n.tp_latch(LAVO_HOLDn, PIN_EXT_D06p.qn());
    /* p08.SAZY*/ SAZY_EXT_DATA_LATCH_D7n.tp_latch(LAVO_HOLDn, PIN_EXT_D07p.qn());

    // Ext latch -> int bus
    /*#p08.RYMA*/ top.cpu_bus.BUS_CPU_D0p.tri_6nn(LAVO_HOLDn, SOMA_EXT_DATA_LATCH_D0n.qp());
    /* p08.RUVO*/ top.cpu_bus.BUS_CPU_D1p.tri_6nn(LAVO_HOLDn, RONY_EXT_DATA_LATCH_D1n.qp());
    /* p08.RYKO*/ top.cpu_bus.BUS_CPU_D2p.tri_6nn(LAVO_HOLDn, RAXY_EXT_DATA_LATCH_D2n.qp());
    /* p08.TAVO*/ top.cpu_bus.BUS_CPU_D3p.tri_6nn(LAVO_HOLDn, SELO_EXT_DATA_LATCH_D3n.qp());
    /* p08.TEPE*/ top.cpu_bus.BUS_CPU_D4p.tri_6nn(LAVO_HOLDn, SODY_EXT_DATA_LATCH_D4n.qp());
    /* p08.SAFO*/ top.cpu_bus.BUS_CPU_D5p.tri_6nn(LAVO_HOLDn, SAGO_EXT_DATA_LATCH_D5n.qp());
    /* p08.SEVU*/ top.cpu_bus.BUS_CPU_D6p.tri_6nn(LAVO_HOLDn, RUPA_EXT_DATA_LATCH_D6n.qp());
    /* p08.TAJU*/ top.cpu_bus.BUS_CPU_D7p.tri_6nn(LAVO_HOLDn, SAZY_EXT_DATA_LATCH_D7n.qp());
  }
}

//------------------------------------------------------------------------------
