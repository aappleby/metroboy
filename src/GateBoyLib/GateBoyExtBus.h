#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct ExtDataLatch {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
  void reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }

  /*#p08.SOMA*/ TpLatch SOMA_EXT_DATA_LATCH_D0n; // AxCxExxx
  /*_p08.RONY*/ TpLatch RONY_EXT_DATA_LATCH_D1n; // AxCxExxx
  /*_p08.RAXY*/ TpLatch RAXY_EXT_DATA_LATCH_D2n; // AxCxExxx
  /*_p08.SELO*/ TpLatch SELO_EXT_DATA_LATCH_D3n; // AxCxExxx
  /*_p08.SODY*/ TpLatch SODY_EXT_DATA_LATCH_D4n; // AxCxExxx
  /*_p08.SAGO*/ TpLatch SAGO_EXT_DATA_LATCH_D5n; // AxCxExxx
  /*_p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_D6n; // AxCxExxx
  /*_p08.SAZY*/ TpLatch SAZY_EXT_DATA_LATCH_D7n; // AxCxExxx
};

//-----------------------------------------------------------------------------

struct ExtAddrLatch {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x0000); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x0000); }
  void reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x004D); }

  /*_p08.ALOR*/ TpLatch ALOR_EXT_ADDR_LATCH_00p; // xBxxxxxx
  /*_p08.APUR*/ TpLatch APUR_EXT_ADDR_LATCH_01p; // xBxxxxxx
  /*_p08.ALYR*/ TpLatch ALYR_EXT_ADDR_LATCH_02p; // xBxxxxxx
  /*_p08.ARET*/ TpLatch ARET_EXT_ADDR_LATCH_03p; // xBxxxxxx
  /*_p08.AVYS*/ TpLatch AVYS_EXT_ADDR_LATCH_04p; // xBxxxxxx
  /*_p08.ATEV*/ TpLatch ATEV_EXT_ADDR_LATCH_05p; // xBxxxxxx
  /*_p08.AROS*/ TpLatch AROS_EXT_ADDR_LATCH_06p; // xBxxxxxx
  /*_p08.ARYM*/ TpLatch ARYM_EXT_ADDR_LATCH_07p; // xBxxxxxx
  /*_p08.LUNO*/ TpLatch LUNO_EXT_ADDR_LATCH_08p; // ABxxxxxx
  /*_p08.LYSA*/ TpLatch LYSA_EXT_ADDR_LATCH_09p; // ABxxxxxx
  /*_p08.PATE*/ TpLatch PATE_EXT_ADDR_LATCH_10p; // ABxxxxxx
  /*_p08.LUMY*/ TpLatch LUMY_EXT_ADDR_LATCH_11p; // ABxxxxxx
  /*_p08.LOBU*/ TpLatch LOBU_EXT_ADDR_LATCH_12p; // ABxxxxxx
  /*_p08.LONU*/ TpLatch LONU_EXT_ADDR_LATCH_13p; // ABxxxxxx
  /*_p08.NYRE*/ TpLatch NYRE_EXT_ADDR_LATCH_14p; // ABxxxxxx
};

//-----------------------------------------------------------------------------

struct ExtControl {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x02); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x02); }
  void reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x02); }

  /*_PIN_80*/ PinOut PIN_80_CSn;      // CS changes on phase C if addr in [A000,FDFF]
  /*_PIN_79*/ PinOut PIN_79_RDn;      // RDn idles low, goes high on phase B for an external write
  /*_PIN_78*/ PinOut PIN_78_WRn;      // WRn idles high, goes low during EFG if there's a write
};

//-----------------------------------------------------------------------------

struct ExtABusLo {
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

struct ExtABusHi {
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

struct ExtDBus {
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

struct GateBoyMBC {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
  void reset_to_cart()    { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }

  Gate MBC1_RAM_EN;
  Gate MBC1_MODE;

  Gate MBC1_BANK0;
  Gate MBC1_BANK1;
  Gate MBC1_BANK2;
  Gate MBC1_BANK3;
  Gate MBC1_BANK4;
  Gate MBC1_BANK5;
  Gate MBC1_BANK6;
};

//-----------------------------------------------------------------------------
