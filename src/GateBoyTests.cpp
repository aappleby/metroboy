#include "GateBoyTests.h"

#include "GateBoy.h"
#include <chrono>

//-----------------------------------------------------------------------------

void GateBoyTests::test_rom_read() {
  GateBoy gateboy;
  gateboy.run_reset_sequence(false, false);

  bool use_fast_impl = false;

  for (int i = 0; i < 256; i++) {
    uint8_t rom = gateboy.dbg_read((uint16_t)i, use_fast_impl);
    printf("addr 0x%04x data 0x%02x\n", i, rom);
  }
}

//-----------------------------------------------------------------------------

#if 0
void test_reset_timing(int phase_a, int phase_b, int phase_c, int phase_d) {
  SchematicTop* top = new SchematicTop();

  top.clk_reg.set_cpu_ready(0);
  top.ext_bus.set_ext_rdwr(0, 0);

  top.clk_reg.set_t1t2(0,0);

  uint64_t phase_hash;

  // Just read DIV forever.
  Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };

  // 8 phases w/ reset high, clock not running.
  top.clk_reg.set_rst(1);
  top.clk_reg.set_clk_a(0);
  run(top, phase_a, req, false);

  // 8 phases w/ reset high, clock running.
  top.clk_reg.set_rst(1);
  top.clk_reg.set_clk_a(1);
  run(top, phase_b, req, false);

  // 8 phases w/ reset low, clock running.
  top.clk_reg.set_rst(0);
  top.clk_reg.set_clk_a(1);
  run(top, phase_c, req, false);

  // Force LCDC_EN on and run until we get the CPU start request (~32k mcycles)

  while(!top.clk_reg._CPU_PIN_STARTp()) {
    run(top, 1, req, false);
  }

  // Ack the start request and run another 24 phases.
  // We should see AFER (global reset) clear and the video clocks start up.
  // FIXME why are the video clocks not running...

  top.clk_reg.set_cpu_ready(1);
  run(top, phase_d, req, false);

  top.clk_reg.set_cpu_ready(0);
  if (top.clk_reg.AFER_SYS_RSTp() || top.clk_reg.ASOL_POR_DONEn()) {
    printf("\nX %d %d %d %d\n", phase_a, phase_b, phase_c, phase_d);
  }
  else {
    printf(".");
  }
}

/*
for (int phase_a = 0; phase_a <= 8; phase_a++) 
for (int phase_b = 1; phase_b <= 8; phase_b++)
for (int phase_c = 0; phase_c <= 8; phase_c++)
for (int phase_d = 8; phase_d <= 16; phase_d++)
  test_reset_timing(phase_a, phase_b, phase_c, phase_d);
return 0;
*/

#endif

//-----------------------------------------------------------------------------

void GateBoyTests::fuzz_reset_sequence(GateBoy& gateboy, bool use_fast_impl) {
  printf("GateBoy::fuzz_reset_sequence %s\n", use_fast_impl ? "fast" : "slow");

  uint64_t rng = 1;

  Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };

#ifdef _DEBUG
  const int fuzz_count = 128;
#else
  const int fuzz_count = 65536;
#endif

  for (int i = 0; i < fuzz_count; i++) {
    mix(rng);

    gateboy.top.clk_reg.set_rst(wire(rng & 0x01));
    gateboy.top.clk_reg.set_clk_a(wire(rng & 0x02));
    gateboy.top.clk_reg.set_cpu_ready(wire(rng & 0x04));
    gateboy.top.clk_reg.set_t1t2(wire(rng & 0x08), wire(rng & 0x10));

    int phase_count = (rng >> 8) & 0x0F;
    gateboy.run(phase_count, req, false, use_fast_impl);

    if ((i & 0xFF) == 0xFF) printf(".");
  }
  printf("\n");
}

//-----------------------------------------------------------------------------

void GateBoyTests::test_reset_sequence() {
  printf("Running reset fuzz test in slow mode\n");
  GateBoy gateboy1;
  fuzz_reset_sequence(gateboy1, false);
  printf("\n");

  printf("Running reset fuzz test in fast mode\n");
  GateBoy gateboy2;
  fuzz_reset_sequence(gateboy2, true);
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

void GateBoyTests::run_benchmark(GateBoy& gateboy, bool use_fast_impl) {
  gateboy.run_reset_sequence(false, use_fast_impl);

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

  Req req = {.addr = 0x0150, .data = 0, .read = 1, .write = 0 };

  bool verbose = false;

  printf("Running perf test");
  for (int iter = 0; iter < iter_count; iter++) {
    gateboy.phase_total = 0;
    gateboy.pass_total = 0;

    //dump_blob(top);

    auto start = std::chrono::high_resolution_clock::now();
    gateboy.run(phase_per_iter, req, verbose, use_fast_impl);
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
  run_benchmark(gateboy1, false);
  printf("\n");

  printf("Running benchmark in fast mode\n");
  GateBoy gateboy2;
  run_benchmark(gateboy2, true);
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
