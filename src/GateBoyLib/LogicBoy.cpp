/// plait_noparse

#include "GateBoyLib/LogicBoy.h"
#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Utils.h"

#include <set>
#include <string>
#include <assert.h>

//#pragma optimize("", off)

#define gb_state FORBIDDEN

#define DELTA_EVEN ((sys.gb_phase_total_old & 1) == 0 && (sys.gb_phase_total_new & 1) == 1)
#define DELTA_ODD  ((sys.gb_phase_total_old & 1) == 1 && (sys.gb_phase_total_new & 1) == 0)

#define DELTA_AB   ((sys.gb_phase_total_old & 7) == 0 && (sys.gb_phase_total_new & 7) == 1)
#define DELTA_BC   ((sys.gb_phase_total_old & 7) == 1 && (sys.gb_phase_total_new & 7) == 2)
#define DELTA_CD   ((sys.gb_phase_total_old & 7) == 2 && (sys.gb_phase_total_new & 7) == 3)
#define DELTA_DE   ((sys.gb_phase_total_old & 7) == 3 && (sys.gb_phase_total_new & 7) == 4)
#define DELTA_EF   ((sys.gb_phase_total_old & 7) == 4 && (sys.gb_phase_total_new & 7) == 5)
#define DELTA_FG   ((sys.gb_phase_total_old & 7) == 5 && (sys.gb_phase_total_new & 7) == 6)
#define DELTA_GH   ((sys.gb_phase_total_old & 7) == 6 && (sys.gb_phase_total_new & 7) == 7)
#define DELTA_HA   ((sys.gb_phase_total_old & 7) == 7 && (sys.gb_phase_total_new & 7) == 0)

#define DELTA_AD   (DELTA_AB || DELTA_BC || DELTA_CD)

#define DELTA_AC   (DELTA_AB || DELTA_BC)
#define DELTA_EG   (DELTA_EF || DELTA_FG)
#define DELTA_HD   (DELTA_HA || DELTA_AB || DELTA_BC || DELTA_CD)
#define DELTA_DH   (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH)
#define DELTA_DG   (DELTA_DE || DELTA_EF || DELTA_FG)

using std::min;

//-----------------------------------------------------------------------------

GBResult LogicBoy::poweron(bool fastboot) {
  assert(false);
}

//-----------------------------------------------------------------------------

GBResult LogicBoy::reset() {
  lb_state.reset();
  cpu.reset();
  mem.reset();
  sys.reset();

  pins.reset();
  pins = bit_purge(pins);

  probes.reset();
  lb_bit_check();
  return GBResult::ok();
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
  sys.gb_phase_total_new = sys.gb_phase_total_old + 1;
  tock_cpu();
  tock_logic(cart_blob);
  sys.gb_phase_total_old = sys.gb_phase_total_new;
  return GBResult::ok();
}

GBResult LogicBoy::run_to(const blob& cart_blob, int phase) {
  while(get_sys().gb_phase_total_old < phase) {
    next_phase(cart_blob);
  }
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
  CHECK_P((sys.gb_phase_total_old & 7) == 7);

  cpu.core.reg.bus_req_new.addr = addr;
  cpu.core.reg.bus_req_new.data = data;
  cpu.core.reg.bus_req_new.read = !write;
  cpu.core.reg.bus_req_new.write = write;

  return GBResult::ok();
}

//-----------------------------------------------------------------------------

GBResult LogicBoy::dbg_read(const blob& cart_blob, int addr) {
  CHECK_P((sys.gb_phase_total_old & 7) == 7);

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
  CHECK_P((sys.gb_phase_total_old & 7) == 7);

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

void LogicBoy::get_flat_blob(const blob& cart_blob, int addr, int size, blob& out) const {
  if (addr + size >= 0xFFFF) {
    size = 0xFFFF - addr;
  }

  out.resize(size);
  for (int i = 0; i < size; i++) {
    out[i] = read_flat_addr(cart_blob, addr + i);
  }
}

//------------------------------------------------------------------------------------------------------------------------

uint8_t LogicBoy::read_flat_addr(const blob& cart_blob, int addr) const {
  if (!bit0(lb_state.cpu_signals.TEPU_BOOT_BITn.state) && addr >= 0x0000 && addr < 0x0100) {
    return mem.bootrom[addr];
  }
  else if (addr >= 0x0000 && addr < 0x4000) {
    return cart_blob[addr & 0x7FFF];

    // FIXME why were we mucking with bank 0?
    /*
    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_mode   = bit(gb_state.ext_mbc.MBC1_MODE.out_old());
      bool mbc1_ram_en = bit(gb_state.ext_mbc.MBC1_RAM_EN.out_old());
      uint32_t mbc1_rom0_bank = mbc1_mode ? bit_pack(&gb_state.ext_mbc.MBC1_BANK5, 2) : 0;
      uint32_t mbc1_rom0_addr = ((addr & 0x3FFF) | (mbc1_rom0_bank << 19)) & cart_rom_addr_mask(cart_blob);
      CHECK_P(mbc1_rom0_addr < cart_blob.size());
      return &cart_blob[mbc1_rom0_addr];
    }
    else {
      return &cart_blob[addr & 0x7FFF];
    }
    */
  }
  else if (addr >= 0x4000 && addr < 0x8000) {
    if (cart_has_mbc1(cart_blob)) {
      uint32_t mbc1_rom1_bank = bit_pack(&lb_state.ext_mbc.MBC1_BANK0, 7);
      if ((mbc1_rom1_bank & 0x1F) == 0) mbc1_rom1_bank |= 1;
      uint32_t mbc1_rom1_addr = ((addr & 0x3FFF) | (mbc1_rom1_bank << 14)) & cart_rom_addr_mask(cart_blob);
      CHECK_P(mbc1_rom1_addr < cart_blob.size());
      return cart_blob[mbc1_rom1_addr];
    }
    else {
      return cart_blob[addr & 0x7FFF];
    }
  }
  else if (addr >= 0x8000 && addr < 0xA000) {
    return mem.vid_ram[addr - 0x8000];
  }
  else if (addr >= 0xA000 && addr < 0xC000) {
    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_mode   = bit0(lb_state.ext_mbc.MBC1_MODE.out_old());
      bool mbc1_ram_en = bit0(lb_state.ext_mbc.MBC1_RAM_EN.out_old());
      uint32_t mbc1_ram_bank = mbc1_mode ? bit_pack(&lb_state.ext_mbc.MBC1_BANK5, 2) : 0;
      if (mbc1_mode == 0) mbc1_ram_bank = 0;
      uint32_t mbc1_ram_addr = ((addr & 0x1FFF) | (mbc1_ram_bank << 13)) & cart_ram_addr_mask(cart_blob);
      CHECK_P(mbc1_ram_addr < 32768);
      return mbc1_ram_en ? mem.cart_ram[mbc1_ram_addr] : 0xFF;
    }
    else {
      return mem.cart_ram[addr & 0x1FFF];
    }
  }
  else if (addr >= 0xC000 && addr < 0xE000) {
    return mem.int_ram[addr & 0x1FFF];
  }
  else if (addr >= 0xE000 && addr < 0xFE00) {
    return mem.int_ram[addr & 0x1FFF];
  }
  else if (addr >= 0xFE00 && addr < 0xFF00) {
    return mem.oam_ram[addr & 0x00FF];
  }
  else if (addr >= 0xFF30 && addr < 0xFF3F) {
    return mem.wave_ram[addr & 0x000F];
  }
  else if (addr >= 0xFF80 && addr < 0xFFFF) {
    return mem.zero_ram[addr & 0x007F];
  }
  else {
    printf("read_flat_addr : bad addr 0x%08x @ phase old %llu\n", addr, (long long unsigned int)sys.gb_phase_total_old);
    //debugbreak();
    return 0;
  }
}

//------------------------------------------------------------------------------------------------------------------------































































void LogicBoy::tock_cpu() {

  //printf("lb %d %d %d\n", (int)sys.gb_phase_total_old, (int)sys.gb_phase_total_new, lb_state.cpu_dbus);

  // SigOut SIG_CPU_BOWA_Axxxxxxx; // top left port PORTD_01: <- this is the "put address on bus" clock
  // SigOut SIG_CPU_BEDO_xBCDEFGH; // top left port PORTD_02: <-
  //
  // SigOut SIG_CPU_BEKO_ABCDxxxx; // top left port PORTD_03: <- this is the "reset for next cycle" clock
  // SigOut SIG_CPU_BUDE_xxxxEFGH; // top left port PORTD_04: <- this is the "put write data on bus" clock
  //
  // SigOut SIG_CPU_BOLO_ABCDEFxx; // top left port PORTD_05: <-
  // SigOut SIG_CPU_BUKE_AxxxxxGH; // top left port PORTD_07: <- this is probably the "latch bus data" clock
  //
  // SigOut SIG_CPU_BOMA_xBCDEFGH; // top left port PORTD_08: <- (RESET_CLK) // These two clocks are the only ones that run before SIG_CPU_READYp is asserted.
  // SigOut SIG_CPU_BOGA_Axxxxxxx; // top left port PORTD_09: <- test pad 3

  // There doesn't seem to be a good config for halt_latch that works with the latch always latching...

  cpu.core.reg.cpu_data_latch &= (uint8_t)lb_state.cpu_dbus;

  if (DELTA_AB) {
    if (sys.cpu_en && !sys.in_por) {
      cpu.core.execute(lb_state.reg_ie, cpu.core.reg.intf_latch);
      cpu.core.reg.bus_req_new = cpu.core.get_bus_req();

      /*
      // Dump writes to audio mem.
      auto r = cpu.core.reg.bus_req_new;
      if (r.write && r.addr >= 0xFF10 && r.addr <= 0xFF3F) {
        printf("0x%08x 0x%04x 0x%02x\n", sys.gb_phase_total, r.addr, r.data);
      }
      */
    }
  }

  if (DELTA_BC) {
  }

  if (DELTA_CD) {
    // -AB +BC +CD -DE
    if (bit(lb_state.reg_if, 0)) cpu.core.reg.halt_latch |= INT_VBLANK_MASK;
    if (bit(lb_state.reg_if, 1)) cpu.core.reg.halt_latch |= INT_STAT_MASK;
    if (bit(lb_state.reg_if, 3)) cpu.core.reg.halt_latch |= INT_SERIAL_MASK;
    if (bit(lb_state.reg_if, 4)) cpu.core.reg.halt_latch |= INT_JOYPAD_MASK;
    if (cpu.core.reg.op_next == 0x76 && (lb_state.reg_ie & cpu.core.reg.halt_latch)) cpu.core.reg.op_state = 0; // +BC +CD +DE +EF +FG
    cpu.core.reg.halt_latch = 0; // +BC +CD +DE +EF +FG
  }

  if (DELTA_DE) {
  }

  if (DELTA_EF) {
    // -CD +DE +EF +FG
    if (cpu.core.reg.op_state == 0) {
      cpu.core.reg.op_addr = cpu.core.reg.bus_addr;
      cpu.core.reg.op_next = lb_state.cpu_dbus;
    }
  }

  if (DELTA_FG) {
    cpu.core.reg.cpu_data_latch = 0xFF; // -DE +EF
  }

  if (DELTA_GH) {
    cpu.core.reg.intf_latch = lb_state.reg_if; // -EF +FG +GH -HA
    if (bit(lb_state.reg_if, 2)) cpu.core.reg.halt_latch |= INT_TIMER_MASK; // +FG +GH -HA : this one latches funny, some hardware bug    

    if (sys.cpu_en && !sys.in_por) {
      if (cpu.core.reg.op_state == 0) {
        if ((lb_state.reg_ie & cpu.core.reg.intf_latch) && cpu.core.reg.ime) {
          cpu.core.reg.op_next = 0xF4; // fake opcode
          cpu.core.reg.ime = false;
          cpu.core.reg.ime_delay = false;
        }
      }
      cpu.core.reg.int_ack = 0;
      cpu.core.reg.ime = cpu.core.reg.ime_delay; // must be after int check, before op execution
    }
  }

  if (DELTA_HA) {
    if (cpu.core.reg.bus_read) {
      cpu.core.reg.data_in = cpu.core.reg.cpu_data_latch; // -FG +GH +HA -AB
    }
  }
}



























































//-----------------------------------------------------------------------------

void LogicBoy::tock_logic(const blob& cart_blob) {

  auto phase_old = (sys.gb_phase_total_old & 7);
  auto phase_new = (sys.gb_phase_total_new & 7);

  //----------------------------------------
  // old signals

  LogicBoyState  state_old = lb_state;

  wire nuko_wx_match_old = (uint8_t(~state_old.reg_wx) == state_old.pix_count) && state_old.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state;

  //----------------------------------------

  LogicBoyState& state_new = lb_state;

  //----------------------------------------

  // Data has to be driven on EFGH or we fail the wave tests

  state_new.cpu_dbus = (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) && cpu.core.reg.bus_req_new.write ? cpu.core.reg.bus_req_new.data_lo : 0xFF; // must be pulled up or we fail regression
  state_new.cpu_abus = (DELTA_HA ? cpu.core.reg.bus_req_new.addr & 0x00FF : cpu.core.reg.bus_req_new.addr);









#if 1
  {
    auto addr  = cpu.core.reg.bus_req_new.addr;
    auto read  = cpu.core.reg.bus_req_new.read;
    auto write = cpu.core.reg.bus_req_new.write;

    probe("cpu_read", read & 1);
    probe("cpu_write", write & 1);

    wire req_addr_boot = (addr >= 0x0000) && (addr <= 0x00FF) && !(state_old.cpu_signals.TEPU_BOOT_BITn.state & 1);
    wire req_addr_rom  = (addr >= 0x0000) && (addr <= 0x7FFF);
    wire req_addr_vram = (addr >= 0x8000) && (addr <= 0x9FFF);
    wire req_addr_ram  = (addr >= 0xA000) && (addr <= 0xFDFF);
    wire req_addr_oam  = (addr >= 0xFE00) && (addr <= 0xFEFF);
    wire req_addr_hi   = (addr >= 0xFE00);

    state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.state = ((DELTA_DH && (read || (write && addr >= 0xFF10))) & 1);

    if (DELTA_HA) {
      wire ext_addr_new = (read || write) && (!req_addr_hi && !req_addr_boot && !req_addr_vram);
      //state_new.cpu_abus.set_addr(cpu.core.reg.bus_req_new.addr & 0x00FF);
      state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = ext_addr_new;
      state_new.cpu_signals.SIG_IN_CPU_WRp.state = 0;
      state_new.cpu_signals.SIG_IN_CPU_RDp.state = 0;
    }
    else if (DELTA_AB) {
      wire ext_addr_new = (read || write) && (!req_addr_hi && !req_addr_boot && !req_addr_vram);
      //state_new.cpu_abus.set_addr(cpu.core.reg.bus_req_new.addr);
      state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = ext_addr_new;
      state_new.cpu_signals.SIG_IN_CPU_WRp.state = write;
      state_new.cpu_signals.SIG_IN_CPU_RDp.state = read;
    }
    else {
      wire ext_addr_new = (read || write) && (!req_addr_hi && !req_addr_boot);
      //state_new.cpu_abus.set_addr(cpu.core.reg.bus_req_new.addr);
      state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = ext_addr_new;
      state_new.cpu_signals.SIG_IN_CPU_WRp.state = write;
      state_new.cpu_signals.SIG_IN_CPU_RDp.state = read;
    }

  }

  probe("DBUS_FREE", state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.state & 1);
  probe("EXT_BUSp",  state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state & 1);
  probe("BUS_RDp",   state_new.cpu_signals.SIG_IN_CPU_RDp.state & 1);
  probe("BUS_WRp",   state_new.cpu_signals.SIG_IN_CPU_WRp.state & 1);

#endif












  wire req_addr_vram = (cpu.core.reg.bus_req_new.addr >= 0x8000) && (cpu.core.reg.bus_req_new.addr <= 0x9FFF);
  wire req_addr_ram  = (cpu.core.reg.bus_req_new.addr >= 0xA000) && (cpu.core.reg.bus_req_new.addr <= 0xFDFF);
  wire req_addr_oam  = (cpu.core.reg.bus_req_new.addr >= 0xFE00) && (cpu.core.reg.bus_req_new.addr <= 0xFEFF);
  wire req_addr_hi   = (cpu.core.reg.bus_req_new.addr >= 0xFE00);
  wire req_addr_lo   = (cpu.core.reg.bus_req_new.addr <= 0x00FF);
  wire req_addr_boot = (cpu.core.reg.bus_req_new.addr >= 0x0000) && (cpu.core.reg.bus_req_new.addr <= 0x00FF) && !state_old.cpu_signals.TEPU_BOOT_BITn.state;

  /*
  wire ext_addr_new = (cpu.core.reg.bus_req_new.read || cpu.core.reg.bus_req_new.write) &&
                            (( DELTA_HA_new && !req_addr_hi && !req_addr_boot && !req_addr_vram) ||
                            (!DELTA_HA_new && !req_addr_hi && !req_addr_boot));
  */
  //state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = ext_addr_new;
  wire ext_addr_new = state_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state;

  wire cpu_addr_oam_old  = (state_old.cpu_abus >= 0xFE00) && (state_old.cpu_abus <= 0xFEFF);

  wire cpu_addr_vram_new = req_addr_vram && !DELTA_HA;
  wire cpu_addr_ram_new  = req_addr_ram && !DELTA_HA;
  wire cpu_addr_oam_new  = req_addr_oam && !DELTA_HA;

  wire cpu_addr_bootrom_new = req_addr_lo && !state_new.cpu_signals.TEPU_BOOT_BITn.state;

  wire cpu_rd = cpu.core.reg.bus_req_new.read  && !DELTA_HA;
  wire cpu_wr = cpu.core.reg.bus_req_new.write && !DELTA_HA;

  const auto cpu_addr_new = state_new.cpu_abus;
  auto& cpu_dbus_new = state_new.cpu_dbus;

  
  {
    //state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.state = DELTA_DH_new && cpu.core.reg.bus_req_new.read;
    auto addr  = cpu.core.reg.bus_req_new.addr;
    auto read  = cpu.core.reg.bus_req_new.read;
    auto write = cpu.core.reg.bus_req_new.write;
    state_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.state = ((DELTA_DH && (read || (write && addr >= 0xFF10))) & 1);
  }

  state_new.cpu_signals.SIG_IN_CPU_WRp.state = cpu_wr;
  state_new.cpu_signals.SIG_IN_CPU_RDp.state = cpu_rd;

  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF50) {
    wire SATO_BOOT_BITn_old = bit(state_old.cpu_dbus, 0) || state_old.cpu_signals.TEPU_BOOT_BITn.state;
    state_new.cpu_signals.TEPU_BOOT_BITn.state = SATO_BOOT_BITn_old;
  }

  // boot bit read

  if (cpu_rd && cpu_addr_new == 0xFF50) {
    state_new.cpu_dbus &= ~1;
    state_new.cpu_dbus |= state_new.cpu_signals.TEPU_BOOT_BITn.state;
  }












  //----------------------------------------
  // Timer & DIV

  {
    if (DELTA_HA) {
      state_new.reg_div = state_new.reg_div + 1;
    }

    if (cpu_wr && (DELTA_DE || DELTA_EF || DELTA_FG)) {
      if (cpu_addr_new == 0xFF04) state_new.reg_div = 0;
    }

    if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF06) state_new.reg_tma  =  state_old.cpu_dbus;
    if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF07) state_new.reg_tac  =  state_old.cpu_dbus & 0b111;

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

    if (DELTA_HA) {
      state_new.int_ctrl.MOBA_TIMER_OVERFLOWp.state = !bit(state_old.reg_tima, 7) && state_old.int_ctrl.NYDU_TIMA7p_DELAY.state;
      state_new.int_ctrl.NYDU_TIMA7p_DELAY.state = bit(state_old.reg_tima, 7);
    }

    if (DELTA_DG && cpu_wr && !cpu_rd && cpu_addr_new == 0xFF05) {
      state_new.int_ctrl.NYDU_TIMA7p_DELAY.state = 0;
      state_new.reg_tima = cpu_dbus_new; // must be new
    }
    else if (state_new.int_ctrl.MOBA_TIMER_OVERFLOWp.state) {
      state_new.int_ctrl.NYDU_TIMA7p_DELAY.state = 0;
      state_new.reg_tima = state_new.reg_tma;
    }
  }






















  //----------------------------------------

  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF00) {
    set_bit(state_new.reg_joy, 0, bit(state_old.cpu_dbus, 4));
    set_bit(state_new.reg_joy, 1, bit(state_old.cpu_dbus, 5));
  }
  // LCDC write has to be near the top as it controls the video reset signal
  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF40) {
    //printf("lcdc write at %lld\n", sys.gb_phase_total);
    state_new.reg_lcdc = ~state_old.cpu_dbus;
  }
  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF45) state_new.reg_lyc  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF42) state_new.reg_scy  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF43) state_new.reg_scx  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF47) state_new.reg_bgp  = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF48) state_new.reg_obp0 = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF49) state_new.reg_obp1 = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF4A) state_new.reg_wy   = ~state_old.cpu_dbus;
  if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF4B) state_new.reg_wx   = ~state_old.cpu_dbus;

  if (cpu_wr && DELTA_GH && (cpu_addr_new >= 0xFF80) && (cpu_addr_new <= 0xFFFE)) mem.zero_ram[cpu_addr_new & 0x007F] = state_old.cpu_dbus;

  wire vid_rst_evn_old  =  bit(state_old.reg_lcdc, 7);

  wire bgw_en_new   = !bit(state_new.reg_lcdc, 0);
  wire spr_en_new   = !bit(state_new.reg_lcdc, 1);
  wire spr_size_new =  bit(state_new.reg_lcdc, 2);
  wire bg_map_new   =  bit(state_new.reg_lcdc, 3);
  wire bgw_tile_new =  bit(state_new.reg_lcdc, 4);
  wire win_en_new   = !bit(state_new.reg_lcdc, 5);
  wire win_map_new  =  bit(state_new.reg_lcdc, 6);
  wire vid_rst_evn_new  =  bit(state_new.reg_lcdc, 7);
  if (vid_rst_evn_old && !vid_rst_evn_new) CHECK_P(DELTA_EVEN);

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
  //int frame_index_new = int(phase_lcd_new / (154 * 912));
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

  if (line_rst_odd_old && !line_rst_odd_new) CHECK_P(DELTA_ODD);


  bool vblank_new = phase_ly_d4_new >= 144;

  //----------------------------------------
  // Joypad

  // FIXME what if both scan bits are set?

  if (DELTA_HA) {
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
      state_new.cpu_dbus = mem.bootrom[cpu_addr_new & 0xFF];
    }
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

    if (cpu_wr && DELTA_GH && cpu_addr_new == 0xFF46) dma_hi  = ~state_old.cpu_dbus;
    if (cpu_rd && cpu_addr_new == 0xFF46) state_new.cpu_dbus = ~dma_hi;

    if (DELTA_HA) {
      ctrl.LUVY_DMA_TRIG_d0_odd.state = ctrl.LYXE_DMA_LATCHp.state;
      if (ctrl.LOKY_DMA_LATCHp_odd.state && !ctrl.LENE_DMA_TRIG_d4_odd.state) dma_lo++;
      state_new.MATU_DMA_RUNNINGp = ctrl.LOKY_DMA_LATCHp_odd.state;
    }
    else if (DELTA_DE) {
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
  // SOBU/SUDA

  if (DELTA_EVEN) {
    state_new.sfetch_control.SOBU_SFETCH_REQp_evn.state = state_old.FEPO_STORE_MATCHp && !state_old.win_ctrl.RYDY_WIN_HITp.state && (state_old.phase_tfetch >= 10) && !state_old.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.state;
  }
  else {
    state_new.sfetch_control.SUDA_SFETCH_REQp_odd.state = state_old.sfetch_control.SOBU_SFETCH_REQp_evn.state;
  }

  wire sfetch_trig_odd_old = state_old.sfetch_control.SOBU_SFETCH_REQp_evn.state && !state_old.sfetch_control.SUDA_SFETCH_REQp_odd.state;
  wire sfetch_trig_odd_new = state_new.sfetch_control.SOBU_SFETCH_REQp_evn.state && !state_new.sfetch_control.SUDA_SFETCH_REQp_odd.state;
  if (sfetch_trig_odd_old && !sfetch_trig_odd_new) CHECK_P(DELTA_ODD);

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
  else if (DELTA_ODD) {
    state_new.win_ctrl.NUNU_WIN_MATCHp_odd.state = state_old.win_ctrl.PYCO_WIN_MATCHp_evn.state;
  }

  //----------------------------------------
  // PYNU

  if (state_new.win_ctrl.NUNU_WIN_MATCHp_odd.state) state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state = 1;
  if (!win_en_new)  state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state = 0;
  if (vid_rst_evn_new)  state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state = 0;
  if (line_rst_odd_new) state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state = 0;

  auto PYNU_WIN_MODE_Ap_odd_old = state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state;
  auto PYNU_WIN_MODE_Ap_odd_new = state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state;

  //----------------------------------------

  auto XYMU_RENDERINGn_old = state_old.XYMU_RENDERINGn;
  auto XYMU_RENDERINGn_new = state_new.XYMU_RENDERINGn;

  if (vid_rst_evn_new || line_rst_odd_new) {
    XYMU_RENDERINGn_new = 1;
  }
  else if (scan_done_trig_new) {
    XYMU_RENDERINGn_new = 0;
  }
  else if (DELTA_EVEN) {
    if (!state_old.FEPO_STORE_MATCHp && (state_old.pix_count == 167)) XYMU_RENDERINGn_new = 1;
  }

  state_new.XYMU_RENDERINGn = XYMU_RENDERINGn_new;

  //----------------------------------------
  // NOPA

  if (vid_rst_evn_new) {
    state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state = 0;
  }
  else if (DELTA_EVEN) {
    if (line_rst_odd_new && !XYMU_RENDERINGn_new) {
      state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state = 0;
    }
    else {
      state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state = state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state;
    }
  }

  wire NUNY_WIN_MODE_TRIGp_old = state_old.win_ctrl.PYNU_WIN_MODE_Ap_odd.state && !state_old.win_ctrl.NOPA_WIN_MODE_Bp_evn.state;
  wire NUNY_WIN_MODE_TRIGp_new = state_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.state && !state_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.state;

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  // There are three clocks combined into one here, and the logic does not untangle nicely. :P

  {
    wire scan_oam_clk_old = !MATU_DMA_RUNNINGp_odd_old && besu_scan_donen_odd_old && gen_clk(phase_old, 0b01110111);
    wire scan_oam_clk_new = !MATU_DMA_RUNNINGp_odd_new && besu_scan_donen_odd_new && gen_clk(phase_new, 0b01110111);

    wire ppu_oam_clk_old = (state_old.phase_sfetch == 1 || state_old.phase_sfetch == 2 || state_old.phase_sfetch == 3) && !state_old.XYMU_RENDERINGn;
    wire ppu_oam_clk_new = (state_new.phase_sfetch == 1 || state_new.phase_sfetch == 2 || state_new.phase_sfetch == 3) && !XYMU_RENDERINGn_new;

    wire dma_oam_clk_old = (cpu_addr_oam_old || MATU_DMA_RUNNINGp_odd_old) && gen_clk(phase_old, 0b00001111);
    wire dma_oam_clk_new = (cpu_addr_oam_new || MATU_DMA_RUNNINGp_odd_new) && gen_clk(phase_new, 0b00001111);

    wire oam_clk_old = scan_oam_clk_old || ppu_oam_clk_old || dma_oam_clk_old;
    wire oam_clk_new = scan_oam_clk_new || ppu_oam_clk_new || dma_oam_clk_new;

    if (!oam_clk_old && oam_clk_new) {
      state_new.oam_temp_a = ~state_old.oam_latch_a;
      state_new.oam_temp_b = ~state_old.oam_latch_b;
    }
  }

  //----------------------------------------

#if 0
  WOTA_SCAN_MATCH_Yn = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, sprite_delta_y.WUHU_YDIFF7.carry, GOVU_SPSIZE_MATCH);
  GESE_SCAN_MATCH_Yp = not1(WOTA_SCAN_MATCH_Yn);

  CEMY_STORE0_CLKp = or(!XOCE_xBCxxFGx, !CENO_SCAN_DONEn_odd, !GESE_SCAN_MATCH_Yp, DEZO_STORE0_SELn);
  YSOK_STORE0_I1p.dff8(CEMY_STORE0_CLKp, BUS_SPR_I1);

