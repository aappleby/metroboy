#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct BusMuxSignals {
  /*p28.GEKA*/ Signal GEKA_OAM_A0p;
  /*p28.ZYFO*/ Signal ZYFO_OAM_A1p;
  /*p28.YFOT*/ Signal YFOT_OAM_A2p;
  /*p28.YFOC*/ Signal YFOC_OAM_A3p;
  /*p28.YVOM*/ Signal YVOM_OAM_A4p;
  /*p28.YMEV*/ Signal YMEV_OAM_A5p;
  /*p28.XEMU*/ Signal XEMU_OAM_A6p;
  /*p28.YZET*/ Signal YZET_OAM_A7p;

  /*p28.AZUL*/ Signal AZUL_D_TO_OAMD;
  /*p25.XEDU*/ Signal XEDU_VRAM_LOCK;
  /*p04.AHOC*/ Signal AHOC_DMA_VRAM_RDn;
  /*p25.RAHU*/ Signal RAHU_VBUS_TRISTATEp;
  /*p04.WYJA*/ Signal WYJA_OAM_WRp;
  /*p28.AMAB*/ Signal AMAB_OAM_LOCKn;
  /*p25.COTA*/ Signal COTA_OAM_CLK;
  /*p28.BODE*/ Signal BODE_OAM_LATCH;
  /*p25.SEBY*/ Signal SEBY_MD_TO_D;

  /*p08.TAZY*/ Signal TAZY_A15;
  /*p08.TYHO*/ Signal TYHO_CS_A;

  /*p08.TYMU*/ Signal TYMU_RD_OUTn;
  /*p08.PUVA*/ Signal PUVA_WR_OUTn;

  /*p08.AMET*/ Signal EXT_ADDR_00;
  /*p08.ATOL*/ Signal EXT_ADDR_01;
  /*p08.APOK*/ Signal EXT_ADDR_02;
  /*p08.AMER*/ Signal EXT_ADDR_03;
  /*p08.ATEM*/ Signal EXT_ADDR_04;
  /*p08.ATOV*/ Signal EXT_ADDR_05;
  /*p08.ATYR*/ Signal EXT_ADDR_06;
  /*p08.ASUR*/ Signal EXT_ADDR_07;
  /*p08.MANO*/ Signal EXT_ADDR_08;
  /*p08.MASU*/ Signal EXT_ADDR_09;
  /*p08.PAMY*/ Signal EXT_ADDR_10;
  /*p08.MALE*/ Signal EXT_ADDR_11;
  /*p08.MOJY*/ Signal EXT_ADDR_12;
  /*p08.MUCE*/ Signal EXT_ADDR_13;
  /*p08.PEGE*/ Signal EXT_ADDR_14;
};

//-----------------------------------------------------------------------------

struct BusMux {
  BusMuxSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit(void);

private:

  /*p04.MAKA*/ Reg MAKA_FROM_CPU5_SYNC;

  /*p08.ALOR*/ TpLatch CPU_ADDR_LATCH_00;
  /*p08.APUR*/ TpLatch CPU_ADDR_LATCH_01;
  /*p08.ALYR*/ TpLatch CPU_ADDR_LATCH_02;
  /*p08.ARET*/ TpLatch CPU_ADDR_LATCH_03;
  /*p08.AVYS*/ TpLatch CPU_ADDR_LATCH_04;
  /*p08.ATEV*/ TpLatch CPU_ADDR_LATCH_05;
  /*p08.AROS*/ TpLatch CPU_ADDR_LATCH_06;
  /*p08.ARYM*/ TpLatch CPU_ADDR_LATCH_07;
  /*p08.LUNO*/ TpLatch CPU_ADDR_LATCH_08;
  /*p08.LYSA*/ TpLatch CPU_ADDR_LATCH_09;
  /*p08.PATE*/ TpLatch CPU_ADDR_LATCH_10;
  /*p08.LUMY*/ TpLatch CPU_ADDR_LATCH_11;
  /*p08.LOBU*/ TpLatch CPU_ADDR_LATCH_12;
  /*p08.LONU*/ TpLatch CPU_ADDR_LATCH_13;
  /*p08.NYRE*/ TpLatch CPU_ADDR_LATCH_14;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics