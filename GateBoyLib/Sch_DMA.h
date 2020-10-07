#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;
struct OamBus;
struct VramBus;

//-----------------------------------------------------------------------------

struct DmaRegisters {
  void reset() {}

  void reset_bootrom() {
    MATU_DMA_RUNNINGp.reset(REG_D0C0);

    LUMA_DMA_READ_CARTp.reset(TRI_HZNP);
    LUFA_DMA_READ_VRAMp.reset(TRI_HZNP);

    NAKY_DMA_A00p.reset(REG_D0C0);
    PYRO_DMA_A01p.reset(REG_D0C0);
    NEFY_DMA_A02p.reset(REG_D0C0);
    MUTY_DMA_A03p.reset(REG_D0C0);
    NYKO_DMA_A04p.reset(REG_D0C0);
    PYLO_DMA_A05p.reset(REG_D0C0);
    NUTO_DMA_A06p.reset(REG_D0C0);
    MUGU_DMA_A07p.reset(REG_D0C0);

    NAFA_DMA_A08n.reset(REG_D0C0);
    PYNE_DMA_A09n.reset(REG_D0C0);
    PARA_DMA_A10n.reset(REG_D0C0);
    NYDO_DMA_A11n.reset(REG_D0C0);
    NYGY_DMA_A12n.reset(REG_D0C0);
    PULA_DMA_A13n.reset(REG_D0C0);
    POKU_DMA_A14n.reset(REG_D0C0);
    MARU_DMA_A15n.reset(REG_D0C0);

    LYXE_DMA_LATCHp.reset(TRI_D0NP);

    MYTE_DMA_DONE.reset(REG_D0C0);
    LUVY_DMA_TRIG_d0.reset(REG_D0C0);
    LENE_DMA_TRIG_d4.reset(REG_D0C0);

    LOKY_DMA_LATCHp.reset(TRI_D0NP);
  }

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