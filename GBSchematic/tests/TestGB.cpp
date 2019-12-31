#include "TestGB.h"

namespace Schematics {

//-----------------------------------------------------------------------------

TestGB::TestGB() {
  pwron();
}

void TestGB::pwron() {
  memset(this, 0, sizeof(*this));
  sys_sig_b.pwron();
  sys_sig_c.pwron();

  clk_reg1.pwron();
  clk_reg2.pwron();
  rst_reg.pwron();
  lcd_reg.pwron();

  update_signals(false);
}

//----------------------------------------
// Boots the chip. Currently no idea if this is actually what the boot
// sequence really looks like, but it's vaguely plausible.

void TestGB::boot(bool fast) {
  sim(16, fast);
  sys_sig_c.set_rst(false);
  sim(16, fast);
  sys_sig_c.set_clk_good(true);
  sim(16, fast);
  sys_sig_c.set_clk_req(true);
  sim(16, fast);
  sys_sig_c.set_lcdc_en(true);
  sim(456*2*154 - 7, fast);
}

//----------------------------------------
// This should put the GB in the same state as the boot sequence.

void TestGB::reset() {
  pwron();

  sys_sig_b.reset();
  sys_sig_c.reset();

  clk_reg1.reset();
  clk_reg2.reset();
  rst_reg.reset();
  lcd_reg.reset();

  update_signals(false);
}

//----------------------------------------

void TestGB::update_signals(bool fast) {
  if (fast) {
    clk_sig1.tick_fast(sys_sig_c, clk_reg1);
    rst_sig1.tick_slow(sys_sig_c, clk_sig1, rst_reg);
    rst_sig2.tick_slow(sys_sig_c, rst_sig1);
    clk_sig2.tick_slow(sys_sig_c, rst_sig2, clk_reg2);
    lcd_sig.tick_slow(sys_sig_c, clk_sig2, rst_sig2, lcd_reg);
  }
  else {
    clk_sig1.tick_slow(sys_sig_c, clk_reg1);
    rst_sig1.tick_slow(sys_sig_c, clk_sig1, rst_reg);
    rst_sig2.tick_slow(sys_sig_c, rst_sig1);
    clk_sig2.tick_slow(sys_sig_c, rst_sig2, clk_reg2);
    lcd_sig.tick_slow(sys_sig_c, clk_sig2, rst_sig2, lcd_reg);
  }
}

//-----------------------------------------------------------------------------

void TestGB::check_signals(const TestGB& a, const TestGB& b) {
  check_byte_match(a.sys_sig_b, b.sys_sig_b);
  check_byte_match(a.sys_sig_c, b.sys_sig_c);
  check_byte_match(a.clk_sig1,  b.clk_sig1);
  check_byte_match(a.clk_sig2,  b.clk_sig2);
  check_byte_match(a.rst_sig1,  b.rst_sig1);
  check_byte_match(a.rst_sig2,  b.rst_sig2);
  check_byte_match(a.lcd_sig,   b.lcd_sig);
}

void TestGB::check_states(const TestGB& a, const TestGB& b) {
  ClockRegisters1::check_match(a.clk_reg1,  b.clk_reg1);
  ClockRegisters2::check_match(a.clk_reg2,  b.clk_reg2);
  ResetRegisters::check_match(a.rst_reg,  b.rst_reg);
  LCDRegisters::check_match(a.lcd_reg,  b.lcd_reg);
}

void TestGB::check_match(const TestGB& a, const TestGB& b) {
  check_signals(a, b);
  check_states(a, b);
}

//-----------------------------------------------------------------------------

void TestGB::sim_slow(int phases) {
  for (int p = 0; p < phases; p++) {
    sys_sig_c.next_phase();

    for (int pass = 0; pass < 15; pass++) {
        
      clk_sig1.tick_slow(sys_sig_c, clk_reg1);
      rst_sig1.tick_slow(sys_sig_c, clk_sig1, rst_reg);
      rst_sig2.tick_slow(sys_sig_c, rst_sig1);
      clk_sig2.tick_slow(sys_sig_c, rst_sig2, clk_reg2);
      lcd_sig.tick_slow(sys_sig_c, clk_sig2, rst_sig2, lcd_reg);

      //----------
      TestGB prev = *this;

      clk_reg1.tock_slow(sys_sig_c, clk_sig1);
      clk_reg2.tock_slow(sys_sig_c, clk_sig1, clk_sig2, rst_sig2);
      rst_reg.tock_slow(sys_sig_c, clk_sig1, prev.rst_reg);
      lcd_reg.tock_slow(sys_sig_c, clk_sig2, rst_sig2, lcd_sig, prev.lcd_reg);

      sys_sig_b = sys_sig_c;
    }
  }
}

//-----------------------------------------------------------------------------

void TestGB::sim_fast(int phases) {
  for (int p = 0; p < phases; p++) {
    sys_sig_c.next_phase();

    for (int pass = 0; pass < 15; pass++) {
        
      clk_sig1.tick_fast(sys_sig_c, clk_reg1);
      rst_sig1.tick_slow(sys_sig_c, clk_sig1, rst_reg);
      rst_sig2.tick_slow(sys_sig_c, rst_sig1);
      clk_sig2.tick_slow(sys_sig_c, rst_sig2, clk_reg2);
      lcd_sig.tick_slow(sys_sig_c, clk_sig2, rst_sig2, lcd_reg);

      //----------
      TestGB prev = *this;

      clk_reg1.tock_slow(sys_sig_c, clk_sig1);
      clk_reg2.tock_slow(sys_sig_c, clk_sig1, clk_sig2, rst_sig2);
      rst_reg.tock_slow(sys_sig_c, clk_sig1, prev.rst_reg);
      lcd_reg.tock_slow(sys_sig_c, clk_sig2, rst_sig2, lcd_sig, prev.lcd_reg);

      sys_sig_b = sys_sig_c;
    }

    /*
    clk_reg1.tock_fast(sys_sig_c);
    clk_sig1.tick_fast(sys_sig_c);

    reset_fast(sys_sig_b, sys_sig_c, rst_reg, rst_sig1, rst_sig2);

    clk_reg2.tock_fast(sys_sig_c, rst_sig2);
    clk_sig2.tick_fast(sys_sig_c, rst_sig2, clk_reg2);
    clk_reg2.tock_fast(sys_sig_c, rst_sig2);

    lcd_reg.tock_fast(sys_sig_c, clk_sig2, rst_sig2);

    clk_sig2.tick_fast(sys_sig_c, rst_sig2, clk_reg2);

    lcd_reg.tock_fast2(sys_sig_c, clk_sig2, rst_sig2);
    
    lcd_sig.tick_fast(sys_sig_c, clk_sig2, rst_sig2, lcd_reg);

    sys_sig_b = sys_sig_c;
    */
  }
}

//-----------------------------------------------------------------------------

};