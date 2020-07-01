#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct DmaSignals {
  /*p28.BOGE*/ Signal BOGE_DMA_RUNNINGn;

  // This seems wrong, like it should be DMA_READ_CART = and(DMA_RUNNING, !DMA_VRAM);
  /*p04.MORY*/ Signal MORY_DMA_READ_CARTn;
  /*p04.LUMA*/ Signal LUMA_DMA_READ_CARTp;
  /*p25.CEDE*/ Signal CEDE_DMA_READ_CARTn;
  /*p04.LUFA*/ Signal LUFA_DMA_READ_VRAMp;

  /*p04.MATU*/ Signal MATU_DMA_OAM_WRp;

  /*p04.NAKY*/ Signal DMA_A00;
  /*p04.PYRO*/ Signal DMA_A01;
  /*p04.NEFY*/ Signal DMA_A02;
  /*p04.MUTY*/ Signal DMA_A03;
  /*p04.NYKO*/ Signal DMA_A04;
  /*p04.PYLO*/ Signal DMA_A05;
  /*p04.NUTO*/ Signal DMA_A06;
  /*p04.MUGU*/ Signal DMA_A07;
  /*p04.NAFA*/ Signal DMA_A08;
  /*p04.PYNE*/ Signal DMA_A09;
  /*p04.PARA*/ Signal DMA_A10;
  /*p04.NYDO*/ Signal DMA_A11;
  /*p04.NYGY*/ Signal DMA_A12;
  /*p04.PULA*/ Signal DMA_A13;
  /*p04.POKU*/ Signal DMA_A14;
  /*p04.MARU*/ Signal DMA_A15;
};

//-----------------------------------------------------------------------------

struct DmaRegisters {

  DmaSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

private:

  /*p04.LYXE*/ NorLatch LYXE_DMA_LATCHn;
  /*p04.MATU*/ Reg MATU_DMA_OAM_WRp; // 17-rung, bottom rung _must_ be DMA_RUNNINGp.
  /*p04.MYTE*/ Reg MYTE_DMA_DONE;
  /*p04.LUVY*/ Reg LUVY_DMA_TRIG_d0;
  /*p04.LENE*/ Reg LENE_DMA_TRIG_d4;

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