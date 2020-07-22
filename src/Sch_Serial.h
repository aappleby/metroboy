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

private:

  /*p06.ETAF*/ Reg2 _XFER_START    = Reg2::D0C0;
  /*p06.CULY*/ Reg2 _XFER_DIR      = Reg2::D0C0;
                                   
  /*p06.COTY*/ Reg2 _SER_CLK       = Reg2::D0C0;
                                   
  /*p06.CAFA*/ Reg2 _SER_CNT0      = Reg2::D0C0;
  /*p06.CYLO*/ Reg2 _SER_CNT1      = Reg2::D0C0;
  /*p06.CYDE*/ Reg2 _SER_CNT2      = Reg2::D0C0;
                                   
  /*p06.CUBA*/ Reg2 _SER_DATA0     = Reg2::D0C0;
  /*p06.DEGU*/ Reg2 _SER_DATA1     = Reg2::D0C0;
  /*p06.DYRA*/ Reg2 _SER_DATA2     = Reg2::D0C0;
  /*p06.DOJO*/ Reg2 _SER_DATA3     = Reg2::D0C0;
  /*p06.DOVU*/ Reg2 _SER_DATA4     = Reg2::D0C0;
  /*p06.EJAB*/ Reg2 _SER_DATA5     = Reg2::D0C0;
  /*p06.EROD*/ Reg2 _SER_DATA6     = Reg2::D0C0;
  /*p06.EDER*/ Reg2 _SER_DATA7     = Reg2::D0C0;
                                   
  /*p06.ELYS*/ Reg2 _SER_OUT       = Reg2::D0C0;

  /*p06.CALY*/ Reg2 _CALY_SER_INTp = Reg2::D0C0;
  //----------
  // Serial pins

  /* PIN_68 */ Pin2 SCK_A = Pin2::HIZ_NP;   // <- P06.KEXU
  /* PIN_68 */ Pin2 SCK_B = Pin2::HIZ_NP;   // <- P06.CULY
  /* PIN_68 */ Pin2 SCK_C = Pin2::HOLD_0;   // -> P06.CAVE
  /* PIN_68 */ Pin2 SCK_D = Pin2::HIZ_NP;   // <- P06.KUJO

  ///* PIN_69 */ Pin2 SIN_A = Pin2::HIZ_NP;   // nc?
  ///* PIN_69 */ Pin2 SIN_B = Pin2::HIZ_NP;   // nc?
  /* PIN_69 */ Pin2 SIN_C = Pin2::HOLD_0;   // -> P06.CAGE
  ///* PIN_69 */ Pin2 SIN_D = Pin2::HIZ_NP;   // nc?

  /* PIN_70 */ Pin2 SOUT  = Pin2::HIZ_NP;   // <- P05.KENA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics