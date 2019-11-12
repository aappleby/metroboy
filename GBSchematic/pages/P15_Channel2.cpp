#include "P15_Channel2.h"
#include "Gameboy.h"

void P15_Channel2::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p15.HOTA = not(b.p01.BYFE_128);
  c.p15.KATY = not(b.p09.APU_RESET1);
  c.p15.JYNA = tock_pos(a.p15.HOTA, b.p15.HOTA, b.p15.KATY, b.p15.JYNA, !b.p15.JYNA);
  c.p15.KYLO = not(b.p15.JYNA);
  c.p15.KENE = not(b.p15.KYLO);

  c.p15.JORE = count_pos(a.p15.KENE, b.p15.KENE, b.p15.JAKE, b.p15.JORE, !b.p14.FF17_D0);
  c.p15.JONA = count_pos(a.p15.JORE, b.p15.JORE, b.p15.JAKE, b.p15.JONA, !b.p14.FF17_D1);
  c.p15.JEVY = count_pos(a.p15.JONA, b.p15.JONA, b.p15.JAKE, b.p15.JEVY, !b.p14.FF17_D2);

  c.p15.KYVO = and(!b.p15.JORE, b.p15.JONA, b.p15.JEVY);
  c.p15.JOPA = tock_pos(a.p01.HORU_512, b.p01.HORU_512, b.p15.HAFE, b.p15.JOPA, b.p15.KYVO);
  c.p15.HEPO = tock_pos(a.p15.JOPA,     b.p15.JOPA,     b.p15.HYPA, b.p15.HEPO, b.p15.GUFY);

  c.p15.CYWU = not(b.p09.APU_RESET1);

  c.p15.DOPE = tock_pos(a.p01.PHIn, b.p01.PHIn, b.p15.CYWU, b.p15.DOPE, b.p14.FF19_D7);

  c.p15.DERA = nor(b.p09.APU_RESET1, b.p15.DOPE);

  c.p15.DALA = or(b.p15.CELO, b.p15.DOPE);

  c.p15.ELOX = tock_pos(a.p15.CEMO, b.p15.CEMO, b.p15.DOXA, b.p15.ELOX, b.p15.DALA);
  c.p15.DORY = tock_pos(a.p15.CEMO, b.p15.CEMO, b.p15.CEXE, b.p15.DORY, b.p15.ELOX);


  c.p15.DOXA = or(b.p09.APU_RESET1, b.p15.DORY);
  c.p15.CELO = not(b.p15.DOXA);
  c.p15.HYLE = or(b.p09.APU_RESET1, b.p15.ELOX);
  c.p15.HYPA = nor(b.p15.ELOX, b.p09.APU_RESET1);
  c.p15.HYLY = nor(b.p15.ELOX, b.p15.JOPA);
  c.p15.JAKE = not(b.p15.HYLY);
  c.p15.JEME = or(b.p15.HEPO, b.p15.HYLE);


  c.p15.GALU = not(b.p14.CH2_FTICK);
  c.p15.GYKO = tock_pos(a.p15.GALU, b.p15.GALU, b.p15.GYRE, b.p15.GYKO, !b.p15.GYKO);
  c.p15.ETUK = not(b.p15.GYKO);
  c.p15.DAVU = not(b.p15.ETUK);
  c.p15.FUJY = and(b.p15.GYKO, b.p15.CEMO);
  c.p15.GYRE = nor(b.p09.APU_RESET1, b.p15.ELOX, b.p15.FUJY);
  c.p15.DUJU = nor(b.p15.DAVU, b.p15.ELOX);
  c.p15.COGU = not(b.p15.DUJU);
  c.p15.EROG = not(b.p15.DUJU);
  c.p15.GYPA = not(b.p15.DUJU);
  c.p15.CEXE = not(b.p09.APU_RESET1);

  c.p15.BUWE = not(b.p09.APU_RESET1);
  c.p15.AZEG = not(b.p01.CLK_xBxDxFxH1);
  c.p15.ATEP = tock_pos(a.p15.AZEG, b.p15.AZEG, b.p15.BUWE, b.p15.ATEP, !b.p15.ATEP);
  c.p15.CAZA = tock_pos(a.p15.CEMO, b.p15.CEMO, b.p15.CEXE, b.p15.CAZA, b.p15.DORY);

  c.p15.BYHO = not(b.p15.BUWE);
  c.p15.BUFO = not(b.p15.ATEP);
  c.p15.BODO = not(b.p15.CAZA);
  c.p15.CEMO = tock_pos(a.p15.BUFO, b.p15.BUFO, b.p15.BYHO, b.p15.CEMO, !b.p15.CEMO);
  c.p15.BUTA = or(b.p15.BODO, b.p15.ARES);
  c.p15.CAMA = nor(b.p15.CEMO, b.p15.BUTA);
  c.p15.DOCA = not(b.p15.CAMA);

  c.p15.CYRE = tock_pos(!a.p15.AKYD_05, !b.p15.AKYD_05, b.p14.BENY, b.p15.CYRE, !b.p15.CYRE);
  c.p15.DEME = or(b.p15.CYRE, b.p01.BUFY_256, b.p14.FF19_D6);
  c.p15.DORA = and(b.p15.CYRE, b.p14.FF19_D6);
  c.p15.FUTE = nor(b.p14.FF17_D3,b.p14.FF17_D4,b.p14.FF17_D5,b.p14.FF17_D6,b.p14.FF17_D7);
  c.p15.DYRO = not(b.p15.DEME);
  c.p15.ESYK = or(b.p09.APU_RESET1, b.p15.DORA, b.p15.FUTE);
  c.p15.ARES = nand(b.p09.APU_RESET1, b.p15.FUTE);
  c.p15.DANE = or(b.p15.ELOX, b.p15.ESYK);
  c.p15.DEFU = not(b.p15.DANE);

  c.p15.CH2_ACTIVEn = b.p15.DEFU;
  c.p15.CH2_AMP_ENn = b.p15.FUTE;

  c.p15.BYMO = not(b.p14.FF16_WR);
  c.p15.AGET = not(b.p14.FF16_WR);

  c.p15.ERYC_00 = count_pos(a.p15.DYRO,     b.p15.DYRO,     b.p15.BYMO, b.p15.ERYC_00, b.D0);
  c.p15.CERA_01 = count_pos(a.p15.ERYC_00,  b.p15.ERYC_00,  b.p15.BYMO, b.p15.CERA_01, b.D1);
  c.p15.CONU_02 = count_pos(a.p15.CERA_01,  b.p15.CERA_01,  b.p15.BYMO, b.p15.CONU_02, b.D2);
  c.p15.CAME_03 = count_pos(a.p15.CONU_02,  b.p15.CONU_02,  b.p15.BYMO, b.p15.CAME_03, b.D3);
  c.p15.BUVA_04 = count_pos(a.p15.BUKO,     b.p15.BUKO,     b.p15.AGET, b.p15.BUVA_04, b.D4);
  c.p15.AKYD_05 = count_pos(a.p15.BUVA_04,  b.p15.BUVA_04,  b.p15.AGET, b.p15.AKYD_05, b.D5);

  c.p15.BUKO = not(!b.p15.CAME_03);

  c.p15.GADE = not(b.p15.JOPA);
  c.p15.HOLY = nor(b.p01.HORU_512, b.p15.GADE);
  c.p15.JUPU = nor(b.p14.FF17_D0, b.p14.FF17_D1, b.p14.FF17_D2);
  c.p15.HOFO = or(b.p15.JOPA, b.p15.JUPU, b.p15.JEME);
  c.p15.HAFE = or(b.p15.HOLY, b.p15.ELOX, b.p09.APU_RESET1);

  c.p15.CULE = not(b.p15.DAVU);

  c.p15.CANO_00 = tock_pos(a.p15.CULE,     b.p15.CULE,     b.p09.APU_RESETn2, b.p15.CANO_00, !b.p15.CANO_00);
  c.p15.CAGY_01 = tock_pos(!a.p15.CANO_00, !b.p15.CANO_00, b.p09.APU_RESETn2, b.p15.CAGY_01, !b.p15.CAGY_01);
  c.p15.DYVE_02 = tock_pos(!a.p15.CAGY_01, !b.p15.CAGY_01, b.p09.APU_RESETn2, b.p15.DYVE_02, !b.p15.DYVE_02);

  c.p15.DYMU = and(b.p15.DYVE_02, b.p15.CAGY_01);
  c.p15.DUGE = not(b.p15.CANO_00);
  c.p15.DARE = not(b.p15.DYMU);
  c.p15.EGOG = and(b.p15.DYMU, b.p15.DUGE);

  c.p15.DOMO = nor( b.p14.FF16_D6,  b.p14.FF16_D7);
  c.p15.DYTA = nor(!b.p14.FF16_D6,  b.p14.FF16_D7);
  c.p15.DOJU = nor( b.p14.FF16_D6, !b.p14.FF16_D7);
  c.p15.DOVE = nor(!b.p14.FF16_D6, !b.p14.FF16_D7);

  c.p15.EXES = amux4(b.p15.EGOG, b.p15.DOMO, b.p15.DYMU, b.p15.DYTA, b.p15.DYVE_02, b.p15.DOJU, b.p15.DARE, b.p15.DOVE);

  c.p15.DOME = tock_pos(a.p15.DAVU, b.p15.DAVU, b.p09.APU_RESETn2, b.p15.DOME, b.p15.EXES);
  c.p15.CYSE = and(b.p15.DANE, b.p15.DOME);
  c.p15.BONU = or(b.p15.CYSE, b.p09.NET03);

  c.p15.FOPY = amux2(b.p15.FOMY, b.p14.FF17_D3, !b.p15.FOMY, !b.p14.FF17_D3);
  c.p15.ETUP = amux2(b.p15.FETE, b.p14.FF17_D3, !b.p15.FETE, !b.p14.FF17_D3);
  c.p15.FARU = amux2(b.p15.FENO, b.p14.FF17_D3, !b.p15.FENO, !b.p14.FF17_D3);
  c.p15.GAFA = amux2(b.p15.HOFO, b.p14.FF17_D3, b.p15.HOFO,  !b.p14.FF17_D3);

  c.p15.FENA = count_pos(a.p15.FOPY, b.p15.FOPY, b.p15.ELOX, b.p15.FENA, b.p14.FF17_D7);
  c.p15.FOMY = count_pos(a.p15.ETUP, b.p15.ETUP, b.p15.ELOX, b.p15.FOMY, b.p14.FF17_D6);
  c.p15.FETE = count_pos(a.p15.FARU, b.p15.FARU, b.p15.ELOX, b.p15.FETE, b.p14.FF17_D5);
  c.p15.FENO = count_pos(a.p15.GAFA, b.p15.GAFA, b.p15.ELOX, b.p15.FENO, b.p14.FF17_D4);

  c.p15.EMYR = nor (b.p14.FF17_D3, b.p15.FENO, b.p15.FETE, b.p15.FOMY, b.p15.FENA);
  c.p15.ERAT = nand(b.p14.FF17_D3, b.p15.FENO, b.p15.FETE, b.p15.FOMY, b.p15.FENA);
  c.p15.FYRE = not(b.p15.ERAT);
  c.p15.GUFY = or(b.p15.EMYR, b.p15.FYRE);

  c.p15.ANYV = and(b.p15.FETE, b.p15.BONU);
  c.p15.ANAN = and(b.p15.FENO, b.p15.BONU);
  c.p15.AMOV = and(b.p15.FENA, b.p15.BONU);
  c.p15.ASOG = and(b.p15.FOMY, b.p15.BONU);
}