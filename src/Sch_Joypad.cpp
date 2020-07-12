#include "Sch_Joypad.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void JoypadRegisters::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void JoypadRegisters::tock(SchematicTop& top) {

  /*p10.AMUS*/ wire _AMUS_0xx00000 = nor(top.CPU_PIN_A00, top.CPU_PIN_A01, top.CPU_PIN_A02, top.CPU_PIN_A03, top.CPU_PIN_A04, top.CPU_PIN_A07);
  /*p10.ANAP*/ wire _ANAP_FF_0xx00000 = and (_AMUS_0xx00000, top.SYKE_FF00_FFFFp());
  /*p10.AKUG*/ wire _AKUG_A06n = not(top.CPU_PIN_A06);
  /*p10.BYKO*/ wire _BYKO_A05n = not(top.CPU_PIN_A05);
  /*p02.KERY*/ wire _KERY_ANY_BUTTONp = or(top.JOY_PIN_P13_C, top.JOY_PIN_P12_C, top.JOY_PIN_P11_C, top.JOY_PIN_P10_C);

  {
    /*p02.AWOB*/ AWOB_WAKE_CPU.tp_latch(top.BOGA_xBCDEFGH(), _KERY_ANY_BUTTONp);
    CPU_PIN_WAKE.set(AWOB_WAKE_CPU.q());
  }

  {
    /*p02.BATU*/ BATU_JP_GLITCH0.set(top.BOGA_xBCDEFGH(), top.ALUR_SYS_RSTn(), _KERY_ANY_BUTTONp);
    /*p02.ACEF*/ ACEF_JP_GLITCH1.set(top.BOGA_xBCDEFGH(), top.ALUR_SYS_RSTn(), BATU_JP_GLITCH0.q());
    /*p02.AGEM*/ AGEM_JP_GLITCH2.set(top.BOGA_xBCDEFGH(), top.ALUR_SYS_RSTn(), ACEF_JP_GLITCH1.q());
    /*p02.APUG*/ APUG_JP_GLITCH3.set(top.BOGA_xBCDEFGH(), top.ALUR_SYS_RSTn(), AGEM_JP_GLITCH2.q());
  }

  {
    /*p10.ACAT*/ wire _ACAT_FF00_RDp = and (top.TEDO_CPU_RDp(), _ANAP_FF_0xx00000, _AKUG_A06n, _BYKO_A05n);
    /*p05.BYZO*/ wire _BYZO_FF00_RDn = not(_ACAT_FF00_RDp);
    /*p05.KEVU*/ KEVU_JOYP_L0.tp_latch(_BYZO_FF00_RDn, top.JOY_PIN_P10_C);
    /*p05.KAPA*/ KAPA_JOYP_L1.tp_latch(_BYZO_FF00_RDn, top.JOY_PIN_P11_C);
    /*p05.KEJA*/ KEJA_JOYP_L2.tp_latch(_BYZO_FF00_RDn, top.JOY_PIN_P12_C);
    /*p05.KOLO*/ KOLO_JOYP_L3.tp_latch(_BYZO_FF00_RDn, top.JOY_PIN_P13_C);

    /*p05.KEMA*/ top.CPU_TRI_D0.set_tribuf_6n(_BYZO_FF00_RDn, KEVU_JOYP_L0.q());
    /*p05.KURO*/ top.CPU_TRI_D1.set_tribuf_6n(_BYZO_FF00_RDn, KAPA_JOYP_L1.q());
    /*p05.KUVE*/ top.CPU_TRI_D2.set_tribuf_6n(_BYZO_FF00_RDn, KEJA_JOYP_L2.q());
    /*p05.JEKU*/ top.CPU_TRI_D3.set_tribuf_6n(_BYZO_FF00_RDn, KOLO_JOYP_L3.q());
    /*p05.KOCE*/ top.CPU_TRI_D4.set_tribuf_6n(_BYZO_FF00_RDn, KELY_JOYP_UDLR.q());
    /*p05.CUDY*/ top.CPU_TRI_D5.set_tribuf_6n(_BYZO_FF00_RDn, COFY_JOYP_ABCS.q());
    /*p??.????*/ top.CPU_TRI_D6.set_tribuf_6n(_BYZO_FF00_RDn, KUKO_DBG_FF00_D6.q());
    /*p??.????*/ top.CPU_TRI_D7.set_tribuf_6n(_BYZO_FF00_RDn, KERU_DBG_FF00_D7.q());
  }

  {
    /*p10.ATOZ*/ wire _ATOZ_FF00_WRn = nand(top.TAPU_CPU_WRp_xxxxEFGx(), _ANAP_FF_0xx00000, _AKUG_A06n, _BYKO_A05n);
    /*p05.JUTE*/ JUTE_JOYP_RA    .set(_ATOZ_FF00_WRn, top.ALUR_SYS_RSTn(), top.CPU_TRI_D0);
    /*p05.KECY*/ KECY_JOYP_LB    .set(_ATOZ_FF00_WRn, top.ALUR_SYS_RSTn(), top.CPU_TRI_D1);
    /*p05.JALE*/ JALE_JOYP_UC    .set(_ATOZ_FF00_WRn, top.ALUR_SYS_RSTn(), top.CPU_TRI_D2);
    /*p05.KYME*/ KYME_JOYP_DS    .set(_ATOZ_FF00_WRn, top.ALUR_SYS_RSTn(), top.CPU_TRI_D3);
    /*p05.KELY*/ KELY_JOYP_UDLR  .set(_ATOZ_FF00_WRn, top.ALUR_SYS_RSTn(), top.CPU_TRI_D4);
    /*p05.COFY*/ COFY_JOYP_ABCS  .set(_ATOZ_FF00_WRn, top.ALUR_SYS_RSTn(), top.CPU_TRI_D5);
    /*p05.KUKO*/ KUKO_DBG_FF00_D6.set(_ATOZ_FF00_WRn, top.ALUR_SYS_RSTn(), top.CPU_TRI_D6);
    /*p05.KERU*/ KERU_DBG_FF00_D7.set(_ATOZ_FF00_WRn, top.ALUR_SYS_RSTn(), top.CPU_TRI_D7);
  }

  {
    // FIXME
    wire _BURO_FF60_0p = 0;
    wire _KURO_FF60_0n = 1;

    top.JOY_PIN_P10_B.set(top.GND);
    top.JOY_PIN_P11_B.set(top.GND);
    top.JOY_PIN_P12_B.set(top.GND);
    top.JOY_PIN_P13_B.set(top.GND);

    /*p05.KOLE*/ top.JOY_PIN_P10_A.set(nand(JUTE_JOYP_RA.q(), _BURO_FF60_0p));
    /*p05.KYBU*/ top.JOY_PIN_P10_D.set(nor (JUTE_JOYP_RA.q(), _KURO_FF60_0n));
    /*p05.KYTO*/ top.JOY_PIN_P11_A.set(nand(KECY_JOYP_LB.q(), _BURO_FF60_0p));
    /*p05.KABU*/ top.JOY_PIN_P11_D.set(nor (KECY_JOYP_LB.q(), _KURO_FF60_0n));
    /*p05.KYHU*/ top.JOY_PIN_P12_A.set(nand(JALE_JOYP_UC.q(), _BURO_FF60_0p));
    /*p05.KASY*/ top.JOY_PIN_P12_D.set(nor (JALE_JOYP_UC.q(), _KURO_FF60_0n)); // schematic wrong
    /*p05.KORY*/ top.JOY_PIN_P13_A.set(nand(KYME_JOYP_DS.q(), _BURO_FF60_0p));
    /*p05.KALE*/ top.JOY_PIN_P13_D.set(nor (KYME_JOYP_DS.q(), _KURO_FF60_0n));

    /*p05.KARU*/ top.JOY_PIN_P14_A.set(or (!KELY_JOYP_UDLR.q(), _KURO_FF60_0n));
    /*p05.KARU*/ top.JOY_PIN_P14_D.set(KELY_JOYP_UDLR.q());
    /*p05.CELA*/ top.JOY_PIN_P15_A.set(or (!COFY_JOYP_ABCS.q(), _KURO_FF60_0n));
    /*p05.CELA*/ top.JOY_PIN_P15_D.set(!COFY_JOYP_ABCS.q()); // double check these
  }
}

//------------------------------------------------------------------------------

SignalHash JoypadRegisters::commit(SchematicTop& top) {
  SignalHash hash;

  hash << top.JOY_PIN_P10_A.commit();    // PIN_67<- KOLE
  hash << top.JOY_PIN_P10_B.commit();    // PIN_67
  hash << top.JOY_PIN_P10_D.commit();    // PIN_67<- KYBU
  hash << top.JOY_PIN_P11_A.commit();    // PIN_66<- KYTO
  hash << top.JOY_PIN_P11_B.commit();    // PIN_66
  hash << top.JOY_PIN_P11_D.commit();    // PIN_66<- KABU
  hash << top.JOY_PIN_P12_A.commit();    // PIN_65<- KYHU
  hash << top.JOY_PIN_P12_B.commit();    // PIN_65
  hash << top.JOY_PIN_P12_D.commit();    // PIN_65<- KASY
  hash << top.JOY_PIN_P13_A.commit();    // PIN_64<- KORY
  hash << top.JOY_PIN_P13_B.commit();    // PIN_64
  hash << top.JOY_PIN_P13_D.commit();    // PIN_64<- KALE
  hash << top.JOY_PIN_P14_A.commit();    // PIN_63<- KARU
  hash << top.JOY_PIN_P14_D.commit();    // PIN_63<- KELY
  hash << top.JOY_PIN_P15_A.commit();    // PIN_62<- CELA
  hash << top.JOY_PIN_P15_D.commit();    // PIN_62<- COFY

  /*p02.BATU*/ hash << BATU_JP_GLITCH0.commit();
  /*p02.ACEF*/ hash << ACEF_JP_GLITCH1.commit();
  /*p02.AGEM*/ hash << AGEM_JP_GLITCH2.commit();
  /*p02.APUG*/ hash << APUG_JP_GLITCH3.commit();
  /*p05.JUTE*/ hash << JUTE_JOYP_RA.commit();
  /*p05.KECY*/ hash << KECY_JOYP_LB.commit();
  /*p05.JALE*/ hash << JALE_JOYP_UC.commit();
  /*p05.KYME*/ hash << KYME_JOYP_DS.commit();
  /*p05.KELY*/ hash << KELY_JOYP_UDLR.commit();
  /*p05.COFY*/ hash << COFY_JOYP_ABCS.commit();
  /*p05.KUKO*/ hash << KUKO_DBG_FF00_D6.commit();
  /*p05.KERU*/ hash << KERU_DBG_FF00_D7.commit();
  /*p05.KEVU*/ hash << KEVU_JOYP_L0.commit();
  /*p05.KAPA*/ hash << KAPA_JOYP_L1.commit();
  /*p05.KEJA*/ hash << KEJA_JOYP_L2.commit();
  /*p05.KOLO*/ hash << KOLO_JOYP_L3.commit();
  /*p02.AWOB*/ hash << AWOB_WAKE_CPU.commit();

  hash << CPU_PIN_WAKE.commit();          // <- P02.AWOB

  return hash;
}

