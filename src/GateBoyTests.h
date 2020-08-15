#pragma once
#include <stdint.h>

struct GateBoy;

struct GateBoyTests {
  static void test_rom_read();
  static void test_reset_sequence();
  static void run_benchmark();

  static void fuzz_reset_sequence(GateBoy& gateboy);
  static void run_benchmark(GateBoy& gateboy);

  static void test_all_mem();
  static void test_bootrom();
  static void test_timer();
  static void test_joypad();
  static void test_dma();
  static void test_serial();
  static void test_ppu();
  static void test_interrupts();

  static void test_reg(GateBoy& gb, const char* regname, uint16_t addr, uint8_t data_in);
  static void test_mem(GateBoy& gb, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write);

};