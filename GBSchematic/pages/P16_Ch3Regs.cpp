#include "../Schematics.h"
#include "Gameboy.h"

void P16_Ch3Regs::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {
  c.p16.GEJO = and(b.p16.FF1A, b.p10.APU_WR);
  c.p16.GUCY = not(b.p16.GEJO);
  c.p16.GUXE = tock_pos(a.p16.GUCY, b.p16.GUCY, b.p16.GOVE, b.p16.GUXE, b.D7);
  c.p16.FUVO = nor(b.p16.GUXE, b.p09.APU_RESET1);
  c.p16.FASY = nand(b.p16.FF1A, b.p16.GAXO);
  c.p16.FEVO = not(!b.p16.GUXE);
  c.p16.GUGU = or(b.p16.FUVO, b.p16.FAJU);
  c.p16.GEKO = not(b.p16.FEVO);

  c.p16.FF1A_D7n = !b.p16.GUXE;

  if (b.p16.FASY) {
    c.D7 = b.p16.GEKO;
  }

  c.p16.GARA = tock_pos(a.p16.FABO, b.p16.FABO, b.p16.FURY, b.p16.GARA, b.p16.GOFY);
  c.p16.GYTA = tock_pos(a.p16.FABO, b.p16.FABO, b.p16.GAZE, b.p16.GYTA, b.p16.GARA);
  c.p16.GYRA = tock_pos(a.p01.CLK_ABxxEFxx1, b.p01.CLK_ABxxEFxx1, b.p16.GAZE, b.p16.GYRA, b.p16.GYTA);
  c.p16.FAJU = not(b.p16.GYRA);
  c.p16.FOBA = tock_pos(a.p01.CPUCLK_xxxxEFGH9, b.p01.CPUCLK_xxxxEFGH9, b.p16.GOMA, b.p16.FOBA, b.p16.GAVU);
  c.p16.GULO = not(b.p16.FURY);
  c.p16.GOFY = or(b.p16.GULO, b.p16.FOBA);
  c.p16.FURY = nor(b.p16.GYTA, b.p09.APU_RESET1);
  c.p16.FAKO = nor(b.p09.APU_RESET1, b.p16.FOBA);
  c.p16.EPYX = nor(b.p10.APU_WR, b.p16.FF1E);
  c.p16.GAVU = tock_pos(a.p16.EPYX, b.p16.EPYX, b.p16.FAKO, b.p16.GAVU, b.D7);

  //----------

  c.p16.KOTA = nand(b.p16.FF1D, b.p10.APU_WR);
  c.p16.JAFA = nand(b.p16.FF1D, b.p10.APU_WR);
  c.p16.KYHO = not(b.p16.KOTA);
  c.p16.KULY = not(b.p16.JAFA);

  c.p16.KOGU_07 = tock_pos(a.p16.KYHO, b.p16.KYHO, b.p16.KUHA, b.p16.KOGU_07, b.D7);
  c.p16.KANA_06 = tock_pos(a.p16.KYHO, b.p16.KYHO, b.p16.KUHA, b.p16.KANA_06, b.D6);
  c.p16.JOVE_05 = tock_pos(a.p16.KYHO, b.p16.KYHO, b.p16.KUHA, b.p16.JOVE_05, b.D5);

  c.p16.JYPO_04 = tock_pos(a.p16.KULY, b.p16.KULY, b.p16.KUHA, b.p16.JYPO_04, b.D4);
  c.p16.JEFE_03 = tock_pos(a.p16.KULY, b.p16.KULY, b.p16.KUHA, b.p16.JEFE_03, b.D3);
  c.p16.JAXA_02 = tock_pos(a.p16.KULY, b.p16.KULY, b.p16.KUHA, b.p16.JAXA_02, b.D2);
  c.p16.JOVY_01 = tock_pos(a.p16.KULY, b.p16.KULY, b.p16.KUHA, b.p16.JOVY_01, b.D1);
  c.p16.KOGA_00 = tock_pos(a.p16.KULY, b.p16.KULY, b.p16.KUHA, b.p16.KOGA_00, b.D0);


  c.p16.FF1D_D7 = b.p16.KOGU_07;
  c.p16.FF1D_D6 = b.p16.KANA_06;
  c.p16.FF1D_D5 = b.p16.JOVE_05;

  c.p16.FF1D_D4 = b.p16.JYPO_04;
  c.p16.FF1D_D3 = b.p16.JEFE_03;
  c.p16.FF1D_D2 = b.p16.JAXA_02;
  c.p16.FF1D_D1 = b.p16.JOVY_01;
  c.p16.FF1D_D0 = b.p16.KOGA_00;

  c.p16.DOVO = not(b.p09.CPU_RDn);
  c.p16.EGAD = nand(b.p09.NET03, b.p16.DOVO);
  c.p16.HOXA = not(b.p16.FF1D);
  c.p16.GUTE = nor(b.p16.HOXA, b.p16.EGAD);
  c.p16.HOVO = not(b.p16.GUTE);

  c.p16.KAMY_07 = not(b.p16.KENO_07);
  c.p16.KORA_06 = not(b.p16.KAFO_06);
  c.p16.JEZA_05 = not(b.p16.KEPA_05);
  c.p16.JUKE_04 = not(b.p16.KYGU_04);
  c.p16.JUDE_03 = not(b.p16.KEMU_03);
  c.p16.KESY_02 = not(b.p16.KUNU_02);
  c.p16.KAFU_01 = not(b.p16.KUPE_01);
  c.p16.JOFO_00 = not(b.p16.KUTU_00);

  //----------

  c.p16.DERY = nand(b.p10.APU_WR, b.p16.FF1B);
  c.p16.GETO = not(b.p16.DERY);
  c.p16.EMUT = not(b.p16.DERY);
  c.p16.GAJY = not(b.p16.DERY);

  c.p16.FF1B_WR3 = b.p16.GETO;
  c.p16.FF1B_WR1 = b.p16.EMUT;
  c.p16.FF1B_WR2 = b.p16.GAJY;

  //----------

  c.p16.ANUJ = and(b.cpu.FROM_CPU5, b.p10.APU_WR);
  c.p16.FOVO = nand(b.p16.ANUJ, b.p16.FF1E);
  c.p16.HOTO = tock_pos(a.p16.FOVO, b.p16.FOVO, b.p16.HEKY, b.p16.HOTO, b.D6);
  c.p16.GORY = not(b.p09.CPU_RDn);
  c.p16.GAWA = nand(b.p16.FF1E, b.p16.GORY);
  c.p16.HACA = not(!b.p16.HOTO);

  if (b.p16.GAWA) {
    c.D6 = b.p16.HACA;
  }

  c.p16.HUDA = nand(b.p16.FF1E, b.p10.APU_WR);
  c.p16.JUZO = not(b.p16.HUDA);

  c.p16.JEMO_00 = tock_pos(a.p16.JUZO, b.p16.JUZO, b.p16.KOPY, b.p16.JEMO_00, b.D0);
  c.p16.JETY_01 = tock_pos(a.p16.JUZO, b.p16.JUZO, b.p16.KOPY, b.p16.JETY_01, b.D1);
  c.p16.JACY_02 = tock_pos(a.p16.JUZO, b.p16.JUZO, b.p16.KOPY, b.p16.JACY_02, b.D2);

  c.p16.FF1E_D0 = b.p16.JEMO_00;
  c.p16.FF1E_D2 = b.p16.JACY_02;
  c.p16.FF1E_D1 = b.p16.JETY_01;

  c.p16.GUNU = not(b.p16.FF1E);
  c.p16.FUVA = or(b.p16.GUNU, b.p16.EGAD);

  c.p16.HUFO_02 = not(b.p16.JAPU_02);
  c.p16.JURA_01 = not(b.p16.KEZA_01);
  c.p16.JUVY_00 = not(b.p16.KEJU_00);

  if (b.p16.FUVA) {
    c.D2 = b.p16.HUFO_02;
    c.D1 = b.p16.JURA_01;
    c.D0 = b.p16.JUVY_00;
  }

  //----------

  c.p16.HAGA = and(b.p10.APU_WR, b.p16.FF1C);
  c.p16.GUZU = not(b.p16.HAGA);
  c.p16.JOTU = not(b.p09.CPU_RDn);
  c.p16.HENU = nand(b.p16.FF1C, b.p16.JOTU);

  c.p16.HUKY_06 = tock_pos(a.p16.GUZU, b.p16.GUZU, b.p16.GURO, b.p16.HUKY_06, b.D6);
  c.p16.HODY_05 = tock_pos(a.p16.GUZU, b.p16.GUZU, b.p16.GURO, b.p16.HODY_05, b.D5);

  c.p16.HUCO = not(!b.p16.HUKY_06);
  c.p16.HAMU = not(!b.p16.HODY_05);

  if (b.p16.HENU) {
    c.D6 = b.p16.HUCO;
    c.D5 = b.p16.HAMU;
  }

  c.p16.FF1C_D6  = b.p16.HUKY_06;
  c.p16.FF1C_D6n = !b.p16.HUKY_06;
  c.p16.FF1C_D5  = b.p16.HODY_05;
  c.p16.FF1C_D5n = !b.p16.HODY_05;


}