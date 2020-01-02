#include "TestGB.h"

#if 0
void test_boot_clock_phases() {
  printf("test_boot_clock_phases()\n");

  TestGB gb;
  string ret;

  printf("pwron\n");
  gb.pwron();
  for (int i = 0; i < 16; i++) {
    gb.sim_slow(1);
    //gb.sys_clk.check_phase(gb.sys_reg);
    //gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("!reset\n");
  gb.sys_reg.set_rst(false);
  for (int i = 0; i < 16; i++) {
    gb.sim_slow(1);
    //gb.sys_clk.check_phase(gb.sys_reg);
    //gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("clkgood\n");
  gb.sys_reg.set_clk_good(true);
  gb.sim_slow(8);

  for (int i = 0; i < 8; i++) {
    gb.sim_slow(1);
    //gb.sys_clk.check_phase(gb.sys_reg);
    //gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("clkreq\n");
  gb.sys_reg.set_clk_req(true);
  gb.sim_slow(8);

  for (int i = 0; i < 8; i++) {
    gb.sim_slow(1);
    //gb.sys_clk.check_phase(gb.sys_reg);
    //gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("lcdc_en\n");
  gb.sys_reg.set_lcdc_en(true);
  gb.sim_slow(8);

  for (int i = 0; i < 8; i++) {
    gb.sim_slow(1);
    //gb.sys_clk.check_phase(gb.sys_reg);
    //gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("frame\n");
  for (int i = 0; i < 456*2*154; i++) {
    gb.sim_slow(1);
    //gb.sys_clk.check_phase(gb.sys_reg);
    //gb.clk_sig1().check_phase(gb.sys_reg);
  }

  printf("test_boot_clock_phases() pass\n");
}

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
// After boot, we should be in phase 7 and 10 phases from x=1.

string check_boot_phase_alignment(TestGB gb) {
  //bool ret = true;
  //ret &= check(gb.sys_reg.phase() == 7);

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


#endif