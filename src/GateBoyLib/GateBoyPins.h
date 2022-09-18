#pragma once
#include "GateBoyLib/Regs.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------

struct PinsControl {
  void reset();

  /*_PIN_80*/ PinOut PIN_80_CSn;      // CS changes on phase C if addr in [A000,FDFF]
  /*_PIN_79*/ PinOut PIN_79_RDn;      // RDn idles low, goes high on phase B for an external write
  /*_PIN_78*/ PinOut PIN_78_WRn;      // WRn idles high, goes low during EFG if there's a write
};

//-----------------------------------------------------------------------------

struct PinsABusLo {
  void reset();

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
  void reset();

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
  void reset();

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
  void reset();

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
  void reset();

  /*_PIN_68*/ PinIO  PIN_68_SCK;
  /*_PIN_69*/ PinIn  PIN_69_SIN;
  /*_PIN_70*/ PinOut PIN_70_SOUT;
};

//-----------------------------------------------------------------------------

struct PinsVramControl {
  void reset();

  /*_PIN_43*/ PinOut PIN_43_VRAM_CSn;
  /*_PIN_45*/ PinOut PIN_45_VRAM_OEn;
  /*_PIN_49*/ PinOut PIN_49_VRAM_WRn;
};

//-----------------------------------------------------------------------------

struct PinsVramABus {
  void reset();

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
  void reset();

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

struct PinsSys {
  void reset();

  wire UCOB_CLKBADp_old() const;
  wire ATEZ_CLKBADp_old() const;

  wire UBETp_old() const;
  wire UVARp_old() const;

  wire UMUT_MODE_DBG1p_old() const;
  wire MULE_MODE_DBG1n_old() const;
  wire UNOR_MODE_DBG2p_old() const;
  wire RYCA_MODE_DBG2n_old() const;
  wire TOVA_MODE_DBG2n_old() const;
  wire UPOJ_MODE_PRODn_old() const;

  wire UCOB_CLKBADp_new() const;
  wire ATEZ_CLKBADp_new() const;

  wire UBETp_new() const;
  wire UVARp_new() const;

  wire UMUT_MODE_DBG1p_new() const;
  wire MULE_MODE_DBG1n_new() const;
  wire UNOR_MODE_DBG2p_new() const;
  wire RYCA_MODE_DBG2n_new() const;
  wire TOVA_MODE_DBG2n_new() const;
  wire UPOJ_MODE_PRODn_new() const;

  /*_PIN_71*/ PinIn    PIN_71_RST;
  /*_PIN_73*/ PinOut   PIN_73_CLK_DRIVE;
  /*_PIN_74*/ PinClock PIN_74_CLK;
  /*_PIN_75*/ PinOut   PIN_75_CLK_OUT;
  /*_PIN_76*/ PinIn    PIN_76_T2;
  /*_PIN_77*/ PinIn    PIN_77_T1;
};

//-----------------------------------------------------------------------------

struct PinsLCD {
  void reset();

  /*_PIN_50*/ PinOut PIN_50_LCD_DATA1;
  /*_PIN_51*/ PinOut PIN_51_LCD_DATA0;
  /*_PIN_54*/ PinOut PIN_54_LCD_HSYNC;
  /*_PIN_56*/ PinOut PIN_56_LCD_FLIPS;
  /*_PIN_52*/ PinOut PIN_52_LCD_CNTRL;
  /*_PIN_55*/ PinOut PIN_55_LCD_LATCH;
  /*_PIN_53*/ PinOut PIN_53_LCD_CLOCK;
  /*_PIN_57*/ PinOut PIN_57_LCD_VSYNC;
};

//-----------------------------------------------------------------------------

struct GateBoyPins {

  PinsABusLo      abus_lo;   // 01-08
  PinsABusHi      abus_hi;   // 09-16
  PinsDBus        dbus;      // 17-24
  PinsVramDBus    vram_dbus; // 25-33
  PinsVramABus    vram_abus; // 34-42
  PinsVramControl vram_ctrl; // 43/45/49
  PinsLCD         lcd;       // 50-57
  PinsJoypad      joy;       // 62-67
  //PinsSerial      serial;    // 68-70
  PinsSys         sys;       // 71/76/77
  PinsControl     ctrl;      // 78-80

  void poweron();
  void reset();
};

//-----------------------------------------------------------------------------

#pragma pack(pop)
