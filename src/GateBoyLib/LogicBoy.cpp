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

  if (lcd_y >= 0 && lcd_y < 144 && lcd_x >= 0 && lcd_x < 160) {
    wire p0 = !pins.lcd.PIN_51_LCD_DATA0.state;
    wire p1 = !pins.lcd.PIN_50_LCD_DATA1.state;
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

  state_new.cpu_dbus = 0xFF; // must be pulled up or we fail regression

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

  CHECK_N(state_new.cpu_signals.SIG_IN_CPU_RDp.state && state_new.cpu_signals.SIG_IN_CPU_WRp);

  // This chunk is weird...
  {
    bool EXT_addr_new = (cpu.bus_req_new.read || cpu.bus_req_new.write);
    if ((cpu.bus_req_new.addr >= 0x8000) && (cpu.bus_req_new.addr < 0x9FFF) && gen_clk_new(phase_total, 0b10000000)) EXT_addr_new = false;
    if ((cpu.bus_req_new.addr >= 0xFE00)) EXT_addr_new = false;
    if ((cpu.bus_req_new.addr <= 0x00FF) && !state_new.cpu_signals.TEPU_BOOT_BITn) EXT_addr_new = false;
    state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = EXT_addr_new;
  }


  // SIGNALS TO KILL:
  //Gate     ACYL_SCANNINGp; // old used?
  //Gate     WODU_HBLANKp; // old used
  //Gate     SATO_BOOT_BITn;
  //Gate     FEPO_STORE_MATCHp; // old used

  //----------------------------------------
  // DIV

  if (DELTA_HA) {
    state_new.reg_div = state_new.reg_div + 1;
  }

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && (DELTA_DE || DELTA_EF || DELTA_FG)) {
    if (state_new.cpu_abus == 0xFF04) state_new.reg_div = 0;
  }

  //----------------------------------------

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && DELTA_GH) {
    if (state_new.cpu_abus == 0xFF00) {
      set_bit(state_new.reg_joy, 0, get_bit(state_old.cpu_dbus, 4));
      set_bit(state_new.reg_joy, 1, get_bit(state_old.cpu_dbus, 5));
    }
    // LCDC write has to be near the top as it controls the video reset signal
    if (state_new.cpu_abus == 0xFF40) state_new.reg_lcdc = ~state_old.cpu_dbus;
    if (state_new.cpu_abus == 0xFF45) state_new.reg_lyc = ~state_old.cpu_dbus;
    if (state_new.cpu_abus == 0xFF06) state_new.reg_tma  =  state_old.cpu_dbus;
    if (state_new.cpu_abus == 0xFF07) state_new.reg_tac  =  state_old.cpu_dbus;
    if (state_new.cpu_abus == 0xFF46) state_new.reg_dma  = ~state_old.cpu_dbus;
    if (state_new.cpu_abus == 0xFF47) state_new.reg_bgp  = ~state_old.cpu_dbus;
    if (state_new.cpu_abus == 0xFF48) state_new.reg_obp0 = ~state_old.cpu_dbus;
    if (state_new.cpu_abus == 0xFF49) state_new.reg_obp1 = ~state_old.cpu_dbus;
    if (state_new.cpu_abus == 0xFF4A) state_new.reg_wy   = ~state_old.cpu_dbus;
    if (state_new.cpu_abus == 0xFF4B) state_new.reg_wx   = ~state_old.cpu_dbus;
  }

  //----------------------------------------
  // LX, LY, lcd flags

  if (get_bit(state_new.reg_lcdc, 7)) {
    state_new.lcd.NYPE_x113p.state = 0;
    state_new.lcd.RUTU_x113p.state = 0;
    state_new.lcd.POPU_y144p.state = 0;
    state_new.lcd.MYTA_y153p.state = 0;
    state_new.reg_lx = 0;
    state_new.reg_ly = 0;
  }
  else {

    if (DELTA_BC) {
      if (!state_new.lcd.NYPE_x113p && state_new.lcd.RUTU_x113p) {
        state_new.lcd.POPU_y144p.state = ((state_new.reg_ly & 144) == 144);
        state_new.lcd.MYTA_y153p.state = ((state_new.reg_ly & 153) == 153);
        if (state_new.lcd.MYTA_y153p) state_new.reg_ly = 0;
      }
    }

    if (DELTA_FG) state_new.lcd.RUTU_x113p.state = (state_new.reg_lx == 113);
    if (DELTA_BC) state_new.lcd.NYPE_x113p.state = state_new.lcd.RUTU_x113p.state;


    if (DELTA_BC) {
      state_new.reg_lx = state_new.lcd.RUTU_x113p ? 0 : state_new.reg_lx + 1;
    }
    if (DELTA_CD) {
      if (state_new.lcd.RUTU_x113p) state_new.reg_lx = 0;
    }
    if (DELTA_FG) {
      if (!state_new.lcd.MYTA_y153p && !state_old.lcd.RUTU_x113p && (state_new.reg_lx == 113)) {
        state_new.reg_ly = uint8_t(state_old.reg_ly + 1);
      }
      if (state_new.reg_lx == 113) state_new.reg_lx = 0;
    }
  }

  //----------------------------------------
  // Line reset trigger

  if (get_bit(state_new.reg_lcdc, 7)) {
    state_new.lcd.ANEL_x113p.state = 0;
    state_new.lcd.CATU_x113p.state = 0;
  }
  else {
    if (DELTA_HA || DELTA_DE) {
      state_new.lcd.CATU_x113p.state = state_new.lcd.RUTU_x113p && (state_new.reg_ly < 144);
    }

    if (DELTA_BC || DELTA_FG) {
      state_new.lcd.ANEL_x113p = state_new.lcd.CATU_x113p;
    }
  }

  bool line_reset = !((state_new.lcd.ANEL_x113p || !state_new.lcd.CATU_x113p) && !get_bit(state_new.reg_lcdc, 7));

  //----------------------------------------
  // Joypad

  // FIXME what if both scan bits are set?

  if (DELTA_HA) {
    uint8_t button_mask = 0b00000000;
    if (!get_bit(state_new.reg_joy, 0)) button_mask |= 0b00001111;
    if (!get_bit(state_new.reg_joy, 1)) button_mask |= 0b11110000;

    state_new.int_ctrl.AWOB_WAKE_CPU.state  = !(sys.buttons & button_mask);
    state_new.int_ctrl.SIG_CPU_WAKE.state   = !(sys.buttons & button_mask);
    state_new.joy_int.APUG_JP_GLITCH3       = state_new.joy_int.AGEM_JP_GLITCH2;
    state_new.joy_int.AGEM_JP_GLITCH2       = state_new.joy_int.ACEF_JP_GLITCH1;
    state_new.joy_int.ACEF_JP_GLITCH1       = state_new.joy_int.BATU_JP_GLITCH0;
    state_new.joy_int.BATU_JP_GLITCH0.state = !(sys.buttons & button_mask);
  }

  if (!state_new.cpu_signals.SIG_IN_CPU_RDp) {
    uint8_t button_mask = 0b00000000;
    if (!get_bit(state_new.reg_joy, 0)) button_mask |= 0b00001111;
    if (!get_bit(state_new.reg_joy, 1)) button_mask |= 0b11110000;
    state_new.joy_latch = (((sys.buttons & button_mask) >> 0) | ((sys.buttons & button_mask) >> 4)) & 0b1111;
  }

  //----------------------------------------
  //tock_serial_logic();
  //tock_timer_logic();

  //----------------------------------------
  // Timer

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

  if (state_new.cpu_abus == 0xFF05 && gen_clk_new(phase_total, 0b00001110) && state_new.cpu_signals.SIG_IN_CPU_WRp) {
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

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) {
    if (state_new.cpu_abus == 0xFF50) state_new.cpu_signals.TEPU_BOOT_BITn.state = get_bit(state_old.cpu_dbus, 0) || state_old.cpu_signals.TEPU_BOOT_BITn;
  }

  if (state_new.cpu_abus <= 0x00FF) {
    if (state_new.cpu_signals.SIG_IN_CPU_RDp && !state_new.cpu_signals.TEPU_BOOT_BITn) {
      state_new.cpu_dbus = DMG_ROM_blob[state_new.cpu_abus & 0xFF];
    }
  }

  if (state_new.cpu_signals.SIG_IN_CPU_RDp && (state_new.cpu_abus == 0xFF50)) {
    state_new.cpu_dbus &= ~1;
    state_new.cpu_dbus |= state_new.cpu_signals.TEPU_BOOT_BITn.state;
  }

  //----------------------------------------
  // DMA

  if (state_new.cpu_abus == 0xFF46 && state_new.cpu_signals.SIG_IN_CPU_RDp) {
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
    if (state_new.cpu_abus == 0xFF46 && state_new.cpu_signals.SIG_IN_CPU_WRp) {
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
    if (line_reset) {
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
        
        if (state_new.scan_counter != 39) state_new.scan_counter++;
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



    if (line_reset) {
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
    uint8_t sprite_pix = state_old.vram_dbus;

    if (get_bit(state_old.oam_temp_b, 5) && state_old.sfetch_control.TEXY_SFETCHINGp) {
      sprite_pix = bit_reverse(state_old.vram_dbus);
    }

    if ((sfetch_phase_old == 5) && (sfetch_phase_new == 6 || state_new.XYMU_RENDERINGn)) {
      state_new.sprite_pix_a = ~sprite_pix;
    }

    if ((sfetch_phase_old == 9) && (sfetch_phase_new == 10 || state_new.XYMU_RENDERINGn)) {
      state_new.sprite_pix_b = ~sprite_pix;
    }
  }

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  const bool scanning_old = !state_old.MATU_DMA_RUNNINGp && state_old.sprite_scanner.BESU_SCANNINGn && !get_bit(state_old.reg_lcdc, 7);
  const bool scanning_new = !state_new.MATU_DMA_RUNNINGp && state_new.sprite_scanner.BESU_SCANNINGn && !get_bit(state_new.reg_lcdc, 7);

  const wire oam_busy_old = (state_old.cpu_abus >= 0xFE00 && state_old.cpu_abus <= 0xFEFF) || state_new.MATU_DMA_RUNNINGp;
  const wire oam_busy_new = (state_new.cpu_abus >= 0xFE00 && state_new.cpu_abus <= 0xFEFF) || state_new.MATU_DMA_RUNNINGp;

  uint8_t BYCU_OAM_CLKp_old = 1;
  if (scanning_old)  BYCU_OAM_CLKp_old &= gen_clk_old(phase_total, 0b10001000);
  if (oam_busy_old)  BYCU_OAM_CLKp_old &= gen_clk_old(phase_total, 0b11110000);
  if (!state_old.XYMU_RENDERINGn) BYCU_OAM_CLKp_old &= sfetch_phase_old != 3;

  uint8_t BYCU_OAM_CLKp_new = 1;
  if (scanning_new)  BYCU_OAM_CLKp_new &= gen_clk_new(phase_total, 0b10001000);
  if (oam_busy_new)  BYCU_OAM_CLKp_new &= gen_clk_new(phase_total, 0b11110000);
  if (!state_new.XYMU_RENDERINGn) BYCU_OAM_CLKp_new &= sfetch_phase_new != 3;

  if (!BYCU_OAM_CLKp_old && BYCU_OAM_CLKp_new) {
    state_new.oam_temp_a = ~state_new.oam_latch_a;
    state_new.oam_temp_b = ~state_new.oam_latch_b;
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if it overlaps the current LCD Y coordinate.

  if (get_bit(state_new.reg_lcdc, 7) || line_reset) {
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

    int ly = (int)state_new.reg_ly;
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

  if (line_reset) CHECK_P(SACU_CLKPIPE_new);

  if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
    state_new.pix_count = state_new.pix_count + 1;
  }

  if (line_reset) {
    state_new.pix_count = 0;
  }

  if (get_bit(state_new.reg_lcdc, 7)) {
    state_new.pix_count = 0;
  }

  //----------------------------------------
  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later

  state_new.WODU_HBLANKp = !state_old.FEPO_STORE_MATCHp && (state_new.pix_count & 167) == 167;

  if (state_new.XYMU_RENDERINGn) {
    state_new.lcd.PAHO_X_8_SYNC.state = 0;
    state_new.sprite_match_flags = 0;
    state_new.sprite_ibus = state_new.sprite_index;
    state_new.sprite_lbus = (~state_new.reg_ly + state_new.oam_temp_a) & 0b00001111;
  }
  else if (!state_new.sprite_scanner.CENO_SCANNINGn) { // FIXME this polarity seems wrong
    auto& s = state_new;
    auto& pc = s.pix_count;
    auto& sf = s.sprite_match_flags;
    auto& si = s.sprite_ibus;
    auto& sl = s.sprite_lbus;

    sf = 0;
    si = 0x3F;
    sl = 0x0F;

    if (!get_bit(s.reg_lcdc, 1)) {
      s.FEPO_STORE_MATCHp = 0;
      if      (pc == s.store_x0) { s.FEPO_STORE_MATCHp = 1; sf = 0x001;  si = s.store_i0 ^ 0x3F; sl = s.store_l0 ^ 0x0F; }
      else if (pc == s.store_x1) { s.FEPO_STORE_MATCHp = 1; sf = 0x002;  si = s.store_i1 ^ 0x3F; sl = s.store_l1 ^ 0x0F; }
      else if (pc == s.store_x2) { s.FEPO_STORE_MATCHp = 1; sf = 0x004;  si = s.store_i2 ^ 0x3F; sl = s.store_l2 ^ 0x0F; }
      else if (pc == s.store_x3) { s.FEPO_STORE_MATCHp = 1; sf = 0x008;  si = s.store_i3 ^ 0x3F; sl = s.store_l3 ^ 0x0F; }
      else if (pc == s.store_x4) { s.FEPO_STORE_MATCHp = 1; sf = 0x010;  si = s.store_i4 ^ 0x3F; sl = s.store_l4 ^ 0x0F; }
      else if (pc == s.store_x5) { s.FEPO_STORE_MATCHp = 1; sf = 0x020;  si = s.store_i5 ^ 0x3F; sl = s.store_l5 ^ 0x0F; }
      else if (pc == s.store_x6) { s.FEPO_STORE_MATCHp = 1; sf = 0x040;  si = s.store_i6 ^ 0x3F; sl = s.store_l6 ^ 0x0F; }
      else if (pc == s.store_x7) { s.FEPO_STORE_MATCHp = 1; sf = 0x080;  si = s.store_i7 ^ 0x3F; sl = s.store_l7 ^ 0x0F; }
      else if (pc == s.store_x8) { s.FEPO_STORE_MATCHp = 1; sf = 0x100;  si = s.store_i8 ^ 0x3F; sl = s.store_l8 ^ 0x0F; }
      else if (pc == s.store_x9) { s.FEPO_STORE_MATCHp = 1; sf = 0x200;  si = s.store_i9 ^ 0x3F; sl = s.store_l9 ^ 0x0F; }
    }
  }
  else {
    state_new.sprite_ibus = state_new.sprite_index;
    state_new.sprite_lbus = 0b00001111;
  }

  if (!state_new.XYMU_RENDERINGn && gen_clk_new(phase_total, 0b01010101)) {
    if (!pause_rendering_new) state_new.lcd.PAHO_X_8_SYNC.state = get_bit(state_old.pix_count, 3);
  }

  if (!state_new.XYMU_RENDERINGn && !state_new.FEPO_STORE_MATCHp) {
    const auto pack_ydiff = ~state_new.reg_ly + state_new.oam_temp_a;
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

    if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
      state_new.win_ctrl.RYFA_WIN_FETCHn_A.state = !state_new.win_ctrl.NUKO_WX_MATCHp && state_new.fine_count == 7;
    }
  }
  else {
    state_new.win_ctrl.RENE_WIN_FETCHn_B.state = 0;
    state_new.win_ctrl.RYFA_WIN_FETCHn_A.state = 0;
  }

  if (gen_clk_new(phase_total, 0b00100000)) {
    state_new.win_ctrl.SARY_WY_MATCHp.state = (state_new.reg_ly == uint8_t(~state_new.reg_wy)) && !get_bit(state_new.reg_lcdc, 5);
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

  if (!state_old.XYMU_RENDERINGn /* must be old */) {
    // These ffs are weird because they latches on phase change _or_ if rendering stops in the middle of a fetch
    // Good example of gate-level behavior that doesn't matter
    const uint8_t bfetch_phase_new = pack(!(state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state ^ get_bit(state_new.tfetch_counter, 0)), get_bit(state_new.tfetch_counter, 0), get_bit(state_new.tfetch_counter, 1), get_bit(state_new.tfetch_counter, 2));

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

  if (line_reset) CHECK_P(state_new.XYMU_RENDERINGn);

  if (state_new.fine_count != 7 && !pause_rendering_old && gen_clk_new(phase_total, 0b10101010)) {
    state_new.fine_count = state_new.fine_count + 1;
  }

  if (state_new.XYMU_RENDERINGn) {
    state_new.fine_count = 0;
  }

  if (line_reset) {
    state_new.win_x.map = 0;
  }
  else if (trigger_win_fetch_state(state_new)) {
    CHECK_N(state_new.XYMU_RENDERINGn);
    state_new.fine_count = 0;
    if (state_new.win_ctrl.PYNU_WIN_MODE_Ap) {
      state_new.win_x.map = state_old.win_x.map + 1;
    }
  }

  if (state_old.win_ctrl.PYNU_WIN_MODE_Ap && !state_new.win_ctrl.PYNU_WIN_MODE_Ap) {
    //CHECK_N(state_new.XYMU_RENDERINGn); nope
    uint8_t win_old = state_old.win_y.tile | (state_old.win_y.map << 3);
    uint8_t win_new = win_old + 1;

    state_new.win_y.tile = win_new & 0b111;
    state_new.win_y.map = win_new >> 3;
  }

  if (state_new.lcd.POPU_y144p) {
    state_new.win_y.tile = 0;
    state_new.win_y.map = 0;
  }

  if (get_bit(state_new.reg_lcdc, 7)) {
    state_new.fine_count = 0;
    state_new.win_x.map = 0;
    state_new.win_y.tile = 0;
    state_new.win_y.map = 0;
  }

  //----------------------------------------
  // Pixel pipes

  if (!state_new.XYMU_RENDERINGn) {
    if (!SACU_CLKPIPE_old && SACU_CLKPIPE_new) {
      state_new.spr_pipe_a = (state_new.spr_pipe_a << 1) | 0;
      state_new.spr_pipe_b = (state_new.spr_pipe_b << 1) | 0;
      state_new.bgw_pipe_a = (state_new.bgw_pipe_a << 1) | 0;
      state_new.bgw_pipe_b = (state_new.bgw_pipe_b << 1) | 0;
      state_new.mask_pipe  = (state_new.mask_pipe  << 1) | 1;
      state_new.pal_pipe   = (state_new.pal_pipe   << 1) | 0;
    }

    if (bit(state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp.state)) {
      uint8_t sprite_mask = state_new.spr_pipe_b | state_new.spr_pipe_a;

      state_new.spr_pipe_a = (state_new.spr_pipe_a & sprite_mask) | (~state_new.sprite_pix_a & ~sprite_mask);
      state_new.spr_pipe_b = (state_new.spr_pipe_b & sprite_mask) | (~state_new.sprite_pix_b & ~sprite_mask);
      state_new.mask_pipe  = get_bit(state_new.oam_temp_b, 7) ? state_new.mask_pipe | ~sprite_mask : state_new.mask_pipe & sprite_mask;
      state_new.pal_pipe   = get_bit(state_new.oam_temp_b, 4) ? state_new.pal_pipe  | ~sprite_mask : state_new.pal_pipe  & sprite_mask;
    }

    if (bit(BFETCH_RSTp_new)) {
      state_new.bgw_pipe_a = ~state_new.tile_temp_a;
      state_new.bgw_pipe_b =  state_new.tile_temp_b;
    }

    uint8_t bg_en = !get_bit(state_new.reg_lcdc, 0);
    uint8_t sp_en = !get_bit(state_new.reg_lcdc, 1);

    uint8_t bgw_pix_a = get_bit(state_new.bgw_pipe_a, 7) & bg_en;
    uint8_t bgw_pix_b = get_bit(state_new.bgw_pipe_b, 7) & bg_en;
    uint8_t spr_pix_a = get_bit(state_new.spr_pipe_a, 7) & sp_en;
    uint8_t spr_pix_b = get_bit(state_new.spr_pipe_b, 7) & sp_en;
    uint8_t mask_pix  = get_bit(state_new.mask_pipe,  7);
    uint8_t pal_pix   = get_bit(state_new.pal_pipe,   7);

    uint8_t spr_col = pack(spr_pix_a, spr_pix_b);
    uint8_t bgw_col = pack(bgw_pix_a, bgw_pix_b);
    
    uint8_t pal = 0;
    uint8_t col = 0;

    if (spr_col == 0 || (mask_pix & (bgw_col != 0))) {
      pal = state_new.reg_bgp ^ 0xFF;
      col = bgw_col;
    }
    else if (pal_pix) {
      pal = state_new.reg_obp1 ^ 0xFF;
      col = spr_col;
    }
    else {
      pal = state_new.reg_obp0 ^ 0xFF;
      col = spr_col;
    }

    state_new.lcd.REMY_LD0n = (pal >> (col * 2 + 0)) & 1;
    state_new.lcd.RAVO_LD1n = (pal >> (col * 2 + 1)) & 1;
  }

  //----------------------------------------
  // LCD pins

  if (!get_bit(state_new.reg_lcdc, 7)) {
    if (DELTA_FG) {
      state_new.lcd.SYGU_LINE_STROBE.state = (state_old.reg_lx == 0) || (state_old.reg_lx == 7) || (state_old.reg_lx == 45) || (state_old.reg_lx == 83);
    }
    pins.lcd.PIN_52_LCD_CNTRL.state = !state_new.lcd.SYGU_LINE_STROBE && !state_new.lcd.RUTU_x113p;

    if (state_old.lcd.RUTU_x113p && !state_new.lcd.RUTU_x113p) state_new.lcd.LUCA_LINE_EVENp.state = !state_new.lcd.LUCA_LINE_EVENp;
    if (!state_old.lcd.POPU_y144p && state_new.lcd.POPU_y144p) state_new.lcd.NAPO_FRAME_EVENp.state = !state_new.lcd.NAPO_FRAME_EVENp;
    pins.lcd.PIN_56_LCD_FLIPS.state = state_new.lcd.NAPO_FRAME_EVENp ^ state_new.lcd.LUCA_LINE_EVENp;

    if (state_old.lcd.NYPE_x113p && !state_new.lcd.NYPE_x113p) {
      state_new.lcd.MEDA_VSYNC_OUTn.state = state_new.reg_ly == 0;
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
    state_new.lcd.SYGU_LINE_STROBE.state = 0;
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

  const bool cpu_addr_vram_new = (state_new.cpu_abus >= 0x8000) && (state_new.cpu_abus <= 0x9FFF);
  if (state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp && !cpu_addr_vram_new) {
    state_new.ext_addr_latch = state_new.cpu_abus & 0x7FFF;
  }

  if (state_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) {
    pins.ctrl.PIN_80_CSn.state = !get_bit(state_new.reg_dma, 7);
    bit_unpack_inv(pins.abus_lo, state_new.dma_lo);
    bit_unpack(pins.abus_hi,     state_new.reg_dma);
  }

  if (!state_new.MATU_DMA_RUNNINGp || dma_addr_vram_new) {
    const bool cpu_addr_ram_new =  (state_new.cpu_abus >= 0xA000) && (state_new.cpu_abus <= 0xFDFF);
    pins.ctrl.PIN_80_CSn.state = (gen_clk_new(phase_total, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp) && cpu_addr_ram_new;
    bit_unpack_inv(pins.abus_lo, (state_new.ext_addr_latch >> 0) & 0xFF);
    bit_unpack_inv(pins.abus_hi, (state_new.ext_addr_latch >> 8) & 0x7F);
  }

  pins.ctrl.PIN_79_RDn.state = 1;
  pins.ctrl.PIN_78_WRn.state = 0;
  if (!(state_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp && state_new.cpu_signals.SIG_IN_CPU_WRp) {
    pins.ctrl.PIN_79_RDn.state = cpu_addr_vram_new;
    pins.ctrl.PIN_78_WRn.state = gen_clk_new(phase_total, 0b00001110) && !cpu_addr_vram_new;
  }

  pins.abus_hi.PIN_16_A15.state = 0;
  if (state_new.MATU_DMA_RUNNINGp && !dma_addr_vram_new) {
    pins.abus_hi.PIN_16_A15.state = !!(state_new.reg_dma & 0b10000000);
  }
  else if (!state_new.cpu_signals.TEPU_BOOT_BITn && state_new.cpu_abus <= 0x00FF) {
  }
  else {
    uint8_t bit = (gen_clk_new(phase_total, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp) && !get_bit(state_new.cpu_abus, 15);
    pins.abus_hi.PIN_16_A15.state = bit;
  }

  uint8_t pins_dbus = 0;
  if (state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp && state_new.cpu_signals.SIG_IN_CPU_WRp && !cpu_addr_vram_new) {
    pins_dbus = ~state_new.cpu_dbus;
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
      pins_dbus = ~data_in;
    }
  }

  //----------------------------------------
  // Ext write

  {
    const uint16_t ext_addr_lo = (uint16_t)bit_pack_inv(pins.abus_lo);
    const uint16_t ext_addr_hi = (uint16_t)bit_pack_inv(pins.abus_hi);
    const uint16_t ext_addr = (ext_addr_lo << 0) | (ext_addr_hi << 8);

    const auto region = ext_addr >> 13;
    const uint8_t data_out = ~pins_dbus;
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

  //--------------------------------------------
  // SCX/SCY regs

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) {
    if (state_new.cpu_abus == 0xFF42) state_new.reg_scy = ~state_old.cpu_dbus;
    if (state_new.cpu_abus == 0xFF43) state_new.reg_scx = ~state_old.cpu_dbus;
  }

  //----------------------------------------
  // VRAM bus

  if (state_new.cpu_signals.SIG_IN_CPU_RDp && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp && !cpu_addr_vram_new && state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
    state_new.cpu_dbus = ~state_new.ext_data_latch;
  }
  else {
    state_new.ext_data_latch = pins_dbus;
  }

  state_new.vram_abus = VRAM_ADDR_MASK;
  state_new.vram_dbus = 0xFF;

  // CPU vram read address

  if (!dma_addr_vram_new && state_new.XYMU_RENDERINGn) {
    state_new.vram_abus = (~state_new.cpu_abus) & VRAM_ADDR_MASK;
  }

  // DMA vram read address

  if (dma_addr_vram_new) {
    state_new.vram_abus = uint8_t(~state_new.dma_lo);
    state_new.vram_abus |= (state_new.reg_dma << 8);
    state_new.vram_abus &= VRAM_ADDR_MASK;
  }

  //--------------------------------------------

  if (state_new.tfetch_control.LONY_FETCHINGp) {
    const auto px  = state_new.pix_count;
    const auto scx = ~state_new.reg_scx;
    const auto scy = ~state_new.reg_scy;

    const auto sum_x = px + scx;
    const auto sum_y = state_new.reg_ly + scy;

    //--------------------------------------------
    // BG map read address

    if (!get_bit(state_new.tfetch_counter, 1) &&
        !get_bit(state_new.tfetch_counter, 2) &&
        !state_new.win_ctrl.PYNU_WIN_MODE_Ap)
    {
      const auto bgmap_en = !get_bit(state_new.reg_lcdc, 3);

      uint32_t addr = 0;
      bit_cat(addr,  0,  4, (px + scx) >> 3);
      bit_cat(addr,  5,  9, (state_new.reg_ly + scy) >> 3);
      bit_cat(addr, 10, 10, bgmap_en);
      bit_cat(addr, 11, 11, 1);
      bit_cat(addr, 12, 12, 1);

      state_new.vram_abus = uint16_t(addr ^ VRAM_ADDR_MASK);
    }

    //--------------------------------------------
    // BG/Win tile read address

    if (get_bit(state_new.tfetch_counter, 1) || get_bit(state_new.tfetch_counter, 2))
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

    if (!get_bit(state_new.tfetch_counter, 1) &&
        !get_bit(state_new.tfetch_counter, 2) &&
         state_new.win_ctrl.PYNU_WIN_MODE_Ap)
    {
      uint32_t addr = 0;
      bit_cat(addr,  0,  4, ~state_new.win_x.map);
      bit_cat(addr,  5,  9, ~state_new.win_y.map);
      bit_cat(addr, 10, 10, get_bit(state_new.reg_lcdc, 6));
      state_new.vram_abus = uint16_t(addr);
    }
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

  state_new.vram_dbus = 0xFF;

  uint8_t pins_vram_dbus = 0xFF;

  if (!state_new.XYMU_RENDERINGn) {
    pins.vram_ctrl.PIN_43_VRAM_CSn.state = dma_addr_vram_new || state_new.tfetch_control.LONY_FETCHINGp || state_new.sfetch_control.TEXY_SFETCHINGp;
    pins.vram_ctrl.PIN_45_VRAM_OEn.state = dma_addr_vram_new || state_new.tfetch_control.LONY_FETCHINGp || (state_new.sfetch_control.TEXY_SFETCHINGp && (!state_new.sfetch_control.TYFO_SFETCH_S0p_D1 || get_bit(state_new.sfetch_counter, 0)));
    pins.vram_ctrl.PIN_49_VRAM_WRn.state = 0;

    if (pins.vram_ctrl.PIN_45_VRAM_OEn) {
      state_new.vram_dbus = mem.vid_ram[state_new.vram_abus ^ 0x1FFF];
    }

    pins_vram_dbus = state_new.vram_dbus;
  }




  else {
    if ((gen_clk_new(phase_total, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp) && 1 && cpu_addr_vram_new && state_new.cpu_signals.SIG_IN_CPU_WRp) {
      state_new.vram_dbus = state_new.cpu_dbus;
    }

    pins.vram_ctrl.PIN_43_VRAM_CSn.state = (cpu_addr_vram_new && gen_clk_new(phase_total, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp) || dma_addr_vram_new;
    pins.vram_ctrl.PIN_45_VRAM_OEn.state = (!cpu_addr_vram_new || !state_new.cpu_signals.SIG_IN_CPU_WRp) || dma_addr_vram_new;
    pins.vram_ctrl.PIN_49_VRAM_WRn.state = cpu_addr_vram_new && gen_clk_new(phase_total, 0b00001110) && state_new.cpu_signals.SIG_IN_CPU_WRp && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp;

    uint8_t vdata = 0xFF;

    if (pins.vram_ctrl.PIN_45_VRAM_OEn) {
      vdata = mem.vid_ram[state_new.vram_abus ^ 0x1FFF];
    }

    pins_vram_dbus = 0;

    if (pins.vram_ctrl.PIN_45_VRAM_OEn) {
      pins_vram_dbus = ~vdata;
    }

    if (cpu_addr_vram_new && (gen_clk_new(phase_total, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp) && state_new.cpu_signals.SIG_IN_CPU_WRp) {
      pins_vram_dbus = ~state_new.vram_dbus;
    }
    else {
      state_new.vram_dbus = ~pins_vram_dbus;
    }

    if (pins.vram_ctrl.PIN_49_VRAM_WRn) {
      mem.vid_ram[state_new.vram_abus ^ 0x1FFF] = ~pins_vram_dbus;
    }

    if (cpu_addr_vram_new && (gen_clk_new(phase_total, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp) && state_new.cpu_signals.SIG_IN_CPU_RDp && state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
      state_new.cpu_dbus = state_new.vram_dbus;
    }
  }

  //----------------------------------------
  // CPU read registers
  
  // This has to be before OAM, because the OAM dbus mirrors CPU dbus if it's not doing other things

  // But ZRAM can't be before OAM because ZRAM _doesn't_ show up on the oam bus? That seems wrong...

  if (state_new.cpu_signals.SIG_IN_CPU_RDp) {
    if (state_new.cpu_abus == 0xFF00) {
      set_bit(state_new.cpu_dbus, 0, !get_bit(state_new.joy_latch, 0));
      set_bit(state_new.cpu_dbus, 1, !get_bit(state_new.joy_latch, 1));
      set_bit(state_new.cpu_dbus, 2, !get_bit(state_new.joy_latch, 2));
      set_bit(state_new.cpu_dbus, 3, !get_bit(state_new.joy_latch, 3));
      set_bit(state_new.cpu_dbus, 4,  get_bit(state_new.reg_joy, 0));
      set_bit(state_new.cpu_dbus, 5,  get_bit(state_new.reg_joy, 1));
    }
    if (state_new.cpu_abus == 0xFF04) state_new.cpu_dbus =  (uint8_t)(state_new.reg_div >> 6);
    if (state_new.cpu_abus == 0xFF05) state_new.cpu_dbus =  state_new.reg_tima;
    if (state_new.cpu_abus == 0xFF06) state_new.cpu_dbus =  state_new.reg_tma;
    if (state_new.cpu_abus == 0xFF07) state_new.cpu_dbus =  state_new.reg_tac | 0b11111000;
    if (state_new.cpu_abus == 0xFF40) state_new.cpu_dbus = ~state_new.reg_lcdc;
    if (state_new.cpu_abus == 0xFF42) state_new.cpu_dbus = ~state_new.reg_scy;
    if (state_new.cpu_abus == 0xFF43) state_new.cpu_dbus = ~state_new.reg_scx;
    if (state_new.cpu_abus == 0xFF44) state_new.cpu_dbus =  state_new.reg_ly;
    if (state_new.cpu_abus == 0xFF45) state_new.cpu_dbus = ~state_old.reg_lyc;
    if (state_new.cpu_abus == 0xFF47) state_new.cpu_dbus = ~state_new.reg_bgp;
    if (state_new.cpu_abus == 0xFF48) state_new.cpu_dbus = ~state_new.reg_obp0;
    if (state_new.cpu_abus == 0xFF49) state_new.cpu_dbus = ~state_new.reg_obp1;
    if (state_new.cpu_abus == 0xFF4A) state_new.cpu_dbus = ~state_new.reg_wy;
    if (state_new.cpu_abus == 0xFF4B) state_new.cpu_dbus = ~state_new.reg_wx;
  }

  //----------------------------------------
  // oam

  // this is weird, why is it always 0 when not in reset?
  state_new.oam_ctrl.MAKA_LATCH_EXTp.state = 0;

  // OAM bus has to be pulled up or we fail regression
  state_new.oam_abus = 0xFF;
  state_new.oam_dbus_a = 0xFF;
  state_new.oam_dbus_b = 0xFF;

  //----------
  // OAM bus and control signals.
  // The inclusion of cpu_addr_oam_new in the SCANNING and RENDERING branches is probably a hardware bug.

  const bool cpu_addr_oam_new =  (state_new.cpu_abus >= 0xFE00) && (state_new.cpu_abus <= 0xFEFF);

  if (scanning_new) {
    state_new.oam_abus = (uint8_t)~((state_new.scan_counter << 2) | 0b00);
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
    state_new.oam_ctrl.SIG_OAM_OEn.state   = 0;

    if (DELTA_GH || DELTA_HA || DELTA_CD || DELTA_DE) {
      state_new.oam_ctrl.SIG_OAM_CLKn.state  = (DELTA_DE && !cpu_addr_oam_new) || DELTA_HA;
      state_new.oam_ctrl.SIG_OAM_OEn.state   = !(cpu_addr_oam_new && state_new.cpu_signals.SIG_IN_CPU_RDp && !state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE);
    }

    if (!state_new.oam_ctrl.SIG_OAM_OEn) {
      state_new.oam_dbus_a = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) & ~1];
      state_new.oam_dbus_b = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) |  1];
    }
  }
  else if (!state_new.XYMU_RENDERINGn) {
    const auto sfetch_oam_clk_new = (get_bit(state_new.sfetch_counter, 1) || get_bit(state_new.sfetch_counter, 2) || (state_new.sfetch_control.TYFO_SFETCH_S0p_D1 && !get_bit(state_new.sfetch_counter, 0)));
    const auto sfetch_oam_oen_new = (get_bit(state_new.sfetch_counter, 1) || get_bit(state_new.sfetch_counter, 2) || !state_new.sfetch_control.TYFO_SFETCH_S0p_D1);

    state_new.oam_abus = (uint8_t)~((state_new.sprite_ibus  << 2) | 0b11);
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = sfetch_oam_clk_new && (!cpu_addr_oam_new || gen_clk_new(phase_total, 0b11110000));
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
    state_new.oam_ctrl.SIG_OAM_OEn.state   = sfetch_oam_oen_new && !(cpu_addr_oam_new && state_new.cpu_signals.SIG_IN_CPU_RDp && !state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE);

    if (!sfetch_oam_oen_new) {
      state_new.oam_dbus_a = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) & ~1];
      state_new.oam_dbus_b = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) |  1];
    }
    else if (cpu_addr_oam_new && state_new.cpu_signals.SIG_IN_CPU_RDp && !state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
      state_new.oam_dbus_a = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) & ~1];
      state_new.oam_dbus_b = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) |  1];
    }
  }
  else if (cpu_addr_oam_new) {
    state_new.oam_abus = (uint8_t)~state_new.cpu_abus;
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = DELTA_HA || DELTA_AB || DELTA_BC || DELTA_CD;
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
    state_new.oam_ctrl.SIG_OAM_OEn.state   = !(state_new.cpu_signals.SIG_IN_CPU_RDp && !state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE);

    if (DELTA_DE || DELTA_EF || DELTA_FG) {
      state_new.oam_ctrl.SIG_OAM_WRn_A.state = !state_new.cpu_signals.SIG_IN_CPU_WRp || !get_bit(state_new.oam_abus, 0);
      state_new.oam_ctrl.SIG_OAM_WRn_B.state = !state_new.cpu_signals.SIG_IN_CPU_WRp ||  get_bit(state_new.oam_abus, 0);
    }

    if (state_new.cpu_signals.SIG_IN_CPU_RDp) {
      if (state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
        state_new.cpu_dbus = get_bit(state_new.oam_abus, 0) ? ~state_new.oam_latch_a : ~state_new.oam_latch_b;
      }
      else {
        state_new.oam_dbus_a = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) & ~1];
        state_new.oam_dbus_b = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) |  1];
      }
    }
  }
  else {
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
    state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
    state_new.oam_abus = (uint8_t)~state_new.cpu_abus;
    state_new.oam_dbus_a = ~state_new.cpu_dbus;
    state_new.oam_dbus_b = ~state_new.cpu_dbus;
  }

  // DMA overrides oam bus

  if (state_new.MATU_DMA_RUNNINGp) {
    state_new.oam_abus = (uint8_t)~state_new.dma_lo;

    if (DELTA_HA || DELTA_AB || DELTA_BC || DELTA_CD) {
      state_new.oam_ctrl.SIG_OAM_CLKn.state  = 1;
      state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
      state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
      state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
    }
    else {
      state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
      state_new.oam_ctrl.SIG_OAM_WRn_A.state = !get_bit(state_new.oam_abus, 0);
      state_new.oam_ctrl.SIG_OAM_WRn_B.state =  get_bit(state_new.oam_abus, 0);
      state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
    }

    if (dma_addr_vram_new) {
      state_new.oam_dbus_a = ~state_new.vram_dbus;
      state_new.oam_dbus_b = ~state_new.vram_dbus;
    }
    else {
      state_new.oam_dbus_a = pins_dbus;
      state_new.oam_dbus_b = pins_dbus;
    }
  }

  // WUJE is weird, not sure why it's necessary. Bugfix?
  if (gen_clk_new(phase_total, 0b11110000)) state_new.oam_ctrl.WUJE_CPU_OAM_WRn.state = 1;
  if (cpu_addr_oam_new && state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00001110)) state_new.oam_ctrl.WUJE_CPU_OAM_WRn.state = 0;

  if (!state_new.oam_ctrl.WUJE_CPU_OAM_WRn) {
    state_new.oam_dbus_a = ~state_new.cpu_dbus;
    state_new.oam_dbus_b = ~state_new.cpu_dbus;
  }

  // OAM latch operates on the overridden bus

  if (scanning_new) {
    if (DELTA_AB || DELTA_BC || DELTA_EF || DELTA_FG) {
      state_new.oam_latch_a = state_new.oam_dbus_a;
      state_new.oam_latch_b = state_new.oam_dbus_b;
    }
  }
  else if (!state_new.XYMU_RENDERINGn) {
    if (!(get_bit(state_new.sfetch_counter, 1) || get_bit(state_new.sfetch_counter, 2) || !state_new.sfetch_control.TYFO_SFETCH_S0p_D1)) {
      state_new.oam_latch_a = state_new.oam_dbus_a;
      state_new.oam_latch_b = state_new.oam_dbus_b;
    }
  }
  else {
    if (cpu_addr_oam_new && state_new.cpu_signals.SIG_IN_CPU_RDp && !state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE) {
      state_new.oam_latch_a = state_new.oam_dbus_a;
      state_new.oam_latch_b = state_new.oam_dbus_b;
    }
  }

  // Actual OAM write
  if (state_old.oam_ctrl.SIG_OAM_CLKn && !state_new.oam_ctrl.SIG_OAM_CLKn) {
    uint8_t oam_addr_new = uint8_t(~state_new.oam_abus) >> 1;
    if (!state_new.oam_ctrl.SIG_OAM_WRn_A) mem.oam_ram[(oam_addr_new << 1) + 0] = ~state_new.oam_dbus_a;
    if (!state_new.oam_ctrl.SIG_OAM_WRn_B) mem.oam_ram[(oam_addr_new << 1) + 1] = ~state_new.oam_dbus_b;
  }
 
  //----------------------------------------
  // zram

  if (state_new.cpu_signals.SIG_IN_CPU_WRp && DELTA_GH) {
    if ((state_new.cpu_abus >= 0xFF80) && (state_new.cpu_abus <= 0xFFFE)) mem.zero_ram[state_new.cpu_abus & 0x007F] = state_old.cpu_dbus;
  }

  if (state_new.cpu_signals.SIG_IN_CPU_RDp) {
    if ((state_new.cpu_abus >= 0xFF80) && (state_new.cpu_abus <= 0xFFFE)) state_new.cpu_dbus = mem.zero_ram[state_new.cpu_abus & 0x007F];
  }

  //----------------------------------------
  // And finally, interrupts.

  auto pack_cpu_dbus_old = state_old.cpu_dbus;
  auto pack_cpu_dbus_new = state_new.cpu_dbus;
  auto pack_ie = state_new.reg_ie;
  auto pack_if = state_new.reg_if;
  auto pack_stat = state_new.reg_stat;

  if (!get_bit(state_new.reg_lcdc, 7) && DELTA_BC) {
    state_new.int_ctrl.ROPO_LY_MATCH_SYNCp.state = state_old.reg_ly == (state_old.reg_lyc ^ 0xFF);
  }

  // FIXME this seems slightly wrong...
  if (state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00001110) && state_new.cpu_abus == 0xFF41) {
  }
  else {
    state_new.int_ctrl.RUPO_LYC_MATCHn.state = 1;
  }

  // but the "reset" arm of the latch overrides the "set" arm, so it doesn't completely break?
  if (state_new.int_ctrl.ROPO_LY_MATCH_SYNCp) {
    state_new.int_ctrl.RUPO_LYC_MATCHn.state = 0;
  }

  if (state_new.cpu_abus == 0xFFFF && state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) {
    pack_ie = pack_cpu_dbus_old;
  }

  if (state_new.cpu_abus == 0xFF41 && state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00000001)) {
    pack_stat = (~pack_cpu_dbus_old >> 3) & 0b00001111;
  }

  if (state_new.cpu_abus == 0xFF41 && state_new.cpu_signals.SIG_IN_CPU_RDp) {
    uint8_t stat = 0x80;

    stat |= (!state_new.XYMU_RENDERINGn || state_new.lcd.POPU_y144p) << 0;
    stat |= (!state_new.XYMU_RENDERINGn || scanning_new) << 1;
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
  if (state_new.cpu_signals.SIG_IN_CPU_WRp && gen_clk_new(phase_total, 0b00001110)) {
    if (state_new.cpu_abus == 0xFF0F) pack_if = pack_cpu_dbus_new;
  }

  pack_if &= ~cpu.core.int_ack;

  if (state_new.cpu_signals.SIG_IN_CPU_RDp) {
    if (state_new.cpu_abus == 0xFFFF) pack_cpu_dbus_new = pack_ie | 0b11100000;
    if (state_new.cpu_abus == 0xFF0F) state_new.int_latch = (uint8_t)pack_if;
    if (state_new.cpu_abus == 0xFF0F) pack_cpu_dbus_new = pack_if | 0b11100000;
  }


  state_new.cpu_dbus = (uint8_t)pack_cpu_dbus_new;
  state_new.cpu_int = (uint8_t)pack_if;
  state_new.reg_ie = (uint8_t)pack_ie;
  state_new.reg_if = (uint8_t)pack_if;
  state_new.reg_stat = (uint8_t)pack_stat;

  // POSTCONDITIONS
  if (scanning_new)    CHECK_P(state_new.XYMU_RENDERINGn);
  if (!state_new.XYMU_RENDERINGn)  CHECK_N(scanning_new);
























  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  //===========================================================================
  // These are all dead (unused) signals that are only needed for regression tests

  if (!config_fastmode) {
    state_new.ACYL_SCANNINGp = scanning_new;
    state_new.sprite_scanner.FETO_SCAN_DONEp = (state_new.scan_counter == 39) && !get_bit(state_new.reg_lcdc, 7);
    state_new.SATO_BOOT_BITn = get_bit(state_new.cpu_dbus, 0) || state_new.cpu_signals.TEPU_BOOT_BITn;
    state_new.ATEJ_LINE_RSTp = line_reset;
    state_new.cpu_signals.SIG_CPU_BOOTp.state = 0;
    state_new.cpu_signals.SIG_BOOT_CSp.state = 0;

    if (state_new.cpu_abus <= 0x00FF) {

      state_new.cpu_signals.SIG_CPU_BOOTp.state = !state_new.cpu_signals.TEPU_BOOT_BITn;

      if (state_new.cpu_signals.SIG_IN_CPU_RDp && !state_new.cpu_signals.TEPU_BOOT_BITn) {
        state_new.cpu_signals.SIG_BOOT_CSp.state = 1;
      }
    }

    bit_unpack(pins.vram_dbus, pins_vram_dbus);
    bit_unpack(pins.vram_abus, state_new.vram_abus);

    bit_unpack(pins.dbus, pins_dbus);

    state_new.win_ctrl.ROGE_WY_MATCHp = (state_new.reg_ly == uint8_t(~state_new.reg_wy)) && !get_bit(state_new.reg_lcdc, 5);

    if (state_new.cpu_signals.SIG_IN_CPU_WRp && DELTA_GH) {
      if (state_new.cpu_abus == 0xFF00) pins.joy.PIN_63_JOY_P14.state = get_bit(state_old.cpu_dbus, 4);
      if (state_new.cpu_abus == 0xFF00) pins.joy.PIN_62_JOY_P15.state = get_bit(state_old.cpu_dbus, 5);
    }

    if (!get_bit(state_new.reg_joy, 0)) {
      pins.joy.PIN_67_JOY_P10.state = get_bit(sys.buttons, 0); // RIGHT
      pins.joy.PIN_66_JOY_P11.state = get_bit(sys.buttons, 1); // LEFT
      pins.joy.PIN_65_JOY_P12.state = get_bit(sys.buttons, 2); // UP
      pins.joy.PIN_64_JOY_P13.state = get_bit(sys.buttons, 3); // DOWN
    }
    else if (!get_bit(state_new.reg_joy, 1)) {
      pins.joy.PIN_67_JOY_P10.state = get_bit(sys.buttons, 4); // A
      pins.joy.PIN_66_JOY_P11.state = get_bit(sys.buttons, 5); // B
      pins.joy.PIN_65_JOY_P12.state = get_bit(sys.buttons, 6); // SELECT
      pins.joy.PIN_64_JOY_P13.state = get_bit(sys.buttons, 7); // START
    }

    if (get_bit(state_new.oam_temp_b, 5) && state_new.sfetch_control.TEXY_SFETCHINGp) {
      state_new.flipped_sprite = bit_reverse(state_new.vram_dbus);
    }
    else {
      state_new.flipped_sprite = state_new.vram_dbus;
    }

    state_new.oam_ctrl.old_oam_clk = !state_new.oam_ctrl.SIG_OAM_CLKn; // Vestige of gate mode
    state_new.zram_bus.clk_old = gen_clk_new(phase_total, 0b00001110) && state_new.cpu_signals.SIG_IN_CPU_WRp;

    state_new.cpu_ack = cpu.core.int_ack;

    pins.sys.PIN_74_CLK.CLK.state = gen_clk_new(phase_total, 0b10101010); // dead signal
    pins.sys.PIN_74_CLK.CLKGOOD.state = 1; // dead signal

    pins.sys.PIN_71_RST = 0; // dead signal
    pins.sys.PIN_76_T2 = 0; // dead signal
    pins.sys.PIN_77_T1 = 0; // dead signal

    state_new.sys_clk.SIG_CPU_CLKREQ.state = 1; // dead signal

    state_new.cpu_signals.SIG_CPU_ADDR_HIp.state = state_new.cpu_abus >= 0xFE00 && state_new.cpu_abus <= 0xFFFF; // dead signal
    state_new.cpu_signals.SIG_CPU_UNOR_DBG.state = 0; // dead signal
    state_new.cpu_signals.SIG_CPU_UMUT_DBG.state = 0; // dead signal

    pins.sys.PIN_73_CLK_DRIVE.state = pins.sys.PIN_74_CLK.CLK; // dead signal
    state_new.sys_clk.AVET_DEGLITCH.state = pins.sys.PIN_74_CLK.CLK; // dead signal
    state_new.sys_clk.ANOS_DEGLITCH.state = !pins.sys.PIN_74_CLK.CLK; // dead signal

    state_new.sys_clk.AFUR_xxxxEFGH.state = gen_clk_new(phase_total, 0b00001111); // dead signal
    state_new.sys_clk.ALEF_AxxxxFGH.state = gen_clk_new(phase_total, 0b10000111); // dead signal
    state_new.sys_clk.APUK_ABxxxxGH.state = gen_clk_new(phase_total, 0b11000011); // dead signal
    state_new.sys_clk.ADYK_ABCxxxxH.state = gen_clk_new(phase_total, 0b11100001); // dead signal

    pins.sys.PIN_75_CLK_OUT.state = gen_clk_new(phase_total, 0b00001111); // dead signal

    state_new.sys_clk.SIG_CPU_BOWA_Axxxxxxx.state = gen_clk_new(phase_total, 0b10000000); // dead signal
    state_new.sys_clk.SIG_CPU_BEDO_xBCDEFGH.state = gen_clk_new(phase_total, 0b01111111); // dead signal
    state_new.sys_clk.SIG_CPU_BEKO_ABCDxxxx.state = gen_clk_new(phase_total, 0b11110000); // dead signal
    state_new.sys_clk.SIG_CPU_BUDE_xxxxEFGH.state = gen_clk_new(phase_total, 0b00001111); // dead signal
    state_new.sys_clk.SIG_CPU_BOLO_ABCDEFxx.state = gen_clk_new(phase_total, 0b11111100); // dead signal
    state_new.sys_clk.SIG_CPU_BUKE_AxxxxxGH.state = gen_clk_new(phase_total, 0b10000011); // dead signal
    state_new.sys_clk.SIG_CPU_BOMA_xBCDEFGH.state = gen_clk_new(phase_total, 0b01111111); // dead signal
    state_new.sys_clk.SIG_CPU_BOGA_Axxxxxxx.state = gen_clk_new(phase_total, 0b10000000); // dead signal

    state_new.cpu_signals.TEDO_CPU_RDp.state = state_new.cpu_signals.SIG_IN_CPU_RDp; // dead signal
    state_new.cpu_signals.APOV_CPU_WRp = gen_clk_new(phase_total, 0b00001110) && state_new.cpu_signals.SIG_IN_CPU_WRp; // dead signal
    state_new.cpu_signals.TAPU_CPU_WRp = gen_clk_new(phase_total, 0b00001110) && state_new.cpu_signals.SIG_IN_CPU_WRp; // dead signal
    state_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK = (gen_clk_new(phase_total, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp); // dead signal

    state_new.sys_rst.AFER_SYS_RSTp = 0; // dead signal
    state_new.sys_rst.TUBO_WAITINGp = 0; // dead signal
    state_new.sys_rst.ASOL_POR_DONEn = 0; // dead signal
    state_new.sys_rst.SIG_CPU_EXT_CLKGOOD = 1; // dead signal
    state_new.sys_rst.SIG_CPU_EXT_RESETp = 0; // dead signal
    state_new.sys_rst.SIG_CPU_STARTp = 0; // dead signal
    state_new.sys_rst.SIG_CPU_INT_RESETp = 0; // dead signal
    state_new.sys_rst.SOTO_DBG_VRAMp = 0; // dead signal

    state_new.sys_clk.WOSU_AxxDExxH.state = !get_bit(state_new.reg_lcdc, 7) && gen_clk_new(phase_total, 0b10011001); // dead signal
    state_new.sys_clk.WUVU_ABxxEFxx.state = !get_bit(state_new.reg_lcdc, 7) && gen_clk_new(phase_total, 0b11001100); // dead signal
    state_new.sys_clk.VENA_xxCDEFxx.state = !get_bit(state_new.reg_lcdc, 7) && gen_clk_new(phase_total, 0b00111100); // dead signal
  }
}
