#include "Sch_ExtBus.h"

#include "Sch_Top.h"

using namespace Schematics;

#if 0
    // cpu data driven to bus on EFGH during write

EXT_PIN_RD           = and2(              CPU_PIN_WRp,  CPU_PIN_ADDR_EXT, !ADDR_VRAM);
EXT_PIN_WR           = and2(              CPU_PIN_WRp,  CPU_PIN_ADDR_EXT, !ADDR_VRAM, AFAS_xxxxEFGx);
EXT_PIN_CS           = and2(                           !CPU_PIN_ADDR_EXT, A000_FFFF,  TUNA_0000_FDFFp);
MATE_LATCH_CPU_ADDRp = and2(                            CPU_PIN_ADDR_EXT, !ADDR_VRAM);
LATCH_CPU_DATA       = and2(CPU_PIN_RDp,                CPU_PIN_ADDR_EXT, !ADDR_VRAM, CPU_PIN_DV);
IBUS_TO_EBUSp        = and2(              CPU_PIN_WRp,  CPU_PIN_ADDR_EXT, !ADDR_VRAM);
EXT_PIN_A15_A        = and2(                           !CPU_PIN_ADDR_EXT, !A15);
EXT_PIN_A15_D        = and2(                           !CPU_PIN_ADDR_EXT, !A15);

#endif

//------------------------------------------------------------------------------

