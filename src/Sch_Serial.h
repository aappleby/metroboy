#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct SerialRegisters {
  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);

  int get_data() {
    return pack(_SER_DATA7.q(), _SER_DATA6.q(), _SER_DATA5.q(), _SER_DATA4.q(),
                _SER_DATA3.q(), _SER_DATA2.q(), _SER_DATA1.q(), _SER_DATA0.q());
  }

  /*p06.CALY*/ wire CALY_SER_INTp() const { return _CALY_SER_INTp.q(); }

  void dump(Dumper& d) {
    d("----------  Serial  ----------\n");
    d("XFER_START  %c\n", _XFER_START.c());
    d("XFER_DIR    %c\n", _XFER_DIR  .c());
    d("SER_CLK     %c\n", _SER_CLK   .c());
    d("SER_CNT     %d\n", pack(0, _SER_CNT2.q(), _SER_CNT1.q(), _SER_CNT0.q()));
    d("SER_DATA    0x%02x\n", get_data());
    d("SER_OUT     %c\n", _SER_OUT.c());
    d("SCK_A       %c\n", SCK_A.c());
    d("SCK_B       %c\n", SCK_B.c());
    d("SCK_C       %c\n", SCK_C.c());
    d("SCK_D       %c\n", SCK_D.c());
    d("SIN_C       %c\n", SIN_C.c());
    d("SOUT        %c\n", SOUT .c());
    d("_CALY_SER_INTp %c\n", _CALY_SER_INTp.c());
    d("\n");
  }

//private:

  /*p06.ETAF*/ Reg _XFER_START    = REG_D0C0;
  /*p06.CULY*/ Reg _XFER_DIR      = REG_D0C0;
                                   
  /*p06.COTY*/ Reg _SER_CLK       = REG_D0C0;
                                   
  /*p06.CAFA*/ Reg _SER_CNT0      = REG_D0C0;
  /*p06.CYLO*/ Reg _SER_CNT1      = REG_D0C0;
  /*p06.CYDE*/ Reg _SER_CNT2      = REG_D0C0;
                                   
  /*p06.CUBA*/ Reg _SER_DATA0     = REG_D0C0;
  /*p06.DEGU*/ Reg _SER_DATA1     = REG_D0C0;
  /*p06.DYRA*/ Reg _SER_DATA2     = REG_D0C0;
  /*p06.DOJO*/ Reg _SER_DATA3     = REG_D0C0;
  /*p06.DOVU*/ Reg _SER_DATA4     = REG_D0C0;
  /*p06.EJAB*/ Reg _SER_DATA5     = REG_D0C0;
  /*p06.EROD*/ Reg _SER_DATA6     = REG_D0C0;
  /*p06.EDER*/ Reg _SER_DATA7     = REG_D0C0;
                                   
  /*p06.ELYS*/ Reg _SER_OUT       = REG_D0C0;

  /*p06.CALY*/ Reg _CALY_SER_INTp = REG_D0C0;
  //----------
  // Serial pins

  /* PIN_68 */ Tri SCK_A = TRI_HZNP;   // <- P06.KEXU
  /* PIN_68 */ Tri SCK_B = TRI_HZNP;   // <- P06.CULY
  /* PIN_68 */ Tri SCK_C = TRI_D0NP;   // -> P06.CAVE
  /* PIN_68 */ Tri SCK_D = TRI_HZNP;   // <- P06.KUJO

  ///* PIN_69 */ Tri SIN_A = TRI_HZNP;   // nc?
  ///* PIN_69 */ Tri SIN_B = TRI_HZNP;   // nc?
  /* PIN_69 */ Tri SIN_C = TRI_D0NP;   // -> P06.CAGE
  ///* PIN_69 */ Tri SIN_D = TRI_HZNP;   // nc?

  /* PIN_70 */ Tri SOUT  = TRI_HZNP;   // <- P05.KENA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics