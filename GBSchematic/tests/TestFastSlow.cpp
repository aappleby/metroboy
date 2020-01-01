
//-----------------------------------------------------------------------------

#if 0
//-----------------------------------------------------------------------------

string check_signals(TestGB& gb) {
  string ret;
  ClockSignals1 slow_clk_sig1 = ClockSignals1::tick_slow(gb.sys_reg, gb.clk_reg1);
  ClockSignals1 fast_clk_sig1 = ClockSignals1::tick_fast(gb.sys_reg, gb.clk_reg1);
  
  ret = check_byte_match(slow_clk_sig1, fast_clk_sig1);
  if (ret.size()) return string("clk_sig1 mismatch\n") + ret;

  ResetSignals1 slow_rst_sig1 = ResetSignals1::tick_slow(gb.sys_reg, slow_clk_sig1, gb.rst_reg);
  ResetSignals1 fast_rst_sig1 = ResetSignals1::tick_fast(gb.sys_reg, gb.rst_reg);
  ret = check_byte_match(slow_rst_sig1, fast_rst_sig1);
  if (ret.size()) return string("rst_sig1 mismatch\n") + ret;

  ResetSignals2 slow_rst_sig2 = ResetSignals2::tick_slow(gb.sys_reg, slow_rst_sig1);
  ResetSignals2 fast_rst_sig2 = ResetSignals2::tick_fast(gb.sys_reg, fast_rst_sig1);
  ret = check_byte_match(slow_rst_sig2, fast_rst_sig2);
  if (ret.size()) return string("rst_sig2 mismatch\n") + ret;

  ClockSignals2 slow_clk_sig2 = ClockSignals2::tick_slow(gb.sys_reg, slow_rst_sig2, gb.clk_reg2);
  ClockSignals2 fast_clk_sig2 = ClockSignals2::tick_fast(gb.sys_reg, fast_rst_sig2, gb.clk_reg2);
  ret = check_byte_match(slow_clk_sig2, fast_clk_sig2);
  if (ret.size()) return string("clk_sig2 mismatch\n") + ret;

  //LCDSignals    slow_lcd_sig     = LCDSignals::tick_slow(gb.sys_reg, slow_clk_sig2, slow_rst_sig2, gb.lcd_reg);
  //LCDSignals    fast_lcd_sig     = LCDSignals::tick_fast(gb.sys_reg, fast_clk_sig2, fast_rst_sig2, gb.lcd_reg);
  //check_byte_match(slow_lcd_sig , fast_lcd_sig );
  return ret;
}

//----------------------------------------
// Slow boot, fast boot, and reset should produce matching states

string sim_fast_slow(TestGB& gb1, TestGB& gb2, int phases, bool check_phases) {
  for (int i = 0; i < phases; i++) {
    gb1.sim_slow(1);
    gb2.sim_fast(1);

    string ret = check_states(gb1, gb2);
    if (ret.size()) return errprintf("sim_fast_slow failed at phase %d\n", i) + ret;

    if (check_phases) {
      ret = check_clock_phases(gb1);
      if (ret.size()) return errprintf("sim_fast_slow phase clock phase mismatch at phas %d\n", i) + ret;
    }
  }
  return {};
}


void test_fast_slow() {
  printf("test_fast_slow:\n");

  TestGB gb1;
  TestGB gb2;
  string ret;

  gb1.pwron();
  gb2.pwron();

  cout << "pwron  ";
  ret = sim_fast_slow(gb1, gb2, 16, false);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }

  gb1.sys_reg.set_rst(false);
  gb2.sys_reg.set_rst(false);

  cout << "!reset ";
  ret = sim_fast_slow(gb1, gb2, 16, false);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }

  gb1.sys_reg.set_clk_good(true);
  gb2.sys_reg.set_clk_good(true);

  cout << "clk_good ";
  ret = sim_fast_slow(gb1, gb2, 16, false);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }

  gb1.sys_reg.set_clk_req(true);
  gb2.sys_reg.set_clk_req(true);

  cout << "clk_req ";
  ret = sim_fast_slow(gb1, gb2, 16, false);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }

  gb1.sys_reg.set_lcdc_en(true);
  gb2.sys_reg.set_lcdc_en(true);

  cout << "lcdc_en ";
  ret = sim_fast_slow(gb1, gb2, 16, false);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }

  // video clocks are stable, run a frame with clock phase checking on.
  cout << "frame ";
  ret = sim_fast_slow(gb1, gb2, 456*2*154 - 23, true);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }

  cout << "boot_phase_alignment gb1 ";
  ret = check_boot_phase_alignment(gb1);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }

  cout << "boot_phase_alignment gb2 ";
  ret = check_boot_phase_alignment(gb2);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }

  TestGB gb3;
  gb3.reset();

  cout << "reset() ";
  ret = check_match(gb2, gb3);
  cout << (ret.size() ? "fail\n" : "pass\n");
  if (ret.size()) { cout << ret; return; }
}
#endif

