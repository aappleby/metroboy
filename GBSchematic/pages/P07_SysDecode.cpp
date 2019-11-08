#include "P07_SysDecode.h"
#include "Gameboy.h"
#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P07_SysDecode::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P07_SysDecode& pa = ga.p07;
  const P07_SysDecode& pb = gb.p07;
  P07_SysDecode& pc = gc.p07;

  //----------
  // debug enable signals

  pc.UBET = not(gb.chip.T1);
  pc.UVAR = not(gb.chip.T2);
  pc.UPOJ = nand(pb.UBET, pb.UVAR, gb.chip.RST);
  pc.UNOR = and(gb.chip.T2, pb.UBET);
  pc.UMUT = and(gb.chip.T1, pb.UVAR);
  
  gc.T1nT2n = pb.UPOJ;
  gc.T1nT2  = pb.UNOR;
  gc.T1T2n  = pb.UMUT;

  //----------
  // debug override of CPU_RD/CPU_WR

  pc.UBAL = mux2(gb.chip.WR_C, gb.CPU_RD_SYNC,    gb.T1nT2);
  pc.UJYV = mux2(gb.chip.RD_C, gb.cpu.CPU_RAW_RD, gb.T1nT2);

  pc.TAPU = not(pb.UBAL); pc.DYKY = not(pb.TAPU); pc.CUPA = not(pb.DYKY);
  pc.TEDO = not(pb.UJYV); pc.AJAS = not(pb.TEDO); pc.ASOT = not(pb.AJAS);

  gc.CPU_WR  = pb.TAPU;
  gc.CPU_RD  = pb.TEDO;
  
  gc.CPU_WR2 = pb.CUPA;
  gc.CPU_RD2 = pb.ASOT;

  //----------
  // doesn't do anything

  bool LEXY = not(gb.cpu.FROM_CPU6);
  gc.chip.PIN_NC = LEXY;

  //----------
  // BOOT_CS

  pc.TYRO = nor(gb.cpu.A07, gb.cpu.A05, gb.cpu.A03, gb.cpu.A02, gb.cpu.A01, gb.cpu.A00);
  pc.TUFA = and(gb.cpu.A04, gb.cpu.A06);
  pc.TUGE = nand(pb.TYRO, pb.TUFA, gb.FFXX, gb.CPU_WR);
  pc.SATO = or(gb.cpu.D0, pb.TEPU);
  pc.TEXE = and(gb.CPU_RD, gb.FFXX, pb.TUFA, pb.TYRO);
  pc.TEPU = tock_pos(pa.TUGE, pb.TUGE, gb.RESET2, pb.TEPU, pb.SATO);
  pc.SYPU = not(!pb.TEPU);
  pc.TERA = not(pb.TEPU);
  pc.TULO = nor(gb.cpu.A15, gb.cpu.A14, gb.cpu.A13, gb.cpu.A12, gb.cpu.A11, gb.cpu.A10, gb.cpu.A09, gb.cpu.A08);
  pc.TUTU = and(pb.TERA, pb.TULO);
  pc.ZORO = nor(gb.cpu.A15, gb.cpu.A14, gb.cpu.A13, gb.cpu.A12);
  pc.ZADU = nor(gb.cpu.A11, gb.cpu.A10, gb.cpu.A09, gb.cpu.A08);
  pc.YAZA = not(gb.T1T2n);
  pc.YULA = and(pb.YAZA, pb.TUTU, gb.CPU_RD);
  pc.ZUFA = and(pb.ZORO, pb.ZADU);
  pc.ZADO = nand(pb.YULA, pb.ZUFA);
  pc.ZERY = not(pb.ZADO);

  gc.BOOT_CS = pb.ZERY;
  gc.TUTU = pb.TUTU;

  if (pb.TEXE) {
    gc.cpu.D0 = pb.SYPU;
  }

  //----------
  // FF0F_RD/WR

  pc.SEMY = nor(gb.cpu.A07, gb.cpu.A06, gb.cpu.A05, gb.cpu.A04);
  pc.SAPA = and(gb.cpu.A00, gb.cpu.A01, gb.cpu.A02, gb.cpu.A03);
  pc.ROLO = and(pb.SEMY, pb.SAPA, gb.FFXX, gb.CPU_RD);
  pc.REFA = and(pb.SEMY, pb.SAPA, gb.FFXX, gb.CPU_WR);

  gc.FF0F_RD = pb.ROLO;
  gc.FF0F_WR = pb.REFA;

  //----------
  // hram select

  pc.WALE = nand(gb.cpu.A00, gb.cpu.A01, gb.cpu.A02, gb.cpu.A03, gb.cpu.A04, gb.cpu.A05, gb.cpu.A06);
  pc.WOLY = nand(pb.WALE, gb.cpu.A07, gb.FFXX);
  pc.WUTA = not(pb.WOLY);

  gc.HRAM_CS = pb.WUTA;

  //----------
  // weird debug thing

  pc.LECO = nor(gb.BEDO, gb.T1nT2);

  pc.ROMY_00 = not(gb.chip.P10_B);
  pc.RYNE_01 = not(gb.chip.P10_B);
  pc.REJY_02 = not(gb.chip.P10_B);
  pc.RASE_03 = not(gb.chip.P10_B);
  pc.REKA_04 = not(gb.chip.P10_B);
  pc.ROWE_05 = not(gb.chip.P10_B);
  pc.RYKE_06 = not(gb.chip.P10_B);
  pc.RARU_07 = not(gb.chip.P10_B);

  if (pb.LECO) {
    gc.cpu.D0 = pb.ROMY_00;
    gc.cpu.D1 = pb.RYNE_01;
    gc.cpu.D2 = pb.REJY_02;
    gc.cpu.D3 = pb.RASE_03;
    gc.cpu.D4 = pb.REKA_04;
    gc.cpu.D5 = pb.ROWE_05;
    gc.cpu.D6 = pb.RYKE_06;
    gc.cpu.D7 = pb.RARU_07;
  }

  //----------
  // random address decoders

  pc.TONA = not(gb.cpu.A08);
  pc.TUNA = nand(gb.cpu.A15, gb.cpu.A14, gb.cpu.A13, gb.cpu.A12, gb.cpu.A11, gb.cpu.A10, gb.cpu.A09);
  pc.SYKE = nor(pb.TONA, pb.TUNA);
  pc.RYCU = not(pb.TUNA);
  pc.SOHA = not(gb.FFXX);
  pc.ROPE = nand(pb.RYCU, pb.SOHA);
  pc.BAKO = not(pb.SYKE);
  pc.SARO = not(pb.ROPE);

  gc.FEXXFFXXn = pb.TUNA;
  gc.FFXX      = pb.SYKE;
  gc.FFXXn     = pb.BAKO;
  gc.SARO      = pb.SARO; // what address range does this trigger on?

  //----------
  // bootrom address generation

  pc.ZYRA = not(gb.cpu.A07);
  pc.ZAGE = not(gb.cpu.A06);
  pc.ZABU = not(gb.cpu.A03);
  pc.ZOKE = not(gb.cpu.A02);

  pc.ZERA = not(gb.cpu.A05);
  pc.ZUFY = not(gb.cpu.A04);
  pc.ZYKY = and(pb.ZERA, pb.ZUFY);
  pc.ZYGA = and(pb.ZERA, gb.cpu.A04);
  pc.ZOVY = and(gb.cpu.A05, pb.ZUFY);
  pc.ZUKO = and(gb.cpu.A05, gb.cpu.A04);

  pc.ZUVY = not(gb.cpu.A01);
  pc.ZYBA = not(gb.cpu.A00);
  pc.ZOLE = and(pb.ZUVY, pb.ZYBA);
  pc.ZAJE = and(pb.ZUVY, gb.cpu.A00);
  pc.ZUBU = and(pb.ZYBA, gb.cpu.A01);
  pc.ZAPY = and(gb.cpu.A01, gb.cpu.A00);

  pc.ZETE = not(pb.ZOLE);
  pc.ZEFU = not(pb.ZAJE);
  pc.ZYRO = not(pb.ZUBU);
  pc.ZAPA = not(pb.ZAPY);

  gc.BOOTROM_A1nA0n = pb.ZETE;
  gc.BOOTROM_A1nA0  = pb.ZEFU;
  gc.BOOTROM_A1A0n  = pb.ZYRO;
  gc.BOOTROM_A1A0   = pb.ZAPA;
  gc.BOOTROM_A2n    = pb.ZOKE;
  gc.BOOTROM_A3n    = pb.ZABU;
  gc.BOOTROM_A5nA4n = pb.ZYKY;
  gc.BOOTROM_A5nA4  = pb.ZYGA;
  gc.BOOTROM_A5A4n  = pb.ZOVY;
  gc.BOOTROM_A5AA4  = pb.ZUKO;
  gc.BOOTROM_A6n    = pb.ZAGE;
  gc.BOOTROM_A7n    = pb.ZYRA;

  //----------
  // FF60 debug reg

  pc.APET = or(gb.T1nT2, gb.T1T2n);
  pc.APER = nand(pb.APET, gb.cpu.A05, gb.cpu.A06, gb.CPU_WR, gb.ANAP);
  pc.BURO_00 = tock_pos(pa.APER, pb.APER, gb.RESET2, pb.BURO_00, gb.cpu.D0);
  pc.AMUT_01 = tock_pos(pa.APER, pb.APER, gb.RESET2, pb.AMUT_01, gb.cpu.D1);

  gc.FF60_D0 = pb.BURO_00;
  gc.FF60_D1 = pb.AMUT_01;
}