#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct JoypadRegisters {

  void tick(TestGB& gb);
  bool commit();

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- JOY REG -----\n");
    JP_GLITCH0.dump(text_painter, "JP_GLITCH0  ");
    JP_GLITCH1.dump(text_painter, "JP_GLITCH1  ");
    JP_GLITCH2.dump(text_painter, "JP_GLITCH2  ");
    JP_GLITCH3.dump(text_painter, "JP_GLITCH3  ");
    JOYP_RA.dump(text_painter, "JOYP_RA     ");
    JOYP_LB.dump(text_painter, "JOYP_LB     ");
    JOYP_UC.dump(text_painter, "JOYP_UC     ");
    JOYP_DS.dump(text_painter, "JOYP_DS     ");
    JOYP_UDLR.dump(text_painter, "JOYP_UDLR   ");
    JOYP_ABCS.dump(text_painter, "JOYP_ABCS   ");
    DBG_FF00_D6.dump(text_painter, "DBG_FF00_D6 ");
    DBG_FF00_D7.dump(text_painter, "DBG_FF00_D7 ");
    JOYP_L0.dump(text_painter, "JOYP_L0     ");
    JOYP_L1.dump(text_painter, "JOYP_L1     ");
    JOYP_L2.dump(text_painter, "JOYP_L2     ");
    JOYP_L3.dump(text_painter, "JOYP_L3     ");
    WAKE_CPU.dump(text_painter, "WAKE_CPU    ");
    text_painter.newline();
  }

  /*p02.BATU*/ Reg JP_GLITCH0;
  /*p02.ACEF*/ Reg JP_GLITCH1;
  /*p02.AGEM*/ Reg JP_GLITCH2;
  /*p02.APUG*/ Reg JP_GLITCH3;
  /*p05.JUTE*/ Reg JOYP_RA;
  /*p05.KECY*/ Reg JOYP_LB;
  /*p05.JALE*/ Reg JOYP_UC;
  /*p05.KYME*/ Reg JOYP_DS;
  /*p05.KELY*/ Reg JOYP_UDLR;
  /*p05.COFY*/ Reg JOYP_ABCS;
  /*p05.KUKO*/ Reg DBG_FF00_D6;
  /*p05.KERU*/ Reg DBG_FF00_D7;
  /*p05.KEVU*/ Reg JOYP_L0;
  /*p05.KAPA*/ Reg JOYP_L1;
  /*p05.KEJA*/ Reg JOYP_L2;
  /*p05.KOLO*/ Reg JOYP_L3;
  /*p02.AWOB*/ Reg WAKE_CPU;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics