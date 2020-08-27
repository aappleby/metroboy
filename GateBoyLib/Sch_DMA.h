#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;
struct VramBus;

//-----------------------------------------------------------------------------

struct DmaRegisters {
  void dump(Dumper& d) const;
  void tick();
  void tock(const SchematicTop& top, CpuBus& cpu_bus);

  /*p04.MATU*/ Sig MATU_DMA_RUNNINGp;
  /*p04.LUMA*/ Sig LUMA_DMA_READ_CARTp;
  /*p04.LUFA*/ Sig LUFA_DMA_VRM_RDp;

  /*p04.NAKY*/ RegQPN NAKY_DMA_A00p = REG_D0C0;
  /*p04.PYRO*/ RegQPN PYRO_DMA_A01p = REG_D0C0;
  /*p04.NEFY*/ RegQPN NEFY_DMA_A02p = REG_D0C0;
  /*p04.MUTY*/ RegQPN MUTY_DMA_A03p = REG_D0C0;
  /*p04.NYKO*/ RegQPN NYKO_DMA_A04p = REG_D0C0;
  /*p04.PYLO*/ RegQPN PYLO_DMA_A05p = REG_D0C0;
  /*p04.NUTO*/ RegQPN NUTO_DMA_A06p = REG_D0C0;
  /*p04.MUGU*/ RegQPN MUGU_DMA_A07p = REG_D0C0;

  /*p04.NAFA*/ RegQPN NAFA_DMA_A08n = REG_D0C0;
  /*p04.PYNE*/ RegQPN PYNE_DMA_A09n = REG_D0C0;
  /*p04.PARA*/ RegQPN PARA_DMA_A10n = REG_D0C0;
  /*p04.NYDO*/ RegQPN NYDO_DMA_A11n = REG_D0C0;
  /*p04.NYGY*/ RegQPN NYGY_DMA_A12n = REG_D0C0;
  /*p04.PULA*/ RegQPN PULA_DMA_A13n = REG_D0C0;
  /*p04.POKU*/ RegQPN POKU_DMA_A14n = REG_D0C0;
  /*p04.MARU*/ RegQPN MARU_DMA_A15n = REG_D0C0;

private:

  /*p04.LYXE*/ Latch _LYXE_DMA_LATCHp   = TRI_D0NP;
  /*p04.MATU*/ RegQP _MATU_DMA_RUNNINGp = REG_D0C0;
  /*p04.MYTE*/ RegQN _MYTE_DMA_DONE     = REG_D0C0;
  /*p04.LUVY*/ RegQP _LUVY_DMA_TRIG_d0  = REG_D0C0;
  /*p04.LENE*/ RegQN _LENE_DMA_TRIG_d4  = REG_D0C0;
  /*p04.LOKY*/ Tri   _LOKY_DMA_LATCHp   = TRI_D0NP; // NAND latch w/ LARA
  /*p04.LARA*/ Tri   _LARA_DMA_LATCHn   = TRI_D1NP; // NAND latch w/ LOKY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics