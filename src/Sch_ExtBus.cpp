#include "Sch_ExtBus.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void ExtBus::dump(Dumper& d) const {
  d("---------- Ext Bus  ----------\n");
  d("EXT BUS ADDR    : _%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    _NYRE_EXT_ADDR_LATCH_14.c(), _LONU_EXT_ADDR_LATCH_13.c(), _LOBU_EXT_ADDR_LATCH_12.c(), _LUMY_EXT_ADDR_LATCH_11.c(),
    _PATE_EXT_ADDR_LATCH_10.c(), _LYSA_EXT_ADDR_LATCH_09.c(), _LUNO_EXT_ADDR_LATCH_08.c(), _ARYM_EXT_ADDR_LATCH_07.c(),
    _AROS_EXT_ADDR_LATCH_06.c(), _ATEV_EXT_ADDR_LATCH_05.c(), _AVYS_EXT_ADDR_LATCH_04.c(), _ARET_EXT_ADDR_LATCH_03.c(),
    _ALYR_EXT_ADDR_LATCH_02.c(), _APUR_EXT_ADDR_LATCH_01.c(), _ALOR_EXT_ADDR_LATCH_00.c());
  d("EXT BUS DATA    : %c%c%c%c%c%c%c%c\n",
    _SAZY_EXT_DATA_LATCH_D7n.c(), _RUPA_EXT_DATA_LATCH_D6n.c(), _SAGO_EXT_DATA_LATCH_D5n.c(), _SODY_EXT_DATA_LATCH_D4n.c(),
    _SELO_EXT_DATA_LATCH_D3n.c(), _RAXY_EXT_DATA_LATCH_D2n.c(), _RONY_EXT_DATA_LATCH_D1n.c(), _SOMA_EXT_DATA_LATCH_D0n.c());
  d("\n");

  d("EXT PIN RD_A    : %c%c%c\n", EXT_PIN_RD_A.c(), EXT_PIN_RD_C.c(), EXT_PIN_RD_D.c());
  d("EXT PIN WR_A    : %c%c%c\n", EXT_PIN_WR_A.c(), EXT_PIN_WR_C.c(), EXT_PIN_WR_D.c());
  d("EXT PIN CS_A    : %c\n",     EXT_PIN_CS_A.c());

  d("EXT PIN ADDR_A  : %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    EXT_PIN_A15_A.c(), EXT_PIN_A14_A.c(), EXT_PIN_A13_A.c(), EXT_PIN_A12_A.c(),
    EXT_PIN_A11_A.c(), EXT_PIN_A10_A.c(), EXT_PIN_A09_A.c(), EXT_PIN_A08_A.c(),
    EXT_PIN_A07_A.c(), EXT_PIN_A06_A.c(), EXT_PIN_A05_A.c(), EXT_PIN_A04_A.c(),
    EXT_PIN_A03_A.c(), EXT_PIN_A02_A.c(), EXT_PIN_A01_A.c(), EXT_PIN_A00_A.c());

  d("EXT PIN DATA_A  : %c%c%c%c%c%c%c%c\n",
    EXT_PIN_D7_A.c(), EXT_PIN_D6_A.c(), EXT_PIN_D5_A.c(), EXT_PIN_D4_A.c(),
    EXT_PIN_D3_A.c(), EXT_PIN_D2_A.c(), EXT_PIN_D1_A.c(), EXT_PIN_D0_A.c());
  d("EXT PIN DATA_B  : %c%c%c%c%c%c%c%c\n",
    EXT_PIN_D7_B.c(), EXT_PIN_D6_B.c(), EXT_PIN_D5_B.c(), EXT_PIN_D4_B.c(),
    EXT_PIN_D3_B.c(), EXT_PIN_D2_B.c(), EXT_PIN_D1_B.c(), EXT_PIN_D0_B.c());
  d("EXT PIN DATA_C  : %c%c%c%c%c%c%c%c\n",
    EXT_PIN_D7n_C.c(), EXT_PIN_D6n_C.c(), EXT_PIN_D5n_C.c(), EXT_PIN_D4n_C.c(),
    EXT_PIN_D3n_C.c(), EXT_PIN_D2n_C.c(), EXT_PIN_D1n_C.c(), EXT_PIN_D0n_C.c());
  d("EXT PIN DATA_D  : %c%c%c%c%c%c%c%c\n",
    EXT_PIN_D7_D.c(), EXT_PIN_D6_D.c(), EXT_PIN_D5_D.c(), EXT_PIN_D4_D.c(),
    EXT_PIN_D3_D.c(), EXT_PIN_D2_D.c(), EXT_PIN_D1_D.c(), EXT_PIN_D0_D.c());

  d("\n");
}

//------------------------------------------------------------------------------

void ExtBus::tock(SchematicTop& top) {

  wire LUMA_DMA_READ_CARTp = top.dma_reg.LUMA_DMA_READ_CARTp();
  wire TUNA_0000_FDFFp = top.cpu_bus.TUNA_0000_FDFFp();
  wire TUTU_ADDR_BOOTp = top.TUTU_ADDR_BOOTp();

  /*p07.UBET*/ wire UBET_T1p = not1(top.clk_reg.SYS_PIN_T1n.qp());
  /*p07.UVAR*/ wire UVAR_T2p = not1(top.clk_reg.SYS_PIN_T2n.qp());
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and2(top.clk_reg.SYS_PIN_T2n.qp(), UBET_T1p);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and2(top.clk_reg.SYS_PIN_T1n.qp(), UVAR_T2p);
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n = not1(UNOR_MODE_DBG2p);

  //----------------------------------------
  // Ext pins

  {
    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15.qp()); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13.qp(), top.cpu_bus.CPU_BUS_A14.qp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXT.qp(), TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.LEVO*/ wire LEVO_8000_9FFFp = not1(TEXO_8000_9FFFn);
    /*p08.LAGU*/ wire LAGU = and_or3(top.cpu_bus.CPU_PIN_RDp.qp(), LEVO_8000_9FFFp, top.cpu_bus.CPU_PIN_WRp.qp());
    /*p08.LYWE*/ wire LYWE = not1(LAGU);
    /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, LYWE);
    /*p08.TYMU*/ wire _TYMUEXT_PIN_RDn = nor2(LUMA_DMA_READ_CARTp, _MOTY_CPU_EXT_RD);
    /*p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMUEXT_PIN_RDn, TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMUEXT_PIN_RDn, UNOR_MODE_DBG2p);
    EXT_PIN_RD_A = _UGAC_RD_A;
    EXT_PIN_RD_D = _URUN_RD_D;
  }

  {
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCxEFGH = nand2(top.cpu_bus.CPU_PIN_WRp.qp(), top.clk_reg.AFAS_xxxxEFGx());
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCxEFGH);
    /*p08.MEXO*/ wire MEXO_CPU_WRn_ABCDxxxH = not1(APOV_CPU_WRp_xxxxEFGx);

    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15.qp()); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13.qp(), top.cpu_bus.CPU_BUS_A14.qp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXT.qp(), TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.NEVY*/ wire _NEVY = or2(MEXO_CPU_WRn_ABCDxxxH, _MOCA_DBG_EXT_RD);
    /*p08.PUVA*/ wire _PUVA_EXT_PIN_WRn = or2(_NEVY, LUMA_DMA_READ_CARTp);
    /*p08.UVER*/ wire _UVER_WR_A = nand2(_PUVA_EXT_PIN_WRn, TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVA_EXT_PIN_WRn, UNOR_MODE_DBG2p);
    EXT_PIN_WR_A = _UVER_WR_A;
    EXT_PIN_WR_D = _USUF_WR_D;
  }

  {
    /*p01.AGUT*/ wire AGUT = or_and3(top.clk_reg.AROV_xxCDEFxx.qp(), top.clk_reg.AJAX_xxxxEFGH.qp(), top.cpu_bus.CPU_PIN_ADDR_EXT.qp());
    
    // if AFAS is (!A && B), AWOD looks like AFAS, this would be AWOD = (!UNOR_MODE_DBG2p && AGUT). Does that make sense?
    // CSn is definitely ABxxxxxxx relative to ext clock. So let's look at ext clock.

    /*p01.AWOD*/ wire AWOD = nor2(UNOR_MODE_DBG2p, AGUT);
    
    /*p01.ABUZ*/ wire ABUZ = not1(AWOD);

    /*p08.SOGY*/ wire _SOGY_A14n = not1(top.cpu_bus.CPU_BUS_A14.qp());
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and3(top.cpu_bus.CPU_BUS_A13.qp(), _SOGY_A14n, top.cpu_bus.CPU_BUS_A15.qp());
    /*p08.TYNU*/ wire TYNU_ADDR_RAM = and_or3(top.cpu_bus.CPU_BUS_A15.qp(), top.cpu_bus.CPU_BUS_A14.qp(), _TUMA_CART_RAM);

    /*p08.TOZA*/ wire _TOZA_EXT_PIN_CS_A = and3(ABUZ, TYNU_ADDR_RAM, TUNA_0000_FDFFp); // suggests ABUZp
    /*p08.TYHO*/ wire _TYHO_EXT_PIN_CS_A = mux2_p(top.dma_reg.MARU_DMA_A15.qn(), _TOZA_EXT_PIN_CS_A, LUMA_DMA_READ_CARTp);
    EXT_PIN_CS_A = _TYHO_EXT_PIN_CS_A;
  }

  //----------------------------------------
  // Ext address

  // DMA address / CPU address latch -> ext addr pins
  {
    /*p08.MULE*/ wire _MULE_MODE_DBG1n = not1(UMUT_MODE_DBG1p);
    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15.qp()); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13.qp(), top.cpu_bus.CPU_BUS_A14.qp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXT.qp(), TEVY_8000_9FFFn);
    /*p08.LOXO*/ wire _LOXO_CBA_TO_EPAp = and_or3(_MULE_MODE_DBG1n, TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.LASY*/ wire _LASY_CBA_TO_EPAn = not1(_LOXO_CBA_TO_EPAp);
    /*p08.MATE*/ wire _MATE_CBA_TO_EPAp = not1(_LASY_CBA_TO_EPAn);

    // Is this acutally a pass gate?

    /* p08.ALOR*/ _ALOR_EXT_ADDR_LATCH_00 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A00.qp());
    /* p08.APUR*/ _APUR_EXT_ADDR_LATCH_01 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A01.qp());
    /* p08.ALYR*/ _ALYR_EXT_ADDR_LATCH_02 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A02.qp());
    /* p08.ARET*/ _ARET_EXT_ADDR_LATCH_03 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A03.qp());
    /* p08.AVYS*/ _AVYS_EXT_ADDR_LATCH_04 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A04.qp());
    /* p08.ATEV*/ _ATEV_EXT_ADDR_LATCH_05 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A05.qp());
    /* p08.AROS*/ _AROS_EXT_ADDR_LATCH_06 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A06.qp());
    /* p08.ARYM*/ _ARYM_EXT_ADDR_LATCH_07 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A07.qp());
    /* p08.LUNO*/ _LUNO_EXT_ADDR_LATCH_08 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A08.qp());
    /* p08.LYSA*/ _LYSA_EXT_ADDR_LATCH_09 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A09.qp());
    /* p08.PATE*/ _PATE_EXT_ADDR_LATCH_10 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A10.qp());
    /* p08.LUMY*/ _LUMY_EXT_ADDR_LATCH_11 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A11.qp());
    /* p08.LOBU*/ _LOBU_EXT_ADDR_LATCH_12 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A12.qp());
    /* p08.LONU*/ _LONU_EXT_ADDR_LATCH_13 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A13.qp());
    /* p08.NYRE*/ _NYRE_EXT_ADDR_LATCH_14 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A14.qp());


    /* p08.AMET*/ wire _EXT_ADDR_00 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NAKY_DMA_A00p.qp(), _ALOR_EXT_ADDR_LATCH_00.qp());
    /* p08.ATOL*/ wire _EXT_ADDR_01 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.PYRO_DMA_A01p.qp(), _APUR_EXT_ADDR_LATCH_01.qp());
    /* p08.APOK*/ wire _EXT_ADDR_02 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NEFY_DMA_A02p.qp(), _ALYR_EXT_ADDR_LATCH_02.qp());
    /* p08.AMER*/ wire _EXT_ADDR_03 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.MUTY_DMA_A03p.qp(), _ARET_EXT_ADDR_LATCH_03.qp());
    /* p08.ATEM*/ wire _EXT_ADDR_04 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NYKO_DMA_A04p.qp(), _AVYS_EXT_ADDR_LATCH_04.qp());
    /* p08.ATOV*/ wire _EXT_ADDR_05 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.PYLO_DMA_A05p.qp(), _ATEV_EXT_ADDR_LATCH_05.qp());
    /* p08.ATYR*/ wire _EXT_ADDR_06 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NUTO_DMA_A06p.qp(), _AROS_EXT_ADDR_LATCH_06.qp());
    /*#p08.ASUR*/ wire _EXT_ADDR_07 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.MUGU_DMA_A07p.qp(), _ARYM_EXT_ADDR_LATCH_07.qp());
    /*#p08.MANO*/ wire _EXT_ADDR_08 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NAFA_DMA_A08.qn(), _LUNO_EXT_ADDR_LATCH_08.qp());
    /* p08.MASU*/ wire _EXT_ADDR_09 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.PYNE_DMA_A09.qn(), _LYSA_EXT_ADDR_LATCH_09.qp());
    /* p08.PAMY*/ wire _EXT_ADDR_10 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.PARA_DMA_A10.qn(), _PATE_EXT_ADDR_LATCH_10.qp());
    /* p08.MALE*/ wire _EXT_ADDR_11 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NYDO_DMA_A11.qn(), _LUMY_EXT_ADDR_LATCH_11.qp());
    /* p08.MOJY*/ wire _EXT_ADDR_12 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.NYGY_DMA_A12.qn(), _LOBU_EXT_ADDR_LATCH_12.qp());
    /* p08.MUCE*/ wire _EXT_ADDR_13 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.PULA_DMA_A13.qn(), _LONU_EXT_ADDR_LATCH_13.qp());
    /* p08.PEGE*/ wire _EXT_ADDR_14 = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.POKU_DMA_A14.qn(), _NYRE_EXT_ADDR_LATCH_14.qp());

    /*p08.KUPO*/ EXT_PIN_A00_A = nand2(_EXT_ADDR_00, TOVA_MODE_DBG2n);
    /*p08.CABA*/ EXT_PIN_A01_A = nand2(_EXT_ADDR_01, TOVA_MODE_DBG2n);
    /*p08.BOKU*/ EXT_PIN_A02_A = nand2(_EXT_ADDR_02, TOVA_MODE_DBG2n);
    /*p08.BOTY*/ EXT_PIN_A03_A = nand2(_EXT_ADDR_03, TOVA_MODE_DBG2n);
    /*p08.BYLA*/ EXT_PIN_A04_A = nand2(_EXT_ADDR_04, TOVA_MODE_DBG2n);
    /*p08.BADU*/ EXT_PIN_A05_A = nand2(_EXT_ADDR_05, TOVA_MODE_DBG2n);
    /*p08.CEPU*/ EXT_PIN_A06_A = nand2(_EXT_ADDR_06, TOVA_MODE_DBG2n);
    /*p08.DEFY*/ EXT_PIN_A07_A = nand2(_EXT_ADDR_07, TOVA_MODE_DBG2n);
    /*p08.MYNY*/ EXT_PIN_A08_A = nand2(_EXT_ADDR_08, TOVA_MODE_DBG2n);
    /*p08.MUNE*/ EXT_PIN_A09_A = nand2(_EXT_ADDR_09, TOVA_MODE_DBG2n);
    /*p08.ROXU*/ EXT_PIN_A10_A = nand2(_EXT_ADDR_10, TOVA_MODE_DBG2n);
    /*p08.LEPY*/ EXT_PIN_A11_A = nand2(_EXT_ADDR_11, TOVA_MODE_DBG2n);
    /*p08.LUCE*/ EXT_PIN_A12_A = nand2(_EXT_ADDR_12, TOVA_MODE_DBG2n);
    /*p08.LABE*/ EXT_PIN_A13_A = nand2(_EXT_ADDR_13, TOVA_MODE_DBG2n);
    /*p08.PUHE*/ EXT_PIN_A14_A = nand2(_EXT_ADDR_14, TOVA_MODE_DBG2n);

    /*p08.KOTY*/ EXT_PIN_A00_D = nor2 (_EXT_ADDR_00, UNOR_MODE_DBG2p);
    /*p08.COTU*/ EXT_PIN_A01_D = nor2 (_EXT_ADDR_01, UNOR_MODE_DBG2p);
    /*p08.BAJO*/ EXT_PIN_A02_D = nor2 (_EXT_ADDR_02, UNOR_MODE_DBG2p);
    /*p08.BOLA*/ EXT_PIN_A03_D = nor2 (_EXT_ADDR_03, UNOR_MODE_DBG2p);
    /*p08.BEVO*/ EXT_PIN_A04_D = nor2 (_EXT_ADDR_04, UNOR_MODE_DBG2p);
    /*p08.AJAV*/ EXT_PIN_A05_D = nor2 (_EXT_ADDR_05, UNOR_MODE_DBG2p);
    /*p08.CYKA*/ EXT_PIN_A06_D = nor2 (_EXT_ADDR_06, UNOR_MODE_DBG2p);
    /*p08.COLO*/ EXT_PIN_A07_D = nor2 (_EXT_ADDR_07, UNOR_MODE_DBG2p);
    /*p08.MEGO*/ EXT_PIN_A08_D = nor2 (_EXT_ADDR_08, UNOR_MODE_DBG2p);
    /*p08.MENY*/ EXT_PIN_A09_D = nor2 (_EXT_ADDR_09, UNOR_MODE_DBG2p);
    /*p08.RORE*/ EXT_PIN_A10_D = nor2 (_EXT_ADDR_10, UNOR_MODE_DBG2p);
    /*p08.LYNY*/ EXT_PIN_A11_D = nor2 (_EXT_ADDR_11, UNOR_MODE_DBG2p);
    /*p08.LOSO*/ EXT_PIN_A12_D = nor2 (_EXT_ADDR_12, UNOR_MODE_DBG2p);
    /*p08.LEVA*/ EXT_PIN_A13_D = nor2 (_EXT_ADDR_13, UNOR_MODE_DBG2p);
    /*p08.PAHY*/ EXT_PIN_A14_D = nor2 (_EXT_ADDR_14, UNOR_MODE_DBG2p);
  }

  {
    // A15 is "special"

    /*#p01.AGUT*/ wire AGUT_ABCDxxGH = or_and3(top.clk_reg.AROV_xxCDEFxx.qp(), top.clk_reg.AJAX_xxxxEFGH.qp(), top.cpu_bus.CPU_PIN_ADDR_EXT.qp());

    /* p01.AWOD*/ wire AWOD_xxxxEFxx = nor2(UNOR_MODE_DBG2p, AGUT_ABCDxxGH);
    /* p01.ABUZ*/ wire ABUZ_ABCDxxGH = not1(AWOD_xxxxEFxx);

    /* p08.RYCA*/ wire _RYCA_MODE_DBG2n = not1(UNOR_MODE_DBG2p);
    /* p08.SOBY*/ wire _SOBY_A15n = nor2(top.cpu_bus.CPU_BUS_A15.qp(), TUTU_ADDR_BOOTp);
    /* p08.SEPY*/ wire _SEPY_A15p = nand2(ABUZ_ABCDxxGH, _SOBY_A15n);
    /* p08.TAZY*/ wire _TAZY_A15p = mux2_p2(LUMA_DMA_READ_CARTp, top.dma_reg.MARU_DMA_A15.qn(), _SEPY_A15p);

    /* p08.SUZE*/ wire _SUZEEXT_PIN_A15n = nand2(_TAZY_A15p, _RYCA_MODE_DBG2n);
    /* p08.RULO*/ wire _RULOEXT_PIN_A15n = nor2 (_TAZY_A15p, UNOR_MODE_DBG2p);
    EXT_PIN_A15_A = _SUZEEXT_PIN_A15n;
    EXT_PIN_A15_D = _RULOEXT_PIN_A15n;
  }

  //----------------------------------------
  // Ext data out

  // CPU data bus -> external data bus
  {
    // So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15.qp()); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13.qp(), top.cpu_bus.CPU_BUS_A14.qp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXT.qp(), TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.LEVO*/ wire LEVO_8000_9FFFp = not1(TEXO_8000_9FFFn);
    /*p08.LAGU*/ wire LAGU = and_or3(top.cpu_bus.CPU_PIN_RDp.qp(), LEVO_8000_9FFFp, top.cpu_bus.CPU_PIN_WRp.qp());
    /*p08.LYWE*/ wire LYWE = not1(LAGU);
    /*p08.MOTY*/ wire MOTY_CPU_EXT_RD = or2(MOCA_DBG_EXT_RD, LYWE);

    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(EXT_PIN_RD_C.qp(), top.cpu_bus.CPU_PIN_RDp.qp(), UNOR_MODE_DBG2p);
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /*p08.REDU*/ wire _REDU_CPU_RDn = not1(TEDO_CPU_RDp);
    /*p08.RORU*/ wire _RORU_CBD_TO_EPDn = mux2_p(_REDU_CPU_RDn, MOTY_CPU_EXT_RD, UNOR_MODE_DBG2p);
    /*p08.LULA*/ wire _LULA_CBD_TO_EPDp = not1(_RORU_CBD_TO_EPDn);

    EXT_PIN_D0_B = _LULA_CBD_TO_EPDp;
    EXT_PIN_D1_B = _LULA_CBD_TO_EPDp;
    EXT_PIN_D2_B = _LULA_CBD_TO_EPDp;
    EXT_PIN_D3_B = _LULA_CBD_TO_EPDp;
    EXT_PIN_D4_B = _LULA_CBD_TO_EPDp;
    EXT_PIN_D5_B = _LULA_CBD_TO_EPDp;
    EXT_PIN_D6_B = _LULA_CBD_TO_EPDp;
    EXT_PIN_D7_B = _LULA_CBD_TO_EPDp;

    /*p25.RUXA*/ EXT_PIN_D0_A = nand2(top.cpu_bus.CPU_BUS_D0.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RUJA*/ EXT_PIN_D1_A = nand2(top.cpu_bus.CPU_BUS_D1.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RABY*/ EXT_PIN_D2_A = nand2(top.cpu_bus.CPU_BUS_D2.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RERA*/ EXT_PIN_D3_A = nand2(top.cpu_bus.CPU_BUS_D3.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RORY*/ EXT_PIN_D4_A = nand2(top.cpu_bus.CPU_BUS_D4.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RYVO*/ EXT_PIN_D5_A = nand2(top.cpu_bus.CPU_BUS_D5.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RAFY*/ EXT_PIN_D6_A = nand2(top.cpu_bus.CPU_BUS_D6.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RAVU*/ EXT_PIN_D7_A = nand2(top.cpu_bus.CPU_BUS_D7.qp(), _LULA_CBD_TO_EPDp);

    /*p08.RUNE*/ EXT_PIN_D0_D = nor2 (top.cpu_bus.CPU_BUS_D0.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RYPU*/ EXT_PIN_D1_D = nor2 (top.cpu_bus.CPU_BUS_D1.qp(), _RORU_CBD_TO_EPDn);
    /*p08.SULY*/ EXT_PIN_D2_D = nor2 (top.cpu_bus.CPU_BUS_D2.qp(), _RORU_CBD_TO_EPDn);
    /*p08.SEZE*/ EXT_PIN_D3_D = nor2 (top.cpu_bus.CPU_BUS_D3.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RESY*/ EXT_PIN_D4_D = nor2 (top.cpu_bus.CPU_BUS_D4.qp(), _RORU_CBD_TO_EPDn);
    /*p08.TAMU*/ EXT_PIN_D5_D = nor2 (top.cpu_bus.CPU_BUS_D5.qp(), _RORU_CBD_TO_EPDn);
    /*p08.ROGY*/ EXT_PIN_D6_D = nor2 (top.cpu_bus.CPU_BUS_D6.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RYDA*/ EXT_PIN_D7_D = nor2 (top.cpu_bus.CPU_BUS_D7.qp(), _RORU_CBD_TO_EPDn);
  }

  //----------------------------------------
  // Ext data in
  {
    // -> ext bus
    /*p08.SORE*/ wire SORE_0000_7FFFp  = not1(top.cpu_bus.CPU_BUS_A15.qp()); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn  = or3(top.cpu_bus.CPU_BUS_A13.qp(), top.cpu_bus.CPU_BUS_A14.qp(), SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn  = and2(top.cpu_bus.CPU_PIN_ADDR_EXT.qp(), TEVY_8000_9FFFn);
    /*p08.LAVO*/ wire LAVO_EPD_TO_CBDn = nand3(top.cpu_bus.CPU_PIN_RDp.qp(), TEXO_8000_9FFFn, top.cpu_bus.CPU_PIN_HOLD_MEM.qp());

    // Ext pin -> Ext latch
    /*#p08.SOMA*/ _SOMA_EXT_DATA_LATCH_D0n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D0n_C.qp());
    /* p08.RONY*/ _RONY_EXT_DATA_LATCH_D1n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D1n_C.qp());
    /* p08.RAXY*/ _RAXY_EXT_DATA_LATCH_D2n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D2n_C.qp());
    /* p08.SELO*/ _SELO_EXT_DATA_LATCH_D3n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D3n_C.qp());
    /* p08.SODY*/ _SODY_EXT_DATA_LATCH_D4n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D4n_C.qp());
    /* p08.SAGO*/ _SAGO_EXT_DATA_LATCH_D5n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D5n_C.qp());
    /* p08.RUPA*/ _RUPA_EXT_DATA_LATCH_D6n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D6n_C.qp());
    /* p08.SAZY*/ _SAZY_EXT_DATA_LATCH_D7n = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D7n_C.qp());

    // Ext latch -> int bus
    /*#p08.RYMA*/ top.cpu_bus.CPU_BUS_D0 = tribuf_6nn(LAVO_EPD_TO_CBDn, _SOMA_EXT_DATA_LATCH_D0n.qp());
    /* p08.RUVO*/ top.cpu_bus.CPU_BUS_D1 = tribuf_6nn(LAVO_EPD_TO_CBDn, _RONY_EXT_DATA_LATCH_D1n.qp());
    /* p08.RYKO*/ top.cpu_bus.CPU_BUS_D2 = tribuf_6nn(LAVO_EPD_TO_CBDn, _RAXY_EXT_DATA_LATCH_D2n.qp());
    /* p08.TAVO*/ top.cpu_bus.CPU_BUS_D3 = tribuf_6nn(LAVO_EPD_TO_CBDn, _SELO_EXT_DATA_LATCH_D3n.qp());
    /* p08.TEPE*/ top.cpu_bus.CPU_BUS_D4 = tribuf_6nn(LAVO_EPD_TO_CBDn, _SODY_EXT_DATA_LATCH_D4n.qp());
    /* p08.SAFO*/ top.cpu_bus.CPU_BUS_D5 = tribuf_6nn(LAVO_EPD_TO_CBDn, _SAGO_EXT_DATA_LATCH_D5n.qp());
    /* p08.SEVU*/ top.cpu_bus.CPU_BUS_D6 = tribuf_6nn(LAVO_EPD_TO_CBDn, _RUPA_EXT_DATA_LATCH_D6n.qp());
    /* p08.TAJU*/ top.cpu_bus.CPU_BUS_D7 = tribuf_6nn(LAVO_EPD_TO_CBDn, _SAZY_EXT_DATA_LATCH_D7n.qp());
  }
}

//------------------------------------------------------------------------------
