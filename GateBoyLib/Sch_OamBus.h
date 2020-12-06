#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoy;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
  void reset_cart() {
    MAKA_LATCH_EXTp.reset(REG_D0C1);
    WUJE_CPU_OAM_WRn.reset(1);

    XYKY_OAM_LATCH_DB0n.reset(1);
    YRUM_OAM_LATCH_DB1n.reset(1);
    YSEX_OAM_LATCH_DB2n.reset(1);
    YVEL_OAM_LATCH_DB3n.reset(1);
    WYNO_OAM_LATCH_DB4n.reset(1);
    CYRA_OAM_LATCH_DB5n.reset(1);
    ZUVE_OAM_LATCH_DB6n.reset(1);
    ECED_OAM_LATCH_DB7n.reset(1);

    YDYV_OAM_LATCH_DA0n.reset(1);
    YCEB_OAM_LATCH_DA1n.reset(1);
    ZUCA_OAM_LATCH_DA2n.reset(1);
    WONE_OAM_LATCH_DA3n.reset(1);
    ZAXE_OAM_LATCH_DA4n.reset(1);
    XAFU_OAM_LATCH_DA5n.reset(1);
    YSES_OAM_LATCH_DA6n.reset(1);
    ZECA_OAM_LATCH_DA7n.reset(1);

    XUSO_OAM_DA0p.reset(REG_D0C1);
    XEGU_OAM_DA1p.reset(REG_D0C1);
    YJEX_OAM_DA2p.reset(REG_D0C1);
    XYJU_OAM_DA3p.reset(REG_D0C1);
    YBOG_OAM_DA4p.reset(REG_D0C1);
    WYSO_OAM_DA5p.reset(REG_D0C1);
    XOTE_OAM_DA6p.reset(REG_D0C1);
    YZAB_OAM_DA7p.reset(REG_D0C1);

    YLOR_OAM_DB0p.reset(REG_D0C1);
    ZYTY_OAM_DB1p.reset(REG_D0C1);
    ZYVE_OAM_DB2p.reset(REG_D0C1);
    ZEZY_OAM_DB3p.reset(REG_D0C1);
    GOMO_OAM_DB4p.reset(REG_D0C1);
    BAXO_OAM_DB5p.reset(REG_D0C1);
    YZOS_OAM_DB6p.reset(REG_D0C1);
    DEPO_OAM_DB7p.reset(REG_D0C1);
  }

  void reset_boot() {
    MAKA_LATCH_EXTp.reset(REG_D0C0);
    WUJE_CPU_OAM_WRn.reset(0);

    XYKY_OAM_LATCH_DB0n.reset(0);
    YRUM_OAM_LATCH_DB1n.reset(0);
    YSEX_OAM_LATCH_DB2n.reset(0);
    YVEL_OAM_LATCH_DB3n.reset(0);
    WYNO_OAM_LATCH_DB4n.reset(0);
    CYRA_OAM_LATCH_DB5n.reset(0);
    ZUVE_OAM_LATCH_DB6n.reset(0);
    ECED_OAM_LATCH_DB7n.reset(0);

    YDYV_OAM_LATCH_DA0n.reset(0);
    YCEB_OAM_LATCH_DA1n.reset(0);
    ZUCA_OAM_LATCH_DA2n.reset(0);
    WONE_OAM_LATCH_DA3n.reset(0);
    ZAXE_OAM_LATCH_DA4n.reset(0);
    XAFU_OAM_LATCH_DA5n.reset(0);
    YSES_OAM_LATCH_DA6n.reset(0);
    ZECA_OAM_LATCH_DA7n.reset(0);

    XUSO_OAM_DA0p.reset(REG_D0C0);
    XEGU_OAM_DA1p.reset(REG_D0C0);
    YJEX_OAM_DA2p.reset(REG_D0C0);
    XYJU_OAM_DA3p.reset(REG_D0C0);
    YBOG_OAM_DA4p.reset(REG_D0C0);
    WYSO_OAM_DA5p.reset(REG_D0C0);
    XOTE_OAM_DA6p.reset(REG_D0C0);
    YZAB_OAM_DA7p.reset(REG_D0C0);

    YLOR_OAM_DB0p.reset(REG_D0C0);
    ZYTY_OAM_DB1p.reset(REG_D0C0);
    ZYVE_OAM_DB2p.reset(REG_D0C0);
    ZEZY_OAM_DB3p.reset(REG_D0C0);
    GOMO_OAM_DB4p.reset(REG_D0C0);
    BAXO_OAM_DB5p.reset(REG_D0C0);
    YZOS_OAM_DB6p.reset(REG_D0C0);
    DEPO_OAM_DB7p.reset(REG_D0C0);
  }

  //----------------------------------------

  /*p04.MAKA*/ DFF17 MAKA_LATCH_EXTp;
  /*p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn;

  /*p31.XYKY*/ TpLatch XYKY_OAM_LATCH_DB0n;
  /*p31.YRUM*/ TpLatch YRUM_OAM_LATCH_DB1n;
  /*p31.YSEX*/ TpLatch YSEX_OAM_LATCH_DB2n;
  /*p31.YVEL*/ TpLatch YVEL_OAM_LATCH_DB3n;
  /*p31.WYNO*/ TpLatch WYNO_OAM_LATCH_DB4n;
  /*p31.CYRA*/ TpLatch CYRA_OAM_LATCH_DB5n;
  /*p31.ZUVE*/ TpLatch ZUVE_OAM_LATCH_DB6n;
  /*p31.ECED*/ TpLatch ECED_OAM_LATCH_DB7n;

  /*p29.YDYV*/ TpLatch YDYV_OAM_LATCH_DA0n;
  /*p29.YCEB*/ TpLatch YCEB_OAM_LATCH_DA1n;
  /*p29.ZUCA*/ TpLatch ZUCA_OAM_LATCH_DA2n;
  /*p29.WONE*/ TpLatch WONE_OAM_LATCH_DA3n;
  /*p29.ZAXE*/ TpLatch ZAXE_OAM_LATCH_DA4n;
  /*p29.XAFU*/ TpLatch XAFU_OAM_LATCH_DA5n;
  /*p29.YSES*/ TpLatch YSES_OAM_LATCH_DA6n;
  /*p29.ZECA*/ TpLatch ZECA_OAM_LATCH_DA7n;

  // oam byte 0, byte 2
  /*p29.XUSO*/ DFF8n XUSO_OAM_DA0p; // sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ DFF8n XEGU_OAM_DA1p; // sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ DFF8n YJEX_OAM_DA2p; // sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ DFF8n XYJU_OAM_DA3p; // sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ DFF8n YBOG_OAM_DA4p; // sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ DFF8n WYSO_OAM_DA5p; // sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ DFF8n XOTE_OAM_DA6p; // sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ DFF8n YZAB_OAM_DA7p; // sprite y bit 7, sprite tile index bit 7

  // oam byte 1, byte 3
  /*p31.YLOR*/ DFF8n YLOR_OAM_DB0p; // sprite x bit 0,
  /*p31.ZYTY*/ DFF8n ZYTY_OAM_DB1p; // sprite x bit 1,
  /*p31.ZYVE*/ DFF8n ZYVE_OAM_DB2p; // sprite x bit 2,
  /*p31.ZEZY*/ DFF8n ZEZY_OAM_DB3p; // sprite x bit 3,
  /*p31.GOMO*/ DFF8n GOMO_OAM_DB4p; // sprite x bit 4, sprite palette
  /*p31.BAXO*/ DFF8n BAXO_OAM_DB5p; // sprite x bit 5, sprite x flip
  /*p31.YZOS*/ DFF8n YZOS_OAM_DB6p; // sprite x bit 6, sprite y flip
  /*p31.DEPO*/ DFF8n DEPO_OAM_DB7p; // sprite x bit 7, sprite priority
};

//-----------------------------------------------------------------------------
