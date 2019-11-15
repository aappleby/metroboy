#include "P07_SysDecode.h"
#include "Gameboy.h"
#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P07_SysDecode::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p07.MODE_DBG1n = not(b.p07.MODE_DBG1);

  //----------
  // debug enable signals

  c.p07.T1n        = not(b.chip.T1);
  c.p07.T2n        = not(b.chip.T2);
  c.p07.MODE_DBG1  = and(b.chip.T1, b.p07.T2n);
  c.p07.MODE_DBG2  = and(b.chip.T2, b.p07.T1n);
  c.p07.MODE_PROD  = nand(b.p07.T1n, b.p07.T2n, b.chip.RST);
  c.p07.MODE_DEBUG = or(b.p07.MODE_DBG1, b.p07.MODE_DBG2);

  //----------
  // doesn't do anything

  c.p07.PIN_NC = not(b.cpu.FROM_CPU6);
  c.chip.PIN_NC = c.p07.PIN_NC;

  //----------
  // Bootrom control bit

  c.p07.ADDR_xxxxxxxx0x0x0000 = nor(b.A07, b.A05, b.A03, b.A02, b.A01, b.A00);
  c.p07.ADDR_xxxxxxxxx1x1xxxx = and(b.A04, b.A06);

  c.p07.BOOT_BIT_WRn = nand(b.p07.CPU_WR, b.p07.ADDR_FFXX, b.p07.ADDR_xxxxxxxx0x0x0000, b.p07.ADDR_xxxxxxxxx1x1xxxx);
  c.p07.BOOT_BIT2    = or(b.D0, b.p07.BOOT_BIT);
  c.p07.BOOT_BIT     = tock_pos(a.p07.BOOT_BIT_WRn, b.p07.BOOT_BIT_WRn, b.p01.SYS_RESETn1, b.p07.BOOT_BIT, b.p07.BOOT_BIT2);

  c.p07.BOOT_BIT_RD  = and(b.p07.CPU_RD, b.p07.ADDR_FFXX, b.p07.ADDR_xxxxxxxxx1x1xxxx, b.p07.ADDR_xxxxxxxx0x0x0000);
  c.p07.BOOT_BIT_D0  = not(!b.p07.BOOT_BIT);

  if (b.p07.BOOT_BIT_RD) {
    c.D0 = b.p07.BOOT_BIT_D0;
  }

  //----------
  // Boot rom CS. Not sure why there are two decoders...

  c.p07.BOOT_EN    = not(b.p07.BOOT_BIT);

  c.p07.ADDR_00XX  = nor(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09, b.A08);
  c.p07.ADDR_0XXX  = nor(b.A15, b.A14, b.A13, b.A12);
  c.p07.ADDR_X0XX  = nor(b.A11, b.A10, b.A09, b.A08);
  c.p07.ADDR_00XX2 = and(b.p07.ADDR_0XXX, b.p07.ADDR_X0XX);
  c.p07.ADDR_BOOT  = and(b.p07.BOOT_EN, b.p07.ADDR_00XX);

  c.p07.BOOT_RD    = and(b.p07.CPU_RD, b.p07.MODE_DBG1n, b.p07.ADDR_BOOT);
  c.p07.BOOT_CSn   = nand(b.p07.BOOT_RD, b.p07.ADDR_00XX2);
  c.p07.BOOT_CS    = not(b.p07.BOOT_CSn);

  //----------
  // FF0F_RD/WR

  c.p07.ADDR_XX0X = nor(b.A07, b.A06, b.A05, b.A04);
  c.p07.ADDR_XXXF = and(b.A00, b.A01, b.A02, b.A03);
  c.p07.FF0F_RD   = and(b.p07.ADDR_XX0X, b.p07.ADDR_XXXF, b.p07.ADDR_FFXX, b.p07.CPU_RD);
  c.p07.FF0F_WR   = and(b.p07.ADDR_XX0X, b.p07.ADDR_XXXF, b.p07.ADDR_FFXX, b.p07.CPU_WR);

  //----------
  // hram select

  // addr >= FF80 and not XXFF

  /*p07.WALE*/ c.p07.ADDR_xxxxxxxxx1111111n = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
  /*p07.WOLY*/ c.p07.HRAM_CSn               = nand(b.p07.ADDR_FFXX, b.A07, b.p07.ADDR_xxxxxxxxx1111111n);
  /*p07.WUTA*/ c.p07.HRAM_CS                = not(b.p07.HRAM_CSn);

  //----------
  // weird debug thing

  c.p07.LECO = nor(b.p01.CPUCLK_xxxxxFGH2, b.p07.MODE_DBG2);

  c.p07.DBG_0 = not(b.chip.P10_B);
  c.p07.DBG_1 = not(b.chip.P10_B);
  c.p07.DBG_2 = not(b.chip.P10_B);
  c.p07.DBG_3 = not(b.chip.P10_B);
  c.p07.DBG_4 = not(b.chip.P10_B);
  c.p07.DBG_5 = not(b.chip.P10_B);
  c.p07.DBG_6 = not(b.chip.P10_B);
  c.p07.DBG_7 = not(b.chip.P10_B);

  if (b.p07.LECO) {
    c.D0 = b.p07.DBG_0;
    c.D1 = b.p07.DBG_1;
    c.D2 = b.p07.DBG_2;
    c.D3 = b.p07.DBG_3;
    c.D4 = b.p07.DBG_4;
    c.D5 = b.p07.DBG_5;
    c.D6 = b.p07.DBG_6;
    c.D7 = b.p07.DBG_7;
  }

  //----------
  // random address decoders

  c.p07.ADDR_00n = not(b.A00);
  c.p07.ADDR_01n = not(b.A01);
  c.p07.ADDR_05n = not(b.A05);
  c.p07.ADDR_04n = not(b.A04);
  c.p07.ADDR_08n = not(b.A08);

  c.p07.ADDR_0000_FE00 = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
  c.p07.ADDR_FE00_FFFF = not(b.p07.ADDR_0000_FE00);

  c.p07.ADDR_FFXX = nor(b.p07.ADDR_0000_FE00, b.p07.ADDR_08n);

  c.p07.ADDR_FEXXn = nand(b.p07.ADDR_FE00_FFFF, b.p07.ADDR_FFXXn2);
  c.p07.ADDR_OAM = not(b.p07.ADDR_FEXXn);

  c.p07.ADDR_FFXXn1 = not(b.p07.ADDR_FFXX);
  c.p07.ADDR_FFXXn2 = not(b.p07.ADDR_FFXX);

  //----------
  // bootrom address generation

  /*p07.ZYRA*/ c.p07.BOOTROM_A7n = not(b.A07);
  /*p07.ZAGE*/ c.p07.BOOTROM_A6n = not(b.A06);
  /*p07.ZYKY*/ c.p07.BOOTROM_A3n = not(b.A03);
  /*p07.ZYGA*/ c.p07.BOOTROM_A2n = not(b.A02);
  
  /*p07.ZYKY*/ c.p07.BOOTROM_A5nA4n = and(b.p07.ADDR_05n, b.p07.ADDR_04n);
  /*p07.ZYGA*/ c.p07.BOOTROM_A5nA4  = and(b.p07.ADDR_05n, b.A04);
  /*p07.ZOVY*/ c.p07.BOOTROM_A5A4n  = and(b.A05, b.p07.ADDR_04n);
  /*p07.ZUKO*/ c.p07.BOOTROM_A5A4   = and(b.A05, b.A04);

  /*p07.ZOLE*/ c.p07.ADDR_xxxxxxxxxxxxxx00 = and(b.p07.ADDR_01n, b.p07.ADDR_00n);
  /*p07.ZAJE*/ c.p07.ADDR_xxxxxxxxxxxxxx01 = and(b.p07.ADDR_01n, b.A00);
  /*p07.ZUBU*/ c.p07.ADDR_xxxxxxxxxxxxxx10 = and(b.A01, b.p07.ADDR_00n);
  /*p07.ZAPY*/ c.p07.ADDR_xxxxxxxxxxxxxx11 = and(b.A01, b.A00);

  /*p07.ZETE*/ c.p07.BOOTROM_A1nA0n = not(b.p07.ADDR_xxxxxxxxxxxxxx00);
  /*p07.ZEFU*/ c.p07.BOOTROM_A1nA0  = not(b.p07.ADDR_xxxxxxxxxxxxxx01);
  /*p07.ZYRO*/ c.p07.BOOTROM_A1A0n  = not(b.p07.ADDR_xxxxxxxxxxxxxx10);
  /*p07.ZAPA*/ c.p07.BOOTROM_A1A0   = not(b.p07.ADDR_xxxxxxxxxxxxxx11);

  //----------
  // FF60 debug reg

  /*p07.APER*/ c.p07.FF60_WRn = nand(b.p07.MODE_DEBUG, b.A05, b.A06, b.p07.CPU_WR, b.p10.FF_00_20_40_60);
  /*p07.BURO*/ c.p07.FF60_0   = tock_pos(a.p07.FF60_WRn, b.p07.FF60_WRn, b.p01.SYS_RESETn1, b.p07.FF60_0, b.D0);
  /*p07.AMUT*/ c.p07.FF60_1   = tock_pos(a.p07.FF60_WRn, b.p07.FF60_WRn, b.p01.SYS_RESETn1, b.p07.FF60_1, b.D1);
}