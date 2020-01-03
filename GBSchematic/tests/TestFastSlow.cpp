
//-----------------------------------------------------------------------------

#if 0
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename T>
string check_byte_match(const T& a, const T& b) {
  uint8_t* blobA = (uint8_t*)&a;
  uint8_t* blobB = (uint8_t*)&b;

  for (int i = 0; i < sizeof(T); i++) {
    if (blobA[i] != blobB[i]) {
      return errprintf("Mismatch at %d - 0x%02x 0x%02x\n", i, blobA[i], blobB[i]);
    }
  }
  return {};
}


//-----------------------------------------------------------------------------

string check_states(TestGB& a, TestGB& b) {
  if (a.sys_clk.PHAZ_ABCDxxxx != b.sys_clk.PHAZ_ABCDxxxx) return "PHAZ_ABCDxxxx mismatch\n";
  if (a.sys_clk.PHAZ_xBCDExxx != b.sys_clk.PHAZ_xBCDExxx) return "PHAZ_xBCDExxx mismatch\n";
  if (a.sys_clk.PHAZ_xxCDEFxx != b.sys_clk.PHAZ_xxCDEFxx) return "PHAZ_xxCDEFxx mismatch\n";
  if (a.sys_clk.PHAZ_xxxDEFGx != b.sys_clk.PHAZ_xxxDEFGx) return "PHAZ_xxxDEFGx mismatch\n";

  if (a.rst_reg.WAITING_FOR_CLKREQ != b.rst_reg.WAITING_FOR_CLKREQ) return "WAITING_FOR_CLKREQ mismatch\n";
  if (a.rst_reg.RESET_REG != b.rst_reg.RESET_REG) return "RESET_REG mismatch\n";

  if (a.vid_clk.WUVU_AxxDExxH != b.vid_clk.WUVU_AxxDExxH) return "WUVU_AxxDExxH mismatch\n";
  if (a.vid_clk.VENA_xBCDExxx != b.vid_clk.VENA_xBCDExxx) return "VENA_xBCDExxx mismatch\n";
  if (a.vid_clk.WOSU_xxCDxxGH != b.vid_clk.WOSU_xxCDxxGH) return "WOSU_xxCDxxGH mismatch\n";

  return {};
}


bool LcdRegisters::check_match(const LcdRegisters& a, const LcdRegisters& b) {
  bool ret = true;
  ret &= check(a.X0.val == b.X0.val);
  ret &= check(a.X1.val == b.X1.val);
  ret &= check(a.X2.val == b.X2.val);
  ret &= check(a.X3.val == b.X3.val);
  ret &= check(a.X4.val == b.X4.val);
  ret &= check(a.X5.val == b.X5.val);
  ret &= check(a.X6.val == b.X6.val);

  ret &= check(a.Y0.val == b.Y0.val);
  ret &= check(a.Y1.val == b.Y1.val);
  ret &= check(a.Y2.val == b.Y2.val);
  ret &= check(a.Y3.val == b.Y3.val);
  ret &= check(a.Y4.val == b.Y4.val);
  ret &= check(a.Y5.val == b.Y5.val);
  ret &= check(a.Y6.val == b.Y6.val);
  ret &= check(a.Y7.val == b.Y7.val);

  ret &= check(a.VID_LINE_d4.val  == b.VID_LINE_d4.val);
  ret &= check(a.VID_LINE_d6.val  == b.VID_LINE_d6.val);
    
  ret &= check(a.NEW_LINE_d0a_val == b.NEW_LINE_d0a_val);

  ret &= check(a.NEW_LINE_d4a.val == b.NEW_LINE_d4a.val);
  ret &= check(a.LINE_153_d4.val  == b.LINE_153_d4.val);
  ret &= check(a.VBLANK_d4.val    == b.VBLANK_d4.val);
  return ret;
}



string check_signals(TestGB& gb) {
  string ret;
  ClkSignals slow_clk_sig1 = ClkSignals::tick_slow(gb.sys_reg, gb.sys_clk);
  ClkSignals fast_clk_sig1 = ClkSignals::tick_fast(gb.sys_reg, gb.sys_clk);
  
  ret = check_byte_match(slow_clk_sig1, fast_clk_sig1);
  if (ret.size()) return string("clk_sig1 mismatch\n") + ret;

  RstSignals slow_rst_sig1 = RstSignals::tick_slow(gb.sys_reg, slow_clk_sig1, gb.rst_reg);
  RstSignals fast_rst_sig1 = RstSignals::tick_fast(gb.sys_reg, gb.rst_reg);
  ret = check_byte_match(slow_rst_sig1, fast_rst_sig1);
  if (ret.size()) return string("rst_sig1 mismatch\n") + ret;

  VrstSignals slow_vid_rst = VrstSignals::tick_slow(gb.sys_reg, slow_rst_sig1);
  VrstSignals fast_vid_rst = VrstSignals::tick_fast(gb.sys_reg, fast_rst_sig1);
  ret = check_byte_match(slow_vid_rst, fast_vid_rst);
  if (ret.size()) return string("vid_rst mismatch\n") + ret;

  VclkSignals slow_vid_clk = VclkSignals::tick_slow(gb.sys_reg, slow_vid_rst, gb.vid_clk);
  VclkSignals fast_vid_clk = VclkSignals::tick_fast(gb.sys_reg, fast_vid_rst, gb.vid_clk);
  ret = check_byte_match(slow_vid_clk, fast_vid_clk);
  if (ret.size()) return string("vid_clk mismatch\n") + ret;

  //LcdSignals    slow_lcd_sig     = LcdSignals::tick_slow(gb.sys_reg, slow_vid_clk, slow_vid_rst, gb.lcd_reg);
  //LcdSignals    fast_lcd_sig     = LcdSignals::tick_fast(gb.sys_reg, fast_vid_clk, fast_vid_rst, gb.lcd_reg);
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

