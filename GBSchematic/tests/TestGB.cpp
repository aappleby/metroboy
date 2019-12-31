#include "TestGB.h"

namespace Schematics {

//-----------------------------------------------------------------------------

TestGB::TestGB() {
  pwron();
}

void TestGB::pwron() {
  memset(this, 0, sizeof(*this));
  sys_reg.pwron();

  clk_reg1.pwron();
  clk_reg2.pwron();
  rst_reg.pwron();
  //lcd_reg.pwron();
}

//----------------------------------------
// Boots the chip. Currently no idea if this is actually what the boot
// sequence really looks like, but it's vaguely plausible.

void TestGB::boot(bool fast) {
  sim(16, fast);
  sys_reg.set_rst(false);
  sim(16, fast);
  sys_reg.set_clk_good(true);
  sim(16, fast);
  sys_reg.set_clk_req(true);
  sim(16, fast);
  sys_reg.set_lcdc_en(true);
  sim(456*2*154 - 7, fast);
}

//----------------------------------------
// This should put the GB in the same state as the boot sequence.

void TestGB::reset() {
  pwron();

  sys_reg.reset();

  clk_reg1.reset();
  clk_reg2.reset();
  rst_reg.reset();
  //lcd_reg.reset();
}

//-----------------------------------------------------------------------------

void TestGB::check_signals(TestGB& gb) {
  ClockSignals1 slow_clk_sig1 = ClockSignals1::tick_slow(gb.sys_reg, gb.clk_reg1);
  ResetSignals1 slow_rst_sig1 = ResetSignals1::tick_slow(gb.sys_reg, slow_clk_sig1, gb.rst_reg);
  ResetSignals2 slow_rst_sig2 = ResetSignals2::tick_slow(gb.sys_reg, slow_rst_sig1);
  ClockSignals2 slow_clk_sig2 = ClockSignals2::tick_slow(gb.sys_reg, slow_rst_sig2, gb.clk_reg2);
  //LCDSignals    slow_lcd_sig     = LCDSignals::tick_slow(gb.sys_reg, slow_clk_sig2, slow_rst_sig2, gb.lcd_reg);

  ClockSignals1 fast_clk_sig1 = ClockSignals1::tick_fast(gb.sys_reg, gb.clk_reg1);
  ResetSignals1 fast_rst_sig1 = ResetSignals1::tick_fast(gb.sys_reg, gb.rst_reg);
  ResetSignals2 fast_rst_sig2 = ResetSignals2::tick_fast(gb.sys_reg, fast_rst_sig1);
  ClockSignals2 fast_clk_sig2 = ClockSignals2::tick_fast(gb.sys_reg, fast_rst_sig2, gb.clk_reg2);
  //LCDSignals    fast_lcd_sig     = LCDSignals::tick_fast(gb.sys_reg, fast_clk_sig2, fast_rst_sig2, gb.lcd_reg);

  check_byte_match(slow_clk_sig1, fast_clk_sig1);
  check_byte_match(slow_rst_sig1, fast_rst_sig1);
  check_byte_match(slow_rst_sig2, fast_rst_sig2);
  check_byte_match(slow_clk_sig2, fast_clk_sig2);
  //check_byte_match(slow_lcd_sig , fast_lcd_sig );
}

void TestGB::check_states(TestGB& a, TestGB& b) {
  ClockRegisters1::check_match(a.clk_reg1,  b.clk_reg1);
  ClockRegisters2::check_match(a.clk_reg2,  b.clk_reg2);
  ResetRegisters::check_match(a.rst_reg,  b.rst_reg);
  //LCDRegisters::check_match(a.lcd_reg,  b.lcd_reg);
}

void TestGB::check_match(TestGB& a, TestGB& b) {
  check_states(a, b);
  check_signals(a);
}

void TestGB::check_clock_phases() {
  ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(sys_reg, clk_reg1);
  ResetSignals1 rst_sig1 = ResetSignals1::tick_fast(sys_reg, rst_reg);
  ResetSignals2 rst_sig2 = ResetSignals2::tick_fast(sys_reg, rst_sig1);
  ClockSignals2 clk_sig2 = ClockSignals2::tick_fast(sys_reg, rst_sig2, clk_reg2);
  //LCDSignals lcd_sig = LCDSignals::tick_slow(sys_reg, clk_sig2, rst_sig2, lcd_reg);

  clk_sig1.check_phase(sys_reg.phaseC());
  clk_sig2.check_phase(sys_reg.phaseC());
  clk_reg1.check_phase(sys_reg.phaseC());
  clk_reg2.check_phase(sys_reg.phaseC());
}

//-----------------------------------------------------------------------------

void TestGB::sim_slow(int phases) {
  for (int p = 0; p < phases; p++) {
    sys_reg.next_phase();

    // needs 10 passes
    for (int pass = 0; pass < 20; pass++) {

      clk_reg1.tick_slow(sys_reg);
      ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(sys_reg, clk_reg1);

      rst_reg.tock_slow(sys_reg, clk_sig1);
      ResetSignals1 rst_sig1 = ResetSignals1::tick_slow(sys_reg, clk_sig1, rst_reg);
      ResetSignals2 rst_sig2 = ResetSignals2::tick_slow(sys_reg, rst_sig1);


      clk_reg2.tock_slow(clk_sig1, rst_sig2);
      //ClockSignals2 clk_sig2 = ClockSignals2::tick_slow(sys_reg, rst_sig2, clk_reg2);
      
      //LCDSignals lcd_sig = LCDSignals::tick_slow(sys_reg, clk_sig2, rst_sig2, lcd_reg);
      //lcd_reg.tock_slow(sys_reg, clk_sig2, rst_sig2, lcd_sig);

      clk_reg1.commit();
      clk_reg2.commit();
    }
  }
}

//-----------------------------------------------------------------------------

void TestGB::sim_fast(int phases) {
  for (int p = 0; p < phases; p++) {
    sys_reg.next_phase();

    clk_reg1.tick_fast(sys_reg);
    ClockSignals1 clk_sig1 = ClockSignals1::tick_fast(sys_reg, clk_reg1);

    rst_reg.tock_fast(sys_reg);
    ResetSignals1 rst_sig1 = ResetSignals1::tick_fast(sys_reg, rst_reg);
    ResetSignals2 rst_sig2 = ResetSignals2::tick_fast(sys_reg, rst_sig1);

    clk_reg2.tock_fast(clk_sig1, rst_sig2);

    //ClockSignals2 clk_sig2A = ClockSignals2::tick_fast(sys_reg, rst_sig2, clk_reg2);

    //lcd_reg.tock_fast(sys_reg, clk_sig2A, rst_sig2);


    //ClockSignals2 clk_sig2B = ClockSignals2::tick_fast(sys_reg, rst_sig2, clk_reg2);

    /*
    lcd_reg.tock_fast(sys_reg, clk_sig2B, rst_sig2);

    lcd_reg.tock_fast(sys_reg, clk_sig2B, rst_sig2);

    lcd_reg.tock_fast(sys_reg, clk_sig2B, rst_sig2);

    lcd_reg.tock_fast(sys_reg, clk_sig2B, rst_sig2);

    lcd_reg.tock_fast(sys_reg, clk_sig2B, rst_sig2);

    lcd_reg.tock_fast(sys_reg, clk_sig2B, rst_sig2);

    lcd_reg.tock_fast(sys_reg, clk_sig2B, rst_sig2);

    lcd_reg.tock_fast(sys_reg, clk_sig2B, rst_sig2);
    */

    clk_reg1.commit();
    clk_reg2.commit();
  }
}

//-----------------------------------------------------------------------------

};