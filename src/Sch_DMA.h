#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;
struct VramBus;

//-----------------------------------------------------------------------------

struct DmaRegisters {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  SignalHash commit();

  // -> bunch of stuff
  /*p04.MATU*/ wire MATU_DMA_RUNNINGp() const { return _MATU_DMA_RUNNINGp.q(); }

  // -> ext bus, oam bus
  /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp() const {
    /*p04.LOGO*/ wire _LOGO_DMA_VRAMn      = not(MUDA_DMA_SRC_VRAMp());
    /*p04.MORY*/ wire _MORY_DMA_READ_CARTn = nand(MATU_DMA_RUNNINGp(), _LOGO_DMA_VRAMn);
    return not(_MORY_DMA_READ_CARTn);
  }

  // -> top.ACYL, top.AJON
  /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn() const { return not(MATU_DMA_RUNNINGp()); }

  // -> oam bus, vram bus
  /*p04.LUFA*/ wire LUFA_DMA_VRM_RDp() const {
    /*p04.MUHO*/ wire MUHO_DMA_VRAM_RDn = nand(MATU_DMA_RUNNINGp(), MUDA_DMA_SRC_VRAMp());
    return not(MUHO_DMA_VRAM_RDn);
  }

  /*p04.NAKY*/ Reg DMA_A00;
  /*p04.PYRO*/ Reg DMA_A01;
  /*p04.NEFY*/ Reg DMA_A02;
  /*p04.MUTY*/ Reg DMA_A03;
  /*p04.NYKO*/ Reg DMA_A04;
  /*p04.PYLO*/ Reg DMA_A05;
  /*p04.NUTO*/ Reg DMA_A06;
  /*p04.MUGU*/ Reg DMA_A07;

  /*p04.NAFA*/ Reg DMA_A08;
  /*p04.PYNE*/ Reg DMA_A09;
  /*p04.PARA*/ Reg DMA_A10;
  /*p04.NYDO*/ Reg DMA_A11;
  /*p04.NYGY*/ Reg DMA_A12;
  /*p04.PULA*/ Reg DMA_A13;
  /*p04.POKU*/ Reg DMA_A14;
  /*p04.MARU*/ Reg DMA_A15;

private:

  wire MUDA_DMA_SRC_VRAMp() const {
    /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n  = not(DMA_A15.q());
    /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp = nor(DMA_A13.q(), DMA_A14.q(), LEBU_DMA_ADDR_A15n);
    return MUDA_DMA_SRC_VRAMp;
  }


  /*p04.LYXE*/ Reg LYXE_DMA_LATCHn;
  /*p04.MATU*/ Reg _MATU_DMA_RUNNINGp; // 17-rung, bottom rung _must_ be DMA_RUNNINGp.
  /*p04.MYTE*/ Reg MYTE_DMA_DONE;
  /*p04.LUVY*/ Reg LUVY_DMA_TRIG_d0;
  /*p04.LENE*/ Reg LENE_DMA_TRIG_d4;

  // NAND latch
  /*p04.LARA*/ Reg LARA_DMA_LATCHn;
  /*p04.LOKY*/ Reg LOKY_DMA_LATCHp;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics