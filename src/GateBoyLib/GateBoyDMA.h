#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyDMA {

  void reset_to_cart() {
    NAKY_DMA_A00p.state = 0b00011000;
    PYRO_DMA_A01p.state = 0b00011010;
    NEFY_DMA_A02p.state = 0b00011010;
    MUTY_DMA_A03p.state = 0b00011010;
    NYKO_DMA_A04p.state = 0b00011010;
    PYLO_DMA_A05p.state = 0b00011010;
    NUTO_DMA_A06p.state = 0b00011010;
    MUGU_DMA_A07p.state = 0b00011010;
    NAFA_DMA_A08n.state = 0b00011010;
    PYNE_DMA_A09n.state = 0b00011010;
    PARA_DMA_A10n.state = 0b00011010;
    NYDO_DMA_A11n.state = 0b00011010;
    NYGY_DMA_A12n.state = 0b00011010;
    PULA_DMA_A13n.state = 0b00011010;
    POKU_DMA_A14n.state = 0b00011010;
    MARU_DMA_A15n.state = 0b00011010;
    MATU_DMA_RUNNINGp.state = 0b00011010;
    LYXE_DMA_LATCHp.state = 0b00011000;
    MYTE_DMA_DONE.state = 0b00011000;
    LUVY_DMA_TRIG_d0.state = 0b00011010;
    LENE_DMA_TRIG_d4.state = 0b00011000;
    LARA_DMA_LATCHn.state = 0b00011001;
    LOKY_DMA_LATCHp.state = 0b00011000;
  }

  /*#p04.NAKY*/ DFF17 NAKY_DMA_A00p;      // Axxxxxxx
  /*#p04.PYRO*/ DFF17 PYRO_DMA_A01p;      // Axxxxxxx
  /*_p04.NEFY*/ DFF17 NEFY_DMA_A02p;      // Axxxxxxx
  /*_p04.MUTY*/ DFF17 MUTY_DMA_A03p;      // Axxxxxxx
  /*_p04.NYKO*/ DFF17 NYKO_DMA_A04p;      // Axxxxxxx
  /*_p04.PYLO*/ DFF17 PYLO_DMA_A05p;      // Axxxxxxx
  /*_p04.NUTO*/ DFF17 NUTO_DMA_A06p;      // Axxxxxxx
  /*_p04.MUGU*/ DFF17 MUGU_DMA_A07p;      // Axxxxxxx

  /*#p04.NAFA*/ DFF8p NAFA_DMA_A08n;      // xxxxxxxH
  /*_p04.PYNE*/ DFF8p PYNE_DMA_A09n;      // xxxxxxxH
  /*_p04.PARA*/ DFF8p PARA_DMA_A10n;      // xxxxxxxH
  /*_p04.NYDO*/ DFF8p NYDO_DMA_A11n;      // xxxxxxxH
  /*_p04.NYGY*/ DFF8p NYGY_DMA_A12n;      // xxxxxxxH
  /*_p04.PULA*/ DFF8p PULA_DMA_A13n;      // xxxxxxxH
  /*_p04.POKU*/ DFF8p POKU_DMA_A14n;      // xxxxxxxH
  /*_p04.MARU*/ DFF8p MARU_DMA_A15n;      // xxxxxxxH

  /*#p04.MATU*/ DFF17 MATU_DMA_RUNNINGp;  // Axxxxxxx
  /*#p04.LYXE*/ NorLatch LYXE_DMA_LATCHp; // xxxxExxx
  /*#p04.MYTE*/ DFF17 MYTE_DMA_DONE;      // xxxxExxx
  /*#p04.LUVY*/ DFF17 LUVY_DMA_TRIG_d0;   // Axxxxxxx
  /*#p04.LENE*/ DFF17 LENE_DMA_TRIG_d4;   // xxxxExxx

  /*_p04.LARA*/ Gate LARA_DMA_LATCHn;     // xxxxExxx - NAND latch w/ LOKY
  /*#p04.LOKY*/ Gate LOKY_DMA_LATCHp;     // xxxxExxx - NAND latch w/ LARA
};

//------------------------------------------------------------------------------------------------------------------------
