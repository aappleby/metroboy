#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct SerialRegisters {
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  void dump(Dumper& d) const;

  void set_pins(RegDelta d_sck, RegDelta d_sin) {
    SCK = d_sck;
    SIN = d_sin;
  }

  int get_data() const {
    return pack_p(EDER_SER_DATA7.qp(), EROD_SER_DATA6.qp(), EJAB_SER_DATA5.qp(), DOVU_SER_DATA4.qp(),
                  DOJO_SER_DATA3.qp(), DYRA_SER_DATA2.qp(), DEGU_SER_DATA1.qp(), CUBA_SER_DATA0.qp());
  }

  /*p06.CALY*/ DFF17 CALY_INT_SERp;

private:

  /*p06.ETAF*/ DFF17 ETAF_XFER_START;
  /*p06.CULY*/ DFF17 CULY_XFER_DIR;
                                   
  /*p06.COTY*/ DFF17 COTY_SER_CLK;
                                   
  /*p06.CAFA*/ DFF17 CAFA_SER_CNT0;
  /*p06.CYLO*/ DFF17 CYLO_SER_CNT1;
  /*p06.CYDE*/ DFF17 CYDE_SER_CNT2;
                                   
  /*p06.CUBA*/ Reg CUBA_SER_DATA0     = REG_D0C0;
  /*p06.DEGU*/ Reg DEGU_SER_DATA1     = REG_D0C0;
  /*p06.DYRA*/ Reg DYRA_SER_DATA2     = REG_D0C0;
  /*p06.DOJO*/ Reg DOJO_SER_DATA3     = REG_D0C0;
  /*p06.DOVU*/ Reg DOVU_SER_DATA4     = REG_D0C0;
  /*p06.EJAB*/ Reg EJAB_SER_DATA5     = REG_D0C0;
  /*p06.EROD*/ Reg EROD_SER_DATA6     = REG_D0C0;
  /*p06.EDER*/ Reg EDER_SER_DATA7     = REG_D0C0;
                                   
  /*p06.ELYS*/ DFF17 ELYS_SER_OUT;

  //----------
  // Serial pins

  /* PIN_68 */ Pin SCK  = TRI_HZPU;
  /* PIN_69 */ Pin SIN  = TRI_HZPU;
  /* PIN_70 */ Pin SOUT = TRI_HZPU;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics