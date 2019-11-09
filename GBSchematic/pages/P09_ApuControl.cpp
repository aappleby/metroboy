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

  pc.AGUR = not(gb.APU_RESET);
  pc.AFAT = not(gb.APU_RESET);
  pc.ATYV = not(gb.APU_RESET);
  pc.DAPA = not(gb.APU_RESET);
  pc.KAME = not(gb.APU_RESET);

  gc.APU_RESETn  = pb.AGUR;
  gc.APU_RESET2n = pb.AFAT;
  gc.APU_RESET3n = pb.ATYV;
  gc.APU_RESET4n = pb.DAPA;
  gc.APU_RESET5n = pb.KAME;

  //----------
  // clock dividers

  pc.AJER = tock_pos(ga.APUV_4M, gb.APUV_4M, gb.APU_RESET3n, pb.AJER, !pb.AJER);
  gc.AJER_2M = pb.AJER;
  gc.AJER_2Mn = not(pb.AJER);

  pc.BATA = not(gb.AJER_2M);
  pc.CALO = tock_pos(pa.BATA, pb.BATA, gb.APU_RESETn, pb.CALO, !pb.CALO);
  pc.DYFA = not(!pb.CALO);
  gc.DYFA_1M = pb.DYFA;

  //----------
  // main logic chunk

  pc.HAWU = nand(pb.FF26, gb.APU_WR);
  pc.BOPY = nand(gb.APU_WR, pb.FF26);
  pc.HAPO = not(gb.RESET2);

  pc.GUFO = not(pb.HAPO);
  pc.HADA = tock_pos(pa.HAWU, pb.HAWU, pb.GUFO, pb.HADA, gb.cpu.D7);

  pc.JYRO = or(pb.HAPO, !pb.HADA);
  pc.KEPY = not(pb.JYRO);

  pc.KUBY = not(pb.JYRO);
  pc.KEBA = not(pb.KUBY);

  gc.APU_RESET = pb.KEBA;

  pc.ETUC = and(gb.APU_WR, pb.FF26);
  pc.EFOP = and(gb.cpu.D4, gb.T1nT2); // schematic bug, said FROM_CPU
  pc.FOKU = not(pb.ETUC);
  pc.FERO = tock_pos(pa.FOKU, pb.FOKU, pb.KEPY, pb.FERO, pb.EFOP);
  pc.EDEK = not(!pb.FERO);

  pc.BOWY = tock_pos(pa.BOPY, pb.BOPY, pb.KEPY, pb.BOWY, gb.cpu.D5);
  pc.BAZA = tock_pos(ga.AJER_2M, gb.AJER_2M, gb.APU_RESET3n, pb.BAZA, pb.BOWY);
  pc.CELY = mux2(pb.BAZA, gb.BYFE_128, gb.NET03);
  pc.CONE = not(pb.CELY);
  pc.CATE = not(pb.CONE);

  pc.AGUZ = not(gb.CPU_RD);
  pc.KYDU = not(gb.CPU_RDn);
  pc.JURE = nand(pb.KYDU, pb.FF26);
  pc.HOPE = not(!pb.HADA);

  gc.CPU_RDn = pb.AGUZ;
  gc.NET03 = pb.EDEK; // what is this exactly?

  if (pb.JURE) {
    gc.cpu.D7 = pb.HOPE;
  }

  //----------
  // FF24 NR50

  pc.BYMA = not(pb.FF24);
  pc.BEFU = nor(pb.AGUZ, pb.BYMA);
  pc.ADAK = not(pb.BEFU);
  
  pc.BOSU = nand(pb.FF24, gb.APU_WR); // BUG - APU_WR
  pc.BAXY = not(pb.BOSU);
  pc.BUBU = not(pb.BAXY);
  pc.BOWE = not(pb.BOSU);
  pc.ATAF = not(pb.BOWE);

  pc.APEG = tock_pos(pa.ATAF, pb.ATAF, pb.JYRO, pc.APEG, gb.cpu.D0);
  pc.BYGA = tock_pos(pa.ATAF, pb.ATAF, pb.JYRO, pc.BYGA, gb.cpu.D1);
  pc.AGER = tock_pos(pa.ATAF, pb.ATAF, pb.JYRO, pc.AGER, gb.cpu.D2);
  pc.APOS = tock_pos(pa.ATAF, pb.ATAF, pb.JYRO, pc.APOS, gb.cpu.D3);
  pc.BYRE = tock_pos(pa.BUBU, pb.BUBU, pb.JYRO, pc.BYRE, gb.cpu.D4);
  pc.BUMO = tock_pos(pa.BUBU, pb.BUBU, pb.JYRO, pc.BUMO, gb.cpu.D5);
  pc.COZU = tock_pos(pa.BUBU, pb.BUBU, pb.JYRO, pc.COZU, gb.cpu.D6);
  pc.BEDU = tock_pos(pa.BUBU, pb.BUBU, pb.JYRO, pc.BEDU, gb.cpu.D7);

  pc.AKOD = not(!pb.APEG);
  pc.AWED = not(!pb.BYGA);
  pc.AVUD = not(!pb.AGER);
  pc.AXEM = not(!pb.APOS);
  pc.AMAD = not(!pb.BYRE);
  pc.ARUX = not(!pb.BUMO);
  pc.BOCY = not(!pb.COZU);
  pc.ATUM = not(!pb.BEDU);

  if (pb.ADAK) {
    gc.cpu.D0 = pb.AKOD;
    gc.cpu.D1 = pb.AWED;
    gc.cpu.D2 = pb.AVUD;
    gc.cpu.D3 = pb.AXEM;
    gc.cpu.D4 = pb.AMAD;
    gc.cpu.D5 = pb.ARUX;
    gc.cpu.D6 = pb.BOCY;
    gc.cpu.D7 = pb.ATUM;
  }

  //----------
  // FF25 NR51

  pc.BUPO = nand(pb.FF25, gb.APU_WR); // BUG APU_WR
  pc.BONO = not(pb.BUPO);
  pc.BYFA = not(pb.BUPO);

  pc.BOGU = tock_pos(pa.BONO, pb.BONO, pb.JYRO, pc.BOGU, gb.cpu.D1);
  pc.BAFO = tock_pos(pa.BONO, pb.BONO, pb.JYRO, pc.BAFO, gb.cpu.D2);
  pc.ATUF = tock_pos(pa.BONO, pb.BONO, pb.JYRO, pc.ATUF, gb.cpu.D3);
  pc.ANEV = tock_pos(pa.BONO, pb.BONO, pb.JYRO, pc.ANEV, gb.cpu.D0);
  pc.BEPU = tock_pos(pa.BYFA, pb.BYFA, pb.JYRO, pc.BEPU, gb.cpu.D7);
  pc.BEFO = tock_pos(pa.BYFA, pb.BYFA, pb.JYRO, pc.BEFO, gb.cpu.D6);
  pc.BUME = tock_pos(pa.BYFA, pb.BYFA, pb.JYRO, pc.BUME, gb.cpu.D4);
  pc.BOFA = tock_pos(pa.BYFA, pb.BYFA, pb.JYRO, pc.BOFA, gb.cpu.D5);

  pc.GEPA = not(pb.FF25);
  pc.HEFA = nor(pb.GEPA, gb.CPU_RDn);
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
    gc.cpu.D1 = pb.CAPU;
    gc.cpu.D2 = pb.CAGA;
    gc.cpu.D3 = pb.BOCA;
    gc.cpu.D0 = pb.BUZU;
    gc.cpu.D7 = pb.CERE;
    gc.cpu.D6 = pb.CADA;
    gc.cpu.D4 = pb.CAVU;
    gc.cpu.D5 = pb.CUDU;
  }

  //----------
  // FF26 NR52

  pc.CETO = not(gb.CPU_RDn);
  pc.KAZO = not(gb.CPU_RDn);
  pc.CURU = not(gb.CPU_RDn);
  pc.GAXO = not(gb.CPU_RDn);

  pc.DOLE = nand(pb.FF26, pb.CETO);
  pc.KAMU = nand(pb.FF26, pb.KAZO);
  pc.DURU = nand(pb.FF26, pb.CURU);
  pc.FEWA = nand(pb.FF26, pb.GAXO);

  pc.COTO = not(gb.CH1_ACTIVEn);
  pc.KOGE = not(gb.CH4_ACTIVEn);
  pc.EFUS = not(gb.CH2_ACTIVEn);
  pc.FATE = not(gb.CH3_ACTIVEn);

  if (pb.DOLE) { gc.cpu.D0 = pb.COTO; }
  if (pb.DURU) { gc.cpu.D1 = pb.EFUS; }
  if (pb.FEWA) { gc.cpu.D2 = pb.FATE; }
  if (pb.KAMU) { gc.cpu.D3 = pb.KOGE; }
}