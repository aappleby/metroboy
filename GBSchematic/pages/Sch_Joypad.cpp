#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void Joypad_tick(const JoypadTickIn& in, JoypadTickOut& out, const Joypad& a, const Joypad& b, Joypad& c) {

  bool RESETn = in.SYS_RESETn;

  /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!in.PHASE_ABCDxxxx1);  
  /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( in.PHASE_xBCDExxx1);
  /*p01.AROV*/ wire AROV_xxCDEFxx = not(!in.PHASE_xxCDEFxx1);

  /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(in.CPUCLK_REQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*p01.NULE*/ wire NULE_xxxxEFGH = nor(in.CPUCLK_REQn, ATYP_ABCDxxxx);
  /*p01.BERU*/ wire BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
  /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
  /*p01.BUFA*/ wire BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
  /*p01.BOLO*/ wire BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);
  /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
  /*p01.BEKO*/ wire BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
  /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
  /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
  /*p01.BELO*/ wire BELO_xxxxEFGH = not(BANE_ABCDxxxx);
  /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
  /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
  /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, in.CLK_BAD2);
  /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);

  /*p01.BOGA*/ c.BOGA_xBCDEFGH = not(BALY_Axxxxxxx); // joypad, timer

  /*p10.AMUS*/ wire ADDR_xxxxxxxx0xx00000 = nor(in.A00, in.A01, in.A02, in.A03, in.A04, in.A07);
  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(in.A15, in.A14, in.A13, in.A12, in.A11, in.A10, in.A09);
  /*p07.TONA*/ wire ADDR_08n = not(in.A08);
  /*p07.SYKE*/ wire ADDR_FFXX = nor(ADDR_0000_FE00, ADDR_08n);
  /*p10.ANAP*/ wire ADDR_111111110xx00000 = and(ADDR_xxxxxxxx0xx00000, ADDR_FFXX);
  /*p10.AKUG*/ wire A06n = not(in.A06);
  /*p10.BYKO*/ wire A05n = not(in.A05);

  /*p10.ACAT*/ wire FF00_RD = and (in.CPU_RD, ADDR_111111110xx00000, A06n, A05n);
  /*p10.ATOZ*/ c.FF00_WRn   = nand(in.CPU_WR, ADDR_111111110xx00000, A06n, A05n);

  /*p02.KERY*/ wire ANY_BUTTON = or(in.P13_C, in.P12_C, in.P11_C, in.P10_C);
  /*p02.ASOK*/ c.INT_JP  = and(b.JP_GLITCH3, b.JP_GLITCH0);
  /*p02.AWOB*/ c.TO_CPU2 = latch_pos(b.BOGA_xBCDEFGH, b.TO_CPU2, ANY_BUTTON);

  /*p05.KURA*/ wire FF60_0n = not(in.FF60_0);

  // FIXME really unsure about these pin assignments, seem to have a few missing signals

  /*p05.KOLE*/ out.P10_A = nand(b.JOYP_RA, in.FF60_0);
  /*p05.KYBU*/ out.P10_D = nor (b.JOYP_RA, FF60_0n);

  /*p05.KYTO*/ out.P11_A = nand(b.JOYP_LB, in.FF60_0);
  /*p05.KABU*/ out.P11_D = nor (b.JOYP_LB, FF60_0n);

  /*p05.KYHU*/ out.P12_A = nand(b.JOYP_UC, in.FF60_0);
  /*p05.KASY*/ out.P12_D = nor (b.JOYP_UC, FF60_0n); // schematic wrong

  /*p05.KORY*/ out.P13_A = nand(b.JOYP_DS, in.FF60_0);
  /*p05.KALE*/ out.P13_D = nor (b.JOYP_DS, FF60_0n);

  /*p05.KARU*/ out.P14_A = or(!b.JOYP_UDLR, FF60_0n);
  /*p05.KARU*/ out.P14_D = b.JOYP_UDLR;

  /*p05.CELA*/ out.P15_A = or(!b.JOYP_ABCS, FF60_0n);
  /*p05.KARU*/ out.P15_D = !b.JOYP_ABCS;

  /*p05.BYZO*/ wire FF00_RDn = not(FF00_RD);
  /*p05.KEVU*/ c.JOYP_L0 = latch_pos(FF00_RDn, b.JOYP_L0, in.P10_C);
  /*p05.KAPA*/ c.JOYP_L1 = latch_pos(FF00_RDn, b.JOYP_L1, in.P11_C);
  /*p05.KEJA*/ c.JOYP_L2 = latch_pos(FF00_RDn, b.JOYP_L2, in.P12_C);
  /*p05.KOLO*/ c.JOYP_L3 = latch_pos(FF00_RDn, b.JOYP_L3, in.P13_C);

  /*p05.KEMA*/ if (!FF00_RDn) out.D0 = b.JOYP_L0;
  /*p05.KURO*/ if (!FF00_RDn) out.D1 = b.JOYP_L1;
  /*p05.KUVE*/ if (!FF00_RDn) out.D2 = b.JOYP_L2;
  /*p05.JEKU*/ if (!FF00_RDn) out.D3 = b.JOYP_L3;
  /*p05.KOCE*/ if (!FF00_RDn) out.D4 = b.JOYP_UDLR;
  /*p05.CUDY*/ if (!FF00_RDn) out.D5 = b.JOYP_ABCS;
}

