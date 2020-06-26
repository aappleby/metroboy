#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;


void TestGB::tick_joypad() {
  auto clk_sig = clk_reg.sig(*this);
  auto adr_sig = adr_reg.sig(cpu_pins);
  auto rst_sig = rst_reg.sig(*this);
  auto cpu_sig = cpu_reg.sig(*this);

  /*p02.KERY*/ wire _ANY_BUTTON = or (joy_pin.P13_C, joy_pin.P12_C, joy_pin.P11_C, joy_pin.P10_C);

  /*p10.AMUS*/ wire _AMUS_0xx00000 = nor(cpu_pins.A00, cpu_pins.A01, cpu_pins.A02, cpu_pins.A03, cpu_pins.A04, cpu_pins.A07);
  /*p10.ANAP*/ wire _ANAP_0xx00000 = and (_AMUS_0xx00000, adr_sig.SYKE_FF00_FFFFp);

  /*p10.ACAT*/ wire _ACAT_FF00_RD = and (cpu_sig.TEDO_CPU_RD, _ANAP_0xx00000, adr_sig.AKUG_A06n, adr_sig.BYKO_A05n);
  /*p05.BYZO*/ wire _BYZO_FF00_RDn = not(_ACAT_FF00_RD);
  /*p10.ATOZ*/ wire _ATOZ_FF00_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, _ANAP_0xx00000, adr_sig.AKUG_A06n, adr_sig.BYKO_A05n);

  /*p02.AWOB*/ joy_reg.WAKE_CPU.set(clk_sig.BOGA_AxCDEFGH, _ANY_BUTTON);
  // cpu_pins.TO_CPU2.set(joy_reg.WAKE_CPU.q());

  /*p02.BATU*/ joy_reg.JP_GLITCH0.set(clk_sig.BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, _ANY_BUTTON);
  /*p02.ACEF*/ joy_reg.JP_GLITCH1.set(clk_sig.BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, joy_reg.JP_GLITCH0.q());
  /*p02.AGEM*/ joy_reg.JP_GLITCH2.set(clk_sig.BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, joy_reg.JP_GLITCH1.q());
  /*p02.APUG*/ joy_reg.JP_GLITCH3.set(clk_sig.BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, joy_reg.JP_GLITCH2.q());

  ///*p02.ASOK*/ wire INT_JP = and (joy_reg.JP_GLITCH3, joy_reg.JP_GLITCH0);

  /*p05.KEVU*/ joy_reg.JOYP_L0.set(_BYZO_FF00_RDn, joy_pin.P10_C);
  /*p05.KAPA*/ joy_reg.JOYP_L1.set(_BYZO_FF00_RDn, joy_pin.P11_C);
  /*p05.KEJA*/ joy_reg.JOYP_L2.set(_BYZO_FF00_RDn, joy_pin.P12_C);
  /*p05.KOLO*/ joy_reg.JOYP_L3.set(_BYZO_FF00_RDn, joy_pin.P13_C);

  /*p05.KEMA*/ cpu_pins.D0.set_tribuf(!_BYZO_FF00_RDn, joy_reg.JOYP_L0.q());
  /*p05.KURO*/ cpu_pins.D1.set_tribuf(!_BYZO_FF00_RDn, joy_reg.JOYP_L1.q());
  /*p05.KUVE*/ cpu_pins.D2.set_tribuf(!_BYZO_FF00_RDn, joy_reg.JOYP_L2.q());
  /*p05.JEKU*/ cpu_pins.D3.set_tribuf(!_BYZO_FF00_RDn, joy_reg.JOYP_L3.q());
  /*p05.KOCE*/ cpu_pins.D4.set_tribuf(!_BYZO_FF00_RDn, joy_reg.JOYP_UDLR.q());
  /*p05.CUDY*/ cpu_pins.D5.set_tribuf(!_BYZO_FF00_RDn, joy_reg.JOYP_ABCS.q());
  /*p??.????*/ cpu_pins.D6.set_tribuf(!_BYZO_FF00_RDn, joy_reg.DBG_FF00_D6.q());
  /*p??.????*/ cpu_pins.D7.set_tribuf(!_BYZO_FF00_RDn, joy_reg.DBG_FF00_D7.q());

  /*p05.JUTE*/ joy_reg.JOYP_RA    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_pins.D0);
  /*p05.KECY*/ joy_reg.JOYP_LB    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_pins.D1);
  /*p05.JALE*/ joy_reg.JOYP_UC    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_pins.D2);
  /*p05.KYME*/ joy_reg.JOYP_DS    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_pins.D3);
  /*p05.KELY*/ joy_reg.JOYP_UDLR  .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_pins.D4);
  /*p05.COFY*/ joy_reg.JOYP_ABCS  .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_pins.D5);
  /*p05.KUKO*/ joy_reg.DBG_FF00_D6.set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_pins.D6);
  /*p05.KERU*/ joy_reg.DBG_FF00_D7.set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_pins.D7);

  // FIXME
  wire FF60_0 = 0;
  wire FF60_0n = 1;

  /*p05.KOLE*/ joy_pin.P10_A.set(nand(joy_reg.JOYP_RA.q(), FF60_0));
  /*p05.KYBU*/ joy_pin.P10_D.set(nor(joy_reg.JOYP_RA.q(), FF60_0n));
  /*p05.KYTO*/ joy_pin.P11_A.set(nand(joy_reg.JOYP_LB.q(), FF60_0));
  /*p05.KABU*/ joy_pin.P11_D.set(nor(joy_reg.JOYP_LB.q(), FF60_0n));
  /*p05.KYHU*/ joy_pin.P12_A.set(nand(joy_reg.JOYP_UC.q(), FF60_0));
  /*p05.KASY*/ joy_pin.P12_D.set(nor(joy_reg.JOYP_UC.q(), FF60_0n)); // schematic wrong
  /*p05.KORY*/ joy_pin.P13_A.set(nand(joy_reg.JOYP_DS.q(), FF60_0));
  /*p05.KALE*/ joy_pin.P13_D.set(nor(joy_reg.JOYP_DS.q(), FF60_0n));
  /*p05.KARU*/ joy_pin.P14_A.set(or (!joy_reg.JOYP_UDLR.q(), FF60_0n));
  /*p05.KARU*/ joy_pin.P14_D.set(joy_reg.JOYP_UDLR.q());
  /*p05.CELA*/ joy_pin.P15_A.set(or (!joy_reg.JOYP_ABCS.q(), FF60_0n));
  /*p05.CELA*/ joy_pin.P15_D.set(!joy_reg.JOYP_ABCS.q()); // double check these
}
