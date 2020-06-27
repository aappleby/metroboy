#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct BusMuxSignals {
  /*p28.GEKA*/ wire GEKA_OAM_A0p;
  /*p28.ZYFO*/ wire ZYFO_OAM_A1p;
  /*p28.YFOT*/ wire YFOT_OAM_A2p;
  /*p28.YFOC*/ wire YFOC_OAM_A3p;
  /*p28.YVOM*/ wire YVOM_OAM_A4p;
  /*p28.YMEV*/ wire YMEV_OAM_A5p;
  /*p28.XEMU*/ wire XEMU_OAM_A6p;
  /*p28.YZET*/ wire YZET_OAM_A7p;

  /*p28.AZUL*/ wire AZUL_D_TO_OAMD;
  /*p25.XEDU*/ wire XEDU_VRAM_LOCK;
  /*p04.AHOC*/ wire AHOC_DMA_VRAM_RDn;
  /*p25.RAHU*/ wire RAHU_VBUS_TRISTATEp;
  /*p04.WYJA*/ wire WYJA_OAM_WRp;
  /*p28.AMAB*/ wire AMAB_OAM_LOCKn;
  /*p25.COTA*/ wire COTA_OAM_CLK;
  /*p28.BODE*/ wire BODE_OAM_LATCH;

};

struct BusMux {
  BusMuxSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);

};

//-----------------------------------------------------------------------------

}; // namespace Schematics