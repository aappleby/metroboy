#include "P05_JoypadIO.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P05_JoypadIO::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P05_JoypadIO& pb = gb.p05;
  P05_JoypadIO& pc = gc.p05;

  pc.JUTE_00 = tock_pos(ga.FF00WR, gb.FF00WR, gb.RESET2, pc.JUTE_00, gb.cpu.D0);
  pc.KECY_01 = tock_pos(ga.FF00WR, gb.FF00WR, gb.RESET2, pc.KECY_01, gb.cpu.D1);
  pc.JALE_02 = tock_pos(ga.FF00WR, gb.FF00WR, gb.RESET2, pc.JALE_02, gb.cpu.D2);
  pc.KYME_03 = tock_pos(ga.FF00WR, gb.FF00WR, gb.RESET2, pc.KYME_03, gb.cpu.D3);
  pc.KELY_04 = tock_pos(ga.FF00WR, gb.FF00WR, gb.RESET2, pc.KELY_04, gb.cpu.D4);
  pc.COFY_05 = tock_pos(ga.FF00WR, gb.FF00WR, gb.RESET2, pc.COFY_05, gb.cpu.D5);
  pc.KUKO_06 = tock_pos(ga.FF00WR, gb.FF00WR, gb.RESET2, pc.KUKO_06, gb.cpu.D6);
  pc.KERU_07 = tock_pos(ga.FF00WR, gb.FF00WR, gb.RESET2, pc.KERU_07, gb.cpu.D7);

  pc.KOLE = nand(pb.JUTE_00, gb.FF60_D0);
  pc.KYBU = nor (pb.JUTE_00, pb.KURA);

  pc.KYTO = nand(pb.KECY_01, gb.FF60_D0);
  pc.KABU = nor (pb.KECY_01, pb.KURA);

  pc.KYHU = nand(pb.JALE_02, gb.FF60_D0);
  pc.KASY = nor (gb.FF60_D0, pb.KURA); // this one doesn't match?

  pc.KORY = nand(pb.KYME_03, gb.FF60_D0);
  pc.KALE = nor (pb.KYME_03, pb.KURA);

  pc.KARU = or(!pb.KELY_04, pb.KURA);

  pc.KURA = not(gb.FF60_D0);
  pc.CELA = or(!pb.COFY_05, pb.KURA);

  pc.KENA = mux2(pb.KUKO_06, gb.SER_OUT, gb.FF60_D0);

  pc.JEVA = not(gb.FF60_D0);
  pc.KORE = nand(pb.KERU_07, gb.FF60_D0);
  pc.KYWE = nor (pb.KERU_07, pb.JEVA);

  // FIXME really unsure about these pin assignments, seem to have a few missing signals
  gc.chip.SOUT  = pb.KENA;

  gc.chip.P10_A = pb.KOLE;
  gc.chip.P10_D = pb.KYBU;

  gc.chip.P11_A = pb.KYTO;
  gc.chip.P11_D = pb.KABU;

  gc.chip.P12_A = pb.KYHU;
  gc.chip.P12_D = pb.KASY;

  gc.chip.P13_A = pb.KORY;
  gc.chip.P13_D = pb.KALE;

  gc.chip.P14_A = pb.KARU;
  gc.chip.P14_D = !pb.KELY_04; // this seems really weird

  gc.chip.P15_A = pb.CELA;
  gc.chip.P15_D = pb.COFY_05;

  //----------

  pc.BYZO    = not(gb.FF00RD);
  pc.KEVU_00 = latch_pos(pb.BYZO, pb.KEVU_00, gb.chip.P10_C);
  pc.KAPA_01 = latch_pos(pb.BYZO, pb.KAPA_01, gb.chip.P11_C);
  pc.KEJA_02 = latch_pos(pb.BYZO, pb.KEJA_02, gb.chip.P12_C);
  pc.KOLO_03 = latch_pos(pb.BYZO, pb.KOLO_03, gb.chip.P13_C);

  pc.KEMA_00 = pb.KEVU_00;
  pc.KURO_01 = pb.KAPA_01;
  pc.KUVE_02 = pb.KEJA_02;
  pc.JEKU_03 = pb.KOLO_03;
  pc.KOCE_04 = not(!pb.KELY_04);
  pc.CUDY_05 = not(!pb.COFY_05);

  if (pb.BYZO) {
    gc.cpu.D0 = pb.KEMA_00;
    gc.cpu.D1 = pb.KURO_01;
    gc.cpu.D2 = pb.KUVE_02;
    gc.cpu.D3 = pb.JEKU_03;
    gc.cpu.D4 = pb.KOCE_04;
    gc.cpu.D5 = pb.CUDY_05;
  }

  //----------

  pc.AXYN = not(gb.BEDO);
  pc.ADYR = not(pb.AXYN);
  pc.APYS = nor(gb.T1nT2, pb.ADYR);
  pc.AFOP = not(pb.APYS);

  pc.ANOC_00 = not(gb.chip.P10_B);
  pc.ATAJ_01 = not(gb.chip.P10_B);
  pc.AJEC_02 = not(gb.chip.P10_B);
  pc.ASUZ_03 = not(gb.chip.P10_B);
  pc.BENU_04 = not(gb.chip.P10_B);
  pc.AKAJ_05 = not(gb.chip.P10_B);
  pc.ARAR_06 = not(gb.chip.P10_B);
  pc.BEDA_07 = not(gb.chip.P10_B);

  if (pb.AFOP) {
    gc.cpu.D0 = pb.ANOC_00;
    gc.cpu.D1 = pb.ATAJ_01;
    gc.cpu.D2 = pb.AJEC_02;
    gc.cpu.D3 = pb.ASUZ_03;
    gc.cpu.D4 = pb.BENU_04;
    gc.cpu.D5 = pb.AKAJ_05;
    gc.cpu.D6 = pb.ARAR_06;
    gc.cpu.D7 = pb.BEDA_07;
  }
}