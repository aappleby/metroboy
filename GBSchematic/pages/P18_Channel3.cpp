#include "../Schematics.h"
#include "Gameboy.h"

struct P18_Channel3 {

  bool JYFO,HUNO,HEMA,GASE,HUPA,GAFU,HEFO,JUTY;
  bool HERA,KYKO,KASO,KYRU,JERA,KESE;
  
  bool KUTU_00,KUPE_01,KUNU_02,KEMU_03,KYGU_04,KEPA_05,KAFO_06,KENO_07,KEJU_08,KEZA_09,JAPU_10;

  bool FETY,FOTO,ETAN,GYRY,DERO;
  bool EFAR,ERUS,EFUZ,EXEL,EFAL;
  bool BOLE,AGYL,AFUM,AXOL;
  bool JECO,HONY,GENO;
  bool FAPY,FARO,FOTE,FANA,FERA;
  bool GEDO,FYGO,FOZU,EZAS,CALU,DORU,DAVO,COKA,ERED;

  bool GOKA,GEMY,GEGE,EZAG,EVUG,DOKY,DORE,BARY,BYKA,BOPA,BELY;
  bool GUDA,FEXU,GEPY,GENU,FALU;

  bool BENO,ATOK;

  bool GEVO_00;
  bool FORY_01;
  bool GATU_02;
  bool GAPO_03;
  bool GEMO_04;
  bool FORO_05;
  bool FAVE_06;
  bool FYRU_07;

  bool GARA,GUGU;

  bool FF1A_D7n;

  bool FF1B_WR1;
  bool FF1B_WR2;
  bool FF1B_WR3;

  bool FF1C_D5;
  bool FF1C_D6;
  bool FF1C_D5n;
  bool FF1C_D6n;

  bool FF1D_D0;
  bool FF1D_D1;
  bool FF1D_D2;
  bool FF1D_D3;
  bool FF1D_D4;
  bool FF1D_D5;
  bool FF1D_D6;
  bool FF1D_D7;

  bool FF1E_D0;
  bool FF1E_D1;
  bool FF1E_D2;

  bool FF1E_D6;
  bool FF1E_D6n;

  bool CH3_ACTIVE;
  bool CH3_ACTIVEn;

  bool WAVE_A0;
  bool WAVE_A1;
  bool WAVE_A2;
  bool WAVE_A3;

  bool WAVE_PLAY_D0;
  bool WAVE_PLAY_D1;
  bool WAVE_PLAY_D2;
  bool WAVE_PLAY_D3;

  bool WAVE_DAC_D0;
  bool WAVE_DAC_D1;
  bool WAVE_DAC_D2;
  bool WAVE_DAC_D3;

  bool WAVE_RAM_RDn;

  bool FF1C;

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};

void P18_Channel3::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P18_Channel3 pa = {};
  const P18_Channel3 pb = {};
  P18_Channel3 pc;

  pc.JYFO = not(pb.JAPU_10);
  pc.HUNO = tock_pos(pa.JYFO, pb.JYFO, pb.GAFU, pb.HUNO, !pb.HUNO);
  pc.HEMA = not(pb.HUNO);
  pc.GASE = not(pb.HEMA);
  pc.HUPA = and(pb.HUNO, gb.CERY_2M);
  pc.GAFU = nor(gb.APU_RESET, pb.GARA, pb.HUPA);
  pc.HEFO = nor(gb.CERY_2M, pb.GUGU);
  pc.HERA = nor(pb.GASE, pb.GARA);

  pc.JUTY = not(pb.HEFO);
  pc.KYKO = not(pb.HERA);

  pc.KUTU_00 = count_pos(pa.JUTY,    pb.JUTY,    pb.KYKO, pb.KUTU_00, pb.FF1D_D0);
  pc.KUPE_01 = count_pos(pa.KUTU_00, pb.KUTU_00, pb.KYKO, pb.KUPE_01, pb.FF1D_D1);
  pc.KUNU_02 = count_pos(pa.KUPE_01, pb.KUPE_01, pb.KYKO, pb.KUNU_02, pb.FF1D_D2);
  pc.KEMU_03 = count_pos(pa.KUNU_02, pb.KUNU_02, pb.KYKO, pb.KEMU_03, pb.FF1D_D3);

  pc.KYRU = not(!pb.KEMU_03);
  pc.KASO = not(pb.HERA);

  pc.KYGU_04 = count_pos(pa.KYRU,    pb.KYRU,    pb.KASO, pb.KYGU_04, pb.FF1D_D4);
  pc.KEPA_05 = count_pos(pa.KYGU_04, pb.KYGU_04, pb.KASO, pb.KEPA_05, pb.FF1D_D5);
  pc.KAFO_06 = count_pos(pa.KEPA_05, pb.KEPA_05, pb.KASO, pb.KAFO_06, pb.FF1D_D6);
  pc.KENO_07 = count_pos(pa.KAFO_06, pb.KAFO_06, pb.KASO, pb.KENO_07, pb.FF1D_D7);

  pc.KESE = not(!pb.KENO_07);
  pc.JERA = not(pb.HERA);

  pc.KEJU_08 = count_pos(pa.KESE,    pb.KESE,    pb.JERA, pb.KEJU_08, pb.FF1E_D0);
  pc.KEZA_09 = count_pos(pa.KEJU_08, pb.KEJU_08, pb.JERA, pb.KEZA_09, pb.FF1E_D1);
  pc.JAPU_10 = count_pos(pa.KEZA_09, pb.KEZA_09, pb.JERA, pb.JAPU_10, pb.FF1E_D2);

  pc.FETY = tock_pos(!pa.EFAL, !pb.EFAL, pb.GYRY, pb.FETY, !pb.FETY);
  pc.FOTO = and(pb.FETY, pb.GASE);
  pc.ETAN = or(pb.GARA, pb.FETY);
  pc.GYRY = nor(gb.APU_RESET, pb.GARA, pb.FOTO);
  pc.DERO = not(pb.GASE);

  pc.EFAR = tock_pos(pa.DERO,  pb.DERO,  pb.ETAN, pb.EFAR, !pb.EFAR);
  pc.ERUS = tock_pos(!pa.EFAR, !pb.EFAR, pb.ETAN, pb.ERUS, !pb.ERUS);
  pc.EFUZ = tock_pos(!pa.ERUS, !pb.ERUS, pb.ETAN, pb.EFUZ, !pb.EFUZ);
  pc.EXEL = tock_pos(!pa.EFUZ, !pb.EFUZ, pb.ETAN, pb.EXEL, !pb.EXEL);
  pc.EFAL = tock_pos(!pa.EXEL, !pb.EXEL, pb.ETAN, pb.EFAL, !pb.EFAL);

  pc.BOLE = mux2(pb.ERUS, gb.cpu.A00, pb.CH3_ACTIVE);
  pc.AGYL = mux2(pb.EFUZ, gb.cpu.A01, pb.CH3_ACTIVE);
  pc.AFUM = mux2(pb.EXEL, gb.cpu.A02, pb.CH3_ACTIVE);
  pc.AXOL = mux2(pb.EFAL, gb.cpu.A03, pb.CH3_ACTIVE);

  pc.WAVE_A0 = pb.BOLE;
  pc.WAVE_A1 = pb.AGYL;
  pc.WAVE_A2 = pb.AFUM;
  pc.WAVE_A3 = pb.AXOL;

  pc.JECO = not(gb.CPU_RDn);
  pc.HONY = and(gb.NET03, pb.JECO, pb.FF1C);
  pc.GENO = not(pb.HONY);

  pc.FAPY = not(!pb.EFAR);
  pc.FARO = not(!pb.ERUS);
  pc.FOTE = not(!pb.EFUZ);
  pc.FANA = not(!pb.EXEL);
  pc.FERA = not(!pb.EFAL);

  if (pb.GENO) {
    gc.cpu.D0 = pb.FAPY;
    gc.cpu.D1 = pb.FARO;
    gc.cpu.D2 = pb.FOTE;
    gc.cpu.D3 = pb.FANA;
    gc.cpu.D4 = pb.FERA;
  }

  pc.GEDO = and(gb.BUFY_256, pb.FF1E_D6);
  pc.FYGO = or(gb.APU_RESET, pb.GEDO, pb.FF1A_D7n);
  pc.FOZU = or(pb.GARA, pb.FYGO);
  pc.EZAS = not(pb.FOZU);
  pc.DORU = not(pb.EZAS);
  pc.CALU = not(gb.APU_RESET);
  pc.DAVO = tock_pos(ga.AJER_2M, gb.AJER_2M, pb.CALU, pb.DAVO, pb.DORU);
  pc.COKA = not(!pb.DAVO);
  pc.ERED = not(pb.COKA);

  pc.CH3_ACTIVE = pb.COKA;
  pc.CH3_ACTIVEn = pb.ERED;

  pc.GUDA = nor(pb.FF1B_WR3, gb.APU_RESET, pb.GARA);
  pc.FEXU = tock_pos(!pa.FYRU_07, !pb.FYRU_07, pb.GUDA, pb.FEXU, !pb.FEXU);
  pc.GEPY = nor(gb.BUFY_256, pb.FF1E_D6n);
  pc.GENU = not(pb.GEPY);

  pc.GEVO_00 = tock_pos(pa.GENU,    pb.GENU,    pb.FF1B_WR2, pb.GEVO_00, gb.cpu.D0);
  pc.FORY_01 = tock_pos(pa.GEVO_00, pb.GEVO_00, pb.FF1B_WR2, pb.FORY_01, gb.cpu.D1);
  pc.GATU_02 = tock_pos(pa.FORY_01, pb.FORY_01, pb.FF1B_WR2, pb.GATU_02, gb.cpu.D2);
  pc.GAPO_03 = tock_pos(pa.GATU_02, pb.GATU_02, pb.FF1B_WR2, pb.GAPO_03, gb.cpu.D3);

  pc.FALU = not(pb.GAPO_03);

  pc.GEMO_04 = tock_pos(pa.FALU,    pb.FALU,    pb.FF1B_WR1, pb.GEMO_04, gb.cpu.D4);
  pc.FORO_05 = tock_pos(pa.GEMO_04, pb.GEMO_04, pb.FF1B_WR1, pb.FORO_05, gb.cpu.D5);
  pc.FAVE_06 = tock_pos(pa.FORO_05, pb.FORO_05, pb.FF1B_WR1, pb.FAVE_06, gb.cpu.D6);
  pc.FYRU_07 = tock_pos(pa.FAVE_06, pb.FAVE_06, pb.FF1B_WR1, pb.FYRU_07, gb.cpu.D7);

  pc.GOKA = nor(pb.FF1C_D6n, pb.FF1C_D5);
  pc.GEMY = nor(pb.FF1C_D6n, pb.FF1C_D5n);
  pc.GEGE = nor(pb.FF1C_D6,  pb.FF1C_D5n);

  pc.EZAG = amux3(pb.WAVE_PLAY_D0, pb.GEGE, pb.WAVE_PLAY_D1, pb.GOKA, pb.WAVE_PLAY_D2, pb.GEMY);
  pc.EVUG = amux3(pb.WAVE_PLAY_D1, pb.GEGE, pb.WAVE_PLAY_D2, pb.GOKA, pb.WAVE_PLAY_D3, pb.GEMY);
  pc.DOKY = amux2(pb.GEGE, pb.WAVE_PLAY_D2, pb.WAVE_PLAY_D3, pb.GEMY);
  pc.DORE = and(pb.WAVE_PLAY_D3, pb.GEGE);

  pc.BARY = and(pb.CH3_ACTIVE, pb.EZAG);
  pc.BYKA = and(pb.CH3_ACTIVE, pb.EVUG);
  pc.BOPA = and(pb.CH3_ACTIVE, pb.DOKY);
  pc.BELY = and(pb.CH3_ACTIVE, pb.DORE);

  pc.WAVE_DAC_D0 = pb.BARY;
  pc.WAVE_DAC_D1 = pb.BYKA;
  pc.WAVE_DAC_D2 = pb.BOPA;
  pc.WAVE_DAC_D3 = pb.BELY;

  pc.BENO = mux2(gb.BUTU_512K, pb.WAVE_RAM_RDn, pb.CH3_ACTIVE);
  pc.ATOK = not(pb.BENO);
}