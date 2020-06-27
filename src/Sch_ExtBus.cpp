#include "Sch_ExtBus.h"

#include "TestGB.h"

using namespace Schematics;

void ExtBus::tick(TestGB& gb) {
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);

  auto& cpu_bus = gb.cpu_bus;
  auto& ext_bus = gb.ext_bus;

  //----------------------------------------

  // internal address bus to external address bus
  {
    // MORY00 << MATU17
    // MORY01
    // MORY02 
    // -> MORY reads DMA_RUNNING.q

    // arm on vcc side
    // LOXO01 << MULE02
    // LOXO02 << TEXO04
    // LOXO03 nc
    // LOXO04 << UMUT04
    // LOXO05 >> LASY01

    // Address bus to address pins mux
    /*p08.MULE*/ wire _MULE_MODE_DBG1n = not(dbg_sig.UMUT_MODE_DBG1p);
    /*p08.LOXO*/ wire _LOXO_LATCH_INT_ADDRp = or (and (_MULE_MODE_DBG1n, adr_sig.TEXO_8000_9FFFn), dbg_sig.UMUT_MODE_DBG1p);
    /*p08.LASY*/ wire _LASY_LATCH_INT_ADDRn = not(_LOXO_LATCH_INT_ADDRp);
    /*p08.MATE*/ wire _MATE_LATCH_INT_ADDRp = not(_LASY_LATCH_INT_ADDRn);

    // Pass gate?
    /*p08.ALOR*/ ext_bus.EXT_ADDR_LATCH_00.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A00);
    /*p08.APUR*/ ext_bus.EXT_ADDR_LATCH_01.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A01);
    /*p08.ALYR*/ ext_bus.EXT_ADDR_LATCH_02.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A02);
    /*p08.ARET*/ ext_bus.EXT_ADDR_LATCH_03.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A03);
    /*p08.AVYS*/ ext_bus.EXT_ADDR_LATCH_04.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A04);
    /*p08.ATEV*/ ext_bus.EXT_ADDR_LATCH_05.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A05);
    /*p08.AROS*/ ext_bus.EXT_ADDR_LATCH_06.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A06);
    /*p08.ARYM*/ ext_bus.EXT_ADDR_LATCH_07.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A07);
    /*p08.LUNO*/ ext_bus.EXT_ADDR_LATCH_08.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A08);
    /*p08.LYSA*/ ext_bus.EXT_ADDR_LATCH_09.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A09);
    /*p08.PATE*/ ext_bus.EXT_ADDR_LATCH_10.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A10);
    /*p08.LUMY*/ ext_bus.EXT_ADDR_LATCH_11.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A11);
    /*p08.LOBU*/ ext_bus.EXT_ADDR_LATCH_12.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A12);
    /*p08.LONU*/ ext_bus.EXT_ADDR_LATCH_13.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A13);
    /*p08.NYRE*/ ext_bus.EXT_ADDR_LATCH_14.tp_latch(_MATE_LATCH_INT_ADDRp, cpu_bus.PIN_A14);

    /*p08.AMET*/ wire EXT_ADDR_00 = mux2_p(dma_sig.DMA_A00, ext_bus.EXT_ADDR_LATCH_00, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATOL*/ wire EXT_ADDR_01 = mux2_p(dma_sig.DMA_A01, ext_bus.EXT_ADDR_LATCH_01, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.APOK*/ wire EXT_ADDR_02 = mux2_p(dma_sig.DMA_A02, ext_bus.EXT_ADDR_LATCH_02, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.AMER*/ wire EXT_ADDR_03 = mux2_p(dma_sig.DMA_A03, ext_bus.EXT_ADDR_LATCH_03, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATEM*/ wire EXT_ADDR_04 = mux2_p(dma_sig.DMA_A04, ext_bus.EXT_ADDR_LATCH_04, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATOV*/ wire EXT_ADDR_05 = mux2_p(dma_sig.DMA_A05, ext_bus.EXT_ADDR_LATCH_05, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATYR*/ wire EXT_ADDR_06 = mux2_p(dma_sig.DMA_A06, ext_bus.EXT_ADDR_LATCH_06, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ASUR*/ wire EXT_ADDR_07 = mux2_p(dma_sig.DMA_A07, ext_bus.EXT_ADDR_LATCH_07, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MANO*/ wire EXT_ADDR_08 = mux2_p(dma_sig.DMA_A08, ext_bus.EXT_ADDR_LATCH_08, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MASU*/ wire EXT_ADDR_09 = mux2_p(dma_sig.DMA_A09, ext_bus.EXT_ADDR_LATCH_09, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.PAMY*/ wire EXT_ADDR_10 = mux2_p(dma_sig.DMA_A10, ext_bus.EXT_ADDR_LATCH_10, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MALE*/ wire EXT_ADDR_11 = mux2_p(dma_sig.DMA_A11, ext_bus.EXT_ADDR_LATCH_11, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MOJY*/ wire EXT_ADDR_12 = mux2_p(dma_sig.DMA_A12, ext_bus.EXT_ADDR_LATCH_12, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MUCE*/ wire EXT_ADDR_13 = mux2_p(dma_sig.DMA_A13, ext_bus.EXT_ADDR_LATCH_13, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.PEGE*/ wire EXT_ADDR_14 = mux2_p(dma_sig.DMA_A14, ext_bus.EXT_ADDR_LATCH_14, dma_sig.LUMA_DMA_READ_CARTp);

    /*p08.KUPO*/ ext_bus.PIN_A00_A.set(nand(EXT_ADDR_00, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CABA*/ ext_bus.PIN_A01_A.set(nand(EXT_ADDR_01, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOKU*/ ext_bus.PIN_A02_A.set(nand(EXT_ADDR_02, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOTY*/ ext_bus.PIN_A03_A.set(nand(EXT_ADDR_03, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BYLA*/ ext_bus.PIN_A04_A.set(nand(EXT_ADDR_04, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BADU*/ ext_bus.PIN_A05_A.set(nand(EXT_ADDR_05, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CEPU*/ ext_bus.PIN_A06_A.set(nand(EXT_ADDR_06, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.DEFY*/ ext_bus.PIN_A07_A.set(nand(EXT_ADDR_07, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MYNY*/ ext_bus.PIN_A08_A.set(nand(EXT_ADDR_08, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MUNE*/ ext_bus.PIN_A09_A.set(nand(EXT_ADDR_09, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.ROXU*/ ext_bus.PIN_A10_A.set(nand(EXT_ADDR_10, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LEPY*/ ext_bus.PIN_A11_A.set(nand(EXT_ADDR_11, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LUCE*/ ext_bus.PIN_A12_A.set(nand(EXT_ADDR_12, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LABE*/ ext_bus.PIN_A13_A.set(nand(EXT_ADDR_13, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.PUHE*/ ext_bus.PIN_A14_A.set(nand(EXT_ADDR_14, dbg_sig.TOVA_MODE_DBG2n));

    /*p08.KOTY*/ ext_bus.PIN_A00_D.set(nor(EXT_ADDR_00, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COTU*/ ext_bus.PIN_A01_D.set(nor(EXT_ADDR_01, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BAJO*/ ext_bus.PIN_A02_D.set(nor(EXT_ADDR_02, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BOLA*/ ext_bus.PIN_A03_D.set(nor(EXT_ADDR_03, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BEVO*/ ext_bus.PIN_A04_D.set(nor(EXT_ADDR_04, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.AJAV*/ ext_bus.PIN_A05_D.set(nor(EXT_ADDR_05, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.CYKA*/ ext_bus.PIN_A06_D.set(nor(EXT_ADDR_06, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COLO*/ ext_bus.PIN_A07_D.set(nor(EXT_ADDR_07, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MEGO*/ ext_bus.PIN_A08_D.set(nor(EXT_ADDR_08, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MENY*/ ext_bus.PIN_A09_D.set(nor(EXT_ADDR_09, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.RORE*/ ext_bus.PIN_A10_D.set(nor(EXT_ADDR_10, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LYNY*/ ext_bus.PIN_A11_D.set(nor(EXT_ADDR_11, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LOSO*/ ext_bus.PIN_A12_D.set(nor(EXT_ADDR_12, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LEVA*/ ext_bus.PIN_A13_D.set(nor(EXT_ADDR_13, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.PAHY*/ ext_bus.PIN_A14_D.set(nor(EXT_ADDR_14, dbg_sig.UNOR_MODE_DBG2p));
  }

  // EXT -> CPU
  // external data bus to internal data bus
  {
    /*p08.LAVO*/ wire _LAVO_LATCH_EXT_DATAp = nand(cpu_bus.PIN_CPU_RAW_RD, adr_sig.TEXO_8000_9FFFn, cpu_bus.PIN_FROM_CPU5p);

    // SOMA01 << LAVO04
    // SOMA02 nc
    // SOMA03 << D0_C
    // SOMA04 nc
    // SOMA05 nc
    // SOMA06 nc
    // SOMA07 >> RYMA04
    // SOMA08 nc
    // SOMA09 == nc

    // Is this actually like a pass gate? We already know the latch cells, and this is bigger than those.

    /*p08.SOMA*/ ext_bus.EXT_DATA_LATCH_00.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.PIN_D0_C);
    /*p08.RONY*/ ext_bus.EXT_DATA_LATCH_01.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.PIN_D1_C);
    /*p08.RAXY*/ ext_bus.EXT_DATA_LATCH_02.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.PIN_D2_C);
    /*p08.SELO*/ ext_bus.EXT_DATA_LATCH_03.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.PIN_D3_C);
    /*p08.SODY*/ ext_bus.EXT_DATA_LATCH_04.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.PIN_D4_C);
    /*p08.SAGO*/ ext_bus.EXT_DATA_LATCH_05.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.PIN_D5_C);
    /*p08.RUPA*/ ext_bus.EXT_DATA_LATCH_06.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.PIN_D6_C);
    /*p08.SAZY*/ ext_bus.EXT_DATA_LATCH_07.tp_latch(_LAVO_LATCH_EXT_DATAp, ext_bus.PIN_D7_C);

    // RYMA 6-rung green tribuf

    /*p08.RYMA*/ cpu_bus.TS_D0.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_00);
    /*p08.RUVO*/ cpu_bus.TS_D1.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_01);
    /*p08.RYKO*/ cpu_bus.TS_D2.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_02);
    /*p08.TAVO*/ cpu_bus.TS_D3.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_03);
    /*p08.TEPE*/ cpu_bus.TS_D4.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_04);
    /*p08.SAFO*/ cpu_bus.TS_D5.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_05);
    /*p08.SEVU*/ cpu_bus.TS_D6.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_06);
    /*p08.TAJU*/ cpu_bus.TS_D7.set_tribuf(_LAVO_LATCH_EXT_DATAp, ext_bus.EXT_DATA_LATCH_07);
  }
}