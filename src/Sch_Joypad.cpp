#include "Sch_Joypad.h"

#include "TestGB.h"

using namespace Schematics;


void JoypadRegisters::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto cpu_sig = gb.cpu_reg.sig(gb);

  auto& joy_pin = gb.joy_pin;
  auto& cpu_bus = gb.cpu_bus;

  /*p02.KERY*/ wire _ANY_BUTTON = or(joy_pin.P13_C, joy_pin.P12_C, joy_pin.P11_C, joy_pin.P10_C);

  /*p10.AMUS*/ wire _AMUS_0xx00000 = nor(cpu_bus.PIN_A00, cpu_bus.PIN_A01, cpu_bus.PIN_A02, cpu_bus.PIN_A03, cpu_bus.PIN_A04, cpu_bus.PIN_A07);
  /*p10.ANAP*/ wire _ANAP_0xx00000 = and (_AMUS_0xx00000, adr_sig.SYKE_FF00_FFFFp);

  /*p10.ACAT*/ wire _ACAT_FF00_RD = and (cpu_sig.TEDO_CPU_RD, _ANAP_0xx00000, adr_sig.AKUG_A06n, adr_sig.BYKO_A05n);
  /*p05.BYZO*/ wire _BYZO_FF00_RDn = not(_ACAT_FF00_RD);
  /*p10.ATOZ*/ wire _ATOZ_FF00_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, _ANAP_0xx00000, adr_sig.AKUG_A06n, adr_sig.BYKO_A05n);

  /*p02.AWOB*/ WAKE_CPU.set(clk_sig.PIN_BOGA_AxCDEFGH, _ANY_BUTTON);
  // cpu_pins.TO_CPU2.set(WAKE_CPU.q());

  /*p02.BATU*/ JP_GLITCH0.set(clk_sig.PIN_BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, _ANY_BUTTON);
  /*p02.ACEF*/ JP_GLITCH1.set(clk_sig.PIN_BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, JP_GLITCH0.q());
  /*p02.AGEM*/ JP_GLITCH2.set(clk_sig.PIN_BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, JP_GLITCH1.q());
  /*p02.APUG*/ JP_GLITCH3.set(clk_sig.PIN_BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, JP_GLITCH2.q());

  ///*p02.ASOK*/ wire INT_JP = and (JP_GLITCH3, JP_GLITCH0);

  /*p05.KEVU*/ JOYP_L0.set(_BYZO_FF00_RDn, joy_pin.P10_C);
  /*p05.KAPA*/ JOYP_L1.set(_BYZO_FF00_RDn, joy_pin.P11_C);
  /*p05.KEJA*/ JOYP_L2.set(_BYZO_FF00_RDn, joy_pin.P12_C);
  /*p05.KOLO*/ JOYP_L3.set(_BYZO_FF00_RDn, joy_pin.P13_C);

  /*p05.KEMA*/ cpu_bus.TS_D0.set_tribuf(!_BYZO_FF00_RDn, JOYP_L0.q());
  /*p05.KURO*/ cpu_bus.TS_D1.set_tribuf(!_BYZO_FF00_RDn, JOYP_L1.q());
  /*p05.KUVE*/ cpu_bus.TS_D2.set_tribuf(!_BYZO_FF00_RDn, JOYP_L2.q());
  /*p05.JEKU*/ cpu_bus.TS_D3.set_tribuf(!_BYZO_FF00_RDn, JOYP_L3.q());
  /*p05.KOCE*/ cpu_bus.TS_D4.set_tribuf(!_BYZO_FF00_RDn, JOYP_UDLR.q());
  /*p05.CUDY*/ cpu_bus.TS_D5.set_tribuf(!_BYZO_FF00_RDn, JOYP_ABCS.q());
  /*p??.????*/ cpu_bus.TS_D6.set_tribuf(!_BYZO_FF00_RDn, DBG_FF00_D6.q());
  /*p??.????*/ cpu_bus.TS_D7.set_tribuf(!_BYZO_FF00_RDn, DBG_FF00_D7.q());

  /*p05.JUTE*/ JOYP_RA    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D0);
  /*p05.KECY*/ JOYP_LB    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D1);
  /*p05.JALE*/ JOYP_UC    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D2);
  /*p05.KYME*/ JOYP_DS    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D3);
  /*p05.KELY*/ JOYP_UDLR  .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D4);
  /*p05.COFY*/ JOYP_ABCS  .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D5);
  /*p05.KUKO*/ DBG_FF00_D6.set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D6);
  /*p05.KERU*/ DBG_FF00_D7.set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D7);

  // FIXME
  wire FF60_0 = 0;
  wire FF60_0n = 1;

  /*p05.KOLE*/ joy_pin.P10_A.set(nand(JOYP_RA.q(), FF60_0));
  /*p05.KYBU*/ joy_pin.P10_D.set(nor(JOYP_RA.q(), FF60_0n));
  /*p05.KYTO*/ joy_pin.P11_A.set(nand(JOYP_LB.q(), FF60_0));
  /*p05.KABU*/ joy_pin.P11_D.set(nor(JOYP_LB.q(), FF60_0n));
  /*p05.KYHU*/ joy_pin.P12_A.set(nand(JOYP_UC.q(), FF60_0));
  /*p05.KASY*/ joy_pin.P12_D.set(nor(JOYP_UC.q(), FF60_0n)); // schematic wrong
  /*p05.KORY*/ joy_pin.P13_A.set(nand(JOYP_DS.q(), FF60_0));
  /*p05.KALE*/ joy_pin.P13_D.set(nor(JOYP_DS.q(), FF60_0n));
  /*p05.KARU*/ joy_pin.P14_A.set(or (!JOYP_UDLR.q(), FF60_0n));
  /*p05.KARU*/ joy_pin.P14_D.set(JOYP_UDLR.q());
  /*p05.CELA*/ joy_pin.P15_A.set(or (!JOYP_ABCS.q(), FF60_0n));
  /*p05.CELA*/ joy_pin.P15_D.set(!JOYP_ABCS.q()); // double check these
}
