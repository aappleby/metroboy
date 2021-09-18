#include "GateBoyLib/LogicBoy.h"
#include "GateBoyLib/GateBoy.h"

//-----------------------------------------------------------------------------

void LogicBoy::reset_to_bootrom(const blob& cart_blob)
{
  lb_state.reset_to_bootrom();
  cpu.reset_to_bootrom();
  mem.reset_to_bootrom();
  sys.reset_to_bootrom();
  pins.reset_to_bootrom();
  probes.reset_to_bootrom();
}

//-----------------------------------------------------------------------------

void LogicBoy::reset_to_cart(const blob& cart_blob) {
  lb_state.reset_to_cart();
  cpu.reset_to_cart();
  mem.reset_to_cart();
  sys.reset_to_cart();
  pins.reset_to_cart();
  probes.reset_to_cart();
}

//-----------------------------------------------------------------------------

Result<uint8_t, Error> LogicBoy::peek(int addr) const {
  if (addr >= 0x8000 && addr <= 0x9FFF) { return mem.vid_ram[addr - 0x8000];      }
  if (addr >= 0xA000 && addr <= 0xBFFF) { return mem.cart_ram[addr - 0xA000];     }
  if (addr >= 0xC000 && addr <= 0xDFFF) { return mem.int_ram[addr - 0xC000];      }
  if (addr >= 0xE000 && addr <= 0xFDFF) { return mem.int_ram[addr - 0xE000];      }
  if (addr >= 0xFE00 && addr <= 0xFEFF) { return mem.oam_ram[addr - 0xFE00];      }
  if (addr >= 0xFF80 && addr <= 0xFFFE) { return mem.zero_ram[addr - 0xFF80];     }
  return lb_state.peek(addr);
}

Result<uint8_t, Error> LogicBoy::poke(int addr, uint8_t data_in) {
  if (addr >= 0x8000 && addr <= 0x9FFF) { mem.vid_ram[addr - 0x8000] = data_in; return data_in; }
  if (addr >= 0xA000 && addr <= 0xBFFF) { mem.cart_ram[addr - 0xA000] = data_in; return data_in; }
  if (addr >= 0xC000 && addr <= 0xDFFF) { mem.int_ram[addr - 0xC000] = data_in; return data_in; }
  if (addr >= 0xE000 && addr <= 0xFDFF) { mem.int_ram[addr - 0xE000] = data_in; return data_in; }
  if (addr >= 0xFE00 && addr <= 0xFEFF) { mem.oam_ram[addr - 0xFE00] = data_in; return data_in; }
  if (addr >= 0xFF80 && addr <= 0xFFFE) { mem.zero_ram[addr - 0xFF80] = data_in; return data_in; }
  return lb_state.poke(addr, data_in);
}

//-----------------------------------------------------------------------------

Result<uint8_t, Error> LogicBoy::dbg_read(const blob& cart_blob, int addr) {
  CHECK_P((sys.phase_total & 7) == 0);

  Req old_req = cpu.bus_req_new;
  bool old_cpu_en = sys.cpu_en;
  sys.cpu_en = false;

  cpu.bus_req_new.addr = uint16_t(addr);
  cpu.bus_req_new.data = 0;
  cpu.bus_req_new.read = 1;
  cpu.bus_req_new.write = 0;
  run_phases(cart_blob, 8);

  cpu.bus_req_new = old_req;
  sys.cpu_en = old_cpu_en;

  return cpu.cpu_data_latch;
}

//------------------------------------------------------------------------------

Result<uint8_t, Error> LogicBoy::dbg_write(const blob& cart_blob, int addr, uint8_t data_in) {
  CHECK_P((sys.phase_total & 7) == 0);

  Req old_req = cpu.bus_req_new;
  bool old_cpu_en = sys.cpu_en;
  sys.cpu_en = false;

  cpu.bus_req_new.addr = uint16_t(addr);
  cpu.bus_req_new.data = data_in;
  cpu.bus_req_new.read = 0;
  cpu.bus_req_new.write = 1;
  run_phases(cart_blob, 8);

  cpu.bus_req_new = old_req;
  sys.cpu_en = old_cpu_en;
  return data_in;
}

//------------------------------------------------------------------------------------------------------------------------

bool LogicBoy::run_phases(const blob& cart_blob, int phase_count) {
  bool result = true;
  for (int i = 0; i < phase_count; i++) {
    result &= next_phase(cart_blob);
  }
  return result;
}

bool LogicBoy::next_phase(const blob& cart_blob) {
  CHECK_N(!sys.clk_req);
  tock_cpu();
  tock_logic(cart_blob, sys.phase_total);
  update_framebuffer();
  sys.phase_total++;
  return true;
}

//------------------------------------------------------------------------------------------------------------------------

void LogicBoy::update_framebuffer()
{
  int lcd_x = lb_state.pix_count - 8;
  int lcd_y = lb_state.reg_ly;
  int DATA0 = ~pins.lcd.PIN_51_LCD_DATA0.state;
  int DATA1 = ~pins.lcd.PIN_50_LCD_DATA1.state;

  if (lcd_y >= 0 && lcd_y < 144 && lcd_x >= 0 && lcd_x < 160) {
    wire p0 = bit(DATA0);
    wire p1 = bit(DATA1);
    auto new_pix = p0 + p1 * 2;

    mem.framebuffer[lcd_x + lcd_y * 160] = uint8_t(3 - new_pix);
  }

#if 0
  if (bit(~lcd.old_lcd_clock.qp_old()) && lcd.PIN_53_LCD_CLOCK.qp_new()) {
    gb_screen_x++;
  }
  if (lcd.PIN_54_LCD_HSYNC.qp_new() || lcd.PIN_55_LCD_LATCH.qp_new()) {
    gb_screen_x = 0;
  }

  if (bit(~lcd.old_lcd_latch.qp_old()) && lcd.PIN_55_LCD_LATCH.qp_new()) {
    if (gb_screen_y < 144) {
      for (int x = 0; x < 159; x++) {
        uint8_t p0 = lcd.lcd_pipe_lo[x + 1].qp_new();
        uint8_t p1 = lcd.lcd_pipe_hi[x + 1].qp_new();
        framebuffer[x + gb_screen_y * 160] = p0 + p1 * 2;
      }
      {
        uint8_t p0 = lcd.lcd_pix_lo.qp_new();
        uint8_t p1 = lcd.lcd_pix_hi.qp_new();
        framebuffer[159 + gb_screen_y * 160] = p0 + p1 * 2;
      }
    }

    if (lcd.PIN_57_LCD_VSYNC.qp_new()) {
      gb_screen_y = 0;
    }
    else {
      gb_screen_y++;
    }
  }

  lcd.old_lcd_clock.set_new(lcd.PIN_53_LCD_CLOCK.qp_new());
  lcd.old_lcd_latch.set_new(lcd.PIN_55_LCD_LATCH.qp_new());
#endif
}

//------------------------------------------------------------------------------------------------------------------------

void LogicBoy::tock_cpu() {
  cpu.cpu_data_latch &= lb_state.cpu_dbus;
  cpu.imask_latch = lb_state.reg_ie;

  if (DELTA_HA) {
    if (cpu.core.op == 0x76 && (cpu.imask_latch & cpu.intf_halt_latch)) cpu.core.state_ = 0;
    cpu.intf_halt_latch = 0;
  }

  // +ha -ab -bc -cd -de -ef -fg -gh
  if (DELTA_HA) {
    // this one latches funny, some hardware bug
    if (get_bit(lb_state.reg_if, 2)) cpu.intf_halt_latch |= INT_TIMER_MASK;
  }

  // -ha +ab -bc
  if (DELTA_AB) {
    if (sys.cpu_en) {
      cpu.core.tock_ab(cpu.imask_latch, cpu.intf_latch, cpu.cpu_data_latch);
    }
  }

  if (DELTA_AB) {
    if (sys.cpu_en) {
      cpu.bus_req_new.addr = cpu.core._bus_addr;
      cpu.bus_req_new.data = cpu.core._bus_data;
      cpu.bus_req_new.read = cpu.core._bus_read;
      cpu.bus_req_new.write = cpu.core._bus_write;
    }
  }

  // -bc +cd +de -ef -fg -gh -ha -ab
  if (DELTA_DE) {
    if (get_bit(lb_state.reg_if, 0)) cpu.intf_halt_latch |= INT_VBLANK_MASK;
    if (get_bit(lb_state.reg_if, 1)) cpu.intf_halt_latch |= INT_STAT_MASK;
    if (get_bit(lb_state.reg_if, 3)) cpu.intf_halt_latch |= INT_SERIAL_MASK;
    if (get_bit(lb_state.reg_if, 4)) cpu.intf_halt_latch |= INT_JOYPAD_MASK;
  }

  // -ha -ab -bc -cd -de -ef +fg +gh
  if (DELTA_GH) {
    cpu.cpu_data_latch = 0xFF;
  }

  // +ha -ab -bc -cd -de -ef -fg +gh
  if (DELTA_GH) {
    cpu.intf_latch = lb_state.reg_if;
  }
}

//-----------------------------------------------------------------------------

