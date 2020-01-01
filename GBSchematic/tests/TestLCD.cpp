#include "TestGB.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;

using namespace Schematics;

//-----------------------------------------------------------------------------
// After boot, we should be in phase 7 and 10 phases from x=1.

string check_boot_phase_alignment(TestGB gb) {
  bool ret = true;
  ret &= check(gb.sys_reg.phaseC() == 7);

  //check(gb.lcd_reg.x() == 0);
  //check(gb.lcd_reg.y() == 0);

  gb.sim_slow(9);

  //check(gb.lcd_reg.x() == 0);
  //check(gb.lcd_reg.y() == 0);

  gb.sim_slow(1);

  //check(gb.lcd_reg.x() == 1);
  //check(gb.lcd_reg.y() == 0);

  return {};
}

//-----------------------------------------------------------------------------

template<typename T>
string check_byte_match(const T& a, const T& b) {
  uint8_t* blobA = (uint8_t*)&a;
  uint8_t* blobB = (uint8_t*)&b;

  for (int i = 0; i < sizeof(T); i++) {
    if (blobA[i] != blobB[i]) {
      return errprintf("Mismatch at %d - 0x%02x 0x%02x\n", i, blobA[i], blobB[i]);
    }
  }
  return {};
}

//-----------------------------------------------------------------------------

string check_states(TestGB& a, TestGB& b) {
  if (a.clk_reg1.PHAZ_ABCDxxxx != b.clk_reg1.PHAZ_ABCDxxxx) return "PHAZ_ABCDxxxx mismatch\n";
  if (a.clk_reg1.PHAZ_xBCDExxx != b.clk_reg1.PHAZ_xBCDExxx) return "PHAZ_xBCDExxx mismatch\n";
  if (a.clk_reg1.PHAZ_xxCDEFxx != b.clk_reg1.PHAZ_xxCDEFxx) return "PHAZ_xxCDEFxx mismatch\n";
  if (a.clk_reg1.PHAZ_xxxDEFGx != b.clk_reg1.PHAZ_xxxDEFGx) return "PHAZ_xxxDEFGx mismatch\n";

  if (a.rst_reg.WAITING_FOR_CLKREQ != b.rst_reg.WAITING_FOR_CLKREQ) return "WAITING_FOR_CLKREQ mismatch\n";
  if (a.rst_reg.RESET_REG != b.rst_reg.RESET_REG) return "RESET_REG mismatch\n";

  if (a.clk_reg2.WUVU_AxxDExxH != b.clk_reg2.WUVU_AxxDExxH) return "WUVU_AxxDExxH mismatch\n";
  if (a.clk_reg2.VENA_xBCDExxx != b.clk_reg2.VENA_xBCDExxx) return "VENA_xBCDExxx mismatch\n";
  if (a.clk_reg2.WOSU_xxCDxxGH != b.clk_reg2.WOSU_xxCDxxGH) return "WOSU_xxCDxxGH mismatch\n";

  return {};
}

//-----------------------------------------------------------------------------
// only call this if LCDC_EN is high and clocks are stable.

string check_clock_phases(TestGB& gb) {
  gb.clk_reg1.check_phase(gb.sys_reg);

  ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(gb.sys_reg, gb.clk_reg1);
  clk_sig1.check_phase(gb.sys_reg);

  //ResetSignals1 rst_sig1 = ResetSignals1::tick_fast(sys_reg, rst_reg);
  //ResetSignals2 rst_sig2 = ResetSignals2::tick_fast(sys_reg, rst_sig1);
  //ClockSignals2 clk_sig2 = ClockSignals2::tick_fast(sys_reg, rst_sig2, clk_reg2);
  //LCDSignals lcd_sig = LCDSignals::tick_slow(sys_reg, clk_sig2, rst_sig2, lcd_reg);

  //clk_reg2.check_phase(sys_reg.phaseC());
  //clk_sig2.check_phase(sys_reg.phaseC());

  return "sdf";
}

//-----------------------------------------------------------------------------

namespace Schematics {
  void check_phase_name(int phase, const bool val, char* name);
};

void test_boot_clock_phases() {
  printf("test_boot_clock_phases()\n");

  TestGB gb;
  string ret;

  printf("pwron\n");
  gb.pwron();
  for (int i = 0; i < 16; i++) {
    gb.sim_slow(1);
    gb.clk_reg1.check_phase(gb.sys_reg);
    gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("!reset\n");
  gb.sys_reg.set_rst(false);
  for (int i = 0; i < 16; i++) {
    gb.sim_slow(1);
    gb.clk_reg1.check_phase(gb.sys_reg);
    gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("clkgood\n");
  gb.sys_reg.set_clk_good(true);
  gb.sim_slow(8);

  for (int i = 0; i < 8; i++) {
    gb.sim_slow(1);
    gb.clk_reg1.check_phase(gb.sys_reg);
    gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("clkreq\n");
  gb.sys_reg.set_clk_req(true);
  gb.sim_slow(8);

  for (int i = 0; i < 8; i++) {
    gb.sim_slow(1);
    gb.clk_reg1.check_phase(gb.sys_reg);
    gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("lcdc_en\n");
  gb.sys_reg.set_lcdc_en(true);
  gb.sim_slow(8);

  for (int i = 0; i < 8; i++) {
    gb.sim_slow(1);
    gb.clk_reg1.check_phase(gb.sys_reg);
    gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("frame\n");
  for (int i = 0; i < 456*2*154; i++) {
    gb.sim_slow(1);
    gb.clk_reg1.check_phase(gb.sys_reg);
    gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("test_boot_clock_phases() pass\n");
}

/*


  cout << "boot_phase_alignment gb1 ";
  ret = check_boot_phase_alignment(gb);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }
}
*/

//-----------------------------------------------------------------------------

/*
void test_reset() {
  TestGB gb3;
  gb3.reset();

  cout << "reset() ";
  ret = check_states(gb1, gb3);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }
}
*/

//-----------------------------------------------------------------------------

void TestLCD() {
  printf("TestGB size %zd\n\n", sizeof(TestGB));

  test_boot_clock_phases();
  //test_fast_slow();
}

//-----------------------------------------------------------------------------


