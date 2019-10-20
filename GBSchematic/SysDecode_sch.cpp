// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "CpuBus.h"
#include "MemBus.h"
#include "ExtBus.h"
#include "Clocks.h"
#include "Debug.h"
#include "System.h"
#include "APU.h"

//-----------------------------------------------------------------------------

void tick_sysdecode(const APU& apu, const Resets& rst, AddressDecoder& dec, MemBus& mem) {
  bool TEPU_Q = sys.TEPU.q();

  //----------

  bool TYRO = nor(mem.A7, mem.A5, mem.A3, mem.A2, mem.A1, mem.A0);
  bool TUFA = and(mem.A4, mem.A6);
  bool TUGE = nand(TYRO, TUFA, dec.FFXX, cpu.CPU_WR);
  bool SATO = or(mem.D0, TEPU_Q);
  bool TEXE = and(cpu.CPU_RD, dec.FFXX, TUFA, TYRO);
  bool SYPU = not(TEPU_Q);

  if (TEXE) {
    mem.D0 = SYPU;
  }

  bool TERA = not(TEPU_Q);
  bool TULO = nor(mem.A15, mem.A14, mem.A13, mem.A12, mem.A11, mem.A10, mem.A9, mem.A8);
  bool TUTU = and(TERA, TULO);
  bool YAZA = not(dbg.T1T2n);
  bool ZORO = nor(mem.A15, mem.A14, mem.A13, mem.A12);
  bool ZADU = nor(mem.A11, mem.A10, mem.A9, mem.A8);
  bool YULA = and(YAZA, TUTU, cpu.CPU_RD);
  bool ZUFA = and(ZORO, ZADU);
  bool ZADO = nand(YULA, ZUFA);
  bool ZERY = not(ZADO);

  sys.BOOT_CS = ZERY;

  //----------

  bool SEMY = nor(mem.A7, mem.A6, mem.A5, mem.A4);
  bool SAPA = and(mem.A0, mem.A1, mem.A2, mem.A3);
  bool ROLO = and(SEMY, SAPA, dec.FFXX, cpu.CPU_RD);
  bool REFA = and(SEMY, SAPA, dec.FFXX, cpu.CPU_WR_RAW);

  cpu.FF0F_RD = ROLO;
  cpu.FF0F_WR = REFA;

  //----------

  bool WALE = nand(mem.A0, mem.A1, mem.A2, mem.A3, mem.A4, mem.A5, mem.A6);
  bool WOLY = nand(WALE, mem.A7, dec.FFXX);
  bool WUTA = not(WOLY);

  sys.HRAM_CS = WUTA;

  //----------

  bool LECO = nor(clk.BEDO, dbg.T1nT2);

  bool RARU = not(ext.P10_B);
  bool ROWE = not(ext.P10_B);
  bool RYKE = not(ext.P10_B);
  bool RYNE = not(ext.P10_B);
  bool RASE = not(ext.P10_B);
  bool REJY = not(ext.P10_B);
  bool REKA = not(ext.P10_B);
  bool ROMY = not(ext.P10_B);

  if (LECO) {
    mem.D7 = RARU;
    mem.D5 = ROWE;
    mem.D6 = RYKE;
    mem.D1 = RYNE;
    mem.D3 = RASE;
    mem.D2 = REJY;
    mem.D4 = REKA;
    mem.D0 = ROMY;
  }

  //----------

  bool TONA = not(mem.A8);
  bool TUNA = nand(mem.A15, mem.A14, mem.A13, mem.A12, mem.A11, mem.A10, mem.A9);
  dec.FEXXFFXXn = TUNA;
  bool SYKE = nor(TONA, TUNA);
  bool RYCU = not(TUNA);
  bool SOHA = not(dec.FFXX);
  bool ROPE = nand(RYCU, SOHA);
  dec.FFXX = SYKE;
  bool BAKO = not(SYKE);
  dec.FFXXn = BAKO;
  sys.SARO = not(ROPE);

  //----------

  bool ZYRA = not(mem.A7);
  bool ZAGE = not(mem.A6);
  bool ZABU = not(mem.A3);
  bool ZOKE = not(mem.A2);

  sys.BOOTROM_A7n = ZYRA;
  sys.BOOTROM_A6n = ZAGE;
  sys.BOOTROM_A3n = ZABU;
  sys.BOOTROM_A2n = ZOKE;

  //----------

  bool ZERA = not(mem.A5);
  bool ZUFY = not(mem.A4);
  bool ZYKY = and(ZERA, ZUFY);
  bool ZYGA = and(ZERA, mem.A4);
  bool ZOVY = and(mem.A5, ZUFY);
  bool ZUKO = and(mem.A5, mem.A4);

  sys.BOOTROM_A5nA4n = ZYKY;
  sys.BOOTROM_A5nA4 = ZYGA;
  sys.BOOTROM_A5A4n = ZOVY;
  sys.BOOTROM_A5AA4 = ZUKO;

  //----------

  bool ZUVY = not(mem.A1);
  bool ZYBA = not(mem.A0);
  bool ZOLE = and(ZUVY, ZYBA);
  bool ZAJE = and(ZUVY, mem.A0);
  bool ZUBU = and(ZYBA, mem.A1);
  bool ZAPY = and(mem.A1, mem.A0);

  bool ZETE = not(ZOLE);
  bool ZEFU = not(ZAJE);
  bool ZYRO = not(ZUBU);
  bool ZAPA = not(ZAPY);

  sys.BOOTROM_A1nA0n = ZETE;
  sys.BOOTROM_A1nA0 = ZEFU;
  sys.BOOTROM_A1A0n = ZYRO;
  sys.BOOTROM_A1A0 = ZAPA;

  //----------

  bool APET = or(dbg.NET02, dbg.T1T2n);
  bool APER = nand(APET, mem.A5, mem.A6, cpu.CPU_WR, apu.ANAP);

  //----------

  // FIXME mux is probably backwards, we should probably be selecting WR_IN/RD_B if T1nT2 is high

  bool UBAL = mux2(ext.WR_IN, cpu.CPU_RD_SYNC, dbg.T1nT2);
  bool TAPU = not(UBAL);
  cpu.CPU_WR = TAPU;
  bool DYKY = not(TAPU);
  bool CUPA = not(DYKY);
  cpu.CPU_WR2 = CUPA;

  bool UJYV = mux2(ext.RD_B, cpu.CPU_RAW_RD, dbg.T1nT2);
  bool TEDO = not(UJYV);
  cpu.CPU_RD = TEDO;
  bool AJAS = not(TEDO);
  bool ASOT = not(AJAS);
  cpu.CPU_RD2 = ASOT;

  bool LEXY = not(cpu.FROM_CPU6);
  ext.PIN_NC = LEXY;

  //----------
  // registers

  sys.TEPU.tock(TUGE, rst.RESET2, SATO);
  sys.AMUT.tock(APER, rst.RESET2, mem.D1);
  sys.BURO.tock(APER, rst.RESET2, mem.D0);
}