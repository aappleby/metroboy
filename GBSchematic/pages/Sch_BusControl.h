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

  void commit() {
    addr_write_count = 0;
    data_write_count = 0;
  }

  void set_addr(int addr) {
    if (addr_write_count) {
      printf("DATA BUS COLLISION\n");
    }
    A00_ = addr & 0x0001; A01_ = addr & 0x0002; A02_ = addr & 0x0004; A03_ = addr & 0x0008;
    A04_ = addr & 0x0010; A05_ = addr & 0x0020; A06_ = addr & 0x0040; A07_ = addr & 0x0080;
    A08_ = addr & 0x0100; A09_ = addr & 0x0200; A10_ = addr & 0x0400; A11_ = addr & 0x0800;
    A12_ = addr & 0x1000; A13_ = addr & 0x2000; A14_ = addr & 0x4000; A15_ = addr & 0x8000;
  }

  void set_data(int data) {
    set_data(data & 0x01, data & 0x02, data & 0x04, data & 0x08,
             data & 0x10, data & 0x20, data & 0x40, data & 0x80);
  }

  int get_addr() const {
    return pack(A00_, A01_, A02_, A03_, A04_, A05_, A06_, A07_,
                A08_, A09_, A10_, A11_, A12_, A13_, A14_, A15_);
  }

  int get_data() const {
    return pack(D0_, D1_, D2_, D3_, D4_, D5_, D6_, D7_);
  }

  void set_addr(bool a00=0, bool a01=0, bool a02=0, bool a03=0, bool a04=0, bool a05=0, bool a06=0, bool a07=0,
                bool a08=0, bool a09=0, bool a10=0, bool a11=0, bool a12=0, bool a13=0, bool a14=0, bool a15=0) {
    if (addr_write_count) {
      printf("DATA BUS COLLISION\n");
    }
    A00_ = a00; A01_ = a01; A02_ = a02; A03_ = a03; A04_ = a04; A05_ = a05; A06_ = a06; A07_ = a07;
    A08_ = a08; A09_ = a09; A10_ = a10; A11_ = a11; A12_ = a12; A13_ = a13; A14_ = a14; A15_ = a15;
    addr_write_count++;
  }

  void set_data(bool d0, bool d1=0, bool d2=0, bool d3=0, bool d4=0, bool d5=0, bool d6=0, bool d7=0) {
    if (data_write_count) {
      printf("DATA BUS COLLISION\n");
    }
    D0_ = d0; D1_ = d1; D2_ = d2; D3_ = d3; D4_ = d4; D5_ = d5; D6_ = d6; D7_ = d7;
    data_write_count++;
  }

  bool A00() const { return A00_; }
  bool A01() const { return A01_; }
  bool A02() const { return A02_; }
  bool A03() const { return A03_; }
  bool A04() const { return A04_; }
  bool A05() const { return A05_; }
  bool A06() const { return A06_; }
  bool A07() const { return A07_; }
  bool A08() const { return A08_; }
  bool A09() const { return A09_; }
  bool A10() const { return A10_; }
  bool A11() const { return A11_; }
  bool A12() const { return A12_; }
  bool A13() const { return A13_; }
  bool A14() const { return A14_; }
  bool A15() const { return A15_; }

  bool D0() const { return D0_; }
  bool D1() const { return D1_; }
  bool D2() const { return D2_; }
  bool D3() const { return D3_; }
  bool D4() const { return D4_; }
  bool D5() const { return D5_; }
  bool D6() const { return D6_; }
  bool D7() const { return D7_; }

private:

  int addr_write_count;
  int data_write_count;

  bool A00_;
  bool A01_;
  bool A02_;
  bool A03_;
  bool A04_;
  bool A05_;
  bool A06_;
  bool A07_;
  bool A08_;
  bool A09_;
  bool A10_;
  bool A11_;
  bool A12_;
  bool A13_;
  bool A14_;
  bool A15_;

  bool D0_;
  bool D1_;
  bool D2_;
  bool D3_;
  bool D4_;
  bool D5_;
  bool D6_;
  bool D7_;
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