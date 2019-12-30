#include "TestGB.h"

namespace Schematics {

//-----------------------------------------------------------------------------

TestGB::TestGB() {
  pwron();
}

void TestGB::pwron() {
  memset(this, 0, sizeof(*this));
  sys_sig.pwron();
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
  clk_sig1.reset();
  clk_sig2.reset();
  clk_reg.reset();

  rst_sig1.reset();
  rst_sig2.reset();
  rst_reg.reset();

  lcd_sig.reset();
  lcd_reg.reset();
}

//----------------------------------------

void TestGB::sim_slow(int phases) {
  for (int p = 0; p < phases; p++) {
    sys_sig.next_phase();

    for (int pass = 0; pass < 20; pass++) {
        
      clk_sig1 = ClockSignals1::tick_slow(sys_sig, clk_reg);
      rst_sig1 = ResetSignals1::tick_slow(sys_sig, clk_sig1, rst_reg);
      rst_sig2 = ResetSignals2::tick_slow(sys_sig, rst_sig1, rst_reg);
      clk_sig2 = ClockSignals2::tick_slow(rst_sig2, clk_reg);
      lcd_sig  = LCDSignals::tick_slow(sys_sig, clk_sig2, rst_sig2, lcd_reg);

      //----------
      TestGB prev = *this;

      ClockRegisters::tock_slow1(sys_sig, clk_sig1, clk_reg);
      ClockRegisters::tock_slow2(sys_sig, clk_sig1, clk_sig2, rst_sig2, clk_reg);
      ResetRegisters::tock_slow(sys_sig, clk_sig1, prev.rst_reg, rst_reg);
      LCDRegisters::tock_slow(sys_sig, clk_sig2, rst_sig2, lcd_sig, prev.lcd_reg, lcd_reg);
    }
  }
}

void TestGB::sim_fast(int phases) {
  for (int p = 0; p < phases; p++) {
    sys_sig.next_phase();

    for (int pass = 0; pass < 20; pass++) {
      clk_sig1 = ClockSignals1::tick_fast(sys_sig, clk_reg);
      rst_sig1 = ResetSignals1::tick_fast(sys_sig, clk_sig1, rst_reg);
      rst_sig2 = ResetSignals2::tick_fast(sys_sig, rst_sig1, rst_reg);
      clk_sig2 = ClockSignals2::tick_fast(sys_sig, rst_sig2, clk_reg);
      lcd_sig  = LCDSignals::tick_fast(sys_sig, clk_sig2, rst_sig2, lcd_reg);

      //----------

      ClockRegisters::tock_fast1(sys_sig, clk_sig1, clk_reg);
      ClockRegisters::tock_fast2(sys_sig, clk_sig1, clk_sig2, rst_sig2, clk_reg);
      ResetRegisters::tock_fast(sys_sig, clk_sig1, rst_reg, rst_reg);
      LCDRegisters::tock_fast(sys_sig, clk_sig2, rst_sig2, lcd_sig, lcd_reg, lcd_reg);
    }
  }
}

void TestGB::sim(int phases, bool fast) {
  fast ? sim_fast(phases) : sim_slow(phases);
}

//-----------------------------------------------------------------------------

};