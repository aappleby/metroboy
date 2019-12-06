#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void Serial_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  const System& pa = a.sys;
  const System& pb = b.sys;
  System& pc = c.sys;

  /*p03.TOVY*/ wire A0n = not(b.A00);
  /*p08.TOLA*/ wire A1n = not(b.A01);
  /*p06.SEFY*/ wire A2n = not(b.A02);

  /*p06.SARE*/ wire ADDR_XX00_XX07 = nor(b.A07, b.A06, b.A05, b.A04, b.A03);
  /*p06.SANO*/ c.sys.ADDR_FF00_FF03 = and(ADDR_XX00_XX07, A2n, b.sys.ADDR_FFXX);

  //----------
  // Serial port

  /*p06.UWAM*/ pc.FF02_CLK = nand(pb.CPU_WR, pb.ADDR_FF00_FF03, b.A01, A0n);
  /*p06.CABY*/ wire XFER_RESET = and(pb.SER_CNT3n, pb.SYS_RESETn1);
  /*p06.ETAF*/ pc.XFER_START = tock_pos(pa.FF02_CLK, pb.FF02_CLK, XFER_RESET,     pb.XFER_START, b.D7);
  /*p06.CULY*/ pc.XFER_DIR   = tock_pos(pa.FF02_CLK, pb.FF02_CLK, pb.SYS_RESETn1, pb.XFER_DIR,   b.D0);

  /*p06.COTY*/ pc.SER_CLK = tock_pos(pa.CLK_16K, pb.CLK_16K, pb.FF02_CLK, pb.SER_CLK, !pb.SER_CLK);

  /*p06.CAVE*/ wire SER_CLK_MUXn = mux2n(pb.SER_CLK, chip_in.SCK_C, pb.XFER_DIR);
  /*p06.DAWA*/ pc.SER_CLK1 = or(SER_CLK_MUXn, !pb.XFER_START); // this must stop the clock or something when the transmit's done
  /*p06.EDYL*/ pc.SER_CLKn = not(pb.SER_CLK1);
  /*p06.EPYT*/ pc.SER_CLK2 = not(pb.SER_CLKn);
  /*p06.DAWE*/ pc.SER_CLK3 = not(/*p06.DEHO*/ not(pb.SER_CLK2));

  /*p06.CARO*/ wire SER_RST  = and(pb.FF02_CLK, pb.SYS_RESETn1);
  /*p06.CAFA*/ pc.SER_CNT0   = tock_pos(pa.SER_CLK1, pb.SER_CLK1, SER_RST, pb.SER_CNT0, !pb.SER_CNT0);
  /*p06.CYLO*/ pc.SER_CNT1   = tock_pos(!pa.SER_CNT0, !pb.SER_CNT0, SER_RST, pb.SER_CNT1, !pb.SER_CNT1);
  /*p06.CYDE*/ pc.SER_CNT2   = tock_pos(!pa.SER_CNT1, !pb.SER_CNT1, SER_RST, pb.SER_CNT2, !pb.SER_CNT2);
  /*p06.CALY*/ pc.SER_CNT3   = tock_pos(!pa.SER_CNT2, !pb.SER_CNT2, SER_RST, pb.SER_CNT3, !pb.SER_CNT3);
  /*p06.COBA*/ pc.SER_CNT3n  = not(pb.SER_CNT3);
  
  /*p06.URYS*/ wire FF01_WRn = nand(pb.CPU_WR, pb.ADDR_FF00_FF03, b.A00, A1n);
  /*p06.DAKU*/ wire FF01_WR  = not (FF01_WRn);

  /*p06.CUFU*/ wire SER_DATA0_SETn = nand(b.D0, FF01_WR);
  /*p06.DOCU*/ wire SER_DATA1_SETn = nand(b.D1, FF01_WR);
  /*p06.DELA*/ wire SER_DATA2_SETn = nand(b.D2, FF01_WR);
  /*p06.DYGE*/ wire SER_DATA3_SETn = nand(b.D3, FF01_WR);
  /*p06.DOLA*/ wire SER_DATA4_SETn = nand(b.D4, FF01_WR);
  /*p06.ELOK*/ wire SER_DATA5_SETn = nand(b.D5, FF01_WR);
  /*p06.EDEL*/ wire SER_DATA6_SETn = nand(b.D6, FF01_WR);
  /*p06.EFEF*/ wire SER_DATA7_SETn = nand(b.D7, FF01_WR);

  /*p06.COHY*/ wire SER_DATA0_RSTn = or(and(FF01_WRn, b.D0), pb.SYS_RESETn1);
  /*p06.DUMO*/ wire SER_DATA1_RSTn = or(and(FF01_WRn, b.D1), pb.SYS_RESETn1);
  /*p06.DYBO*/ wire SER_DATA2_RSTn = or(and(FF01_WRn, b.D2), pb.SYS_RESETn1);
  /*p06.DAJU*/ wire SER_DATA3_RSTn = or(and(FF01_WRn, b.D3), pb.SYS_RESETn1);
  /*p06.DYLY*/ wire SER_DATA4_RSTn = or(and(FF01_WRn, b.D4), pb.SYS_RESETn1);
  /*p06.EHUJ*/ wire SER_DATA5_RSTn = or(and(FF01_WRn, b.D5), pb.SYS_RESETn1);
  /*p06.EFAK*/ wire SER_DATA6_RSTn = or(and(FF01_WRn, b.D6), pb.SYS_RESETn1);
  /*p06.EGUV*/ wire SER_DATA7_RSTn = or(and(FF01_WRn, b.D7), pb.SYS_RESETn1);

  /*p06.CAGE*/ pc.SIN_Cn = not(chip_in.SIN_C);
  /*p06.CUBA*/ pc.SER_DATA0 = srtock_pos(pa.SER_CLK3, pb.SER_CLK3, SER_DATA0_SETn, SER_DATA0_RSTn, pb.SER_DATA0, pb.SIN_Cn);
  /*p06.DEGU*/ pc.SER_DATA1 = srtock_pos(pa.SER_CLK3, pb.SER_CLK3, SER_DATA1_SETn, SER_DATA1_RSTn, pb.SER_DATA1, pb.SER_DATA0);
  /*p06.DYRA*/ pc.SER_DATA2 = srtock_pos(pa.SER_CLK3, pb.SER_CLK3, SER_DATA2_SETn, SER_DATA2_RSTn, pb.SER_DATA2, pb.SER_DATA1);
  /*p06.DOJO*/ pc.SER_DATA3 = srtock_pos(pa.SER_CLK3, pb.SER_CLK3, SER_DATA3_SETn, SER_DATA3_RSTn, pb.SER_DATA3, pb.SER_DATA2);
  /*p06.DOVU*/ pc.SER_DATA4 = srtock_pos(pa.SER_CLK2, pb.SER_CLK2, SER_DATA4_SETn, SER_DATA4_RSTn, pb.SER_DATA4, pb.SER_DATA3);
  /*p06.EJAB*/ pc.SER_DATA5 = srtock_pos(pa.SER_CLK2, pb.SER_CLK2, SER_DATA5_SETn, SER_DATA5_RSTn, pb.SER_DATA5, pb.SER_DATA4);
  /*p06.EROD*/ pc.SER_DATA6 = srtock_pos(pa.SER_CLK2, pb.SER_CLK2, SER_DATA6_SETn, SER_DATA6_RSTn, pb.SER_DATA6, pb.SER_DATA5);
  /*p06.EDER*/ pc.SER_DATA7 = srtock_pos(pa.SER_CLK2, pb.SER_CLK2, SER_DATA7_SETn, SER_DATA7_RSTn, pb.SER_DATA7, pb.SER_DATA6);
  /*p06.ELYS*/ pc.SER_OUT   = tock_pos(pa.SER_CLKn, pb.SER_CLKn, pb.SYS_RESETn1, pb.SER_OUT, pb.SER_DATA7);

  /*p06.UFEG*/ wire FF01_RD = and(pb.CPU_RD, pb.ADDR_FF00_FF03, b.A00, A1n);
  /*p06.CUGY*/ if (FF01_RD) c.D0 = pb.SER_DATA0;
  /*p06.DUDE*/ if (FF01_RD) c.D1 = pb.SER_DATA1;
  /*p06.DETU*/ if (FF01_RD) c.D2 = pb.SER_DATA2;
  /*p06.DASO*/ if (FF01_RD) c.D3 = pb.SER_DATA3;
  /*p06.DAME*/ if (FF01_RD) c.D4 = pb.SER_DATA4;
  /*p06.EVOK*/ if (FF01_RD) c.D5 = pb.SER_DATA5;
  /*p06.EFAB*/ if (FF01_RD) c.D6 = pb.SER_DATA6;
  /*p06.ETAK*/ if (FF01_RD) c.D7 = pb.SER_DATA7;

  /*p06.UCOM*/ wire FF02_RD = and (pb.CPU_RD, pb.ADDR_FF00_FF03, b.A01, A0n);
  /*p06.CORE*/ if (FF02_RD) c.D0 = pb.XFER_DIR;
  /*p06.ELUV*/ if (FF02_RD) c.D7 = pb.XFER_START;

  /*p05.KENA*/ c.chip_out.SOUT  = mux2n(pb.DBG_FF00_D6, pb.SER_OUT, pb.FF60_0);
  /*p06.KEXU*/ c.chip_out.SCK_A = nand(pb.SER_CLK, pb.XFER_DIR);
  /*p06.CULY*/ c.chip_out.SCK_B = pb.XFER_DIR;
  /*p06.KUJO*/ c.chip_out.SCK_D = nor (pb.SER_CLK, /*p06.JAGO*/ not(pb.XFER_DIR));
}

//-----------------------------------------------------------------------------

};