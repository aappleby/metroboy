#include "Sch_Joypad.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void JoypadRegisters::tick(SchematicTop& top) {

  PSignal ACAT_FF00_RDp;
  NSignal ATOZ_FF00_WRn;
  {
    /*p07.TEDO*/ pwire TEDO_CPU_RDp = not(top.UJYV_CPU_RDn());
    /*p07.TAPU*/ nwire TAPU_CPU_WRn_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());

    /*p10.BYKO*/ wire BYKO_A05n = not(top.CPU_PIN_A05);
    /*p10.AKUG*/ wire AKUG_A06n = not(top.CPU_PIN_A06);
    /*p10.AMUS*/ wire AMUS_0xx00000 = nor(top.CPU_PIN_A00, top.CPU_PIN_A01, top.CPU_PIN_A02, top.CPU_PIN_A03, top.CPU_PIN_A04, top.CPU_PIN_A07);
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(top.CPU_PIN_A15, top.CPU_PIN_A14, top.CPU_PIN_A13, top.CPU_PIN_A12, top.CPU_PIN_A11, top.CPU_PIN_A10, top.CPU_PIN_A09);
    /*p07.TONA*/ wire TONA_A08n = not(top.CPU_PIN_A08);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
    /*p10.ANAP*/ wire ANAP_FF_0xx00000 = and (AMUS_0xx00000, SYKE_FF00_FFFFp);

    /*p10.ACAT*/ ACAT_FF00_RDp = and (TEDO_CPU_RDp,          ANAP_FF_0xx00000, AKUG_A06n, BYKO_A05n);
    /*p10.ATOZ*/ ATOZ_FF00_WRn = nand(TAPU_CPU_WRn_xxxxxFGH, ANAP_FF_0xx00000, AKUG_A06n, BYKO_A05n);
  }

  {
    /*p01.BALY*/ wire BALY_xBxxxxxx = not(top.BYJU_AxCDEFGH());
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
    /*p02.KERY*/ pwire KERY_ANY_BUTTONp = or(top.JOY_PIN_P13_C, top.JOY_PIN_P12_C, top.JOY_PIN_P11_C, top.JOY_PIN_P10_C);

    /*p02.AWOB*/ AWOB_WAKE_CPU.tp_latch(BOGA_AxCDEFGH, KERY_ANY_BUTTONp);
    // cpu_pins.TO_CPU2.set(WAKE_CPU.q());
  }

  {
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());
    /*p01.BALY*/ wire BALY_xBxxxxxx = not(top.BYJU_AxCDEFGH());
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
    /*p02.KERY*/ wire ANY_BUTTON = or(top.JOY_PIN_P13_C, top.JOY_PIN_P12_C, top.JOY_PIN_P11_C, top.JOY_PIN_P10_C);

    /*p02.BATU*/ BATU_JP_GLITCH0.set(BOGA_AxCDEFGH, ALUR_RSTn, ANY_BUTTON);
    /*p02.ACEF*/ ACEF_JP_GLITCH1.set(BOGA_AxCDEFGH, ALUR_RSTn, BATU_JP_GLITCH0.q());
    /*p02.AGEM*/ AGEM_JP_GLITCH2.set(BOGA_AxCDEFGH, ALUR_RSTn, ACEF_JP_GLITCH1.q());
    /*p02.APUG*/ APUG_JP_GLITCH3.set(BOGA_AxCDEFGH, ALUR_RSTn, AGEM_JP_GLITCH2.q());
  }

  {
    /*p05.BYZO*/ nwire BYZO_FF00_RDn = not(ACAT_FF00_RDp);

    /*p05.KEVU*/ KEVU_JOYP_L0.tp_latch(BYZO_FF00_RDn, top.JOY_PIN_P10_C);
    /*p05.KAPA*/ KAPA_JOYP_L1.tp_latch(BYZO_FF00_RDn, top.JOY_PIN_P11_C);
    /*p05.KEJA*/ KEJA_JOYP_L2.tp_latch(BYZO_FF00_RDn, top.JOY_PIN_P12_C);
    /*p05.KOLO*/ KOLO_JOYP_L3.tp_latch(BYZO_FF00_RDn, top.JOY_PIN_P13_C);

    /*p05.KEMA*/ top.CPU_TRI_D0.set_tribuf_6n(BYZO_FF00_RDn, KEVU_JOYP_L0.q());
    /*p05.KURO*/ top.CPU_TRI_D1.set_tribuf_6n(BYZO_FF00_RDn, KAPA_JOYP_L1.q());
    /*p05.KUVE*/ top.CPU_TRI_D2.set_tribuf_6n(BYZO_FF00_RDn, KEJA_JOYP_L2.q());
    /*p05.JEKU*/ top.CPU_TRI_D3.set_tribuf_6n(BYZO_FF00_RDn, KOLO_JOYP_L3.q());
    /*p05.KOCE*/ top.CPU_TRI_D4.set_tribuf_6n(BYZO_FF00_RDn, KELY_JOYP_UDLR.q());
    /*p05.CUDY*/ top.CPU_TRI_D5.set_tribuf_6n(BYZO_FF00_RDn, COFY_JOYP_ABCS.q());
    /*p??.????*/ top.CPU_TRI_D6.set_tribuf_6n(BYZO_FF00_RDn, KUKO_DBG_FF00_D6.q());
    /*p??.????*/ top.CPU_TRI_D7.set_tribuf_6n(BYZO_FF00_RDn, KERU_DBG_FF00_D7.q());
  }

  {
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());
    /*p05.JUTE*/ JUTE_JOYP_RA    .set(ATOZ_FF00_WRn.as_pwire(), ALUR_RSTn, top.CPU_TRI_D0);
    /*p05.KECY*/ KECY_JOYP_LB    .set(ATOZ_FF00_WRn.as_pwire(), ALUR_RSTn, top.CPU_TRI_D1);
    /*p05.JALE*/ JALE_JOYP_UC    .set(ATOZ_FF00_WRn.as_pwire(), ALUR_RSTn, top.CPU_TRI_D2);
    /*p05.KYME*/ KYME_JOYP_DS    .set(ATOZ_FF00_WRn.as_pwire(), ALUR_RSTn, top.CPU_TRI_D3);
    /*p05.KELY*/ KELY_JOYP_UDLR  .set(ATOZ_FF00_WRn.as_pwire(), ALUR_RSTn, top.CPU_TRI_D4);
    /*p05.COFY*/ COFY_JOYP_ABCS  .set(ATOZ_FF00_WRn.as_pwire(), ALUR_RSTn, top.CPU_TRI_D5);
    /*p05.KUKO*/ KUKO_DBG_FF00_D6.set(ATOZ_FF00_WRn.as_pwire(), ALUR_RSTn, top.CPU_TRI_D6);
    /*p05.KERU*/ KERU_DBG_FF00_D7.set(ATOZ_FF00_WRn.as_pwire(), ALUR_RSTn, top.CPU_TRI_D7);
  }

  {
    // FIXME
    wire BURO_FF60_0 = 0;
    wire FF60_0n = 1;

    top.JOY_PIN_P10_B.set(top.GND);
    top.JOY_PIN_P11_B.set(top.GND);
    top.JOY_PIN_P12_B.set(top.GND);
    top.JOY_PIN_P13_B.set(top.GND);

    /*p05.KOLE*/ top.JOY_PIN_P10_A.set(nand(JUTE_JOYP_RA.q(), BURO_FF60_0));
    /*p05.KYBU*/ top.JOY_PIN_P10_D.set(nor (JUTE_JOYP_RA.q(), FF60_0n));
    /*p05.KYTO*/ top.JOY_PIN_P11_A.set(nand(KECY_JOYP_LB.q(), BURO_FF60_0));
    /*p05.KABU*/ top.JOY_PIN_P11_D.set(nor (KECY_JOYP_LB.q(), FF60_0n));
    /*p05.KYHU*/ top.JOY_PIN_P12_A.set(nand(JALE_JOYP_UC.q(), BURO_FF60_0));
    /*p05.KASY*/ top.JOY_PIN_P12_D.set(nor (JALE_JOYP_UC.q(), FF60_0n)); // schematic wrong
    /*p05.KORY*/ top.JOY_PIN_P13_A.set(nand(KYME_JOYP_DS.q(), BURO_FF60_0));
    /*p05.KALE*/ top.JOY_PIN_P13_D.set(nor (KYME_JOYP_DS.q(), FF60_0n));

    /*p05.KARU*/ top.JOY_PIN_P14_A.set(or (!KELY_JOYP_UDLR.q(), FF60_0n));
    /*p05.KARU*/ top.JOY_PIN_P14_D.set(KELY_JOYP_UDLR.q());
    /*p05.CELA*/ top.JOY_PIN_P15_A.set(or (!COFY_JOYP_ABCS.q(), FF60_0n));
    /*p05.CELA*/ top.JOY_PIN_P15_D.set(!COFY_JOYP_ABCS.q()); // double check these
  }
}

//------------------------------------------------------------------------------

SignalHash JoypadRegisters::commit() {
  SignalHash hash;
  /*p02.BATU*/ hash << BATU_JP_GLITCH0.commit_reg();
  /*p02.ACEF*/ hash << ACEF_JP_GLITCH1.commit_reg();
  /*p02.AGEM*/ hash << AGEM_JP_GLITCH2.commit_reg();
  /*p02.APUG*/ hash << APUG_JP_GLITCH3.commit_reg();
  /*p05.JUTE*/ hash << JUTE_JOYP_RA.commit_reg();
  /*p05.KECY*/ hash << KECY_JOYP_LB.commit_reg();
  /*p05.JALE*/ hash << JALE_JOYP_UC.commit_reg();
  /*p05.KYME*/ hash << KYME_JOYP_DS.commit_reg();
  /*p05.KELY*/ hash << KELY_JOYP_UDLR.commit_reg();
  /*p05.COFY*/ hash << COFY_JOYP_ABCS.commit_reg();
  /*p05.KUKO*/ hash << KUKO_DBG_FF00_D6.commit_reg();
  /*p05.KERU*/ hash << KERU_DBG_FF00_D7.commit_reg();
  /*p05.KEVU*/ hash << KEVU_JOYP_L0.commit_latch();
  /*p05.KAPA*/ hash << KAPA_JOYP_L1.commit_latch();
  /*p05.KEJA*/ hash << KEJA_JOYP_L2.commit_latch();
  /*p05.KOLO*/ hash << KOLO_JOYP_L3.commit_latch();
  /*p02.AWOB*/ hash << AWOB_WAKE_CPU.commit_latch();

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