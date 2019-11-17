#include "../Schematics.h"
#include "Gameboy.h"

void P16_Ch3Regs_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {
  /*p16.GEJO*/ c.ch3.FF1A_WR = and(b.apu.APU_WR, b.apu.ADDR_FF1A);
  /*p16.GUCY*/ c.ch3.FF1A_WRn = not(b.ch3.FF1A_WR);
  /*p16.GUXE*/ c.ch3.CH3_AMP_ENna = tock_pos(a.ch3.FF1A_WRn, b.ch3.FF1A_WRn, b.ch3.GOVE, b.ch3.CH3_AMP_ENna, b.D7);

  /*p16.FASY*/ c.ch3.FASY = nand(b.apu.ADDR_FF1A, b.ch3.GAXO);
  /*p16.FEVO*/ c.ch3.CH3_AMP_ENa = not(!b.ch3.CH3_AMP_ENna);
  /*p16.FUVO*/ c.ch3.FUVO = nor(b.ch3.CH3_AMP_ENna, b.apu.APU_RESET1);
  /*p16.FAJU*/ c.ch3.FAJU = not(b.ch3.GYRA);
  /*p16.GUGU*/ c.ch3.GUGU = or(b.ch3.FUVO, b.ch3.FAJU);

  /*p16.GEKO*/ if (b.ch3.FASY) c.D7 = c.ch3.CH3_AMP_ENa;

  /*p16.GARA*/ c.ch3.GARA = tock_pos(a.ch3.CLK_xxCDxxGH,  b.ch3.CLK_xxCDxxGH,  b.ch3.FURY, b.ch3.GARA, b.ch3.GOFY);
  /*p16.GYTA*/ c.ch3.GYTA = tock_pos(a.ch3.CLK_xxCDxxGH,  b.ch3.CLK_xxCDxxGH,  b.ch3.GAZE, b.ch3.GYTA, b.ch3.GARA);
  /*p16.GYRA*/ c.ch3.GYRA = tock_pos(a.p01.CLK_ABxxEFxx1, b.p01.CLK_ABxxEFxx1, b.ch3.GAZE, b.ch3.GYRA, b.ch3.GYTA);

  /*p16.EPYX*/ c.ch3.FF1E_WRn = nor(b.apu.APU_WR, b.apu.ADDR_FF1E); // polarity?
  /*p16.GAVU*/ c.ch3.CH3_RESTART = tock_pos(a.ch3.FF1E_WRn, b.ch3.FF1E_WRn, b.ch3.FAKO, b.ch3.CH3_RESTART, b.D7);

  /*p16.FOBA*/ c.ch3.FOBA = tock_pos(a.p01.CPUCLK_xxxxEFGH9, b.p01.CPUCLK_xxxxEFGH9, b.ch3.GOMA, b.ch3.FOBA, b.ch3.CH3_RESTART);

  /*p16.GULO*/ c.ch3.GULO = not(b.ch3.FURY);
  /*p16.GOFY*/ c.ch3.GOFY = or(b.ch3.GULO, b.ch3.FOBA);
  /*p16.FURY*/ c.ch3.FURY = nor(b.ch3.GYTA, b.apu.APU_RESET1);
  /*p16.FAKO*/ c.ch3.FAKO = nor(b.apu.APU_RESET1, b.ch3.FOBA);

  //----------
  // FF1A NR30

  //----------
  // FF1B NR31

  /*p16.DERY*/ c.ch3.DERY = nand(b.apu.APU_WR, b.apu.ADDR_FF1B);
  /*p16.GETO*/ c.ch3.GETO = not(b.ch3.DERY);
  /*p16.EMUT*/ c.ch3.EMUT = not(b.ch3.DERY);
  /*p16.GAJY*/ c.ch3.GAJY = not(b.ch3.DERY);

  //----------
  // FF1C NR32

  //----------
  // FF1D NR33

  /*p16.KOTA*/ c.ch3.KOTA = nand(b.apu.ADDR_FF1D, b.apu.APU_WR);
  /*p16.JAFA*/ c.ch3.JAFA = nand(b.apu.ADDR_FF1D, b.apu.APU_WR);
  /*p16.KYHO*/ c.ch3.KYHO = not(b.ch3.KOTA);
  /*p16.KULY*/ c.ch3.KULY = not(b.ch3.JAFA);

  /*p16.KOGA*/ c.ch3.NR33_FREQ0 = tock_pos(a.ch3.KULY, b.ch3.KULY, b.ch3.KUHA, b.ch3.NR33_FREQ0, b.D0);
  /*p16.JOVY*/ c.ch3.NR33_FREQ1 = tock_pos(a.ch3.KULY, b.ch3.KULY, b.ch3.KUHA, b.ch3.NR33_FREQ1, b.D1);
  /*p16.JAXA*/ c.ch3.NR33_FREQ2 = tock_pos(a.ch3.KULY, b.ch3.KULY, b.ch3.KUHA, b.ch3.NR33_FREQ2, b.D2);
  /*p16.JEFE*/ c.ch3.NR33_FREQ3 = tock_pos(a.ch3.KULY, b.ch3.KULY, b.ch3.KUHA, b.ch3.NR33_FREQ3, b.D3);
  /*p16.JYPO*/ c.ch3.NR33_FREQ4 = tock_pos(a.ch3.KULY, b.ch3.KULY, b.ch3.KUHA, b.ch3.NR33_FREQ4, b.D4);
  /*p16.JOVE*/ c.ch3.NR33_FREQ5 = tock_pos(a.ch3.KYHO, b.ch3.KYHO, b.ch3.KUHA, b.ch3.NR33_FREQ5, b.D5);
  /*p16.KANA*/ c.ch3.NR33_FREQ6 = tock_pos(a.ch3.KYHO, b.ch3.KYHO, b.ch3.KUHA, b.ch3.NR33_FREQ6, b.D6);
  /*p16.KOGU*/ c.ch3.NR33_FREQ7 = tock_pos(a.ch3.KYHO, b.ch3.KYHO, b.ch3.KUHA, b.ch3.NR33_FREQ7, b.D7);

  /*p16.DOVO*/ c.ch3.DOVO = not(b.apu.CPU_RDn);
  /*p16.EGAD*/ c.ch3.EGAD = nand(b.apu.DBG_APU, b.ch3.DOVO);
  /*p16.HOXA*/ c.ch3.HOXA = not(b.apu.ADDR_FF1D);
  /*p16.GUTE*/ c.ch3.GUTE = nor(b.ch3.HOXA, b.ch3.EGAD);
  /*p16.HOVO*/ c.ch3.HOVO = not(b.ch3.GUTE);

  /*p16.JOFO*/ if (b.ch3.HOVO) c.D0 = b.ch3.KUTU_00;
  /*p16.KAFU*/ if (b.ch3.HOVO) c.D1 = b.ch3.KUPE_01;
  /*p16.KESY*/ if (b.ch3.HOVO) c.D2 = b.ch3.KUNU_02;
  /*p16.JUDE*/ if (b.ch3.HOVO) c.D3 = b.ch3.KEMU_03;
  /*p16.JUKE*/ if (b.ch3.HOVO) c.D4 = b.ch3.KYGU_04;
  /*p16.JEZA*/ if (b.ch3.HOVO) c.D5 = b.ch3.KEPA_05;
  /*p16.KORA*/ if (b.ch3.HOVO) c.D6 = b.ch3.KAFO_06;
  /*p16.KAMY*/ if (b.ch3.HOVO) c.D7 = b.ch3.KENO_07;

  //----------
  // FF1E NR34

  /*p16.ANUJ*/ c.ch3.CPU_WR_WEIRD = and(b.cpu.FROM_CPU5, b.apu.APU_WR);
  /*p16.FOVO*/ c.ch3.FOVO = nand(b.ch3.CPU_WR_WEIRD, b.apu.ADDR_FF1E);
  /*p16.HOTO*/ c.ch3.NR34_STOP = tock_pos(a.ch3.FOVO, b.ch3.FOVO, b.ch3.HEKY, b.ch3.NR34_STOP, b.D6);
  /*p16.GORY*/ c.ch3.GORY = not(b.apu.CPU_RDn);
  /*p16.GAWA*/ c.ch3.GAWA = nand(b.apu.ADDR_FF1E, b.ch3.GORY);

  /*p16.HUDA*/ c.ch3.HUDA = nand(b.apu.ADDR_FF1E, b.apu.APU_WR);
  /*p16.JUZO*/ c.ch3.JUZO = not(b.ch3.HUDA);

  /*p16.JEMO*/ c.ch3.NR34_FREQ8  = tock_pos(a.ch3.JUZO, b.ch3.JUZO, b.ch3.KOPY, b.ch3.NR34_FREQ8,  b.D0);
  /*p16.JETY*/ c.ch3.NR34_FREQ9  = tock_pos(a.ch3.JUZO, b.ch3.JUZO, b.ch3.KOPY, b.ch3.NR34_FREQ9,  b.D1);
  /*p16.JACY*/ c.ch3.NR34_FREQ10 = tock_pos(a.ch3.JUZO, b.ch3.JUZO, b.ch3.KOPY, b.ch3.NR34_FREQ10, b.D2);

  /*p16.GUNU*/ c.ch3.GUNU = not(b.apu.ADDR_FF1E);
  /*p16.FUVA*/ c.ch3.FUVA = or(b.ch3.GUNU, b.ch3.EGAD);

  /*p16.JUVY*/ if (b.ch3.FUVA) c.D0 = b.ch3.KEJU_08;
  /*p16.JURA*/ if (b.ch3.FUVA) c.D1 = b.ch3.KEZA_09;
  /*p16.HUFO*/ if (b.ch3.FUVA) c.D2 = b.ch3.JAPU_10;
  /*p16.HACA*/ if (b.ch3.GAWA) c.D6 = b.ch3.NR34_STOP;

  //----------

  /*p16.HAGA*/ c.ch3.HAGA = and(b.apu.APU_WR, b.apu.ADDR_FF1C);
  /*p16.GUZU*/ c.ch3.GUZU = not(b.ch3.HAGA);
  /*p16.JOTU*/ c.ch3.JOTU = not(b.apu.CPU_RDn);
  /*p16.HENU*/ c.ch3.HENU = nand(b.apu.ADDR_FF1C, b.ch3.JOTU);

  /*p16.HODY*/ c.ch3.NR32_VOL1 = tock_pos(a.ch3.GUZU, b.ch3.GUZU, b.ch3.GURO, b.ch3.NR32_VOL1, b.D6);
  /*p16.HUKY*/ c.ch3.NR32_VOL0 = tock_pos(a.ch3.GUZU, b.ch3.GUZU, b.ch3.GURO, b.ch3.NR32_VOL0, b.D5);

  /*p16.HAMU*/ if (b.ch3.HENU) c.D5 = b.ch3.NR32_VOL0;
  /*p16.HUCO*/ if (b.ch3.HENU) c.D6 = b.ch3.NR32_VOL1;

  //----------

  // some weird clocky thing...
  /*p18.JYFO*/ c.ch3.JYFO = not(b.ch3.JAPU_10);
  /*p18.HUNO*/ c.ch3.HUNO = tock_pos(a.ch3.JYFO, b.ch3.JYFO, b.ch3.GAFU, b.ch3.HUNO, !b.ch3.HUNO);
  /*p18.HEMA*/ c.ch3.HEMA = not(b.ch3.HUNO);
  /*p18.GASE*/ c.ch3.GASE = not(b.ch3.HEMA);
  /*p18.HUPA*/ c.ch3.HUPA = and(b.ch3.HUNO, b.p01.CLK_ABxxEFxx1);
  /*p18.GAFU*/ c.ch3.GAFU = nor(b.apu.APU_RESET1, b.ch3.GARA, b.ch3.HUPA);
  /*p18.HEFO*/ c.ch3.HEFO = nor(b.p01.CLK_ABxxEFxx1, b.ch3.GUGU);
  /*p18.HERA*/ c.ch3.HERA = nor(b.ch3.GASE, b.ch3.GARA);
  /*p18.JUTY*/ c.ch3.JUTY = not(b.ch3.HEFO);
  /*p18.KYKO*/ c.ch3.KYKO = not(b.ch3.HERA);
  /*p18.KYRU*/ c.ch3.KYRU = not(!b.ch3.KEMU_03);
  /*p18.KASO*/ c.ch3.KASO = not(b.ch3.HERA);
  /*p18.KESE*/ c.ch3.KESE = not(!b.ch3.KENO_07);
  /*p18.JERA*/ c.ch3.JERA = not(b.ch3.HERA);

  /*p18.KUTU*/ c.ch3.KUTU_00 = count_pos(a.ch3.JUTY,    b.ch3.JUTY,    b.ch3.KYKO, b.ch3.KUTU_00, b.ch3.NR33_FREQ0);
  /*p18.KUPE*/ c.ch3.KUPE_01 = count_pos(a.ch3.KUTU_00, b.ch3.KUTU_00, b.ch3.KYKO, b.ch3.KUPE_01, b.ch3.NR33_FREQ1);
  /*p18.KUNU*/ c.ch3.KUNU_02 = count_pos(a.ch3.KUPE_01, b.ch3.KUPE_01, b.ch3.KYKO, b.ch3.KUNU_02, b.ch3.NR33_FREQ2);
  /*p18.KEMU*/ c.ch3.KEMU_03 = count_pos(a.ch3.KUNU_02, b.ch3.KUNU_02, b.ch3.KYKO, b.ch3.KEMU_03, b.ch3.NR33_FREQ3);
  /*p18.KYGU*/ c.ch3.KYGU_04 = count_pos(a.ch3.KYRU,    b.ch3.KYRU,    b.ch3.KASO, b.ch3.KYGU_04, b.ch3.NR33_FREQ4);
  /*p18.KEPA*/ c.ch3.KEPA_05 = count_pos(a.ch3.KYGU_04, b.ch3.KYGU_04, b.ch3.KASO, b.ch3.KEPA_05, b.ch3.NR33_FREQ5);
  /*p18.KAFO*/ c.ch3.KAFO_06 = count_pos(a.ch3.KEPA_05, b.ch3.KEPA_05, b.ch3.KASO, b.ch3.KAFO_06, b.ch3.NR33_FREQ6);
  /*p18.KENO*/ c.ch3.KENO_07 = count_pos(a.ch3.KAFO_06, b.ch3.KAFO_06, b.ch3.KASO, b.ch3.KENO_07, b.ch3.NR33_FREQ7);
  /*p18.KEJU*/ c.ch3.KEJU_08 = count_pos(a.ch3.KESE,    b.ch3.KESE,    b.ch3.JERA, b.ch3.KEJU_08, b.ch3.NR34_FREQ8);
  /*p18.KEZA*/ c.ch3.KEZA_09 = count_pos(a.ch3.KEJU_08, b.ch3.KEJU_08, b.ch3.JERA, b.ch3.KEZA_09, b.ch3.NR34_FREQ9);
  /*p18.JAPU*/ c.ch3.JAPU_10 = count_pos(a.ch3.KEZA_09, b.ch3.KEZA_09, b.ch3.JERA, b.ch3.JAPU_10, b.ch3.NR34_FREQ10);

  /*p18.FOTO*/ c.ch3.FOTO = and(b.ch3.WAVE_LOOP, b.ch3.GASE);
  /*p18.ETAN*/ c.ch3.WAVE_RST = or(b.ch3.GARA, b.ch3.WAVE_LOOP);
  /*p18.GYRY*/ c.ch3.GYRY = nor(b.apu.APU_RESET1, b.ch3.GARA, b.ch3.FOTO);

  /*p18.DERO*/ c.ch3.WAVE_CLK = not(b.ch3.GASE);
  /*p18.EFAR*/ c.ch3.WAVE_IDX0 = tock_pos( a.ch3.WAVE_CLK,   b.ch3.WAVE_CLK,  b.ch3.WAVE_RST, b.ch3.WAVE_IDX0, !b.ch3.WAVE_IDX0);
  /*p18.ERUS*/ c.ch3.WAVE_IDX1 = tock_pos(!a.ch3.WAVE_IDX0, !b.ch3.WAVE_IDX0, b.ch3.WAVE_RST, b.ch3.WAVE_IDX1, !b.ch3.WAVE_IDX1);
  /*p18.EFUZ*/ c.ch3.WAVE_IDX2 = tock_pos(!a.ch3.WAVE_IDX1, !b.ch3.WAVE_IDX1, b.ch3.WAVE_RST, b.ch3.WAVE_IDX2, !b.ch3.WAVE_IDX2);
  /*p18.EXEL*/ c.ch3.WAVE_IDX3 = tock_pos(!a.ch3.WAVE_IDX2, !b.ch3.WAVE_IDX2, b.ch3.WAVE_RST, b.ch3.WAVE_IDX3, !b.ch3.WAVE_IDX3);
  /*p18.EFAL*/ c.ch3.WAVE_IDX4 = tock_pos(!a.ch3.WAVE_IDX3, !b.ch3.WAVE_IDX3, b.ch3.WAVE_RST, b.ch3.WAVE_IDX4, !b.ch3.WAVE_IDX4);
  /*p18.FETY*/ c.ch3.WAVE_LOOP = tock_pos(!a.ch3.WAVE_IDX4, !b.ch3.WAVE_IDX4, b.ch3.GYRY,     b.ch3.WAVE_LOOP, !b.ch3.WAVE_LOOP);

  /*p18.BOLE*/ c.ch3.WAVE_A0 = mux2(b.ch3.WAVE_IDX1, b.A00, b.ch3.CH3_ACTIVE);
  /*p18.AGYL*/ c.ch3.WAVE_A1 = mux2(b.ch3.WAVE_IDX2, b.A01, b.ch3.CH3_ACTIVE);
  /*p18.AFUM*/ c.ch3.WAVE_A2 = mux2(b.ch3.WAVE_IDX3, b.A02, b.ch3.CH3_ACTIVE);
  /*p18.AXOL*/ c.ch3.WAVE_A3 = mux2(b.ch3.WAVE_IDX4, b.A03, b.ch3.CH3_ACTIVE);

  /*p18.JECO*/ c.ch3.JECO = not(b.apu.CPU_RDn);
  /*p18.HONY*/ c.ch3.HONY = and(b.apu.DBG_APU, b.ch3.JECO, b.apu.ADDR_FF1C);
  /*p18.GENO*/ c.ch3.GENO = not(b.ch3.HONY);

  /*p18.FAPY*/ if (b.ch3.GENO) c.D0 = b.ch3.WAVE_IDX0;
  /*p18.FARO*/ if (b.ch3.GENO) c.D1 = b.ch3.WAVE_IDX1;
  /*p18.FOTE*/ if (b.ch3.GENO) c.D2 = b.ch3.WAVE_IDX2;
  /*p18.FANA*/ if (b.ch3.GENO) c.D3 = b.ch3.WAVE_IDX3;
  /*p18.FERA*/ if (b.ch3.GENO) c.D4 = b.ch3.WAVE_IDX4;

  /*p18.COKA*/ c.ch3.CH3_ACTIVE = not(!b.ch3.DAVO);
  /*p18.ERED*/ c.ch3.CH3_ACTIVEn = not(b.ch3.CH3_ACTIVE);

  /*p18.GEDO*/ c.ch3.GEDO = and(b.p01.CLK_256a, b.ch3.NR34_STOP);
  /*p18.FYGO*/ c.ch3.FYGO = or(b.apu.APU_RESET1, b.ch3.GEDO, b.ch3.CH3_AMP_ENna);
  /*p18.FOZU*/ c.ch3.FOZU = or(b.ch3.GARA, b.ch3.FYGO);
  /*p18.EZAS*/ c.ch3.EZAS = not(b.ch3.FOZU);
  /*p18.DORU*/ c.ch3.DORU = not(b.ch3.EZAS);
  /*p18.DAVO*/ c.ch3.DAVO = tock_pos(a.p01.AJER_2M, b.p01.AJER_2M, b.ch3.CALU, b.ch3.DAVO, b.ch3.DORU);
  /*p18.GUDA*/ c.ch3.GUDA = nor(b.ch3.GETO, b.apu.APU_RESET1, b.ch3.GARA);
  /*p18.FEXU*/ c.ch3.FEXU = tock_pos(!a.ch3.FYRU_07, !b.ch3.FYRU_07, b.ch3.GUDA, b.ch3.FEXU, !b.ch3.FEXU);
  /*p18.GEPY*/ c.ch3.GEPY = nor(b.p01.CLK_256a, !b.ch3.NR34_STOP);
  /*p18.GENU*/ c.ch3.GENU = not(b.ch3.GEPY);
  /*p18.GEVO*/ c.ch3.GEVO_00 = tock_pos(a.ch3.GENU,    b.ch3.GENU,    b.ch3.GAJY, b.ch3.GEVO_00, b.D0);
  /*p18.FORY*/ c.ch3.FORY_01 = tock_pos(a.ch3.GEVO_00, b.ch3.GEVO_00, b.ch3.GAJY, b.ch3.FORY_01, b.D1);
  /*p18.GATU*/ c.ch3.GATU_02 = tock_pos(a.ch3.FORY_01, b.ch3.FORY_01, b.ch3.GAJY, b.ch3.GATU_02, b.D2);
  /*p18.GAPO*/ c.ch3.GAPO_03 = tock_pos(a.ch3.GATU_02, b.ch3.GATU_02, b.ch3.GAJY, b.ch3.GAPO_03, b.D3);
  /*p18.FALU*/ c.ch3.FALU = not(b.ch3.GAPO_03);
  /*p18.GEMO*/ c.ch3.GEMO_04 = tock_pos(a.ch3.FALU,    b.ch3.FALU,    b.ch3.EMUT, b.ch3.GEMO_04, b.D4);
  /*p18.FORO*/ c.ch3.FORO_05 = tock_pos(a.ch3.GEMO_04, b.ch3.GEMO_04, b.ch3.EMUT, b.ch3.FORO_05, b.D5);
  /*p18.FAVE*/ c.ch3.FAVE_06 = tock_pos(a.ch3.FORO_05, b.ch3.FORO_05, b.ch3.EMUT, b.ch3.FAVE_06, b.D6);
  /*p18.FYRU*/ c.ch3.FYRU_07 = tock_pos(a.ch3.FAVE_06, b.ch3.FAVE_06, b.ch3.EMUT, b.ch3.FYRU_07, b.D7);
  /*p18.GOKA*/ c.ch3.GOKA = nor(!b.ch3.NR32_VOL0,  b.ch3.NR32_VOL0);
  /*p18.GEMY*/ c.ch3.GEMY = nor(!b.ch3.NR32_VOL0, !b.ch3.NR32_VOL0);
  /*p18.GEGE*/ c.ch3.GEGE = nor( b.ch3.NR32_VOL0, !b.ch3.NR32_VOL0);
  /*p18.EZAG*/ c.ch3.EZAG = amux3(b.ch3.WAVE_PLAY_D0, b.ch3.GEGE, b.ch3.WAVE_PLAY_D1, b.ch3.GOKA, b.ch3.WAVE_PLAY_D2, b.ch3.GEMY);
  /*p18.EVUG*/ c.ch3.EVUG = amux3(b.ch3.WAVE_PLAY_D1, b.ch3.GEGE, b.ch3.WAVE_PLAY_D2, b.ch3.GOKA, b.ch3.WAVE_PLAY_D3, b.ch3.GEMY);
  /*p18.DOKY*/ c.ch3.DOKY = amux2(b.ch3.GEGE, b.ch3.WAVE_PLAY_D2, b.ch3.WAVE_PLAY_D3, b.ch3.GEMY);
  /*p18.DORE*/ c.ch3.DORE = and(b.ch3.WAVE_PLAY_D3, b.ch3.GEGE);

  /*p18.BARY*/ c.ch3.WAVE_DAC0 = and(b.ch3.CH3_ACTIVE, b.ch3.EZAG);
  /*p18.BYKA*/ c.ch3.WAVE_DAC1 = and(b.ch3.CH3_ACTIVE, b.ch3.EVUG);
  /*p18.BOPA*/ c.ch3.WAVE_DAC2 = and(b.ch3.CH3_ACTIVE, b.ch3.DOKY);
  /*p18.BELY*/ c.ch3.WAVE_DAC3 = and(b.ch3.CH3_ACTIVE, b.ch3.DORE);

  /*p18.BENO*/ c.ch3.BENO = mux2(b.ch3.BUTU, b.ch3.WAVE_RAM_RDn, b.ch3.CH3_ACTIVE);
  /*p18.ATOK*/ c.ch3.WAVE_RAM_CTRL3 = not(b.ch3.BENO);

  //----------

  /*p17.BOKE*/ c.ch3.BOKE = not(b.apu.CPU_RDn);
  /*p17.BENA*/ c.ch3.BENA = nand(b.ch3.BOKE, b.apu.ADDR_FF3X);

  /*p17.BYZA*/ c.ch3.WAVE_RAM_WR  = and(b.apu.APU_WR, b.apu.ADDR_FF3X);
  /*p17.CAZU*/ c.ch3.WAVE_RAM_RDn = not(b.ch3.BENA);
  /*p17.AMYT*/ c.ch3.WAVE_RAM_WRn = not(b.ch3.WAVE_RAM_WR);

  //----------

  /*p17.BORY*/ c.ch3.BORY = not(b.ch3.ABUR);
  /*p17.BETA*/ c.ch3.BETA = or(b.ch3.WAVE_RAM_WR, b.ch3.WAVE_RAM_RDn, b.ch3.BORY);
  /*p17.AZOR*/ c.ch3.AZOR = not(b.ch3.BETA);
  /*p17.BUKU*/ c.ch3.BUKU = not(b.ch3.AZOR);
  /*p17.BUSA*/ c.ch3.BUSA = tock_pos(a.p01.CLK_xBxDxFxH1, b.p01.CLK_xBxDxFxH1, b.ch3.BAMA, b.ch3.BUSA, b.ch3.GASE);
  /*p17.BANO*/ c.ch3.BANO = tock_pos(a.ch3.COZY,          b.ch3.COZY,          b.ch3.BAMA, b.ch3.BANO, b.ch3.BUSA);
  /*p17.AZUS*/ c.ch3.AZUS = tock_pos(a.p01.CLK_xBxDxFxH1, b.p01.CLK_xBxDxFxH1, b.ch3.BAMA, b.ch3.AZUS, b.ch3.BANO);
  /*p17.AZET*/ c.ch3.AZET = tock_pos(a.ch3.ARUC,          b.ch3.ARUC,          b.ch3.BAMA, b.ch3.AZET, b.ch3.AZUS);
  /*p17.BUTU*/ c.ch3.BUTU = not(b.ch3.AZUS);
  /*p17.BOXO*/ c.ch3.BOXO = nor(b.ch3.AZUS, b.ch3.AZET);
  /*p17.BORU*/ c.ch3.BORU = not(b.ch3.BOXO);
  /*p17.ATUR*/ c.ch3.ATUR = mux2(b.ch3.BORU, b.ch3.BUKU, b.ch3.CH3_ACTIVE);
  /*p17.BEKA*/ c.ch3.BEKA = not(b.ch3.BUTU); // where does this go?
  /*p17.COJU*/ c.ch3.COJU = not(b.ch3.BUTU);
  /*p17.BAJA*/ c.ch3.BAJA = not(b.ch3.BUTU);
  /*p17.BUFE*/ c.ch3.BUFE = not(b.ch3.BUTU);

  /*p17.ALER*/ c.ch3.WAVE_RAM_CTRL1 = not(b.ch3.ATUR);

  //----------

  /*p17.CUGO*/ c.ch3.WAVE_D0n = not(b.WAVE_D0);
  /*p17.CEGU*/ c.ch3.WAVE_D1n = not(b.WAVE_D1);
  /*p17.ATEC*/ c.ch3.WAVE_D2n = not(b.WAVE_D2);
  /*p17.ADOK*/ c.ch3.WAVE_D3n = not(b.WAVE_D3);
  /*p17.BACA*/ c.ch3.WAVE_D4n = not(b.WAVE_D4);
  /*p17.BERO*/ c.ch3.WAVE_D5n = not(b.WAVE_D5);
  /*p17.CUTO*/ c.ch3.WAVE_D6n = not(b.WAVE_D6);
  /*p17.AKAF*/ c.ch3.WAVE_D7n = not(b.WAVE_D7);

  // uhhhh polarity? this is probably wrong...
  /*p17.DUGU*/ if (b.ch3.WAVE_RAM_RDn) c.D7 = b.ch3.WAVE_D0n;
  /*p17.DESY*/ if (b.ch3.WAVE_RAM_RDn) c.D6 = b.ch3.WAVE_D1n;
  /*p17.BATY*/ if (b.ch3.WAVE_RAM_RDn) c.D5 = b.ch3.WAVE_D2n;
  /*p17.BADE*/ if (b.ch3.WAVE_RAM_RDn) c.D4 = b.ch3.WAVE_D3n;
  /*p17.BUNE*/ if (b.ch3.WAVE_RAM_RDn) c.D3 = b.ch3.WAVE_D4n;
  /*p17.BAVA*/ if (b.ch3.WAVE_RAM_RDn) c.D2 = b.ch3.WAVE_D5n;
  /*p17.DESA*/ if (b.ch3.WAVE_RAM_RDn) c.D1 = b.ch3.WAVE_D6n;
  /*p17.BEZU*/ if (b.ch3.WAVE_RAM_RDn) c.D0 = b.ch3.WAVE_D7n;

  /*p17.CYFO*/ c.ch3.SAMPLE_0 = tock_pos(a.ch3.BEKA, b.ch3.BEKA, b.ch3.ACOR, b.ch3.SAMPLE_0, b.WAVE_D0);
  /*p17.CESY*/ c.ch3.SAMPLE_1 = tock_pos(a.ch3.BUFE, b.ch3.BUFE, b.ch3.ACOR, b.ch3.SAMPLE_1, b.WAVE_D1);
  /*p17.BUDY*/ c.ch3.SAMPLE_2 = tock_pos(a.ch3.BAJA, b.ch3.BAJA, b.ch3.ACOR, b.ch3.SAMPLE_2, b.WAVE_D2);
  /*p17.BEGU*/ c.ch3.SAMPLE_3 = tock_pos(a.ch3.COJU, b.ch3.COJU, b.ch3.ACOR, b.ch3.SAMPLE_3, b.WAVE_D3);
  /*p17.CUVO*/ c.ch3.SAMPLE_4 = tock_pos(a.ch3.BEKA, b.ch3.BEKA, b.ch3.ACOR, b.ch3.SAMPLE_4, b.WAVE_D4);
  /*p17.CEVO*/ c.ch3.SAMPLE_5 = tock_pos(a.ch3.BUFE, b.ch3.BUFE, b.ch3.ACOR, b.ch3.SAMPLE_5, b.WAVE_D5);
  /*p17.BORA*/ c.ch3.SAMPLE_6 = tock_pos(a.ch3.BAJA, b.ch3.BAJA, b.ch3.ACOR, b.ch3.SAMPLE_6, b.WAVE_D6);
  /*p17.BEPA*/ c.ch3.SAMPLE_7 = tock_pos(a.ch3.COJU, b.ch3.COJU, b.ch3.ACOR, b.ch3.SAMPLE_7, b.WAVE_D7);

  /*p17.DATE*/ c.ch3.WAVE_PLAY_D0 = mux2n(!b.ch3.SAMPLE_0, !b.ch3.SAMPLE_4, b.ch3.WAVE_IDX0);
  /*p17.DAZY*/ c.ch3.WAVE_PLAY_D1 = mux2n(!b.ch3.SAMPLE_1, !b.ch3.SAMPLE_5, b.ch3.WAVE_IDX0);
  /*p17.COPO*/ c.ch3.WAVE_PLAY_D3 = mux2n(!b.ch3.SAMPLE_2, !b.ch3.SAMPLE_6, b.ch3.WAVE_IDX0);
  /*p17.CUZO*/ c.ch3.WAVE_PLAY_D2 = mux2n(!b.ch3.SAMPLE_3, !b.ch3.SAMPLE_7, b.ch3.WAVE_IDX0);

}
