#include "GateBoyTests.h"

#include "GateBoy.h"
#include "Constants.h"
#include <chrono>
#include <stdarg.h>

//-----------------------------------------------------------------------------

void GateBoyTests::test_rom_read() {
  GateBoy gateboy;
  gateboy.reset();

  gateboy.mem[0] =  0x23;

  for (int i = 0; i < 16; i++) {
    uint8_t rom = gateboy.dbg_read((uint16_t)i);
    printf("addr 0x%04x data 0x%02x\n", i, rom);
  }
}

//-----------------------------------------------------------------------------

void GateBoyTests::fuzz_reset_sequence(GateBoy& gateboy) {
  printf("GateBoy::fuzz_reset_sequence\n");

  uint64_t rng = 1;

  Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };

#ifdef _DEBUG
  const int fuzz_count = 128;
#else
  const int fuzz_count = 65536;
#endif

  for (int i = 0; i < fuzz_count; i++) {
    mix(rng);

    gateboy.top.clk_reg.preset_rst(wire(rng & 0x01));
    gateboy.top.clk_reg.preset_clk_a(wire(rng & 0x02));
    gateboy.top.clk_reg.preset_cpu_ready(wire(rng & 0x04));
    gateboy.top.clk_reg.preset_t1t2(wire(rng & 0x08), wire(rng & 0x10));

    int phase_count = (rng >> 8) & 0x0F;
    gateboy.run(phase_count);

    if ((i & 0xFF) == 0xFF) printf(".");
  }
  printf("\n");
}

//-----------------------------------------------------------------------------

void GateBoyTests::test_reset_sequence() {
  printf("Running reset fuzz test in slow mode\n");
  GateBoy gateboy1;
  fuzz_reset_sequence(gateboy1);
  printf("\n");

  printf("Running reset fuzz test in fast mode\n");
  GateBoy gateboy2;
  fuzz_reset_sequence(gateboy2);
  printf("\n");

  if (gateboy1.phase_hash != gateboy2.phase_hash) {
    printf("XXXXXXXXXX FAIL PHASE HASH XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS PHASE HASH ----------\n");
  }

  if (gateboy1.total_hash != gateboy2.total_hash) {
    printf("XXXXXXXXXX FAIL TOTAL HASH XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS TOTAL HASH ----------\n");
  }
}

//-----------------------------------------------------------------------------

template<typename T>
void dump_blob(T& blob) {
  uint8_t* base = (uint8_t*)(&blob);

  for (int i = 0; i < sizeof(T); i++) {
    printf("%02x ", base[i]);
    if ((i % 32) == 31) printf("\n");
  }
  printf("\n");
}

//-----------------------------------------------------------------------------

void GateBoyTests::run_benchmark(GateBoy& gateboy) {

  gateboy.reset();

  printf("Hash 1 after reset: 0x%016llx\n", gateboy.phase_hash);

#if _DEBUG
  const int iter_count = 16;
  const int phase_per_iter = 128;
  const int warmup = 0;
#else
  const int iter_count = 74;
  const int phase_per_iter = 8192;
  const int warmup = 10;
#endif

  double phase_rate_sum1 = 0;
  double phase_rate_sum2 = 0;
  double phase_rate_n = 0;

  double pass_rate_sum1 = 0;
  double pass_rate_sum2 = 0;
  double pass_rate_n = 0;

  gateboy.dbg_req = {.addr = 0x0150, .data = 0, .read = 1, .write = 0 };
  gateboy.cpu_en = false;

  printf("Running perf test");
  for (int iter = 0; iter < iter_count; iter++) {
    gateboy.phase_total = 0;
    gateboy.pass_total = 0;

    //dump_blob(top);

    auto start = std::chrono::high_resolution_clock::now();
    gateboy.run(phase_per_iter);
    auto finish = std::chrono::high_resolution_clock::now();

    //dump_blob(top);

    if (iter >= warmup) {
      std::chrono::duration<double> elapsed = finish - start;
      double time = elapsed.count();

      double phase_rate = double(gateboy.phase_total) / time;
      phase_rate_sum1 += phase_rate;
      phase_rate_sum2 += phase_rate * phase_rate;
      phase_rate_n++;

      double pass_rate = double(gateboy.pass_total) / time;
      pass_rate_sum1 += pass_rate;
      pass_rate_sum2 += pass_rate * pass_rate;
      pass_rate_n++;
    }
    printf(".");
  }
  printf("Done\n");

  printf("Phase total %d\n", gateboy.phase_total);
  printf("Pass total %d\n", gateboy.pass_total);

  double phase_rate_mean     = phase_rate_sum1 / phase_rate_n;
  double phase_rate_variance = (phase_rate_sum2 / phase_rate_n) - (phase_rate_mean * phase_rate_mean);
  double phase_rate_sigma    = sqrt(phase_rate_variance);
  printf("Mean phase/sec %f sigma %f\n", phase_rate_mean, phase_rate_sigma);

  double pass_rate_mean     = pass_rate_sum1 / pass_rate_n;
  double pass_rate_variance = (pass_rate_sum2 / pass_rate_n) - (pass_rate_mean * pass_rate_mean);
  double pass_rate_sigma    = sqrt(pass_rate_variance);
  printf("Mean pass/sec %f sigma %f\n", pass_rate_mean, pass_rate_sigma);

  printf("Commit phase_hash   0x%016llx\n", gateboy.phase_hash);
  printf("Combined phase_hash 0x%016llx\n", gateboy.total_hash);
}

//-----------------------------------------------------------------------------

void GateBoyTests::run_benchmark() {
  printf("Running benchmark in slow mode\n");
  GateBoy gateboy1;
  run_benchmark(gateboy1);
  printf("\n");

#if 0
  printf("Running benchmark in fast mode\n");
  GateBoy gateboy2;
  run_benchmark(gateboy2);
  printf("\n");

  if (gateboy1.phase_hash != gateboy2.phase_hash) {
    printf("XXXXXXXXXX FAIL PHASE HASH XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS PHASE HASH ----------\n");
  }

  if (gateboy1.total_hash != gateboy2.total_hash) {
    printf("XXXXXXXXXX FAIL TOTAL HASH XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS TOTAL HASH ----------\n");
  }
#endif
}

//-----------------------------------------------------------------------------

void GateBoyTests::test_all_mem() {
  GateBoy gb;
  gb.reset();

  gb.run_bootrom();

  gb.log("Test Cart ROM read\n");
  test_mem(gb, 0x0000, 0x7FFF, 256, false);

  gb.log("Test VRAM read/write\n");
  test_mem(gb, 0x8000, 0x9FFF, 256, true);

  gb.log("Test Cart RAM read/write\n");
  test_mem(gb, 0xA000, 0xBFFF, 256, true);

  gb.log("Test Main RAM read/write\n");
  test_mem(gb, 0xC000, 0xDFFF, 256, true);

  gb.log("Test Echo RAM read/write\n");
  test_mem(gb, 0xE000, 0xFDFF, 256, true);

  gb.log("Test OAM RAM read/write\n");
  test_mem(gb, 0xFE00, 0xFEFF, 1, true);

  gb.log("Test ZRAM read/write\n");
  test_mem(gb, 0xFF80, 0xFFFE, 1, true);

  gb.log("\n");
}

//------------------------------------------------------------------------------

void GateBoyTests::test_interrupts() {
  GateBoy gb;
  gb.reset();

  // hblank no stat int
  // vblank no stat int

  /*
  #define EI_GLITCH    0b10000000
  #define EI_LYC       0b01000000
  #define EI_OAM       0b00100000
  #define EI_VBLANK    0b00010000
  #define EI_HBLANK    0b00001000
  */
  //dbg_write(ADDR_STAT, EI_OAM);
  //dbg_write(ADDR_IE,   0b11111111);

  gb.log("Test interrupts\n");
  //test_reg("IF",   0xFF0F, 0b00011111); // broken
  //test_reg("IE",   0xFFFF, 0b00011111);
  gb.log("\n");
}

//------------------------------------------------------------------------------

void GateBoyTests::test_bootrom() {
  GateBoy gb;
  gb.reset();

  for (int i = 0; i < 16; i++) {
    printf("bootrom @ 0x%04x = 0x%02x\n", i, gb.dbg_read(i));
  }

  gb.log("Test bootrom\n");
  gb.log("\n");
}

//------------------------------------------------------------------------------

void GateBoyTests::test_timer() {
  GateBoy gb;
  gb.reset();

  gb.log("Test Timer\n");
  //test_reg("DIV",  ADDR_DIV, 0b11111111); // not standard reg
  //test_reg("TIMA", ADDR_TIMA, 0b11111111); // broken
  //test_reg("TMA",  ADDR_TMA,  0b11111111); // works
  //test_reg("TAC",  ADDR_TAC,  0b00000111); // works
  gb.log("\n");
}

//------------------------------------------------------------------------------

void GateBoyTests::test_joypad() {
  GateBoy gb;
  gb.reset();

  gb.log("Test Joypad\n");
  test_reg(gb, "JOYP", ADDR_P1,   0b00110000);
  gb.log("\n");
}

//------------------------------------------------------------------------------

void GateBoyTests::test_dma() {
  GateBoy gb;
  gb.reset();

  gb.log("Test DMA\n");
  //test_reg("DMA",  0xFF46, 0b11111111); // works, but let's not trigger random dmas...
  gb.log("\n");
}

//------------------------------------------------------------------------------

void GateBoyTests::test_serial() {
  GateBoy gb;
  gb.log("Test serial port\n");
  //test_reg("SB",   ADDR_SB,   0b11111111); // something wrong with these ones
  //test_reg("SC",   ADDR_SC, 0b10000011);
  gb.log("\n");
}

//------------------------------------------------------------------------------

void GateBoyTests::test_ppu() {
  GateBoy gb;
  gb.reset();

  gb.log("Test PPU\n");
  //test_reg("LCDC", ADDR_LCDC, 0b11111111); // works but let's not muck up lcdc
  //test_reg("STAT", ADDR_STAT, 0b11110000); // broken
  test_reg(gb, "SCY",  ADDR_SCY,  0b11111111);
  test_reg(gb, "SCX",  ADDR_SCX,  0b11111111);
  //test_reg("LY",   0xFF44, 177); // not standard reg
  test_reg(gb, "LYC",  ADDR_LYC, 0b11111111);
  test_reg(gb, "BGP",  ADDR_BGP,  0b11111111);
  test_reg(gb, "OBP0", ADDR_OBP0, 0b11111111);
  test_reg(gb, "OBP1", ADDR_OBP1, 0b11111111);
  test_reg(gb, "WY",   ADDR_WY,   0b11111111);
  test_reg(gb, "WX",   ADDR_WX,   0b11111111);
  gb.log("\n");
}

//------------------------------------------------------------------------------

void GateBoyTests::test_mem(GateBoy& gb, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write) {
  for (uint16_t addr = addr_start; addr <= addr_end; addr += step) {
    uint8_t data_wr = 0x55;
    if (test_write) {
      gb.mem[addr] = 0;
      gb.dbg_write(addr, data_wr);
    }
    else {
      gb.mem[addr] = data_wr;
    }

    uint8_t data_rd = gb.dbg_read(addr);
    if (data_rd != data_wr) {
      gb.log("XXXXXXXXXXXXXXXXX FAIL XXXXXXXXXXXXXXXXX @ 0x%04x : expected 0x%02x, was 0x%02x\n", addr, data_wr, data_rd);
      return;
    }
  }

  for (uint16_t addr = addr_start; addr <= addr_end; addr += step) {
    uint8_t data_wr = 0xAA;
    if (test_write) {
      gb.mem[addr] = 0;
      gb.dbg_write(addr, data_wr);
    }
    else {
      gb.mem[addr] = data_wr;
    }

    uint8_t data_rd = gb.dbg_read(addr);
    if (data_rd != data_wr) {
      gb.log("XXXXXXXXXXXXXXXXX FAIL XXXXXXXXXXXXXXXXX @ 0x%04x : expected 0x%02x, was 0x%02x\n", addr, data_wr, data_rd);
      return;
    }
  }

  gb.log("PASS\n");
}

//------------------------------------------------------------------------------

void GateBoyTests::test_reg(GateBoy& gb, const char* regname, uint16_t addr, uint8_t mask) {

  gb.log("Testing reg %5s: ", regname);
  for (int data_in = 0; data_in < 256; data_in++) {
    if (!(data_in & mask)) continue;
    gb.dbg_write(addr, uint8_t(data_in));
    uint8_t data_out = gb.dbg_read(addr);
    //printf("%5s @ %04x: wrote %02x, read %02x\n", regname, addr, data_in & mask, data_out & mask);

    if ((data_in & mask) != (data_out & mask)) {
      gb.log("XXXXXXXXXXXXXXXXX FAIL XXXXXXXXXXXXXXXXX - wrote 0x%02x, read 0x%02x\n", uint8_t(data_in & mask), uint8_t(data_out & mask));
      //return;
    }
  }
  gb.log("PASS\n");
}
