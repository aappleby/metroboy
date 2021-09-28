#include "GateBoyLib/LogicBoy.h"
#include "GateBoyLib/GateBoy.h"

#include "GateBoyLib/Utils.h"

FieldInfo LogicBoy::fields[] = {
  DECLARE_FIELD(LogicBoy, lb_state),
  DECLARE_FIELD(LogicBoy, gb_state),
  DECLARE_FIELD(LogicBoy, cpu),
  DECLARE_FIELD(LogicBoy, mem),
  DECLARE_FIELD(LogicBoy, sys),
  DECLARE_FIELD(LogicBoy, pins),
  DECLARE_FIELD(LogicBoy, probes),
};

#define gb_state FORBIDDEN

//-----------------------------------------------------------------------------

GBResult LogicBoy::reset_to_bootrom(const blob& cart_blob)
{
  lb_state.reset_to_bootrom();
  cpu.reset_to_bootrom();
  mem.reset_to_bootrom();
  sys.reset_to_bootrom();
  
  pins.reset_to_bootrom();
  pins = bit_purge(pins);

  probes.reset_to_bootrom();
  lb_bit_check();
  return GBResult::ok();
}

//-----------------------------------------------------------------------------

GBResult LogicBoy::reset_to_cart(const blob& cart_blob) {
  lb_state.reset_to_cart();
  cpu.reset_to_cart();
  mem.reset_to_cart();
  sys.reset_to_cart();
  
  pins.reset_to_cart();
  pins = bit_purge(pins);

  probes.reset_to_cart();
  lb_bit_check();
  return GBResult::ok();
}

//-----------------------------------------------------------------------------

GBResult LogicBoy::peek(int addr) const {
  if (addr >= 0x8000 && addr <= 0x9FFF) { return GBResult(mem.vid_ram[addr - 0x8000]);  }
  if (addr >= 0xA000 && addr <= 0xBFFF) { return GBResult(mem.cart_ram[addr - 0xA000]); }
  if (addr >= 0xC000 && addr <= 0xDFFF) { return GBResult(mem.int_ram[addr - 0xC000]);  }
  if (addr >= 0xE000 && addr <= 0xFDFF) { return GBResult(mem.int_ram[addr - 0xE000]);  }
  if (addr >= 0xFE00 && addr <= 0xFEFF) { return GBResult(mem.oam_ram[addr - 0xFE00]);  }
  if (addr >= 0xFF80 && addr <= 0xFFFE) { return GBResult(mem.zero_ram[addr - 0xFF80]); }
  return lb_state.peek(addr);
}

GBResult LogicBoy::poke(int addr, uint8_t data_in) {
  if (addr >= 0x8000 && addr <= 0x9FFF) { mem.vid_ram[addr - 0x8000] = data_in;  return GBResult::ok(); }
  if (addr >= 0xA000 && addr <= 0xBFFF) { mem.cart_ram[addr - 0xA000] = data_in; return GBResult::ok(); }
  if (addr >= 0xC000 && addr <= 0xDFFF) { mem.int_ram[addr - 0xC000] = data_in;  return GBResult::ok(); }
  if (addr >= 0xE000 && addr <= 0xFDFF) { mem.int_ram[addr - 0xE000] = data_in;  return GBResult::ok(); }
  if (addr >= 0xFE00 && addr <= 0xFEFF) { mem.oam_ram[addr - 0xFE00] = data_in;  return GBResult::ok(); }
  if (addr >= 0xFF80 && addr <= 0xFFFE) { mem.zero_ram[addr - 0xFF80] = data_in; return GBResult::ok(); }
  return lb_state.poke(addr, data_in);
}

//-----------------------------------------------------------------------------

GBResult LogicBoy::dbg_req(uint16_t addr, uint8_t data, bool write) {
  CHECK_P((sys.gb_phase_total & 7) == 0);

  cpu.bus_req_new.addr = addr;
  cpu.bus_req_new.data = data;
  cpu.bus_req_new.read = !write;
  cpu.bus_req_new.write = write;

  return GBResult::ok();
}

//-----------------------------------------------------------------------------

GBResult LogicBoy::dbg_read(const blob& cart_blob, int addr) {
  CHECK_P((sys.gb_phase_total & 7) == 0);

  Req old_req = cpu.bus_req_new;
  bool old_cpu_en = sys.cpu_en;
  sys.cpu_en = false;

  dbg_req((uint16_t)addr, 0, 0);
  run_phases(cart_blob, 8);

  cpu.bus_req_new = old_req;
  sys.cpu_en = old_cpu_en;

  return GBResult(cpu.cpu_data_latch);
}

//------------------------------------------------------------------------------

GBResult LogicBoy::dbg_write(const blob& cart_blob, int addr, uint8_t data_in) {
  CHECK_P((sys.gb_phase_total & 7) == 0);

  Req old_req = cpu.bus_req_new;
  bool old_cpu_en = sys.cpu_en;
  sys.cpu_en = false;

  dbg_req((uint16_t)addr, data_in, 1);
  GBResult res = run_phases(cart_blob, 8);

  cpu.bus_req_new = old_req;
  sys.cpu_en = old_cpu_en;
  return res;
}

//------------------------------------------------------------------------------------------------------------------------

GBResult LogicBoy::run_phases(const blob& cart_blob, int phase_count) {
  GBResult res = GBResult::ok();
  for (int i = 0; i < phase_count; i++) {
    res &= next_phase(cart_blob);
  }
  return res;
}

GBResult LogicBoy::next_phase(const blob& cart_blob) {
  sys.gb_phase_total++;
  tock_cpu();
  tock_logic(cart_blob);
  return GBResult::ok();
}

//------------------------------------------------------------------------------------------------------------------------

#define PHASE_A_NEW (phase_new == 0)
#define PHASE_B_NEW (phase_new == 1)
#define PHASE_C_NEW (phase_new == 2)
#define PHASE_D_NEW (phase_new == 3)
#define PHASE_E_NEW (phase_new == 4)
#define PHASE_F_NEW (phase_new == 5)
#define PHASE_G_NEW (phase_new == 6)
#define PHASE_H_NEW (phase_new == 7)

void LogicBoy::tock_cpu() {
  auto phase_old = (sys.gb_phase_total - 1) & 7;
  auto phase_new = (sys.gb_phase_total - 0) & 7;

  cpu.cpu_data_latch &= lb_state.cpu_dbus;
  cpu.imask_latch = lb_state.reg_ie;

  if (PHASE_A_NEW) {
    if (cpu.core.op == 0x76 && (cpu.imask_latch & cpu.intf_halt_latch)) cpu.core.state_ = 0;
    cpu.intf_halt_latch = 0;
  }

  // +ha -ab -bc -cd -de -ef -fg -gh
  if (PHASE_A_NEW) {
    // this one latches funny, some hardware bug
    if (get_bit(lb_state.reg_if, 2)) cpu.intf_halt_latch |= INT_TIMER_MASK;
  }

  // -ha +ab -bc
  if (PHASE_B_NEW) {
    if (sys.cpu_en) {
      cpu.core.tock_ab(cpu.imask_latch, cpu.intf_latch, cpu.cpu_data_latch);
    }
  }

  if (PHASE_B_NEW) {
    if (sys.cpu_en) {
      cpu.bus_req_new.addr = cpu.core._bus_addr;
      cpu.bus_req_new.data = cpu.core._bus_data;
      cpu.bus_req_new.read = cpu.core._bus_read;
      cpu.bus_req_new.write = cpu.core._bus_write;
    }
  }

  // -bc +cd +de -ef -fg -gh -ha -ab
  if (PHASE_E_NEW) {
    if (get_bit(lb_state.reg_if, 0)) cpu.intf_halt_latch |= INT_VBLANK_MASK;
    if (get_bit(lb_state.reg_if, 1)) cpu.intf_halt_latch |= INT_STAT_MASK;
    if (get_bit(lb_state.reg_if, 3)) cpu.intf_halt_latch |= INT_SERIAL_MASK;
    if (get_bit(lb_state.reg_if, 4)) cpu.intf_halt_latch |= INT_JOYPAD_MASK;
  }

  // -ha -ab -bc -cd -de -ef +fg +gh
  if (PHASE_H_NEW) {
    cpu.cpu_data_latch = 0xFF;
  }

  // +ha -ab -bc -cd -de -ef -fg +gh
  if (PHASE_H_NEW) {
    cpu.intf_latch = lb_state.reg_if;
  }
}

//-----------------------------------------------------------------------------

#define DELTA_AB_old   (phase_old == 1)
#define DELTA_BC_old   (phase_old == 2)
#define DELTA_CD_old   (phase_old == 3)
#define DELTA_DE_old   (phase_old == 4)
#define DELTA_EF_old   (phase_old == 5)
#define DELTA_FG_old   (phase_old == 6)
#define DELTA_GH_old   (phase_old == 7)
#define DELTA_HA_old   (phase_old == 0)

#define DELTA_AB_new   (phase_old == 0)
#define DELTA_BC_new   (phase_old == 1)
#define DELTA_CD_new   (phase_old == 2)
#define DELTA_DE_new   (phase_old == 3)
#define DELTA_EF_new   (phase_old == 4)
#define DELTA_FG_new   (phase_old == 5)
#define DELTA_GH_new   (phase_old == 6)
#define DELTA_HA_new   (phase_old == 7)

#define DELTA_EVEN_new ((phase_old & 1) == 0)
#define DELTA_ODD_new  ((phase_old & 1) == 1)

#define DELTA_EVEN_old ((phase_old & 1) == 1)
#define DELTA_ODD_old  ((phase_old & 1) == 0)

