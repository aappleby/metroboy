#include "Sch_Joypad.h"

#include "TestGB.h"

using namespace Schematics;

//------------------------------------------------------------------------------

JoypadSignals JoypadRegisters::sig() const {
  /*p02.ASOK*/ wire ASOK_INT_JPp = and (JP_GLITCH3, JP_GLITCH0);

  return {
    .ASOK_INT_JPp = ASOK_INT_JPp,
  };
}

//------------------------------------------------------------------------------

void JoypadRegisters::tick(ResetSignals& rst_sig, ClockSignals& clk_sig, AddressSignals& adr_sig, CpuBus& cpu_bus, CpuBusSignals& cpu_sig) {

  /*p02.KERY*/ wire _ANY_BUTTON = or(P13_C, P12_C, P11_C, P10_C);

  /*p10.AMUS*/ wire _AMUS_0xx00000 = nor(cpu_bus.PIN_A00, cpu_bus.PIN_A01, cpu_bus.PIN_A02, cpu_bus.PIN_A03, cpu_bus.PIN_A04, cpu_bus.PIN_A07);
  /*p10.ANAP*/ wire _ANAP_0xx00000 = and (_AMUS_0xx00000, adr_sig.SYKE_FF00_FFFFp);

  /*p10.ACAT*/ wire _ACAT_FF00_RD = and (cpu_sig.TEDO_CPU_RD, _ANAP_0xx00000, adr_sig.AKUG_A06n, adr_sig.BYKO_A05n);
  /*p05.BYZO*/ wire _BYZO_FF00_RDn = not(_ACAT_FF00_RD);
  /*p10.ATOZ*/ wire _ATOZ_FF00_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, _ANAP_0xx00000, adr_sig.AKUG_A06n, adr_sig.BYKO_A05n);

  /*p02.AWOB*/ AWOB_WAKE_CPU.set(clk_sig.BOGA_AxCDEFGH, _ANY_BUTTON);
  // cpu_pins.TO_CPU2.set(WAKE_CPU.q());

  /*p02.BATU*/ JP_GLITCH0.set(clk_sig.BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, _ANY_BUTTON);
  /*p02.ACEF*/ JP_GLITCH1.set(clk_sig.BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, JP_GLITCH0.q());
  /*p02.AGEM*/ JP_GLITCH2.set(clk_sig.BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, JP_GLITCH1.q());
  /*p02.APUG*/ JP_GLITCH3.set(clk_sig.BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, JP_GLITCH2.q());

  ///*p02.ASOK*/ wire INT_JP = and (JP_GLITCH3, JP_GLITCH0);

  /*p05.KEVU*/ JOYP_L0.set(_BYZO_FF00_RDn, P10_C);
  /*p05.KAPA*/ JOYP_L1.set(_BYZO_FF00_RDn, P11_C);
  /*p05.KEJA*/ JOYP_L2.set(_BYZO_FF00_RDn, P12_C);
  /*p05.KOLO*/ JOYP_L3.set(_BYZO_FF00_RDn, P13_C);

  /*p05.KEMA*/ cpu_bus.TRI_D0.set_tribuf(!_BYZO_FF00_RDn, JOYP_L0.q());
  /*p05.KURO*/ cpu_bus.TRI_D1.set_tribuf(!_BYZO_FF00_RDn, JOYP_L1.q());
  /*p05.KUVE*/ cpu_bus.TRI_D2.set_tribuf(!_BYZO_FF00_RDn, JOYP_L2.q());
  /*p05.JEKU*/ cpu_bus.TRI_D3.set_tribuf(!_BYZO_FF00_RDn, JOYP_L3.q());
  /*p05.KOCE*/ cpu_bus.TRI_D4.set_tribuf(!_BYZO_FF00_RDn, JOYP_UDLR.q());
  /*p05.CUDY*/ cpu_bus.TRI_D5.set_tribuf(!_BYZO_FF00_RDn, JOYP_ABCS.q());
  /*p??.????*/ cpu_bus.TRI_D6.set_tribuf(!_BYZO_FF00_RDn, DBG_FF00_D6.q());
  /*p??.????*/ cpu_bus.TRI_D7.set_tribuf(!_BYZO_FF00_RDn, DBG_FF00_D7.q());

  /*p05.JUTE*/ JOYP_RA    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TRI_D0);
  /*p05.KECY*/ JOYP_LB    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TRI_D1);
  /*p05.JALE*/ JOYP_UC    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TRI_D2);
  /*p05.KYME*/ JOYP_DS    .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TRI_D3);
  /*p05.KELY*/ JOYP_UDLR  .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TRI_D4);
  /*p05.COFY*/ JOYP_ABCS  .set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TRI_D5);
  /*p05.KUKO*/ DBG_FF00_D6.set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TRI_D6);
  /*p05.KERU*/ DBG_FF00_D7.set(_ATOZ_FF00_WRn, rst_sig.ALUR_RSTn, cpu_bus.TRI_D7);

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

