#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct DmaRegisters {

  void reset_cart() {
    MATU_DMA_RUNNINGp.reset(REG_D0C1);

    NAKY_DMA_A00p.reset(REG_D0C0);
    PYRO_DMA_A01p.reset(REG_D0C1);
    NEFY_DMA_A02p.reset(REG_D0C1);
    MUTY_DMA_A03p.reset(REG_D0C1);
    NYKO_DMA_A04p.reset(REG_D0C1);
    PYLO_DMA_A05p.reset(REG_D0C1);
    NUTO_DMA_A06p.reset(REG_D0C1);
    MUGU_DMA_A07p.reset(REG_D0C1);

    NAFA_DMA_A08n.reset(REG_D0C1);
    PYNE_DMA_A09n.reset(REG_D0C1);
    PARA_DMA_A10n.reset(REG_D0C1);
    NYDO_DMA_A11n.reset(REG_D0C1);
    NYGY_DMA_A12n.reset(REG_D0C1);
    PULA_DMA_A13n.reset(REG_D0C1);
    POKU_DMA_A14n.reset(REG_D0C1);
    MARU_DMA_A15n.reset(REG_D0C1);

    LYXE_DMA_LATCHp.reset(0);

    MYTE_DMA_DONE.reset(REG_D0C0);
    LUVY_DMA_TRIG_d0.reset(REG_D0C1);
    LENE_DMA_TRIG_d4.reset(REG_D0C0);

    LOKY_DMA_LATCHp.reset(0);
  }

  void reset_boot() {
    MATU_DMA_RUNNINGp.reset(REG_D0C0);

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

    LYXE_DMA_LATCHp.reset(0);

    MYTE_DMA_DONE.reset(REG_D0C0);
    LUVY_DMA_TRIG_d0.reset(REG_D0C0);
    LENE_DMA_TRIG_d4.reset(REG_D0C0);

    LOKY_DMA_LATCHp.reset(0);
  }

  /*p04.MATU*/ DFF17 MATU_DMA_RUNNINGp;

  /*p04.NAKY*/ DFF17 NAKY_DMA_A00p;
  /*p04.PYRO*/ DFF17 PYRO_DMA_A01p;
  /*p04.NEFY*/ DFF17 NEFY_DMA_A02p;
  /*p04.MUTY*/ DFF17 MUTY_DMA_A03p;
  /*p04.NYKO*/ DFF17 NYKO_DMA_A04p;
  /*p04.PYLO*/ DFF17 PYLO_DMA_A05p;
  /*p04.NUTO*/ DFF17 NUTO_DMA_A06p;
  /*p04.MUGU*/ DFF17 MUGU_DMA_A07p;

  /*p04.NAFA*/ DFF8p NAFA_DMA_A08n;
  /*p04.PYNE*/ DFF8p PYNE_DMA_A09n;
  /*p04.PARA*/ DFF8p PARA_DMA_A10n;
  /*p04.NYDO*/ DFF8p NYDO_DMA_A11n;
  /*p04.NYGY*/ DFF8p NYGY_DMA_A12n;
  /*p04.PULA*/ DFF8p PULA_DMA_A13n;
  /*p04.POKU*/ DFF8p POKU_DMA_A14n;
  /*p04.MARU*/ DFF8p MARU_DMA_A15n;

  /*p04.LYXE*/ NorLatch LYXE_DMA_LATCHp;
  /*p04.MYTE*/ DFF17 MYTE_DMA_DONE;
  /*p04.LUVY*/ DFF17 LUVY_DMA_TRIG_d0;
  /*p04.LENE*/ DFF17 LENE_DMA_TRIG_d4;

  /*p04.LOKY*/ NandLatch LOKY_DMA_LATCHp; // NAND latch w/ LARA
  ///*p04.LARA*/ Tri   LARA_DMA_LATCHn = TRI_D1NP; // NAND latch w/ LOKY
};

//-----------------------------------------------------------------------------
