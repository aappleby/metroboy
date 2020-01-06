#include "Sch_Serial.h"

#include "Sch_BusControl.h"
#include "Sch_Decoder.h"
#include "Sch_Resets.h"
#include "Sch_Debug.h"
#include "Sch_Pins.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void SerialRegisters::pwron() {
  pwron_all(XFER_START, XFER_DIR, SER_CLK, SER_CNT0, SER_CNT1, SER_CNT2, SER_CNT3, SER_OUT);
  pwron_all(SER_DATA0, SER_DATA1, SER_DATA2, SER_DATA3, SER_DATA4, SER_DATA5, SER_DATA6, SER_DATA7);
}

void SerialRegisters::reset() {
  // fixme...
  pwron_all(XFER_START, XFER_DIR, SER_CLK, SER_CNT0, SER_CNT1, SER_CNT2, SER_CNT3, SER_OUT);
  pwron_all(SER_DATA0, SER_DATA1, SER_DATA2, SER_DATA3, SER_DATA4, SER_DATA5, SER_DATA6, SER_DATA7);
}

void SerialRegisters::phase_begin() {
}

void SerialRegisters::phase_end() {
}

void SerialRegisters::pass_begin() {
}

bool SerialRegisters::pass_end() {
  bool changed = false;
  changed |= commit_all(XFER_START, XFER_DIR, SER_CLK, SER_CNT0, SER_CNT1, SER_CNT2, SER_CNT3, SER_OUT);
  changed |= commit_all(SER_DATA0, SER_DATA1, SER_DATA2, SER_DATA3, SER_DATA4, SER_DATA5, SER_DATA6, SER_DATA7);
  return changed;
}

//-----------------------------------------------------------------------------

SerialSignals SerialRegisters::tick(const BusSignals& bus_sig,
                                    const RstSignals& rst_sig,
                                    const DecoderSignals& dec_sig,
                                    SerialPins& ser_pins,
                                    BusTristates& bus_tri) {
  // HAX
  wire DIV_05 = false;
  wire DBG_FF00_D6 = false;
  wire FF60_0 = false;

  //----------
  // SerialRegisters port

  /*p06.COBA*/ wire SER_CNT3n = not(SER_CNT3);

  /*p06.UWAM*/ wire FF02_WRn = nand(bus_sig.TAPU_CPUWR, dec_sig.ADDR_FF00_FF03, bus_tri.A01(), dec_sig.TOVY_A00n);
  /*p06.CABY*/ wire XFER_RESET = and(SER_CNT3n, rst_sig.SYS_RESETn);
  /*p06.ETAF*/ XFER_START.set(FF02_WRn, XFER_RESET,         bus_tri.D0());
  /*p06.CULY*/ XFER_DIR  .set(FF02_WRn, rst_sig.SYS_RESETn, bus_tri.D1());

  /*p01.UVYN*/ wire CLK_16K = not(DIV_05);
  /*p06.COTY*/ SER_CLK.set(CLK_16K, FF02_WRn, !SER_CLK);

  /*p06.CAVE*/ wire SER_CLK_MUXn = mux2n(SER_CLK, ser_pins.SCK_C, XFER_DIR);

  /*p06.CARO*/ wire SER_RST  = and(FF02_WRn, rst_sig.SYS_RESETn);
  /*p06.DAWA*/ wire SER_CLK1 = or(SER_CLK_MUXn, !XFER_START); // this must stop the clock or something when the transmit's done
  /*p06.CAFA*/ SER_CNT0.set(SER_CLK1,    SER_RST, !SER_CNT0);
  /*p06.CYLO*/ SER_CNT1.set(!SER_CNT0, SER_RST, !SER_CNT1);
  /*p06.CYDE*/ SER_CNT2.set(!SER_CNT1, SER_RST, !SER_CNT2);
  /*p06.CALY*/ SER_CNT3.set(!SER_CNT2, SER_RST, !SER_CNT3);
  
  /*p06.URYS*/ wire FF01_WRn = nand(bus_sig.TAPU_CPUWR, dec_sig.ADDR_FF00_FF03, bus_tri.A00(), dec_sig.TOLA_A01n);
  /*p06.DAKU*/ wire FF01_WR  = not (FF01_WRn);

  /*p06.CUFU*/ wire SER_DATA0_SETn = nand(bus_tri.D0(), FF01_WR);
  /*p06.DOCU*/ wire SER_DATA1_SETn = nand(bus_tri.D1(), FF01_WR);
  /*p06.DELA*/ wire SER_DATA2_SETn = nand(bus_tri.D2(), FF01_WR);
  /*p06.DYGE*/ wire SER_DATA3_SETn = nand(bus_tri.D3(), FF01_WR);
  /*p06.DOLA*/ wire SER_DATA4_SETn = nand(bus_tri.D4(), FF01_WR);
  /*p06.ELOK*/ wire SER_DATA5_SETn = nand(bus_tri.D5(), FF01_WR);
  /*p06.EDEL*/ wire SER_DATA6_SETn = nand(bus_tri.D6(), FF01_WR);
  /*p06.EFEF*/ wire SER_DATA7_SETn = nand(bus_tri.D7(), FF01_WR);

  /*p06.COHY*/ wire SER_DATA0_RSTn = or(and(FF01_WRn, bus_tri.D0()), rst_sig.SYS_RESETn);
  /*p06.DUMO*/ wire SER_DATA1_RSTn = or(and(FF01_WRn, bus_tri.D1()), rst_sig.SYS_RESETn);
  /*p06.DYBO*/ wire SER_DATA2_RSTn = or(and(FF01_WRn, bus_tri.D2()), rst_sig.SYS_RESETn);
  /*p06.DAJU*/ wire SER_DATA3_RSTn = or(and(FF01_WRn, bus_tri.D3()), rst_sig.SYS_RESETn);
  /*p06.DYLY*/ wire SER_DATA4_RSTn = or(and(FF01_WRn, bus_tri.D4()), rst_sig.SYS_RESETn);
  /*p06.EHUJ*/ wire SER_DATA5_RSTn = or(and(FF01_WRn, bus_tri.D5()), rst_sig.SYS_RESETn);
  /*p06.EFAK*/ wire SER_DATA6_RSTn = or(and(FF01_WRn, bus_tri.D6()), rst_sig.SYS_RESETn);
  /*p06.EGUV*/ wire SER_DATA7_RSTn = or(and(FF01_WRn, bus_tri.D7()), rst_sig.SYS_RESETn);

  /*p06.CAGE*/ wire SIN_Cn = not(ser_pins.SIN_C);
  /*p06.EDYL*/ wire SER_CLKn = not(SER_CLK1);
  /*p06.EPYT*/ wire SER_CLK2 = not(SER_CLKn);
  /*p06.DEHO*/ wire DEHO = not(SER_CLK2);
  /*p06.DAWE*/ wire SER_CLK3 = not(DEHO);
  /*p06.CUBA*/ SER_DATA0.set(SER_CLK3, SER_DATA0_SETn, SER_DATA0_RSTn, SIN_Cn);
  /*p06.DEGU*/ SER_DATA1.set(SER_CLK3, SER_DATA1_SETn, SER_DATA1_RSTn, SER_DATA0);
  /*p06.DYRA*/ SER_DATA2.set(SER_CLK3, SER_DATA2_SETn, SER_DATA2_RSTn, SER_DATA1);
  /*p06.DOJO*/ SER_DATA3.set(SER_CLK3, SER_DATA3_SETn, SER_DATA3_RSTn, SER_DATA2);
  /*p06.DOVU*/ SER_DATA4.set(SER_CLK2, SER_DATA4_SETn, SER_DATA4_RSTn, SER_DATA3);
  /*p06.EJAB*/ SER_DATA5.set(SER_CLK2, SER_DATA5_SETn, SER_DATA5_RSTn, SER_DATA4);
  /*p06.EROD*/ SER_DATA6.set(SER_CLK2, SER_DATA6_SETn, SER_DATA6_RSTn, SER_DATA5);
  /*p06.EDER*/ SER_DATA7.set(SER_CLK2, SER_DATA7_SETn, SER_DATA7_RSTn, SER_DATA6);
  /*p06.ELYS*/ SER_OUT.set(SER_CLKn, rst_sig.SYS_RESETn, SER_DATA7);

  /*p05.KENA*/ ser_pins.SOUT  = mux2n(DBG_FF00_D6, SER_OUT, FF60_0);
  /*p06.KEXU*/ ser_pins.SCK_A = nand(SER_CLK, XFER_DIR);
  /*p06.CULY*/ ser_pins.SCK_B = XFER_DIR;
  /*p06.KUJO*/ ser_pins.SCK_D = nor (SER_CLK, /*p06.JAGO*/ not(XFER_DIR));

  /*p06.UFEG*/ wire FF01_RD = and(bus_sig.TEDO_CPURD, dec_sig.ADDR_FF00_FF03, bus_tri.A00(), dec_sig.TOLA_A01n);

  if (FF01_RD) bus_tri.set_data(
    /*p06.CUGY*/ SER_DATA0,
    /*p06.DUDE*/ SER_DATA1,
    /*p06.DETU*/ SER_DATA2,
    /*p06.DASO*/ SER_DATA3,
    /*p06.DAME*/ SER_DATA4,
    /*p06.EVOK*/ SER_DATA5,
    /*p06.EFAB*/ SER_DATA6,
    /*p06.ETAK*/ SER_DATA7
  );

  /*p06.UCOM*/ wire FF02_RD = and (bus_sig.TEDO_CPURD, dec_sig.ADDR_FF00_FF03, bus_tri.A01(), dec_sig.TOVY_A00n);

  if (FF02_RD) bus_tri.set_data(
    /*p06.CORE*/ XFER_DIR,
    /*p06.ELUV*/ XFER_START
  );

  return {
    .SER_CNT3 = SER_CNT3,
  };
}

//-----------------------------------------------------------------------------

};