#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct SerialInput {
  bool DIV_05;
  bool DBG_FF00_D6;
  bool FF60_0;
};

struct Serial {

  /*p06.ETAF*/ Reg XFER_START;
  /*p06.CULY*/ Reg XFER_DIR;

  /*p06.COTY*/ Reg SER_CLK;

  /*p06.CAFA*/ Reg SER_CNT0;   // counter that triggers INT_SERIAL
  /*p06.CYLO*/ Reg SER_CNT1;
  /*p06.CYDE*/ Reg SER_CNT2;
  /*p06.CALY*/ Reg SER_CNT3;
  /*p06.COBA*/ bool SER_CNT3n;


  /*p06.CUBA*/ Reg SER_DATA0;
  /*p06.DEGU*/ Reg SER_DATA1;
  /*p06.DYRA*/ Reg SER_DATA2;
  /*p06.DOJO*/ Reg SER_DATA3;
  /*p06.DOVU*/ Reg SER_DATA4;
  /*p06.EJAB*/ Reg SER_DATA5;
  /*p06.EROD*/ Reg SER_DATA6;
  /*p06.EDER*/ Reg SER_DATA7;

  /*p06.ELYS*/ bool SER_OUT;

  //----------
  // Serial pins

  /* PIN_68 */ bool SCK_A;   // <- P06.KEXU
  /* PIN_68 */ bool SCK_B;   // <- P06.CULY
  /* PIN_68 */ bool SCK_C;   // -> P06.CAVE
  /* PIN_68 */ bool SCK_D;   // <- P06.KUJO

  /* PIN_69 */ bool SIN_A;   // nc?
  /* PIN_69 */ bool SIN_B;   // nc?
  /* PIN_69 */ bool SIN_C;   // -> P06.CAGE
  /* PIN_69 */ bool SIN_D;   // nc?

  /* PIN_70 */ bool SOUT;    // <- P05.KENA
};

//-----------------------------------------------------------------------------

};