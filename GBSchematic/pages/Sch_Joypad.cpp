#include "Sch_Joypad.h"

#include "Sch_Resets.h"
#include "Sch_BusControl.h"
#include "Sch_Debug.h"
#include "Sch_Clocks.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Joypad_tick(const Bus& bus,
                 const BusControl& ctl,
                 const Debug& dbg,
                 const ClockSignals& clk,
                 const Joypad& prev,
                 Joypad& next,
                 Bus& bus_out) {
  /*p10.AMUS*/ wire ADDR_xxxxxxxx0xx00000 = nor(bus.A00, bus.A01, bus.A02, bus.A03, bus.A04, bus.A07);
  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(bus.A15, bus.A14, bus.A13, bus.A12, bus.A11, bus.A10, bus.A09);
  /*p07.TONA*/ wire ADDR_08n = not(bus.A08);
  /*p07.SYKE*/ wire ADDR_FFXX = nor(ADDR_0000_FE00, ADDR_08n);
  /*p10.ANAP*/ wire ADDR_111111110xx00000 = and(ADDR_xxxxxxxx0xx00000, ADDR_FFXX);
  /*p10.AKUG*/ wire A06n = not(bus.A06);
  /*p10.BYKO*/ wire A05n = not(bus.A05);

  /*p02.KERY*/ wire ANY_BUTTON = or(prev.PIN_P13_C, prev.PIN_P12_C, prev.PIN_P11_C, prev.PIN_P10_C);
  /*p02.ASOK*/ next.INT_JP  = and(prev.JP_GLITCH3, prev.JP_GLITCH0);
  /*p02.AWOB*/ next.WAKE_CPU = latch_pos(clk.BOGA_AxCDEFGH, prev.WAKE_CPU, ANY_BUTTON);

  // FIXME really unsure about these pin assignments, seem to have a few missing signals

  /*p05.KOLE*/ next.PIN_P10_A = nand(prev.JOYP_RA, dbg.FF60_0);
  /*p05.KYBU*/ next.PIN_P10_D = nor (prev.JOYP_RA, dbg.FF60_0n);
  /*p05.KYTO*/ next.PIN_P11_A = nand(prev.JOYP_LB, dbg.FF60_0);
  /*p05.KABU*/ next.PIN_P11_D = nor (prev.JOYP_LB, dbg.FF60_0n);
  /*p05.KYHU*/ next.PIN_P12_A = nand(prev.JOYP_UC, dbg.FF60_0);
  /*p05.KASY*/ next.PIN_P12_D = nor (prev.JOYP_UC, dbg.FF60_0n); // schematic wrong
  /*p05.KORY*/ next.PIN_P13_A = nand(prev.JOYP_DS, dbg.FF60_0);
  /*p05.KALE*/ next.PIN_P13_D = nor (prev.JOYP_DS, dbg.FF60_0n);
  /*p05.KARU*/ next.PIN_P14_A = or(!prev.JOYP_UDLR, dbg.FF60_0n);
  /*p05.KARU*/ next.PIN_P14_D = prev.JOYP_UDLR;
  /*p05.CELA*/ next.PIN_P15_A = or(!prev.JOYP_ABCS, dbg.FF60_0n);
  /*p05.KARU*/ next.PIN_P15_D = !prev.JOYP_ABCS;

  /*p10.ACAT*/ wire FF00_RD = and (ctl.CPU_RD, ADDR_111111110xx00000, A06n, A05n);
  /*p05.BYZO*/ wire FF00_RDn = not(FF00_RD);

  /*p05.KEVU*/ next.JOYP_L0 = latch_pos(FF00_RDn, prev.JOYP_L0, prev.PIN_P10_C);
  /*p05.KAPA*/ next.JOYP_L1 = latch_pos(FF00_RDn, prev.JOYP_L1, prev.PIN_P11_C);
  /*p05.KEJA*/ next.JOYP_L2 = latch_pos(FF00_RDn, prev.JOYP_L2, prev.PIN_P12_C);
  /*p05.KOLO*/ next.JOYP_L3 = latch_pos(FF00_RDn, prev.JOYP_L3, prev.PIN_P13_C);

  /*p05.KEMA*/ if (!FF00_RDn) bus_out.D0 = prev.JOYP_L0;
  /*p05.KURO*/ if (!FF00_RDn) bus_out.D1 = prev.JOYP_L1;
  /*p05.KUVE*/ if (!FF00_RDn) bus_out.D2 = prev.JOYP_L2;
  /*p05.JEKU*/ if (!FF00_RDn) bus_out.D3 = prev.JOYP_L3;
  /*p05.KOCE*/ if (!FF00_RDn) bus_out.D4 = prev.JOYP_UDLR;
  /*p05.CUDY*/ if (!FF00_RDn) bus_out.D5 = prev.JOYP_ABCS;
}

