#pragma once
#include <stdint.h>
#include "CoreLib/File.h"

struct GateBoy;

struct GateBoyTests {

  int test_init();
  int test_bootrom();
  int test_clk();
  int test_ext_bus();
  int test_mem();
  int test_dma();
  int test_interrupts();
  int test_joypad();
  int test_ppu();
  int test_serial();
  int test_timer();
  int test_post_bootrom_state();

  int test_micro_poweron();
  int test_micro_lcden();
  int test_micro_timer();

  int test_micro_int_vblank();
  int test_micro_int_stat();
  int test_micro_int_timer();
  int test_micro_int_serial();
  int test_micro_int_joypad();

  int test_micro_lock_oam();
  int test_micro_lock_vram();
  int test_micro_window();
  int test_micro_dma();
  int test_micro_ppu();

  int run_microtest(const char* filename);

  int test_dma(uint16_t src);
  int test_reg(const char* tag, uint16_t addr, uint8_t data_in);
  int test_mem(const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write);

  void run_benchmark();

  GateBoy create_test_gb_boot();

  bool verbose = false;
  blob cart_rom;
};