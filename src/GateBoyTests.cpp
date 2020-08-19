#include <stdio.h>
#include <stdarg.h>

void LOG(const char* format = "", ...) {
  static int log_indent = 0;
  static int log_cursor_x = 0;
  static char buffer[256];

  va_list args;
  va_start (args, format);
  vsnprintf(buffer, 256, format, args);
  va_end (args);

  for (char* b = buffer; *b; b++) {
    if (*b == '\n') {
      putchar(*b);
      log_cursor_x = 0;
    }
    else if (*b == '\t') {
      log_indent += 2;
    }
    else if (*b == '\v') {
      log_indent -= 2;
    }
    else {
      for (;log_cursor_x < log_indent; log_cursor_x++) putchar(' ');
      putchar(*b);
    }
  }
}

#define LOG_R(...) do { printf("\u001b[38;2;255;128;128m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_G(...) do { printf("\u001b[38;2;128;255;128m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_B(...) do { printf("\u001b[38;2;128;128;255m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);
#define LOG_Y(...) do { printf("\u001b[38;2;255;255;128m"); LOG(__VA_ARGS__); printf("\u001b[0m"); } while(0);

#define TEST_START(...) do { LOG("%s: ", __FUNCTION__); LOG_B(__VA_ARGS__); LOG("\n\t"); } while(0); int err = 0;
#define TEST_END()      do { LOG("\v"); if (err) LOG_R("%s: FAIL\n", __FUNCTION__); return err; } while(0);

#define EXPECT_EQ(A, B, ...) if ((A) != (B)) { LOG_Y(__VA_ARGS__); LOG("\n"); err++; }
#define EXPECT_NE(A, B, ...) if ((A) == (B)) { LOG_Y(__VA_ARGS__); LOG("\n"); err++; }

#define ASSERT_EQ(A, B, ...) if ((A) != (B)) { LOG_R(__VA_ARGS__); LOG("\n"); err++; TEST_END(); }
#define ASSERT_NE(A, B, ...) if ((A) == (B)) { LOG_R(__VA_ARGS__); LOG("\n"); err++; TEST_END(); }

#include <chrono>
#include "GateBoyTests.h"
#include "GateBoy.h"
#include "Constants.h"

//-----------------------------------------------------------------------------

int GateBoyTests::test_main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  TEST_START("Maaaaaain");

  err += test_mem();
  err += test_interrupts();
  err += test_bootrom();
  err += test_dma();
  err += test_joypad();
  err += test_ppu();
  err += test_serial();
  err += test_timer();

  if (!err) LOG_G("Everything passed!\n");

  TEST_END();
}

//-----------------------------------------------------------------------------

void GateBoyTests::fuzz_reset_sequence(GateBoy& gateboy) {
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
}

//-----------------------------------------------------------------------------

void GateBoyTests::test_reset_sequence() {
  LOG("Running reset fuzz test in slow mode\n");
  GateBoy gateboy1;
  fuzz_reset_sequence(gateboy1);
  LOG("\n");

  LOG("Running reset fuzz test in fast mode\n");
  GateBoy gateboy2;
  fuzz_reset_sequence(gateboy2);
  LOG("\n");

  if (gateboy1.phase_hash != gateboy2.phase_hash) {
    LOG("XXXXXXXXXX FAIL PHASE HASH XXXXXXXXXX\n");
  }
  else {
    LOG("---------- PASS PHASE HASH ----------\n");
  }

  if (gateboy1.total_hash != gateboy2.total_hash) {
    LOG("XXXXXXXXXX FAIL TOTAL HASH XXXXXXXXXX\n");
  }
  else {
    LOG("---------- PASS TOTAL HASH ----------\n");
  }
}

//-----------------------------------------------------------------------------

void GateBoyTests::run_benchmark(GateBoy& gateboy) {

  gateboy.reset();

  LOG("Hash 1 after reset: 0x%016llx\n", gateboy.phase_hash);

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

  LOG("Running perf test");
  for (int iter = 0; iter < iter_count; iter++) {
    gateboy.phase_total = 0;
    gateboy.pass_total = 0;

    auto start = std::chrono::high_resolution_clock::now();
    gateboy.run(phase_per_iter);
    auto finish = std::chrono::high_resolution_clock::now();

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
    LOG(".");
  }
  LOG("Done\n");

  LOG("Phase total %d\n", gateboy.phase_total);
  LOG("Pass total %d\n", gateboy.pass_total);

  double phase_rate_mean     = phase_rate_sum1 / phase_rate_n;
  double phase_rate_variance = (phase_rate_sum2 / phase_rate_n) - (phase_rate_mean * phase_rate_mean);
  double phase_rate_sigma    = sqrt(phase_rate_variance);
  LOG("Mean phase/sec %f sigma %f\n", phase_rate_mean, phase_rate_sigma);

  double pass_rate_mean     = pass_rate_sum1 / pass_rate_n;
  double pass_rate_variance = (pass_rate_sum2 / pass_rate_n) - (pass_rate_mean * pass_rate_mean);
  double pass_rate_sigma    = sqrt(pass_rate_variance);
  LOG("Mean pass/sec %f sigma %f\n", pass_rate_mean, pass_rate_sigma);

  LOG("Commit phase_hash   0x%016llx\n", gateboy.phase_hash);
  LOG("Combined phase_hash 0x%016llx\n", gateboy.total_hash);
}

//-----------------------------------------------------------------------------

void GateBoyTests::run_benchmark() {
  LOG("Running benchmark in slow mode\n");
  GateBoy gateboy1;
  run_benchmark(gateboy1);
  LOG("\n");

#if 0
  LOG("Running benchmark in fast mode\n");
  GateBoy gateboy2;
  run_benchmark(gateboy2);
  LOG("\n");

  if (gateboy1.phase_hash != gateboy2.phase_hash) {
    LOG("XXXXXXXXXX FAIL PHASE HASH XXXXXXXXXX\n");
  }
  else {
    LOG("---------- PASS PHASE HASH ----------\n");
  }

  if (gateboy1.total_hash != gateboy2.total_hash) {
    LOG("XXXXXXXXXX FAIL TOTAL HASH XXXXXXXXXX\n");
  }
  else {
    LOG("---------- PASS TOTAL HASH ----------\n");
  }
#endif
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_mem() {
  TEST_START();

  GateBoy gb;
  gb.reset();
  gb.run_bootrom();

  err += test_mem(gb, "ROM",  0x0000, 0x7FFF, 256, false);
  err += test_mem(gb, "VRAM", 0x8000, 0x9FFF, 256, true);
  err += test_mem(gb, "CRAM", 0xA000, 0xBFFF, 256, true);
  err += test_mem(gb, "IRAM", 0xC000, 0xDFFF, 256, true);
  err += test_mem(gb, "ERAM", 0xE000, 0xFDFF, 256, true);
  err += test_mem(gb, "OAM",  0xFE00, 0xFEFF, 1,   true);
  err += test_mem(gb, "ZRAM", 0xFF80, 0xFFFE, 1,   true);

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_interrupts() {
  TEST_START();

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

  //test_reg("IF",   0xFF0F, 0b00011111); // broken
  //test_reg("IE",   0xFFFF, 0b00011111);

  TEST_END();
}

//------------------------------------------------------------------------------

extern const uint8_t DMG_ROM_bin[];

int GateBoyTests::test_bootrom() {
  TEST_START();

  GateBoy gb;
  gb.reset();

  for (int i = 0; i < 16; i++) {
    uint8_t byte = gb.dbg_read(i);
    EXPECT_EQ(byte, DMG_ROM_bin[i], "bootrom @ 0x%04x = 0x%02x, expected 0x%02x", i, byte, DMG_ROM_bin[i]);
  }

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_timer() {
  TEST_START();

  GateBoy gb;
  gb.reset();

  //test_reg("DIV",  ADDR_DIV,  0b11111111); // not standard reg
  //test_reg("TIMA", ADDR_TIMA, 0b11111111); // broken
  //test_reg("TMA",  ADDR_TMA,  0b11111111); // works
  //test_reg("TAC",  ADDR_TAC,  0b00000111); // works

  LOG("Testing div reset + rollover: ");
  gb.dbg_write(ADDR_DIV, 0);
  for (int i = 1; i < 32768; i++) {
    int div_a = gb.dbg_read(ADDR_DIV);
    int div_b = (i >> 6) & 0xFF;
    EXPECT_EQ(div_a, div_b, "div match fail");
  }
  LOG("\n");

  gb.dbg_write(ADDR_TIMA, 0x00);
  gb.dbg_write(ADDR_TMA,  0x00);
  gb.dbg_write(ADDR_TAC,  0x00);
  LOG("TIMA 0x%02X\n", gb.dbg_read(ADDR_TIMA));
  LOG("TMA  0x%02X\n", gb.dbg_read(ADDR_TMA));
  LOG("TAC  0x%02X\n", gb.dbg_read(ADDR_TAC));

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_joypad() {
  TEST_START();

  GateBoy gb;
  gb.reset();

  err += test_reg(gb, "JOYP", ADDR_P1,   0b00110000);

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_dma() {
  TEST_START();

  GateBoy gb;
  gb.reset();

  for (int src = 0x0000; src < 0xFE00; src += 0x1000) {
    err += test_dma(uint16_t(src));
  }

  TEST_END();
}

//----------------------------------------

int GateBoyTests::test_dma(uint16_t src) {
  TEST_START("0x%04x", src);

  GateBoy gb;
  gb.reset();

  for (int i = 0; i < 256; i++) {
    gb.mem[src + i] = uint8_t(rand());
  }

  memset(gb.mem + 0xFE00, 0, 256);

  gb.dbg_write(0xFF46, uint8_t(src >> 8));

  for (int i = 0; i < 1288; i++) gb.next_phase();

  for (int i = 0; i < 160; i++) {
    uint8_t a = gb.mem[src + i];
    uint8_t b = gb.dbg_read(0xFE00 + i);
    EXPECT_EQ(a, b, "dma mismatch @ 0x%04x : expected 0x%02x, got 0x%02x", src + i, a, b);
  }

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_serial() {
  TEST_START();

  GateBoy gb;
  //err += test_reg(gb, "SB",   ADDR_SB,   0b11111111); // something wrong with these ones
  //test_reg("SC",   ADDR_SC, 0b10000011);

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_ppu() {
  TEST_START();

  GateBoy gb;
  gb.reset();

  //err += test_reg("LCDC", ADDR_LCDC, 0b11111111); // works but let's not muck up lcdc
  //err += test_reg(gb, "STAT", ADDR_STAT, 0b11110000); // broken
  err += test_reg(gb, "SCY",  ADDR_SCY,  0b11111111);
  err += test_reg(gb, "SCX",  ADDR_SCX,  0b11111111);
  //err += test_reg("LY",   0xFF44, 177); // not standard reg
  err += test_reg(gb, "LYC",  ADDR_LYC,  0b11111111);
  err += test_reg(gb, "BGP",  ADDR_BGP,  0b11111111);
  err += test_reg(gb, "OBP0", ADDR_OBP0, 0b11111111);
  err += test_reg(gb, "OBP1", ADDR_OBP1, 0b11111111);
  err += test_reg(gb, "WY",   ADDR_WY,   0b11111111);
  err += test_reg(gb, "WX",   ADDR_WX,   0b11111111);

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_mem(GateBoy& gb, const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write) {
  TEST_START("%-4s @ [0x%04x,0x%04x], step %3d write %d", tag, addr_start, addr_end, step, test_write);

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
    EXPECT_EQ(data_rd, data_wr, "addr 0x%04x : expected 0x%02x, was 0x%02x", addr, data_wr, data_rd);
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
    EXPECT_EQ(data_rd, data_wr, "addr 0x%04x : expected 0x%02x, was 0x%02x", addr, data_wr, data_rd);
  }

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_reg(GateBoy& gb, const char* tag, uint16_t addr, uint8_t mask) {
  TEST_START("%-4s @ 0x%04x, mask 0x%02x", tag, addr, mask);

  for (int i = 0; i < 256; i++) {
    uint8_t data_in = uint8_t(i & mask);
    gb.dbg_write(addr, uint8_t(data_in));
    uint8_t data_out = gb.dbg_read(addr) & mask;
    EXPECT_EQ(data_in, data_out, "reg %s @ 0x%04x: wrote 0x%02x, read 0x%02x", tag, addr, data_in, data_out);
  }

  TEST_END();
}

//------------------------------------------------------------------------------
