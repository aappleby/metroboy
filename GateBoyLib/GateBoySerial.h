#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct GateBoySerial {
  void reset_to_cart() {
    ETAF_SER_RUN.state = 0b00011010;
    CULY_SER_DIR.state = 0b00011010;
    COTY_SER_CLK.state = 0b00011000;
    ELYS_SER_OUT.state = 0b00011000;
    CAFA_SER_CNT0.state = 0b00011010;
    CYLO_SER_CNT1.state = 0b00011010;
    CYDE_SER_CNT2.state = 0b00011010;
    CALY_SER_CNT3.state = 0b00011010;
    CUBA_SER_DATA0.state = 0b00011010;
    DEGU_SER_DATA1.state = 0b00011010;
    DYRA_SER_DATA2.state = 0b00011010;
    DOJO_SER_DATA3.state = 0b00011010;
    DOVU_SER_DATA4.state = 0b00011010;
    EJAB_SER_DATA5.state = 0b00011010;
    EROD_SER_DATA6.state = 0b00011010;
    EDER_SER_DATA7.state = 0b00011010;

    PIN_68_SCK. state = 0b00010101;
    PIN_69_SIN. state = 0b00011000;
    PIN_70_SOUT.state = 0b00011000;
  }

  /* p06.ETAF*/ DFF17 ETAF_SER_RUN; // xxxxxxxH ?
  /* p06.CULY*/ DFF17 CULY_SER_DIR; // AxxxDxxH ?
  /* p06.COTY*/ DFF17 COTY_SER_CLK; // AxxxDxxH ? schematic wrong, clock is UVYN
  /* p06.ELYS*/ DFF17 ELYS_SER_OUT; // AxxxDxxH ?

  /* p06.CAFA*/ DFF17 CAFA_SER_CNT0; // ????????
  /* p06.CYLO*/ DFF17 CYLO_SER_CNT1; // ????????
  /* p06.CYDE*/ DFF17 CYDE_SER_CNT2; // ????????
  /* p06.CALY*/ DFF17 CALY_SER_CNT3; // ????????

  /* p06.CUBA*/ DFF22 CUBA_SER_DATA0; // xxxxExxx
  /* p06.DEGU*/ DFF22 DEGU_SER_DATA1; // xxxxExxx
  /* p06.DYRA*/ DFF22 DYRA_SER_DATA2; // xxxxExxx
  /* p06.DOJO*/ DFF22 DOJO_SER_DATA3; // xxxxExxx
  /* p06.DOVU*/ DFF22 DOVU_SER_DATA4; // xxxxExxx
  /* p06.EJAB*/ DFF22 EJAB_SER_DATA5; // xxxxExxx
  /* p06.EROD*/ DFF22 EROD_SER_DATA6; // xxxxExxx
  /* p06.EDER*/ DFF22 EDER_SER_DATA7; // xxxxExxx

  /* PIN_68*/ PinIO  PIN_68_SCK;
  /* PIN_69*/ PinIn  PIN_69_SIN;
  /* PIN_70*/ PinOut PIN_70_SOUT;
};

//-----------------------------------------------------------------------------
