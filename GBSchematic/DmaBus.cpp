#include "DmaBus.h"

#include "Schematics.h"
#include "MemBus.h"
#include "VramBus.h"
#include "DmaBus.h"
#include "OAM.h"
#include "CpuBus.h"
#include "AddressDecoder.h"
#include "Clocks.h"
#include "Sprites.h"

DmaBus dma;

//-----------------------------------------------------------------------------
// 4_DMA.png

void DmaBus::tick(OAM& oam, const Resets& rst) {
  bool MAKA_Q = MAKA.q();
  bool LUVY_Q = LUVY.q();
  bool MYTE_Q = MYTE.q();
  bool LENE_Q = LENE.q();
  bool MATU_Q = MATU.q();

  bool DECY = not(cpu.FROM_CPU5);
  dma.CATY = not(DECY);
  bool NAXY = nor(MAKA_Q, LUVY_Q);

  bool MOPA = not(clk.PHI_OUTn);
  dma.MOPA_PHI = MOPA;

  bool LENE_Qn = not(LENE_Q);
  bool LOKO = nand(rst.RESET6, LENE_Qn);

  bool MOLU = nand(dec.FF46, cpu.CPU_RD2);
  bool LAVY = nand(dec.FF46, cpu.CPU_WR2);
  bool NYGO = not(MOLU);
  bool LORU = not(LAVY);

  bool LYXE = unk2(LORU, LOKO);

  bool LUPA = nor(LAVY, LYXE);

  bool POWU = and(MATU_Q, NAXY);

  // FIXME loopy thing, glitch filter? def broken.
  bool LARA = nand(/*LOKY,*/ !MYTE_Q, rst.RESET6);
  bool LOKY = nand(LARA, LENE_Qn);

  // dma_a == 159
  bool NAVO = nand(nand(dma.DMA_A0, dma.DMA_A1, dma.DMA_A2), nand(dma.DMA_A3, dma.DMA_A4, dma.DMA_A7));

  bool NOLO = not(NAVO);

  dma.WYJA = unk3(spr.AMAB, cpu.CPU_WR2, POWU);

  bool NAFA_Q = NAFA.tock(LORU, 0, mem.D0);
  bool NYGY_Q = NYGY.tock(LORU, 0, mem.D4);
  bool PARA_Q = PARA.tock(LORU, 0, mem.D2);
  bool PYNE_Q = PYNE.tock(LORU, 0, mem.D1);
  bool PULA_Q = PULA.tock(LORU, 0, mem.D5);
  bool NYDO_Q = NYDO.tock(LORU, 0, mem.D3);
  bool POKU_Q = POKU.tock(LORU, 0, mem.D6);
  bool MARU_Q = MARU.tock(LORU, 0, mem.D7);

  bool POLY = not(!NAFA_Q);
  bool PARE = not(!NYGY_Q);
  bool REMA = not(!PARA_Q);
  bool ROFO = not(!PYNE_Q);
  bool RALY = not(!PULA_Q);
  bool PANE = not(!NYDO_Q);
  bool RESU = not(!POKU_Q);
  bool NUVY = not(!MARU_Q);

  dma.DMA_A8  = NAFA_Q;
  dma.DMA_A12 = NYGY_Q;
  dma.DMA_A10 = PARA_Q;
  dma.DMA_A9  = PYNE_Q;
  dma.DMA_A13 = PULA_Q;
  dma.DMA_A11 = NYDO_Q;
  dma.DMA_A14 = POKU_Q;
  dma.DMA_A15 = MARU_Q;

  bool EVAX = not(NAFA_Q);
  bool EXYF = not(NYGY_Q);
  bool ERAF = not(PARA_Q);
  bool DUVE = not(PYNE_Q);
  bool FUSY = not(NYDO_Q);


  bool LAPA = not(LOKO);
  bool META = and(clk.PHI_OUTn, LOKY);

  bool NAKY_Q = NAKY.flip(META,    LAPA);
  bool PYRO_Q = PYRO.flip(!NAKY_Q, LAPA);
  bool NEFY_Q = NEFY.flip(!PYRO_Q, LAPA);
  bool MUTY_Q = MUTY.flip(!NEFY_Q, LAPA);
  bool NYKO_Q = NYKO.flip(!MUTY_Q, LAPA);
  bool PYLO_Q = PYLO.flip(!NYKO_Q, LAPA);
  bool NUTO_Q = NUTO.flip(!PYLO_Q, LAPA);
  bool MUGU_Q = MUGU.flip(!NUTO_Q, LAPA);

  dma.DMA_A0 = NAKY_Q;
  dma.DMA_A1 = PYRO_Q;
  dma.DMA_A2 = NEFY_Q;
  dma.DMA_A3 = MUTY_Q;
  dma.DMA_A4 = NYKO_Q;
  dma.DMA_A5 = PYLO_Q;
  dma.DMA_A6 = NUTO_Q;
  dma.DMA_A7 = MUGU_Q;

  // tribuffer, not inverter? FIXME - check this elsewhere, we could be driving inverted signals onto the tribus
  bool ECAL = NAKY_Q;
  bool EGEZ = PYRO_Q;
  bool FUHE = NEFY_Q;
  bool FYZY = MUTY_Q;
  bool DAMU = NYKO_Q;
  bool DAVA = PYLO_Q;
  bool ETEG = NUTO_Q;
  bool EREW = MUGU_Q;

  bool LEBU = not(dma.DMA_A15);
  bool MUDA = nor(dma.DMA_A13, dma.DMA_A14, LEBU);
  bool LOGO = not(MUDA);
  bool MUHO = nand(MATU_Q, MUDA);
  bool MORY = nand(MATU_Q, LOGO);
  bool DUGA = not(MATU_Q);
  bool LUFA = not(MUHO);
  dma.LUMA = not(MORY);
  oam.OAM_ADDR_DMA = DUGA;
  dma.VRAM_TO_OAM = LUFA;

  bool PUSY = not(NYGO);
  if (PUSY) {
    mem.D0 = POLY;
    mem.D4 = PARE;
    mem.D2 = REMA;
    mem.D1 = ROFO;
    mem.D5 = RALY;
    mem.D3 = PANE;
    mem.D6 = RESU;
    mem.D7 = NUVY;
  }

  bool AHOC = not(dma.VRAM_TO_OAM);
  if (AHOC) {
    vram.MA0 = ECAL;
    vram.MA1 = EGEZ;
    vram.MA2 = FUHE;
    vram.MA3 = FYZY;
    vram.MA4 = DAMU;
    vram.MA5 = DAVA;
    vram.MA6 = ETEG;
    vram.MA7 = EREW;
    vram.MA8 = EVAX;
    vram.MA9 = DUVE;
    vram.MA10 = ERAF;
    vram.MA11 = FUSY;
    vram.MA12 = EXYF;
  }

  //----------
  // registers

  MAKA.tock(clk.CLK1,     rst.RESET6, dma.CATY);
  LUVY.tock(clk.PHI_OUTn, rst.RESET6, LUPA);
  LENE.tock(MOPA,     rst.RESET6, LUVY_Q);
  MATU.tock(clk.PHI_OUTn, rst.RESET6, LOKY);
  MYTE.tock(MOPA,     LAPA,   NOLO);
}