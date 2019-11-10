#include "P04_DMA.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P04_DMA::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p04.DECY = not(b.cpu.FROM_CPU5);
  c.p04.CATY = not(b.p04.DECY);
  c.p04.MAKA = tock_pos(a.p01.CLK1, b.p01.CLK1, b.p01.RESET6, b.p04.MAKA, b.p04.CATY);
  c.p04.NAXY = nor(b.p04.MAKA, b.p04.LUVY);
  c.p04.POWU = and(b.p04.MATU, b.p04.NAXY);
  c.p04.WYJA = unk3(b.p28.AMAB, b.p07.CPU_WR2, b.p04.POWU);
  c.p04.LUVY = tock_pos(a.p01.PHI_OUTn, b.p01.PHI_OUTn, b.p01.RESET6, b.p04.LUVY, b.p04.LUPA);

  c.p04.MOPA = not(b.p01.PHI_OUTn);
  c.p04.NAVO = nand(b.p04.DMA_A00, b.p04.DMA_A01, b.p04.DMA_A02, b.p04.DMA_A03, b.p04.DMA_A04, b.p04.DMA_A07);
  c.p04.NOLO = not(b.p04.NAVO);
  c.p04.MYTE = tock_pos(a.p04.MOPA, b.p04.MOPA, b.p04.LAPA,   b.p04.MYTE, b.p04.NOLO);
  c.p04.LENE = tock_pos(a.p04.MOPA, b.p04.MOPA, b.p01.RESET6, b.p04.LENE, b.p04.LUVY);
  c.p04.LARA = nand(b.p04.LOKY, !b.p04.MYTE, b.p01.RESET6);
  c.p04.LOKY = nand(b.p04.LARA, !b.p04.LENE);
  c.p04.MATU = tock_pos(a.p01.PHI_OUTn, b.p01.PHI_OUTn, b.p01.RESET6, b.p04.MATU, b.p04.LOKY);
  c.p04.MORY = nand(b.p04.MATU, b.p04.LOGO);
  c.p04.LUMA = not(b.p04.MORY);
  c.p04.LOGO = not(b.p04.MUDA);
  c.p04.LEBU = not(b.p04.DMA_A15);
  c.p04.MUDA = nor(b.p04.DMA_A13, b.p04.DMA_A14, b.p04.LEBU);
  c.p04.MUHO = nand(b.p04.MATU, b.p04.MUDA);
  c.p04.DUGA = not(b.p04.MATU);
  c.p04.LUFA = not(b.p04.MUHO);

  c.p04.LYXE = unk2(b.p04.LORU, b.p04.LOKO);
  c.p04.LUPA = nor(b.p04.LAVY, b.p04.LYXE);
  c.p04.MOLU = nand(b.p22.FF46, b.p07.CPU_RD2);
  c.p04.LAVY = nand(b.p22.FF46, b.p07.CPU_WR2);
  c.p04.NYGO = not(b.p04.MOLU);
  c.p04.LORU = not(b.p04.LAVY);
  c.p04.PUSY = not(b.p04.NYGO);

  c.p04.AHOC = not(b.p04.VRAM_TO_OAM);
  c.p04.LOKO = nand(b.p01.RESET6, !b.p04.LENE);
  c.p04.LAPA = not(b.p04.LOKO);
  c.p04.META = and(b.p01.PHI_OUTn, b.p04.LOKY);

  //----------
  // FF46 DMA

  c.p04.NAFA_00 = tock_pos(a.p04.LORU, b.p04.LORU, 0, b.p04.NAFA_00, b.D0);
  c.p04.PYNE_01 = tock_pos(a.p04.LORU, b.p04.LORU, 0, b.p04.PYNE_01, b.D1);
  c.p04.PARA_02 = tock_pos(a.p04.LORU, b.p04.LORU, 0, b.p04.PARA_02, b.D2);
  c.p04.NYDO_03 = tock_pos(a.p04.LORU, b.p04.LORU, 0, b.p04.NYDO_03, b.D3);
  c.p04.NYGY_04 = tock_pos(a.p04.LORU, b.p04.LORU, 0, b.p04.NYGY_04, b.D4);
  c.p04.PULA_05 = tock_pos(a.p04.LORU, b.p04.LORU, 0, b.p04.PULA_05, b.D5);
  c.p04.POKU_06 = tock_pos(a.p04.LORU, b.p04.LORU, 0, b.p04.POKU_06, b.D6);
  c.p04.MARU_07 = tock_pos(a.p04.LORU, b.p04.LORU, 0, b.p04.MARU_07, b.D7);

  c.p04.POLY_00 = not(!b.p04.NAFA_00);
  c.p04.ROFO_01 = not(!b.p04.PYNE_01);
  c.p04.REMA_02 = not(!b.p04.PARA_02);
  c.p04.PANE_03 = not(!b.p04.NYDO_03);
  c.p04.PARE_04 = not(!b.p04.NYGY_04);
  c.p04.RALY_05 = not(!b.p04.PULA_05);
  c.p04.RESU_06 = not(!b.p04.POKU_06);
  c.p04.NUVY_07 = not(!b.p04.MARU_07);

  // there's an inverting tribuf on the schematic, but this can't be inverted...
  c.p04.EVAX_08 = b.p04.NAFA_00;
  c.p04.DUVE_09 = b.p04.PYNE_01;
  c.p04.ERAF_10 = b.p04.PARA_02;
  c.p04.FUSY_11 = b.p04.NYDO_03;
  c.p04.EXYF_12 = b.p04.NYGY_04;

  if (b.p04.PUSY) {
    c.D0 = b.p04.POLY_00;
    c.D1 = b.p04.ROFO_01;
    c.D2 = b.p04.REMA_02;
    c.D3 = b.p04.PANE_03;
    c.D4 = b.p04.PARE_04;
    c.D5 = b.p04.RALY_05;
    c.D6 = b.p04.RESU_06;
    c.D7 = b.p04.NUVY_07;
  }

  //----------
  // DMA counter

  c.p04.NAKY_00 = tock_pos(a.p04.META,     b.p04.META,     b.p04.LAPA, b.p04.NAKY_00, !b.p04.NAKY_00);
  c.p04.PYRO_01 = tock_pos(!a.p04.NAKY_00, !b.p04.NAKY_00, b.p04.LAPA, b.p04.PYRO_01, !b.p04.PYRO_01);
  c.p04.NEFY_02 = tock_pos(!a.p04.PYRO_01, !b.p04.PYRO_01, b.p04.LAPA, b.p04.NEFY_02, !b.p04.NEFY_02);
  c.p04.MUTY_03 = tock_pos(!a.p04.NEFY_02, !b.p04.NEFY_02, b.p04.LAPA, b.p04.MUTY_03, !b.p04.MUTY_03);
  c.p04.NYKO_04 = tock_pos(!a.p04.MUTY_03, !b.p04.MUTY_03, b.p04.LAPA, b.p04.NYKO_04, !b.p04.NYKO_04);
  c.p04.PYLO_05 = tock_pos(!a.p04.NYKO_04, !b.p04.NYKO_04, b.p04.LAPA, b.p04.PYLO_05, !b.p04.PYLO_05);
  c.p04.NUTO_06 = tock_pos(!a.p04.PYLO_05, !b.p04.PYLO_05, b.p04.LAPA, b.p04.NUTO_06, !b.p04.NUTO_06);
  c.p04.MUGU_07 = tock_pos(!a.p04.NUTO_06, !b.p04.NUTO_06, b.p04.LAPA, b.p04.MUGU_07, !b.p04.MUGU_07);

  // this tribuffer is _not_ inverting
  c.p04.ECAL_00 = b.p04.NAKY_00;
  c.p04.EGEZ_01 = b.p04.PYRO_01;
  c.p04.FUHE_02 = b.p04.NEFY_02;
  c.p04.FYZY_03 = b.p04.MUTY_03;
  c.p04.DAMU_04 = b.p04.NYKO_04;
  c.p04.DAVA_05 = b.p04.PYLO_05;
  c.p04.ETEG_06 = b.p04.NUTO_06;
  c.p04.EREW_07 = b.p04.MUGU_07;

  if (b.p04.AHOC) {
    c.chip.MA00 = b.p04.ECAL_00;
    c.chip.MA01 = b.p04.EGEZ_01;
    c.chip.MA02 = b.p04.FUHE_02;
    c.chip.MA03 = b.p04.FYZY_03;
    c.chip.MA04 = b.p04.DAMU_04;
    c.chip.MA05 = b.p04.DAVA_05;
    c.chip.MA06 = b.p04.ETEG_06;
    c.chip.MA07 = b.p04.EREW_07;
    c.chip.MA08 = b.p04.EVAX_08;
    c.chip.MA09 = b.p04.DUVE_09;
    c.chip.MA10 = b.p04.ERAF_10;
    c.chip.MA11 = b.p04.FUSY_11;
    c.chip.MA12 = b.p04.EXYF_12;
  }
}
