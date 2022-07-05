/// plait_noparse

#include "GateBoyLib/LogicBoy.h"
#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Utils.h"

#include <set>
#include <string>
#include <assert.h>

//#pragma optimize("", off)

/*
FieldInfo LogicBoy::fields[] = {
  DECLARE_FIELD(LogicBoy, lb_state),
  DECLARE_FIELD(LogicBoy, gb_state),
  DECLARE_FIELD(LogicBoy, cpu),
  DECLARE_FIELD(LogicBoy, mem),
  DECLARE_FIELD(LogicBoy, sys),
  DECLARE_FIELD(LogicBoy, pins),
  DECLARE_FIELD(LogicBoy, probes),
};
*/

#define gb_state FORBIDDEN

using std::min;

//-----------------------------------------------------------------------------

GBResult LogicBoy::reset_to_poweron(const blob& cart_blob) {
  assert(false);
}

//-----------------------------------------------------------------------------

GBResult LogicBoy::reset_to_bootrom(const blob& cart_blob, bool slow) {
  (void)slow;

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

  cpu.core.reg.bus_req_new.addr = addr;
  cpu.core.reg.bus_req_new.data = data;
  cpu.core.reg.bus_req_new.read = !write;
  cpu.core.reg.bus_req_new.write = write;

  return GBResult::ok();
}

//-----------------------------------------------------------------------------

GBResult LogicBoy::dbg_read(const blob& cart_blob, int addr) {
  CHECK_P((sys.gb_phase_total & 7) == 0);

  Req old_req = cpu.core.reg.bus_req_new;
  bool old_cpu_en = sys.cpu_en;
  sys.cpu_en = false;

  dbg_req((uint16_t)addr, 0, 0);
  run_phases(cart_blob, 8);

  cpu.core.reg.bus_req_new = old_req;
  sys.cpu_en = old_cpu_en;

  return GBResult(cpu.core.reg.cpu_data_latch);
}

//------------------------------------------------------------------------------

