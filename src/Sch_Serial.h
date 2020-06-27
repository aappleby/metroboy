#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct SerialRegisters {

  void tick(TestGB& gb);
  bool commit();

  uint8_t ser_cnt() { return (uint8_t)pack(SER_CNT0.q(), SER_CNT1.q(), SER_CNT2.q(), CALY_INT_SERIAL.q()); }
  uint8_t ser_data() { return (uint8_t)pack(SER_DATA0.q(), SER_DATA1.q(), SER_DATA2.q(), SER_DATA3.q(), SER_DATA4.q(), SER_DATA5.q(), SER_DATA6.q(), SER_DATA7.q()); }

  void dump_regs(TextPainter& text_painter);
  void dump_pins(TextPainter& text_painter);

  /*p06.ETAF*/ Reg XFER_START;
  /*p06.CULY*/ Reg XFER_DIR;

  /*p06.COTY*/ Reg SER_CLK;

  /*p06.CAFA*/ Reg SER_CNT0;
  /*p06.CYLO*/ Reg SER_CNT1;
  /*p06.CYDE*/ Reg SER_CNT2;
  /*p06.CALY*/ Reg CALY_INT_SERIAL;

  /*p06.CUBA*/ Reg SER_DATA0;
  /*p06.DEGU*/ Reg SER_DATA1;
  /*p06.DYRA*/ Reg SER_DATA2;
  /*p06.DOJO*/ Reg SER_DATA3;
  /*p06.DOVU*/ Reg SER_DATA4;
  /*p06.EJAB*/ Reg SER_DATA5;
  /*p06.EROD*/ Reg SER_DATA6;
  /*p06.EDER*/ Reg SER_DATA7;

  /*p06.ELYS*/ Reg SER_OUT;

  //----------
  // Serial pins

  /* PIN_68 */ PinOut SCK_A;   // <- P06.KEXU
  /* PIN_68 */ PinOut SCK_B;   // <- P06.CULY
  /* PIN_68 */ PinIn  SCK_C;   // -> P06.CAVE
  /* PIN_68 */ PinOut SCK_D;   // <- P06.KUJO

  /* PIN_69 */ PinOut SIN_A;   // nc?
  /* PIN_69 */ PinOut SIN_B;   // nc?
  /* PIN_69 */ PinIn  SIN_C;   // -> P06.CAGE
  /* PIN_69 */ PinOut SIN_D;   // nc?

  /* PIN_70 */ PinOut SOUT;    // <- P05.KENA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics