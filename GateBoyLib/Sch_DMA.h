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

  /*p04.MATU*/ DFF17 MATU_DMA_RUNNINGp;

  /*p04.LUMA*/ Sig LUMA_DMA_READ_CARTp;
  /*p04.LUFA*/ Sig LUFA_DMA_READ_VRAMp;

  /*p04.NAKY*/ DFF17 NAKY_DMA_A00p;
  /*p04.PYRO*/ DFF17 PYRO_DMA_A01p;
  /*p04.NEFY*/ DFF17 NEFY_DMA_A02p;
  /*p04.MUTY*/ DFF17 MUTY_DMA_A03p;
  /*p04.NYKO*/ DFF17 NYKO_DMA_A04p;
  /*p04.PYLO*/ DFF17 PYLO_DMA_A05p;
  /*p04.NUTO*/ DFF17 NUTO_DMA_A06p;
  /*p04.MUGU*/ DFF17 MUGU_DMA_A07p;

  /*p04.NAFA*/ DFF8 NAFA_DMA_A08n;
  /*p04.PYNE*/ DFF8 PYNE_DMA_A09n;
  /*p04.PARA*/ DFF8 PARA_DMA_A10n;
  /*p04.NYDO*/ DFF8 NYDO_DMA_A11n;
  /*p04.NYGY*/ DFF8 NYGY_DMA_A12n;
  /*p04.PULA*/ DFF8 PULA_DMA_A13n;
  /*p04.POKU*/ DFF8 POKU_DMA_A14n;
  /*p04.MARU*/ DFF8 MARU_DMA_A15n;

private:

  /*p04.LYXE*/ NorLatch LYXE_DMA_LATCHp;
  /*p04.MYTE*/ DFF17 MYTE_DMA_DONE;
  /*p04.LUVY*/ DFF17 LUVY_DMA_TRIG_d0;
  /*p04.LENE*/ DFF17 LENE_DMA_TRIG_d4;

  /*p04.LOKY*/ NandLatch LOKY_DMA_LATCHp; // NAND latch w/ LARA
  ///*p04.LARA*/ Tri   LARA_DMA_LATCHn = TRI_D1NP; // NAND latch w/ LOKY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics