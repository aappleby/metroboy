#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct DmaSignals {
  /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn;

  // This seems wrong, like it should be DMA_READ_CART = and(DMA_RUNNING, !DMA_VRAM);
  /*p04.MORY*/ wire MORY_DMA_READ_CARTn;
  /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp;
  /*p04.LUFA*/ wire LUFA_DMA_READ_VRAMp;

  /*p04.MATU*/ wire MATU_DMA_OAM_WRp;

  /*p04.NAKY*/ wire DMA_A00;
  /*p04.PYRO*/ wire DMA_A01;
  /*p04.NEFY*/ wire DMA_A02;
  /*p04.MUTY*/ wire DMA_A03;
  /*p04.NYKO*/ wire DMA_A04;
  /*p04.PYLO*/ wire DMA_A05;
  /*p04.NUTO*/ wire DMA_A06;
  /*p04.MUGU*/ wire DMA_A07;
  /*p04.NAFA*/ wire DMA_A08;
  /*p04.PYNE*/ wire DMA_A09;
  /*p04.PARA*/ wire DMA_A10;
  /*p04.NYDO*/ wire DMA_A11;
  /*p04.NYGY*/ wire DMA_A12;
  /*p04.PULA*/ wire DMA_A13;
  /*p04.POKU*/ wire DMA_A14;
  /*p04.MARU*/ wire DMA_A15;
};

struct DmaRegisters {

  DmaSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- DMA REG -----\n");
    //FROM_CPU5_SYNC.dump(text_painter, "FROM_CPU5_SYNC   ");
    MATU_DMA_OAM_WRp.dump(text_painter, "DMA_RUNNING  ");
    MYTE_DMA_DONE.dump(text_painter, "MYTE_DMA_DONE    ");
    LUVY_DMA_TRIG_d0.dump(text_painter, "LUVY    ");
    LENE_DMA_TRIG_d4.dump(text_painter, "LENE    ");
    LYXE_DMA_LATCHn.dump(text_painter, "LYXE ");
    LOKY_DMA_LATCHp.dump(text_painter, "LOKY  ");
    text_painter.dprintf("DMA ADDR LO      0x%02x\n", get_addr_lo());
    text_painter.dprintf("DMA ADDR HI      0x%02x\n", get_addr_hi());
    text_painter.newline();
  }

  /*p04.LYXE*/ NorLatch LYXE_DMA_LATCHn;
  /*p04.MATU*/ Reg MATU_DMA_OAM_WRp; // 17-rung, bottom rung _must_ be DMA_RUNNINGp.
  /*p04.MYTE*/ Reg MYTE_DMA_DONE;
  /*p04.LUVY*/ Reg LUVY_DMA_TRIG_d0;
  /*p04.LENE*/ Reg LENE_DMA_TRIG_d4;

  int get_addr_lo() {
    return pack(DMA_A00.q(), DMA_A01.q(), DMA_A02.q(), DMA_A03.q(), DMA_A04.q(), DMA_A05.q(), DMA_A06.q(), DMA_A07.q());
  }
  int get_addr_hi() {
    return pack(DMA_A08.q(), DMA_A09.q(), DMA_A10.q(), DMA_A11.q(), DMA_A12.q(), DMA_A13.q(), DMA_A14.q(), DMA_A15.q());
  }

  /*p04.NAKY*/ Reg DMA_A00; // 17-rung
  /*p04.PYRO*/ Reg DMA_A01;
  /*p04.NEFY*/ Reg DMA_A02;
  /*p04.MUTY*/ Reg DMA_A03;
  /*p04.NYKO*/ Reg DMA_A04;
  /*p04.PYLO*/ Reg DMA_A05;
  /*p04.NUTO*/ Reg DMA_A06;
  /*p04.MUGU*/ Reg DMA_A07; // 17-rung
  /*p04.NAFA*/ Reg DMA_A08; // 9-rung, bottom rung missing?
  /*p04.PYNE*/ Reg DMA_A09; // 9-rung, bottom rung missing?
  /*p04.PARA*/ Reg DMA_A10; // 9-rung, bottom rung missing?
  /*p04.NYDO*/ Reg DMA_A11; // 9-rung 
  /*p04.NYGY*/ Reg DMA_A12; // 9-rung 
  /*p04.PULA*/ Reg DMA_A13; // 9-rung 
  /*p04.POKU*/ Reg DMA_A14; // 9-rung 
  /*p04.MARU*/ Reg DMA_A15; // 9-rung 

  // NAND latch
  /*p04.LARA*/ Gate LARA_DMA_LATCHn;
  /*p04.LOKY*/ Gate LOKY_DMA_LATCHp;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics