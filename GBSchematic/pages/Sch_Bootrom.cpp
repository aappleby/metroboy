#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void Bootrom_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p07.TYRO*/ wire ADDR_0x0x0000 = nor(b.A07, b.A05, b.A03, b.A02, b.A01, b.A00);
  /*p07.TUFA*/ wire ADDR_x1x1xxxx = and(b.A04, b.A06);

  /*p07.TEXE*/ wire BOOT_BIT_RD = and(b.sys.CPU_RD, b.sys.ADDR_FFXX, ADDR_x1x1xxxx, ADDR_0x0x0000);
  /*p07.SYPU*/ if (BOOT_BIT_RD) c.D0 = b.sys.BOOT_BIT;

  // Bootrom signal gen

  /*p07.TULO*/ wire ADDR_00XX  = nor(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09, b.A08);

  /*p07.TERA*/ wire BOOT_BITn  = not(b.sys.BOOT_BIT);
  /*p07.TUTU*/ c.sys.ADDR_BOOT  = and(BOOT_BITn, ADDR_00XX);

  /*p07.YULA*/ wire BOOT_RD    = and(b.sys.CPU_RD, b.sys.MODE_DBG1n, b.sys.ADDR_BOOT);

  /*p07.ZYBA*/ wire ADDR_00n = not(b.A00);
  /*p07.ZUVY*/ wire ADDR_01n = not(b.A01);
  /*p07.ZUFY*/ wire ADDR_04n = not(b.A04);
  /*p07.ZERA*/ wire ADDR_05n = not(b.A05);

  /*p07.ZORO*/ wire ZORO = nor(b.A15, b.A14, b.A13, b.A12);
  /*p07.ZADU*/ wire ZADU = nor(b.A11, b.A10, b.A09, b.A08);
  /*p07.ZUFA*/ wire ADDR_00XX2 = and(ZORO, ZADU);

  /*p07.ZADO*/ wire ZADO = nand(BOOT_RD, ADDR_00XX2);
  /*p07.ZERY*/ c.BOOT_CS = not(ZADO);

  /*p07.ZOLE*/ wire ZOLE = and(ADDR_01n, ADDR_00n);
  /*p07.ZAJE*/ wire ZAJE = and(ADDR_01n, b.A00);
  /*p07.ZUBU*/ wire ZUBU = and(b.A01, ADDR_00n);
  /*p07.ZAPY*/ wire ZAPY = and(b.A01, b.A00);

  /*p07.ZETE*/ c.BOOTROM_A1nA0n = not(ZOLE);
  /*p07.ZEFU*/ c.BOOTROM_A1nA0  = not(ZAJE);
  /*p07.ZYRO*/ c.BOOTROM_A1A0n  = not(ZUBU);
  /*p07.ZAPA*/ c.BOOTROM_A1A0   = not(ZAPY);
  /*p07.ZYGA*/ c.BOOTROM_A2n    = not(b.A02);
  /*p07.ZYKY*/ c.BOOTROM_A3n    = not(b.A03);
  /*p07.ZYKY*/ c.BOOTROM_A5nA4n = and(ADDR_05n, ADDR_04n);
  /*p07.ZYGA*/ c.BOOTROM_A5nA4  = and(ADDR_05n, b.A04);
  /*p07.ZOVY*/ c.BOOTROM_A5A4n  = and(b.A05, ADDR_04n);
  /*p07.ZUKO*/ c.BOOTROM_A5A4   = and(b.A05, b.A04);
  /*p07.ZAGE*/ c.BOOTROM_A6n    = not(b.A06);
  /*p07.ZYRA*/ c.BOOTROM_A7n    = not(b.A07);
}

//-----------------------------------------------------------------------------

void Bootrom_tock(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p07.TYRO*/ wire ADDR_0x0x0000 = nor(b.A07, b.A05, b.A03, b.A02, b.A01, b.A00);
  /*p07.TUFA*/ wire ADDR_x1x1xxxx = and(b.A04, b.A06);

  /*p07.TUGE*/ c.sys.BOOT_BIT_CLK = nand(b.sys.CPU_WR, b.sys.ADDR_FFXX, ADDR_0x0x0000, ADDR_x1x1xxxx);
  /*p07.SATO*/ wire BOOT_BIT_IN  = or(b.D0, b.sys.BOOT_BIT);
  /*p07.TEPU*/ c.sys.BOOT_BIT     = tock_pos(a.sys.BOOT_BIT_CLK, b.sys.BOOT_BIT_CLK, b.sys.SYS_RESETn1, b.sys.BOOT_BIT, BOOT_BIT_IN);
}

//-----------------------------------------------------------------------------

};