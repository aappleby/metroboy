#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct OamBus {
  void reset_cart() {
    COTA_OAM_CLKn.reset(TRI_D1NP); // FIXME not sure about cart boot state
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

    PIN_OAM_CLK .reset(TRI_D1NP, TRI_D1NP);
    PIN_OAM_OE  .reset(TRI_D1NP, TRI_D1NP);
    PIN_OAM_WR_A.reset(TRI_D1NP, TRI_D1NP);
    PIN_OAM_WR_B.reset(TRI_D1NP, TRI_D1NP);

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

    //oam_glitch.da.value = 0x0F;
    //oam_glitch.db.value = 0x0F;
    //oam_glitch.dc.value = 0x0F;
    //oam_glitch.dd.value = 0x0F;
    //oam_glitch.de.value = 0x0F;
    //oam_glitch.df.value = 0x0F;
    //oam_glitch.dg.value = 0x0F;
    //oam_glitch.dh.value = 0x0F;
    //oam_glitch.di.value = 0x0F;
  }

  void reset_boot() {
    COTA_OAM_CLKn.reset(TRI_D0NP);
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

    //oam_glitch.reset();
  }

  void dump(Dumper& d) const;
  void tick(const SchematicTop& top);
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

  /*p25.COTA*/ Sig COTA_OAM_CLKn;
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

  //DelayGlitch oam_glitch;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics