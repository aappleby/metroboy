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
  pc.MAKA = tock_pos(ga.CLK1, gb.CLK1, gb.RESET6, pb.MAKA, pb.CATY);
  pc.NAXY = nor(pb.MAKA, pb.LUVY);
  pc.POWU = and(pb.MATU, pb.NAXY);
  pc.WYJA = unk3(gb.AMAB, gb.CPU_WR2, pb.POWU);
  pc.LUVY = tock_pos(ga.PHI_OUTn, gb.PHI_OUTn, gb.RESET6, pb.LUVY, pb.LUPA);

  pc.MOPA = not(gb.PHI_OUTn);
  pc.NAVO = nand(gb.DMA_A00, gb.DMA_A01, gb.DMA_A02, gb.DMA_A03, gb.DMA_A04, gb.DMA_A07);
  pc.NOLO = not(pb.NAVO);
  pc.MYTE = tock_pos(pa.MOPA, pb.MOPA, pb.LAPA,   pb.MYTE, pb.NOLO);
  pc.LENE = tock_pos(pa.MOPA, pb.MOPA, gb.RESET6, pb.LENE, pb.LUVY);
  pc.LARA = nand(pb.LOKY, !pb.MYTE, gb.RESET6);
  pc.LOKY = nand(pb.LARA, !pb.LENE);
  pc.MATU = tock_pos(ga.PHI_OUTn, gb.PHI_OUTn, gb.RESET6, pb.MATU, pb.LOKY);
  pc.MORY = nand(pb.MATU, pb.LOGO);
  pc.LUMA = not(pb.MORY);
  pc.LOGO = not(pb.MUDA);
  pc.LEBU = not(gb.DMA_A15);
  pc.MUDA = nor(gb.DMA_A13, gb.DMA_A14, pb.LEBU);
  pc.MUHO = nand(pb.MATU, pb.MUDA);
  pc.DUGA = not(pb.MATU);
  pc.LUFA = not(pb.MUHO);

  pc.LYXE = unk2(pb.LORU, pb.LOKO);
  pc.LUPA = nor(pb.LAVY, pb.LYXE);
  pc.MOLU = nand(gb.FF46, gb.CPU_RD2);
  pc.LAVY = nand(gb.FF46, gb.CPU_WR2);
  pc.NYGO = not(pb.MOLU);
  pc.LORU = not(pb.LAVY);
  pc.PUSY = not(pb.NYGO);

  pc.AHOC = not(gb.VRAM_TO_OAM);
  pc.LOKO = nand(gb.RESET6, !pb.LENE);
  pc.LAPA = not(pb.LOKO);
  pc.META = and(gb.PHI_OUTn, pb.LOKY);

  gc.CATY         = pb.CATY;
  gc.WYJA         = pb.WYJA;
  gc.MOPA_PHI     = pb.MOPA;
  gc.LUMA         = pb.LUMA;
  gc.OAM_ADDR_DMA = pb.DUGA;
  gc.VRAM_TO_OAM  = pb.LUFA;

  //----------
  // FF46 DMA

  pc.NAFA_00 = tock_pos(pa.LORU, pb.LORU, 0, pb.NAFA_00, gb.cpu.D0);
  pc.PYNE_01 = tock_pos(pa.LORU, pb.LORU, 0, pb.PYNE_01, gb.cpu.D1);
  pc.PARA_02 = tock_pos(pa.LORU, pb.LORU, 0, pb.PARA_02, gb.cpu.D2);
  pc.NYDO_03 = tock_pos(pa.LORU, pb.LORU, 0, pb.NYDO_03, gb.cpu.D3);
  pc.NYGY_04 = tock_pos(pa.LORU, pb.LORU, 0, pb.NYGY_04, gb.cpu.D4);
  pc.PULA_05 = tock_pos(pa.LORU, pb.LORU, 0, pb.PULA_05, gb.cpu.D5);
  pc.POKU_06 = tock_pos(pa.LORU, pb.LORU, 0, pb.POKU_06, gb.cpu.D6);
  pc.MARU_07 = tock_pos(pa.LORU, pb.LORU, 0, pb.MARU_07, gb.cpu.D7);

  gc.DMA_A08 = pb.NAFA_00;
  gc.DMA_A09 = pb.PYNE_01;
  gc.DMA_A10 = pb.PARA_02;
  gc.DMA_A11 = pb.NYDO_03;
  gc.DMA_A12 = pb.NYGY_04;
  gc.DMA_A13 = pb.PULA_05;
  gc.DMA_A14 = pb.POKU_06;
  gc.DMA_A15 = pb.MARU_07;

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
    gc.cpu.D0 = pb.POLY_00;
    gc.cpu.D1 = pb.ROFO_01;
    gc.cpu.D2 = pb.REMA_02;
    gc.cpu.D3 = pb.PANE_03;
    gc.cpu.D4 = pb.PARE_04;
    gc.cpu.D5 = pb.RALY_05;
    gc.cpu.D6 = pb.RESU_06;
    gc.cpu.D7 = pb.NUVY_07;
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

  gc.DMA_A00 = pb.NAKY_00;
  gc.DMA_A01 = pb.PYRO_01;
  gc.DMA_A02 = pb.NEFY_02;
  gc.DMA_A03 = pb.MUTY_03;
  gc.DMA_A04 = pb.NYKO_04;
  gc.DMA_A05 = pb.PYLO_05;
  gc.DMA_A06 = pb.NUTO_06;
  gc.DMA_A07 = pb.MUGU_07;

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
    gc.chip.MA0  = pb.ECAL_00;
    gc.chip.MA1  = pb.EGEZ_01;
    gc.chip.MA2  = pb.FUHE_02;
    gc.chip.MA3  = pb.FYZY_03;
    gc.chip.MA4  = pb.DAMU_04;
    gc.chip.MA5  = pb.DAVA_05;
    gc.chip.MA6  = pb.ETEG_06;
    gc.chip.MA7  = pb.EREW_07;
    gc.chip.MA8  = pb.EVAX_08;
    gc.chip.MA9  = pb.DUVE_09;
    gc.chip.MA10 = pb.ERAF_10;
    gc.chip.MA11 = pb.FUSY_11;
    gc.chip.MA12 = pb.EXYF_12;
  }
}
