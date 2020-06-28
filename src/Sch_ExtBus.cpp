#include "Sch_ExtBus.h"

#include "TestGB.h"

using namespace Schematics;

//------------------------------------------------------------------------------

ExtBusSignals ExtBus::sig(const TestGB& /*gb*/) const {
  return {
    .PIN_D0_C = PIN_D0_C,
    .PIN_D1_C = PIN_D1_C,
    .PIN_D2_C = PIN_D2_C,
    .PIN_D3_C = PIN_D3_C,
    .PIN_D4_C = PIN_D4_C,
    .PIN_D5_C = PIN_D5_C,
    .PIN_D6_C = PIN_D6_C,
    .PIN_D7_C = PIN_D7_C,
  };
}

//------------------------------------------------------------------------------

void ExtBus::tick(TestGB& gb) {
  auto dma_sig = gb.dma_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto clk_sig = gb.clk_reg.sig(gb);

  auto& cpu_bus = gb.cpu_bus;
  auto& dma_reg = gb.dma_reg;

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


    // Pass gate?
    /*p08.ALOR*/ CPU_ADDR_LATCH_00.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A00);
    /*p08.APUR*/ CPU_ADDR_LATCH_01.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A01);
    /*p08.ALYR*/ CPU_ADDR_LATCH_02.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A02);
    /*p08.ARET*/ CPU_ADDR_LATCH_03.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A03);
    /*p08.AVYS*/ CPU_ADDR_LATCH_04.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A04);
    /*p08.ATEV*/ CPU_ADDR_LATCH_05.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A05);
    /*p08.AROS*/ CPU_ADDR_LATCH_06.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A06);
    /*p08.ARYM*/ CPU_ADDR_LATCH_07.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A07);
    /*p08.LUNO*/ CPU_ADDR_LATCH_08.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A08);
    /*p08.LYSA*/ CPU_ADDR_LATCH_09.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A09);
    /*p08.PATE*/ CPU_ADDR_LATCH_10.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A10);
    /*p08.LUMY*/ CPU_ADDR_LATCH_11.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A11);
    /*p08.LOBU*/ CPU_ADDR_LATCH_12.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A12);
    /*p08.LONU*/ CPU_ADDR_LATCH_13.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A13);
    /*p08.NYRE*/ CPU_ADDR_LATCH_14.tp_latch(cpu_sig.MATE_LATCH_CPU_ADDRp, cpu_bus.PIN_A14);

    /*p08.AMET*/ wire EXT_ADDR_00 = mux2_p(dma_sig.DMA_A00, CPU_ADDR_LATCH_00, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATOL*/ wire EXT_ADDR_01 = mux2_p(dma_sig.DMA_A01, CPU_ADDR_LATCH_01, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.APOK*/ wire EXT_ADDR_02 = mux2_p(dma_sig.DMA_A02, CPU_ADDR_LATCH_02, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.AMER*/ wire EXT_ADDR_03 = mux2_p(dma_sig.DMA_A03, CPU_ADDR_LATCH_03, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATEM*/ wire EXT_ADDR_04 = mux2_p(dma_sig.DMA_A04, CPU_ADDR_LATCH_04, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATOV*/ wire EXT_ADDR_05 = mux2_p(dma_sig.DMA_A05, CPU_ADDR_LATCH_05, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATYR*/ wire EXT_ADDR_06 = mux2_p(dma_sig.DMA_A06, CPU_ADDR_LATCH_06, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ASUR*/ wire EXT_ADDR_07 = mux2_p(dma_sig.DMA_A07, CPU_ADDR_LATCH_07, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MANO*/ wire EXT_ADDR_08 = mux2_p(dma_sig.DMA_A08, CPU_ADDR_LATCH_08, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MASU*/ wire EXT_ADDR_09 = mux2_p(dma_sig.DMA_A09, CPU_ADDR_LATCH_09, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.PAMY*/ wire EXT_ADDR_10 = mux2_p(dma_sig.DMA_A10, CPU_ADDR_LATCH_10, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MALE*/ wire EXT_ADDR_11 = mux2_p(dma_sig.DMA_A11, CPU_ADDR_LATCH_11, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MOJY*/ wire EXT_ADDR_12 = mux2_p(dma_sig.DMA_A12, CPU_ADDR_LATCH_12, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MUCE*/ wire EXT_ADDR_13 = mux2_p(dma_sig.DMA_A13, CPU_ADDR_LATCH_13, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.PEGE*/ wire EXT_ADDR_14 = mux2_p(dma_sig.DMA_A14, CPU_ADDR_LATCH_14, dma_sig.LUMA_DMA_READ_CARTp);

    /*p08.KUPO*/ PIN_A00_A.set(nand(EXT_ADDR_00, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CABA*/ PIN_A01_A.set(nand(EXT_ADDR_01, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOKU*/ PIN_A02_A.set(nand(EXT_ADDR_02, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOTY*/ PIN_A03_A.set(nand(EXT_ADDR_03, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BYLA*/ PIN_A04_A.set(nand(EXT_ADDR_04, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BADU*/ PIN_A05_A.set(nand(EXT_ADDR_05, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CEPU*/ PIN_A06_A.set(nand(EXT_ADDR_06, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.DEFY*/ PIN_A07_A.set(nand(EXT_ADDR_07, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MYNY*/ PIN_A08_A.set(nand(EXT_ADDR_08, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MUNE*/ PIN_A09_A.set(nand(EXT_ADDR_09, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.ROXU*/ PIN_A10_A.set(nand(EXT_ADDR_10, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LEPY*/ PIN_A11_A.set(nand(EXT_ADDR_11, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LUCE*/ PIN_A12_A.set(nand(EXT_ADDR_12, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LABE*/ PIN_A13_A.set(nand(EXT_ADDR_13, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.PUHE*/ PIN_A14_A.set(nand(EXT_ADDR_14, dbg_sig.TOVA_MODE_DBG2n));

    /*p08.KOTY*/ PIN_A00_D.set(nor(EXT_ADDR_00, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COTU*/ PIN_A01_D.set(nor(EXT_ADDR_01, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BAJO*/ PIN_A02_D.set(nor(EXT_ADDR_02, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BOLA*/ PIN_A03_D.set(nor(EXT_ADDR_03, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BEVO*/ PIN_A04_D.set(nor(EXT_ADDR_04, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.AJAV*/ PIN_A05_D.set(nor(EXT_ADDR_05, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.CYKA*/ PIN_A06_D.set(nor(EXT_ADDR_06, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COLO*/ PIN_A07_D.set(nor(EXT_ADDR_07, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MEGO*/ PIN_A08_D.set(nor(EXT_ADDR_08, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MENY*/ PIN_A09_D.set(nor(EXT_ADDR_09, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.RORE*/ PIN_A10_D.set(nor(EXT_ADDR_10, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LYNY*/ PIN_A11_D.set(nor(EXT_ADDR_11, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LOSO*/ PIN_A12_D.set(nor(EXT_ADDR_12, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LEVA*/ PIN_A13_D.set(nor(EXT_ADDR_13, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.PAHY*/ PIN_A14_D.set(nor(EXT_ADDR_14, dbg_sig.UNOR_MODE_DBG2p));
  }

  // EXT -> CPU
  // external data bus to internal data bus
  {
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

    /*p08.SOMA*/ CPU_DATA_LATCH_00.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, PIN_D0_C);
    /*p08.RONY*/ CPU_DATA_LATCH_01.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, PIN_D1_C);
    /*p08.RAXY*/ CPU_DATA_LATCH_02.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, PIN_D2_C);
    /*p08.SELO*/ CPU_DATA_LATCH_03.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, PIN_D3_C);
    /*p08.SODY*/ CPU_DATA_LATCH_04.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, PIN_D4_C);
    /*p08.SAGO*/ CPU_DATA_LATCH_05.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, PIN_D5_C);
    /*p08.RUPA*/ CPU_DATA_LATCH_06.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, PIN_D6_C);
    /*p08.SAZY*/ CPU_DATA_LATCH_07.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, PIN_D7_C);

    // RYMA 6-rung green tribuf

    /*p08.RYMA*/ cpu_bus.TS_D0.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, CPU_DATA_LATCH_00);
    /*p08.RUVO*/ cpu_bus.TS_D1.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, CPU_DATA_LATCH_01);
    /*p08.RYKO*/ cpu_bus.TS_D2.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, CPU_DATA_LATCH_02);
    /*p08.TAVO*/ cpu_bus.TS_D3.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, CPU_DATA_LATCH_03);
    /*p08.TEPE*/ cpu_bus.TS_D4.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, CPU_DATA_LATCH_04);
    /*p08.SAFO*/ cpu_bus.TS_D5.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, CPU_DATA_LATCH_05);
    /*p08.SEVU*/ cpu_bus.TS_D6.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, CPU_DATA_LATCH_06);
    /*p08.TAJU*/ cpu_bus.TS_D7.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, CPU_DATA_LATCH_07);
  }

  // internal data bus to external data bus
  {
    /*p08.LULA*/ wire _LULA_IBUS_TO_EBUSp = not(cpu_sig.RORU_IBUS_TO_EBUSn);
    PIN_D0_B.set(_LULA_IBUS_TO_EBUSp);
    PIN_D1_B.set(_LULA_IBUS_TO_EBUSp);
    PIN_D2_B.set(_LULA_IBUS_TO_EBUSp);
    PIN_D3_B.set(_LULA_IBUS_TO_EBUSp);
    PIN_D4_B.set(_LULA_IBUS_TO_EBUSp);
    PIN_D5_B.set(_LULA_IBUS_TO_EBUSp);
    PIN_D6_B.set(_LULA_IBUS_TO_EBUSp);
    PIN_D7_B.set(_LULA_IBUS_TO_EBUSp);

    /*p25.RUXA*/ PIN_D0_A.set(nand(cpu_bus.TS_D0, _LULA_IBUS_TO_EBUSp));
    /*p25.RUJA*/ PIN_D1_A.set(nand(cpu_bus.TS_D1, _LULA_IBUS_TO_EBUSp));
    /*p25.RABY*/ PIN_D2_A.set(nand(cpu_bus.TS_D2, _LULA_IBUS_TO_EBUSp));
    /*p25.RERA*/ PIN_D3_A.set(nand(cpu_bus.TS_D3, _LULA_IBUS_TO_EBUSp));
    /*p25.RORY*/ PIN_D4_A.set(nand(cpu_bus.TS_D4, _LULA_IBUS_TO_EBUSp));
    /*p25.RYVO*/ PIN_D5_A.set(nand(cpu_bus.TS_D5, _LULA_IBUS_TO_EBUSp));
    /*p25.RAFY*/ PIN_D6_A.set(nand(cpu_bus.TS_D6, _LULA_IBUS_TO_EBUSp));
    /*p25.RAVU*/ PIN_D7_A.set(nand(cpu_bus.TS_D7, _LULA_IBUS_TO_EBUSp));

    /*p08.RUNE*/ PIN_D0_D.set(nor(cpu_bus.TS_D0, cpu_sig.RORU_IBUS_TO_EBUSn));
    /*p08.RYPU*/ PIN_D1_D.set(nor(cpu_bus.TS_D1, cpu_sig.RORU_IBUS_TO_EBUSn));
    /*p08.SULY*/ PIN_D2_D.set(nor(cpu_bus.TS_D2, cpu_sig.RORU_IBUS_TO_EBUSn));
    /*p08.SEZE*/ PIN_D3_D.set(nor(cpu_bus.TS_D3, cpu_sig.RORU_IBUS_TO_EBUSn));
    /*p08.RESY*/ PIN_D4_D.set(nor(cpu_bus.TS_D4, cpu_sig.RORU_IBUS_TO_EBUSn));
    /*p08.TAMU*/ PIN_D5_D.set(nor(cpu_bus.TS_D5, cpu_sig.RORU_IBUS_TO_EBUSn));
    /*p08.ROGY*/ PIN_D6_D.set(nor(cpu_bus.TS_D6, cpu_sig.RORU_IBUS_TO_EBUSn));
    /*p08.RYDA*/ PIN_D7_D.set(nor(cpu_bus.TS_D7, cpu_sig.RORU_IBUS_TO_EBUSn));
  }

  // Something weird here
  /*p07.TERA*/ wire _TERA_BOOT_BITp  = not(gb.bootrom.BOOT_BITn);
  /*p07.TULO*/ wire _TULO_ADDR_00XXp = nor(cpu_bus.PIN_A15, cpu_bus.PIN_A14, cpu_bus.PIN_A13, cpu_bus.PIN_A12, cpu_bus.PIN_A11, cpu_bus.PIN_A10, cpu_bus.PIN_A09, cpu_bus.PIN_A08);
  /*p07.TUTU*/ wire _TUTU_ADDR_BOOTp = and (_TERA_BOOT_BITp, _TULO_ADDR_00XXp);
  /*p08.SOBY*/ wire _SOBY = nor(cpu_bus.PIN_A15, _TUTU_ADDR_BOOTp);
  /*p08.SEPY*/ wire _SEPY = nand(dbg_sig.ABUZ, _SOBY);
  /*p08.TAZY*/ wire _TAZY = mux2_p(dma_reg.DMA_A15.q(), _SEPY, dma_sig.LUMA_DMA_READ_CARTp);
  /*p08.SUZE*/ PIN_A15_A.set(nand(_TAZY, dbg_sig.RYCA_MODE_DBG2n));
  /*p08.RULO*/ PIN_A15_D.set(nor (_TAZY, dbg_sig.UNOR_MODE_DBG2p));

  {
    /* PIN_75 */ PIN_PHI.set(clk_sig.PIN_BUDE_AxxxxFGH);
  }

  {
    /*p08.TYMU*/ wire _TYMU_RD_OUTn = nor(dma_sig.LUMA_DMA_READ_CARTp, cpu_sig.MOTY_CPU_EXT_RD);

    /*p08.UGAC*/ wire _UGAC_RDp_A = nand(_TYMU_RD_OUTn, dbg_sig.TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire _URUN_RDp_D = nor (_TYMU_RD_OUTn, dbg_sig.UNOR_MODE_DBG2p);
    /* PIN_79 */ PIN_RD_A.set(_UGAC_RDp_A);
    /* PIN_79 */ PIN_RD_D.set(_URUN_RDp_D);
  }

  {
    /*p08.MEXO*/ wire _MEXO_ABCDExxx = not(cpu_sig.APOV_CPU_WR_xxxxxFGH);
    /*p08.NEVY*/ wire _NEVY = or(_MEXO_ABCDExxx, cpu_sig.MOCA_DBG_EXT_RD);
    /*p08.PUVA*/ wire _PUVA_WR_OUTn = or(_NEVY, dma_sig.LUMA_DMA_READ_CARTp);

    /*p08.UVER*/ wire _UVER_WRp_A = nand(_PUVA_WR_OUTn, dbg_sig.TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire _USUF_WRp_D = nor (_PUVA_WR_OUTn, dbg_sig.UNOR_MODE_DBG2p);
    /* PIN_78 */ PIN_WR_A.set(_UVER_WRp_A);
    /* PIN_78 */ PIN_WR_D.set(_USUF_WRp_D);
  }

  {
    // if TYNU is and(or()) things don't make sense.
    ///*p08.TYNU*/ wire TYNU_ADDR_RAM = and(ADDR >= 0x4000, TUMA_CART_RAM);

    // Die trace:
    // TOZA = and(TYNU, ABUZ, TUNA);
    // TYHO = mux2_p(LUMA, MARU.QN?, TOZA);

    // TOZA = address valid, address ram, address not highmem
    // The A15 in the other half of the mux is weird.

    /*p08.SOGY*/ wire _SOGY_A14n = not(cpu_bus.PIN_A14);
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and(cpu_bus.PIN_A13, _SOGY_A14n, cpu_bus.PIN_A15);

    // TYNU 5-rung
    // TYNU01

    /*p08.TYNU*/ wire _TYNU_ADDR_RAM = or(and(cpu_bus.PIN_A15, cpu_bus.PIN_A14), _TUMA_CART_RAM);


    /*p08.TOZA*/ wire _TOZA = and(dbg_sig.ABUZ, _TYNU_ADDR_RAM, adr_sig.TUNA_0000_FDFFp); // suggests ABUZp
    /*p08.TYHO*/ wire _TYHO_CS_A = mux2_p(dma_reg.DMA_A15.q(), _TOZA, dma_sig.LUMA_DMA_READ_CARTp); // ABxxxxxx

    /* PIN_80 */ PIN_CS_A.set(_TYHO_CS_A);
  }

}

//------------------------------------------------------------------------------

bool ExtBus::commit() {
  bool changed = false;

  /* PIN_71 */ changed |= PIN_RST.clear_preset();
  /* PIN_72 */ /*GND*/
  /* PIN_73 */ /*CLKOUT*/
  /* PIN_74 */ changed |= PIN_CLK_GOOD.clear_preset();
  /* PIN_74 */ changed |= PIN_CLK_IN_xBxDxFxH.clear_preset();
  /* PIN_75 */ changed |= PIN_PHI.commit_pinout();     // <- BUDE/BEVA
  /* PIN_76 */ changed |= PIN_T2.clear_preset();
  /* PIN_77 */ changed |= PIN_T1.clear_preset();
  /* PIN_78 */ changed |= PIN_WR_A.commit_pinout();    // <- UVER
  /* PIN_78 */ changed |= PIN_WR_C.clear_preset();     // -> UBAL
  /* PIN_78 */ changed |= PIN_WR_D.commit_pinout();    // <- USUF
  /* PIN_79 */ changed |= PIN_RD_A.commit_pinout();    // <- UGAC
  /* PIN_79 */ changed |= PIN_RD_C.clear_preset();     // -> UJYV
  /* PIN_79 */ changed |= PIN_RD_D.commit_pinout();    // <- URUN
  /* PIN_80 */ changed |= PIN_CS_A.commit_pinout();    // <- TYHO


  /*p08.ALOR*/ changed |= CPU_ADDR_LATCH_00.commit_latch();
  /*p08.APUR*/ changed |= CPU_ADDR_LATCH_01.commit_latch();
  /*p08.ALYR*/ changed |= CPU_ADDR_LATCH_02.commit_latch();
  /*p08.ARET*/ changed |= CPU_ADDR_LATCH_03.commit_latch();
  /*p08.AVYS*/ changed |= CPU_ADDR_LATCH_04.commit_latch();
  /*p08.ATEV*/ changed |= CPU_ADDR_LATCH_05.commit_latch();
  /*p08.AROS*/ changed |= CPU_ADDR_LATCH_06.commit_latch();
  /*p08.ARYM*/ changed |= CPU_ADDR_LATCH_07.commit_latch();
  /*p08.LUNO*/ changed |= CPU_ADDR_LATCH_08.commit_latch();
  /*p08.LYSA*/ changed |= CPU_ADDR_LATCH_09.commit_latch();
  /*p08.PATE*/ changed |= CPU_ADDR_LATCH_10.commit_latch();
  /*p08.LUMY*/ changed |= CPU_ADDR_LATCH_11.commit_latch();
  /*p08.LOBU*/ changed |= CPU_ADDR_LATCH_12.commit_latch();
  /*p08.LONU*/ changed |= CPU_ADDR_LATCH_13.commit_latch();
  /*p08.NYRE*/ changed |= CPU_ADDR_LATCH_14.commit_latch();

  /*p08.SOMA*/ changed |= CPU_DATA_LATCH_00.commit_latch();
  /*p08.RONY*/ changed |= CPU_DATA_LATCH_01.commit_latch();
  /*p08.RAXY*/ changed |= CPU_DATA_LATCH_02.commit_latch();
  /*p08.SELO*/ changed |= CPU_DATA_LATCH_03.commit_latch();
  /*p08.SODY*/ changed |= CPU_DATA_LATCH_04.commit_latch();
  /*p08.SAGO*/ changed |= CPU_DATA_LATCH_05.commit_latch();
  /*p08.RUPA*/ changed |= CPU_DATA_LATCH_06.commit_latch();
  /*p08.SAZY*/ changed |= CPU_DATA_LATCH_07.commit_latch();

  /* PIN_01 */ changed |= PIN_A00_A.commit_pinout();     // <- KUPO
  /* PIN_01 */ changed |= PIN_A00_C.clear_preset();      // -> KOVA
  /* PIN_01 */ changed |= PIN_A00_D.commit_pinout();     // <- KOTY
  /* PIN_02 */ changed |= PIN_A01_A.commit_pinout();     // <- CABA
  /* PIN_02 */ changed |= PIN_A01_C.clear_preset();      // -> CAMU
  /* PIN_02 */ changed |= PIN_A01_D.commit_pinout();     // <- COTU
  /* PIN_03 */ changed |= PIN_A02_A.commit_pinout();     // <- BOKU
  /* PIN_03 */ changed |= PIN_A02_C.clear_preset();      // -> BUXU
  /* PIN_03 */ changed |= PIN_A02_D.commit_pinout();     // <- BAJO
  /* PIN_04 */ changed |= PIN_A03_A.commit_pinout();     // <- BOTY
  /* PIN_04 */ changed |= PIN_A03_C.clear_preset();      // -> BASE
  /* PIN_04 */ changed |= PIN_A03_D.commit_pinout();     // <- BOLA
  /* PIN_05 */ changed |= PIN_A04_A.commit_pinout();     // <- BYLA
  /* PIN_05 */ changed |= PIN_A04_C.clear_preset();      // -> AFEC
  /* PIN_05 */ changed |= PIN_A04_D.commit_pinout();     // <- BEVO
  /* PIN_06 */ changed |= PIN_A05_A.commit_pinout();     // <- BADU
  /* PIN_06 */ changed |= PIN_A05_C.clear_preset();      // -> ABUP
  /* PIN_06 */ changed |= PIN_A05_D.commit_pinout();     // <- AJAV
  /* PIN_07 */ changed |= PIN_A06_A.commit_pinout();     // <- CEPU
  /* PIN_07 */ changed |= PIN_A06_C.clear_preset();      // -> CYGU
  /* PIN_07 */ changed |= PIN_A06_D.commit_pinout();     // <- CYKA
  /* PIN_08 */ changed |= PIN_A07_A.commit_pinout();     // <- DEFY
  /* PIN_08 */ changed |= PIN_A07_C.clear_preset();      // -> COGO
  /* PIN_08 */ changed |= PIN_A07_D.commit_pinout();     // <- COLO
  /* PIN_09 */ changed |= PIN_A08_A.commit_pinout();     // <- MYNY
  /* PIN_09 */ changed |= PIN_A08_C.clear_preset();      // -> MUJY
  /* PIN_09 */ changed |= PIN_A08_D.commit_pinout();     // <- MEGO
  /* PIN_10 */ changed |= PIN_A09_A.commit_pinout();     // <- MUNE
  /* PIN_10 */ changed |= PIN_A09_C.clear_preset();      // -> NENA
  /* PIN_10 */ changed |= PIN_A09_D.commit_pinout();     // <- MENY
  /* PIN_11 */ changed |= PIN_A10_A.commit_pinout();     // <- ROXU
  /* PIN_11 */ changed |= PIN_A10_C.clear_preset();      // -> SURA
  /* PIN_11 */ changed |= PIN_A10_D.commit_pinout();     // <- RORE
  /* PIN_12 */ changed |= PIN_A11_A.commit_pinout();     // <- LEPY
  /* PIN_12 */ changed |= PIN_A11_C.clear_preset();      // -> MADY
  /* PIN_12 */ changed |= PIN_A11_D.commit_pinout();     // <- LYNY
  /* PIN_13 */ changed |= PIN_A12_A.commit_pinout();     // <- LUCE
  /* PIN_13 */ changed |= PIN_A12_C.clear_preset();      // -> LAHE
  /* PIN_13 */ changed |= PIN_A12_D.commit_pinout();     // <- LOSO
  /* PIN_14 */ changed |= PIN_A13_A.commit_pinout();     // <- LABE
  /* PIN_14 */ changed |= PIN_A13_C.clear_preset();      // -> LURA
  /* PIN_14 */ changed |= PIN_A13_D.commit_pinout();     // <- LEVA
  /* PIN_15 */ changed |= PIN_A14_A.commit_pinout();     // <- PUHE
  /* PIN_15 */ changed |= PIN_A14_C.clear_preset();      // -> PEVO
  /* PIN_15 */ changed |= PIN_A14_D.commit_pinout();     // <- PAHY
  /* PIN_16 */ changed |= PIN_A15_A.commit_pinout();     // <- SUZE
  /* PIN_16 */ changed |= PIN_A15_C.clear_preset();      // -> RAZA
  /* PIN_16 */ changed |= PIN_A15_D.commit_pinout();     // <- RULO
  /* PIN_17 */ changed |= PIN_D0_A.commit_pinout();      // <- RUXA
  /* PIN_17 */ changed |= PIN_D0_B.commit_pinout();      // <- LULA
  /* PIN_17 */ changed |= PIN_D0_C.clear_preset();       // -> TOVO,SOMA
  /* PIN_17 */ changed |= PIN_D0_D.commit_pinout();      // <- RUNE
  /* PIN_18 */ changed |= PIN_D1_A.commit_pinout();      // <- RUJA
  /* PIN_18 */ changed |= PIN_D1_B.commit_pinout();      // <- LULA
  /* PIN_18 */ changed |= PIN_D1_C.clear_preset();       // -> RUZY,RONY
  /* PIN_18 */ changed |= PIN_D1_D.commit_pinout();      // <- RYPU
  /* PIN_19 */ changed |= PIN_D2_A.commit_pinout();      // <- RABY
  /* PIN_19 */ changed |= PIN_D2_B.commit_pinout();      // <- LULA
  /* PIN_19 */ changed |= PIN_D2_C.clear_preset();       // -> ROME,RAXY
  /* PIN_19 */ changed |= PIN_D2_D.commit_pinout();      // <- SULY
  /* PIN_20 */ changed |= PIN_D3_A.commit_pinout();      // <- RERA
  /* PIN_20 */ changed |= PIN_D3_B.commit_pinout();      // <- LULA
  /* PIN_20 */ changed |= PIN_D3_C.clear_preset();       // -> SAZA,SELO
  /* PIN_20 */ changed |= PIN_D3_D.commit_pinout();      // <- SEZE
  /* PIN_21 */ changed |= PIN_D4_A.commit_pinout();      // <- RORY
  /* PIN_21 */ changed |= PIN_D4_B.commit_pinout();      // <- LULA
  /* PIN_21 */ changed |= PIN_D4_C.clear_preset();       // -> TEHE,SODY
  /* PIN_21 */ changed |= PIN_D4_D.commit_pinout();      // <- RESY
  /* PIN_22 */ changed |= PIN_D5_A.commit_pinout();      // <- RYVO
  /* PIN_22 */ changed |= PIN_D5_B.commit_pinout();      // <- LULA
  /* PIN_22 */ changed |= PIN_D5_C.clear_preset();       // -> RATU,SAGO
  /* PIN_22 */ changed |= PIN_D5_D.commit_pinout();      // <- TAMU
  /* PIN_23 */ changed |= PIN_D6_A.commit_pinout();      // <- RAFY
  /* PIN_23 */ changed |= PIN_D6_B.commit_pinout();      // <- LULA
  /* PIN_23 */ changed |= PIN_D6_C.clear_preset();       // -> SOCA,RUPA
  /* PIN_23 */ changed |= PIN_D6_D.commit_pinout();      // <- ROGY
  /* PIN_24 */ changed |= PIN_D7_A.commit_pinout();      // <- RAVU
  /* PIN_24 */ changed |= PIN_D7_B.commit_pinout();      // <- LULA
  /* PIN_24 */ changed |= PIN_D7_C.clear_preset();       // -> RYBA,SAZY
  /* PIN_24 */ changed |= PIN_D7_D.commit_pinout();      // <- RYDA

  return changed;
}

//------------------------------------------------------------------------------

#if 0
// so the address bus is technically a tribuf, but we're going to ignore
// this debug circuit for now.
{
  // If we're in debug mode 2, drive external address bus onto internal address
  // these may be backwards, probably don't want to drive external address onto bus normally...

  /*p08.KOVA*/ wire A00_Cn = not(PIN_A00_C);
  /*p08.CAMU*/ wire A01_Cn = not(PIN_A01_C);
  /*p08.BUXU*/ wire A02_Cn = not(PIN_A02_C);
  /*p08.BASE*/ wire A03_Cn = not(PIN_A03_C);
  /*p08.AFEC*/ wire A04_Cn = not(PIN_A04_C);
  /*p08.ABUP*/ wire A05_Cn = not(PIN_A05_C);
  /*p08.CYGU*/ wire A06_Cn = not(PIN_A06_C);
  /*p08.COGO*/ wire A07_Cn = not(PIN_A07_C);
  /*p08.MUJY*/ wire A08_Cn = not(PIN_A08_C);
  /*p08.NENA*/ wire A09_Cn = not(PIN_A09_C);
  /*p08.SURA*/ wire A10_Cn = not(PIN_A10_C);
  /*p08.MADY*/ wire A11_Cn = not(PIN_A11_C);
  /*p08.LAHE*/ wire A12_Cn = not(PIN_A12_C);
  /*p08.LURA*/ wire A13_Cn = not(PIN_A13_C);
  /*p08.PEVO*/ wire A14_Cn = not(PIN_A14_C);
  /*p08.RAZA*/ wire A15_Cn = not(PIN_A15_C);

  /*p08.KEJO*/ cpu_bus.PIN_A00.set(!TOVA_MODE_DBG2n, not(A00_Cn));
  /*p08.BYXE*/ cpu_bus.PIN_A01.set(!TOVA_MODE_DBG2n, not(A01_Cn));
  /*p08.AKAN*/ cpu_bus.PIN_A02.set(!TOVA_MODE_DBG2n, not(A02_Cn));
  /*p08.ANAR*/ cpu_bus.PIN_A03.set(!TOVA_MODE_DBG2n, not(A03_Cn));
  /*p08.AZUV*/ cpu_bus.PIN_A04.set(!TOVA_MODE_DBG2n, not(A04_Cn));
  /*p08.AJOV*/ cpu_bus.PIN_A05.set(!TOVA_MODE_DBG2n, not(A05_Cn));
  /*p08.BYNE*/ cpu_bus.PIN_A06.set(!TOVA_MODE_DBG2n, not(A06_Cn));
  /*p08.BYNA*/ cpu_bus.PIN_A07.set(!TOVA_MODE_DBG2n, not(A07_Cn));
  /*p08.LOFA*/ cpu_bus.PIN_A08.set(!TOVA_MODE_DBG2n, not(A08_Cn));
  /*p08.MAPU*/ cpu_bus.PIN_A09.set(!TOVA_MODE_DBG2n, not(A09_Cn));
  /*p08.RALA*/ cpu_bus.PIN_A10.set(!TOVA_MODE_DBG2n, not(A10_Cn));
  /*p08.LORA*/ cpu_bus.PIN_A11.set(!TOVA_MODE_DBG2n, not(A11_Cn));
  /*p08.LYNA*/ cpu_bus.PIN_A12.set(!TOVA_MODE_DBG2n, not(A12_Cn));
  /*p08.LEFY*/ cpu_bus.PIN_A13.set(!TOVA_MODE_DBG2n, not(A13_Cn));
  /*p08.NEFE*/ cpu_bus.PIN_A14.set(!TOVA_MODE_DBG2n, not(A14_Cn));
  /*p08.SYZU*/ cpu_bus.PIN_A15.set(!TOVA_MODE_DBG2n, not(A15_Cn));
}
#endif

