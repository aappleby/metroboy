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

void DmaBus::tick(OAM& oam, const Resets& rst, const AddressDecoder& dec, Vram& vram, MemBus& mem, const Clocks& clk) {
  bool MAKA_Q = MAKA.q();
  bool LUVY_Q = LUVY.q();
  bool DMA_DONE_Q = DMA_DONE.q();
  bool LENE_Q = LENE.q();
  bool MATU_Q = MATU.q();

  dma.CATY = cpu.FROM_CPU5;
  MAKA.tock(clk.CLK_1357, rst.RESET2, dma.CATY);

  dma.MOPA_PHI = not(clk.PHI_OUTn);

  // controls something in sprites?
  dma.WYJA = unk3(spr.AMAB, cpu.CPU_WR2, and(MATU_Q, nor(MAKA_Q, LUVY_Q)));

  // FF46 write
  bool NAFA_Q = NAFA.tock(and(dec.FF46, cpu.CPU_WR2), 0, mem.D0);
  bool NYGY_Q = NYGY.tock(and(dec.FF46, cpu.CPU_WR2), 0, mem.D4);
  bool PARA_Q = PARA.tock(and(dec.FF46, cpu.CPU_WR2), 0, mem.D2);
  bool PYNE_Q = PYNE.tock(and(dec.FF46, cpu.CPU_WR2), 0, mem.D1);
  bool PULA_Q = PULA.tock(and(dec.FF46, cpu.CPU_WR2), 0, mem.D5);
  bool NYDO_Q = NYDO.tock(and(dec.FF46, cpu.CPU_WR2), 0, mem.D3);
  bool POKU_Q = POKU.tock(and(dec.FF46, cpu.CPU_WR2), 0, mem.D6);
  bool MARU_Q = MARU.tock(and(dec.FF46, cpu.CPU_WR2), 0, mem.D7);

  dma.DMA_A8  = NAFA_Q;
  dma.DMA_A9  = PYNE_Q;
  dma.DMA_A10 = PARA_Q;
  dma.DMA_A11 = NYDO_Q;
  dma.DMA_A12 = NYGY_Q;
  dma.DMA_A13 = PULA_Q;
  dma.DMA_A14 = POKU_Q;
  dma.DMA_A15 = MARU_Q;

  // FIXME loopy thing, glitch filter? def broken.
  bool LARA = nand(/*LOKY,*/ !DMA_DONE_Q, rst.RESET2);
  bool LOKY = nand(LARA, not(LENE_Q));
  MATU.tock(clk.PHI_OUTn, rst.RESET2, LOKY);

  bool META = and(clk.PHI_OUTn, LOKY);
  bool NAKY_Q = NAKY.flip(META,    and(rst.RESET2, not(LENE_Q)));
  bool PYRO_Q = PYRO.flip(!NAKY_Q, and(rst.RESET2, not(LENE_Q)));
  bool NEFY_Q = NEFY.flip(!PYRO_Q, and(rst.RESET2, not(LENE_Q)));
  bool MUTY_Q = MUTY.flip(!NEFY_Q, and(rst.RESET2, not(LENE_Q)));
  bool NYKO_Q = NYKO.flip(!MUTY_Q, and(rst.RESET2, not(LENE_Q)));
  bool PYLO_Q = PYLO.flip(!NYKO_Q, and(rst.RESET2, not(LENE_Q)));
  bool NUTO_Q = NUTO.flip(!PYLO_Q, and(rst.RESET2, not(LENE_Q)));
  bool MUGU_Q = MUGU.flip(!NUTO_Q, and(rst.RESET2, not(LENE_Q)));

  dma.DMA_A0 = NAKY_Q;
  dma.DMA_A1 = PYRO_Q;
  dma.DMA_A2 = NEFY_Q;
  dma.DMA_A3 = MUTY_Q;
  dma.DMA_A4 = NYKO_Q;
  dma.DMA_A5 = PYLO_Q;
  dma.DMA_A6 = NUTO_Q;
  dma.DMA_A7 = MUGU_Q;

  oam.OAM_ADDR_DMA = not(MATU_Q);

  dma.LUMA = and(MATU_Q, or(dma.DMA_A13, dma.DMA_A14, not(dma.DMA_A15)));

  // FF46 read
  if (nand(dec.FF46, cpu.CPU_RD2)) {
    mem.D0 = NAFA_Q;
    mem.D4 = NYGY_Q;
    mem.D2 = PARA_Q;
    mem.D1 = PYNE_Q;
    mem.D5 = PULA_Q;
    mem.D3 = NYDO_Q;
    mem.D6 = POKU_Q;
    mem.D7 = MARU_Q;
  }

  // 0b100????????????? = 0x8000 to 0x9FFF
  dma.VRAM_TO_OAM = and(MATU_Q, nor(dma.DMA_A13, dma.DMA_A14, not(dma.DMA_A15)));
  if (not(dma.VRAM_TO_OAM)) {
    vram.MA0  = NAKY_Q;
    vram.MA1  = PYRO_Q;
    vram.MA2  = NEFY_Q;
    vram.MA3  = MUTY_Q;
    vram.MA4  = NYKO_Q;
    vram.MA5  = PYLO_Q;
    vram.MA6  = NUTO_Q;
    vram.MA7  = MUGU_Q;

    vram.MA8  = NAFA_Q;
    vram.MA9  = NYGY_Q;
    vram.MA10 = PARA_Q;
    vram.MA11 = PYNE_Q;
    vram.MA12 = NYDO_Q;
  }

  //----------
  // registers

  // so this probably resets the dma counter when there's a write to FF46
  bool LYXE = unk2(and(dec.FF46, cpu.CPU_WR2), nand(rst.RESET2, not(LENE_Q)));
  bool LUPA = nor(nand(dec.FF46, cpu.CPU_WR2), LYXE);
  LUVY.tock(clk.PHI_OUTn, rst.RESET2, LUPA);

  LENE.tock(not(clk.PHI_OUTn), rst.RESET2, LUVY_Q);
  

  // dma_a == 159
  bool NOLO = not(nand(dma.DMA_A0, dma.DMA_A1, dma.DMA_A2, dma.DMA_A3, dma.DMA_A4, dma.DMA_A7));
  DMA_DONE.tock(not(clk.PHI_OUTn),     and(rst.RESET2, not(LENE_Q)),   NOLO);
}