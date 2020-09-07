#pragma once
#include "GateBoyLib/Gates.h"

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

  /*p04.MATU*/ RegQP MATU_DMA_RUNNINGp = REG_D0C0;

  /*p04.LUMA*/ Sig LUMA_DMA_READ_CARTp;
  /*p04.LUFA*/ Sig LUFA_DMA_READ_VRAMp;

  /*p04.NAKY*/ RegQPN NAKY_DMA_A00p = REG_D0C0;
  /*p04.PYRO*/ RegQPN PYRO_DMA_A01p = REG_D0C0;
  /*p04.NEFY*/ RegQPN NEFY_DMA_A02p = REG_D0C0;
  /*p04.MUTY*/ RegQPN MUTY_DMA_A03p = REG_D0C0;
  /*p04.NYKO*/ RegQPN NYKO_DMA_A04p = REG_D0C0;
  /*p04.PYLO*/ RegQPN PYLO_DMA_A05p = REG_D0C0;
  /*p04.NUTO*/ RegQPN NUTO_DMA_A06p = REG_D0C0;
  /*p04.MUGU*/ RegQPN MUGU_DMA_A07p = REG_D0C0;

  /*p04.NAFA*/ DFF8 NAFA_DMA_A08n;
  /*p04.PYNE*/ DFF8 PYNE_DMA_A09n;
  /*p04.PARA*/ DFF8 PARA_DMA_A10n;
  /*p04.NYDO*/ DFF8 NYDO_DMA_A11n;
  /*p04.NYGY*/ DFF8 NYGY_DMA_A12n;
  /*p04.PULA*/ DFF8 PULA_DMA_A13n;
  /*p04.POKU*/ DFF8 POKU_DMA_A14n;
  /*p04.MARU*/ DFF8 MARU_DMA_A15n;

private:

  /*p04.LYXE*/ Latch LYXE_DMA_LATCHp   = TRI_D0NP;
  /*p04.MYTE*/ RegQN MYTE_DMA_DONE     = REG_D0C0;
  /*p04.LUVY*/ RegQP LUVY_DMA_TRIG_d0  = REG_D0C0;
  /*p04.LENE*/ RegQN LENE_DMA_TRIG_d4  = REG_D0C0;
  /*p04.LOKY*/ Tri   LOKY_DMA_LATCHp   = TRI_D0NP; // NAND latch w/ LARA
  /*p04.LARA*/ Tri   LARA_DMA_LATCHn   = TRI_D1NP; // NAND latch w/ LOKY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics