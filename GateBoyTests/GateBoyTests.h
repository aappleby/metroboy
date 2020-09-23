#pragma once
#include <stdint.h>

struct GateBoy;

struct GateBoyTests {

  int test_init();
  int test_clk();
  int test_ext_bus();
  int test_mem();
  int test_bootrom();
  int test_dma();
  int test_interrupts();
  int test_joypad();
  int test_ppu();
  int test_serial();
  int test_timer();
  int test_micro();
  int test_micro_ints();
  
  int run_microtest(const char* filename);

  int test_dma(uint16_t src);
  int test_reg(const char* tag, uint16_t addr, uint8_t data_in);
  int test_mem(const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write);

  void run_benchmark();
};