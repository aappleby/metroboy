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
  
  //----------
  // debug override of CPU_RD/CPU_WR

  pc.UBAL = mux2(gb.chip.WR_C, gb.p01.CPU_WR_SYNC, gb.p07.T1nT2);
  pc.UJYV = mux2(gb.chip.RD_C, gb.cpu.CPU_RAW_RD,  gb.p07.T1nT2);

  pc.TAPU = not(pb.UBAL);
  pc.DYKY = not(pb.TAPU);
  pc.CUPA = not(pb.DYKY);
  pc.TEDO = not(pb.UJYV);
  pc.AJAS = not(pb.TEDO);
  pc.ASOT = not(pb.AJAS);

  //----------
  // doesn't do anything

  bool LEXY = not(gb.cpu.FROM_CPU6);
  gc.chip.PIN_NC = LEXY;

  //----------
  // BOOT_CS

  pc.TYRO = nor(gb.A07, gb.A05, gb.A03, gb.A02, gb.A01, gb.A00);
  pc.TUFA = and(gb.A04, gb.A06);
  pc.TUGE = nand(pb.TYRO, pb.TUFA, pb.FFXX, pb.CPU_WR);
  pc.SATO = or(gb.D0, pb.TEPU);
  pc.TEXE = and(pb.CPU_RD, pb.FFXX, pb.TUFA, pb.TYRO);
  pc.TEPU = tock_pos(pa.TUGE, pb.TUGE, gb.p01.RESET2, pb.TEPU, pb.SATO);
  pc.SYPU = not(!pb.TEPU);
  pc.TERA = not(pb.TEPU);
  pc.TULO = nor(gb.A15, gb.A14, gb.A13, gb.A12, gb.A11, gb.A10, gb.A09, gb.A08);
  pc.TUTU = and(pb.TERA, pb.TULO);
  pc.ZORO = nor(gb.A15, gb.A14, gb.A13, gb.A12);
  pc.ZADU = nor(gb.A11, gb.A10, gb.A09, gb.A08);
  pc.YAZA = not(gb.p07.T1T2n);
  pc.YULA = and(pb.YAZA, pb.TUTU, pb.CPU_RD);
  pc.ZUFA = and(pb.ZORO, pb.ZADU);
  pc.ZADO = nand(pb.YULA, pb.ZUFA);
  pc.ZERY = not(pb.ZADO);

  if (pb.TEXE) {
    gc.D0 = pb.SYPU;
  }

  //----------
  // FF0F_RD/WR

  pc.SEMY = nor(gb.A07, gb.A06, gb.A05, gb.A04);
  pc.SAPA = and(gb.A00, gb.A01, gb.A02, gb.A03);
  pc.ROLO = and(pb.SEMY, pb.SAPA, pb.FFXX, pb.CPU_RD);
  pc.REFA = and(pb.SEMY, pb.SAPA, pb.FFXX, pb.CPU_WR);

  //----------
  // hram select

  pc.WALE = nand(gb.A00, gb.A01, gb.A02, gb.A03, gb.A04, gb.A05, gb.A06);
  pc.WOLY = nand(pb.WALE, gb.A07, pb.FFXX);
  pc.WUTA = not(pb.WOLY);

  //----------
  // weird debug thing

  pc.LECO = nor(gb.p01.BEDO, pb.T1nT2);

  pc.ROMY_00 = not(gb.chip.P10_B);
  pc.RYNE_01 = not(gb.chip.P10_B);
  pc.REJY_02 = not(gb.chip.P10_B);
  pc.RASE_03 = not(gb.chip.P10_B);
  pc.REKA_04 = not(gb.chip.P10_B);
  pc.ROWE_05 = not(gb.chip.P10_B);
  pc.RYKE_06 = not(gb.chip.P10_B);
  pc.RARU_07 = not(gb.chip.P10_B);

  if (pb.LECO) {
    gc.D0 = pb.ROMY_00;
    gc.D1 = pb.RYNE_01;
    gc.D2 = pb.REJY_02;
    gc.D3 = pb.RASE_03;
    gc.D4 = pb.REKA_04;
    gc.D5 = pb.ROWE_05;
    gc.D6 = pb.RYKE_06;
    gc.D7 = pb.RARU_07;
  }

  //----------
  // random address decoders

  pc.TONA = not(gb.A08);
  pc.TUNA = nand(gb.A15, gb.A14, gb.A13, gb.A12, gb.A11, gb.A10, gb.A09);
  pc.SYKE = nor(pb.TONA, pb.TUNA);
  pc.RYCU = not(pb.TUNA);
  pc.SOHA = not(pb.FFXX);
  pc.ROPE = nand(pb.RYCU, pb.SOHA);
  pc.BAKO = not(pb.SYKE);
  pc.SARO = not(pb.ROPE);

  //----------
  // bootrom address generation

  pc.ZYRA = not(gb.A07);
  pc.ZAGE = not(gb.A06);
  pc.ZABU = not(gb.A03);
  pc.ZOKE = not(gb.A02);

  pc.ZERA = not(gb.A05);
  pc.ZUFY = not(gb.A04);
  pc.ZYKY = and(pb.ZERA, pb.ZUFY);
  pc.ZYGA = and(pb.ZERA, gb.A04);
  pc.ZOVY = and(gb.A05, pb.ZUFY);
  pc.ZUKO = and(gb.A05, gb.A04);

  pc.ZUVY = not(gb.A01);
  pc.ZYBA = not(gb.A00);
  pc.ZOLE = and(pb.ZUVY, pb.ZYBA);
  pc.ZAJE = and(pb.ZUVY, gb.A00);
  pc.ZUBU = and(pb.ZYBA, gb.A01);
  pc.ZAPY = and(gb.A01, gb.A00);

  pc.ZETE = not(pb.ZOLE);
  pc.ZEFU = not(pb.ZAJE);
  pc.ZYRO = not(pb.ZUBU);
  pc.ZAPA = not(pb.ZAPY);

  //----------
  // FF60 debug reg

  pc.APET = or(gb.p07.T1nT2, gb.p07.T1T2n);
  pc.APER = nand(pb.APET, gb.A05, gb.A06, pb.CPU_WR, gb.p10.ANAP);
  pc.BURO_00 = tock_pos(pa.APER, pb.APER, gb.p01.RESET2, pb.BURO_00, gb.D0);
  pc.AMUT_01 = tock_pos(pa.APER, pb.APER, gb.p01.RESET2, pb.AMUT_01, gb.D1);
}