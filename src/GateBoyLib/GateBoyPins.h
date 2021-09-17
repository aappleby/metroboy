#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct PinsControl {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x02); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x02); }
  void reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x02); }

  /*_PIN_80*/ PinOut PIN_80_CSn;      // CS changes on phase C if addr in [A000,FDFF]
  /*_PIN_79*/ PinOut PIN_79_RDn;      // RDn idles low, goes high on phase B for an external write
  /*_PIN_78*/ PinOut PIN_78_WRn;      // WRn idles high, goes low during EFG if there's a write
};

//-----------------------------------------------------------------------------

struct PinsABusLo {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0xFF); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0xFF); }
  void reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0xB2); }

  /*_PIN_01*/ PinOut PIN_01_A00;
  /*_PIN_02*/ PinOut PIN_02_A01;
  /*_PIN_03*/ PinOut PIN_03_A02;
  /*_PIN_04*/ PinOut PIN_04_A03;
  /*_PIN_05*/ PinOut PIN_05_A04;
  /*_PIN_06*/ PinOut PIN_06_A05;
  /*_PIN_07*/ PinOut PIN_07_A06;
  /*_PIN_08*/ PinOut PIN_08_A07;
};

//-----------------------------------------------------------------------------

struct PinsABusHi {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x7F); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x7F); }
  void reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x7F); }

  /*_PIN_09*/ PinOut PIN_09_A08;
  /*_PIN_10*/ PinOut PIN_10_A09;
  /*_PIN_11*/ PinOut PIN_11_A10;
  /*_PIN_12*/ PinOut PIN_12_A11;
  /*_PIN_13*/ PinOut PIN_13_A12;
  /*_PIN_14*/ PinOut PIN_14_A13;
  /*_PIN_15*/ PinOut PIN_15_A14;
  /*_PIN_16*/ PinOut PIN_16_A15;
};

//-----------------------------------------------------------------------------

struct PinsDBus {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_PULLED, 0x00); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_PULLED, 0x00); }
  void reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_PULLED, 0x00); }

  /*_PIN_17*/ PinIO PIN_17_D00;
  /*_PIN_18*/ PinIO PIN_18_D01;
  /*_PIN_19*/ PinIO PIN_19_D02;
  /*_PIN_20*/ PinIO PIN_20_D03;
  /*_PIN_21*/ PinIO PIN_21_D04;
  /*_PIN_22*/ PinIO PIN_22_D05;
  /*_PIN_23*/ PinIO PIN_23_D06;
  /*_PIN_24*/ PinIO PIN_24_D07;
};

//-----------------------------------------------------------------------------

struct PinsJoypad {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x30); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x30); }
  void reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x30); }

  // Pressing a button pulls the corresponding pin _down_.
  /*_PIN_67*/ PinIn  PIN_67_JOY_P10;
  /*_PIN_66*/ PinIn  PIN_66_JOY_P11;
  /*_PIN_65*/ PinIn  PIN_65_JOY_P12;
  /*_PIN_64*/ PinIn  PIN_64_JOY_P13;
  /*#PIN_63*/ PinOut PIN_63_JOY_P14;
  /*#PIN_62*/ PinOut PIN_62_JOY_P15;
};

//-----------------------------------------------------------------------------

struct GateBoyPins {

  PinsABusLo  abus_lo; // 01-08
  PinsABusHi  abus_hi; // 09-16
  PinsDBus    dbus;    // 17-24
  PinsJoypad  joy;     // 62-67
  PinsControl control; // 78-80

  void reset_to_poweron() {
    abus_lo.reset_to_poweron();
    abus_hi.reset_to_poweron();
    dbus.reset_to_poweron();
    joy.reset_to_poweron();
    control.reset_to_poweron();
  }
  
  void reset_to_bootrom() {
    abus_lo.reset_to_bootrom();
    abus_hi.reset_to_bootrom();
    dbus.reset_to_bootrom();
    joy.reset_to_bootrom();
    control.reset_to_bootrom();
  }
  
  void reset_to_cart()    {
    abus_lo.reset_to_cart();
    abus_hi.reset_to_cart();
    dbus.reset_to_cart();
    joy.reset_to_cart();
    control.reset_to_cart();
  }
};

//-----------------------------------------------------------------------------
