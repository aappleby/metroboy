#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------
// These signals connect directly to OAM

struct OamBus {

  void tick(TestGB& gb);
  bool commit();

  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- OAM_PINS -----\n");
    text_painter.dprintf("PIN_CLK %d\n", PIN_CLK.a.val);
    text_painter.dprintf("PIN_OE    %d\n", PIN_OE.a.val);
    text_painter.dprintf("PIN_WR_A  %d\n", PIN_WR_A.a.val);
    text_painter.dprintf("PIN_WR_B  %d\n", PIN_WR_B.a.val);

    text_painter.add_text("Axx   ");
    dump2(text_painter, PIN_A7.a);
    dump2(text_painter, PIN_A6.a);
    dump2(text_painter, PIN_A5.a);
    dump2(text_painter, PIN_A4.a);
    dump2(text_painter, PIN_A3.a);
    dump2(text_painter, PIN_A2.a);
    dump2(text_painter, PIN_A1.a);
    dump2(text_painter, PIN_A0.a);
    text_painter.newline();

    text_painter.add_text("A_Dx  ");
    dump2(text_painter, PIN_DA7.a);
    dump2(text_painter, PIN_DA6.a);
    dump2(text_painter, PIN_DA5.a);
    dump2(text_painter, PIN_DA4.a);
    dump2(text_painter, PIN_DA3.a);
    dump2(text_painter, PIN_DA2.a);
    dump2(text_painter, PIN_DA1.a);
    dump2(text_painter, PIN_DA0.a);
    text_painter.newline();

    text_painter.add_text("B_Dx  ");
    dump2(text_painter, PIN_DB7.a);
    dump2(text_painter, PIN_DB6.a);
    dump2(text_painter, PIN_DB5.a);
    dump2(text_painter, PIN_DB4.a);
    dump2(text_painter, PIN_DB3.a);
    dump2(text_painter, PIN_DB2.a);
    dump2(text_painter, PIN_DB1.a);
    dump2(text_painter, PIN_DB0.a);
    text_painter.newline();

    text_painter.newline();
  }

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- OAM_REG -----\n");
    text_painter.dprintf("LATCH_A 0x%02x\n", pack(XYKY_LATCH_OAM_A0.q(), YRUM_LATCH_OAM_A1.q(), YSEX_LATCH_OAM_A2.q(), YVEL_LATCH_OAM_A3.q(), WYNO_LATCH_OAM_A4.q(), CYRA_LATCH_OAM_A5.q(), ZUVE_LATCH_OAM_A6.q(), ECED_LATCH_OAM_A7.q()));
    text_painter.dprintf("LATCH_B 0x%02x\n", pack(YDYV_LATCH_OAM_B0.q(), YCEB_LATCH_OAM_B1.q(), ZUCA_LATCH_OAM_B2.q(), WONE_LATCH_OAM_B3.q(), ZAXE_LATCH_OAM_B4.q(), XAFU_LATCH_OAM_B5.q(), YSES_LATCH_OAM_B6.q(), ZECA_LATCH_OAM_B7.q()));
    text_painter.dprintf("REG_A   0x%02x\n", pack(YLOR_SPRITE_X0.q(), ZYTY_SPRITE_X1.q(), ZYVE_SPRITE_X2.q(), ZEZY_SPRITE_X3.q(), GOMO_SPRITE_X4.q(), BAXO_SPRITE_X5.q(), YZOS_SPRITE_X6.q(), DEPO_SPRITE_X7.q()));
    text_painter.dprintf("REG_B   0x%02x\n", pack(XUSO_SPRITE_Y0.q(), XEGU_SPRITE_Y1.q(), YJEX_SPRITE_Y2.q(), XYJU_SPRITE_Y3.q(), YBOG_SPRITE_Y4.q(), WYSO_SPRITE_Y5.q(), XOTE_SPRITE_Y6.q(), YZAB_SPRITE_Y7.q()));
    text_painter.newline();
  }

  /*p31.YLOR*/ Reg YLOR_SPRITE_X0;
  /*p31.ZYTY*/ Reg ZYTY_SPRITE_X1;
  /*p31.ZYVE*/ Reg ZYVE_SPRITE_X2;
  /*p31.ZEZY*/ Reg ZEZY_SPRITE_X3;
  /*p31.GOMO*/ Reg GOMO_SPRITE_X4;
  /*p31.BAXO*/ Reg BAXO_SPRITE_X5;
  /*p31.YZOS*/ Reg YZOS_SPRITE_X6;
  /*p31.DEPO*/ Reg DEPO_SPRITE_X7;

  /*p29.XUSO*/ Reg XUSO_SPRITE_Y0;
  /*p29.XEGU*/ Reg XEGU_SPRITE_Y1;
  /*p29.YJEX*/ Reg YJEX_SPRITE_Y2;
  /*p29.XYJU*/ Reg XYJU_SPRITE_Y3;
  /*p29.YBOG*/ Reg YBOG_SPRITE_Y4;
  /*p29.WYSO*/ Reg WYSO_SPRITE_Y5;
  /*p29.XOTE*/ Reg XOTE_SPRITE_Y6;
  /*p29.YZAB*/ Reg YZAB_SPRITE_Y7;

private:

  PinOut PIN_CLK;
  PinOut PIN_OE;
  PinOut PIN_WR_A; // definitely write
  PinOut PIN_WR_B; // definitely write

  PinOut PIN_A0;
  PinOut PIN_A1;
  PinOut PIN_A2;
  PinOut PIN_A3;
  PinOut PIN_A4;
  PinOut PIN_A5;
  PinOut PIN_A6;
  PinOut PIN_A7;

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


  Tribuf PIN_DA0;
  Tribuf PIN_DA1;
  Tribuf PIN_DA2;
  Tribuf PIN_DA3;
  Tribuf PIN_DA4;
  Tribuf PIN_DA5;
  Tribuf PIN_DA6;
  Tribuf PIN_DA7;

  Tribuf PIN_DB0;
  Tribuf PIN_DB1;
  Tribuf PIN_DB2;
  Tribuf PIN_DB3;
  Tribuf PIN_DB4;
  Tribuf PIN_DB5;
  Tribuf PIN_DB6;
  Tribuf PIN_DB7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics