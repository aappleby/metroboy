#include "GateBoy.h"

#include <chrono>

#define FAST_HASH

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

void GateBoy::run_reset_sequence(bool verbose, bool use_fast_impl) {
  top.clk_reg.set_cpu_ready(0);
  top.ext_bus.set_ext_rdwr(0, 0);
  top.clk_reg.set_t1t2(0,0);

  // Just read DIV forever.
  Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };

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

void GateBoy::run_benchmark(bool use_fast_impl) {
  run_reset_sequence(false, use_fast_impl);
  printf("Hash 1 after reset: 0x%016llx : 0x%016llx : 0x%016llx\n", phase_hash_bytes, phase_hash_regs, phase_hash_bits);

  /*
  uint64_t hash_bytes2 = HASH_INIT;
  uint64_t hash_regs2  = HASH_INIT;
  uint64_t hash_bits2  = HASH_INIT;

  hash_blob(&top, sizeof(top), hash_bytes2, hash_regs2, hash_bits2);
  printf("Hash 2 after reset: 0x%016llx : 0x%016llx : 0x%016llx\n", hash_bytes2, hash_regs2, hash_bits2);
  */

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

  Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };

  bool verbose = false;

  printf("Running perf test");
  for (int iter = 0; iter < iter_count; iter++) {
    phase_total = 0;
    pass_total = 0;

    //dump_blob(top);

    auto start = std::chrono::high_resolution_clock::now();
    run(phase_per_iter, req, verbose, use_fast_impl);
    auto finish = std::chrono::high_resolution_clock::now();

    //dump_blob(top);

    if (iter >= warmup) {
      std::chrono::duration<double> elapsed = finish - start;
      double time = elapsed.count();

      double phase_rate = double(phase_total) / time;
      phase_rate_sum1 += phase_rate;
      phase_rate_sum2 += phase_rate * phase_rate;
      phase_rate_n++;

      double pass_rate = double(pass_total) / time;
      pass_rate_sum1 += pass_rate;
      pass_rate_sum2 += pass_rate * pass_rate;
      pass_rate_n++;
    }
    printf(".");
  }
  printf("Done\n");

  printf("Phase total %d\n", phase_total);
  printf("Pass total %d\n", pass_total);

  double phase_rate_mean     = phase_rate_sum1 / phase_rate_n;
  double phase_rate_variance = (phase_rate_sum2 / phase_rate_n) - (phase_rate_mean * phase_rate_mean);
  double phase_rate_sigma    = sqrt(phase_rate_variance);
  printf("Mean phase/sec %f sigma %f\n", phase_rate_mean, phase_rate_sigma);

  double pass_rate_mean     = pass_rate_sum1 / pass_rate_n;
  double pass_rate_variance = (pass_rate_sum2 / pass_rate_n) - (pass_rate_mean * pass_rate_mean);
  double pass_rate_sigma    = sqrt(pass_rate_variance);
  printf("Mean pass/sec %f sigma %f\n", pass_rate_mean, pass_rate_sigma);

  printf("Commit phase_hash   0x%016llx : 0x%016llx : 0x%016llx\n", phase_hash_bytes, phase_hash_regs, phase_hash_bits);
  printf("Combined phase_hash 0x%016llx : 0x%016llx : 0x%016llx\n", combined_hash_bytes, combined_hash_regs, combined_hash_bits);
}

//------------------------------------------------------------------------------

void GateBoy::run(int _phase_count, Req req, bool verbose, bool use_fast_impl) {
  for (int i = 0; i < _phase_count; i++) {
    phase(req, verbose, use_fast_impl);
  }
}

//------------------------------------------------------------------------------

void GateBoy::phase(Req req, bool verbose, bool use_fast_impl) {
  //printf("phase\n");

  phase_total++;
  wire CLK = (phase_total & 1) & (top.clk_reg.get_clk_a());
  top.clk_reg.set_clk_b(CLK);

  pass_count = 0;

  uint64_t hash_bytes_old = HASH_INIT;
  uint64_t hash_regs_old  = HASH_INIT;
  uint64_t hash_bits_old  = HASH_INIT;

  for (int i = 0; i < 256; i++) {
    pass_count++;
    pass_total++;

    top.cpu_bus.set_cpu_req(req);
    top.vram_bus.set_vram_data(0);
    top.oam_bus.set_oam_data(0, 0);
    top.ext_bus.set_ext_data(0);
    top.joypad.set_buttons(0);
    
    const int phase = phase_total & 7;

    if (use_fast_impl) {
      top.tick_fast(phase);
    }
    else {
      top.tick_slow(phase);
    }

    uint64_t hash_bytes_new = HASH_INIT;
    uint64_t hash_regs_new  = HASH_INIT;
    uint64_t hash_bits_new  = HASH_INIT;

#ifdef FAST_HASH
    //if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(top, hash_bytes_new, hash_regs_new, hash_bits_new);
    //if (verbose) printf("0x%016llx\n", _pass_hash);
#else
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.oam_bus, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.ext_bus, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.cpu_bus, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.vram_bus, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.clk_reg, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.dma_reg, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.int_reg, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.joypad, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.lcd_reg, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.pix_pipe, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.ser_reg, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.sprite_store, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.tim_reg, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.tile_fetcher, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.sprite_fetcher, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.sprite_scanner, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
    commit_and_hash(_pass_hash, top.bootrom, byte_mask);
    if (verbose) printf("0x%016llx\n", _pass_hash);
#endif
    
    //printf("hash 0x%016llx\n", new_hash.h);
    
    if (hash_bytes_old == hash_bytes_new) break;
    hash_bytes_old = hash_bytes_new;
    hash_regs_old = hash_regs_new;
    hash_bits_old = hash_bits_new;
    if (i == 199) printf("stuck!\n");
    CHECK_N(i == 200);
  }

  phase_hash_bytes = hash_bytes_old;
  phase_hash_regs  = hash_regs_old;
  phase_hash_bits  = hash_bits_old;

  combine_hash(combined_hash_bytes, phase_hash_bytes);
  combine_hash(combined_hash_regs,  phase_hash_regs);
  combine_hash(combined_hash_bits,  phase_hash_bits);

#if 1
  if (verbose) {
    printf("Phase %c @ %08d:%02d phase_hash %016llx : %016llx : %016llx\n",
      'A' + (phase_total & 7),
      phase_total,
      pass_count,
      phase_hash_bytes,
      phase_hash_regs,
      phase_hash_bits);
  }
#else
  if (verbose) {
    printf("Phase %c @ %08d:%02d phase_hash %016llx CLK_GOOD %d CLK %d RST %d phz %d%d%d%d vid %d%d%d BOMA %d CPU_START %d CPU_RDY %d DIV %05d TUBO %d AFER %d ASOL %d\n",
      'A' + (phase_total & 7),
      phase_total,
      pass_count,
      phase_hash_bytes,
      top.clk_reg.get_clk_a(),
      top.clk_reg.get_clk_b(),
      top.clk_reg.SYS_PIN_RSTp(),
      top.clk_reg.AFUR_ABCDxxxx(),
      top.clk_reg.ALEF_xBCDExxx(),
      top.clk_reg.APUK_xxCDEFxx(),
      top.clk_reg.ADYK_xxxDEFGx(),
      top.clk_reg.WUVU_xxCDxxGH(),
      top.clk_reg.VENA_xxxxEFGH(),
      top.clk_reg.WOSU_xBCxxFGx(),
      top.clk_reg.BOMA_Axxxxxxx(),
      top.clk_reg.CPU_PIN_STARTp(),
      top.clk_reg.CPU_PIN_READYp(),
      top.tim_reg.get_div(),
      top.clk_reg.TUBO_CPU_READYn(),
      top.clk_reg.AFER_SYS_RSTp(),
      top.clk_reg.ASOL_POR_DONEn()
      //hash.h);
      );
  }
#endif
}

//-----------------------------------------------------------------------------
