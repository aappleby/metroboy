#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoy;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
  void reset_cart() {
    MAKA_LATCH_EXTp_evn.reset(REG_D0C1);
    WUJE_CPU_OAM_WRn_evn.reset(1);

    XYKY_OAM_LATCH_DB0n_odd.reset(1);
    YRUM_OAM_LATCH_DB1n_odd.reset(1);
    YSEX_OAM_LATCH_DB2n_odd.reset(1);
    YVEL_OAM_LATCH_DB3n_odd.reset(1);
    WYNO_OAM_LATCH_DB4n_odd.reset(1);
    CYRA_OAM_LATCH_DB5n_odd.reset(1);
    ZUVE_OAM_LATCH_DB6n_odd.reset(1);
    ECED_OAM_LATCH_DB7n_odd.reset(1);

    YDYV_OAM_LATCH_DA0n_odd.reset(1);
    YCEB_OAM_LATCH_DA1n_odd.reset(1);
    ZUCA_OAM_LATCH_DA2n_odd.reset(1);
    WONE_OAM_LATCH_DA3n_odd.reset(1);
    ZAXE_OAM_LATCH_DA4n_odd.reset(1);
    XAFU_OAM_LATCH_DA5n_odd.reset(1);
    YSES_OAM_LATCH_DA6n_odd.reset(1);
    ZECA_OAM_LATCH_DA7n_odd.reset(1);

    XUSO_OAM_DA0p_evn.reset(REG_D0C1);
    XEGU_OAM_DA1p_evn.reset(REG_D0C1);
    YJEX_OAM_DA2p_evn.reset(REG_D0C1);
    XYJU_OAM_DA3p_evn.reset(REG_D0C1);
    YBOG_OAM_DA4p_evn.reset(REG_D0C1);
    WYSO_OAM_DA5p_evn.reset(REG_D0C1);
    XOTE_OAM_DA6p_evn.reset(REG_D0C1);
    YZAB_OAM_DA7p_evn.reset(REG_D0C1);

    YLOR_OAM_DB0p_evn.reset(REG_D0C1);
    ZYTY_OAM_DB1p_evn.reset(REG_D0C1);
    ZYVE_OAM_DB2p_evn.reset(REG_D0C1);
    ZEZY_OAM_DB3p_evn.reset(REG_D0C1);
    GOMO_OAM_DB4p_evn.reset(REG_D0C1);
    BAXO_OAM_DB5p_evn.reset(REG_D0C1);
    YZOS_OAM_DB6p_evn.reset(REG_D0C1);
    DEPO_OAM_DB7p_evn.reset(REG_D0C1);
  }

  //----------------------------------------

  /*p04.MAKA*/ DFF17 MAKA_LATCH_EXTp_evn;       // AxxxExxx
  /*p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn_evn;   // AxxxExxx

  /*p31.XYKY*/ TpLatch XYKY_OAM_LATCH_DB0n_odd; // xBxxxFxx // Proooobably all odd clocks?
  /*p31.YRUM*/ TpLatch YRUM_OAM_LATCH_DB1n_odd; // xBxxxFxx
  /*p31.YSEX*/ TpLatch YSEX_OAM_LATCH_DB2n_odd; // xBxxxFxx
  /*p31.YVEL*/ TpLatch YVEL_OAM_LATCH_DB3n_odd; // xBxxxFxx
  /*p31.WYNO*/ TpLatch WYNO_OAM_LATCH_DB4n_odd; // xBxxxFxx
  /*p31.CYRA*/ TpLatch CYRA_OAM_LATCH_DB5n_odd; // xBxxxFxx
  /*p31.ZUVE*/ TpLatch ZUVE_OAM_LATCH_DB6n_odd; // xBxxxFxx
  /*p31.ECED*/ TpLatch ECED_OAM_LATCH_DB7n_odd; // xBxxxFxx

  /*p29.YDYV*/ TpLatch YDYV_OAM_LATCH_DA0n_odd; // xBxDxFxx // Proooobably all odd clocks?
  /*p29.YCEB*/ TpLatch YCEB_OAM_LATCH_DA1n_odd; // xBxDxFxx
  /*p29.ZUCA*/ TpLatch ZUCA_OAM_LATCH_DA2n_odd; // xBxDxFxx
  /*p29.WONE*/ TpLatch WONE_OAM_LATCH_DA3n_odd; // xBxDxFxx
  /*p29.ZAXE*/ TpLatch ZAXE_OAM_LATCH_DA4n_odd; // xBxDxFxx
  /*p29.XAFU*/ TpLatch XAFU_OAM_LATCH_DA5n_odd; // xBxDxFxx
  /*p29.YSES*/ TpLatch YSES_OAM_LATCH_DA6n_odd; // xBxDxFxx
  /*p29.ZECA*/ TpLatch ZECA_OAM_LATCH_DA7n_odd; // xBxDxFxx

  // oam byte 0, byte 2
  /*p29.XUSO*/ DFF8n XUSO_OAM_DA0p_evn; // AxxxExxx - sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ DFF8n XEGU_OAM_DA1p_evn; // AxxxExxx - sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ DFF8n YJEX_OAM_DA2p_evn; // AxxxExxx - sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ DFF8n XYJU_OAM_DA3p_evn; // AxxxExxx - sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ DFF8n YBOG_OAM_DA4p_evn; // AxxxExxx - sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ DFF8n WYSO_OAM_DA5p_evn; // AxxxExxx - sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ DFF8n XOTE_OAM_DA6p_evn; // AxxxExxx - sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ DFF8n YZAB_OAM_DA7p_evn; // AxxxExxx - sprite y bit 7, sprite tile index bit 7

  // oam byte 1, byte 3
  /*p31.YLOR*/ DFF8n YLOR_OAM_DB0p_evn; // AxxxExxx - sprite x bit 0,
  /*p31.ZYTY*/ DFF8n ZYTY_OAM_DB1p_evn; // AxxxExxx - sprite x bit 1,
  /*p31.ZYVE*/ DFF8n ZYVE_OAM_DB2p_evn; // AxxxExxx - sprite x bit 2,
  /*p31.ZEZY*/ DFF8n ZEZY_OAM_DB3p_evn; // AxxxExxx - sprite x bit 3,
  /*p31.GOMO*/ DFF8n GOMO_OAM_DB4p_evn; // AxxxExxx - sprite x bit 4, sprite palette
  /*p31.BAXO*/ DFF8n BAXO_OAM_DB5p_evn; // AxxxExxx - sprite x bit 5, sprite x flip
  /*p31.YZOS*/ DFF8n YZOS_OAM_DB6p_evn; // AxxxExxx - sprite x bit 6, sprite y flip
  /*p31.DEPO*/ DFF8n DEPO_OAM_DB7p_evn; // AxxxExxx - sprite x bit 7, sprite priority
};

//-----------------------------------------------------------------------------
