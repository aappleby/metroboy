#include "../Schematics.h"
#include "Gameboy.h"

struct P20_Channel4 {

  bool EMOF,ELAR,DUDU,ETAT,FURA,ETAR,EVER,ETOV;

  bool CEXO_00;
  bool DEKO_01;
  bool EZEF_02;
  bool EPOR_03;
  bool DURE_04;
  bool DALE_05;
  bool DOKE_06;
  bool DEMO_07;
  bool DOSE_08;
  bool DETE_09;
  bool ERUT_10;
  bool DOTA_11;
  bool DERE_12;
  bool ESEP_13;

  bool ETYR,ELYX,DARY,ERYF;

  bool COSA,DYRY,CEPY,FEME,COMO,COTE,EZUL,BAGU,BEFA,DATO,ATEL,GUFA,GYVE,KARA,KOPA;

  //----------

  bool FELO;
  bool FOLE,ETEF,EDYF,ELAF;
  bool FEKO,FATY,FERU,FYRO;
  bool AKOF,BYZY,APYR,BOZA;
  bool DARO,CUTY;
  bool DUBO,EVUR,EMET,FYNO,ENUR,EROX;

  //----------

  bool ALOP,BOKY,ABEL,BAWA,BUXO,DOGO,COFE,CUNA,EJEX,FOSY,GEXE,HURY,FOWA,ENEC,DAPY,GOPA;

  //----------

  bool GASO,GYSU,FALE,HELU,HAZO,GUZY,GONE,FEBY,GORA,GATY,HAPU,GEVY,EFOT;
  bool HERY,FEGY,JERY,GENA,KYKU,JUWA,KONY,KANU,JYCO,JYRE,JYFU;
  bool HYNO,GYBA,GARY,CARY,GOFU,GUNY,HUCE;

  bool GEPO,GOGE,GAME,HURA,KAVU,JYJA;

  bool JEPE,JAVO,HEPA,HORY,HENO,HYRO,HEZU;
  bool JOTO,KOMU,KETU,KUTA,KUZY,KYWY,JAJU,HAPE,JUXE;

  bool FF21_D0;
  bool FF21_D0n;
  bool FF21_D1;
  bool FF21_D1n;
  bool FF21_D2;
  bool FF21_D2n;
  bool FF21_D3;
  bool FF21_D3n;
  bool FF21_D4;
  bool FF21_D5;
  bool FF21_D6;
  bool FF21_D7;

  bool FF22_D0n;
  bool FF22_D1n;
  bool FF22_D2n;
  bool FF22_D3;
  bool FF22_D3n;
  bool FF22_D4;
  bool FF22_D4n;
  bool FF22_D5;
  bool FF22_D5n;
  bool FF22_D6;
  bool FF22_D6n;
  bool FF22_D7;

  bool FF23_D6;
  bool FF23_D6n;
  bool FF23_D7;

  bool FF23;

  bool CH4_BIT;
  bool CH4_LFSR_CLK1;
  bool CH4_LFSR_CLK2;
  bool CH4_LFSR_CLK3;
  bool CH4_EG_DISABLE;
  bool CH4_EG_TICK;
  bool CH4_RESTART;
  bool CH4_ACTIVEn;

  bool LFSR_OUT;
  bool FUGO;

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};

void P20_Channel4::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P20_Channel4 pa = {};
  const P20_Channel4 pb = {};
  P20_Channel4 pc;

  (void)gc;

  pc.EMOF = nor(pb.FF22_D4n, pb.FF22_D5n, pb.FF22_D6n);
  pc.ELAR = nor(pb.FF22_D4n, pb.FF22_D5n, pb.FF22_D6 );
  pc.DUDU = nor(pb.FF22_D4n, pb.FF22_D5 , pb.FF22_D6n);
  pc.ETAT = nor(pb.FF22_D4n, pb.FF22_D5 , pb.FF22_D6 );
  pc.FURA = nor(pb.FF22_D4 , pb.FF22_D5n, pb.FF22_D6n);
  pc.ETAR = nor(pb.FF22_D4 , pb.FF22_D5n, pb.FF22_D6 );
  pc.EVER = nor(pb.FF22_D4 , pb.FF22_D5 , pb.FF22_D6n);
  pc.ETOV = nor(pb.FF22_D4 , pb.FF22_D5 , pb.FF22_D6 );

  pc.CEXO_00 = tock_pos(pa.CARY,     pb.CARY,     gb.APU_RESET4n, pb.CEXO_00, !pb.CEXO_00);
  pc.DEKO_01 = tock_pos(!pa.CEXO_00, !pb.CEXO_00, gb.APU_RESET4n, pb.DEKO_01, !pb.DEKO_01);
  pc.EZEF_02 = tock_pos(!pa.DEKO_01, !pb.DEKO_01, gb.APU_RESET4n, pb.EZEF_02, !pb.EZEF_02);
  pc.EPOR_03 = tock_pos(!pa.EZEF_02, !pb.EZEF_02, gb.APU_RESET4n, pb.EPOR_03, !pb.EPOR_03);
  pc.DURE_04 = tock_pos(!pa.EPOR_03, !pb.EPOR_03, gb.APU_RESET4n, pb.DURE_04, !pb.DURE_04);
  pc.DALE_05 = tock_pos(!pa.DURE_04, !pb.DURE_04, gb.APU_RESET4n, pb.DALE_05, !pb.DALE_05);
  pc.DOKE_06 = tock_pos(!pa.DALE_05, !pb.DALE_05, gb.APU_RESET4n, pb.DOKE_06, !pb.DOKE_06);
  pc.DEMO_07 = tock_pos(!pa.DOKE_06, !pb.DOKE_06, gb.APU_RESET4n, pb.DEMO_07, !pb.DEMO_07);
  pc.DOSE_08 = tock_pos(!pa.DEMO_07, !pb.DEMO_07, gb.APU_RESET4n, pb.DOSE_08, !pb.DOSE_08);
  pc.DETE_09 = tock_pos(!pa.DOSE_08, !pb.DOSE_08, gb.APU_RESET4n, pb.DETE_09, !pb.DETE_09);
  pc.ERUT_10 = tock_pos(!pa.DETE_09, !pb.DETE_09, gb.APU_RESET4n, pb.ERUT_10, !pb.ERUT_10);
  pc.DOTA_11 = tock_pos(!pa.ERUT_10, !pb.ERUT_10, gb.APU_RESET4n, pb.DOTA_11, !pb.DOTA_11);
  pc.DERE_12 = tock_pos(!pa.DOTA_11, !pb.DOTA_11, gb.APU_RESET4n, pb.DERE_12, !pb.DERE_12);
  pc.ESEP_13 = tock_pos(!pa.DERE_12, !pb.DERE_12, gb.APU_RESET4n, pb.ESEP_13, !pb.ESEP_13);

  pc.ETYR = amux6(pb.ESEP_13, pb.DUDU,
                  pb.DERE_12, pb.ETAT,
                  pb.DOTA_11, pb.FURA,
                  pb.ERUT_10, pb.ETAR,
                  pb.DETE_09, pb.EVER,
                  pb.DOSE_08, pb.ETOV);

  pc.ELYX = amux4(pb.DEMO_07, pb.EMOF,
                  pb.DOKE_06, pb.ELAR,
                  pb.DALE_05, pb.DUDU,
                  pb.DURE_04, pb.ETAT);

  pc.DARY = amux4(pb.EPOR_03, pb.FURA,
                  pb.EZEF_02, pb.ETAR,
                  pb.DEKO_01, pb.EVER,
                  pb.CEXO_00, pb.ETOV);

  pc.ERYF = or(pb.ELYX, pb.DARY);
  
  pc.COSA = not(gb.CPU_RDn);
  pc.DYRY = and(pb.FF23_D6n, gb.NET03);
  pc.CEPY = not(pb.FF23_D6);
  pc.COMO = and(pb.COSA, pb.DYRY);
  pc.COTE = and(pb.CEPY, gb.NET03);
  pc.BAGU = nand(pb.FF23, pb.COMO);
  pc.BEFA = not(pb.CARY);
  pc.ATEL = not(pb.BEFA);
  pc.DATO = or(pb.COTE, pb.EZUL);

  pc.FEME = mux2(pb.ETYR, pb.ERYF, pb.FF22_D7);
  pc.EZUL = mux2(pb.FEME, pb.LFSR_OUT, pb.DYRY);
  pc.GUFA = not(pb.FEME);
  pc.GYVE = not(pb.GUFA);
  pc.KARA = not(pb.GYVE);
  pc.KOPA = not(pb.KARA);

  pc.CH4_LFSR_CLK1 = pb.FEME;
  pc.CH4_LFSR_CLK2 = pb.GYVE;
  pc.CH4_LFSR_CLK3 = pb.KOPA;
  pc.CH4_BIT       = pb.DATO;
  pc.FF23_D6n      = pb.CEPY;

  //----------

  pc.FELO = or(pb.EROX, pb.CH4_EG_DISABLE, pb.CH4_EG_TICK);

  pc.FOLE = amux2(pb.FELO, pb.FF21_D3,  pb.FELO, pb.FF21_D3n);
  pc.ETEF = amux2(pb.FEKO, pb.FF21_D3, !pb.FEKO, pb.FF21_D3n);
  pc.EDYF = amux2(pb.FATY, pb.FF21_D3, !pb.FATY, pb.FF21_D3n);
  pc.ELAF = amux2(pb.FERU, pb.FF21_D3, !pb.FERU, pb.FF21_D3n);

  pc.FEKO = count_pos(pa.FOLE, pb.FOLE, pb.CH4_RESTART, pb.FEKO, pb.FF21_D4);
  pc.FATY = count_pos(pa.ETEF, pb.ETEF, pb.CH4_RESTART, pb.FEKO, pb.FF21_D5);
  pc.FERU = count_pos(pa.EDYF, pb.EDYF, pb.CH4_RESTART, pb.FEKO, pb.FF21_D6);
  pc.FYRO = count_pos(pa.ELAF, pb.ELAF, pb.CH4_RESTART, pb.FEKO, pb.FF21_D7);

  pc.AKOF = and(pb.FEKO, pb.CH4_BIT);
  pc.BYZY = and(pb.FATY, pb.CH4_BIT);
  pc.APYR = and(pb.FERU, pb.CH4_BIT);
  pc.BOZA = and(pb.FYRO, pb.CH4_BIT);

  pc.DARO = nor(pb.FF21_D3, pb.FEKO, pb.FATY, pb.FERU, pb.FYRO);
  pc.CUTY = nand(pb.FF21_D3, pb.FEKO, pb.FATY, pb.FERU, pb.FYRO);
  pc.DUBO = not(pb.CUTY);
  pc.EVUR = or(pb.DARO, pb.DUBO);
  pc.EMET = nor(pb.CH4_RESTART, gb.APU_RESET);
  pc.FYNO = tock_pos(pa.CH4_EG_TICK, pb.CH4_EG_TICK, pb.EMET, pb.FYNO, pb.EVUR);
  pc.ENUR = or(gb.APU_RESET, pb.CH4_RESTART);
  pc.EROX = or(!pb.FYNO, pb.ENUR);

  //----------

  pc.ALOP = not(gb.BYFE_128);
  pc.BOKY = not(gb.APU_RESET);
  pc.ABEL = tock_pos(pa.ALOP, pb.ALOP, pb.BOKY, pb.ABEL, !pb.ABEL);
  pc.BAWA = not(pb.ABEL);
  pc.BUXO = not(pb.BAWA);
  pc.CUNA = count_pos(pa.BUXO, pb.BUXO, pb.DAPY, pb.CUNA, pb.FF21_D0n);
  pc.COFE = count_pos(pa.CUNA, pb.CUNA, pb.DAPY, pb.COFE, pb.FF21_D1n);
  pc.DOGO = count_pos(pa.COFE, pb.COFE, pb.DAPY, pb.DOGO, pb.FF21_D2n);
  pc.EJEX = or(pb.DOGO, pb.COFE, pb.CUNA);
  pc.FOSY = tock_pos(ga.HORU_512, gb.HORU_512, pb.GOPA, pb.FOSY, pb.EJEX);
  pc.ENEC = nor(pb.CH4_RESTART, pb.FOSY);
  pc.DAPY = not(pb.ENEC);
  pc.GEXE = not(pb.FOSY);
  pc.HURY = nor(gb.HORU_512, pb.GEXE);
  pc.GOPA = nor(pb.HURY, pb.FOWA, pb.CH4_RESTART, gb.APU_RESET);
  pc.FOWA = nor(pb.FF21_D0, pb.FF21_D1, pb.FF21_D2);

  pc.CH4_EG_DISABLE = pb.FOWA;

  //----------

  pc.GASO = not(gb.APU_RESET);
  pc.GYSU = tock_pos(ga.PHIn, gb.PHIn, pb.GASO, pb.GYSU, pb.FF23_D7);
  pc.FALE = nor(pb.GORA, gb.APU_RESET);
  pc.HELU = not(pb.FALE);
  pc.HAZO = or(pb.HELU, pb.GYSU);
  pc.GUZY = nor(pb.GYSU, gb.APU_RESET);
  pc.GONE = tock_pos(ga.HAMA_512Kn, gb.HAMA_512Kn, pb.FALE, pb.GONE, pb.HAZO);
  pc.FEBY = not(gb.APU_RESET);
  pc.GORA = tock_pos(ga.HAMA_512Kn, gb.HAMA_512Kn, pb.FEBY, pb.GORA, pb.GONE);
  pc.GATY = tock_pos(ga.HAMA_512Kn, gb.HAMA_512Kn, pb.FEBY, pb.GATY, pb.GORA);
  pc.HAPU = not(pb.GATY);
  pc.GEVY = nor(pb.FF21_D3, pb.FF21_D4, pb.FF21_D5, pb.FF21_D6, pb.FF21_D7);
  pc.EFOT = and(pb.FF23_D6, pb.FUGO);
  pc.HERY = nor(gb.APU_RESET, pb.GEVY);
  pc.FEGY = or(pb.GEVY, pb.EFOT, gb.APU_RESET);
  pc.JERY = or(pb.HERY, pb.HAPU);
  pc.GENA = or(pb.CH4_RESTART, pb.FEGY);
  pc.KYKU = or(pb.JERY, gb.JESO_512K);
  pc.JUWA = not(pb.GENA);
  pc.KONY = not(pb.KYKU);
  pc.KANU = not(pb.KONY);

  pc.JYCO = count_pos(pa.KANU, pb.KANU, pb.HUCE, pb.JYCO, pb.FF22_D0n);
  pc.JYRE = count_pos(pa.JYCO, pb.JYCO, pb.HUCE, pb.JYRE, pb.FF22_D1n);
  pc.JYFU = count_pos(pa.JYRE, pb.JYRE, pb.HUCE, pb.JYFU, pb.FF22_D2n);

  pc.HYNO = or(pb.JYCO, pb.JYRE, pb.JYFU);
  pc.GYBA = not(gb.BAVU_1M);
  pc.GARY = tock_pos(pa.GYBA, pb.GYBA, pb.GUNY, pb.GARY, pb.HYNO);
  pc.CARY = and(gb.BAVU_1M, pb.GARY);
  pc.GOFU = nor(pb.CH4_RESTART, pb.GARY);
  pc.GUNY = nor(pb.CH4_RESTART, gb.APU_RESET);
  pc.HUCE = not(pb.GOFU);

  pc.CH4_ACTIVEn = pb.JUWA;

  //----------

  pc.GEPO = or(pb.CH4_RESTART, gb.APU_RESET);
  pc.GOGE = not(pb.GEPO);
  pc.JYJA = not(pb.CH4_LFSR_CLK1);
  pc.GAME = nand(pb.GENA, pb.HEZU);
  pc.JYJA = not(pb.CH4_LFSR_CLK1);

  pc.HURA = xor(pb.HEZU, pb.HYRO);
  pc.JOTO = tock_pos(pa.JYJA,          pb.JYJA,          pb.GOGE, pb.JOTO, pb.HURA);
  pc.KOMU = tock_pos(pa.CH4_LFSR_CLK3, pb.CH4_LFSR_CLK3, pb.GOGE, pb.KOMU, pb.JOTO);
  pc.KETU = tock_pos(pa.CH4_LFSR_CLK3, pb.CH4_LFSR_CLK3, pb.GOGE, pb.KETU, pb.KOMU);
  pc.KUTA = tock_pos(pa.CH4_LFSR_CLK3, pb.CH4_LFSR_CLK3, pb.GOGE, pb.KUTA, pb.KETU);
  pc.KUZY = tock_pos(pa.CH4_LFSR_CLK3, pb.CH4_LFSR_CLK3, pb.GOGE, pb.KUZY, pb.KUTA);
  pc.KYWY = tock_pos(pa.CH4_LFSR_CLK3, pb.CH4_LFSR_CLK3, pb.GOGE, pb.KYWY, pb.KUZY);
  pc.JAJU = tock_pos(pa.CH4_LFSR_CLK2, pb.CH4_LFSR_CLK2, pb.GOGE, pb.JAJU, pb.KYWY);
  pc.HAPE = tock_pos(pa.CH4_LFSR_CLK2, pb.CH4_LFSR_CLK2, pb.GOGE, pb.HAPE, pb.JAJU);
  pc.JUXE = tock_pos(pa.CH4_LFSR_CLK2, pb.CH4_LFSR_CLK2, pb.GOGE, pb.JUXE, pb.HAPE);
  pc.KAVU = amux2(pb.JOTO, pb.FF22_D3, pb.FF22_D3n, pb.JUXE);
  pc.JEPE = tock_pos(pa.CH4_LFSR_CLK2, pb.CH4_LFSR_CLK2, pb.GOGE, pb.JEPE, pb.KAVU);
  pc.JAVO = tock_pos(pa.CH4_LFSR_CLK2, pb.CH4_LFSR_CLK2, pb.GOGE, pb.JAVO, pb.JEPE);
  pc.HEPA = tock_pos(pa.CH4_LFSR_CLK1, pb.CH4_LFSR_CLK1, pb.GOGE, pb.HEPA, pb.JAVO);
  pc.HORY = tock_pos(pa.CH4_LFSR_CLK1, pb.CH4_LFSR_CLK1, pb.GOGE, pb.HORY, pb.HEPA);
  pc.HENO = tock_pos(pa.CH4_LFSR_CLK1, pb.CH4_LFSR_CLK1, pb.GOGE, pb.HENO, pb.HORY);
  pc.HYRO = tock_pos(pa.CH4_LFSR_CLK1, pb.CH4_LFSR_CLK1, pb.GOGE, pb.HYRO, pb.HENO);
  pc.HEZU = tock_pos(pa.CH4_LFSR_CLK1, pb.CH4_LFSR_CLK1, pb.GOGE, pb.HEZU, pb.HYRO);

}
