#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct SerialRegisters {
  void reset_cart() {
    reset_boot();

    CALY_SER_CNT3.reset(REG_D0C1);
    ETAF_SER_RUNNING.reset(REG_D0C1);
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
  }

  void reset_boot() {
    CALY_SER_CNT3.reset(REG_D0C0);
    ETAF_SER_RUNNING.reset(REG_D0C0);
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
  }

  /*p06.ETAF*/ DFF17 ETAF_SER_RUNNING; // xxxxxxxH ?
  /*p06.CULY*/ DFF17 CULY_XFER_DIR;    // AxxxDxxH ?
  /*p06.COTY*/ DFF17 COTY_SER_CLK;     // AxxxDxxH ?
  /*p06.ELYS*/ DFF17 ELYS_SER_OUT;     // AxxxDxxH ?

  /*p06.CAFA*/ DFF17 CAFA_SER_CNT0; // ????????
  /*p06.CYLO*/ DFF17 CYLO_SER_CNT1; // ????????
  /*p06.CYDE*/ DFF17 CYDE_SER_CNT2; // ????????
  /*p06.CALY*/ DFF17 CALY_SER_CNT3; // ????????

  /*p06.CUBA*/ DFF22 CUBA_SER_DATA0; // xxxxExxx
  /*p06.DEGU*/ DFF22 DEGU_SER_DATA1; // xxxxExxx
  /*p06.DYRA*/ DFF22 DYRA_SER_DATA2; // xxxxExxx
  /*p06.DOJO*/ DFF22 DOJO_SER_DATA3; // xxxxExxx
  /*p06.DOVU*/ DFF22 DOVU_SER_DATA4; // xxxxExxx
  /*p06.EJAB*/ DFF22 EJAB_SER_DATA5; // xxxxExxx
  /*p06.EROD*/ DFF22 EROD_SER_DATA6; // xxxxExxx
  /*p06.EDER*/ DFF22 EDER_SER_DATA7; // xxxxExxx

};

//-----------------------------------------------------------------------------
