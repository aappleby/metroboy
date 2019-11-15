#include "../Schematics.h"
#include "Gameboy.h"

void P18_Channel3::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  // some weird clocky thing...
  c.p18.JYFO = not(b.p18.JAPU_10);
  c.p18.HUNO = tock_pos(a.p18.JYFO, b.p18.JYFO, b.p18.GAFU, b.p18.HUNO, !b.p18.HUNO);
  c.p18.HEMA = not(b.p18.HUNO);
  c.p18.GASE = not(b.p18.HEMA);

  c.p18.HUPA = and(b.p18.HUNO, b.p01.CLK_ABxxEFxx1);
  c.p18.GAFU = nor(b.p09.APU_RESET1, b.p16.GARA, b.p18.HUPA);
  c.p18.HEFO = nor(b.p01.CLK_ABxxEFxx1, b.p16.GUGU);
  c.p18.HERA = nor(b.p18.GASE, b.p16.GARA);

  c.p18.JUTY = not(b.p18.HEFO);
  c.p18.KYKO = not(b.p18.HERA);

  c.p18.KUTU_00 = count_pos(a.p18.JUTY,    b.p18.JUTY,    b.p18.KYKO, b.p18.KUTU_00, b.p18.FF1D_D0);
  c.p18.KUPE_01 = count_pos(a.p18.KUTU_00, b.p18.KUTU_00, b.p18.KYKO, b.p18.KUPE_01, b.p18.FF1D_D1);
  c.p18.KUNU_02 = count_pos(a.p18.KUPE_01, b.p18.KUPE_01, b.p18.KYKO, b.p18.KUNU_02, b.p18.FF1D_D2);
  c.p18.KEMU_03 = count_pos(a.p18.KUNU_02, b.p18.KUNU_02, b.p18.KYKO, b.p18.KEMU_03, b.p18.FF1D_D3);

  c.p18.KYRU = not(!b.p18.KEMU_03);
  c.p18.KASO = not(b.p18.HERA);

  c.p18.KYGU_04 = count_pos(a.p18.KYRU,    b.p18.KYRU,    b.p18.KASO, b.p18.KYGU_04, b.p18.FF1D_D4);
  c.p18.KEPA_05 = count_pos(a.p18.KYGU_04, b.p18.KYGU_04, b.p18.KASO, b.p18.KEPA_05, b.p18.FF1D_D5);
  c.p18.KAFO_06 = count_pos(a.p18.KEPA_05, b.p18.KEPA_05, b.p18.KASO, b.p18.KAFO_06, b.p18.FF1D_D6);
  c.p18.KENO_07 = count_pos(a.p18.KAFO_06, b.p18.KAFO_06, b.p18.KASO, b.p18.KENO_07, b.p18.FF1D_D7);

  c.p18.KESE = not(!b.p18.KENO_07);
  c.p18.JERA = not(b.p18.HERA);

  c.p18.KEJU_08 = count_pos(a.p18.KESE,    b.p18.KESE,    b.p18.JERA, b.p18.KEJU_08, b.p18.FF1E_D0);
  c.p18.KEZA_09 = count_pos(a.p18.KEJU_08, b.p18.KEJU_08, b.p18.JERA, b.p18.KEZA_09, b.p18.FF1E_D1);
  c.p18.JAPU_10 = count_pos(a.p18.KEZA_09, b.p18.KEZA_09, b.p18.JERA, b.p18.JAPU_10, b.p18.FF1E_D2);

  /*p18.FOTO*/ c.p18.FOTO = and(b.p18.WAVE_LOOP, b.p18.GASE);
  /*p18.ETAN*/ c.p18.WAVE_RST = or(b.p16.GARA, b.p18.WAVE_LOOP);
  /*p18.GYRY*/ c.p18.GYRY = nor(b.p09.APU_RESET1, b.p16.GARA, b.p18.FOTO);

  /*p18.DERO*/ c.p18.WAVE_CLK = not(b.p18.GASE);
  /*p18.EFAR*/ c.p18.WAVE_IDX0 = tock_pos( a.p18.WAVE_CLK,   b.p18.WAVE_CLK,  b.p18.WAVE_RST, b.p18.WAVE_IDX0, !b.p18.WAVE_IDX0);
  /*p18.ERUS*/ c.p18.WAVE_IDX1 = tock_pos(!a.p18.WAVE_IDX0, !b.p18.WAVE_IDX0, b.p18.WAVE_RST, b.p18.WAVE_IDX1, !b.p18.WAVE_IDX1);
  /*p18.EFUZ*/ c.p18.WAVE_IDX2 = tock_pos(!a.p18.WAVE_IDX1, !b.p18.WAVE_IDX1, b.p18.WAVE_RST, b.p18.WAVE_IDX2, !b.p18.WAVE_IDX2);
  /*p18.EXEL*/ c.p18.WAVE_IDX3 = tock_pos(!a.p18.WAVE_IDX2, !b.p18.WAVE_IDX2, b.p18.WAVE_RST, b.p18.WAVE_IDX3, !b.p18.WAVE_IDX3);
  /*p18.EFAL*/ c.p18.WAVE_IDX4 = tock_pos(!a.p18.WAVE_IDX3, !b.p18.WAVE_IDX3, b.p18.WAVE_RST, b.p18.WAVE_IDX4, !b.p18.WAVE_IDX4);
  /*p18.FETY*/ c.p18.WAVE_LOOP = tock_pos(!a.p18.WAVE_IDX4, !b.p18.WAVE_IDX4, b.p18.GYRY,     b.p18.WAVE_LOOP, !b.p18.WAVE_LOOP);

  c.p18.WAVE_A0 = mux2(b.p18.WAVE_IDX1, b.A00, b.p18.CH3_ACTIVE);
  c.p18.WAVE_A1 = mux2(b.p18.WAVE_IDX2, b.A01, b.p18.CH3_ACTIVE);
  c.p18.WAVE_A2 = mux2(b.p18.WAVE_IDX3, b.A02, b.p18.CH3_ACTIVE);
  c.p18.WAVE_A3 = mux2(b.p18.WAVE_IDX4, b.A03, b.p18.CH3_ACTIVE);

  c.p18.JECO = not(b.p09.CPU_RDn);
  c.p18.HONY = and(b.p09.NET03, b.p18.JECO, b.p18.FF1C);
  c.p18.GENO = not(b.p18.HONY);

  /*p18.FAPY*/ c.p18.WAVE_IDX_D0 = not(!b.p18.WAVE_IDX0);
  /*p18.FARO*/ c.p18.WAVE_IDX_D1 = not(!b.p18.WAVE_IDX1);
  /*p18.FOTE*/ c.p18.WAVE_IDX_D2 = not(!b.p18.WAVE_IDX2);
  /*p18.FANA*/ c.p18.WAVE_IDX_D3 = not(!b.p18.WAVE_IDX3);
  /*p18.FERA*/ c.p18.WAVE_IDX_D4 = not(!b.p18.WAVE_IDX4);

  if (b.p18.GENO) {
    c.D0 = b.p18.WAVE_IDX_D0;
    c.D1 = b.p18.WAVE_IDX_D1;
    c.D2 = b.p18.WAVE_IDX_D2;
    c.D3 = b.p18.WAVE_IDX_D3;
    c.D4 = b.p18.WAVE_IDX_D4;
  }

  c.p18.GEDO = and(b.p01.BUFY_256, b.p18.FF1E_D6);
  c.p18.FYGO = or(b.p09.APU_RESET1, b.p18.GEDO, b.p18.FF1A_D7n);
  c.p18.FOZU = or(b.p16.GARA, b.p18.FYGO);
  c.p18.EZAS = not(b.p18.FOZU);
  c.p18.DORU = not(b.p18.EZAS);
  c.p18.DAVO = tock_pos(a.p01.AJER_2M, b.p01.AJER_2M, b.p18.CALU, b.p18.DAVO, b.p18.DORU);
  c.p18.COKA = not(!b.p18.DAVO);
  c.p18.ERED = not(b.p18.COKA);

  c.p18.CH3_ACTIVE = b.p18.COKA;
  c.p18.CH3_ACTIVEn = b.p18.ERED;

  c.p18.GUDA = nor(b.p18.FF1B_WR3, b.p09.APU_RESET1, b.p16.GARA);
  c.p18.FEXU = tock_pos(!a.p18.FYRU_07, !b.p18.FYRU_07, b.p18.GUDA, b.p18.FEXU, !b.p18.FEXU);
  c.p18.GEPY = nor(b.p01.BUFY_256, b.p18.FF1E_D6n);
  c.p18.GENU = not(b.p18.GEPY);

  c.p18.GEVO_00 = tock_pos(a.p18.GENU,    b.p18.GENU,    b.p18.FF1B_WR2, b.p18.GEVO_00, b.D0);
  c.p18.FORY_01 = tock_pos(a.p18.GEVO_00, b.p18.GEVO_00, b.p18.FF1B_WR2, b.p18.FORY_01, b.D1);
  c.p18.GATU_02 = tock_pos(a.p18.FORY_01, b.p18.FORY_01, b.p18.FF1B_WR2, b.p18.GATU_02, b.D2);
  c.p18.GAPO_03 = tock_pos(a.p18.GATU_02, b.p18.GATU_02, b.p18.FF1B_WR2, b.p18.GAPO_03, b.D3);

  c.p18.FALU = not(b.p18.GAPO_03);

  c.p18.GEMO_04 = tock_pos(a.p18.FALU,    b.p18.FALU,    b.p18.FF1B_WR1, b.p18.GEMO_04, b.D4);
  c.p18.FORO_05 = tock_pos(a.p18.GEMO_04, b.p18.GEMO_04, b.p18.FF1B_WR1, b.p18.FORO_05, b.D5);
  c.p18.FAVE_06 = tock_pos(a.p18.FORO_05, b.p18.FORO_05, b.p18.FF1B_WR1, b.p18.FAVE_06, b.D6);
  c.p18.FYRU_07 = tock_pos(a.p18.FAVE_06, b.p18.FAVE_06, b.p18.FF1B_WR1, b.p18.FYRU_07, b.D7);

  c.p18.GOKA = nor(b.p18.FF1C_D6n, b.p18.FF1C_D5);
  c.p18.GEMY = nor(b.p18.FF1C_D6n, b.p18.FF1C_D5n);
  c.p18.GEGE = nor(b.p18.FF1C_D6,  b.p18.FF1C_D5n);

  c.p18.EZAG = amux3(b.p17.WAVE_PLAY_D0, b.p18.GEGE, b.p17.WAVE_PLAY_D1, b.p18.GOKA, b.p17.WAVE_PLAY_D2, b.p18.GEMY);
  c.p18.EVUG = amux3(b.p17.WAVE_PLAY_D1, b.p18.GEGE, b.p17.WAVE_PLAY_D2, b.p18.GOKA, b.p17.WAVE_PLAY_D3, b.p18.GEMY);
  c.p18.DOKY = amux2(b.p18.GEGE, b.p17.WAVE_PLAY_D2, b.p17.WAVE_PLAY_D3, b.p18.GEMY);
  c.p18.DORE = and(b.p17.WAVE_PLAY_D3, b.p18.GEGE);

  c.p18.BARY = and(b.p18.CH3_ACTIVE, b.p18.EZAG);
  c.p18.BYKA = and(b.p18.CH3_ACTIVE, b.p18.EVUG);
  c.p18.BOPA = and(b.p18.CH3_ACTIVE, b.p18.DOKY);
  c.p18.BELY = and(b.p18.CH3_ACTIVE, b.p18.DORE);

  c.p18.WAVE_DAC_D0 = b.p18.BARY;
  c.p18.WAVE_DAC_D1 = b.p18.BYKA;
  c.p18.WAVE_DAC_D2 = b.p18.BOPA;
  c.p18.WAVE_DAC_D3 = b.p18.BELY;

  c.p18.BENO = mux2(b.p17.BUTU, b.p17.WAVE_RAM_RDn, b.p18.CH3_ACTIVE);
  c.p18.WAVE_RAM_CTRL3 = not(b.p18.BENO);
}