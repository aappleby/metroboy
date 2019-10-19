// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "CpuBus.h"
#include "MemBus.h"
#include "ExtBus.h"
#include "Clocks.h"
#include "Debug.h"

//----------
// inputs

extern bool BEDO;

extern bool ANAP;

//----------
// outputs

bool BOOT_CS; // output enables boot rom bus override?
bool HRAM_CS; // output enables hram bus override?


bool FF60_D0;
bool FF60_D1;

bool SARO;

// all output only
bool BOOTROM_A7n;
bool BOOTROM_A6n;
bool BOOTROM_A5nA4n;
bool BOOTROM_A5nA4;
bool BOOTROM_A5A4n;
bool BOOTROM_A5AA4;
bool BOOTROM_A3n;
bool BOOTROM_A2n;
bool BOOTROM_A1nA0n;
bool BOOTROM_A1nA0;
bool BOOTROM_A1A0n;
bool BOOTROM_A1A0;

//----------
// registers

reg TEPU;
reg AMUT;
reg BURO;

//-----------------------------------------------------------------------------

void tick_sysdecode() {
  bool TEPU_Q = TEPU.q();

  //----------

  bool TYRO = nor(mem.A7, mem.A5, mem.A3, mem.A2, mem.A1, mem.A0);
  bool TUFA = and(mem.A4, mem.A6);
  bool TUGE = nand(TYRO, TUFA, FFXX, cpu.CPU_WR);
  bool SATO = or(mem.D0, TEPU_Q);
  bool TEXE = and(cpu.CPU_RD, FFXX, TUFA, TYRO);
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

  BOOT_CS = ZERY;

  //----------

  bool SEMY = nor(mem.A7, mem.A6, mem.A5, mem.A4);
  bool SAPA = and(mem.A0, mem.A1, mem.A2, mem.A3);
  bool ROLO = and(SEMY, SAPA, FFXX, cpu.CPU_RD);
  bool REFA = and(SEMY, SAPA, FFXX, cpu.CPU_WR_RAW);

  FF0F_RD = ROLO;
  FF0F_WR = REFA;

  //----------

  bool WALE = nand(mem.A0, mem.A1, mem.A2, mem.A3, mem.A4, mem.A5, mem.A6);
  bool WOLY = nand(WALE, mem.A7, FFXX);
  bool WUTA = not(WOLY);

  HRAM_CS = WUTA;

  //----------

  bool LECO = nor(BEDO, dbg.T1nT2);

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
  FEXXFFXXn = TUNA;
  bool SYKE = nor(TONA, TUNA);
  bool RYCU = not(TUNA);
  bool SOHA = not(FFXX);
  bool ROPE = nand(RYCU, SOHA);
  FFXX = SYKE;
  bool BAKO = not(SYKE);
  FFXXn = BAKO;
  SARO = not(ROPE);

  //----------

  bool ZYRA = not(mem.A7);
  bool ZAGE = not(mem.A6);
  bool ZABU = not(mem.A3);
  bool ZOKE = not(mem.A2);

  BOOTROM_A7n = ZYRA;
  BOOTROM_A6n = ZAGE;
  BOOTROM_A3n = ZABU;
  BOOTROM_A2n = ZOKE;

  //----------

  bool ZERA = not(mem.A5);
  bool ZUFY = not(mem.A4);
  bool ZYKY = and(ZERA, ZUFY);
  bool ZYGA = and(ZERA, mem.A4);
  bool ZOVY = and(mem.A5, ZUFY);
  bool ZUKO = and(mem.A5, mem.A4);

  BOOTROM_A5nA4n = ZYKY;
  BOOTROM_A5nA4 = ZYGA;
  BOOTROM_A5A4n = ZOVY;
  BOOTROM_A5AA4 = ZUKO;

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

  BOOTROM_A1nA0n = ZETE;
  BOOTROM_A1nA0 = ZEFU;
  BOOTROM_A1A0n = ZYRO;
  BOOTROM_A1A0 = ZAPA;

  //----------

  bool APET = or(NET02, dbg.T1T2n);
  bool APER = nand(APET, mem.A5, mem.A6, cpu.CPU_WR, ANAP);

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

  TEPU.tock(TUGE, rst.RESET2, SATO);
  AMUT.tock(APER, rst.RESET2, mem.D1);
  BURO.tock(APER, rst.RESET2, mem.D0);
}