#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct OamSignals {
};

struct OamRegisters {

  OamSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- OAM_REG -----\n");
    text_painter.dprintf("LATCH_A 0x%02x\n", pack(LATCH_OAM_A0.q(), LATCH_OAM_A1.q(), LATCH_OAM_A2.q(), LATCH_OAM_A3.q(), LATCH_OAM_A4.q(), LATCH_OAM_A5.q(), LATCH_OAM_A6.q(), LATCH_OAM_A7.q()));
    text_painter.dprintf("LATCH_B 0x%02x\n", pack(LATCH_OAM_B0.q(), LATCH_OAM_B1.q(), LATCH_OAM_B2.q(), LATCH_OAM_B3.q(), LATCH_OAM_B4.q(), LATCH_OAM_B5.q(), LATCH_OAM_B6.q(), LATCH_OAM_B7.q()));
    text_painter.dprintf("REG_A   0x%02x\n", pack(REG_OAM_A0.q(), REG_OAM_A1.q(), REG_OAM_A2.q(), REG_OAM_A3.q(), REG_OAM_A4.q(), REG_OAM_A5.q(), REG_OAM_A6.q(), REG_OAM_A7.q()));
    text_painter.dprintf("REG_B   0x%02x\n", pack(REG_OAM_B0.q(), REG_OAM_B1.q(), REG_OAM_B2.q(), REG_OAM_B3.q(), REG_OAM_B4.q(), REG_OAM_B5.q(), REG_OAM_B6.q(), REG_OAM_B7.q()));
    text_painter.newline();
  }

  /*p31.XYKY*/ TpLatch LATCH_OAM_A0;
  /*p31.YRUM*/ TpLatch LATCH_OAM_A1;
  /*p31.YSEX*/ TpLatch LATCH_OAM_A2;
  /*p31.YVEL*/ TpLatch LATCH_OAM_A3;
  /*p31.WYNO*/ TpLatch LATCH_OAM_A4;
  /*p31.CYRA*/ TpLatch LATCH_OAM_A5;
  /*p31.ZUVE*/ TpLatch LATCH_OAM_A6;
  /*p31.ECED*/ TpLatch LATCH_OAM_A7;

  /*p29.YDYV*/ TpLatch LATCH_OAM_B0;
  /*p29.YCEB*/ TpLatch LATCH_OAM_B1;
  /*p29.ZUCA*/ TpLatch LATCH_OAM_B2;
  /*p29.WONE*/ TpLatch LATCH_OAM_B3;
  /*p29.ZAXE*/ TpLatch LATCH_OAM_B4;
  /*p29.XAFU*/ TpLatch LATCH_OAM_B5;
  /*p29.YSES*/ TpLatch LATCH_OAM_B6;
  /*p29.ZECA*/ TpLatch LATCH_OAM_B7;

  /*p31.YLOR*/ Reg REG_OAM_A0;
  /*p31.ZYTY*/ Reg REG_OAM_A1;
  /*p31.ZYVE*/ Reg REG_OAM_A2;
  /*p31.ZEZY*/ Reg REG_OAM_A3;
  /*p31.GOMO*/ Reg REG_OAM_A4;
  /*p31.BAXO*/ Reg REG_OAM_A5;
  /*p31.YZOS*/ Reg REG_OAM_A6;
  /*p31.DEPO*/ Reg REG_OAM_A7;

  /*p29.XUSO*/ Reg REG_OAM_B0;
  /*p29.XEGU*/ Reg REG_OAM_B1;
  /*p29.YJEX*/ Reg REG_OAM_B2;
  /*p29.XYJU*/ Reg REG_OAM_B3;
  /*p29.YBOG*/ Reg REG_OAM_B4;
  /*p29.WYSO*/ Reg REG_OAM_B5;
  /*p29.XOTE*/ Reg REG_OAM_B6;
  /*p29.YZAB*/ Reg REG_OAM_B7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics