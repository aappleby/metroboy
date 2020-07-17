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

  /*p06.ETAF*/ Reg _XFER_START;
  /*p06.CULY*/ Reg _XFER_DIR;

  /*p06.COTY*/ Reg _SER_CLK;

  /*p06.CAFA*/ Reg _SER_CNT0;
  /*p06.CYLO*/ Reg _SER_CNT1;
  /*p06.CYDE*/ Reg _SER_CNT2;
  /*p06.CALY*/ Reg _CALY_INT_SERIALp;

  /*p06.CUBA*/ Reg _SER_DATA0;
  /*p06.DEGU*/ Reg _SER_DATA1;
  /*p06.DYRA*/ Reg _SER_DATA2;
  /*p06.DOJO*/ Reg _SER_DATA3;
  /*p06.DOVU*/ Reg _SER_DATA4;
  /*p06.EJAB*/ Reg _SER_DATA5;
  /*p06.EROD*/ Reg _SER_DATA6;
  /*p06.EDER*/ Reg _SER_DATA7;

  /*p06.ELYS*/ Reg _SER_OUT;

  //----------
  // Serial pins

  /* PIN_68 */ Reg SCK_A;   // <- P06.KEXU
  /* PIN_68 */ Reg SCK_B;   // <- P06.CULY
  /* PIN_68 */ ExtPinIn  SCK_C;   // -> P06.CAVE
  /* PIN_68 */ Reg SCK_D;   // <- P06.KUJO

  /* PIN_69 */ Reg SIN_A;   // nc?
  /* PIN_69 */ Reg SIN_B;   // nc?
  /* PIN_69 */ ExtPinIn  SIN_C;   // -> P06.CAGE
  /* PIN_69 */ Reg SIN_D;   // nc?

  /* PIN_70 */ Reg SOUT;    // <- P05.KENA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics