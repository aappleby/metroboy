#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
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

    XADU_SPRITE_IDX0p.reset(REG_D0C1);
    XEDY_SPRITE_IDX1p.reset(REG_D0C1);
    ZUZE_SPRITE_IDX2p.reset(REG_D1C1);
    XOBE_SPRITE_IDX3p.reset(REG_D0C1);
    YDUF_SPRITE_IDX4p.reset(REG_D1C1);
    XECU_SPRITE_IDX5p.reset(REG_D0C1);

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

    PIN_OAM_CLK .reset(TRI_D1NP);
    PIN_OAM_OE  .reset(TRI_D1NP);
    PIN_OAM_WR_A.reset(TRI_D1NP);
    PIN_OAM_WR_B.reset(TRI_D1NP);

    BUS_OAM_A0n .reset(TRI_D1NP);
    BUS_OAM_A1n .reset(TRI_D1NP);
    BUS_OAM_A2n .reset(TRI_D1NP);
    BUS_OAM_A3n .reset(TRI_D1NP);
    BUS_OAM_A4n .reset(TRI_D0NP);
    BUS_OAM_A5n .reset(TRI_D1NP);
    BUS_OAM_A6n .reset(TRI_D0NP);
    BUS_OAM_A7n .reset(TRI_D1NP);

    BUS_OAM_DA0n.reset(TRI_D0PU);
    BUS_OAM_DA1n.reset(TRI_D0PU);
    BUS_OAM_DA2n.reset(TRI_D0PU);
    BUS_OAM_DA3n.reset(TRI_D0PU);
    BUS_OAM_DA4n.reset(TRI_D0PU);
    BUS_OAM_DA5n.reset(TRI_D0PU);
    BUS_OAM_DA6n.reset(TRI_D0PU);
    BUS_OAM_DA7n.reset(TRI_D0PU);
    BUS_OAM_DB0n.reset(TRI_D0PU);
    BUS_OAM_DB1n.reset(TRI_D0PU);
    BUS_OAM_DB2n.reset(TRI_D0PU);
    BUS_OAM_DB3n.reset(TRI_D0PU);
    BUS_OAM_DB4n.reset(TRI_D0PU);
    BUS_OAM_DB5n.reset(TRI_D0PU);
    BUS_OAM_DB6n.reset(TRI_D0PU);
    BUS_OAM_DB7n.reset(TRI_D0PU);
  }

  void reset_bootrom() {
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

    XADU_SPRITE_IDX0p.reset(REG_D0C0);
    XEDY_SPRITE_IDX1p.reset(REG_D0C0);
    ZUZE_SPRITE_IDX2p.reset(REG_D0C0);
    XOBE_SPRITE_IDX3p.reset(REG_D0C0);
    YDUF_SPRITE_IDX4p.reset(REG_D0C0);
    XECU_SPRITE_IDX5p.reset(REG_D0C0);

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
    BUS_OAM_A0n.reset(TRI_D0NP);
    BUS_OAM_A1n.reset(TRI_D0NP);
    BUS_OAM_A2n.reset(TRI_D0NP);
    BUS_OAM_A3n.reset(TRI_D0NP);
    BUS_OAM_A4n.reset(TRI_D0NP);
    BUS_OAM_A5n.reset(TRI_D0NP);
    BUS_OAM_A6n.reset(TRI_D0NP);
    BUS_OAM_A7n.reset(TRI_D0NP);
    BUS_OAM_DA0n.reset(TRI_D0PU);
    BUS_OAM_DA1n.reset(TRI_D0PU);
    BUS_OAM_DA2n.reset(TRI_D0PU);
    BUS_OAM_DA3n.reset(TRI_D0PU);
    BUS_OAM_DA4n.reset(TRI_D0PU);
    BUS_OAM_DA5n.reset(TRI_D0PU);
    BUS_OAM_DA6n.reset(TRI_D0PU);
    BUS_OAM_DA7n.reset(TRI_D0PU);
    BUS_OAM_DB0n.reset(TRI_D0PU);
    BUS_OAM_DB1n.reset(TRI_D0PU);
    BUS_OAM_DB2n.reset(TRI_D0PU);
    BUS_OAM_DB3n.reset(TRI_D0PU);
    BUS_OAM_DB4n.reset(TRI_D0PU);
    BUS_OAM_DB5n.reset(TRI_D0PU);
    BUS_OAM_DB6n.reset(TRI_D0PU);
    BUS_OAM_DB7n.reset(TRI_D0PU);
  }

  void dump(Dumper& d) const;
  void tock(SchematicTop& top);

  //----------------------------------------

  uint8_t get_oam_pin_addr() const {
    /* p28.ZYFO*/ wire ZYFO_OAM_A1p = not1(BUS_OAM_A1n.qp());
    /* p28.YFOT*/ wire YFOT_OAM_A2p = not1(BUS_OAM_A2n.qp());
    /* p28.YFOC*/ wire YFOC_OAM_A3p = not1(BUS_OAM_A3n.qp());
    /* p28.YVOM*/ wire YVOM_OAM_A4p = not1(BUS_OAM_A4n.qp());
    /* p28.YMEV*/ wire YMEV_OAM_A5p = not1(BUS_OAM_A5n.qp());
    /* p28.XEMU*/ wire XEMU_OAM_A6p = not1(BUS_OAM_A6n.qp());
    /* p28.YZET*/ wire YZET_OAM_A7p = not1(BUS_OAM_A7n.qp());

    return (uint8_t)pack_p(ZYFO_OAM_A1p, YFOT_OAM_A2p, YFOC_OAM_A3p, YVOM_OAM_A4p,
                           YMEV_OAM_A5p, XEMU_OAM_A6p, YZET_OAM_A7p, 0);
  }

  uint8_t get_oam_pin_data_a() const {
    return ~(uint8_t)pack_p(BUS_OAM_DA0n.qp(), BUS_OAM_DA1n.qp(), BUS_OAM_DA2n.qp(), BUS_OAM_DA3n.qp(),
                            BUS_OAM_DA4n.qp(), BUS_OAM_DA5n.qp(), BUS_OAM_DA6n.qp(), BUS_OAM_DA7n.qp());
  }

  uint8_t get_oam_pin_data_b() const {
    return ~(uint8_t)pack_p(BUS_OAM_DB0n.qp(), BUS_OAM_DB1n.qp(), BUS_OAM_DB2n.qp(), BUS_OAM_DB3n.qp(),
                            BUS_OAM_DB4n.qp(), BUS_OAM_DB5n.qp(), BUS_OAM_DB6n.qp(), BUS_OAM_DB7n.qp());
  }

  void set_pin_data_a(uint8_t data) {
    BUS_OAM_DA0n.set(!(data & 0x01));
    BUS_OAM_DA1n.set(!(data & 0x02));
    BUS_OAM_DA2n.set(!(data & 0x04));
    BUS_OAM_DA3n.set(!(data & 0x08));
    BUS_OAM_DA4n.set(!(data & 0x10));
    BUS_OAM_DA5n.set(!(data & 0x20));
    BUS_OAM_DA6n.set(!(data & 0x40));
    BUS_OAM_DA7n.set(!(data & 0x80));
  }

  void set_pin_data_b(uint8_t data) {
    BUS_OAM_DB0n.set(!(data & 0x01));
    BUS_OAM_DB1n.set(!(data & 0x02));
    BUS_OAM_DB2n.set(!(data & 0x04));
    BUS_OAM_DB3n.set(!(data & 0x08));
    BUS_OAM_DB4n.set(!(data & 0x10));
    BUS_OAM_DB5n.set(!(data & 0x20));
    BUS_OAM_DB6n.set(!(data & 0x40));
    BUS_OAM_DB7n.set(!(data & 0x80));
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

  /*p30.XADU*/ DFF13 XADU_SPRITE_IDX0p;
  /*p30.XEDY*/ DFF13 XEDY_SPRITE_IDX1p;
  /*p30.ZUZE*/ DFF13 ZUZE_SPRITE_IDX2p;
  /*p30.XOBE*/ DFF13 XOBE_SPRITE_IDX3p;
  /*p30.YDUF*/ DFF13 YDUF_SPRITE_IDX4p;
  /*p30.XECU*/ DFF13 XECU_SPRITE_IDX5p;

  // oam byte 0, byte 2
  /*p29.XUSO*/ DFF8 XUSO_OAM_DA0p; // sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ DFF8 XEGU_OAM_DA1p; // sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ DFF8 YJEX_OAM_DA2p; // sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ DFF8 XYJU_OAM_DA3p; // sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ DFF8 YBOG_OAM_DA4p; // sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ DFF8 WYSO_OAM_DA5p; // sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ DFF8 XOTE_OAM_DA6p; // sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ DFF8 YZAB_OAM_DA7p; // sprite y bit 7, sprite tile index bit 7

  // oam byte 1, byte 3
  /*p31.YLOR*/ DFF8 YLOR_OAM_DB0p; // sprite x bit 0,
  /*p31.ZYTY*/ DFF8 ZYTY_OAM_DB1p; // sprite x bit 1,
  /*p31.ZYVE*/ DFF8 ZYVE_OAM_DB2p; // sprite x bit 2,
  /*p31.ZEZY*/ DFF8 ZEZY_OAM_DB3p; // sprite x bit 3,
  /*p31.GOMO*/ DFF8 GOMO_OAM_DB4p; // sprite x bit 4, sprite palette
  /*p31.BAXO*/ DFF8 BAXO_OAM_DB5p; // sprite x bit 5, sprite x flip
  /*p31.YZOS*/ DFF8 YZOS_OAM_DB6p; // sprite x bit 6, sprite y flip
  /*p31.DEPO*/ DFF8 DEPO_OAM_DB7p; // sprite x bit 7, sprite priority

  Pin PIN_OAM_CLK ;
  Pin PIN_OAM_OE  ;
  Pin PIN_OAM_WR_A;
  Pin PIN_OAM_WR_B;

  Bus BUS_OAM_A0n ;
  Bus BUS_OAM_A1n ;
  Bus BUS_OAM_A2n ;
  Bus BUS_OAM_A3n ;
  Bus BUS_OAM_A4n ;
  Bus BUS_OAM_A5n ;
  Bus BUS_OAM_A6n ;
  Bus BUS_OAM_A7n ;

  Bus BUS_OAM_DA0n;
  Bus BUS_OAM_DA1n;
  Bus BUS_OAM_DA2n;
  Bus BUS_OAM_DA3n;
  Bus BUS_OAM_DA4n;
  Bus BUS_OAM_DA5n;
  Bus BUS_OAM_DA6n;
  Bus BUS_OAM_DA7n;

  Bus BUS_OAM_DB0n;
  Bus BUS_OAM_DB1n;
  Bus BUS_OAM_DB2n;
  Bus BUS_OAM_DB3n;
  Bus BUS_OAM_DB4n;
  Bus BUS_OAM_DB5n;
  Bus BUS_OAM_DB6n;
  Bus BUS_OAM_DB7n;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics