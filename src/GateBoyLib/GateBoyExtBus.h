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
