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

struct PinsSerial {
  void reset_to_cart() {
    PIN_68_SCK. state = 0b00010100;
    PIN_69_SIN. state = 0b00011000;
    PIN_70_SOUT.state = 0b00011000;
  }

  /*_PIN_68*/ PinIO  PIN_68_SCK;
  /*_PIN_69*/ PinIn  PIN_69_SIN;
  /*_PIN_70*/ PinOut PIN_70_SOUT;
};

//-----------------------------------------------------------------------------

struct PinsVramControl {
  void reset_to_poweron() {
    memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
  }

  void reset_to_bootrom() {
    PIN_43_VRAM_CSn.state = BIT_OLD | BIT_DRIVEN | 0;
    PIN_45_VRAM_OEn.state = BIT_OLD | BIT_DRIVEN | 1;
    PIN_49_VRAM_WRn.state = BIT_OLD | BIT_DRIVEN | 0;
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

struct PinsVramABus {
  void reset_to_poweron() {
    memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
  }

  void reset_to_bootrom() {
    memset(this, BIT_OLD | BIT_DRIVEN | BIT_DATA, sizeof(*this));
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

struct PinsVramDBus {
  void reset_to_poweron() {
    memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
  }

  void reset_to_bootrom() {
    memset(this, BIT_OLD | BIT_DRIVEN | BIT_DATA, sizeof(*this));
  }

  void reset_to_cart() {
    memset(this, BIT_OLD | BIT_DRIVEN | BIT_DATA, sizeof(*this));
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

struct PinsSys {
  void reset_to_poweron() {
    memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
  }

  void reset_to_bootrom() {
    PIN_71_RST.state = BIT_OLD | BIT_DRIVEN;
    PIN_77_T1. state = BIT_OLD | BIT_DRIVEN;
    PIN_76_T2. state = BIT_OLD | BIT_DRIVEN;
  }

  void reset_to_cart() {
    PIN_71_RST.state = 0b00011000;
    PIN_77_T1. state = 0b00011000;
    PIN_76_T2. state = 0b00011000;
  }

  /*_p07.UBET*/ wire UBETp()           const { return not1(PIN_77_T1.qp_int_new()); }
  /*_p07.UVAR*/ wire UVARp()           const { return not1(PIN_76_T2.qp_int_new()); }
  /*_p07.UMUT*/ wire UMUT_MODE_DBG1p() const { return and2(PIN_77_T1.qp_int_new(), UVARp()); }
  /*_p08.MULE*/ wire MULE_MODE_DBG1n() const { return not1(UMUT_MODE_DBG1p()); }
  /*_p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and2(PIN_76_T2.qp_int_new(), UBETp()); }
  /*_p08.RYCA*/ wire RYCA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /*_p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /*_p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand3(UBETp(), UVARp(), PIN_71_RST.qp_int_new()); }

  /*_PIN_71*/ PinIn  PIN_71_RST;
  /*_PIN_77*/ PinIn  PIN_77_T1;
  /*_PIN_76*/ PinIn  PIN_76_T2;
};

//-----------------------------------------------------------------------------

struct GateBoyPins {

  PinsABusLo      abus_lo;   // 01-08
  PinsABusHi      abus_hi;   // 09-16
  PinsDBus        dbus;      // 17-24
  PinsVramDBus    vram_dbus; // 25-33
  PinsVramABus    vram_abus; // 34-42
  PinsVramControl vram_ctrl; // 43/45/49
  PinsJoypad      joy;       // 62-67
  //PinsSerial      serial;    // 68-70
  PinsSys         sys;    // 71/76/77
  PinsControl     ctrl;   // 78-80

  void reset_to_poweron() {
    abus_lo.reset_to_poweron();
    abus_hi.reset_to_poweron();
    dbus.reset_to_poweron();
    vram_dbus.reset_to_poweron();
    vram_abus.reset_to_poweron();
    vram_ctrl.reset_to_poweron();
    joy.reset_to_poweron();
    ctrl.reset_to_poweron();
  }
  
  void reset_to_bootrom() {
    abus_lo.reset_to_bootrom();
    abus_hi.reset_to_bootrom();
    dbus.reset_to_bootrom();
    vram_dbus.reset_to_bootrom();
    vram_abus.reset_to_bootrom();
    vram_ctrl.reset_to_bootrom();
    joy.reset_to_bootrom();
    ctrl.reset_to_bootrom();
  }
  
  void reset_to_cart()    {
    abus_lo.reset_to_cart();
    abus_hi.reset_to_cart();
    dbus.reset_to_cart();
    vram_dbus.reset_to_cart();
    vram_abus.reset_to_cart();
    vram_ctrl.reset_to_cart();
    joy.reset_to_cart();
    ctrl.reset_to_cart();
  }
};

//-----------------------------------------------------------------------------
