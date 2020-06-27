#include "Sch_Cart.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

// Die trace:

// TEBA = not(TEPU17) - TEPU17 must be Q
// TULO = nor(........)
// TUTU = and(TERA, TULO);

// SOBY = nor(A15, TUTU);
// SEPY = nand(ABUZ, SOBY);
// TAZY = mux2_p(LUMA, MARU06, SEPY);

// RYCA = not(UNOR)
// SUZE = nand(TAZY,RYCA)
// RULO = nor (TAZY,UNOR)
// A15.A = SUZE
// A15.D = RULO


// Die trace:
// YAZA = not(UMUT)
// YULA = and/or(YAZA, TUTU, TEDO)
// TERA = not(TEPU);
// TULO = nor(........)
// TUTU = and(TERA, TULO);


static const uint8_t DMG_ROM_bin[] = {
  0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32,
  0xcb, 0x7c, 0x20, 0xfb, 0x21, 0x26, 0xff, 0x0e,
  0x11, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3,
  0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0,
  0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1a,
  0xcd, 0x95, 0x00, 0xcd, 0x96, 0x00, 0x13, 0x7b,
  0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x00, 0x06,
  0x08, 0x1a, 0x13, 0x22, 0x23, 0x05, 0x20, 0xf9,
  0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99,
  0x0e, 0x0c, 0x3d, 0x28, 0x08, 0x32, 0x0d, 0x20,
  0xf9, 0x2e, 0x0f, 0x18, 0xf3, 0x67, 0x3e, 0x64,
  0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x04,
  0x1e, 0x02, 0x0e, 0x0c, 0xf0, 0x44, 0xfe, 0x90,
  0x20, 0xfa, 0x0d, 0x20, 0xf7, 0x1d, 0x20, 0xf2,
  0x0e, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62,
  0x28, 0x06, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x06,
  0x7b, 0xe2, 0x0c, 0x3e, 0x87, 0xe2, 0xf0, 0x42,
  0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x05, 0x20,
  0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x06, 0x04,
  0xc5, 0xcb, 0x11, 0x17, 0xc1, 0xcb, 0x11, 0x17,
  0x05, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9,

  // the logo @ 0x00A8
  0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b,
  0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d,
  0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
  0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
  0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc,
  0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e,

  0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c,
  0x21, 0x04, 0x01, 0x11, 0xa8, 0x00, 0x1a, 0x13,
  0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20,
  0xf5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20,
  0xfb, 0x86, 0x20, 0xfe, 0x3e, 0x01, 0xe0, 0x50
};

//------------------------------------------------------------------------------

void CartRegisters::tick(TestGB& gb) {
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);

  auto& bus_reg = gb.bus_reg;
  auto& cpu_bus = gb.cpu_bus;
  auto& ext_bus = gb.ext_bus;
  auto& dma_reg = gb.dma_reg;

  auto adr_sig = gb.adr_reg.sig(cpu_bus);

  //----------------------------------------

  /*p07.TERA*/ wire _TERA_BOOT_BITp  = not(bus_reg.BOOT_BITn.q());
  /*p07.TULO*/ wire _TULO_ADDR_00XXp = nor(cpu_bus.PIN_A15, cpu_bus.PIN_A14, cpu_bus.PIN_A13, cpu_bus.PIN_A12, cpu_bus.PIN_A11, cpu_bus.PIN_A10, cpu_bus.PIN_A09, cpu_bus.PIN_A08);
  /*p07.TUTU*/ wire _TUTU_ADDR_BOOTp = and (_TERA_BOOT_BITp, _TULO_ADDR_00XXp);

  cpu_bus.PIN_READ_BOOTROM.set(_TUTU_ADDR_BOOTp);

  // Something weird here
  /*p08.SOBY*/ wire _SOBY = nor(cpu_bus.PIN_A15, _TUTU_ADDR_BOOTp);
  /*p08.SEPY*/ wire _SEPY = nand(dbg_sig.ABUZ, _SOBY);
  /*p08.TAZY*/ wire _TAZY = mux2_p(dma_reg.DMA_A15.q(), _SEPY, dma_sig.LUMA_DMA_READ_CARTp);
  /*p08.SUZE*/ ext_bus.PIN_A15_A.set(nand(_TAZY, dbg_sig.RYCA_MODE_DBG2n));
  /*p08.RULO*/ ext_bus.PIN_A15_D.set(nor (_TAZY, dbg_sig.UNOR_MODE_DBG2p));

  // Bootrom -> CPU
  ///*p07.ZORO*/ wire ADDR_0XXX = nor(cpu_pins.A15, cpu_pins.A14, cpu_pins.A13, cpu_pins.A12);
  ///*p07.ZADU*/ wire ADDR_X0XX = nor(cpu_pins.A11, cpu_pins.A10, cpu_pins.A09, cpu_pins.A08);
  ///*p07.ZUFA*/ wire ADDR_00XX2 = and(ADDR_0XXX, ADDR_X0XX);
  ///*p07.ZADO*/ wire BOOT_CSn   = nand(BOOT_RD, ADDR_00XX2);
  ///*p07.ZERY*/ wire BOOT_CS    = not(BOOT_CSn);

  ///*p07.ZYBA*/ wire ADDR_00n = not(cpu_pins.A00);
  ///*p07.ZUVY*/ wire ADDR_01n = not(cpu_pins.A01);
  ///*p07.ZUFY*/ wire ADDR_04n = not(cpu_pins.A04);
  ///*p07.ZERA*/ wire ADDR_05n = not(cpu_pins.A05);
  ///*p07.ZOLE*/ wire ADDR_00  = and(ADDR_01n, ADDR_00n);
  ///*p07.ZAJE*/ wire ADDR_01  = and(ADDR_01n, cpu_pins.A00);
  ///*p07.ZUBU*/ wire ADDR_10  = and(cpu_pins.A01, ADDR_00n);
  ///*p07.ZAPY*/ wire ADDR_11  = and(cpu_pins.A01, cpu_pins.A00);

  ///*p07.ZETE*/ wire BOOTROM_A1nA0n = not(ADDR_00);
  ///*p07.ZEFU*/ wire BOOTROM_A1nA0  = not(ADDR_01);
  ///*p07.ZYRO*/ wire BOOTROM_A1A0n  = not(ADDR_10);
  ///*p07.ZAPA*/ wire BOOTROM_A1A0   = not(ADDR_11);
  ///*p07.ZYGA*/ wire BOOTROM_A2n    = not(cpu_pins.A02);
  ///*p07.ZYKY*/ wire BOOTROM_A3n    = not(cpu_pins.A03);
  ///*p07.ZYKY*/ wire BOOTROM_A5nA4n = and(ADDR_05n, ADDR_04n);
  ///*p07.ZYGA*/ wire BOOTROM_A5nA4  = and(ADDR_05n, cpu_pins.A04);
  ///*p07.ZOVY*/ wire BOOTROM_A5A4n  = and(cpu_pins.A05, ADDR_04n);
  ///*p07.ZUKO*/ wire BOOTROM_A5A4   = and(cpu_pins.A05, cpu_pins.A04);
  ///*p07.ZAGE*/ wire BOOTROM_A6n    = not(cpu_pins.A06);
  ///*p07.ZYRA*/ wire BOOTROM_A7n    = not(cpu_pins.A07);

  /*p07.YAZA*/ wire _YAZA_MODE_DBG1n = not(dbg_sig.UMUT_MODE_DBG1p); // suggests UMUTp
  /*p07.YULA*/ wire _YULA_BOOT_RD = and (cpu_sig.TEDO_CPU_RD, _YAZA_MODE_DBG1n, _TUTU_ADDR_BOOTp); // def AND

  // this is kind of a hack
  uint16_t addr = (uint16_t)cpu_bus.get_addr();
  uint8_t data = DMG_ROM_bin[addr & 0xFF];

  cpu_bus.TS_D0.set_tribuf(_YULA_BOOT_RD, data & 0x01);
  cpu_bus.TS_D1.set_tribuf(_YULA_BOOT_RD, data & 0x02);
  cpu_bus.TS_D2.set_tribuf(_YULA_BOOT_RD, data & 0x04);
  cpu_bus.TS_D3.set_tribuf(_YULA_BOOT_RD, data & 0x08);
  cpu_bus.TS_D4.set_tribuf(_YULA_BOOT_RD, data & 0x10);
  cpu_bus.TS_D5.set_tribuf(_YULA_BOOT_RD, data & 0x20);
  cpu_bus.TS_D6.set_tribuf(_YULA_BOOT_RD, data & 0x40);
  cpu_bus.TS_D7.set_tribuf(_YULA_BOOT_RD, data & 0x80);

  // internal data bus to external data bus
  {
    /*p08.REDU*/ wire _REDU_CPU_RD = not(cpu_sig.TEDO_CPU_RD);
    /*p08.RORU*/ wire _RORU_IBUS_TO_EBUSn = mux2_p(_REDU_CPU_RD, cpu_sig.MOTY_CPU_EXT_RD, dbg_sig.UNOR_MODE_DBG2p);
    /*p08.LULA*/ wire _LULA_IBUS_TO_EBUSp = not(_RORU_IBUS_TO_EBUSn);

    ext_bus.PIN_D0_B.set(_LULA_IBUS_TO_EBUSp);
    ext_bus.PIN_D1_B.set(_LULA_IBUS_TO_EBUSp);
    ext_bus.PIN_D2_B.set(_LULA_IBUS_TO_EBUSp);
    ext_bus.PIN_D3_B.set(_LULA_IBUS_TO_EBUSp);
    ext_bus.PIN_D4_B.set(_LULA_IBUS_TO_EBUSp);
    ext_bus.PIN_D5_B.set(_LULA_IBUS_TO_EBUSp);
    ext_bus.PIN_D6_B.set(_LULA_IBUS_TO_EBUSp);
    ext_bus.PIN_D7_B.set(_LULA_IBUS_TO_EBUSp);

    /*p25.RUXA*/ ext_bus.PIN_D0_A.set(nand(cpu_bus.TS_D0, _LULA_IBUS_TO_EBUSp));
    /*p25.RUJA*/ ext_bus.PIN_D1_A.set(nand(cpu_bus.TS_D1, _LULA_IBUS_TO_EBUSp));
    /*p25.RABY*/ ext_bus.PIN_D2_A.set(nand(cpu_bus.TS_D2, _LULA_IBUS_TO_EBUSp));
    /*p25.RERA*/ ext_bus.PIN_D3_A.set(nand(cpu_bus.TS_D3, _LULA_IBUS_TO_EBUSp));
    /*p25.RORY*/ ext_bus.PIN_D4_A.set(nand(cpu_bus.TS_D4, _LULA_IBUS_TO_EBUSp));
    /*p25.RYVO*/ ext_bus.PIN_D5_A.set(nand(cpu_bus.TS_D5, _LULA_IBUS_TO_EBUSp));
    /*p25.RAFY*/ ext_bus.PIN_D6_A.set(nand(cpu_bus.TS_D6, _LULA_IBUS_TO_EBUSp));
    /*p25.RAVU*/ ext_bus.PIN_D7_A.set(nand(cpu_bus.TS_D7, _LULA_IBUS_TO_EBUSp));

    /*p08.RUNE*/ ext_bus.PIN_D0_D.set(nor(cpu_bus.TS_D0, _RORU_IBUS_TO_EBUSn));
    /*p08.RYPU*/ ext_bus.PIN_D1_D.set(nor(cpu_bus.TS_D1, _RORU_IBUS_TO_EBUSn));
    /*p08.SULY*/ ext_bus.PIN_D2_D.set(nor(cpu_bus.TS_D2, _RORU_IBUS_TO_EBUSn));
    /*p08.SEZE*/ ext_bus.PIN_D3_D.set(nor(cpu_bus.TS_D3, _RORU_IBUS_TO_EBUSn));
    /*p08.RESY*/ ext_bus.PIN_D4_D.set(nor(cpu_bus.TS_D4, _RORU_IBUS_TO_EBUSn));
    /*p08.TAMU*/ ext_bus.PIN_D5_D.set(nor(cpu_bus.TS_D5, _RORU_IBUS_TO_EBUSn));
    /*p08.ROGY*/ ext_bus.PIN_D6_D.set(nor(cpu_bus.TS_D6, _RORU_IBUS_TO_EBUSn));
    /*p08.RYDA*/ ext_bus.PIN_D7_D.set(nor(cpu_bus.TS_D7, _RORU_IBUS_TO_EBUSn));
  }

  //----------------------------------------

  {
    /* PIN_75 */ ext_bus.PIN_PHI.set(clk_sig.PIN_BUDE_AxxxxFGH);
  }

  {
    /*p08.TYMU*/ wire _TYMU_RD_OUTn = nor(dma_sig.LUMA_DMA_READ_CARTp, cpu_sig.MOTY_CPU_EXT_RD);

    /*p08.UGAC*/ wire _UGAC_RDp_A = nand(_TYMU_RD_OUTn, dbg_sig.TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire _URUN_RDp_D = nor (_TYMU_RD_OUTn, dbg_sig.UNOR_MODE_DBG2p);
    /* PIN_79 */ ext_bus.PIN_RD_A.set(_UGAC_RDp_A);
    /* PIN_79 */ ext_bus.PIN_RD_D.set(_URUN_RDp_D);
  }

  {
    /*p08.MEXO*/ wire _MEXO_ABCDExxx = not(cpu_sig.APOV_CPU_WR_xxxxxFGH);
    /*p08.NEVY*/ wire _NEVY = or(_MEXO_ABCDExxx, cpu_sig.MOCA_DBG_EXT_RD);
    /*p08.PUVA*/ wire _PUVA_WR_OUTn = or(_NEVY, dma_sig.LUMA_DMA_READ_CARTp);

    /*p08.UVER*/ wire _UVER_WRp_A = nand(_PUVA_WR_OUTn, dbg_sig.TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire _USUF_WRp_D = nor (_PUVA_WR_OUTn, dbg_sig.UNOR_MODE_DBG2p);
    /* PIN_78 */ ext_bus.PIN_WR_A.set(_UVER_WRp_A);
    /* PIN_78 */ ext_bus.PIN_WR_D.set(_USUF_WRp_D);
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

    /* PIN_80 */ ext_bus.PIN_CS_A.set(_TYHO_CS_A);
  }
}