#include "GateBoyTests/GateBoyTests.h"
#include "GateBoyLib/GateBoy.h"
#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include <chrono>

//-----------------------------------------------------------------------------

int GateBoyTests::test_main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  TEST_START("Maaaaaain");

  err += test_clk();
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

#define EXPECT_CLK(A, B) EXPECT_EQ(wire(A), wire((B) & (1 << (7 - phase))), "Clock phase mismatch, %s at phase %d", #A, phase);

int GateBoyTests::test_clk() {
  TEST_START();

  GateBoy gb;
  gb.reset();

  auto& clk_reg = gb.top.clk_reg;

  for (int i = 0; i < 32; i++) {
    int phase = gb.phase_total & 7;
    wire CLK = phase & 1;
    wire CLKGOOD = 1;

    EXPECT_CLK(clk_reg.ATAL_xBxDxFxH(CLK), 0b01010101);
    EXPECT_CLK(clk_reg.ZAXY_xBxDxFxH(CLK), 0b01010101);
    EXPECT_CLK(clk_reg.AZOF_AxCxExGx(CLK), 0b10101010); 
    EXPECT_CLK(clk_reg.ZAXY_xBxDxFxH(CLK), 0b01010101); 
    EXPECT_CLK(clk_reg.ZEME_AxCxExGx(CLK), 0b10101010); 
    EXPECT_CLK(clk_reg.ALET_xBxDxFxH(CLK), 0b01010101); 
    EXPECT_CLK(clk_reg.MEHE_AxCxExGx(CLK), 0b10101010); 
    EXPECT_CLK(clk_reg.MYVO_AxCxExGx(CLK), 0b10101010); 

    EXPECT_CLK(clk_reg.AFUR_xxxxEFGH.qp(), 0b00001111);
    EXPECT_CLK(clk_reg.ALEF_AxxxxFGH.qp(), 0b10000111);
    EXPECT_CLK(clk_reg.APUK_ABxxxxGH.qp(), 0b11000011);
    EXPECT_CLK(clk_reg.ADYK_ABCxxxxH.qp(), 0b11100001);

    EXPECT_CLK(clk_reg.AROV_xxCDEFxx,      0b00111100);
    EXPECT_CLK(clk_reg.AFEP_AxxxxFGH,      0b10000111);
    EXPECT_CLK(clk_reg.ATYP_ABCDxxxx,      0b11110000);
    EXPECT_CLK(clk_reg.AJAX_xxxxEFGH,      0b00001111);

    EXPECT_CLK(clk_reg.ADAR_ABCxxxxH(),    0b11100001);
    EXPECT_CLK(clk_reg.AFAS_xxxxEFGx(),    0b00001110);

    EXPECT_CLK(clk_reg.BELU_xxxxEFGH(), 0b00001111);
    EXPECT_CLK(clk_reg.BYRY_ABCDxxxx(), 0b11110000);
    EXPECT_CLK(clk_reg.BUDE_xxxxEFGH(), 0b00001111);
    EXPECT_CLK(clk_reg.UVYT_ABCDxxxx(), 0b11110000);
    EXPECT_CLK(clk_reg.BEKO_ABCDxxxx(), 0b11110000);
    EXPECT_CLK(clk_reg.MOPA_xxxxEFGH(), 0b00001111);
    EXPECT_CLK(clk_reg.XYNY_ABCDxxxx(), 0b11110000);

    EXPECT_CLK(clk_reg.BAPY_xxxxxxGH(), 0b00000011);
    EXPECT_CLK(clk_reg.BERU_ABCDEFxx(), 0b11111100);
    EXPECT_CLK(clk_reg.BUFA_xxxxxxGH(), 0b00000011);
    EXPECT_CLK(clk_reg.BOLO_ABCDEFxx(), 0b11111100);

    EXPECT_CLK(clk_reg.BEJA_xxxxEFGH(), 0b00001111);
    EXPECT_CLK(clk_reg.BANE_ABCDxxxx(), 0b11110000);
    EXPECT_CLK(clk_reg.BELO_xxxxEFGH(), 0b00001111);
    EXPECT_CLK(clk_reg.BAZE_ABCDxxxx(), 0b11110000);

    EXPECT_CLK(clk_reg.BUTO_xBCDEFGH(), 0b01111111);
    EXPECT_CLK(clk_reg.BELE_Axxxxxxx(), 0b10000000);
    EXPECT_CLK(clk_reg.BYJU_xBCDEFGH(CLKGOOD), 0b01111111);
    EXPECT_CLK(clk_reg.BALY_Axxxxxxx(CLKGOOD), 0b10000000);
    EXPECT_CLK(clk_reg.BOGA_xBCDEFGH(CLKGOOD), 0b01111111);
    EXPECT_CLK(clk_reg.BOMA_Axxxxxxx(CLKGOOD), 0b10000000);

    EXPECT_CLK(clk_reg.CPU_PIN_BOWA_xBCDEFGH.tp(), 0b01111111);
    EXPECT_CLK(clk_reg.CPU_PIN_BEDO_Axxxxxxx.tp(), 0b10000000);
    EXPECT_CLK(clk_reg.CPU_PIN_BEKO_ABCDxxxx.tp(), 0b11110000);
    EXPECT_CLK(clk_reg.CPU_PIN_BUDE_xxxxEFGH.tp(), 0b00001111);
    EXPECT_CLK(clk_reg.CPU_PIN_BOLO_ABCDEFxx.tp(), 0b11111100);
    EXPECT_CLK(clk_reg.CPU_PIN_BUKE_AxxxxxGH.tp(), 0b10000011);
    EXPECT_CLK(clk_reg.CPU_PIN_BOMA_Axxxxxxx.tp(), 0b10000000);
    EXPECT_CLK(clk_reg.CPU_PIN_BOGA_xBCDEFGH.tp(), 0b01111111);
    EXPECT_CLK(clk_reg.EXT_PIN_CLK_xxxxEFGH.tp(),  0b00001111);
    gb.next_phase();
  }

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_mem() {
  TEST_START();

  GateBoy gb;
  gb.reset();
  gb.set_boot_bit();

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

  test_reg("TIMA", ADDR_TIMA, 0b11111111); // works
  test_reg("TMA",  ADDR_TMA,  0b11111111); // works
  test_reg("TAC",  ADDR_TAC,  0b00000111); // works

  // 00 - 2048 phases per TIMA tick (looks ok)
  // 01 - 32 phases per TIMA tick   (looks ok)
  // 10 - 128 phases per TIMA tick  (looks ok)
  // 11 - 512 phases per TIMA tick

  // FF04 DIV
  // FF05 TIMA
  // FF06 TMA
  // FF07 TAC

  {
    //int delay = 0;

    GateBoy gb;
    gb.reset();

    gb.dbg_write(ADDR_TMA, 0x80);
    gb.dbg_write(ADDR_TIMA,0xFD);
    gb.dbg_write(ADDR_DIV, 0x00);

    LOG("DIV  %d\n",     gb.top.tim_reg.get_div());
    LOG("TIMA 0x%02x\n", gb.top.tim_reg.get_tima());
    gb.dbg_write(ADDR_TAC, 0b00000101);
    LOG("DIV  %d\n",     gb.top.tim_reg.get_div());
    LOG("TIMA 0x%02x\n", gb.top.tim_reg.get_tima());
    gb.dbg_write(ADDR_DIV, 0x00);
    LOG("DIV  %d\n",     gb.top.tim_reg.get_div());
    LOG("TIMA 0x%02x\n", gb.top.tim_reg.get_tima());

    /*
    gb.NOP();
    gb.LDH_A8_A(0x06, 0x80);
    gb.NOP();
    gb.LDH_A8_A(0x05, 0xFD);

    // ld a, 0x05
    gb.NOP();
    gb.NOP();

    gb.LDH_A8_A(07, 0x05);
    gb.LDH_A8_A(04, 0x05);
    
    //gb.NOPS(delay);
    */

    //uint8_t a = gb.LDH_A_A8(0x05);
    //LOG("delay %d TIMA 0x%02x\n", delay, a);
  }

  /*
  00:   4096 Hz    (~4194 Hz SGB)
  01: 262144 Hz  (~268400 Hz SGB)
  10:  65536 Hz   (~67110 Hz SGB)
  11:  16384 Hz   (~16780 Hz SGB)
  */

#if 0
  GateBoy gb;
  gb.reset();

  // passes, but slow :/
  LOG("Testing div reset + rollover, this takes a minute...");
  gb.dbg_write(ADDR_DIV, 0);
  for (int i = 1; i < 32768; i++) {
    int div_a = gb.dbg_read(ADDR_DIV);
    int div_b = (i >> 6) & 0xFF;
    EXPECT_EQ(div_a, div_b, "div match fail");
  }
  LOG("\n");
#endif

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_joypad() {
  TEST_START();

  err += test_reg("JOYP", ADDR_P1,   0b00110000);

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

  //err += test_reg("LCDC", ADDR_LCDC, 0b11111111); // works but let's not muck up lcdc
  //err += test_reg("STAT", ADDR_STAT, 0b11110000); // broken
  err += test_reg("SCY",  ADDR_SCY,  0b11111111);
  err += test_reg("SCX",  ADDR_SCX,  0b11111111);
  //err += test_reg("LY",   0xFF44, 177); // not standard reg
  err += test_reg("LYC",  ADDR_LYC,  0b11111111);
  err += test_reg("BGP",  ADDR_BGP,  0b11111111);
  err += test_reg("OBP0", ADDR_OBP0, 0b11111111);
  err += test_reg("OBP1", ADDR_OBP1, 0b11111111);
  err += test_reg("WY",   ADDR_WY,   0b11111111);
  err += test_reg("WX",   ADDR_WX,   0b11111111);

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

int GateBoyTests::test_reg(const char* tag, uint16_t addr, uint8_t mask) {
  TEST_START("%-4s @ 0x%04x, mask 0x%02x", tag, addr, mask);

  GateBoy gb;
  gb.reset();

  for (int i = 0; i < 256; i++) {
    uint8_t data_in = uint8_t(i & mask);
    gb.dbg_write(addr, uint8_t(data_in));
    uint8_t data_out = gb.dbg_read(addr) & mask;
    EXPECT_EQ(data_in, data_out, "reg %s @ 0x%04x: wrote 0x%02x, read 0x%02x", tag, addr, data_in, data_out);
  }

  TEST_END();
}

//------------------------------------------------------------------------------







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
    //gateboy.top.clk_reg.preset_clk_a(wire(rng & 0x02));
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

