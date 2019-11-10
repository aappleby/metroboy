#include "P04_DMA.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P04_DMA::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P04_DMA& pa = ga.p04;
  const P04_DMA& pb = gb.p04;
  P04_DMA& pc = gc.p04;

  pc.DECY = not(gb.cpu.FROM_CPU5);
  pc.CATY = not(pb.DECY);
  pc.MAKA = tock_pos(ga.p01.CLK1, gb.p01.CLK1, gb.p01.RESET6, pb.MAKA, pb.CATY);
  pc.NAXY = nor(pb.MAKA, pb.LUVY);
  pc.POWU = and(pb.MATU, pb.NAXY);
  pc.WYJA = unk3(gb.p28.AMAB, gb.p07.CPU_WR2, pb.POWU);
  pc.LUVY = tock_pos(ga.p01.PHI_OUTn, gb.p01.PHI_OUTn, gb.p01.RESET6, pb.LUVY, pb.LUPA);

  pc.MOPA = not(gb.p01.PHI_OUTn);
  pc.NAVO = nand(pb.DMA_A00, pb.DMA_A01, pb.DMA_A02, pb.DMA_A03, pb.DMA_A04, pb.DMA_A07);
  pc.NOLO = not(pb.NAVO);
  pc.MYTE = tock_pos(pa.MOPA, pb.MOPA, pb.LAPA,   pb.MYTE, pb.NOLO);
  pc.LENE = tock_pos(pa.MOPA, pb.MOPA, gb.p01.RESET6, pb.LENE, pb.LUVY);
  pc.LARA = nand(pb.LOKY, !pb.MYTE, gb.p01.RESET6);
  pc.LOKY = nand(pb.LARA, !pb.LENE);
  pc.MATU = tock_pos(ga.p01.PHI_OUTn, gb.p01.PHI_OUTn, gb.p01.RESET6, pb.MATU, pb.LOKY);
  pc.MORY = nand(pb.MATU, pb.LOGO);
  pc.LUMA = not(pb.MORY);
  pc.LOGO = not(pb.MUDA);
  pc.LEBU = not(pb.DMA_A15);
  pc.MUDA = nor(pb.DMA_A13, pb.DMA_A14, pb.LEBU);
  pc.MUHO = nand(pb.MATU, pb.MUDA);
  pc.DUGA = not(pb.MATU);
  pc.LUFA = not(pb.MUHO);

  pc.LYXE = unk2(pb.LORU, pb.LOKO);
  pc.LUPA = nor(pb.LAVY, pb.LYXE);
  pc.MOLU = nand(gb.p22.FF46, gb.p07.CPU_RD2);
  pc.LAVY = nand(gb.p22.FF46, gb.p07.CPU_WR2);
  pc.NYGO = not(pb.MOLU);
  pc.LORU = not(pb.LAVY);
  pc.PUSY = not(pb.NYGO);

  pc.AHOC = not(pb.VRAM_TO_OAM);
  pc.LOKO = nand(gb.p01.RESET6, !pb.LENE);
  pc.LAPA = not(pb.LOKO);
  pc.META = and(gb.p01.PHI_OUTn, pb.LOKY);

  //----------
  // FF46 DMA

  pc.NAFA_00 = tock_pos(pa.LORU, pb.LORU, 0, pb.NAFA_00, gb.D0);
  pc.PYNE_01 = tock_pos(pa.LORU, pb.LORU, 0, pb.PYNE_01, gb.D1);
  pc.PARA_02 = tock_pos(pa.LORU, pb.LORU, 0, pb.PARA_02, gb.D2);
  pc.NYDO_03 = tock_pos(pa.LORU, pb.LORU, 0, pb.NYDO_03, gb.D3);
  pc.NYGY_04 = tock_pos(pa.LORU, pb.LORU, 0, pb.NYGY_04, gb.D4);
  pc.PULA_05 = tock_pos(pa.LORU, pb.LORU, 0, pb.PULA_05, gb.D5);
  pc.POKU_06 = tock_pos(pa.LORU, pb.LORU, 0, pb.POKU_06, gb.D6);
  pc.MARU_07 = tock_pos(pa.LORU, pb.LORU, 0, pb.MARU_07, gb.D7);

  pc.POLY_00 = not(!pb.NAFA_00);
  pc.ROFO_01 = not(!pb.PYNE_01);
  pc.REMA_02 = not(!pb.PARA_02);
  pc.PANE_03 = not(!pb.NYDO_03);
  pc.PARE_04 = not(!pb.NYGY_04);
  pc.RALY_05 = not(!pb.PULA_05);
  pc.RESU_06 = not(!pb.POKU_06);
  pc.NUVY_07 = not(!pb.MARU_07);

  pc.EVAX_08 = pb.NAFA_00;
  pc.DUVE_09 = pb.PYNE_01;
  pc.ERAF_10 = pb.PARA_02;
  pc.FUSY_11 = pb.NYDO_03;
  pc.EXYF_12 = pb.NYGY_04;

  if (pb.PUSY) {
    gc.D0 = pb.POLY_00;
    gc.D1 = pb.ROFO_01;
    gc.D2 = pb.REMA_02;
    gc.D3 = pb.PANE_03;
    gc.D4 = pb.PARE_04;
    gc.D5 = pb.RALY_05;
    gc.D6 = pb.RESU_06;
    gc.D7 = pb.NUVY_07;
  }

  //----------
  // DMA counter

  pc.NAKY_00 = tock_pos(pa.META,     pb.META,     pb.LAPA, pb.NAKY_00, !pb.NAKY_00);
  pc.PYRO_01 = tock_pos(!pa.NAKY_00, !pb.NAKY_00, pb.LAPA, pb.PYRO_01, !pb.PYRO_01);
  pc.NEFY_02 = tock_pos(!pa.PYRO_01, !pb.PYRO_01, pb.LAPA, pb.NEFY_02, !pb.NEFY_02);
  pc.MUTY_03 = tock_pos(!pa.NEFY_02, !pb.NEFY_02, pb.LAPA, pb.MUTY_03, !pb.MUTY_03);
  pc.NYKO_04 = tock_pos(!pa.MUTY_03, !pb.MUTY_03, pb.LAPA, pb.NYKO_04, !pb.NYKO_04);
  pc.PYLO_05 = tock_pos(!pa.NYKO_04, !pb.NYKO_04, pb.LAPA, pb.PYLO_05, !pb.PYLO_05);
  pc.NUTO_06 = tock_pos(!pa.PYLO_05, !pb.PYLO_05, pb.LAPA, pb.NUTO_06, !pb.NUTO_06);
  pc.MUGU_07 = tock_pos(!pa.NUTO_06, !pb.NUTO_06, pb.LAPA, pb.MUGU_07, !pb.MUGU_07);

  // this tribuffer is _not_ inverting
  pc.ECAL_00 = pb.NAKY_00;
  pc.EGEZ_01 = pb.PYRO_01;
  pc.FUHE_02 = pb.NEFY_02;
  pc.FYZY_03 = pb.MUTY_03;
  pc.DAMU_04 = pb.NYKO_04;
  pc.DAVA_05 = pb.PYLO_05;
  pc.ETEG_06 = pb.NUTO_06;
  pc.EREW_07 = pb.MUGU_07;

  if (pb.AHOC) {
    gc.chip.MA00 = pb.ECAL_00;
    gc.chip.MA01 = pb.EGEZ_01;
    gc.chip.MA02 = pb.FUHE_02;
    gc.chip.MA03 = pb.FYZY_03;
    gc.chip.MA04 = pb.DAMU_04;
    gc.chip.MA05 = pb.DAVA_05;
    gc.chip.MA06 = pb.ETEG_06;
    gc.chip.MA07 = pb.EREW_07;
    gc.chip.MA08 = pb.EVAX_08;
    gc.chip.MA09 = pb.DUVE_09;
    gc.chip.MA10 = pb.ERAF_10;
    gc.chip.MA11 = pb.FUSY_11;
    gc.chip.MA12 = pb.EXYF_12;
  }
}