GBResult LogicBoy::dbg_write(const blob& cart_blob, int addr, uint8_t data_in) {
  CHECK_P((sys.gb_phase_total & 7) == 0);

  Req old_req = cpu.core.reg.bus_req_new;
  bool old_cpu_en = sys.cpu_en;
  sys.cpu_en = false;

  dbg_req((uint16_t)addr, data_in, 1);
  GBResult res = run_phases(cart_blob, 8);

  cpu.core.reg.bus_req_new = old_req;
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

GBResult LogicBoy::run_to(const blob& cart_blob, int phase) {
  while(get_sys().gb_phase_total < phase) {
    next_phase(cart_blob);
  }
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

#define DELTA_AD_new   (DELTA_AB_new || DELTA_BC_new || DELTA_CD_new)

#define DELTA_AC_new   (DELTA_AB_new || DELTA_BC_new)
#define DELTA_EG_new   (DELTA_EF_new || DELTA_FG_new)
#define DELTA_HD_new   (DELTA_HA_new || DELTA_AB_new || DELTA_BC_new || DELTA_CD_new)
#define DELTA_DH_old   (DELTA_DE_old || DELTA_EF_old || DELTA_FG_old || DELTA_GH_old)
#define DELTA_DH_new   (DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new)
#define DELTA_DG_new   (DELTA_DE_new || DELTA_EF_new || DELTA_FG_new)

#define DELTA_EVEN_new ((phase_old & 1) == 0)
#define DELTA_ODD_new  ((phase_old & 1) == 1)

#define DELTA_EVEN_old ((phase_old & 1) == 1)
#define DELTA_ODD_old  ((phase_old & 1) == 0)

void LogicBoy::tock_cpu() {
  auto phase_old = (sys.gb_phase_total - 1) & 7;
  auto phase_new = (sys.gb_phase_total - 0) & 7;

#if 0
  cpu.cpu_data_latch &= lb_state.cpu_dbus;
  //cpu.imask_latch = lb_state.reg_ie;

  if (DELTA_HA_new) {
    debugbreak();
    //cpu.core.update_halt(cpu.imask_latch, cpu.intf_halt_latch);
    //if (cpu.core.op == 0x76 && (cpu.imask_latch & cpu.intf_halt_latch)) cpu.core.state_ = 0;
    //cpu.intf_halt_latch = 0;
  }

  // +ha -ab -bc -cd -de -ef -fg -gh
  if (DELTA_HA_new) {
    // this one latches funny, some hardware bug
    if (get_bit(lb_state.reg_if, 2)) cpu.halt_latch |= INT_TIMER_MASK;
  }

  // -ha +ab -bc
  if (DELTA_AB_new) {
    if (sys.cpu_en) {
      //cpu.core.tock_ab(cpu.imask_latch, cpu.intf_latch, cpu.cpu_data_latch);

      // FIXME ONCE GATEBOY CPU IS CLEANED UP
      debugbreak();
      cpu.core.state = cpu.core.state_;

      if (cpu.core._bus_read) cpu.core.in = cpu.cpu_data_latch;

      if (cpu.core.state == 0) {
        cpu.core.op_addr = cpu.core._bus_addr;
        cpu.core.op = cpu.cpu_data_latch;

        if ((cpu.imask_latch & cpu.intf_latch) && cpu.core.ime) {
          cpu.core.op = 0xF4; // fake opcode
          cpu.core.ime = false;
          cpu.core.ime_delay = false;
        }
      }

      cpu.core.int_ack = 0;
      cpu.core.ime = cpu.core.ime_delay; // must be after int check, before op execution

      // #define HALT (op == 0x76)

      if      (cpu.core.op == 0xF4 /*INT*/ ) cpu.core.execute_int(cpu.imask_latch, cpu.intf_latch);
      else if (cpu.core.op == 0x76 /*HALT*/) cpu.core.execute_halt(cpu.imask_latch, cpu.intf_latch);
      else if (cpu.core.op == 0xCB /*CB*/  ) cpu.core.execute_cb();
      else                                   cpu.core.execute_op();

    }
  }

  if (DELTA_AB_new) {
    if (sys.cpu_en) {
      cpu.bus_req_new = cpu.core.get_bus_req();
      //cpu.bus_req_new.addr = cpu.core.get_bus_addr();
      //cpu.bus_req_new.data = cpu.core.get_bus_data();
      //cpu.bus_req_new.read = cpu.core.get_bus_read();
      //cpu.bus_req_new.write = cpu.core.get_bus_write();
    }
  }

  // -bc +cd +de -ef -fg -gh -ha -ab
  if (DELTA_DE_new) {
    if (get_bit(lb_state.reg_if, 0)) cpu.halt_latch |= INT_VBLANK_MASK;
    if (get_bit(lb_state.reg_if, 1)) cpu.halt_latch |= INT_STAT_MASK;
    if (get_bit(lb_state.reg_if, 3)) cpu.halt_latch |= INT_SERIAL_MASK;
    if (get_bit(lb_state.reg_if, 4)) cpu.halt_latch |= INT_JOYPAD_MASK;
  }

  // -ha -ab -bc -cd -de -ef +fg +gh
  if (DELTA_GH_new) cpu.cpu_data_latch = 0xFF;

  // +ha -ab -bc -cd -de -ef -fg +gh
  if (DELTA_GH_new) cpu.intf_latch = lb_state.reg_if;
#endif
}

//-----------------------------------------------------------------------------

void LogicBoy::tock_logic(const blob& cart_blob) {

  //----------------------------------------
  // old signals

  LogicBoyState  state_old = lb_state;
  auto phase_total_old = sys.gb_phase_total - 1;
  int phase_old = int((sys.gb_phase_total - 1) & 7);

  wire nuko_wx_match_old = (uint8_t(~state_old.reg_wx) == state_old.pix_count_odd) && state_old.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state;

  //----------------------------------------

  LogicBoyState& state_new = lb_state;
  auto phase_total_new = sys.gb_phase_total - 0;
  int phase_new = int((sys.gb_phase_total - 0) & 7);

  //----------------------------------------

  const auto cpu_dbus_old = state_old.cpu_dbus;

  // Data has to be driven on EFGH or we fail the wave tests

  state_new.cpu_dbus = (DELTA_DE_new || DELTA_EF_new || DELTA_FG_new || DELTA_GH_new) && cpu.core.reg.bus_req_new.write ? cpu.core.reg.bus_req_new.data_lo : 0xFF; // must be pulled up or we fail regression
  state_new.cpu_abus = (DELTA_HA_new ? cpu.core.reg.bus_req_new.addr & 0x00FF : cpu.core.reg.bus_req_new.addr);

  wire req_addr_vram = (cpu.core.reg.bus_req_new.addr >= 0x8000) && (cpu.core.reg.bus_req_new.addr <= 0x9FFF);
  wire req_addr_ram  = (cpu.core.reg.bus_req_new.addr >= 0xA000) && (cpu.core.reg.bus_req_new.addr <= 0xFDFF);
  wire req_addr_oam  = (cpu.core.reg.bus_req_new.addr >= 0xFE00) && (cpu.core.reg.bus_req_new.addr <= 0xFEFF);
  wire req_addr_hi   = (cpu.core.reg.bus_req_new.addr >= 0xFE00);
  wire req_addr_lo   = (cpu.core.reg.bus_req_new.addr <= 0x00FF);

  wire cpu_addr_oam_old  = (state_old.cpu_abus >= 0xFE00) && (state_old.cpu_abus <= 0xFEFF);

  wire cpu_addr_vram_new = req_addr_vram && !DELTA_HA_new;
  wire cpu_addr_ram_new  = req_addr_ram && !DELTA_HA_new;
  wire cpu_addr_oam_new  = req_addr_oam && !DELTA_HA_new;

  wire cpu_rd = cpu.core.reg.bus_req_new.read  && !DELTA_HA_new;
  wire cpu_wr = cpu.core.reg.bus_req_new.write && !DELTA_HA_new;

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
      state_new.int_ctrl.MOBA_TIMER_OVERFLOWp.state = !bit(state_old.reg_tima, 7) && state_old.int_ctrl.NYDU_TIMA7p_DELAY.state;
      state_new.int_ctrl.NYDU_TIMA7p_DELAY.state = bit(state_old.reg_tima, 7);
    }

    if (DELTA_DG_new && cpu_wr && !cpu_rd && cpu_addr_new == 0xFF05) {
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
    set_bit(state_new.reg_joy, 0, bit(state_old.cpu_dbus, 4));
    set_bit(state_new.reg_joy, 1, bit(state_old.cpu_dbus, 5));
  }
  // LCDC write has to be near the top as it controls the video reset signal
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF40) {
    //printf("lcdc write at %lld\n", sys.gb_phase_total);
    state_new.reg_lcdc = ~state_old.cpu_dbus;
  }
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF45) state_new.reg_lyc  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF42) state_new.reg_scy  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF43) state_new.reg_scx  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF47) state_new.reg_bgp  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF48) state_new.reg_obp0 = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF49) state_new.reg_obp1 = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF4A) state_new.reg_wy   = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF4B) state_new.reg_wx   = ~state_old.cpu_dbus;

  if (cpu_wr && DELTA_GH_new && cpu_addr_new == 0xFF50) {
    wire SATO_BOOT_BITn_old = bit(state_old.cpu_dbus, 0) || state_old.cpu_signals.TEPU_BOOT_BITn.state;
    state_new.cpu_signals.TEPU_BOOT_BITn.state = SATO_BOOT_BITn_old;
  }

  if (cpu_wr && DELTA_GH_new && (cpu_addr_new >= 0xFF80) && (cpu_addr_new <= 0xFFFE)) mem.zero_ram[cpu_addr_new & 0x007F] = state_old.cpu_dbus;

  wire req_addr_boot = (cpu.core.reg.bus_req_new.addr >= 0x0000) && (cpu.core.reg.bus_req_new.addr <= 0x00FF) && !state_new.cpu_signals.TEPU_BOOT_BITn.state;

  wire ext_addr_new = (cpu.core.reg.bus_req_new.read || cpu.core.reg.bus_req_new.write) &&
                            (( DELTA_HA_new && !req_addr_hi && !req_addr_boot && !req_addr_vram) ||
                            (!DELTA_HA_new && !req_addr_hi && !req_addr_boot));

  bool cpu_addr_bootrom_new = req_addr_lo && !state_new.cpu_signals.TEPU_BOOT_BITn.state;

  wire vid_rst_evn_old  =  bit(state_old.reg_lcdc, 7);

  wire bgw_en_new   = !bit(state_new.reg_lcdc, 0);
  wire spr_en_new   = !bit(state_new.reg_lcdc, 1);
  wire spr_size_new =  bit(state_new.reg_lcdc, 2);
  wire bg_map_new   =  bit(state_new.reg_lcdc, 3);
  wire bgw_tile_new =  bit(state_new.reg_lcdc, 4);
  wire win_en_new   = !bit(state_new.reg_lcdc, 5);
  wire win_map_new  =  bit(state_new.reg_lcdc, 6);
  wire vid_rst_evn_new  =  bit(state_new.reg_lcdc, 7);
  if (vid_rst_evn_old && !vid_rst_evn_new) CHECK_P(DELTA_EVEN_new);

  //----------------------------------------
  // LX, LY, lcd flags

  int64_t phase_lcd_old = state_new.phase_lcd;
  bool first_line_old = phase_lcd_old < 912;

  int phase_frame_old = int(phase_lcd_old % (154 * 912));
  int phase_lx_old = phase_frame_old % 912;
  int phase_ly_old = phase_frame_old / 912;

  uint8_t reg_lx_old = (uint8_t)((phase_lx_old - 4) / 8);
  uint8_t reg_ly_old = (uint8_t)(phase_frame_old >= 153 * 912 + 4 ? 0 : phase_ly_old);

  int phase_frame_d4_old = (phase_lcd_old - 4) % (154 * 912);
  int phase_ly_d4_old = phase_frame_d4_old / 912;
  bool vblank_old = phase_ly_d4_old >= 144;

  // if we're just coming out of reset, lcd phase is off by 8 (hardware glitch)
  if (vid_rst_evn_new) {
    state_new.phase_lcd = 0;
  }
  else {
    if (vid_rst_evn_old) state_new.phase_lcd = 8;
    state_new.phase_lcd++;
  }

  int64_t phase_lcd_new = state_new.phase_lcd;

  int phase_frame_new = int(phase_lcd_new % (154 * 912));
  int frame_index_new = int(phase_lcd_new / (154 * 912));
  int phase_lx_new = phase_frame_new % 912;
  int phase_ly_new = phase_frame_new / 912;
  bool first_line_new = phase_lcd_new < 912;

  int phase_frame_d4_new = (phase_lcd_new - 4) % (154 * 912);
  int phase_ly_d4_new = phase_frame_d4_new / 912;

  uint8_t reg_lx_new = (uint8_t)((phase_lx_new - 4) / 8);
  uint8_t reg_ly_new = (uint8_t)(phase_frame_new >= 153 * 912 + 4 ? 0 : phase_ly_new);

  bool line_rst_odd_old = false;
  if (phase_ly_old < 144)  line_rst_odd_old = (phase_lx_old == 2 || phase_lx_old == 3);
  if (phase_ly_old == 153) line_rst_odd_old = (phase_lx_old == 6 || phase_lx_old == 7);

  bool line_rst_odd_new = false;
  if (phase_ly_new < 144)  line_rst_odd_new = (phase_lx_new == 2 || phase_lx_new == 3);
  if (phase_ly_new == 153) line_rst_odd_new = (phase_lx_new == 6 || phase_lx_new == 7);

  if (line_rst_odd_old && !line_rst_odd_new) CHECK_P(DELTA_ODD_new);


  bool vblank_new = phase_ly_d4_new >= 144;

  //----------------------------------------
  // Joypad

  // FIXME what if both scan bits are set?

  if (DELTA_HA_new) {
    uint8_t button_mask = 0b00000000;
    if (!bit(state_new.reg_joy, 0)) button_mask |= 0b00001111;
    if (!bit(state_new.reg_joy, 1)) button_mask |= 0b11110000;

    state_new.int_ctrl.AWOB_WAKE_CPU.state  = !(sys.buttons & button_mask);
    state_new.int_ctrl.SIG_CPU_WAKE.state   = !(sys.buttons & button_mask);
    state_new.joy_int.APUG_JP_GLITCH3.state = state_new.joy_int.AGEM_JP_GLITCH2.state;
    state_new.joy_int.AGEM_JP_GLITCH2.state = state_new.joy_int.ACEF_JP_GLITCH1.state;
    state_new.joy_int.ACEF_JP_GLITCH1.state = state_new.joy_int.BATU_JP_GLITCH0.state;
    state_new.joy_int.BATU_JP_GLITCH0.state = !(sys.buttons & button_mask);
  }
  else if (!cpu_rd) {
    uint8_t button_mask = 0b00000000;
    if (!bit(state_new.reg_joy, 0)) button_mask |= 0b00001111;
    if (!bit(state_new.reg_joy, 1)) button_mask |= 0b11110000;
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

  auto& MATU_DMA_RUNNINGp_odd_old = state_old.MATU_DMA_RUNNINGp;

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

  auto& MATU_DMA_RUNNINGp_odd_new = state_new.MATU_DMA_RUNNINGp;

  const uint16_t dma_addr_new = ((state_new.reg_dma ^ 0xFF) << 8) | state_new.dma_lo;
  wire dma_addr_vram_new = (dma_addr_new >= 0x8000) && (dma_addr_new <= 0x9FFF);

  //----------------------------------------






























  //----------------------------------------
  // Sprite scanner

  int scan_counter_old = 39;
  uint8_t besu_scan_donen_odd_old = 0;
  uint8_t ceno_scan_donen_odd_old = 0;

  if (vid_rst_evn_old) {
    scan_counter_old = 0;
    besu_scan_donen_odd_old = 0;
    ceno_scan_donen_odd_old = 0;
  }
  else if (first_line_old) {
    if (phase_lx_old >= 6) scan_counter_old = (phase_lx_old - 6) / 4;
    besu_scan_donen_odd_old = 0;
    ceno_scan_donen_odd_old = 0;
  }
  else if (phase_ly_old >= 0 && phase_ly_old < 144) {
    if (phase_lx_old >= 2) scan_counter_old = (phase_lx_old - 2) / 4;
    besu_scan_donen_odd_old = phase_lx_old >= 2 && phase_lx_old < 162;
    ceno_scan_donen_odd_old = phase_lx_old >= 6 && phase_lx_old < 166;
  }
  else if (phase_ly_old >= 144 && phase_ly_old < 153) {
    scan_counter_old = 39;
    besu_scan_donen_odd_old = 0;
    ceno_scan_donen_odd_old = 0;
  }
  else if (phase_ly_old == 153) {
    if (phase_lx_old >= 6) scan_counter_old = (phase_lx_old - 6) / 4;
    besu_scan_donen_odd_old = phase_lx_old >= 6 && phase_lx_old < 166;
    ceno_scan_donen_odd_old = phase_lx_old >= 10 && phase_lx_old < 170;
  }






  uint8_t scan_counter_new = 39;
  uint8_t scan_done_trig_new = 0;
  uint8_t besu_scan_donen_odd_new = 0;
  uint8_t ceno_scan_donen_odd_new = 0;

  if (vid_rst_evn_new) {
    scan_counter_new = 0;
    scan_done_trig_new = 0;
    besu_scan_donen_odd_new = 0;
    ceno_scan_donen_odd_new = 0;
  }
  else if (first_line_new) {
    if (phase_lx_new >= 6) scan_counter_new = uint8_t((phase_lx_new - 6) / 4);
    scan_done_trig_new = phase_lx_new == 166;
    besu_scan_donen_odd_new = 0;
    ceno_scan_donen_odd_new = 0;
  }
  else if (phase_ly_new >= 0 && phase_ly_new < 144) {
    if (phase_lx_new >= 2) scan_counter_new = uint8_t((phase_lx_new - 2) / 4);
    scan_done_trig_new = phase_lx_new == 162;
    besu_scan_donen_odd_new = phase_lx_new >= 2 && phase_lx_new < 162;
    ceno_scan_donen_odd_new = phase_lx_new >= 6 && phase_lx_new < 166;
  }
  else if (phase_ly_new >= 144 && phase_ly_new < 153) {
    scan_counter_new = 39;
    scan_done_trig_new = 0;
    besu_scan_donen_odd_new = 0;
    ceno_scan_donen_odd_new = 0;
  }
  else if (phase_ly_new == 153) {
    if (phase_lx_new >= 6) scan_counter_new = uint8_t((phase_lx_new - 6) / 4);
    scan_done_trig_new = phase_lx_new == 166;
    besu_scan_donen_odd_new = phase_lx_new >= 6 && phase_lx_new < 166;
    ceno_scan_donen_odd_new = phase_lx_new >= 10 && phase_lx_new < 170;
  }

  if (scan_counter_new > 39) scan_counter_new = 39;





  //----------------------------------------

  if (vid_rst_evn_new) {
    state_new.sprite_counter = 0;
    state_new.sprite_store_flags = 0;
    memset(&state_new.store_x0, 0xFF, 10);
    state_new.sprite_scanner.DEZY_INC_COUNTn_odd.state = 0;
  }
  else if (line_rst_odd_new) {
    state_new.sprite_counter = 0;
    state_new.sprite_store_flags = 0;
    memset(&state_new.store_x0, 0xFF, 10);
  }
  else {
    int sy = (int)state_new.oam_temp_a - 16;
    int sprite_height = spr_size_new ? 8 : 16;
    wire sprite_hit = (reg_ly_new >= sy) && (reg_ly_new < sy + sprite_height) && ceno_scan_donen_odd_old;

    if (DELTA_HA_new || DELTA_DE_new) {
      if (!state_old.sprite_scanner.DEZY_INC_COUNTn_odd.state && state_new.sprite_counter < 10) {
        state_new.sprite_counter++;
      }
      state_new.sprite_scanner.DEZY_INC_COUNTn_odd.state = 1;
    }

    if (DELTA_BC_new || DELTA_FG_new) {
      if (sprite_hit) {
        state_new.sprite_scanner.DEZY_INC_COUNTn_odd.state = 0;
      }
    }

    if (DELTA_AB_new || DELTA_EF_new) {
      if (sprite_hit && state_new.sprite_counter < 10) {
        state_new.sprite_store_flags = (1 << state_new.sprite_counter);
        (&state_new.store_i0)[state_new.sprite_counter] = state_old.sprite_ibus ^ 0b111111;
        (&state_new.store_l0)[state_new.sprite_counter] = state_old.sprite_lbus ^ 0b1111;
      }
    }

    if (DELTA_CD_new || DELTA_GH_new) {
      if (state_new.sprite_store_flags && state_new.sprite_counter < 10) {
        (&state_new.store_x0)[state_new.sprite_counter] = state_new.oam_temp_b;
      }
      state_new.sprite_store_flags = 0;
    }
  }


  if (vid_rst_evn_new || line_rst_odd_new) {
    state_new.XYMU_RENDERINGn = 1;
  }
  else if (scan_done_trig_new) {
    state_new.XYMU_RENDERINGn = 0;
  }
  else if (DELTA_EVEN_new) {
    if (!state_old.FEPO_STORE_MATCHp_odd && (state_old.pix_count_odd == 167)) state_new.XYMU_RENDERINGn = 1;
  }



  //----------------------------------------
  // SOBU/SUDA

  if (DELTA_EVEN_new) {
    state_new.sfetch_control.SOBU_SFETCH_REQp_evn.state = state_old.FEPO_STORE_MATCHp_odd && !state_old.win_ctrl.RYDY_WIN_HITp_odd.state && (state_old.phase_tfetch >= 10) && !state_old.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.state;
  }
  else {
    state_new.sfetch_control.SUDA_SFETCH_REQp_odd.state = state_old.sfetch_control.SOBU_SFETCH_REQp_evn.state;
  }

  wire sfetch_trig_odd_old = state_old.sfetch_control.SOBU_SFETCH_REQp_evn.state && !state_old.sfetch_control.SUDA_SFETCH_REQp_odd.state;
  wire sfetch_trig_odd_new = state_new.sfetch_control.SOBU_SFETCH_REQp_evn.state && !state_new.sfetch_control.SUDA_SFETCH_REQp_odd.state;
  if (sfetch_trig_odd_old && !sfetch_trig_odd_new) CHECK_P(DELTA_ODD_new);

  //----------------------------------------

  if (vid_rst_evn_new) {
    state_new.phase_sfetch = 1;
  }
  else if (line_rst_odd_new || sfetch_trig_odd_new) {
    state_new.phase_sfetch = 0;
  }
  else {
    state_new.phase_sfetch++;
  }

  state_new.sfetch_counter_evn = state_new.phase_sfetch > 10 ? 5 : uint8_t(state_new.phase_sfetch / 2);

  //----------------------------------------
  // NUNU

  if (vid_rst_evn_new) {
    state_new.win_ctrl.NUNU_WIN_MATCHp_odd.state = 0;
  }
  else if (DELTA_ODD_new) {
    state_new.win_ctrl.NUNU_WIN_MATCHp_odd.state = state_old.win_ctrl.PYCO_WIN_MATCHp_evn.state;
  }

  //----------------------------------------
  // PYNU

  if (state_new.win_ctrl.NUNU_WIN_MATCHp_odd.state) state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state = 1;
  if (!win_en_new)  state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state = 0;
  if (vid_rst_evn_new)  state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state = 0;
  if (line_rst_odd_new) state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state = 0;

  //----------------------------------------
  // NOPA

  if (vid_rst_evn_new) {
    state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state = 0;
  }
  else if (DELTA_EVEN_new) {
    if (line_rst_odd_new && !state_new.XYMU_RENDERINGn) {
      state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state = 0;
    }
    else {
      state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state = state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state;
    }
  }

  wire NUNY_WIN_MODE_TRIGp_old = state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state && !state_old.win_ctrl.NOPA_WIN_MODE_Bp_evn.state;
  wire NUNY_WIN_MODE_TRIGp_new = state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state && !state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state;

  //----------------------------------------
  // NYKA / PORY
  // pory can be set on the phase after phase_tfetch gets reset to 0 by nyxu, so we can't use phase_tfetch to drive it?

  if (DELTA_EVEN_new) {
    state_new.tfetch_control.NYKA_FETCH_DONEp_evn.state = state_old.phase_tfetch >= 10;
  }
  if (vid_rst_evn_new || state_new.XYMU_RENDERINGn || line_rst_odd_new || NUNY_WIN_MODE_TRIGp_new) {
    state_new.tfetch_control.NYKA_FETCH_DONEp_evn.state = 0;
  }

  if (DELTA_ODD_new) {
    state_new.tfetch_control.PORY_FETCH_DONEp_odd.state = state_old.tfetch_control.NYKA_FETCH_DONEp_evn.state;
  }
  if (vid_rst_evn_new || state_new.XYMU_RENDERINGn || line_rst_odd_new || NUNY_WIN_MODE_TRIGp_new) {
    state_new.tfetch_control.PORY_FETCH_DONEp_odd.state = 0;
  }

  uint8_t fetch_done_old = (state_old.phase_tfetch >= 10) && (state_old.phase_tfetch <= 14);
  if (vid_rst_evn_old || state_old.XYMU_RENDERINGn || line_rst_odd_old || NUNY_WIN_MODE_TRIGp_old) fetch_done_old = 0;

  uint8_t fetch_done_new = (state_old.phase_tfetch >= 11) && (state_old.phase_tfetch <= 15);
  if (vid_rst_evn_new || state_new.XYMU_RENDERINGn || line_rst_odd_new || NUNY_WIN_MODE_TRIGp_new) fetch_done_new = 0;


  //----------------------------------------
  // POKY

  if (vid_rst_evn_new) {
    state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state = 0;
  }
  else if (state_new.XYMU_RENDERINGn) {
    state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state = 0;
  }
  else {
    if (DELTA_EVEN_new) {
      if (state_old.tfetch_control.PORY_FETCH_DONEp_odd.state) {
        //printf("%d\n", state_old.phase_tfetch);
        state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state = 1;
      }
    }
  }

  wire something_trig_old = !state_old.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && fetch_done_old;
  wire something_trig_new = !state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && fetch_done_new;

  //----------------------------------------
  // RYDY/SOVY

  if (vid_rst_evn_new) {
    state_new.win_ctrl.RYDY_WIN_HITp_odd.state = 0;
    state_new.win_ctrl.SOVY_WIN_HITp_evn.state = 0;
  }
  else {
    if (NUNY_WIN_MODE_TRIGp_new) state_new.win_ctrl.RYDY_WIN_HITp_odd.state = state_new.XYMU_RENDERINGn || !DELTA_EVEN_new || !line_rst_odd_new;
    if (state_new.tfetch_control.PORY_FETCH_DONEp_odd.state) state_new.win_ctrl.RYDY_WIN_HITp_odd.state = 0;
    if (DELTA_EVEN_new) state_new.win_ctrl.SOVY_WIN_HITp_evn.state = state_old.win_ctrl.RYDY_WIN_HITp_odd.state;
  }

  wire win_hit_trig_old  = state_old.win_ctrl.SOVY_WIN_HITp_evn.state && !state_old.win_ctrl.RYDY_WIN_HITp_odd.state;
  wire win_hit_trig_new  = state_new.win_ctrl.SOVY_WIN_HITp_evn.state && !state_new.win_ctrl.RYDY_WIN_HITp_odd.state;

  //----------------------------------------
  // CLKPIPE is an even clock, it can only go high on even deltas. FEPO/WODU/SOCY are odd signals, they stay constant during even deltas.
  // SO, it is guaranteed safe to use the old values of FEPO/WODU/SOCY to compute CLKPIPE

  wire clkpipe_gate = !state_old.win_ctrl.RYDY_WIN_HITp_odd.state && state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && !state_old.FEPO_STORE_MATCHp_odd && (state_old.pix_count_odd != 167);
  wire clkpipe_posedge_evn_new = DELTA_EVEN_new && clkpipe_gate;
  wire clkpipe_negedge_odd_new = DELTA_ODD_new && clkpipe_gate;

  //----------------------------------------
  // PYCO

  if (clkpipe_posedge_evn_new) state_new.win_ctrl.PYCO_WIN_MATCHp_evn.state = nuko_wx_match_old;
  if (vid_rst_evn_new) state_new.win_ctrl.PYCO_WIN_MATCHp_evn.state = 0;

  //----------------------------------------
  // WUTY

  uint8_t sfetch_done_trig_old = state_old.phase_sfetch == 11;
  if (vid_rst_evn_old || line_rst_odd_old || state_old.XYMU_RENDERINGn) sfetch_done_trig_old = 0;

  uint8_t sfetch_done_trig_new = state_new.phase_sfetch == 11;
  if (vid_rst_evn_new || line_rst_odd_new || state_new.XYMU_RENDERINGn) sfetch_done_trig_new = 0;

  //----------------------------------------
  // RYFA/RENE

  if (state_new.XYMU_RENDERINGn) {
    state_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.state = 0;
    state_new.win_ctrl.RENE_WIN_FETCHn_B_evn.state = 0;
  }
  else {
    if (clkpipe_negedge_odd_new) state_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.state = !nuko_wx_match_old && state_old.fine_count_odd == 7;
    if (DELTA_EVEN_new)  state_new.win_ctrl.RENE_WIN_FETCHn_B_evn.state = state_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.state; 
  }

  wire win_fetch_trig_old = state_old.win_ctrl.RYFA_WIN_FETCHn_A_evn.state && !state_old.win_ctrl.RENE_WIN_FETCHn_B_evn.state;
  wire win_fetch_trig_new = state_new.win_ctrl.RYFA_WIN_FETCHn_A_evn.state && !state_new.win_ctrl.RENE_WIN_FETCHn_B_evn.state;

  //----------------------------------------

  wire TEVO_WIN_FETCH_TRIGp_old = (win_fetch_trig_old || win_hit_trig_old || something_trig_old) && !state_old.XYMU_RENDERINGn;
  wire TEVO_WIN_FETCH_TRIGp_new = (win_fetch_trig_new || win_hit_trig_new || something_trig_new) && !state_new.XYMU_RENDERINGn;

  wire NYXU_BFETCH_RSTn_new = (line_rst_odd_new || vid_rst_evn_new || !scan_done_trig_new) && !NUNY_WIN_MODE_TRIGp_new && !TEVO_WIN_FETCH_TRIGp_new;

  if (state_new.XYMU_RENDERINGn) CHECK_P(NYXU_BFETCH_RSTn_new);

  //----------------------------------------
  // Win map x counter

  if (DELTA_ODD_new) {

    wire VETU_WIN_MAPp_old = TEVO_WIN_FETCH_TRIGp_old && state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state;
    wire VETU_WIN_MAPp_new = TEVO_WIN_FETCH_TRIGp_new && state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state;

    if (!VETU_WIN_MAPp_old && VETU_WIN_MAPp_new) {
      state_new.win_x.map++;
    }
  }

  if (vid_rst_evn_new || line_rst_odd_new || !win_en_new) {
    state_new.win_x.map = 0;
  }

  //----------------------------------------
  // Win map y counter - increments when we _leave_ window mode, fails fuzz test if we only update on odd
  
  if (state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state && !state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state) {
    uint8_t win_y_old = state_old.win_y.tile | (state_old.win_y.map << 3);
    uint8_t win_y_new = win_y_old + 1;

    state_new.win_y.tile = win_y_new & 0b111;
    state_new.win_y.map  = win_y_new >> 3;
  }

  if (vid_rst_evn_new || vblank_new) {
    state_new.win_y.tile = 0;
    state_new.win_y.map = 0;
  }

  //----------------------------------------
  // TAKA - only old used

  if (vid_rst_evn_new || line_rst_odd_new || sfetch_trig_odd_new) state_new.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.state = 1;
  if (sfetch_done_trig_new) state_new.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.state = 0;
  if (!state_new.XYMU_RENDERINGn && !state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && fetch_done_new) state_new.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.state = 0;









  //----------------------------------------
  // sprite_reset_flags <- old.sprite_match_flags

  if (!sfetch_done_trig_old && sfetch_done_trig_new) state_new.sprite_reset_flags = state_old.sprite_match_flags;
  if (vid_rst_evn_new || line_rst_odd_new) state_new.sprite_reset_flags = 0;

  //----------------------------------------
  // sprite reset

  int sprite_reset_index = 32 - __builtin_clz(state_new.sprite_reset_flags - 1);
  if (sprite_reset_index != 32) state_new.store_x[sprite_reset_index] = 0xFF;







  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  // There are three clocks combined into one here, and the logic does not untangle nicely. :P

  {
    wire scan_oam_clk_old = !MATU_DMA_RUNNINGp_odd_old && besu_scan_donen_odd_old && gen_clk(phase_old, 0b01110111);
    wire scan_oam_clk_new = !MATU_DMA_RUNNINGp_odd_new && besu_scan_donen_odd_new && gen_clk(phase_new, 0b01110111);

    wire ppu_oam_clk_old = (state_old.phase_sfetch == 1 || state_old.phase_sfetch == 2 || state_old.phase_sfetch == 3) && !state_old.XYMU_RENDERINGn;
    wire ppu_oam_clk_new = (state_new.phase_sfetch == 1 || state_new.phase_sfetch == 2 || state_new.phase_sfetch == 3) && !state_new.XYMU_RENDERINGn;

    wire dma_oam_clk_old = (cpu_addr_oam_old || MATU_DMA_RUNNINGp_odd_old) && gen_clk(phase_old, 0b00001111);
    wire dma_oam_clk_new = (cpu_addr_oam_new || MATU_DMA_RUNNINGp_odd_new) && gen_clk(phase_new, 0b00001111);

    wire oam_clk_old = scan_oam_clk_old || ppu_oam_clk_old || dma_oam_clk_old;
    wire oam_clk_new = scan_oam_clk_new || ppu_oam_clk_new || dma_oam_clk_new;

    if (oam_clk_old && !oam_clk_new) {
      state_new.oam_temp_a = ~state_old.oam_latch_a;
      state_new.oam_temp_b = ~state_old.oam_latch_b;
    }
  }

  //----------------------------------------
  // Fine match ROXY/NYZE/PUXA

  if (vid_rst_evn_new || line_rst_odd_new ||state_new.XYMU_RENDERINGn) {
    state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state = 1;
    state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state = 0;
    state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state = 0;
    state_new.fine_count_odd = 0;
  }
  else {
    if (clkpipe_negedge_odd_new && state_new.fine_count_odd < 7) state_new.fine_count_odd++;
    if (TEVO_WIN_FETCH_TRIGp_new) state_new.fine_count_odd = 0;

    if (clkpipe_posedge_evn_new) {
      wire fine_match = state_old.fine_count_odd == (~state_old.reg_scx & 0b111);
      state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state = state_old.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state && fine_match;
    }

    if (DELTA_ODD_new) {
      state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state = state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state;
    }


    if (state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state && !state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state) {
      state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state = 0;
    }
  }

  //----------------------------------------
  // pix count

  if (vid_rst_evn_new || line_rst_odd_new) {
    state_new.pix_count_odd = 0;
  }

  if (clkpipe_negedge_odd_new && !state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state) {
    state_new.pix_count_odd++;
  }

  //----------------------------------------
  // Window match

  if (vid_rst_evn_new) {
    state_new.win_ctrl.SARY_WY_MATCHp_odd.state = 0;
    state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state = 0;
    state_new.win_ctrl.NUKO_WX_MATCHp_odd.state = 0;
  }
  else {
    if (DELTA_BC_new) state_new.win_ctrl.SARY_WY_MATCHp_odd.state = (reg_ly_new == uint8_t(~state_new.reg_wy)) && win_en_new;

    if (state_new.win_ctrl.SARY_WY_MATCHp_odd.state) state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state = 1;
    if (vblank_new) state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state = 0;

    // old used
    state_new.win_ctrl.NUKO_WX_MATCHp_odd.state = (uint8_t(~state_new.reg_wx) == state_new.pix_count_odd) && state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state;
  }

  //----------------------------------------
  // FEPO

  if (MATU_DMA_RUNNINGp_odd_new) {
    state_new.oam_abus = (uint8_t)~state_new.dma_lo;
  }
  else if (besu_scan_donen_odd_new && !vid_rst_evn_new) {
    state_new.oam_abus = (scan_counter_new << 2) ^ 0xFF;
  }
  else if (!state_new.XYMU_RENDERINGn) {
    state_new.oam_abus = (uint8_t)~((state_new.sprite_ibus << 2) | 0b11);
  }
  else if (cpu_addr_oam_new) {
    state_new.oam_abus = uint8_t(~cpu_addr_new);
  }
  else {
    state_new.oam_abus = (uint8_t)~cpu_addr_new;
  }

  if (!vid_rst_evn_new) {
    if (DELTA_HA_new || DELTA_DE_new) {
      state_new.sprite_index = (state_old.oam_abus >> 2) ^ 0b111111;
    }
  }

  if (state_new.XYMU_RENDERINGn || ceno_scan_donen_odd_new) {
    state_new.FEPO_STORE_MATCHp_odd = 0;
    state_new.sprite_ibus = state_new.sprite_index;
    state_new.sprite_lbus = (~reg_ly_new + state_new.oam_temp_a) & 0b00001111;
    state_new.sprite_match_flags = 0;
  }
  else {
    state_new.FEPO_STORE_MATCHp_odd = 0;
    auto& s = state_new;
    auto& sf = s.sprite_match_flags;
    auto& si = s.sprite_ibus;
    auto& sl = s.sprite_lbus;

    sf = 0;
    si = 0x3F;
    sl = 0x0F;

    if (spr_en_new) {
      if      (state_new.pix_count_odd == s.store_x0) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x001;  si = s.store_i0 ^ 0x3F; sl = s.store_l0 ^ 0x0F; }
      else if (state_new.pix_count_odd == s.store_x1) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x002;  si = s.store_i1 ^ 0x3F; sl = s.store_l1 ^ 0x0F; }
      else if (state_new.pix_count_odd == s.store_x2) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x004;  si = s.store_i2 ^ 0x3F; sl = s.store_l2 ^ 0x0F; }
      else if (state_new.pix_count_odd == s.store_x3) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x008;  si = s.store_i3 ^ 0x3F; sl = s.store_l3 ^ 0x0F; }
      else if (state_new.pix_count_odd == s.store_x4) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x010;  si = s.store_i4 ^ 0x3F; sl = s.store_l4 ^ 0x0F; }
      else if (state_new.pix_count_odd == s.store_x5) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x020;  si = s.store_i5 ^ 0x3F; sl = s.store_l5 ^ 0x0F; }
      else if (state_new.pix_count_odd == s.store_x6) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x040;  si = s.store_i6 ^ 0x3F; sl = s.store_l6 ^ 0x0F; }
      else if (state_new.pix_count_odd == s.store_x7) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x080;  si = s.store_i7 ^ 0x3F; sl = s.store_l7 ^ 0x0F; }
      else if (state_new.pix_count_odd == s.store_x8) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x100;  si = s.store_i8 ^ 0x3F; sl = s.store_l8 ^ 0x0F; }
      else if (state_new.pix_count_odd == s.store_x9) { state_new.FEPO_STORE_MATCHp_odd = 1; sf = 0x200;  si = s.store_i9 ^ 0x3F; sl = s.store_l9 ^ 0x0F; }
    }

    if (!state_new.FEPO_STORE_MATCHp_odd) {
      state_new.sprite_lbus = (~reg_ly_new + state_new.oam_temp_a) & 0b00001111;
    }
  }

  //----------------------------------------
  // tfetch counter

  if (!NYXU_BFETCH_RSTn_new) {
    state_new.phase_tfetch = 0;
  }
  else {
    state_new.phase_tfetch++;
  }

  if (!state_old.XYMU_RENDERINGn) {
    if (state_old.phase_tfetch ==  2) state_new.tile_temp_b =  state_old.vram_dbus;
    if (state_old.phase_tfetch ==  6) state_new.tile_temp_a = ~state_old.vram_dbus;
    if (state_old.phase_tfetch == 10) state_new.tile_temp_b =  state_old.vram_dbus;
  }

  //----------------------------------------
  // Pixel pipes

  if (clkpipe_negedge_odd_new && !state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn_evn.state) {
    state_new.spr_pipe_a = (state_new.spr_pipe_a << 1) | 0;
    state_new.spr_pipe_b = (state_new.spr_pipe_b << 1) | 0;
    state_new.bgw_pipe_a = (state_new.bgw_pipe_a << 1) | 0;
    state_new.bgw_pipe_b = (state_new.bgw_pipe_b << 1) | 0;
    state_new.mask_pipe  = (state_new.mask_pipe  << 1) | 1;
    state_new.pal_pipe   = (state_new.pal_pipe   << 1) | 0;
  }

  uint8_t sprite_pix = state_old.vram_dbus;
  if (!state_new.XYMU_RENDERINGn) {

    if (bit(state_old.oam_temp_b, 5) && ((state_new.phase_sfetch >= 4) && (state_new.phase_sfetch < 12) && !vid_rst_evn_old && !state_old.XYMU_RENDERINGn)) {
      sprite_pix = bit_reverse(state_old.vram_dbus);
    }

    if (state_new.phase_sfetch == 6) {
      state_new.sprite_pix_a = ~sprite_pix;
    }

    if (state_new.phase_sfetch == 10) {
      state_new.sprite_pix_b = ~sprite_pix;
    }
  }

  if (sfetch_done_trig_new) {
    uint8_t sprite_mask = state_new.spr_pipe_b | state_new.spr_pipe_a;
    state_new.spr_pipe_a = (state_new.spr_pipe_a & sprite_mask) | (~state_new.sprite_pix_a & ~sprite_mask);
    state_new.spr_pipe_b = (state_new.spr_pipe_b & sprite_mask) | (~state_new.sprite_pix_b & ~sprite_mask);
    state_new.mask_pipe  = bit(state_new.oam_temp_b, 7) ? state_new.mask_pipe | ~sprite_mask : state_new.mask_pipe & sprite_mask;
    state_new.pal_pipe   = bit(state_new.oam_temp_b, 4) ? state_new.pal_pipe  | ~sprite_mask : state_new.pal_pipe  & sprite_mask;
  }

  if (!NYXU_BFETCH_RSTn_new) {
    state_new.bgw_pipe_a = ~state_new.tile_temp_a;
    state_new.bgw_pipe_b =  state_new.tile_temp_b;
  }

  uint8_t bgw_pix_a = bit(state_new.bgw_pipe_a, 7) && bgw_en_new;
  uint8_t bgw_pix_b = bit(state_new.bgw_pipe_b, 7) && bgw_en_new;
  uint8_t spr_pix_a = bit(state_new.spr_pipe_a, 7) && spr_en_new;
  uint8_t spr_pix_b = bit(state_new.spr_pipe_b, 7) && spr_en_new;
  uint8_t mask_pix  = bit(state_new.mask_pipe,  7);
  uint8_t pal_pix   = bit(state_new.pal_pipe,   7);

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

  //----------------------------------------
  // Audio

  //tock_spu_logic();

  //----------------------------------------
  // Memory buses

  // The differing prioritization here seems like a bug

  uint8_t pins_ctrl_csn_new = 0;
  if (ext_addr_new && cpu_addr_ram_new)             pins_ctrl_csn_new = gen_clk(phase_new, 0b00111111);
  if (MATU_DMA_RUNNINGp_odd_new && !dma_addr_vram_new)  pins_ctrl_csn_new = !!(dma_addr_new & 0x8000);

  //----------------------------------------

  if (ext_addr_new && !cpu_addr_vram_new) {
    state_new.ext_addr_latch = cpu_addr_new & 0x7FFF;
  }

  uint16_t cart_addr;

  if (MATU_DMA_RUNNINGp_odd_new && !dma_addr_vram_new) {
    cart_addr = dma_addr_new;
  }
  else if ((state_new.cpu_signals.TEPU_BOOT_BITn.state || cpu_addr_new > 0x00FF) && gen_clk(phase_new, 0b00111111) && ext_addr_new) {
    cart_addr = state_new.ext_addr_latch | (cpu_addr_new & 0x8000);
  }
  else {
    cart_addr = state_new.ext_addr_latch | 0x8000;
  }

  const int cart_octant = cart_addr >> 13;

  //----------------------------------------

  uint8_t cart_dbus = 0xFF;
  if (cpu_wr && !req_addr_hi && !cpu_addr_bootrom_new && !cpu_addr_vram_new) {
    cart_dbus = state_new.cpu_dbus;
  }

  //----------------------------------------
  // Ext read

  uint8_t pins_ctrl_rdn_new = 0;
  if (MATU_DMA_RUNNINGp_odd_new && dma_addr_vram_new)   pins_ctrl_rdn_new = 1;
  if (!ext_addr_new || !cpu_wr)                     pins_ctrl_rdn_new = 1;
  if (cpu_addr_vram_new)                            pins_ctrl_rdn_new = 1;

  if (pins_ctrl_rdn_new) {

    const auto rom_addr_mask = cart_rom_addr_mask(cart_blob);
    const auto ram_addr_mask = cart_ram_addr_mask(cart_blob);

    uint8_t data_in = 0x00;

    bool ext_read_en = false;

    if (cart_has_mbc1(cart_blob)) {

      wire mbc1_ram_en = state_new.ext_mbc.MBC1_RAM_EN.state;
      wire mbc1_mode = state_new.ext_mbc.MBC1_MODE.state;

      const uint32_t mbc1_rom0_bank = mbc1_mode ? bit_pack(&state_new.ext_mbc.MBC1_BANK5, 2) : 0;
      const uint32_t mbc1_rom0_addr = ((cart_addr & 0x3FFF) | (mbc1_rom0_bank << 19)) & rom_addr_mask;

      uint32_t mbc1_rom1_bank = bit_pack(&state_new.ext_mbc.MBC1_BANK0, 7);
      if ((mbc1_rom1_bank & 0x1F) == 0) mbc1_rom1_bank |= 1;
      const uint32_t mbc1_rom1_addr = ((cart_addr & 0x3FFF) | (mbc1_rom1_bank << 14)) & rom_addr_mask;

      const uint32_t mbc1_ram_bank = mbc1_mode ? bit_pack(&state_new.ext_mbc.MBC1_BANK5, 2) : 0;
      const uint32_t mbc1_ram_addr = ((cart_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & ram_addr_mask;

      switch (cart_octant) {
      case 0: ext_read_en = true; data_in = cart_blob[mbc1_rom0_addr]; break;
      case 1: ext_read_en = true; data_in = cart_blob[mbc1_rom0_addr]; break;
      case 2: ext_read_en = true; data_in = cart_blob[mbc1_rom1_addr]; break;
      case 3: ext_read_en = true; data_in = cart_blob[mbc1_rom1_addr]; break;
      case 4: data_in = 0x00; break;
      case 5: ext_read_en = true; data_in = mbc1_ram_en ? mem.cart_ram[mbc1_ram_addr] : 0xFF; break;
      case 6: ext_read_en = true; data_in = mem.int_ram[cart_addr & 0x1FFF]; break;
      case 7: ext_read_en = true; data_in = mem.int_ram[cart_addr & 0x1FFF]; break;
      }
    }
    else {
      switch (cart_octant) {
      case 0: ext_read_en = true; data_in = cart_blob[cart_addr & rom_addr_mask]; break;
      case 1: ext_read_en = true; data_in = cart_blob[cart_addr & rom_addr_mask]; break;
      case 2: ext_read_en = true; data_in = cart_blob[cart_addr & rom_addr_mask]; break;
      case 3: ext_read_en = true; data_in = cart_blob[cart_addr & rom_addr_mask]; break;
      case 4: data_in = 0x00; break;
      case 5: data_in = 0x00; break;
      case 6: ext_read_en = true; data_in = mem.int_ram[cart_addr & 0x1FFF]; break;
      case 7: ext_read_en = true; data_in = mem.int_ram[cart_addr & 0x1FFF]; break;
      }
    }

    if (ext_read_en) {
      cart_dbus = data_in;
    }
  }

  //----------------------------------------
  // Ext write

  uint8_t pins_ctrl_wrn_new = 0;
  if (ext_addr_new && cpu_wr && !cpu_addr_vram_new) pins_ctrl_wrn_new = gen_clk(phase_new, 0b00001110);
  if (MATU_DMA_RUNNINGp_odd_new && dma_addr_vram_new)   pins_ctrl_wrn_new = 0;

  {
    const uint8_t data_out = cart_dbus;
    wire mbc1_ram_en = state_new.ext_mbc.MBC1_RAM_EN.state;
    wire mbc1_mode = state_new.ext_mbc.MBC1_MODE.state;

    const auto mbc1_ram_bank = mbc1_mode ? bit_pack(&state_new.ext_mbc.MBC1_BANK5, 2) : 0;
    const auto mbc1_ram_addr = ((cart_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & cart_ram_addr_mask(cart_blob);

    if (pins_ctrl_wrn_new && cart_has_mbc1(cart_blob)) {
      switch (cart_octant) {
      case 0: state_new.ext_mbc.MBC1_RAM_EN.state = (data_out & 0x0F) == 0x0A; break;
      case 1: bit_unpack(&state_new.ext_mbc.MBC1_BANK0, 5, data_out); break;
      case 2: bit_unpack(&state_new.ext_mbc.MBC1_BANK5, 2, data_out); break;
      case 3: state_new.ext_mbc.MBC1_MODE.state = (data_out & 1); break;
      case 4: break;
      case 5: if (cart_has_ram(cart_blob) && mbc1_ram_en) mem.cart_ram[mbc1_ram_addr & cart_ram_addr_mask(cart_blob)] = (uint8_t)data_out; break;
      case 6: mem.int_ram[cart_addr & 0x1FFF] = (uint8_t)data_out; break;
      case 7: mem.int_ram[cart_addr & 0x1FFF] = (uint8_t)data_out; break;
      }
    }

    if (pins_ctrl_wrn_new && !cart_has_mbc1(cart_blob)) {
      switch (cart_octant) {
      case 0: break;
      case 1: break;
      case 2: break;
      case 3: break;
      case 4: break;
      case 5: if (cart_has_ram(cart_blob)) mem.cart_ram[cart_addr & cart_ram_addr_mask(cart_blob)] = (uint8_t)data_out; break;
      case 6: mem.int_ram[cart_addr & 0x1FFF] = (uint8_t)data_out;break;
      case 7: mem.int_ram[cart_addr & 0x1FFF] = (uint8_t)data_out;break;
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
     DELTA_DH_new)
  {
    state_new.cpu_dbus = ~state_new.ext_data_latch;
  }
  else {
    state_new.ext_data_latch = ~cart_dbus;
  }

  //--------------------------------------------
  // vram_abus driver

  if (((state_new.phase_sfetch >= 4) && (state_new.phase_sfetch < 12) && !vid_rst_evn_old && !state_old.XYMU_RENDERINGn)) {

    // Sprite read address

    wire hilo = state_new.phase_sfetch >= 8;

    const auto line = state_new.sprite_lbus ^ (bit(state_new.oam_temp_b, 6) ? 0b0000 : 0b1111);
    const auto tile = state_new.oam_temp_a;

    if (spr_size_new) {
      bit_cat(state_new.vram_abus,  0,  0, hilo);
      bit_cat(state_new.vram_abus,  1,  3, line);
      bit_cat(state_new.vram_abus,  4, 11, tile);
      bit_cat(state_new.vram_abus, 12, 12, 0);
    }
    else {
      bit_cat(state_new.vram_abus,  0,  0, hilo);
      bit_cat(state_new.vram_abus,  1,  4, line);
      bit_cat(state_new.vram_abus,  5, 11, tile >> 1);
      bit_cat(state_new.vram_abus, 12, 12, 0);
    }
  }
  else if (!state_new.XYMU_RENDERINGn && state_new.phase_tfetch < 12) {

    // BG map read address

    const auto px  = state_new.pix_count_odd;
    const auto scx = ~state_new.reg_scx;
    const auto scy = ~state_new.reg_scy;

    const auto sum_x = px + scx;
    const auto sum_y = reg_ly_new + scy;

    if (state_new.phase_tfetch >= 4 && state_new.phase_tfetch <= 11) {
      const auto hilo = state_new.phase_tfetch >= 8;
      const auto tile_y = (state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state ? state_new.win_y.tile : (sum_y & 0b111));
      const auto map_y = state_new.tile_temp_b;
      const auto map = !bit(state_new.tile_temp_b, 7) && bgw_tile_new;

      bit_cat(state_new.vram_abus,  0,  0, hilo);
      bit_cat(state_new.vram_abus,  1,  3, tile_y);
      bit_cat(state_new.vram_abus,  4, 11, map_y);
      bit_cat(state_new.vram_abus, 12, 12, map);
    }
    else if (state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state) {
      bit_cat(state_new.vram_abus,  0,  4, state_new.win_x.map);
      bit_cat(state_new.vram_abus,  5,  9, state_new.win_y.map);
      bit_cat(state_new.vram_abus, 10, 10, ~win_map_new);
      bit_cat(state_new.vram_abus, 11, 12, 0b11);
    }
    else {
      bit_cat(state_new.vram_abus,  0,  4, (px + scx) >> 3);
      bit_cat(state_new.vram_abus,  5,  9, (reg_ly_new + scy) >> 3);
      bit_cat(state_new.vram_abus, 10, 10, !bg_map_new);
      bit_cat(state_new.vram_abus, 11, 11, 1);
      bit_cat(state_new.vram_abus, 12, 12, 1);
    }
  }
  else if (MATU_DMA_RUNNINGp_odd_new && dma_addr_vram_new) {
    // DMA vram read address
    state_new.vram_abus = dma_addr_new;
  }
  else if (state_new.XYMU_RENDERINGn) {
    // CPU vram read address
    state_new.vram_abus = cpu_addr_new;
  }
  else {
    state_new.vram_abus = 0;
  }

  state_new.vram_abus = ~state_new.vram_abus & VRAM_ADDR_MASK;


  //--------------------------------------------
  // Vram address pin driver

  state_new.vram_dbus = 0xFF;

  uint8_t pins_vram_dbus = 0xFF;

  uint8_t pins_vram_ctrl_csn_new = 1;
  uint8_t pins_vram_ctrl_oen_new = 1;
  uint8_t pins_vram_ctrl_wrn_new = 1;

  if (MATU_DMA_RUNNINGp_odd_new && dma_addr_vram_new) {
    pins_vram_ctrl_csn_new = 1;
    pins_vram_ctrl_oen_new = 1;
  }
  else if (!state_new.XYMU_RENDERINGn) {
    
    if (vid_rst_evn_old) {
      pins_vram_ctrl_csn_new = 0;
      pins_vram_ctrl_oen_new = state_new.phase_tfetch < 12;
    }
    else {
      
      pins_vram_ctrl_csn_new = 0;
      if (state_new.phase_tfetch < 12) {
        pins_vram_ctrl_csn_new = 1;
      }
      else if (state_new.phase_sfetch >= 4 && state_new.phase_sfetch < 12) {
        pins_vram_ctrl_csn_new = 1;
      }


      if (state_new.phase_tfetch < 12) {
        pins_vram_ctrl_oen_new = 1;
      }
      else {

        pins_vram_ctrl_oen_new = 0;
        if (state_new.phase_sfetch >= 5 && state_new.phase_sfetch <=  7) pins_vram_ctrl_oen_new = 1;
        if (state_new.phase_sfetch >= 9 && state_new.phase_sfetch <= 11) pins_vram_ctrl_oen_new = 1;
      }

    }

  }
  else if (ext_addr_new) {
    pins_vram_ctrl_csn_new = (cpu_addr_vram_new && gen_clk(phase_new, 0b00111111) && 1);
    pins_vram_ctrl_oen_new = !cpu_addr_vram_new || !cpu.core.reg.bus_req_new.write || DELTA_HA_new;
  }
  else {
    pins_vram_ctrl_csn_new = 0;
    pins_vram_ctrl_oen_new = !cpu_addr_vram_new || !cpu.core.reg.bus_req_new.write || DELTA_HA_new;;
  }


  if (!state_new.XYMU_RENDERINGn) {
    pins_vram_ctrl_wrn_new = 0;
  }
  else if (ext_addr_new) {
    pins_vram_ctrl_wrn_new = cpu_addr_vram_new && gen_clk(phase_new, 0b00001110) && cpu_wr && 1;
  }
  else {
    pins_vram_ctrl_wrn_new = cpu_addr_vram_new && gen_clk(phase_new, 0b00001110) && cpu_wr && 0;
  }


  if (!state_new.XYMU_RENDERINGn) {
    if (pins_vram_ctrl_oen_new) {
      state_new.vram_dbus = mem.vid_ram[state_new.vram_abus ^ 0x1FFF];
    }
    pins_vram_dbus = ~state_new.vram_dbus;
  }
  else if (ext_addr_new) {
    if (gen_clk(phase_new, 0b00111111) && cpu_addr_vram_new && cpu_wr) {
      state_new.vram_dbus = state_new.cpu_dbus;
    }

    uint8_t vdata = 0xFF;

    if (pins_vram_ctrl_oen_new) vdata = mem.vid_ram[state_new.vram_abus ^ 0x1FFF];

    pins_vram_dbus = pins_vram_ctrl_oen_new ? ~vdata : 0;

    if (cpu_addr_vram_new && gen_clk(phase_new, 0b00111111) && cpu_wr) {
      pins_vram_dbus = ~state_new.vram_dbus;
    }
    else {
      state_new.vram_dbus = ~pins_vram_dbus;
    }

    if (pins_vram_ctrl_wrn_new) mem.vid_ram[state_new.vram_abus ^ 0x1FFF] = ~pins_vram_dbus;

    if (cpu_addr_vram_new && gen_clk(phase_new, 0b00111111) && (cpu.core.reg.bus_req_new.read && !DELTA_HA_new) && (DELTA_DH_new && cpu.core.reg.bus_req_new.read)) {
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

  if ((cpu.core.reg.bus_req_new.read && !DELTA_HA_new)) {
    if (cpu_addr_new == 0xFF00) {
      set_bit(state_new.cpu_dbus, 0, !bit(state_new.joy_latch, 0));
      set_bit(state_new.cpu_dbus, 1, !bit(state_new.joy_latch, 1));
      set_bit(state_new.cpu_dbus, 2, !bit(state_new.joy_latch, 2));
      set_bit(state_new.cpu_dbus, 3, !bit(state_new.joy_latch, 3));
      set_bit(state_new.cpu_dbus, 4,  bit(state_new.reg_joy, 0));
      set_bit(state_new.cpu_dbus, 5,  bit(state_new.reg_joy, 1));
    }
    if (cpu_addr_new == 0xFF04) state_new.cpu_dbus =  (uint8_t)(state_new.reg_div >> 6);
    if (cpu_addr_new == 0xFF05) state_new.cpu_dbus =  state_new.reg_tima;
    if (cpu_addr_new == 0xFF06) state_new.cpu_dbus =  state_new.reg_tma;
    if (cpu_addr_new == 0xFF07) state_new.cpu_dbus =  (state_new.reg_tac & 0b111) | 0b11111000;
    if (cpu_addr_new == 0xFF40) state_new.cpu_dbus = ~state_new.reg_lcdc;
    if (cpu_addr_new == 0xFF42) state_new.cpu_dbus = ~state_new.reg_scy;
    if (cpu_addr_new == 0xFF43) state_new.cpu_dbus = ~state_new.reg_scx;
    if (cpu_addr_new == 0xFF44) state_new.cpu_dbus =  reg_ly_new;
    if (cpu_addr_new == 0xFF45) state_new.cpu_dbus = ~state_new.reg_lyc;
    if (cpu_addr_new == 0xFF47) state_new.cpu_dbus = ~state_new.reg_bgp;
    if (cpu_addr_new == 0xFF48) state_new.cpu_dbus = ~state_new.reg_obp0;
    if (cpu_addr_new == 0xFF49) state_new.cpu_dbus = ~state_new.reg_obp1;
    if (cpu_addr_new == 0xFF4A) state_new.cpu_dbus = ~state_new.reg_wy;
    if (cpu_addr_new == 0xFF4B) state_new.cpu_dbus = ~state_new.reg_wx;
  }

  //----------------------------------------
  // oam

  // OAM bus has to be pulled up or we fail regression
  uint8_t oam_dbus_a = 0xFF;
  uint8_t oam_dbus_b = 0xFF;

  if (MATU_DMA_RUNNINGp_odd_new) {
    if (DELTA_DH_new) mem.oam_ram[state_new.dma_lo] = dma_addr_vram_new ? state_new.vram_dbus : cart_dbus;
    oam_dbus_a = dma_addr_vram_new ? ~state_new.vram_dbus : ~cart_dbus;
    oam_dbus_b = dma_addr_vram_new ? ~state_new.vram_dbus : ~cart_dbus;
  }
  else if (besu_scan_donen_odd_new && !vid_rst_evn_new) {
    // Scanning
    if (DELTA_HA_new) {
    }
    if (DELTA_AB_new) {
      state_new.oam_latch_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      state_new.oam_latch_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_BC_new) {
      state_new.oam_latch_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      state_new.oam_latch_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_CD_new) {
    }
    if (DELTA_DE_new) {
    }
    if (DELTA_EF_new) {
      state_new.oam_latch_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      state_new.oam_latch_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_FG_new) {
      state_new.oam_latch_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      state_new.oam_latch_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_GH_new) {
    }

    if (DELTA_HA_new) {
    }
    if (DELTA_AB_new) {
      oam_dbus_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      oam_dbus_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_BC_new) {
      oam_dbus_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      oam_dbus_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_CD_new) {
      if (cpu_addr_oam_new && cpu.core.reg.bus_req_new.read) {
        oam_dbus_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
        oam_dbus_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
      }
    }
    if (DELTA_DE_new) {
    }
    if (DELTA_EF_new) {
      oam_dbus_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      oam_dbus_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_FG_new) {
      oam_dbus_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      oam_dbus_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_GH_new) {
    }
  }
  else if (!state_new.XYMU_RENDERINGn) {
    // Rendering

    uint8_t sfetch_oam_oen_new =  !(state_new.phase_sfetch == 0 || state_new.phase_sfetch == 3);
    uint8_t sfetch_oam_clk_new =  !(state_new.phase_sfetch >= 1 && state_new.phase_sfetch <= 3);

    if (!sfetch_oam_oen_new) {
      oam_dbus_a = ~mem.oam_ram[(((state_new.sprite_ibus << 2) | 0b11)) & ~1];
      oam_dbus_b = ~mem.oam_ram[(((state_new.sprite_ibus << 2) | 0b11)) |  1];
    }
    else if (cpu_addr_oam_new && (!cpu.core.reg.bus_req_new.read || DELTA_AD_new)) {
      oam_dbus_a = ~mem.oam_ram[(((state_new.sprite_ibus << 2) | 0b11)) & ~1];
      oam_dbus_b = ~mem.oam_ram[(((state_new.sprite_ibus << 2) | 0b11)) |  1];
    }


    if (state_new.phase_sfetch == 0 || state_new.phase_sfetch == 3) {
      state_new.oam_latch_a = oam_dbus_a;
      state_new.oam_latch_b = oam_dbus_b;
    }
  }
  else if (cpu_addr_oam_new) {
    // CPUing

    if (DELTA_HD_new) {
      if (cpu.core.reg.bus_req_new.read && !DELTA_HA_new) {
        oam_dbus_a = ~mem.oam_ram[(cpu_addr_new & 0xFF) & ~1];
        oam_dbus_b = ~mem.oam_ram[(cpu_addr_new & 0xFF) |  1];
        state_new.oam_latch_a = ~mem.oam_ram[(cpu_addr_new & 0xFF) & ~1];
        state_new.oam_latch_b = ~mem.oam_ram[(cpu_addr_new & 0xFF) |  1];
      }
    }
    else if (DELTA_DH_new) {
      if (cpu.core.reg.bus_req_new.read) {
        state_new.cpu_dbus = !bit(cpu_addr_new, 0) ? ~state_old.oam_latch_a : ~state_old.oam_latch_b;
      }

      if (cpu_wr && DELTA_DE_new) {
        mem.oam_ram[cpu_addr_new & 0xFF] = state_new.cpu_dbus;
      }

      if (cpu_wr) {
        oam_dbus_a = ~state_new.cpu_dbus;
        oam_dbus_b = ~state_new.cpu_dbus;
      }
    }
  }
  else {
    // Idle
    oam_dbus_a = ~state_new.cpu_dbus;
    oam_dbus_b = ~state_new.cpu_dbus;
  }

  //----------------------------------------
  // zram

  if ((cpu.core.reg.bus_req_new.read && !DELTA_HA_new)) {
    if ((cpu_addr_new >= 0xFF80) && (cpu_addr_new <= 0xFFFE)) state_new.cpu_dbus = mem.zero_ram[cpu_addr_new & 0x007F];
  }

  //----------------------------------------
  // And finally, interrupts.

  auto pack_cpu_dbus_old = state_old.cpu_dbus;
  auto pack_cpu_dbus_new = state_new.cpu_dbus;
  auto pack_ie = state_new.reg_ie;
  auto pack_if = state_new.reg_if;
  auto pack_stat = state_new.reg_stat;

  if (!vid_rst_evn_new && DELTA_BC_new) {
    state_new.int_ctrl.ROPO_LY_MATCH_SYNCp.state = reg_ly_old == (state_old.reg_lyc ^ 0xFF); // must be reg_ly_old
  }

  // FIXME this seems slightly wrong...
  if (cpu_wr && gen_clk(phase_new, 0b00001110) && cpu_addr_new == 0xFF41) {
  }
  else {
    state_new.int_ctrl.RUPO_LYC_MATCHn.state = 1;
  }

  // but the "reset" arm of the latch overrides the "set" arm, so it doesn't completely break?
  if (state_new.int_ctrl.ROPO_LY_MATCH_SYNCp.state) {
    state_new.int_ctrl.RUPO_LYC_MATCHn.state = 0;
  }

  if (cpu_addr_new == 0xFFFF && cpu_wr && gen_clk(phase_new, 0b00000001)) {
    pack_ie = pack_cpu_dbus_old;
  }

  if (cpu_addr_new == 0xFF41 && cpu_wr && gen_clk(phase_new, 0b00000001)) {
    pack_stat = (~pack_cpu_dbus_old >> 3) & 0b00001111;
  }

  if (cpu_addr_new == 0xFF41 && (cpu.core.reg.bus_req_new.read && !DELTA_HA_new)) {
    uint8_t stat = 0x80;

    stat |= (!state_new.XYMU_RENDERINGn || vblank_new) << 0;
    stat |= (!state_new.XYMU_RENDERINGn || (!MATU_DMA_RUNNINGp_odd_new && besu_scan_donen_odd_new && !vid_rst_evn_new)) << 1;
    stat |= (!state_new.int_ctrl.RUPO_LYC_MATCHn.state) << 2;
    stat |= (pack_stat ^ 0b1111) << 3;

    pack_cpu_dbus_new = stat;
  }

  state_new.lcd.RUTU_LINE_ENDp_odd.state = !first_line_new && (phase_lx_new >= 0) && (phase_lx_new <= 7);

  bool int_stat_old = 0;
  if (!bit(state_old.reg_stat, 0) && (!state_old.FEPO_STORE_MATCHp_odd && (state_old.pix_count_odd == 167)) && !vblank_old) int_stat_old = 1;
  if (!bit(state_old.reg_stat, 1) && vblank_old) int_stat_old = 1;
  if (!bit(state_old.reg_stat, 2) && !vblank_old && state_old.lcd.RUTU_LINE_ENDp_odd.state) int_stat_old = 1;
  if (!bit(state_old.reg_stat, 3) && state_old.int_ctrl.ROPO_LY_MATCH_SYNCp.state) int_stat_old = 1;

  wire int_lcd_old = vblank_old;
  wire int_joy_old = !state_old.joy_int.APUG_JP_GLITCH3.state || !state_old.joy_int.BATU_JP_GLITCH0.state;
  wire int_tim_old = state_old.int_ctrl.MOBA_TIMER_OVERFLOWp.state;
  //wire int_ser_old = serial.CALY_SER_CNT3;
  wire int_ser_old = 0;

  bool hblank_new = !state_new.FEPO_STORE_MATCHp_odd && (state_new.pix_count_odd & 167) == 167; // FEPO _must_ be new or we get a mismatch

  bool int_stat_new = 0;
  if (!bit(pack_stat, 0) && hblank_new && !vblank_new) int_stat_new = 1;
  if (!bit(pack_stat, 1) && vblank_new) int_stat_new = 1;
  if (!bit(pack_stat, 2) && !vblank_new && state_new.lcd.RUTU_LINE_ENDp_odd.state) int_stat_new = 1;
  if (!bit(pack_stat, 3) && state_new.int_ctrl.ROPO_LY_MATCH_SYNCp.state) int_stat_new = 1;

  const wire int_lcd_new = vblank_new;
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
  if (cpu_wr && gen_clk(phase_new, 0b00001110)) {
    if (cpu_addr_new == 0xFF0F) pack_if = pack_cpu_dbus_new;
  }

  pack_if &= ~cpu.core.get_int_ack();

  if ((cpu.core.reg.bus_req_new.read && !DELTA_HA_new)) {
    if (cpu_addr_new == 0xFFFF) pack_cpu_dbus_new = pack_ie | 0b11100000;
    if (cpu_addr_new == 0xFF0F) state_new.int_latch = (uint8_t)pack_if;
    if (cpu_addr_new == 0xFF0F) pack_cpu_dbus_new = pack_if | 0b11100000;
  }


  state_new.cpu_dbus = (uint8_t)pack_cpu_dbus_new;
  state_new.cpu_int = (uint8_t)pack_if;
  state_new.reg_ie = (uint8_t)pack_ie;
  state_new.reg_if = (uint8_t)pack_if;
  state_new.reg_stat = (uint8_t)pack_stat;




  int lcd_x = state_new.pix_count_odd - 8;
  int lcd_y = reg_ly_new;

  if (lcd_y >= 0 && lcd_y < 144 && lcd_x >= 0 && lcd_x < 160) {
    wire p0 = !REMY_LD0n;
    wire p1 = !RAVO_LD1n;
    auto new_pix = p0 + p1 * 2;

    mem.framebuffer[lcd_x + lcd_y * 160] = uint8_t(3 - new_pix);
  }
















  //===========================================================================
  //===========================================================================
  // CPU update



  cpu.core.reg.cpu_data_latch &= (uint8_t)state_new.cpu_dbus;

  if (DELTA_AB_new) {
  }

  if (DELTA_BC_new) {
    // -AB +BC +CD -DE
    if (bit(state_new.reg_if, 0)) cpu.core.reg.halt_latch |= INT_VBLANK_MASK;
    if (bit(state_new.reg_if, 1)) cpu.core.reg.halt_latch |= INT_STAT_MASK;
    if (bit(state_new.reg_if, 3)) cpu.core.reg.halt_latch |= INT_SERIAL_MASK;
    if (bit(state_new.reg_if, 4)) cpu.core.reg.halt_latch |= INT_JOYPAD_MASK;
    if (cpu.core.reg.op_next == 0x76 && (bit_pack(state_new.reg_ie) & cpu.core.reg.halt_latch)) cpu.core.reg.op_state = 0; // +BC +CD +DE +EF +FG
    cpu.core.reg.halt_latch = 0; // +BC +CD +DE +EF +FG
  }

  if (DELTA_CD_new) {
  }

  if (DELTA_DE_new) {
    // -CD +DE +EF +FG
    if (cpu.core.reg.op_state == 0) {
      cpu.core.reg.op_addr = cpu.core.reg.bus_addr;
      cpu.core.reg.op_next = (uint8_t)state_new.cpu_dbus;
    }
  }

  if (DELTA_EF_new) {
    cpu.core.reg.cpu_data_latch = 0xFF; // -DE +EF
  }

  if (DELTA_FG_new) {

    cpu.core.reg.intf_latch = state_new.reg_if; // -EF +FG +GH -HA
    if (bit(state_new.reg_if, 2)) cpu.core.reg.halt_latch |= INT_TIMER_MASK; // +FG +GH -HA : this one latches funny, some hardware bug    


    if (sys.cpu_en) {
      if (cpu.core.reg.op_state == 0) {
        if ((state_new.reg_ie & cpu.core.reg.intf_latch) && cpu.core.reg.ime) {
          cpu.core.reg.op_next = 0xF4; // fake opcode
          cpu.core.reg.ime = false;
          cpu.core.reg.ime_delay = false;
        }
      }
      cpu.core.reg.int_ack = 0;
      cpu.core.reg.ime = cpu.core.reg.ime_delay; // must be after int check, before op execution
    }
  }

  if (DELTA_GH_new) {
  }

  if (DELTA_HA_new) {
    if (cpu.core.reg.bus_read) cpu.core.reg.data_in = cpu.core.reg.cpu_data_latch; // -FG +GH +HA -AB


    if (sys.cpu_en) {
      cpu.core.execute(state_new.reg_ie, cpu.core.reg.intf_latch);
      cpu.core.reg.bus_req_new = cpu.core.get_bus_req();
    }
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
    if (DELTA_ODD_new) {
      state_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state = state_old.phase_sfetch > 10 ? bit(5, 0) : bit(state_old.phase_sfetch / 2, 0);
    }

    // this is weird, why is it always 0 when not in reset?
    state_new.oam_ctrl.MAKA_LATCH_EXTp.state = 0;



    //----------
    // OAM bus and control signals.
    // The inclusion of cpu_addr_oam_new in the SCANNING and RENDERING branches is probably a hardware bug.

    if (MATU_DMA_RUNNINGp_odd_new) {
      state_new.oam_abus = (uint8_t)~state_new.dma_lo;
      if (DELTA_HD_new) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 1;
        state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
        state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
      }
      else {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_WRn_A.state =  bit(state_new.dma_lo, 0);
        state_new.oam_ctrl.SIG_OAM_WRn_B.state = !bit(state_new.dma_lo, 0);
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
      }
    }
    else if (besu_scan_donen_odd_new && !vid_rst_evn_new) {
      state_new.oam_abus = (scan_counter_new << 2) ^ 0xFF;
      state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
      state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;

      if (DELTA_HA_new) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 1;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
      }
      if (DELTA_AB_new) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 0;
      }
      if (DELTA_BC_new) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 0;
      }
      if (DELTA_CD_new) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = !(cpu_addr_oam_new && cpu.core.reg.bus_req_new.read);
      }
      if (DELTA_DE_new) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = !cpu_addr_oam_new;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
      }
      if (DELTA_EF_new) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 0;
      }
      if (DELTA_FG_new) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 0;
      }
      if (DELTA_GH_new) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
      }
    }
    else if (!state_new.XYMU_RENDERINGn) {
      uint8_t sfetch_oam_oen_new =  !(state_new.phase_sfetch == 0 || state_new.phase_sfetch == 3);
      uint8_t sfetch_oam_clk_new =  !(state_new.phase_sfetch >= 1 && state_new.phase_sfetch <= 3);

      state_new.oam_abus = (uint8_t)~((state_new.sprite_ibus << 2) | 0b11);
      state_new.oam_ctrl.SIG_OAM_CLKn.state  = sfetch_oam_clk_new && (!cpu_addr_oam_new || gen_clk(phase_new, 0b11110000));
      state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
      state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
      state_new.oam_ctrl.SIG_OAM_OEn.state   = sfetch_oam_oen_new && !(cpu_addr_oam_new && (cpu.core.reg.bus_req_new.read && !DELTA_HA_new) && !(DELTA_DH_new && cpu.core.reg.bus_req_new.read));
    }
    else if (cpu_addr_oam_new) {
      state_new.oam_abus = uint8_t(~cpu_addr_new);
      state_new.oam_ctrl.SIG_OAM_CLKn.state  = DELTA_HD_new;
      state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
      state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
      state_new.oam_ctrl.SIG_OAM_OEn.state   = !DELTA_AD_new || !cpu.core.reg.bus_req_new.read;

      if (DELTA_DH_new) {
        if (cpu_wr) {
          if (!DELTA_GH_new) {
            state_new.oam_ctrl.SIG_OAM_WRn_A.state =  bit(cpu_addr_new, 0);
            state_new.oam_ctrl.SIG_OAM_WRn_B.state = !bit(cpu_addr_new, 0);
          }
        }
      }
    }
    else {
      state_new.oam_ctrl.SIG_OAM_CLKn.state  = 1;
      state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
      state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
      state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
      state_new.oam_abus = (uint8_t)~cpu_addr_new;
    }

    state_new.oam_dbus_a = oam_dbus_a;
    state_new.oam_dbus_b = oam_dbus_b;


    // TOBU/VONU/SEBA/TYFO

    if (vid_rst_evn_new) {
      state_new.sfetch_control.TOBU_SFETCH_S1p_D2_evn.state = 0;
      state_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.state = 0;
      state_new.sfetch_control.SEBA_SFETCH_S1p_D5_odd.state = 0;
    }
    else if (state_new.XYMU_RENDERINGn) {
      state_new.sfetch_control.TOBU_SFETCH_S1p_D2_evn.state = 0;
      state_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.state = 0;
      state_new.sfetch_control.SEBA_SFETCH_S1p_D5_odd.state = 0;
    }
    else {
      if (DELTA_EVEN_new) {
        state_new.sfetch_control.TOBU_SFETCH_S1p_D2_evn.state = state_old.phase_sfetch > 10 ? bit(5, 1) : bit(state_old.phase_sfetch / 2, 1);
        state_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.state = state_old.sfetch_control.TOBU_SFETCH_S1p_D2_evn.state;
      }
      else {
        state_new.sfetch_control.SEBA_SFETCH_S1p_D5_odd.state = state_old.sfetch_control.VONU_SFETCH_S1p_D4_evn.state;
      }
    }

    state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.state = sfetch_done_trig_new;

    state_new.tfetch_control.LONY_TFETCHINGp.state = !state_new.XYMU_RENDERINGn && state_new.phase_tfetch < 12;

    if (!NYXU_BFETCH_RSTn_new) {
      state_new.tfetch_control.LOVY_TFETCH_DONEp.state = 0;
    }
    else if (DELTA_ODD_new) {
      state_new.tfetch_control.LOVY_TFETCH_DONEp.state = state_old.phase_tfetch >= 10;
    }

    state_new.tfetch_counter_odd = uint8_t(min(state_new.phase_tfetch, 10) / 2);

    // ROGE
    state_new.win_ctrl.ROGE_WY_MATCHp_odd.state = (reg_ly_new == uint8_t(~state_new.reg_wy)) && win_en_new;

    uint8_t doba_scan_donep_evn_new = 0;
    uint8_t byba_scan_donep_odd_new = 0;

    if (vid_rst_evn_new) {
      doba_scan_donep_evn_new = 0;
      byba_scan_donep_odd_new = 0;
    }
    else if (first_line_new) {
      doba_scan_donep_evn_new = phase_lx_new < 0 || phase_lx_new >= 167;
      byba_scan_donep_odd_new = phase_lx_new < 0 || phase_lx_new >= 166;
    }
    else if (phase_ly_new >= 0 && phase_ly_new < 144) {
      doba_scan_donep_evn_new = phase_lx_new < 2 || phase_lx_new >= 163;
      byba_scan_donep_odd_new = phase_lx_new < 2 || phase_lx_new >= 162;
    }
    else if (phase_ly_new >= 144 && phase_ly_new < 153) {
      doba_scan_donep_evn_new = 1;
      byba_scan_donep_odd_new = 1;
    }
    else if (phase_ly_new == 153) {
      doba_scan_donep_evn_new = phase_lx_new < 6 || phase_lx_new >= 167;
      byba_scan_donep_odd_new = phase_lx_new < 6 || phase_lx_new >= 166;
    }

    if (DELTA_EVEN_new) state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state = bit(state_old.tfetch_counter_odd, 0);
    if (state_new.XYMU_RENDERINGn) state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state = 0;

    if (vid_rst_evn_new) {
      state_new.tfetch_control.PYGO_FETCH_DONEp_evn.state = 0;
    }
    else if (state_new.XYMU_RENDERINGn) {
      state_new.tfetch_control.PYGO_FETCH_DONEp_evn.state = 0;
    }
    else {
      if (DELTA_EVEN_new) {
        state_new.tfetch_control.PYGO_FETCH_DONEp_evn.state = state_old.tfetch_control.PORY_FETCH_DONEp_odd.state;
      }
    }

    state_new.win_ctrl.PUKU_WIN_HITn_odd.state = !state_new.win_ctrl.RYDY_WIN_HITp_odd.state;
    if (vid_rst_evn_new) state_new.win_ctrl.PUKU_WIN_HITn_odd.state = 1;
    state_new.oam_ctrl.WUJE_CPU_OAM_WRn.state = !(cpu_addr_oam_new && cpu_wr && DELTA_DH_new);
    state_new.sprite_scanner.CENO_SCAN_DONEn_odd.state = ceno_scan_donen_odd_new;
    state_new.sprite_scanner.BESU_SCAN_DONEn_odd.state = besu_scan_donen_odd_new;
    state_new.sprite_scanner.DOBA_SCAN_DONEp_evn.state = doba_scan_donep_evn_new;
    state_new.sprite_scanner.BYBA_SCAN_DONEp_odd.state = byba_scan_donep_odd_new;

    state_new.scan_counter = (uint8_t)scan_counter_new;

    if (phase_ly_new < 144) {
      state_new.lcd.CATU_LINE_ENDp_odd.state = !first_line_new && (phase_lx_new >= 2) && (phase_lx_new <= 9);
      state_new.lcd.ANEL_LINE_ENDp_odd.state = !first_line_new && (phase_lx_new >= 4) && (phase_lx_new <= 11);
    }
    else if (phase_ly_new >= 144 && phase_ly_new < 153) {
      state_new.lcd.CATU_LINE_ENDp_odd.state = 0;
      state_new.lcd.ANEL_LINE_ENDp_odd.state = 0;
    }
    if (phase_ly_new == 153) {
      state_new.lcd.CATU_LINE_ENDp_odd.state = (phase_lx_new >= 6) && (phase_lx_new <= 9);
      state_new.lcd.ANEL_LINE_ENDp_odd.state = (phase_lx_new >= 8) && (phase_lx_new <= 11);
    }

    state_new.lcd.POPU_VBLANKp_odd.state = vblank_new;
    state_new.lcd.MYTA_FRAME_ENDp_odd.state = phase_ly_d4_new == 153;
    state_new.lcd.NYPE_LINE_ENDp_odd.state = !first_line_new && (phase_lx_new >= 4) && (phase_lx_new <= 11);

    state_new.reg_lx = reg_lx_new;
    state_new.reg_ly = reg_ly_new;

    if (vid_rst_evn_new || line_rst_odd_new) {
      state_new.VOGA_HBLANKp = 0;
    }
    else if (DELTA_EVEN_new) {
      state_new.VOGA_HBLANKp = (!state_old.FEPO_STORE_MATCHp_odd && (state_old.pix_count_odd == 167));
    }

    state_new.sprite_scanner.FETO_SCAN_DONEp.state = state_new.scan_counter == 39;
    state_new.tfetch_control.LYRY_BFETCH_DONEp_odd.state = state_new.tfetch_counter_odd >= 5;


    state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = ext_addr_new;
    state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.state = DELTA_DH_new && cpu.core.reg.bus_req_new.read;
    state_new.cpu_signals.SIG_IN_CPU_WRp.state = cpu_wr;
    state_new.cpu_signals.SIG_IN_CPU_RDp.state = cpu_rd;
    if (!vid_rst_evn_new) {
      if (DELTA_FG_new) {
        state_new.lcd.SYGU_LINE_STROBE.state = (reg_lx_old == 0) || (reg_lx_old == 7) || (reg_lx_old == 45) || (reg_lx_old == 83);
      }

      if (state_old.lcd.RUTU_LINE_ENDp_odd.state && !state_new.lcd.RUTU_LINE_ENDp_odd.state) state_new.lcd.LUCA_LINE_EVENp.state = !state_new.lcd.LUCA_LINE_EVENp.state;
      if (!vblank_old && vblank_new) state_new.lcd.NAPO_FRAME_EVENp.state = !state_new.lcd.NAPO_FRAME_EVENp.state;

      if (state_old.lcd.NYPE_LINE_ENDp_odd.state && !state_new.lcd.NYPE_LINE_ENDp_odd.state) {
        state_new.lcd.MEDA_VSYNC_OUTn.state = reg_ly_new == 0;
      }

      if (bit(state_new.pix_count_odd, 0) && bit(state_new.pix_count_odd, 3)) state_new.lcd.WUSA_LCD_CLOCK_GATE.state = 1;
      if (state_new.VOGA_HBLANKp) state_new.lcd.WUSA_LCD_CLOCK_GATE.state = 0;
    }
    if (vid_rst_evn_new) {
      state_new.lcd.SYGU_LINE_STROBE.state = 0;
      state_new.lcd.LUCA_LINE_EVENp.state = 0;
      state_new.lcd.NAPO_FRAME_EVENp.state = 0;
      state_new.lcd.MEDA_VSYNC_OUTn.state = 0;
      state_new.lcd.WUSA_LCD_CLOCK_GATE.state = 0;
    }

    bit_unpack(pins.abus_lo, (~cart_addr >> 0) & 0xFF);
    bit_unpack(pins.abus_hi, (~cart_addr >> 8) & 0xFF);

    pins.ctrl.PIN_78_WRn.state = pins_ctrl_wrn_new;
    pins.ctrl.PIN_79_RDn.state = pins_ctrl_rdn_new;
    pins.ctrl.PIN_80_CSn.state = pins_ctrl_csn_new;

    pins.vram_ctrl.PIN_43_VRAM_CSn.state = pins_vram_ctrl_csn_new;
    pins.vram_ctrl.PIN_45_VRAM_OEn.state = pins_vram_ctrl_oen_new;
    pins.vram_ctrl.PIN_49_VRAM_WRn.state = pins_vram_ctrl_wrn_new;

    // the "avap_scan_done_new && state_new.lcd.PAHO_X_8_SYNC" latch branch is never hit, would probably cause
    // latch oscillation or something

    if (clkpipe_posedge_evn_new) {
      state_new.lcd.PAHO_X8_SYNC.state = bit(state_old.pix_count_odd, 3);
    }

    if (state_new.XYMU_RENDERINGn) {
      state_new.lcd.PAHO_X8_SYNC.state = 0;
    }

    if (vid_rst_evn_new) {
      state_new.lcd.POME_X8_LATCH.state = 1;
    }
    else if (scan_done_trig_new) {
      state_new.lcd.POME_X8_LATCH.state = 0;
    }
    else if (state_new.lcd.PAHO_X8_SYNC.state) {
      state_new.lcd.POME_X8_LATCH.state = 1;
    }

    if (!vid_rst_evn_new) {
      wire TYFA_CLKPIPE_evn_new = (!state_new.win_ctrl.RYDY_WIN_HITp_odd.state && state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && !state_old.FEPO_STORE_MATCHp_odd && (state_old.pix_count_odd != 167) && DELTA_EVEN_new);

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
      pins.lcd.PIN_55_LCD_LATCH.state = !bit(state_new.reg_div, 6);
      pins.lcd.PIN_56_LCD_FLIPS.state = !bit(state_new.reg_div, 7);
      pins.lcd.PIN_57_LCD_VSYNC.state = 1;
    }


    state_new.lcd.RUJU.state = state_new.lcd.POME_X8_LATCH.state;
    state_new.lcd.POFY.state = !state_new.lcd.POME_X8_LATCH.state;
    state_new.lcd.REMY_LD0n.state = REMY_LD0n;
    state_new.lcd.RAVO_LD1n.state = RAVO_LD1n;
    state_new.sprite_scanner.AVAP_SCAN_DONE_tp_odd.state = !vid_rst_evn_new && !line_rst_odd_new && scan_done_trig_new;
    state_new.sfetch_control.TEXY_SFETCHINGp_evn.state = ((state_new.phase_sfetch >= 4) && (state_new.phase_sfetch < 12) && !vid_rst_evn_old && !state_old.XYMU_RENDERINGn);
    state_new.WODU_HBLANKp_odd = hblank_new;
    state_new.ACYL_SCANNINGp_odd = (!MATU_DMA_RUNNINGp_odd_new && besu_scan_donen_odd_new && !vid_rst_evn_new);
    state_new.ATEJ_LINE_RSTp_odd = line_rst_odd_new || vid_rst_evn_new;
    state_new.cpu_signals.SIG_CPU_BOOTp.state = 0;
    state_new.cpu_signals.SIG_BOOT_CSp.state = 0;

    if (cpu_addr_new <= 0x00FF) {

      state_new.cpu_signals.SIG_CPU_BOOTp.state = !state_new.cpu_signals.TEPU_BOOT_BITn.state;

      if ((cpu.core.reg.bus_req_new.read && !DELTA_HA_new) && !state_new.cpu_signals.TEPU_BOOT_BITn.state) {
        state_new.cpu_signals.SIG_BOOT_CSp.state = 1;
      }
    }

    bit_unpack(pins.vram_dbus, pins_vram_dbus);
    bit_unpack(pins.vram_abus, state_new.vram_abus);

    bit_unpack(pins.dbus, ~cart_dbus);


    pins.joy.PIN_63_JOY_P14.state = !bit(state_new.reg_joy, 0);
    pins.joy.PIN_62_JOY_P15.state = !bit(state_new.reg_joy, 1);

    pins.joy.PIN_67_JOY_P10.state = bit(state_new.joy_latch, 0);
    pins.joy.PIN_66_JOY_P11.state = bit(state_new.joy_latch, 1);
    pins.joy.PIN_65_JOY_P12.state = bit(state_new.joy_latch, 2);
    pins.joy.PIN_64_JOY_P13.state = bit(state_new.joy_latch, 3);


    if (bit(state_new.oam_temp_b, 5) && ((state_new.phase_sfetch >= 4) && (state_new.phase_sfetch < 12) && !vid_rst_evn_old && !state_old.XYMU_RENDERINGn)) {
      state_new.flipped_sprite = bit_reverse(state_new.vram_dbus);
    }
    else {
      state_new.flipped_sprite = state_new.vram_dbus;
    }

    state_new.oam_ctrl.old_oam_clk.state = !state_new.oam_ctrl.SIG_OAM_CLKn.state; // Vestige of gate mode
    state_new.zram_bus.clk_old.state = gen_clk(phase_new, 0b00001110) && cpu_wr;

    state_new.cpu_ack = cpu.core.get_int_ack();

    pins.sys.PIN_74_CLK.CLK.state = gen_clk(phase_new, 0b10101010); // dead signal
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

    state_new.sys_clk.AFUR_xxxxEFGH.set_state(gen_clk(phase_new, 0b00001111)); // dead signal
    state_new.sys_clk.ALEF_AxxxxFGH.state = gen_clk(phase_new, 0b10000111); // dead signal
    state_new.sys_clk.APUK_ABxxxxGH.state = gen_clk(phase_new, 0b11000011); // dead signal
    state_new.sys_clk.ADYK_ABCxxxxH.state = gen_clk(phase_new, 0b11100001); // dead signal

    pins.sys.PIN_75_CLK_OUT.state = gen_clk(phase_new, 0b00001111); // dead signal

    state_new.sys_clk.SIG_CPU_BOWA_Axxxxxxx.state = gen_clk(phase_new, 0b10000000); // dead signal
    state_new.sys_clk.SIG_CPU_BEDO_xBCDEFGH.state = gen_clk(phase_new, 0b01111111); // dead signal
    state_new.sys_clk.SIG_CPU_BEKO_ABCDxxxx.state = gen_clk(phase_new, 0b11110000); // dead signal
    state_new.sys_clk.SIG_CPU_BUDE_xxxxEFGH.state = gen_clk(phase_new, 0b00001111); // dead signal
    state_new.sys_clk.SIG_CPU_BOLO_ABCDEFxx.state = gen_clk(phase_new, 0b11111100); // dead signal
    state_new.sys_clk.SIG_CPU_BUKE_AxxxxxGH.state = gen_clk(phase_new, 0b10000011); // dead signal
    state_new.sys_clk.SIG_CPU_BOMA_xBCDEFGH.state = gen_clk(phase_new, 0b01111111); // dead signal
    state_new.sys_clk.SIG_CPU_BOGA_Axxxxxxx.state = gen_clk(phase_new, 0b10000000); // dead signal

    state_new.cpu_signals.TEDO_CPU_RDp.state = (cpu.core.reg.bus_req_new.read && !DELTA_HA_new); // dead signal
    state_new.cpu_signals.APOV_CPU_WRp.state = gen_clk(phase_new, 0b00001110) && cpu_wr; // dead signal
    state_new.cpu_signals.TAPU_CPU_WRp.state = gen_clk(phase_new, 0b00001110) && cpu_wr; // dead signal
    state_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK.state = (gen_clk(phase_new, 0b00111111) && state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state); // dead signal

    state_new.sys_rst.AFER_SYS_RSTp.state = 0; // dead signal
    state_new.sys_rst.TUBO_WAITINGp.state = 0; // dead signal
    state_new.sys_rst.ASOL_POR_DONEn.state = 0; // dead signal
    state_new.sys_rst.SIG_CPU_EXT_CLKGOOD.state = 1; // dead signal
    state_new.sys_rst.SIG_CPU_EXT_RESETp.state = 0; // dead signal
    state_new.sys_rst.SIG_CPU_STARTp.state = 0; // dead signal
    state_new.sys_rst.SIG_CPU_INT_RESETp.state = 0; // dead signal
    state_new.sys_rst.SOTO_DBG_VRAMp.state = 0; // dead signal

    state_new.sys_clk.WOSU_AxxDExxH.state = !vid_rst_evn_new && gen_clk(phase_new, 0b10011001); // dead signal
    state_new.sys_clk.WUVU_ABxxEFxx.state = !vid_rst_evn_new && gen_clk(phase_new, 0b11001100); // dead signal
    state_new.sys_clk.VENA_xxCDEFxx.state = !vid_rst_evn_new && gen_clk(phase_new, 0b00111100); // dead signal
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