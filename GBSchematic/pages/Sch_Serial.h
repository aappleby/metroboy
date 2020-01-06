#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct SerialSignals {
  bool SER_CNT3;
};

struct SerialRegisters {

  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  SerialSignals tick(const BusSignals& bus_sig,
                     const RstSignals& rst_sig,
                     const DecoderSignals& dec_sig,
                     SerialPins& ser_pins,
                     BusTristates& bus_tri);

  /*p06.ETAF*/ Reg2 XFER_START;
  /*p06.CULY*/ Reg2 XFER_DIR;

  /*p06.COTY*/ Reg2 SER_CLK;

  /*p06.CAFA*/ Reg2 SER_CNT0;
  /*p06.CYLO*/ Reg2 SER_CNT1;
  /*p06.CYDE*/ Reg2 SER_CNT2;
  /*p06.CALY*/ Reg2 SER_CNT3;

  /*p06.CUBA*/ Reg2 SER_DATA0;
  /*p06.DEGU*/ Reg2 SER_DATA1;
  /*p06.DYRA*/ Reg2 SER_DATA2;
  /*p06.DOJO*/ Reg2 SER_DATA3;
  /*p06.DOVU*/ Reg2 SER_DATA4;
  /*p06.EJAB*/ Reg2 SER_DATA5;
  /*p06.EROD*/ Reg2 SER_DATA6;
  /*p06.EDER*/ Reg2 SER_DATA7;

  /*p06.ELYS*/ Reg2 SER_OUT;
};

//-----------------------------------------------------------------------------

};