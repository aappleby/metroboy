#include "P14_Ch2Regs.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P14_Ch2Regs::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p14.FOGE = not(b.p09.CPU_RDn);
  c.p14.FAPE = and(b.p14.FOGE, b.p09.NET03);

  //----------
  // FF16 NR21

  c.p14.AGYN = nand(b.p10.APU_WR, b.p10.FF16); // FF16_WR
  c.p14.ASYP = not(b.p14.AGYN);
  c.p14.BENY = nor(b.p14.ASYP, b.p09.APU_RESET1, b.p15.ELOX);

  c.p14.BACU = and(b.p10.FF16, b.p10.APU_WR);
  c.p14.BUDU = not(b.p14.BACU);
  c.p14.BAMY = tock_pos(a.p14.BUDU, b.p14.BUDU, b.p09.APU_RESETn2, b.p14.BAMY, b.D7);
  c.p14.BERA = tock_pos(a.p14.BUDU, b.p14.BUDU, b.p09.APU_RESETn2, b.p14.BERA, b.D6);

  c.p14.BYGO = not(b.p09.CPU_RDn);
  c.p14.CORO = nand(b.p10.FF16, b.p14.BYGO);
  c.p14.CEKA = not(!b.p14.BAMY);
  c.p14.CECY = not(!b.p14.BERA);

  if (b.p14.CORO) {
    c.D7 = b.p14.CEKA;
    c.D6 = b.p14.CECY;
  }

  //----------
  // FF17 NR22

  c.p14.ENUF = and(b.p10.FF17, b.p10.APU_WR);
  c.p14.FYRY = not(b.p10.FF17);
  c.p14.GURU = or(b.p14.FYRY, b.p09.CPU_RDn);

  c.p14.GURE = not(b.p10.FF17);
  c.p14.GEXA = or(b.p14.GURE, b.p09.CPU_RDn);
  c.p14.GERE = and(b.p10.APU_WR, b.p10.FF17);
  c.p14.JEDE = not(b.p14.GERE);

  c.p14.FF17_D0 = tock_pos(a.p14.JEDE, b.p14.JEDE, b.p14.JYBU, b.p14.FF17_D0, b.D0);
  c.p14.FF17_D1 = tock_pos(a.p14.JEDE, b.p14.JEDE, b.p14.JYBU, b.p14.FF17_D1, b.D1);
  c.p14.FF17_D2 = tock_pos(a.p14.JEDE, b.p14.JEDE, b.p14.JYBU, b.p14.FF17_D2, b.D2);
  c.p14.FF17_D3 = tock_pos(a.p14.ENUF, b.p14.ENUF, b.p14.JYBU, b.p14.FF17_D3, b.D3);
  c.p14.FF17_D4 = tock_pos(a.p14.ENUF, b.p14.ENUF, b.p14.JYBU, b.p14.FF17_D4, b.D4);
  c.p14.FF17_D5 = tock_pos(a.p14.ENUF, b.p14.ENUF, b.p14.JYBU, b.p14.FF17_D5, b.D5);
  c.p14.FF17_D6 = tock_pos(a.p14.ENUF, b.p14.ENUF, b.p14.JYBU, b.p14.FF17_D6, b.D6);
  c.p14.FF17_D7 = tock_pos(a.p14.ENUF, b.p14.ENUF, b.p14.JYBU, b.p14.FF17_D7, b.D7);

  c.p14.HUVU = not(!b.p14.FF17_D0);
  c.p14.HYRE = not(!b.p14.FF17_D1);
  c.p14.HAVU = not(!b.p14.FF17_D2);
  c.p14.GENE = not(!b.p14.FF17_D3);
  c.p14.HUPE = not(!b.p14.FF17_D4);
  c.p14.HERE = not(!b.p14.FF17_D5);
  c.p14.HORO = not(!b.p14.FF17_D6);
  c.p14.HYRY = not(!b.p14.FF17_D7);

  if (b.p14.GEXA) {
    c.D0 = b.p14.HUVU;
    c.D1 = b.p14.HYRE;
    c.D2 = b.p14.HAVU;
  }

  if (b.p14.GURU) {
    c.D3 = b.p14.GENE;
    c.D4 = b.p14.HUPE;
    c.D5 = b.p14.HERE;
    c.D6 = b.p14.HORO;
    c.D7 = b.p14.HYRY;
  }

  //----------
  // FF18 NR23

  c.p14.NR23_WR1 = and(b.p10.FF18, b.p10.APU_WR);
  c.p14.NR32_WR2 = and(b.p10.FF18, b.p10.APU_WR);

  c.p14.NR23_WRn1 = not(b.p14.NR23_WR1);
  c.p14.NR32_WRn2 = not(b.p14.NR32_WR2);

  c.p14.FF18_D0 = tock_pos(a.p14.NR23_WRn1, b.p14.NR23_WRn1, b.p14.APU_RESETn1, b.p14.FF18_D0, b.D0);
  c.p14.FF18_D1 = tock_pos(a.p14.NR23_WRn1, b.p14.NR23_WRn1, b.p14.APU_RESETn1, b.p14.FF18_D1, b.D1);
  c.p14.FF18_D2 = tock_pos(a.p14.NR23_WRn1, b.p14.NR23_WRn1, b.p14.APU_RESETn1, b.p14.FF18_D2, b.D2);
  c.p14.FF18_D3 = tock_pos(a.p14.NR23_WRn1, b.p14.NR23_WRn1, b.p14.APU_RESETn1, b.p14.FF18_D3, b.D3);
  c.p14.FF18_D4 = tock_pos(a.p14.NR23_WRn1, b.p14.NR23_WRn1, b.p14.APU_RESETn1, b.p14.FF18_D4, b.D4);
  c.p14.FF18_D5 = tock_pos(a.p14.NR32_WRn2, b.p14.NR32_WRn2, b.p14.APU_RESETn1, b.p14.FF18_D5, b.D5);
  c.p14.FF18_D6 = tock_pos(a.p14.NR32_WRn2, b.p14.NR32_WRn2, b.p14.APU_RESETn1, b.p14.FF18_D6, b.D6);
  c.p14.FF18_D7 = tock_pos(a.p14.NR32_WRn2, b.p14.NR32_WRn2, b.p14.APU_RESETn1, b.p14.FF18_D7, b.D7);

  c.p14.CH2_FREQ_00 = count_pos(a.p15.DOCA,        b.p15.DOCA,        b.p15.COGU, b.p14.CH2_FREQ_00, b.p14.FF18_D0);
  c.p14.CH2_FREQ_01 = count_pos(a.p14.CH2_FREQ_00, b.p14.CH2_FREQ_00, b.p15.COGU, b.p14.CH2_FREQ_01, b.p14.FF18_D1);
  c.p14.CH2_FREQ_02 = count_pos(a.p14.CH2_FREQ_01, b.p14.CH2_FREQ_01, b.p15.COGU, b.p14.CH2_FREQ_02, b.p14.FF18_D2);
  c.p14.CH2_FREQ_03 = count_pos(a.p14.CH2_FREQ_02, b.p14.CH2_FREQ_02, b.p15.COGU, b.p14.CH2_FREQ_03, b.p14.FF18_D3);

  c.p14.EDEP = not(!b.p14.CH2_FREQ_03);

  c.p14.CH2_FREQ_04 = count_pos(a.p14.EDEP,        b.p14.EDEP,        b.p15.EROG, b.p14.CH2_FREQ_04, b.p14.FF18_D4);
  c.p14.CH2_FREQ_05 = count_pos(a.p14.CH2_FREQ_04, b.p14.CH2_FREQ_04, b.p15.EROG, b.p14.CH2_FREQ_05, b.p14.FF18_D5);
  c.p14.CH2_FREQ_06 = count_pos(a.p14.CH2_FREQ_05, b.p14.CH2_FREQ_05, b.p15.EROG, b.p14.CH2_FREQ_06, b.p14.FF18_D6);
  c.p14.CH2_FREQ_07 = count_pos(a.p14.CH2_FREQ_06, b.p14.CH2_FREQ_06, b.p15.EROG, b.p14.CH2_FREQ_07, b.p14.FF18_D7);

  c.p14.FAVA = not(!b.p14.CH2_FREQ_00);
  c.p14.FAJY = not(!b.p14.CH2_FREQ_01);
  c.p14.FEGU = not(!b.p14.CH2_FREQ_02);
  c.p14.FOSE = not(!b.p14.CH2_FREQ_03);
  c.p14.GERO = not(!b.p14.CH2_FREQ_04);
  c.p14.GAKY = not(!b.p14.CH2_FREQ_05);
  c.p14.GADU = not(!b.p14.CH2_FREQ_06);
  c.p14.GAZO = not(!b.p14.CH2_FREQ_07);

  c.p14.FERY = not(b.p10.DUCE);
  c.p14.GUZA = nor(b.p14.FERY, b.p14.FAPE);
  c.p14.FUTY = not(b.p14.GUZA);

  if (b.p14.FUTY) {
    c.D0 = b.p14.FAVA;
    c.D1 = b.p14.FAJY;
    c.D2 = b.p14.FEGU;
    c.D3 = b.p14.FOSE;
    c.D5 = b.p14.GERO; // wtf, d4 and d5 are switched on the schematic
    c.D4 = b.p14.GAKY;
    c.D6 = b.p14.GADU;
    c.D7 = b.p14.GAZO;
  }

  //----------
  // FF19 NR24

  c.p14.JENU = and(b.p10.FF19, b.p10.APU_WR);
  c.p14.KYSA = not(b.p14.JENU);
  c.p14.JUPY = tock_pos(a.p14.KYSA, b.p14.KYSA, b.p14.KYPU, c.p14.JUPY, b.D2);
  c.p14.JANY = tock_pos(a.p14.KYSA, b.p14.KYSA, b.p14.KYPU, c.p14.JANY, b.D1);
  c.p14.JEFU = tock_pos(a.p14.KYSA, b.p14.KYSA, b.p14.KYPU, c.p14.JEFU, b.D0);

  c.p14.DETA = nand(b.p10.APU_WR, b.p10.FF19);
  c.p14.GOTE = not(b.p10.FF19);
  c.p14.ETAP = tock_pos(a.p14.DETA, b.p14.DETA, b.p15.DERA, b.p14.ETAP, b.D7);
  c.p14.HYPO = or(b.p14.GOTE, b.p14.FAPE);

  c.p14.GALA = not(!b.p14.CH2_FREQ_07);
  c.p14.CH2_FREQ_08 = count_pos(a.p14.GALA, b.p14.GALA, b.p15.GYPA, c.p14.CH2_FREQ_08, b.p14.FF19_D0);
  c.p14.CH2_FREQ_09 = count_pos(a.p14.CH2_FREQ_08, b.p14.CH2_FREQ_08, b.p15.GYPA, c.p14.CH2_FREQ_09, b.p14.FF19_D1);
  c.p14.CH2_FREQ_10 = count_pos(a.p14.CH2_FREQ_09, b.p14.CH2_FREQ_09, b.p15.GYPA, c.p14.CH2_FREQ_10, b.p14.FF19_D2);

  c.p14.JEKE = not(!b.p14.CH2_FREQ_10);
  c.p14.JARO = not(!b.p14.CH2_FREQ_09);
  c.p14.HUNA = not(!b.p14.CH2_FREQ_08);

  if (b.p14.HYPO) {
    c.D2 = b.p14.JEKE;
    c.D1 = b.p14.JARO;
    c.D0 = b.p14.HUNA;
  }

  c.p14.GADO = not(b.p09.CPU_RDn);
  c.p14.EVYF = nor(b.p16.ANUJ, b.p10.FF19);
  c.p14.HUMA = nor(b.p10.FF19, b.p14.GADO);
  c.p14.EMER = tock_pos(a.p14.EVYF, b.p14.EVYF, b.p14.FAZO, b.p14.EMER, b.D6);
  c.p14.GOJY = b.p14.EMER;

  if (b.p14.HUMA) {
    c.D6 = b.p14.GOJY;
  }
}