void LogicBoy::tock_logic(const blob& cart_blob) {

  //----------------------------------------
  // old signals

  LogicBoyState  state_old = lb_state;
  auto phase_total_old = sys.gb_phase_total - 1;
  auto phase_old = (sys.gb_phase_total - 1) & 7;

  wire nuko_wx_match_old = (uint8_t(~state_old.reg_wx) == state_old.pix_count) && state_old.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state;

  //----------------------------------------

  LogicBoyState& state_new = lb_state;
  auto phase_total_new = sys.gb_phase_total - 0;
  auto phase_new = (sys.gb_phase_total - 0) & 7;

  //----------------------------------------

  const auto cpu_dbus_old = state_old.cpu_dbus;

  // Data has to be driven on EFGH or we fail the wave tests

  state_new.cpu_dbus = (DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new) && cpu.bus_req_new.write ? cpu.bus_req_new.data_lo : 0xFF; // must be pulled up or we fail regression
  state_new.cpu_abus = (DELTA_HA_new ? cpu.bus_req_new.addr & 0x00FF : cpu.bus_req_new.addr);

  wire req_addr_vram = (cpu.bus_req_new.addr >= 0x8000) && (cpu.bus_req_new.addr <= 0x9FFF);
  wire req_addr_ram  = (cpu.bus_req_new.addr >= 0xA000) && (cpu.bus_req_new.addr <= 0xFDFF);
  wire req_addr_oam  = (cpu.bus_req_new.addr >= 0xFE00) && (cpu.bus_req_new.addr <= 0xFEFF);
  wire req_addr_hi   = (cpu.bus_req_new.addr >= 0xFE00);
  wire req_addr_lo   = (cpu.bus_req_new.addr <= 0x00FF);

  wire cpu_addr_oam_old  = (state_old.cpu_abus >= 0xFE00) && (state_old.cpu_abus <= 0xFEFF);

  wire cpu_addr_vram_new = req_addr_vram && !DELTA_HA_new;
  wire cpu_addr_ram_new  = req_addr_ram && !DELTA_HA_new;
  wire cpu_addr_oam_new  = req_addr_oam && !DELTA_HA_new;

  wire cpu_rd = cpu.bus_req_new.read  && !DELTA_HA_new;
  wire cpu_wr = cpu.bus_req_new.write && !DELTA_HA_new;

  const auto cpu_addr_new = state_new.cpu_abus;
  auto& cpu_dbus_new = state_new.cpu_dbus;

  //----------------------------------------
  // Timer & DIV

  {
    if (DELTA_HA_new) {
      state_new.reg_div = state_new.reg_div + 1;
    }

    if (cpu_wr && (DELTA_DE_new || DELTA_EF_new || DELTA_FG_new)) {
      if (cpu_addr_new == 0xFF04) state_new.reg_div = 0;
    }

    if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF06) state_new.reg_tma  =  state_old.cpu_dbus;
    if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF07) state_new.reg_tac  =  state_old.cpu_dbus & 0b111;

    bool SOGU_TIMA_CLKn_old = 0;
    bool SOGU_TIMA_CLKn_new = 0;

    if ((state_old.reg_tac & 0b111) == 4) SOGU_TIMA_CLKn_old = (state_old.reg_div >> 7) & 1;
    if ((state_old.reg_tac & 0b111) == 5) SOGU_TIMA_CLKn_old = (state_old.reg_div >> 1) & 1;
    if ((state_old.reg_tac & 0b111) == 6) SOGU_TIMA_CLKn_old = (state_old.reg_div >> 3) & 1;
    if ((state_old.reg_tac & 0b111) == 7) SOGU_TIMA_CLKn_old = (state_old.reg_div >> 5) & 1;

    if ((state_new.reg_tac & 0b111) == 4) SOGU_TIMA_CLKn_new = (state_new.reg_div >> 7) & 1;
    if ((state_new.reg_tac & 0b111) == 5) SOGU_TIMA_CLKn_new = (state_new.reg_div >> 1) & 1;
    if ((state_new.reg_tac & 0b111) == 6) SOGU_TIMA_CLKn_new = (state_new.reg_div >> 3) & 1;
    if ((state_new.reg_tac & 0b111) == 7) SOGU_TIMA_CLKn_new = (state_new.reg_div >> 5) & 1;

    if (SOGU_TIMA_CLKn_old && !SOGU_TIMA_CLKn_new) {
      state_new.reg_tima++;
    }

    if (DELTA_HA_new) {
      state_new.int_ctrl.MOBA_TIMER_OVERFLOWp.state = !get_bit(state_old.reg_tima, 7) && state_old.int_ctrl.NYDU_TIMA7p_DELAY.state;
      state_new.int_ctrl.NYDU_TIMA7p_DELAY.state = get_bit(state_old.reg_tima, 7);
    }

    if ((DELTA_DE_new || DELTA_EF_new || DELTA_FG_new) && cpu_wr && !cpu_rd && cpu_addr_new == 0xFF05) {
      state_new.int_ctrl.NYDU_TIMA7p_DELAY.state = 0;
      state_new.reg_tima = cpu_dbus_new; // must be new
    }
    else if (state_new.int_ctrl.MOBA_TIMER_OVERFLOWp.state) {
      state_new.int_ctrl.NYDU_TIMA7p_DELAY.state = 0;
      state_new.reg_tima = state_new.reg_tma;
    }
  }

  //----------------------------------------

  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF00) {
    set_bit(state_new.reg_joy, 0, get_bit(state_old.cpu_dbus, 4));
    set_bit(state_new.reg_joy, 1, get_bit(state_old.cpu_dbus, 5));
  }
  // LCDC write has to be near the top as it controls the video reset signal
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF40) state_new.reg_lcdc = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF45) state_new.reg_lyc  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF42) state_new.reg_scy  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF43) state_new.reg_scx  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF47) state_new.reg_bgp  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF48) state_new.reg_obp0 = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF49) state_new.reg_obp1 = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF4A) state_new.reg_wy   = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF4B) state_new.reg_wx   = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF50) state_new.cpu_signals.TEPU_BOOT_BITn.state = get_bit(state_old.cpu_dbus, 0) || state_old.cpu_signals.TEPU_BOOT_BITn.state;

  if (cpu_wr && DELTA_GH_new && (cpu_addr_new >= 0xFF80) && (cpu_addr_new <= 0xFFFE)) mem.zero_ram[cpu_addr_new & 0x007F] = state_old.cpu_dbus;

  wire req_addr_boot = (cpu.bus_req_new.addr >= 0x0000) && (cpu.bus_req_new.addr <= 0x00FF) && !state_new.cpu_signals.TEPU_BOOT_BITn.state;

  wire ext_addr_new = (cpu.bus_req_new.read || cpu.bus_req_new.write) &&
                            (( DELTA_HA_new && !req_addr_hi && !req_addr_boot && !req_addr_vram) ||
                            (!DELTA_HA_new && !req_addr_hi && !req_addr_boot));

  bool cpu_addr_bootrom_new = req_addr_lo && !state_new.cpu_signals.TEPU_BOOT_BITn.state;

  wire vid_rst_old  =  get_bit(state_old.reg_lcdc, 7);

  wire bgw_en_new   = !get_bit(state_new.reg_lcdc, 0);
  wire spr_en_new   = !get_bit(state_new.reg_lcdc, 1);
  wire spr_size_new =  get_bit(state_new.reg_lcdc, 2);
  wire bg_map_new   =  get_bit(state_new.reg_lcdc, 3);
  wire bgw_tile_new =  get_bit(state_new.reg_lcdc, 4);
  wire win_en_new   = !get_bit(state_new.reg_lcdc, 5);
  wire win_map_new  =  get_bit(state_new.reg_lcdc, 6);
  wire vid_rst_new  =  get_bit(state_new.reg_lcdc, 7);

  //----------------------------------------
  // LX, LY, lcd flags

  wire ATEJ_LINE_RSTp_odd_old = !((state_old.lcd.ANEL_x113p_odd.state || !state_old.lcd.CATU_x113p_odd.state) && vid_rst_old);

  if (vid_rst_new) {
    state_new.lcd.RUTU_LINE_ENDp_odd.state = 0;
    state_new.lcd.NYPE_LINE_ENDp_odd.state = 0;
    state_new.lcd.POPU_VBLANKp_odd.state = 0;
    state_new.lcd.MYTA_FRAME_ENDp_odd.state = 0;
    state_new.reg_ly = 0;
    state_new.reg_lx = 0;
    state_new.lcd.CATU_x113p_odd.state = 0;
    state_new.lcd.ANEL_x113p_odd.state = 0;
  }
  else if (DELTA_ODD_new) {
    if (DELTA_HA_new) {
      state_new.lcd.CATU_x113p_odd.state = state_old.lcd.RUTU_LINE_ENDp_odd.state && (state_old.reg_ly < 144);
    }

    if (DELTA_BC_new) {
      if (!state_old.lcd.RUTU_LINE_ENDp_odd.state) state_new.reg_lx++;


      if (!state_old.lcd.NYPE_LINE_ENDp_odd.state && state_old.lcd.RUTU_LINE_ENDp_odd.state) {
        state_new.lcd.POPU_VBLANKp_odd.state = state_old.reg_ly >= 144;
        state_new.lcd.MYTA_FRAME_ENDp_odd.state = state_old.reg_ly >= 153;
        if (state_old.reg_ly >= 153) state_new.reg_ly = 0;
      }

      state_new.lcd.NYPE_LINE_ENDp_odd.state = state_old.lcd.RUTU_LINE_ENDp_odd.state;
      state_new.lcd.ANEL_x113p_odd.state = state_old.lcd.CATU_x113p_odd.state;
    }

    if (DELTA_DE_new) {
      state_new.lcd.CATU_x113p_odd.state = state_old.lcd.RUTU_LINE_ENDp_odd.state && (state_old.reg_ly < 144);
    }

    if (DELTA_FG_new) {
      if (state_old.reg_lx == 113) {
        state_new.reg_lx = 0;
        state_new.reg_ly++;
        state_new.lcd.RUTU_LINE_ENDp_odd.state = 1;
      }
      else {
        state_new.lcd.RUTU_LINE_ENDp_odd.state = 0;
      }

      state_new.lcd.ANEL_x113p_odd.state = state_old.lcd.CATU_x113p_odd.state;
      if (state_new.lcd.MYTA_FRAME_ENDp_odd.state) state_new.reg_ly = 0;
    }

  }

  wire line_rst_new = state_new.lcd.CATU_x113p_odd.state && !state_new.lcd.ANEL_x113p_odd.state;

  //----------------------------------------
  // Joypad

  // FIXME what if both scan bits are set?

  if (DELTA_HA_new) {
    uint8_t button_mask = 0b00000000;
    if (!get_bit(state_new.reg_joy, 0)) button_mask |= 0b00001111;
    if (!get_bit(state_new.reg_joy, 1)) button_mask |= 0b11110000;

    state_new.int_ctrl.AWOB_WAKE_CPU.state  = !(sys.buttons & button_mask);
    state_new.int_ctrl.SIG_CPU_WAKE.state   = !(sys.buttons & button_mask);
    state_new.joy_int.APUG_JP_GLITCH3.state = state_new.joy_int.AGEM_JP_GLITCH2.state;
    state_new.joy_int.AGEM_JP_GLITCH2.state = state_new.joy_int.ACEF_JP_GLITCH1.state;
    state_new.joy_int.ACEF_JP_GLITCH1.state = state_new.joy_int.BATU_JP_GLITCH0.state;
    state_new.joy_int.BATU_JP_GLITCH0.state = !(sys.buttons & button_mask);
  }
  else if (!cpu_rd) {
    uint8_t button_mask = 0b00000000;
    if (!get_bit(state_new.reg_joy, 0)) button_mask |= 0b00001111;
    if (!get_bit(state_new.reg_joy, 1)) button_mask |= 0b11110000;
    state_new.joy_latch = (((sys.buttons & button_mask) >> 0) | ((sys.buttons & button_mask) >> 4)) & 0b1111;
  }

  //----------------------------------------
  //tock_serial_logic();
  //tock_timer_logic();

  //----------------------------------------
  // bootrom read

  if (cpu_addr_new <= 0x00FF) {
    if (cpu_rd && !state_new.cpu_signals.TEPU_BOOT_BITn.state) {
      state_new.cpu_dbus = DMG_ROM_blob[cpu_addr_new & 0xFF];
    }
  }

  // boot bit read

  if (cpu_rd && cpu_addr_new == 0xFF50) {
    state_new.cpu_dbus &= ~1;
    state_new.cpu_dbus |= state_new.cpu_signals.TEPU_BOOT_BITn.state;
  }

  //----------------------------------------
  // DMA
  // this block is all sealed up
  // +dma_ctrl +reg_dma +dma_lo

  auto& MATU_DMA_RUNNINGp_old = state_old.MATU_DMA_RUNNINGp;

  {
    auto& ctrl = state_new.dma_ctrl;
    auto& dma_lo = state_new.dma_lo;
    auto& dma_hi = state_new.reg_dma;

    if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF46) dma_hi  = ~state_old.cpu_dbus;
    if (cpu_rd && cpu_addr_new == 0xFF46) state_new.cpu_dbus = ~dma_hi;

    if (DELTA_HA_new) {
      ctrl.LUVY_DMA_TRIG_d0_odd.state = ctrl.LYXE_DMA_LATCHp.state;
      if (ctrl.LOKY_DMA_LATCHp_odd.state && !ctrl.LENE_DMA_TRIG_d4_odd.state) dma_lo++;
      state_new.MATU_DMA_RUNNINGp = ctrl.LOKY_DMA_LATCHp_odd.state;
    }
    else if (DELTA_DE_new) {
      if (cpu_wr && cpu_addr_new == 0xFF46) {
        ctrl.LYXE_DMA_LATCHp.state = 1;
      }

      if (state_old.dma_lo == 159) {
        ctrl.MYTE_DMA_DONE_odd.state = 1;
        ctrl.LARA_DMA_LATCHn_odd.state = 1;
        ctrl.LOKY_DMA_LATCHp_odd.state = 0;
      }

      ctrl.LENE_DMA_TRIG_d4_odd.state = state_old.dma_ctrl.LUVY_DMA_TRIG_d0_odd.state;

      if (ctrl.LUVY_DMA_TRIG_d0_odd.state) {
        ctrl.MYTE_DMA_DONE_odd.state = 0;
        ctrl.LYXE_DMA_LATCHp.state = 0;
        ctrl.LARA_DMA_LATCHn_odd.state = 0;
        ctrl.LOKY_DMA_LATCHp_odd.state = 1;
        dma_lo = 0;
      }
    }
  }

  auto& MATU_DMA_RUNNINGp_new = state_new.MATU_DMA_RUNNINGp;

  const auto dma_addr_new = ((state_new.reg_dma ^ 0xFF) << 8) | state_new.dma_lo;
  wire dma_addr_vram_new = (dma_addr_new >= 0x8000) && (dma_addr_new <= 0x9FFF);

  //----------------------------------------




























  wire TEVO_WIN_FETCH_TRIGp_old =
    (state_old.win_ctrl.RYFA_WIN_FETCHn_A_evn.state && !state_old.win_ctrl.RENE_WIN_FETCHn_B_evn.state) ||
    (!state_old.win_ctrl.RYDY_WIN_HITp_odd.state && state_old.win_ctrl.SOVY_WIN_HITp_evn.state) ||
    (!state_old.XYMU_RENDERINGn && !state_old.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && state_old.tfetch_control.NYKA_FETCH_DONEp_evn.state && state_old.tfetch_control.PORY_FETCH_DONEp_odd.state);

  wire AVAP_SCAN_DONE_tp_odd_old    = (!ATEJ_LINE_RSTp_odd_old && !vid_rst_old && state_old.sprite_scanner.BYBA_SCAN_DONEp_odd.state && !state_old.sprite_scanner.DOBA_SCAN_DONEp_evn.state);
  wire NYXU_BFETCH_RSTn_old         = !(AVAP_SCAN_DONE_tp_odd_old || (state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state && !state_old.win_ctrl.NOPA_WIN_MODE_Bp_evn.state) || TEVO_WIN_FETCH_TRIGp_old);

  //----------------------------------------
  // Sprite scanner

  if (vid_rst_new || line_rst_new) {
    state_new.scan_counter = 0;
    state_new.sprite_counter = 0;
    state_new.sprite_store_flags = 0;

    state_new.sprite_scanner.DOBA_SCAN_DONEp_evn.state = 0;
    state_new.sprite_scanner.BYBA_SCAN_DONEp_odd.state = 0;

    if (line_rst_new) state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state = 1;
    if (vid_rst_new)  state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state = 0;

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

    if (vid_rst_new) {
      state_new.sprite_scanner.CENO_SCAN_DONEn_odd.state = 0;
      state_new.sprite_scanner.DEZY_INC_COUNTn_odd.state = 0;
    }
  }
  else {
    int ly = (int)state_new.reg_ly;
    int sy = (int)state_new.oam_temp_a - 16;
    int sprite_height = spr_size_new ? 8 : 16;
    wire sprite_hit = ly >= sy && ly < sy + sprite_height && state_new.sprite_scanner.CENO_SCAN_DONEn_odd.state;

    if (DELTA_HA_new || DELTA_DE_new) {
      state_new.sprite_scanner.CENO_SCAN_DONEn_odd.state = state_old.sprite_scanner.BESU_SCAN_DONEn_odd.state;
      if (state_new.scan_counter == 39) {
        state_new.sprite_scanner.BYBA_SCAN_DONEp_odd.state = 1;
        state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state = 0;
      }
      else {
        state_new.scan_counter++;
      }
      if (!state_new.sprite_scanner.DEZY_INC_COUNTn_odd.state && state_new.sprite_counter < 10) state_new.sprite_counter++;
      state_new.sprite_scanner.DEZY_INC_COUNTn_odd.state = 1;
    }
    if (DELTA_AB_new || DELTA_EF_new) {
      state_new.sprite_scanner.DOBA_SCAN_DONEp_evn.state = state_old.sprite_scanner.BYBA_SCAN_DONEp_odd.state;

      if (sprite_hit) {
        state_new.sprite_store_flags = (1 << state_new.sprite_counter);
        (&state_new.store_i0)[state_new.sprite_counter] = state_new.sprite_ibus ^ 0b111111;
        (&state_new.store_l0)[state_new.sprite_counter] = state_new.sprite_lbus ^ 0b1111;
      }
      else {
        state_new.sprite_store_flags = 0;
      }
    }
    if (DELTA_BC_new || DELTA_FG_new) {
      if (sprite_hit) {
        state_new.sprite_scanner.DEZY_INC_COUNTn_odd.state = 0;
      }
    }
    if (DELTA_CD_new || DELTA_GH_new) {
      state_new.sprite_scanner.DOBA_SCAN_DONEp_evn.state = state_old.sprite_scanner.BYBA_SCAN_DONEp_odd.state;
      if (state_new.sprite_store_flags) {
        (&state_new.store_x0)[state_new.sprite_counter] = state_new.oam_temp_b;
      }
      state_new.sprite_store_flags = 0;
    }
  }

  // FETO
  state_new.sprite_scanner.FETO_SCAN_DONEp.state = state_new.scan_counter == 39;

  // BYBA
  if (DELTA_HA_new || DELTA_DE_new) state_new.sprite_scanner.BYBA_SCAN_DONEp_odd.state = state_old.sprite_scanner.FETO_SCAN_DONEp.state;
  if (line_rst_new || vid_rst_new) state_new.sprite_scanner.BYBA_SCAN_DONEp_odd.state = 0;

  wire scan_done_trig_new = state_new.sprite_scanner.BYBA_SCAN_DONEp_odd.state && !state_new.sprite_scanner.DOBA_SCAN_DONEp_evn.state;

  //----------------------------------------

  if (vid_rst_new) {
    state_new.VOGA_HBLANKp = 0;
    state_new.XYMU_RENDERINGn = 1;
  }
  else if (line_rst_new) {
    state_new.VOGA_HBLANKp = 0;
  }
  else {
    if (DELTA_EVEN_new) state_new.VOGA_HBLANKp = (!state_old.FEPO_STORE_MATCHp_odd && (state_old.pix_count == 167));

    if (state_new.VOGA_HBLANKp) state_new.XYMU_RENDERINGn = 1;
    if (scan_done_trig_new)     state_new.XYMU_RENDERINGn = 0;
  }

  //----------------------------------------
  // Sprite fetch state counter

  // TYFO
  if (DELTA_ODD_new) state_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state = get_bit(state_old.sfetch_counter_evn, 0);

  // TOBU
  if (DELTA_EVEN_new) state_new.sfetch_control.TOBU_SFETCH_S1p_D2_evn.state = get_bit(state_old.sfetch_counter_evn, 1);
  if (state_new.XYMU_RENDERINGn) state_new.sfetch_control.TOBU_SFETCH_S1p_D2_evn.state = 0;

  // VONU
  if (DELTA_EVEN_new) state_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.state = state_old.sfetch_control.TOBU_SFETCH_S1p_D2_evn.state;
  if (state_new.XYMU_RENDERINGn) state_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.state = 0;

  // SOBU
  if (DELTA_EVEN_new) state_new.sfetch_control.SOBU_SFETCH_REQp_evn.state = state_old.FEPO_STORE_MATCHp_odd && !state_old.win_ctrl.RYDY_WIN_HITp_odd.state && state_old.tfetch_control.LYRY_BFETCH_DONEp_odd.state && !state_old.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.state;

  // SUDA
  if (DELTA_ODD_new) state_new.sfetch_control.SUDA_SFETCH_REQp_odd.state = state_old.sfetch_control.SOBU_SFETCH_REQp_evn.state;

  // sfetch_counter_evn
  wire TOMA_SFETCH_evn_old = DELTA_EVEN_old || (get_bit(state_old.sfetch_counter_evn, 2) && get_bit(state_old.sfetch_counter_evn, 0));
  wire TOMA_SFETCH_evn_new = DELTA_EVEN_new || (get_bit(state_new.sfetch_counter_evn, 2) && get_bit(state_new.sfetch_counter_evn, 0));
  if (!TOMA_SFETCH_evn_old && TOMA_SFETCH_evn_new) state_new.sfetch_counter_evn++;
  if ((state_new.sfetch_control.SOBU_SFETCH_REQp_evn.state && !state_new.sfetch_control.SUDA_SFETCH_REQp_odd.state) || vid_rst_new || line_rst_new) state_new.sfetch_counter_evn = 0;

  // SEBA
  if (DELTA_ODD_new)             state_new.sfetch_control.SEBA_SFETCH_S1p_D5_odd.state = state_old.sfetch_control.VONU_SFETCH_S1p_D4_evn.state;
  if (state_new.XYMU_RENDERINGn) state_new.sfetch_control.SEBA_SFETCH_S1p_D5_odd.state = 0;

  // WUTY
  state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp.state = state_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state && get_bit(state_new.sfetch_counter_evn, 0) && state_new.sfetch_control.SEBA_SFETCH_S1p_D5_odd.state && state_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.state;

  // PYGO
  if (DELTA_EVEN_new) state_new.tfetch_control.PYGO_FETCH_DONEp_evn.state = state_old.tfetch_control.PORY_FETCH_DONEp_odd.state;
  if (state_new.XYMU_RENDERINGn) state_new.tfetch_control.PYGO_FETCH_DONEp_evn.state = 0;

  // POKY
  if (state_new.tfetch_control.PYGO_FETCH_DONEp_evn.state) state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state = 1;
  if (state_new.XYMU_RENDERINGn) state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state = 0;

  // NUNU
  if (DELTA_ODD_new)   state_new.win_ctrl.NUNU_WIN_MATCHp_odd.state = state_old.win_ctrl.PYCO_WIN_MATCHp_evn.state;
  if (vid_rst_new) state_new.win_ctrl.NUNU_WIN_MATCHp_odd.state = 0;

  // PYNU
  if (state_new.win_ctrl.NUNU_WIN_MATCHp_odd.state)         state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state = 1;
  if (line_rst_new || vid_rst_new || !win_en_new) state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state = 0;

  // NOPA
  if (DELTA_EVEN_new)  state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state = state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state;
  if (vid_rst_new) state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state = 0;

  wire NUNY_WIN_MODE_TRIGp_new = state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state && !state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state;

  // NYKA
  if (DELTA_EVEN_new) state_new.tfetch_control.NYKA_FETCH_DONEp_evn.state = state_old.tfetch_control.LYRY_BFETCH_DONEp_odd.state;
  if (NUNY_WIN_MODE_TRIGp_new || state_new.XYMU_RENDERINGn) state_new.tfetch_control.NYKA_FETCH_DONEp_evn.state = 0;

  // PORY
  if (DELTA_ODD_new) state_new.tfetch_control.PORY_FETCH_DONEp_odd.state = state_old.tfetch_control.NYKA_FETCH_DONEp_evn.state;
  if (NUNY_WIN_MODE_TRIGp_new || state_new.XYMU_RENDERINGn) state_new.tfetch_control.PORY_FETCH_DONEp_odd.state = 0;

  // TAKA
  if ((state_new.sfetch_control.SOBU_SFETCH_REQp_evn.state && !state_new.sfetch_control.SUDA_SFETCH_REQp_odd.state) || vid_rst_new || line_rst_new) state_new.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.state = 1;
  if (state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp.state || (!state_new.XYMU_RENDERINGn && !state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && state_new.tfetch_control.NYKA_FETCH_DONEp_evn.state && state_new.tfetch_control.PORY_FETCH_DONEp_odd.state))                                                         state_new.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.state = 0;

  // RYDY
  // PUKY
  if (vid_rst_new) {
    state_new.win_ctrl.RYDY_WIN_HITp_odd.state = 0;
    state_new.win_ctrl.PUKU_WIN_HITn_odd.state = 1;
  }
  else if (state_new.tfetch_control.PORY_FETCH_DONEp_odd.state) {
    state_new.win_ctrl.RYDY_WIN_HITp_odd.state = 0;
    state_new.win_ctrl.PUKU_WIN_HITn_odd.state = !NUNY_WIN_MODE_TRIGp_new;
  }
  else if (NUNY_WIN_MODE_TRIGp_new) {
    state_new.win_ctrl.RYDY_WIN_HITp_odd.state = 1;
    state_new.win_ctrl.PUKU_WIN_HITn_odd.state = 0;
  }

  // SOVY
  if (DELTA_EVEN_new) state_new.win_ctrl.SOVY_WIN_HITp_evn.state = state_old.win_ctrl.RYDY_WIN_HITp_odd.state;
  if (vid_rst_new) state_new.win_ctrl.SOVY_WIN_HITp_evn.state = 0;

  // CLKPIPE is an even clock, it can only go high on even deltas. FEPO/WODU/SOCY are odd signals, they stay constant during even deltas.
  // SO, it is guaranteed safe to use the old values of FEPO/WODU/SOCY to compute CLKPIPE

  wire TYFA_CLKPIPE_evn_old = !state_old.win_ctrl.RYDY_WIN_HITp_odd.state && state_old.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && (!state_old.FEPO_STORE_MATCHp_odd && !(!state_old.FEPO_STORE_MATCHp_odd && (state_old.pix_count == 167)) && !(DELTA_EVEN_new));
  wire TYFA_CLKPIPE_evn_new = !state_new.win_ctrl.RYDY_WIN_HITp_odd.state && state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && (!state_old.FEPO_STORE_MATCHp_odd && !(!state_old.FEPO_STORE_MATCHp_odd && (state_old.pix_count == 167)) && !(DELTA_ODD_new));

  // RYVA
  if (TYFA_CLKPIPE_evn_old && !TYFA_CLKPIPE_evn_new) state_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.state = !nuko_wx_match_old && state_old.fine_count_odd == 7;
  if (state_new.XYMU_RENDERINGn) state_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.state = 0;

  // RENE
  if (DELTA_EVEN_new) state_new.win_ctrl.RENE_WIN_FETCHn_B_evn.state   = state_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.state;
  if (state_new.XYMU_RENDERINGn) state_new.win_ctrl.RENE_WIN_FETCHn_B_evn.state = 0;

  wire TEVO_WIN_FETCH_TRIGp_new =
    (state_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.state && !state_new.win_ctrl.RENE_WIN_FETCHn_B_evn.state) ||
    (state_new.win_ctrl.SOVY_WIN_HITp_evn.state && !state_new.win_ctrl.RYDY_WIN_HITp_odd.state) ||
    (!state_new.XYMU_RENDERINGn && !state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && state_new.tfetch_control.NYKA_FETCH_DONEp_evn.state && state_new.tfetch_control.PORY_FETCH_DONEp_odd.state);

  wire NYXU_BFETCH_RSTn_new = (line_rst_new || vid_rst_new || !scan_done_trig_new) && !NUNY_WIN_MODE_TRIGp_new && !TEVO_WIN_FETCH_TRIGp_new;







  // tfetch counter
  {
    wire LEBO_ODD_old = !((DELTA_ODD_new)  && !(get_bit(state_old.tfetch_counter, 0) && get_bit(state_old.tfetch_counter, 2) && NYXU_BFETCH_RSTn_old));
    wire LEBO_ODD_new = !((DELTA_EVEN_new) && !(get_bit(state_new.tfetch_counter, 0) && get_bit(state_new.tfetch_counter, 2) && NYXU_BFETCH_RSTn_new));

    if (!LEBO_ODD_old && LEBO_ODD_new) state_new.tfetch_counter++;
    if (!NYXU_BFETCH_RSTn_new) state_new.tfetch_counter = 0;
  }
    


  // LYRY
  state_new.tfetch_control.LYRY_BFETCH_DONEp_odd.state = NYXU_BFETCH_RSTn_new && get_bit(state_new.tfetch_counter, 0) && get_bit(state_new.tfetch_counter, 2);

  // LOVY
  if (DELTA_ODD_new) state_new.tfetch_control.LOVY_FETCH_DONEp.state = state_old.tfetch_control.LYRY_BFETCH_DONEp_odd.state;
  if (!NYXU_BFETCH_RSTn_new) state_new.tfetch_control.LOVY_FETCH_DONEp.state = 0;

  // LONY
  if (!NYXU_BFETCH_RSTn_new) state_new.tfetch_control.LONY_FETCHINGp.state = 1;
  if (state_new.tfetch_control.LOVY_FETCH_DONEp.state || state_new.XYMU_RENDERINGn) state_new.tfetch_control.LONY_FETCHINGp.state = 0;

  // sprite_reset_flags <- old.sprite_match_flags
  if (!state_old.sfetch_control.WUTY_SFETCH_DONE_TRIGp.state && state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp.state) state_new.sprite_reset_flags = state_old.sprite_match_flags;
  if (vid_rst_new || (line_rst_new || vid_rst_new)) state_new.sprite_reset_flags = 0;

  // sprite reset
  int sprite_reset_index = 32 - __lzcnt(state_new.sprite_reset_flags - 1);
  if (sprite_reset_index != 32) (&state_new.store_x0)[sprite_reset_index] = 0xFF;





  const uint8_t sfetch_phase_old = pack(
    !(state_old.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state ^ get_bit(state_old.sfetch_counter_evn, 0)),
    get_bit(state_old.sfetch_counter_evn, 0),
    get_bit(state_old.sfetch_counter_evn, 1),
    get_bit(state_old.sfetch_counter_evn, 2));

  const uint8_t sfetch_phase_new = pack(
    !(state_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state ^ get_bit(state_new.sfetch_counter_evn, 0)),
    get_bit(state_new.sfetch_counter_evn, 0),
    get_bit(state_new.sfetch_counter_evn, 1),
    get_bit(state_new.sfetch_counter_evn, 2));


  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  {
    
    bool BYCU_OAM_CLKp_old = false;

    auto b0 = get_bit(state_old.sfetch_counter_evn, 0);
    auto b1 = get_bit(state_old.sfetch_counter_evn, 1);
    auto b2 = get_bit(state_old.sfetch_counter_evn, 2);
    auto b1d = state_old.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state;

    auto clk = state_old.sprite_scanner.BESU_SCAN_DONEn_odd.state && !vid_rst_old && gen_clk(phase_old, 0b01110111);
    auto bphase = b1 || b2 || (b1d && !b0);
    auto delta = DELTA_DE_old || DELTA_EF_old || DELTA_FG_old || DELTA_GH_old;

    if (MATU_DMA_RUNNINGp_old) {
      if (state_old.XYMU_RENDERINGn) BYCU_OAM_CLKp_old = delta;
      else BYCU_OAM_CLKp_old = bphase && !delta;

    }
    else if (!state_old.XYMU_RENDERINGn) {
      BYCU_OAM_CLKp_old = clk || !bphase || cpu_addr_oam_old;   // this "cpu_addr_oam_old" seems like a bug
    }
    else {
      BYCU_OAM_CLKp_old = clk || (cpu_addr_oam_old && delta);
    }

  


    wire XYSO_xBCDxFGH_new = (!vid_rst_new && gen_clk(phase_new, 0b01110111));
    wire CUFE_OAM_CLKp_new       = !((cpu_addr_oam_new || MATU_DMA_RUNNINGp_new) && (DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new));
    wire acyl_new                = (!MATU_DMA_RUNNINGp_new && state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state && !vid_rst_new);
    wire AVER_AxxxExxx_new       = !(acyl_new && XYSO_xBCDxFGH_new);
    wire TYTU_SFETCH_S0n_new     = !get_bit(state_new.sfetch_counter_evn, 0);
    wire TACU_SPR_SEQ_5_TRIG_new = !(state_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state && TYTU_SFETCH_S0n_new);
    wire TUVO_PPU_OAM_RDp_new    = !(state_new.XYMU_RENDERINGn || get_bit(state_new.sfetch_counter_evn, 1) || get_bit(state_new.sfetch_counter_evn, 2));
    wire VAPE_OAM_CLKENn_new     = (TUVO_PPU_OAM_RDp_new && TACU_SPR_SEQ_5_TRIG_new);
    wire XUJY_OAM_CLKENp_new     = !VAPE_OAM_CLKENn_new;
    wire BYCU_OAM_CLKp_new       = !(AVER_AxxxExxx_new && XUJY_OAM_CLKENp_new &&  CUFE_OAM_CLKp_new);

    if (bit(BYCU_OAM_CLKp_old) && !bit(BYCU_OAM_CLKp_new)) {
      state_new.oam_temp_a = ~state_new.oam_latch_a;
      state_new.oam_temp_b = ~state_new.oam_latch_b;
    }
  }

  auto& oam_temp_a_new = state_new.oam_temp_a;
  auto& oam_temp_b_new = state_new.oam_temp_b;









  if (!vid_rst_new) {
    if (DELTA_HA_new || DELTA_DE_new) {
      state_new.sprite_index = (state_new.oam_abus >> 2) ^ 0b111111;
    }
  }

  if (state_new.XYMU_RENDERINGn) {
    state_new.sprite_ibus = state_new.sprite_index;
    state_new.sprite_lbus = (~state_new.reg_ly + oam_temp_a_new) & 0b00001111;
  }

  else if (!state_new.XYMU_RENDERINGn && state_new.sprite_scanner.CENO_SCAN_DONEn_odd.state) {
    state_new.sprite_ibus = state_new.sprite_index;
    state_new.sprite_lbus = 0b00001111;
  }

  if (!TYFA_CLKPIPE_evn_old && TYFA_CLKPIPE_evn_new) state_new.win_ctrl.PYCO_WIN_MATCHp_evn.state = nuko_wx_match_old;
  if (vid_rst_new) state_new.win_ctrl.PYCO_WIN_MATCHp_evn.state = 0;



  // ROGE
  state_new.win_ctrl.ROGE_WY_MATCHp_odd.state = (state_new.reg_ly == uint8_t(~state_new.reg_wy)) && win_en_new;

  // SARY
  if (DELTA_BC_new) state_new.win_ctrl.SARY_WY_MATCHp_odd.state = state_old.win_ctrl.ROGE_WY_MATCHp_odd.state;
  if (vid_rst_new) state_new.win_ctrl.SARY_WY_MATCHp_odd.state = 0;

  // REJO
  if (state_new.win_ctrl.SARY_WY_MATCHp_odd.state) state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state = 1;
  if (state_new.lcd.POPU_VBLANKp_odd.state) state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state = 0;
  if (vid_rst_new) state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state = 0;


  wire win_fetch_trig_new = state_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.state && !state_new.win_ctrl.RENE_WIN_FETCHn_B_evn.state;
  wire win_hit_trig_new   = state_new.win_ctrl.SOVY_WIN_HITp_evn.state && !state_new.win_ctrl.RYDY_WIN_HITp_odd.state;

  //----------------------------------------
  // Pixel counter

  wire PECU_FINE_CLK_odd_old = !(TYFA_CLKPIPE_evn_old && (state_old.fine_count_odd != 7));
  wire PECU_FINE_CLK_odd_new = !(TYFA_CLKPIPE_evn_new && (state_new.fine_count_odd != 7));
  if (!PECU_FINE_CLK_odd_old && PECU_FINE_CLK_odd_new) {
    state_new.fine_count_odd++;
  }
  if (state_new.XYMU_RENDERINGn || TEVO_WIN_FETCH_TRIGp_new) state_new.fine_count_odd = 0;
  auto& fine_count_odd_new = state_new.fine_count_odd;


  if (!TYFA_CLKPIPE_evn_old && TYFA_CLKPIPE_evn_new) {
    state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state = state_old.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state && (((state_old.reg_scx & 0b111) ^ 0b111) == state_old.fine_count_odd);
  }

  if (DELTA_ODD_new) {
    state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state = state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state;
  }

  if (state_new.XYMU_RENDERINGn) {
    state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state = 1;
    state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state = 0;
    state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state = 0;
  }

  if (state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state && !state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state) {
    state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state = 0;
  }


  // pix count
  auto& pix_count_new = state_new.pix_count;
  if (vid_rst_new || (line_rst_new || vid_rst_new)) {
    pix_count_new = 0;
  }
  else {
    wire SACU_CLKPIPE_odd_old = !TYFA_CLKPIPE_evn_old || state_old.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state;
    wire SACU_CLKPIPE_odd_new = !TYFA_CLKPIPE_evn_new || state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state;
    if (!bit(SACU_CLKPIPE_odd_old) && bit(SACU_CLKPIPE_odd_new)) {
      pix_count_new++;
    }
  }

  state_new.win_ctrl.NUKO_WX_MATCHp_odd.state = (uint8_t(~state_new.reg_wx) == state_new.pix_count) && state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state;

  //----------------------------------------

  // FEPO

  if (!state_new.XYMU_RENDERINGn && !state_new.sprite_scanner.CENO_SCAN_DONEn_odd.state) {
    auto& s = state_new;
    auto& sf = s.sprite_match_flags;
    auto& si = s.sprite_ibus;
    auto& sl = s.sprite_lbus;

    sf = 0;
    si = 0x3F;
    sl = 0x0F;

    if (spr_en_new) {
      state_new.FEPO_STORE_MATCHp_odd = 0;
      if      (pix_count_new == s.store_x0) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x001;  si = s.store_i0 ^ 0x3F; sl = s.store_l0 ^ 0x0F; }
      else if (pix_count_new == s.store_x1) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x002;  si = s.store_i1 ^ 0x3F; sl = s.store_l1 ^ 0x0F; }
      else if (pix_count_new == s.store_x2) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x004;  si = s.store_i2 ^ 0x3F; sl = s.store_l2 ^ 0x0F; }
      else if (pix_count_new == s.store_x3) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x008;  si = s.store_i3 ^ 0x3F; sl = s.store_l3 ^ 0x0F; }
      else if (pix_count_new == s.store_x4) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x010;  si = s.store_i4 ^ 0x3F; sl = s.store_l4 ^ 0x0F; }
      else if (pix_count_new == s.store_x5) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x020;  si = s.store_i5 ^ 0x3F; sl = s.store_l5 ^ 0x0F; }
      else if (pix_count_new == s.store_x6) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x040;  si = s.store_i6 ^ 0x3F; sl = s.store_l6 ^ 0x0F; }
      else if (pix_count_new == s.store_x7) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x080;  si = s.store_i7 ^ 0x3F; sl = s.store_l7 ^ 0x0F; }
      else if (pix_count_new == s.store_x8) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x100;  si = s.store_i8 ^ 0x3F; sl = s.store_l8 ^ 0x0F; }
      else if (pix_count_new == s.store_x9) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x200;  si = s.store_i9 ^ 0x3F; sl = s.store_l9 ^ 0x0F; }
    }
  }

  if (state_new.XYMU_RENDERINGn) state_new.sprite_match_flags = 0;

  if (!state_new.XYMU_RENDERINGn && !state_new.FEPO_STORE_MATCHp_odd) {
    const auto pack_ydiff = ~state_new.reg_ly + oam_temp_a_new;
    state_new.sprite_lbus = pack_ydiff & 0b00001111;
  }

  //----------------------------------------

  if (!TYFA_CLKPIPE_evn_old && TYFA_CLKPIPE_evn_new) {
    state_new.lcd.PAHO_X8_SYNC.state = get_bit(state_old.pix_count, 3);
  }

  if (state_new.XYMU_RENDERINGn) {
    state_new.lcd.PAHO_X8_SYNC.state = 0;
  }

  //----------------------------------------
  // Vram to tile temp

  // LYZU
  if (DELTA_EVEN_new) state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state = get_bit(state_old.tfetch_counter, 0);
  if (state_new.XYMU_RENDERINGn) state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state = 0;


  {
    wire LAXE_BFETCH_S0n_old = !get_bit(state_old.tfetch_counter, 0);
    wire NAKO_BFETCH_S1n_old = !get_bit(state_old.tfetch_counter, 1);
    wire NOFU_BFETCH_S2n_old = !get_bit(state_old.tfetch_counter, 2);

    wire MYSO_STORE_VRAM_DATA_TRIGp_old = !(state_old.XYMU_RENDERINGn || LAXE_BFETCH_S0n_old || state_old.tfetch_control.LYZU_BFETCH_S0p_D1.state);
    wire NYDY_LATCH_TILE_DAn_old = !(MYSO_STORE_VRAM_DATA_TRIGp_old && get_bit(state_old.tfetch_counter, 1) && NOFU_BFETCH_S2n_old);
    wire METE_LATCH_TILE_DAp_old = !NYDY_LATCH_TILE_DAn_old;
    wire LOMA_LATCH_TILE_DAn_old = !METE_LATCH_TILE_DAp_old;
    wire MOFU_LATCH_TILE_DBp_old = (MYSO_STORE_VRAM_DATA_TRIGp_old && NAKO_BFETCH_S1n_old);
    wire LESO_LATCH_TILE_DBn_old = !MOFU_LATCH_TILE_DBp_old;
    wire LUVE_LATCH_TILE_DBp_old = !LESO_LATCH_TILE_DBn_old;
    wire LABU_LATCH_TILE_DBn_old = !LUVE_LATCH_TILE_DBp_old;

    wire LAXE_BFETCH_S0n_new = !get_bit(state_new.tfetch_counter, 0);
    wire NAKO_BFETCH_S1n_new = !get_bit(state_new.tfetch_counter, 1);
    wire NOFU_BFETCH_S2n_new = !get_bit(state_new.tfetch_counter, 2);
    wire MYSO_STORE_VRAM_DATA_TRIGp_new = !(state_new.XYMU_RENDERINGn || LAXE_BFETCH_S0n_new || state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state);
    wire NYDY_LATCH_TILE_DAn_new = !(MYSO_STORE_VRAM_DATA_TRIGp_new && get_bit(state_new.tfetch_counter, 1) && NOFU_BFETCH_S2n_new);
    wire METE_LATCH_TILE_DAp_new = !NYDY_LATCH_TILE_DAn_new;
    wire LOMA_LATCH_TILE_DAn_new = !METE_LATCH_TILE_DAp_new;
    wire MOFU_LATCH_TILE_DBp_new = (MYSO_STORE_VRAM_DATA_TRIGp_new && NAKO_BFETCH_S1n_new);
    wire LESO_LATCH_TILE_DBn_new = !MOFU_LATCH_TILE_DBp_new;
    wire LUVE_LATCH_TILE_DBp_new = !LESO_LATCH_TILE_DBn_new;
    wire LABU_LATCH_TILE_DBn_new = !LUVE_LATCH_TILE_DBp_new;


    if (!LOMA_LATCH_TILE_DAn_old && LOMA_LATCH_TILE_DAn_new) {
      state_new.tile_temp_a = ~state_old.vram_dbus;
    }

    if (!LABU_LATCH_TILE_DBn_old && LABU_LATCH_TILE_DBn_new) {
      state_new.tile_temp_b = state_old.vram_dbus;
    }
  }






  //----------------------------------------
  // Win map x counter

  wire VETU_WIN_MAPp_old = TEVO_WIN_FETCH_TRIGp_old && state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state;
  wire VETU_WIN_MAPp_new = TEVO_WIN_FETCH_TRIGp_new && state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state;

  if (!VETU_WIN_MAPp_old && VETU_WIN_MAPp_new) {
    state_new.win_x.map++;
  }

  if (vid_rst_new || (line_rst_new || vid_rst_new) || !win_en_new) {
    state_new.win_x.map = 0;
  }

  //----------------------------------------
  // Win map y counter - increments when we _leave_ window mode.

  if (state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state && !state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state) {
    uint8_t win_y_old = state_old.win_y.tile | (state_old.win_y.map << 3);
    uint8_t win_y_new = win_y_old + 1;

    state_new.win_y.tile = win_y_new & 0b111;
    state_new.win_y.map  = win_y_new >> 3;
  }

  if (vid_rst_new || state_new.lcd.POPU_VBLANKp_odd.state) {
    state_new.win_y.tile = 0;
    state_new.win_y.map = 0;
  }

  //----------------------------------------
  // Pixel pipes

  if (DELTA_ODD_new) {
    wire SACU_CLKPIPE_odd_old = !TYFA_CLKPIPE_evn_old || state_old.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state;
    wire SACU_CLKPIPE_odd_new = !TYFA_CLKPIPE_evn_new || state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state;
    if (!SACU_CLKPIPE_odd_old && SACU_CLKPIPE_odd_new) {
      state_new.spr_pipe_a = (state_new.spr_pipe_a << 1) | 0;
      state_new.spr_pipe_b = (state_new.spr_pipe_b << 1) | 0;
      state_new.bgw_pipe_a = (state_new.bgw_pipe_a << 1) | 0;
      state_new.bgw_pipe_b = (state_new.bgw_pipe_b << 1) | 0;
      state_new.mask_pipe  = (state_new.mask_pipe  << 1) | 1;
      state_new.pal_pipe   = (state_new.pal_pipe   << 1) | 0;
    }
  }

  wire TEXY_SFETCHINGp_evn_new = (!vid_rst_new && !state_new.XYMU_RENDERINGn && ((get_bit(state_new.sfetch_counter_evn, 1) || state_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.state)));

  uint8_t sprite_pix = state_old.vram_dbus;
  if (!state_new.XYMU_RENDERINGn) {

    if (get_bit(state_old.oam_temp_b, 5) && TEXY_SFETCHINGp_evn_new) {
      sprite_pix = bit_reverse(state_old.vram_dbus);
    }

    if ((sfetch_phase_old == 5) && (sfetch_phase_new == 6)) {
      state_new.sprite_pix_a = ~sprite_pix;
    }

    if ((sfetch_phase_old == 9) && (sfetch_phase_new == 10)) {
      state_new.sprite_pix_b = ~sprite_pix;
    }
  }

  if (state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp.state) {
    uint8_t sprite_mask = state_new.spr_pipe_b | state_new.spr_pipe_a;
    state_new.spr_pipe_a = (state_new.spr_pipe_a & sprite_mask) | (~state_new.sprite_pix_a & ~sprite_mask);
    state_new.spr_pipe_b = (state_new.spr_pipe_b & sprite_mask) | (~state_new.sprite_pix_b & ~sprite_mask);
    state_new.mask_pipe  = get_bit(oam_temp_b_new, 7) ? state_new.mask_pipe | ~sprite_mask : state_new.mask_pipe & sprite_mask;
    state_new.pal_pipe   = get_bit(oam_temp_b_new, 4) ? state_new.pal_pipe  | ~sprite_mask : state_new.pal_pipe  & sprite_mask;
  }

  if (!NYXU_BFETCH_RSTn_new) {
    state_new.bgw_pipe_a = ~state_new.tile_temp_a;
    state_new.bgw_pipe_b =  state_new.tile_temp_b;
  }

  uint8_t bgw_pix_a = get_bit(state_new.bgw_pipe_a, 7) && bgw_en_new;
  uint8_t bgw_pix_b = get_bit(state_new.bgw_pipe_b, 7) && bgw_en_new;
  uint8_t spr_pix_a = get_bit(state_new.spr_pipe_a, 7) && spr_en_new;
  uint8_t spr_pix_b = get_bit(state_new.spr_pipe_b, 7) && spr_en_new;
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

  uint8_t REMY_LD0n = (pal >> (col * 2 + 0)) & 1;
  uint8_t RAVO_LD1n = (pal >> (col * 2 + 1)) & 1;

  //----------------------------------------
  // LCD pins

  // the "avap_scan_done_new && state_new.lcd.PAHO_X_8_SYNC" latch branch is never hit, would probably cause
  // latch oscillation or something
  if (vid_rst_new) {
    state_new.lcd.POME_X8_LATCH.state = 1;
  }
  else if (scan_done_trig_new) {
    state_new.lcd.POME_X8_LATCH.state = 0;
  }
  else if (state_new.lcd.PAHO_X8_SYNC.state) {
    state_new.lcd.POME_X8_LATCH.state = 1;
  }

  //----------------------------------------
  // Audio

  //tock_spu_logic();

  //----------------------------------------
  // Memory buses

  uint8_t pins_ctrl_csn_new = 1;
  uint8_t pins_ctrl_rdn_new = 1;
  uint8_t pins_ctrl_wrn_new = 1;

  uint8_t pins_abus_lo = 0xFF;
  uint8_t pins_abus_hi = 0xFF;

  if (ext_addr_new && !cpu_addr_vram_new) {
    state_new.ext_addr_latch = cpu_addr_new & 0x7FFF;
  }

  if (MATU_DMA_RUNNINGp_new && !dma_addr_vram_new) {
    pins_ctrl_csn_new = !!(dma_addr_new & 0x8000);
    pins_abus_lo = (~dma_addr_new >> 0) & 0xFF;
    pins_abus_hi = (~dma_addr_new >> 8) & 0xFF;
  }
  else {
    pins_ctrl_csn_new = gen_clk_new(phase_total_old, 0b00111111) && ext_addr_new && cpu_addr_ram_new;
    pins_abus_lo = ~((state_new.ext_addr_latch >> 0) & 0xFF);
    pins_abus_hi = ~((state_new.ext_addr_latch >> 8) & 0x7F);
  }

  pins_ctrl_rdn_new = 1;
  pins_ctrl_wrn_new = 0;
  if (!(MATU_DMA_RUNNINGp_new && dma_addr_vram_new) && ext_addr_new && cpu_wr) {
    pins_ctrl_rdn_new = cpu_addr_vram_new;
    pins_ctrl_wrn_new = gen_clk_new(phase_total_old, 0b00001110) && !cpu_addr_vram_new;
  }

  pins_abus_hi &= 0b01111111;
  if (MATU_DMA_RUNNINGp_new && !dma_addr_vram_new) {
    pins_abus_hi |= (!!((~dma_addr_new >> 8) & 0b10000000)) << 7;
  }
  else if (!state_new.cpu_signals.TEPU_BOOT_BITn.state && cpu_addr_new <= 0x00FF) {
  }
  else {
    uint8_t bit = gen_clk_new(phase_total_old, 0b00111111) && ext_addr_new && !get_bit(cpu_addr_new, 15);
    pins_abus_hi |= bit << 7;
  }

  uint8_t pins_dbus = 0;
  
  if (cpu_wr && !req_addr_hi && !cpu_addr_bootrom_new && !cpu_addr_vram_new) {
    pins_dbus = ~state_new.cpu_dbus;
  }

  //----------------------------------------
  // Ext read

  if (pins_ctrl_rdn_new) {
    const uint16_t ext_addr_lo = pins_abus_lo ^ 0xFF;
    const uint16_t ext_addr_hi = pins_abus_hi ^ 0xFF;
    const uint16_t ext_addr = (ext_addr_lo << 0) | (ext_addr_hi << 8);

    const auto rom_addr_mask = cart_rom_addr_mask(cart_blob);
    const auto ram_addr_mask = cart_ram_addr_mask(cart_blob);

    const int region = ext_addr >> 13;
    uint8_t data_in = 0x00;

    bool ext_read_en = false;

    if (cart_has_mbc1(cart_blob)) {

      wire mbc1_ram_en = state_new.ext_mbc.MBC1_RAM_EN.state;
      wire mbc1_mode = state_new.ext_mbc.MBC1_MODE.state;

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
    const uint16_t ext_addr_lo = pins_abus_lo ^ 0xFF;
    const uint16_t ext_addr_hi = pins_abus_hi ^ 0xFF;
    const uint16_t ext_addr = (ext_addr_lo << 0) | (ext_addr_hi << 8);

    const auto region = ext_addr >> 13;
    const uint8_t data_out = ~pins_dbus;
    wire mbc1_ram_en = state_new.ext_mbc.MBC1_RAM_EN.state;
    wire mbc1_mode = state_new.ext_mbc.MBC1_MODE.state;

    const auto mbc1_ram_bank = mbc1_mode ? bit_pack(&state_new.ext_mbc.MBC1_BANK5, 2) : 0;
    const auto mbc1_ram_addr = ((ext_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & cart_ram_addr_mask(cart_blob);

    if (pins_ctrl_wrn_new && cart_has_mbc1(cart_blob)) {
      switch (region) {
      case 0: state_new.ext_mbc.MBC1_RAM_EN.state = (data_out & 0x0F) == 0x0A; break;
      case 1: bit_unpack(&state_new.ext_mbc.MBC1_BANK0, 5, data_out); break;
      case 2: bit_unpack(&state_new.ext_mbc.MBC1_BANK5, 2, data_out); break;
      case 3: state_new.ext_mbc.MBC1_MODE.state = (data_out & 1); break;
      case 4: break;
      case 5: if (cart_has_ram(cart_blob) && mbc1_ram_en) mem.cart_ram[mbc1_ram_addr & cart_ram_addr_mask(cart_blob)] = (uint8_t)data_out; break;
      case 6: mem.int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out; break;
      case 7: mem.int_ram[ext_addr & 0x1FFF] = (uint8_t)data_out; break;
      }
    }

    if (pins_ctrl_wrn_new && !cart_has_mbc1(cart_blob)) {
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

  if (cpu_rd &&
     !(req_addr_vram && DELTA_HA_new) &&
     !req_addr_hi &&
     !cpu_addr_bootrom_new &&
     !cpu_addr_vram_new &&
     (DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new))
  {
    state_new.cpu_dbus = ~state_new.ext_data_latch;
  }
  else {
    state_new.ext_data_latch = pins_dbus;
  }

  // vram read address

  if (MATU_DMA_RUNNINGp_new && dma_addr_vram_new) {
    state_new.vram_abus = ~dma_addr_new & VRAM_ADDR_MASK;
  }
  else if (state_new.XYMU_RENDERINGn) {
    state_new.vram_abus = ~cpu_addr_new & VRAM_ADDR_MASK;
  }
  else {
    state_new.vram_abus = VRAM_ADDR_MASK;
  }


  //--------------------------------------------

  if (state_new.tfetch_control.LONY_FETCHINGp.state) {
    const auto px  = state_new.pix_count;
    const auto scx = ~state_new.reg_scx;
    const auto scy = ~state_new.reg_scy;

    const auto sum_x = px + scx;
    const auto sum_y = state_new.reg_ly + scy;

    //--------------------------------------------
    // BG map read address


    if (get_bit(state_new.tfetch_counter, 1) || get_bit(state_new.tfetch_counter, 2)) {

      const auto hilo = get_bit(state_new.tfetch_counter, 2);
      const auto tile_y = (state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state ? state_new.win_y.tile : (sum_y & 0b111));
      const auto map_y = state_new.tile_temp_b;
      const auto map = !get_bit(state_new.tile_temp_b, 7) && bgw_tile_new;

      uint32_t addr  = 0;
      bit_cat(addr,  0,  0, hilo);
      bit_cat(addr,  1,  3, tile_y);
      bit_cat(addr,  4, 11, map_y);
      bit_cat(addr, 12, 12, map);
      
      state_new.vram_abus = uint16_t(addr ^ VRAM_ADDR_MASK);
    }
    else {
      if (state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state) {
        uint32_t addr = 0;
        bit_cat(addr,  0,  4, ~state_new.win_x.map);
        bit_cat(addr,  5,  9, ~state_new.win_y.map);
        bit_cat(addr, 10, 10, win_map_new);
        state_new.vram_abus = uint16_t(addr);
      }
      else {
        uint32_t addr = 0;
        bit_cat(addr,  0,  4, (px + scx) >> 3);
        bit_cat(addr,  5,  9, (state_new.reg_ly + scy) >> 3);
        bit_cat(addr, 10, 10, !bg_map_new);
        bit_cat(addr, 11, 11, 1);
        bit_cat(addr, 12, 12, 1);

        state_new.vram_abus = uint16_t(addr ^ VRAM_ADDR_MASK);
      }

    }
  }

  //--------------------------------------------
  // Sprite read address


  if (TEXY_SFETCHINGp_evn_new) {
    wire hilo = state_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.state;
    const auto line = state_new.sprite_lbus ^ (get_bit(oam_temp_b_new, 6) ? 0b0000 : 0b1111);
    const auto tile = oam_temp_a_new;

    uint32_t addr = 0;
    bit_cat(addr,  0,  0, hilo);
    if (spr_size_new) {
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

  uint8_t pins_vram_ctrl_csn_new = 1;
  uint8_t pins_vram_ctrl_oen_new = 1;
  uint8_t pins_vram_ctrl_wrn_new = 1;

  if (MATU_DMA_RUNNINGp_new && dma_addr_vram_new) {
    pins_vram_ctrl_csn_new = 1;
    pins_vram_ctrl_oen_new = 1;
  }
  else if (!state_new.XYMU_RENDERINGn) {
    pins_vram_ctrl_csn_new = state_new.tfetch_control.LONY_FETCHINGp.state || TEXY_SFETCHINGp_evn_new;
    pins_vram_ctrl_oen_new = state_new.tfetch_control.LONY_FETCHINGp.state || (TEXY_SFETCHINGp_evn_new && (!state_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state || get_bit(state_new.sfetch_counter_evn, 0)));
  }
  else if (ext_addr_new) {
    pins_vram_ctrl_csn_new = (cpu_addr_vram_new && gen_clk_new(phase_total_old, 0b00111111) && 1);
    pins_vram_ctrl_oen_new = !cpu_addr_vram_new || !cpu.bus_req_new.write || DELTA_HA_new;
  }
  else {
    pins_vram_ctrl_csn_new = 0;
    pins_vram_ctrl_oen_new = !cpu_addr_vram_new || !cpu.bus_req_new.write || DELTA_HA_new;;
  }


  if (!state_new.XYMU_RENDERINGn) {
    pins_vram_ctrl_wrn_new = 0;
  }
  else if (ext_addr_new) {
    pins_vram_ctrl_wrn_new = cpu_addr_vram_new && gen_clk_new(phase_total_old, 0b00001110) && cpu_wr && 1;
  }
  else {
    pins_vram_ctrl_wrn_new = cpu_addr_vram_new && gen_clk_new(phase_total_old, 0b00001110) && cpu_wr && 0;
  }


  if (!state_new.XYMU_RENDERINGn) {
    if (pins_vram_ctrl_oen_new) {
      state_new.vram_dbus = mem.vid_ram[state_new.vram_abus ^ 0x1FFF];
    }
    pins_vram_dbus = ~state_new.vram_dbus;
  }
  else if (ext_addr_new) {
    if (gen_clk_new(phase_total_old, 0b00111111) && cpu_addr_vram_new && cpu_wr) {
      state_new.vram_dbus = state_new.cpu_dbus;
    }

    uint8_t vdata = 0xFF;

    if (pins_vram_ctrl_oen_new) vdata = mem.vid_ram[state_new.vram_abus ^ 0x1FFF];

    pins_vram_dbus = pins_vram_ctrl_oen_new ? ~vdata : 0;

    if (cpu_addr_vram_new && gen_clk_new(phase_total_old, 0b00111111) && cpu_wr) {
      pins_vram_dbus = ~state_new.vram_dbus;
    }
    else {
      state_new.vram_dbus = ~pins_vram_dbus;
    }

    if (pins_vram_ctrl_wrn_new) mem.vid_ram[state_new.vram_abus ^ 0x1FFF] = ~pins_vram_dbus;

    if (cpu_addr_vram_new && gen_clk_new(phase_total_old, 0b00111111) && (cpu.bus_req_new.read && !DELTA_HA_new) && ((DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new) && cpu.bus_req_new.read)) {
      state_new.cpu_dbus = state_new.vram_dbus;
    }
  }
  else {
    uint8_t vdata = 0xFF;

    if (pins_vram_ctrl_oen_new) vdata = mem.vid_ram[state_new.vram_abus ^ 0x1FFF];

    pins_vram_dbus = pins_vram_ctrl_oen_new ? ~vdata : 0;

    state_new.vram_dbus = ~pins_vram_dbus;

    if (pins_vram_ctrl_wrn_new) mem.vid_ram[state_new.vram_abus ^ 0x1FFF] = ~pins_vram_dbus;
  }

  //----------------------------------------
  // CPU read registers
  
  // This has to be before OAM, because the OAM dbus mirrors CPU dbus if it's not doing other things

  // But ZRAM can't be before OAM because ZRAM _doesn't_ show up on the oam bus? That seems wrong...

  if ((cpu.bus_req_new.read && !DELTA_HA_new)) {
    if (cpu_addr_new == 0xFF00) {
      set_bit(state_new.cpu_dbus, 0, !get_bit(state_new.joy_latch, 0));
      set_bit(state_new.cpu_dbus, 1, !get_bit(state_new.joy_latch, 1));
      set_bit(state_new.cpu_dbus, 2, !get_bit(state_new.joy_latch, 2));
      set_bit(state_new.cpu_dbus, 3, !get_bit(state_new.joy_latch, 3));
      set_bit(state_new.cpu_dbus, 4,  get_bit(state_new.reg_joy, 0));
      set_bit(state_new.cpu_dbus, 5,  get_bit(state_new.reg_joy, 1));
    }
    if (cpu_addr_new == 0xFF04) state_new.cpu_dbus =  (uint8_t)(state_new.reg_div >> 6);
    if (cpu_addr_new == 0xFF05) state_new.cpu_dbus =  state_new.reg_tima;
    if (cpu_addr_new == 0xFF06) state_new.cpu_dbus =  state_new.reg_tma;
    if (cpu_addr_new == 0xFF07) state_new.cpu_dbus =  (state_new.reg_tac & 0b111) | 0b11111000;
    if (cpu_addr_new == 0xFF40) state_new.cpu_dbus = ~state_new.reg_lcdc;
    if (cpu_addr_new == 0xFF42) state_new.cpu_dbus = ~state_new.reg_scy;
    if (cpu_addr_new == 0xFF43) state_new.cpu_dbus = ~state_new.reg_scx;
    if (cpu_addr_new == 0xFF44) state_new.cpu_dbus =  state_new.reg_ly;
    if (cpu_addr_new == 0xFF45) state_new.cpu_dbus = ~state_old.reg_lyc;
    if (cpu_addr_new == 0xFF47) state_new.cpu_dbus = ~state_new.reg_bgp;
    if (cpu_addr_new == 0xFF48) state_new.cpu_dbus = ~state_new.reg_obp0;
    if (cpu_addr_new == 0xFF49) state_new.cpu_dbus = ~state_new.reg_obp1;
    if (cpu_addr_new == 0xFF4A) state_new.cpu_dbus = ~state_new.reg_wy;
    if (cpu_addr_new == 0xFF4B) state_new.cpu_dbus = ~state_new.reg_wx;
  }

  //----------------------------------------
  // oam

  // this is weird, why is it always 0 when not in reset?
  state_new.oam_ctrl.MAKA_LATCH_EXTp.state = 0;

  // OAM bus has to be pulled up or we fail regression
  state_new.oam_abus = 0xFF;
  state_new.oam_dbus_a = 0xFF;
  state_new.oam_dbus_b = 0xFF;

  if (gen_clk_new(phase_total_old, 0b11110000)) state_new.oam_ctrl.WUJE_CPU_OAM_WRn.state = 1;
  if (cpu_addr_oam_new && cpu_wr && gen_clk_new(phase_total_old, 0b00001110)) state_new.oam_ctrl.WUJE_CPU_OAM_WRn.state = 0;

  //----------
  // OAM bus and control signals.
  // The inclusion of cpu_addr_oam_new in the SCANNING and RENDERING branches is probably a hardware bug.

  if (MATU_DMA_RUNNINGp_new) {
    state_new.oam_abus = (uint8_t)~state_new.dma_lo;

    if (DELTA_HA_new || DELTA_AB_new || DELTA_BC_new || DELTA_CD_new) {
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

    state_new.oam_dbus_a = dma_addr_vram_new ? ~state_new.vram_dbus : pins_dbus;
    state_new.oam_dbus_b = dma_addr_vram_new ? ~state_new.vram_dbus : pins_dbus;
  }
  else if (state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state && !vid_rst_new) {
    state_new.oam_abus = (uint8_t)~((state_new.scan_counter << 2) | 0b00);
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
    state_new.oam_ctrl.SIG_OAM_OEn.state   = 0;

    if (DELTA_GH_new || DELTA_HA_new || DELTA_CD_new || DELTA_DE_new) {
      state_new.oam_ctrl.SIG_OAM_CLKn.state  = (DELTA_DE_new && !cpu_addr_oam_new) || DELTA_HA_new;
      state_new.oam_ctrl.SIG_OAM_OEn.state   = !(cpu_addr_oam_new && (cpu.bus_req_new.read && !DELTA_HA_new) && !((DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new) && cpu.bus_req_new.read));
    }

    if (!state_new.oam_ctrl.SIG_OAM_OEn.state) {
      state_new.oam_dbus_a = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) & ~1];
      state_new.oam_dbus_b = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) |  1];
    }
  }
  else if (!state_new.XYMU_RENDERINGn) {
    const auto sfetch_oam_clk_new = (get_bit(state_new.sfetch_counter_evn, 1) || get_bit(state_new.sfetch_counter_evn, 2) || (state_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state && !get_bit(state_new.sfetch_counter_evn, 0)));
    const auto sfetch_oam_oen_new = (get_bit(state_new.sfetch_counter_evn, 1) || get_bit(state_new.sfetch_counter_evn, 2) || !state_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state);

    state_new.oam_abus = (uint8_t)~((state_new.sprite_ibus << 2) | 0b11);
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = sfetch_oam_clk_new && (!cpu_addr_oam_new || gen_clk_new(phase_total_old, 0b11110000));
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
    state_new.oam_ctrl.SIG_OAM_OEn.state   = sfetch_oam_oen_new && !(cpu_addr_oam_new && (cpu.bus_req_new.read && !DELTA_HA_new) && !((DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new) && cpu.bus_req_new.read));

    if (!sfetch_oam_oen_new) {
      state_new.oam_dbus_a = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) & ~1];
      state_new.oam_dbus_b = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) |  1];
    }
    else if (cpu_addr_oam_new && (cpu.bus_req_new.read && !DELTA_HA_new) && !((DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new) && cpu.bus_req_new.read)) {
      state_new.oam_dbus_a = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) & ~1];
      state_new.oam_dbus_b = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) |  1];
    }
  }
  else if (cpu_addr_oam_new) {
    state_new.oam_abus = (uint8_t)~cpu_addr_new;
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = DELTA_HA_new || DELTA_AB_new || DELTA_BC_new || DELTA_CD_new;
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
    state_new.oam_ctrl.SIG_OAM_OEn.state   = !((cpu.bus_req_new.read && !DELTA_HA_new) && !((DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new) && cpu.bus_req_new.read));

    if (DELTA_DE_new || DELTA_EF_new || DELTA_FG_new) {
      state_new.oam_ctrl.SIG_OAM_WRn_A.state = !cpu_wr || !get_bit(state_new.oam_abus, 0);
      state_new.oam_ctrl.SIG_OAM_WRn_B.state = !cpu_wr ||  get_bit(state_new.oam_abus, 0);
    }

    if ((cpu.bus_req_new.read && !DELTA_HA_new)) {
      if (((DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new) && cpu.bus_req_new.read)) {
        state_new.cpu_dbus = get_bit(state_new.oam_abus, 0) ? ~state_new.oam_latch_a : ~state_new.oam_latch_b;
      }
      else {
        state_new.oam_dbus_a = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) & ~1];
        state_new.oam_dbus_b = ~mem.oam_ram[(state_new.oam_abus ^ 0xFF) |  1];
      }
    }

    wire XUPA_CPU_OAM_WRp  = !state_new.oam_ctrl.WUJE_CPU_OAM_WRn.state;
    wire AJUJ_OAM_BUSYn    = !(state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state && !vid_rst_new) && state_new.XYMU_RENDERINGn;
    wire APAG_CBD_TO_OBDp  = (XUPA_CPU_OAM_WRp && AJUJ_OAM_BUSYn);
    if (APAG_CBD_TO_OBDp) {
      state_new.oam_dbus_a = ~state_new.cpu_dbus;
      state_new.oam_dbus_b = ~state_new.cpu_dbus;
    }
  }
  else {
    state_new.oam_ctrl.SIG_OAM_CLKn.state  = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
    state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
    state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
    state_new.oam_abus = (uint8_t)~cpu_addr_new;
    state_new.oam_dbus_a = ~state_new.cpu_dbus;
    state_new.oam_dbus_b = ~state_new.cpu_dbus;

    wire APAG_CBD_TO_OBDp  = !(state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state && !vid_rst_new) && state_new.XYMU_RENDERINGn;
    if (APAG_CBD_TO_OBDp) {
      state_new.oam_dbus_a = ~state_new.cpu_dbus;
      state_new.oam_dbus_b = ~state_new.cpu_dbus;
    }
  }

  // OAM latch stores the contents of the oam dbus

  if ((!MATU_DMA_RUNNINGp_new && state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state && !vid_rst_new)) {
    if (DELTA_AB_new || DELTA_BC_new || DELTA_EF_new || DELTA_FG_new) {
      state_new.oam_latch_a = state_new.oam_dbus_a;
      state_new.oam_latch_b = state_new.oam_dbus_b;
    }
  }
  else if (!state_new.XYMU_RENDERINGn) {
    if (!(get_bit(state_new.sfetch_counter_evn, 1) || get_bit(state_new.sfetch_counter_evn, 2) || !state_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state)) {
      state_new.oam_latch_a = state_new.oam_dbus_a;
      state_new.oam_latch_b = state_new.oam_dbus_b;
    }
  }
  else {
    if (cpu_addr_oam_new && (cpu.bus_req_new.read && !DELTA_HA_new) && !((DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new) && cpu.bus_req_new.read)) {
      state_new.oam_latch_a = state_new.oam_dbus_a;
      state_new.oam_latch_b = state_new.oam_dbus_b;
    }
  }

  // Actual OAM write
  if (state_old.oam_ctrl.SIG_OAM_CLKn.state && !state_new.oam_ctrl.SIG_OAM_CLKn.state) {
    uint8_t oam_addr_new = uint8_t(~state_new.oam_abus) >> 1;
    if (!state_new.oam_ctrl.SIG_OAM_WRn_A.state) mem.oam_ram[(oam_addr_new << 1) + 0] = ~state_new.oam_dbus_a;
    if (!state_new.oam_ctrl.SIG_OAM_WRn_B.state) mem.oam_ram[(oam_addr_new << 1) + 1] = ~state_new.oam_dbus_b;
  }
 
  //----------------------------------------
  // zram

  if ((cpu.bus_req_new.read && !DELTA_HA_new)) {
    if ((cpu_addr_new >= 0xFF80) && (cpu_addr_new <= 0xFFFE)) state_new.cpu_dbus = mem.zero_ram[cpu_addr_new & 0x007F];
  }

  //----------------------------------------
  // And finally, interrupts.

  auto pack_cpu_dbus_old = state_old.cpu_dbus;
  auto pack_cpu_dbus_new = state_new.cpu_dbus;
  auto pack_ie = state_new.reg_ie;
  auto pack_if = state_new.reg_if;
  auto pack_stat = state_new.reg_stat;

  if (!vid_rst_new && DELTA_BC_new) {
    state_new.int_ctrl.ROPO_LY_MATCH_SYNCp.state = state_old.reg_ly == (state_old.reg_lyc ^ 0xFF);
  }

  // FIXME this seems slightly wrong...
  if (cpu_wr && gen_clk_new(phase_total_old, 0b00001110) && cpu_addr_new == 0xFF41) {
  }
  else {
    state_new.int_ctrl.RUPO_LYC_MATCHn.state = 1;
  }

  // but the "reset" arm of the latch overrides the "set" arm, so it doesn't completely break?
  if (state_new.int_ctrl.ROPO_LY_MATCH_SYNCp.state) {
    state_new.int_ctrl.RUPO_LYC_MATCHn.state = 0;
  }

  if (cpu_addr_new == 0xFFFF && cpu_wr && gen_clk_new(phase_total_old, 0b00000001)) {
    pack_ie = pack_cpu_dbus_old;
  }

  if (cpu_addr_new == 0xFF41 && cpu_wr && gen_clk_new(phase_total_old, 0b00000001)) {
    pack_stat = (~pack_cpu_dbus_old >> 3) & 0b00001111;
  }

  if (cpu_addr_new == 0xFF41 && (cpu.bus_req_new.read && !DELTA_HA_new)) {
    uint8_t stat = 0x80;

    stat |= (!state_new.XYMU_RENDERINGn || state_new.lcd.POPU_VBLANKp_odd.state) << 0;
    stat |= (!state_new.XYMU_RENDERINGn || (!MATU_DMA_RUNNINGp_new && state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state && !vid_rst_new)) << 1;
    stat |= (!state_new.int_ctrl.RUPO_LYC_MATCHn.state) << 2;
    stat |= (pack_stat ^ 0b1111) << 3;

    pack_cpu_dbus_new = stat;
  }

  bool int_stat_old = 0;
  auto& POPU_VBLANKp_odd_old = state_old.lcd.POPU_VBLANKp_odd.state;
  if (!get_bit(state_old.reg_stat, 0) && (!state_old.FEPO_STORE_MATCHp_odd && (state_old.pix_count == 167)) && !POPU_VBLANKp_odd_old) int_stat_old = 1;
  if (!get_bit(state_old.reg_stat, 1) && POPU_VBLANKp_odd_old) int_stat_old = 1;
  if (!get_bit(state_old.reg_stat, 2) && !POPU_VBLANKp_odd_old && state_old.lcd.RUTU_LINE_ENDp_odd.state) int_stat_old = 1;
  if (!get_bit(state_old.reg_stat, 3) && state_old.int_ctrl.ROPO_LY_MATCH_SYNCp.state) int_stat_old = 1;

  wire int_lcd_old = POPU_VBLANKp_odd_old;
  wire int_joy_old = !state_old.joy_int.APUG_JP_GLITCH3.state || !state_old.joy_int.BATU_JP_GLITCH0.state;
  wire int_tim_old = state_old.int_ctrl.MOBA_TIMER_OVERFLOWp.state;
  //wire int_ser_old = serial.CALY_SER_CNT3;
  wire int_ser_old = 0;

  bool hblank_new = !state_old.FEPO_STORE_MATCHp_odd && (state_new.pix_count & 167) == 167;

  bool int_stat_new = 0;
  if (!get_bit(pack_stat, 0) && hblank_new && !state_new.lcd.POPU_VBLANKp_odd.state) int_stat_new = 1;
  if (!get_bit(pack_stat, 1) && state_new.lcd.POPU_VBLANKp_odd.state) int_stat_new = 1;
  if (!get_bit(pack_stat, 2) && !state_new.lcd.POPU_VBLANKp_odd.state && state_new.lcd.RUTU_LINE_ENDp_odd.state) int_stat_new = 1;
  if (!get_bit(pack_stat, 3) && state_new.int_ctrl.ROPO_LY_MATCH_SYNCp.state) int_stat_new = 1;

  const wire int_lcd_new = state_new.lcd.POPU_VBLANKp_odd.state;
  const wire int_joy_new = !state_new.joy_int.APUG_JP_GLITCH3.state || !state_new.joy_int.BATU_JP_GLITCH0.state;
  const wire int_tim_new = state_new.int_ctrl.MOBA_TIMER_OVERFLOWp.state;
  //const wire int_ser = state_new.serial.CALY_SER_CNT3;
  const wire int_ser_new = 0;

  if (!int_lcd_old  && int_lcd_new)  pack_if |= (1 << 0);
  if (!int_stat_old && int_stat_new) pack_if |= (1 << 1);
  if (!int_tim_old  && int_tim_new)  pack_if |= (1 << 2);
  if (!int_ser_old  && int_ser_new)  pack_if |= (1 << 3);
  if (!int_joy_old  && int_joy_new)  pack_if |= (1 << 4);

  // note this is an async set so it doesn't happen on the GH clock edge like other writes
  if (cpu_wr && gen_clk_new(phase_total_old, 0b00001110)) {
    if (cpu_addr_new == 0xFF0F) pack_if = pack_cpu_dbus_new;
  }

  pack_if &= ~cpu.core.int_ack;

  if ((cpu.bus_req_new.read && !DELTA_HA_new)) {
    if (cpu_addr_new == 0xFFFF) pack_cpu_dbus_new = pack_ie | 0b11100000;
    if (cpu_addr_new == 0xFF0F) state_new.int_latch = (uint8_t)pack_if;
    if (cpu_addr_new == 0xFF0F) pack_cpu_dbus_new = pack_if | 0b11100000;
  }


  state_new.cpu_dbus = (uint8_t)pack_cpu_dbus_new;
  state_new.cpu_int = (uint8_t)pack_if;
  state_new.reg_ie = (uint8_t)pack_ie;
  state_new.reg_if = (uint8_t)pack_if;
  state_new.reg_stat = (uint8_t)pack_stat;




  int lcd_x = pix_count_new - 8;
  int lcd_y = state_new.reg_ly;

  if (lcd_y >= 0 && lcd_y < 144 && lcd_x >= 0 && lcd_x < 160) {
    wire p0 = !REMY_LD0n;
    wire p1 = !RAVO_LD1n;
    auto new_pix = p0 + p1 * 2;

    mem.framebuffer[lcd_x + lcd_y * 160] = uint8_t(3 - new_pix);
  }
















































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

    state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = ext_addr_new;
    state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.state = ((DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new) && cpu.bus_req_new.read);
    state_new.cpu_signals.SIG_IN_CPU_WRp.state = cpu_wr;
    state_new.cpu_signals.SIG_IN_CPU_RDp.state = cpu_rd;
    if (!vid_rst_new) {
      if (DELTA_FG_new) {
        state_new.lcd.SYGU_LINE_STROBE.state = (state_old.reg_lx == 0) || (state_old.reg_lx == 7) || (state_old.reg_lx == 45) || (state_old.reg_lx == 83);
      }

      if (state_old.lcd.RUTU_LINE_ENDp_odd.state && !state_new.lcd.RUTU_LINE_ENDp_odd.state) state_new.lcd.LUCA_LINE_EVENp.state = !state_new.lcd.LUCA_LINE_EVENp.state;
      if (!POPU_VBLANKp_odd_old && state_new.lcd.POPU_VBLANKp_odd.state) state_new.lcd.NAPO_FRAME_EVENp.state = !state_new.lcd.NAPO_FRAME_EVENp.state;

      if (state_old.lcd.NYPE_LINE_ENDp_odd.state && !state_new.lcd.NYPE_LINE_ENDp_odd.state) {
        state_new.lcd.MEDA_VSYNC_OUTn.state = state_new.reg_ly == 0;
      }

      if (get_bit(state_new.pix_count, 0) && get_bit(state_new.pix_count, 3)) state_new.lcd.WUSA_LCD_CLOCK_GATE.state = 1;
      if (state_new.VOGA_HBLANKp) state_new.lcd.WUSA_LCD_CLOCK_GATE.state = 0;
    }
    if (vid_rst_new) {
      state_new.lcd.SYGU_LINE_STROBE.state = 0;
      state_new.lcd.LUCA_LINE_EVENp.state = 0;
      state_new.lcd.NAPO_FRAME_EVENp.state = 0;
      state_new.lcd.MEDA_VSYNC_OUTn.state = 0;
      state_new.lcd.WUSA_LCD_CLOCK_GATE.state = 0;
    }

    bit_unpack(pins.abus_lo, pins_abus_lo);
    bit_unpack(pins.abus_hi, pins_abus_hi);

    pins.ctrl.PIN_78_WRn.state = pins_ctrl_wrn_new;
    pins.ctrl.PIN_79_RDn.state = pins_ctrl_rdn_new;
    pins.ctrl.PIN_80_CSn.state = pins_ctrl_csn_new;

    pins.vram_ctrl.PIN_43_VRAM_CSn.state = pins_vram_ctrl_csn_new;
    pins.vram_ctrl.PIN_45_VRAM_OEn.state = pins_vram_ctrl_oen_new;
    pins.vram_ctrl.PIN_49_VRAM_WRn.state = pins_vram_ctrl_wrn_new;

    if (!vid_rst_new) {

      pins.lcd.PIN_50_LCD_DATA1.state = RAVO_LD1n;
      pins.lcd.PIN_51_LCD_DATA0.state = REMY_LD0n;
      pins.lcd.PIN_52_LCD_CNTRL.state = !state_new.lcd.SYGU_LINE_STROBE.state && !state_new.lcd.RUTU_LINE_ENDp_odd.state;
      pins.lcd.PIN_53_LCD_CLOCK.state = (!state_new.lcd.WUSA_LCD_CLOCK_GATE.state || (TYFA_CLKPIPE_evn_new && !state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state)) && (!state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state || state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state);
      pins.lcd.PIN_54_LCD_HSYNC.state = state_new.lcd.POME_X8_LATCH.state;
      pins.lcd.PIN_55_LCD_LATCH.state = !state_new.lcd.RUTU_LINE_ENDp_odd.state;
      pins.lcd.PIN_56_LCD_FLIPS.state = state_new.lcd.NAPO_FRAME_EVENp.state ^ state_new.lcd.LUCA_LINE_EVENp.state;
      pins.lcd.PIN_57_LCD_VSYNC.state = !state_new.lcd.MEDA_VSYNC_OUTn.state;
    }
    else {
      pins.lcd.PIN_50_LCD_DATA1.state = RAVO_LD1n;
      pins.lcd.PIN_51_LCD_DATA0.state = REMY_LD0n;
      pins.lcd.PIN_52_LCD_CNTRL.state = 1;
      pins.lcd.PIN_53_LCD_CLOCK.state = 1;
      pins.lcd.PIN_54_LCD_HSYNC.state = 1;
      pins.lcd.PIN_55_LCD_LATCH.state = !get_bit(state_new.reg_div, 6);
      pins.lcd.PIN_56_LCD_FLIPS.state = !get_bit(state_new.reg_div, 7);
      pins.lcd.PIN_57_LCD_VSYNC.state = 1;
    }


    state_new.lcd.RUJU.state = state_new.lcd.POME_X8_LATCH.state;
    state_new.lcd.POFY.state = !state_new.lcd.POME_X8_LATCH.state;
    state_new.lcd.REMY_LD0n.state = REMY_LD0n;
    state_new.lcd.RAVO_LD1n.state = RAVO_LD1n;
    state_new.sprite_scanner.AVAP_SCAN_DONE_tp_odd.state = !vid_rst_new && !line_rst_new && scan_done_trig_new;
    state_new.sfetch_control.TEXY_SFETCHINGp_evn.state = TEXY_SFETCHINGp_evn_new;
    state_new.WODU_HBLANKp_odd = hblank_new;
    state_new.ACYL_SCANNINGp_odd = (!MATU_DMA_RUNNINGp_new && state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state && !vid_rst_new);
    state_new.ATEJ_LINE_RSTp_odd = line_rst_new || vid_rst_new;
    state_new.SATO_BOOT_BITn = get_bit(state_new.cpu_dbus, 0) || state_new.cpu_signals.TEPU_BOOT_BITn.state;
    state_new.cpu_signals.SIG_CPU_BOOTp.state = 0;
    state_new.cpu_signals.SIG_BOOT_CSp.state = 0;

    if (cpu_addr_new <= 0x00FF) {

      state_new.cpu_signals.SIG_CPU_BOOTp.state = !state_new.cpu_signals.TEPU_BOOT_BITn.state;

      if ((cpu.bus_req_new.read && !DELTA_HA_new) && !state_new.cpu_signals.TEPU_BOOT_BITn.state) {
        state_new.cpu_signals.SIG_BOOT_CSp.state = 1;
      }
    }

    bit_unpack(pins.vram_dbus, pins_vram_dbus);
    bit_unpack(pins.vram_abus, state_new.vram_abus);

    bit_unpack(pins.dbus, pins_dbus);


    pins.joy.PIN_63_JOY_P14.state = !get_bit(state_new.reg_joy, 0);
    pins.joy.PIN_62_JOY_P15.state = !get_bit(state_new.reg_joy, 1);

    pins.joy.PIN_67_JOY_P10.state = get_bit(state_new.joy_latch, 0);
    pins.joy.PIN_66_JOY_P11.state = get_bit(state_new.joy_latch, 1);
    pins.joy.PIN_65_JOY_P12.state = get_bit(state_new.joy_latch, 2);
    pins.joy.PIN_64_JOY_P13.state = get_bit(state_new.joy_latch, 3);
    

    if (get_bit(oam_temp_b_new, 5) && TEXY_SFETCHINGp_evn_new) {
      state_new.flipped_sprite = bit_reverse(state_new.vram_dbus);
    }
    else {
      state_new.flipped_sprite = state_new.vram_dbus;
    }

    state_new.oam_ctrl.old_oam_clk.state = !state_new.oam_ctrl.SIG_OAM_CLKn.state; // Vestige of gate mode
    state_new.zram_bus.clk_old.state = gen_clk_new(phase_total_old, 0b00001110) && cpu_wr;

    state_new.cpu_ack = cpu.core.int_ack;

    pins.sys.PIN_74_CLK.CLK.state = gen_clk_new(phase_total_old, 0b10101010); // dead signal
    pins.sys.PIN_74_CLK.CLKGOOD.state = 1; // dead signal

    pins.sys.PIN_71_RST.state = 0; // dead signal
    pins.sys.PIN_76_T2.state = 0; // dead signal
    pins.sys.PIN_77_T1.state = 0; // dead signal

    state_new.sys_clk.SIG_CPU_CLKREQ.state = 1; // dead signal

    state_new.cpu_signals.SIG_CPU_ADDR_HIp.state = cpu_addr_new >= 0xFE00 && cpu_addr_new <= 0xFFFF; // dead signal
    state_new.cpu_signals.SIG_CPU_UNOR_DBG.state = 0; // dead signal
    state_new.cpu_signals.SIG_CPU_UMUT_DBG.state = 0; // dead signal

    pins.sys.PIN_73_CLK_DRIVE.state = pins.sys.PIN_74_CLK.CLK.state; // dead signal
    state_new.sys_clk.AVET_DEGLITCH.state = pins.sys.PIN_74_CLK.CLK.state; // dead signal
    state_new.sys_clk.ANOS_DEGLITCH.state = !pins.sys.PIN_74_CLK.CLK.state; // dead signal

    state_new.sys_clk.AFUR_xxxxEFGH.state = gen_clk_new(phase_total_old, 0b00001111); // dead signal
    state_new.sys_clk.ALEF_AxxxxFGH.state = gen_clk_new(phase_total_old, 0b10000111); // dead signal
    state_new.sys_clk.APUK_ABxxxxGH.state = gen_clk_new(phase_total_old, 0b11000011); // dead signal
    state_new.sys_clk.ADYK_ABCxxxxH.state = gen_clk_new(phase_total_old, 0b11100001); // dead signal

    pins.sys.PIN_75_CLK_OUT.state = gen_clk_new(phase_total_old, 0b00001111); // dead signal

    state_new.sys_clk.SIG_CPU_BOWA_Axxxxxxx.state = gen_clk_new(phase_total_old, 0b10000000); // dead signal
    state_new.sys_clk.SIG_CPU_BEDO_xBCDEFGH.state = gen_clk_new(phase_total_old, 0b01111111); // dead signal
    state_new.sys_clk.SIG_CPU_BEKO_ABCDxxxx.state = gen_clk_new(phase_total_old, 0b11110000); // dead signal
    state_new.sys_clk.SIG_CPU_BUDE_xxxxEFGH.state = gen_clk_new(phase_total_old, 0b00001111); // dead signal
    state_new.sys_clk.SIG_CPU_BOLO_ABCDEFxx.state = gen_clk_new(phase_total_old, 0b11111100); // dead signal
    state_new.sys_clk.SIG_CPU_BUKE_AxxxxxGH.state = gen_clk_new(phase_total_old, 0b10000011); // dead signal
    state_new.sys_clk.SIG_CPU_BOMA_xBCDEFGH.state = gen_clk_new(phase_total_old, 0b01111111); // dead signal
    state_new.sys_clk.SIG_CPU_BOGA_Axxxxxxx.state = gen_clk_new(phase_total_old, 0b10000000); // dead signal

    state_new.cpu_signals.TEDO_CPU_RDp.state = (cpu.bus_req_new.read && !DELTA_HA_new); // dead signal
    state_new.cpu_signals.APOV_CPU_WRp.state = gen_clk_new(phase_total_old, 0b00001110) && cpu_wr; // dead signal
    state_new.cpu_signals.TAPU_CPU_WRp.state = gen_clk_new(phase_total_old, 0b00001110) && cpu_wr; // dead signal
    state_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK.state = (gen_clk_new(phase_total_old, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state); // dead signal

    state_new.sys_rst.AFER_SYS_RSTp.state = 0; // dead signal
    state_new.sys_rst.TUBO_WAITINGp.state = 0; // dead signal
    state_new.sys_rst.ASOL_POR_DONEn.state = 0; // dead signal
    state_new.sys_rst.SIG_CPU_EXT_CLKGOOD.state = 1; // dead signal
    state_new.sys_rst.SIG_CPU_EXT_RESETp.state = 0; // dead signal
    state_new.sys_rst.SIG_CPU_STARTp.state = 0; // dead signal
    state_new.sys_rst.SIG_CPU_INT_RESETp.state = 0; // dead signal
    state_new.sys_rst.SOTO_DBG_VRAMp.state = 0; // dead signal

    state_new.sys_clk.WOSU_AxxDExxH.state = !vid_rst_new && gen_clk_new(phase_total_old, 0b10011001); // dead signal
    state_new.sys_clk.WUVU_ABxxEFxx.state = !vid_rst_new && gen_clk_new(phase_total_old, 0b11001100); // dead signal
    state_new.sys_clk.VENA_xxCDEFxx.state = !vid_rst_new && gen_clk_new(phase_total_old, 0b00111100); // dead signal
  }

  if (config_debug) {
    lb_bit_check();
  }
}

void LogicBoy::lb_bit_check() {
  bit_check(pins);
  bit_check(lb_state.sys_rst);
  bit_check(lb_state.sys_clk);
  bit_check(lb_state.cpu_signals);
  bit_check(lb_state.int_ctrl);
  bit_check(lb_state.oam_ctrl);
  bit_check(lb_state.ext_mbc);
  bit_check(lb_state.zram_bus);
  bit_check(lb_state.dma_ctrl);
  bit_check(lb_state.joy_int);
  bit_check(lb_state.sprite_scanner);
  bit_check(lb_state.sfetch_control);
  bit_check(lb_state.tfetch_control);
  bit_check(lb_state.win_ctrl);
  bit_check(lb_state.fine_scroll);
  bit_check(lb_state.lcd);
}