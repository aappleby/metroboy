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
  /*p04.MATU*/ Reg17 MATU_DMA_OAM_WRp; // 17-rung, bottom rung _must_ be DMA_RUNNINGp.
  /*p04.MYTE*/ Reg MYTE_DMA_DONE;
  /*p04.LUVY*/ Reg LUVY_DMA_TRIG_d0;
  /*p04.LENE*/ Reg LENE_DMA_TRIG_d4;

  /*p04.NAKY*/ Reg17 DMA_A00;
  /*p04.PYRO*/ Reg17 DMA_A01;
  /*p04.NEFY*/ Reg17 DMA_A02;
  /*p04.MUTY*/ Reg17 DMA_A03;
  /*p04.NYKO*/ Reg17 DMA_A04;
  /*p04.PYLO*/ Reg17 DMA_A05;
  /*p04.NUTO*/ Reg17 DMA_A06;
  /*p04.MUGU*/ Reg17 DMA_A07;

  /*p04.NAFA*/ Reg8 DMA_A08;
  /*p04.PYNE*/ Reg8 DMA_A09;
  /*p04.PARA*/ Reg8 DMA_A10;
  /*p04.NYDO*/ Reg8 DMA_A11;
  /*p04.NYGY*/ Reg8 DMA_A12;
  /*p04.PULA*/ Reg8 DMA_A13;
  /*p04.POKU*/ Reg8 DMA_A14;
  /*p04.MARU*/ Reg8 DMA_A15;

  // NAND latch
  /*p04.LARA*/ Gate LARA_DMA_LATCHn;
  /*p04.LOKY*/ Gate LOKY_DMA_LATCHp;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics