#pragma once
#include <stdint.h>
#include <functional>
#include <stdarg.h>

struct GateBoy;

struct GateBoyTests {

  int test_main(int argc, char** argv);

  void test_reset_sequence();
  void fuzz_reset_sequence(GateBoy& gateboy);

  void run_benchmark();
  void run_benchmark(GateBoy& gateboy);

  int test_mem();
  int test_bootrom();
  int test_dma();
  int test_interrupts();
  int test_joypad();
  int test_ppu();
  int test_serial();
  int test_timer();

  int test_dma(uint16_t src);

  int test_reg(GateBoy& gb, const char* tag, uint16_t addr, uint8_t data_in);
  int test_mem(GateBoy& gb, const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write);
};