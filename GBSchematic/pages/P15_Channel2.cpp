#pragma once

#include "../Schematics.h"
#include "Gameboy.h"


struct P15_Channel2 {
  bool HOTA,KATY,JYNA,KYLO,KENE;

  // EG timer
  bool JORE,JONA,JEVY;

  bool KYVO;

  bool JOPA;
  bool HEPO,DOPE;
  bool JAKE;

  bool FF16_D6;
  bool FF16_D7;

  bool FF16_D6n;
  bool FF16_D7n;

  bool FF17_D0n;
  bool FF17_D1n;
  bool FF17_D2n;
  bool FF17_D3n;
  
  bool FF17_D0;
  bool FF17_D1;
  bool FF17_D2;
  bool FF17_D3;
  bool FF17_D4;
  bool FF17_D5;
  bool FF17_D6;
  bool FF17_D7;

  bool HAFE,HYPA;
  bool CYWU,DERA,DALA,CELO,ELOX,DOXA,DORY,CEXE;
  bool HYLE,HYLY,JEME,GALU,GYKO,ETUK,DAVU,FUJY,GYRE,DUJU,COGU,EROG,GYPA;
  bool BUWE,AZEG,ATEP,CAZA,BYHO,BUFO,BODO,CEMO,BUTA,CAMA,DOCA;
  bool FOPY,FENA,ETUP,FOMY,FARU,FETE,GAFA,FENO,EMYR,ERAT,FYRE,GUFY;

  bool CYRE,DEME,DORA,DYRO,ESYK,ARES,FUTE,DANE,DEFU;
  bool ERYC_00,CERA_01,CONU_02,CAME_03,BUVA_04,AKYD_05;
  bool BYMO,AGET,BUKO;

  bool GADE,HOLY,JUPU,HOFO;

  bool CULE,DYMU,DUGE,DARE,EGOG,DOMO,DYTA,DOJU,DOVE,EXES;
  bool DOME;
  
  bool CYSE,BONU,ANYV,ANAN,AMOV,ASOG;

  bool CANO_00,CAGY_01,DYVE_02;
  bool BUKO_OUT;    // P15.BUKO

  //----------

  bool FF19_D6;     // P14.EMER
  bool FF19_D7;     // ???

  bool CH2_ACTIVEn; // P15.DEFU
  bool CH2_AMP_ENn; // P15.FUTE
  bool FF16_WR;     // ???

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};

