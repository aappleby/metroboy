#include "P06_SerialLink.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P06_SerialLink::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // decoders

  c.p06.ADDR_XX00_XX07 = nor(b.A07, b.A06, b.A05, b.A04, b.A03); // XX00-XX07
  c.p06.A02n           = not(b.A02);
  c.p06.ADDR_FF00_FF03 = and(b.p06.ADDR_XX00_XX07, b.p06.A02n, b.p07.ADDR_FFXX);

  //----------
  // serial clock i think?

  c.p06.SER_CLK      = tock_pos(a.p01.CLK_16K, b.p01.CLK_16K, b.p06.FF02_WRn, b.p06.SER_CLK, !b.p06.SER_CLK);
  c.p06.SER_CLK_MUXn = mux2n(b.p06.SER_CLK, b.chip.SCK_C, b.p06.SCK_B);
  c.p06.SER_TICK     = or  (b.p06.SER_CLK_MUXn, !b.p06.XFER_START); // this must stop the clock or something when the transmit's done
  c.p06.SER_TICKn    = not (b.p06.SER_TICK);

  //----------
  // FF02 SC

  c.p06.SER_RST = and(b.p06.FF02_WRn, b.p01.SYS_RESETn1);
   
  c.p06.SER_CNT0 = tock_pos( a.p06.SER_TICK,  b.p06.SER_TICK, b.p06.SER_RST, b.p06.SER_CNT0, !b.p06.SER_CNT0);
  c.p06.SER_CNT1 = tock_pos(!a.p06.SER_CNT0, !b.p06.SER_CNT0, b.p06.SER_RST, b.p06.SER_CNT1, !b.p06.SER_CNT1);
  c.p06.SER_CNT2 = tock_pos(!a.p06.SER_CNT1, !b.p06.SER_CNT1, b.p06.SER_RST, b.p06.SER_CNT2, !b.p06.SER_CNT2);
  c.p06.SER_CNT3 = tock_pos(!a.p06.SER_CNT2, !b.p06.SER_CNT2, b.p06.SER_RST, b.p06.SER_CNT3, !b.p06.SER_CNT3);

  c.p06.SER_CNT3n  = not(b.p06.SER_CNT3);

  c.p06.SCK_Bn = not(b.chip.SCK_B);

  c.p06.SCK_A = nand(b.p06.SER_TICK, b.chip.SCK_B);
  c.p06.SCK_D = nand(b.p06.SER_TICK, b.p06.SCK_Bn);

  c.chip.SCK_A = b.p06.SCK_A;
  c.chip.SCK_B = b.p06.SCK_B;
  c.chip.SCK_D = b.p06.SCK_D;

  //----------
  // FF02 read

  c.p06.FF02_RD = and (b.p07.CPU_RD, b.p06.ADDR_FF00_FF03, b.p03.A0n, b.A01);
  c.p06.FF02_D0 = not(!b.p06.SCK_B);
  c.p06.FF02_D7 = not(!b.p06.XFER_START);

  if (b.p06.FF02_RD) {
    c.D0 = b.p06.FF02_D0;
    c.D7 = b.p06.FF02_D7;
  }

  //----------
  // FF02 write

  c.p06.FF02_WRn   = nand(b.p07.CPU_WR, b.p06.ADDR_FF00_FF03, b.p03.A0n, b.A01);
  c.p06.XFER_RESET = and(b.p06.SER_CNT3n, b.p01.SYS_RESETn1);
  c.p06.XFER_START = tock_pos(a.p06.FF02_WRn, b.p06.FF02_WRn, b.p06.XFER_RESET, b.p06.XFER_START, b.D7);
  c.p06.SCK_B      = tock_pos(a.p06.FF02_WRn, b.p06.FF02_WRn, b.p01.SYS_RESETn1,     b.p06.SCK_B,      b.D0);

  //----------
  // FF01 SB

  c.p06.FF01_WRn       = nand(b.p07.CPU_WR, b.p06.ADDR_FF00_FF03, b.A00, b.p08.A1n);
  c.p06.FF01_WR        = not (b.p06.FF01_WRn);

  c.p06.SER_DATA0_SETn = nand(b.D0, b.p06.FF01_WR);
  c.p06.SER_DATA1_SETn = nand(b.D1, b.p06.FF01_WR);
  c.p06.SER_DATA2_SETn = nand(b.D2, b.p06.FF01_WR);
  c.p06.SER_DATA3_SETn = nand(b.D3, b.p06.FF01_WR);
  c.p06.SER_DATA4_SETn = nand(b.D4, b.p06.FF01_WR);
  c.p06.SER_DATA5_SETn = nand(b.D5, b.p06.FF01_WR);
  c.p06.SER_DATA6_SETn = nand(b.D6, b.p06.FF01_WR);
  c.p06.SER_DATA7_SETn = nand(b.D7, b.p06.FF01_WR);

  c.p06.SER_DATA0_RSTn = or(and(b.p06.FF01_WRn, b.D0), b.p01.SYS_RESETn1);
  c.p06.SER_DATA1_RSTn = or(and(b.p06.FF01_WRn, b.D1), b.p01.SYS_RESETn1);
  c.p06.SER_DATA2_RSTn = or(and(b.p06.FF01_WRn, b.D2), b.p01.SYS_RESETn1);
  c.p06.SER_DATA3_RSTn = or(and(b.p06.FF01_WRn, b.D3), b.p01.SYS_RESETn1);
  c.p06.SER_DATA4_RSTn = or(and(b.p06.FF01_WRn, b.D4), b.p01.SYS_RESETn1);
  c.p06.SER_DATA5_RSTn = or(and(b.p06.FF01_WRn, b.D5), b.p01.SYS_RESETn1);
  c.p06.SER_DATA6_RSTn = or(and(b.p06.FF01_WRn, b.D6), b.p01.SYS_RESETn1);
  c.p06.SER_DATA7_RSTn = or(and(b.p06.FF01_WRn, b.D7), b.p01.SYS_RESETn1);

  // serial in.out shift register

  c.p06.SER_TICK2  = not(b.p06.SER_TICKn);
  c.p06.SER_TICKn2 = not(b.p06.SER_TICK2);
  c.p06.SER_TICK3  = not(b.p06.SER_TICKn2);

  c.p06.SIN_Cn = not(b.chip.SIN_C); // check this
  c.p06.SER_DATA0 = srtock_pos(a.p06.SER_TICK3, b.p06.SER_TICK3, b.p06.SER_DATA0_SETn, b.p06.SER_DATA0_RSTn, b.p06.SER_DATA0, b.p06.SIN_Cn);
  c.p06.SER_DATA1 = srtock_pos(a.p06.SER_TICK3, b.p06.SER_TICK3, b.p06.SER_DATA1_SETn, b.p06.SER_DATA1_RSTn, b.p06.SER_DATA1, b.p06.SER_DATA0);
  c.p06.SER_DATA2 = srtock_pos(a.p06.SER_TICK3, b.p06.SER_TICK3, b.p06.SER_DATA2_SETn, b.p06.SER_DATA2_RSTn, b.p06.SER_DATA2, b.p06.SER_DATA1);
  c.p06.SER_DATA3 = srtock_pos(a.p06.SER_TICK3, b.p06.SER_TICK3, b.p06.SER_DATA3_SETn, b.p06.SER_DATA3_RSTn, b.p06.SER_DATA3, b.p06.SER_DATA2);
  c.p06.SER_DATA4 = srtock_pos(a.p06.SER_TICK2, b.p06.SER_TICK2, b.p06.SER_DATA4_SETn, b.p06.SER_DATA4_RSTn, b.p06.SER_DATA4, b.p06.SER_DATA3);
  c.p06.SER_DATA5 = srtock_pos(a.p06.SER_TICK2, b.p06.SER_TICK2, b.p06.SER_DATA5_SETn, b.p06.SER_DATA5_RSTn, b.p06.SER_DATA5, b.p06.SER_DATA4);
  c.p06.SER_DATA6 = srtock_pos(a.p06.SER_TICK2, b.p06.SER_TICK2, b.p06.SER_DATA6_SETn, b.p06.SER_DATA6_RSTn, b.p06.SER_DATA6, b.p06.SER_DATA5);
  c.p06.SER_DATA7 = srtock_pos(a.p06.SER_TICK2, b.p06.SER_TICK2, b.p06.SER_DATA7_SETn, b.p06.SER_DATA7_RSTn, b.p06.SER_DATA7, b.p06.SER_DATA6);

  c.p06.SER_OUT = tock_pos(a.p06.SER_TICKn, b.p06.SER_TICKn, b.p01.SYS_RESETn1, b.p06.SER_OUT, b.p06.SER_DATA7);

  c.p06.FF01_D0 = not(!b.p06.FF01_D0);
  c.p06.FF01_D1 = not(!b.p06.FF01_D1);
  c.p06.FF01_D2 = not(!b.p06.FF01_D2);
  c.p06.FF01_D3 = not(!b.p06.FF01_D3);
  c.p06.FF01_D4 = not(!b.p06.FF01_D4);
  c.p06.FF01_D5 = not(!b.p06.FF01_D5);
  c.p06.FF01_D6 = not(!b.p06.FF01_D6);
  c.p06.FF01_D7 = not(!b.p06.FF01_D7);

  c.p06.FF01_RD = and(b.p07.CPU_RD, b.p06.ADDR_FF00_FF03, b.A00, b.p08.A1n);
  if (b.p06.FF01_RD) {
    c.D0 = b.p06.FF01_D0;
    c.D1 = b.p06.FF01_D1;
    c.D2 = b.p06.FF01_D2;
    c.D3 = b.p06.FF01_D3;
    c.D4 = b.p06.FF01_D4;
    c.D5 = b.p06.FF01_D5;
    c.D6 = b.p06.FF01_D6;
    c.D7 = b.p06.FF01_D7;
  }
}
