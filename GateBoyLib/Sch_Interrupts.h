#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct InterruptRegisters {
  void reset_cart() {
    LOPE_FF0F_D0p.reset(REG_D1C1);
    UBUL_FF0F_D3p.reset(REG_D0C0);
    ULAK_FF0F_D4p.reset(REG_D0C0);
    LALU_FF0F_D1p.reset(REG_D0C0);
    NYBO_FF0F_D2p.reset(REG_D0C0);

    MATY_FF0F_L0p.reset(0);
    NEJY_FF0F_L3p.reset(0);
    NUTY_FF0F_L4p.reset(0);
    MOPO_FF0F_L1p.reset(0);
    PAVY_FF0F_L2p.reset(0);
  }

  /*p02.LOPE*/ DFF22 LOPE_FF0F_D0p;
  /*p02.UBUL*/ DFF22 UBUL_FF0F_D3p;
  /*p02.ULAK*/ DFF22 ULAK_FF0F_D4p;
  /*p02.LALU*/ DFF22 LALU_FF0F_D1p;
  /*p02.NYBO*/ DFF22 NYBO_FF0F_D2p;

  /*p02.MATY*/ TpLatch MATY_FF0F_L0p;
  /*p02.NEJY*/ TpLatch NEJY_FF0F_L3p;
  /*p02.NUTY*/ TpLatch NUTY_FF0F_L4p;
  /*p02.MOPO*/ TpLatch MOPO_FF0F_L1p;
  /*p02.PAVY*/ TpLatch PAVY_FF0F_L2p;
};

//-----------------------------------------------------------------------------
