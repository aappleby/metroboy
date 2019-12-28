#include "Sch_Serial.h"

#include "Sch_BusControl.h"
#include "Sch_Decoder.h"
#include "Sch_Resets.h"
#include "Sch_Debug.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Serial_tick(const Bus& bus,
                 const BusControl& ctl,
                 const SerialInput& in,
                 const ResetRegisters& rst_reg,
                 const Decoder& dec,
                 const Serial& prev,
                 Serial& next,
                 Bus& bus_out) {

  //----------
  // Serial port

  /*p06.UWAM*/ wire FF02_WRn = nand(ctl.CPU_WR, dec.ADDR_FF00_FF03, bus.A01, dec.TOVY_A00n);
  /*p06.CABY*/ wire XFER_RESET = and(prev.SER_CNT3n, rst_reg.sig.SYS_RESETn);
  /*p06.ETAF*/ next.XFER_START.tock(FF02_WRn, XFER_RESET,    bus.D7);
  /*p06.CULY*/ next.XFER_DIR  .tock(FF02_WRn, rst_reg.sig.SYS_RESETn, bus.D0);

  /*p01.UVYN*/ wire CLK_16K = not(in.DIV_05);
  /*p06.COTY*/ next.SER_CLK.tock(CLK_16K, FF02_WRn, !prev.SER_CLK);

  /*p06.CAVE*/ wire SER_CLK_MUXn = mux2n(prev.SER_CLK, prev.SCK_C, prev.XFER_DIR);

  /*p06.CARO*/ wire SER_RST  = and(FF02_WRn, rst_reg.sig.SYS_RESETn);
  /*p06.DAWA*/ wire SER_CLK1 = or(SER_CLK_MUXn, !prev.XFER_START); // this must stop the clock or something when the transmit's done
  /*p06.CAFA*/ next.SER_CNT0.tock(SER_CLK1,    SER_RST, !prev.SER_CNT0);
  /*p06.CYLO*/ next.SER_CNT1.tock(!prev.SER_CNT0, SER_RST, !prev.SER_CNT1);
  /*p06.CYDE*/ next.SER_CNT2.tock(!prev.SER_CNT1, SER_RST, !prev.SER_CNT2);
  /*p06.CALY*/ next.SER_CNT3.tock(!prev.SER_CNT2, SER_RST, !prev.SER_CNT3);
  /*p06.COBA*/ next.SER_CNT3n  = not(prev.SER_CNT3);
  
  /*p06.URYS*/ wire FF01_WRn = nand(ctl.CPU_WR, dec.ADDR_FF00_FF03, bus.A00, dec.TOLA_A01n);
  /*p06.DAKU*/ wire FF01_WR  = not (FF01_WRn);

  /*p06.CUFU*/ wire SER_DATA0_SETn = nand(bus.D0, FF01_WR);
  /*p06.DOCU*/ wire SER_DATA1_SETn = nand(bus.D1, FF01_WR);
  /*p06.DELA*/ wire SER_DATA2_SETn = nand(bus.D2, FF01_WR);
  /*p06.DYGE*/ wire SER_DATA3_SETn = nand(bus.D3, FF01_WR);
  /*p06.DOLA*/ wire SER_DATA4_SETn = nand(bus.D4, FF01_WR);
  /*p06.ELOK*/ wire SER_DATA5_SETn = nand(bus.D5, FF01_WR);
  /*p06.EDEL*/ wire SER_DATA6_SETn = nand(bus.D6, FF01_WR);
  /*p06.EFEF*/ wire SER_DATA7_SETn = nand(bus.D7, FF01_WR);

  /*p06.COHY*/ wire SER_DATA0_RSTn = or(and(FF01_WRn, bus.D0), rst_reg.sig.SYS_RESETn);
  /*p06.DUMO*/ wire SER_DATA1_RSTn = or(and(FF01_WRn, bus.D1), rst_reg.sig.SYS_RESETn);
  /*p06.DYBO*/ wire SER_DATA2_RSTn = or(and(FF01_WRn, bus.D2), rst_reg.sig.SYS_RESETn);
  /*p06.DAJU*/ wire SER_DATA3_RSTn = or(and(FF01_WRn, bus.D3), rst_reg.sig.SYS_RESETn);
  /*p06.DYLY*/ wire SER_DATA4_RSTn = or(and(FF01_WRn, bus.D4), rst_reg.sig.SYS_RESETn);
  /*p06.EHUJ*/ wire SER_DATA5_RSTn = or(and(FF01_WRn, bus.D5), rst_reg.sig.SYS_RESETn);
  /*p06.EFAK*/ wire SER_DATA6_RSTn = or(and(FF01_WRn, bus.D6), rst_reg.sig.SYS_RESETn);
  /*p06.EGUV*/ wire SER_DATA7_RSTn = or(and(FF01_WRn, bus.D7), rst_reg.sig.SYS_RESETn);

  /*p06.CAGE*/ wire SIN_Cn = not(prev.SIN_C);
  /*p06.EDYL*/ wire SER_CLKn = not(SER_CLK1);
  /*p06.EPYT*/ wire SER_CLK2 = not(SER_CLKn);
  /*p06.DAWE*/ wire SER_CLK3 = not(/*p06.DEHO*/ not(SER_CLK2));
  /*p06.CUBA*/ next.SER_DATA0.srtock(SER_CLK3, SER_DATA0_SETn, SER_DATA0_RSTn, SIN_Cn);
  /*p06.DEGU*/ next.SER_DATA1.srtock(SER_CLK3, SER_DATA1_SETn, SER_DATA1_RSTn, prev.SER_DATA0);
  /*p06.DYRA*/ next.SER_DATA2.srtock(SER_CLK3, SER_DATA2_SETn, SER_DATA2_RSTn, prev.SER_DATA1);
  /*p06.DOJO*/ next.SER_DATA3.srtock(SER_CLK3, SER_DATA3_SETn, SER_DATA3_RSTn, prev.SER_DATA2);
  /*p06.DOVU*/ next.SER_DATA4.srtock(SER_CLK2, SER_DATA4_SETn, SER_DATA4_RSTn, prev.SER_DATA3);
  /*p06.EJAB*/ next.SER_DATA5.srtock(SER_CLK2, SER_DATA5_SETn, SER_DATA5_RSTn, prev.SER_DATA4);
  /*p06.EROD*/ next.SER_DATA6.srtock(SER_CLK2, SER_DATA6_SETn, SER_DATA6_RSTn, prev.SER_DATA5);
  /*p06.EDER*/ next.SER_DATA7.srtock(SER_CLK2, SER_DATA7_SETn, SER_DATA7_RSTn, prev.SER_DATA6);
  /*p06.ELYS*/ next.SER_OUT   = tock_pos(SER_CLKn, SER_CLKn, rst_reg.sig.SYS_RESETn, prev.SER_OUT, prev.SER_DATA7);

  /*p05.KENA*/ next.SOUT  = mux2n(in.DBG_FF00_D6, prev.SER_OUT, in.FF60_0);
  /*p06.KEXU*/ next.SCK_A = nand(prev.SER_CLK, prev.XFER_DIR);
  /*p06.CULY*/ next.SCK_B = prev.XFER_DIR;
  /*p06.KUJO*/ next.SCK_D = nor (prev.SER_CLK, /*p06.JAGO*/ not(prev.XFER_DIR));

  /*p06.UFEG*/ wire FF01_RD = and(ctl.CPU_RD, dec.ADDR_FF00_FF03, bus.A00, dec.TOLA_A01n);
  /*p06.CUGY*/ if (FF01_RD) bus_out.D0 = prev.SER_DATA0;
  /*p06.DUDE*/ if (FF01_RD) bus_out.D1 = prev.SER_DATA1;
  /*p06.DETU*/ if (FF01_RD) bus_out.D2 = prev.SER_DATA2;
  /*p06.DASO*/ if (FF01_RD) bus_out.D3 = prev.SER_DATA3;
  /*p06.DAME*/ if (FF01_RD) bus_out.D4 = prev.SER_DATA4;
  /*p06.EVOK*/ if (FF01_RD) bus_out.D5 = prev.SER_DATA5;
  /*p06.EFAB*/ if (FF01_RD) bus_out.D6 = prev.SER_DATA6;
  /*p06.ETAK*/ if (FF01_RD) bus_out.D7 = prev.SER_DATA7;

  /*p06.UCOM*/ wire FF02_RD = and (ctl.CPU_RD, dec.ADDR_FF00_FF03, bus.A01, dec.TOVY_A00n);
  /*p06.CORE*/ if (FF02_RD) bus_out.D0 = prev.XFER_DIR;
  /*p06.ELUV*/ if (FF02_RD) bus_out.D7 = prev.XFER_START;
}

//-----------------------------------------------------------------------------

};