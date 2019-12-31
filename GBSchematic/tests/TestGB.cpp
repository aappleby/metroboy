#include "TestGB.h"

namespace Schematics {

//-----------------------------------------------------------------------------

TestGB::TestGB() {
  pwron();
}

void TestGB::pwron() {
  memset(this, 0, sizeof(*this));
  sys_sig.pwron();

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
  sys_sig.set_rst(false);
  sim(16, fast);
  sys_sig.set_clk_good(true);
  sim(16, fast);
  sys_sig.set_clk_req(true);
  sim(16, fast);
  sys_sig.set_lcdc_en(true);
  sim(456*2*154 - 7, fast);
}

//----------------------------------------
// This should put the GB in the same state as the boot sequence.

void TestGB::reset() {
  pwron();

  sys_sig.reset();

  clk_reg1.reset();
  clk_reg2.reset();
  rst_reg.reset();
  lcd_reg.reset();

  update_signals(false);
}

//-----------------------------------------------------------------------------

void TestGB::update_signals(bool fast) {
  if (fast) {
    clk_sig1.tick_fast(sys_sig, clk_reg1);
    rst_sig1.tick_fast(sys_sig, rst_reg);
    rst_sig2.tick_fast(sys_sig, rst_sig1);
    clk_sig2.tick_fast(sys_sig, rst_sig2, clk_reg2);
    lcd_sig.tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);
  }
  else {
    clk_sig1.tick_slow(sys_sig, clk_reg1);
    rst_sig1.tick_slow(sys_sig, clk_sig1, rst_reg);
    rst_sig2.tick_slow(sys_sig, rst_sig1);
    clk_sig2.tick_slow(sys_sig, rst_sig2, clk_reg2);
    lcd_sig.tick_slow(sys_sig, clk_sig2, rst_sig2, lcd_reg);
  }
}

void TestGB::update_states(bool fast) {
  if (fast) {
    clk_reg1.tock_fast(sys_sig);
    clk_reg2.tock_fast(sys_sig, rst_sig2);
    rst_reg.tock_fast(sys_sig);
    lcd_reg.tock_fast(sys_sig, clk_sig2, rst_sig2);
  }
  else {
    clk_reg1.tock_slow(sys_sig);
    clk_reg2.tock_slow(sys_sig, clk_sig1, clk_sig2, rst_sig2);
    rst_reg.tock_slow(sys_sig, clk_sig1);
    lcd_reg.tock_slow(sys_sig, clk_sig2, rst_sig2, lcd_sig);
  }
}

//-----------------------------------------------------------------------------

void TestGB::check_signals(TestGB& a, TestGB& b) {
  check_byte_match(a.sys_sig,  b.sys_sig);
  check_byte_match(a.clk_sig1, b.clk_sig1);
  check_byte_match(a.clk_sig2, b.clk_sig2);
  check_byte_match(a.rst_sig1, b.rst_sig1);
  check_byte_match(a.rst_sig2, b.rst_sig2);
  check_byte_match(a.lcd_sig,  b.lcd_sig);
}

void TestGB::check_states(TestGB& a, TestGB& b) {
  ClockRegisters1::check_match(a.clk_reg1,  b.clk_reg1);
  ClockRegisters2::check_match(a.clk_reg2,  b.clk_reg2);
  ResetRegisters::check_match(a.rst_reg,  b.rst_reg);
  LCDRegisters::check_match(a.lcd_reg,  b.lcd_reg);
}

void TestGB::check_match(TestGB& a, TestGB& b) {
  check_signals(a, b);
  check_states(a, b);
}

void TestGB::check_clock_phases() {
  clk_sig1.check_phase(sys_sig.phaseC());
  clk_sig2.check_phase(sys_sig.phaseC());
  clk_reg1.check_phase(sys_sig.phaseC());
  clk_reg2.check_phase(sys_sig.phaseC());
}

//-----------------------------------------------------------------------------

void TestGB::sim_slow(int phases) {
  for (int p = 0; p < phases; p++) {
    sys_sig.next_phase();

    // needs 10 passes
    for (int pass = 0; pass < 20; pass++) {
      clk_reg1.tock_slow(sys_sig);
      clk_sig1.tick_slow(sys_sig, clk_reg1);

      rst_reg.tock_slow(sys_sig, clk_sig1);
      rst_sig1.tick_slow(sys_sig, clk_sig1, rst_reg);
      rst_sig2.tick_slow(sys_sig, rst_sig1);

      clk_reg2.tock_slow(sys_sig, clk_sig1, clk_sig2, rst_sig2);
      clk_sig2.tick_slow(sys_sig, rst_sig2, clk_reg2);

      lcd_reg.tock_slow(sys_sig, clk_sig2, rst_sig2, lcd_sig);
      lcd_sig.tick_slow(sys_sig, clk_sig2, rst_sig2, lcd_reg);
    }
  }
}

//-----------------------------------------------------------------------------

void TestGB::sim_fast(int phases) {
  for (int p = 0; p < phases; p++) {
    sys_sig.next_phase();

    clk_reg1.tock_fast(sys_sig);
    clk_sig1.tick_fast(sys_sig, clk_reg1);

    rst_reg.tock_fast(sys_sig);
    rst_sig1.tick_fast(sys_sig, rst_reg);
    rst_sig2.tick_fast(sys_sig, rst_sig1);

    clk_reg2.tock_fast(sys_sig, rst_sig2);
    clk_sig2.tick_fast(sys_sig, rst_sig2, clk_reg2);

    lcd_reg.tock_fast(sys_sig, clk_sig2, rst_sig2);
    lcd_sig.tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);

    clk_reg2.tock_fast(sys_sig, rst_sig2);
    clk_sig2.tick_fast(sys_sig, rst_sig2, clk_reg2);

    lcd_reg.tock_fast(sys_sig, clk_sig2, rst_sig2);
    lcd_sig.tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);

    lcd_reg.tock_fast(sys_sig, clk_sig2, rst_sig2);
    lcd_sig.tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);

    lcd_reg.tock_fast(sys_sig, clk_sig2, rst_sig2);
    lcd_sig.tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);

    lcd_reg.tock_fast(sys_sig, clk_sig2, rst_sig2);
    lcd_sig.tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);

    lcd_reg.tock_fast(sys_sig, clk_sig2, rst_sig2);
    lcd_sig.tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);

    lcd_reg.tock_fast(sys_sig, clk_sig2, rst_sig2);
    lcd_sig.tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);

    lcd_reg.tock_fast(sys_sig, clk_sig2, rst_sig2);
    lcd_sig.tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);

    lcd_reg.tock_fast(sys_sig, clk_sig2, rst_sig2);
    lcd_sig.tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);
  }
}

//-----------------------------------------------------------------------------

};