#endif

  if (vid_rst_evn_new) {
    state_new.sprite_counter = 0;
    state_new.sprite_store_flags = 0;
    memset(state_new.store_x, 0xFF, 10);
    state_new.sprite_scanner.DEZY_INC_COUNTn_odd.state = 0;
  }
  else if (line_rst_odd_new) {
    state_new.sprite_counter = 0;
    state_new.sprite_store_flags = 0;
    memset(state_new.store_x, 0xFF, 10);
  }
  else {
    int sy = (int)state_new.oam_temp_a - 16;
    int sprite_height = spr_size_new ? 8 : 16;
    wire sprite_hit = (reg_ly_new >= sy) && (reg_ly_new < sy + sprite_height) && ceno_scan_donen_odd_old;

    if (DELTA_HA || DELTA_DE) {
      if (!state_old.sprite_scanner.DEZY_INC_COUNTn_odd.state && state_new.sprite_counter < 10) {
        state_new.sprite_counter++;
      }
      state_new.sprite_scanner.DEZY_INC_COUNTn_odd.state = 1;
    }

    if (DELTA_BC || DELTA_FG) {
      if (sprite_hit) {
        state_new.sprite_scanner.DEZY_INC_COUNTn_odd.state = 0;
      }
    }

    if (DELTA_AB || DELTA_EF) {
      if (sprite_hit && state_new.sprite_counter < 10) {
        state_new.sprite_store_flags = (1 << state_new.sprite_counter);
      }
    }

    if (DELTA_CD || DELTA_GH) {
      if (state_new.sprite_store_flags && state_new.sprite_counter < 10) {
        state_new.store_x[state_new.sprite_counter] = state_new.oam_temp_b;
      }
      state_new.sprite_store_flags = 0;
    }
  }

  if (DELTA_CD || DELTA_GH) {
    int sy = (int)state_new.oam_temp_a - 16;
    int sprite_height = spr_size_new ? 8 : 16;
    wire sprite_hit = (reg_ly_new >= sy) && (reg_ly_new < sy + sprite_height) && ceno_scan_donen_odd_old;

    if (sprite_hit && state_new.sprite_counter < 10) {
      state_new.store_i[state_new.sprite_counter] = state_old.sprite_ibus ^ 0b111111;
      state_new.store_l[state_new.sprite_counter] = state_old.sprite_lbus ^ 0b1111;
    }
  }


  //----------------------------------------
  // sprite_reset_flags <- old.sprite_match_flags

  uint8_t sfetch_done_trig_old = state_old.phase_sfetch == 11;
  if (vid_rst_evn_old || line_rst_odd_old || state_old.XYMU_RENDERINGn) sfetch_done_trig_old = 0;

  uint8_t sfetch_done_trig_new = state_new.phase_sfetch == 11;
  if (vid_rst_evn_new || line_rst_odd_new || XYMU_RENDERINGn_new) sfetch_done_trig_new = 0;

  if (!sfetch_done_trig_old && sfetch_done_trig_new) state_new.sprite_reset_flags = state_old.sprite_match_flags;
  if (vid_rst_evn_new || line_rst_odd_new) state_new.sprite_reset_flags = 0;

  //int bleh = 32 - __builtin_clz(uint16_t(0));
  //printf("bleh %d\n", bleh);

  if (state_new.sprite_reset_flags) {
    int sprite_reset_index = __builtin_ctz(state_new.sprite_reset_flags);
    state_new.store_x[sprite_reset_index] = 0xFF;
  }

  //----------------------------------------
  // NYKA / PORY
  // pory can be set on the phase after phase_tfetch gets reset to 0 by nyxu, so we can't use phase_tfetch to drive it?

  if (DELTA_EVEN) {
    state_new.tfetch_control.NYKA_FETCH_DONEp_evn.state = state_old.phase_tfetch >= 10;
  }
  if (vid_rst_evn_new || XYMU_RENDERINGn_new || line_rst_odd_new || NUNY_WIN_MODE_TRIGp_new) {
    state_new.tfetch_control.NYKA_FETCH_DONEp_evn.state = 0;
  }

  auto NYKA_FETCH_DONEp_evn_old = state_old.tfetch_control.NYKA_FETCH_DONEp_evn.state;
  auto NYKA_FETCH_DONEp_evn_new = state_new.tfetch_control.NYKA_FETCH_DONEp_evn.state;

  if (DELTA_ODD) {
    state_new.tfetch_control.PORY_FETCH_DONEp_odd.state = state_old.tfetch_control.NYKA_FETCH_DONEp_evn.state;
  }
  if (vid_rst_evn_new || XYMU_RENDERINGn_new || line_rst_odd_new || NUNY_WIN_MODE_TRIGp_new) {
    state_new.tfetch_control.PORY_FETCH_DONEp_odd.state = 0;
  }

  auto PORY_FETCH_DONEp_odd_old = state_old.tfetch_control.PORY_FETCH_DONEp_odd.state;
  auto PORY_FETCH_DONEp_odd_new = state_new.tfetch_control.PORY_FETCH_DONEp_odd.state;

  uint8_t fetch_done_old = (state_old.phase_tfetch >= 10) && (state_old.phase_tfetch <= 14);
  if (vid_rst_evn_old || state_old.XYMU_RENDERINGn || line_rst_odd_old || NUNY_WIN_MODE_TRIGp_old) fetch_done_old = 0;

  uint8_t fetch_done_new = (state_old.phase_tfetch >= 11) && (state_old.phase_tfetch <= 15);
  if (vid_rst_evn_new || XYMU_RENDERINGn_new || line_rst_odd_new || NUNY_WIN_MODE_TRIGp_new) fetch_done_new = 0;

  //----------------------------------------
  // POKY

  if (vid_rst_evn_new) {
    state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state = 0;
  }
  else if (XYMU_RENDERINGn_new) {
    state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state = 0;
  }
  else {
    if (DELTA_EVEN) {
      if (state_old.tfetch_control.PORY_FETCH_DONEp_odd.state) {
        //printf("%d\n", state_old.phase_tfetch);
        state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state = 1;
      }
    }
  }

  auto POKY_PRELOAD_LATCHp_old = state_old.tfetch_control.POKY_PRELOAD_LATCHp_evn.state;
  auto POKY_PRELOAD_LATCHp_new = state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state;

  wire something_trig_old = !state_old.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && fetch_done_old;
  wire something_trig_new = !state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && fetch_done_new;

  //----------------------------------------
  // RYDY/SOVY


  if (vid_rst_evn_new) {
    state_new.win_ctrl.RYDY_WIN_HITp.state = 0;
    state_new.win_ctrl.SOVY_WIN_HITp.state = 0;
  }
  else {
    if (XYMU_RENDERINGn_new) {
      if (NUNY_WIN_MODE_TRIGp_new) state_new.win_ctrl.RYDY_WIN_HITp.state = 1;
      if (state_new.tfetch_control.PORY_FETCH_DONEp_odd.state) state_new.win_ctrl.RYDY_WIN_HITp.state = 0;
      if (DELTA_EVEN) state_new.win_ctrl.SOVY_WIN_HITp.state = state_old.win_ctrl.RYDY_WIN_HITp.state;
    }
    else {
      if (NUNY_WIN_MODE_TRIGp_new) state_new.win_ctrl.RYDY_WIN_HITp.state = !DELTA_EVEN || !line_rst_odd_new;
      if (state_new.tfetch_control.PORY_FETCH_DONEp_odd.state) state_new.win_ctrl.RYDY_WIN_HITp.state = 0;
      if (DELTA_EVEN) state_new.win_ctrl.SOVY_WIN_HITp.state = state_old.win_ctrl.RYDY_WIN_HITp.state;
    }
  }


  auto RYDY_WIN_HITp_old = state_old.win_ctrl.RYDY_WIN_HITp.state;
  auto RYDY_WIN_HITp_new = state_new.win_ctrl.RYDY_WIN_HITp.state;
  auto SOVY_WIN_HITp_evn_old = state_old.win_ctrl.SOVY_WIN_HITp.state;
  auto SOVY_WIN_HITp_evn_new = state_new.win_ctrl.SOVY_WIN_HITp.state;

  wire win_hit_trig_old  = state_old.win_ctrl.SOVY_WIN_HITp.state && !state_old.win_ctrl.RYDY_WIN_HITp.state;
  wire win_hit_trig_new  = state_new.win_ctrl.SOVY_WIN_HITp.state && !state_new.win_ctrl.RYDY_WIN_HITp.state;

  //----------------------------------------
  auto RYFA_WIN_FETCHn_A_evn_old = state_old.win_ctrl.RYFA_WIN_FETCHn_A.state;
  auto RENE_WIN_FETCHn_B_evn_old = state_old.win_ctrl.RENE_WIN_FETCHn_B.state;
  wire win_fetch_trig_old = state_old.win_ctrl.RYFA_WIN_FETCHn_A.state && !state_old.win_ctrl.RENE_WIN_FETCHn_B.state;
  wire TEVO_WIN_FETCH_TRIGp_old = (win_fetch_trig_old || win_hit_trig_old || something_trig_old) && !state_old.XYMU_RENDERINGn;

  bool WODU_HBLANK_old = !state_old.FEPO_STORE_MATCHp && (state_old.pix_count & 167) == 167;

  wire FEPO_STORE_MATCHp_old = state_old.FEPO_STORE_MATCHp;

  wire SOCY_WIN_HITn_old = not1(state_old.win_ctrl.RYDY_WIN_HITp.state);
  wire ROXY_FINE_SCROLL_DONEn_old = state_old.fine_scroll.ROXY_FINE_SCROLL_DONEn.state;
  auto pix_count_old = state_old.pix_count;
























  // TEH LOOP
  // TEH LOOP
  // TEH LOOP
  // TEH LOOP
  // TEH LOOP
  // TEH LOOP
  // TEH LOOP
  // TEH LOOP

  // VYBO_CLKPIPE    = nor3(FEPO_STORE_MATCHp, WODU_HBLANK, MYVO_AxCxExGx);
  // TYFA_CLKPIPE    = and3(SOCY_WIN_HITn, POKY_PRELOAD_LATCHp, VYBO_CLKPIPE);
  // SEGU_CLKPIPE    = not1(TYFA_CLKPIPE);
  // SACU_CLKPIPE    = or2(SEGU_CLKPIPE, ROXY_FINE_SCROLL_DONEn);
  // RYFA_WIN_FETCHn = dff17(SEGU_CLKPIPE, PANY_WIN_FETCHn);

  bool ROXY_FINE_SCROLL_DONEn_new = ROXY_FINE_SCROLL_DONEn_old;
  int pix_count_new = state_old.pix_count;
  bool FEPO_STORE_MATCHp_new = FEPO_STORE_MATCHp_old;

  wire clkpipe_gate = !RYDY_WIN_HITp_old && POKY_PRELOAD_LATCHp_new && !FEPO_STORE_MATCHp_old && (pix_count_old != 167);

  //--------------------------------------------------------------------------------

  if (XYMU_RENDERINGn_new) {
    state_new.win_ctrl.RYFA_WIN_FETCHn_A.state = 0;
    state_new.win_ctrl.RENE_WIN_FETCHn_B.state = 0;
    state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.state = 1;
    state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state = 0;
    state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state = 0;
    state_new.fine_count_odd = 0;
    ROXY_FINE_SCROLL_DONEn_new = 1;
    if (vid_rst_evn_new || line_rst_odd_new) state_new.pix_count = 0;
    pix_count_new = state_new.pix_count;
    state_new.FEPO_STORE_MATCHp = 0;
    FEPO_STORE_MATCHp_new = state_new.FEPO_STORE_MATCHp;
  }

  //--------------------------------------------------------------------------------

  else if (DELTA_ODD) {
    wire TYFA_CLKPIPE_old = or5(RYDY_WIN_HITp_old, !POKY_PRELOAD_LATCHp_old, FEPO_STORE_MATCHp_old, WODU_HBLANK_old, DELTA_EVEN);
    wire SACU_CLKPIPE_old = or6(RYDY_WIN_HITp_old, !POKY_PRELOAD_LATCHp_old, FEPO_STORE_MATCHp_old, WODU_HBLANK_old, DELTA_EVEN, ROXY_FINE_SCROLL_DONEn_old);

    wire TYFA_CLKPIPE_new = 1;
    wire SACU_CLKPIPE_new = 1;

    if (posedge(TYFA_CLKPIPE_old, TYFA_CLKPIPE_new)) {
      state_new.win_ctrl.RYFA_WIN_FETCHn_A.state = !nuko_wx_match_old && state_old.fine_count_odd == 7;
      if (state_new.fine_count_odd < 7) state_new.fine_count_odd++;
    }

    wire SEKO_WIN_FETCH_TRIG_new = and2(state_new.win_ctrl.RYFA_WIN_FETCHn_A.state, !state_new.win_ctrl.RENE_WIN_FETCHn_B.state);
    wire SUZU_WIN_HIT_TRIG_new   = and2(not1(state_new.win_ctrl.RYDY_WIN_HITp.state), state_new.win_ctrl.SOVY_WIN_HITp.state);

    // wire TEVO_WIN_FETCH_TRIGp  = or3(SEKO_WIN_FETCH_TRIG_new, SUZU_WIN_HIT_TRIG_new, and(ROMO_PRELOAD_DONEn, NYKA_FETCH_DONEp, PORY_FETCH_DONEp));
    // wire PASO_FINE_RST = !TEVO_WIN_FETCH_TRIGp;

    wire win_fetch_trig_new = state_new.win_ctrl.RYFA_WIN_FETCHn_A.state && !state_new.win_ctrl.RENE_WIN_FETCHn_B.state;
    if (win_fetch_trig_new || win_hit_trig_new || something_trig_new) state_new.fine_count_odd = 0;

    state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state = state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state;

    if (state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state && !state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state) {
      state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.state = 0;
    }

    ROXY_FINE_SCROLL_DONEn_new = state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.state;

    if (posedge(SACU_CLKPIPE_old, SACU_CLKPIPE_new)) {
      state_new.pix_count++;
    }

    state_new.FEPO_STORE_MATCHp = 0;
    if (!ceno_scan_donen_odd_new && spr_en_new) {
      if      (state_new.pix_count == state_new.store_x[0]) { state_new.FEPO_STORE_MATCHp = 1; }
      else if (state_new.pix_count == state_new.store_x[1]) { state_new.FEPO_STORE_MATCHp = 1; }
      else if (state_new.pix_count == state_new.store_x[2]) { state_new.FEPO_STORE_MATCHp = 1; }
      else if (state_new.pix_count == state_new.store_x[3]) { state_new.FEPO_STORE_MATCHp = 1; }
      else if (state_new.pix_count == state_new.store_x[4]) { state_new.FEPO_STORE_MATCHp = 1; }
      else if (state_new.pix_count == state_new.store_x[5]) { state_new.FEPO_STORE_MATCHp = 1; }
      else if (state_new.pix_count == state_new.store_x[6]) { state_new.FEPO_STORE_MATCHp = 1; }
      else if (state_new.pix_count == state_new.store_x[7]) { state_new.FEPO_STORE_MATCHp = 1; }
      else if (state_new.pix_count == state_new.store_x[8]) { state_new.FEPO_STORE_MATCHp = 1; }
      else if (state_new.pix_count == state_new.store_x[9]) { state_new.FEPO_STORE_MATCHp = 1; }
    }
    FEPO_STORE_MATCHp_new = state_new.FEPO_STORE_MATCHp;
  }

  //--------------------------------------------------------------------------------

  else if (DELTA_EVEN) {
    state_new.win_ctrl.RENE_WIN_FETCHn_B.state = state_new.win_ctrl.RYFA_WIN_FETCHn_A.state;

    wire win_fetch_trig_new = state_new.win_ctrl.RYFA_WIN_FETCHn_A.state && !state_new.win_ctrl.RENE_WIN_FETCHn_B.state;
    wire TEVO_WIN_FETCH_TRIGp_new = (win_fetch_trig_new || win_hit_trig_new || something_trig_new) && !XYMU_RENDERINGn_new;
    if (TEVO_WIN_FETCH_TRIGp_new) state_new.fine_count_odd = 0;

    if (clkpipe_gate) {
      wire fine_match = state_old.fine_count_odd == (~state_old.reg_scx & 0b111);
      state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state = state_old.fine_scroll.ROXY_FINE_SCROLL_DONEn.state && fine_match;
    }

    if (state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state && !state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state) {
      state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.state = 0;
    }

    ROXY_FINE_SCROLL_DONEn_new = state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.state;

    if (vid_rst_evn_new || line_rst_odd_new) {
      state_new.pix_count = 0;
    }

    CHECK_P(FEPO_STORE_MATCHp_old == FEPO_STORE_MATCHp_new);
  }

  //--------------------------------------------------------------------------------


































  pix_count_new = state_new.pix_count;

  bool WODU_HBLANK_new = !FEPO_STORE_MATCHp_new && (pix_count_new & 167) == 167; // FEPO _must_ be new or we get a mismatch

  wire win_fetch_trig_new = state_new.win_ctrl.RYFA_WIN_FETCHn_A.state && !state_new.win_ctrl.RENE_WIN_FETCHn_B.state;
  wire TEVO_WIN_FETCH_TRIGp_new = (win_fetch_trig_new || win_hit_trig_new || something_trig_new) && !XYMU_RENDERINGn_new;

  auto RYFA_WIN_FETCHn_A_evn_new = state_new.win_ctrl.RYFA_WIN_FETCHn_A.state;
  auto RENE_WIN_FETCHn_B_evn_new = state_new.win_ctrl.RENE_WIN_FETCHn_B.state;

  //----------------------------------------
  // PYCO

  if (DELTA_EVEN && clkpipe_gate) state_new.win_ctrl.PYCO_WIN_MATCHp_evn.state = nuko_wx_match_old;
  if (vid_rst_evn_new) state_new.win_ctrl.PYCO_WIN_MATCHp_evn.state = 0;


  wire NYXU_BFETCH_RSTn_new = (line_rst_odd_new || vid_rst_evn_new || !scan_done_trig_new) && !NUNY_WIN_MODE_TRIGp_new && !TEVO_WIN_FETCH_TRIGp_new;

  // FIXME this fails in fuzz tests
  //if (XYMU_RENDERINGn_new) CHECK_P(NYXU_BFETCH_RSTn_new);

  //----------------------------------------
  // Win map x counter

  if (DELTA_ODD) {

    wire SYLO_WIN_HITn_odd_old = not1(RYDY_WIN_HITp_old);
    wire TOMU_WIN_HITp_odd_old = not1(SYLO_WIN_HITn_odd_old);
    wire SEKO_WIN_FETCH_TRIGp_old = nor2(not1(RYFA_WIN_FETCHn_A_evn_old), RENE_WIN_FETCHn_B_evn_old);
    wire TUXY_WIN_FIRST_TILEne_old = nand2(SYLO_WIN_HITn_odd_old, SOVY_WIN_HITp_evn_old);
    wire SUZU_WIN_FIRST_TILEne_old = not1(TUXY_WIN_FIRST_TILEne_old);
    wire ROMO_PRELOAD_DONEn_evn_old = not1(POKY_PRELOAD_LATCHp_old);
    wire SUVU_PRELOAD_DONE_TRIGn_old = nand4(not1(XYMU_RENDERINGn_old), ROMO_PRELOAD_DONEn_evn_old, NYKA_FETCH_DONEp_evn_old, PORY_FETCH_DONEp_odd_old);
    wire TAVE_PRELOAD_DONE_TRIGp_old = not1(SUVU_PRELOAD_DONE_TRIGn_old);
    wire TEVO_WIN_FETCH_TRIGp_old = or3(SEKO_WIN_FETCH_TRIGp_old, SUZU_WIN_FIRST_TILEne_old, TAVE_PRELOAD_DONE_TRIGp_old); // Schematic wrong, this is OR
    wire NOCU_WIN_MODEn_old = not1(PYNU_WIN_MODE_Ap_odd_old);
    wire PORE_WIN_MODEp_old = not1(NOCU_WIN_MODEn_old);
    wire VETU_WIN_MAPp_old = and2(TEVO_WIN_FETCH_TRIGp_old, PORE_WIN_MODEp_old);


    wire SYLO_WIN_HITn_new = not1(RYDY_WIN_HITp_new);
    wire TOMU_WIN_HITp_new = not1(SYLO_WIN_HITn_new);
    wire SEKO_WIN_FETCH_TRIGp_new = nor2(not(RYFA_WIN_FETCHn_A_evn_new), RENE_WIN_FETCHn_B_evn_new);
    wire TUXY_WIN_FIRST_TILEne_new = nand2(SYLO_WIN_HITn_new, SOVY_WIN_HITp_evn_new);
    wire SUZU_WIN_FIRST_TILEne_new = not1(TUXY_WIN_FIRST_TILEne_new);
    wire ROMO_PRELOAD_DONEn_evn_new = not1(POKY_PRELOAD_LATCHp_new);
    wire SUVU_PRELOAD_DONE_TRIGn_new = nand4(not1(XYMU_RENDERINGn_new), ROMO_PRELOAD_DONEn_evn_new, NYKA_FETCH_DONEp_evn_new, PORY_FETCH_DONEp_odd_new);
    wire TAVE_PRELOAD_DONE_TRIGp_new = not1(SUVU_PRELOAD_DONE_TRIGn_new);
    wire TEVO_WIN_FETCH_TRIGp_new = or3(SEKO_WIN_FETCH_TRIGp_new, SUZU_WIN_FIRST_TILEne_new, TAVE_PRELOAD_DONE_TRIGp_new); // Schematic wrong, this is OR
    wire NOCU_WIN_MODEn_new = not1(PYNU_WIN_MODE_Ap_odd_new);
    wire PORE_WIN_MODEp_new = not1(NOCU_WIN_MODEn_new);
    wire VETU_WIN_MAPp_new = and2(TEVO_WIN_FETCH_TRIGp_new, PORE_WIN_MODEp_new);

    if (posedge(VETU_WIN_MAPp_old, VETU_WIN_MAPp_new)) {
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
  if (!XYMU_RENDERINGn_new && !state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && fetch_done_new) state_new.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.state = 0;


  //----------------------------------------
  // Window match

  if (vid_rst_evn_new) {
    state_new.win_ctrl.SARY_WY_MATCHp_odd.state = 0;
    state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state = 0;
    state_new.win_ctrl.NUKO_WX_MATCHp_odd.state = 0;
  }
  else {
    if (DELTA_BC) state_new.win_ctrl.SARY_WY_MATCHp_odd.state = (reg_ly_new == uint8_t(~state_new.reg_wy)) && win_en_new;

    if (state_new.win_ctrl.SARY_WY_MATCHp_odd.state) state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state = 1;
    if (vblank_new) state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state = 0;

    // old used
    state_new.win_ctrl.NUKO_WX_MATCHp_odd.state = (uint8_t(~state_new.reg_wx) == state_new.pix_count) && state_new.win_ctrl.REJO_WY_MATCH_LATCHp_odd.state;
  }

  //----------------------------------------
  // FEPO

  if (MATU_DMA_RUNNINGp_odd_new) {
    state_new.oam_abus = (uint8_t)~state_new.dma_lo;
  }
  else if (besu_scan_donen_odd_new && !vid_rst_evn_new) {
    state_new.oam_abus = (scan_counter_new << 2) ^ 0xFF;
  }
  else if (!XYMU_RENDERINGn_new) {
    state_new.oam_abus = (uint8_t)~((state_new.sprite_ibus << 2) | 0b11);
  }
  else if (cpu_addr_oam_new) {
    state_new.oam_abus = uint8_t(~cpu_addr_new);
  }
  else {
    state_new.oam_abus = (uint8_t)~cpu_addr_new;
  }

  if (!vid_rst_evn_new) {
    if (DELTA_HA || DELTA_DE) {
      state_new.sprite_index = (state_old.oam_abus >> 2) ^ 0b111111;
    }
  }




  if (XYMU_RENDERINGn_new || ceno_scan_donen_odd_new) {
    state_new.sprite_ibus = state_new.sprite_index;
    state_new.sprite_lbus = (~reg_ly_new + state_new.oam_temp_a) & 0b00001111;
    state_new.sprite_match_flags = 0;
  }
  else {
    auto& s = state_new;

    s.sprite_match_flags = 0;
    s.sprite_ibus = 0x3F;
    s.sprite_lbus = 0x0F;

    if (spr_en_new) {
      if      (state_new.pix_count == s.store_x[0]) { s.sprite_match_flags = 0x001;  s.sprite_ibus = s.store_i[0] ^ 0x3F;  s.sprite_lbus = s.store_l[0] ^ 0x0F; }
      else if (state_new.pix_count == s.store_x[1]) { s.sprite_match_flags = 0x002;  s.sprite_ibus = s.store_i[1] ^ 0x3F;  s.sprite_lbus = s.store_l[1] ^ 0x0F; }
      else if (state_new.pix_count == s.store_x[2]) { s.sprite_match_flags = 0x004;  s.sprite_ibus = s.store_i[2] ^ 0x3F;  s.sprite_lbus = s.store_l[2] ^ 0x0F; }
      else if (state_new.pix_count == s.store_x[3]) { s.sprite_match_flags = 0x008;  s.sprite_ibus = s.store_i[3] ^ 0x3F;  s.sprite_lbus = s.store_l[3] ^ 0x0F; }
      else if (state_new.pix_count == s.store_x[4]) { s.sprite_match_flags = 0x010;  s.sprite_ibus = s.store_i[4] ^ 0x3F;  s.sprite_lbus = s.store_l[4] ^ 0x0F; }
      else if (state_new.pix_count == s.store_x[5]) { s.sprite_match_flags = 0x020;  s.sprite_ibus = s.store_i[5] ^ 0x3F;  s.sprite_lbus = s.store_l[5] ^ 0x0F; }
      else if (state_new.pix_count == s.store_x[6]) { s.sprite_match_flags = 0x040;  s.sprite_ibus = s.store_i[6] ^ 0x3F;  s.sprite_lbus = s.store_l[6] ^ 0x0F; }
      else if (state_new.pix_count == s.store_x[7]) { s.sprite_match_flags = 0x080;  s.sprite_ibus = s.store_i[7] ^ 0x3F;  s.sprite_lbus = s.store_l[7] ^ 0x0F; }
      else if (state_new.pix_count == s.store_x[8]) { s.sprite_match_flags = 0x100;  s.sprite_ibus = s.store_i[8] ^ 0x3F;  s.sprite_lbus = s.store_l[8] ^ 0x0F; }
      else if (state_new.pix_count == s.store_x[9]) { s.sprite_match_flags = 0x200;  s.sprite_ibus = s.store_i[9] ^ 0x3F;  s.sprite_lbus = s.store_l[9] ^ 0x0F; }
    }

    if (!state_new.FEPO_STORE_MATCHp) {
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
    if (state_old.phase_tfetch ==  6) state_new.tile_temp_a =  state_old.vram_dbus;
    if (state_old.phase_tfetch == 10) state_new.tile_temp_b =  state_old.vram_dbus;
  }

  //----------------------------------------
  // Pixel pipes

  if (DELTA_ODD && clkpipe_gate && !state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.state) {
    state_new.spr_pipe_a = (state_new.spr_pipe_a << 1) | 0;
    state_new.spr_pipe_b = (state_new.spr_pipe_b << 1) | 0;
    state_new.bgw_pipe_a = (state_new.bgw_pipe_a << 1) | 0;
    state_new.bgw_pipe_b = (state_new.bgw_pipe_b << 1) | 0;
    state_new.mask_pipe  = (state_new.mask_pipe  << 1) | 1;
    state_new.pal_pipe   = (state_new.pal_pipe   << 1) | 0;
  }

  uint8_t sprite_pix = state_old.vram_dbus;
  if (!XYMU_RENDERINGn_new) {

    if (bit(state_old.oam_temp_b, 5) && ((state_new.phase_sfetch >= 4) && (state_new.phase_sfetch < 12) && !vid_rst_evn_old && !state_old.XYMU_RENDERINGn)) {
      sprite_pix = bit_reverse(state_old.vram_dbus);
    }

    if (state_new.phase_sfetch == 7) {
      state_new.sprite_pix_a = ~sprite_pix;
    }

    if (state_new.phase_sfetch == 11) {
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
    state_new.bgw_pipe_a = state_new.tile_temp_a;
    state_new.bgw_pipe_b = state_new.tile_temp_b;
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
     !(req_addr_vram && DELTA_HA) &&
     !req_addr_hi &&
     !cpu_addr_bootrom_new &&
     !cpu_addr_vram_new &&
     DELTA_DH)
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
  else if (!XYMU_RENDERINGn_new && state_new.phase_tfetch < 12) {

    // BG map read address

    const auto px  = state_new.pix_count;
    const auto scx = ~state_new.reg_scx;
    const auto scy = ~state_new.reg_scy;

    //const auto sum_x = px + scx;
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
  else if (XYMU_RENDERINGn_new) {
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
  else if (!XYMU_RENDERINGn_new) {
    
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
    pins_vram_ctrl_oen_new = !cpu_addr_vram_new || !cpu.core.reg.bus_req_new.write || DELTA_HA;
  }
  else {
    pins_vram_ctrl_csn_new = 0;
    pins_vram_ctrl_oen_new = !cpu_addr_vram_new || !cpu.core.reg.bus_req_new.write || DELTA_HA;
  }


  if (!XYMU_RENDERINGn_new) {
    pins_vram_ctrl_wrn_new = 0;
  }
  else if (ext_addr_new) {
    pins_vram_ctrl_wrn_new = cpu_addr_vram_new && gen_clk(phase_new, 0b00001110) && cpu_wr && 1;
  }
  else {
    pins_vram_ctrl_wrn_new = cpu_addr_vram_new && gen_clk(phase_new, 0b00001110) && cpu_wr && 0;
  }


  if (!XYMU_RENDERINGn_new) {
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

    if (cpu_addr_vram_new && gen_clk(phase_new, 0b00111111) && (cpu.core.reg.bus_req_new.read && !DELTA_HA) && (DELTA_DH && cpu.core.reg.bus_req_new.read)) {
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

  if ((cpu.core.reg.bus_req_new.read && !DELTA_HA)) {
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
    if (DELTA_DH) mem.oam_ram[state_new.dma_lo] = dma_addr_vram_new ? state_new.vram_dbus : cart_dbus;
    oam_dbus_a &= dma_addr_vram_new ? ~state_new.vram_dbus : ~cart_dbus;
    oam_dbus_b &= dma_addr_vram_new ? ~state_new.vram_dbus : ~cart_dbus;
  }
  else if (besu_scan_donen_odd_new && !vid_rst_evn_new) {
    // Scanning
    if (DELTA_HA) {
    }
    if (DELTA_AB) {
      state_new.oam_latch_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      state_new.oam_latch_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_BC) {
      state_new.oam_latch_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      state_new.oam_latch_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_CD) {
    }
    if (DELTA_DE) {
    }
    if (DELTA_EF) {
      state_new.oam_latch_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      state_new.oam_latch_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_FG) {
      state_new.oam_latch_a = ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      state_new.oam_latch_b = ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_GH) {
    }

    if (DELTA_HA) {
    }
    if (DELTA_AB) {
      oam_dbus_a &= ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      oam_dbus_b &= ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_BC) {
      oam_dbus_a &= ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      oam_dbus_b &= ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_CD) {
      if (cpu_addr_oam_new && cpu.core.reg.bus_req_new.read) {
        oam_dbus_a &= ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
        oam_dbus_b &= ~mem.oam_ram[((scan_counter_new << 2)) |  1];
      }
    }
    if (DELTA_DE) {
    }
    if (DELTA_EF) {
      oam_dbus_a &= ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      oam_dbus_b &= ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_FG) {
      oam_dbus_a &= ~mem.oam_ram[((scan_counter_new << 2)) & ~1];
      oam_dbus_b &= ~mem.oam_ram[((scan_counter_new << 2)) |  1];
    }
    if (DELTA_GH) {
    }
  }
  else if (!XYMU_RENDERINGn_new) {
    // Rendering

    uint8_t sfetch_oam_oen_new =  !(state_new.phase_sfetch == 0 || state_new.phase_sfetch == 3);
    //uint8_t sfetch_oam_clk_new =  !(state_new.phase_sfetch >= 1 && state_new.phase_sfetch <= 3);

    if (!sfetch_oam_oen_new) {
      oam_dbus_a &= ~mem.oam_ram[(((state_new.sprite_ibus << 2) | 0b11)) & ~1];
      oam_dbus_b &= ~mem.oam_ram[(((state_new.sprite_ibus << 2) | 0b11)) |  1];
    }
    //else if (cpu_addr_oam_new && (!cpu.core.reg.bus_req_new.read || DELTA_AD)) {
    else if (cpu_addr_oam_new && cpu.core.reg.bus_req_new.read) {
      oam_dbus_a &= ~mem.oam_ram[(((state_new.sprite_ibus << 2) | 0b11)) & ~1];
      oam_dbus_b &= ~mem.oam_ram[(((state_new.sprite_ibus << 2) | 0b11)) |  1];
    }


    if (state_new.phase_sfetch == 0 || state_new.phase_sfetch == 3) {
      state_new.oam_latch_a = oam_dbus_a;
      state_new.oam_latch_b = oam_dbus_b;
    }
  }
  else if (cpu_addr_oam_new) {
    // CPUing

    if (DELTA_HD) {
      if (cpu.core.reg.bus_req_new.read && !DELTA_HA) {
        oam_dbus_a &= ~mem.oam_ram[(cpu_addr_new & 0xFF) & ~1];
        oam_dbus_b &= ~mem.oam_ram[(cpu_addr_new & 0xFF) |  1];
        state_new.oam_latch_a = ~mem.oam_ram[(cpu_addr_new & 0xFF) & ~1];
        state_new.oam_latch_b = ~mem.oam_ram[(cpu_addr_new & 0xFF) |  1];
      }
    }
    else if (DELTA_DH) {
      if (cpu.core.reg.bus_req_new.read) {
        state_new.cpu_dbus = !bit(cpu_addr_new, 0) ? ~state_old.oam_latch_a : ~state_old.oam_latch_b;
      }

      if (cpu_wr && DELTA_DE) {
        mem.oam_ram[cpu_addr_new & 0xFF] = state_new.cpu_dbus;
      }

      if (cpu_wr) {
        oam_dbus_a &= ~state_new.cpu_dbus;
        oam_dbus_b &= ~state_new.cpu_dbus;
      }
    }
  }
  else {
    // Idle
    oam_dbus_a &= ~state_new.cpu_dbus;
    oam_dbus_b &= ~state_new.cpu_dbus;
  }

  //----------------------------------------
  // zram

  if ((cpu.core.reg.bus_req_new.read && !DELTA_HA)) {
    if ((cpu_addr_new >= 0xFF80) && (cpu_addr_new <= 0xFFFE)) state_new.cpu_dbus = mem.zero_ram[cpu_addr_new & 0x007F];
  }

  //----------------------------------------
  // And finally, interrupts.

  auto pack_cpu_dbus_old = state_old.cpu_dbus;
  auto pack_cpu_dbus_new = state_new.cpu_dbus;
  auto pack_ie = state_new.reg_ie;
  auto pack_if = state_new.reg_if;
  auto pack_stat = state_new.reg_stat;

  if (!vid_rst_evn_new && DELTA_BC) {
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

  if (cpu_addr_new == 0xFF41 && (cpu.core.reg.bus_req_new.read && !DELTA_HA)) {
    uint8_t stat = 0x80;

    stat |= (!XYMU_RENDERINGn_new || vblank_new) << 0;
    stat |= (!XYMU_RENDERINGn_new || (!MATU_DMA_RUNNINGp_odd_new && besu_scan_donen_odd_new && !vid_rst_evn_new)) << 1;
    stat |= (!state_new.int_ctrl.RUPO_LYC_MATCHn.state) << 2;
    stat |= (pack_stat ^ 0b1111) << 3;

    pack_cpu_dbus_new = stat;
  }

  state_new.lcd.RUTU_LINE_ENDp_odd.state = !first_line_new && (phase_lx_new >= 0) && (phase_lx_new <= 7);

  bool int_stat_old = 0;
  if (!bit(state_old.reg_stat, 0) && (!state_old.FEPO_STORE_MATCHp && (state_old.pix_count == 167)) && !vblank_old) int_stat_old = 1;
  if (!bit(state_old.reg_stat, 1) && vblank_old) int_stat_old = 1;
  if (!bit(state_old.reg_stat, 2) && !vblank_old && state_old.lcd.RUTU_LINE_ENDp_odd.state) int_stat_old = 1;
  if (!bit(state_old.reg_stat, 3) && state_old.int_ctrl.ROPO_LY_MATCH_SYNCp.state) int_stat_old = 1;

  wire int_lcd_old = vblank_old;
  wire int_joy_old = !state_old.joy_int.APUG_JP_GLITCH3.state || !state_old.joy_int.BATU_JP_GLITCH0.state;
  wire int_tim_old = state_old.int_ctrl.MOBA_TIMER_OVERFLOWp.state;
  //wire int_ser_old = serial.CALY_SER_CNT3;
  wire int_ser_old = 0;

  bool int_stat_new = 0;
  if (!bit(pack_stat, 0) && WODU_HBLANK_new && !vblank_new) int_stat_new = 1;
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

  if ((cpu.core.reg.bus_req_new.read && !DELTA_HA)) {
    if (cpu_addr_new == 0xFFFF) pack_cpu_dbus_new = pack_ie | 0b11100000;
    if (cpu_addr_new == 0xFF0F) state_new.int_latch = (uint8_t)pack_if;
    if (cpu_addr_new == 0xFF0F) pack_cpu_dbus_new = pack_if | 0b11100000;
  }


  state_new.cpu_dbus = (uint8_t)pack_cpu_dbus_new;
  state_new.cpu_int = (uint8_t)pack_if;
  state_new.reg_ie = (uint8_t)pack_ie;
  state_new.reg_if = (uint8_t)pack_if;
  state_new.reg_stat = (uint8_t)pack_stat;




  int lcd_x = state_new.pix_count - 8;
  int lcd_y = reg_ly_new;

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
    if (DELTA_ODD) {
      state_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.state = state_old.phase_sfetch > 10 ? bit(5, 0) : bit(state_old.phase_sfetch / 2, 0);
    }

    // this is weird, why is it always 0 when not in reset?
    state_new.oam_ctrl.MAKA_LATCH_EXTp.state = 0;



    //----------
    // OAM bus and control signals.
    // The inclusion of cpu_addr_oam_new in the SCANNING and RENDERING branches is probably a hardware bug.

    if (MATU_DMA_RUNNINGp_odd_new) {
      state_new.oam_abus = (uint8_t)~state_new.dma_lo;
      if (DELTA_HD) {
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

      if (DELTA_HA) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 1;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
      }
      if (DELTA_AB) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 0;
      }
      if (DELTA_BC) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 0;
      }
      if (DELTA_CD) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = !(cpu_addr_oam_new && cpu.core.reg.bus_req_new.read);
      }
      if (DELTA_DE) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = !cpu_addr_oam_new;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
      }
      if (DELTA_EF) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 0;
      }
      if (DELTA_FG) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 0;
      }
      if (DELTA_GH) {
        state_new.oam_ctrl.SIG_OAM_CLKn.state  = 0;
        state_new.oam_ctrl.SIG_OAM_OEn.state   = 1;
      }
    }
    else if (!XYMU_RENDERINGn_new) {
      uint8_t sfetch_oam_oen_new =  !(state_new.phase_sfetch == 0 || state_new.phase_sfetch == 3);
      uint8_t sfetch_oam_clk_new =  !(state_new.phase_sfetch >= 1 && state_new.phase_sfetch <= 3);

      state_new.oam_abus = (uint8_t)~((state_new.sprite_ibus << 2) | 0b11);
      state_new.oam_ctrl.SIG_OAM_CLKn.state  = sfetch_oam_clk_new && (!cpu_addr_oam_new || gen_clk(phase_new, 0b11110000));
      state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
      state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
      state_new.oam_ctrl.SIG_OAM_OEn.state   = sfetch_oam_oen_new && !(cpu_addr_oam_new && (cpu.core.reg.bus_req_new.read && !DELTA_HA) && !(DELTA_DH && cpu.core.reg.bus_req_new.read));
    }
    else if (cpu_addr_oam_new) {
      state_new.oam_abus = uint8_t(~cpu_addr_new);
      state_new.oam_ctrl.SIG_OAM_CLKn.state  = DELTA_HD;
      state_new.oam_ctrl.SIG_OAM_WRn_A.state = 1;
      state_new.oam_ctrl.SIG_OAM_WRn_B.state = 1;
      state_new.oam_ctrl.SIG_OAM_OEn.state   = !DELTA_AD || !cpu.core.reg.bus_req_new.read;

      if (DELTA_DH) {
        if (cpu_wr) {
          if (!DELTA_GH) {
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
    else if (XYMU_RENDERINGn_new) {
      state_new.sfetch_control.TOBU_SFETCH_S1p_D2_evn.state = 0;
      state_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.state = 0;
      state_new.sfetch_control.SEBA_SFETCH_S1p_D5_odd.state = 0;
    }
    else {
      if (DELTA_EVEN) {
        state_new.sfetch_control.TOBU_SFETCH_S1p_D2_evn.state = state_old.phase_sfetch > 10 ? bit(5, 1) : bit(state_old.phase_sfetch / 2, 1);
        state_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.state = state_old.sfetch_control.TOBU_SFETCH_S1p_D2_evn.state;
      }
      else {
        state_new.sfetch_control.SEBA_SFETCH_S1p_D5_odd.state = state_old.sfetch_control.VONU_SFETCH_S1p_D4_evn.state;
      }
    }

    state_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.state = sfetch_done_trig_new;

    state_new.tfetch_control.LONY_TFETCHINGp.state = !XYMU_RENDERINGn_new && state_new.phase_tfetch < 12;

    if (!NYXU_BFETCH_RSTn_new) {
      state_new.tfetch_control.LOVY_TFETCH_DONEp.state = 0;
    }
    else if (DELTA_ODD) {
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

    if (DELTA_EVEN) state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state = bit(state_old.tfetch_counter_odd, 0);
    if (XYMU_RENDERINGn_new) state_new.tfetch_control.LYZU_BFETCH_S0p_D1.state = 0;

    if (vid_rst_evn_new) {
      state_new.tfetch_control.PYGO_FETCH_DONEp_evn.state = 0;
    }
    else if (XYMU_RENDERINGn_new) {
      state_new.tfetch_control.PYGO_FETCH_DONEp_evn.state = 0;
    }
    else {
      if (DELTA_EVEN) {
        state_new.tfetch_control.PYGO_FETCH_DONEp_evn.state = state_old.tfetch_control.PORY_FETCH_DONEp_odd.state;
      }
    }

    state_new.win_ctrl.PUKU_WIN_HITn_odd.state = !state_new.win_ctrl.RYDY_WIN_HITp.state;
    if (vid_rst_evn_new) state_new.win_ctrl.PUKU_WIN_HITn_odd.state = 1;
    state_new.oam_ctrl.WUJE_CPU_OAM_WRn.state = !(cpu_addr_oam_new && cpu_wr && DELTA_DH);
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
    else if (DELTA_EVEN) {
      state_new.VOGA_HBLANKp = (!state_old.FEPO_STORE_MATCHp && (state_old.pix_count == 167));
    }

    state_new.sprite_scanner.FETO_SCAN_DONEp.state = state_new.scan_counter == 39;
    state_new.tfetch_control.LYRY_BFETCH_DONEp_odd.state = state_new.tfetch_counter_odd >= 5;


    if (!vid_rst_evn_new) {
      if (DELTA_FG) {
        state_new.lcd.SYGU_LINE_STROBE.state = (reg_lx_old == 0) || (reg_lx_old == 7) || (reg_lx_old == 45) || (reg_lx_old == 83);
      }

      if (state_old.lcd.RUTU_LINE_ENDp_odd.state && !state_new.lcd.RUTU_LINE_ENDp_odd.state) state_new.lcd.LUCA_LINE_EVENp.state = !state_new.lcd.LUCA_LINE_EVENp.state;
      if (!vblank_old && vblank_new) state_new.lcd.NAPO_FRAME_EVENp.state = !state_new.lcd.NAPO_FRAME_EVENp.state;

      if (state_old.lcd.NYPE_LINE_ENDp_odd.state && !state_new.lcd.NYPE_LINE_ENDp_odd.state) {
        state_new.lcd.MEDA_VSYNC_OUTn.state = reg_ly_new == 0;
      }

      if (bit(state_new.pix_count, 0) && bit(state_new.pix_count, 3)) state_new.lcd.WUSA_LCD_CLOCK_GATE.state = 1;
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

    if (DELTA_EVEN && clkpipe_gate) {
      state_new.lcd.PAHO_X8_SYNC.state = bit(state_old.pix_count, 3);
    }

    if (XYMU_RENDERINGn_new) {
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
      wire TYFA_CLKPIPE_evn_new = (!state_new.win_ctrl.RYDY_WIN_HITp.state && state_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.state && !state_old.FEPO_STORE_MATCHp && (state_old.pix_count != 167) && DELTA_EVEN);

      pins.lcd.PIN_50_LCD_DATA1.state = RAVO_LD1n;
      pins.lcd.PIN_51_LCD_DATA0.state = REMY_LD0n;
      pins.lcd.PIN_52_LCD_CNTRL.state = !state_new.lcd.SYGU_LINE_STROBE.state && !state_new.lcd.RUTU_LINE_ENDp_odd.state;
      pins.lcd.PIN_53_LCD_CLOCK.state = (!state_new.lcd.WUSA_LCD_CLOCK_GATE.state || (TYFA_CLKPIPE_evn_new && !state_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.state)) && (!state_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.state || state_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.state);
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
    state_new.WODU_HBLANKp_odd = WODU_HBLANK_new;
    state_new.ACYL_SCANNINGp_odd = (!MATU_DMA_RUNNINGp_odd_new && besu_scan_donen_odd_new && !vid_rst_evn_new);
    state_new.ATEJ_LINE_RSTp_odd = line_rst_odd_new || vid_rst_evn_new;
    state_new.cpu_signals.SIG_CPU_BOOTp.state = 0;
    state_new.cpu_signals.SIG_BOOT_CSp.state = 0;

    if (cpu_addr_new <= 0x00FF) {

      state_new.cpu_signals.SIG_CPU_BOOTp.state = !state_new.cpu_signals.TEPU_BOOT_BITn.state;

      if ((cpu.core.reg.bus_req_new.read && !DELTA_HA) && !state_new.cpu_signals.TEPU_BOOT_BITn.state) {
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

    state_new.oam_ctrl.old_oam_clk.state = state_new.oam_ctrl.SIG_OAM_CLKn.state; // Vestige of gate mode
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
    state_new.sys_clk.AVET_AxCxExGx.state = pins.sys.PIN_74_CLK.CLK.state; // dead signal
    state_new.sys_clk.ANOS_xBxDxFxH.state = !pins.sys.PIN_74_CLK.CLK.state; // dead signal

    state_new.sys_clk.AFUR_ABCDxxxx.set_state(gen_clk(phase_new, 0b11110000)); // dead signal
    state_new.sys_clk.ALEF_xBCDExxx.set_state(gen_clk(phase_new, 0b01111000)); // dead signal
    state_new.sys_clk.APUK_xxCDEFxx.set_state(gen_clk(phase_new, 0b00111100)); // dead signal
    state_new.sys_clk.ADYK_xxxDEFGx.set_state(gen_clk(phase_new, 0b00011110)); // dead signal

    pins.sys.PIN_75_CLK_OUT.state = gen_clk(phase_new, 0b00001111); // dead signal

    state_new.sys_clk.SIG_CPU_BOWA_Axxxxxxx.state = gen_clk(phase_new, 0b10000000); // dead signal
    state_new.sys_clk.SIG_CPU_BEDO_xBCDEFGH.state = gen_clk(phase_new, 0b01111111); // dead signal
    state_new.sys_clk.SIG_CPU_BEKO_ABCDxxxx.state = gen_clk(phase_new, 0b11110000); // dead signal
    state_new.sys_clk.SIG_CPU_BUDE_xxxxEFGH.state = gen_clk(phase_new, 0b00001111); // dead signal
    state_new.sys_clk.SIG_CPU_BOLO_ABCDEFxx.state = gen_clk(phase_new, 0b11111100); // dead signal
    state_new.sys_clk.SIG_CPU_BUKE_AxxxxxGH.state = gen_clk(phase_new, 0b10000011); // dead signal
    state_new.sys_clk.SIG_CPU_BOMA_xBCDEFGH.state = gen_clk(phase_new, 0b01111111); // dead signal
    state_new.sys_clk.SIG_CPU_BOGA_Axxxxxxx.state = gen_clk(phase_new, 0b10000000); // dead signal

    state_new.cpu_signals.TEDO_CPU_RDp.state = (cpu.core.reg.bus_req_new.read && !DELTA_HA); // dead signal
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