//-----------------------------------------------------------------------------

void Joypad_tock(const JoypadTickIn& in, JoypadTickOut& out, const Joypad& a, const Joypad& b, Joypad& c) {

  bool RESETn = in.SYS_RESETn;

  /*p02.KERY*/ wire ANY_BUTTON = or(in.P13_C, in.P12_C, in.P11_C, in.P10_C);
  /*p02.BATU*/ c.JP_GLITCH0  = tock_pos(a.BOGA_xBCDEFGH, b.BOGA_xBCDEFGH, RESETn, b.JP_GLITCH0, ANY_BUTTON);
  /*p02.ACEF*/ c.JP_GLITCH1  = tock_pos(a.BOGA_xBCDEFGH, b.BOGA_xBCDEFGH, RESETn, b.JP_GLITCH1, b.JP_GLITCH0);
  /*p02.AGEM*/ c.JP_GLITCH2  = tock_pos(a.BOGA_xBCDEFGH, b.BOGA_xBCDEFGH, RESETn, b.JP_GLITCH2, b.JP_GLITCH1);
  /*p02.APUG*/ c.JP_GLITCH3  = tock_pos(a.BOGA_xBCDEFGH, b.BOGA_xBCDEFGH, RESETn, b.JP_GLITCH3, c.JP_GLITCH2);

  /*p05.JUTE*/ c.JOYP_RA     = tock_pos(a.FF00_WRn, b.FF00_WRn, RESETn, c.JOYP_RA,     in.D0);
  /*p05.KECY*/ c.JOYP_LB     = tock_pos(a.FF00_WRn, b.FF00_WRn, RESETn, c.JOYP_LB,     in.D1);
  /*p05.JALE*/ c.JOYP_UC     = tock_pos(a.FF00_WRn, b.FF00_WRn, RESETn, c.JOYP_UC,     in.D2);
  /*p05.KYME*/ c.JOYP_DS     = tock_pos(a.FF00_WRn, b.FF00_WRn, RESETn, c.JOYP_DS,     in.D3);
  /*p05.KELY*/ c.JOYP_UDLR   = tock_pos(a.FF00_WRn, b.FF00_WRn, RESETn, c.JOYP_UDLR,   in.D4);
  /*p05.COFY*/ c.JOYP_ABCS   = tock_pos(a.FF00_WRn, b.FF00_WRn, RESETn, c.JOYP_ABCS,   in.D5);
  /*p05.KUKO*/ c.DBG_FF00_D6 = tock_pos(a.FF00_WRn, b.FF00_WRn, RESETn, c.DBG_FF00_D6, in.D6);
  /*p05.KERU*/ c.DBG_FF00_D7 = tock_pos(a.FF00_WRn, b.FF00_WRn, RESETn, c.DBG_FF00_D7, in.D7);
}

//-----------------------------------------------------------------------------

};