bool JoypadRegisters::commit() {
  bool changed = false;
  /*p02.BATU*/ changed |= JP_GLITCH0.commit_reg();
  /*p02.ACEF*/ changed |= JP_GLITCH1.commit_reg();
  /*p02.AGEM*/ changed |= JP_GLITCH2.commit_reg();
  /*p02.APUG*/ changed |= JP_GLITCH3.commit_reg();
  /*p05.JUTE*/ changed |= JOYP_RA.commit_reg();
  /*p05.KECY*/ changed |= JOYP_LB.commit_reg();
  /*p05.JALE*/ changed |= JOYP_UC.commit_reg();
  /*p05.KYME*/ changed |= JOYP_DS.commit_reg();
  /*p05.KELY*/ changed |= JOYP_UDLR.commit_reg();
  /*p05.COFY*/ changed |= JOYP_ABCS.commit_reg();
  /*p05.KUKO*/ changed |= DBG_FF00_D6.commit_reg();
  /*p05.KERU*/ changed |= DBG_FF00_D7.commit_reg();
  /*p05.KEVU*/ changed |= JOYP_L0.commit_reg();
  /*p05.KAPA*/ changed |= JOYP_L1.commit_reg();
  /*p05.KEJA*/ changed |= JOYP_L2.commit_reg();
  /*p05.KOLO*/ changed |= JOYP_L3.commit_reg();
  /*p02.AWOB*/ changed |= AWOB_WAKE_CPU.commit_reg();

  /* PIN_58 */ /*VCC*/
  /* PIN_59 */ /*ROUT*/
  /* PIN_60 */ /*LOUT*/
  /* PIN_61 */ /*VIN*/

  /* PIN_62 */ changed |= P15_A.commit_pinout();   // <- CELA
  /* PIN_62 */ changed |= P15_D.commit_pinout();   // <- COFY
  /* PIN_63 */ changed |= P14_A.commit_pinout();   // <- KARU
  /* PIN_63 */ changed |= P14_D.commit_pinout();   // <- KELY
  /* PIN_64 */ changed |= P13_A.commit_pinout();   // <- KORY
  /* PIN_64 */ changed |= P13_B.commit_pinout();   
  /* PIN_64 */ changed |= P13_C.clear_preset();    // -> KERY, P05.KOLO
  /* PIN_64 */ changed |= P13_D.commit_pinout();   // <- KALE
  /* PIN_65 */ changed |= P12_A.commit_pinout();   // <- KYHU
  /* PIN_65 */ changed |= P12_B.commit_pinout();   
  /* PIN_65 */ changed |= P12_C.clear_preset();    // -> KERY, P05.KEJA
  /* PIN_65 */ changed |= P12_D.commit_pinout();   // <- KASY
  /* PIN_66 */ changed |= P11_A.commit_pinout();   // <- KYTO
  /* PIN_66 */ changed |= P11_B.commit_pinout();   
  /* PIN_66 */ changed |= P11_C.clear_preset();    // -> KERY, P05.KAPA
  /* PIN_66 */ changed |= P11_D.commit_pinout();   // <- KABU
  /* PIN_67 */ changed |= P10_A.commit_pinout();   // <- KOLE
  /* PIN_67 */ changed |= P10_B.commit_pinout();   
  /* PIN_67 */ changed |= P10_C.clear_preset();    // -> KERY, KEVU
  /* PIN_67 */ changed |= P10_D.commit_pinout();   // <- KYBU

  return changed;
}

//------------------------------------------------------------------------------