#include "Sch_Joypad.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

JoypadSignals JoypadRegisters::sig() const {
  JoypadSignals sig;

  /*p02.ASOK*/ sig.ASOK_INT_JPp = and (JP_GLITCH3, JP_GLITCH0);

  return sig;
}

//------------------------------------------------------------------------------

void JoypadRegisters::tick(ResetSignals& rst_sig, ClockSignals& clk_sig, CpuBus& cpu_bus, CpuBusSignals& cpu_sig) {

  /*p02.KERY*/ wire _ANY_BUTTON = or(P13_C, P12_C, P11_C, P10_C);

  /*p10.AMUS*/ wire _AMUS_0xx00000 = nor(cpu_bus.PIN_A00, cpu_bus.PIN_A01, cpu_bus.PIN_A02, cpu_bus.PIN_A03, cpu_bus.PIN_A04, cpu_bus.PIN_A07);
  /*p10.ANAP*/ wire _ANAP_0xx00000 = and (_AMUS_0xx00000, cpu_sig.SYKE_FF00_FFFFp);

  /*p10.ACAT*/ wire _ACAT_FF00_RD = and (cpu_sig.TEDO_CPU_RD, _ANAP_0xx00000, cpu_sig.AKUG_A06n, cpu_sig.BYKO_A05n);
  /*p05.BYZO*/ wire _BYZO_FF00_RDn = not(_ACAT_FF00_RD);
  /*p10.ATOZ*/ wire _ATOZ_FF00_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, _ANAP_0xx00000, cpu_sig.AKUG_A06n, cpu_sig.BYKO_A05n);

  /*p01.BALY*/ wire BALY_xBxxxxxx = not(clk_sig.BYJU_AxCDEFGH);
  /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
  /*p02.AWOB*/ AWOB_WAKE_CPU.setx(BOGA_AxCDEFGH, _ANY_BUTTON);
  // cpu_pins.TO_CPU2.set(WAKE_CPU.q());

  /*p01.ALUR*/ wire ALUR_RSTn = not(rst_sig.AVOR_RSTp);   // this goes all over the place
  /*p02.BATU*/ JP_GLITCH0.set(BOGA_AxCDEFGH, ALUR_RSTn, _ANY_BUTTON);
  /*p02.ACEF*/ JP_GLITCH1.set(BOGA_AxCDEFGH, ALUR_RSTn, JP_GLITCH0.q());
  /*p02.AGEM*/ JP_GLITCH2.set(BOGA_AxCDEFGH, ALUR_RSTn, JP_GLITCH1.q());
  /*p02.APUG*/ JP_GLITCH3.set(BOGA_AxCDEFGH, ALUR_RSTn, JP_GLITCH2.q());

  ///*p02.ASOK*/ wire INT_JP = and (JP_GLITCH3, JP_GLITCH0);

  /*p05.KEVU*/ JOYP_L0.setx(_BYZO_FF00_RDn, P10_C);
  /*p05.KAPA*/ JOYP_L1.setx(_BYZO_FF00_RDn, P11_C);
  /*p05.KEJA*/ JOYP_L2.setx(_BYZO_FF00_RDn, P12_C);
  /*p05.KOLO*/ JOYP_L3.setx(_BYZO_FF00_RDn, P13_C);

  /*p05.KEMA*/ cpu_bus.TRI_D0.set_tribuf(!_BYZO_FF00_RDn, JOYP_L0.q());
  /*p05.KURO*/ cpu_bus.TRI_D1.set_tribuf(!_BYZO_FF00_RDn, JOYP_L1.q());
  /*p05.KUVE*/ cpu_bus.TRI_D2.set_tribuf(!_BYZO_FF00_RDn, JOYP_L2.q());
  /*p05.JEKU*/ cpu_bus.TRI_D3.set_tribuf(!_BYZO_FF00_RDn, JOYP_L3.q());
  /*p05.KOCE*/ cpu_bus.TRI_D4.set_tribuf(!_BYZO_FF00_RDn, JOYP_UDLR.q());
  /*p05.CUDY*/ cpu_bus.TRI_D5.set_tribuf(!_BYZO_FF00_RDn, JOYP_ABCS.q());
  /*p??.????*/ cpu_bus.TRI_D6.set_tribuf(!_BYZO_FF00_RDn, DBG_FF00_D6.q());
  /*p??.????*/ cpu_bus.TRI_D7.set_tribuf(!_BYZO_FF00_RDn, DBG_FF00_D7.q());

  /*p05.JUTE*/ JOYP_RA    .set(_ATOZ_FF00_WRn, ALUR_RSTn, cpu_bus.TRI_D0);
  /*p05.KECY*/ JOYP_LB    .set(_ATOZ_FF00_WRn, ALUR_RSTn, cpu_bus.TRI_D1);
  /*p05.JALE*/ JOYP_UC    .set(_ATOZ_FF00_WRn, ALUR_RSTn, cpu_bus.TRI_D2);
  /*p05.KYME*/ JOYP_DS    .set(_ATOZ_FF00_WRn, ALUR_RSTn, cpu_bus.TRI_D3);
  /*p05.KELY*/ JOYP_UDLR  .set(_ATOZ_FF00_WRn, ALUR_RSTn, cpu_bus.TRI_D4);
  /*p05.COFY*/ JOYP_ABCS  .set(_ATOZ_FF00_WRn, ALUR_RSTn, cpu_bus.TRI_D5);
  /*p05.KUKO*/ DBG_FF00_D6.set(_ATOZ_FF00_WRn, ALUR_RSTn, cpu_bus.TRI_D6);
  /*p05.KERU*/ DBG_FF00_D7.set(_ATOZ_FF00_WRn, ALUR_RSTn, cpu_bus.TRI_D7);

  // FIXME
  wire BURO_FF60_0 = 0;
  wire FF60_0n = 1;

  /*p05.KOLE*/ P10_A.set(nand(JOYP_RA.q(), BURO_FF60_0));
  /*p05.KYBU*/ P10_D.set(nor(JOYP_RA.q(), FF60_0n));
  /*p05.KYTO*/ P11_A.set(nand(JOYP_LB.q(), BURO_FF60_0));
  /*p05.KABU*/ P11_D.set(nor(JOYP_LB.q(), FF60_0n));
  /*p05.KYHU*/ P12_A.set(nand(JOYP_UC.q(), BURO_FF60_0));
  /*p05.KASY*/ P12_D.set(nor(JOYP_UC.q(), FF60_0n)); // schematic wrong
  /*p05.KORY*/ P13_A.set(nand(JOYP_DS.q(), BURO_FF60_0));
  /*p05.KALE*/ P13_D.set(nor(JOYP_DS.q(), FF60_0n));
  /*p05.KARU*/ P14_A.set(or (!JOYP_UDLR.q(), FF60_0n));
  /*p05.KARU*/ P14_D.set(JOYP_UDLR.q());
  /*p05.CELA*/ P15_A.set(or (!JOYP_ABCS.q(), FF60_0n));
  /*p05.CELA*/ P15_D.set(!JOYP_ABCS.q()); // double check these
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

  /* PIN_58 */ /*VCC*/
  /* PIN_59 */ /*ROUT*/
  /* PIN_60 */ /*LOUT*/
  /* PIN_61 */ /*VIN*/

  /* PIN_62 */ hash << P15_A.commit_pinout();   // <- CELA
  /* PIN_62 */ hash << P15_D.commit_pinout();   // <- COFY
  /* PIN_63 */ hash << P14_A.commit_pinout();   // <- KARU
  /* PIN_63 */ hash << P14_D.commit_pinout();   // <- KELY
  /* PIN_64 */ hash << P13_A.commit_pinout();   // <- KORY
  /* PIN_64 */ hash << P13_B.commit_pinout();   
  /* PIN_64 */ hash << P13_C.clear_preset();    // -> KERY, P05.KOLO
  /* PIN_64 */ hash << P13_D.commit_pinout();   // <- KALE
  /* PIN_65 */ hash << P12_A.commit_pinout();   // <- KYHU
  /* PIN_65 */ hash << P12_B.commit_pinout();   
  /* PIN_65 */ hash << P12_C.clear_preset();    // -> KERY, P05.KEJA
  /* PIN_65 */ hash << P12_D.commit_pinout();   // <- KASY
  /* PIN_66 */ hash << P11_A.commit_pinout();   // <- KYTO
  /* PIN_66 */ hash << P11_B.commit_pinout();   
  /* PIN_66 */ hash << P11_C.clear_preset();    // -> KERY, P05.KAPA
  /* PIN_66 */ hash << P11_D.commit_pinout();   // <- KABU
  /* PIN_67 */ hash << P10_A.commit_pinout();   // <- KOLE
  /* PIN_67 */ hash << P10_B.commit_pinout();   
  /* PIN_67 */ hash << P10_C.clear_preset();    // -> KERY, KEVU
  /* PIN_67 */ hash << P10_D.commit_pinout();   // <- KYBU

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
  text_painter.dprintf("P10 %d:%d:%d:%d\n", P10_A.a.val, P10_B.a.val, P10_C.a.val, P10_D.a.val);
  text_painter.dprintf("P11 %d:%d:%d:%d\n", P11_A.a.val, P11_B.a.val, P11_C.a.val, P11_D.a.val);
  text_painter.dprintf("P12 %d:%d:%d:%d\n", P12_A.a.val, P12_B.a.val, P12_C.a.val, P12_D.a.val);
  text_painter.dprintf("P13 %d:%d:%d:%d\n", P13_A.a.val, P13_B.a.val, P13_C.a.val, P13_D.a.val);
  text_painter.dprintf("P14 %d:x:x:%d\n", P14_A.a.val, P14_D.a.val);
  text_painter.dprintf("P15 %d:x:x:%d\n", P15_A.a.val, P15_D.a.val);
  text_painter.newline();
}

void clear_dir() {
  // FIXME still don't know who drives these, so we always set them to 0.
  P10_B.set(0);
  P11_B.set(0);
  P12_B.set(0);
  P13_B.set(0);
}
#endif