#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct DmaRegisters {

  void tick(SchematicTop& gb);
  void tock(SchematicTop& gb);
  SignalHash commit();

private:
  friend struct SchematicTop;

  /*p04.LYXE*/ NorLatch LYXE_DMA_LATCHn;
  /*p04.MATU*/ Reg17 MATU_DMA_RUNNINGp; // 17-rung, bottom rung _must_ be DMA_RUNNINGp.
  /*p04.MYTE*/ Reg17 MYTE_DMA_DONE;
  /*p04.LUVY*/ Reg17 LUVY_DMA_TRIG_d0;
  /*p04.LENE*/ Reg17 LENE_DMA_TRIG_d4;

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