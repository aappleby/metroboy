#include "GateBoyLib/GateBoy.h"
#include <memory.h>
#include <stdio.h>

#include "metrolib/audio/Audio.h"
#include "metrolib/gameboy/GBConstants.h"
#include "metrolib/core/Tests.h"
#include "metrolib/core/Result.h"

#include "GateBoyLib/Probe.h"
#include "GateBoyLib/GateBoyState.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

FieldInfo GateBoyCpu::fields[] = {
  DECLARE_FIELD(GateBoyCpu, core.reg.bus_addr),
  DECLARE_FIELD(GateBoyCpu, core.reg.bus_data),
  DECLARE_FIELD(GateBoyCpu, core.reg.bus_read),
  DECLARE_FIELD(GateBoyCpu, core.reg.bus_write),

  DECLARE_FIELD(GateBoyCpu, core.reg.op_addr),
  DECLARE_FIELD(GateBoyCpu, core.reg.op_prev),
  DECLARE_FIELD(GateBoyCpu, core.reg.op_next),
  DECLARE_FIELD(GateBoyCpu, core.reg.op_cb),
  DECLARE_FIELD(GateBoyCpu, core.reg.op_state),
  DECLARE_FIELD(GateBoyCpu, core.reg.data_in),

  DECLARE_FIELD(GateBoyCpu, core.reg.ime),
  DECLARE_FIELD(GateBoyCpu, core.reg.ime_delay),
  DECLARE_FIELD(GateBoyCpu, core.reg.int_addr),
  DECLARE_FIELD(GateBoyCpu, core.reg.int_ack),

  DECLARE_FIELD(GateBoyCpu, core.reg.alu_f),
  DECLARE_FIELD(GateBoyCpu, core.reg.alu_o),

  DECLARE_FIELD(GateBoyCpu, core.reg.pcl),
  DECLARE_FIELD(GateBoyCpu, core.reg.pch),
  DECLARE_FIELD(GateBoyCpu, core.reg.spl),
  DECLARE_FIELD(GateBoyCpu, core.reg.sph),
  DECLARE_FIELD(GateBoyCpu, core.reg.xyl),
  DECLARE_FIELD(GateBoyCpu, core.reg.xyh),

  DECLARE_FIELD(GateBoyCpu, core.reg.c),
  DECLARE_FIELD(GateBoyCpu, core.reg.b),
  DECLARE_FIELD(GateBoyCpu, core.reg.e),
  DECLARE_FIELD(GateBoyCpu, core.reg.d),
  DECLARE_FIELD(GateBoyCpu, core.reg.l),
  DECLARE_FIELD(GateBoyCpu, core.reg.h),
  DECLARE_FIELD(GateBoyCpu, core.reg.f),
  DECLARE_FIELD(GateBoyCpu, core.reg.a),

  DECLARE_FIELD(GateBoyCpu, core.reg.bus_req_new.addr),
  DECLARE_FIELD(GateBoyCpu, core.reg.bus_req_new.data),
  DECLARE_FIELD(GateBoyCpu, core.reg.bus_req_new.read),
  DECLARE_FIELD(GateBoyCpu, core.reg.bus_req_new.write),

  DECLARE_FIELD(GateBoyCpu, core.reg.cpu_data_latch),
  DECLARE_FIELD(GateBoyCpu, core.reg.intf_latch),
  DECLARE_FIELD(GateBoyCpu, core.reg.halt_latch),

  END_FIELDS()
};

//-----------------------------------------------------------------------------

GBResult GateBoy::poweron(bool fastboot) {
  gb_state.poweron();
  cpu.poweron();
  mem.poweron();
  sys.poweron();
  pins.poweron();
  probes.reset();

  sys.fastboot = fastboot;
  sys.rst = 1;
  sys.gb_phase_total_old = 0;
  sys.gb_phase_total_new = 1;
  sys.in_por = 1;

  // These registers do not reset cleanly at poweron, so we're forcing them to a sane state.
#ifdef SIM_AUDIO
  gb_state.ch3.EFAR_WAVE_IDX0.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  gb_state.ch3.ERUS_WAVE_IDX1.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  gb_state.ch3.EFUZ_WAVE_IDX2.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  gb_state.ch3.EXEL_WAVE_IDX3.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  gb_state.ch3.EFAL_WAVE_IDX4.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  gb_state.ch3.FETY_WAVE_LOOP.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
#endif

  return GBResult::ok();
}

//-----------------------------------------------------------------------------

GBResult GateBoy::reset() {
  poweron(true);
  gb_state.reset();
  cpu.reset();
  mem.reset();
  sys.reset();
  pins.reset();
  probes.reset();

  return GBResult::ok();
}

//-----------------------------------------------------------------------------

GBResult GateBoy::peek(int addr) const {
  //if (addr >= 0x0000 && addr <= 0x7FFF) { return cart_blob.data()[addr - 0x0000]; }
  if (addr >= 0x8000 && addr <= 0x9FFF) { return GBResult(mem.vid_ram [addr - 0x8000]);  }
  if (addr >= 0xA000 && addr <= 0xBFFF) { return GBResult(mem.cart_ram[addr - 0xA000]); }
  if (addr >= 0xC000 && addr <= 0xDFFF) { return GBResult(mem.int_ram [addr - 0xC000]);  }
  if (addr >= 0xE000 && addr <= 0xFDFF) { return GBResult(mem.int_ram [addr - 0xE000]);  }
  if (addr >= 0xFE00 && addr <= 0xFEFF) { return GBResult(mem.oam_ram [addr - 0xFE00]);  }
  if (addr >= 0xFF30 && addr <= 0xFF3F) { return GBResult(mem.wave_ram[addr - 0xFF30]); }
  if (addr >= 0xFF80 && addr <= 0xFFFE) { return GBResult(mem.zero_ram[addr - 0xFF80]); }
  return gb_state.peek(addr);
}

GBResult GateBoy::poke(int addr, uint8_t data_in) {
  //if (addr >= 0x0000 && addr <= 0x7FFF) { cart_blob.data()[addr - 0x0000] = data_in; return data_in; }
  if (addr >= 0x8000 && addr <= 0x9FFF) { mem.vid_ram [addr - 0x8000] = data_in; return GBResult::ok(); }
  if (addr >= 0xA000 && addr <= 0xBFFF) { mem.cart_ram[addr - 0xA000] = data_in; return GBResult::ok(); }
  if (addr >= 0xC000 && addr <= 0xDFFF) { mem.int_ram [addr - 0xC000] = data_in; return GBResult::ok(); }
  if (addr >= 0xE000 && addr <= 0xFDFF) { mem.int_ram [addr - 0xE000] = data_in; return GBResult::ok(); }
  if (addr >= 0xFE00 && addr <= 0xFEFF) { mem.oam_ram [addr - 0xFE00] = data_in; return GBResult::ok(); }
  if (addr >= 0xFF30 && addr <= 0xFF3F) { mem.wave_ram[addr - 0xFF30] = data_in; return GBResult::ok(); }
  if (addr >= 0xFF80 && addr <= 0xFFFE) { mem.zero_ram[addr - 0xFF80] = data_in; return GBResult::ok(); }
  return gb_state.poke(addr, data_in);
}

//-----------------------------------------------------------------------------

GBResult GateBoy::dbg_req(uint16_t addr, uint8_t data, bool write) {
  CHECK_P((sys.gb_phase_total_old & 7) == 7);

  cpu.core.reg.bus_req_new.addr = addr;
  cpu.core.reg.bus_req_new.data = data;
  cpu.core.reg.bus_req_new.read = !write;
  cpu.core.reg.bus_req_new.write = write;

  return GBResult::ok();
}

//-----------------------------------------------------------------------------

GBResult GateBoy::dbg_read(const blob& cart_blob, int addr) {
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

//-----------------------------------------------------------------------------

GBResult GateBoy::dbg_write(const blob& cart_blob, int addr, uint8_t data) {
  CHECK_P((sys.gb_phase_total_old & 7) == 7);

  Req old_req = cpu.core.reg.bus_req_new;
  bool old_cpu_en = sys.cpu_en;
  sys.cpu_en = false;

  dbg_req((uint16_t)addr, data, 1);
  GBResult res = run_phases(cart_blob, 8);

  cpu.core.reg.bus_req_new = old_req;
  sys.cpu_en = old_cpu_en;
  return res;
}

//-----------------------------------------------------------------------------

GBResult GateBoy::run_phases(const blob& cart_blob, int phase_count) {
  GBResult res = GBResult::ok();
  for (int i = 0; i < phase_count; i++) {
    res &= next_phase(cart_blob);
  }
  return res;
}

//-----------------------------------------------------------------------------

GBResult GateBoy::next_phase(const blob& cart_blob) {
  //LOG_G("GateBoy::next_phase()\n");

  sys.gb_phase_total_new = sys.gb_phase_total_old + 1;

  if (sys.gb_phase_total_new == 7) {
    // Release reset, start clock, and sync with phase
    sys.rst = 0;
    sys.clk_en = 1;
    sys.clk_good = 1;
  }

  if (sys.gb_phase_total_new == 9) {
    CHECK_P(bit0(gb_state.sys_clk.AFUR_ABCDxxxx.qp_old()));
    CHECK_N(bit0(gb_state.sys_clk.ALEF_xBCDExxx.qp_old()));
    CHECK_N(bit0(gb_state.sys_clk.APUK_xxCDEFxx.qp_old()));
    CHECK_N(bit0(gb_state.sys_clk.ADYK_xxxDEFGx.qp_old()));
  }

  if (sys.gb_phase_total_new == 65) {
    // Wait for SIG_CPU_START
    CHECK_P(gb_state.sys_rst.SIG_CPU_STARTp.out_old());
  }

  if (sys.gb_phase_total_new == 80) {
    // Fetch the first instruction in the bootrom
    dbg_req(0x0000, 0, 0);
  }

  if (sys.gb_phase_total_new == 88) {
    // We're ready to go, release the CPU so it can start running the bootrom.

    sys.clk_req = 1;
    sys.in_por = false;

    if (sys.fastboot) {
      gb_state.reg_div.TERO_DIV03p.state = 0b00011010;
      gb_state.reg_div.UNYK_DIV04p.state = 0b00011010;
      gb_state.reg_div.UPOF_DIV15p.state = 0b00011011;
    }
  }

  probes.begin_pass(sys.gb_phase_total_new & 7);
  tock_cpu(cart_blob);
  tock_gates(cart_blob);
  probes.end_pass();

  commit_regs(gb_state);
  commit_regs(pins);

  if (config_idempotence) {
    //printf("idempotence?\n");
    auto gb_state_old = gb_state;

    tock_gates(cart_blob);
    commit_regs(gb_state);
    commit_regs(pins);

    if (gb_state.diff(gb_state_old, 0xFF)) {
      LOG_R("idempotence fail!\n");
      //debugbreak();
    }
  }

  update_framebuffer();

  if ((sys.gb_phase_total_new & 7) == 0) {
#if 0
    /*_p10.TACE*/ wire TACE_AMP_ENn =  and4(gb_state.ch1.HOCA_CH1_AMP_ENn_new(),
                                            gb_state.ch2.FUTE_CH2_AMP_ENn_new(),
                                            gb_state.ch3.GUXE_NR30_AMP_ENp.qn_new(),
                                            gb_state.ch4.GEVY_CH4_AMP_ENn());
#endif

#ifdef SIM_AUDIO
    sample_t l = spu_audio_out_l(gb_state);
    sample_t r = spu_audio_out_r(gb_state);

    mem.audio_1[(sys.gb_phase_total_old >> 9) & 0xFF] = ch1_audio_out(gb_state.ch1);
    mem.audio_2[(sys.gb_phase_total_old >> 9) & 0xFF] = ch2_audio_out(gb_state.ch2);
    mem.audio_3[(sys.gb_phase_total_old >> 9) & 0xFF] = ch3_audio_out(gb_state.ch3);
    mem.audio_4[(sys.gb_phase_total_old >> 9) & 0xFF] = ch4_audio_out(gb_state.ch4);

    mem.audio_l[(sys.gb_phase_total_old >> 7) & 0xFF] = l;
    mem.audio_r[(sys.gb_phase_total_old >> 7) & 0xFF] = r;

    audio_post(input_hz, l, r);
#endif
  }

  sys.gb_phase_total_old = sys.gb_phase_total_new;

  return GBResult::ok();
}

//-----------------------------------------------------------------------------

GBResult GateBoy::run_to(const blob& cart_blob, int phase) {
  while(get_sys().gb_phase_total_old < phase) {
    next_phase(cart_blob);
  }
  return GBResult::ok();
}

//-----------------------------------------------------------------------------

void GateBoy::update_framebuffer() {
  int lcd_x = bit_pack(gb_state.pix_count) - 8;
  int lcd_y = bit_pack(gb_state.reg_ly);
  int DATA0 = pins.lcd.PIN_51_LCD_DATA0.qp_ext_old();
  int DATA1 = pins.lcd.PIN_50_LCD_DATA1.qp_ext_old();

  if (lcd_y >= 0 && lcd_y < 144 && lcd_x >= 0 && lcd_x < 160) {
    wire p0 = bit0(DATA0);
    wire p1 = bit0(DATA1);
    auto new_pix = p0 + p1 * 2;

    mem.framebuffer[lcd_x + lcd_y * 160] = uint8_t(3 - new_pix);
  }
}











































































//-----------------------------------------------------------------------------

void GateBoy::tock_cpu(const blob& cart_blob) {

  //printf("gb %d %d %d\n", (int)sys.gb_phase_total_old, (int)sys.gb_phase_total_new, bit_pack(gb_state.cpu_dbus));

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

  cpu.core.reg.cpu_data_latch &= (uint8_t)bit_pack(gb_state.cpu_dbus);

  if (DELTA_AB) {
    if (sys.cpu_en && !sys.in_por) {
      cpu.core.execute((uint8_t)bit_pack(gb_state.reg_ie), cpu.core.reg.intf_latch);
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
    if (bit0(gb_state.reg_if.LOPE_FF0F_D0p.state)) cpu.core.reg.halt_latch |= INT_VBLANK_MASK;
    if (bit0(gb_state.reg_if.LALU_FF0F_D1p.state)) cpu.core.reg.halt_latch |= INT_STAT_MASK;
    if (bit0(gb_state.reg_if.UBUL_FF0F_D3p.state)) cpu.core.reg.halt_latch |= INT_SERIAL_MASK;
    if (bit0(gb_state.reg_if.ULAK_FF0F_D4p.state)) cpu.core.reg.halt_latch |= INT_JOYPAD_MASK;
    if (cpu.core.reg.op_next == 0x76 && (bit_pack(gb_state.reg_ie) & cpu.core.reg.halt_latch)) cpu.core.reg.op_state = 0; // +BC +CD +DE +EF +FG
    cpu.core.reg.halt_latch = 0; // +BC +CD +DE +EF +FG
  }

  if (DELTA_DE) {
  }

  if (DELTA_EF) {
    // -CD +DE +EF +FG
    if (cpu.core.reg.op_state == 0) {
      cpu.core.reg.op_addr = cpu.core.reg.bus_addr;
      cpu.core.reg.op_next = (uint8_t)bit_pack(gb_state.cpu_dbus);
    }
  }

  if (DELTA_FG) {
    cpu.core.reg.cpu_data_latch = 0xFF; // -DE +EF
  }

  if (DELTA_GH) {
    cpu.core.reg.intf_latch = (uint8_t)bit_pack(gb_state.reg_if); // -EF +FG +GH -HA
    if (bit0(gb_state.reg_if.NYBO_FF0F_D2p.state)) cpu.core.reg.halt_latch |= INT_TIMER_MASK; // +FG +GH -HA : this one latches funny, some hardware bug

    if (sys.cpu_en && !sys.in_por) {
      if (cpu.core.reg.op_state == 0) {
        if ((bit_pack(gb_state.reg_ie) & cpu.core.reg.intf_latch) && cpu.core.reg.ime) {
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

void GateBoy::tock_gates(const blob& cart_blob) {

  const GateBoyState  reg_old = gb_state;

  memset(&gb_state.cpu_abus,  BIT_NEW | BIT_PULLED | 1, sizeof(gb_state.cpu_abus));
  memset(&gb_state.cpu_dbus,  BIT_NEW | BIT_PULLED | 1, sizeof(gb_state.cpu_dbus));

  if (DELTA_DE || DELTA_EF || DELTA_FG || DELTA_GH) {
    // Data has to be driven on EFGH or we fail the wave tests
    gb_state.cpu_dbus.set_data(cpu.core.reg.bus_req_new.write, cpu.core.reg.bus_req_new.data_lo);
  }
  else {
    gb_state.cpu_dbus.set_data(false, 0);
  }





  GateBoyState& reg_new = gb_state;

  // Clear BIT_OLD from reg_new so we can't accidentally read old fields from it.
  bit_mask(reg_new, uint8_t(~BIT_OLD));
  bit_mask(pins, uint8_t(~BIT_OLD));



#if 1
  // SIG_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  // SIG_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp

  ///*_p07.TUNA*/ wire TUNA_0000_FDFF_old = nand7(
  //  reg_old.cpu_abus.BUS_CPU_A15p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A14p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A13p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A12p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A11p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A10p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A09p.out_old()
  //);
  ///*_p25.SYRO*/ wire SYRO_FE00_FFFF_old = not1(TUNA_0000_FDFF_old);

  ///*_p07.TULO*/ wire TULO_ADDR_BOOTROMp_old = nor8(
  //  reg_old.cpu_abus.BUS_CPU_A15p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A14p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A13p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A12p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A11p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A10p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A09p.out_old(),
  //  reg_old.cpu_abus.BUS_CPU_A08p.out_old()
  //);

  ///*_p07.TERA*/ wire TERA_BOOT_BITp_old  = not1(reg_old.cpu_signals.TEPU_BOOT_BITn.qp_old());
  ///*_p07.TUTU*/ wire TUTU_READ_BOOTROMp_old = and2(TERA_BOOT_BITp_old, TULO_ADDR_BOOTROMp_old);

#endif

  // SIG_IN_CPU_EXT_BUSp _must_ be high on xxCDEFGH, otherwise PIN_16_A15 would
  // be high while reading 0x0000-0x7FFF

#if 1
  {
    auto addr  = cpu.core.reg.bus_req_new.addr;
    auto read  = cpu.core.reg.bus_req_new.read;
    auto write = cpu.core.reg.bus_req_new.write;

    probe("cpu_read", read & 1);
    probe("cpu_write", write & 1);

    wire req_addr_boot = (addr >= 0x0000) && (addr <= 0x00FF) && !(reg_old.cpu_signals.TEPU_BOOT_BITn.state & 1);
    //wire req_addr_rom  = (addr >= 0x0000) && (addr <= 0x7FFF);
    wire req_addr_vram = (addr >= 0x8000) && (addr <= 0x9FFF);
    //wire req_addr_ram  = (addr >= 0xA000) && (addr <= 0xFDFF);
    //wire req_addr_oam  = (addr >= 0xFE00) && (addr <= 0xFEFF);
    wire req_addr_hi   = (addr >= 0xFE00);

    reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.state = ((DELTA_DH && (read || (write && addr >= 0xFF10))) & 1) | BIT_DRIVEN | BIT_NEW;

    if (DELTA_HA) {
      wire ext_addr_new = (read || write) && (!req_addr_hi && !req_addr_boot && !req_addr_vram);
      reg_new.cpu_abus.set_addr(cpu.core.reg.bus_req_new.addr & 0x00FF);
      reg_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = ext_addr_new | BIT_DRIVEN | BIT_NEW;
      reg_new.cpu_signals.SIG_IN_CPU_WRp.state = 0 | BIT_DRIVEN | BIT_NEW;
      reg_new.cpu_signals.SIG_IN_CPU_RDp.state = 0 | BIT_DRIVEN | BIT_NEW;
    }
    else if (DELTA_AB) {
      wire ext_addr_new = (read || write) && (!req_addr_hi && !req_addr_boot && !req_addr_vram);
      reg_new.cpu_abus.set_addr(cpu.core.reg.bus_req_new.addr);
      reg_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = ext_addr_new | BIT_DRIVEN | BIT_NEW;
      reg_new.cpu_signals.SIG_IN_CPU_WRp.state = write | BIT_DRIVEN | BIT_NEW;
      reg_new.cpu_signals.SIG_IN_CPU_RDp.state = read | BIT_DRIVEN | BIT_NEW;
    }
    else {
      wire ext_addr_new = (read || write) && (!req_addr_hi && !req_addr_boot);
      reg_new.cpu_abus.set_addr(cpu.core.reg.bus_req_new.addr);
      reg_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state = ext_addr_new | BIT_DRIVEN | BIT_NEW;
      reg_new.cpu_signals.SIG_IN_CPU_WRp.state = write | BIT_DRIVEN | BIT_NEW;
      reg_new.cpu_signals.SIG_IN_CPU_RDp.state = read | BIT_DRIVEN | BIT_NEW;
    }

  }

  probe("DBUS_FREE", reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.state & 1);
  probe("EXT_BUSp",  reg_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.state & 1);
  probe("BUS_RDp",   reg_new.cpu_signals.SIG_IN_CPU_RDp.state & 1);
  probe("BUS_WRp",   reg_new.cpu_signals.SIG_IN_CPU_WRp.state & 1);

#endif























  //printf("read_or_write = %d\n", read_or_write & 1);

  /*_p07.UJYV*/ wire UJYV_CPU_RDn = not1(reg_new.cpu_signals.SIG_IN_CPU_RDp.out_new());
  /*_p07.TEDO*/ reg_new.cpu_signals.TEDO_CPU_RDp <<= not1(UJYV_CPU_RDn);

  //----------------------------------------

  {
    wire EXT_sys_clkreq = bit0(sys.clk_req);
    wire EXT_sys_rst = bit0(~sys.rst);
    wire EXT_sys_t2 = bit0(~sys.t2);
    wire EXT_sys_t1 = bit0(~sys.t1);

    wire EXT_clkin = (sys.gb_phase_total_new & 1) && sys.clk_en;
    wire EXT_clkgood = bit0(~sys.clk_good);

    /*_PIN_74*/ pins.sys.PIN_74_CLK.pin_clk(EXT_clkin, EXT_clkgood);
    /*_PIN_71*/ pins.sys.PIN_71_RST.pin_in(EXT_sys_rst);
    /*_PIN_76*/ pins.sys.PIN_76_T2.pin_in(EXT_sys_t2);
    /*_PIN_77*/ pins.sys.PIN_77_T1.pin_in(EXT_sys_t1);

    auto cpu_ack = cpu.core.get_int_ack();
    wire EXT_ack_vblank = bit(cpu_ack, BIT_VBLANK);
    wire EXT_ack_stat = bit(cpu_ack, BIT_STAT);
    wire EXT_ack_timer = bit(cpu_ack, BIT_TIMER);
    wire EXT_ack_serial = bit(cpu_ack, BIT_SERIAL);
    wire EXT_ack_joypad = bit(cpu_ack, BIT_JOYPAD);

    /*_SIG_CPU_ACK_VBLANK*/ reg_new.cpu_ack.SIG_CPU_ACK_VBLANK.sig_in(EXT_ack_vblank);
    /*_SIG_CPU_ACK_STAT  */ reg_new.cpu_ack.SIG_CPU_ACK_STAT.sig_in(EXT_ack_stat);
    /*_SIG_CPU_ACK_TIMER */ reg_new.cpu_ack.SIG_CPU_ACK_TIMER.sig_in(EXT_ack_timer);
    /*_SIG_CPU_ACK_SERIAL*/ reg_new.cpu_ack.SIG_CPU_ACK_SERIAL.sig_in(EXT_ack_serial);
    /*_SIG_CPU_ACK_JOYPAD*/ reg_new.cpu_ack.SIG_CPU_ACK_JOYPAD.sig_in(EXT_ack_joypad);

    /*_SIG_CPU_CLKREQ*/ reg_new.sys_clk.SIG_CPU_CLKREQ.sig_in(EXT_sys_clkreq);

    /*_SIG_CPU_ADDR_HIp*/ reg_new.cpu_signals.SIG_CPU_ADDR_HIp.sig_out(reg_new.cpu_abus.SYRO_FE00_FFFF_new());
    /*_SIG_CPU_UNOR_DBG*/ reg_new.cpu_signals.SIG_CPU_UNOR_DBG.sig_out(pins.sys.UNOR_MODE_DBG2p_new());
    /*_SIG_CPU_UMUT_DBG*/ reg_new.cpu_signals.SIG_CPU_UMUT_DBG.sig_out(pins.sys.UMUT_MODE_DBG1p_new());
  }

  //----------------------------------------

  /*#p27.SYLO*/ wire SYLO_WIN_HITn_odd_old = not1(reg_old.win_ctrl.RYDY_WIN_HITp.out_old());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp_odd_old = not1(SYLO_WIN_HITn_odd_old);

  wire EXT_vcc = 1;
  wire EXT_gnd = 0;

  /*_SIG_VCC*/ reg_new.SIG_VCC.sig_in(EXT_vcc);
  /*_SIG_GND*/ reg_new.SIG_GND.sig_in(EXT_gnd);

  //----------------------------------------
  // Sys clock signals

  tock_clocks_gates(reg_old);

  {
    /*#p01.ADAR*/ wire ADAR_ABCxxxxH = not1(reg_new.sys_clk.ADYK_xxxDEFGx.qp_new());
    /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_new());
    /*#p01.AFAS*/ wire AFAS_xxxxEFGx = nor2(ADAR_ABCxxxxH, ATYP_ABCDxxxx);
    /*_p01.AREV*/ wire AREV_CPU_WRn = nand2(reg_new.cpu_signals.SIG_IN_CPU_WRp.out_new(), AFAS_xxxxEFGx);
    /*_p01.APOV*/ reg_new.cpu_signals.APOV_CPU_WRp <<= not1(AREV_CPU_WRn);
    /*_p07.UBAL*/ wire UBAL_CPU_WRn = not1(reg_new.cpu_signals.APOV_CPU_WRp.out_new());
    /*_p07.TAPU*/ reg_new.cpu_signals.TAPU_CPU_WRp <<= not1(UBAL_CPU_WRn); // xxxxEFGx
  }

  {
    /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_new());
    /*#p01.AJAX*/ wire AJAX_xxxxEFGH = not1(ATYP_ABCDxxxx);
    /*#p01.AROV*/ wire AROV_xxCDEFxx = not1(reg_new.sys_clk.APUK_xxCDEFxx.qn_new());
    /*#p01.AGUT*/ wire AGUT_xxCDEFGH = or_and3(AROV_xxCDEFxx, AJAX_xxxxEFGH, reg_new.cpu_signals.SIG_IN_CPU_EXT_BUSp.out_new());
    /*#p01.AWOD*/ wire AWOD_ABxxxxxx = nor2(pins.sys.UNOR_MODE_DBG2p_new(), AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ reg_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK <<= not1(AWOD_ABxxxxxx);
  }

  tock_div_gates(reg_old);
  tock_reset_gates(reg_old, bit0(sys.fastboot) ? reg_new.reg_div.TERO_DIV03p : reg_new.reg_div.UPOF_DIV15p);
  tock_lcdc_gates(reg_old); // LCDC has to be near the top as it controls the video reset signal
  tock_vid_clocks_gates(reg_old);
  tock_lyc_gates(reg_old);
  tock_lcd_gates(reg_old);
  tock_joypad_gates(reg_old);
  //tock_serial_gates(reg_old);
  tock_timer_gates(reg_old);
  tock_bootrom_gates(reg_old, mem.bootrom);
  tock_dma_gates(reg_old);

  /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(reg_new.sys_clk.AVET_AxCxExGx.out_new());
  /*_p01.AZOF*/ wire AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
  /*_p01.ZAXY*/ wire ZAXY_xBxDxFxH = not1(AZOF_AxCxExGx);
  /*#p01.ZEME*/ wire ZEME_AxCxExGx = not1(ZAXY_xBxDxFxH);
  /*#p01.ALET*/ wire ALET_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*_p01.LAPE*/ wire LAPE_AxCxExGx = not1(ALET_xBxDxFxH);
  /*_p27.TAVA*/ wire TAVA_xBxDxFxH = not1(LAPE_AxCxExGx);

  //----------------------------------------


  /*#p21.PARU*/ wire PARU_VBLANKp_odd_new = not1(reg_new.lcd.POPU_VBLANKp_odd.qn_new());
  /*_p27.REPU*/ wire REPU_VBLANKp_odd_new = or2(PARU_VBLANKp_odd_new, reg_new.PYRY_VID_RSTp_new());

  /*_p21.TADY*/ wire TADY_LINE_RSTn_odd_new = nor2(reg_new.ATEJ_LINE_RST_TRIGp_odd.out_new(), reg_new.TOFU_VID_RSTp_new());
  /*#p28.ANOM*/ wire ANOM_LINE_RSTn_odd_new = nor2(reg_new.ATEJ_LINE_RST_TRIGp_odd.out_new(), reg_new.ATAR_VID_RSTp_new());
  /*#p29.BALU*/ wire BALU_LINE_RSTp_odd_new = not1(ANOM_LINE_RSTn_odd_new);
  /*#p29.BAGY*/ wire BAGY_LINE_RSTn_odd_new = not1(BALU_LINE_RSTp_odd_new);
  /*_p27.XAHY*/ wire XAHY_LINE_RSTn_odd_new = not1(reg_new.ATEJ_LINE_RST_TRIGp_odd.out_new());

  //----------------------------------------
  // Sprite scanner

  {
    /*#p29.XUPY*/ wire XUPY_ABxxEFxx = not1(reg_new.sys_clk.WUVU_ABxxEFxx.qn_new());
    /*#p29.BYBA*/ reg_new.sprite_scanner.BYBA_SCAN_DONEp_odd.dff17(XUPY_ABxxEFxx, BAGY_LINE_RSTn_odd_new, reg_old.sprite_scanner.FETO_SCAN_DONEp.out_old());
    /*#p29.DOBA*/ reg_new.sprite_scanner.DOBA_SCAN_DONEp_evn.dff17(ALET_xBxDxFxH, BAGY_LINE_RSTn_odd_new, reg_old.sprite_scanner.BYBA_SCAN_DONEp_odd.qp_old());

    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_tn_odd_new = or3(reg_new.sprite_scanner.DOBA_SCAN_DONEp_evn.qp_new(), BALU_LINE_RSTp_odd_new, reg_new.sprite_scanner.BYBA_SCAN_DONEp_odd.qn_new());
    /*#p29.AVAP*/ reg_new.sprite_scanner.AVAP_SCAN_DONE_tp_odd <<= not1(BEBU_SCAN_DONE_tn_odd_new);

    /*#p28.ASEN*/ wire ASEN_SCAN_DONE_tp_odd_new = or2(reg_new.ATAR_VID_RSTp_new(), reg_new.sprite_scanner.AVAP_SCAN_DONE_tp_odd.out_new());
    /*#p28.BESU*/ reg_new.sprite_scanner.BESU_SCAN_DONEn_odd.nor_latch(reg_new.lcd.CATU_LINE_ENDp_odd.qp_new(), ASEN_SCAN_DONE_tp_odd_new);
    /*#p29.CENO*/ reg_new.sprite_scanner.CENO_SCAN_DONEn_odd.dff17(XUPY_ABxxEFxx, reg_new.ABEZ_VID_RSTn_new(), reg_old.sprite_scanner.BESU_SCAN_DONEn_odd.qp_old());

    /*#p28.BOGE*/ wire BOGE_DMA_RUNNINGn_odd = not1(reg_new.MATU_DMA_RUNNINGp_odd.qp_new());
    /*#p28.ACYL*/ reg_new.ACYL_SCANNINGp_odd <<= and2(BOGE_DMA_RUNNINGn_odd, reg_new.sprite_scanner.BESU_SCAN_DONEn_odd.qp_new());

    // Sprite scanner grabs the sprite index off the _old_ oam address bus

    /*#p30.CYKE*/ wire CYKE_ABxxEFxx = not1(XUPY_ABxxEFxx);
    /*#p30.WUDA*/ wire WUDA_xxCDxxGH = not1(CYKE_ABxxEFxx);

    /*_p28.YFOT*/ wire YFOT_OAM_A2p_old = not1(reg_old.oam_abus.BUS_OAM_A02n.out_old());
    /*_p28.YFOC*/ wire YFOC_OAM_A3p_old = not1(reg_old.oam_abus.BUS_OAM_A03n.out_old());
    /*_p28.YVOM*/ wire YVOM_OAM_A4p_old = not1(reg_old.oam_abus.BUS_OAM_A04n.out_old());
    /*_p28.YMEV*/ wire YMEV_OAM_A5p_old = not1(reg_old.oam_abus.BUS_OAM_A05n.out_old());
    /*_p28.XEMU*/ wire XEMU_OAM_A6p_old = not1(reg_old.oam_abus.BUS_OAM_A06n.out_old());
    /*_p28.YZET*/ wire YZET_OAM_A7p_old = not1(reg_old.oam_abus.BUS_OAM_A07n.out_old());
    /*_p30.XADU*/ reg_new.sprite_index.XADU_SPRITE_IDX0p_odd.dff13(WUDA_xxCDxxGH, reg_new.SIG_VCC.out_new(), YFOT_OAM_A2p_old);
    /*_p30.XEDY*/ reg_new.sprite_index.XEDY_SPRITE_IDX1p_odd.dff13(WUDA_xxCDxxGH, reg_new.SIG_VCC.out_new(), YFOC_OAM_A3p_old);
    /*_p30.ZUZE*/ reg_new.sprite_index.ZUZE_SPRITE_IDX2p_odd.dff13(WUDA_xxCDxxGH, reg_new.SIG_VCC.out_new(), YVOM_OAM_A4p_old);
    /*_p30.XOBE*/ reg_new.sprite_index.XOBE_SPRITE_IDX3p_odd.dff13(WUDA_xxCDxxGH, reg_new.SIG_VCC.out_new(), YMEV_OAM_A5p_old);
    /*_p30.YDUF*/ reg_new.sprite_index.YDUF_SPRITE_IDX4p_odd.dff13(WUDA_xxCDxxGH, reg_new.SIG_VCC.out_new(), XEMU_OAM_A6p_old);
    /*_p30.XECU*/ reg_new.sprite_index.XECU_SPRITE_IDX5p_odd.dff13(WUDA_xxCDxxGH, reg_new.SIG_VCC.out_new(), YZET_OAM_A7p_old);

    // FIXME is this OK without the second assignment of FETO?

    for (int feedback = 0; feedback < 2; feedback++) {
      // 32 + 4 + 2 + 1 = 39
      /*#p28.FETO*/ reg_new.sprite_scanner.FETO_SCAN_DONEp <<= and4(reg_new.scan_counter.YFEL_SCAN0_odd.qp_mid(), reg_new.scan_counter.WEWY_SCAN1_odd.qp_mid(), reg_new.scan_counter.GOSO_SCAN2_odd.qp_mid(), reg_new.scan_counter.FONY_SCAN5_odd.qp_mid());

      ///*#p29.XUPY*/ wire XUPY_ABxxEFxx = not1(reg_new.sys_clk.WUVU_ABxxEFxx.qn_new());
      /*#p28.GAVA*/ wire GAVA_SCAN_CLOCKp_odd = or2(reg_new.sprite_scanner.FETO_SCAN_DONEp.out_new(), XUPY_ABxxEFxx);

      /*#p28.YFEL*/ reg_new.scan_counter.YFEL_SCAN0_odd.dff17_any(GAVA_SCAN_CLOCKp_odd,                         ANOM_LINE_RSTn_odd_new, reg_old.scan_counter.YFEL_SCAN0_odd.qn_any());
      /*_p28.WEWY*/ reg_new.scan_counter.WEWY_SCAN1_odd.dff17_any(reg_new.scan_counter.YFEL_SCAN0_odd.qn_any(), ANOM_LINE_RSTn_odd_new, reg_old.scan_counter.WEWY_SCAN1_odd.qn_any());
      /*_p28.GOSO*/ reg_new.scan_counter.GOSO_SCAN2_odd.dff17_any(reg_new.scan_counter.WEWY_SCAN1_odd.qn_any(), ANOM_LINE_RSTn_odd_new, reg_old.scan_counter.GOSO_SCAN2_odd.qn_any());
      /*_p28.ELYN*/ reg_new.scan_counter.ELYN_SCAN3_odd.dff17_any(reg_new.scan_counter.GOSO_SCAN2_odd.qn_any(), ANOM_LINE_RSTn_odd_new, reg_old.scan_counter.ELYN_SCAN3_odd.qn_any());
      /*_p28.FAHA*/ reg_new.scan_counter.FAHA_SCAN4_odd.dff17_any(reg_new.scan_counter.ELYN_SCAN3_odd.qn_any(), ANOM_LINE_RSTn_odd_new, reg_old.scan_counter.FAHA_SCAN4_odd.qn_any());
      /*_p28.FONY*/ reg_new.scan_counter.FONY_SCAN5_odd.dff17_any(reg_new.scan_counter.FAHA_SCAN4_odd.qn_any(), ANOM_LINE_RSTn_odd_new, reg_old.scan_counter.FONY_SCAN5_odd.qn_any());
    }

    ///*#p28.FETO*/ sprite_scanner.FETO_SCAN_DONEp = and4(sprite_scanner.YFEL_SCAN0.qp_new(), sprite_scanner.WEWY_SCAN1.qp_new(), sprite_scanner.GOSO_SCAN2.qp_new(), sprite_scanner.FONY_SCAN5.qp_new());
  }


  //----------------------------------------
  // Global rendering flag 'XYMU'

  // So XYMU must somehow end up 1 during line 0.
  // WEGO could fire after AVAP_SCAN_DONE, or AVAP could fail to fire.
  // Or they could both fire on the same cycle and something weird happens?
  // If the scan failed to trigger on line 0, that might explain it...


  /*#p21.VOGA*/ reg_new.VOGA_HBLANKp_evn.dff17(ALET_xBxDxFxH, TADY_LINE_RSTn_odd_new, reg_old.WODU_HBLANK_GATEp_odd.out_old());
  /*#p21.WEGO*/ wire WEGO_HBLANKp_evn_new = or2(reg_new.TOFU_VID_RSTp_new(), reg_new.VOGA_HBLANKp_evn.qp_new());
  /*#p21.XYMU*/ reg_new.XYMU_RENDERING_LATCHn.nor_latch(WEGO_HBLANKp_evn_new, reg_new.sprite_scanner.AVAP_SCAN_DONE_tp_odd.out_new());

  /*_p24.LOBY*/ wire LOBY_RENDERINGn = not1(reg_new.XYMU_RENDERING_LATCHn.qn_new());
  /*#p27.PAHA*/ wire PAHA_RENDERINGn = not1(reg_new.XYMU_RENDERING_LATCHn.qn_new());
  /*_p29.TEPA*/ wire TEPA_RENDERINGn = not1(reg_new.XYMU_RENDERING_LATCHn.qn_new());

  //----------------------------------------
  // Sprite fetch sequencer

  {
    /*_p27.TUKU*/ wire TUKU_WIN_HITn_odd_old = not1(TOMU_WIN_HITp_odd_old);
    /*_p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn_evn_old = not1(reg_old.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.qp_old());
    /*_p27.TEKY*/ wire TEKY_SFETCH_REQp_odd_old = and4(reg_old.FEPO_STORE_MATCHp_odd.out_old(), TUKU_WIN_HITn_odd_old, reg_old.tfetch_control.LYRY_BFETCH_DONEp_odd.out_old(), SOWO_SFETCH_RUNNINGn_evn_old);

    /*_p27.SOBU*/ reg_new.sfetch_control.SOBU_SFETCH_REQp_evn  .dff17(TAVA_xBxDxFxH, reg_new.SIG_VCC.out_new(),              TEKY_SFETCH_REQp_odd_old);
    /*_p27.SUDA*/ reg_new.sfetch_control.SUDA_SFETCH_REQp_odd  .dff17(LAPE_AxCxExGx, reg_new.SIG_VCC.out_new(),              reg_old.sfetch_control.SOBU_SFETCH_REQp_evn.qp_old());
    /*#p29.TYFO*/ reg_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.dff17(LAPE_AxCxExGx, reg_new.SIG_VCC.out_new(),              reg_old.sfetch_counter_evn.TOXE_SFETCH_S0p_evn.qp_old());
    /*#p29.TOBU*/ reg_new.sfetch_control.TOBU_SFETCH_S1p_D2_evn.dff17(TAVA_xBxDxFxH, reg_new.XYMU_RENDERING_LATCHn.qn_new(), reg_old.sfetch_counter_evn.TULY_SFETCH_S1p_evn.qp_old());
    /*#p29.VONU*/ reg_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.dff17(TAVA_xBxDxFxH, reg_new.XYMU_RENDERING_LATCHn.qn_new(), reg_old.sfetch_control.TOBU_SFETCH_S1p_D2_evn.qp_old());
    /*#p29.SEBA*/ reg_new.sfetch_control.SEBA_SFETCH_S1p_D5_odd.dff17(LAPE_AxCxExGx, reg_new.XYMU_RENDERING_LATCHn.qn_new(), reg_old.sfetch_control.VONU_SFETCH_S1p_D4_evn.qp_old());

    /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp_evn_new = and2(reg_new.sfetch_control.SOBU_SFETCH_REQp_evn.qp_new(), reg_new.sfetch_control.SUDA_SFETCH_REQp_odd.qn_new());
    /*#p27.SECA*/ wire SECA_SFETCH_STARTn_evn_new = nor3(RYCE_SFETCH_TRIGp_evn_new, reg_new.ROSY_VID_RSTp_new(), reg_new.ATEJ_LINE_RST_TRIGp_odd.out_new());

    // Feedback loop
    for (int feedback = 0; feedback < 2; feedback++) {
      /*#p29.TAME*/ wire TAME_SFETCH_CLK_GATE = nand2(reg_new.sfetch_counter_evn.TESE_SFETCH_S2p_evn.qp_any(), reg_new.sfetch_counter_evn.TOXE_SFETCH_S0p_evn.qp_any());
      /*#p29.TOMA*/ wire TOMA_SFETCH_evn = nand2(LAPE_AxCxExGx, TAME_SFETCH_CLK_GATE);
      /*#p29.TOXE*/ reg_new.sfetch_counter_evn.TOXE_SFETCH_S0p_evn.dff17_any(TOMA_SFETCH_evn,                                         SECA_SFETCH_STARTn_evn_new, reg_new.sfetch_counter_evn.TOXE_SFETCH_S0p_evn.qn_any());
      /*#p29.TULY*/ reg_new.sfetch_counter_evn.TULY_SFETCH_S1p_evn.dff17_any(reg_new.sfetch_counter_evn.TOXE_SFETCH_S0p_evn.qn_any(), SECA_SFETCH_STARTn_evn_new, reg_new.sfetch_counter_evn.TULY_SFETCH_S1p_evn.qn_any());
      /*#p29.TESE*/ reg_new.sfetch_counter_evn.TESE_SFETCH_S2p_evn.dff17_any(reg_new.sfetch_counter_evn.TULY_SFETCH_S1p_evn.qn_any(), SECA_SFETCH_STARTn_evn_new, reg_new.sfetch_counter_evn.TESE_SFETCH_S2p_evn.qn_any());
    }

    /*_p29.SAKY*/ wire SAKY_SFETCHn_evn = nor2(reg_new.sfetch_counter_evn.TULY_SFETCH_S1p_evn.qp_new(), reg_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.qp_new());
    /*_p29.TYSO*/ wire TYSO_SFETCHINGn_evn = or2(SAKY_SFETCHn_evn, TEPA_RENDERINGn); // def or
    /*_p29.TEXY*/ reg_new.sfetch_control.TEXY_SFETCHINGp_evn <<= not1(TYSO_SFETCHINGn_evn);

    /*_p29.TYNO*/ wire TYNO = nand3(reg_new.sfetch_counter_evn.TOXE_SFETCH_S0p_evn.qp_new(), reg_new.sfetch_control.SEBA_SFETCH_S1p_D5_odd.qp_new(), reg_new.sfetch_control.VONU_SFETCH_S1p_D4_evn.qp_new());
    /*_p29.VUSA*/ wire VUSA_SPRITE_DONEn = or2(reg_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.qn_new(), TYNO);
    /*_p29.WUTY*/ reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd <<= not1(VUSA_SPRITE_DONEn);
  }

  /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp_evn_new = and2(reg_new.sfetch_control.SOBU_SFETCH_REQp_evn.qp_new(), reg_new.sfetch_control.SUDA_SFETCH_REQp_odd.qn_new());
  /*#p27.SECA*/ wire SECA_SFETCH_STARTn_evn = nor3(RYCE_SFETCH_TRIGp_evn_new, reg_new.ROSY_VID_RSTp_new(), reg_new.ATEJ_LINE_RST_TRIGp_odd.out_new());

  //----------------------------------------
  // Window state has some interaction with the tile fetcher here.























  // There's what looks like a combinatorial loop here, but it isn't actually a loop because half of it happens on odd phases and half happens on even phases.

  /*#p27.MEHE*/ wire MEHE_AxCxExGx = not1(ALET_xBxDxFxH);
  /*#p27.MYVO*/ wire MYVO_AxCxExGx = not1(ALET_xBxDxFxH);

  /*_p27.NUNU*/ reg_new.win_ctrl.NUNU_WIN_MATCHp_odd.dff17 (MEHE_AxCxExGx, reg_new.XAPO_VID_RSTn_new(), reg_old.win_ctrl.PYCO_WIN_MATCHp_evn.qp_old());
  /*_p27.NOPA*/ reg_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.dff17(ALET_xBxDxFxH, reg_new.XAPO_VID_RSTn_new(), reg_old.win_ctrl.PYNU_WIN_MODE_LATCHp.qp_old());

  /*#p27.XOFO*/ wire XOFO_WIN_RSTp = nand3(reg_new.reg_lcdc.WYMO_LCDC_WINENp.qp_new(), XAHY_LINE_RSTn_odd_new, reg_new.XAPO_VID_RSTn_new());
  /*_p27.PYNU*/ reg_new.win_ctrl.PYNU_WIN_MODE_LATCHp.nor_latch(reg_new.win_ctrl.NUNU_WIN_MATCHp_odd.qp_new(), XOFO_WIN_RSTp);

  /*#p27.NUNY*/ uint8_t NUNY_WIN_MODE_TRIGp_odd = and2(reg_new.win_ctrl.PYNU_WIN_MODE_LATCHp.qp_new(), reg_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.qn_new());
  /*_p27.NYFO*/ wire NYFO_WIN_MODE_tn_odd = not1(NUNY_WIN_MODE_TRIGp_odd);
  /*_p27.MOSU*/ wire MOSU_WIN_MODE_tp_odd = not1(NYFO_WIN_MODE_tn_odd);
  /*_p24.NAFY*/ wire NAFY_WIN_MODE_tn_odd = nor2(MOSU_WIN_MODE_tp_odd, LOBY_RENDERINGn);

  /*_p24.PYGO*/ reg_new.tfetch_control.PYGO_FETCH_DONEp_evn.dff17(ALET_xBxDxFxH, reg_new.XYMU_RENDERING_LATCHn.qn_new(),  reg_old.tfetch_control.PORY_FETCH_DONEp_odd.qp_old());
  /*_p24.PORY*/ reg_new.tfetch_control.PORY_FETCH_DONEp_odd.dff17(MYVO_AxCxExGx, NAFY_WIN_MODE_tn_odd,                    reg_old.tfetch_control.NYKA_FETCH_DONEp_evn.qp_old());
  /*_p24.NYKA*/ reg_new.tfetch_control.NYKA_FETCH_DONEp_evn.dff17(ALET_xBxDxFxH, NAFY_WIN_MODE_tn_odd,                    reg_old.tfetch_control.LYRY_BFETCH_DONEp_odd.out_old());

  /*#p27.NUNY*/ NUNY_WIN_MODE_TRIGp_odd = and2(reg_new.win_ctrl.PYNU_WIN_MODE_LATCHp.qp_new(), reg_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.qn_new());
  /*_p27.RYDY*/ reg_new.win_ctrl.RYDY_WIN_HITp <<= nor3(reg_new.win_ctrl.PUKU_WIN_HITn_odd.out_mid(), reg_new.tfetch_control.PORY_FETCH_DONEp_odd.qp_new(), reg_new.PYRY_VID_RSTp_new());
  /*_p27.PUKU*/ reg_new.win_ctrl.PUKU_WIN_HITn_odd <<= nor2(reg_new.win_ctrl.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp_odd);
  /*_p27.RYDY*/ reg_new.win_ctrl.RYDY_WIN_HITp <<= nor3(reg_new.win_ctrl.PUKU_WIN_HITn_odd.out_mid(), reg_new.tfetch_control.PORY_FETCH_DONEp_odd.qp_new(), reg_new.PYRY_VID_RSTp_new());
  /*_p27.PUKU*/ reg_new.win_ctrl.PUKU_WIN_HITn_odd <<= nor2(reg_new.win_ctrl.RYDY_WIN_HITp.out_mid(), NUNY_WIN_MODE_TRIGp_odd);















  /*_p24.POKY*/ reg_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.nor_latch(reg_new.tfetch_control.PYGO_FETCH_DONEp_evn.qp_new(), LOBY_RENDERINGn);


  /*_p27.SOVY*/ reg_new.win_ctrl.SOVY_WIN_HITp.dff17(ALET_xBxDxFxH, reg_new.XAPO_VID_RSTn_new(), reg_old.win_ctrl.RYDY_WIN_HITp.out_old());

  /*#p27.SYLO*/ wire SYLO_WIN_HITn_odd_new = not1(reg_new.win_ctrl.RYDY_WIN_HITp.out_new());
  /*#p24.TOMU*/ wire TOMU_WIN_HITp_odd_new = not1(SYLO_WIN_HITn_odd_new);
  /*#p24.SOCY*/ wire SOCY_WIN_HITn_odd_new = not1(TOMU_WIN_HITp_odd_new);

  // CLKPIPE is an even clock, it can only go high on even deltas. FEPO/WODU/SOCY are odd signals, they stay constant during even deltas.
  // SO, it is guaranteed safe to use the old values of FEPO/WODU/SOCY to compute CLKPIPE
  /*#p24.VYBO*/ wire VYBO_CLKPIPE_evn_new = nor3(reg_old.FEPO_STORE_MATCHp_odd.out_old(), reg_old.WODU_HBLANK_GATEp_odd.out_old(), MYVO_AxCxExGx);
  /*#p24.TYFA*/ wire TYFA_CLKPIPE_evn_new = and3(SOCY_WIN_HITn_odd_new, reg_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.qp_new(), VYBO_CLKPIPE_evn_new);
  /*#p24.SEGU*/ wire SEGU_CLKPIPE_odd_new = not1(TYFA_CLKPIPE_evn_new);
  /*_p27.ROCO*/ wire ROCO_CLKPIPE_evn_new = not1(SEGU_CLKPIPE_odd_new);
  /*#p24.ROXO*/ wire ROXO_CLKPIPE_evn_new = not1(SEGU_CLKPIPE_odd_new);


  /*_p27.ROMO*/ wire ROMO_PRELOAD_DONEn_evn_new = not1(reg_new.tfetch_control.POKY_PRELOAD_LATCHp_evn.qp_new());
  /*_p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn_xxx_new = nand4(
    reg_new.XYMU_RENDERING_LATCHn.qn_new(),
    ROMO_PRELOAD_DONEn_evn_new,
    reg_new.tfetch_control.NYKA_FETCH_DONEp_evn.qp_new(),
    reg_new.tfetch_control.PORY_FETCH_DONEp_odd.qp_new());
  /*_p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp_xxx_new = not1(SUVU_PRELOAD_DONE_TRIGn_xxx_new);

  /*_p27.VEKU*/ wire VEKU_SFETCH_ENDn_xxx_new = nor2(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), TAVE_PRELOAD_DONE_TRIGp_xxx_new); // def nor
  /*_p27.TAKA*/ reg_new.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.nand_latch(SECA_SFETCH_STARTn_evn, VEKU_SFETCH_ENDn_xxx_new);

  //----------------------------------------
  // OAM temp register B stores sprite X coordinate during scan, so we need a copy of it for later.

  auto oam_temp_b_old = reg_old.oam_temp_b;

  //----------------------------------------
  // OAM latch from last cycle gets moved into temp registers.

  {
    /*_p01.ABOL*/ wire ABOL_CLKREQn  = not1(reg_new.sys_clk.SIG_CPU_CLKREQ.out_new());
    /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_new());
    /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);
    /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
    /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
    /*_p01.UVYT*/ wire UVYT_ABCDxxxx = not1(BUDE_xxxxEFGH);
    /*_p04.MOPA*/ wire MOPA_xxxxEFGH = not1(UVYT_ABCDxxxx);

    /*_p25.CUFE*/ wire CUFE_OAM_CLKp = not_or_and3(reg_new.cpu_abus.SARO_ADDR_OAMp_new(), reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), MOPA_xxxxEFGH); // CUFE looks like BYHA minus an inverter

    /*#p29.WOJO*/ wire WOJO_AxxxExxx = nor2(reg_new.sys_clk.WOSU_AxxDExxH.qn_new(), reg_new.sys_clk.WUVU_ABxxEFxx.qn_new());
    /*_p29.XYSO*/ wire XYSO_xBCDxFGH = not1(WOJO_AxxxExxx);
    /*_p25.AVER*/ wire AVER_AxxxExxx = nand2(reg_new.ACYL_SCANNINGp_odd.out_new(), XYSO_xBCDxFGH);
    /*_p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(reg_new.sfetch_counter_evn.TOXE_SFETCH_S0p_evn.qp_new());
    /*_p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(reg_new.sfetch_control.TYFO_SFETCH_S0p_D1_odd.qp_new(), TYTU_SFETCH_S0n);
    /*_p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor3(TEPA_RENDERINGn, reg_new.sfetch_counter_evn.TULY_SFETCH_S1p_evn.qp_new(), reg_new.sfetch_counter_evn.TESE_SFETCH_S2p_evn.qp_new());
    /*_p25.VAPE*/ wire VAPE_OAM_CLKENn = and2(TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
    /*_p25.XUJY*/ wire XUJY_OAM_CLKENp = not1(VAPE_OAM_CLKENn);
    /*_p25.BYCU*/ wire BYCU_OAM_CLKp = nand3(AVER_AxxxExxx, XUJY_OAM_CLKENp, CUFE_OAM_CLKp);
    /*_p25.COTA*/ wire COTA_OAM_CLKn = not1(BYCU_OAM_CLKp);
    oam_latch_to_temp_a_gates(reg_old, COTA_OAM_CLKn);
    oam_latch_to_temp_b_gates(reg_old, COTA_OAM_CLKn);
  }

  //----------------------------------------
  // Sprite scanner triggers the sprite store clock, increments the sprite counter, and puts the sprite in the sprite store if it overlaps the current LCD Y coordinate.

  SpriteDeltaY sprite_delta_y = sub_sprite_y_gates();

  {
    ///*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(reg_new.sys_clk.AVET_AxCxExGx.out_new());
    ///*_p01.AZOF*/ wire AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
    ///*_p01.ZAXY*/ wire ZAXY_xBxDxFxH = not1(AZOF_AxCxExGx);
    ///*#p01.ZEME*/ wire ZEME_AxCxExGx = not1(ZAXY_xBxDxFxH);

    /*_p29.GACE*/ wire GACE_SPRITE_DELTA4 = not1(sprite_delta_y.GOPU_YDIFF4.sum);
    /*_p29.GUVU*/ wire GUVU_SPRITE_DELTA5 = not1(sprite_delta_y.FUWA_YDIFF5.sum);
    /*_p29.GYDA*/ wire GYDA_SPRITE_DELTA6 = not1(sprite_delta_y.GOJU_YDIFF6.sum);
    /*_p29.GEWY*/ wire GEWY_SPRITE_DELTA7 = not1(sprite_delta_y.WUHU_YDIFF7.sum);

    /*#p29.GOVU*/ wire GOVU_SPSIZE_MATCH = or2(reg_new.reg_lcdc.XYMO_LCDC_SPSIZEp.qp_new(), sprite_delta_y.GYKY_YDIFF3.sum);
    /*_p29.WOTA*/ wire WOTA_SCAN_MATCH_Yn = nand6(GACE_SPRITE_DELTA4, GUVU_SPRITE_DELTA5, GYDA_SPRITE_DELTA6, GEWY_SPRITE_DELTA7, sprite_delta_y.WUHU_YDIFF7.carry, GOVU_SPSIZE_MATCH);
    /*_p29.GESE*/ wire GESE_SCAN_MATCH_Yp = not1(WOTA_SCAN_MATCH_Yn);
    /*_p29.CEHA*/ wire CEHA_SCANNINGp_odd = not1(reg_new.sprite_scanner.CENO_SCAN_DONEn_odd.qn_new());
    /*#p29.XOCE*/ wire XOCE_xBCxxFGx = not1(reg_new.sys_clk.WOSU_AxxDExxH.qp_new());
    /*_p29.CARE*/ wire CARE_COUNT_CLKn_evn = and3(XOCE_xBCxxFGx, CEHA_SCANNINGp_odd, GESE_SCAN_MATCH_Yp); // Dots on VCC, this is AND. Die shot and schematic wrong.
    /*_p29.DYTY*/ wire DYTY_COUNT_CLKp_evn = not1(CARE_COUNT_CLKn_evn);
    /*_p29.DEZY*/ reg_new.sprite_scanner.DEZY_INC_COUNTn_odd.dff17_any(ZEME_AxCxExGx, reg_new.XAPO_VID_RSTn_new(), DYTY_COUNT_CLKp_evn);

    // There's a feedback loop here, but we don't actually need to loop - BAKY holds the clock line high once the sprite store is full, so doing a second logic pass
    // doesn't actually change any of the dffs.
    /*#p29.BAKY*/ wire BAKY_SPRITES_FULL_new = and2(reg_new.sprite_counter.CUXY_SPRITE_COUNT1_odd.qp_any(), reg_new.sprite_counter.DYBE_SPRITE_COUNT3_odd.qp_any());
    /*#p29.CAKE*/ wire CAKE_COUNT_CLKp_new = or2(BAKY_SPRITES_FULL_new, reg_new.sprite_scanner.DEZY_INC_COUNTn_odd.qp_any());
    /*#p28.AZYB*/ wire AZYB_LINE_TRIGn = not1(reg_new.ATEJ_LINE_RST_TRIGp_odd.out_new());
    /*_p29.BESE*/ reg_new.sprite_counter.BESE_SPRITE_COUNT0_odd.dff17_any(CAKE_COUNT_CLKp_new,                                    AZYB_LINE_TRIGn, reg_new.sprite_counter.BESE_SPRITE_COUNT0_odd.qn_any());
    /*_p29.CUXY*/ reg_new.sprite_counter.CUXY_SPRITE_COUNT1_odd.dff17_any(reg_new.sprite_counter.BESE_SPRITE_COUNT0_odd.qn_any(), AZYB_LINE_TRIGn, reg_new.sprite_counter.CUXY_SPRITE_COUNT1_odd.qn_any());
    /*_p29.BEGO*/ reg_new.sprite_counter.BEGO_SPRITE_COUNT2_odd.dff17_any(reg_new.sprite_counter.CUXY_SPRITE_COUNT1_odd.qn_any(), AZYB_LINE_TRIGn, reg_new.sprite_counter.BEGO_SPRITE_COUNT2_odd.qn_any());
    /*_p29.DYBE*/ reg_new.sprite_counter.DYBE_SPRITE_COUNT3_odd.dff17_any(reg_new.sprite_counter.BEGO_SPRITE_COUNT2_odd.qn_any(), AZYB_LINE_TRIGn, reg_new.sprite_counter.DYBE_SPRITE_COUNT3_odd.qn_any());

    /*_p01.AMYG*/ wire AMYG_VID_RSTp_new = not1(reg_new.XAPO_VID_RSTn_new());
    /*_p28.ABAK*/ wire ABAK_LINE_RSTp = or2(reg_new.ATEJ_LINE_RST_TRIGp_odd.out_new(), AMYG_VID_RSTp_new);
    /*_p28.BYVA*/ wire BYVA_LINE_RSTn = not1(ABAK_LINE_RSTp);

    //update_sprite_reset_flags(sfetch_control.WUTY_SFETCH_DONE_TRIGp, BYVA_LINE_RSTn, sprite_match_flags, sprite_reset_flags);

    /*_p29.EBOJ*/ reg_new.sprite_reset_flags_odd.EBOJ_STORE0_RSTp.dff17(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags_odd.GUVA_SPRITE0_GETp.out_old());
    /*_p29.CEDY*/ reg_new.sprite_reset_flags_odd.CEDY_STORE1_RSTp.dff17(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags_odd.ENUT_SPRITE1_GETp.out_old());
    /*_p29.EGAV*/ reg_new.sprite_reset_flags_odd.EGAV_STORE2_RSTp.dff17(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags_odd.EMOL_SPRITE2_GETp.out_old());
    /*_p29.GOTA*/ reg_new.sprite_reset_flags_odd.GOTA_STORE3_RSTp.dff17(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags_odd.GYFY_SPRITE3_GETp.out_old());
    /*_p29.XUDY*/ reg_new.sprite_reset_flags_odd.XUDY_STORE4_RSTp.dff17(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags_odd.GONO_SPRITE4_GETp.out_old());
    /*_p29.WAFY*/ reg_new.sprite_reset_flags_odd.WAFY_STORE5_RSTp.dff17(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags_odd.GEGA_SPRITE5_GETp.out_old());
    /*_p29.WOMY*/ reg_new.sprite_reset_flags_odd.WOMY_STORE6_RSTp.dff17(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags_odd.XOJA_SPRITE6_GETp.out_old());
    /*_p29.WAPO*/ reg_new.sprite_reset_flags_odd.WAPO_STORE7_RSTp.dff17(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags_odd.GUTU_SPRITE7_GETp.out_old());
    /*_p29.EXUQ*/ reg_new.sprite_reset_flags_odd.EXUQ_STORE8_RSTp.dff17(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags_odd.FOXA_SPRITE8_GETp.out_old());
    /*_p29.FONO*/ reg_new.sprite_reset_flags_odd.FONO_STORE9_RSTp.dff17(reg_new.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), BYVA_LINE_RSTn, reg_old.sprite_match_flags_odd.GUZE_SPRITE9_GETp.out_old());

    store_sprite_gates(reg_old, reg_new.sprite_counter, reg_new.sprite_reset_flags_odd, BYVA_LINE_RSTn, oam_temp_b_old, DYTY_COUNT_CLKp_evn);
  }

  //----------------------------------------
  // Fine scroll match, sprite store match, clock pipe, and pixel counter are intertwined here.

  auto XYDO_PX3p_old = reg_old.pix_count.XYDO_PX3p_odd;

  // Weird feedback loop here - fine scroll and pix count affect store match, which affects pipe clock, which affects fine scroll and pix count.
  // Implementing the feedback doesn't seem to change any test results, but... paranoia...

  //for (int feedback = 0; feedback < 2; feedback++) {
  {
    /*_p27.SUHA*/ wire SUHA_SCX_FINE_MATCHp_old_odd = xnor2(reg_old.reg_scx.DATY_SCX0p.qp_old(), reg_old.fine_count_odd.RYKU_FINE_CNT0_odd.qp_old());
    /*_p27.SYBY*/ wire SYBY_SCX_FINE_MATCHp_old_odd = xnor2(reg_old.reg_scx.DUZU_SCX1p.qp_old(), reg_old.fine_count_odd.ROGA_FINE_CNT1_odd.qp_old());
    /*_p27.SOZU*/ wire SOZU_SCX_FINE_MATCHp_old_odd = xnor2(reg_old.reg_scx.CYXU_SCX2p.qp_old(), reg_old.fine_count_odd.RUBU_FINE_CNT2_odd.qp_old());
    /*#p27.RONE*/ wire RONE_SCX_FINE_MATCHn_old_odd = nand4(reg_old.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_old(), SUHA_SCX_FINE_MATCHp_old_odd, SYBY_SCX_FINE_MATCHp_old_odd, SOZU_SCX_FINE_MATCHp_old_odd);
    /*#p27.POHU*/ wire POHU_SCX_FINE_MATCHp_old_odd = not1(RONE_SCX_FINE_MATCHn_old_odd);

    /*_p29.CEHA*/ wire CEHA_SCANNINGp_odd  = not1(reg_new.sprite_scanner.CENO_SCAN_DONEn_odd.qn_new());
    /*#p29.BYJO*/ wire BYJO_SCANNINGn_odd  = not1(CEHA_SCANNINGp_odd);
    /*#p29.AZEM*/ wire AZEM_RENDERINGp_odd = and2(reg_new.XYMU_RENDERING_LATCHn.qn_new(), BYJO_SCANNINGn_odd);
    /*#p29.AROR*/ wire AROR_MATCH_ENp_odd  = and2(AZEM_RENDERINGp_odd, reg_new.reg_lcdc.XYLO_LCDC_SPENp.qp_new());

    /*_p27.MOXE*/ wire MOXE_AxCxExGx = not1(ALET_xBxDxFxH);

    /*#p27.PUXA*/ reg_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.dff17_any(ROXO_CLKPIPE_evn_new,           reg_new.XYMU_RENDERING_LATCHn.qn_new(), POHU_SCX_FINE_MATCHp_old_odd);
    /*#p27.NYZE*/ reg_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.dff17_any(MOXE_AxCxExGx, reg_new.XYMU_RENDERING_LATCHn.qn_new(), reg_old.fine_scroll.PUXA_SCX_FINE_MATCH_evn.qp_old());

    /*#p27.POVA*/ wire POVA_FINE_MATCH_TRIGp_evn = and2(reg_new.fine_scroll.PUXA_SCX_FINE_MATCH_evn.qp_any(), reg_new.fine_scroll.NYZE_SCX_FINE_MATCH_odd.qn_any());
    /*#p27.ROXY*/ reg_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.nor_latch(PAHA_RENDERINGn, POVA_FINE_MATCH_TRIGp_evn);

    // SACU high on ACEG, gated high

    /*#p24.SACU*/ wire SACU_CLKPIPE_odd_new = or2(SEGU_CLKPIPE_odd_new, reg_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_any());

    // Pixel counter, has carry lookahead because this can increment every tcycle
    /*_p21.RYBO*/ wire RYBO_old = xor2(reg_old.pix_count.XEHO_PX0p_odd.qp_any(), reg_old.pix_count.SAVY_PX1p_odd.qp_any()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /*_p21.XUKE*/ wire XUKE_old = and2(reg_old.pix_count.XEHO_PX0p_odd.qp_any(), reg_old.pix_count.SAVY_PX1p_odd.qp_any());
    /*_p21.XYLE*/ wire XYLE_old = and2(reg_old.pix_count.XODU_PX2p_odd.qp_any(), XUKE_old);
    /*_p21.XEGY*/ wire XEGY_old = xor2(reg_old.pix_count.XODU_PX2p_odd.qp_any(), XUKE_old); // feet facing gnd
    /*_p21.XORA*/ wire XORA_old = xor2(reg_old.pix_count.XYDO_PX3p_odd.qp_any(), XYLE_old); // feet facing gnd

    /*_p21.XEHO*/ reg_new.pix_count.XEHO_PX0p_odd.dff17_any(SACU_CLKPIPE_odd_new, TADY_LINE_RSTn_odd_new, reg_new.pix_count.XEHO_PX0p_odd.qn_any());
    /*_p21.SAVY*/ reg_new.pix_count.SAVY_PX1p_odd.dff17_any(SACU_CLKPIPE_odd_new, TADY_LINE_RSTn_odd_new, RYBO_old);
    /*_p21.XODU*/ reg_new.pix_count.XODU_PX2p_odd.dff17_any(SACU_CLKPIPE_odd_new, TADY_LINE_RSTn_odd_new, XEGY_old);
    /*_p21.XYDO*/ reg_new.pix_count.XYDO_PX3p_odd.dff17_any(SACU_CLKPIPE_odd_new, TADY_LINE_RSTn_odd_new, XORA_old);

    /*_p24.TOCA*/ wire TOCA_new = not1(reg_new.pix_count.XYDO_PX3p_odd.qp_any());
    /*_p21.SAKE*/ wire SAKE_old = xor2(reg_old.pix_count.TUHU_PX4p_odd.qp_any(), reg_old.pix_count.TUKY_PX5p_odd.qp_any());
    /*_p21.TYBA*/ wire TYBA_old = and2(reg_old.pix_count.TUHU_PX4p_odd.qp_any(), reg_old.pix_count.TUKY_PX5p_odd.qp_any());
    /*_p21.SURY*/ wire SURY_old = and2(reg_old.pix_count.TAKO_PX6p_odd.qp_any(), TYBA_old);
    /*_p21.TYGE*/ wire TYGE_old = xor2(reg_old.pix_count.TAKO_PX6p_odd.qp_any(), TYBA_old);
    /*_p21.ROKU*/ wire ROKU_old = xor2(reg_old.pix_count.SYBE_PX7p_odd.qp_any(), SURY_old);

    /*_p21.TUHU*/ reg_new.pix_count.TUHU_PX4p_odd.dff17_any(TOCA_new, TADY_LINE_RSTn_odd_new, reg_new.pix_count.TUHU_PX4p_odd.qn_any());
    /*_p21.TUKY*/ reg_new.pix_count.TUKY_PX5p_odd.dff17_any(TOCA_new, TADY_LINE_RSTn_odd_new, SAKE_old);
    /*_p21.TAKO*/ reg_new.pix_count.TAKO_PX6p_odd.dff17_any(TOCA_new, TADY_LINE_RSTn_odd_new, TYGE_old);
    /*_p21.SYBE*/ reg_new.pix_count.SYBE_PX7p_odd.dff17_any(TOCA_new, TADY_LINE_RSTn_odd_new, ROKU_old);

    get_sprite_match_flags_gates(AROR_MATCH_ENp_odd, reg_new.sprite_match_flags_odd); // this changes FEPO
  }

  //=================
  //=================
  //=================
  //=================

  // Pix counter triggers HBLANK if there's no sprite store match and enables the pixel pipe clocks for later
  {
    /*#p21.XENA*/ wire XENA_STORE_MATCHn_odd = not1(reg_new.FEPO_STORE_MATCHp_odd.out_new());
    /*#p21.XUGU*/ wire XUGU_PX167n_odd = nand5(
      reg_new.pix_count.XEHO_PX0p_odd.qp_new(),
      reg_new.pix_count.SAVY_PX1p_odd.qp_new(),
      reg_new.pix_count.XODU_PX2p_odd.qp_new(),
      reg_new.pix_count.TUKY_PX5p_odd.qp_new(),
      reg_new.pix_count.SYBE_PX7p_odd.qp_new()); // 128 + 32 + 4 + 2 + 1 = 167
    /*#p21.XANO*/ wire XANO_PX167p_odd = not1(XUGU_PX167n_odd);
    /*#p21.WODU*/ reg_new.WODU_HBLANK_GATEp_odd <<= and2(XENA_STORE_MATCHn_odd, XANO_PX167p_odd); // WODU goes high on odd, cleared on H
  }

  /*_p27.PYCO*/ reg_new.win_ctrl.PYCO_WIN_MATCHp_evn.dff17(ROCO_CLKPIPE_evn_new, reg_new.XAPO_VID_RSTn_new(), reg_old.win_ctrl.NUKO_WX_MATCHp_odd.out_old());


  //=================
  //=================
  //=================
  //=================




  /*#p24.SACU*/ wire SACU_CLKPIPE_odd_new = or2(SEGU_CLKPIPE_odd_new, reg_new.fine_scroll.ROXY_FINE_SCROLL_DONEn.qp_new());

  /*_p24.PAHO*/ reg_new.lcd.PAHO_X8_SYNC.dff17(ROXO_CLKPIPE_evn_new, reg_new.XYMU_RENDERING_LATCHn.qn_new(), XYDO_PX3p_old.qp_old());

  //----------------------------------------

  memset(&reg_new.sprite_ibus, BIT_NEW | BIT_PULLED | 1, sizeof(reg_new.sprite_ibus));
  memset(&reg_new.sprite_lbus, BIT_NEW | BIT_PULLED | 1, sizeof(reg_new.sprite_lbus));

  sprite_match_to_bus_gates(reg_new.sprite_match_flags_odd);
  sprite_scan_to_bus_gates(sprite_delta_y, reg_new.XYMU_RENDERING_LATCHn, reg_new.FEPO_STORE_MATCHp_odd);

  //----------------------------------------
  // WY/WX/window match


  tock_window_gates(reg_old, SEGU_CLKPIPE_odd_new, REPU_VBLANKp_odd_new);

  //----------------------------------------
  // Tile fetch sequencer

  /*_p27.LYZU*/ reg_new.tfetch_control.LYZU_BFETCH_S0p_D1.dff17(ALET_xBxDxFxH, reg_new.XYMU_RENDERING_LATCHn.qn_new(), reg_old.tfetch_counter.LAXU_BFETCH_S0p_odd.qp_old());

  /*_p27.SEKO*/ wire SEKO_WIN_FETCH_TRIGp_old = nor2(reg_old.win_ctrl.RYFA_WIN_FETCHn_A.qn_old(), reg_old.win_ctrl.RENE_WIN_FETCHn_B.qp_old());
  /*_p27.TUXY*/ wire TUXY_WIN_FIRST_TILEne_old = nand2(SYLO_WIN_HITn_odd_old, reg_old.win_ctrl.SOVY_WIN_HITp.qp_old());
  /*_p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne_old = not1(TUXY_WIN_FIRST_TILEne_old);
  /*_p27.ROMO*/ wire ROMO_PRELOAD_DONEn_evn_old = not1(reg_old.tfetch_control.POKY_PRELOAD_LATCHp_evn.qp_old());
  /*_p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn_old = nand4(reg_old.XYMU_RENDERING_LATCHn.qn_old(), ROMO_PRELOAD_DONEn_evn_old, reg_old.tfetch_control.NYKA_FETCH_DONEp_evn.qp_old(), reg_old.tfetch_control.PORY_FETCH_DONEp_odd.qp_old());
  /*_p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp_old = not1(SUVU_PRELOAD_DONE_TRIGn_old);
  /*_p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp_old = or3(SEKO_WIN_FETCH_TRIGp_old, SUZU_WIN_FIRST_TILEne_old, TAVE_PRELOAD_DONE_TRIGp_old); // Schematic wrong, this is OR

  /*_p27.SEKO*/ wire SEKO_WIN_FETCH_TRIGp_evn_new = nor2(reg_new.win_ctrl.RYFA_WIN_FETCHn_A.qn_new(), reg_new.win_ctrl.RENE_WIN_FETCHn_B.qp_new());
  /*_p27.TUXY*/ wire TUXY_WIN_FIRST_TILEne_xxx_new = nand2(SYLO_WIN_HITn_odd_new, reg_new.win_ctrl.SOVY_WIN_HITp.qp_new());
  /*_p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne_xxx_new = not1(TUXY_WIN_FIRST_TILEne_xxx_new);
  /*_p27.TEVO*/ wire TEVO_WIN_FETCH_TRIGp_xxx_new = or3(SEKO_WIN_FETCH_TRIGp_evn_new, SUZU_WIN_FIRST_TILEne_xxx_new, TAVE_PRELOAD_DONE_TRIGp_xxx_new); // Schematic wrong, this is OR
  ///*#p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp_odd = and2(reg_new.win_ctrl.PYNU_WIN_MODE_Ap_odd.qp_new(), reg_new.win_ctrl.NOPA_WIN_MODE_Bp_evn.qn_new());
  ///*_p27.NYFO*/ wire NYFO_WIN_MODE_tn_odd = not1(NUNY_WIN_MODE_TRIGp);
  ///*_p27.MOSU*/ wire MOSU_WIN_MODE_tp_odd = not1(NYFO_WIN_MODE_tn_odd);
  /*_p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor3(reg_new.sprite_scanner.AVAP_SCAN_DONE_tp_odd.out_new(), MOSU_WIN_MODE_tp_odd, TEVO_WIN_FETCH_TRIGp_xxx_new);

  for (int feedback = 0; feedback < 2; feedback++) {
    /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(reg_new.tfetch_counter.LAXU_BFETCH_S0p_odd.qp_any(), reg_new.tfetch_counter.NYVA_BFETCH_S2p_odd.qp_any(), NYXU_BFETCH_RSTn);
    /*_p27.LEBO*/ wire LEBO_ODD = nand2(ALET_xBxDxFxH, MOCE_BFETCH_DONEn);

    // fixme feedback
    /*_p27.LAXU*/ reg_new.tfetch_counter.LAXU_BFETCH_S0p_odd.dff17_any(LEBO_ODD,                                            NYXU_BFETCH_RSTn, reg_new.tfetch_counter.LAXU_BFETCH_S0p_odd.qn_any());
    /*_p27.MESU*/ reg_new.tfetch_counter.MESU_BFETCH_S1p_odd.dff17_any(reg_new.tfetch_counter.LAXU_BFETCH_S0p_odd.qn_any(), NYXU_BFETCH_RSTn, reg_new.tfetch_counter.MESU_BFETCH_S1p_odd.qn_any());
    /*_p27.NYVA*/ reg_new.tfetch_counter.NYVA_BFETCH_S2p_odd.dff17_any(reg_new.tfetch_counter.MESU_BFETCH_S1p_odd.qn_any(), NYXU_BFETCH_RSTn, reg_new.tfetch_counter.NYVA_BFETCH_S2p_odd.qn_any());
  }

  /*_p27.LOVY*/ reg_new.tfetch_control.LOVY_TFETCH_DONEp.dff17(MYVO_AxCxExGx, NYXU_BFETCH_RSTn, reg_old.tfetch_control.LYRY_BFETCH_DONEp_odd.out_old());

  /*_p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(reg_new.tfetch_counter.LAXU_BFETCH_S0p_odd.qp_new(), reg_new.tfetch_counter.NYVA_BFETCH_S2p_odd.qp_new(), NYXU_BFETCH_RSTn);
  /*_p27.LYRY*/ reg_new.tfetch_control.LYRY_BFETCH_DONEp_odd <<= not1(MOCE_BFETCH_DONEn);

  /*_p27.LURY*/ wire LURY_BG_FETCH_DONEn = and2(reg_new.tfetch_control.LOVY_TFETCH_DONEp.qn_new(), reg_new.XYMU_RENDERING_LATCHn.qn_new());
  /*_p27.LONY*/ reg_new.tfetch_control.LONY_TFETCHINGp.nand_latch(NYXU_BFETCH_RSTn, LURY_BG_FETCH_DONEn);


  //----------------------------------------
  // Fine match counter

  /*#p27.PASO*/ wire PASO_FINE_RST = nor2(PAHA_RENDERINGn, TEVO_WIN_FETCH_TRIGp_xxx_new);

  /*#p27.ROZE*/ uint8_t ROZE_FINE_COUNT_7n_odd = nand3(reg_new.fine_count_odd.RUBU_FINE_CNT2_odd.qp_any(), reg_new.fine_count_odd.ROGA_FINE_CNT1_odd.qp_any(), reg_new.fine_count_odd.RYKU_FINE_CNT0_odd.qp_any());
  /*#p27.PECU*/ uint8_t PECU_FINE_CLK_odd = nand2(ROXO_CLKPIPE_evn_new, ROZE_FINE_COUNT_7n_odd);

  /*#p27.RYKU*/ reg_new.fine_count_odd.RYKU_FINE_CNT0_odd.dff17_any(PECU_FINE_CLK_odd,                               PASO_FINE_RST, reg_new.fine_count_odd.RYKU_FINE_CNT0_odd.qn_any());
  /*#p27.ROGA*/ reg_new.fine_count_odd.ROGA_FINE_CNT1_odd.dff17_any(reg_new.fine_count_odd.RYKU_FINE_CNT0_odd.qn_any(), PASO_FINE_RST, reg_new.fine_count_odd.ROGA_FINE_CNT1_odd.qn_any());
  /*#p27.RUBU*/ reg_new.fine_count_odd.RUBU_FINE_CNT2_odd.dff17_any(reg_new.fine_count_odd.ROGA_FINE_CNT1_odd.qn_any(), PASO_FINE_RST, reg_new.fine_count_odd.RUBU_FINE_CNT2_odd.qn_any());

  ROZE_FINE_COUNT_7n_odd = nand3(reg_new.fine_count_odd.RUBU_FINE_CNT2_odd.qp_any(), reg_new.fine_count_odd.ROGA_FINE_CNT1_odd.qp_any(), reg_new.fine_count_odd.RYKU_FINE_CNT0_odd.qp_any());
  PECU_FINE_CLK_odd = nand2(ROXO_CLKPIPE_evn_new, ROZE_FINE_COUNT_7n_odd);

  /*#p27.RYKU*/ reg_new.fine_count_odd.RYKU_FINE_CNT0_odd.dff17_any(PECU_FINE_CLK_odd,                               PASO_FINE_RST, reg_new.fine_count_odd.RYKU_FINE_CNT0_odd.qn_any());
  /*#p27.ROGA*/ reg_new.fine_count_odd.ROGA_FINE_CNT1_odd.dff17_any(reg_new.fine_count_odd.RYKU_FINE_CNT0_odd.qn_any(), PASO_FINE_RST, reg_new.fine_count_odd.ROGA_FINE_CNT1_odd.qn_any());
  /*#p27.RUBU*/ reg_new.fine_count_odd.RUBU_FINE_CNT2_odd.dff17_any(reg_new.fine_count_odd.ROGA_FINE_CNT1_odd.qn_any(), PASO_FINE_RST, reg_new.fine_count_odd.RUBU_FINE_CNT2_odd.qn_any());


  {
    ///*#p27.SYLO*/ wire SYLO_WIN_HITn_odd_new = not1(reg_new.win_ctrl.RYDY_WIN_HITp_odd.out_new());
    ///*#p24.TOMU*/ wire TOMU_WIN_HITp_odd_new = not1(SYLO_WIN_HITn_odd_new);
    ///*_p27.TUKU*/ wire TUKU_WIN_HITn_odd_new = not1(TOMU_WIN_HITp_odd_new);
    ///*_p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn_evn_new = not1(reg_new.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.qp_new());
    ///*_p27.TEKY*/ wire TEKY_SFETCH_REQp_odd_new = and4(reg_new.FEPO_STORE_MATCHp_odd.out_new(), TUKU_WIN_HITn_odd_new, reg_new.tfetch_control.LYRY_BFETCH_DONEp_odd.out_new(), SOWO_SFETCH_RUNNINGn_evn_new);
  }

  //----------------------------------------
  // PPU / LCD output

  tock_pix_pipes_gates(reg_old, SACU_CLKPIPE_odd_new, NYXU_BFETCH_RSTn);
  set_lcd_pins_gates(reg_old, SACU_CLKPIPE_odd_new);

  //----------------------------------------
  // Audio

  //tock_spu_gates(reg_old);

  //----------------------------------------
  // Memory buses

  tock_ext_gates(reg_old, cart_blob);
  tock_vram_bus_gates(reg_old, TEVO_WIN_FETCH_TRIGp_old, TEVO_WIN_FETCH_TRIGp_xxx_new);
  tock_oam_bus_gates(reg_old);
  tock_zram_gates(reg_old, reg_new, mem.zero_ram);

  //----------------------------------------
  // Sound

#ifdef SIM_AUDIO
  tick_spu(reg_old, reg_new, mem.wave_ram);
#endif

  //----------------------------------------
  // And finally, interrupts.

  tock_interrupts_gates(reg_old);
}