void P15_Channel2::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& /*gc*/) {
  const P15_Channel2 pa = {};
  const P15_Channel2 pb = {};
  P15_Channel2 pc;

  pc.HOTA = not(gb.BYFE_128);
  pc.KATY = not(gb.APU_RESET);
  pc.JYNA = tock_pos(pa.HOTA, pb.HOTA, pb.KATY, pb.JYNA, !pb.JYNA);
  pc.KYLO = not(pb.JYNA);
  pc.KENE = not(pb.KYLO);

  pc.JORE = count_pos(pa.KENE, pb.KENE, pb.JAKE, pb.JORE, pb.FF17_D0n);
  pc.JONA = count_pos(pa.JORE, pb.JORE, pb.JAKE, pb.JONA, pb.FF17_D1n);
  pc.JEVY = count_pos(pa.JONA, pb.JONA, pb.JAKE, pb.JEVY, pb.FF17_D2n);

  pc.KYVO = and(!pb.JORE, pb.JONA, pb.JEVY);
  pc.JOPA = tock_pos(ga.HORU_512, gb.HORU_512, pb.HAFE, pb.JOPA, pb.KYVO);
  pc.HEPO = tock_pos(pa.JOPA,     pb.JOPA,     pb.HYPA, pb.HEPO, pb.GUFY);

  pc.CYWU = not(gb.APU_RESET);

  pc.DOPE = tock_pos(ga.PHIn, gb.PHIn, pb.CYWU, pb.DOPE, pb.FF19_D7);

  pc.DERA = nor(gb.APU_RESET, pb.DOPE);

  pc.DALA = or(pb.CELO, pb.DOPE);

  pc.ELOX = tock_pos(ga.CEMO_1M, gb.CEMO_1M, pb.DOXA, pb.ELOX, pb.DALA);
  pc.DORY = tock_pos(ga.CEMO_1M, gb.CEMO_1M, pb.CEXE, pb.DORY, pb.ELOX);


  pc.DOXA = or(gb.APU_RESET, pb.DORY);
  pc.CELO = not(pb.DOXA);
  pc.HYLE = or(gb.APU_RESET, pb.ELOX);
  pc.HYPA = nor(pb.ELOX, gb.APU_RESET);
  pc.HYLY = nor(pb.ELOX, pb.JOPA);
  pc.JAKE = not(pb.HYLY);
  pc.JEME = or(pb.HEPO, pb.HYLE);

  pc.GALU = not(gb.CH2_FTICK);
  pc.GYKO = tock_pos(pa.GALU, pb.GALU, pb.GYRE, pb.GYKO, !pb.GYKO);
  pc.ETUK = not(pb.GYKO);
  pc.DAVU = not(pb.ETUK);
  pc.FUJY = and(pb.GYKO, gb.CEMO_1M);
  pc.GYRE = nor(gb.APU_RESET, pb.ELOX, pb.FUJY);
  pc.DUJU = nor(pb.DAVU, pb.ELOX);
  pc.COGU = not(pb.DUJU);
  pc.EROG = not(pb.DUJU);
  pc.GYPA = not(pb.DUJU);
  pc.CEXE = not(gb.APU_RESET);

  pc.BUWE = not(gb.APU_RESET);
  pc.AZEG = not(gb.AMUK_4M);
  pc.ATEP = tock_pos(pa.AZEG, pb.AZEG, pb.BUWE, pb.ATEP, !pb.ATEP);
  pc.CAZA = tock_pos(pa.CEMO, pb.CEMO, pb.CEXE, pb.CAZA, gb.DORY);

  pc.BYHO = not(pb.BUWE);
  pc.BUFO = not(pb.ATEP);
  pc.BODO = not(pb.CAZA);
  pc.CEMO = tock_pos(pa.BUFO, pb.BUFO, pb.BYHO, pb.CEMO, !pb.CEMO);
  pc.BUTA = or(pb.BODO, gb.ARES);
  pc.CAMA = nor(pb.CEMO, pb.BUTA);
  pc.DOCA = not(pb.CAMA);

  pc.CYRE = tock_pos(ga.AKYD_NQ, gb.AKYD_NQ, gb.BENY_OUT, pb.CYRE, !pb.CYRE);
  pc.DEME = or(pb.CYRE, gb.BUFY_256, pb.FF19_D6);
  pc.DORA = and(pb.CYRE, pb.FF19_D6);
  pc.FUTE = nor(pb.FF17_D3,pb.FF17_D4,pb.FF17_D5,pb.FF17_D6,pb.FF17_D7);
  pc.DYRO = not(pb.DEME);
  pc.ESYK = or(gb.APU_RESET, pb.DORA, pb.FUTE);
  pc.ARES = nand(gb.APU_RESET, pb.FUTE);
  pc.DANE = or(pb.ELOX, pb.ESYK);
  pc.DEFU = not(pb.DANE);

  pc.CH2_ACTIVEn = pb.DEFU;
  pc.CH2_AMP_ENn = pb.FUTE;

  pc.BYMO = not(pb.FF16_WR);
  pc.AGET = not(pb.FF16_WR);

  pc.ERYC_00 = count_pos(pa.DYRO,     pb.DYRO,     pb.BYMO, pb.ERYC_00, gb.cpu.D0);
  pc.CERA_01 = count_pos(pa.ERYC_00,  pb.ERYC_00,  pb.BYMO, pb.CERA_01, gb.cpu.D1);
  pc.CONU_02 = count_pos(pa.CERA_01,  pb.CERA_01,  pb.BYMO, pb.CONU_02, gb.cpu.D2);
  pc.CAME_03 = count_pos(pa.CONU_02,  pb.CONU_02,  pb.BYMO, pb.CAME_03, gb.cpu.D3);
  pc.BUVA_04 = count_pos(pa.BUKO_OUT, pb.BUKO_OUT, pb.AGET, pb.BUVA_04, gb.cpu.D4);
  pc.AKYD_05 = count_pos(pa.BUVA_04,  pb.BUVA_04,  pb.AGET, pb.AKYD_05, gb.cpu.D5);

  pc.BUKO = not(!pb.CAME_03);
  pc.BUKO_OUT = pb.BUKO;

  pc.GADE = not(pb.JOPA);
  pc.HOLY = nor(gb.HORU_512, pb.GADE);
  pc.JUPU = nor(pb.FF17_D0, pb.FF17_D1, pb.FF17_D2);
  pc.HOFO = or(pb.JOPA, pb.JUPU, pb.JEME);
  pc.HAFE = or(pb.HOLY, pb.ELOX, gb.APU_RESET);

  pc.CULE = not(pb.DAVU);

  pc.CANO_00 = tock_pos(pa.CULE,     pb.CULE,     gb.APU_RESET2n, pb.CANO_00, !pb.CANO_00);
  pc.CAGY_01 = tock_pos(!pa.CANO_00, !pb.CANO_00, gb.APU_RESET2n, pb.CAGY_01, !pb.CAGY_01);
  pc.DYVE_02 = tock_pos(!pa.CAGY_01, !pb.CAGY_01, gb.APU_RESET2n, pb.DYVE_02, !pb.DYVE_02);

  pc.DYMU = and(pb.DYVE_02, pb.CAGY_01);
  pc.DUGE = not(pb.CANO_00);
  pc.DARE = not(pb.DYMU);
  pc.EGOG = and(pb.DYMU, pb.DUGE);

  pc.DOMO = nor(pb.FF16_D6,  pb.FF16_D7);
  pc.DYTA = nor(pb.FF16_D6n, pb.FF16_D7);
  pc.DOJU = nor(pb.FF16_D6,  pb.FF16_D7n);
  pc.DOVE = nor(pb.FF16_D6n, pb.FF16_D7n);

  pc.EXES = amux4(pb.EGOG, pb.DOMO, pb.DYMU, pb.DYTA, pb.DYVE_02, pb.DOJU, pb.DARE, pb.DOVE);

  pc.DOME = tock_pos(pa.DAVU, pb.DAVU, gb.APU_RESET2n, pb.DOME, pb.EXES);
  pc.CYSE = and(pb.DANE, pb.DOME);
  pc.BONU = or(pb.CYSE, gb.NET03);

  pc.FOPY = amux2(pb.FOMY, pb.FF17_D3, !pb.FOMY, pb.FF17_D3n);
  pc.ETUP = amux2(pb.FETE, pb.FF17_D3, !pb.FETE, pb.FF17_D3n);
  pc.FARU = amux2(pb.FENO, pb.FF17_D3, !pb.FENO, pb.FF17_D3n);
  pc.GAFA = amux2(pb.HOFO, pb.FF17_D3, pb.HOFO,  pb.FF17_D3n);

  pc.FENA = count_pos(pa.FOPY, pb.FOPY, pb.ELOX, pb.FENA, pb.FF17_D7);
  pc.FOMY = count_pos(pa.ETUP, pb.ETUP, pb.ELOX, pb.FOMY, pb.FF17_D6);
  pc.FETE = count_pos(pa.FARU, pb.FARU, pb.ELOX, pb.FETE, pb.FF17_D5);
  pc.FENO = count_pos(pa.GAFA, pb.GAFA, pb.ELOX, pb.FENO, pb.FF17_D4);

  pc.EMYR = nor (pb.FF17_D3, pb.FENO, pb.FETE, pb.FOMY, pb.FENA);
  pc.ERAT = nand(pb.FF17_D3, pb.FENO, pb.FETE, pb.FOMY, pb.FENA);
  pc.FYRE = not(pb.ERAT);
  pc.GUFY = or(pb.EMYR, pb.FYRE);

  pc.ANYV = and(pb.FETE, pb.BONU);
  pc.ANAN = and(pb.FENO, pb.BONU);
  pc.AMOV = and(pb.FENA, pb.BONU);
  pc.ASOG = and(pb.FOMY, pb.BONU);
}