#include "P09_ApuControl.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P09_ApuControl::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //---------
  // reset tree

  c.p09.AGUR = not(b.p09.APU_RESET);
  c.p09.AFAT = not(b.p09.APU_RESET);
  c.p09.ATYV = not(b.p09.APU_RESET);
  c.p09.DAPA = not(b.p09.APU_RESET);
  c.p09.KAME = not(b.p09.APU_RESET);

  //----------
  // clock dividers

  c.p09.AJER = tock_pos(a.p01.APUV_4M, b.p01.APUV_4M, b.p09.APU_RESET3n, b.p09.AJER, !b.p09.AJER);

  c.p09.BATA = not(b.p09.AJER_2M);
  c.p09.CALO = tock_pos(a.p09.BATA, b.p09.BATA, b.p09.APU_RESETn, b.p09.CALO, !b.p09.CALO);
  c.p09.DYFA = not(!b.p09.CALO);

  //----------
  // main logic chunk

  c.p09.HAWU = nand(b.p10.FF26, b.p10.APU_WR);
  c.p09.BOPY = nand(b.p10.APU_WR, b.p10.FF26);
  c.p09.HAPO = not(b.p01.RESET2);

  c.p09.GUFO = not(b.p09.HAPO);
  c.p09.HADA = tock_pos(a.p09.HAWU, b.p09.HAWU, b.p09.GUFO, b.p09.HADA, b.D7);

  c.p09.JYRO = or(b.p09.HAPO, !b.p09.HADA);
  c.p09.KEPY = not(b.p09.JYRO);

  c.p09.KUBY = not(b.p09.JYRO);
  c.p09.KEBA = not(b.p09.KUBY);

  c.p09.ETUC = and(b.p10.APU_WR, b.p10.FF26);
  c.p09.EFOP = and(b.D4, b.p07.T1nT2); // schematic bug, said FROM_CPU
  c.p09.FOKU = not(b.p09.ETUC);
  c.p09.FERO = tock_pos(a.p09.FOKU, b.p09.FOKU, b.p09.KEPY, b.p09.FERO, b.p09.EFOP);
  c.p09.EDEK = not(!b.p09.FERO);

  c.p09.BOWY = tock_pos(a.p09.BOPY, b.p09.BOPY, b.p09.KEPY, b.p09.BOWY, b.D5);
  c.p09.BAZA = tock_pos(a.p09.AJER_2M, b.p09.AJER_2M, b.p09.APU_RESET3n, b.p09.BAZA, b.p09.BOWY);
  c.p09.CELY = mux2(b.p09.BAZA, b.p01.BYFE_128, b.p09.NET03);
  c.p09.CONE = not(b.p09.CELY);
  c.p09.CATE = not(b.p09.CONE);

  c.p09.AGUZ = not(b.p07.CPU_RD);
  c.p09.KYDU = not(b.p09.CPU_RDn);
  c.p09.JURE = nand(b.p09.KYDU, b.p10.FF26);
  c.p09.HOPE = not(!b.p09.HADA);

  if (b.p09.JURE) {
    c.D7 = b.p09.HOPE;
  }

  //----------
  // FF24 NR50

  c.p09.BYMA = not(b.p10.FF24);
  c.p09.BEFU = nor(b.p09.AGUZ, b.p09.BYMA);
  c.p09.ADAK = not(b.p09.BEFU);
  
  c.p09.BOSU = nand(b.p10.FF24, b.p10.APU_WR);
  c.p09.BAXY = not(b.p09.BOSU);
  c.p09.BUBU = not(b.p09.BAXY);
  c.p09.BOWE = not(b.p09.BOSU);
  c.p09.ATAF = not(b.p09.BOWE);

  c.p09.APEG = tock_pos(a.p09.ATAF, b.p09.ATAF, b.p09.JYRO, c.p09.APEG, b.D0);
  c.p09.BYGA = tock_pos(a.p09.ATAF, b.p09.ATAF, b.p09.JYRO, c.p09.BYGA, b.D1);
  c.p09.AGER = tock_pos(a.p09.ATAF, b.p09.ATAF, b.p09.JYRO, c.p09.AGER, b.D2);
  c.p09.APOS = tock_pos(a.p09.ATAF, b.p09.ATAF, b.p09.JYRO, c.p09.APOS, b.D3);
  c.p09.BYRE = tock_pos(a.p09.BUBU, b.p09.BUBU, b.p09.JYRO, c.p09.BYRE, b.D4);
  c.p09.BUMO = tock_pos(a.p09.BUBU, b.p09.BUBU, b.p09.JYRO, c.p09.BUMO, b.D5);
  c.p09.COZU = tock_pos(a.p09.BUBU, b.p09.BUBU, b.p09.JYRO, c.p09.COZU, b.D6);
  c.p09.BEDU = tock_pos(a.p09.BUBU, b.p09.BUBU, b.p09.JYRO, c.p09.BEDU, b.D7);

  c.p09.AKOD = not(!b.p09.APEG);
  c.p09.AWED = not(!b.p09.BYGA);
  c.p09.AVUD = not(!b.p09.AGER);
  c.p09.AXEM = not(!b.p09.APOS);
  c.p09.AMAD = not(!b.p09.BYRE);
  c.p09.ARUX = not(!b.p09.BUMO);
  c.p09.BOCY = not(!b.p09.COZU);
  c.p09.ATUM = not(!b.p09.BEDU);

  if (b.p09.ADAK) {
    c.D0 = b.p09.AKOD;
    c.D1 = b.p09.AWED;
    c.D2 = b.p09.AVUD;
    c.D3 = b.p09.AXEM;
    c.D4 = b.p09.AMAD;
    c.D5 = b.p09.ARUX;
    c.D6 = b.p09.BOCY;
    c.D7 = b.p09.ATUM;
  }

  //----------
  // FF25 NR51

  c.p09.BUPO = nand(b.p10.FF25, b.p10.APU_WR); // BUG APU_WR
  c.p09.BONO = not(b.p09.BUPO);
  c.p09.BYFA = not(b.p09.BUPO);

  c.p09.BOGU = tock_pos(a.p09.BONO, b.p09.BONO, b.p09.JYRO, c.p09.BOGU, b.D1);
  c.p09.BAFO = tock_pos(a.p09.BONO, b.p09.BONO, b.p09.JYRO, c.p09.BAFO, b.D2);
  c.p09.ATUF = tock_pos(a.p09.BONO, b.p09.BONO, b.p09.JYRO, c.p09.ATUF, b.D3);
  c.p09.ANEV = tock_pos(a.p09.BONO, b.p09.BONO, b.p09.JYRO, c.p09.ANEV, b.D0);
  c.p09.BEPU = tock_pos(a.p09.BYFA, b.p09.BYFA, b.p09.JYRO, c.p09.BEPU, b.D7);
  c.p09.BEFO = tock_pos(a.p09.BYFA, b.p09.BYFA, b.p09.JYRO, c.p09.BEFO, b.D6);
  c.p09.BUME = tock_pos(a.p09.BYFA, b.p09.BYFA, b.p09.JYRO, c.p09.BUME, b.D4);
  c.p09.BOFA = tock_pos(a.p09.BYFA, b.p09.BYFA, b.p09.JYRO, c.p09.BOFA, b.D5);

  c.p09.GEPA = not(b.p10.FF25);
  c.p09.HEFA = nor(b.p09.GEPA, b.p09.CPU_RDn);
  c.p09.GUMU = not(b.p09.HEFA);

  c.p09.CAPU = not(!b.p09.BOGU);
  c.p09.CAGA = not(!b.p09.BAFO);
  c.p09.BOCA = not(!b.p09.ATUF);
  c.p09.BUZU = not(!b.p09.ANEV);
  c.p09.CERE = not(!b.p09.BEPU);
  c.p09.CADA = not(!b.p09.BEFO);
  c.p09.CAVU = not(!b.p09.BUME);
  c.p09.CUDU = not(!b.p09.BOFA);

  if (b.p09.GUMU) {
    c.D1 = b.p09.CAPU;
    c.D2 = b.p09.CAGA;
    c.D3 = b.p09.BOCA;
    c.D0 = b.p09.BUZU;
    c.D7 = b.p09.CERE;
    c.D6 = b.p09.CADA;
    c.D4 = b.p09.CAVU;
    c.D5 = b.p09.CUDU;
  }

  //----------
  // FF26 NR52

  c.p09.CETO = not(b.p09.CPU_RDn);
  c.p09.KAZO = not(b.p09.CPU_RDn);
  c.p09.CURU = not(b.p09.CPU_RDn);
  c.p09.GAXO = not(b.p09.CPU_RDn);

  c.p09.DOLE = nand(b.p10.FF26, b.p09.CETO);
  c.p09.KAMU = nand(b.p10.FF26, b.p09.KAZO);
  c.p09.DURU = nand(b.p10.FF26, b.p09.CURU);
  c.p09.FEWA = nand(b.p10.FF26, b.p09.GAXO);

  c.p09.COTO = not(b.p13.CH1_ACTIVEn);
  c.p09.EFUS = not(b.p15.CH2_ACTIVEn);
  c.p09.FATE = not(b.p18.CH3_ACTIVEn);
  c.p09.KOGE = not(b.p20.CH4_ACTIVEn);

  if (b.p09.DOLE) { c.D0 = b.p09.COTO; }
  if (b.p09.DURU) { c.D1 = b.p09.EFUS; }
  if (b.p09.FEWA) { c.D2 = b.p09.FATE; }
  if (b.p09.KAMU) { c.D3 = b.p09.KOGE; }
}