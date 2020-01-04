#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct BusTristates {

  void pwron() {
    *this = {};
  }
  
  void reset() {
    *this = {};
  }

  void phase_begin() {
    addr_write_count = 0;
    data_write_count = 0;
  }

  void phase_end() {
  }

  void pass_begin() {
  }

  bool pass_end() {
    bool ret = changed;
    changed = false;
    return ret;
  }


  int get_addr() const {
    return addr;
  }

  int get_data() const {
    return data;
  }

  void set_addr(uint16_t new_addr) {
    if (addr_write_count) {
      printf("ADDR BUS COLLISION\n");
    }
    if (addr != new_addr) {
      addr = new_addr;
      changed = true;
    }
    addr_write_count++;
  }

  void set_data(uint8_t new_data) {
    if (data_write_count) {
      printf("DATA BUS COLLISION\n");
    }
    if (data != new_data) {
      data = new_data;
      changed = true;
    }
    data_write_count++;
  }

  void set_addr(bool a00=0, bool a01=0, bool a02=0, bool a03=0, bool a04=0, bool a05=0, bool a06=0, bool a07=0,
                bool a08=0, bool a09=0, bool a10=0, bool a11=0, bool a12=0, bool a13=0, bool a14=0, bool a15=0) {
    return set_addr((uint16_t)pack(a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, a10, a11, a12, a13, a14, a15));
  }

  void set_data(bool d0, bool d1=0, bool d2=0, bool d3=0, bool d4=0, bool d5=0, bool d6=0, bool d7=0) {
    return set_data((uint8_t)pack(d0, d1, d2, d3, d4, d5, d6, d7));
  }

  bool A00() const { return (addr & 0x0001); }
  bool A01() const { return (addr & 0x0002); }
  bool A02() const { return (addr & 0x0004); }
  bool A03() const { return (addr & 0x0008); }
  bool A04() const { return (addr & 0x0010); }
  bool A05() const { return (addr & 0x0020); }
  bool A06() const { return (addr & 0x0040); }
  bool A07() const { return (addr & 0x0080); }
  bool A08() const { return (addr & 0x0100); }
  bool A09() const { return (addr & 0x0200); }
  bool A10() const { return (addr & 0x0400); }
  bool A11() const { return (addr & 0x0800); }
  bool A12() const { return (addr & 0x1000); }
  bool A13() const { return (addr & 0x2000); }
  bool A14() const { return (addr & 0x4000); }
  bool A15() const { return (addr & 0x8000); }

  bool D0()  const { return (data & 0x0001); }
  bool D1()  const { return (data & 0x0002); }
  bool D2()  const { return (data & 0x0004); }
  bool D3()  const { return (data & 0x0008); }
  bool D4()  const { return (data & 0x0010); }
  bool D5()  const { return (data & 0x0020); }
  bool D6()  const { return (data & 0x0040); }
  bool D7()  const { return (data & 0x0080); }

private:

  int addr_write_count;
  int data_write_count;

  uint16_t addr;
  uint8_t data;
  bool changed;
};

struct BusSignals {

  static BusSignals tick(const SysSignals& sys_sig, const ClkSignals& clk_sig1);

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