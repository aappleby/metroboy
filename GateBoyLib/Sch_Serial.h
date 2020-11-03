#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct SerialRegisters {
  void dump(Dumper& d) const {
    d("\002===== Serial =====\001\n");
    d("XFER_START  %c\n", ETAF_XFER_START.c());
    d("XFER_DIR    %c\n", CULY_XFER_DIR  .c());
    d("SER_CLK     %c\n", COTY_SER_CLK   .c());
    d("SER_CNT     %d\n", pack_p(!CAFA_SER_CNT0.qn16(), !CYLO_SER_CNT1.qn16(), !CYDE_SER_CNT2.qn16(), 0));
    d("SER_DATA    0x%02x\n", get_data());
    d("SER_OUT     %c\n", ELYS_SER_OUT.c());
    d("SCK         %c\n", PIN_SCK.c());
    d("SIN         %c\n", PIN_SIN.c());
    d("SOUT        %c\n", PIN_SOUT.c());
    d("_CALY_SER_INTp %c\n", CALY_INT_SERp.c());
    d("\n");
  }

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
  }

  void tock(SchematicTop& top, CpuBus& cpu_bus);

  int get_data() const {
    return pack_p(EDER_SER_DATA7.qp16(), EROD_SER_DATA6.qp16(), EJAB_SER_DATA5.qp16(), DOVU_SER_DATA4.qp16(),
                  DOJO_SER_DATA3.qp16(), DYRA_SER_DATA2.qp16(), DEGU_SER_DATA1.qp16(), CUBA_SER_DATA0.qp16());
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

  /* PIN_68 */ PinNP PIN_SCK;
  /* PIN_69 */ PinNP PIN_SIN;
  /* PIN_70 */ PinNP PIN_SOUT;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics