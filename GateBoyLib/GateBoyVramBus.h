#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyBuses.h"
#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct GateBoyVramBus {

  void reset_to_cart() {
    PIN_34_VRAM_A00.reset(1);
    PIN_35_VRAM_A01.reset(1);
    PIN_36_VRAM_A02.reset(1);
    PIN_37_VRAM_A03.reset(1);
    PIN_38_VRAM_A04.reset(0);
    PIN_39_VRAM_A05.reset(1);
    PIN_40_VRAM_A06.reset(0);
    PIN_41_VRAM_A07.reset(1);
    PIN_48_VRAM_A08.reset(1);
    PIN_47_VRAM_A09.reset(1);
    PIN_44_VRAM_A10.reset(1);
    PIN_46_VRAM_A11.reset(1);
    PIN_42_VRAM_A12.reset(1);
  }

  void reset_buses() {
    PIN_43_VRAM_CSn.reset_for_pass();
    PIN_45_VRAM_OEn.reset_for_pass();
    PIN_49_VRAM_WRn.reset_for_pass();

    PIN_34_VRAM_A00.reset_for_pass();
    PIN_35_VRAM_A01.reset_for_pass();
    PIN_36_VRAM_A02.reset_for_pass();
    PIN_37_VRAM_A03.reset_for_pass();
    PIN_38_VRAM_A04.reset_for_pass();
    PIN_39_VRAM_A05.reset_for_pass();
    PIN_40_VRAM_A06.reset_for_pass();
    PIN_41_VRAM_A07.reset_for_pass();
    PIN_48_VRAM_A08.reset_for_pass();
    PIN_47_VRAM_A09.reset_for_pass();
    PIN_44_VRAM_A10.reset_for_pass();
    PIN_46_VRAM_A11.reset_for_pass();
    PIN_42_VRAM_A12.reset_for_pass();

    PIN_33_VRAM_D00.reset_for_pass();
    PIN_31_VRAM_D01.reset_for_pass();
    PIN_30_VRAM_D02.reset_for_pass();
    PIN_29_VRAM_D03.reset_for_pass();
    PIN_28_VRAM_D04.reset_for_pass();
    PIN_27_VRAM_D05.reset_for_pass();
    PIN_26_VRAM_D06.reset_for_pass();
    PIN_25_VRAM_D07.reset_for_pass();
  }

  void dump(Dumper& d) {
    d.dump_bitp   ("PIN_43_VRAM_CSn  : ", PIN_43_VRAM_CSn.state);
    d.dump_bitp   ("PIN_45_VRAM_OEn  : ", PIN_45_VRAM_OEn.state);
    d.dump_bitp   ("PIN_49_VRAM_WRn  : ", PIN_49_VRAM_WRn.state);
    d.dump_slice2p("PIN_34_VRAM_ADDR : ", &PIN_34_VRAM_A00, 13);
    d.dump_slice2p("PIN_25_VRAM_DATA : ", &PIN_33_VRAM_D00, 8);
    //d.dump_slice2n("BUS_VRAM_An     : ", &BUS_VRAM_A00n, 13);
    //d.dump_slice2p("BUS_VRAM_Dp     : ", &BUS_VRAM_D00p, 8);
  }

  /*PIN_43*/ PinOut PIN_43_VRAM_CSn;
  /*PIN_45*/ PinOut PIN_45_VRAM_OEn;
  /*PIN_49*/ PinOut PIN_49_VRAM_WRn;

  /*PIN_34*/ PinOut PIN_34_VRAM_A00;
  /*PIN_35*/ PinOut PIN_35_VRAM_A01;
  /*PIN_36*/ PinOut PIN_36_VRAM_A02;
  /*PIN_37*/ PinOut PIN_37_VRAM_A03;
  /*PIN_38*/ PinOut PIN_38_VRAM_A04;
  /*PIN_39*/ PinOut PIN_39_VRAM_A05;
  /*PIN_40*/ PinOut PIN_40_VRAM_A06;
  /*PIN_41*/ PinOut PIN_41_VRAM_A07;
  /*PIN_48*/ PinOut PIN_48_VRAM_A08;
  /*PIN_47*/ PinOut PIN_47_VRAM_A09;
  /*PIN_44*/ PinOut PIN_44_VRAM_A10;
  /*PIN_46*/ PinOut PIN_46_VRAM_A11;
  /*PIN_42*/ PinOut PIN_42_VRAM_A12;

  /*PIN_33*/ PinIO  PIN_33_VRAM_D00;
  /*PIN_31*/ PinIO  PIN_31_VRAM_D01;
  /*PIN_30*/ PinIO  PIN_30_VRAM_D02;
  /*PIN_29*/ PinIO  PIN_29_VRAM_D03;
  /*PIN_28*/ PinIO  PIN_28_VRAM_D04;
  /*PIN_27*/ PinIO  PIN_27_VRAM_D05;
  /*PIN_26*/ PinIO  PIN_26_VRAM_D06;
  /*PIN_25*/ PinIO  PIN_25_VRAM_D07;
};

//-----------------------------------------------------------------------------