//------------------------------------------------------------------------------

#if 0
void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf(" ----- JOY REG -----\n");
  BATU_JP_GLITCH0.dump(text_painter, "BATU_JP_GLITCH0  ");
  ACEF_JP_GLITCH1.dump(text_painter, "ACEF_JP_GLITCH1  ");
  AGEM_JP_GLITCH2.dump(text_painter, "AGEM_JP_GLITCH2  ");
  APUG_JP_GLITCH3.dump(text_painter, "APUG_JP_GLITCH3  ");
  JUTE_JOYP_RA.dump(text_painter, "JUTE_JOYP_RA     ");
  KECY_JOYP_LB.dump(text_painter, "KECY_JOYP_LB     ");
  JALE_JOYP_UC.dump(text_painter, "JALE_JOYP_UC     ");
  KYME_JOYP_DS.dump(text_painter, "KYME_JOYP_DS     ");
  KELY_JOYP_UDLR.dump(text_painter, "KELY_JOYP_UDLR   ");
  COFY_JOYP_ABCS.dump(text_painter, "COFY_JOYP_ABCS   ");
  KUKO_DBG_FF00_D6.dump(text_painter, "KUKO_DBG_FF00_D6 ");
  KERU_DBG_FF00_D7.dump(text_painter, "KERU_DBG_FF00_D7 ");
  KEVU_JOYP_L0.dump(text_painter, "KEVU_JOYP_L0     ");
  KAPA_JOYP_L1.dump(text_painter, "KAPA_JOYP_L1     ");
  KEJA_JOYP_L2.dump(text_painter, "KEJA_JOYP_L2     ");
  KOLO_JOYP_L3.dump(text_painter, "KOLO_JOYP_L3     ");
  AWOB_WAKE_CPU.dump(text_painter, "AWOB_WAKE_CPU    ");
  text_painter.newline();
  text_painter.dprintf("----- JOY_PINS -----\n");
  text_painter.dprintf("P10 %d:%d:%d:%d\n", JOY_PIN_P10_A.a.val, top.GND, JOY_PIN_P10_C.a.val, JOY_PIN_P10_D.a.val);
  text_painter.dprintf("P11 %d:%d:%d:%d\n", JOY_PIN_P11_A.a.val, JOY_PIN_P11_B.a.val, JOY_PIN_P11_C.a.val, JOY_PIN_P11_D.a.val);
  text_painter.dprintf("P12 %d:%d:%d:%d\n", JOY_PIN_P12_A.a.val, JOY_PIN_P12_B.a.val, JOY_PIN_P12_C.a.val, JOY_PIN_P12_D.a.val);
  text_painter.dprintf("P13 %d:%d:%d:%d\n", JOY_PIN_P13_A.a.val, JOY_PIN_P13_B.a.val, JOY_PIN_P13_C.a.val, JOY_PIN_P13_D.a.val);
  text_painter.dprintf("P14 %d:x:x:%d\n", JOY_PIN_P14_A.a.val, JOY_PIN_P14_D.a.val);
  text_painter.dprintf("P15 %d:x:x:%d\n", JOY_PIN_P15_A.a.val, JOY_PIN_P15_D.a.val);
  text_painter.newline();
}

void clear_dir() {
  JOY_PIN_P10_B.set(top.GND);
  JOY_PIN_P11_B.set(top.GND);
  JOY_PIN_P12_B.set(top.GND);
  JOY_PIN_P13_B.set(top.GND);
}
#endif