void ExtBus::tock(SchematicTop& top) {

  wire LUMA_DMA_READ_CARTp = top.dma_reg.LUMA_DMA_READ_CARTp();
  wire TUNA_0000_FDFFp = top.cpu_bus.TUNA_0000_FDFFp();
  wire TUTU_ADDR_BOOTp = top.TUTU_ADDR_BOOTp();

  /*p07.UBET*/ wire UBET_T1p = not1(top.clk_reg._SYS_PIN_T1n);
  /*p07.UVAR*/ wire UVAR_T2p = not1(top.clk_reg._SYS_PIN_T2n);
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and2(top.clk_reg._SYS_PIN_T2n, UBET_T1p);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and2(top.clk_reg._SYS_PIN_T1n, UVAR_T2p);
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n = not1(UNOR_MODE_DBG2p);

  //----------------------------------------
  // Ext pins

  {
#if 0

    SORE is NOT
    SORE_01 << A15
    SORE_02 >> TEVY_03

    TEVY is OR3
    TEVY_01 << A13
    TEVY_02 << A14
    TEVY_03 << SORE_02
    TEVY_04 nc
    TEVY_05 >> TEXO_02

    TEXO is AND2
    TEXO_01 << top right cpu port pin 7 
    TEXO_02 << TEVY_05
    TEXO_03 nc
    TEXO_04 >> LAVO_02, MOCA_01, LEVO_01, LOXO_02

    MOCA is NOR2
    MOCA_01 << TEXO_04
    MOCA_02 << UMUT_04
    MOCA_03 >> NEVY_02, MOTY_01

    LEVO is NOT
    LEVO_01 << TEXO_04
    LEVO_02 >> LAGU_02

    LAGU is (probably) or2(AND())
    LAGU_01 << CPU_PIN_RD
    LAGU_02 << LEVO_02
    LAGU_03 nc
    LAGU_04 << CPU_PIN_WR
    LAGU_05 >> LYWE_01

    LYWE is NOT
    LYWE_01 << LAGU_05
    LYWE_02 >> MOTY_02

    MOTY is OR
    MOTY_01 << MOCA_03
    MOTY_02 << LYWE_02
    MOTY_03 nc
    MOTY_04 >> TYMU_02, RORU_03

    TYMU is NOR2
    TYMU_01 << LUMA_02
    TYMU_02 << MOTY_03
    TYMU_03 >> UGAC_01, URUN_02

    UGAC is NAND2
    UGAC_01 << TYMU_03
    UGAC_02 << TOVA_02
    UGAC_03 >> RD_A

    URUN is NOR2
    URUN_01 << UNOR_04
    URUN_02 << TYMU_03
    URUN_03 >> RD_D

    SORE = not1(A15);
    TEVY = or2(A13, A14, SORE);
    TEXO = and2(CPU_PIN_ADDR_EXT, TEVY);
    MOCA = nor4(TEXO, UMUT)
    LEVO = not1(TEXO)
    LAGU = or2(and(CPU_PIN_RD, LEVO), CPU_PIN_WR);
    LYWE = not1(LAGU)
    MOTY = or2(MOCA, LYWE)
    TYMU = nor4(LUMA, MOTY)
    UGAC = nand2(TYMU, TOVA)
    URUN = nor4(UNOR, TYMU)
    RD_A = UGAC
    RD_D = URUN

    // so we're always reading _unless_ we have a valid write to not-vram
    EXT_PIN_RD_A = or2(!CPU_PIN_WR, !CPU_PIN_ADDR_EXT, ADDR_VRAM);

#endif

    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13, top.cpu_bus.CPU_BUS_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXT, TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.LEVO*/ wire LEVO_8000_9FFFp = not1(TEXO_8000_9FFFn);
    /*p08.LAGU*/ wire LAGU = or2(and2(top.cpu_bus.CPU_PIN_RDp, LEVO_8000_9FFFp), top.cpu_bus.CPU_PIN_WRp);
    /*p08.LYWE*/ wire LYWE = not1(LAGU);
    /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, LYWE);
    /*p08.TYMU*/ wire _TYMUEXT_PIN_RDn = nor2(LUMA_DMA_READ_CARTp, _MOTY_CPU_EXT_RD);
    /*p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMUEXT_PIN_RDn, TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMUEXT_PIN_RDn, UNOR_MODE_DBG2p);
    EXT_PIN_RD_A = _UGAC_RD_A;
    EXT_PIN_RD_D = _URUN_RD_D;
  }

  {
#if 0

    AREV is NAND2
    AREV_01 << AFAS_03
    AREV_02 << CPU_PIN_WR
    AREV_03 >> APOV_01

    APOV is NOT4
    APOV_01 << AREV_03
    APOV_02 nc
    APOV_03 nc
    APOV_04 >> MEXO_01

    MEXO is NOT2
    MEXO_01 << APOV_04
    MEXO_02 >> NEVY_01

    SORE is NOT
    SORE_01 << A15
    SORE_02 >> TEVY_03

    TEVY is OR3
    TEVY_01 << A13
    TEVY_02 << A14
    TEVY_03 << SORE_02
    TEVY_04 nc
    TEVY_05 >> TEXO_02

    TEXO is AND2
    TEXO_01 << top right cpu port pin 7 
    TEXO_02 << TEVY_05
    TEXO_03 nc
    TEXO_04 >> LAVO_02, MOCA_01, LEVO_01, LOXO_02

    MOCA is NOR2
    MOCA_01 << TEXO_04
    MOCA_02 << UMUT_04
    MOCA_03 >> NEVY_02, MOTY_01

    NEVY is OR2
    NEVY_01 << MEXO_02
    NEVY_02 << MOCA_03
    NEVY_03 nc
    NEVY_04 >> PUVA_01

    PUVA is OR2
    PUVA_01 << NEVY_04
    PUVA_02 << LUMA_02
    PUVA_03 nc
    PUVA_04 >> USUF_02, UVER_01

    UVER is NAND2
    UVER_01 << PUVA_04
    UVER_02 << TOVA_02
    UVER_03 >> WR_A

    USUF is NOR2
    USUF_01 << UNOR_04
    USUF_02 << PUVA_04
    USUF_03 >> WR_D

    AREV = nand2(AFAS, CPU_PIN_WR);
    APOV = not1(AREV);
    MEXO = not1(APOV);
    SORE = not1(A15);
    TEVY = or2(A13, A14, SORE);
    TEXO = and2(CPU_PIN_ADDR_EXT, TEVY);
    MOCA = nor4(TEXO, UMUT)
    NEVY = or2(MEXO, MOCA);
    PUVA = or2(NEVY, LUMA)
    UVER = nand2(PUVA, TOVA);
    USUF = nor4(UNOR, PUVA);
    WR_A = UVER;
    WR_D = USUF;


    EXT_PIN_WR_A = and2(AFAS, CPU_PIN_WR, CPU_PIN_ADDR_EXT, !ADDR_VRAM);

#endif

    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH = nand2(top.cpu_bus.CPU_PIN_WRp, top.clk_reg.AFAS_xxxxEFGx());
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCDxxxH);
    /*p08.MEXO*/ wire MEXO_CPU_WRn_ABCDxxxH = not1(APOV_CPU_WRp_xxxxEFGx);

    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13, top.cpu_bus.CPU_BUS_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXT, TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.NEVY*/ wire _NEVY = or2(MEXO_CPU_WRn_ABCDxxxH, _MOCA_DBG_EXT_RD);
    /*p08.PUVA*/ wire _PUVAEXT_PIN_WRn = or2(_NEVY, LUMA_DMA_READ_CARTp);
    /*p08.UVER*/ wire _UVER_WR_A = nand2(_PUVAEXT_PIN_WRn, TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVAEXT_PIN_WRn, UNOR_MODE_DBG2p);
    EXT_PIN_WR_A = _UVER_WR_A;
    EXT_PIN_WR_D = _USUF_WR_D;
  }

  {
#if 0
    AGUT is (probably) and2(OR())
    AGUT_01 << AROV
    AGUT_02 << AJAX
    AGUT_03 nc
    AGUT_04 << CPU_PIN_ADDR_EXT
    AGUT_05 >> AWOD_02

    AWOD is NOR
    AWOD_01 << UNOR
    AWOD_02 << AGUT_05
    AWOD_03 >> ABUZ_01

    ABUZ is NOT4
    ABUZ_01 << AWOD_03
    ABUZ_02 nc
    ABUZ_03 nc
    ABUZ_04 >> *

    CS_A = and2(xxCDEFGH, CPU_PIN_ADDR_EXT, A000_FDFF);

#endif

    /*p01.AGUT*/ wire AGUT = and2(or2(top.clk_reg.AROV_xxCDEFxx(), top.clk_reg.AJAX_xxxxEFGH()), top.cpu_bus.CPU_PIN_ADDR_EXT);
    /*p01.AWOD*/ wire AWOD = nor2(UNOR_MODE_DBG2p, AGUT);
    /*p01.ABUZ*/ wire ABUZ = not1(AWOD);

    /*p08.SOGY*/ wire _SOGY_A14n = not1(top.cpu_bus.CPU_BUS_A14);
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and3(top.cpu_bus.CPU_BUS_A13, _SOGY_A14n, top.cpu_bus.CPU_BUS_A15);
    /*p08.TYNU*/ wire TYNU_ADDR_RAM = or2(and2(top.cpu_bus.CPU_BUS_A15, top.cpu_bus.CPU_BUS_A14), _TUMA_CART_RAM);

    /*p08.TOZA*/ wire _TOZAEXT_PIN_CS_A = and3(ABUZ, TYNU_ADDR_RAM, TUNA_0000_FDFFp); // suggests ABUZp
    /*p08.TYHO*/ wire _TYHOEXT_PIN_CS_A = mux2_p(top.dma_reg.MARU_DMA_A15.q(), _TOZAEXT_PIN_CS_A, LUMA_DMA_READ_CARTp);
    EXT_PIN_CS_A = _TYHOEXT_PIN_CS_A;
  }

  //----------------------------------------
  // Ext address

  // DMA address / CPU address latch -> ext addr pins
  {
#if 0
    EXT_PIN_A00_A = not1(tp_latch(and(CPU_PIN_ADDR_EXT, !ADDR_VRAM), CPU_BUS_A00));

#endif

    /*p08.MULE*/ wire _MULE_MODE_DBG1n = not1(UMUT_MODE_DBG1p);
    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13, top.cpu_bus.CPU_BUS_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXT, TEVY_8000_9FFFn);
    /*p08.LOXO*/ wire _LOXO_CBA_TO_EPAp = or2(and2(_MULE_MODE_DBG1n, TEXO_8000_9FFFn), UMUT_MODE_DBG1p);
    /*p08.LASY*/ wire _LASY_CBA_TO_EPAn = not1(_LOXO_CBA_TO_EPAp);
    /*p08.MATE*/ wire _MATE_CBA_TO_EPAp = not1(_LASY_CBA_TO_EPAn);

    // Is this acutally a pass gate?

    /*p08.ALOR*/ _ALOR_EXT_ADDR_LATCH_00 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A00);
    /*p08.APUR*/ _APUR_EXT_ADDR_LATCH_01 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A01);
    /*p08.ALYR*/ _ALYR_EXT_ADDR_LATCH_02 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A02);
    /*p08.ARET*/ _ARET_EXT_ADDR_LATCH_03 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A03);
    /*p08.AVYS*/ _AVYS_EXT_ADDR_LATCH_04 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A04);
    /*p08.ATEV*/ _ATEV_EXT_ADDR_LATCH_05 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A05);
    /*p08.AROS*/ _AROS_EXT_ADDR_LATCH_06 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A06);
    /*p08.ARYM*/ _ARYM_EXT_ADDR_LATCH_07 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A07);
    /*p08.LUNO*/ _LUNO_EXT_ADDR_LATCH_08 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A08);
    /*p08.LYSA*/ _LYSA_EXT_ADDR_LATCH_09 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A09);
    /*p08.PATE*/ _PATE_EXT_ADDR_LATCH_10 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A10);
    /*p08.LUMY*/ _LUMY_EXT_ADDR_LATCH_11 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A11);
    /*p08.LOBU*/ _LOBU_EXT_ADDR_LATCH_12 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A12);
    /*p08.LONU*/ _LONU_EXT_ADDR_LATCH_13 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A13);
    /*p08.NYRE*/ _NYRE_EXT_ADDR_LATCH_14 = tp_latch_A(_MATE_CBA_TO_EPAp, top.cpu_bus.CPU_BUS_A14);

    /*p08.AMET*/ wire _EXT_ADDR_00 = mux2_p(top.dma_reg.NAKY_DMA_A00.q(), _ALOR_EXT_ADDR_LATCH_00, LUMA_DMA_READ_CARTp);
    /*p08.ATOL*/ wire _EXT_ADDR_01 = mux2_p(top.dma_reg.PYRO_DMA_A01.q(), _APUR_EXT_ADDR_LATCH_01, LUMA_DMA_READ_CARTp);
    /*p08.APOK*/ wire _EXT_ADDR_02 = mux2_p(top.dma_reg.NEFY_DMA_A02.q(), _ALYR_EXT_ADDR_LATCH_02, LUMA_DMA_READ_CARTp);
    /*p08.AMER*/ wire _EXT_ADDR_03 = mux2_p(top.dma_reg.MUTY_DMA_A03.q(), _ARET_EXT_ADDR_LATCH_03, LUMA_DMA_READ_CARTp);
    /*p08.ATEM*/ wire _EXT_ADDR_04 = mux2_p(top.dma_reg.NYKO_DMA_A04.q(), _AVYS_EXT_ADDR_LATCH_04, LUMA_DMA_READ_CARTp);
    /*p08.ATOV*/ wire _EXT_ADDR_05 = mux2_p(top.dma_reg.PYLO_DMA_A05.q(), _ATEV_EXT_ADDR_LATCH_05, LUMA_DMA_READ_CARTp);
    /*p08.ATYR*/ wire _EXT_ADDR_06 = mux2_p(top.dma_reg.NUTO_DMA_A06.q(), _AROS_EXT_ADDR_LATCH_06, LUMA_DMA_READ_CARTp);
    /*p08.ASUR*/ wire _EXT_ADDR_07 = mux2_p(top.dma_reg.MUGU_DMA_A07.q(), _ARYM_EXT_ADDR_LATCH_07, LUMA_DMA_READ_CARTp);
    /*p08.MANO*/ wire _EXT_ADDR_08 = mux2_p(top.dma_reg.NAFA_DMA_A08.q(), _LUNO_EXT_ADDR_LATCH_08, LUMA_DMA_READ_CARTp);
    /*p08.MASU*/ wire _EXT_ADDR_09 = mux2_p(top.dma_reg.PYNE_DMA_A09.q(), _LYSA_EXT_ADDR_LATCH_09, LUMA_DMA_READ_CARTp);
    /*p08.PAMY*/ wire _EXT_ADDR_10 = mux2_p(top.dma_reg.PARA_DMA_A10.q(), _PATE_EXT_ADDR_LATCH_10, LUMA_DMA_READ_CARTp);
    /*p08.MALE*/ wire _EXT_ADDR_11 = mux2_p(top.dma_reg.NYDO_DMA_A11.q(), _LUMY_EXT_ADDR_LATCH_11, LUMA_DMA_READ_CARTp);
    /*p08.MOJY*/ wire _EXT_ADDR_12 = mux2_p(top.dma_reg.NYGY_DMA_A12.q(), _LOBU_EXT_ADDR_LATCH_12, LUMA_DMA_READ_CARTp);
    /*p08.MUCE*/ wire _EXT_ADDR_13 = mux2_p(top.dma_reg.PULA_DMA_A13.q(), _LONU_EXT_ADDR_LATCH_13, LUMA_DMA_READ_CARTp);
    /*p08.PEGE*/ wire _EXT_ADDR_14 = mux2_p(top.dma_reg.POKU_DMA_A14.q(), _NYRE_EXT_ADDR_LATCH_14, LUMA_DMA_READ_CARTp);

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

