#include "P07_SysDecode.h"
#include "Gameboy.h"
#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P07_SysDecode::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // debug enable signals

  c.p07.UBET = not(b.chip.T1);
  c.p07.UVAR = not(b.chip.T2);
  c.p07.UPOJ = nand(b.p07.UBET, b.p07.UVAR, b.chip.RST);
  c.p07.UNOR = and(b.chip.T2, b.p07.UBET);
  c.p07.UMUT = and(b.chip.T1, b.p07.UVAR);
  
  //----------
  // debug override of CPU_RD/CPU_WR

  c.p07.UBAL = mux2(b.chip.WR_C, b.p01.CPU_WR_SYNC, b.p07.T1nT2);
  c.p07.UJYV = mux2(b.chip.RD_C, b.cpu.CPU_RAW_RD,  b.p07.T1nT2);

  c.p07.TAPU = not(b.p07.UBAL);
  c.p07.DYKY = not(b.p07.TAPU);
  c.p07.CUPA = not(b.p07.DYKY);
  c.p07.TEDO = not(b.p07.UJYV);
  c.p07.AJAS = not(b.p07.TEDO);
  c.p07.ASOT = not(b.p07.AJAS);

  //----------
  // doesn't do anything

  bool LEXY = not(b.cpu.FROM_CPU6);
  c.chip.PIN_NC = LEXY;

  //----------
  // BOOT_CS

  c.p07.TYRO = nor(b.A07, b.A05, b.A03, b.A02, b.A01, b.A00);
  c.p07.TUFA = and(b.A04, b.A06);
  c.p07.TUGE = nand(b.p07.TYRO, b.p07.TUFA, b.p07.FFXX, b.p07.CPU_WR);
  c.p07.SATO = or(b.D0, b.p07.TEPU);
  c.p07.TEXE = and(b.p07.CPU_RD, b.p07.FFXX, b.p07.TUFA, b.p07.TYRO);
  c.p07.TEPU = tock_pos(a.p07.TUGE, b.p07.TUGE, b.p01.RESET2, b.p07.TEPU, b.p07.SATO);
  c.p07.SYPU = not(!b.p07.TEPU);
  c.p07.TERA = not(b.p07.TEPU);
  c.p07.TULO = nor(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09, b.A08);
  c.p07.TUTU = and(b.p07.TERA, b.p07.TULO);
  c.p07.ZORO = nor(b.A15, b.A14, b.A13, b.A12);
  c.p07.ZADU = nor(b.A11, b.A10, b.A09, b.A08);
  c.p07.YAZA = not(b.p07.T1T2n);
  c.p07.YULA = and(b.p07.YAZA, b.p07.TUTU, b.p07.CPU_RD);
  c.p07.ZUFA = and(b.p07.ZORO, b.p07.ZADU);
  c.p07.ZADO = nand(b.p07.YULA, b.p07.ZUFA);
  c.p07.ZERY = not(b.p07.ZADO);

  if (b.p07.TEXE) {
    c.D0 = b.p07.SYPU;
  }

  //----------
  // FF0F_RD/WR

  c.p07.SEMY = nor(b.A07, b.A06, b.A05, b.A04);
  c.p07.SAPA = and(b.A00, b.A01, b.A02, b.A03);
  c.p07.ROLO = and(b.p07.SEMY, b.p07.SAPA, b.p07.FFXX, b.p07.CPU_RD);
  c.p07.REFA = and(b.p07.SEMY, b.p07.SAPA, b.p07.FFXX, b.p07.CPU_WR);

  //----------
  // hram select

  c.p07.WALE = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
  c.p07.WOLY = nand(b.p07.WALE, b.A07, b.p07.FFXX);
  c.p07.WUTA = not(b.p07.WOLY);

  //----------
  // weird debug thing

  c.p07.LECO = nor(b.p01.BEDO, b.p07.T1nT2);

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

  c.p07.TONA = not(b.A08);
  c.p07.TUNA = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
  c.p07.SYKE = nor(b.p07.TONA, b.p07.TUNA);
  c.p07.RYCU = not(b.p07.TUNA);
  c.p07.SOHA = not(b.p07.FFXX);
  c.p07.ROPE = nand(b.p07.RYCU, b.p07.SOHA);
  c.p07.BAKO = not(b.p07.SYKE);
  c.p07.SARO = not(b.p07.ROPE);

  //----------
  // bootrom address generation

  c.p07.ZYRA = not(b.A07);
  c.p07.ZAGE = not(b.A06);
  c.p07.ZABU = not(b.A03);
  c.p07.ZOKE = not(b.A02);

  c.p07.ZERA = not(b.A05);
  c.p07.ZUFY = not(b.A04);
  c.p07.ZYKY = and(b.p07.ZERA, b.p07.ZUFY);
  c.p07.ZYGA = and(b.p07.ZERA, b.A04);
  c.p07.ZOVY = and(b.A05, b.p07.ZUFY);
  c.p07.ZUKO = and(b.A05, b.A04);

  c.p07.ZUVY = not(b.A01);
  c.p07.ZYBA = not(b.A00);
  c.p07.ZOLE = and(b.p07.ZUVY, b.p07.ZYBA);
  c.p07.ZAJE = and(b.p07.ZUVY, b.A00);
  c.p07.ZUBU = and(b.p07.ZYBA, b.A01);
  c.p07.ZAPY = and(b.A01, b.A00);

  c.p07.ZETE = not(b.p07.ZOLE);
  c.p07.ZEFU = not(b.p07.ZAJE);
  c.p07.ZYRO = not(b.p07.ZUBU);
  c.p07.ZAPA = not(b.p07.ZAPY);

  //----------
  // FF60 debug reg

  c.p07.APET = or(b.p07.T1nT2, b.p07.T1T2n);
  c.p07.APER = nand(b.p07.APET, b.A05, b.A06, b.p07.CPU_WR, b.p10.ANAP);
  c.p07.BURO_00 = tock_pos(a.p07.APER, b.p07.APER, b.p01.RESET2, b.p07.BURO_00, b.D0);
  c.p07.AMUT_01 = tock_pos(a.p07.APER, b.p07.APER, b.p01.RESET2, b.p07.AMUT_01, b.D1);
}