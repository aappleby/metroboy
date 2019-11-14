#include "P07_SysDecode.h"
#include "Gameboy.h"
#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P07_SysDecode::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

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

  bool LEXY = not(b.cpu.FROM_CPU6);
  c.chip.PIN_NC = LEXY;

  //----------
  // BOOT_CS

  c.p07.TYRO = nor(b.A07, b.A05, b.A03, b.A02, b.A01, b.A00);
  c.p07.TUFA = and(b.A04, b.A06);
  c.p07.TUGE = nand(b.p07.TYRO, b.p07.TUFA, b.p07.ADDR_FFXX, b.p07.CPU_WR);
  c.p07.SATO = or(b.D0, b.p07.DISABLE_BOOTROM1);
  c.p07.TEXE = and(b.p07.CPU_RD, b.p07.ADDR_FFXX, b.p07.TUFA, b.p07.TYRO);

  c.p07.DISABLE_BOOTROM1 = tock_pos(a.p07.TUGE, b.p07.TUGE, b.p01.SYS_RESETn1, b.p07.DISABLE_BOOTROM1, b.p07.SATO);
  c.p07.DISABLE_BOOTROM2 = not(!b.p07.DISABLE_BOOTROM1);
  c.p07.ENABLE_BOOTROM1  = not(b.p07.DISABLE_BOOTROM1);

  c.p07.ADDR_00XX        = nor(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09, b.A08);
  c.p07.READ_BOOTROM     = and(b.p07.ENABLE_BOOTROM1, b.p07.ADDR_00XX);

  c.p07.ADDR_0XXX    = nor(b.A15, b.A14, b.A13, b.A12);
  c.p07.ADDR_X0XX    = nor(b.A11, b.A10, b.A09, b.A08);
  
  c.p07.YAZA    = not(b.p07.MODE_DBG1);
  c.p07.YULA    = and(b.p07.YAZA, b.p07.READ_BOOTROM, b.p07.CPU_RD);
  c.p07.ADDR_00XX2    = and(b.p07.ADDR_0XXX, b.p07.ADDR_X0XX);
  
  c.p07.ZADO    = nand(b.p07.YULA, b.p07.ADDR_00XX2);
  c.p07.BOOT_CS = not(b.p07.ZADO);

  if (b.p07.TEXE) {
    c.D0 = b.p07.DISABLE_BOOTROM2;
  }

  //----------
  // FF0F_RD/WR

  c.p07.ADDR_XX0X = nor(b.A07, b.A06, b.A05, b.A04);
  c.p07.ADDR_XXXF = and(b.A00, b.A01, b.A02, b.A03);
  c.p07.FF0F_RD   = and(b.p07.ADDR_XX0X, b.p07.ADDR_XXXF, b.p07.ADDR_FFXX, b.p07.CPU_RD);
  c.p07.FF0F_WR   = and(b.p07.ADDR_XX0X, b.p07.ADDR_XXXF, b.p07.ADDR_FFXX, b.p07.CPU_WR);

  //----------
  // hram select

  // addr >= FF80 and not XXFF

  c.p07.WALE    = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
  c.p07.WOLY    = nand(b.p07.WALE, b.A07, b.p07.ADDR_FFXX);
  c.p07.HRAM_CS = not(b.p07.WOLY);

  //----------
  // weird debug thing

  c.p07.LECO = nor(b.p01.CPUCLK_xxxxxFGH2, b.p07.MODE_DBG2);

  c.p07.ROMY_00 = not(b.chip.P10_B);
  c.p07.RYNE_01 = not(b.chip.P10_B);
  c.p07.REJY_02 = not(b.chip.P10_B);
  c.p07.RASE_03 = not(b.chip.P10_B);
  c.p07.REKA_04 = not(b.chip.P10_B);
  c.p07.ROWE_05 = not(b.chip.P10_B);
  c.p07.RYKE_06 = not(b.chip.P10_B);
  c.p07.RARU_07 = not(b.chip.P10_B);

  if (b.p07.LECO) {
    c.D0 = b.p07.ROMY_00;
    c.D1 = b.p07.RYNE_01;
    c.D2 = b.p07.REJY_02;
    c.D3 = b.p07.RASE_03;
    c.D4 = b.p07.REKA_04;
    c.D5 = b.p07.ROWE_05;
    c.D6 = b.p07.RYKE_06;
    c.D7 = b.p07.RARU_07;
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

  c.p07.ADDR_FEXXn = nand(b.p07.ADDR_FE00_FFFF, b.p07.FFXXn2);
  c.p07.ADDR_OAM = not(b.p07.ADDR_FEXXn);

  c.p07.FFXXn1 = not(b.p07.ADDR_FFXX);
  c.p07.FFXXn2   = not(b.p07.ADDR_FFXX);

  //----------
  // bootrom address generation

  c.p07.BOOTROM_A7n = not(b.A07);
  c.p07.BOOTROM_A6n = not(b.A06);
  c.p07.BOOTROM_A3n = not(b.A03);
  c.p07.BOOTROM_A2n = not(b.A02);
  
  c.p07.BOOTROM_A5nA4n = and(b.p07.ADDR_05n, b.p07.ADDR_04n);
  c.p07.BOOTROM_A5nA4  = and(b.p07.ADDR_05n, b.A04);
  c.p07.BOOTROM_A5A4n  = and(b.A05, b.p07.ADDR_04n);
  c.p07.BOOTROM_A5A4   = and(b.A05, b.A04);

  c.p07.ZOLE = and(b.p07.ADDR_01n, b.p07.ADDR_00n);
  c.p07.ZAJE = and(b.p07.ADDR_01n, b.A00);
  c.p07.ZUBU = and(b.p07.ADDR_00n, b.A01);
  c.p07.ZAPY = and(b.A01, b.A00);

  c.p07.BOOTROM_A1nA0n = not(b.p07.ZOLE);
  c.p07.BOOTROM_A1nA0  = not(b.p07.ZAJE);
  c.p07.BOOTROM_A1A0n  = not(b.p07.ZUBU);
  c.p07.BOOTROM_A1A0   = not(b.p07.ZAPY);

  //----------
  // FF60 debug reg

  c.p07.FF60_WRn   = nand(b.p07.MODE_DEBUG, b.A05, b.A06, b.p07.CPU_WR, b.p10.FF_00_20_40_60);
  c.p07.FF60_D0    = tock_pos(a.p07.FF60_WRn, b.p07.FF60_WRn, b.p01.SYS_RESETn1, b.p07.FF60_D0, b.D0);
  c.p07.FF60_D1    = tock_pos(a.p07.FF60_WRn, b.p07.FF60_WRn, b.p01.SYS_RESETn1, b.p07.FF60_D1, b.D1);
}