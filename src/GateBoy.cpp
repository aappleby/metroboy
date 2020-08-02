#include "GateBoy.h"
#include <memory.h>

#include "Debug.h"

constexpr bool FAST_HASH = 0;

#if 0
  // these are all "normal operating mode" w/o rendering or dma 

  OAM_PIN_OE     = nand(CPU_PIN_RDp, !CPU_PIN_HOLD_MEM,                     ADDR_OAM);
  OPD_TO_OBD     = nand(CPU_PIN_RDp, !CPU_PIN_HOLD_MEM,                     ADDR_OAM);
  OBD_TO_OBL     =  and(CPU_PIN_RDp, !CPU_PIN_HOLD_MEM,                     ADDR_OAM);
  OBL_TO_CBD     =  and(CPU_PIN_RDp,  CPU_PIN_HOLD_MEM,                     ADDR_OAM);

  EPD_TO_CBD     =  and(CPU_PIN_RDp,  CPU_PIN_HOLD_MEM,  CPU_PIN_ADDR_EXT, !ADDR_VRAM);
  EXT_PIN_RD     = nand(CPU_PIN_WRp,                     CPU_PIN_ADDR_EXT, !ADDR_VRAM);
  EXT_PIN_WR     =  and(CPU_PIN_WRp,                     CPU_PIN_ADDR_EXT, !ADDR_VRAM,   xxxxEFGx);
  CBD_TO_EPD     =  and(CPU_PIN_WRp,                     CPU_PIN_ADDR_EXT, !ADDR_VRAM);
  CBA_TO_EPA     =  and(                                 CPU_PIN_ADDR_EXT, !ADDR_VRAM);

  EXT_PIN_CS     =  and(                                !CPU_PIN_ADDR_EXT,  ADDR_ERAM,   xxCDEFGH);

  EXT_PIN_A15_A  =  !or(                                 CPU_PIN_ADDR_EXT, A15);
  EXT_PIN_A15_D  =  !or(                                 CPU_PIN_ADDR_EXT, A15);

  OAM_PIN_WRn    = nand(CPU_PIN_WRp,                                        ADDR_OAM,    xxxxEFGx);
  TIMA_LOADp     =  and(CPU_PIN_WRp, !CPU_PIN_HOLD_MEM,                     FF05,        xxxxEFGx);


  VBD_TO_CBD     =  and(CPU_PIN_RDp,  CPU_PIN_HOLD_MEM, !CPU_PIN_ADDR_EXT,  ADDR_VRAM);
  VRAM_PIN_WR    = nand(CPU_PIN_WRp,                    !CPU_PIN_ADDR_EXT,  ADDR_VRAM,   xxxxEFGx);
  VRAM_PIN_CS    = nand(                                !CPU_PIN_ADDR_EXT,  ADDR_VRAM);
  VRAM_PIN_OE    = nand(CPU_PIN_WRp,                                        ADDR_VRAM);


  CBD_TO_VPD     =  and(CPU_PIN_WRp,                    !CPU_PIN_ADDR_EXT,  ADDR_VRAM);


  EXT_PIN_A00_A = not(tp_latch(and(CPU_PIN_ADDR_EXT, !ADDR_VRAM), CPU_BUS_A00));


  /*read  rom */ CPU_PIN_HOLD_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write rom */ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  vram*/ CPU_PIN_HOLD_MEM = true;  CPU_PIN_ADDR_EXT = false;
  /*write vram*/ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = false;

  /*read  cram*/ CPU_PIN_HOLD_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write cram*/ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  eram*/ CPU_PIN_HOLD_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write eram*/ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  oam */ CPU_PIN_HOLD_MEM = true;  CPU_PIN_ADDR_EXT = false;
  /*write oam */ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = false;

  /*read  hram*/ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = dc;
  /*write hram*/ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = false;

#endif

//-----------------------------------------------------------------------------

GateBoy::GateBoy() {
  for (int i = 0; i < 65536; i++) {
    mem[i] = uint8_t(i);
  }
}

//-----------------------------------------------------------------------------

void GateBoy::run_reset_sequence(bool verbose, bool use_fast_impl) {

  // No bus activity during reset
  Req req = {.addr = 0x0100, .data = 0, .read = 0, .write = 0 };

  //----------
  // 8 phases in reset

  printf("In reset\n");
  run(8, req, verbose, use_fast_impl);
  if (verbose) printf("\n");

  //----------
  // 8 phases out of reset

  printf("Out of reset\n");
  sys_rst = 0;
  run(8, req, verbose, use_fast_impl);
  if (verbose) printf("\n");

  //----------
  // 11 phases with enabled clock, which should put us in phase A

  printf("Sync with phase B\n");
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

  printf("Clock good\n");
  sys_clkgood = 1;
  run(8, req, verbose, use_fast_impl);
  if (verbose) printf("\n");

  //----------
  // Wait for START

  printf("Wait for CPU_PIN_START\n");
  while(!top.clk_reg.CPU_PIN_STARTp) {
    run(8, req, verbose, use_fast_impl);
    if (verbose) printf("\n");
  }

  //----------
  // 8 phases after START

  printf("Delay\n");
  run(8, req, verbose, use_fast_impl);

  //----------
  // 24 phases with CPU ready

  printf("Set CPU_READY = 1\n");
  sys_cpuready = 1;
  run(8, req, verbose, use_fast_impl);

  //----------
  // Set boot bit, otherwise we can't read rom

  printf("Set BOOT = 1\n");
  {
    //uint8_t boot_bit = dbg_read(0xFF50, use_fast_impl);
    //printf("%5s @ %04x => %d\n", "BOOT", 0xFF50, boot_bit);
    dbg_write(0xFF50, 0xFF, use_fast_impl);
    //boot_bit = dbg_read(0xFF50, use_fast_impl);

    //printf("%5s @ %04x => %d\n", "BOOT", 0xFF50, boot_bit);
  }

  // Addr bus on ext pins notes -
  // 0000 - 7FFF : addr on bus, no CSn
  // 8800 - 9FFF : only A15 changes
  // A000 - F800 : addr on bus and CSn goes low for a bit
  // FE00 - FFFF : addr on bus but no CSn

#if 0
  test_reg("JOYP", 0xFF00, 177, use_fast_impl);

  test_reg("SB",   0xFF01, 177, use_fast_impl);
  test_reg("SC",   0xFF02, 177, use_fast_impl);

  test_reg("DIV",  0xFF04, 177, use_fast_impl);
  test_reg("TIMA", 0xFF05, 177, use_fast_impl);
  test_reg("TMA",  0xFF06, 177, use_fast_impl);
  test_reg("TAC",  0xFF07, 177, use_fast_impl);

  test_reg("LCDC", 0xFF40, 177, use_fast_impl); // so if we enable the ppu, we crash after a while because we get stuck in rendering state because we're not emitting pixels
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
  printf("\n");
#endif

#if 0
  printf("//----------------------------------------\n");

  printf("// Testing Cart ROM read: ");
  test_mem(0x0000, 0x7FFF, 256, false, use_fast_impl);
  printf("done\n");

  printf("// Testing VRAM read/write: ");
  test_mem(0x8000, 0x9FFF, 256, true, use_fast_impl);
  printf("done\n");

  printf("// Testing Cart RAM read/write: ");
  test_mem(0xA000, 0xBFFF, 256, true, use_fast_impl);
  printf("done\n");

  printf("// Testing Main RAM read/write: ");
  test_mem(0xC000, 0xDFFF, 256, true, use_fast_impl);
  printf("done\n");

  printf("// Testing Echo RAM read/write: ");
  test_mem(0xE000, 0xFDFF, 256, true, use_fast_impl);
  printf("done\n");

  printf("// Testing OAM RAM read/write: ");
  test_mem(0xFE00, 0xFEFF, 1, true, use_fast_impl);
  printf("done\n");

  printf("// Testing ZRAM read/write: ");
  test_mem(0xFF80, 0xFFFE, 1, true, use_fast_impl);
  printf("done\n");
#endif

  dbg_write(0xFF40, 0x80, use_fast_impl);

  if (verbose) printf("\n");
}

//------------------------------------------------------------------------------

void GateBoy::test_mem(uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write, bool use_fast_impl) {
  for (uint16_t addr = addr_start; addr <= addr_end; addr += step) {
    uint8_t data_wr = uint8_t(mix3(addr ^ 1234) | 1);
    if (test_write) {
      mem[addr] = 0;
      dbg_write(addr, data_wr, use_fast_impl);
    }
    else {
      mem[addr] = data_wr;
    }

    uint8_t data_rd = dbg_read(addr, use_fast_impl);
    if (data_rd != data_wr) {
      printf("FAIL @ 0x%04x : expected 0x%02x, was 0x%02x\n", addr, data_wr, data_rd);
    }
  }
}

//------------------------------------------------------------------------------

void GateBoy::test_reg(const char* regname, uint16_t addr, uint8_t data_in, bool use_fast_impl) {
  dbg_write(addr, data_in, use_fast_impl);
  uint8_t data_out = dbg_read(addr, use_fast_impl);
  printf("%5s @ %04x: wrote %02x, read %02x\n", regname, addr, data_in, data_out);
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

  /* A->B */ phase(req, false, use_fast_impl);
  /* B->C */ phase(req, false, use_fast_impl);
  /* C->D */ phase(req, false, use_fast_impl);
  /* D->E */ phase(req, false, use_fast_impl);
  /* E->F */ phase(req, false, use_fast_impl);
  /* F->G */ phase(req, false, use_fast_impl);

  uint8_t sample = top.cpu_bus.get_bus_data();

  /* G->H */ phase(req, false, use_fast_impl);
  /* H->A */ phase(req, false, use_fast_impl);

  return sample;
}

void GateBoy::dbg_write(uint16_t addr, uint8_t data, bool use_fast_impl) {
  CHECK_P((phase_total & 7) == 0);
  Req req = {.addr = addr, .data = data, .read = 0, .write = 1 };

  /* A->B */ phase(req, false, use_fast_impl);
  /* B->C */ phase(req, false, use_fast_impl);
  /* C->D */ phase(req, false, use_fast_impl);
  /* D->E */ phase(req, false, use_fast_impl);
  /* E->F */ phase(req, false, use_fast_impl);
  /* F->G */ phase(req, false, use_fast_impl);
  /* G->H */ phase(req, false, use_fast_impl);
  /* H->A */ phase(req, false, use_fast_impl);
}

//------------------------------------------------------------------------------

void GateBoy::phase(Req req, bool verbose, bool use_fast_impl) {
  (void)use_fast_impl;

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
    update_zram_bus(phase);

    top.tick_slow(phase);
    /*
    if (use_fast_impl) {
      top.tick_fast(phase);
    }
    else {
      top.tick_slow(phase);
    }
    */

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
  (void)phase_clock;

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
  /*read  rom */ CPU_PIN_HOLD_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write rom */ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  vram*/ CPU_PIN_HOLD_MEM = true;  CPU_PIN_ADDR_EXT = false;
  /*write vram*/ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = false;

  /*read  cram*/ CPU_PIN_HOLD_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write cram*/ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  eram*/ CPU_PIN_HOLD_MEM = true;  CPU_PIN_ADDR_EXT = true;
  /*write eram*/ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = true;

  /*read  oam */ CPU_PIN_HOLD_MEM = true;  CPU_PIN_ADDR_EXT = false;
  /*write oam */ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = false;

  /*read  hram*/ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = dc;
  /*write hram*/ CPU_PIN_HOLD_MEM = dc;    CPU_PIN_ADDR_EXT = false;

#endif

  // CPU_PIN_ADDR_EXT latches the cpu bus address into the ext address

  bool read = req.read;
  bool write = req.write;
  uint16_t addr = req.addr;
  uint8_t data = req.data_lo;

  bool io = req.read || req.write;
  bool addr_ext = io && ((req.addr <= 0x7FFF) || (req.addr >= 0xA000 && req.addr < 0xFDFF));
  bool hold_mem = io && (req.addr < 0xFF00);

#if 0
    NR14_STOP = dff(nand(CPU_PIN_HOLD_MEM, CPU_PIN_WRp, xxxxEFGx, FF14), APU_RSTn, TS_D6());
    NR24_STOP = dff(nand(CPU_PIN_HOLD_MEM, CPU_PIN_WRp, xxxxEFGx, FF19), APU_RSTn, TS_D3());
    NR34_STOP = dff(nand(CPU_PIN_HOLD_MEM, CPU_PIN_WRp, xxxxEFGx, FF1E), APU_RSTn, TS_D3());
    NR44_STOP = dff(nand(CPU_PIN_HOLD_MEM, CPU_PIN_WRp, xxxxEFGx, FF23), APU_RSTn, TS_D6());

    OAM_PIN_WRp = and(CPU_PIN_WRp,                      ADDR_OAM, xxxxEFGx);
    OAM_PIN_OEp = and(CPU_PIN_RDp, !CPU_PIN_HOLD_MEM,   ADDR_OAM);
    OBD_TO_OBL  = and(CPU_PIN_RDp, !CPU_PIN_HOLD_MEM,   ADDR_OAM);
    TIMA_LOADp  = and(CPU_PIN_WRp, !CPU_PIN_HOLD_MEM,   ADDR_TIMA, xxxxEFGx, );

    EPD_TO_CBD  = and(CPU_PIN_RDp,  CPU_PIN_HOLD_MEM,  !ADDR_VRAM);
    OBL_TO_CBD  = and(CPU_PIN_RDp,  CPU_PIN_HOLD_MEM,   ADDR_OAM);
    VBD_TO_CBD  = and(CPU_PIN_RDp,  CPU_PIN_HOLD_MEM,   ADDR_VRAM, !CPU_PIN_ADDR_EXT);

    if (TIMA_LOADp) {
      TIMA_7 = TIMA_LD_7);
      TIMA_MAX = 0;
    }
    else {
      TIMA_7 = ff20(!TIMA_6, TIMA_LD_7);
      TIMA_MAX = dff17(xBCDEFGH, 1, TIMA_7);
    }


#endif

  if (PHASE_A) {
    bus.preset_addr_z();   bus.preset_data_z();          bus.CPU_PIN_RDp.preset(0);    bus.CPU_PIN_WRp.preset(0);     bus.CPU_PIN_HOLD_MEM.preset(0);        bus.CPU_PIN_ADDR_EXT.preset(0);
  }
  if (PHASE_B) {
    bus.preset_addr(addr); bus.preset_data_z();          bus.CPU_PIN_RDp.preset(read); bus.CPU_PIN_WRp.preset(write); bus.CPU_PIN_HOLD_MEM.preset(0);        bus.CPU_PIN_ADDR_EXT.preset(addr_ext);
  }
  if (PHASE_C) {
    bus.preset_addr(addr); bus.preset_data_z();          bus.CPU_PIN_RDp.preset(read); bus.CPU_PIN_WRp.preset(write); bus.CPU_PIN_HOLD_MEM.preset(0);        bus.CPU_PIN_ADDR_EXT.preset(addr_ext);
  }
  if (PHASE_D) {
    bus.preset_addr(addr); bus.preset_data_z();          bus.CPU_PIN_RDp.preset(read); bus.CPU_PIN_WRp.preset(write); bus.CPU_PIN_HOLD_MEM.preset(0);        bus.CPU_PIN_ADDR_EXT.preset(addr_ext);
  }
  if (PHASE_E) {
    bus.preset_addr(addr); bus.preset_data(write, data); bus.CPU_PIN_RDp.preset(read); bus.CPU_PIN_WRp.preset(write); bus.CPU_PIN_HOLD_MEM.preset(hold_mem); bus.CPU_PIN_ADDR_EXT.preset(addr_ext);
  }
  if (PHASE_F) {
    bus.preset_addr(addr); bus.preset_data(write, data); bus.CPU_PIN_RDp.preset(read); bus.CPU_PIN_WRp.preset(write); bus.CPU_PIN_HOLD_MEM.preset(hold_mem); bus.CPU_PIN_ADDR_EXT.preset(addr_ext);
  }
  if (PHASE_G) {
    bus.preset_addr(addr); bus.preset_data(write, data); bus.CPU_PIN_RDp.preset(read); bus.CPU_PIN_WRp.preset(write); bus.CPU_PIN_HOLD_MEM.preset(hold_mem); bus.CPU_PIN_ADDR_EXT.preset(addr_ext);
  }
  if (PHASE_H) {
    bus.preset_addr(addr); bus.preset_data(write, data); bus.CPU_PIN_RDp.preset(read); bus.CPU_PIN_WRp.preset(write); bus.CPU_PIN_HOLD_MEM.preset(hold_mem); bus.CPU_PIN_ADDR_EXT.preset(addr_ext);
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

  top.ext_bus.EXT_PIN_WR_C.preset(top.ext_bus.EXT_PIN_WR_A);
  top.ext_bus.EXT_PIN_RD_C.preset(top.ext_bus.EXT_PIN_RD_A);

  top.ext_bus.EXT_PIN_A00_C.preset(top.ext_bus.EXT_PIN_A00_A);
  top.ext_bus.EXT_PIN_A01_C.preset(top.ext_bus.EXT_PIN_A01_A);
  top.ext_bus.EXT_PIN_A02_C.preset(top.ext_bus.EXT_PIN_A02_A);
  top.ext_bus.EXT_PIN_A03_C.preset(top.ext_bus.EXT_PIN_A03_A);
  top.ext_bus.EXT_PIN_A04_C.preset(top.ext_bus.EXT_PIN_A04_A);
  top.ext_bus.EXT_PIN_A05_C.preset(top.ext_bus.EXT_PIN_A05_A);
  top.ext_bus.EXT_PIN_A06_C.preset(top.ext_bus.EXT_PIN_A06_A);
  top.ext_bus.EXT_PIN_A07_C.preset(top.ext_bus.EXT_PIN_A07_A);
  top.ext_bus.EXT_PIN_A08_C.preset(top.ext_bus.EXT_PIN_A08_A);
  top.ext_bus.EXT_PIN_A09_C.preset(top.ext_bus.EXT_PIN_A09_A);
  top.ext_bus.EXT_PIN_A10_C.preset(top.ext_bus.EXT_PIN_A10_A);
  top.ext_bus.EXT_PIN_A11_C.preset(top.ext_bus.EXT_PIN_A11_A);
  top.ext_bus.EXT_PIN_A12_C.preset(top.ext_bus.EXT_PIN_A12_A);
  top.ext_bus.EXT_PIN_A13_C.preset(top.ext_bus.EXT_PIN_A13_A);
  top.ext_bus.EXT_PIN_A14_C.preset(top.ext_bus.EXT_PIN_A14_A);
  top.ext_bus.EXT_PIN_A15_C.preset(top.ext_bus.EXT_PIN_A15_A);

  if (top.ext_bus.EXT_PIN_WR_A) {
    uint16_t ext_addr = ~top.ext_bus.get_pin_addr();
    uint8_t ext_data = ~top.ext_bus.get_pin_data_out();
    
    if (ext_addr >= 0 && ext_addr <= 0x7FFF) {
      // FIXME yeah we don't actually want to allow writing to ROM...
      mem[ext_addr] = ext_data;
      //printf("PHASE %C: EXT WRITE %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xA000 && ext_addr <= 0xBFFF) {
      mem[ext_addr] = ext_data;
      //printf("PHASE %C: EXT WRITE %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xC000 && ext_addr <= 0xDFFF) {
      mem[ext_addr] = ext_data;
      //printf("PHASE %C: EXT WRITE %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xE000 && ext_addr <= 0xFFFF) {
      mem[ext_addr] = ext_data;
      //printf("PHASE %C: EXT WRITE %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
  }

  if (top.ext_bus.EXT_PIN_RD_A) {
    uint16_t ext_addr = ~top.ext_bus.get_pin_addr();

    if (ext_addr >= 0 && ext_addr <= 0x7FFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.preset_pin_data_in(ext_data);
      //printf("PHASE %C: EXT_READ  %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xA000 && ext_addr <= 0xBFFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.preset_pin_data_in(ext_data);
      //printf("PHASE %C: EXT_READ  %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xC000 && ext_addr <= 0xDFFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.preset_pin_data_in(ext_data);
      //printf("PHASE %C: EXT_READ  %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else if (ext_addr >= 0xE000 && ext_addr <= 0xFFFF) {
      uint8_t ext_data = mem[ext_addr];
      top.ext_bus.preset_pin_data_in(ext_data);
      //printf("PHASE %C: EXT_READ  %04x %d\n", 'A' + phase, ext_addr, ext_data);
    }
    else {
      //printf("PHASE %C: EXT_READ  %04x (bad addr)\n", 'A' + phase, ext_addr);
      top.ext_bus.preset_pin_data_z();
    }
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
    uint8_t vram_pin_data_out = ~top.vram_bus.get_pin_data_out();
    //printf("vram[%04x] = %d\n", vram_pin_addr, data);
    mem[vram_pin_addr + 0x8000] = vram_pin_data_out;
    //printf("PHASE %C: VRAM WRITE %04x %d\n", 'A' + phase, vram_pin_addr, vram_pin_data_out);
  }

  if (top.vram_bus._VRAM_PIN_OE_A) {
    uint8_t vram_pin_data_in = mem[vram_pin_addr + 0x8000];
    top.vram_bus.preset_pin_data_in(vram_pin_data_in);

    // io reads are showing up on the vram bus and they probably shouldn't be
    //printf("PHASE %C: VRAM READ %04x %d\n", 'A' + phase, vram_pin_addr, vram_pin_data_in);
  }
  else {
    top.vram_bus.preset_pin_data_z();
  }
}

//-----------------------------------------------------------------------------

void GateBoy::update_oam_bus(int phase) {
  (void)phase;

  int oam_addr = uint8_t(~top.oam_bus.get_oam_bus_addr());
  uint16_t* oam_base = (uint16_t*)(&mem[0xFE00]);

  if (!top.oam_bus.OAM_PIN_OE) {
    top.oam_bus.preset_bus_data(true, oam_base[oam_addr >> 1]);
  }

  if (!top.oam_bus.OAM_PIN_WR_A) {
    mem[0xFE00 + oam_addr] = top.oam_bus.get_oam_bus_data_a();
  }
  
  if (!top.oam_bus.OAM_PIN_WR_B) {
    mem[0xFE00 + oam_addr] = top.oam_bus.get_oam_bus_data_b();
  }
}
//-----------------------------------------------------------------------------

void GateBoy::update_zram_bus(int phase) {
  (void)phase;

  // Control signals for zram aren't traced, so we're just going to make up something
  // reasonable.

  int addr = top.cpu_bus.get_bus_addr();
  bool hit_zram = (addr >= 0xFF80) && (addr <= 0xFFFE);

  if (hit_zram) {
    if (top.cpu_bus.CPU_PIN_WRp && PHASE_H) {
      mem[addr] = top.cpu_bus.get_bus_data();
    }

    if (top.cpu_bus.CPU_PIN_RDp) {
      top.cpu_bus.preset_data(true, mem[addr]);
    }
  }
}


//-----------------------------------------------------------------------------