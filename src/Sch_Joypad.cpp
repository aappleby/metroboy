#include "Sch_Joypad.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void JoypadRegisters::tick(SchematicTop& top) {

  Signal BYZO_FF00_RDn;
  Signal ATOZ_FF00_WRn;
  {
    /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());

    /*p10.BYKO*/ wire BYKO_A05n = not(top.CPU_PIN_A05);
    /*p10.AKUG*/ wire AKUG_A06n = not(top.CPU_PIN_A06);
    /*p10.AMUS*/ wire AMUS_0xx00000 = nor(top.CPU_PIN_A00, top.CPU_PIN_A01, top.CPU_PIN_A02, top.CPU_PIN_A03, top.CPU_PIN_A04, top.CPU_PIN_A07);
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(top.CPU_PIN_A15, top.CPU_PIN_A14, top.CPU_PIN_A13, top.CPU_PIN_A12, top.CPU_PIN_A11, top.CPU_PIN_A10, top.CPU_PIN_A09);
    /*p07.TONA*/ wire TONA_A08n = not(top.CPU_PIN_A08);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
    /*p10.ANAP*/ wire ANAP_0xx00000 = and (AMUS_0xx00000, SYKE_FF00_FFFFp);
    /*p10.ACAT*/ wire ACAT_FF00_RD = and (TEDO_CPU_RD, ANAP_0xx00000, AKUG_A06n, BYKO_A05n);

    /*p05.BYZO*/ BYZO_FF00_RDn = not(ACAT_FF00_RD);
    /*p10.ATOZ*/ ATOZ_FF00_WRn = nand(TAPU_CPU_WR_xxxxxFGH, ANAP_0xx00000, AKUG_A06n, BYKO_A05n);
  }

  {
    /*p01.BALY*/ wire BALY_xBxxxxxx = not(top.BYJU_AxCDEFGH());
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
    /*p02.KERY*/ wire ANY_BUTTON = or(top.EXT_P13_C, top.EXT_P12_C, top.EXT_P11_C, top.EXT_P10_C);
    /*p02.AWOB*/ AWOB_WAKE_CPU.setx(BOGA_AxCDEFGH, ANY_BUTTON);
    // cpu_pins.TO_CPU2.set(WAKE_CPU.q());
  }

  {
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
    /*p01.BALY*/ wire BALY_xBxxxxxx = not(top.BYJU_AxCDEFGH());
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
    /*p02.BATU*/ JP_GLITCH0.set(BOGA_AxCDEFGH, ALUR_RSTn, ANY_BUTTON);
    /*p02.ACEF*/ JP_GLITCH1.set(BOGA_AxCDEFGH, ALUR_RSTn, JP_GLITCH0.q());
    /*p02.AGEM*/ JP_GLITCH2.set(BOGA_AxCDEFGH, ALUR_RSTn, JP_GLITCH1.q());
    /*p02.APUG*/ JP_GLITCH3.set(BOGA_AxCDEFGH, ALUR_RSTn, JP_GLITCH2.q());
  }

  {
    /*p05.KEVU*/ JOYP_L0.setx(BYZO_FF00_RDn, top.EXT_P10_C);
    /*p05.KAPA*/ JOYP_L1.setx(BYZO_FF00_RDn, top.EXT_P11_C);
    /*p05.KEJA*/ JOYP_L2.setx(BYZO_FF00_RDn, top.EXT_P12_C);
    /*p05.KOLO*/ JOYP_L3.setx(BYZO_FF00_RDn, top.EXT_P13_C);
  }

  {
    /*p05.KEMA*/ top.CPU_TRI_D0.set_tribuf(!BYZO_FF00_RDn, JOYP_L0.q());
    /*p05.KURO*/ top.CPU_TRI_D1.set_tribuf(!BYZO_FF00_RDn, JOYP_L1.q());
    /*p05.KUVE*/ top.CPU_TRI_D2.set_tribuf(!BYZO_FF00_RDn, JOYP_L2.q());
    /*p05.JEKU*/ top.CPU_TRI_D3.set_tribuf(!BYZO_FF00_RDn, JOYP_L3.q());
    /*p05.KOCE*/ top.CPU_TRI_D4.set_tribuf(!BYZO_FF00_RDn, JOYP_UDLR.q());
    /*p05.CUDY*/ top.CPU_TRI_D5.set_tribuf(!BYZO_FF00_RDn, JOYP_ABCS.q());
    /*p??.????*/ top.CPU_TRI_D6.set_tribuf(!BYZO_FF00_RDn, DBG_FF00_D6.q());
    /*p??.????*/ top.CPU_TRI_D7.set_tribuf(!BYZO_FF00_RDn, DBG_FF00_D7.q());
  }

  {
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
    /*p05.JUTE*/ JOYP_RA    .set(ATOZ_FF00_WRn, ALUR_RSTn, top.CPU_TRI_D0);
    /*p05.KECY*/ JOYP_LB    .set(ATOZ_FF00_WRn, ALUR_RSTn, top.CPU_TRI_D1);
    /*p05.JALE*/ JOYP_UC    .set(ATOZ_FF00_WRn, ALUR_RSTn, top.CPU_TRI_D2);
    /*p05.KYME*/ JOYP_DS    .set(ATOZ_FF00_WRn, ALUR_RSTn, top.CPU_TRI_D3);
    /*p05.KELY*/ JOYP_UDLR  .set(ATOZ_FF00_WRn, ALUR_RSTn, top.CPU_TRI_D4);
    /*p05.COFY*/ JOYP_ABCS  .set(ATOZ_FF00_WRn, ALUR_RSTn, top.CPU_TRI_D5);
    /*p05.KUKO*/ DBG_FF00_D6.set(ATOZ_FF00_WRn, ALUR_RSTn, top.CPU_TRI_D6);
    /*p05.KERU*/ DBG_FF00_D7.set(ATOZ_FF00_WRn, ALUR_RSTn, top.CPU_TRI_D7);
  }

  {
    // FIXME
    wire BURO_FF60_0 = 0;
    wire FF60_0n = 1;

    /*p05.KOLE*/ top.EXT_P10_A.set(nand(JOYP_RA.q(), BURO_FF60_0));
    /*p05.KYBU*/ top.EXT_P10_D.set(nor(JOYP_RA.q(), FF60_0n));
    /*p05.KYTO*/ top.EXT_P11_A.set(nand(JOYP_LB.q(), BURO_FF60_0));
    /*p05.KABU*/ top.EXT_P11_D.set(nor(JOYP_LB.q(), FF60_0n));
    /*p05.KYHU*/ top.EXT_P12_A.set(nand(JOYP_UC.q(), BURO_FF60_0));
    /*p05.KASY*/ top.EXT_P12_D.set(nor(JOYP_UC.q(), FF60_0n)); // schematic wrong
    /*p05.KORY*/ top.EXT_P13_A.set(nand(JOYP_DS.q(), BURO_FF60_0));
    /*p05.KALE*/ top.EXT_P13_D.set(nor(JOYP_DS.q(), FF60_0n));
    /*p05.KARU*/ top.EXT_P14_A.set(or (!JOYP_UDLR.q(), FF60_0n));
    /*p05.KARU*/ top.EXT_P14_D.set(JOYP_UDLR.q());
    /*p05.CELA*/ top.EXT_P15_A.set(or (!JOYP_ABCS.q(), FF60_0n));
    /*p05.CELA*/ top.EXT_P15_D.set(!JOYP_ABCS.q()); // double check these
  }
}

//------------------------------------------------------------------------------

SignalHash JoypadRegisters::commit() {
  SignalHash hash;
  /*p02.BATU*/ hash << JP_GLITCH0.commit_reg();
  /*p02.ACEF*/ hash << JP_GLITCH1.commit_reg();
  /*p02.AGEM*/ hash << JP_GLITCH2.commit_reg();
  /*p02.APUG*/ hash << JP_GLITCH3.commit_reg();
  /*p05.JUTE*/ hash << JOYP_RA.commit_reg();
  /*p05.KECY*/ hash << JOYP_LB.commit_reg();
  /*p05.JALE*/ hash << JOYP_UC.commit_reg();
  /*p05.KYME*/ hash << JOYP_DS.commit_reg();
  /*p05.KELY*/ hash << JOYP_UDLR.commit_reg();
  /*p05.COFY*/ hash << JOYP_ABCS.commit_reg();
  /*p05.KUKO*/ hash << DBG_FF00_D6.commit_reg();
  /*p05.KERU*/ hash << DBG_FF00_D7.commit_reg();
  /*p05.KEVU*/ hash << JOYP_L0.commit_reg();
  /*p05.KAPA*/ hash << JOYP_L1.commit_reg();
  /*p05.KEJA*/ hash << JOYP_L2.commit_reg();
  /*p05.KOLO*/ hash << JOYP_L3.commit_reg();
  /*p02.AWOB*/ hash << AWOB_WAKE_CPU.commit_reg();

  return hash;
}

//------------------------------------------------------------------------------

#if 0
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
  AWOB_WAKE_CPU.dump(text_painter, "AWOB_WAKE_CPU    ");
  text_painter.newline();
  text_painter.dprintf("----- JOY_PINS -----\n");
  text_painter.dprintf("P10 %d:%d:%d:%d\n", EXT_P10_A.a.val, EXT_P10_B.a.val, EXT_P10_C.a.val, EXT_P10_D.a.val);
  text_painter.dprintf("P11 %d:%d:%d:%d\n", EXT_P11_A.a.val, EXT_P11_B.a.val, EXT_P11_C.a.val, EXT_P11_D.a.val);
  text_painter.dprintf("P12 %d:%d:%d:%d\n", EXT_P12_A.a.val, EXT_P12_B.a.val, EXT_P12_C.a.val, EXT_P12_D.a.val);
  text_painter.dprintf("P13 %d:%d:%d:%d\n", EXT_P13_A.a.val, EXT_P13_B.a.val, EXT_P13_C.a.val, EXT_P13_D.a.val);
  text_painter.dprintf("P14 %d:x:x:%d\n", EXT_P14_A.a.val, EXT_P14_D.a.val);
  text_painter.dprintf("P15 %d:x:x:%d\n", EXT_P15_A.a.val, EXT_P15_D.a.val);
  text_painter.newline();
}

void clear_dir() {
  // FIXME still don't know who drives these, so we always set them to 0.
  EXT_P10_B.set(0);
  EXT_P11_B.set(0);
  EXT_P12_B.set(0);
  EXT_P13_B.set(0);
}
#endif