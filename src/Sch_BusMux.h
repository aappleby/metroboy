#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct BusMuxSignals {
  /*p28.GEKA*/ bool GEKA_OAM_A0p;
  /*p28.ZYFO*/ bool ZYFO_OAM_A1p;
  /*p28.YFOT*/ bool YFOT_OAM_A2p;
  /*p28.YFOC*/ bool YFOC_OAM_A3p;
  /*p28.YVOM*/ bool YVOM_OAM_A4p;
  /*p28.YMEV*/ bool YMEV_OAM_A5p;
  /*p28.XEMU*/ bool XEMU_OAM_A6p;
  /*p28.YZET*/ bool YZET_OAM_A7p;

  /*p28.AZUL*/ bool AZUL_D_TO_OAMD;
  /*p25.XEDU*/ bool XEDU_VRAM_LOCK;
  /*p04.AHOC*/ bool AHOC_DMA_VRAM_RDn;
  /*p25.RAHU*/ bool RAHU_VBUS_TRISTATEp;
  /*p04.WYJA*/ bool WYJA_OAM_WRp;
  /*p28.AMAB*/ bool AMAB_OAM_LOCKn;
  /*p25.COTA*/ bool COTA_OAM_CLK;
  /*p28.BODE*/ bool BODE_OAM_LATCH;
  /*p25.SEBY*/ wire SEBY_MD_TO_D;

};

struct BusMux {
  BusMuxSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);

};

//-----------------------------------------------------------------------------

}; // namespace Schematics