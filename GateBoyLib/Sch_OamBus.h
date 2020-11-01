#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
  void dump(Dumper& d) const {
    d("\002===== OAM Bus =====\001\n");
    d("PIN_OAM_CLK    %c\n", PIN_OAM_CLK.c());
    d("PIN_OAM_OE     %c\n", PIN_OAM_OE.c());
    d("PIN_OAM_WR_A   %c\n", PIN_OAM_WR_A.c());
    d("PIN_OAM_WR_B   %c\n", PIN_OAM_WR_B.c());

    int oam_tri_addr = pack_p(BUS_OAM_A0n.qn(), BUS_OAM_A1n.qn(), BUS_OAM_A2n.qn(), BUS_OAM_A3n.qn(),
                              BUS_OAM_A4n.qn(), BUS_OAM_A5n.qn(), BUS_OAM_A6n.qn(), BUS_OAM_A7n.qn());

    d("OAM TRI ADDR   %03d %02x %c%c%c%c%c%c%c%c\n",
      oam_tri_addr,
      oam_tri_addr,
      BUS_OAM_A7n.cn(), BUS_OAM_A6n.cn(), BUS_OAM_A5n.cn(), BUS_OAM_A4n.cn(),
      BUS_OAM_A3n.cn(), BUS_OAM_A2n.cn(), BUS_OAM_A1n.cn(), BUS_OAM_A0n.cn());

    int oam_latch_data_a = pack_p(YDYV_OAM_LATCH_DA0n.c(), YCEB_OAM_LATCH_DA1n.c(), ZUCA_OAM_LATCH_DA2n.c(), WONE_OAM_LATCH_DA3n.c(),
                                  ZAXE_OAM_LATCH_DA4n.c(), XAFU_OAM_LATCH_DA5n.c(), YSES_OAM_LATCH_DA6n.c(), ZECA_OAM_LATCH_DA7n.c());

    int oam_latch_data_b = pack_p(XYKY_OAM_LATCH_DB0n.c(), YRUM_OAM_LATCH_DB1n.c(), YSEX_OAM_LATCH_DB2n.c(), YVEL_OAM_LATCH_DB3n.c(),
                                  WYNO_OAM_LATCH_DB4n.c(), CYRA_OAM_LATCH_DB5n.c(), ZUVE_OAM_LATCH_DB6n.c(), ECED_OAM_LATCH_DB7n.c());

    /*
    d("OAM BUS ADDR   %03d %02x -%c%c%c%c%c%c%c\n",
      get_oam_pin_addr(),
      get_oam_pin_addr(),
      BUS_OAM_A7n.cn(), BUS_OAM_A6n.cn(), BUS_OAM_A5n.cn(), BUS_OAM_A4n.cn(),
      BUS_OAM_A3n.cn(), BUS_OAM_A2n.cn(), BUS_OAM_A1n.cn());

    d("OAM BUS DATA A %03d %02x %c%c%c%c%c%c%c%c\n",
      get_oam_pin_data_a(),
      get_oam_pin_data_a(),
      BUS_OAM_DA7n.c(), BUS_OAM_DA6n.c(), BUS_OAM_DA5n.c(), BUS_OAM_DA4n.c(),
      BUS_OAM_DA3n.c(), BUS_OAM_DA2n.c(), BUS_OAM_DA1n.c(), BUS_OAM_DA0n.c());

    d("OAM BUS DATA B %03d %02x %c%c%c%c%c%c%c%c\n",
      get_oam_pin_data_b(),
      get_oam_pin_data_b(),
      BUS_OAM_DB7n.c(), BUS_OAM_DB6n.c(), BUS_OAM_DB5n.c(), BUS_OAM_DB4n.c(),
      BUS_OAM_DB3n.c(), BUS_OAM_DB2n.c(), BUS_OAM_DB1n.c(), BUS_OAM_DB0n.c());
    */

    d("MAKA_HOLD_MEMp   %c\n", MAKA_HOLD_MEMp.c());
    d("WUJE_CPU_OAM_WRn %c\n", WUJE_CPU_OAM_WRn.c());

    d("OAM LATCH A    %03d %02x %c%c%c%c%c%c%c%c\n",
      oam_latch_data_a,
      oam_latch_data_a,
      ZECA_OAM_LATCH_DA7n.c(), YSES_OAM_LATCH_DA6n.c(), XAFU_OAM_LATCH_DA5n.c(), ZAXE_OAM_LATCH_DA4n.c(),
      WONE_OAM_LATCH_DA3n.c(), ZUCA_OAM_LATCH_DA2n.c(), YCEB_OAM_LATCH_DA1n.c(), YDYV_OAM_LATCH_DA0n.c());

    d("OAM LATCH B    %03d %02x %c%c%c%c%c%c%c%c\n",
      oam_latch_data_b,
      oam_latch_data_b,
      ECED_OAM_LATCH_DB7n.c(), ZUVE_OAM_LATCH_DB6n.c(), CYRA_OAM_LATCH_DB5n.c(), WYNO_OAM_LATCH_DB4n.c(),
      YVEL_OAM_LATCH_DB3n.c(), YSEX_OAM_LATCH_DB2n.c(), YRUM_OAM_LATCH_DB1n.c(), XYKY_OAM_LATCH_DB0n.c());

    int oam_temp_a = pack_p(XUSO_OAM_DA0p.qp08(), XEGU_OAM_DA1p.qp08(), YJEX_OAM_DA2p.qp08(), XYJU_OAM_DA3p.qp08(),
                            YBOG_OAM_DA4p.qp08(), WYSO_OAM_DA5p.qp08(), XOTE_OAM_DA6p.qp08(), YZAB_OAM_DA7p.qp08());

    int oam_temp_b = pack_p(YLOR_OAM_DB0p.qp08(), ZYTY_OAM_DB1p.qp08(), ZYVE_OAM_DB2p.qp08(), ZEZY_OAM_DB3p.qp08(),
                            GOMO_OAM_DB4p.qp08(), BAXO_OAM_DB5p.qp08(), YZOS_OAM_DB6p.qp08(), DEPO_OAM_DB7p.qp08());


    d("OAM TEMP A     %03d %02x %c%c%c%c%c%c%c%c\n",
      oam_temp_a,
      oam_temp_a,
      YZAB_OAM_DA7p.c(), XOTE_OAM_DA6p.c(), WYSO_OAM_DA5p.c(), YBOG_OAM_DA4p.c(),
      XYJU_OAM_DA3p.c(), YJEX_OAM_DA2p.c(), XEGU_OAM_DA1p.c(), XUSO_OAM_DA0p.c());

    d("OAM TEMP B     %03d %2x %c%c%c%c%c%c%c%c\n",
      oam_temp_b,
      oam_temp_b,
      DEPO_OAM_DB7p.c(), YZOS_OAM_DB6p.c(), BAXO_OAM_DB5p.c(), GOMO_OAM_DB4p.c(),
      ZEZY_OAM_DB3p.c(), ZYVE_OAM_DB2p.c(), ZYTY_OAM_DB1p.c(), YLOR_OAM_DB0p.c());


    d("\n");
  }


  void reset_cart() {
    MAKA_HOLD_MEMp.reset(REG_D0C1);
    WUJE_CPU_OAM_WRn.reset(TRI_D1NP);

    XYKY_OAM_LATCH_DB0n.reset(TRI_D1NP);
    YRUM_OAM_LATCH_DB1n.reset(TRI_D1NP);
    YSEX_OAM_LATCH_DB2n.reset(TRI_D1NP);
    YVEL_OAM_LATCH_DB3n.reset(TRI_D1NP);
    WYNO_OAM_LATCH_DB4n.reset(TRI_D1NP);
    CYRA_OAM_LATCH_DB5n.reset(TRI_D1NP);
    ZUVE_OAM_LATCH_DB6n.reset(TRI_D1NP);
    ECED_OAM_LATCH_DB7n.reset(TRI_D1NP);

    YDYV_OAM_LATCH_DA0n.reset(TRI_D1NP);
    YCEB_OAM_LATCH_DA1n.reset(TRI_D1NP);
    ZUCA_OAM_LATCH_DA2n.reset(TRI_D1NP);
    WONE_OAM_LATCH_DA3n.reset(TRI_D1NP);
    ZAXE_OAM_LATCH_DA4n.reset(TRI_D1NP);
    XAFU_OAM_LATCH_DA5n.reset(TRI_D1NP);
    YSES_OAM_LATCH_DA6n.reset(TRI_D1NP);
    ZECA_OAM_LATCH_DA7n.reset(TRI_D1NP);

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

    PIN_OAM_CLK .reset(TRI_D1NP, TRI_D1NP);
    PIN_OAM_OE  .reset(TRI_D1NP, TRI_D1NP);
    PIN_OAM_WR_A.reset(TRI_D1NP, TRI_D1NP);
    PIN_OAM_WR_B.reset(TRI_D1NP, TRI_D1NP);
  }

  void reset_boot() {
    MAKA_HOLD_MEMp.reset(REG_D0C0);
    WUJE_CPU_OAM_WRn.reset(TRI_D0NP);

    XYKY_OAM_LATCH_DB0n.reset(TRI_D0NP);
    YRUM_OAM_LATCH_DB1n.reset(TRI_D0NP);
    YSEX_OAM_LATCH_DB2n.reset(TRI_D0NP);
    YVEL_OAM_LATCH_DB3n.reset(TRI_D0NP);
    WYNO_OAM_LATCH_DB4n.reset(TRI_D0NP);
    CYRA_OAM_LATCH_DB5n.reset(TRI_D0NP);
    ZUVE_OAM_LATCH_DB6n.reset(TRI_D0NP);
    ECED_OAM_LATCH_DB7n.reset(TRI_D0NP);

    YDYV_OAM_LATCH_DA0n.reset(TRI_D0NP);
    YCEB_OAM_LATCH_DA1n.reset(TRI_D0NP);
    ZUCA_OAM_LATCH_DA2n.reset(TRI_D0NP);
    WONE_OAM_LATCH_DA3n.reset(TRI_D0NP);
    ZAXE_OAM_LATCH_DA4n.reset(TRI_D0NP);
    XAFU_OAM_LATCH_DA5n.reset(TRI_D0NP);
    YSES_OAM_LATCH_DA6n.reset(TRI_D0NP);
    ZECA_OAM_LATCH_DA7n.reset(TRI_D0NP);

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

    PIN_OAM_CLK.reset(TRI_D0NP);
    PIN_OAM_OE.reset(TRI_D1NP);
    PIN_OAM_WR_A.reset(TRI_D1NP);
    PIN_OAM_WR_B.reset(TRI_D1NP);
  }

  //----------------------------------------

  /*p04.MAKA*/ DFF17 MAKA_HOLD_MEMp;
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

  Pin PIN_OAM_CLK ;
  Pin PIN_OAM_OE  ;
  Pin PIN_OAM_WR_A;
  Pin PIN_OAM_WR_B;

  Bus2 BUS_OAM_A0n ;
  Bus2 BUS_OAM_A1n ;
  Bus2 BUS_OAM_A2n ;
  Bus2 BUS_OAM_A3n ;
  Bus2 BUS_OAM_A4n ;
  Bus2 BUS_OAM_A5n ;
  Bus2 BUS_OAM_A6n ;
  Bus2 BUS_OAM_A7n ;

  Bus2 BUS_OAM_DA0n;
  Bus2 BUS_OAM_DA1n;
  Bus2 BUS_OAM_DA2n;
  Bus2 BUS_OAM_DA3n;
  Bus2 BUS_OAM_DA4n;
  Bus2 BUS_OAM_DA5n;
  Bus2 BUS_OAM_DA6n;
  Bus2 BUS_OAM_DA7n;

  Bus2 BUS_OAM_DB0n;
  Bus2 BUS_OAM_DB1n;
  Bus2 BUS_OAM_DB2n;
  Bus2 BUS_OAM_DB3n;
  Bus2 BUS_OAM_DB4n;
  Bus2 BUS_OAM_DB5n;
  Bus2 BUS_OAM_DB6n;
  Bus2 BUS_OAM_DB7n;

  //DelayGlitch oam_glitch;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics