#include "Sch_Joypad.h"

#include "Sch_Resets.h"
#include "Sch_BusControl.h"
#include "Sch_Debug.h"
#include "Sch_Clocks.h"
#include "Sch_Pins.h"
#include "Sch_Decoder.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void JoypadRegisters::pwron() {
  pwron_all(JP_GLITCH0, JP_GLITCH1, JP_GLITCH2, JP_GLITCH3);
  pwron_all(JOYP_RA, JOYP_LB, JOYP_UC, JOYP_DS,
            JOYP_UDLR, JOYP_ABCS, DBG_FF00_D6, DBG_FF00_D7);
  pwron_all(JOYP_L0, JOYP_L1, JOYP_L2, JOYP_L3, WAKE_CPU);
}

void JoypadRegisters::reset() {
}

void JoypadRegisters::phase_begin() {
}

void JoypadRegisters::phase_end() {
}

void JoypadRegisters::pass_begin() {
}

bool JoypadRegisters::pass_end() {
  bool changed = false;
  changed |= commit_all(JP_GLITCH0, JP_GLITCH1, JP_GLITCH2, JP_GLITCH3);
  changed |= commit_all(JOYP_RA, JOYP_LB, JOYP_UC, JOYP_DS,
                        JOYP_UDLR, JOYP_ABCS, DBG_FF00_D6, DBG_FF00_D7);
  changed |= commit_all(JOYP_L0, JOYP_L1, JOYP_L2, JOYP_L3, WAKE_CPU);
  return changed;
}

//-----------------------------------------------------------------------------

JoypadSignals JoypadRegisters::tick(const ClkSignals& clk,
                                    const RstSignals& rst_sig,
                                    const BusSignals& bus_ctl,
                                    const DebugSignals& /*dbg_sig*/,
                                    const DecoderSignals& dec_sig,
                                    JoypadPins& joy_pins,
                                    BusTristates& bus_tri) {

  // FIXME
  wire FF60_0 = 0;
  wire FF60_0n = 1;

  /*p10.AKUG*/ wire A06n = not(bus_tri.A06());
  /*p10.BYKO*/ wire A05n = not(bus_tri.A05());

  /*p05.KOLE*/ joy_pins.P10_A = nand(JOYP_RA,  FF60_0);
  /*p05.KYBU*/ joy_pins.P10_D = nor (JOYP_RA,  FF60_0n);
  /*p05.KYTO*/ joy_pins.P11_A = nand(JOYP_LB,  FF60_0);
  /*p05.KABU*/ joy_pins.P11_D = nor (JOYP_LB,  FF60_0n);
  /*p05.KYHU*/ joy_pins.P12_A = nand(JOYP_UC,  FF60_0);
  /*p05.KASY*/ joy_pins.P12_D = nor (JOYP_UC,  FF60_0n); // schematic wrong
  /*p05.KORY*/ joy_pins.P13_A = nand(JOYP_DS,  FF60_0);
  /*p05.KALE*/ joy_pins.P13_D = nor (JOYP_DS,  FF60_0n);
  /*p05.KARU*/ joy_pins.P14_A = or(!JOYP_UDLR, FF60_0n);
  /*p05.KARU*/ joy_pins.P14_D = JOYP_UDLR;
  /*p05.CELA*/ joy_pins.P15_A = or(!JOYP_ABCS, FF60_0n);
  /*p05.KARU*/ joy_pins.P15_D = !JOYP_ABCS;

  /*p02.KERY*/ wire ANY_BUTTON = or(joy_pins.P13_C, joy_pins.P12_C, joy_pins.P11_C, joy_pins.P10_C);

  /*p02.AWOB*/ WAKE_CPU.set(ANY_BUTTON, clk.BOGA_xBCDEFGH);

  /*p02.BATU*/ JP_GLITCH0.set(clk.BOGA_xBCDEFGH, rst_sig.SYS_RESETn, ANY_BUTTON);
  /*p02.ACEF*/ JP_GLITCH1.set(clk.BOGA_xBCDEFGH, rst_sig.SYS_RESETn, JP_GLITCH0);
  /*p02.AGEM*/ JP_GLITCH2.set(clk.BOGA_xBCDEFGH, rst_sig.SYS_RESETn, JP_GLITCH1);
  /*p02.APUG*/ JP_GLITCH3.set(clk.BOGA_xBCDEFGH, rst_sig.SYS_RESETn, JP_GLITCH2);
  /*p02.ASOK*/ wire INT_JP = and(JP_GLITCH3, JP_GLITCH0);

  /*p10.ATOZ*/ wire FF00_WRn   = nand(bus_ctl.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000, A06n, A05n);
  /*p05.JUTE*/ JOYP_RA    .set(FF00_WRn, rst_sig.SYS_RESETn, bus_tri.D0());
  /*p05.KECY*/ JOYP_LB    .set(FF00_WRn, rst_sig.SYS_RESETn, bus_tri.D1());
  /*p05.JALE*/ JOYP_UC    .set(FF00_WRn, rst_sig.SYS_RESETn, bus_tri.D2());
  /*p05.KYME*/ JOYP_DS    .set(FF00_WRn, rst_sig.SYS_RESETn, bus_tri.D3());
  /*p05.KELY*/ JOYP_UDLR  .set(FF00_WRn, rst_sig.SYS_RESETn, bus_tri.D4());
  /*p05.COFY*/ JOYP_ABCS  .set(FF00_WRn, rst_sig.SYS_RESETn, bus_tri.D5());
  /*p05.KUKO*/ DBG_FF00_D6.set(FF00_WRn, rst_sig.SYS_RESETn, bus_tri.D6());
  /*p05.KERU*/ DBG_FF00_D7.set(FF00_WRn, rst_sig.SYS_RESETn, bus_tri.D7());

  /*p10.ACAT*/ wire FF00_RD = and (bus_ctl.TEDO_CPURD, dec_sig.ADDR_111111110xx00000, A06n, A05n);
  /*p05.BYZO*/ wire FF00_RDn = not(FF00_RD);
  /*p05.KEVU*/ JOYP_L0.set(joy_pins.P10_C, FF00_RDn);
  /*p05.KAPA*/ JOYP_L1.set(joy_pins.P11_C, FF00_RDn);
  /*p05.KEJA*/ JOYP_L2.set(joy_pins.P12_C, FF00_RDn);
  /*p05.KOLO*/ JOYP_L3.set(joy_pins.P13_C, FF00_RDn);

  if (!FF00_RDn) bus_tri.set_data(
    /*p05.KEMA*/ JOYP_L0,
    /*p05.KURO*/ JOYP_L1,
    /*p05.KUVE*/ JOYP_L2,
    /*p05.JEKU*/ JOYP_L3,
    /*p05.KOCE*/ JOYP_UDLR,
    /*p05.CUDY*/ JOYP_ABCS,
    /*p05.KUKO*/ DBG_FF00_D6,
    /*p05.KERU*/ DBG_FF00_D7
  );

  return {
    .INT_JP = INT_JP,
    .WAKE_CPU = WAKE_CPU,
  };
}

//-----------------------------------------------------------------------------

};