#if 0

    EXT_PIN_A15_A = and2(xxCDEFGH, CPU_PIN_ADDR_EXT, !A15);
    EXT_PIN_A15_D = and2(xxCDEFGH, CPU_PIN_ADDR_EXT, !A15);

#endif

    /*p01.AGUT*/ wire AGUT = and2(or2(top.clk_reg.AROV_xxCDEFxx(), top.clk_reg.AJAX_xxxxEFGH()), top.cpu_bus.CPU_PIN_ADDR_EXT);
    /*p01.AWOD*/ wire AWOD = nor2(UNOR_MODE_DBG2p, AGUT);
    /*p01.ABUZ*/ wire ABUZ = not1(AWOD);

    /*p08.RYCA*/ wire _RYCA_MODE_DBG2n = not1(UNOR_MODE_DBG2p);
    /*p08.SOBY*/ wire _SOBY_A15n = nor2(top.cpu_bus.CPU_BUS_A15, TUTU_ADDR_BOOTp);
    /*p08.SEPY*/ wire _SEPY_A15p = nand2(ABUZ, _SOBY_A15n);
    /*p08.TAZY*/ wire _TAZY_A15p = mux2_p(top.dma_reg.MARU_DMA_A15.q(), _SEPY_A15p, LUMA_DMA_READ_CARTp);
    /*p08.SUZE*/ wire _SUZEEXT_PIN_A15n = nand2(_TAZY_A15p, _RYCA_MODE_DBG2n);
    /*p08.RULO*/ wire _RULOEXT_PIN_A15n = nor2 (_TAZY_A15p, UNOR_MODE_DBG2p);

    EXT_PIN_A15_A = _SUZEEXT_PIN_A15n;
    EXT_PIN_A15_D = _RULOEXT_PIN_A15n;
  }

  //----------------------------------------
  // Ext data out

  // CPU data bus -> external data bus
  {
#if 0

    SORE = not1(A15);
    TEVY = or2(A13, A14, SORE);
    TEXO = and2(CPU_PIN_ADDR_EXT, TEVY);
    MOCA = nor4(TEXO, UMUT)
    NEVY = or2(MEXO, MOCA);
    LAGU = or2(and(CPU_PIN_RD, LEVO), CPU_PIN_WR);
    LYWE = not1(LAGU)
    MOTY = or2(MOCA, LYWE)

    UJYV is MUX5
    UJYV_01 << UNOR_04
    UJYV_02 << RD_C
    UJYV_03 << CPU_PIN_RD
    UJYV_04 nc
    UJYV_05 >> TEDO_01

    TEDO is NOT4
    TEDO_01 << UJYV_05
    TEDO_02 nc
    TEDO_03 nc
    TEDO_04 >> TEDA_02, TUBY_02, UFEG_02, REDU_01, SORA_02, *

    REDU is NOT
    REDU_01 << TEDO_04
    REDU_02 >> RORU_02

    RORU is MUX6
    RORU_01 << UNOR_04
    RORU_02 << REDU_02
    RORU_03 << MOTY_04
    RORU_04 nc
    RORU_05 nc 
    RORU_06 >> LULA_01, *

    LULA is NOT
    LULA_01 << RORU_06
    LULA_02 >> D_B

    RUXA is NAND
    RUXA_01 << CPU_D0
    RUXA_02 << LULA_02
    RUXA_03 >> D0_A


    UJYV = mux5n(UNOR, RD_C, CPU_PIN_RD);
    TEDO = not1(UJYV)
    REDU = not1(TEDO)
    RORU = mux6p(UNOR, REDU, MOTY);
    LULA = not1(RORU)
    RUXA = nand2(CPU_D0, LULA);
    D0_A = RUXA;
    

    // if RORU selects MOTY:

    D0_A = or2(!CPU_D0, !CPU_PIN_WR, ADDR_VRAM, !CPU_PIN_ADDR_EXT);


    LULA_CBD_TO_EPDp = CPU_PIN_WRp;
    if (ADDR_VRAM) LULA_CBD_TO_EPDp = 0;
    if (!CPU_PIN_ADDR_EXT) LULA_CBD_TO_EPDp = 0;

    D*_B = and2(CPU_PIN_WRp, CPU_PIN_ADDR_EXT, !ADDR_VRAM);

#endif

    // So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13, top.cpu_bus.CPU_BUS_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXT, TEVY_8000_9FFFn);
    /*p08.MOCA*/ wire MOCA_DBG_EXT_RD = nor2(TEXO_8000_9FFFn, UMUT_MODE_DBG1p);
    /*p08.LEVO*/ wire LEVO_8000_9FFFp = not1(TEXO_8000_9FFFn);
    /*p08.LAGU*/ wire LAGU = or2(and2(top.cpu_bus.CPU_PIN_RDp, LEVO_8000_9FFFp), top.cpu_bus.CPU_PIN_WRp);
    /*p08.LYWE*/ wire LYWE = not1(LAGU);
    /*p08.MOTY*/ wire MOTY_CPU_EXT_RD = or2(MOCA_DBG_EXT_RD, LYWE);

    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(EXT_PIN_RD_C, top.cpu_bus.CPU_PIN_RDp, UNOR_MODE_DBG2p);
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

    /*p25.RUXA*/ EXT_PIN_D0_A = nand2(top.cpu_bus.CPU_BUS_D0, _LULA_CBD_TO_EPDp);
    /*p25.RUJA*/ EXT_PIN_D1_A = nand2(top.cpu_bus.CPU_BUS_D1, _LULA_CBD_TO_EPDp);
    /*p25.RABY*/ EXT_PIN_D2_A = nand2(top.cpu_bus.CPU_BUS_D2, _LULA_CBD_TO_EPDp);
    /*p25.RERA*/ EXT_PIN_D3_A = nand2(top.cpu_bus.CPU_BUS_D3, _LULA_CBD_TO_EPDp);
    /*p25.RORY*/ EXT_PIN_D4_A = nand2(top.cpu_bus.CPU_BUS_D4, _LULA_CBD_TO_EPDp);
    /*p25.RYVO*/ EXT_PIN_D5_A = nand2(top.cpu_bus.CPU_BUS_D5, _LULA_CBD_TO_EPDp);
    /*p25.RAFY*/ EXT_PIN_D6_A = nand2(top.cpu_bus.CPU_BUS_D6, _LULA_CBD_TO_EPDp);
    /*p25.RAVU*/ EXT_PIN_D7_A = nand2(top.cpu_bus.CPU_BUS_D7, _LULA_CBD_TO_EPDp);

    /*p08.RUNE*/ EXT_PIN_D0_D = nor2 (top.cpu_bus.CPU_BUS_D0, _RORU_CBD_TO_EPDn);
    /*p08.RYPU*/ EXT_PIN_D1_D = nor2 (top.cpu_bus.CPU_BUS_D1, _RORU_CBD_TO_EPDn);
    /*p08.SULY*/ EXT_PIN_D2_D = nor2 (top.cpu_bus.CPU_BUS_D2, _RORU_CBD_TO_EPDn);
    /*p08.SEZE*/ EXT_PIN_D3_D = nor2 (top.cpu_bus.CPU_BUS_D3, _RORU_CBD_TO_EPDn);
    /*p08.RESY*/ EXT_PIN_D4_D = nor2 (top.cpu_bus.CPU_BUS_D4, _RORU_CBD_TO_EPDn);
    /*p08.TAMU*/ EXT_PIN_D5_D = nor2 (top.cpu_bus.CPU_BUS_D5, _RORU_CBD_TO_EPDn);
    /*p08.ROGY*/ EXT_PIN_D6_D = nor2 (top.cpu_bus.CPU_BUS_D6, _RORU_CBD_TO_EPDn);
    /*p08.RYDA*/ EXT_PIN_D7_D = nor2 (top.cpu_bus.CPU_BUS_D7, _RORU_CBD_TO_EPDn);
  }

  //----------------------------------------
  // Ext data in
  {
#if 0

    LAVO is NAND3

    LAVO_01 << CPU_PIN_RD
    LAVO_02 << TEXO_04
    LAVO_03 << CPU_PIN_DV
    LAVO_04 >>

    // SOMA = latch(LAVO, EXT_PIN_D0_C)
    // RYMA = tribuf_6n(LAVO, SOMA_08)
#endif

    // -> ext bus
    /*p08.SORE*/ wire SORE_0000_7FFFp = not1(top.cpu_bus.CPU_BUS_A15); 
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or3(top.cpu_bus.CPU_BUS_A13, top.cpu_bus.CPU_BUS_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and2(top.cpu_bus.CPU_PIN_ADDR_EXT, TEVY_8000_9FFFn);
    /*p08.LAVO*/ wire LAVO_EPD_TO_CBDn = nand3(top.cpu_bus.CPU_PIN_RDp, TEXO_8000_9FFFn, top.cpu_bus.CPU_PIN_HOLD_MEM);



    // Ext pin -> Ext latch
    /*p08.SOMA*/ _SOMA_EXT_DATA_LATCH_00 = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D0_C);
    /*p08.RONY*/ _RONY_EXT_DATA_LATCH_01 = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D1_C);
    /*p08.RAXY*/ _RAXY_EXT_DATA_LATCH_02 = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D2_C);
    /*p08.SELO*/ _SELO_EXT_DATA_LATCH_03 = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D3_C);
    /*p08.SODY*/ _SODY_EXT_DATA_LATCH_04 = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D4_C);
    /*p08.SAGO*/ _SAGO_EXT_DATA_LATCH_05 = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D5_C);
    /*p08.RUPA*/ _RUPA_EXT_DATA_LATCH_06 = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D6_C);
    /*p08.SAZY*/ _SAZY_EXT_DATA_LATCH_07 = tp_latch_A(LAVO_EPD_TO_CBDn, EXT_PIN_D7_C);

    // Ext latch -> int bus
    /*p08.RYMA*/ top.cpu_bus.CPU_BUS_D0 = tribuf_6n(LAVO_EPD_TO_CBDn, _SOMA_EXT_DATA_LATCH_00);
    /*p08.RUVO*/ top.cpu_bus.CPU_BUS_D1 = tribuf_6n(LAVO_EPD_TO_CBDn, _RONY_EXT_DATA_LATCH_01);
    /*p08.RYKO*/ top.cpu_bus.CPU_BUS_D2 = tribuf_6n(LAVO_EPD_TO_CBDn, _RAXY_EXT_DATA_LATCH_02);
    /*p08.TAVO*/ top.cpu_bus.CPU_BUS_D3 = tribuf_6n(LAVO_EPD_TO_CBDn, _SELO_EXT_DATA_LATCH_03);
    /*p08.TEPE*/ top.cpu_bus.CPU_BUS_D4 = tribuf_6n(LAVO_EPD_TO_CBDn, _SODY_EXT_DATA_LATCH_04);
    /*p08.SAFO*/ top.cpu_bus.CPU_BUS_D5 = tribuf_6n(LAVO_EPD_TO_CBDn, _SAGO_EXT_DATA_LATCH_05);
    /*p08.SEVU*/ top.cpu_bus.CPU_BUS_D6 = tribuf_6n(LAVO_EPD_TO_CBDn, _RUPA_EXT_DATA_LATCH_06);
    /*p08.TAJU*/ top.cpu_bus.CPU_BUS_D7 = tribuf_6n(LAVO_EPD_TO_CBDn, _SAZY_EXT_DATA_LATCH_07);
  }
}

//------------------------------------------------------------------------------
