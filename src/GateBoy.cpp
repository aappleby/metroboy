#include "GateBoy.h"
#include <memory.h>

#include "Debug.h"

constexpr bool FAST_HASH = 0;

//-----------------------------------------------------------------------------

GateBoy::GateBoy() {
  for (int i = 0; i < 65536; i++) {
    mem2[i] = uint8_t(i);
  }
}

//-----------------------------------------------------------------------------

void GateBoy::run_reset_sequence(bool verbose, bool use_fast_impl) {
  top.clk_reg.set_cpu_ready(0);
  top.clk_reg.set_t1t2(0,0);

  // Just read DIV forever.
  Req req = {.addr = 0x0100, .data = 0, .read = 1, .write = 0 };

  //----------
  // RST = 1, CLK_A = 0, CPU_READY = 0

  top.clk_reg.set_rst(1);
  top.clk_reg.set_clk_a(0);
  top.clk_reg.set_cpu_ready(0);
  run(8, req, verbose, use_fast_impl);
  if (verbose) printf("\n");

  //----------
  // RST = 0, CLK_A = 0, CPU_READY = 0

  top.clk_reg.set_rst(0);
  top.clk_reg.set_clk_a(0);
  top.clk_reg.set_cpu_ready(0);
  run(8, req, verbose, use_fast_impl);
  if (verbose) printf("\n");

  //----------
  // RST = 0, CLK_A = 1, CPU_READY = 0

  top.clk_reg.set_rst(0);
  top.clk_reg.set_clk_a(1);
  top.clk_reg.set_cpu_ready(0);
  run(8, req, verbose, use_fast_impl);
  if (verbose) printf("\n");

  //----------
  // Wait for START

  while(!top.clk_reg.CPU_PIN_STARTp()) {
    run(8, req, verbose, use_fast_impl);
    if (verbose) printf("\n");
  }

  //----------
  // RST = 0, CLK_A = 1, CPU_READY = 0

  top.clk_reg.set_rst(0);
  top.clk_reg.set_clk_a(1);
  top.clk_reg.set_cpu_ready(0);
  run(8, req, verbose, use_fast_impl);

  //----------
  // RST = 0, CLK_A = 1, CPU_READY = 1

  top.clk_reg.set_rst(0);
  top.clk_reg.set_clk_a(1);
  top.clk_reg.set_cpu_ready(1);
  run(24, req, verbose, use_fast_impl);

  if (verbose) printf("\n");
}

//------------------------------------------------------------------------------

void GateBoy::run(int _phase_count, Req req, bool verbose, bool use_fast_impl) {
  for (int i = 0; i < _phase_count; i++) {
    phase(req, verbose, use_fast_impl);
  }
}

//------------------------------------------------------------------------------

uint8_t GateBoy::dbg_read(uint16_t addr, bool use_fast_impl) {
  Req req = {.addr = addr, .data = 0, .read = 1, .write = 0 };
  run(8, req, false, use_fast_impl);
  return top.cpu_bus.bus_data();
}

void GateBoy::dbg_write(uint16_t addr, uint8_t data, bool use_fast_impl) {
  Req req = {.addr = addr, .data = data, .read = 0, .write = 1 };
  run(8, req, false, use_fast_impl);
}

//------------------------------------------------------------------------------

void GateBoy::phase(Req req, bool verbose, bool use_fast_impl) {
  phase_total++;
  const int phase = phase_total & 7;

  //----------
  // Update clock and buses

  wire CLK = (phase_total & 1) & (top.clk_reg.get_clk_a());
  top.clk_reg.set_clk_b(CLK);

  //----------
  // Run passes until we stabilize

  uint64_t hash_regs_old  = HASH_INIT;
  uint64_t hash_regs_new  = HASH_INIT;

  StringDumper d;

  for (pass_count = 0; pass_count < 100; pass_count++) {

    update_cpu_bus(phase, req);
    update_ext_bus(phase);
    update_vrm_bus(phase);
    update_oam_bus(phase);
    top.joypad.set_buttons(0);

    if (use_fast_impl) {
      top.tick_fast(phase);
    }
    else {
      top.tick_slow(phase);
    }

    hash_regs_old = hash_regs_new;
    hash_regs_new  = HASH_INIT;
    if (FAST_HASH) {
      commit_and_hash(top, hash_regs_new);
    }
    else {
      commit_and_hash(top.oam_bus, hash_regs_new);
      commit_and_hash(top.ext_bus, hash_regs_new);
      commit_and_hash(top.cpu_bus, hash_regs_new);
      commit_and_hash(top.vram_bus, hash_regs_new);
      commit_and_hash(top.clk_reg, hash_regs_new);
      commit_and_hash(top.dma_reg, hash_regs_new);
      commit_and_hash(top.int_reg, hash_regs_new);
      commit_and_hash(top.joypad, hash_regs_new);
      commit_and_hash(top.lcd_reg, hash_regs_new);
      commit_and_hash(top.pix_pipe, hash_regs_new);
      commit_and_hash(top.ser_reg, hash_regs_new);
      commit_and_hash(top.sprite_store, hash_regs_new);
      commit_and_hash(top.tim_reg, hash_regs_new);
      commit_and_hash(top.tile_fetcher, hash_regs_new);
      commit_and_hash(top.sprite_fetcher, hash_regs_new);
      commit_and_hash(top.sprite_scanner, hash_regs_new);
      commit_and_hash(top.bootrom, hash_regs_new);
    }
    if (hash_regs_new == hash_regs_old) break;
  }

  CHECK_P(pass_count < 100);

  //----------
  // Done

  phase_hash = hash_regs_old;
  combine_hash(total_hash, phase_hash);

  if (verbose) {
    printf("Phase %c @ %08d:%02d phase_hash %016llx\n",
      'A' + (phase_total & 7),
      phase_total,
      pass_count,
      phase_hash);
  }
}

//-----------------------------------------------------------------------------

void GateBoy::update_cpu_bus(int phase, Req req) {
  auto& bus = top.cpu_bus;

  bus._CPU_PIN6.hold(0);

  if (PHASE_A) {
    bus._CPU_PIN_RDp.hold(0);
    bus._CPU_PIN_WRp.hold(0);
    bus._CPU_PIN_AVp.hold(0);
    bus._CPU_PIN_DVn.hold(1);
  }
  else {
    bus._CPU_PIN_RDp.hold(req.read);
    bus._CPU_PIN_WRp.hold(req.write);
    bus._CPU_PIN_AVp.hold((req.read || req.write));
    bus._CPU_PIN_DVn.hold(1);
  }

  if (PHASE_B) {
    bus.CPU_BUS_A00.hold(req.addr & 0x0001);
    bus.CPU_BUS_A01.hold(req.addr & 0x0002);
    bus.CPU_BUS_A02.hold(req.addr & 0x0004);
    bus.CPU_BUS_A03.hold(req.addr & 0x0008);
    bus.CPU_BUS_A04.hold(req.addr & 0x0010);
    bus.CPU_BUS_A05.hold(req.addr & 0x0020);
    bus.CPU_BUS_A06.hold(req.addr & 0x0040);
    bus.CPU_BUS_A07.hold(req.addr & 0x0080);
    bus.CPU_BUS_A08.hold(req.addr & 0x0100);
    bus.CPU_BUS_A09.hold(req.addr & 0x0200);
    bus.CPU_BUS_A10.hold(req.addr & 0x0400);
    bus.CPU_BUS_A11.hold(req.addr & 0x0800);
    bus.CPU_BUS_A12.hold(req.addr & 0x1000);
    bus.CPU_BUS_A13.hold(req.addr & 0x2000);
    bus.CPU_BUS_A14.hold(req.addr & 0x4000);
    bus.CPU_BUS_A15.hold(req.addr & 0x8000);
  }

  if (PHASE(0b00001111) && req.write) {
    bus._CPU_PIN_DVn.hold(0);
    bus.CPU_BUS_D0.hold(req.data_lo & 0x01);
    bus.CPU_BUS_D1.hold(req.data_lo & 0x02);
    bus.CPU_BUS_D2.hold(req.data_lo & 0x04);
    bus.CPU_BUS_D3.hold(req.data_lo & 0x08);
    bus.CPU_BUS_D4.hold(req.data_lo & 0x10);
    bus.CPU_BUS_D5.hold(req.data_lo & 0x20);
    bus.CPU_BUS_D6.hold(req.data_lo & 0x40);
    bus.CPU_BUS_D7.hold(req.data_lo & 0x80);
  }
  else {
    bus._CPU_PIN_DVn.hold(1);
    bus.CPU_BUS_D0.release();
    bus.CPU_BUS_D1.release();
    bus.CPU_BUS_D2.release();
    bus.CPU_BUS_D3.release();
    bus.CPU_BUS_D4.release();
    bus.CPU_BUS_D5.release();
    bus.CPU_BUS_D6.release();
    bus.CPU_BUS_D7.release();
  }
}

//-----------------------------------------------------------------------------

void GateBoy::update_ext_bus(int phase) {
  (void)phase;
  //if (PHASE_C) {
    if (top.ext_bus._EXT_PIN_RD_A) {
      uint16_t ext_addr = top.ext_bus.get_pin_addr();
      uint8_t data = mem2[ext_addr & 0x7FFF];
      top.ext_bus.hold_pin_data_in(~data);
    }
    else {
      top.ext_bus.hold_pin_data_z();
    }
  //}
}

//-----------------------------------------------------------------------------

void GateBoy::update_vrm_bus(int phase) {
  (void)phase;

  top.vram_bus._VRAM_PIN_CS_C.hold(0);
  top.vram_bus._VRAM_PIN_OE_C.hold(0);
  top.vram_bus._VRAM_PIN_WR_C.hold(0);

  if (top.vram_bus._VRAM_PIN_OE_A) {
    uint16_t vram_pin_addr = top.vram_bus.get_pin_addr();
    uint8_t vram_data = mem2[vram_pin_addr + 0x8000];
    top.vram_bus.hold_pin_data_in(vram_data);
  }
  else {
    top.vram_bus.hold_pin_data_z();
  }
}

//-----------------------------------------------------------------------------

void GateBoy::update_oam_bus(int phase) {
  (void)phase;

  if (!top.oam_bus.OAM_PIN_OE) {
    int oam_addr = top.oam_bus.get_bus_addr();
    uint16_t* oam_base = (uint16_t*)(&mem2[0xFE00]);
    top.oam_bus.set_bus_data(true, oam_base[oam_addr >> 1]);
  }
  else {
    top.oam_bus.set_bus_data(false, 0);
  }
}

//-----------------------------------------------------------------------------