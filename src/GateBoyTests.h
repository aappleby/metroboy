#pragma once
struct GateBoy;

struct GateBoyTests {
  static void test_rom_read();
  static void test_reset_sequence();
  static void run_benchmark();

  static void fuzz_reset_sequence(GateBoy& gateboy, bool use_fast_impl);
  static void run_benchmark(GateBoy& gateboy, bool use_fast_impl);
};