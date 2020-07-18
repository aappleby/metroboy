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

  /*p04.NAKY*/ Reg2 DMA_A00 = Reg2::D0C0;
  /*p04.PYRO*/ Reg2 DMA_A01 = Reg2::D0C0;
  /*p04.NEFY*/ Reg2 DMA_A02 = Reg2::D0C0;
  /*p04.MUTY*/ Reg2 DMA_A03 = Reg2::D0C0;
  /*p04.NYKO*/ Reg2 DMA_A04 = Reg2::D0C0;
  /*p04.PYLO*/ Reg2 DMA_A05 = Reg2::D0C0;
  /*p04.NUTO*/ Reg2 DMA_A06 = Reg2::D0C0;
  /*p04.MUGU*/ Reg2 DMA_A07 = Reg2::D0C0;

  /*p04.NAFA*/ Reg2 DMA_A08 = Reg2::D0C0;
  /*p04.PYNE*/ Reg2 DMA_A09 = Reg2::D0C0;
  /*p04.PARA*/ Reg2 DMA_A10 = Reg2::D0C0;
  /*p04.NYDO*/ Reg2 DMA_A11 = Reg2::D0C0;
  /*p04.NYGY*/ Reg2 DMA_A12 = Reg2::D0C0;
  /*p04.PULA*/ Reg2 DMA_A13 = Reg2::D0C0;
  /*p04.POKU*/ Reg2 DMA_A14 = Reg2::D0C0;
  /*p04.MARU*/ Reg2 DMA_A15 = Reg2::D0C0;

private:

  wire MUDA_DMA_SRC_VRAMp() const {
    /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n  = not(DMA_A15.q());
    /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp = nor(DMA_A13.q(), DMA_A14.q(), LEBU_DMA_ADDR_A15n);
    return MUDA_DMA_SRC_VRAMp;
  }

  /*p04.LYXE*/ Reg2 _LYXE_DMA_LATCHn   = Reg2::D0C0;
  /*p04.MATU*/ Reg2 _MATU_DMA_RUNNINGp = Reg2::D0C0;
  /*p04.MYTE*/ Reg2 _MYTE_DMA_DONE     = Reg2::D0C0;
  /*p04.LUVY*/ Reg2 _LUVY_DMA_TRIG_d0  = Reg2::D0C0;
  /*p04.LENE*/ Reg2 _LENE_DMA_TRIG_d4  = Reg2::D0C0;
  /*p04.LOKY*/ Reg2 _LOKY_DMA_LATCHp   = Reg2::D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics