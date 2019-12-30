#include "TestGB.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void sim_fast_slow(TestGB& gb1, TestGB& gb2, int phases) {
  for (int i = 0; i < phases; i++) {
    gb1.sim_slow(1);
    gb2.sim_fast(1);
    TestGB::check_match(gb1, gb2);
  }
}

//----------------------------------------
// After boot, we should be in phase 7 and 10 phases from x=1.

void check_boot_phase_alignment(TestGB gb) {
  check(gb.sys_sig.phaseC() == 7);

  check(gb.lcd_reg.x() == 0);
  check(gb.lcd_reg.y() == 0);

  gb.sim_slow(9);

  check(gb.lcd_reg.x() == 0);
  check(gb.lcd_reg.y() == 0);

  gb.sim_slow(1);

  check(gb.lcd_reg.x() == 1);
  check(gb.lcd_reg.y() == 0);
}

//----------------------------------------
// Slow boot, fast boot, and reset should produce matching states

void test_reset() {
  printf("test_reset: ");

  TestGB gb1;
  TestGB gb2;

  sim_fast_slow(gb1, gb2, 16);

  gb1.sys_sig.set_rst(false);
  gb2.sys_sig.set_rst(false);
  sim_fast_slow(gb1, gb2, 16);

  gb1.sys_sig.set_clk_good(true);
  gb2.sys_sig.set_clk_good(true);
  sim_fast_slow(gb1, gb2, 16);

  gb1.sys_sig.set_clk_req(true);
  gb2.sys_sig.set_clk_req(true);
  sim_fast_slow(gb1, gb2, 16);

  gb1.sys_sig.set_lcdc_en(true);
  gb2.sys_sig.set_lcdc_en(true);

  sim_fast_slow(gb1, gb2, 456*2*154 - 7);
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


