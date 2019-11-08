#include "../Schematics.h"
#include "Gameboy.h"

struct P16_Ch3Regs {
  bool GEJO,GUCY,GOVE,GUXE,FUVO,GUGU,FEVO,GEKO,FASY;
  bool FABO,GARA,GYTA,GYRA,FAJU,GAZE,FURY,GOMA,FOBA;
  bool GULO,GOFY,EPYX,FAKO,GAVU;

  bool KOTA,KUHA,KYHO,JAFA,KULY;
  bool KOGA_00,JOVY_01,JAXA_02,JEFE_03,JYPO_04,JOVE_05,KANA_06,KOGU_07;

  bool DOVO,EGAD,HOXA,GUTE,HOVO;
  bool JOFO_00,KAFU_01,KESY_02,JUDE_03,JUKE_04,JEZA_05,KORA_06,KAMY_07;
  bool KUTU_00,KUPE_01,KUNU_02,KEMU_03,KYGU_04,KEPA_05,KAFO_06,KENO_07;

  bool DERY,GETO,EMUT,GAJY;

  bool ANUJ,FOVO,HEKY,HOTO,GORY,GAWA,HACA;
  bool HUDA,JUZO,KOPY;
  bool JEMO_00,JETY_01,JACY_02;
  bool GUNU,FUVA;

  bool HUFO_02,JURA_01,JUVY_00;
  bool JAPU_02,KEZA_01,KEJU_00;

  bool JOTU,HENU,GURO,GUZU,HAGA,HUCO,HAMU;
  bool HUKY_06,HODY_05;

  bool GAXO;

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


  bool FF1A,FF1B,FF1C,FF1D,FF1E;

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};

