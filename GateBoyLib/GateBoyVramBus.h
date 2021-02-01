#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct VramBus {
  void reset_to_cart() {
    BUS_VRAM_A00n.state = 0b00011101;
    BUS_VRAM_A01n.state = 0b00011101;
    BUS_VRAM_A02n.state = 0b00011101;
    BUS_VRAM_A03n.state = 0b00011101;
    BUS_VRAM_A04n.state = 0b00011100;
    BUS_VRAM_A05n.state = 0b00011101;
    BUS_VRAM_A06n.state = 0b00011100;
    BUS_VRAM_A07n.state = 0b00011101;
    BUS_VRAM_A08n.state = 0b00011101;
    BUS_VRAM_A09n.state = 0b00011101;
    BUS_VRAM_A10n.state = 0b00011101;
    BUS_VRAM_A11n.state = 0b00011101;
    BUS_VRAM_A12n.state = 0b00011101;

    BUS_VRAM_D00p.state = 0b00011100;
    BUS_VRAM_D01p.state = 0b00011100;
    BUS_VRAM_D02p.state = 0b00011100;
    BUS_VRAM_D03p.state = 0b00011100;
    BUS_VRAM_D04p.state = 0b00011100;
    BUS_VRAM_D05p.state = 0b00011100;
    BUS_VRAM_D06p.state = 0b00011100;
    BUS_VRAM_D07p.state = 0b00011100;
  }

  void reset_for_pass() {
    BUS_VRAM_A00n.reset_for_pass();
    BUS_VRAM_A01n.reset_for_pass();
    BUS_VRAM_A02n.reset_for_pass();
    BUS_VRAM_A03n.reset_for_pass();
    BUS_VRAM_A04n.reset_for_pass();
    BUS_VRAM_A05n.reset_for_pass();
    BUS_VRAM_A06n.reset_for_pass();
    BUS_VRAM_A07n.reset_for_pass();
    BUS_VRAM_A08n.reset_for_pass();
    BUS_VRAM_A09n.reset_for_pass();
    BUS_VRAM_A10n.reset_for_pass();
    BUS_VRAM_A11n.reset_for_pass();
    BUS_VRAM_A12n.reset_for_pass();

    BUS_VRAM_D00p.reset_for_pass();
    BUS_VRAM_D01p.reset_for_pass();
    BUS_VRAM_D02p.reset_for_pass();
    BUS_VRAM_D03p.reset_for_pass();
    BUS_VRAM_D04p.reset_for_pass();
    BUS_VRAM_D05p.reset_for_pass();
    BUS_VRAM_D06p.reset_for_pass();
    BUS_VRAM_D07p.reset_for_pass();
  }

  /*BUS_VRAM_A00n*/ Bus BUS_VRAM_A00n;
  /*BUS_VRAM_A01n*/ Bus BUS_VRAM_A01n;
  /*BUS_VRAM_A02n*/ Bus BUS_VRAM_A02n;
  /*BUS_VRAM_A03n*/ Bus BUS_VRAM_A03n;
  /*BUS_VRAM_A04n*/ Bus BUS_VRAM_A04n;
  /*BUS_VRAM_A05n*/ Bus BUS_VRAM_A05n;
  /*BUS_VRAM_A06n*/ Bus BUS_VRAM_A06n;
  /*BUS_VRAM_A07n*/ Bus BUS_VRAM_A07n;
  /*BUS_VRAM_A08n*/ Bus BUS_VRAM_A08n;
  /*BUS_VRAM_A09n*/ Bus BUS_VRAM_A09n;
  /*BUS_VRAM_A10n*/ Bus BUS_VRAM_A10n;
  /*BUS_VRAM_A11n*/ Bus BUS_VRAM_A11n;
  /*BUS_VRAM_A12n*/ Bus BUS_VRAM_A12n;

  /*BUS_VRAM_D00p*/ Bus BUS_VRAM_D00p;
  /*BUS_VRAM_D01p*/ Bus BUS_VRAM_D01p;
  /*BUS_VRAM_D02p*/ Bus BUS_VRAM_D02p;
  /*BUS_VRAM_D03p*/ Bus BUS_VRAM_D03p;
  /*BUS_VRAM_D04p*/ Bus BUS_VRAM_D04p;
  /*BUS_VRAM_D05p*/ Bus BUS_VRAM_D05p;
  /*BUS_VRAM_D06p*/ Bus BUS_VRAM_D06p;
  /*BUS_VRAM_D07p*/ Bus BUS_VRAM_D07p;
};

//-----------------------------------------------------------------------------

struct GateBoyVramPins {

  void reset_to_cart() {
    PIN_43_VRAM_CSn.state = 0b00011000;
    PIN_45_VRAM_OEn.state = 0b00011001;
    PIN_49_VRAM_WRn.state = 0b00011000;
    PIN_34_VRAM_A00.state = 0b00011001;
    PIN_35_VRAM_A01.state = 0b00011001;
    PIN_36_VRAM_A02.state = 0b00011001;
    PIN_37_VRAM_A03.state = 0b00011001;
    PIN_38_VRAM_A04.state = 0b00011000;
    PIN_39_VRAM_A05.state = 0b00011001;
    PIN_40_VRAM_A06.state = 0b00011000;
    PIN_41_VRAM_A07.state = 0b00011001;
    PIN_48_VRAM_A08.state = 0b00011001;
    PIN_47_VRAM_A09.state = 0b00011001;
    PIN_44_VRAM_A10.state = 0b00011001;
    PIN_46_VRAM_A11.state = 0b00011001;
    PIN_42_VRAM_A12.state = 0b00011001;
    PIN_33_VRAM_D00.state = 0b00011101;
    PIN_31_VRAM_D01.state = 0b00011101;
    PIN_30_VRAM_D02.state = 0b00011101;
    PIN_29_VRAM_D03.state = 0b00011101;
    PIN_28_VRAM_D04.state = 0b00011101;
    PIN_27_VRAM_D05.state = 0b00011101;
    PIN_26_VRAM_D06.state = 0b00011101;
    PIN_25_VRAM_D07.state = 0b00011101;
  }

  void reset_for_pass() {
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
