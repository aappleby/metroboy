#include "Sch_Bootrom.h"

#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

BootSignals Bootrom::sig(const SchematicTop& /*gb*/) const {
  BootSignals sig;
  sig.BOOT_BITn = BOOT_BITn;
  return sig;
}

//-----------------------------------------------------------------------------

void Bootrom::tick(SchematicTop& gb) {

  // FF50
  {
    auto& cpu_bus = gb.cpu_bus;
    auto cpu_sig = gb.cpu_bus.sig(gb);
    auto rst_sig = gb.rst_reg.sig(gb);

    /*p07.TYRO*/ wire ADDR_0x0x0000p = nor(cpu_bus.PIN_A07, cpu_bus.PIN_A05, cpu_bus.PIN_A03, cpu_bus.PIN_A02, cpu_bus.PIN_A01, cpu_bus.PIN_A00);
    /*p07.TUFA*/ wire ADDR_x1x1xxxxp = and(cpu_bus.PIN_A04, cpu_bus.PIN_A06);

    /*p07.TEXE*/ wire FF50_RDp = and(cpu_sig.TEDO_CPU_RD, cpu_sig.SYKE_FF00_FFFFp, ADDR_0x0x0000p, ADDR_x1x1xxxxp);
    /*p07.SYPU*/ cpu_bus.TRI_D0.set_tribuf(FF50_RDp, BOOT_BITn); // does the rung of the tribuf control polarity?

    /*p07.TUGE*/ wire FF50_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, cpu_sig.SYKE_FF00_FFFFp, ADDR_0x0x0000p, ADDR_x1x1xxxxp);
    /*p07.SATO*/ wire BOOT_BIT_IN = or (cpu_bus.TRI_D0, BOOT_BITn);

    /*p01.ALUR*/ wire ALUR_RSTn = not(rst_sig.AVOR_RSTp);   // this goes all over the place
    /*p07.TEPU*/ BOOT_BITn.set(FF50_WRn, ALUR_RSTn, BOOT_BIT_IN);
  }

  {
    auto& cpu_bus = gb.cpu_bus;
    auto dbg_sig = gb.dbg_reg.sig(gb);
    auto cpu_sig = gb.cpu_bus.sig(gb);

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

    /*p07.TERA*/ wire _TERA_BOOT_BITp  = not(BOOT_BITn.q());
    /*p07.TULO*/ wire _TULO_ADDR_00XXp = nor(cpu_bus.PIN_A15, cpu_bus.PIN_A14, cpu_bus.PIN_A13, cpu_bus.PIN_A12, cpu_bus.PIN_A11, cpu_bus.PIN_A10, cpu_bus.PIN_A09, cpu_bus.PIN_A08);
    /*p07.TUTU*/ wire _TUTU_ADDR_BOOTp = and (_TERA_BOOT_BITp, _TULO_ADDR_00XXp);

    /*p07.YAZA*/ wire _YAZA_MODE_DBG1n = not(dbg_sig.UMUT_MODE_DBG1p); // suggests UMUTp
    /*p07.YULA*/ wire _YULA_BOOT_RD = and (cpu_sig.TEDO_CPU_RD, _YAZA_MODE_DBG1n, _TUTU_ADDR_BOOTp); // def AND

    // this is kind of a hack
    uint16_t addr = (uint16_t)cpu_bus.get_addr();
    uint8_t data = DMG_ROM_bin[addr & 0xFF];

    cpu_bus.TRI_D0.set_tribuf(_YULA_BOOT_RD, data & 0x01);
    cpu_bus.TRI_D1.set_tribuf(_YULA_BOOT_RD, data & 0x02);
    cpu_bus.TRI_D2.set_tribuf(_YULA_BOOT_RD, data & 0x04);
    cpu_bus.TRI_D3.set_tribuf(_YULA_BOOT_RD, data & 0x08);
    cpu_bus.TRI_D4.set_tribuf(_YULA_BOOT_RD, data & 0x10);
    cpu_bus.TRI_D5.set_tribuf(_YULA_BOOT_RD, data & 0x20);
    cpu_bus.TRI_D6.set_tribuf(_YULA_BOOT_RD, data & 0x40);
    cpu_bus.TRI_D7.set_tribuf(_YULA_BOOT_RD, data & 0x80);
  }
}

//-----------------------------------------------------------------------------

SignalHash Bootrom::commit() {
  SignalHash hash;
  hash << BOOT_BITn.commit_reg();
  return hash;
}

//-----------------------------------------------------------------------------
