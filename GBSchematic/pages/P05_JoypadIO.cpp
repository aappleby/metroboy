#include "P05_JoypadIO.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P05_JoypadIO::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p05.JUTE_00 = tock_pos(a.p10.FF00WR, b.p10.FF00WR, b.p01.SYS_RESETn1, c.p05.JUTE_00, b.D0);
  c.p05.KECY_01 = tock_pos(a.p10.FF00WR, b.p10.FF00WR, b.p01.SYS_RESETn1, c.p05.KECY_01, b.D1);
  c.p05.JALE_02 = tock_pos(a.p10.FF00WR, b.p10.FF00WR, b.p01.SYS_RESETn1, c.p05.JALE_02, b.D2);
  c.p05.KYME_03 = tock_pos(a.p10.FF00WR, b.p10.FF00WR, b.p01.SYS_RESETn1, c.p05.KYME_03, b.D3);
  c.p05.KELY_04 = tock_pos(a.p10.FF00WR, b.p10.FF00WR, b.p01.SYS_RESETn1, c.p05.KELY_04, b.D4);
  c.p05.COFY_05 = tock_pos(a.p10.FF00WR, b.p10.FF00WR, b.p01.SYS_RESETn1, c.p05.COFY_05, b.D5);
  c.p05.KUKO_06 = tock_pos(a.p10.FF00WR, b.p10.FF00WR, b.p01.SYS_RESETn1, c.p05.KUKO_06, b.D6);
  c.p05.KERU_07 = tock_pos(a.p10.FF00WR, b.p10.FF00WR, b.p01.SYS_RESETn1, c.p05.KERU_07, b.D7);

  c.p05.KOLE = nand(b.p05.JUTE_00, b.p07.FF60_0);
  c.p05.KYBU = nor (b.p05.JUTE_00, b.p05.KURA);

  c.p05.KYTO = nand(b.p05.KECY_01, b.p07.FF60_0);
  c.p05.KABU = nor (b.p05.KECY_01, b.p05.KURA);

  c.p05.KYHU = nand(b.p05.JALE_02, b.p07.FF60_0);
  c.p05.KASY = nor (b.p07.FF60_0, b.p05.KURA); // this one doesn't match?

  c.p05.KORY = nand(b.p05.KYME_03, b.p07.FF60_0);
  c.p05.KALE = nor (b.p05.KYME_03, b.p05.KURA);

  c.p05.KARU = or(!b.p05.KELY_04, b.p05.KURA);

  c.p05.KURA = not(b.p07.FF60_0);
  c.p05.CELA = or(!b.p05.COFY_05, b.p05.KURA);

  c.p05.KENA = mux2(b.p05.KUKO_06, b.p06.SER_OUT, b.p07.FF60_0);

  c.p05.JEVA = not(b.p07.FF60_0);
  c.p05.KORE = nand(b.p05.KERU_07, b.p07.FF60_0);
  c.p05.KYWE = nor (b.p05.KERU_07, b.p05.JEVA);

  // FIXME really unsure about these pin assignments, seem to have a few missing signals
  c.chip.SOUT  = b.p05.KENA;

  c.chip.P10_A = b.p05.KOLE;
  c.chip.P10_D = b.p05.KYBU;

  c.chip.P11_A = b.p05.KYTO;
  c.chip.P11_D = b.p05.KABU;

  c.chip.P12_A = b.p05.KYHU;
  c.chip.P12_D = b.p05.KASY;

  c.chip.P13_A = b.p05.KORY;
  c.chip.P13_D = b.p05.KALE;

  c.chip.P14_A = b.p05.KARU;
  c.chip.P14_D = !b.p05.KELY_04; // this seems really weird

  c.chip.P15_A = b.p05.CELA;
  c.chip.P15_D = b.p05.COFY_05;

  //----------

  c.p05.BYZO    = not(b.p10.FF00RD);

  c.p05.KEVU_00 = latch_pos(b.p05.BYZO, b.p05.KEVU_00, b.chip.P10_C);
  c.p05.KAPA_01 = latch_pos(b.p05.BYZO, b.p05.KAPA_01, b.chip.P11_C);
  c.p05.KEJA_02 = latch_pos(b.p05.BYZO, b.p05.KEJA_02, b.chip.P12_C);
  c.p05.KOLO_03 = latch_pos(b.p05.BYZO, b.p05.KOLO_03, b.chip.P13_C);

  // inverting tribuf on schematic, but this can't be inverting
  c.p05.KEMA_00 = b.p05.KEVU_00;
  c.p05.KURO_01 = b.p05.KAPA_01;
  c.p05.KUVE_02 = b.p05.KEJA_02;
  c.p05.JEKU_03 = b.p05.KOLO_03;

  c.p05.KOCE_04 = not(!b.p05.KELY_04);
  c.p05.CUDY_05 = not(!b.p05.COFY_05);

  if (b.p05.BYZO) {
    c.D0 = b.p05.KEMA_00;
    c.D1 = b.p05.KURO_01;
    c.D2 = b.p05.KUVE_02;
    c.D3 = b.p05.JEKU_03;
    c.D4 = b.p05.KOCE_04;
    c.D5 = b.p05.CUDY_05;
  }

  //----------
  // weird debug thing, probably not right

  c.p05.AXYN = not(b.p01.CPUCLK_xxxxxFGH2);
  c.p05.ADYR = not(b.p05.AXYN);
  c.p05.APYS = nor(b.p07.MODE_DBG2, b.p05.ADYR);
  c.p05.AFOP = not(b.p05.APYS);

  c.p05.ANOC_00 = not(b.chip.P10_B);
  c.p05.ATAJ_01 = not(b.chip.P10_B);
  c.p05.AJEC_02 = not(b.chip.P10_B);
  c.p05.ASUZ_03 = not(b.chip.P10_B);
  c.p05.BENU_04 = not(b.chip.P10_B);
  c.p05.AKAJ_05 = not(b.chip.P10_B);
  c.p05.ARAR_06 = not(b.chip.P10_B);
  c.p05.BEDA_07 = not(b.chip.P10_B);

  if (b.p05.AFOP) {
    c.D0 = b.p05.ANOC_00;
    c.D1 = b.p05.ATAJ_01;
    c.D2 = b.p05.AJEC_02;
    c.D3 = b.p05.ASUZ_03;
    c.D4 = b.p05.BENU_04;
    c.D5 = b.p05.AKAJ_05;
    c.D6 = b.p05.ARAR_06;
    c.D7 = b.p05.BEDA_07;
  }
}