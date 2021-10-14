#pragma once
#include <stdint.h>
#include "CoreLib/File.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/GateBoyPair.h"
#include <memory>

//-----------------------------------------------------------------------------

struct GateBoyTests {
  GateBoyTests();

  TestResults test_generic(const IGateBoy* proto);
  TestResults test_regs(const IGateBoy* proto);

  TestResults test_regression_cart     (const char* filename, int cycles, bool from_bootrom);
  TestResults test_regression_dump     (const char* filename, int cycles);

  TestResults test_fastboot            (const GateBoy* proto, uint8_t mask);
  TestResults test_reset_to_bootrom    (const IGateBoy* proto, uint8_t mask);
  TestResults test_reset_to_cart       (const IGateBoy* proto, uint8_t mask);

  TestResults test_init(const IGateBoy* proto);
  TestResults test_first_op(const IGateBoy* proto);
  TestResults test_bootrom(const IGateBoy* proto);
  TestResults test_clk(const IGateBoy* proto);
  TestResults test_ext_bus(const IGateBoy* proto);
  TestResults test_mem(const IGateBoy* proto);
  TestResults test_dma(const IGateBoy* proto);
  TestResults test_ppu(const IGateBoy* proto);
  TestResults test_timer(const IGateBoy* proto);

  TestResults test_micro_poweron(const IGateBoy* proto);
  TestResults test_micro_lcden(const IGateBoy* proto);
  TestResults test_micro_timer(const IGateBoy* proto);

  TestResults test_micro_int_vblank(const IGateBoy* proto);
  TestResults test_micro_int_stat(const IGateBoy* proto);
  TestResults test_micro_int_timer(const IGateBoy* proto);
  TestResults test_micro_int_serial(const IGateBoy* proto);
  TestResults test_micro_int_joypad(const IGateBoy* proto);

  TestResults test_micro_lock_oam(const IGateBoy* proto);
  TestResults test_micro_lock_vram(const IGateBoy* proto);
  TestResults test_micro_window(const IGateBoy* proto);
  TestResults test_micro_dma(const IGateBoy* proto);
  TestResults test_micro_ppu(const IGateBoy* proto);
  TestResults test_micro_mbc1(const IGateBoy* proto);

  TestResults run_microtest(const IGateBoy* proto, const char* filename, bool verbose = false);

  TestResults test_dma(const IGateBoy* proto, uint16_t src);

  TestResults test_fuzz_reg(const IGateBoy* proto, uint16_t addr, int reps);

  TestResults test_reg(const IGateBoy* proto, const char* tag, uint16_t addr, uint8_t data_in);
  TestResults test_mem(const IGateBoy* proto, const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write);
  TestResults test_spu(const IGateBoy* proto, const char* tag, uint16_t addr, uint8_t data_in);

  void run_benchmark(const IGateBoy* proto);

  TestResults test_mooneye_generic(const IGateBoy* proto);
  TestResults test_mooneye_mbc1(const IGateBoy* proto);
  TestResults test_mooneye_timer(const IGateBoy* proto);
  TestResults test_mooneye_ppu(const IGateBoy* proto);
  TestResults run_mooneye_test(const IGateBoy* proto, const char* path, const char* filename);

  //std::unique_ptr<IGateBoy> create_debug_gb(const blob& cart_blob, bool cpu_en);

  //bool verbose = false;
  const blob dummy_cart;
};

//-----------------------------------------------------------------------------
