#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct SerialRegisters {
  void reset_cart() {
    reset_boot();

    CALY_INT_SERp.reset(REG_D0C1);
    ETAF_XFER_START.reset(REG_D0C1);
    CULY_XFER_DIR.reset(REG_D0C1);
    COTY_SER_CLK.reset(REG_D0C0);
    CAFA_SER_CNT0.reset(REG_D0C1);
    CYLO_SER_CNT1.reset(REG_D0C1);
    CYDE_SER_CNT2.reset(REG_D0C1);

    CUBA_SER_DATA0.reset(REG_D0C1);
    DEGU_SER_DATA1.reset(REG_D0C1);
    DYRA_SER_DATA2.reset(REG_D0C1);
    DOJO_SER_DATA3.reset(REG_D0C1);
    DOVU_SER_DATA4.reset(REG_D0C1);
    EJAB_SER_DATA5.reset(REG_D0C1);
    EROD_SER_DATA6.reset(REG_D0C1);
    EDER_SER_DATA7.reset(REG_D0C1);

    ELYS_SER_OUT.reset(REG_D0C0);

    SCK.reset(TRI_HZNP);
    SIN.reset(TRI_HZNP);
    SOUT.reset(TRI_D1NP);
  }

  void reset_boot() {
    CALY_INT_SERp.reset(REG_D0C0);
    ETAF_XFER_START.reset(REG_D0C0);
    CULY_XFER_DIR.reset(REG_D0C0);
    COTY_SER_CLK.reset(REG_D0C0);
    CAFA_SER_CNT0.reset(REG_D0C0);
    CYLO_SER_CNT1.reset(REG_D0C0);
    CYDE_SER_CNT2.reset(REG_D0C0);

    CUBA_SER_DATA0.reset(REG_D0C0);
    DEGU_SER_DATA1.reset(REG_D0C0);
    DYRA_SER_DATA2.reset(REG_D0C0);
    DOJO_SER_DATA3.reset(REG_D0C0);
    DOVU_SER_DATA4.reset(REG_D0C0);
    EJAB_SER_DATA5.reset(REG_D0C0);
    EROD_SER_DATA6.reset(REG_D0C0);
    EDER_SER_DATA7.reset(REG_D0C0);

    ELYS_SER_OUT.reset(REG_D0C0);

    SCK.reset(TRI_HZNP);
    SIN.reset(TRI_HZNP);
    SOUT.reset(TRI_HZNP);
  }

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
  /*p06.ETAF*/ DFF17 ETAF_XFER_START;
  /*p06.CULY*/ DFF17 CULY_XFER_DIR;

  /*p06.COTY*/ DFF17 COTY_SER_CLK;

  /*p06.CAFA*/ DFF17 CAFA_SER_CNT0;
  /*p06.CYLO*/ DFF17 CYLO_SER_CNT1;
  /*p06.CYDE*/ DFF17 CYDE_SER_CNT2;

  /*p06.CUBA*/ DFF22 CUBA_SER_DATA0;
  /*p06.DEGU*/ DFF22 DEGU_SER_DATA1;
  /*p06.DYRA*/ DFF22 DYRA_SER_DATA2;
  /*p06.DOJO*/ DFF22 DOJO_SER_DATA3;
  /*p06.DOVU*/ DFF22 DOVU_SER_DATA4;
  /*p06.EJAB*/ DFF22 EJAB_SER_DATA5;
  /*p06.EROD*/ DFF22 EROD_SER_DATA6;
  /*p06.EDER*/ DFF22 EDER_SER_DATA7;

  /*p06.ELYS*/ DFF17 ELYS_SER_OUT;

  //----------
  // Serial pins

  /* PIN_68 */ Pin SCK;
  /* PIN_69 */ Pin SIN;
  /* PIN_70 */ Pin SOUT;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics