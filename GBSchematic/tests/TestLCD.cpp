#include "TestGB.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void sim_fast_slow(TestGB& gb1, TestGB& gb2, int phases, bool check_phases) {
  for (int i = 0; i < phases; i++) {
    gb1.sim_slow(1);
    gb2.sim_fast(1);
    TestGB::check_match(gb1, gb2);
    if (check_phases) gb1.check_clock_phases();
  }
}

//----------------------------------------
// After boot, we should be in phase 7 and 10 phases from x=1.

void check_boot_phase_alignment(TestGB gb) {
  check(gb.sys_reg.phaseC() == 7);

  //check(gb.lcd_reg.x() == 0);
  //check(gb.lcd_reg.y() == 0);

  gb.sim_slow(9);

  //check(gb.lcd_reg.x() == 0);
  //check(gb.lcd_reg.y() == 0);

  gb.sim_slow(1);

  //check(gb.lcd_reg.x() == 1);
  //check(gb.lcd_reg.y() == 0);
}

//----------------------------------------
// Slow boot, fast boot, and reset should produce matching states

void test_reset() {
  printf("test_reset: ");

  TestGB gb1;
  TestGB gb2;

  gb1.pwron();
  gb2.pwron();
  sim_fast_slow(gb1, gb2, 16, false);

  gb1.sys_reg.set_rst(false);
  gb2.sys_reg.set_rst(false);
  sim_fast_slow(gb1, gb2, 16, false);

  gb1.sys_reg.set_clk_good(true);
  gb2.sys_reg.set_clk_good(true);
  sim_fast_slow(gb1, gb2, 16, false);

  gb1.sys_reg.set_clk_req(true);
  gb2.sys_reg.set_clk_req(true);
  sim_fast_slow(gb1, gb2, 16, false);

  gb1.sys_reg.set_lcdc_en(true);
  gb2.sys_reg.set_lcdc_en(true);
  sim_fast_slow(gb1, gb2, 16, false);

  // video clocks are stable, run a frame with clock phase checking on.
  sim_fast_slow(gb1, gb2, 456*2*154 - 23, true);
  check_boot_phase_alignment(gb1);
  check_boot_phase_alignment(gb2);

  TestGB gb3;
  gb3.reset();
  TestGB::check_match(gb2, gb3);

  printf("pass\n");
}

//-----------------------------------------------------------------------------

void TestLCD() {

  printf("\n");
  printf("TestGB size %zd\n", sizeof(TestGB));
  test_reset();
  printf("\n");

  return;
}

//-----------------------------------------------------------------------------


