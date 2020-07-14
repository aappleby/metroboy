#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct SerialRegisters {
  SerialRegisters();
  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  SignalHash commit();

  /*p06.CALY*/ wire CALY_INT_SERIALp() const { return _CALY_INT_SERIALp.q(); }

private:

  /*p06.ETAF*/ Reg17 _XFER_START;
  /*p06.CULY*/ Reg17 _XFER_DIR;

  /*p06.COTY*/ Reg17 _SER_CLK;

  /*p06.CAFA*/ Reg17 _SER_CNT0;
  /*p06.CYLO*/ Reg17 _SER_CNT1;
  /*p06.CYDE*/ Reg17 _SER_CNT2;
  /*p06.CALY*/ Reg17 _CALY_INT_SERIALp;

  /*p06.CUBA*/ Reg22 _SER_DATA0;
  /*p06.DEGU*/ Reg22 _SER_DATA1;
  /*p06.DYRA*/ Reg22 _SER_DATA2;
  /*p06.DOJO*/ Reg22 _SER_DATA3;
  /*p06.DOVU*/ Reg22 _SER_DATA4;
  /*p06.EJAB*/ Reg22 _SER_DATA5;
  /*p06.EROD*/ Reg22 _SER_DATA6;
  /*p06.EDER*/ Reg22 _SER_DATA7;

  /*p06.ELYS*/ Reg17 _SER_OUT;

  //----------
  // Serial pins

  /* PIN_68 */ ExtPinOut SCK_A;   // <- P06.KEXU
  /* PIN_68 */ ExtPinOut SCK_B;   // <- P06.CULY
  /* PIN_68 */ ExtPinIn  SCK_C;   // -> P06.CAVE
  /* PIN_68 */ ExtPinOut SCK_D;   // <- P06.KUJO

  /* PIN_69 */ ExtPinOut SIN_A;   // nc?
  /* PIN_69 */ ExtPinOut SIN_B;   // nc?
  /* PIN_69 */ ExtPinIn  SIN_C;   // -> P06.CAGE
  /* PIN_69 */ ExtPinOut SIN_D;   // nc?

  /* PIN_70 */ ExtPinOut SOUT;    // <- P05.KENA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics