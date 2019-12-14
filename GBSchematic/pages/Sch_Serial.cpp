#include "Sch_Serial.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Serial_tick(const SerialInput& in, SerialOutput& out, const Serial& a, const Serial& b, Serial& next) {

  /*p03.TOVY*/ wire A0n = not(in.A00);
  /*p08.TOLA*/ wire A1n = not(in.A01);
  /*p06.SEFY*/ wire A2n = not(in.A02);

  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(in.A15, in.A14, in.A13, in.A12, in.A11, in.A10, in.A09);
  /*p07.TONA*/ wire ADDR_08n = not(in.A08);
  /*p07.SYKE*/ wire ADDR_FFXX = nor(ADDR_0000_FE00, ADDR_08n);
  /*p06.SARE*/ wire ADDR_XX00_XX07 = nor(in.A07, in.A06, in.A05, in.A04, in.A03);
  /*p06.SANO*/ wire ADDR_FF00_FF03 = and(ADDR_XX00_XX07, A2n, ADDR_FFXX);

  //----------
  // Serial port

  /*p06.UWAM*/ next.FF02_CLK = nand(in.CPU_WR, ADDR_FF00_FF03, in.A01, A0n);
  /*p06.CABY*/ wire XFER_RESET = and(b.SER_CNT3n, in.SYS_RESETn);
  /*p06.ETAF*/ next.XFER_START = tock_pos(a.FF02_CLK, b.FF02_CLK, XFER_RESET,     b.XFER_START, in.D7);
  /*p06.CULY*/ next.XFER_DIR   = tock_pos(a.FF02_CLK, b.FF02_CLK, in.SYS_RESETn, b.XFER_DIR,   in.D0);

  /*p01.UVYN*/ next.CLK_16K = not(in.DIV_05);
  /*p06.COTY*/ next.SER_CLK = tock_pos(a.CLK_16K, b.CLK_16K, b.FF02_CLK, b.SER_CLK, !b.SER_CLK);

  /*p06.CAVE*/ wire SER_CLK_MUXn = mux2n(b.SER_CLK, b.SCK_C, b.XFER_DIR);

  /*p06.CARO*/ wire SER_RST  = and(b.FF02_CLK, in.SYS_RESETn);
  /*p06.DAWA*/ next.SER_CLK1 = or(SER_CLK_MUXn, !b.XFER_START); // this must stop the clock or something when the transmit's done
  /*p06.CAFA*/ next.SER_CNT0   = tock_pos(a.SER_CLK1, b.SER_CLK1, SER_RST, b.SER_CNT0, !b.SER_CNT0);
  /*p06.CYLO*/ next.SER_CNT1   = tock_pos(!a.SER_CNT0, !b.SER_CNT0, SER_RST, b.SER_CNT1, !b.SER_CNT1);
  /*p06.CYDE*/ next.SER_CNT2   = tock_pos(!a.SER_CNT1, !b.SER_CNT1, SER_RST, b.SER_CNT2, !b.SER_CNT2);
  /*p06.CALY*/ next.SER_CNT3   = tock_pos(!a.SER_CNT2, !b.SER_CNT2, SER_RST, b.SER_CNT3, !b.SER_CNT3);
  /*p06.COBA*/ next.SER_CNT3n  = not(b.SER_CNT3);
  
  /*p06.URYS*/ wire FF01_WRn = nand(in.CPU_WR, ADDR_FF00_FF03, in.A00, A1n);
  /*p06.DAKU*/ wire FF01_WR  = not (FF01_WRn);

  /*p06.CUFU*/ wire SER_DATA0_SETn = nand(in.D0, FF01_WR);
  /*p06.DOCU*/ wire SER_DATA1_SETn = nand(in.D1, FF01_WR);
  /*p06.DELA*/ wire SER_DATA2_SETn = nand(in.D2, FF01_WR);
  /*p06.DYGE*/ wire SER_DATA3_SETn = nand(in.D3, FF01_WR);
  /*p06.DOLA*/ wire SER_DATA4_SETn = nand(in.D4, FF01_WR);
  /*p06.ELOK*/ wire SER_DATA5_SETn = nand(in.D5, FF01_WR);
  /*p06.EDEL*/ wire SER_DATA6_SETn = nand(in.D6, FF01_WR);
  /*p06.EFEF*/ wire SER_DATA7_SETn = nand(in.D7, FF01_WR);

  /*p06.COHY*/ wire SER_DATA0_RSTn = or(and(FF01_WRn, in.D0), in.SYS_RESETn);
  /*p06.DUMO*/ wire SER_DATA1_RSTn = or(and(FF01_WRn, in.D1), in.SYS_RESETn);
  /*p06.DYBO*/ wire SER_DATA2_RSTn = or(and(FF01_WRn, in.D2), in.SYS_RESETn);
  /*p06.DAJU*/ wire SER_DATA3_RSTn = or(and(FF01_WRn, in.D3), in.SYS_RESETn);
  /*p06.DYLY*/ wire SER_DATA4_RSTn = or(and(FF01_WRn, in.D4), in.SYS_RESETn);
  /*p06.EHUJ*/ wire SER_DATA5_RSTn = or(and(FF01_WRn, in.D5), in.SYS_RESETn);
  /*p06.EFAK*/ wire SER_DATA6_RSTn = or(and(FF01_WRn, in.D6), in.SYS_RESETn);
  /*p06.EGUV*/ wire SER_DATA7_RSTn = or(and(FF01_WRn, in.D7), in.SYS_RESETn);

  /*p06.CAGE*/ next.SIN_Cn = not(b.SIN_C);
  /*p06.EDYL*/ next.SER_CLKn = not(b.SER_CLK1);
  /*p06.EPYT*/ next.SER_CLK2 = not(b.SER_CLKn);
  /*p06.DAWE*/ next.SER_CLK3 = not(/*p06.DEHO*/ not(b.SER_CLK2));
  /*p06.CUBA*/ next.SER_DATA0 = srtock_pos(a.SER_CLK3, b.SER_CLK3, SER_DATA0_SETn, SER_DATA0_RSTn, b.SER_DATA0, b.SIN_Cn);
  /*p06.DEGU*/ next.SER_DATA1 = srtock_pos(a.SER_CLK3, b.SER_CLK3, SER_DATA1_SETn, SER_DATA1_RSTn, b.SER_DATA1, b.SER_DATA0);
  /*p06.DYRA*/ next.SER_DATA2 = srtock_pos(a.SER_CLK3, b.SER_CLK3, SER_DATA2_SETn, SER_DATA2_RSTn, b.SER_DATA2, b.SER_DATA1);
  /*p06.DOJO*/ next.SER_DATA3 = srtock_pos(a.SER_CLK3, b.SER_CLK3, SER_DATA3_SETn, SER_DATA3_RSTn, b.SER_DATA3, b.SER_DATA2);
  /*p06.DOVU*/ next.SER_DATA4 = srtock_pos(a.SER_CLK2, b.SER_CLK2, SER_DATA4_SETn, SER_DATA4_RSTn, b.SER_DATA4, b.SER_DATA3);
  /*p06.EJAB*/ next.SER_DATA5 = srtock_pos(a.SER_CLK2, b.SER_CLK2, SER_DATA5_SETn, SER_DATA5_RSTn, b.SER_DATA5, b.SER_DATA4);
  /*p06.EROD*/ next.SER_DATA6 = srtock_pos(a.SER_CLK2, b.SER_CLK2, SER_DATA6_SETn, SER_DATA6_RSTn, b.SER_DATA6, b.SER_DATA5);
  /*p06.EDER*/ next.SER_DATA7 = srtock_pos(a.SER_CLK2, b.SER_CLK2, SER_DATA7_SETn, SER_DATA7_RSTn, b.SER_DATA7, b.SER_DATA6);
  /*p06.ELYS*/ next.SER_OUT   = tock_pos(a.SER_CLKn, b.SER_CLKn, in.SYS_RESETn, b.SER_OUT, b.SER_DATA7);

  /*p05.KENA*/ next.SOUT  = mux2n(in.DBG_FF00_D6, b.SER_OUT, in.FF60_0);
  /*p06.KEXU*/ next.SCK_A = nand(b.SER_CLK, b.XFER_DIR);
  /*p06.CULY*/ next.SCK_B = b.XFER_DIR;
  /*p06.KUJO*/ next.SCK_D = nor (b.SER_CLK, /*p06.JAGO*/ not(b.XFER_DIR));

  /*p06.UFEG*/ wire FF01_RD = and(in.CPU_RD, ADDR_FF00_FF03, in.A00, A1n);
  /*p06.CUGY*/ if (FF01_RD) out.D0 = b.SER_DATA0;
  /*p06.DUDE*/ if (FF01_RD) out.D1 = b.SER_DATA1;
  /*p06.DETU*/ if (FF01_RD) out.D2 = b.SER_DATA2;
  /*p06.DASO*/ if (FF01_RD) out.D3 = b.SER_DATA3;
  /*p06.DAME*/ if (FF01_RD) out.D4 = b.SER_DATA4;
  /*p06.EVOK*/ if (FF01_RD) out.D5 = b.SER_DATA5;
  /*p06.EFAB*/ if (FF01_RD) out.D6 = b.SER_DATA6;
  /*p06.ETAK*/ if (FF01_RD) out.D7 = b.SER_DATA7;

  /*p06.UCOM*/ wire FF02_RD = and (in.CPU_RD, ADDR_FF00_FF03, in.A01, A0n);
  /*p06.CORE*/ if (FF02_RD) out.D0 = b.XFER_DIR;
  /*p06.ELUV*/ if (FF02_RD) out.D7 = b.XFER_START;
}

//-----------------------------------------------------------------------------

};