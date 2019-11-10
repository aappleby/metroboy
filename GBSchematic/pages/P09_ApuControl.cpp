#include "P09_ApuControl.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P09_ApuControl::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P09_ApuControl pa = ga.p09;
  const P09_ApuControl pb = gb.p09;
  P09_ApuControl pc = gc.p09;

  //---------
  // reset tree

  pc.AGUR = not(pb.APU_RESET);
  pc.AFAT = not(pb.APU_RESET);
  pc.ATYV = not(pb.APU_RESET);
  pc.DAPA = not(pb.APU_RESET);
  pc.KAME = not(pb.APU_RESET);

  //----------
  // clock dividers

  pc.AJER = tock_pos(ga.p01.APUV_4M, gb.p01.APUV_4M, pb.APU_RESET3n, pb.AJER, !pb.AJER);

  pc.BATA = not(pb.AJER_2M);
  pc.CALO = tock_pos(pa.BATA, pb.BATA, pb.APU_RESETn, pb.CALO, !pb.CALO);
  pc.DYFA = not(!pb.CALO);

  //----------
  // main logic chunk

  pc.HAWU = nand(pb.FF26, gb.p10.APU_WR);
  pc.BOPY = nand(gb.p10.APU_WR, pb.FF26);
  pc.HAPO = not(gb.p01.RESET2);

  pc.GUFO = not(pb.HAPO);
  pc.HADA = tock_pos(pa.HAWU, pb.HAWU, pb.GUFO, pb.HADA, gb.D7);

  pc.JYRO = or(pb.HAPO, !pb.HADA);
  pc.KEPY = not(pb.JYRO);

  pc.KUBY = not(pb.JYRO);
  pc.KEBA = not(pb.KUBY);

  pc.ETUC = and(gb.p10.APU_WR, pb.FF26);
  pc.EFOP = and(gb.D4, gb.p07.T1nT2); // schematic bug, said FROM_CPU
  pc.FOKU = not(pb.ETUC);
  pc.FERO = tock_pos(pa.FOKU, pb.FOKU, pb.KEPY, pb.FERO, pb.EFOP);
  pc.EDEK = not(!pb.FERO);

  pc.BOWY = tock_pos(pa.BOPY, pb.BOPY, pb.KEPY, pb.BOWY, gb.D5);
  pc.BAZA = tock_pos(pa.AJER_2M, pb.AJER_2M, pb.APU_RESET3n, pb.BAZA, pb.BOWY);
  pc.CELY = mux2(pb.BAZA, gb.p01.BYFE_128, pb.NET03);
  pc.CONE = not(pb.CELY);
  pc.CATE = not(pb.CONE);

  pc.AGUZ = not(gb.p07.CPU_RD);
  pc.KYDU = not(gb.p09.CPU_RDn);
  pc.JURE = nand(pb.KYDU, pb.FF26);
  pc.HOPE = not(!pb.HADA);

  if (pb.JURE) {
    gc.D7 = pb.HOPE;
  }

  //----------
  // FF24 NR50

  pc.BYMA = not(pb.FF24);
  pc.BEFU = nor(pb.AGUZ, pb.BYMA);
  pc.ADAK = not(pb.BEFU);
  
  pc.BOSU = nand(pb.FF24, gb.p10.APU_WR);
  pc.BAXY = not(pb.BOSU);
  pc.BUBU = not(pb.BAXY);
  pc.BOWE = not(pb.BOSU);
  pc.ATAF = not(pb.BOWE);

  pc.APEG = tock_pos(pa.ATAF, pb.ATAF, pb.JYRO, pc.APEG, gb.D0);
  pc.BYGA = tock_pos(pa.ATAF, pb.ATAF, pb.JYRO, pc.BYGA, gb.D1);
  pc.AGER = tock_pos(pa.ATAF, pb.ATAF, pb.JYRO, pc.AGER, gb.D2);
  pc.APOS = tock_pos(pa.ATAF, pb.ATAF, pb.JYRO, pc.APOS, gb.D3);
  pc.BYRE = tock_pos(pa.BUBU, pb.BUBU, pb.JYRO, pc.BYRE, gb.D4);
  pc.BUMO = tock_pos(pa.BUBU, pb.BUBU, pb.JYRO, pc.BUMO, gb.D5);
  pc.COZU = tock_pos(pa.BUBU, pb.BUBU, pb.JYRO, pc.COZU, gb.D6);
  pc.BEDU = tock_pos(pa.BUBU, pb.BUBU, pb.JYRO, pc.BEDU, gb.D7);

  pc.AKOD = not(!pb.APEG);
  pc.AWED = not(!pb.BYGA);
  pc.AVUD = not(!pb.AGER);
  pc.AXEM = not(!pb.APOS);
  pc.AMAD = not(!pb.BYRE);
  pc.ARUX = not(!pb.BUMO);
  pc.BOCY = not(!pb.COZU);
  pc.ATUM = not(!pb.BEDU);

  if (pb.ADAK) {
    gc.D0 = pb.AKOD;
    gc.D1 = pb.AWED;
    gc.D2 = pb.AVUD;
    gc.D3 = pb.AXEM;
    gc.D4 = pb.AMAD;
    gc.D5 = pb.ARUX;
    gc.D6 = pb.BOCY;
    gc.D7 = pb.ATUM;
  }

  //----------
  // FF25 NR51

  pc.BUPO = nand(pb.FF25, gb.p10.APU_WR); // BUG APU_WR
  pc.BONO = not(pb.BUPO);
  pc.BYFA = not(pb.BUPO);

  pc.BOGU = tock_pos(pa.BONO, pb.BONO, pb.JYRO, pc.BOGU, gb.D1);
  pc.BAFO = tock_pos(pa.BONO, pb.BONO, pb.JYRO, pc.BAFO, gb.D2);
  pc.ATUF = tock_pos(pa.BONO, pb.BONO, pb.JYRO, pc.ATUF, gb.D3);
  pc.ANEV = tock_pos(pa.BONO, pb.BONO, pb.JYRO, pc.ANEV, gb.D0);
  pc.BEPU = tock_pos(pa.BYFA, pb.BYFA, pb.JYRO, pc.BEPU, gb.D7);
  pc.BEFO = tock_pos(pa.BYFA, pb.BYFA, pb.JYRO, pc.BEFO, gb.D6);
  pc.BUME = tock_pos(pa.BYFA, pb.BYFA, pb.JYRO, pc.BUME, gb.D4);
  pc.BOFA = tock_pos(pa.BYFA, pb.BYFA, pb.JYRO, pc.BOFA, gb.D5);

  pc.GEPA = not(pb.FF25);
  pc.HEFA = nor(pb.GEPA, gb.p09.CPU_RDn);
  pc.GUMU = not(pb.HEFA);

  pc.CAPU = not(!pb.BOGU);
  pc.CAGA = not(!pb.BAFO);
  pc.BOCA = not(!pb.ATUF);
  pc.BUZU = not(!pb.ANEV);
  pc.CERE = not(!pb.BEPU);
  pc.CADA = not(!pb.BEFO);
  pc.CAVU = not(!pb.BUME);
  pc.CUDU = not(!pb.BOFA);

  if (pb.GUMU) {
    gc.D1 = pb.CAPU;
    gc.D2 = pb.CAGA;
    gc.D3 = pb.BOCA;
    gc.D0 = pb.BUZU;
    gc.D7 = pb.CERE;
    gc.D6 = pb.CADA;
    gc.D4 = pb.CAVU;
    gc.D5 = pb.CUDU;
  }

  //----------
  // FF26 NR52

  pc.CETO = not(gb.p09.CPU_RDn);
  pc.KAZO = not(gb.p09.CPU_RDn);
  pc.CURU = not(gb.p09.CPU_RDn);
  pc.GAXO = not(gb.p09.CPU_RDn);

  pc.DOLE = nand(pb.FF26, pb.CETO);
  pc.KAMU = nand(pb.FF26, pb.KAZO);
  pc.DURU = nand(pb.FF26, pb.CURU);
  pc.FEWA = nand(pb.FF26, pb.GAXO);

  pc.COTO = not(gb.p13.CH1_ACTIVEn);
  pc.EFUS = not(gb.p15.CH2_ACTIVEn);
  pc.FATE = not(gb.p18.CH3_ACTIVEn);
  pc.KOGE = not(gb.p20.CH4_ACTIVEn);

  if (pb.DOLE) { gc.D0 = pb.COTO; }
  if (pb.DURU) { gc.D1 = pb.EFUS; }
  if (pb.FEWA) { gc.D2 = pb.FATE; }
  if (pb.KAMU) { gc.D3 = pb.KOGE; }
}