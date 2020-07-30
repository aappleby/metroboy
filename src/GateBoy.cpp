#include "GateBoy.h"
#include <memory.h>

#include "Debug.h"

constexpr bool FAST_HASH = 0;

#if 0
  // these are all "normal operating mode" w/o rendering or dma 

  OAM_PIN_OE     = nand(CPU_PIN_RDp, !CPU_PIN_READ_MEM,               ADDR_OAM);
  OPD_TO_OBD     = nand(CPU_PIN_RDp, !CPU_PIN_READ_MEM,               ADDR_OAM);
  OBD_TO_OBL     =  and(CPU_PIN_RDp, !CPU_PIN_READ_MEM,               ADDR_OAM);
  OBL_TO_CBD     =  and(CPU_PIN_RDp,  CPU_PIN_READ_MEM,               ADDR_OAM);
  EPD_TO_CBD     =  and(CPU_PIN_RDp,  CPU_PIN_READ_MEM,  CPU_PIN_ADDR_EXT, !ADDR_VRAM);
  VBD_TO_CBD     =  and(CPU_PIN_RDp,  CPU_PIN_READ_MEM, !CPU_PIN_ADDR_EXT,  ADDR_VRAM);
  OAM_PIN_WRn    = nand(CPU_PIN_WRp,                                  ADDR_OAM,    xxxxEFGx);
  TIMA_LOADp     =  and(CPU_PIN_WRp, !CPU_PIN_READ_MEM,               FF05,        xxxxEFGx);
  VRAM_PIN_WR    = nand(CPU_PIN_WRp,                    !CPU_PIN_ADDR_EXT,  ADDR_VRAM,   xxxxEFGx);
  VRAM_PIN_CS    = nand(                                !CPU_PIN_ADDR_EXT,  ADDR_VRAM);
  VRAM_PIN_OE    = nand(CPU_PIN_WRp,                                  ADDR_VRAM);
  EXT_PIN_RD     = nand(CPU_PIN_WRp,                     CPU_PIN_ADDR_EXT, !ADDR_VRAM);
  EXT_PIN_WR     =  and(CPU_PIN_WRp,                     CPU_PIN_ADDR_EXT, !ADDR_VRAM,   xxxxEFGx);
  EXT_PIN_CS     =  and(                                !CPU_PIN_ADDR_EXT,  ADDR_ERAM);
  EXT_PIN_A15_A  =  and(                                !CPU_PIN_ADDR_EXT, !A15);
  EXT_PIN_A15_D  =  and(                                !CPU_PIN_ADDR_EXT, !A15);
  CBD_TO_VPD     =  and(CPU_PIN_WRp,                    !CPU_PIN_ADDR_EXT,  ADDR_VRAM);
  CBD_TO_EPD     =  and(CPU_PIN_WRp,                     CPU_PIN_ADDR_EXT, !ADDR_VRAM);
  CBA_TO_EPA     =  and(                                 CPU_PIN_ADDR_EXT, !ADDR_VRAM);


  /*read  rom */ CPU_PIN_READ_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write rom */ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  vram*/ CPU_PIN_READ_MEM = true;  CPU_PIN_ADDR_EXT = false;
  /*write vram*/ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = false;

  /*read  cram*/ CPU_PIN_READ_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write cram*/ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  eram*/ CPU_PIN_READ_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write eram*/ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  oam */ CPU_PIN_READ_MEM = true;  CPU_PIN_ADDR_EXT = false;
  /*write oam */ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = false;

  /*read  hram*/ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = dc;
  /*write hram*/ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = false;

#endif

//-----------------------------------------------------------------------------

GateBoy::GateBoy() {
  for (int i = 0; i < 65536; i++) {
    mem2[i] = uint8_t(i);
  }
}

//-----------------------------------------------------------------------------

void GateBoy::run_reset_sequence(bool verbose, bool use_fast_impl) {

  // No bus activity during reset
  Req req = {.addr = 0x0100, .data = 0, .read = 0, .write = 0 };

  //----------
  // 8 phases in reset

  run(8, req, verbose, use_fast_impl);
  if (verbose) printf("\n");

  //----------
  // 8 phases out of reset

  sys_rst = 0;
  run(8, req, verbose, use_fast_impl);
  if (verbose) printf("\n");

  //----------
  // 11 phases with enabled clock, which should put us in phase A

  sys_clken = 1;
  run(11, req, verbose, use_fast_impl);
  if (verbose) printf("\n");

  CHECK_P(top.clk_reg._AFUR_ABCDxxxx.q());
  CHECK_N(top.clk_reg._ALEF_xBCDExxx.q());
  CHECK_N(top.clk_reg._APUK_xxCDEFxx.q());
  CHECK_N(top.clk_reg._ADYK_xxxDEFGx.q());

  CHECK_P((phase_total & 7) == 0);


  //----------
  // 8 phases with "good" clock (still not really sure what the CLKIN_A circuit does)

  sys_clkgood = 1;
  run(8, req, verbose, use_fast_impl);
  if (verbose) printf("\n");

  //----------
  // Wait for START

  while(!top.clk_reg.CPU_PIN_STARTp()) {
    run(8, req, verbose, use_fast_impl);
    if (verbose) printf("\n");
  }

  //----------
  // 8 phases after START

  run(8, req, verbose, use_fast_impl);

  //----------
  // 24 phases with CPU ready

  sys_cpuready = 1;
  run(8, req, verbose, use_fast_impl);

  test_reg("JOYP", 0xFF00, 177, use_fast_impl);

  test_reg("SB",   0xFF01, 177, use_fast_impl);
  test_reg("SC",   0xFF02, 177, use_fast_impl);

  test_reg("DIV",  0xFF04, 177, use_fast_impl);
  test_reg("TIMA", 0xFF05, 177, use_fast_impl);
  test_reg("TMA",  0xFF06, 177, use_fast_impl);
  test_reg("TAC",  0xFF07, 177, use_fast_impl);

  //test_reg("LCDC", 0xFF40, 177, use_fast_impl);
  test_reg("STAT", 0xFF41, 177, use_fast_impl);
  test_reg("SCY",  0xFF42, 177, use_fast_impl);
  test_reg("SCX",  0xFF43, 177, use_fast_impl);
  test_reg("LY",   0xFF44, 177, use_fast_impl);
  test_reg("LYC",  0xFF45, 177, use_fast_impl);

  //test_reg("DMA", 0xFF46, 177, use_fast_impl); // does not work
  test_reg("BGP",  0xFF47, 177, use_fast_impl);
  test_reg("OBP0", 0xFF48, 177, use_fast_impl);
  test_reg("OBP1", 0xFF49, 177, use_fast_impl);
  test_reg("WY",   0xFF4A, 177, use_fast_impl);
  test_reg("WX",   0xFF4B, 177, use_fast_impl);

  test_reg("IF",   0xFF0F, 177, use_fast_impl);
  test_reg("IE",   0xFFFF, 177, use_fast_impl);

  mem2[0x000] = 177;
  test_reg("ROM",  0x0000, 177, use_fast_impl);
  test_reg("VRAM", 0x8000, 177, use_fast_impl);
  test_reg("CRAM", 0xA000, 177, use_fast_impl);
  test_reg("IRAM", 0xC000, 177, use_fast_impl);
  test_reg("ERAM", 0xE000, 177, use_fast_impl);

  mem2[0xFE00] = 177;

  test_reg("ORAM", 0xFE00, 177, use_fast_impl);
  test_reg("ZP00", 0xFF80, 177, use_fast_impl);

  if (verbose) printf("\n");
}

//------------------------------------------------------------------------------

void GateBoy::test_reg(const char* regname, uint16_t addr, uint8_t data_in, bool use_fast_impl) {
  printf("%5s @ %04x <= %d\n", regname, addr, data_in);
  dbg_write(addr, data_in, use_fast_impl);
  uint8_t data_out = dbg_read(addr, use_fast_impl);
  printf("%5s @ %04x => %d\n", regname, addr, data_out);
}

//------------------------------------------------------------------------------

void GateBoy::run(int _phase_count, Req req, bool verbose, bool use_fast_impl) {
  for (int i = 0; i < _phase_count; i++) {
    phase(req, verbose, use_fast_impl);
  }
}

//------------------------------------------------------------------------------

uint8_t GateBoy::dbg_read(uint16_t addr, bool use_fast_impl) {
  CHECK_P((phase_total & 7) == 0);
  Req req = {.addr = addr, .data = 0, .read = 1, .write = 0 };
  run(4, req, false, use_fast_impl);
  uint8_t res = top.cpu_bus.bus_data();
  run(4, req, false, use_fast_impl);
  return res;
}

void GateBoy::dbg_write(uint16_t addr, uint8_t data, bool use_fast_impl) {
  CHECK_P((phase_total & 7) == 0);
  Req req = {.addr = addr, .data = data, .read = 0, .write = 1 };
  run(8, req, false, use_fast_impl);
}

//------------------------------------------------------------------------------