void LogicBoy::tock_logic(const blob& cart_blob, int64_t phase_total) {
  LogicBoyState  state_old = lb_state;
  LogicBoyState& state_new = lb_state;

  const uint8_t phase_old = 1 << (7 - ((sys.phase_total + 0) & 7));
  const uint8_t phase_new = 1 << (7 - ((sys.phase_total + 1) & 7));

  //----------------------------------------

  const uint16_t cpu_addr_old = state_old.cpu_abus;
  const auto cpu_data_old = state_old.cpu_dbus;

  const bool cpu_addr_vram_old = (cpu_addr_old >= 0x8000) && (cpu_addr_old <= 0x9FFF);
  const bool cpu_addr_oam_old = (cpu_addr_old >= 0xFE00) && (cpu_addr_old <= 0xFEFF);

  state_new.cpu_abus = 0xFFFF;
  state_new.cpu_dbus = 0xFF;

  if (gen_clk_new(phase_total, 0b00001111)) {
    // Data has to be driven on EFGH or we fail the wave tests
    if (cpu.bus_req_new.write) state_new.cpu_dbus = cpu.bus_req_new.data_lo;
    state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.state = cpu.bus_req_new.read;
  }
  else {
    state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.state = 0;
  }

  if (gen_clk_new(phase_total, 0b10000000)) {
    state_new.cpu_signals.SIG_IN_CPU_RDp.state = 0;
    state_new.cpu_signals.SIG_IN_CPU_WRp.state = 0;
    state_new.cpu_abus = cpu.bus_req_new.addr & 0x00FF;
  }
  else {
    state_new.cpu_signals.SIG_IN_CPU_RDp.state = cpu.bus_req_new.read;
    state_new.cpu_signals.SIG_IN_CPU_WRp.state = cpu.bus_req_new.write;
    state_new.cpu_abus = cpu.bus_req_new.addr;
  }

  bool EXT_addr_new = (cpu.bus_req_new.read || cpu.bus_req_new.write);
  if ((cpu.bus_req_new.addr >= 0x8000) && (cpu.bus_req_new.addr < 0x9FFF) && gen_clk_new(phase_total, 0b10000000)) EXT_addr_new = false;
  if ((cpu.bus_req_new.addr >= 0xFE00)) EXT_addr_new = false;
  if ((cpu.bus_req_new.addr <= 0x00FF) && !state_new.cpu_signals.TEPU_BOOT_BITn) EXT_addr_new = false;
  state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = EXT_addr_new;

  const uint16_t cpu_addr_new = state_new.cpu_abus;
  const bool cpu_addr_vram_new = (cpu_addr_new >= 0x8000) && (cpu_addr_new <= 0x9FFF);
  const bool cpu_addr_ram_new = (cpu_addr_new >= 0xA000) && (cpu_addr_new <= 0xFDFF);
  const bool cpu_addr_oam_new = (cpu_addr_new >= 0xFE00) && (cpu_addr_new <= 0xFEFF);

  //----------------------------------------

  pins.sys.PIN_74_CLK.CLK.state = gen_clk_new(phase_total, 0b10101010);
  pins.sys.PIN_74_CLK.CLKGOOD.state = 1;

  pins.sys.PIN_71_RST = 0;
  pins.sys.PIN_76_T2 = 0;
  pins.sys.PIN_77_T1 = 0;

  state_new.cpu_ack = cpu.core.int_ack;

  state_new.sys_clk.SIG_CPU_CLKREQ.state = 1;

  state_new.cpu_signals.SIG_CPU_ADDR_HIp.state = cpu_addr_new >= 0xFE00 && cpu_addr_new <= 0xFFFF;
  state_new.cpu_signals.SIG_CPU_UNOR_DBG.state = 0;
  state_new.cpu_signals.SIG_CPU_UMUT_DBG.state = 0;

  //----------------------------------------
  // Sys clock signals

  pins.sys.PIN_73_CLK_DRIVE.state = pins.sys.PIN_74_CLK.CLK;
  state_new.sys_clk.AVET_DEGLITCH.state = pins.sys.PIN_74_CLK.CLK;
  state_new.sys_clk.ANOS_DEGLITCH.state = !pins.sys.PIN_74_CLK.CLK;

  state_new.sys_clk.AFUR_xxxxEFGH.state = gen_clk_new(phase_total, 0b00001111);
  state_new.sys_clk.ALEF_AxxxxFGH.state = gen_clk_new(phase_total, 0b10000111);
  state_new.sys_clk.APUK_ABxxxxGH.state = gen_clk_new(phase_total, 0b11000011);
  state_new.sys_clk.ADYK_ABCxxxxH.state = gen_clk_new(phase_total, 0b11100001);

  pins.sys.PIN_75_CLK_OUT.state = gen_clk_new(phase_total, 0b00001111);

  state_new.sys_clk.SIG_CPU_BOWA_Axxxxxxx.state = gen_clk_new(phase_total, 0b10000000);
  state_new.sys_clk.SIG_CPU_BEDO_xBCDEFGH.state = gen_clk_new(phase_total, 0b01111111);
  state_new.sys_clk.SIG_CPU_BEKO_ABCDxxxx.state = gen_clk_new(phase_total, 0b11110000);
  state_new.sys_clk.SIG_CPU_BUDE_xxxxEFGH.state = gen_clk_new(phase_total, 0b00001111);
  state_new.sys_clk.SIG_CPU_BOLO_ABCDEFxx.state = gen_clk_new(phase_total, 0b11111100);
  state_new.sys_clk.SIG_CPU_BUKE_AxxxxxGH.state = gen_clk_new(phase_total, 0b10000011);
  state_new.sys_clk.SIG_CPU_BOMA_xBCDEFGH.state = gen_clk_new(phase_total, 0b01111111);
  state_new.sys_clk.SIG_CPU_BOGA_Axxxxxxx.state = gen_clk_new(phase_total, 0b10000000);

  state_new.cpu_signals.TEDO_CPU_RDp.state = state_new.cpu_signals.SIG_IN_CPU_RDp;
  state_new.cpu_signals.APOV_CPU_WRp = gen_clk_new(phase_total, 0b00001110) && state_new.cpu_signals.SIG_IN_CPU_WRp;
  state_new.cpu_signals.TAPU_CPU_WRp = state_new.cpu_signals.APOV_CPU_WRp;
  state_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK = gen_clk_new(phase_total, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp;

  //----------------------------------------
  // DIV

  if (gen_clk_new(phase_total, 0b10000000)) state_new.reg_div = state_new.reg_div + 1;
  if (cpu_addr_new == 0xFF04 && state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00001110)) state_new.reg_div = 0;
  if (cpu_addr_new == 0xFF04 && state_new.cpu_signals.SIG_IN_CPU_RDp) state_new.cpu_dbus = uint8_t(state_new.reg_div >> 6);

  //----------------------------------------
  // In logic mode we don't care about the power-on behavior, we only want behavior to match when running code. So, we set
  // this stuff to zeroes.

  state_new.sys_rst.AFER_SYS_RSTp = 0;
  state_new.sys_rst.TUBO_WAITINGp = 0;
  state_new.sys_rst.ASOL_POR_DONEn = 0;
  state_new.sys_rst.SIG_CPU_EXT_CLKGOOD = 1;
  state_new.sys_rst.SIG_CPU_EXT_RESETp = 0;
  state_new.sys_rst.SIG_CPU_STARTp = 0;
  state_new.sys_rst.SIG_CPU_INT_RESETp = 0;;
  state_new.sys_rst.SOTO_DBG_VRAMp = 0;

  //----------------------------------------
  // LCDC
  // has to be near the top as it controls the video reset signal

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && cpu_addr_new == 0xFF40 && gen_clk_new(phase_total, 0b00000001)) {
    state_new.reg_lcdc = ~state_old.cpu_dbus;
  }

  if (state_new.cpu_signals.SIG_IN_CPU_RDp && (cpu_addr_new == 0xFF40)) {
    state_new.cpu_dbus = ~state_new.reg_lcdc;
  }

  //----------------------------------------
  // Video clocks

  state_new.sys_clk.WOSU_AxxDExxH.state = !get_bit(state_new.reg_lcdc, 7) && gen_clk_new(phase_total, 0b10011001);
  state_new.sys_clk.WUVU_ABxxEFxx.state = !get_bit(state_new.reg_lcdc, 7) && gen_clk_new(phase_total, 0b11001100);
  state_new.sys_clk.VENA_xxCDEFxx.state = !get_bit(state_new.reg_lcdc, 7) && gen_clk_new(phase_total, 0b00111100);

  //----------------------------------------
  // LYC

  const auto reg_ly_old = state_old.reg_ly;
  const auto reg_lyc_old = state_old.reg_lyc ^ 0xFF;

  if (cpu_addr_new == 0xFF45) {
    if (state_new.cpu_signals.SIG_IN_CPU_RDp) state_new.cpu_dbus = uint8_t(reg_lyc_old);
    if (state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) state_new.reg_lyc = uint8_t(~cpu_data_old);
  }

  if (!get_bit(state_new.reg_lcdc, 7) && gen_clk_new(phase_total, 0b00100000)) {
    state_new.int_ctrl.ROPO_LY_MATCH_SYNCp.state = reg_ly_old == reg_lyc_old;
  }

  //----------------------------------------
  // LX, LY, lcd flags

  if (get_bit(state_new.reg_lcdc, 7)) {
    state_new.lcd.ANEL_x113p.state = 0;
    state_new.lcd.CATU_x113p.state = 0;
    state_new.lcd.NYPE_x113p.state = 0;
    state_new.lcd.RUTU_x113p.state = 0;
    state_new.lcd.POPU_y144p.state = 0;
    state_new.lcd.MYTA_y153p.state = 0;
    state_new.lcd.SYGU_LINE_STROBE.state = 0;
    state_new.ATEJ_LINE_RSTp = 1;
    state_new.reg_lx = 0;
    state_new.reg_ly = 0;
  }
  else {
    wire ly_144_old = (reg_ly_old & 144) == 144;
    wire ly_153_old = (reg_ly_old & 153) == 153;

    if (gen_clk_new(phase_total, 0b10000000)) {
      state_new.lcd.CATU_x113p.state = state_new.lcd.RUTU_x113p && !ly_144_old;
    }

    if (gen_clk_new(phase_total, 0b00100000)) {
      state_new.lcd.ANEL_x113p = state_old.lcd.CATU_x113p;
      state_new.lcd.NYPE_x113p = state_old.lcd.RUTU_x113p;

      if (!state_old.lcd.NYPE_x113p && state_new.lcd.NYPE_x113p) {
        state_new.lcd.POPU_y144p.state = ly_144_old;
        state_new.lcd.MYTA_y153p.state = ly_153_old;
      }

      state_new.reg_lx = state_old.reg_lx + 1;
    }

    if (gen_clk_new(phase_total, 0b00001000)) {
      state_new.lcd.CATU_x113p.state = state_new.lcd.RUTU_x113p && !ly_144_old;
    }

    if (gen_clk_new(phase_total, 0b00000010)) {
      state_new.lcd.ANEL_x113p = state_old.lcd.CATU_x113p;
      state_new.lcd.RUTU_x113p.state = (state_old.reg_lx == 113);

      if (!state_old.lcd.RUTU_x113p && state_new.lcd.RUTU_x113p) {
        state_new.reg_ly = uint8_t(reg_ly_old + 1);
      }

      uint8_t lx_old = (uint8_t)state_old.reg_lx;
      state_new.lcd.SYGU_LINE_STROBE.state = (lx_old == 0) || (lx_old == 7) || (lx_old == 45) || (lx_old == 83);
    }

    state_new.ATEJ_LINE_RSTp = !state_new.lcd.ANEL_x113p && state_new.lcd.CATU_x113p;
    if (state_new.lcd.RUTU_x113p) state_new.reg_lx = 0;
    if (state_new.lcd.MYTA_y153p) state_new.reg_ly = 0;
  }

  const auto reg_ly_new = state_new.reg_ly;

  if (state_new.cpu_signals.SIG_IN_CPU_RDp && (cpu_addr_new == 0xFF44)) {
    state_new.cpu_dbus = reg_ly_new;
  }

  //----------------------------------------
  // Joypad

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && cpu_addr_new == 0xFF00 && gen_clk_new(phase_total, 0b00000001)) {
    set_bit(state_new.reg_joy, 0, get_bit(state_old.cpu_dbus, 4));
    set_bit(state_new.reg_joy, 1, get_bit(state_old.cpu_dbus, 5));

    pins.joy.PIN_63_JOY_P14.state = get_bit(state_new.reg_joy, 0);
    pins.joy.PIN_62_JOY_P15.state = get_bit(state_new.reg_joy, 1);

  }

  bool EXT_button0 = 0, EXT_button1 = 0, EXT_button2 = 0, EXT_button3 = 0;

  if (bit(pins.joy.PIN_63_JOY_P14.state)) {
    EXT_button0 = get_bit(sys.buttons, 0); // RIGHT
    EXT_button1 = get_bit(sys.buttons, 1); // LEFT
    EXT_button2 = get_bit(sys.buttons, 2); // UP
    EXT_button3 = get_bit(sys.buttons, 3); // DOWN
  }
  else if (bit(pins.joy.PIN_62_JOY_P15.state)) {
    EXT_button0 = get_bit(sys.buttons, 4); // A
    EXT_button1 = get_bit(sys.buttons, 5); // B
    EXT_button2 = get_bit(sys.buttons, 6); // SELECT
    EXT_button3 = get_bit(sys.buttons, 7); // START
  }

  pins.joy.PIN_67_JOY_P10.state = EXT_button0;
  pins.joy.PIN_66_JOY_P11.state = EXT_button1;
  pins.joy.PIN_65_JOY_P12.state = EXT_button2;
  pins.joy.PIN_64_JOY_P13.state = EXT_button3;

  wire any_button = EXT_button0 || EXT_button1 || EXT_button2 || EXT_button3;


  if (gen_clk_new(phase_total, 0b10000000)) {
    state_new.int_ctrl.AWOB_WAKE_CPU.state = !any_button;
    state_new.int_ctrl.SIG_CPU_WAKE.state = !any_button;
  }

  if (gen_clk_new(phase_total, 0b10000000)) {
    state_new.joy_int.APUG_JP_GLITCH3 = state_new.joy_int.AGEM_JP_GLITCH2;
    state_new.joy_int.AGEM_JP_GLITCH2 = state_new.joy_int.ACEF_JP_GLITCH1;
    state_new.joy_int.ACEF_JP_GLITCH1 = state_new.joy_int.BATU_JP_GLITCH0;
    state_new.joy_int.BATU_JP_GLITCH0.state = !any_button;
  }

  if (cpu_addr_new == 0xFF00 && state_new.cpu_signals.SIG_IN_CPU_RDp) {
    set_bit(state_new.cpu_dbus, 0, !get_bit(state_new.joy_latch, 0));
    set_bit(state_new.cpu_dbus, 1, !get_bit(state_new.joy_latch, 1));
    set_bit(state_new.cpu_dbus, 2, !get_bit(state_new.joy_latch, 2));
    set_bit(state_new.cpu_dbus, 3, !get_bit(state_new.joy_latch, 3));
    set_bit(state_new.cpu_dbus, 4,  get_bit(state_new.reg_joy, 0));
    set_bit(state_new.cpu_dbus, 5,  get_bit(state_new.reg_joy, 1));
  }
  else {
    set_bit(state_new.joy_latch, 0, pins.joy.PIN_67_JOY_P10.state);
    set_bit(state_new.joy_latch, 1, pins.joy.PIN_66_JOY_P11.state);
    set_bit(state_new.joy_latch, 2, pins.joy.PIN_65_JOY_P12.state);
    set_bit(state_new.joy_latch, 3, pins.joy.PIN_64_JOY_P13.state);
  }

  //----------------------------------------
  //tock_serial_logic();
  //tock_timer_logic();

  //----------------------------------------
  // Timer

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) {
    if (cpu_addr_new == 0xFF06) state_new.reg_tma = state_new.cpu_dbus;
    if (cpu_addr_new == 0xFF07) state_new.reg_tac = state_new.cpu_dbus;
  }

  if (gen_clk_new(phase_total, 0b10000000)) {
    state_new.int_ctrl.MOBA_TIMER_OVERFLOWp.state = !get_bit(state_old.reg_tima, 7) && state_old.int_ctrl.NYDU_TIMA7p_DELAY;
    state_new.int_ctrl.NYDU_TIMA7p_DELAY.state = get_bit(state_old.reg_tima, 7);
  }

  {
    const wire UKAP_CLK_MUXa_new = get_bit(state_new.reg_tac, 0) ? get_bit(state_new.reg_div, 5) : get_bit(state_new.reg_div, 3);
    const wire TEKO_CLK_MUXb_new = get_bit(state_new.reg_tac, 0) ? get_bit(state_new.reg_div, 1) : get_bit(state_new.reg_div, 7);
    const wire TECY_CLK_MUXc_new = get_bit(state_new.reg_tac, 1) ? UKAP_CLK_MUXa_new : TEKO_CLK_MUXb_new;
    const wire SOGU_TIMA_CLKn_new = TECY_CLK_MUXc_new && get_bit(state_new.reg_tac, 2);

    const wire UKAP_CLK_MUXa_old = get_bit(state_old.reg_tac, 0) ? get_bit(state_old.reg_div, 5) : get_bit(state_old.reg_div, 3);
    const wire TEKO_CLK_MUXb_old = get_bit(state_old.reg_tac, 0) ? get_bit(state_old.reg_div, 1) : get_bit(state_old.reg_div, 7);


    const wire TECY_CLK_MUXc_old = get_bit(state_old.reg_tac, 1) ? UKAP_CLK_MUXa_old : TEKO_CLK_MUXb_old;

    const wire SOGU_TIMA_CLKn_old = TECY_CLK_MUXc_old && get_bit(state_old.reg_tac, 2);


    if (SOGU_TIMA_CLKn_old && !SOGU_TIMA_CLKn_new) {
      state_new.reg_tima = state_new.reg_tima + 1;
    }
  }

  if (cpu_addr_new == 0xFF05 && gen_clk_new(phase_total, 0b00001110) && state_new.cpu_signals.SIG_IN_CPU_WRp) {
    if (!state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE || state_new.int_ctrl.MOBA_TIMER_OVERFLOWp) {
      state_new.int_ctrl.NYDU_TIMA7p_DELAY.state = 0;
      state_new.reg_tima = state_new.cpu_dbus;
    }
  }
  else {
    if (state_new.int_ctrl.MOBA_TIMER_OVERFLOWp) {
      state_new.int_ctrl.NYDU_TIMA7p_DELAY.state = 0;
      state_new.reg_tima = state_new.reg_tma;
    }
  }

  if (state_new.cpu_signals.SIG_IN_CPU_RDp) {
    if (cpu_addr_new == 0xFF05) state_new.cpu_dbus = state_new.reg_tima;
    if (cpu_addr_new == 0xFF06) state_new.cpu_dbus = state_new.reg_tma;
    if (cpu_addr_new == 0xFF07) state_new.cpu_dbus = state_new.reg_tac | 0b11111000;
  }

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && cpu_addr_new == 0xFF50 && gen_clk_new(phase_total, 0b00000001)) {
    state_new.cpu_signals.TEPU_BOOT_BITn.state = state_new.SATO_BOOT_BITn;
  }

  state_new.cpu_signals.SIG_CPU_BOOTp.state = 0;
  state_new.cpu_signals.SIG_BOOT_CSp.state = 0;

  if (cpu_addr_new <= 0x00FF) {

    state_new.cpu_signals.SIG_CPU_BOOTp.state = !state_new.cpu_signals.TEPU_BOOT_BITn;

    if (state_new.cpu_signals.SIG_IN_CPU_RDp && !state_new.cpu_signals.TEPU_BOOT_BITn) {
      state_new.cpu_signals.SIG_BOOT_CSp.state = 1;
      state_new.cpu_dbus = DMG_ROM_blob[cpu_addr_new & 0xFF];
    }
  }

  if (state_new.cpu_signals.SIG_IN_CPU_RDp && (cpu_addr_new == 0xFF50)) {
    state_new.cpu_dbus &= ~1;
    state_new.cpu_dbus |= state_new.cpu_signals.TEPU_BOOT_BITn.state;
  }

  state_new.SATO_BOOT_BITn = get_bit(state_new.cpu_dbus, 0) || state_new.cpu_signals.TEPU_BOOT_BITn;

  //----------------------------------------
  // DMA

  if (cpu_addr_new == 0xFF46 && state_new.cpu_signals.SIG_IN_CPU_RDp) {
    state_new.cpu_dbus = ~state_old.reg_dma;
  }

  if (gen_clk_new(phase_total, 0b10000000)) {
    state_new.dma_ctrl.LUVY_DMA_TRIG_d0.state = state_new.dma_ctrl.LYXE_DMA_LATCHp;
    state_new.MATU_DMA_RUNNINGp = state_new.dma_ctrl.LOKY_DMA_LATCHp;

    if (state_new.dma_ctrl.LOKY_DMA_LATCHp && !state_new.dma_ctrl.LENE_DMA_TRIG_d4) {
      state_new.dma_lo = state_old.dma_lo + 1;
    }
  }
  else if (gen_clk_new(phase_total, 0b00001000)) {
    if (cpu_addr_new == 0xFF46 && state_new.cpu_signals.SIG_IN_CPU_WRp) {
      state_new.dma_ctrl.LYXE_DMA_LATCHp.state = 1;
    }

    if (state_old.dma_lo == 159) {
      state_new.dma_ctrl.MYTE_DMA_DONE.state = 1;
      state_new.dma_ctrl.LARA_DMA_LATCHn = 1;
      state_new.dma_ctrl.LOKY_DMA_LATCHp = 0;
    }

    state_new.dma_ctrl.LENE_DMA_TRIG_d4 = state_old.dma_ctrl.LUVY_DMA_TRIG_d0;

    if (state_new.dma_ctrl.LUVY_DMA_TRIG_d0) {
      state_new.dma_ctrl.MYTE_DMA_DONE.state = 0;
      state_new.dma_ctrl.LYXE_DMA_LATCHp.state = 0;
      state_new.dma_lo = 0;
      state_new.dma_ctrl.LARA_DMA_LATCHn = 0;
      state_new.dma_ctrl.LOKY_DMA_LATCHp = 1;
    }
  }
  else if (gen_clk_new(phase_total, 0b00000001)) {
    if (cpu_addr_new == 0xFF46 && state_new.cpu_signals.SIG_IN_CPU_WRp) {
      state_new.reg_dma = ~state_old.cpu_dbus;
    }
  }

  const auto dma_addr_new = ((state_new.reg_dma ^ 0xFF) << 8) | state_new.dma_lo;
  const auto dma_addr_vram_new = state_new.MATU_DMA_RUNNINGp && (dma_addr_new >= 0x8000) && (dma_addr_new <= 0x9FFF);

  //----------------------------------------
  // VID RST BRANCH

  if (get_bit(state_new.reg_lcdc, 7)) {

    state_new.sprite_scanner.DOBA_SCAN_DONE_Bp.state = 0;
    state_new.sprite_scanner.BYBA_SCAN_DONE_Ap.state = 0;
    state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp = 0;
    state_new.sprite_scanner.BESU_SCANNINGn.state = 0;
    state_new.sprite_scanner.CENO_SCANNINGn.state = 0;
    state_new.scan_counter = 0;
    state_new.sprite_scanner.FETO_SCAN_DONEp = 0;
    state_new.VOGA_HBLANKp = 0;
    state_new.XYMU_RENDERINGn = 1;

    if (gen_clk_new(phase_total, 0b01010101)) {
      state_new.sfetch_control.SOBU_SFETCH_REQp.  state = state_new.FEPO_STORE_MATCHp && !state_old.win_ctrl.RYDY_WIN_HITp && state_new.tfetch_control.LYRY_BFETCH_DONEp && !state_new.sfetch_control.TAKA_SFETCH_RUNNINGp;
    }
    if (gen_clk_new(phase_total, 0b10101010)) {
      state_new.sfetch_control.SUDA_SFETCH_REQp   = state_new.sfetch_control.SOBU_SFETCH_REQp;
      state_new.sfetch_control.TYFO_SFETCH_S0p_D1.state = get_bit(state_new.sfetch_counter, 0);
    }

    state_new.sfetch_counter = 0;
    state_new.sfetch_control.TOBU_SFETCH_S1p_D2.state = 0;
    state_new.sfetch_control.VONU_SFETCH_S1p_D4.state = 0;
    state_new.sfetch_control.SEBA_SFETCH_S1p_D5.state = 0;

    state_new.sfetch_control.TEXY_SFETCHINGp = 0;

    state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp = 0;

    state_new.win_ctrl.NUNU_WIN_MATCHp.state = 0;
    state_new.win_ctrl.NOPA_WIN_MODE_Bp.state = 0;
    state_new.win_ctrl.PYNU_WIN_MODE_Ap.state = 0;
    state_new.win_ctrl.SOVY_WIN_HITp.state = 0;
    state_new.win_ctrl.RYDY_WIN_HITp.state = 0;
    state_new.win_ctrl.PUKU_WIN_HITn.state = 1;

    state_new.tfetch_control.PYGO_FETCH_DONEp.state = 0;
    state_new.tfetch_control.PORY_FETCH_DONEp.state = 0;
    state_new.tfetch_control.NYKA_FETCH_DONEp.state = 0;
    state_new.tfetch_control.POKY_PRELOAD_LATCHp.state = 0;

    state_new.sfetch_control.TAKA_SFETCH_RUNNINGp.state = 1;

    state_new.sprite_scanner.DEZY_COUNT_CLKp.state = 0;

    state_new.sprite_counter = 0;
    state_new.sprite_reset_flags = 0;
    state_new.sprite_store_flags = 0;

    state_new.store_x0 = 0xFF;
    state_new.store_x1 = 0xFF;
    state_new.store_x2 = 0xFF;
    state_new.store_x3 = 0xFF;
    state_new.store_x4 = 0xFF;
    state_new.store_x5 = 0xFF;
    state_new.store_x6 = 0xFF;
    state_new.store_x7 = 0xFF;
    state_new.store_x8 = 0xFF;
    state_new.store_x9 = 0xFF;
  }

  //----------------------------------------
  // VID RUN BRANCH

  if (!get_bit(state_new.reg_lcdc, 7)) {
    if (state_new.ATEJ_LINE_RSTp) {
      state_new.sprite_scanner.DOBA_SCAN_DONE_Bp.state = 0;
      state_new.sprite_scanner.BYBA_SCAN_DONE_Ap.state = 0;
      state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp = 0;
      state_new.scan_counter = 0;
      state_new.sprite_scanner.BESU_SCANNINGn.state = 1;
      state_new.VOGA_HBLANKp = 0;
    }
    else {
      if (gen_clk_new(phase_total, 0b01010101)) {
        state_new.sprite_scanner.DOBA_SCAN_DONE_Bp = state_old.sprite_scanner.BYBA_SCAN_DONE_Ap;
        state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp = !state_new.sprite_scanner.DOBA_SCAN_DONE_Bp && state_new.sprite_scanner.BYBA_SCAN_DONE_Ap;
      }
      else if (gen_clk_new(phase_total, 0b10001000)) {
        state_new.sprite_scanner.BYBA_SCAN_DONE_Ap.state = (state_old.scan_counter == 39);
        state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp = !state_new.sprite_scanner.DOBA_SCAN_DONE_Bp && state_new.sprite_scanner.BYBA_SCAN_DONE_Ap;
        
        if (state_old.scan_counter != 39) {
          state_new.scan_counter = state_old.scan_counter + 1;
        }
      }

      if (state_new.lcd.CATU_x113p) state_new.sprite_scanner.BESU_SCANNINGn.state = 1;
      if (state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp) state_new.sprite_scanner.BESU_SCANNINGn.state = 0;
    }

    if (gen_clk_new(phase_total, 0b01010101)) {
      state_new.VOGA_HBLANKp = state_new.WODU_HBLANKp;
      state_new.sfetch_control.SOBU_SFETCH_REQp.state   = state_new.FEPO_STORE_MATCHp && !state_old.win_ctrl.RYDY_WIN_HITp && state_new.tfetch_control.LYRY_BFETCH_DONEp && !state_new.sfetch_control.TAKA_SFETCH_RUNNINGp;
      state_new.sfetch_control.VONU_SFETCH_S1p_D4 = state_new.sfetch_control.TOBU_SFETCH_S1p_D2;
      state_new.sfetch_control.TOBU_SFETCH_S1p_D2.state = get_bit(state_new.sfetch_counter, 1);

      uint8_t sfetch_phase_old = pack(
        !(state_old.sfetch_control.TYFO_SFETCH_S0p_D1.state ^ get_bit(state_old.sfetch_counter, 0)),
        get_bit(state_old.sfetch_counter, 0),
        get_bit(state_old.sfetch_counter, 1),
        get_bit(state_old.sfetch_counter, 2));

      if ((sfetch_phase_old >> 1) != 5) {
        state_new.sfetch_counter = (sfetch_phase_old >> 1) + 1;
      }

      if (state_new.sfetch_control.SOBU_SFETCH_REQp && !state_new.sfetch_control.SUDA_SFETCH_REQp) {
        state_new.sfetch_counter = 0;
      }
      state_new.win_ctrl.NOPA_WIN_MODE_Bp.state = state_new.win_ctrl.PYNU_WIN_MODE_Ap;
      state_new.tfetch_control.PYGO_FETCH_DONEp = state_new.tfetch_control.PORY_FETCH_DONEp;
      state_new.tfetch_control.NYKA_FETCH_DONEp.state = state_new.tfetch_control.LYRY_BFETCH_DONEp;

      state_new.win_ctrl.SOVY_WIN_HITp.state = state_new.win_ctrl.RYDY_WIN_HITp;
    }

    if (gen_clk_new(phase_total, 0b10101010)) {
      if (gen_clk_new(phase_total, 0b10001000)) {
        
        state_new.sprite_index = (state_new.oam_abus >> 2) ^ 0b111111;
        state_new.sprite_scanner.CENO_SCANNINGn.state = state_old.sprite_scanner.BESU_SCANNINGn;
      }

      state_new.sfetch_control.SUDA_SFETCH_REQp   = state_new.sfetch_control.SOBU_SFETCH_REQp;
      state_new.sfetch_control.TYFO_SFETCH_S0p_D1.state = get_bit(state_new.sfetch_counter, 0);
      state_new.sfetch_control.SEBA_SFETCH_S1p_D5 = state_new.sfetch_control.VONU_SFETCH_S1p_D4;
      state_new.win_ctrl.NUNU_WIN_MATCHp = state_new.win_ctrl.PYCO_WIN_MATCHp;
      state_new.tfetch_control.PORY_FETCH_DONEp = state_new.tfetch_control.NYKA_FETCH_DONEp;
    }

    if (state_new.sfetch_control.SOBU_SFETCH_REQp && !state_new.sfetch_control.SUDA_SFETCH_REQp) {
      state_new.sfetch_control.TAKA_SFETCH_RUNNINGp.state = 1;
    }

    state_new.sprite_scanner.FETO_SCAN_DONEp = (state_new.scan_counter == 39);



    if (state_new.ATEJ_LINE_RSTp) {
      state_new.sfetch_counter = 0;
      state_new.win_ctrl.PYNU_WIN_MODE_Ap.state = 0;
      state_new.sfetch_control.TAKA_SFETCH_RUNNINGp.state = 1;
    }


    if (state_new.VOGA_HBLANKp) state_new.XYMU_RENDERINGn = 1;
    if (state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp) state_new.XYMU_RENDERINGn = 0;

    if (state_new.XYMU_RENDERINGn) {
      state_new.sfetch_control.TOBU_SFETCH_S1p_D2.state = 0;
      state_new.sfetch_control.VONU_SFETCH_S1p_D4.state = 0;
      state_new.sfetch_control.SEBA_SFETCH_S1p_D5.state = 0;
      state_new.sfetch_control.TEXY_SFETCHINGp = 0;
      state_new.tfetch_control.PYGO_FETCH_DONEp.state = 0;
      state_new.tfetch_control.PORY_FETCH_DONEp.state = 0;
      state_new.tfetch_control.NYKA_FETCH_DONEp.state = 0;
      state_new.tfetch_control.POKY_PRELOAD_LATCHp.state = 0;
      state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp = 0;
    }
    else {
      state_new.sfetch_control.TEXY_SFETCHINGp = (get_bit(state_new.sfetch_counter, 1) || state_new.sfetch_control.VONU_SFETCH_S1p_D4);

      if (!state_new.tfetch_control.POKY_PRELOAD_LATCHp && state_new.tfetch_control.NYKA_FETCH_DONEp && state_new.tfetch_control.PORY_FETCH_DONEp) {
        state_new.sfetch_control.TAKA_SFETCH_RUNNINGp.state = 0;
      }

      state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp =
        state_new.sfetch_control.TYFO_SFETCH_S0p_D1 &&
        get_bit(state_new.sfetch_counter, 0) &&
        state_new.sfetch_control.SEBA_SFETCH_S1p_D5 &&
        state_new.sfetch_control.VONU_SFETCH_S1p_D4;

      if (state_new.tfetch_control.PYGO_FETCH_DONEp) {
        state_new.tfetch_control.POKY_PRELOAD_LATCHp.state = 1;
      }

      if (state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp) {
        state_new.sfetch_control.TAKA_SFETCH_RUNNINGp.state = 0;
      }

      if (!state_old.sfetch_control.WUTY_SFETCH_DONE_TRIGp && state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp) {
        state_new.sprite_reset_flags = state_old.sprite_match_flags;
      }
    }
  }

  const uint8_t sfetch_phase_old = pack(
    !(state_old.sfetch_control.TYFO_SFETCH_S0p_D1.state ^ get_bit(state_old.sfetch_counter, 0)),
    get_bit(state_old.sfetch_counter, 0),
    get_bit(state_old.sfetch_counter, 1),
    get_bit(state_old.sfetch_counter, 2));

  const uint8_t sfetch_phase_new = pack(
    !(state_new.sfetch_control.TYFO_SFETCH_S0p_D1.state ^ get_bit(state_new.sfetch_counter, 0)),
    get_bit(state_new.sfetch_counter, 0),
    get_bit(state_new.sfetch_counter, 1),
    get_bit(state_new.sfetch_counter, 2));

  if (!state_old.XYMU_RENDERINGn) {
    if ((sfetch_phase_old == 5) && (sfetch_phase_new == 6 || state_new.XYMU_RENDERINGn)) {
      state_new.sprite_pix_a = ~state_old.flipped_sprite;
    }

    if ((sfetch_phase_old == 9) && (sfetch_phase_new == 10 || state_new.XYMU_RENDERINGn)) {
      state_new.sprite_pix_b = ~state_old.flipped_sprite;
    }
  }

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  {
    state_new.ACYL_SCANNINGp = !state_new.MATU_DMA_RUNNINGp && state_new.sprite_scanner.BESU_SCANNINGn && !get_bit(state_new.reg_lcdc, 7);

    const wire oam_busy_old = (cpu_addr_old >= 0xFE00 && cpu_addr_old <= 0xFEFF) || state_new.MATU_DMA_RUNNINGp;
    const wire oam_busy_new = (cpu_addr_new >= 0xFE00 && cpu_addr_new <= 0xFEFF) || state_new.MATU_DMA_RUNNINGp;

    CHECK_N(!state_old.XYMU_RENDERINGn && state_new.ACYL_SCANNINGp);
    CHECK_N(!state_new.XYMU_RENDERINGn && state_new.ACYL_SCANNINGp);
    CHECK_N(!state_old.XYMU_RENDERINGn && state_old.ACYL_SCANNINGp);

    uint8_t BYCU_OAM_CLKp_old = 1;
    if (state_old.ACYL_SCANNINGp)  BYCU_OAM_CLKp_old &= gen_clk_old(phase_total, 0b10001000);
    if (oam_busy_old)  BYCU_OAM_CLKp_old &= gen_clk_old(phase_total, 0b11110000);
    if (!state_old.XYMU_RENDERINGn) BYCU_OAM_CLKp_old &= sfetch_phase_old != 3;

    uint8_t BYCU_OAM_CLKp_new = 1;
    if (state_new.ACYL_SCANNINGp)  BYCU_OAM_CLKp_new &= gen_clk_new(phase_total, 0b10001000);
    if (oam_busy_new)  BYCU_OAM_CLKp_new &= gen_clk_new(phase_total, 0b11110000);
    if (!state_new.XYMU_RENDERINGn) BYCU_OAM_CLKp_new &= sfetch_phase_new != 3;

    if (!BYCU_OAM_CLKp_old && BYCU_OAM_CLKp_new) {
      state_new.oam_temp_a = ~state_new.oam_latch_a;
      state_new.oam_temp_b = ~state_new.oam_latch_b;
    }
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if it overlaps the current LCD Y coordinate.

  if (get_bit(state_new.reg_lcdc, 7) || state_new.ATEJ_LINE_RSTp) {
    state_new.sprite_counter = 0;
    state_new.sprite_reset_flags = 0;
    state_new.sprite_store_flags = 0;

    state_new.store_x0 = 0xFF;
    state_new.store_x1 = 0xFF;
    state_new.store_x2 = 0xFF;
    state_new.store_x3 = 0xFF;
    state_new.store_x4 = 0xFF;
    state_new.store_x5 = 0xFF;
    state_new.store_x6 = 0xFF;
    state_new.store_x7 = 0xFF;
    state_new.store_x8 = 0xFF;
    state_new.store_x9 = 0xFF;
  }
  else {
    bool ssf_clk = gen_clk_new(phase_total, 0b10011001) || !state_new.sprite_scanner.CENO_SCANNINGn;

    int ly = (int)reg_ly_new;
    int sy = (int)state_new.oam_temp_a - 16;
    int sprite_height = get_bit(state_new.reg_lcdc, 2) ? 8 : 16;

    if (ly < sy || ly >= sy + sprite_height) ssf_clk = 1;

    if (gen_clk_new(phase_total, 0b10101010)) {
      state_new.sprite_scanner.DEZY_COUNT_CLKp.state = ssf_clk;
      if (!state_old.sprite_scanner.DEZY_COUNT_CLKp && state_new.sprite_scanner.DEZY_COUNT_CLKp) {
        if (state_old.sprite_counter != 10) {
          state_new.sprite_counter = state_old.sprite_counter + 1;
        }
      }
    }
    else {
      if (!ssf_clk) {
        state_new.sprite_store_flags = (1 << state_new.sprite_counter);
      }
      else {
        state_new.sprite_store_flags = 0;
      }
    }

    const auto sprite_store_flags_old = state_old.sprite_store_flags ^ 0b1111111111;
    const auto sprite_store_flags_new = state_new.sprite_store_flags ^ 0b1111111111;

    const auto store_clk_pe = ~sprite_store_flags_old & sprite_store_flags_new;
    const auto store_clk_ne = sprite_store_flags_old & ~sprite_store_flags_new;

    const auto sprite_ibus = state_new.sprite_ibus;
    const auto sprite_lbus = state_new.sprite_lbus;
    const auto sprite_reset_flags = state_new.sprite_reset_flags;
    const auto oam_temp_b = state_new.oam_temp_b;

    if (get_bit(store_clk_ne, 0)) state_new.store_i0 = sprite_ibus ^ 0b111111;
    if (get_bit(store_clk_ne, 1)) state_new.store_i1 = sprite_ibus ^ 0b111111;
    if (get_bit(store_clk_ne, 2)) state_new.store_i2 = sprite_ibus ^ 0b111111;
    if (get_bit(store_clk_ne, 3)) state_new.store_i3 = sprite_ibus ^ 0b111111;
    if (get_bit(store_clk_ne, 4)) state_new.store_i4 = sprite_ibus ^ 0b111111;
    if (get_bit(store_clk_ne, 5)) state_new.store_i5 = sprite_ibus ^ 0b111111;
    if (get_bit(store_clk_ne, 6)) state_new.store_i6 = sprite_ibus ^ 0b111111;
    if (get_bit(store_clk_ne, 7)) state_new.store_i7 = sprite_ibus ^ 0b111111;
    if (get_bit(store_clk_ne, 8)) state_new.store_i8 = sprite_ibus ^ 0b111111;
    if (get_bit(store_clk_ne, 9)) state_new.store_i9 = sprite_ibus ^ 0b111111;

    if (get_bit(store_clk_ne, 0)) state_new.store_l0 = sprite_lbus ^ 0b1111;
    if (get_bit(store_clk_ne, 1)) state_new.store_l1 = sprite_lbus ^ 0b1111;
    if (get_bit(store_clk_ne, 2)) state_new.store_l2 = sprite_lbus ^ 0b1111;
    if (get_bit(store_clk_ne, 3)) state_new.store_l3 = sprite_lbus ^ 0b1111;
    if (get_bit(store_clk_ne, 4)) state_new.store_l4 = sprite_lbus ^ 0b1111;
    if (get_bit(store_clk_ne, 5)) state_new.store_l5 = sprite_lbus ^ 0b1111;
    if (get_bit(store_clk_ne, 6)) state_new.store_l6 = sprite_lbus ^ 0b1111;
    if (get_bit(store_clk_ne, 7)) state_new.store_l7 = sprite_lbus ^ 0b1111;
    if (get_bit(store_clk_ne, 8)) state_new.store_l8 = sprite_lbus ^ 0b1111;
    if (get_bit(store_clk_ne, 9)) state_new.store_l9 = sprite_lbus ^ 0b1111;

    if (get_bit(store_clk_pe, 0)) state_new.store_x0 = oam_temp_b;
    if (get_bit(store_clk_pe, 1)) state_new.store_x1 = oam_temp_b;
    if (get_bit(store_clk_pe, 2)) state_new.store_x2 = oam_temp_b;
    if (get_bit(store_clk_pe, 3)) state_new.store_x3 = oam_temp_b;
    if (get_bit(store_clk_pe, 4)) state_new.store_x4 = oam_temp_b;
    if (get_bit(store_clk_pe, 5)) state_new.store_x5 = oam_temp_b;
    if (get_bit(store_clk_pe, 6)) state_new.store_x6 = oam_temp_b;
    if (get_bit(store_clk_pe, 7)) state_new.store_x7 = oam_temp_b;
    if (get_bit(store_clk_pe, 8)) state_new.store_x8 = oam_temp_b;
    if (get_bit(store_clk_pe, 9)) state_new.store_x9 = oam_temp_b;


    if (get_bit(sprite_reset_flags, 0)) state_new.store_x0 = 0xFF;
    if (get_bit(sprite_reset_flags, 1)) state_new.store_x1 = 0xFF;
    if (get_bit(sprite_reset_flags, 2)) state_new.store_x2 = 0xFF;
    if (get_bit(sprite_reset_flags, 3)) state_new.store_x3 = 0xFF;
    if (get_bit(sprite_reset_flags, 4)) state_new.store_x4 = 0xFF;
    if (get_bit(sprite_reset_flags, 5)) state_new.store_x5 = 0xFF;
    if (get_bit(sprite_reset_flags, 6)) state_new.store_x6 = 0xFF;
    if (get_bit(sprite_reset_flags, 7)) state_new.store_x7 = 0xFF;
    if (get_bit(sprite_reset_flags, 8)) state_new.store_x8 = 0xFF;
    if (get_bit(sprite_reset_flags, 9)) state_new.store_x9 = 0xFF;

  }

  //----------------------------------------
  // Fine scroll match, sprite store match, clock pipe, and pixel counter are intertwined here.

  // NOTE we reassign this below because there's a bit of a feedback loop
  wire pause_rendering_new = state_new.win_ctrl.RYDY_WIN_HITp || !state_new.tfetch_control.POKY_PRELOAD_LATCHp || state_new.FEPO_STORE_MATCHp || state_new.WODU_HBLANKp;

  if (gen_clk_new(phase_total, 0b01010101)) {
    if (!pause_rendering_new) {
      state_new.fine_scroll.PUXA_SCX_FINE_MATCH_A.state = state_old.fine_scroll.ROXY_FINE_SCROLL_DONEn && (((state_old.reg_scx & 0b111) ^ 0b111) == state_old.fine_count);
    }
  }
  else {
    state_new.fine_scroll.NYZE_SCX_FINE_MATCH_B = state_new.fine_scroll.PUXA_SCX_FINE_MATCH_A;
  }

  if (state_new.XYMU_RENDERINGn) {
    state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.state = 1;
    state_new.fine_scroll.NYZE_SCX_FINE_MATCH_B.state = 0;
    state_new.fine_scroll.PUXA_SCX_FINE_MATCH_A.state = 0;
  }

  if (state_new.fine_scroll.PUXA_SCX_FINE_MATCH_A && !state_new.fine_scroll.NYZE_SCX_FINE_MATCH_B) {
    state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.state = 0;
  }

  if (!get_bit(state_new.reg_lcdc, 7)) {
    if (get_bit(state_new.reg_lcdc, 5)) {
      state_new.win_ctrl.PYNU_WIN_MODE_Ap.state = 0;
      state_new.win_ctrl.RYDY_WIN_HITp = 0;
      state_new.win_ctrl.PUKU_WIN_HITn = 1;
    }
    else {
      
      if (state_new.win_ctrl.NUNU_WIN_MATCHp) {
        state_new.win_ctrl.PYNU_WIN_MODE_Ap.state = 1;
      }
      
      if (state_new.win_ctrl.PYNU_WIN_MODE_Ap && !state_new.win_ctrl.NOPA_WIN_MODE_Bp) {
        state_new.tfetch_control.PORY_FETCH_DONEp.state = 0;
        state_new.tfetch_control.NYKA_FETCH_DONEp.state = 0;
      }

      if (state_new.tfetch_control.PORY_FETCH_DONEp) {
        state_new.win_ctrl.RYDY_WIN_HITp = 0;
        state_new.win_ctrl.PUKU_WIN_HITn = 1;
      }
      else if (state_new.win_ctrl.PYNU_WIN_MODE_Ap && !state_new.win_ctrl.NOPA_WIN_MODE_Bp) {
        state_new.win_ctrl.RYDY_WIN_HITp = 1;
        state_new.win_ctrl.PUKU_WIN_HITn = 0;
      }
    }
  }

  pause_rendering_new = state_new.win_ctrl.RYDY_WIN_HITp || !state_new.tfetch_control.POKY_PRELOAD_LATCHp || state_new.FEPO_STORE_MATCHp || state_new.WODU_HBLANKp;

  const wire pause_rendering_old = state_old.win_ctrl.RYDY_WIN_HITp || !state_old.tfetch_control.POKY_PRELOAD_LATCHp || state_old.FEPO_STORE_MATCHp || state_old.WODU_HBLANKp;
  const bool SACU_CLKPIPE_old = gen_clk_old(phase_total, 0b10101010) || pause_rendering_old || state_old.fine_scroll.ROXY_FINE_SCROLL_DONEn;
  const wire SACU_CLKPIPE_new = gen_clk_new(phase_total, 0b10101010) || pause_rendering_new || state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn;

  if (state_new.ATEJ_LINE_RSTp) CHECK_P(SACU_CLKPIPE_new);

  if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
    state_new.pix_count = state_new.pix_count + 1;
  }

  if (state_new.ATEJ_LINE_RSTp) {
    state_new.pix_count = 0;
  }

  if (get_bit(state_new.reg_lcdc, 7)) {
    state_new.pix_count = 0;
  }

  if (state_new.XYMU_RENDERINGn || state_new.sprite_scanner.CENO_SCANNINGn || get_bit(state_new.reg_lcdc, 1)) {
    state_new.sprite_match_flags = 0;
  }

  if (!state_new.XYMU_RENDERINGn && !state_new.sprite_scanner.CENO_SCANNINGn && !get_bit(state_new.reg_lcdc, 1)) {
    const uint8_t px = state_new.pix_count;
    
    state_new.FEPO_STORE_MATCHp = 0;
    state_new.sprite_match_flags = 0;

    if      (px == state_new.store_x0) { state_new.FEPO_STORE_MATCHp = 1; state_new.sprite_match_flags = 0b0000000001; }
    else if (px == state_new.store_x1) { state_new.FEPO_STORE_MATCHp = 1; state_new.sprite_match_flags = 0b0000000010; }
    else if (px == state_new.store_x2) { state_new.FEPO_STORE_MATCHp = 1; state_new.sprite_match_flags = 0b0000000100; }
    else if (px == state_new.store_x3) { state_new.FEPO_STORE_MATCHp = 1; state_new.sprite_match_flags = 0b0000001000; }
    else if (px == state_new.store_x4) { state_new.FEPO_STORE_MATCHp = 1; state_new.sprite_match_flags = 0b0000010000; }
    else if (px == state_new.store_x5) { state_new.FEPO_STORE_MATCHp = 1; state_new.sprite_match_flags = 0b0000100000; }
    else if (px == state_new.store_x6) { state_new.FEPO_STORE_MATCHp = 1; state_new.sprite_match_flags = 0b0001000000; }
    else if (px == state_new.store_x7) { state_new.FEPO_STORE_MATCHp = 1; state_new.sprite_match_flags = 0b0010000000; }
    else if (px == state_new.store_x8) { state_new.FEPO_STORE_MATCHp = 1; state_new.sprite_match_flags = 0b0100000000; }
    else if (px == state_new.store_x9) { state_new.FEPO_STORE_MATCHp = 1; state_new.sprite_match_flags = 0b1000000000; }
  }

  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later
  state_new.WODU_HBLANKp = !state_new.FEPO_STORE_MATCHp && (state_new.pix_count & 167) == 167;

  if (gen_clk_new(phase_total, 0b01010101)) {
    if (!pause_rendering_new) state_new.lcd.PAHO_X_8_SYNC.state = get_bit(state_old.pix_count, 3);
  }

  //----------------------------------------
  // Sprite bus

  state_new.sprite_ibus = 0b00111111;
  state_new.sprite_lbus = 0b00001111;

  if (get_bit(state_new.sprite_match_flags, 0)) { state_new.sprite_ibus = state_new.store_i0 ^ 0b00111111; state_new.sprite_lbus = state_new.store_l0 ^ 0b00001111; } 
  if (get_bit(state_new.sprite_match_flags, 1)) { state_new.sprite_ibus = state_new.store_i1 ^ 0b00111111; state_new.sprite_lbus = state_new.store_l1 ^ 0b00001111; } 
  if (get_bit(state_new.sprite_match_flags, 2)) { state_new.sprite_ibus = state_new.store_i2 ^ 0b00111111; state_new.sprite_lbus = state_new.store_l2 ^ 0b00001111; } 
  if (get_bit(state_new.sprite_match_flags, 3)) { state_new.sprite_ibus = state_new.store_i3 ^ 0b00111111; state_new.sprite_lbus = state_new.store_l3 ^ 0b00001111; } 
  if (get_bit(state_new.sprite_match_flags, 4)) { state_new.sprite_ibus = state_new.store_i4 ^ 0b00111111; state_new.sprite_lbus = state_new.store_l4 ^ 0b00001111; } 
  if (get_bit(state_new.sprite_match_flags, 5)) { state_new.sprite_ibus = state_new.store_i5 ^ 0b00111111; state_new.sprite_lbus = state_new.store_l5 ^ 0b00001111; } 
  if (get_bit(state_new.sprite_match_flags, 6)) { state_new.sprite_ibus = state_new.store_i6 ^ 0b00111111; state_new.sprite_lbus = state_new.store_l6 ^ 0b00001111; } 
  if (get_bit(state_new.sprite_match_flags, 7)) { state_new.sprite_ibus = state_new.store_i7 ^ 0b00111111; state_new.sprite_lbus = state_new.store_l7 ^ 0b00001111; } 
  if (get_bit(state_new.sprite_match_flags, 8)) { state_new.sprite_ibus = state_new.store_i8 ^ 0b00111111; state_new.sprite_lbus = state_new.store_l8 ^ 0b00001111; } 
  if (get_bit(state_new.sprite_match_flags, 9)) { state_new.sprite_ibus = state_new.store_i9 ^ 0b00111111; state_new.sprite_lbus = state_new.store_l9 ^ 0b00001111; } 

  if (state_new.sprite_scanner.CENO_SCANNINGn) {
    state_new.sprite_ibus = state_new.sprite_index;
  }

  if (state_new.XYMU_RENDERINGn) {
    state_new.sprite_ibus = state_new.sprite_index;
  }

  if (!state_new.FEPO_STORE_MATCHp) {
    const auto pack_ydiff = ~reg_ly_new + state_new.oam_temp_a;
    state_new.sprite_lbus = pack_ydiff & 0b00001111;
  }

  //----------------------------------------
  // WY/WX/window match

  if (gen_clk_new(phase_total, 0b01010101)) {
    if (!pause_rendering_new) state_new.win_ctrl.PYCO_WIN_MATCHp.state = state_new.win_ctrl.NUKO_WX_MATCHp;
  }

  if (!state_new.XYMU_RENDERINGn) {
    if (gen_clk_new(phase_total, 0b01010101)) {
      state_new.win_ctrl.RENE_WIN_FETCHn_B = state_new.win_ctrl.RYFA_WIN_FETCHn_A;
    }

    const auto fine_count_new = state_new.fine_count;

    if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
      state_new.win_ctrl.RYFA_WIN_FETCHn_A.state = !state_new.win_ctrl.NUKO_WX_MATCHp && fine_count_new == 7;
    }
  }
  else {
    state_new.win_ctrl.RENE_WIN_FETCHn_B.state = 0;
    state_new.win_ctrl.RYFA_WIN_FETCHn_A.state = 0;
  }

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) {
    if (cpu_addr_new == 0xFF4A) state_new.reg_wy = ~state_old.cpu_dbus;
    if (cpu_addr_new == 0xFF4B) state_new.reg_wx = ~state_old.cpu_dbus;
  }

  if (state_new.cpu_signals.SIG_IN_CPU_RDp) {
    if (cpu_addr_new == 0xFF4A) state_new.cpu_dbus = ~state_new.reg_wy;
    if (cpu_addr_new == 0xFF4B) state_new.cpu_dbus = ~state_new.reg_wx;
  }

  // FIXME get rid of this signal
  state_new.win_ctrl.ROGE_WY_MATCHp = (reg_ly_new == uint8_t(~state_new.reg_wy)) && !get_bit(state_new.reg_lcdc, 5);

  if (gen_clk_new(phase_total, 0b00100000)) {
    state_new.win_ctrl.SARY_WY_MATCHp.state = state_old.win_ctrl.ROGE_WY_MATCHp;
  }

  if (get_bit(state_new.reg_lcdc, 7)) {
    state_new.win_ctrl.PYCO_WIN_MATCHp.state = 0;
    state_new.win_ctrl.SARY_WY_MATCHp.state = 0;
  }

  if (state_new.win_ctrl.SARY_WY_MATCHp) state_new.win_ctrl.REJO_WY_MATCH_LATCHp.state = 1;
  if (state_new.lcd.POPU_y144p) state_new.win_ctrl.REJO_WY_MATCH_LATCHp.state = 0;
  if (get_bit(state_new.reg_lcdc, 7)) state_new.win_ctrl.REJO_WY_MATCH_LATCHp.state = 0;

  state_new.win_ctrl.NUKO_WX_MATCHp = (uint8_t(~state_new.reg_wx) == state_new.pix_count) && state_new.win_ctrl.REJO_WY_MATCH_LATCHp;

  //----------------------------------------
  // Tile fetch sequencer

  const uint8_t bfetch_phase_old = pack(
    !(state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state ^ get_bit(state_new.tfetch_counter, 0)),
    get_bit(state_new.tfetch_counter, 0),
    get_bit(state_new.tfetch_counter, 1),
    get_bit(state_new.tfetch_counter, 2));

  auto restart_fetch_state = [](const LogicBoyState& state) {
    return !state.XYMU_RENDERINGn && !state.tfetch_control.POKY_PRELOAD_LATCHp && state.tfetch_control.NYKA_FETCH_DONEp && state.tfetch_control.PORY_FETCH_DONEp;
  };

  auto trigger_win_fetch_state = [&](const LogicBoyState& state) {
    bool TEVO_WIN_FETCH_TRIGp = 0;
    if (state.win_ctrl.RYFA_WIN_FETCHn_A && !state.win_ctrl.RENE_WIN_FETCHn_B) TEVO_WIN_FETCH_TRIGp = 1;
    if (!state.win_ctrl.RYDY_WIN_HITp && state.win_ctrl.SOVY_WIN_HITp) TEVO_WIN_FETCH_TRIGp = 1;
    if (restart_fetch_state(state)) TEVO_WIN_FETCH_TRIGp = 1;
    return TEVO_WIN_FETCH_TRIGp;
  };

  const wire BFETCH_RSTp_new =
    state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp ||
    (state_new.win_ctrl.PYNU_WIN_MODE_Ap && !state_new.win_ctrl.NOPA_WIN_MODE_Bp) ||
    (state_new.win_ctrl.RYFA_WIN_FETCHn_A && !state_new.win_ctrl.RENE_WIN_FETCHn_B) ||
    (state_new.win_ctrl.SOVY_WIN_HITp && !state_new.win_ctrl.RYDY_WIN_HITp) ||
    restart_fetch_state(state_new);

  if (gen_clk_new(phase_total, 0b01010101)) {
    state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state = get_bit(state_new.tfetch_counter, 0);
  }

  if (state_new.XYMU_RENDERINGn) {
    state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state = 0;
  }

  if (BFETCH_RSTp_new) {
    state_new.tfetch_counter = 0;
    state_new.tfetch_control.LOVY_FETCH_DONEp.state = 0;
    state_new.tfetch_control.LONY_FETCHINGp.state = 1;
    state_new.tfetch_control.LYRY_BFETCH_DONEp = 0;
  }
  else {
    if ((bfetch_phase_old < 10) && gen_clk_new(phase_total, 0b10101010)) {
      state_new.tfetch_counter = (bfetch_phase_old >> 1) + 1;
    }

    if (gen_clk_new(phase_total, 0b10101010)) {
      state_new.tfetch_control.LOVY_FETCH_DONEp.state = state_new.tfetch_control.LYRY_BFETCH_DONEp;
    }
    state_new.tfetch_control.LYRY_BFETCH_DONEp = get_bit(state_new.tfetch_counter, 0) && get_bit(state_new.tfetch_counter, 2);
  }

  if (state_new.tfetch_control.LOVY_FETCH_DONEp || state_new.XYMU_RENDERINGn) {
    state_new.tfetch_control.LONY_FETCHINGp.state = 0;
  }

  const uint8_t bfetch_phase_new = pack(!(state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state ^ get_bit(state_new.tfetch_counter, 0)), get_bit(state_new.tfetch_counter, 0), get_bit(state_new.tfetch_counter, 1), get_bit(state_new.tfetch_counter, 2));

  if (!state_old.XYMU_RENDERINGn) {
    // These ffs are weird because they latches on phase change _or_ if rendering stops in the middle of a fetch
    // Good example of gate-level behavior that doesn't matter

    if ((bfetch_phase_old == 6) && (bfetch_phase_new == 7 || state_new.XYMU_RENDERINGn)) {
      state_new.tile_temp_a = ~state_new.vram_dbus;
    }

    if ((bfetch_phase_old == 2) && (bfetch_phase_new == 3 || state_new.XYMU_RENDERINGn)) {
      state_new.tile_temp_b = state_new.vram_dbus;
    }

    if ((bfetch_phase_old == 10) && (bfetch_phase_new == 11 || state_new.XYMU_RENDERINGn)) {
      state_new.tile_temp_b = state_new.vram_dbus;
    }
  }

  //----------------------------------------
  // Fine match counter

  const wire TEVO_WIN_FETCH_TRIGp_old = trigger_win_fetch_state(state_old);
  const wire TEVO_WIN_FETCH_TRIGp_new = trigger_win_fetch_state(state_new);
  
  if (state_new.ATEJ_LINE_RSTp) CHECK_P(state_new.XYMU_RENDERINGn);

  if (get_bit(state_new.reg_lcdc, 7)) {
    state_new.fine_count = 0;
    state_new.win_x.map = 0;
    state_new.win_y.tile = 0;
    state_new.win_y.map = 0;
  }
  else {
    if (state_new.fine_count != 7 && !pause_rendering_old && gen_clk_new(phase_total, 0b10101010)) {
      state_new.fine_count = state_new.fine_count + 1;
    }

    if (state_new.XYMU_RENDERINGn) {
      state_new.fine_count = 0;
    }

    if (state_new.ATEJ_LINE_RSTp) {
      state_new.win_x.map = 0;
    }
    else if (TEVO_WIN_FETCH_TRIGp_new) {
      state_new.fine_count = 0;
      if (state_new.win_ctrl.PYNU_WIN_MODE_Ap) {
        state_new.win_x.map = state_old.win_x.map + 1;
      }
    }


    if (state_old.win_ctrl.PYNU_WIN_MODE_Ap && !state_new.win_ctrl.PYNU_WIN_MODE_Ap) {
      uint8_t win_old = state_old.win_y.tile | (state_old.win_y.map << 3);

      uint8_t win_new = win_old + 1;

      state_new.win_y.tile = win_new & 0b111;
      state_new.win_y.map = win_new >> 3;
    }

    if (state_new.lcd.POPU_y144p) {
      state_new.win_y.tile = 0;
      state_new.win_y.map = 0;
    }
  }

  //----------------------------------------
  // Pal reg read/write

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) {
    if (cpu_addr_new == 0xFF47) state_new.reg_bgp  = ~state_old.cpu_dbus;
    if (cpu_addr_new == 0xFF48) state_new.reg_obp0 = ~state_old.cpu_dbus;
    if (cpu_addr_new == 0xFF49) state_new.reg_obp1 = ~state_old.cpu_dbus;
  }

  if (state_new.cpu_signals.SIG_IN_CPU_RDp) {
    if (cpu_addr_new == 0xFF47) state_new.cpu_dbus = ~state_new.reg_bgp;
    if (cpu_addr_new == 0xFF48) state_new.cpu_dbus = ~state_new.reg_obp0;
    if (cpu_addr_new == 0xFF49) state_new.cpu_dbus = ~state_new.reg_obp1;
  }

  //----------------------------------------
  // Pixel pipes

  {
    wire LOBY_RENDERINGn = state_new.XYMU_RENDERINGn;

    wire LAXE_BFETCH_S0n = not1(get_bit(state_new.tfetch_counter, 0));
    wire NAKO_BFETCH_S1n = not1(get_bit(state_new.tfetch_counter, 1));
    wire NOFU_BFETCH_S2n = not1(get_bit(state_new.tfetch_counter, 2));
    wire TYTU_SFETCH_S0n = not1(get_bit(state_new.sfetch_counter, 3));
    wire SYCU_SFETCH_S0pe = nor3(TYTU_SFETCH_S0n, LOBY_RENDERINGn, state_new.sfetch_control.TYFO_SFETCH_S0p_D1.state);

    wire MYSO_STORE_VRAM_DATA_TRIGp = nor3(LOBY_RENDERINGn, LAXE_BFETCH_S0n, state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state); // MYSO fires on fetch phase 2, 6, 10

    wire NYDY_LATCH_TILE_DAn = nand3(MYSO_STORE_VRAM_DATA_TRIGp, state_new.tfetch_counter, NOFU_BFETCH_S2n); // NYDY on fetch phase 6
    wire MOFU_LATCH_TILE_DBp = and2(MYSO_STORE_VRAM_DATA_TRIGp, NAKO_BFETCH_S1n); // MOFU fires on fetch phase 2 and 10
    wire TOPU_STORE_SPRITE_Ap = and2(get_bit(state_new.sfetch_counter, 1), SYCU_SFETCH_S0pe);
    wire RACA_STORE_SPRITE_Bp = and2(state_new.sfetch_control.VONU_SFETCH_S1p_D4.state, SYCU_SFETCH_S0pe);

    wire XEFY_SPRITE_DONEn = not1(state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp.state);

    // FIXME - old? well,i guess there's another feedback loop here...

    wire MEFU_SPRITE_MASK0n = or3(XEFY_SPRITE_DONEn, get_bit(state_new.spr_pipe_b, 0), get_bit(state_new.spr_pipe_a, 0));
    wire MEVE_SPRITE_MASK1n = or3(XEFY_SPRITE_DONEn, get_bit(state_new.spr_pipe_b, 1), get_bit(state_new.spr_pipe_a, 1));
    wire MYZO_SPRITE_MASK2n = or3(XEFY_SPRITE_DONEn, get_bit(state_new.spr_pipe_b, 2), get_bit(state_new.spr_pipe_a, 2));
    wire RUDA_SPRITE_MASK3n = or3(XEFY_SPRITE_DONEn, get_bit(state_new.spr_pipe_b, 3), get_bit(state_new.spr_pipe_a, 3));
    wire VOTO_SPRITE_MASK4n = or3(XEFY_SPRITE_DONEn, get_bit(state_new.spr_pipe_b, 4), get_bit(state_new.spr_pipe_a, 4));
    wire VYSA_SPRITE_MASK5n = or3(XEFY_SPRITE_DONEn, get_bit(state_new.spr_pipe_b, 5), get_bit(state_new.spr_pipe_a, 5));
    wire TORY_SPRITE_MASK6n = or3(XEFY_SPRITE_DONEn, get_bit(state_new.spr_pipe_b, 6), get_bit(state_new.spr_pipe_a, 6));
    wire WOPE_SPRITE_MASK7n = or3(XEFY_SPRITE_DONEn, get_bit(state_new.spr_pipe_b, 7), get_bit(state_new.spr_pipe_a, 7));

    wire LESY_SPRITE_MASK0p = not1(MEFU_SPRITE_MASK0n);
    wire LOTA_SPRITE_MASK1p = not1(MEVE_SPRITE_MASK1n);
    wire LYKU_SPRITE_MASK2p = not1(MYZO_SPRITE_MASK2n);
    wire ROBY_SPRITE_MASK3p = not1(RUDA_SPRITE_MASK3n);
    wire TYTA_SPRITE_MASK4p = not1(VOTO_SPRITE_MASK4n);
    wire TYCO_SPRITE_MASK5p = not1(VYSA_SPRITE_MASK5n);
    wire SOKA_SPRITE_MASK6p = not1(TORY_SPRITE_MASK6n);
    wire XOVU_SPRITE_MASK7p = not1(WOPE_SPRITE_MASK7n);

    //----------------------------------------
    // Sprite pipe A
    {
      wire PABE_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, !get_bit(state_new.sprite_pix_a, 0));
      wire MYTO_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, !get_bit(state_new.sprite_pix_a, 1));
      wire LELA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, !get_bit(state_new.sprite_pix_a, 2));
      wire MAME_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, !get_bit(state_new.sprite_pix_a, 3));
      wire VEXU_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, !get_bit(state_new.sprite_pix_a, 4));
      wire VABY_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, !get_bit(state_new.sprite_pix_a, 5));
      wire TUXA_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, !get_bit(state_new.sprite_pix_a, 6));
      wire VUNE_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, !get_bit(state_new.sprite_pix_a, 7));

      wire RATA_SPR_PIX_DA0n = not1(!get_bit(state_new.sprite_pix_a, 0));
      wire NUCA_SPR_PIX_DA1n = not1(!get_bit(state_new.sprite_pix_a, 1));
      wire LASE_SPR_PIX_DA2n = not1(!get_bit(state_new.sprite_pix_a, 2));
      wire LUBO_SPR_PIX_DA3n = not1(!get_bit(state_new.sprite_pix_a, 3));
      wire WERY_SPR_PIX_DA4n = not1(!get_bit(state_new.sprite_pix_a, 4));
      wire WURA_SPR_PIX_DA5n = not1(!get_bit(state_new.sprite_pix_a, 5));
      wire SULU_SPR_PIX_DA6n = not1(!get_bit(state_new.sprite_pix_a, 6));
      wire WAMY_SPR_PIX_DA7n = not1(!get_bit(state_new.sprite_pix_a, 7));

      wire PYZU_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, RATA_SPR_PIX_DA0n);
      wire MADA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, NUCA_SPR_PIX_DA1n);
      wire LYDE_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LASE_SPR_PIX_DA2n);
      wire LUFY_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, LUBO_SPR_PIX_DA3n);
      wire XATO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, WERY_SPR_PIX_DA4n);
      wire XEXU_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WURA_SPR_PIX_DA5n);
      wire TUPE_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SULU_SPR_PIX_DA6n);
      wire XYVE_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, WAMY_SPR_PIX_DA7n);

      //state_new.spr_pipe_a.WUFY_SPR_PIPE_A7.dff22(SACU_CLKPIPE_new, VUNE_SPR_PIX_SET7, XYVE_SPR_PIX_RST7, state_new.spr_pipe_a.VAFO_SPR_PIPE_A6.qp_old());
      //state_new.spr_pipe_a.VAFO_SPR_PIPE_A6.dff22(SACU_CLKPIPE_new, TUXA_SPR_PIX_SET6, TUPE_SPR_PIX_RST6, state_new.spr_pipe_a.WORA_SPR_PIPE_A5.qp_old());
      //state_new.spr_pipe_a.WORA_SPR_PIPE_A5.dff22(SACU_CLKPIPE_new, VABY_SPR_PIX_SET5, XEXU_SPR_PIX_RST5, state_new.spr_pipe_a.WYHO_SPR_PIPE_A4.qp_old());
      //state_new.spr_pipe_a.WYHO_SPR_PIPE_A4.dff22(SACU_CLKPIPE_new, VEXU_SPR_PIX_SET4, XATO_SPR_PIX_RST4, state_new.spr_pipe_a.LESU_SPR_PIPE_A3.qp_old());
      //state_new.spr_pipe_a.LESU_SPR_PIPE_A3.dff22(SACU_CLKPIPE_new, MAME_SPR_PIX_SET3, LUFY_SPR_PIX_RST3, state_new.spr_pipe_a.LEFE_SPR_PIPE_A2.qp_old());
      //state_new.spr_pipe_a.LEFE_SPR_PIPE_A2.dff22(SACU_CLKPIPE_new, LELA_SPR_PIX_SET2, LYDE_SPR_PIX_RST2, state_new.spr_pipe_a.MASO_SPR_PIPE_A1.qp_old());
      //state_new.spr_pipe_a.MASO_SPR_PIPE_A1.dff22(SACU_CLKPIPE_new, MYTO_SPR_PIX_SET1, MADA_SPR_PIX_RST1, state_new.spr_pipe_a.NURO_SPR_PIPE_A0.qp_old());
      //state_new.spr_pipe_a.NURO_SPR_PIPE_A0.dff22(SACU_CLKPIPE_new, PABE_SPR_PIX_SET0, PYZU_SPR_PIX_RST0, state_new.SIG_GND.out_new());

      if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
        state_new.spr_pipe_a = (state_new.spr_pipe_a << 1) | 0;
      }

      if (!bit(VUNE_SPR_PIX_SET7)) state_new.spr_pipe_a |= 0b10000000;
      if (!bit(TUXA_SPR_PIX_SET6)) state_new.spr_pipe_a |= 0b01000000;
      if (!bit(VABY_SPR_PIX_SET5)) state_new.spr_pipe_a |= 0b00100000;
      if (!bit(VEXU_SPR_PIX_SET4)) state_new.spr_pipe_a |= 0b00010000;
      if (!bit(MAME_SPR_PIX_SET3)) state_new.spr_pipe_a |= 0b00001000;
      if (!bit(LELA_SPR_PIX_SET2)) state_new.spr_pipe_a |= 0b00000100;
      if (!bit(MYTO_SPR_PIX_SET1)) state_new.spr_pipe_a |= 0b00000010;
      if (!bit(PABE_SPR_PIX_SET0)) state_new.spr_pipe_a |= 0b00000001;

      if (!bit(XYVE_SPR_PIX_RST7)) state_new.spr_pipe_a &= ~0b10000000;
      if (!bit(TUPE_SPR_PIX_RST6)) state_new.spr_pipe_a &= ~0b01000000;
      if (!bit(XEXU_SPR_PIX_RST5)) state_new.spr_pipe_a &= ~0b00100000;
      if (!bit(XATO_SPR_PIX_RST4)) state_new.spr_pipe_a &= ~0b00010000;
      if (!bit(LUFY_SPR_PIX_RST3)) state_new.spr_pipe_a &= ~0b00001000;
      if (!bit(LYDE_SPR_PIX_RST2)) state_new.spr_pipe_a &= ~0b00000100;
      if (!bit(MADA_SPR_PIX_RST1)) state_new.spr_pipe_a &= ~0b00000010;
      if (!bit(PYZU_SPR_PIX_RST0)) state_new.spr_pipe_a &= ~0b00000001;
    }

    //----------------------------------------
    // Sprite pipe B
    {
      wire MEZU_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, !get_bit(state_new.sprite_pix_b, 0));
      wire RUSY_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, !get_bit(state_new.sprite_pix_b, 1));
      wire MYXA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, !get_bit(state_new.sprite_pix_b, 2));
      wire RANO_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, !get_bit(state_new.sprite_pix_b, 3));
      wire TYGA_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, !get_bit(state_new.sprite_pix_b, 4));
      wire VUME_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, !get_bit(state_new.sprite_pix_b, 5));
      wire TAPO_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, !get_bit(state_new.sprite_pix_b, 6));
      wire TESO_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, !get_bit(state_new.sprite_pix_b, 7));

      wire LOZA_SPR_PIX_DB0n = not1(!get_bit(state_new.sprite_pix_b, 0));
      wire SYBO_SPR_PIX_DB1n = not1(!get_bit(state_new.sprite_pix_b, 1));
      wire LUMO_SPR_PIX_DB2n = not1(!get_bit(state_new.sprite_pix_b, 2));
      wire SOLO_SPR_PIX_DB3n = not1(!get_bit(state_new.sprite_pix_b, 3));
      wire VOBY_SPR_PIX_DB4n = not1(!get_bit(state_new.sprite_pix_b, 4));
      wire WYCO_SPR_PIX_DB5n = not1(!get_bit(state_new.sprite_pix_b, 5));
      wire SERY_SPR_PIX_DB6n = not1(!get_bit(state_new.sprite_pix_b, 6));
      wire SELU_SPR_PIX_DB7n = not1(!get_bit(state_new.sprite_pix_b, 7));

      wire MOFY_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, LOZA_SPR_PIX_DB0n);
      wire RUCA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, SYBO_SPR_PIX_DB1n);
      wire MAJO_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LUMO_SPR_PIX_DB2n);
      wire REHU_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, SOLO_SPR_PIX_DB3n);
      wire WAXO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, VOBY_SPR_PIX_DB4n);
      wire XOLE_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WYCO_SPR_PIX_DB5n);
      wire TABY_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SERY_SPR_PIX_DB6n);
      wire TULA_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, SELU_SPR_PIX_DB7n);

      //state_new.spr_pipe_b.VUPY_SPR_PIPE_B7.dff22(SACU_CLKPIPE_new, TESO_SPR_PIX_SET7, TULA_SPR_PIX_RST7, state_new.spr_pipe_b.VANU_SPR_PIPE_B6.qp_old());
      //state_new.spr_pipe_b.VANU_SPR_PIPE_B6.dff22(SACU_CLKPIPE_new, TAPO_SPR_PIX_SET6, TABY_SPR_PIX_RST6, state_new.spr_pipe_b.WEBA_SPR_PIPE_B5.qp_old());
      //state_new.spr_pipe_b.WEBA_SPR_PIPE_B5.dff22(SACU_CLKPIPE_new, VUME_SPR_PIX_SET5, XOLE_SPR_PIX_RST5, state_new.spr_pipe_b.VARE_SPR_PIPE_B4.qp_old());
      //state_new.spr_pipe_b.VARE_SPR_PIPE_B4.dff22(SACU_CLKPIPE_new, TYGA_SPR_PIX_SET4, WAXO_SPR_PIX_RST4, state_new.spr_pipe_b.PYJO_SPR_PIPE_B3.qp_old());
      //state_new.spr_pipe_b.PYJO_SPR_PIPE_B3.dff22(SACU_CLKPIPE_new, RANO_SPR_PIX_SET3, REHU_SPR_PIX_RST3, state_new.spr_pipe_b.NATY_SPR_PIPE_B2.qp_old());
      //state_new.spr_pipe_b.NATY_SPR_PIPE_B2.dff22(SACU_CLKPIPE_new, MYXA_SPR_PIX_SET2, MAJO_SPR_PIX_RST2, state_new.spr_pipe_b.PEFU_SPR_PIPE_B1.qp_old());
      //state_new.spr_pipe_b.PEFU_SPR_PIPE_B1.dff22(SACU_CLKPIPE_new, RUSY_SPR_PIX_SET1, RUCA_SPR_PIX_RST1, state_new.spr_pipe_b.NYLU_SPR_PIPE_B0.qp_old());
      //state_new.spr_pipe_b.NYLU_SPR_PIPE_B0.dff22(SACU_CLKPIPE_new, MEZU_SPR_PIX_SET0, MOFY_SPR_PIX_RST0, state_new.SIG_GND.out_new());

      if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
        state_new.spr_pipe_b = (state_new.spr_pipe_b << 1) | 0;
      }

      if (!bit(TESO_SPR_PIX_SET7)) state_new.spr_pipe_b |= 0b10000000;
      if (!bit(TAPO_SPR_PIX_SET6)) state_new.spr_pipe_b |= 0b01000000;
      if (!bit(VUME_SPR_PIX_SET5)) state_new.spr_pipe_b |= 0b00100000;
      if (!bit(TYGA_SPR_PIX_SET4)) state_new.spr_pipe_b |= 0b00010000;
      if (!bit(RANO_SPR_PIX_SET3)) state_new.spr_pipe_b |= 0b00001000;
      if (!bit(MYXA_SPR_PIX_SET2)) state_new.spr_pipe_b |= 0b00000100;
      if (!bit(RUSY_SPR_PIX_SET1)) state_new.spr_pipe_b |= 0b00000010;
      if (!bit(MEZU_SPR_PIX_SET0)) state_new.spr_pipe_b |= 0b00000001;

      if (!bit(TULA_SPR_PIX_RST7)) state_new.spr_pipe_b &= ~0b10000000;
      if (!bit(TABY_SPR_PIX_RST6)) state_new.spr_pipe_b &= ~0b01000000;
      if (!bit(XOLE_SPR_PIX_RST5)) state_new.spr_pipe_b &= ~0b00100000;
      if (!bit(WAXO_SPR_PIX_RST4)) state_new.spr_pipe_b &= ~0b00010000;
      if (!bit(REHU_SPR_PIX_RST3)) state_new.spr_pipe_b &= ~0b00001000;
      if (!bit(MAJO_SPR_PIX_RST2)) state_new.spr_pipe_b &= ~0b00000100;
      if (!bit(RUCA_SPR_PIX_RST1)) state_new.spr_pipe_b &= ~0b00000010;
      if (!bit(MOFY_SPR_PIX_RST0)) state_new.spr_pipe_b &= ~0b00000001;
    }

    wire NUNY_WIN_MODE_TRIGp = and2(state_new.win_ctrl.PYNU_WIN_MODE_Ap, !state_new.win_ctrl.NOPA_WIN_MODE_Bp);
    wire NYFO_WIN_MODE_TRIGn = not1(NUNY_WIN_MODE_TRIGp);
    wire SEKO_WIN_FETCH_TRIGp = nor2(!state_new.win_ctrl.RYFA_WIN_FETCHn_A, state_new.win_ctrl.RENE_WIN_FETCHn_B);
    wire MOSU_WIN_MODE_TRIGp = not1(NYFO_WIN_MODE_TRIGn);
    wire SYLO_WIN_HITn = not1(state_new.win_ctrl.RYDY_WIN_HITp);
    wire TUXY_WIN_FIRST_TILEne = nand2(SYLO_WIN_HITn, state_new.win_ctrl.SOVY_WIN_HITp);
    wire SUZU_WIN_FIRST_TILEne = not1(TUXY_WIN_FIRST_TILEne);
    wire ROMO_PRELOAD_DONEn = not1(state_new.tfetch_control.POKY_PRELOAD_LATCHp);
    wire SUVU_PRELOAD_DONE_TRIGn = nand4(!state_new.XYMU_RENDERINGn,
                                          ROMO_PRELOAD_DONEn,
                                          state_new.tfetch_control.NYKA_FETCH_DONEp,
                                          state_new.tfetch_control.PORY_FETCH_DONEp);
    wire TAVE_PRELOAD_DONE_TRIGp = not1(SUVU_PRELOAD_DONE_TRIGn);
    wire TEVO_WIN_FETCH_TRIGp = or3(SEKO_WIN_FETCH_TRIGp, SUZU_WIN_FIRST_TILEne, TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
    wire NYXU_BFETCH_RSTn = nor3(state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp, MOSU_WIN_MODE_TRIGp, TEVO_WIN_FETCH_TRIGp);

    //----------------------------------------
    // Background pipe A
    {
      wire LOZE_PIPE_A_LOADp = not1(NYXU_BFETCH_RSTn);
      wire LAKY_BG_PIX_SET0 = nand2(LOZE_PIPE_A_LOADp, !get_bit(state_new.tile_temp_a, 0));
      wire NYXO_BG_PIX_SET1 = nand2(LOZE_PIPE_A_LOADp, !get_bit(state_new.tile_temp_a, 1));
      wire LOTO_BG_PIX_SET2 = nand2(LOZE_PIPE_A_LOADp, !get_bit(state_new.tile_temp_a, 2));
      wire LYDU_BG_PIX_SET3 = nand2(LOZE_PIPE_A_LOADp, !get_bit(state_new.tile_temp_a, 3));
      wire MYVY_BG_PIX_SET4 = nand2(LOZE_PIPE_A_LOADp, !get_bit(state_new.tile_temp_a, 4));
      wire LODO_BG_PIX_SET5 = nand2(LOZE_PIPE_A_LOADp, !get_bit(state_new.tile_temp_a, 5));
      wire NUTE_BG_PIX_SET6 = nand2(LOZE_PIPE_A_LOADp, !get_bit(state_new.tile_temp_a, 6));
      wire NAJA_BG_PIX_SET7 = nand2(LOZE_PIPE_A_LOADp, !get_bit(state_new.tile_temp_a, 7));

      wire LUHE_BG_PIX_DA0n = not1(!get_bit(state_new.tile_temp_a, 0));
      wire NOLY_BG_PIX_DA1n = not1(!get_bit(state_new.tile_temp_a, 1));
      wire LEKE_BG_PIX_DA2n = not1(!get_bit(state_new.tile_temp_a, 2));
      wire LOMY_BG_PIX_DA3n = not1(!get_bit(state_new.tile_temp_a, 3));
      wire LALA_BG_PIX_DA4n = not1(!get_bit(state_new.tile_temp_a, 4));
      wire LOXA_BG_PIX_DA5n = not1(!get_bit(state_new.tile_temp_a, 5));
      wire NEZE_BG_PIX_DA6n = not1(!get_bit(state_new.tile_temp_a, 6));
      wire NOBO_BG_PIX_DA7n = not1(!get_bit(state_new.tile_temp_a, 7));

      wire LOTY_BG_PIX_RST0 = nand2(LOZE_PIPE_A_LOADp, LUHE_BG_PIX_DA0n);
      wire NEXA_BG_PIX_RST1 = nand2(LOZE_PIPE_A_LOADp, NOLY_BG_PIX_DA1n);
      wire LUTU_BG_PIX_RST2 = nand2(LOZE_PIPE_A_LOADp, LEKE_BG_PIX_DA2n);
      wire LUJA_BG_PIX_RST3 = nand2(LOZE_PIPE_A_LOADp, LOMY_BG_PIX_DA3n);
      wire MOSY_BG_PIX_RST4 = nand2(LOZE_PIPE_A_LOADp, LALA_BG_PIX_DA4n);
      wire LERU_BG_PIX_RST5 = nand2(LOZE_PIPE_A_LOADp, LOXA_BG_PIX_DA5n);
      wire NYHA_BG_PIX_RST6 = nand2(LOZE_PIPE_A_LOADp, NEZE_BG_PIX_DA6n);
      wire NADY_BG_PIX_RST7 = nand2(LOZE_PIPE_A_LOADp, NOBO_BG_PIX_DA7n);

      if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
        state_new.bgw_pipe_a = (state_new.bgw_pipe_a << 1) | 0;
      }

      if (!bit(NAJA_BG_PIX_SET7)) state_new.bgw_pipe_a |= 0b10000000;
      if (!bit(NUTE_BG_PIX_SET6)) state_new.bgw_pipe_a |= 0b01000000;
      if (!bit(LODO_BG_PIX_SET5)) state_new.bgw_pipe_a |= 0b00100000;
      if (!bit(MYVY_BG_PIX_SET4)) state_new.bgw_pipe_a |= 0b00010000;
      if (!bit(LYDU_BG_PIX_SET3)) state_new.bgw_pipe_a |= 0b00001000;
      if (!bit(LOTO_BG_PIX_SET2)) state_new.bgw_pipe_a |= 0b00000100;
      if (!bit(NYXO_BG_PIX_SET1)) state_new.bgw_pipe_a |= 0b00000010;
      if (!bit(LAKY_BG_PIX_SET0)) state_new.bgw_pipe_a |= 0b00000001;

      if (!bit(NADY_BG_PIX_RST7)) state_new.bgw_pipe_a &= ~0b10000000;
      if (!bit(NYHA_BG_PIX_RST6)) state_new.bgw_pipe_a &= ~0b01000000;
      if (!bit(LERU_BG_PIX_RST5)) state_new.bgw_pipe_a &= ~0b00100000;
      if (!bit(MOSY_BG_PIX_RST4)) state_new.bgw_pipe_a &= ~0b00010000;
      if (!bit(LUJA_BG_PIX_RST3)) state_new.bgw_pipe_a &= ~0b00001000;
      if (!bit(LUTU_BG_PIX_RST2)) state_new.bgw_pipe_a &= ~0b00000100;
      if (!bit(NEXA_BG_PIX_RST1)) state_new.bgw_pipe_a &= ~0b00000010;
      if (!bit(LOTY_BG_PIX_RST0)) state_new.bgw_pipe_a &= ~0b00000001;
    }

    //----------------------------------------
    // Background pipe B
    {
      wire LUXA_PIPE_B_LOADp = not1(NYXU_BFETCH_RSTn);
      wire TUXE_BG_PIX_SET0 = nand2(LUXA_PIPE_B_LOADp, get_bit(state_new.tile_temp_b, 0));
      wire SOLY_BG_PIX_SET1 = nand2(LUXA_PIPE_B_LOADp, get_bit(state_new.tile_temp_b, 1));
      wire RUCE_BG_PIX_SET2 = nand2(LUXA_PIPE_B_LOADp, get_bit(state_new.tile_temp_b, 2));
      wire RYJA_BG_PIX_SET3 = nand2(LUXA_PIPE_B_LOADp, get_bit(state_new.tile_temp_b, 3));
      wire RUTO_BG_PIX_SET4 = nand2(LUXA_PIPE_B_LOADp, get_bit(state_new.tile_temp_b, 4));
      wire RAJA_BG_PIX_SET5 = nand2(LUXA_PIPE_B_LOADp, get_bit(state_new.tile_temp_b, 5));
      wire RAJO_BG_PIX_SET6 = nand2(LUXA_PIPE_B_LOADp, get_bit(state_new.tile_temp_b, 6));
      wire RAGA_BG_PIX_SET7 = nand2(LUXA_PIPE_B_LOADp, get_bit(state_new.tile_temp_b, 7));

      wire TOSA_BG_PIX_DB0n = not1(get_bit(state_new.tile_temp_b, 0));
      wire RUCO_BG_PIX_DB1n = not1(get_bit(state_new.tile_temp_b, 1));
      wire TYCE_BG_PIX_DB2n = not1(get_bit(state_new.tile_temp_b, 2));
      wire REVY_BG_PIX_DB3n = not1(get_bit(state_new.tile_temp_b, 3));
      wire RYGA_BG_PIX_DB4n = not1(get_bit(state_new.tile_temp_b, 4));
      wire RYLE_BG_PIX_DB5n = not1(get_bit(state_new.tile_temp_b, 5));
      wire RAPU_BG_PIX_DB6n = not1(get_bit(state_new.tile_temp_b, 6));
      wire SOJA_BG_PIX_DB7n = not1(get_bit(state_new.tile_temp_b, 7));

      wire SEJA_BG_PIX_RST0 = nand2(LUXA_PIPE_B_LOADp, TOSA_BG_PIX_DB0n);
      wire SENO_BG_PIX_RST1 = nand2(LUXA_PIPE_B_LOADp, RUCO_BG_PIX_DB1n);
      wire SURE_BG_PIX_RST2 = nand2(LUXA_PIPE_B_LOADp, TYCE_BG_PIX_DB2n);
      wire SEBO_BG_PIX_RST3 = nand2(LUXA_PIPE_B_LOADp, REVY_BG_PIX_DB3n);
      wire SUCA_BG_PIX_RST4 = nand2(LUXA_PIPE_B_LOADp, RYGA_BG_PIX_DB4n);
      wire SYWE_BG_PIX_RST5 = nand2(LUXA_PIPE_B_LOADp, RYLE_BG_PIX_DB5n);
      wire SUPU_BG_PIX_RST6 = nand2(LUXA_PIPE_B_LOADp, RAPU_BG_PIX_DB6n);
      wire RYJY_BG_PIX_RST7 = nand2(LUXA_PIPE_B_LOADp, SOJA_BG_PIX_DB7n);

      //state_new.bgw_pipe_b.SOHU_BGW_PIPE_B7.dff22(SACU_CLKPIPE_new, RAGA_BG_PIX_SET7, RYJY_BG_PIX_RST7, state_new.bgw_pipe_b.RALU_BGW_PIPE_B6.qp_old());
      //state_new.bgw_pipe_b.RALU_BGW_PIPE_B6.dff22(SACU_CLKPIPE_new, RAJO_BG_PIX_SET6, SUPU_BG_PIX_RST6, state_new.bgw_pipe_b.SETU_BGW_PIPE_B5.qp_old());
      //state_new.bgw_pipe_b.SETU_BGW_PIPE_B5.dff22(SACU_CLKPIPE_new, RAJA_BG_PIX_SET5, SYWE_BG_PIX_RST5, state_new.bgw_pipe_b.SOBO_BGW_PIPE_B4.qp_old());
      //state_new.bgw_pipe_b.SOBO_BGW_PIPE_B4.dff22(SACU_CLKPIPE_new, RUTO_BG_PIX_SET4, SUCA_BG_PIX_RST4, state_new.bgw_pipe_b.RYSA_BGW_PIPE_B3.qp_old());
      //state_new.bgw_pipe_b.RYSA_BGW_PIPE_B3.dff22(SACU_CLKPIPE_new, RYJA_BG_PIX_SET3, SEBO_BG_PIX_RST3, state_new.bgw_pipe_b.SADY_BGW_PIPE_B2.qp_old());
      //state_new.bgw_pipe_b.SADY_BGW_PIPE_B2.dff22(SACU_CLKPIPE_new, RUCE_BG_PIX_SET2, SURE_BG_PIX_RST2, state_new.bgw_pipe_b.TACA_BGW_PIPE_B1.qp_old());
      //state_new.bgw_pipe_b.TACA_BGW_PIPE_B1.dff22(SACU_CLKPIPE_new, SOLY_BG_PIX_SET1, SENO_BG_PIX_RST1, state_new.bgw_pipe_b.TOMY_BGW_PIPE_B0.qp_old());
      //state_new.bgw_pipe_b.TOMY_BGW_PIPE_B0.dff22(SACU_CLKPIPE_new, TUXE_BG_PIX_SET0, SEJA_BG_PIX_RST0, state_new.SIG_GND.out_new());

      if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
        state_new.bgw_pipe_b = (state_new.bgw_pipe_b << 1) | 0;
      }

      if (!bit(RAGA_BG_PIX_SET7)) state_new.bgw_pipe_b |= 0b10000000;
      if (!bit(RAJO_BG_PIX_SET6)) state_new.bgw_pipe_b |= 0b01000000;
      if (!bit(RAJA_BG_PIX_SET5)) state_new.bgw_pipe_b |= 0b00100000;
      if (!bit(RUTO_BG_PIX_SET4)) state_new.bgw_pipe_b |= 0b00010000;
      if (!bit(RYJA_BG_PIX_SET3)) state_new.bgw_pipe_b |= 0b00001000;
      if (!bit(RUCE_BG_PIX_SET2)) state_new.bgw_pipe_b |= 0b00000100;
      if (!bit(SOLY_BG_PIX_SET1)) state_new.bgw_pipe_b |= 0b00000010;
      if (!bit(TUXE_BG_PIX_SET0)) state_new.bgw_pipe_b |= 0b00000001;

      if (!bit(RYJY_BG_PIX_RST7)) state_new.bgw_pipe_b &= ~0b10000000;
      if (!bit(SUPU_BG_PIX_RST6)) state_new.bgw_pipe_b &= ~0b01000000;
      if (!bit(SYWE_BG_PIX_RST5)) state_new.bgw_pipe_b &= ~0b00100000;
      if (!bit(SUCA_BG_PIX_RST4)) state_new.bgw_pipe_b &= ~0b00010000;
      if (!bit(SEBO_BG_PIX_RST3)) state_new.bgw_pipe_b &= ~0b00001000;
      if (!bit(SURE_BG_PIX_RST2)) state_new.bgw_pipe_b &= ~0b00000100;
      if (!bit(SENO_BG_PIX_RST1)) state_new.bgw_pipe_b &= ~0b00000010;
      if (!bit(SEJA_BG_PIX_RST0)) state_new.bgw_pipe_b &= ~0b00000001;
    }

    //----------------------------------------
    // Mask pipe
    {
      wire TEDE_MASK_PIPE_SET0 = nand2(LESY_SPRITE_MASK0p, get_bit(state_new.oam_temp_b, 7));
      wire XALA_MASK_PIPE_SET1 = nand2(LOTA_SPRITE_MASK1p, get_bit(state_new.oam_temp_b, 7));
      wire TYRA_MASK_PIPE_SET2 = nand2(LYKU_SPRITE_MASK2p, get_bit(state_new.oam_temp_b, 7));
      wire XYRU_MASK_PIPE_SET3 = nand2(ROBY_SPRITE_MASK3p, get_bit(state_new.oam_temp_b, 7));
      wire XUKU_MASK_PIPE_SET4 = nand2(TYTA_SPRITE_MASK4p, get_bit(state_new.oam_temp_b, 7));
      wire XELY_MASK_PIPE_SET5 = nand2(TYCO_SPRITE_MASK5p, get_bit(state_new.oam_temp_b, 7));
      wire TYKO_MASK_PIPE_SET6 = nand2(SOKA_SPRITE_MASK6p, get_bit(state_new.oam_temp_b, 7));
      wire TUWU_MASK_PIPE_SET7 = nand2(XOVU_SPRITE_MASK7p, get_bit(state_new.oam_temp_b, 7));

      wire XOGA_MASK_PIPE_DB7n = not1(get_bit(state_new.oam_temp_b, 7));
      wire XURA_MASK_PIPE_DB7n = not1(get_bit(state_new.oam_temp_b, 7));
      wire TAJO_MASK_PIPE_DB7n = not1(get_bit(state_new.oam_temp_b, 7));
      wire XENU_MASK_PIPE_DB7n = not1(get_bit(state_new.oam_temp_b, 7));
      wire XYKE_MASK_PIPE_DB7n = not1(get_bit(state_new.oam_temp_b, 7));
      wire XABA_MASK_PIPE_DB7n = not1(get_bit(state_new.oam_temp_b, 7));
      wire TAFU_MASK_PIPE_DB7n = not1(get_bit(state_new.oam_temp_b, 7));
      wire XUHO_MASK_PIPE_DB7n = not1(get_bit(state_new.oam_temp_b, 7));

      wire WOKA_MASK_PIPE_RST0 = nand2(LESY_SPRITE_MASK0p, XOGA_MASK_PIPE_DB7n);
      wire WEDE_MASK_PIPE_RST1 = nand2(LOTA_SPRITE_MASK1p, XURA_MASK_PIPE_DB7n);
      wire TUFO_MASK_PIPE_RST2 = nand2(LYKU_SPRITE_MASK2p, TAJO_MASK_PIPE_DB7n);
      wire WEVO_MASK_PIPE_RST3 = nand2(ROBY_SPRITE_MASK3p, XENU_MASK_PIPE_DB7n);
      wire WEDY_MASK_PIPE_RST4 = nand2(TYTA_SPRITE_MASK4p, XYKE_MASK_PIPE_DB7n);
      wire WUJA_MASK_PIPE_RST5 = nand2(TYCO_SPRITE_MASK5p, XABA_MASK_PIPE_DB7n);
      wire TENA_MASK_PIPE_RST6 = nand2(SOKA_SPRITE_MASK6p, TAFU_MASK_PIPE_DB7n);
      wire WUBU_MASK_PIPE_RST7 = nand2(XOVU_SPRITE_MASK7p, XUHO_MASK_PIPE_DB7n);

      //state_new.mask_pipe.VAVA_MASK_PIPE_7.dff22(SACU_CLKPIPE_new, TUWU_MASK_PIPE_SET7, WUBU_MASK_PIPE_RST7, state_new.mask_pipe.VUMO_MASK_PIPE_6.qp_old());
      //state_new.mask_pipe.VUMO_MASK_PIPE_6.dff22(SACU_CLKPIPE_new, TYKO_MASK_PIPE_SET6, TENA_MASK_PIPE_RST6, state_new.mask_pipe.WODA_MASK_PIPE_5.qp_old());
      //state_new.mask_pipe.WODA_MASK_PIPE_5.dff22(SACU_CLKPIPE_new, XELY_MASK_PIPE_SET5, WUJA_MASK_PIPE_RST5, state_new.mask_pipe.XETE_MASK_PIPE_4.qp_old());
      //state_new.mask_pipe.XETE_MASK_PIPE_4.dff22(SACU_CLKPIPE_new, XUKU_MASK_PIPE_SET4, WEDY_MASK_PIPE_RST4, state_new.mask_pipe.WYFU_MASK_PIPE_3.qp_old());
      //state_new.mask_pipe.WYFU_MASK_PIPE_3.dff22(SACU_CLKPIPE_new, XYRU_MASK_PIPE_SET3, WEVO_MASK_PIPE_RST3, state_new.mask_pipe.VOSA_MASK_PIPE_2.qp_old());
      //state_new.mask_pipe.VOSA_MASK_PIPE_2.dff22(SACU_CLKPIPE_new, TYRA_MASK_PIPE_SET2, TUFO_MASK_PIPE_RST2, state_new.mask_pipe.WURU_MASK_PIPE_1.qp_old());
      //state_new.mask_pipe.WURU_MASK_PIPE_1.dff22(SACU_CLKPIPE_new, XALA_MASK_PIPE_SET1, WEDE_MASK_PIPE_RST1, state_new.mask_pipe.VEZO_MASK_PIPE_0.qp_old());
      //state_new.mask_pipe.VEZO_MASK_PIPE_0.dff22(SACU_CLKPIPE_new, TEDE_MASK_PIPE_SET0, WOKA_MASK_PIPE_RST0, state_new.SIG_VCC.out_new());

      if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
        state_new.mask_pipe = (state_new.mask_pipe << 1) | 1;
      }

      if (!bit(TUWU_MASK_PIPE_SET7)) state_new.mask_pipe |= 0b10000000;
      if (!bit(TYKO_MASK_PIPE_SET6)) state_new.mask_pipe |= 0b01000000;
      if (!bit(XELY_MASK_PIPE_SET5)) state_new.mask_pipe |= 0b00100000;
      if (!bit(XUKU_MASK_PIPE_SET4)) state_new.mask_pipe |= 0b00010000;
      if (!bit(XYRU_MASK_PIPE_SET3)) state_new.mask_pipe |= 0b00001000;
      if (!bit(TYRA_MASK_PIPE_SET2)) state_new.mask_pipe |= 0b00000100;
      if (!bit(XALA_MASK_PIPE_SET1)) state_new.mask_pipe |= 0b00000010;
      if (!bit(TEDE_MASK_PIPE_SET0)) state_new.mask_pipe |= 0b00000001;

      if (!bit(WUBU_MASK_PIPE_RST7)) state_new.mask_pipe &= ~0b10000000;
      if (!bit(TENA_MASK_PIPE_RST6)) state_new.mask_pipe &= ~0b01000000;
      if (!bit(WUJA_MASK_PIPE_RST5)) state_new.mask_pipe &= ~0b00100000;
      if (!bit(WEDY_MASK_PIPE_RST4)) state_new.mask_pipe &= ~0b00010000;
      if (!bit(WEVO_MASK_PIPE_RST3)) state_new.mask_pipe &= ~0b00001000;
      if (!bit(TUFO_MASK_PIPE_RST2)) state_new.mask_pipe &= ~0b00000100;
      if (!bit(WEDE_MASK_PIPE_RST1)) state_new.mask_pipe &= ~0b00000010;
      if (!bit(WOKA_MASK_PIPE_RST0)) state_new.mask_pipe &= ~0b00000001;

    }

    //----------------------------------------
    // Pal pipe
    {
      wire PUME_PAL_PIPE_SET0 = nand2(LESY_SPRITE_MASK0p, get_bit(state_new.oam_temp_b, 4));
      wire SORO_PAL_PIPE_SET1 = nand2(LOTA_SPRITE_MASK1p, get_bit(state_new.oam_temp_b, 4));
      wire PAMO_PAL_PIPE_SET2 = nand2(LYKU_SPRITE_MASK2p, get_bit(state_new.oam_temp_b, 4));
      wire SUKY_PAL_PIPE_SET3 = nand2(ROBY_SPRITE_MASK3p, get_bit(state_new.oam_temp_b, 4));
      wire RORA_PAL_PIPE_SET4 = nand2(TYTA_SPRITE_MASK4p, get_bit(state_new.oam_temp_b, 4));
      wire MENE_PAL_PIPE_SET5 = nand2(TYCO_SPRITE_MASK5p, get_bit(state_new.oam_temp_b, 4));
      wire LUKE_PAL_PIPE_SET6 = nand2(SOKA_SPRITE_MASK6p, get_bit(state_new.oam_temp_b, 4));
      wire LAMY_PAL_PIPE_SET7 = nand2(XOVU_SPRITE_MASK7p, get_bit(state_new.oam_temp_b, 4));

      wire SYPY_PAL_PIPE_DB4n = not1(get_bit(state_new.oam_temp_b, 4));
      wire TOTU_PAL_PIPE_DB4n = not1(get_bit(state_new.oam_temp_b, 4));
      wire NARO_PAL_PIPE_DB4n = not1(get_bit(state_new.oam_temp_b, 4));
      wire WEXY_PAL_PIPE_DB4n = not1(get_bit(state_new.oam_temp_b, 4));
      wire RYZY_PAL_PIPE_DB4n = not1(get_bit(state_new.oam_temp_b, 4));
      wire RYFE_PAL_PIPE_DB4n = not1(get_bit(state_new.oam_temp_b, 4));
      wire LADY_PAL_PIPE_DB4n = not1(get_bit(state_new.oam_temp_b, 4));
      wire LAFY_PAL_PIPE_DB4n = not1(get_bit(state_new.oam_temp_b, 4));

      wire SUCO_PAL_PIPE_RST0 = nand2(LESY_SPRITE_MASK0p, SYPY_PAL_PIPE_DB4n);
      wire TAFA_PAL_PIPE_RST1 = nand2(LOTA_SPRITE_MASK1p, TOTU_PAL_PIPE_DB4n);
      wire PYZY_PAL_PIPE_RST2 = nand2(LYKU_SPRITE_MASK2p, NARO_PAL_PIPE_DB4n);
      wire TOWA_PAL_PIPE_RST3 = nand2(ROBY_SPRITE_MASK3p, WEXY_PAL_PIPE_DB4n);
      wire RUDU_PAL_PIPE_RST4 = nand2(TYTA_SPRITE_MASK4p, RYZY_PAL_PIPE_DB4n);
      wire PAZO_PAL_PIPE_RST5 = nand2(TYCO_SPRITE_MASK5p, RYFE_PAL_PIPE_DB4n);
      wire LOWA_PAL_PIPE_RST6 = nand2(SOKA_SPRITE_MASK6p, LADY_PAL_PIPE_DB4n);
      wire LUNU_PAL_PIPE_RST7 = nand2(XOVU_SPRITE_MASK7p, LAFY_PAL_PIPE_DB4n);

      //state_new.pal_pipe.LYME_PAL_PIPE_D7.dff22(SACU_CLKPIPE_new, LAMY_PAL_PIPE_SET7, LUNU_PAL_PIPE_RST7, state_new.pal_pipe.MODA_PAL_PIPE_D6.qp_old());
      //state_new.pal_pipe.MODA_PAL_PIPE_D6.dff22(SACU_CLKPIPE_new, LUKE_PAL_PIPE_SET6, LOWA_PAL_PIPE_RST6, state_new.pal_pipe.NUKE_PAL_PIPE_D5.qp_old());
      //state_new.pal_pipe.NUKE_PAL_PIPE_D5.dff22(SACU_CLKPIPE_new, MENE_PAL_PIPE_SET5, PAZO_PAL_PIPE_RST5, state_new.pal_pipe.PALU_PAL_PIPE_D4.qp_old());
      //state_new.pal_pipe.PALU_PAL_PIPE_D4.dff22(SACU_CLKPIPE_new, RORA_PAL_PIPE_SET4, RUDU_PAL_PIPE_RST4, state_new.pal_pipe.SOMY_PAL_PIPE_D3.qp_old());
      //state_new.pal_pipe.SOMY_PAL_PIPE_D3.dff22(SACU_CLKPIPE_new, SUKY_PAL_PIPE_SET3, TOWA_PAL_PIPE_RST3, state_new.pal_pipe.ROSA_PAL_PIPE_D2.qp_old());
      //state_new.pal_pipe.ROSA_PAL_PIPE_D2.dff22(SACU_CLKPIPE_new, PAMO_PAL_PIPE_SET2, PYZY_PAL_PIPE_RST2, state_new.pal_pipe.SATA_PAL_PIPE_D1.qp_old());
      //state_new.pal_pipe.SATA_PAL_PIPE_D1.dff22(SACU_CLKPIPE_new, SORO_PAL_PIPE_SET1, TAFA_PAL_PIPE_RST1, state_new.pal_pipe.RUGO_PAL_PIPE_D0.qp_old());
      //state_new.pal_pipe.RUGO_PAL_PIPE_D0.dff22(SACU_CLKPIPE_new, PUME_PAL_PIPE_SET0, SUCO_PAL_PIPE_RST0, state_new.SIG_GND.out_new());

      if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
        state_new.pal_pipe = (state_new.pal_pipe << 1) | 0;
      }

      if (!bit(LAMY_PAL_PIPE_SET7)) state_new.pal_pipe |= 0b10000000;
      if (!bit(LUKE_PAL_PIPE_SET6)) state_new.pal_pipe |= 0b01000000;
      if (!bit(MENE_PAL_PIPE_SET5)) state_new.pal_pipe |= 0b00100000;
      if (!bit(RORA_PAL_PIPE_SET4)) state_new.pal_pipe |= 0b00010000;
      if (!bit(SUKY_PAL_PIPE_SET3)) state_new.pal_pipe |= 0b00001000;
      if (!bit(PAMO_PAL_PIPE_SET2)) state_new.pal_pipe |= 0b00000100;
      if (!bit(SORO_PAL_PIPE_SET1)) state_new.pal_pipe |= 0b00000010;
      if (!bit(PUME_PAL_PIPE_SET0)) state_new.pal_pipe |= 0b00000001;

      if (!bit(LUNU_PAL_PIPE_RST7)) state_new.pal_pipe &= ~0b10000000;
      if (!bit(LOWA_PAL_PIPE_RST6)) state_new.pal_pipe &= ~0b01000000;
      if (!bit(PAZO_PAL_PIPE_RST5)) state_new.pal_pipe &= ~0b00100000;
      if (!bit(RUDU_PAL_PIPE_RST4)) state_new.pal_pipe &= ~0b00010000;
      if (!bit(TOWA_PAL_PIPE_RST3)) state_new.pal_pipe &= ~0b00001000;
      if (!bit(PYZY_PAL_PIPE_RST2)) state_new.pal_pipe &= ~0b00000100;
      if (!bit(TAFA_PAL_PIPE_RST1)) state_new.pal_pipe &= ~0b00000010;
      if (!bit(SUCO_PAL_PIPE_RST0)) state_new.pal_pipe &= ~0b00000001;
    }

    //----------------------------------------
    // Pipe merge

    wire RAJY_PIX_BG_LOp = and2(get_bit(state_new.bgw_pipe_a, 7), !get_bit(state_new.reg_lcdc, 0));
    wire TADE_PIX_BG_HIp = and2(get_bit(state_new.bgw_pipe_b, 7), !get_bit(state_new.reg_lcdc, 0));
    wire XULA_PIX_SP_LOp = and2(!get_bit(state_new.reg_lcdc, 1),   get_bit(state_new.spr_pipe_a, 7));
    wire WOXA_PIX_SP_HIp = and2(!get_bit(state_new.reg_lcdc, 1),   get_bit(state_new.spr_pipe_b, 7));

    wire NULY_PIX_SP_MASKn = nor2(WOXA_PIX_SP_HIp, XULA_PIX_SP_LOp);

    wire RYFU_MASK_BG0 = and2(RAJY_PIX_BG_LOp, get_bit(state_new.mask_pipe, 7));
    wire RUTA_MASK_BG1 = and2(TADE_PIX_BG_HIp, get_bit(state_new.mask_pipe, 7));
    wire POKA_BGPIXELn = nor3(NULY_PIX_SP_MASKn, RUTA_MASK_BG1, RYFU_MASK_BG0);

    wire LOME_PAL_PIPE_7n = not1(get_bit(state_new.pal_pipe, 7));
    wire LAFU_OBP0PIXELn = nand2(LOME_PAL_PIPE_7n, POKA_BGPIXELn);
    wire LEKA_OBP1PIXELn = nand2(get_bit(state_new.pal_pipe, 7), POKA_BGPIXELn);

    //----------------------------------------
    // Sprite palette 0 lookup

    wire WELE_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
    wire WOLO_PIX_SP_LOp = not1(WELE_PIX_SP_LOn);
    wire VUMU_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
    wire WYRU_PIX_SP_HIp = not1(VUMU_PIX_SP_HIn);

    wire LAVA_MASK_OPB0 = not1(LAFU_OBP0PIXELn);

    wire VUGO_PAL_OBP0A = and3(VUMU_PIX_SP_HIn, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
    wire VOLO_PAL_OBP0B = and3(VUMU_PIX_SP_HIn, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm
    wire VATA_PAL_OBP0C = and3(WYRU_PIX_SP_HIp, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
    wire VYRO_PAL_OBP0D = and3(WYRU_PIX_SP_HIp, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm

    wire WUFU_COL_OBP0_HI = amux4(get_bit(state_new.reg_obp0, 7), VYRO_PAL_OBP0D,
                                  get_bit(state_new.reg_obp0, 5), VATA_PAL_OBP0C,
                                  get_bit(state_new.reg_obp0, 3), VOLO_PAL_OBP0B,
                                  get_bit(state_new.reg_obp0, 1), VUGO_PAL_OBP0A);

    wire WALY_COL_OBP0_LO = amux4(get_bit(state_new.reg_obp0, 6), VYRO_PAL_OBP0D,
                                  get_bit(state_new.reg_obp0, 4), VATA_PAL_OBP0C,
                                  get_bit(state_new.reg_obp0, 2), VOLO_PAL_OBP0B,
                                  get_bit(state_new.reg_obp0, 0), VUGO_PAL_OBP0A);

    //----------------------------------------
    // Sprite palette 1 lookup

    wire MABY_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
    wire LYLE_PIX_SP_LOp = not1(MABY_PIX_SP_LOn);
    wire MEXA_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
    wire LOZO_PIX_SP_HIp = not1(MEXA_PIX_SP_HIn);

    wire LUKU_MASK_OBP1 = not1(LEKA_OBP1PIXELn);

    wire LOPU_PAL_OBP1A = and3(MEXA_PIX_SP_HIn, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
    wire LYKY_PAL_OBP1B = and3(MEXA_PIX_SP_HIn, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm
    wire LARU_PAL_OBP1C = and3(LOZO_PIX_SP_HIp, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
    wire LEDO_PAL_OBP1D = and3(LOZO_PIX_SP_HIp, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm

    wire MOKA_COL_OBP1_HI = amux4(get_bit(state_new.reg_obp1, 7), LEDO_PAL_OBP1D,
                                  get_bit(state_new.reg_obp1, 5), LARU_PAL_OBP1C,
                                  get_bit(state_new.reg_obp1, 3), LYKY_PAL_OBP1B,
                                  get_bit(state_new.reg_obp1, 1), LOPU_PAL_OBP1A);

    wire MUFA_COL_OBP1_LO = amux4(LEDO_PAL_OBP1D, get_bit(state_new.reg_obp1, 6),
                                  LARU_PAL_OBP1C, get_bit(state_new.reg_obp1, 4),
                                  LYKY_PAL_OBP1B, get_bit(state_new.reg_obp1, 2),
                                  LOPU_PAL_OBP1A, get_bit(state_new.reg_obp1, 0));

    //----------------------------------------
    // Background/window palette lookup

    wire SOBA_PIX_BG_LOn = not1(RAJY_PIX_BG_LOp);
    wire NUPO_PIX_BG_LOp = not1(SOBA_PIX_BG_LOn);
    wire VYCO_PIX_BG_HIn = not1(TADE_PIX_BG_HIp);
    wire NALE_PIX_BG_HIp = not1(VYCO_PIX_BG_HIn);

    wire MUVE_MASK_BGP = not1(POKA_BGPIXELn);

    wire POBU_PAL_BGPA = and3(VYCO_PIX_BG_HIn, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
    wire NUXO_PAL_BGPB = and3(VYCO_PIX_BG_HIn, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm
    wire NUMA_PAL_BGPC = and3(NALE_PIX_BG_HIp, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
    wire NYPO_PAL_BGPD = and3(NALE_PIX_BG_HIp, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm

    wire NELO_COL_BG_LO = amux4(NYPO_PAL_BGPD, get_bit(state_new.reg_bgp, 6),
                                NUMA_PAL_BGPC, get_bit(state_new.reg_bgp, 4),
                                NUXO_PAL_BGPB, get_bit(state_new.reg_bgp, 2),
                                POBU_PAL_BGPA, get_bit(state_new.reg_bgp, 0));

    wire NURA_COL_BG_HI = amux4(get_bit(state_new.reg_bgp, 7), NYPO_PAL_BGPD,
                                get_bit(state_new.reg_bgp, 5), NUMA_PAL_BGPC,
                                get_bit(state_new.reg_bgp, 3), NUXO_PAL_BGPB,
                                get_bit(state_new.reg_bgp, 1), POBU_PAL_BGPA);

    //----------------------------------------
    // Pixel merge and send

    wire PERO_COL_LO = or3(NELO_COL_BG_LO, WALY_COL_OBP0_LO, MUFA_COL_OBP1_LO);
    wire PATY_COL_HI = or3(NURA_COL_BG_HI, WUFU_COL_OBP0_HI, MOKA_COL_OBP1_HI);

    state_new.lcd.REMY_LD0n = not1(PERO_COL_LO);
    state_new.lcd.RAVO_LD1n = not1(PATY_COL_HI);
  }

#if 0
  uint8_t tpix_a = (uint8_t)~state_new.tile_temp_a;
  uint8_t tpix_b = (uint8_t)state_new.tile_temp_b;
  uint8_t spix_a = (uint8_t)~state_new.sprite_pix_a;
  uint8_t spix_b = (uint8_t)~state_new.sprite_pix_b;

  uint8_t spipe_a = (uint8_t)state_new.spr_pipe_a;
  uint8_t spipe_b = (uint8_t)state_new.spr_pipe_b;
  uint8_t bpipe_a = (uint8_t)state_new.bgw_pipe_a;
  uint8_t bpipe_b = (uint8_t)state_new.bgw_pipe_b;
  uint8_t mpipe   = (uint8_t)state_new.mask_pipe;
  uint8_t ppipe   = (uint8_t)state_new.pal_pipe;

  if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
    spipe_a = (spipe_a << 1) | 0;
    spipe_b = (spipe_b << 1) | 0;
    bpipe_a = (bpipe_a << 1) | 0;
    bpipe_b = (bpipe_b << 1) | 0;
    mpipe   = (mpipe   << 1) | 1;
    ppipe   = (ppipe   << 1) | 0;
  }
    
  if (state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp || (state_new.win_ctrl.PYNU_WIN_MODE_Ap && !state_new.win_ctrl.NOPA_WIN_MODE_Bp) || TEVO_WIN_FETCH_TRIGp_new) {
    bpipe_a = tpix_a;
    bpipe_b = tpix_b;
  }

  if (state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp) {
    auto smask = (spipe_a | spipe_b);
    spipe_a = (spipe_a & smask) | (spix_a & ~smask);
    spipe_b = (spipe_b & smask) | (spix_b & ~smask);
    mpipe = uint8_t(get_bit(state_new.oam_temp_b, 7) ? mpipe | ~smask : mpipe & smask);
    ppipe = uint8_t(get_bit(state_new.oam_temp_b, 4) ? ppipe | ~smask : ppipe & smask);
  }

  state_new.spr_pipe_a = spipe_a;
  state_new.spr_pipe_b = spipe_b;
  state_new.bgw_pipe_a = bpipe_a;
  state_new.bgw_pipe_b = bpipe_b;
  state_new.mask_pipe = mpipe;
  state_new.pal_pipe = ppipe;

  //----------------------------------------
  // Pipe merge and output

  wire PIX_BG_LOp = get_bit(state_new.bgw_pipe_a, 7) && !get_bit(state_new.reg_lcdc, 0);
  wire PIX_BG_HIp = get_bit(state_new.bgw_pipe_b, 7) && !get_bit(state_new.reg_lcdc, 0);
  wire PIX_SP_LOp = get_bit(state_new.spr_pipe_a, 7) && !get_bit(state_new.reg_lcdc, 1);
  wire PIX_SP_HIp = get_bit(state_new.spr_pipe_b, 7) && !get_bit(state_new.reg_lcdc, 1);

  auto pal_idx = 0;
  auto pal = 0;

  const auto bgp  = state_new.reg_bgp ^ 0xFF;
  const auto obp0 = state_new.reg_obp0 ^ 0xFF;
  const auto obp1 = state_new.reg_obp1 ^ 0xFF;

  if (PIX_SP_HIp || PIX_SP_LOp) {
    pal_idx = pack(PIX_SP_LOp, PIX_SP_HIp);
    pal = get_bit(state_new.pal_pipe, 7) ? obp1 : obp0;
  }
  else {
    pal_idx = pack(PIX_BG_LOp, PIX_BG_HIp);
    pal = bgp;
  }

  state_new.lcd.REMY_LD0n = (pal >> (pal_idx * 2 + 0)) & 1;
  state_new.lcd.RAVO_LD1n = (pal >> (pal_idx * 2 + 1)) & 1;
#endif

  //----------------------------------------
  // LCD pins

  if (!get_bit(state_new.reg_lcdc, 7)) {
    pins.lcd.PIN_52_LCD_CNTRL.state = !state_new.lcd.SYGU_LINE_STROBE && !state_new.lcd.RUTU_x113p;

    if (state_old.lcd.RUTU_x113p && !state_new.lcd.RUTU_x113p) state_new.lcd.LUCA_LINE_EVENp.state = !state_new.lcd.LUCA_LINE_EVENp;
    if (!state_old.lcd.POPU_y144p && state_new.lcd.POPU_y144p) state_new.lcd.NAPO_FRAME_EVENp.state = !state_new.lcd.NAPO_FRAME_EVENp;
    pins.lcd.PIN_56_LCD_FLIPS.state = state_new.lcd.NAPO_FRAME_EVENp ^ state_new.lcd.LUCA_LINE_EVENp;

    if (state_old.lcd.NYPE_x113p && !state_new.lcd.NYPE_x113p) {
      state_new.lcd.MEDA_VSYNC_OUTn.state = reg_ly_new == 0;
    }

    pins.lcd.PIN_57_LCD_VSYNC.state = !state_new.lcd.MEDA_VSYNC_OUTn;

    if (state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp && state_new.lcd.PAHO_X_8_SYNC) {
      state_new.lcd.POME = 0;
      state_new.lcd.RUJU = 1;
      state_new.lcd.POFY = 0;
    }
    else if (state_new.sprite_scanner.AVAP_SCAN_DONE_TRIGp) {
      state_new.lcd.POME = 0;
      state_new.lcd.RUJU = 0;
      state_new.lcd.POFY = 1;
    }
    else if (state_new.lcd.PAHO_X_8_SYNC) {
      state_new.lcd.POME = 1;
      state_new.lcd.RUJU = 1;
      state_new.lcd.POFY = 0;
    }

    pins.lcd.PIN_50_LCD_DATA1.state = state_new.lcd.RAVO_LD1n;
    pins.lcd.PIN_51_LCD_DATA0.state = state_new.lcd.REMY_LD0n;
    pins.lcd.PIN_54_LCD_HSYNC.state = !state_new.lcd.POFY;
    pins.lcd.PIN_55_LCD_LATCH.state = !state_new.lcd.RUTU_x113p;

    if (get_bit(state_new.pix_count, 0) && get_bit(state_new.pix_count, 3)) state_new.lcd.WUSA_LCD_CLOCK_GATE.state = 1;
    if (state_new.VOGA_HBLANKp) state_new.lcd.WUSA_LCD_CLOCK_GATE.state = 0;

    pins.lcd.PIN_53_LCD_CLOCK.state = (!state_new.lcd.WUSA_LCD_CLOCK_GATE || !SACU_CLKPIPE_new) && (!state_new.fine_scroll.PUXA_SCX_FINE_MATCH_A || state_new.fine_scroll.NYZE_SCX_FINE_MATCH_B);
  }
  else {
    state_new.lcd.LUCA_LINE_EVENp.state = 0;
    state_new.lcd.NAPO_FRAME_EVENp.state = 0;
    state_new.lcd.MEDA_VSYNC_OUTn.state = 0;
    state_new.lcd.WUSA_LCD_CLOCK_GATE.state = 0;

    state_new.lcd.POME = 1;
    state_new.lcd.RUJU = 1;
    state_new.lcd.POFY = 0;

    pins.lcd.PIN_50_LCD_DATA1.state = state_new.lcd.RAVO_LD1n;
    pins.lcd.PIN_51_LCD_DATA0.state = state_new.lcd.REMY_LD0n;
    pins.lcd.PIN_52_LCD_CNTRL.state = 1;
    pins.lcd.PIN_53_LCD_CLOCK.state = 1;
    pins.lcd.PIN_54_LCD_HSYNC.state = 1;
    pins.lcd.PIN_55_LCD_LATCH.state = !get_bit(state_new.reg_div, 6);
    pins.lcd.PIN_56_LCD_FLIPS.state = !get_bit(state_new.reg_div, 7);
    pins.lcd.PIN_57_LCD_VSYNC.state = 1;
  }

  //----------------------------------------
  // Audio

  //tock_spu_logic();

  //----------------------------------------
  // Memory buses

  if (state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp && !cpu_addr_vram_new) {
    state_new.ext_addr_latch = cpu_addr_new & 0x7FFF;
  }

  if (state_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) {
    pins.ctrl.PIN_80_CSn.state = !get_bit(state_new.reg_dma, 7);
    //pins.abus_lo = uint8_t(~state_new.dma_lo);
    //pins.abus_hi = state_new.reg_dma;
    bit_unpack_inv(pins.abus_lo, state_new.dma_lo);
    bit_unpack(pins.abus_hi,     state_new.reg_dma);
  }
  else {
    pins.ctrl.PIN_80_CSn.state = state_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK && cpu_addr_ram_new;
    //pins.abus_lo = ((state_new.ext_addr_latch >> 0) & 0xFF) ^ 0xFF;
    //pins.abus_hi = ((state_new.ext_addr_latch >> 8) & 0x7F) ^ 0x7F;
    bit_unpack_inv(pins.abus_lo, (state_new.ext_addr_latch >> 0) & 0xFF);
    bit_unpack_inv(pins.abus_hi, (state_new.ext_addr_latch >> 8) & 0x7F);
  }

  if (!(state_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp && state_new.cpu_signals.SIG_IN_CPU_WRp) {
    pins.ctrl.PIN_79_RDn.state = cpu_addr_vram_new;
    pins.ctrl.PIN_78_WRn.state = gen_clk_new(phase_total, 0b00001110) && !cpu_addr_vram_new;
  }
  else {
    pins.ctrl.PIN_79_RDn.state = 1;
    pins.ctrl.PIN_78_WRn.state = 0;
  }

  //pins.abus_hi &= 0b01111111;
  pins.abus_hi.PIN_16_A15.state = 0;
  if (state_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) {
    //pins.abus_hi |= state_new.reg_dma & 0b10000000;
    pins.abus_hi.PIN_16_A15.state = !!(state_new.reg_dma & 0b10000000);
  }
  else if (!state_new.cpu_signals.TEPU_BOOT_BITn && cpu_addr_new <= 0x00FF) {
  }
  else {
    uint8_t bit = state_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK && !get_bit(state_new.cpu_abus, 15);
    //pins.abus_hi |= (bit << 7);
    pins.abus_hi.PIN_16_A15.state = bit;
  }

  CHECK_N(state_new.cpu_signals.SIG_IN_CPU_RDp && state_new.cpu_signals.SIG_IN_CPU_WRp);

  if (state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp && state_new.cpu_signals.SIG_IN_CPU_WRp && !cpu_addr_vram_new) {
    //pins.dbus = ~state_new.cpu_dbus;
    bit_unpack_inv(pins.dbus, state_new.cpu_dbus);
  }
  else {
    bit_unpack(pins.dbus, 0);
  }

  //----------------------------------------
  // Ext read

  if (pins.ctrl.PIN_79_RDn) {
    const uint16_t ext_addr_lo = (uint16_t)bit_pack_inv(pins.abus_lo);
    const uint16_t ext_addr_hi = (uint16_t)bit_pack_inv(pins.abus_hi);
    const uint16_t ext_addr = (ext_addr_lo << 0) | (ext_addr_hi << 8);

    //const uint16_t ext_addr = ~(pins.abus_lo | (pins.abus_hi << 8));
    
    const auto rom_addr_mask = cart_rom_addr_mask(cart_blob);
    const auto ram_addr_mask = cart_ram_addr_mask(cart_blob);

    const int region = ext_addr >> 13;
    uint8_t data_in = 0x00;

    bool ext_read_en = false;

    if (cart_has_mbc1(cart_blob)) {

      const bool mbc1_ram_en = state_new.ext_mbc.MBC1_RAM_EN;
      const bool mbc1_mode = state_new.ext_mbc.MBC1_MODE;

      const uint32_t mbc1_rom0_bank = mbc1_mode ? bit_pack(&state_new.ext_mbc.MBC1_BANK5, 2) : 0;
      const uint32_t mbc1_rom0_addr = ((ext_addr & 0x3FFF) | (mbc1_rom0_bank << 19)) & rom_addr_mask;

      uint32_t mbc1_rom1_bank = bit_pack(&state_new.ext_mbc.MBC1_BANK0, 7);
      if ((mbc1_rom1_bank & 0x1F) == 0) mbc1_rom1_bank |= 1;
      const uint32_t mbc1_rom1_addr = ((ext_addr & 0x3FFF) | (mbc1_rom1_bank << 14)) & rom_addr_mask;

      const uint32_t mbc1_ram_bank = mbc1_mode ? bit_pack(&state_new.ext_mbc.MBC1_BANK5, 2) : 0;
      const uint32_t mbc1_ram_addr = ((ext_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & ram_addr_mask;

      switch (region) {
      case 0: ext_read_en = true; data_in = cart_blob[mbc1_rom0_addr]; break;
      case 1: ext_read_en = true; data_in = cart_blob[mbc1_rom0_addr]; break;
      case 2: ext_read_en = true; data_in = cart_blob[mbc1_rom1_addr]; break;
      case 3: ext_read_en = true; data_in = cart_blob[mbc1_rom1_addr]; break;
      case 4: data_in = 0x00; break;
      case 5: ext_read_en = true; data_in = mbc1_ram_en ? mem.cart_ram[mbc1_ram_addr] : 0xFF; break;
      case 6: ext_read_en = true; data_in = mem.int_ram[ext_addr & 0x1FFF]; break;
      case 7: ext_read_en = true; data_in = mem.int_ram[ext_addr & 0x1FFF]; break;
      }
    }
    else {
      switch (region) {
      case 0: ext_read_en = true; data_in = cart_blob[ext_addr & rom_addr_mask]; break;
      case 1: ext_read_en = true; data_in = cart_blob[ext_addr & rom_addr_mask]; break;
      case 2: ext_read_en = true; data_in = cart_blob[ext_addr & rom_addr_mask]; break;
      case 3: ext_read_en = true; data_in = cart_blob[ext_addr & rom_addr_mask]; break;
      case 4: data_in = 0x00; break;
      case 5: data_in = 0x00; break;
      case 6: ext_read_en = true; data_in = mem.int_ram[ext_addr & 0x1FFF]; break;
      case 7: ext_read_en = true; data_in = mem.int_ram[ext_addr & 0x1FFF]; break;
      }
    }

    if (ext_read_en) {
      //pins.dbus = uint8_t(~data_in);
      bit_unpack_inv(pins.dbus, data_in);
    }
  }

  //----------------------------------------
  // Ext write

  {
    const uint16_t ext_addr_lo = (uint16_t)bit_pack_inv(pins.abus_lo);
    const uint16_t ext_addr_hi = (uint16_t)bit_pack_inv(pins.abus_hi);
    const uint16_t ext_addr = (ext_addr_lo << 0) | (ext_addr_hi << 8);

    const auto region = ext_addr >> 13;
    //const uint8_t data_out = uint8_t(~pins.dbus);
    const uint8_t data_out = (uint8_t)bit_pack_inv(pins.dbus);
    const bool mbc1_ram_en = state_new.ext_mbc.MBC1_RAM_EN;
    const bool mbc1_mode = state_new.ext_mbc.MBC1_MODE;

    const auto mbc1_ram_bank = mbc1_mode ? bit_pack(&state_new.ext_mbc.MBC1_BANK5, 2) : 0;
    const auto mbc1_ram_addr = ((ext_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & cart_ram_addr_mask(cart_blob);

    if (pins.ctrl.PIN_78_WRn && cart_has_mbc1(cart_blob)) {
      switch (region) {
      case 0: state_new.ext_mbc.MBC1_RAM_EN = (data_out & 0x0F) == 0x0A; break;
      case 1: bit_unpack(&state_new.ext_mbc.MBC1_BANK0, 5, data_out); break;
      case 2: bit_unpack(&state_new.ext_mbc.MBC1_BANK5, 2, data_out); break;
      case 3: state_new.ext_mbc.MBC1_MODE = (data_out & 1); break;
      case 4: break;
      case 5: if (cart_has_ram(cart_blob) && mbc1_ram_en) mem.cart_ram[mbc1_ram_addr & cart_ram_addr_mask(cart_blob)] = (uint8_t)data_out; break;
      case 6: mem.int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out; break;
      case 7: mem.int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out; break;
      }
    }

    if (pins.ctrl.PIN_78_WRn && !cart_has_mbc1(cart_blob)) {
      switch (region) {
      case 0: break;
      case 1: break;
      case 2: break;
      case 3: break;
      case 4: break;
      case 5: if (cart_has_ram(cart_blob)) mem.cart_ram[ext_addr & cart_ram_addr_mask(cart_blob)] = (uint8_t)data_out; break;
      case 6: mem.int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out;break;
      case 7: mem.int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out;break;
      }
    }
  }

  //----------------------------------------
  // VRAM bus

  if (state_new.cpu_signals.SIG_IN_CPU_RDp && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp && !cpu_addr_vram_new && state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
    state_new.cpu_dbus = ~state_new.ext_data_latch;
  }
  else {
    //state_new.ext_data_latch = pins.dbus;
    state_new.ext_data_latch = (uint8_t)bit_pack(pins.dbus);
  }

  state_new.vram_abus = VRAM_ADDR_MASK;
  state_new.vram_dbus = 0xFF;

  // CPU vram read address

  if (!dma_addr_vram_new && state_new.XYMU_RENDERINGn) {
    state_new.vram_abus = (~cpu_addr_new) & VRAM_ADDR_MASK;
  }

  // DMA vram read address

  if (dma_addr_vram_new) {
    state_new.vram_abus = uint8_t(~state_new.dma_lo);
    state_new.vram_abus |= (state_new.reg_dma << 8);
    state_new.vram_abus &= VRAM_ADDR_MASK;
  }

  //--------------------------------------------
  // SCX/SCY regs and BG map read address

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) {
    if (cpu_addr_new == 0xFF42) state_new.reg_scy = ~state_old.cpu_dbus;
    if (cpu_addr_new == 0xFF43) state_new.reg_scx = ~state_old.cpu_dbus;
  }

  if (state_new.cpu_signals.SIG_IN_CPU_RDp) {
    if (cpu_addr_new == 0xFF42) state_new.cpu_dbus = ~state_new.reg_scy;
    if (cpu_addr_new == 0xFF43) state_new.cpu_dbus = ~state_new.reg_scx;
  }

  if (state_new.tfetch_control.LONY_FETCHINGp) {
    const auto px  = state_new.pix_count;
    const auto scx = ~state_new.reg_scx;
    const auto scy = ~state_new.reg_scy;

    const auto sum_x = px + scx;
    const auto sum_y = reg_ly_new + scy;

    //--------------------------------------------
    // BG map read address

    if (state_new.tfetch_control.LONY_FETCHINGp && 
        !get_bit(state_new.tfetch_counter, 1) &&
        !get_bit(state_new.tfetch_counter, 2) &&
        !state_new.win_ctrl.PYNU_WIN_MODE_Ap)
    {
      const auto bgmap_en = !get_bit(state_new.reg_lcdc, 3);

      uint32_t addr = 0;
      bit_cat(addr,  0,  4, (px + scx) >> 3);
      bit_cat(addr,  5,  9, (reg_ly_new + scy) >> 3);
      bit_cat(addr, 10, 10, bgmap_en);
      bit_cat(addr, 11, 11, 1);
      bit_cat(addr, 12, 12, 1);

      state_new.vram_abus = uint16_t(addr ^ VRAM_ADDR_MASK);
    }

    //--------------------------------------------
    // BG/Win tile read address

    if (state_new.tfetch_control.LONY_FETCHINGp &&
        get_bit(state_new.tfetch_counter, 1) || get_bit(state_new.tfetch_counter, 2))
    {
      const auto hilo = get_bit(state_new.tfetch_counter, 2);
      const auto tile_y = (state_new.win_ctrl.PYNU_WIN_MODE_Ap ? state_new.win_y.tile : (sum_y & 0b111));
      const auto map_y = state_new.tile_temp_b;
      const auto map = !get_bit(state_new.tile_temp_b, 7) && get_bit(state_new.reg_lcdc, 4);

      uint32_t addr  = 0;
      bit_cat(addr,  0,  0, hilo);
      bit_cat(addr,  1,  3, tile_y);
      bit_cat(addr,  4, 11, map_y);
      bit_cat(addr, 12, 12, map);
      
      state_new.vram_abus = uint16_t(addr ^ VRAM_ADDR_MASK);
    }
  }

  //--------------------------------------------
  // Win map read address

  if (state_new.tfetch_control.LONY_FETCHINGp &&
      !get_bit(state_new.tfetch_counter, 1) &&
      !get_bit(state_new.tfetch_counter, 2) &&
       state_new.win_ctrl.PYNU_WIN_MODE_Ap)
  {
    uint32_t addr = 0;
    bit_cat(addr,  0,  4, ~state_new.win_x.map);
    bit_cat(addr,  5,  9, ~state_new.win_y.map);
    bit_cat(addr, 10, 10, get_bit(state_new.reg_lcdc, 6));
    state_new.vram_abus = uint16_t(addr);
  }

  //--------------------------------------------
  // Sprite read address

  if (state_new.sfetch_control.TEXY_SFETCHINGp) {
    const bool hilo = state_new.sfetch_control.VONU_SFETCH_S1p_D4;
    const auto line = state_new.sprite_lbus ^ (get_bit(state_new.oam_temp_b, 6) ? 0b0000 : 0b1111);
    const auto tile = state_new.oam_temp_a;

    uint32_t addr = 0;
    bit_cat(addr,  0,  0, hilo);
    if (get_bit(state_new.reg_lcdc, 2)) {
      bit_cat(addr,  1,  3, line);
      bit_cat(addr,  4, 11, tile);
    }
    else {
      bit_cat(addr,  1,  4, line);
      bit_cat(addr,  5, 11, tile >> 1);
    }
    state_new.vram_abus = uint16_t(addr ^ VRAM_ADDR_MASK);
  }

  //--------------------------------------------
  // Vram address pin driver

  //state_new.vram_ext_abus = state_new.vram_abus;
  bit_unpack(pins.vram_abus, state_new.vram_abus);

  //--------------------------------------------
  // CPU bus to Vram data bus

  if (state_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK && state_new.XYMU_RENDERINGn && cpu_addr_vram_new && state_new.cpu_signals.SIG_IN_CPU_WRp) {
    state_new.vram_dbus = state_new.cpu_dbus;
  }

  //--------------------------------------------
  // Vram control pins

  if (state_new.XYMU_RENDERINGn) {
    pins.vram_ctrl.PIN_43_VRAM_CSn.state = (cpu_addr_vram_new && gen_clk_new(phase_total, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp) || dma_addr_vram_new;
    pins.vram_ctrl.PIN_45_VRAM_OEn.state = (!cpu_addr_vram_new || !state_new.cpu_signals.SIG_IN_CPU_WRp) || dma_addr_vram_new;
    pins.vram_ctrl.PIN_49_VRAM_WRn.state = cpu_addr_vram_new && gen_clk_new(phase_total, 0b00001110) && state_new.cpu_signals.SIG_IN_CPU_WRp && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp;
  }
  else {
    pins.vram_ctrl.PIN_43_VRAM_CSn.state = dma_addr_vram_new || state_new.tfetch_control.LONY_FETCHINGp || state_new.sfetch_control.TEXY_SFETCHINGp;
    pins.vram_ctrl.PIN_45_VRAM_OEn.state = dma_addr_vram_new || state_new.tfetch_control.LONY_FETCHINGp || (state_new.sfetch_control.TEXY_SFETCHINGp && (!state_new.sfetch_control.TYFO_SFETCH_S0p_D1 || get_bit(state_new.sfetch_counter, 0)));
    pins.vram_ctrl.PIN_49_VRAM_WRn.state = 0;
  }

  uint8_t vdata = 0xFF;

  if (pins.vram_ctrl.PIN_45_VRAM_OEn) {
    //vdata = mem.vid_ram[pins.vram_abus ^ VRAM_ADDR_MASK];
    vdata = mem.vid_ram[bit_pack_inv(pins.vram_abus)];
  }

  //--------------------------------------------
  // Vram data pin driver

  bit_unpack(pins.vram_dbus, 0);

  if (pins.vram_ctrl.PIN_45_VRAM_OEn) {
    bit_unpack_inv(pins.vram_dbus, vdata);
  }

  if (cpu_addr_vram_new && state_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK && state_new.XYMU_RENDERINGn && state_new.cpu_signals.SIG_IN_CPU_WRp) {
    bit_unpack_inv(pins.vram_dbus, state_new.vram_dbus);
  }
  else {
    state_new.vram_dbus = (uint8_t)bit_pack_inv(pins.vram_dbus);
  }

  if (pins.vram_ctrl.PIN_49_VRAM_WRn) {
    mem.vid_ram[bit_pack_inv(pins.vram_abus)] = (uint8_t)bit_pack_inv(pins.vram_dbus);
  }

  //--------------------------------------------
  // Vram bus to cpu bus

  if (cpu_addr_vram_new && state_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK && state_new.XYMU_RENDERINGn && state_new.cpu_signals.SIG_IN_CPU_RDp && state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
    state_new.cpu_dbus = state_new.vram_dbus;
  }

  //--------------------------------------------
  // Vram bus to sprite x flipper

  uint8_t pix = state_new.vram_dbus;
  if (get_bit(state_new.oam_temp_b, 5) && state_new.sfetch_control.TEXY_SFETCHINGp) pix = bit_reverse(pix);
  state_new.flipped_sprite = pix;

  //----------------------------------------
  // oam

  // this is weird, why is it always 0 when not in reset?
  state_new.oam_ctrl.MAKA_LATCH_EXTp.state = 0;

  state_new.oam_abus = 0xFF;
  state_new.oam_dbus_a = 0xFF;
  state_new.oam_dbus_b = 0xFF;
  state_new.oam_ctrl.SIG_OAM_CLKn.state  = 1;
  state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
  state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
  state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;

  //----------
  // oam address

  const auto cpu_oam_rd_new = cpu_addr_oam_new && state_new.cpu_signals.SIG_IN_CPU_RDp;
  const auto cpu_oam_wr_new = cpu_addr_oam_new && state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00001110);

  const auto sfetch_oam_clk_new = (get_bit(state_new.sfetch_counter, 1) || get_bit(state_new.sfetch_counter, 2) || (state_new.sfetch_control.TYFO_SFETCH_S0p_D1 && !get_bit(state_new.sfetch_counter, 0)));
  const auto sfetch_oam_oen_new = (get_bit(state_new.sfetch_counter, 1) || get_bit(state_new.sfetch_counter, 2) || !state_new.sfetch_control.TYFO_SFETCH_S0p_D1);

  const auto sscan_oam_addr_new  = (state_new.scan_counter << 2) | 0b00;
  const auto sfetch_oam_addr_new = (state_new.sprite_ibus  << 2) | 0b11;
  const auto dma_oam_addr_new    = state_new.dma_lo;

  if      (state_new.MATU_DMA_RUNNINGp) state_new.oam_abus = (uint8_t)~dma_oam_addr_new;
  else if (state_new.ACYL_SCANNINGp)    state_new.oam_abus = (uint8_t)~sscan_oam_addr_new ;
  else if (!state_new.XYMU_RENDERINGn)  state_new.oam_abus = (uint8_t)~sfetch_oam_addr_new;
  else                                  state_new.oam_abus = (uint8_t)~cpu_addr_new;

  //----------
  // oam control signals depend on address
  // The inclusion of cpu_addr_oam_new in the SCANNING and RENDERING branches is probably a hardware bug.

  if (state_new.MATU_DMA_RUNNINGp) {
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = gen_clk_new(phase_total, 0b11110000);
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = gen_clk_new(phase_total, 0b11110000) || !get_bit(state_new.oam_abus, 0);
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = gen_clk_new(phase_total, 0b11110000) ||  get_bit(state_new.oam_abus, 0);
    state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
  }
  else if (state_new.ACYL_SCANNINGp) {
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = gen_clk_new(phase_total, 0b10001000) && (!cpu_addr_oam_new || gen_clk_new(phase_total, 0b11110000));
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
    state_new.oam_ctrl.SIG_OAM_OEn.state   = gen_clk_new(phase_total, 0b10011001) && !(cpu_oam_rd_new && !state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE);
  }
  else if (!state_new.XYMU_RENDERINGn) {
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = sfetch_oam_clk_new && (!cpu_addr_oam_new || gen_clk_new(phase_total, 0b11110000));
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
    state_new.oam_ctrl.SIG_OAM_OEn.state   = sfetch_oam_oen_new && !(cpu_oam_rd_new && !state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE);
  }
  else if (cpu_addr_oam_new) {
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = gen_clk_new(phase_total, 0b11110000);
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = !cpu_oam_wr_new || !get_bit(state_new.oam_abus, 0);
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = !cpu_oam_wr_new ||  get_bit(state_new.oam_abus, 0);
    state_new.oam_ctrl.SIG_OAM_OEn.state   = !state_new.cpu_signals.SIG_IN_CPU_RDp || state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE;
  }

  //----------
  // the actual oam read

  if (!state_new.oam_ctrl.SIG_OAM_OEn) {
    uint8_t oam_addr_new = uint8_t(~state_new.oam_abus) >> 1;
    state_new.oam_dbus_a = ~mem.oam_ram[(oam_addr_new << 1) + 0];
    state_new.oam_dbus_b = ~mem.oam_ram[(oam_addr_new << 1) + 1];
  }

  //----------
  // latch data from oam

  bool latch_oam = false;
  if (state_new.ACYL_SCANNINGp) latch_oam = gen_clk_new(phase_total, 0b01100110);
  else if (!state_new.XYMU_RENDERINGn)         latch_oam = !sfetch_oam_oen_new;
  else                                       latch_oam = cpu_oam_rd_new && !state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE;

  if (latch_oam) {
    state_new.oam_latch_a = state_new.oam_dbus_a;
    state_new.oam_latch_b = state_new.oam_dbus_b;
  }

  //----------
  // put oam latch on cpu bus

  if (!state_new.MATU_DMA_RUNNINGp && !state_new.ACYL_SCANNINGp && state_new.XYMU_RENDERINGn) {
    if (cpu_oam_rd_new && state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
      if (get_bit(state_new.oam_abus, 0)) {
        state_new.cpu_dbus = ~state_new.oam_latch_a;
      }
      else {
        state_new.cpu_dbus = ~state_new.oam_latch_b;
      }
    }
  }

  //----------
  // if we're writing to oam, put source data on oam bus

  const auto vram_data_new    = state_new.vram_dbus;
  const auto ext_data_new     = bit_pack_inv(pins.dbus);
  const auto cpu_oam_data_new = state_new.cpu_dbus; // have to repack here...

  // WUJE is weird, not sure why it's necessary.
  if (gen_clk_new(phase_total, 0b11110000)) state_new.oam_ctrl.WUJE_CPU_OAM_WRn.state = 1;
  if (cpu_addr_oam_new && state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00001110)) state_new.oam_ctrl.WUJE_CPU_OAM_WRn.state = 0;

  if (state_new.MATU_DMA_RUNNINGp && dma_addr_vram_new) {
    state_new.oam_dbus_a = ~vram_data_new;
    state_new.oam_dbus_b = ~vram_data_new;
  }
  else if (state_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) {
    state_new.oam_dbus_a = uint8_t(~ext_data_new);
    state_new.oam_dbus_b = uint8_t(~ext_data_new);
  }
  else if (!state_new.ACYL_SCANNINGp && state_new.XYMU_RENDERINGn) {
    if (cpu_addr_oam_new) {
      if (!state_new.oam_ctrl.WUJE_CPU_OAM_WRn) {
        state_new.oam_dbus_a = ~cpu_oam_data_new;
        state_new.oam_dbus_b = ~cpu_oam_data_new;
      }
    }
    else {
      state_new.oam_dbus_a = ~cpu_oam_data_new;
      state_new.oam_dbus_b = ~cpu_oam_data_new;
    }
  }

  //----------
  // the actual oam write

  if (state_old.oam_ctrl.SIG_OAM_CLKn && !state_new.oam_ctrl.SIG_OAM_CLKn) {
    uint8_t oam_addr_new = uint8_t(~state_new.oam_abus) >> 1;
    if (!state_new.oam_ctrl.SIG_OAM_WRn_A) mem.oam_ram[(oam_addr_new << 1) + 0] = ~state_new.oam_dbus_a;
    if (!state_new.oam_ctrl.SIG_OAM_WRn_B) mem.oam_ram[(oam_addr_new << 1) + 1] = ~state_new.oam_dbus_b;
  }

  state_new.oam_ctrl.old_oam_clk = !state_new.oam_ctrl.SIG_OAM_CLKn; // vestige of gate mode

  //----------------------------------------
  // zram

  {
    wire CSp = (cpu_addr_new >= 0xFF80) && (cpu_addr_new <= 0xFFFE);

    if (state_new.zram_bus.clk_old && !state_new.cpu_signals.TAPU_CPU_WRp && CSp) {
      mem.zero_ram[cpu_addr_new & 0x007F] = state_old.cpu_dbus;
    }
    state_new.zram_bus.clk_old = state_new.cpu_signals.TAPU_CPU_WRp;

    uint8_t zdata = mem.zero_ram[cpu_addr_new & 0x007F];

    if (CSp && state_new.cpu_signals.TEDO_CPU_RDp) {
      state_new.cpu_dbus = zdata;
    }
  }



  //----------------------------------------
  // And finally, interrupts.

  auto pack_cpu_dbus_old = state_old.cpu_dbus;
  auto pack_cpu_dbus_new = state_new.cpu_dbus;
  auto pack_ie = state_new.reg_ie;
  auto pack_if = state_new.reg_if;
  auto pack_stat = state_new.reg_stat;

  // FIXME this seems slightly wrong...
  if (state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00001110) && cpu_addr_new == 0xFF41) {
  }
  else {
    state_new.int_ctrl.RUPO_LYC_MATCHn.state = 1;
  }

  // but the "reset" arm of the latch overrides the "set" arm, so it doesn't completely break?
  if (state_new.int_ctrl.ROPO_LY_MATCH_SYNCp) {
    state_new.int_ctrl.RUPO_LYC_MATCHn.state = 0;
  }

  if (cpu_addr_new == 0xFFFF && state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) {
    pack_ie = pack_cpu_dbus_old;
  }

  if (cpu_addr_new == 0xFF41 && state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) {
    pack_stat = (~pack_cpu_dbus_old >> 3) & 0b00001111;
  }

  if (cpu_addr_new == 0xFF41 && state_new.cpu_signals.SIG_IN_CPU_RDp) {
    uint8_t stat = 0x80;

    stat |= (!state_new.XYMU_RENDERINGn || state_new.lcd.POPU_y144p) << 0;
    stat |= (!state_new.XYMU_RENDERINGn || state_new.ACYL_SCANNINGp) << 1;
    stat |= (!state_new.int_ctrl.RUPO_LYC_MATCHn) << 2;
    stat |= (pack_stat ^ 0b1111) << 3;

    pack_cpu_dbus_new = stat;
  }

  bool int_stat_old = 0;
  if (!get_bit(state_old.reg_stat, 0) && state_old.WODU_HBLANKp && !state_old.lcd.POPU_y144p) int_stat_old = 1;
  if (!get_bit(state_old.reg_stat, 1) && state_old.lcd.POPU_y144p) int_stat_old = 1;
  if (!get_bit(state_old.reg_stat, 2) && !state_old.lcd.POPU_y144p && state_old.lcd.RUTU_x113p) int_stat_old = 1;
  if (!get_bit(state_old.reg_stat, 3) && state_old.int_ctrl.ROPO_LY_MATCH_SYNCp) int_stat_old = 1;

  const bool int_lcd_old = state_old.lcd.POPU_y144p;
  const bool int_joy_old = !state_old.joy_int.APUG_JP_GLITCH3 || !state_old.joy_int.BATU_JP_GLITCH0;
  const bool int_tim_old = state_old.int_ctrl.MOBA_TIMER_OVERFLOWp;
  //const bool int_ser_old = serial.CALY_SER_CNT3;
  const bool int_ser_old = 0;

  bool int_stat_new = 0;
  if (!get_bit(pack_stat, 0) && state_new.WODU_HBLANKp && !state_new.lcd.POPU_y144p) int_stat_new = 1;
  if (!get_bit(pack_stat, 1) && state_new.lcd.POPU_y144p) int_stat_new = 1;
  if (!get_bit(pack_stat, 2) && !state_new.lcd.POPU_y144p && state_new.lcd.RUTU_x113p) int_stat_new = 1;
  if (!get_bit(pack_stat, 3) && state_new.int_ctrl.ROPO_LY_MATCH_SYNCp) int_stat_new = 1;

  const wire int_lcd_new = state_new.lcd.POPU_y144p;
  const wire int_joy_new = !state_new.joy_int.APUG_JP_GLITCH3 || !state_new.joy_int.BATU_JP_GLITCH0;
  const wire int_tim_new = state_new.int_ctrl.MOBA_TIMER_OVERFLOWp;
  //const wire int_ser = state_new.serial.CALY_SER_CNT3;
  const wire int_ser_new = 0;

  if (!int_lcd_old  && int_lcd_new)  pack_if |= (1 << 0);
  if (!int_stat_old && int_stat_new) pack_if |= (1 << 1);
  if (!int_tim_old  && int_tim_new)  pack_if |= (1 << 2);
  if (!int_ser_old  && int_ser_new)  pack_if |= (1 << 3);
  if (!int_joy_old  && int_joy_new)  pack_if |= (1 << 4);

  // note this is an async set so it doesn't happen on the GH clock edge like other writes
  if (state_new.cpu_signals.SIG_IN_CPU_WRp && (cpu_addr_new == 0xFF0F) && gen_clk_new(phase_total, 0b00001110)) {
    pack_if = pack_cpu_dbus_new;
  }

  pack_if &= ~state_new.cpu_ack;

  if (cpu_addr_new == 0xFFFF && state_new.cpu_signals.SIG_IN_CPU_RDp) {
    pack_cpu_dbus_new = pack_ie | 0b11100000;
  }

  if (cpu_addr_new == 0xFF0F && state_new.cpu_signals.SIG_IN_CPU_RDp) {
    state_new.int_latch = (uint8_t)pack_if;
    pack_cpu_dbus_new = pack_if | 0b11100000;
  }


  state_new.cpu_dbus = (uint8_t)pack_cpu_dbus_new;
  state_new.cpu_int = (uint8_t)pack_if;
  state_new.reg_ie = (uint8_t)pack_ie;
  state_new.reg_if = (uint8_t)pack_if;
  state_new.reg_stat = (uint8_t)pack_stat;

  // POSTCONDITIONS
  if (state_new.ACYL_SCANNINGp)    CHECK_P(state_new.XYMU_RENDERINGn);
  if (!state_new.XYMU_RENDERINGn)  CHECK_N(state_new.ACYL_SCANNINGp);
}
