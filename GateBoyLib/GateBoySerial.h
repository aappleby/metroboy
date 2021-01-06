#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct GateBoySerial {
  void reset_to_cart() {
    COTY_SER_CLK.reset(0, 0);
  }

  void tock1(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus, GateBoyDiv& div);
  void tock2(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);

  void read_sb(GateBoyCpuBus& cpu_bus);
  void read_sc(GateBoyCpuBus& cpu_bus);

  /*p06.ETAF*/ DFF17 ETAF_SER_RUNNING; // xxxxxxxH ?
  /*p06.CULY*/ DFF17 CULY_SER_DIR;    // AxxxDxxH ?
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

  /*PIN_68*/ PinIO  PIN_68_SCK;
  /*PIN_69*/ PinIn  PIN_69_SIN;
  /*PIN_70*/ PinOut PIN_70_SOUT;
};

//-----------------------------------------------------------------------------