void GateBoy::phase(Req req, bool verbose, bool use_fast_impl) {
  phase_total++;
  const int phase = phase_total & 7;

  //----------
  // Update clock and buses

  wire CLK = (phase_total & 1) & sys_clken;

  //----------
  // Run passes until we stabilize

  uint64_t hash_regs_old  = HASH_INIT;
  uint64_t hash_regs_new  = HASH_INIT;

  StringDumper d;

  for (pass_count = 0; pass_count < 100; pass_count++) {
    top.clk_reg.preset_rst(sys_rst);
    top.clk_reg.preset_t1t2(sys_t1, sys_t2);
    top.clk_reg.preset_cpu_ready(sys_cpuready);
    top.clk_reg.preset_clk_a(sys_clkgood);
    top.clk_reg.preset_clk_b(CLK);
    top.joypad.preset_buttons(0);
    update_cpu_bus(phase, req);
    update_ext_bus(phase);
    update_vrm_bus(phase);
    update_oam_bus(phase);

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

  uint8_t phase_clock = top.clk_reg.get_phase_clock();

  if (sys_clken && sys_clkgood) {
    if (PHASE_A) CHECK_P(phase_clock == 0b0001);
    if (PHASE_B) CHECK_P(phase_clock == 0b0011);
    if (PHASE_C) CHECK_P(phase_clock == 0b0111);
    if (PHASE_D) CHECK_P(phase_clock == 0b1111);
    if (PHASE_E) CHECK_P(phase_clock == 0b1110);
    if (PHASE_F) CHECK_P(phase_clock == 0b1100);
    if (PHASE_G) CHECK_P(phase_clock == 0b1000);
    if (PHASE_H) CHECK_P(phase_clock == 0b0000);
  }

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

  bus.CPU_PIN6.preset(0);

#if 0
  /*read  rom */ CPU_PIN_READ_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write rom */ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  vram*/ CPU_PIN_READ_MEM = true;  CPU_PIN_ADDR_EXT = false;
  /*write vram*/ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = false;

  /*read  cram*/ CPU_PIN_READ_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write cram*/ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  eram*/ CPU_PIN_READ_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write eram*/ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  oam */ CPU_PIN_READ_MEM = true;  CPU_PIN_ADDR_EXT = false;
  /*write oam */ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = false;

  /*read  hram*/ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = dc;
  /*write hram*/ CPU_PIN_READ_MEM = dc;    CPU_PIN_ADDR_EXT = false;

#endif

  if (PHASE_A) {
    bus.preset_addr_z();
    bus.CPU_PIN_RDp.preset(0);
    bus.CPU_PIN_WRp.preset(0);
    bus.CPU_PIN_ADDR_EXT.preset(0);
    bus.CPU_PIN_READ_MEM.preset(0);
  }
  else {
    bus.preset_addr(req.addr);
    bus.CPU_PIN_RDp.preset(req.read);
    bus.CPU_PIN_WRp.preset(req.write);

    if (req.read) {
      /*erom*/ if      (req.addr <= 0x7FFF) { bus.CPU_PIN_ADDR_EXT.preset(1); }
      /*vram*/ else if (req.addr <= 0x9FFF) { bus.CPU_PIN_ADDR_EXT.preset(0); }
      /*cram*/ else if (req.addr <= 0xBFFF) { bus.CPU_PIN_ADDR_EXT.preset(1); }
      /*eram*/ else if (req.addr <= 0xDFFF) { bus.CPU_PIN_ADDR_EXT.preset(1); }
      /*eram*/ else if (req.addr <= 0xFDFF) { bus.CPU_PIN_ADDR_EXT.preset(1); }
      /*oram*/ else if (req.addr <= 0xFEFF) { bus.CPU_PIN_ADDR_EXT.preset(0); }
      /*hram*/ else if (req.addr <= 0xFFFF) { bus.CPU_PIN_ADDR_EXT.preset(0); }
    }
    else if (req.write) {
      /*erom*/ if      (req.addr <= 0x7FFF) { bus.CPU_PIN_ADDR_EXT.preset(1); }
      /*vram*/ else if (req.addr <= 0x9FFF) { bus.CPU_PIN_ADDR_EXT.preset(0); }
      /*cram*/ else if (req.addr <= 0xBFFF) { bus.CPU_PIN_ADDR_EXT.preset(1); }
      /*eram*/ else if (req.addr <= 0xDFFF) { bus.CPU_PIN_ADDR_EXT.preset(1); }
      /*eram*/ else if (req.addr <= 0xFDFF) { bus.CPU_PIN_ADDR_EXT.preset(1); }
      /*oram*/ else if (req.addr <= 0xFEFF) { bus.CPU_PIN_ADDR_EXT.preset(0); }
      /*hram*/ else if (req.addr <= 0xFFFF) { bus.CPU_PIN_ADDR_EXT.preset(0); }
    }
    else {
      bus.CPU_PIN_ADDR_EXT.preset(0);
    }

    bus.CPU_PIN_READ_MEM.preset(req.read && (req.addr < 0xFF00));
  }

  if (PHASE(0b00001111) && req.write) {
    bus.preset_data(req.data_lo);
  }
  else {
    bus.preset_data_z();
  }

  top.int_reg.CPU_PIN_ACK_VBLANK.preset(0);
  top.int_reg.CPU_PIN_ACK_STAT.preset(0);
  top.int_reg.CPU_PIN_ACK_TIMER.preset(0);
  top.int_reg.CPU_PIN_ACK_SERIAL.preset(0);
  top.int_reg.CPU_PIN_ACK_JOYPAD.preset(0);

  top.ser_reg.SCK_C.preset(0);
  top.ser_reg.SIN_C.preset(0);
}

//-----------------------------------------------------------------------------

void GateBoy::update_ext_bus(int phase) {
  (void)phase;

  top.ext_bus._EXT_PIN_WR_C.preset(top.ext_bus._EXT_PIN_WR_A);
  top.ext_bus._EXT_PIN_RD_C.preset(top.ext_bus._EXT_PIN_RD_A);

  top.ext_bus._EXT_PIN_A00_C.preset(top.ext_bus._EXT_PIN_A00_A);
  top.ext_bus._EXT_PIN_A01_C.preset(top.ext_bus._EXT_PIN_A01_A);
  top.ext_bus._EXT_PIN_A02_C.preset(top.ext_bus._EXT_PIN_A02_A);
  top.ext_bus._EXT_PIN_A03_C.preset(top.ext_bus._EXT_PIN_A03_A);
  top.ext_bus._EXT_PIN_A04_C.preset(top.ext_bus._EXT_PIN_A04_A);
  top.ext_bus._EXT_PIN_A05_C.preset(top.ext_bus._EXT_PIN_A05_A);
  top.ext_bus._EXT_PIN_A06_C.preset(top.ext_bus._EXT_PIN_A06_A);
  top.ext_bus._EXT_PIN_A07_C.preset(top.ext_bus._EXT_PIN_A07_A);
  top.ext_bus._EXT_PIN_A08_C.preset(top.ext_bus._EXT_PIN_A08_A);
  top.ext_bus._EXT_PIN_A09_C.preset(top.ext_bus._EXT_PIN_A09_A);
  top.ext_bus._EXT_PIN_A10_C.preset(top.ext_bus._EXT_PIN_A10_A);
  top.ext_bus._EXT_PIN_A11_C.preset(top.ext_bus._EXT_PIN_A11_A);
  top.ext_bus._EXT_PIN_A12_C.preset(top.ext_bus._EXT_PIN_A12_A);
  top.ext_bus._EXT_PIN_A13_C.preset(top.ext_bus._EXT_PIN_A13_A);
  top.ext_bus._EXT_PIN_A14_C.preset(top.ext_bus._EXT_PIN_A14_A);
  top.ext_bus._EXT_PIN_A15_C.preset(top.ext_bus._EXT_PIN_A15_A);

  if (top.ext_bus._EXT_PIN_RD_A) {
    uint16_t ext_addr = top.ext_bus.get_pin_addr();
    uint8_t data = mem2[ext_addr & 0x7FFF];
    top.ext_bus.preset_pin_data_in(data);
    //top.ext_bus.preset_pin_data_in(55);
  }
  else {
    top.ext_bus.preset_pin_data_z();
  }
}

//-----------------------------------------------------------------------------

void GateBoy::update_vrm_bus(int phase) {
  (void)phase;

  top.vram_bus._VRAM_PIN_CS_C.preset(0);
  top.vram_bus._VRAM_PIN_OE_C.preset(0);
  top.vram_bus._VRAM_PIN_WR_C.preset(0);

  uint16_t vram_pin_addr = top.vram_bus.get_pin_addr();

  if (top.vram_bus._VRAM_PIN_WR_A) {
    uint8_t data = ~top.vram_bus.get_pin_data_out();
    //printf("vram[%04x] = %d\n", vram_pin_addr, data);
    mem2[vram_pin_addr + 0x8000] = data;
  }

  if (top.vram_bus._VRAM_PIN_OE_A) {
    uint8_t vram_data = mem2[vram_pin_addr + 0x8000];
    top.vram_bus.preset_pin_data_in(vram_data);
  }
  else {
    top.vram_bus.preset_pin_data_z();
  }
}

//-----------------------------------------------------------------------------

void GateBoy::update_oam_bus(int phase) {
  (void)phase;

  if (!top.oam_bus.OAM_PIN_OE) {
    int oam_addr = top.oam_bus.get_bus_addr();
    uint16_t* oam_base = (uint16_t*)(&mem2[0xFE00]);
    top.oam_bus.preset_bus_data(true, oam_base[oam_addr >> 1]);
  }
  else {
    top.oam_bus.preset_bus_data(false, 0);
  }
}

//-----------------------------------------------------------------------------