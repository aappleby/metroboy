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
  rst_reg.pwron();
  clk_reg2.pwron();
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
  rst_reg.reset();
  
  //clk_reg2.pwron();
  clk_reg2.reset();
  //lcd_reg.reset();
}

//-----------------------------------------------------------------------------

void TestGB::sim_slow(int phases) {
  for (int p = 0; p < phases; p++) {
    sys_reg.next_phase();

    // needs 10 passes
    for (int pass = 0; pass < 20; pass++) {
      clk_reg1.tick_slow(sys_reg);
      ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(sys_reg, clk_reg1);

      rst_reg.tick_slow(sys_reg, clk_sig1);
      ResetSignals1 rst_sig1 = ResetSignals1::tick_slow(sys_reg, clk_sig1, rst_reg);
      ResetSignals2 rst_sig2 = ResetSignals2::tick_slow(sys_reg, rst_sig1);

      clk_reg2.tock_slow(clk_sig1, rst_sig2);

      clk_reg1.commit();
      rst_reg.commit();
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
    
    rst_reg.tick_fast(sys_reg);
    ResetSignals1 rst_sig1 = ResetSignals1::tick_fast(sys_reg, rst_reg);
    ResetSignals2 rst_sig2 = ResetSignals2::tick_fast(sys_reg, rst_sig1);

    clk_reg2.tock_slow(clk_sig1, rst_sig2);

    clk_reg1.commit();
    rst_reg.commit();
  }

#if 0
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
#endif
}

//-----------------------------------------------------------------------------

};