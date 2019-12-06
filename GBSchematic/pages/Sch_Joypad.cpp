#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void Joypad_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  bool RESETn = b.sys.SYS_RESETn1;

  {
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!b.sys.PHASE_ABCDxxxx1);  
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( b.sys.PHASE_xBCDExxx1);
    /*p01.AROV*/ wire AROV_xxCDEFxx = not(!b.sys.PHASE_xxCDEFxx1);

    /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(b.sys.CPUCLK_REQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor(b.sys.CPUCLK_REQn, ATYP_ABCDxxxx);
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
    /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, b.sys.CLK_BAD2);
    /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);

    /*p01.BOGA*/ c.sys.BOGA_xBCDEFGH = not(BALY_Axxxxxxx); // joypad, timer
  }

  wire aBOGA_xBCDEFGH = a.sys.BOGA_xBCDEFGH;
  wire bBOGA_xBCDEFGH = b.sys.BOGA_xBCDEFGH;

  //----------
  // P02

  /*p02.AWOB*/ c.sys.TO_CPU2 = latch_pos(bBOGA_xBCDEFGH, b.sys.TO_CPU2, b.sys.ANY_BUTTON);

  /*p02.KERY*/ c.sys.ANY_BUTTON = or(chip_in.P13_C, chip_in.P12_C, chip_in.P11_C, chip_in.P10_C);
  /*p02.BATU*/ c.sys.JP_GLITCH0 = tock_pos(aBOGA_xBCDEFGH, bBOGA_xBCDEFGH, RESETn, b.sys.JP_GLITCH0, b.sys.ANY_BUTTON);
  /*p02.ACEF*/ c.sys.JP_GLITCH1 = tock_pos(aBOGA_xBCDEFGH, bBOGA_xBCDEFGH, RESETn, b.sys.JP_GLITCH1, b.sys.JP_GLITCH0);
  /*p02.AGEM*/ c.sys.JP_GLITCH2 = tock_pos(aBOGA_xBCDEFGH, bBOGA_xBCDEFGH, RESETn, b.sys.JP_GLITCH2, b.sys.JP_GLITCH1);
  /*p02.APUG*/ c.sys.JP_GLITCH3 = tock_pos(aBOGA_xBCDEFGH, bBOGA_xBCDEFGH, RESETn, b.sys.JP_GLITCH3, c.sys.JP_GLITCH2);

  /*p02.ASOK*/ c.sys.INT_JP = and(b.sys.JP_GLITCH3, b.sys.JP_GLITCH0);

  /*p10.AMUS*/ c.sys.ADDR_xxxxxxxx0xx00000 = nor(b.A00, b.A01, b.A02, b.A03, b.A04, b.A07);
  /*p10.ANAP*/ c.sys.ADDR_111111110xx00000 = and(b.sys.ADDR_xxxxxxxx0xx00000, b.sys.ADDR_FFXX);
  /*p10.ATOZ*/ c.sys.FF00_CLK    = nand(b.sys.CPU_WR, b.sys.ADDR_111111110xx00000, /*p10.AKUG*/ not(b.A06), /*p10.BYKO*/ not(b.A05));

  /*p05.JUTE*/ c.sys.JOYP_RA     = tock_pos(a.sys.FF00_CLK, b.sys.FF00_CLK, RESETn, c.sys.JOYP_RA,     b.D0);
  /*p05.KECY*/ c.sys.JOYP_LB     = tock_pos(a.sys.FF00_CLK, b.sys.FF00_CLK, RESETn, c.sys.JOYP_LB,     b.D1);
  /*p05.JALE*/ c.sys.JOYP_UC     = tock_pos(a.sys.FF00_CLK, b.sys.FF00_CLK, RESETn, c.sys.JOYP_UC,     b.D2);
  /*p05.KYME*/ c.sys.JOYP_DS     = tock_pos(a.sys.FF00_CLK, b.sys.FF00_CLK, RESETn, c.sys.JOYP_DS,     b.D3);
  /*p05.KELY*/ c.sys.KELY        = tock_pos(a.sys.FF00_CLK, b.sys.FF00_CLK, RESETn, c.sys.KELY,        b.D4);
  /*p05.COFY*/ c.sys.COFY        = tock_pos(a.sys.FF00_CLK, b.sys.FF00_CLK, RESETn, c.sys.COFY,        b.D5);
  /*p05.KUKO*/ c.sys.DBG_FF00_D6 = tock_pos(a.sys.FF00_CLK, b.sys.FF00_CLK, RESETn, c.sys.DBG_FF00_D6, b.D6);
  /*p05.KERU*/ c.sys.DBG_FF00_D7 = tock_pos(a.sys.FF00_CLK, b.sys.FF00_CLK, RESETn, c.sys.DBG_FF00_D7, b.D7);

  /*p05.KURA*/ wire FF60_0n = not(b.sys.FF60_0);

  // FIXME really unsure about these pin assignments, seem to have a few missing signals

  /*p05.KOLE*/ c.chip_out.P10_A = nand(b.sys.JOYP_RA, b.sys.FF60_0);
  /*p05.KYBU*/ c.chip_out.P10_D = nor (b.sys.JOYP_RA, FF60_0n);

  /*p05.KYTO*/ c.chip_out.P11_A = nand(b.sys.JOYP_LB, b.sys.FF60_0);
  /*p05.KABU*/ c.chip_out.P11_D = nor (b.sys.JOYP_LB, FF60_0n);

  /*p05.KYHU*/ c.chip_out.P12_A = nand(b.sys.JOYP_UC, b.sys.FF60_0);
  /*p05.KASY*/ c.chip_out.P12_D = nor (b.sys.FF60_0,  FF60_0n); // this one doesn't match?

  /*p05.KORY*/ c.chip_out.P13_A = nand(b.sys.JOYP_DS, b.sys.FF60_0);
  /*p05.KALE*/ c.chip_out.P13_D = nor (b.sys.JOYP_DS, FF60_0n);

  /*p05.KARU*/ c.chip_out.P14_A = or(!b.sys.KELY, FF60_0n);
  /*p05.KARU*/ c.chip_out.P14_D = b.sys.KELY;

  /*p05.CELA*/ c.chip_out.P15_A = or(!b.sys.COFY, FF60_0n);
  /*p05.KARU*/ c.chip_out.P15_D = b.sys.COFY;

  /*p10.AKUG*/ wire AKUG = not(b.A06);
  /*p10.BYKO*/ wire BYKO = not(b.A05);
  /*p10.ACAT*/ wire FF00_RD  = and(b.sys.CPU_RD, b.sys.ADDR_111111110xx00000, AKUG, BYKO);
  /*p05.BYZO*/ wire FF00_RDn = not(FF00_RD);

  /*p05.KEVU*/ c.sys.JOYP_L0 = latch_pos(FF00_RDn, b.sys.JOYP_L0, chip_in.P10_C);
  /*p05.KAPA*/ c.sys.JOYP_L1 = latch_pos(FF00_RDn, b.sys.JOYP_L1, chip_in.P11_C);
  /*p05.KEJA*/ c.sys.JOYP_L2 = latch_pos(FF00_RDn, b.sys.JOYP_L2, chip_in.P12_C);
  /*p05.KOLO*/ c.sys.JOYP_L3 = latch_pos(FF00_RDn, b.sys.JOYP_L3, chip_in.P13_C);

  // polarity?
  /*p05.KEMA*/ if (FF00_RDn) c.D0 = b.sys.JOYP_L0;
  /*p05.KURO*/ if (FF00_RDn) c.D1 = b.sys.JOYP_L1;
  /*p05.KUVE*/ if (FF00_RDn) c.D2 = b.sys.JOYP_L2;
  /*p05.JEKU*/ if (FF00_RDn) c.D3 = b.sys.JOYP_L3;
  /*p05.KOCE*/ if (FF00_RDn) c.D4 = b.sys.KELY;
  /*p05.CUDY*/ if (FF00_RDn) c.D5 = b.sys.COFY;
}

//-----------------------------------------------------------------------------

};