void P16_Ch3Regs::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P16_Ch3Regs pa = {};
  const P16_Ch3Regs pb = {};
  P16_Ch3Regs pc;

  pc.GEJO = and(pb.FF1A, gb.APU_WR);
  pc.GUCY = not(pb.GEJO);
  pc.GOVE = not(gb.APU_RESET);
  pc.GUXE = tock_pos(pa.GUCY, pb.GUCY, pb.GOVE, pb.GUXE, gb.cpu.D7);
  pc.FUVO = nor(pb.GUXE, gb.APU_RESET);
  pc.FASY = nand(pb.FF1A, pb.GAXO);
  pc.FEVO = not(!pb.GUXE);
  pc.GUGU = or(pb.FUVO, pb.FAJU);
  pc.GEKO = not(pb.FEVO);

  pc.FF1A_D7n = !pb.GUXE;

  if (pb.FASY) {
    gc.cpu.D7 = pb.GEKO;
  }

  pc.FABO = not(gb.CERY_2M);
  pc.GAZE = not(gb.APU_RESET);
  pc.GARA = tock_pos(pa.FABO, pb.FABO, pb.FURY, pb.GARA, pb.GOFY);
  pc.GYTA = tock_pos(pa.FABO, pb.FABO, pb.GAZE, pb.GYTA, pb.GARA);
  pc.GYRA = tock_pos(ga.CERY_2M, gb.CERY_2M, pb.GAZE, pb.GYRA, pb.GYTA);
  pc.FAJU = not(pb.GYRA);
  pc.GOMA = not(gb.APU_RESET);
  pc.FOBA = tock_pos(ga.PHIn, gb.PHIn, pb.GOMA, pb.FOBA, pb.GAVU);
  pc.GULO = not(pb.FURY);
  pc.GOFY = or(pb.GULO, pb.FOBA);
  pc.FURY = nor(pb.GYTA, gb.APU_RESET);
  pc.FAKO = nor(gb.APU_RESET, pb.FOBA);
  pc.EPYX = nor(gb.APU_WR, pb.FF1E);
  pc.GAVU = tock_pos(pa.EPYX, pb.EPYX, pb.FAKO, pb.GAVU, gb.cpu.D7);

  //----------

  pc.KOTA = nand(pb.FF1D, gb.APU_WR);
  pc.JAFA = nand(pb.FF1D, gb.APU_WR);
  pc.KUHA = not(gb.APU_RESET);
  pc.KYHO = not(pb.KOTA);
  pc.KULY = not(pb.JAFA);

  pc.KOGU_07 = tock_pos(pa.KYHO, pb.KYHO, pb.KUHA, pb.KOGU_07, gb.cpu.D7);
  pc.KANA_06 = tock_pos(pa.KYHO, pb.KYHO, pb.KUHA, pb.KANA_06, gb.cpu.D6);
  pc.JOVE_05 = tock_pos(pa.KYHO, pb.KYHO, pb.KUHA, pb.JOVE_05, gb.cpu.D5);

  pc.JYPO_04 = tock_pos(pa.KULY, pb.KULY, pb.KUHA, pb.JYPO_04, gb.cpu.D4);
  pc.JEFE_03 = tock_pos(pa.KULY, pb.KULY, pb.KUHA, pb.JEFE_03, gb.cpu.D3);
  pc.JAXA_02 = tock_pos(pa.KULY, pb.KULY, pb.KUHA, pb.JAXA_02, gb.cpu.D2);
  pc.JOVY_01 = tock_pos(pa.KULY, pb.KULY, pb.KUHA, pb.JOVY_01, gb.cpu.D1);
  pc.KOGA_00 = tock_pos(pa.KULY, pb.KULY, pb.KUHA, pb.KOGA_00, gb.cpu.D0);


  pc.FF1D_D7 = pb.KOGU_07;
  pc.FF1D_D6 = pb.KANA_06;
  pc.FF1D_D5 = pb.JOVE_05;

  pc.FF1D_D4 = pb.JYPO_04;
  pc.FF1D_D3 = pb.JEFE_03;
  pc.FF1D_D2 = pb.JAXA_02;
  pc.FF1D_D1 = pb.JOVY_01;
  pc.FF1D_D0 = pb.KOGA_00;

  pc.DOVO = not(gb.CPU_RDn);
  pc.EGAD = nand(gb.NET03, pb.DOVO);
  pc.HOXA = not(pb.FF1D);
  pc.GUTE = nor(pb.HOXA, pb.EGAD);
  pc.HOVO = not(pb.GUTE);

  pc.KAMY_07 = not(pb.KENO_07);
  pc.KORA_06 = not(pb.KAFO_06);
  pc.JEZA_05 = not(pb.KEPA_05);
  pc.JUKE_04 = not(pb.KYGU_04);
  pc.JUDE_03 = not(pb.KEMU_03);
  pc.KESY_02 = not(pb.KUNU_02);
  pc.KAFU_01 = not(pb.KUPE_01);
  pc.JOFO_00 = not(pb.KUTU_00);

  //----------

  pc.DERY = nand(gb.APU_WR, pb.FF1B);
  pc.GETO = not(pb.DERY);
  pc.EMUT = not(pb.DERY);
  pc.GAJY = not(pb.DERY);

  pc.FF1B_WR3 = pb.GETO;
  pc.FF1B_WR1 = pb.EMUT;
  pc.FF1B_WR2 = pb.GAJY;

  //----------

  pc.ANUJ = and(gb.cpu.FROM_CPU5, gb.APU_WR);
  pc.FOVO = nand(pb.ANUJ, pb.FF1E);
  pc.HEKY = not(gb.APU_RESET);
  pc.HOTO = tock_pos(pa.FOVO, pb.FOVO, pb.HEKY, pb.HOTO, gb.cpu.D6);
  pc.GORY = not(gb.CPU_RDn);
  pc.GAWA = nand(pb.FF1E, pb.GORY);
  pc.HACA = not(!pb.HOTO);

  if (pb.GAWA) {
    gc.cpu.D6 = pb.HACA;
  }

  pc.HUDA = nand(pb.FF1E, gb.APU_WR);
  pc.JUZO = not(pb.HUDA);
  pc.KOPY = not(gb.APU_RESET);

  pc.JEMO_00 = tock_pos(pa.JUZO, pb.JUZO, pb.KOPY, pb.JEMO_00, gb.cpu.D0);
  pc.JETY_01 = tock_pos(pa.JUZO, pb.JUZO, pb.KOPY, pb.JETY_01, gb.cpu.D1);
  pc.JACY_02 = tock_pos(pa.JUZO, pb.JUZO, pb.KOPY, pb.JACY_02, gb.cpu.D2);

  pc.FF1E_D0 = pb.JEMO_00;
  pc.FF1E_D2 = pb.JACY_02;
  pc.FF1E_D1 = pb.JETY_01;

  pc.GUNU = not(pb.FF1E);
  pc.FUVA = or(pb.GUNU, pb.EGAD);

  pc.HUFO_02 = not(pb.JAPU_02);
  pc.JURA_01 = not(pb.KEZA_01);
  pc.JUVY_00 = not(pb.KEJU_00);

  if (pb.FUVA) {
    gc.cpu.D2 = pb.HUFO_02;
    gc.cpu.D1 = pb.JURA_01;
    gc.cpu.D0 = pb.JUVY_00;
  }

  //----------

  pc.HAGA = and(gb.APU_WR, pb.FF1C);
  pc.GUZU = not(pb.HAGA);
  pc.GURO = not(gb.APU_RESET);
  pc.JOTU = not(gb.CPU_RDn);
  pc.HENU = nand(pb.FF1C, pb.JOTU);

  pc.HUKY_06 = tock_pos(pa.GUZU, pb.GUZU, pb.GURO, pb.HUKY_06, gb.cpu.D6);
  pc.HODY_05 = tock_pos(pa.GUZU, pb.GUZU, pb.GURO, pb.HODY_05, gb.cpu.D5);

  pc.HUCO = not(!pb.HUKY_06);
  pc.HAMU = not(!pb.HODY_05);

  if (pb.HENU) {
    gc.cpu.D6 = pb.HUCO;
    gc.cpu.D5 = pb.HAMU;
  }

  pc.FF1C_D6  = pb.HUKY_06;
  pc.FF1C_D6n = !pb.HUKY_06;
  pc.FF1C_D5  = pb.HODY_05;
  pc.FF1C_D5n = !pb.HODY_05;


}