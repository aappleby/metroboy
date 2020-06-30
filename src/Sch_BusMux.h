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
  /*p25.SEBY*/ bool SEBY_MD_TO_D;

  /*p08.TAZY*/ bool TAZY_A15;
  /*p08.TYHO*/ bool TYHO_CS_A;

  /*p08.TYMU*/ bool TYMU_RD_OUTn;
  /*p08.PUVA*/ bool PUVA_WR_OUTn;

  /*p08.AMET*/ bool EXT_ADDR_00;
  /*p08.ATOL*/ bool EXT_ADDR_01;
  /*p08.APOK*/ bool EXT_ADDR_02;
  /*p08.AMER*/ bool EXT_ADDR_03;
  /*p08.ATEM*/ bool EXT_ADDR_04;
  /*p08.ATOV*/ bool EXT_ADDR_05;
  /*p08.ATYR*/ bool EXT_ADDR_06;
  /*p08.ASUR*/ bool EXT_ADDR_07;
  /*p08.MANO*/ bool EXT_ADDR_08;
  /*p08.MASU*/ bool EXT_ADDR_09;
  /*p08.PAMY*/ bool EXT_ADDR_10;
  /*p08.MALE*/ bool EXT_ADDR_11;
  /*p08.MOJY*/ bool EXT_ADDR_12;
  /*p08.MUCE*/ bool EXT_ADDR_13;
  /*p08.PEGE*/ bool EXT_ADDR_14;
};

struct BusMux {
  BusMuxSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit(void);

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