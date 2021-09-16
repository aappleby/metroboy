#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct VramABusLo {
  void reset_to_poweron() {
  }

  void reset_to_bootrom() {
  }

  void reset_to_cart() {
    BUS_VRAM_A00n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A01n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A02n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A03n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A04n.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_A05n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A06n.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_A07n.state = BIT_OLD | BIT_DRIVEN | 1;
  }

  /*_BUS_VRAM_A00n*/ Bus BUS_VRAM_A00n;
  /*_BUS_VRAM_A01n*/ Bus BUS_VRAM_A01n;
  /*_BUS_VRAM_A02n*/ Bus BUS_VRAM_A02n;
  /*_BUS_VRAM_A03n*/ Bus BUS_VRAM_A03n;
  /*_BUS_VRAM_A04n*/ Bus BUS_VRAM_A04n;
  /*_BUS_VRAM_A05n*/ Bus BUS_VRAM_A05n;
  /*_BUS_VRAM_A06n*/ Bus BUS_VRAM_A06n;
  /*_BUS_VRAM_A07n*/ Bus BUS_VRAM_A07n;
};

struct VramABusHi {
  void reset_to_poweron() {
  }

  void reset_to_bootrom() {
  }

  void reset_to_cart() {
    BUS_VRAM_A08n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A09n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A10n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A11n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A12n.state = BIT_OLD | BIT_DRIVEN | 1;
  }

  /*_BUS_VRAM_A08n*/ Bus BUS_VRAM_A08n;
  /*_BUS_VRAM_A09n*/ Bus BUS_VRAM_A09n;
  /*_BUS_VRAM_A10n*/ Bus BUS_VRAM_A10n;
  /*_BUS_VRAM_A11n*/ Bus BUS_VRAM_A11n;
  /*_BUS_VRAM_A12n*/ Bus BUS_VRAM_A12n;
};

//-----------------------------------------------------------------------------

struct VramDBus {
  void reset_to_poweron() {
  }

  void reset_to_bootrom() {
  }

  void reset_to_cart() {
    BUS_VRAM_D00p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D01p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D02p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D03p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D04p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D05p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D06p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D07p.state = BIT_OLD | BIT_DRIVEN | 0;
  }

  /*_BUS_VRAM_D00p*/ Bus BUS_VRAM_D00p;
  /*_BUS_VRAM_D01p*/ Bus BUS_VRAM_D01p;
  /*_BUS_VRAM_D02p*/ Bus BUS_VRAM_D02p;
  /*_BUS_VRAM_D03p*/ Bus BUS_VRAM_D03p;
  /*_BUS_VRAM_D04p*/ Bus BUS_VRAM_D04p;
  /*_BUS_VRAM_D05p*/ Bus BUS_VRAM_D05p;
  /*_BUS_VRAM_D06p*/ Bus BUS_VRAM_D06p;
  /*_BUS_VRAM_D07p*/ Bus BUS_VRAM_D07p;
};

//-----------------------------------------------------------------------------

struct VramExtControl {
  void reset_to_poweron() {
  }

  void reset_to_bootrom() {
  }

  void reset_to_cart() {
    PIN_43_VRAM_CSn.state = BIT_OLD | BIT_DRIVEN | 0;
    PIN_45_VRAM_OEn.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_49_VRAM_WRn.state = BIT_OLD | BIT_DRIVEN | 0;
  }

  /*_PIN_43*/ PinOut PIN_43_VRAM_CSn;
  /*_PIN_45*/ PinOut PIN_45_VRAM_OEn;
  /*_PIN_49*/ PinOut PIN_49_VRAM_WRn;
};

//-----------------------------------------------------------------------------

struct VramExtABus {
  void reset_to_poweron() {
  }

  void reset_to_bootrom() {
  }

  void reset_to_cart() {
    PIN_34_VRAM_A00.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_35_VRAM_A01.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_36_VRAM_A02.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_37_VRAM_A03.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_38_VRAM_A04.state = BIT_OLD | BIT_DRIVEN | 0;
    PIN_39_VRAM_A05.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_40_VRAM_A06.state = BIT_OLD | BIT_DRIVEN | 0;
    PIN_41_VRAM_A07.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_48_VRAM_A08.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_47_VRAM_A09.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_44_VRAM_A10.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_46_VRAM_A11.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_42_VRAM_A12.state = BIT_OLD | BIT_DRIVEN | 1;
  }

  /*_PIN_34*/ PinOut PIN_34_VRAM_A00;
  /*_PIN_35*/ PinOut PIN_35_VRAM_A01;
  /*_PIN_36*/ PinOut PIN_36_VRAM_A02;
  /*_PIN_37*/ PinOut PIN_37_VRAM_A03;
  /*_PIN_38*/ PinOut PIN_38_VRAM_A04;
  /*_PIN_39*/ PinOut PIN_39_VRAM_A05;
  /*_PIN_40*/ PinOut PIN_40_VRAM_A06;
  /*_PIN_41*/ PinOut PIN_41_VRAM_A07;
  /*_PIN_48*/ PinOut PIN_48_VRAM_A08;
  /*_PIN_47*/ PinOut PIN_47_VRAM_A09;
  /*_PIN_44*/ PinOut PIN_44_VRAM_A10;
  /*_PIN_46*/ PinOut PIN_46_VRAM_A11;
  /*_PIN_42*/ PinOut PIN_42_VRAM_A12;
};

//-----------------------------------------------------------------------------

struct VramExtDBus {
  void reset_to_poweron() {
  }

  void reset_to_bootrom() {
  }

  void reset_to_cart() {
    PIN_33_VRAM_D00.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_31_VRAM_D01.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_30_VRAM_D02.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_29_VRAM_D03.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_28_VRAM_D04.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_27_VRAM_D05.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_26_VRAM_D06.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_25_VRAM_D07.state = BIT_OLD | BIT_DRIVEN | 1;
  }

  /*_PIN_33*/ PinIO PIN_33_VRAM_D00;
  /*_PIN_31*/ PinIO PIN_31_VRAM_D01;
  /*_PIN_30*/ PinIO PIN_30_VRAM_D02;
  /*_PIN_29*/ PinIO PIN_29_VRAM_D03;
  /*_PIN_28*/ PinIO PIN_28_VRAM_D04;
  /*_PIN_27*/ PinIO PIN_27_VRAM_D05;
  /*_PIN_26*/ PinIO PIN_26_VRAM_D06;
  /*_PIN_25*/ PinIO PIN_25_VRAM_D07;
};

//-----------------------------------------------------------------------------
