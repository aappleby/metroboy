#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct RegDmaLo {
  void reset();

  /*#p04.NAKY*/ DFF17 NAKY_DMA_A00p_odd;      // Axxxxxxx
  /*#p04.PYRO*/ DFF17 PYRO_DMA_A01p_odd;      // Axxxxxxx
  /*_p04.NEFY*/ DFF17 NEFY_DMA_A02p_odd;      // Axxxxxxx
  /*_p04.MUTY*/ DFF17 MUTY_DMA_A03p_odd;      // Axxxxxxx
  /*_p04.NYKO*/ DFF17 NYKO_DMA_A04p_odd;      // Axxxxxxx
  /*_p04.PYLO*/ DFF17 PYLO_DMA_A05p_odd;      // Axxxxxxx
  /*_p04.NUTO*/ DFF17 NUTO_DMA_A06p_odd;      // Axxxxxxx
  /*_p04.MUGU*/ DFF17 MUGU_DMA_A07p_odd;      // Axxxxxxx
};

//-----------------------------------------------------------------------------

struct RegDmaHi {
  void reset();

  /*#p04.NAFA*/ DFF8 NAFA_DMA_A08p;      // xxxxxxxH
  /*_p04.PYNE*/ DFF8 PYNE_DMA_A09p;      // xxxxxxxH
  /*_p04.PARA*/ DFF8 PARA_DMA_A10p;      // xxxxxxxH
  /*_p04.NYDO*/ DFF8 NYDO_DMA_A11p;      // xxxxxxxH
  /*_p04.NYGY*/ DFF8 NYGY_DMA_A12p;      // xxxxxxxH
  /*_p04.PULA*/ DFF8 PULA_DMA_A13p;      // xxxxxxxH
  /*_p04.POKU*/ DFF8 POKU_DMA_A14p;      // xxxxxxxH
  /*_p04.MARU*/ DFF8 MARU_DMA_A15p;      // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct DmaControl {
  void reset();

  /*#p04.LYXE*/ NorLatch LYXE_DMA_LATCHp; // xxxxExxx
  /*#p04.MYTE*/ DFF17 MYTE_DMA_DONE_odd;      // xxxxExxx
  /*#p04.LUVY*/ DFF17 LUVY_DMA_TRIG_d0_odd;   // Axxxxxxx
  /*#p04.LENE*/ DFF17 LENE_DMA_TRIG_d4_odd;   // xxxxExxx
  /*_p04.LARA*/ Gate LARA_DMA_LATCHn_odd;     // xxxxExxx - NAND latch w/ LOKY
  /*#p04.LOKY*/ Gate LOKY_DMA_LATCHp_odd;     // xxxxExxx - NAND latch w/ LARA
};

//-----------------------------------------------------------------------------
