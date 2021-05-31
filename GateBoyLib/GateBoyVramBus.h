#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct VramBus {
  void reset_to_cart() {
    BUS_VRAM_A00n.reset(BIT_OLD | BIT_DRIVEN | 1);
    BUS_VRAM_A01n.reset(BIT_OLD | BIT_DRIVEN | 1);
    BUS_VRAM_A02n.reset(BIT_OLD | BIT_DRIVEN | 1);
    BUS_VRAM_A03n.reset(BIT_OLD | BIT_DRIVEN | 1);
    BUS_VRAM_A04n.reset(BIT_OLD | BIT_DRIVEN | 0);
    BUS_VRAM_A05n.reset(BIT_OLD | BIT_DRIVEN | 1);
    BUS_VRAM_A06n.reset(BIT_OLD | BIT_DRIVEN | 0);
    BUS_VRAM_A07n.reset(BIT_OLD | BIT_DRIVEN | 1);
    BUS_VRAM_A08n.reset(BIT_OLD | BIT_DRIVEN | 1);
    BUS_VRAM_A09n.reset(BIT_OLD | BIT_DRIVEN | 1);
    BUS_VRAM_A10n.reset(BIT_OLD | BIT_DRIVEN | 1);
    BUS_VRAM_A11n.reset(BIT_OLD | BIT_DRIVEN | 1);
    BUS_VRAM_A12n.reset(BIT_OLD | BIT_DRIVEN | 1);

    BUS_VRAM_D00p.reset(BIT_OLD | BIT_PULLED | 0);
    BUS_VRAM_D01p.reset(BIT_OLD | BIT_PULLED | 0);
    BUS_VRAM_D02p.reset(BIT_OLD | BIT_PULLED | 0);
    BUS_VRAM_D03p.reset(BIT_OLD | BIT_PULLED | 0);
    BUS_VRAM_D04p.reset(BIT_OLD | BIT_PULLED | 0);
    BUS_VRAM_D05p.reset(BIT_OLD | BIT_PULLED | 0);
    BUS_VRAM_D06p.reset(BIT_OLD | BIT_PULLED | 0);
    BUS_VRAM_D07p.reset(BIT_OLD | BIT_PULLED | 0);
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
    PIN_43_VRAM_CSn.reset(BIT_OLD | BIT_DRIVEN | 0);
    PIN_45_VRAM_OEn.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_49_VRAM_WRn.reset(BIT_OLD | BIT_DRIVEN | 0);

    PIN_34_VRAM_A00.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_35_VRAM_A01.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_36_VRAM_A02.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_37_VRAM_A03.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_38_VRAM_A04.reset(BIT_OLD | BIT_DRIVEN | 0);
    PIN_39_VRAM_A05.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_40_VRAM_A06.reset(BIT_OLD | BIT_DRIVEN | 0);
    PIN_41_VRAM_A07.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_48_VRAM_A08.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_47_VRAM_A09.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_44_VRAM_A10.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_46_VRAM_A11.reset(BIT_OLD | BIT_DRIVEN | 1);
    PIN_42_VRAM_A12.reset(BIT_OLD | BIT_DRIVEN | 1);

    PIN_33_VRAM_D00.reset_int(BIT_OLD | BIT_DRIVEN | 1);
    PIN_31_VRAM_D01.reset_int(BIT_OLD | BIT_DRIVEN | 1);
    PIN_30_VRAM_D02.reset_int(BIT_OLD | BIT_DRIVEN | 1);
    PIN_29_VRAM_D03.reset_int(BIT_OLD | BIT_DRIVEN | 1);
    PIN_28_VRAM_D04.reset_int(BIT_OLD | BIT_DRIVEN | 1);
    PIN_27_VRAM_D05.reset_int(BIT_OLD | BIT_DRIVEN | 1);
    PIN_26_VRAM_D06.reset_int(BIT_OLD | BIT_DRIVEN | 1);
    PIN_25_VRAM_D07.reset_int(BIT_OLD | BIT_DRIVEN | 1);
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

  /*PIN_33*/ PinIO PIN_33_VRAM_D00;
  /*PIN_31*/ PinIO PIN_31_VRAM_D01;
  /*PIN_30*/ PinIO PIN_30_VRAM_D02;
  /*PIN_29*/ PinIO PIN_29_VRAM_D03;
  /*PIN_28*/ PinIO PIN_28_VRAM_D04;
  /*PIN_27*/ PinIO PIN_27_VRAM_D05;
  /*PIN_26*/ PinIO PIN_26_VRAM_D06;
  /*PIN_25*/ PinIO PIN_25_VRAM_D07;
};

//-----------------------------------------------------------------------------
