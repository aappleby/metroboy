#pragma once
#include <stdint.h>
#include "CoreLib/File.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/GateBoyPair.h"
#include <memory>

//-----------------------------------------------------------------------------

struct GateBoyTests {

  TestResults test_regs();

  TestResults test_fastboot_vs_slowboot(IGateBoy* gb1, IGateBoy* gb2);
  TestResults test_reset_to_bootrom(IGateBoy* gb1, IGateBoy* gb2);
  TestResults test_reset_to_cart(IGateBoy* gb1, IGateBoy* gb2);

  TestResults test_init();
  TestResults test_bootrom();
  TestResults test_clk();
  TestResults test_ext_bus();
  TestResults test_mem();
  TestResults test_dma();
  TestResults test_ppu();
  TestResults test_timer();

  TestResults test_micro_poweron();
  TestResults test_micro_lcden();
  TestResults test_micro_timer();

  TestResults test_micro_int_vblank();
  TestResults test_micro_int_stat();
  TestResults test_micro_int_timer();
  TestResults test_micro_int_serial();
  TestResults test_micro_int_joypad();

  TestResults test_micro_lock_oam();
  TestResults test_micro_lock_vram();
  TestResults test_micro_window();
  TestResults test_micro_dma();
  TestResults test_micro_ppu();
  TestResults test_micro_mbc1();

  TestResults run_microtest(const char* filename);

  TestResults test_dma(uint16_t src);
  TestResults test_reg(const char* tag, uint16_t addr, uint8_t data_in);
  TestResults test_mem(const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write);
  TestResults test_spu_reg(const char* tag, uint16_t addr, uint8_t data_in);

  void run_benchmark();

  TestResults test_mooneye_generic();
  TestResults test_mooneye_mbc1();
  TestResults test_mooneye_timer();
  TestResults test_mooneye_ppu();
  TestResults run_mooneye_test(const char* path, const char* filename);

  std::unique_ptr<IGateBoy> create_debug_gb(const blob& cart_blob, bool cpu_en);

  bool verbose = false;
};

//-----------------------------------------------------------------------------
