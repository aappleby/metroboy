#pragma once
#include "Schematics.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Serial {

  /*p06.UWAM*/ bool FF02_CLK;
  /*p06.ETAF*/ bool XFER_START;
  /*p06.CULY*/ bool XFER_DIR;

  /*p01.UVYN*/ bool CLK_16K;
  /*p06.COTY*/ bool SER_CLK;

  /*p06.DAWA*/ bool SER_CLK1;
  /*p06.CAFA*/ bool SER_CNT0;   // counter that triggers INT_SERIAL
  /*p06.CYLO*/ bool SER_CNT1;
  /*p06.CYDE*/ bool SER_CNT2;
  /*p06.CALY*/ bool SER_CNT3;
  /*p06.COBA*/ bool SER_CNT3n;


  /*p06.CAGE*/ bool SIN_Cn;
  /*p06.EDYL*/ bool SER_CLKn;
  /*p06.EPYT*/ bool SER_CLK2;
  /*p06.DAWE*/ bool SER_CLK3;
  /*p06.CUBA*/ bool SER_DATA0;
  /*p06.DEGU*/ bool SER_DATA1;
  /*p06.DYRA*/ bool SER_DATA2;
  /*p06.DOJO*/ bool SER_DATA3;
  /*p06.DOVU*/ bool SER_DATA4;
  /*p06.EJAB*/ bool SER_DATA5;
  /*p06.EROD*/ bool SER_DATA6;
  /*p06.EDER*/ bool SER_DATA7;
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

struct SerialInput {

  bool CPU_RD;
  bool CPU_WR;
  bool SYS_RESETn;
  bool DIV_05;
  
  bool DBG_FF00_D6;
  bool FF60_0;

  union {
    uint16_t A;
    struct {
      bool A00 : 1;
      bool A01 : 1;
      bool A02 : 1;
      bool A03 : 1;
      bool A04 : 1;
      bool A05 : 1;
      bool A06 : 1;
      bool A07 : 1;
      bool A08 : 1;
      bool A09 : 1;
      bool A10 : 1;
      bool A11 : 1;
      bool A12 : 1;
      bool A13 : 1;
      bool A14 : 1;
      bool A15 : 1;
    };
  };

  union { 
    uint8_t D;
    struct {
      bool D0 : 1;
      bool D1 : 1;
      bool D2 : 1;
      bool D3 : 1;
      bool D4 : 1;
      bool D5 : 1;
      bool D6 : 1;
      bool D7 : 1;
    };
  };
};

//-----------------------------------------------------------------------------

struct SerialOutput {
  union { 
    uint8_t D;
    struct {
      bool D0 : 1;
      bool D1 : 1;
      bool D2 : 1;
      bool D3 : 1;
      bool D4 : 1;
      bool D5 : 1;
      bool D6 : 1;
      bool D7 : 1;
    };
  };
};

//-----------------------------------------------------------------------------

};