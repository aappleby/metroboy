#include "GateBoyLib/Sch_ExtBus.h"

#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void ExtBus::dump(Dumper& d) const {
  d("---------- Ext Bus  ----------\n");
  d("EXT_PIN_CLK     : %d\n", EXT_PIN_CLK.c());
  d("EXT_PIN_RDn     : %c\n", EXT_PIN_RDn.c());
  d("EXT_PIN_WRn     : %c\n", EXT_PIN_WRn.c());
  d("EXT_PIN_CSn     : %c\n", EXT_PIN_CSn.c());
  d("\n");

  d("EXT_PIN_ADDR    : %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    EXT_PIN_A15p.c(), EXT_PIN_A14p.c(), EXT_PIN_A13p.c(), EXT_PIN_A12p.c(),
    EXT_PIN_A11p.c(), EXT_PIN_A10p.c(), EXT_PIN_A09p.c(), EXT_PIN_A08p.c(),
    EXT_PIN_A07p.c(), EXT_PIN_A06p.c(), EXT_PIN_A05p.c(), EXT_PIN_A04p.c(),
    EXT_PIN_A03p.c(), EXT_PIN_A02p.c(), EXT_PIN_A01p.c(), EXT_PIN_A00p.c());
  d("EXT_PIN_DATA    : %c%c%c%c%c%c%c%c\n",
    EXT_PIN_D07p.c(), EXT_PIN_D06p.c(), EXT_PIN_D05p.c(), EXT_PIN_D04p.c(),
    EXT_PIN_D03p.c(), EXT_PIN_D02p.c(), EXT_PIN_D01p.c(), EXT_PIN_D00p.c());
  d("\n");

  d("EXT ADDR LATCH  : _%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    NYRE_EXT_ADDR_LATCH_14p.c(), LONU_EXT_ADDR_LATCH_13p.c(), LOBU_EXT_ADDR_LATCH_12p.c(), LUMY_EXT_ADDR_LATCH_11p.c(),
    PATE_EXT_ADDR_LATCH_10p.c(), LYSA_EXT_ADDR_LATCH_09p.c(), LUNO_EXT_ADDR_LATCH_08p.c(), ARYM_EXT_ADDR_LATCH_07p.c(),
    AROS_EXT_ADDR_LATCH_06p.c(), ATEV_EXT_ADDR_LATCH_05p.c(), AVYS_EXT_ADDR_LATCH_04p.c(), ARET_EXT_ADDR_LATCH_03p.c(),
    ALYR_EXT_ADDR_LATCH_02p.c(), APUR_EXT_ADDR_LATCH_01p.c(), ALOR_EXT_ADDR_LATCH_00p.c());
  d("EXT DATA LATCH  : %c%c%c%c%c%c%c%c\n",
    SAZY_EXT_DATA_LATCH_D7n.c(), RUPA_EXT_DATA_LATCH_D6n.c(), SAGO_EXT_DATA_LATCH_D5n.c(), SODY_EXT_DATA_LATCH_D4n.c(),
    SELO_EXT_DATA_LATCH_D3n.c(), RAXY_EXT_DATA_LATCH_D2n.c(), RONY_EXT_DATA_LATCH_D1n.c(), SOMA_EXT_DATA_LATCH_D0n.c());
  d("\n");

}

//------------------------------------------------------------------------------

void ExtBus::tock(SchematicTop& top) {

  wire LUMA_DMA_READ_CARTp = top.dma_reg.LUMA_DMA_READ_CARTp();
  wire TUNA_0000_FDFFp = top.cpu_bus.TUNA_0000_FDFFp();
  wire TUTU_ADDR_BOOTp = top.TUTU_ADDR_BOOTp();

  /*p07.UBET*/ wire UBET_T1p = not1(top.clk_reg.SYS_PIN_T1n.tp());
  /*p07.UVAR*/ wire UVAR_T2p = not1(top.clk_reg.SYS_PIN_T2n.tp());
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and2(top.clk_reg.SYS_PIN_T2n.tp(), UBET_T1p);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and2(top.clk_reg.SYS_PIN_T1n.tp(), UVAR_T2p);
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n = not1(UNOR_MODE_DBG2p);

  //----------------------------------------
  // Ext pins


  {
    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15.tp()); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13.tp(), top.cpu_bus.CPU_BUS_A14.tp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXTp.tp(), TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.LEVO*/ wire LEVO_8000_9FFFp = not1(TEXO_8000_9FFFn);
    /*p08.LAGU*/ wire LAGU = and_or3(top.cpu_bus.CPU_PIN_RDp.tp(), LEVO_8000_9FFFp, top.cpu_bus.CPU_PIN_WRp.tp());
    /*p08.LYWE*/ wire LYWE = not1(LAGU);
    /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, LYWE);
    /*p08.TYMU*/ wire _TYMUEXT_PIN_RDn = nor2(LUMA_DMA_READ_CARTp, _MOTY_CPU_EXT_RD);
    /*p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMUEXT_PIN_RDn, TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMUEXT_PIN_RDn, UNOR_MODE_DBG2p);
    EXT_PIN_RDn = io_pin(_UGAC_RD_A, _URUN_RD_D);
  }

  {
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCxEFGH = nand2(top.cpu_bus.CPU_PIN_WRp.tp(), top.clk_reg.AFAS_xxxxEFGx());
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCxEFGH);
    /*p08.MEXO*/ wire MEXO_CPU_WRn_ABCDxxxH = not1(APOV_CPU_WRp_xxxxEFGx);

    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15.tp()); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13.tp(), top.cpu_bus.CPU_BUS_A14.tp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXTp.tp(), TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.NEVY*/ wire _NEVY = or2(MEXO_CPU_WRn_ABCDxxxH, _MOCA_DBG_EXT_RD);
    /*p08.PUVA*/ wire _PUVA_EXT_PIN_WRn = or2(_NEVY, LUMA_DMA_READ_CARTp);
    /*p08.UVER*/ wire _UVER_WR_A = nand2(_PUVA_EXT_PIN_WRn, TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVA_EXT_PIN_WRn, UNOR_MODE_DBG2p);
    EXT_PIN_WRn = io_pin(_UVER_WR_A, _USUF_WR_D);
  }

  {
#if 0
    /*p08.TYNU*/ wire TYNU_ADDR_RAM = (CPU_BUS_A15 && CPU_BUS_A14) || (CPU_BUS_A13 && !CPU_BUS_A14 && CPU_BUS_A15);
    EXT_PIN_CSn = nand(xxCDEFGH, CPU_PIN_ADDR_EXTp, TYNU_ADDR_RAM, 0000_FDFFp);
#endif

    /*#p01.AGUT*/ wire AGUT_xxCDEFGH = or_and3(top.clk_reg.AROV_xxCDEFxx.qp(), top.clk_reg.AJAX_xxxxEFGH.qp(), top.cpu_bus.CPU_PIN_ADDR_EXTp.tp());
    /*#p01.AWOD*/ wire AWOD_ABxxxxxx = nor2(UNOR_MODE_DBG2p, AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ wire ABUZ_xxCDEFGH = not1(AWOD_ABxxxxxx);

    /*p08.SOGY*/ wire _SOGY_A14n = not1(top.cpu_bus.CPU_BUS_A14.tp());
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and3(top.cpu_bus.CPU_BUS_A13.tp(), _SOGY_A14n, top.cpu_bus.CPU_BUS_A15.tp());
    /*p08.TYNU*/ wire TYNU_ADDR_RAM = and_or3(top.cpu_bus.CPU_BUS_A15.tp(), top.cpu_bus.CPU_BUS_A14.tp(), _TUMA_CART_RAM);

    /*p08.TOZA*/ wire _TOZA_EXT_PIN_CS_A_xxCDEFGH = and3(ABUZ_xxCDEFGH, TYNU_ADDR_RAM, TUNA_0000_FDFFp); // suggests ABUZp
    /*p08.TYHO*/ wire _TYHO_EXT_PIN_CS_A_xxCDEFGH = mux2_p(top.dma_reg.MARU_DMA_A15n.qn(), _TOZA_EXT_PIN_CS_A_xxCDEFGH, LUMA_DMA_READ_CARTp);
    EXT_PIN_CSn = io_pin(_TYHO_EXT_PIN_CS_A_xxCDEFGH, _TYHO_EXT_PIN_CS_A_xxCDEFGH);
  }

  //----------------------------------------
  // Ext address

  // DMA address / CPU address latch -> ext addr pins
  {
#if 0
    NYRE_EXT_ADDR_LATCH_14p = tp_latch_A(and(CPU_PIN_ADDR_EXTp, or(CPU_BUS_A13, CPU_BUS_A14, !CPU_BUS_A15)),
                                         CPU_BUS_A14);

#endif
    /*p08.MULE*/ wire MULE_MODE_DBG1n  = not1(UMUT_MODE_DBG1p);
    /*p08.SORE*/ wire SORE_0000_7FFFp  = not1(top.cpu_bus.CPU_BUS_A15.tp()); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn  = or3(top.cpu_bus.CPU_BUS_A13.tp(), top.cpu_bus.CPU_BUS_A14.tp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn  = and2(top.cpu_bus.CPU_PIN_ADDR_EXTp.tp(), TEVY_8000_9FFFn);
    /*p08.LOXO*/ wire LOXO_CBA_TO_EPAp = and_or3(MULE_MODE_DBG1n, TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.LASY*/ wire LASY_CBA_TO_EPAn = not1(LOXO_CBA_TO_EPAp);
    /*p08.MATE*/ wire MATE_CBA_TO_EPAp = not1(LASY_CBA_TO_EPAn);

    // Is this acutally a pass gate?

    /* p08.ALOR*/ ALOR_EXT_ADDR_LATCH_00p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A00.tp());
    /* p08.APUR*/ APUR_EXT_ADDR_LATCH_01p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A01.tp());
    /* p08.ALYR*/ ALYR_EXT_ADDR_LATCH_02p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A02.tp());
    /* p08.ARET*/ ARET_EXT_ADDR_LATCH_03p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A03.tp());
    /* p08.AVYS*/ AVYS_EXT_ADDR_LATCH_04p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A04.tp());
    /* p08.ATEV*/ ATEV_EXT_ADDR_LATCH_05p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A05.tp());
    /* p08.AROS*/ AROS_EXT_ADDR_LATCH_06p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A06.tp());
    /* p08.ARYM*/ ARYM_EXT_ADDR_LATCH_07p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A07.tp());
    /* p08.LUNO*/ LUNO_EXT_ADDR_LATCH_08p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A08.tp());
    /* p08.LYSA*/ LYSA_EXT_ADDR_LATCH_09p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A09.tp());
    /* p08.PATE*/ PATE_EXT_ADDR_LATCH_10p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A10.tp());
    /* p08.LUMY*/ LUMY_EXT_ADDR_LATCH_11p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A11.tp());
    /* p08.LOBU*/ LOBU_EXT_ADDR_LATCH_12p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A12.tp());
    /* p08.LONU*/ LONU_EXT_ADDR_LATCH_13p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A13.tp());
    /* p08.NYRE*/ NYRE_EXT_ADDR_LATCH_14p = tp_latch_A(MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A14.tp());


    /* p08.AMET*/ wire EXT_ADDR_00p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NAKY_DMA_A00p.qp(), ALOR_EXT_ADDR_LATCH_00p.tp());
    /* p08.ATOL*/ wire EXT_ADDR_01p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.PYRO_DMA_A01p.qp(), APUR_EXT_ADDR_LATCH_01p.tp());
    /* p08.APOK*/ wire EXT_ADDR_02p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NEFY_DMA_A02p.qp(), ALYR_EXT_ADDR_LATCH_02p.tp());
    /* p08.AMER*/ wire EXT_ADDR_03p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.MUTY_DMA_A03p.qp(), ARET_EXT_ADDR_LATCH_03p.tp());
    /* p08.ATEM*/ wire EXT_ADDR_04p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NYKO_DMA_A04p.qp(), AVYS_EXT_ADDR_LATCH_04p.tp());
    /* p08.ATOV*/ wire EXT_ADDR_05p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.PYLO_DMA_A05p.qp(), ATEV_EXT_ADDR_LATCH_05p.tp());
    /* p08.ATYR*/ wire EXT_ADDR_06p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NUTO_DMA_A06p.qp(), AROS_EXT_ADDR_LATCH_06p.tp());
    /*#p08.ASUR*/ wire EXT_ADDR_07p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.MUGU_DMA_A07p.qp(), ARYM_EXT_ADDR_LATCH_07p.tp());
    /*#p08.MANO*/ wire EXT_ADDR_08p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NAFA_DMA_A08n.qn(), LUNO_EXT_ADDR_LATCH_08p.tp());
    /* p08.MASU*/ wire EXT_ADDR_09p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.PYNE_DMA_A09n.qn(), LYSA_EXT_ADDR_LATCH_09p.tp());
    /* p08.PAMY*/ wire EXT_ADDR_10p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.PARA_DMA_A10n.qn(), PATE_EXT_ADDR_LATCH_10p.tp());
    /* p08.MALE*/ wire EXT_ADDR_11p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NYDO_DMA_A11n.qn(), LUMY_EXT_ADDR_LATCH_11p.tp());
    /* p08.MOJY*/ wire EXT_ADDR_12p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NYGY_DMA_A12n.qn(), LOBU_EXT_ADDR_LATCH_12p.tp());
    /* p08.MUCE*/ wire EXT_ADDR_13p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.PULA_DMA_A13n.qn(), LONU_EXT_ADDR_LATCH_13p.tp());
    /* p08.PEGE*/ wire EXT_ADDR_14p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.POKU_DMA_A14n.qn(), NYRE_EXT_ADDR_LATCH_14p.tp());

    /*p08.KUPO*/ wire KUPO = nand2(EXT_ADDR_00p, TOVA_MODE_DBG2n);
    /*p08.CABA*/ wire CABA = nand2(EXT_ADDR_01p, TOVA_MODE_DBG2n);
    /*p08.BOKU*/ wire BOKU = nand2(EXT_ADDR_02p, TOVA_MODE_DBG2n);
    /*p08.BOTY*/ wire BOTY = nand2(EXT_ADDR_03p, TOVA_MODE_DBG2n);
    /*p08.BYLA*/ wire BYLA = nand2(EXT_ADDR_04p, TOVA_MODE_DBG2n);
    /*p08.BADU*/ wire BADU = nand2(EXT_ADDR_05p, TOVA_MODE_DBG2n);
    /*p08.CEPU*/ wire CEPU = nand2(EXT_ADDR_06p, TOVA_MODE_DBG2n);
    /*p08.DEFY*/ wire DEFY = nand2(EXT_ADDR_07p, TOVA_MODE_DBG2n);
    /*p08.MYNY*/ wire MYNY = nand2(EXT_ADDR_08p, TOVA_MODE_DBG2n);
    /*p08.MUNE*/ wire MUNE = nand2(EXT_ADDR_09p, TOVA_MODE_DBG2n);
    /*p08.ROXU*/ wire ROXU = nand2(EXT_ADDR_10p, TOVA_MODE_DBG2n);
    /*p08.LEPY*/ wire LEPY = nand2(EXT_ADDR_11p, TOVA_MODE_DBG2n);
    /*p08.LUCE*/ wire LUCE = nand2(EXT_ADDR_12p, TOVA_MODE_DBG2n);
    /*p08.LABE*/ wire LABE = nand2(EXT_ADDR_13p, TOVA_MODE_DBG2n);
    /*p08.PUHE*/ wire PUHE = nand2(EXT_ADDR_14p, TOVA_MODE_DBG2n);

    /*p08.KOTY*/ wire KOTY = nor2 (EXT_ADDR_00p, UNOR_MODE_DBG2p);
    /*p08.COTU*/ wire COTU = nor2 (EXT_ADDR_01p, UNOR_MODE_DBG2p);
    /*p08.BAJO*/ wire BAJO = nor2 (EXT_ADDR_02p, UNOR_MODE_DBG2p);
    /*p08.BOLA*/ wire BOLA = nor2 (EXT_ADDR_03p, UNOR_MODE_DBG2p);
    /*p08.BEVO*/ wire BEVO = nor2 (EXT_ADDR_04p, UNOR_MODE_DBG2p);
    /*p08.AJAV*/ wire AJAV = nor2 (EXT_ADDR_05p, UNOR_MODE_DBG2p);
    /*p08.CYKA*/ wire CYKA = nor2 (EXT_ADDR_06p, UNOR_MODE_DBG2p);
    /*p08.COLO*/ wire COLO = nor2 (EXT_ADDR_07p, UNOR_MODE_DBG2p);
    /*p08.MEGO*/ wire MEGO = nor2 (EXT_ADDR_08p, UNOR_MODE_DBG2p);
    /*p08.MENY*/ wire MENY = nor2 (EXT_ADDR_09p, UNOR_MODE_DBG2p);
    /*p08.RORE*/ wire RORE = nor2 (EXT_ADDR_10p, UNOR_MODE_DBG2p);
    /*p08.LYNY*/ wire LYNY = nor2 (EXT_ADDR_11p, UNOR_MODE_DBG2p);
    /*p08.LOSO*/ wire LOSO = nor2 (EXT_ADDR_12p, UNOR_MODE_DBG2p);
    /*p08.LEVA*/ wire LEVA = nor2 (EXT_ADDR_13p, UNOR_MODE_DBG2p);
    /*p08.PAHY*/ wire PAHY = nor2 (EXT_ADDR_14p, UNOR_MODE_DBG2p);

    EXT_PIN_A00p = io_pin(KUPO, KOTY);
    EXT_PIN_A01p = io_pin(CABA, COTU);
    EXT_PIN_A02p = io_pin(BOKU, BAJO);
    EXT_PIN_A03p = io_pin(BOTY, BOLA);
    EXT_PIN_A04p = io_pin(BYLA, BEVO);
    EXT_PIN_A05p = io_pin(BADU, AJAV);
    EXT_PIN_A06p = io_pin(CEPU, CYKA);
    EXT_PIN_A07p = io_pin(DEFY, COLO);
    EXT_PIN_A08p = io_pin(MYNY, MEGO);
    EXT_PIN_A09p = io_pin(MUNE, MENY);
    EXT_PIN_A10p = io_pin(ROXU, RORE);
    EXT_PIN_A11p = io_pin(LEPY, LYNY);
    EXT_PIN_A12p = io_pin(LUCE, LOSO);
    EXT_PIN_A13p = io_pin(LABE, LEVA);
    EXT_PIN_A14p = io_pin(PUHE, PAHY);
  }

  {
    // A15 is "special"

#if 0
    EXT_PIN_A15n_A = and(xxCDEFGH, CPU_PIN_ADDR_EXTp, !top.cpu_bus.CPU_BUS_A15.tp(), !TUTU_ADDR_BOOTp);
    EXT_PIN_A15n_D = and(xxCDEFGH, CPU_PIN_ADDR_EXTp, !top.cpu_bus.CPU_BUS_A15.tp(), !TUTU_ADDR_BOOTp);
#endif

    /*#p01.AGUT*/ wire AGUT_xxCDEFGH = or_and3(top.clk_reg.AROV_xxCDEFxx.qp(), top.clk_reg.AJAX_xxxxEFGH.qp(), top.cpu_bus.CPU_PIN_ADDR_EXTp.tp());
    /*#p01.AWOD*/ wire AWOD_ABxxxxxx = nor2(UNOR_MODE_DBG2p, AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ wire ABUZ_xxCDEFGH = not1(AWOD_ABxxxxxx);

    /* p08.RYCA*/ wire RYCA_MODE_DBG2n = not1(UNOR_MODE_DBG2p);
    /* p08.SOBY*/ wire SOBY_A15n = nor2(top.cpu_bus.CPU_BUS_A15.tp(), TUTU_ADDR_BOOTp);
    /* p08.SEPY*/ wire SEPY_A15p_ABxxxxxx = nand2(ABUZ_xxCDEFGH, SOBY_A15n);
    /* p08.TAZY*/ wire TAZY_A15p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.MARU_DMA_A15n.qn(), SEPY_A15p_ABxxxxxx);

    /* p08.SUZE*/ wire SUZE_EXT_PIN_A15n = nand2(TAZY_A15p, RYCA_MODE_DBG2n);
    /* p08.RULO*/ wire RULO_EXT_PIN_A15n = nor2 (TAZY_A15p, UNOR_MODE_DBG2p);
    
    EXT_PIN_A15p = io_pin(SUZE_EXT_PIN_A15n, RULO_EXT_PIN_A15n);
  }

  //----------------------------------------
  // Ext data out

  // CPU data bus -> external data bus
  {
    // So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15.tp()); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13.tp(), top.cpu_bus.CPU_BUS_A14.tp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXTp.tp(), TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.LEVO*/ wire LEVO_8000_9FFFp = not1(TEXO_8000_9FFFn);
    /*p08.LAGU*/ wire LAGU = and_or3(top.cpu_bus.CPU_PIN_RDp.tp(), LEVO_8000_9FFFp, top.cpu_bus.CPU_PIN_WRp.tp());
    /*p08.LYWE*/ wire LYWE = not1(LAGU);
    /*p08.MOTY*/ wire MOTY_CPU_EXT_RD = or2(MOCA_DBG_EXT_RD, LYWE);

    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(!EXT_PIN_RDn.qp(), top.cpu_bus.CPU_PIN_RDp.tp(), UNOR_MODE_DBG2p);
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /*p08.REDU*/ wire REDU_CPU_RDn = not1(TEDO_CPU_RDp);
    /*p08.RORU*/ wire RORU_CBD_TO_EPDn = mux2_p(REDU_CPU_RDn, MOTY_CPU_EXT_RD, UNOR_MODE_DBG2p);
    /*p08.LULA*/ wire LULA_CBD_TO_EPDp = not1(RORU_CBD_TO_EPDn);

    /*p25.RUXA*/ wire RUXA = nand2(top.cpu_bus.CPU_BUS_D0p.tp(), LULA_CBD_TO_EPDp);
    /*p25.RUJA*/ wire RUJA = nand2(top.cpu_bus.CPU_BUS_D1p.tp(), LULA_CBD_TO_EPDp);
    /*p25.RABY*/ wire RABY = nand2(top.cpu_bus.CPU_BUS_D2p.tp(), LULA_CBD_TO_EPDp);
    /*p25.RERA*/ wire RERA = nand2(top.cpu_bus.CPU_BUS_D3p.tp(), LULA_CBD_TO_EPDp);
    /*p25.RORY*/ wire RORY = nand2(top.cpu_bus.CPU_BUS_D4p.tp(), LULA_CBD_TO_EPDp);
    /*p25.RYVO*/ wire RYVO = nand2(top.cpu_bus.CPU_BUS_D5p.tp(), LULA_CBD_TO_EPDp);
    /*p25.RAFY*/ wire RAFY = nand2(top.cpu_bus.CPU_BUS_D6p.tp(), LULA_CBD_TO_EPDp);
    /*p25.RAVU*/ wire RAVU = nand2(top.cpu_bus.CPU_BUS_D7p.tp(), LULA_CBD_TO_EPDp);

    /*p08.RUNE*/ wire RUNE = nor2 (top.cpu_bus.CPU_BUS_D0p.tp(), RORU_CBD_TO_EPDn);
    /*p08.RYPU*/ wire RYPU = nor2 (top.cpu_bus.CPU_BUS_D1p.tp(), RORU_CBD_TO_EPDn);
    /*p08.SULY*/ wire SULY = nor2 (top.cpu_bus.CPU_BUS_D2p.tp(), RORU_CBD_TO_EPDn);
    /*p08.SEZE*/ wire SEZE = nor2 (top.cpu_bus.CPU_BUS_D3p.tp(), RORU_CBD_TO_EPDn);
    /*p08.RESY*/ wire RESY = nor2 (top.cpu_bus.CPU_BUS_D4p.tp(), RORU_CBD_TO_EPDn);
    /*p08.TAMU*/ wire TAMU = nor2 (top.cpu_bus.CPU_BUS_D5p.tp(), RORU_CBD_TO_EPDn);
    /*p08.ROGY*/ wire ROGY = nor2 (top.cpu_bus.CPU_BUS_D6p.tp(), RORU_CBD_TO_EPDn);
    /*p08.RYDA*/ wire RYDA = nor2 (top.cpu_bus.CPU_BUS_D7p.tp(), RORU_CBD_TO_EPDn);

    EXT_PIN_D00p = io_pin(RUXA, RUNE, LULA_CBD_TO_EPDp);
    EXT_PIN_D01p = io_pin(RUJA, RYPU, LULA_CBD_TO_EPDp);
    EXT_PIN_D02p = io_pin(RABY, SULY, LULA_CBD_TO_EPDp);
    EXT_PIN_D03p = io_pin(RERA, SEZE, LULA_CBD_TO_EPDp);
    EXT_PIN_D04p = io_pin(RORY, RESY, LULA_CBD_TO_EPDp);
    EXT_PIN_D05p = io_pin(RYVO, TAMU, LULA_CBD_TO_EPDp);
    EXT_PIN_D06p = io_pin(RAFY, ROGY, LULA_CBD_TO_EPDp);
    EXT_PIN_D07p = io_pin(RAVU, RYDA, LULA_CBD_TO_EPDp);
  }

  //----------------------------------------
  // Ext data in
  {
    // -> ext bus
    /*p08.SORE*/ wire SORE_0000_7FFFp  = not1(top.cpu_bus.CPU_BUS_A15.tp()); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn  = or3(top.cpu_bus.CPU_BUS_A13.tp(), top.cpu_bus.CPU_BUS_A14.tp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn  = and2(top.cpu_bus.CPU_PIN_ADDR_EXTp.tp(), TEVY_8000_9FFFn);
    /*p08.LAVO*/ wire LAVO_EPD_TO_CBDn = nand3(top.cpu_bus.CPU_PIN_RDp.tp(), TEXO_8000_9FFFn, top.cpu_bus.CPU_PIN_LATCH_EXT.tp());

    // Ext pin -> Ext latch
    /*#p08.SOMA*/ SOMA_EXT_DATA_LATCH_D0n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D00p.qn());
    /* p08.RONY*/ RONY_EXT_DATA_LATCH_D1n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D01p.qn());
    /* p08.RAXY*/ RAXY_EXT_DATA_LATCH_D2n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D02p.qn());
    /* p08.SELO*/ SELO_EXT_DATA_LATCH_D3n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D03p.qn());
    /* p08.SODY*/ SODY_EXT_DATA_LATCH_D4n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D04p.qn());
    /* p08.SAGO*/ SAGO_EXT_DATA_LATCH_D5n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D05p.qn());
    /* p08.RUPA*/ RUPA_EXT_DATA_LATCH_D6n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D06p.qn());
    /* p08.SAZY*/ SAZY_EXT_DATA_LATCH_D7n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D07p.qn());

    // Ext latch -> int bus
    /*#p08.RYMA*/ top.cpu_bus.CPU_BUS_D0p = tribuf_6nn(LAVO_EPD_TO_CBDn, SOMA_EXT_DATA_LATCH_D0n.tp());
    /* p08.RUVO*/ top.cpu_bus.CPU_BUS_D1p = tribuf_6nn(LAVO_EPD_TO_CBDn, RONY_EXT_DATA_LATCH_D1n.tp());
    /* p08.RYKO*/ top.cpu_bus.CPU_BUS_D2p = tribuf_6nn(LAVO_EPD_TO_CBDn, RAXY_EXT_DATA_LATCH_D2n.tp());
    /* p08.TAVO*/ top.cpu_bus.CPU_BUS_D3p = tribuf_6nn(LAVO_EPD_TO_CBDn, SELO_EXT_DATA_LATCH_D3n.tp());
    /* p08.TEPE*/ top.cpu_bus.CPU_BUS_D4p = tribuf_6nn(LAVO_EPD_TO_CBDn, SODY_EXT_DATA_LATCH_D4n.tp());
    /* p08.SAFO*/ top.cpu_bus.CPU_BUS_D5p = tribuf_6nn(LAVO_EPD_TO_CBDn, SAGO_EXT_DATA_LATCH_D5n.tp());
    /* p08.SEVU*/ top.cpu_bus.CPU_BUS_D6p = tribuf_6nn(LAVO_EPD_TO_CBDn, RUPA_EXT_DATA_LATCH_D6n.tp());
    /* p08.TAJU*/ top.cpu_bus.CPU_BUS_D7p = tribuf_6nn(LAVO_EPD_TO_CBDn, SAZY_EXT_DATA_LATCH_D7n.tp());
  }
}

//------------------------------------------------------------------------------
