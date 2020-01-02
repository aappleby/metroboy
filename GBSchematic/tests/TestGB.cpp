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
  vid_clk_reg.pwron();
  lcd_reg.pwron();
  bus.pwron();
}

// This should put the GB in the same state as the boot sequence.

void TestGB::reset() {
  pwron();

  sys_reg.reset();
  clk_reg1.reset();
  rst_reg.reset();
  vid_clk_reg.reset();
  lcd_reg.reset();
  bus.reset();
}

//----------------------------------------
// Boots the chip. Currently no idea if this is actually what the boot
// sequence really looks like, but it's vaguely plausible.

void TestGB::boot() {
  sim(16);
  sys_reg.RST = false;
  sim(16);
  sys_reg.CLK_GOOD = true;
  sim(16);
  sys_reg.CLK_REQ = true;
  sim(16);
  sys_reg.LCDC_EN = true;
  sim(456*2*154 - 7);
}

//-----------------------------------------------------------------------------

void TestGB::sim(int phases) {
  for (int p = 0; p < phases; p++) {
    sys_reg.next_phase();

    // needs 10 passes
    for (int pass = 0; pass < 20; pass++) {
      clk_reg1.tick_slow(sys_reg);
      ClockSignals1 clk_sig1 = ClockSignals1::tick_slow(sys_reg, clk_reg1);

      rst_reg.tick_slow(sys_reg, clk_sig1);
      ResetSignals1 rst_sig1 = ResetSignals1::tick_slow(sys_reg, clk_sig1, rst_reg);
      VideoResets vid_rst = VideoResets::tick_slow(sys_reg, rst_sig1);

      vid_clk_reg.tick_slow(clk_sig1, vid_rst);
      ClockSignals2 vid_clk_sig = ClockSignals2::tick_slow(sys_reg, vid_rst, vid_clk_reg);

      lcd_reg.tock_slow(vid_clk_sig, vid_rst);
      LcdSignals lcd_sig = LcdSignals::tick_slow(vid_rst, lcd_reg);

      clk_reg1.commit();
      rst_reg.commit();
      vid_clk_reg.commit();
      lcd_reg.commit();
    }
  }
}

//-----------------------------------------------------------------------------

};