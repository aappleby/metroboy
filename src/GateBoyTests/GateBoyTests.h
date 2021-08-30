#pragma once
#include <stdint.h>
#include "CoreLib/File.h"

struct GateBoy;
struct GateBoyCart;

struct GateBoyTests {

  int test_regs();

  int test_fastboot_vs_slowboot();
  int test_reset_cart_vs_dump();

  int test_init();
  int test_bootrom();
  int test_clk();
  int test_ext_bus();
  int test_mem();
  int test_dma();
  int test_ppu();
  int test_timer();

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
  int test_micro_mbc1();

  int run_microtest(const char* filename);

  int test_dma(uint16_t src);
  int test_reg(const char* tag, uint16_t addr, uint8_t data_in);
  int test_mem(const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write);
  int test_spu_reg(const char* tag, uint16_t addr, uint8_t data_in);

  void run_benchmark();

  int test_mooneye_generic();
  int test_mooneye_mbc1();
  int test_mooneye_timer();
  int test_mooneye_ppu();
  int run_mooneye_test(const char* path, const char* filename);

  GateBoy create_gb_poweron(const blob& cart_blob);

  bool verbose = false;
};