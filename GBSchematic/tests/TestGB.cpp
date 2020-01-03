#include "TestGB.h"

namespace Schematics {

//-----------------------------------------------------------------------------

TestGB::TestGB() {
  pwron();
}

//----------------------------------------

void TestGB::pwron() {
  memset(this, 0, sizeof(*this));

  sys_reg.pwron();
  clk_reg.pwron();
  rst_reg.pwron();
  vclk_reg.pwron();
  lcd_reg.pwron();
  bus_tri.pwron();
}

//----------------------------------------
// This should put the GB in the same state as the boot sequence.

void TestGB::reset() {
  pwron();

  sys_reg.reset();
  clk_reg.reset();
  rst_reg.reset();
  vclk_reg.reset();
  lcd_reg.reset();
  bus_tri.reset();
}

//----------------------------------------
// Boots the chip. Currently no idea if this is actually what the boot
// sequence really looks like, but it's vaguely plausible.

void TestGB::boot() {
  sim(16);
  sys_reg.PIN_RST = false;
  sim(16);
  sys_reg.PIN_CLK_GOOD = true;
  sim(16);
  sys_reg.CPU_CLK_REQ = true;
  sim(16);
  sys_reg.LCDC_EN = true;
  sim(456*2*154 - 7);
}

//-----------------------------------------------------------------------------

void TestGB::sim(int phases) {
  for (int p = 0; p < phases; p++) {
    SysSignals sys_sig = sys_reg.next_phase();

    // needs 10 passes
    for (int pass = 0; pass < 20; pass++) {
      
      ClkSignals clk_sig1 = clk_reg.tick_slow(sys_sig);
      RstSignals rst_sig1 = rst_reg.tick_slow(sys_sig, clk_sig1);
      VrstSignals vid_rst = rst_reg.vrst_signals(sys_sig, rst_sig1);

      vclk_reg.tick_slow(clk_sig1, vid_rst);
      VclkSignals vid_clk_sig = vclk_reg.signals();

      lcd_reg.tock_slow(vid_clk_sig, vid_rst);
      LcdSignals lcd_sig = lcd_reg.signals(vid_rst);

      commit();
    }
  }
}

void TestGB::commit() {
  clk_reg.commit();
  rst_reg.commit();
  vclk_reg.commit();
  lcd_reg.commit();
}

//-----------------------------------------------------------------------------

};