//-----------------------------------------------------------------------------

void Joypad_tock(const Bus& bus,
                 const BusControl& ctl,
                 const ClockSignals& clk,
                 const ResetRegisters& rst,
                 const Joypad& prev,
                 Joypad& next) {

  /*p10.AMUS*/ wire ADDR_xxxxxxxx0xx00000 = nor(bus.A00, bus.A01, bus.A02, bus.A03, bus.A04, bus.A07);
  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(bus.A15, bus.A14, bus.A13, bus.A12, bus.A11, bus.A10, bus.A09);
  /*p07.TONA*/ wire ADDR_08n = not(bus.A08);
  /*p07.SYKE*/ wire ADDR_FFXX = nor(ADDR_0000_FE00, ADDR_08n);
  /*p10.ANAP*/ wire ADDR_111111110xx00000 = and(ADDR_xxxxxxxx0xx00000, ADDR_FFXX);
  /*p10.AKUG*/ wire A06n = not(bus.A06);
  /*p10.BYKO*/ wire A05n = not(bus.A05);

  /*p02.KERY*/ wire ANY_BUTTON = or(prev.PIN_P13_C, prev.PIN_P12_C, prev.PIN_P11_C, prev.PIN_P10_C);
  /*p02.BATU*/ next.JP_GLITCH0.tock(clk.BOGA_AxCDEFGH, rst.SYS_RESETn, ANY_BUTTON);
  /*p02.ACEF*/ next.JP_GLITCH1.tock(clk.BOGA_AxCDEFGH, rst.SYS_RESETn, prev.JP_GLITCH0);
  /*p02.AGEM*/ next.JP_GLITCH2.tock(clk.BOGA_AxCDEFGH, rst.SYS_RESETn, prev.JP_GLITCH1);
  /*p02.APUG*/ next.JP_GLITCH3.tock(clk.BOGA_AxCDEFGH, rst.SYS_RESETn, prev.JP_GLITCH2);

  /*p10.ATOZ*/ wire FF00_WRn   = nand(ctl.CPU_WR, ADDR_111111110xx00000, A06n, A05n);
  /*p05.JUTE*/ next.JOYP_RA    .tock(FF00_WRn, rst.SYS_RESETn, bus.D0);
  /*p05.KECY*/ next.JOYP_LB    .tock(FF00_WRn, rst.SYS_RESETn, bus.D1);
  /*p05.JALE*/ next.JOYP_UC    .tock(FF00_WRn, rst.SYS_RESETn, bus.D2);
  /*p05.KYME*/ next.JOYP_DS    .tock(FF00_WRn, rst.SYS_RESETn, bus.D3);
  /*p05.KELY*/ next.JOYP_UDLR  .tock(FF00_WRn, rst.SYS_RESETn, bus.D4);
  /*p05.COFY*/ next.JOYP_ABCS  .tock(FF00_WRn, rst.SYS_RESETn, bus.D5);
  /*p05.KUKO*/ next.DBG_FF00_D6.tock(FF00_WRn, rst.SYS_RESETn, bus.D6);
  /*p05.KERU*/ next.DBG_FF00_D7.tock(FF00_WRn, rst.SYS_RESETn, bus.D7);
}

//-----------------------------------------------------------------------------

};