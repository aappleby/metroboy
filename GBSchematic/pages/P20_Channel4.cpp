#include "../Schematics.h"
#include "Gameboy.h"

void P20_Channel4::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p20.EMOF = nor(b.p20.FF22_D4n, b.p20.FF22_D5n, b.p20.FF22_D6n);
  c.p20.ELAR = nor(b.p20.FF22_D4n, b.p20.FF22_D5n, b.p20.FF22_D6 );
  c.p20.DUDU = nor(b.p20.FF22_D4n, b.p20.FF22_D5 , b.p20.FF22_D6n);
  c.p20.ETAT = nor(b.p20.FF22_D4n, b.p20.FF22_D5 , b.p20.FF22_D6 );
  c.p20.FURA = nor(b.p20.FF22_D4 , b.p20.FF22_D5n, b.p20.FF22_D6n);
  c.p20.ETAR = nor(b.p20.FF22_D4 , b.p20.FF22_D5n, b.p20.FF22_D6 );
  c.p20.EVER = nor(b.p20.FF22_D4 , b.p20.FF22_D5 , b.p20.FF22_D6n);
  c.p20.ETOV = nor(b.p20.FF22_D4 , b.p20.FF22_D5 , b.p20.FF22_D6 );

  c.p20.CEXO_00 = tock_pos(a.p20.CARY,     b.p20.CARY,     b.p09.APU_RESET4n, b.p20.CEXO_00, !b.p20.CEXO_00);
  c.p20.DEKO_01 = tock_pos(!a.p20.CEXO_00, !b.p20.CEXO_00, b.p09.APU_RESET4n, b.p20.DEKO_01, !b.p20.DEKO_01);
  c.p20.EZEF_02 = tock_pos(!a.p20.DEKO_01, !b.p20.DEKO_01, b.p09.APU_RESET4n, b.p20.EZEF_02, !b.p20.EZEF_02);
  c.p20.EPOR_03 = tock_pos(!a.p20.EZEF_02, !b.p20.EZEF_02, b.p09.APU_RESET4n, b.p20.EPOR_03, !b.p20.EPOR_03);
  c.p20.DURE_04 = tock_pos(!a.p20.EPOR_03, !b.p20.EPOR_03, b.p09.APU_RESET4n, b.p20.DURE_04, !b.p20.DURE_04);
  c.p20.DALE_05 = tock_pos(!a.p20.DURE_04, !b.p20.DURE_04, b.p09.APU_RESET4n, b.p20.DALE_05, !b.p20.DALE_05);
  c.p20.DOKE_06 = tock_pos(!a.p20.DALE_05, !b.p20.DALE_05, b.p09.APU_RESET4n, b.p20.DOKE_06, !b.p20.DOKE_06);
  c.p20.DEMO_07 = tock_pos(!a.p20.DOKE_06, !b.p20.DOKE_06, b.p09.APU_RESET4n, b.p20.DEMO_07, !b.p20.DEMO_07);
  c.p20.DOSE_08 = tock_pos(!a.p20.DEMO_07, !b.p20.DEMO_07, b.p09.APU_RESET4n, b.p20.DOSE_08, !b.p20.DOSE_08);
  c.p20.DETE_09 = tock_pos(!a.p20.DOSE_08, !b.p20.DOSE_08, b.p09.APU_RESET4n, b.p20.DETE_09, !b.p20.DETE_09);
  c.p20.ERUT_10 = tock_pos(!a.p20.DETE_09, !b.p20.DETE_09, b.p09.APU_RESET4n, b.p20.ERUT_10, !b.p20.ERUT_10);
  c.p20.DOTA_11 = tock_pos(!a.p20.ERUT_10, !b.p20.ERUT_10, b.p09.APU_RESET4n, b.p20.DOTA_11, !b.p20.DOTA_11);
  c.p20.DERE_12 = tock_pos(!a.p20.DOTA_11, !b.p20.DOTA_11, b.p09.APU_RESET4n, b.p20.DERE_12, !b.p20.DERE_12);
  c.p20.ESEP_13 = tock_pos(!a.p20.DERE_12, !b.p20.DERE_12, b.p09.APU_RESET4n, b.p20.ESEP_13, !b.p20.ESEP_13);

  c.p20.ETYR = amux6(b.p20.ESEP_13, b.p20.DUDU,
                  b.p20.DERE_12, b.p20.ETAT,
                  b.p20.DOTA_11, b.p20.FURA,
                  b.p20.ERUT_10, b.p20.ETAR,
                  b.p20.DETE_09, b.p20.EVER,
                  b.p20.DOSE_08, b.p20.ETOV);

  c.p20.ELYX = amux4(b.p20.DEMO_07, b.p20.EMOF,
                  b.p20.DOKE_06, b.p20.ELAR,
                  b.p20.DALE_05, b.p20.DUDU,
                  b.p20.DURE_04, b.p20.ETAT);

  c.p20.DARY = amux4(b.p20.EPOR_03, b.p20.FURA,
                  b.p20.EZEF_02, b.p20.ETAR,
                  b.p20.DEKO_01, b.p20.EVER,
                  b.p20.CEXO_00, b.p20.ETOV);

  c.p20.ERYF = or(b.p20.ELYX, b.p20.DARY);
  
  c.p20.COSA = not(b.p09.CPU_RDn);
  c.p20.DYRY = and(b.p20.FF23_D6n, b.p09.NET03);
  c.p20.CEPY = not(b.p20.FF23_D6);
  c.p20.COMO = and(b.p20.COSA, b.p20.DYRY);
  c.p20.COTE = and(b.p20.CEPY, b.p09.NET03);
  c.p20.BAGU = nand(b.p20.FF23, b.p20.COMO);
  c.p20.BEFA = not(b.p20.CARY);
  c.p20.ATEL = not(b.p20.BEFA);
  c.p20.DATO = or(b.p20.COTE, b.p20.EZUL);

  c.p20.FEME = mux2(b.p20.ETYR, b.p20.ERYF, b.p20.FF22_D7);
  c.p20.EZUL = mux2(b.p20.FEME, b.p20.LFSR_OUT, b.p20.DYRY);
  c.p20.GUFA = not(b.p20.FEME);
  c.p20.GYVE = not(b.p20.GUFA);
  c.p20.KARA = not(b.p20.GYVE);
  c.p20.KOPA = not(b.p20.KARA);

  //----------

  c.p20.FELO = or(b.p20.EROX, b.p20.CH4_EG_DISABLE, b.p20.CH4_EG_TICK);

  c.p20.FOLE = amux2(b.p20.FELO, b.p20.FF21_D3,  b.p20.FELO, b.p20.FF21_D3n);
  c.p20.ETEF = amux2(b.p20.FEKO, b.p20.FF21_D3, !b.p20.FEKO, b.p20.FF21_D3n);
  c.p20.EDYF = amux2(b.p20.FATY, b.p20.FF21_D3, !b.p20.FATY, b.p20.FF21_D3n);
  c.p20.ELAF = amux2(b.p20.FERU, b.p20.FF21_D3, !b.p20.FERU, b.p20.FF21_D3n);

  c.p20.FEKO = count_pos(a.p20.FOLE, b.p20.FOLE, b.p20.CH4_RESTART, b.p20.FEKO, b.p20.FF21_D4);
  c.p20.FATY = count_pos(a.p20.ETEF, b.p20.ETEF, b.p20.CH4_RESTART, b.p20.FEKO, b.p20.FF21_D5);
  c.p20.FERU = count_pos(a.p20.EDYF, b.p20.EDYF, b.p20.CH4_RESTART, b.p20.FEKO, b.p20.FF21_D6);
  c.p20.FYRO = count_pos(a.p20.ELAF, b.p20.ELAF, b.p20.CH4_RESTART, b.p20.FEKO, b.p20.FF21_D7);

  c.p20.AKOF = and(b.p20.FEKO, b.p20.CH4_BIT);
  c.p20.BYZY = and(b.p20.FATY, b.p20.CH4_BIT);
  c.p20.APYR = and(b.p20.FERU, b.p20.CH4_BIT);
  c.p20.BOZA = and(b.p20.FYRO, b.p20.CH4_BIT);

  c.p20.DARO = nor(b.p20.FF21_D3, b.p20.FEKO, b.p20.FATY, b.p20.FERU, b.p20.FYRO);
  c.p20.CUTY = nand(b.p20.FF21_D3, b.p20.FEKO, b.p20.FATY, b.p20.FERU, b.p20.FYRO);
  c.p20.DUBO = not(b.p20.CUTY);
  c.p20.EVUR = or(b.p20.DARO, b.p20.DUBO);
  c.p20.EMET = nor(b.p20.CH4_RESTART, b.p09.APU_RESET);
  c.p20.FYNO = tock_pos(a.p20.CH4_EG_TICK, b.p20.CH4_EG_TICK, b.p20.EMET, b.p20.FYNO, b.p20.EVUR);
  c.p20.ENUR = or(b.p09.APU_RESET, b.p20.CH4_RESTART);
  c.p20.EROX = or(!b.p20.FYNO, b.p20.ENUR);

  //----------

  c.p20.ALOP = not(b.p01.BYFE_128);
  c.p20.BOKY = not(b.p09.APU_RESET);
  c.p20.ABEL = tock_pos(a.p20.ALOP, b.p20.ALOP, b.p20.BOKY, b.p20.ABEL, !b.p20.ABEL);
  c.p20.BAWA = not(b.p20.ABEL);
  c.p20.BUXO = not(b.p20.BAWA);
  c.p20.CUNA = count_pos(a.p20.BUXO, b.p20.BUXO, b.p20.DAPY, b.p20.CUNA, b.p20.FF21_D0n);
  c.p20.COFE = count_pos(a.p20.CUNA, b.p20.CUNA, b.p20.DAPY, b.p20.COFE, b.p20.FF21_D1n);
  c.p20.DOGO = count_pos(a.p20.COFE, b.p20.COFE, b.p20.DAPY, b.p20.DOGO, b.p20.FF21_D2n);
  c.p20.EJEX = or(b.p20.DOGO, b.p20.COFE, b.p20.CUNA);
  c.p20.FOSY = tock_pos(a.p01.HORU_512, b.p01.HORU_512, b.p20.GOPA, b.p20.FOSY, b.p20.EJEX);
  c.p20.ENEC = nor(b.p20.CH4_RESTART, b.p20.FOSY);
  c.p20.DAPY = not(b.p20.ENEC);
  c.p20.GEXE = not(b.p20.FOSY);
  c.p20.HURY = nor(b.p01.HORU_512, b.p20.GEXE);
  c.p20.GOPA = nor(b.p20.HURY, b.p20.FOWA, b.p20.CH4_RESTART, b.p09.APU_RESET);
  c.p20.FOWA = nor(b.p20.FF21_D0, b.p20.FF21_D1, b.p20.FF21_D2);

  //----------

  c.p20.GASO = not(b.p09.APU_RESET);
  c.p20.GYSU = tock_pos(a.p01.PHIn, b.p01.PHIn, b.p20.GASO, b.p20.GYSU, b.p20.FF23_D7);
  c.p20.FALE = nor(b.p20.GORA, b.p09.APU_RESET);
  c.p20.HELU = not(b.p20.FALE);
  c.p20.HAZO = or(b.p20.HELU, b.p20.GYSU);
  c.p20.GUZY = nor(b.p20.GYSU, b.p09.APU_RESET);
  c.p20.GONE = tock_pos(a.p01.HAMA_512Kn, b.p01.HAMA_512Kn, b.p20.FALE, b.p20.GONE, b.p20.HAZO);
  c.p20.FEBY = not(b.p09.APU_RESET);
  c.p20.GORA = tock_pos(a.p01.HAMA_512Kn, b.p01.HAMA_512Kn, b.p20.FEBY, b.p20.GORA, b.p20.GONE);
  c.p20.GATY = tock_pos(a.p01.HAMA_512Kn, b.p01.HAMA_512Kn, b.p20.FEBY, b.p20.GATY, b.p20.GORA);
  c.p20.HAPU = not(b.p20.GATY);
  c.p20.GEVY = nor(b.p20.FF21_D3, b.p20.FF21_D4, b.p20.FF21_D5, b.p20.FF21_D6, b.p20.FF21_D7);
  c.p20.EFOT = and(b.p20.FF23_D6, b.p20.FUGO);
  c.p20.HERY = nor(b.p09.APU_RESET, b.p20.GEVY);
  c.p20.FEGY = or(b.p20.GEVY, b.p20.EFOT, b.p09.APU_RESET);
  c.p20.JERY = or(b.p20.HERY, b.p20.HAPU);
  c.p20.GENA = or(b.p20.CH4_RESTART, b.p20.FEGY);
  c.p20.KYKU = or(b.p20.JERY, b.p01.JESO_512K);
  c.p20.JUWA = not(b.p20.GENA);
  c.p20.KONY = not(b.p20.KYKU);
  c.p20.KANU = not(b.p20.KONY);

  c.p20.JYCO = count_pos(a.p20.KANU, b.p20.KANU, b.p20.HUCE, b.p20.JYCO, b.p20.FF22_D0n);
  c.p20.JYRE = count_pos(a.p20.JYCO, b.p20.JYCO, b.p20.HUCE, b.p20.JYRE, b.p20.FF22_D1n);
  c.p20.JYFU = count_pos(a.p20.JYRE, b.p20.JYRE, b.p20.HUCE, b.p20.JYFU, b.p20.FF22_D2n);

  c.p20.HYNO = or(b.p20.JYCO, b.p20.JYRE, b.p20.JYFU);
  c.p20.GYBA = not(b.p01.BAVU_1M);
  c.p20.GARY = tock_pos(a.p20.GYBA, b.p20.GYBA, b.p20.GUNY, b.p20.GARY, b.p20.HYNO);
  c.p20.CARY = and(b.p01.BAVU_1M, b.p20.GARY);
  c.p20.GOFU = nor(b.p20.CH4_RESTART, b.p20.GARY);
  c.p20.GUNY = nor(b.p20.CH4_RESTART, b.p09.APU_RESET);
  c.p20.HUCE = not(b.p20.GOFU);

  //----------

  c.p20.GEPO = or(b.p20.CH4_RESTART, b.p09.APU_RESET);
  c.p20.GOGE = not(b.p20.GEPO);
  c.p20.JYJA = not(b.p20.CH4_LFSR_CLK1);
  c.p20.GAME = nand(b.p20.GENA, b.p20.HEZU);
  c.p20.JYJA = not(b.p20.CH4_LFSR_CLK1);

  c.p20.HURA = xor(b.p20.HEZU, b.p20.HYRO);
  c.p20.JOTO = tock_pos(a.p20.JYJA,          b.p20.JYJA,          b.p20.GOGE, b.p20.JOTO, b.p20.HURA);
  c.p20.KOMU = tock_pos(a.p20.CH4_LFSR_CLK3, b.p20.CH4_LFSR_CLK3, b.p20.GOGE, b.p20.KOMU, b.p20.JOTO);
  c.p20.KETU = tock_pos(a.p20.CH4_LFSR_CLK3, b.p20.CH4_LFSR_CLK3, b.p20.GOGE, b.p20.KETU, b.p20.KOMU);
  c.p20.KUTA = tock_pos(a.p20.CH4_LFSR_CLK3, b.p20.CH4_LFSR_CLK3, b.p20.GOGE, b.p20.KUTA, b.p20.KETU);
  c.p20.KUZY = tock_pos(a.p20.CH4_LFSR_CLK3, b.p20.CH4_LFSR_CLK3, b.p20.GOGE, b.p20.KUZY, b.p20.KUTA);
  c.p20.KYWY = tock_pos(a.p20.CH4_LFSR_CLK3, b.p20.CH4_LFSR_CLK3, b.p20.GOGE, b.p20.KYWY, b.p20.KUZY);
  c.p20.JAJU = tock_pos(a.p20.CH4_LFSR_CLK2, b.p20.CH4_LFSR_CLK2, b.p20.GOGE, b.p20.JAJU, b.p20.KYWY);
  c.p20.HAPE = tock_pos(a.p20.CH4_LFSR_CLK2, b.p20.CH4_LFSR_CLK2, b.p20.GOGE, b.p20.HAPE, b.p20.JAJU);
  c.p20.JUXE = tock_pos(a.p20.CH4_LFSR_CLK2, b.p20.CH4_LFSR_CLK2, b.p20.GOGE, b.p20.JUXE, b.p20.HAPE);
  c.p20.KAVU = amux2(b.p20.JOTO, b.p20.FF22_D3, b.p20.FF22_D3n, b.p20.JUXE);
  c.p20.JEPE = tock_pos(a.p20.CH4_LFSR_CLK2, b.p20.CH4_LFSR_CLK2, b.p20.GOGE, b.p20.JEPE, b.p20.KAVU);
  c.p20.JAVO = tock_pos(a.p20.CH4_LFSR_CLK2, b.p20.CH4_LFSR_CLK2, b.p20.GOGE, b.p20.JAVO, b.p20.JEPE);
  c.p20.HEPA = tock_pos(a.p20.CH4_LFSR_CLK1, b.p20.CH4_LFSR_CLK1, b.p20.GOGE, b.p20.HEPA, b.p20.JAVO);
  c.p20.HORY = tock_pos(a.p20.CH4_LFSR_CLK1, b.p20.CH4_LFSR_CLK1, b.p20.GOGE, b.p20.HORY, b.p20.HEPA);
  c.p20.HENO = tock_pos(a.p20.CH4_LFSR_CLK1, b.p20.CH4_LFSR_CLK1, b.p20.GOGE, b.p20.HENO, b.p20.HORY);
  c.p20.HYRO = tock_pos(a.p20.CH4_LFSR_CLK1, b.p20.CH4_LFSR_CLK1, b.p20.GOGE, b.p20.HYRO, b.p20.HENO);
  c.p20.HEZU = tock_pos(a.p20.CH4_LFSR_CLK1, b.p20.CH4_LFSR_CLK1, b.p20.GOGE, b.p20.HEZU, b.p20.HYRO);
}
