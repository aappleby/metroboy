#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct DmaRegisters {

  void reset_cart() {
    MATU_DMA_RUNNINGp_evn.reset(REG_D0C1);

    NAKY_DMA_A00p_evn.reset(REG_D0C0);
    PYRO_DMA_A01p_evn.reset(REG_D0C1);
    NEFY_DMA_A02p_evn.reset(REG_D0C1);
    MUTY_DMA_A03p_evn.reset(REG_D0C1);
    NYKO_DMA_A04p_evn.reset(REG_D0C1);
    PYLO_DMA_A05p_evn.reset(REG_D0C1);
    NUTO_DMA_A06p_evn.reset(REG_D0C1);
    MUGU_DMA_A07p_evn.reset(REG_D0C1);

    NAFA_DMA_A08n_h.reset(REG_D0C1);
    PYNE_DMA_A09n_h.reset(REG_D0C1);
    PARA_DMA_A10n_h.reset(REG_D0C1);
    NYDO_DMA_A11n_h.reset(REG_D0C1);
    NYGY_DMA_A12n_h.reset(REG_D0C1);
    PULA_DMA_A13n_h.reset(REG_D0C1);
    POKU_DMA_A14n_h.reset(REG_D0C1);
    MARU_DMA_A15n_h.reset(REG_D0C1);

    LYXE_DMA_LATCHp_evn.reset(0);

    MYTE_DMA_DONE_evn.reset(REG_D0C0);
    LUVY_DMA_TRIG_d0_evn.reset(REG_D0C1);
    LENE_DMA_TRIG_d4_evn.reset(REG_D0C0);

    LARA_DMA_LATCHn_evn.reset(1);
    LOKY_DMA_LATCHp_evn.reset(0);
  }

  void reset_boot() {
    MATU_DMA_RUNNINGp_evn.reset(REG_D0C0);

    NAKY_DMA_A00p_evn.reset(REG_D0C0);
    PYRO_DMA_A01p_evn.reset(REG_D0C0);
    NEFY_DMA_A02p_evn.reset(REG_D0C0);
    MUTY_DMA_A03p_evn.reset(REG_D0C0);
    NYKO_DMA_A04p_evn.reset(REG_D0C0);
    PYLO_DMA_A05p_evn.reset(REG_D0C0);
    NUTO_DMA_A06p_evn.reset(REG_D0C0);
    MUGU_DMA_A07p_evn.reset(REG_D0C0);

    NAFA_DMA_A08n_h.reset(REG_D0C0);
    PYNE_DMA_A09n_h.reset(REG_D0C0);
    PARA_DMA_A10n_h.reset(REG_D0C0);
    NYDO_DMA_A11n_h.reset(REG_D0C0);
    NYGY_DMA_A12n_h.reset(REG_D0C0);
    PULA_DMA_A13n_h.reset(REG_D0C0);
    POKU_DMA_A14n_h.reset(REG_D0C0);
    MARU_DMA_A15n_h.reset(REG_D0C0);

    LYXE_DMA_LATCHp_evn.reset(0);

    MYTE_DMA_DONE_evn.reset(REG_D0C0);
    LUVY_DMA_TRIG_d0_evn.reset(REG_D0C0);
    LENE_DMA_TRIG_d4_evn.reset(REG_D0C0);

    LARA_DMA_LATCHn_evn.reset(1);
    LOKY_DMA_LATCHp_evn.reset(0);
  }

  /*p04.MATU*/ DFF17 MATU_DMA_RUNNINGp_evn;  // Axxxxxxx
  /*p04.LYXE*/ NorLatch LYXE_DMA_LATCHp_evn; // xxxxExxx
  /*p04.MYTE*/ DFF17 MYTE_DMA_DONE_evn;      // xxxxExxx
  /*p04.LUVY*/ DFF17 LUVY_DMA_TRIG_d0_evn;   // Axxxxxxx
  /*p04.LENE*/ DFF17 LENE_DMA_TRIG_d4_evn;   // xxxxExxx

  /*p04.LARA*/ Gate LARA_DMA_LATCHn_evn;     // xxxxExxx - NAND latch w/ LOKY
  /*p04.LOKY*/ Gate LOKY_DMA_LATCHp_evn;     // xxxxExxx - NAND latch w/ LARA

  /*p04.NAKY*/ DFF17 NAKY_DMA_A00p_evn;      // Axxxxxxx
  /*p04.PYRO*/ DFF17 PYRO_DMA_A01p_evn;      // Axxxxxxx
  /*p04.NEFY*/ DFF17 NEFY_DMA_A02p_evn;      // Axxxxxxx
  /*p04.MUTY*/ DFF17 MUTY_DMA_A03p_evn;      // Axxxxxxx
  /*p04.NYKO*/ DFF17 NYKO_DMA_A04p_evn;      // Axxxxxxx
  /*p04.PYLO*/ DFF17 PYLO_DMA_A05p_evn;      // Axxxxxxx
  /*p04.NUTO*/ DFF17 NUTO_DMA_A06p_evn;      // Axxxxxxx
  /*p04.MUGU*/ DFF17 MUGU_DMA_A07p_evn;      // Axxxxxxx

  /*p04.NAFA*/ DFF8p NAFA_DMA_A08n_h;      // xxxxxxxH
  /*p04.PYNE*/ DFF8p PYNE_DMA_A09n_h;      // xxxxxxxH
  /*p04.PARA*/ DFF8p PARA_DMA_A10n_h;      // xxxxxxxH
  /*p04.NYDO*/ DFF8p NYDO_DMA_A11n_h;      // xxxxxxxH
  /*p04.NYGY*/ DFF8p NYGY_DMA_A12n_h;      // xxxxxxxH
  /*p04.PULA*/ DFF8p PULA_DMA_A13n_h;      // xxxxxxxH
  /*p04.POKU*/ DFF8p POKU_DMA_A14n_h;      // xxxxxxxH
  /*p04.MARU*/ DFF8p MARU_DMA_A15n_h;      // xxxxxxxH

};

//-----------------------------------------------------------------------------
