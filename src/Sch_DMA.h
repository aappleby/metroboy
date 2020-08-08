#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;
struct VramBus;

//-----------------------------------------------------------------------------

struct DmaRegisters {
  void dump(Dumper& d) const;
  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);

  // -> bunch of stuff
  /*p04.MATU*/ wire MATU_DMA_RUNNINGp() const { return _MATU_DMA_RUNNINGp.qp(); }

  // -> ext bus, oam bus
  /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp() const {
    /*p04.LOGO*/ wire _LOGO_DMA_VRAMn      = not1(MUDA_DMA_SRC_VRAMp());
    /*p04.MORY*/ wire _MORY_DMA_READ_CARTn = nand2(MATU_DMA_RUNNINGp(), _LOGO_DMA_VRAMn);
    return not1(_MORY_DMA_READ_CARTn);
  }

  // -> top.ACYL, top.AJON
  /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn() const { return not1(MATU_DMA_RUNNINGp()); }

  // -> oam bus, vram bus
  /*p04.LUFA*/ wire LUFA_DMA_VRM_RDp() const {
    /*p04.MUHO*/ wire MUHO_DMA_VRAM_RDn = nand2(MATU_DMA_RUNNINGp(), MUDA_DMA_SRC_VRAMp());
    return not1(MUHO_DMA_VRAM_RDn);
  }

  /*p04.NAKY*/ RegQPN NAKY_DMA_A00 = REG_D0C0;
  /*p04.PYRO*/ RegQPN PYRO_DMA_A01 = REG_D0C0;
  /*p04.NEFY*/ RegQPN NEFY_DMA_A02 = REG_D0C0;
  /*p04.MUTY*/ RegQPN MUTY_DMA_A03 = REG_D0C0;
  /*p04.NYKO*/ RegQPN NYKO_DMA_A04 = REG_D0C0;
  /*p04.PYLO*/ RegQPN PYLO_DMA_A05 = REG_D0C0;
  /*p04.NUTO*/ RegQPN NUTO_DMA_A06 = REG_D0C0;
  /*p04.MUGU*/ RegQPN MUGU_DMA_A07 = REG_D0C0;

  /*p04.NAFA*/ RegQPN NAFA_DMA_A08 = REG_D0C0;
  /*p04.PYNE*/ RegQPN PYNE_DMA_A09 = REG_D0C0;
  /*p04.PARA*/ RegQPN PARA_DMA_A10 = REG_D0C0;
  /*p04.NYDO*/ RegQPN NYDO_DMA_A11 = REG_D0C0;
  /*p04.NYGY*/ RegQPN NYGY_DMA_A12 = REG_D0C0;
  /*p04.PULA*/ RegQPN PULA_DMA_A13 = REG_D0C0;
  /*p04.POKU*/ RegQPN POKU_DMA_A14 = REG_D0C0;
  /*p04.MARU*/ RegQPN MARU_DMA_A15 = REG_D0C0;

//private:

  wire MUDA_DMA_SRC_VRAMp() const {
    // LEBU := not(MARU_QN)
    // MUDA := nor(PULA_QN, POKU_QN, LEBU)

    // something wrong here... polarity of high addr bits inverted
    // MUDA = nor(!A13, !A14, A15)

    /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n  = not1(MARU_DMA_A15.qn());
    /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp = nor3(PULA_DMA_A13.qn(), POKU_DMA_A14.qn(), LEBU_DMA_ADDR_A15n);
    return MUDA_DMA_SRC_VRAMp;
  }

  /*p04.LYXE*/ Tri   _LYXE_DMA_LATCHn   = TRI_D0NP;
  /*p04.MATU*/ RegQP _MATU_DMA_RUNNINGp = REG_D0C0;
  /*p04.MYTE*/ RegQN _MYTE_DMA_DONE     = REG_D0C0;
  /*p04.LUVY*/ RegQP _LUVY_DMA_TRIG_d0  = REG_D0C0;
  /*p04.LENE*/ RegQN _LENE_DMA_TRIG_d4  = REG_D0C0;
  /*p04.LOKY*/ Tri   _LOKY_DMA_LATCHp   = TRI_D0NP;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics