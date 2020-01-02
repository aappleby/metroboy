#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Bus {

  void pwron() { *this = {}; }
  void reset() { *this = {}; }

  bool A00;
  bool A01;
  bool A02;
  bool A03;
  bool A04;
  bool A05;
  bool A06;
  bool A07;
  bool A08;
  bool A09;
  bool A10;
  bool A11;
  bool A12;
  bool A13;
  bool A14;
  bool A15;

  bool D0;
  bool D1;
  bool D2;
  bool D3;
  bool D4;
  bool D5;
  bool D6;
  bool D7;
};

struct BusControl {

  static BusControl tick(const SystemRegisters& sys_reg, const ClockSignals1& clk_sig1);

  /*p07.TEDO*/ bool TEDO_CPURD;
  /*p07.TAPU*/ bool TAPU_CPUWR; //p4-p6
  /*p07.ASOT*/ bool ASOT_CPURD;
  /*p07.CUPA*/ bool CUPA_CPUWR; //p4-p6

  /*p08.TEXO*/ bool ADDR_VALID_AND_NOT_VRAM;
  /*p08.LEVO*/ bool ADDR_VALID_AND_NOT_VRAMn;

#if 0
  /*p08.RORU*/ bool CBUS_TO_CEXTn;
  /*p08.LULA*/ bool CBUS_TO_CEXT;

  /*p25.RAHU*/ bool CBUS_TO_VBUSn;
  /*p25.RENA*/ bool VEXT_TO_VBUSn;

  /*p25.SERE*/ bool SERE;

  /*p25.TYVY*/ bool MD_TO_D;
  /*p25.SEBY*/ bool MD_TO_Dn;

  /*p08.ALOR*/ bool ADDR_LATCH_00;
  /*p08.APUR*/ bool ADDR_LATCH_01;
  /*p08.ALYR*/ bool ADDR_LATCH_02;
  /*p08.ARET*/ bool ADDR_LATCH_03;
  /*p08.AVYS*/ bool ADDR_LATCH_04;
  /*p08.ATEV*/ bool ADDR_LATCH_05;
  /*p08.AROS*/ bool ADDR_LATCH_06;
  /*p08.ARYM*/ bool ADDR_LATCH_07;
  /*p08.LUNO*/ bool ADDR_LATCH_08;
  /*p08.LYSA*/ bool ADDR_LATCH_09;
  /*p08.PATE*/ bool ADDR_LATCH_10;
  /*p08.LUMY*/ bool ADDR_LATCH_11;
  /*p08.LOBU*/ bool ADDR_LATCH_12;
  /*p08.LONU*/ bool ADDR_LATCH_13;
  /*p08.NYRE*/ bool ADDR_LATCH_14;
  /*p08.SEPY*/ bool ADDR_LATCH_15;

  /*p08.AMET*/ bool ADDR_OUT_00;
  /*p08.ATOL*/ bool ADDR_OUT_01;
  /*p08.APOK*/ bool ADDR_OUT_02;
  /*p08.AMER*/ bool ADDR_OUT_03;
  /*p08.ATEM*/ bool ADDR_OUT_04;
  /*p08.ATOV*/ bool ADDR_OUT_05;
  /*p08.ATYR*/ bool ADDR_OUT_06;
  /*p08.ASUR*/ bool ADDR_OUT_07;
  /*p08.MANO*/ bool ADDR_OUT_08;
  /*p08.MASU*/ bool ADDR_OUT_09;
  /*p08.PAMY*/ bool ADDR_OUT_10;
  /*p08.MALE*/ bool ADDR_OUT_11;
  /*p08.MOJY*/ bool ADDR_OUT_12;
  /*p08.MUCE*/ bool ADDR_OUT_13;
  /*p08.PEGE*/ bool ADDR_OUT_14;
  /*p08.TAZY*/ bool ADDR_OUT_15;

  /*p08.SOMA*/ bool LATCH_D0;
  /*p08.RONY*/ bool LATCH_D1;
  /*p08.RAXY*/ bool LATCH_D2;
  /*p08.SELO*/ bool LATCH_D3;
  /*p08.SODY*/ bool LATCH_D4;
  /*p08.SAGO*/ bool LATCH_D5;
  /*p08.RUPA*/ bool LATCH_D6;
  /*p08.SAZY*/ bool LATCH_D7;

#endif
};

//-----------------------------------------------------------------------------

};