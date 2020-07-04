#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct BusMux {
  void tick(SchematicTop& gb);
  SignalHash commit();

private:
  friend SchematicTop;

  /*p04.MAKA*/ Reg17 MAKA_FROM_CPU5_SYNC;

  /*p28.GEKA*/ Signal GEKA_OAM_A0p;
  /*p28.ZYFO*/ Signal ZYFO_OAM_A1p;
  /*p28.YFOT*/ Signal YFOT_OAM_A2p;
  /*p28.YFOC*/ Signal YFOC_OAM_A3p;
  /*p28.YVOM*/ Signal YVOM_OAM_A4p;
  /*p28.YMEV*/ Signal YMEV_OAM_A5p;
  /*p28.XEMU*/ Signal XEMU_OAM_A6p;
  /*p28.YZET*/ Signal YZET_OAM_A7p;

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

  // Ext-to-cpu "latch" - looks more like a pass gate really

  /*p08.SOMA*/ TpLatch SOMA_EXT_DATA_LATCH_00;
  /*p08.RONY*/ TpLatch RONY_EXT_DATA_LATCH_01;
  /*p08.RAXY*/ TpLatch RAXY_EXT_DATA_LATCH_02;
  /*p08.SELO*/ TpLatch SELO_EXT_DATA_LATCH_03;
  /*p08.SODY*/ TpLatch SODY_EXT_DATA_LATCH_04;
  /*p08.SAGO*/ TpLatch SAGO_EXT_DATA_LATCH_05;
  /*p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_06;
  /*p08.SAZY*/ TpLatch SAZY_EXT_DATA_LATCH_07;

  /*p31.YLOR*/ Reg8 YLOR_SPRITE_X0;
  /*p31.ZYTY*/ Reg8 ZYTY_SPRITE_X1;
  /*p31.ZYVE*/ Reg8 ZYVE_SPRITE_X2;
  /*p31.ZEZY*/ Reg8 ZEZY_SPRITE_X3;
  /*p31.GOMO*/ Reg8 GOMO_SPRITE_X4;
  /*p31.BAXO*/ Reg8 BAXO_SPRITE_X5;
  /*p31.YZOS*/ Reg8 YZOS_SPRITE_X6;
  /*p31.DEPO*/ Reg8 DEPO_SPRITE_X7;

  /*p29.XUSO*/ Reg8 XUSO_SPRITE_Y0;
  /*p29.XEGU*/ Reg8 XEGU_SPRITE_Y1;
  /*p29.YJEX*/ Reg8 YJEX_SPRITE_Y2;
  /*p29.XYJU*/ Reg8 XYJU_SPRITE_Y3;
  /*p29.YBOG*/ Reg8 YBOG_SPRITE_Y4;
  /*p29.WYSO*/ Reg8 WYSO_SPRITE_Y5;
  /*p29.XOTE*/ Reg8 XOTE_SPRITE_Y6;
  /*p29.YZAB*/ Reg8 YZAB_SPRITE_Y7;

  /*p31.XYKY*/ TpLatch XYKY_LATCH_OAM_A0;
  /*p31.YRUM*/ TpLatch YRUM_LATCH_OAM_A1;
  /*p31.YSEX*/ TpLatch YSEX_LATCH_OAM_A2;
  /*p31.YVEL*/ TpLatch YVEL_LATCH_OAM_A3;
  /*p31.WYNO*/ TpLatch WYNO_LATCH_OAM_A4;
  /*p31.CYRA*/ TpLatch CYRA_LATCH_OAM_A5;
  /*p31.ZUVE*/ TpLatch ZUVE_LATCH_OAM_A6;
  /*p31.ECED*/ TpLatch ECED_LATCH_OAM_A7;

  /*p29.YDYV*/ TpLatch YDYV_LATCH_OAM_B0;
  /*p29.YCEB*/ TpLatch YCEB_LATCH_OAM_B1;
  /*p29.ZUCA*/ TpLatch ZUCA_LATCH_OAM_B2;
  /*p29.WONE*/ TpLatch WONE_LATCH_OAM_B3;
  /*p29.ZAXE*/ TpLatch ZAXE_LATCH_OAM_B4;
  /*p29.XAFU*/ TpLatch XAFU_LATCH_OAM_B5;
  /*p29.YSES*/ TpLatch YSES_LATCH_OAM_B6;
  /*p29.ZECA*/ TpLatch ZECA_LATCH_OAM_B7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics