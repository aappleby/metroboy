#pragma once
#include <stdint.h>
#include "CoreLib/File.h"
#include "CoreLib/Tests.h"
#include "GateBoyLib/GateBoy.h"

struct GateBoy;
struct GateBoyCart;

//-----------------------------------------------------------------------------

struct GateBoyPair {

  GateBoyPair() {
    gba.logic_mode = false;
    gbb.logic_mode = true;
  }

  GateBoy gba;
  GateBoy gbb;

  bool reset_to_bootrom(const blob& cart_blob, bool fastboot) {
    gba.reset_to_bootrom(cart_blob, fastboot);
    gbb.reset_to_bootrom(cart_blob, fastboot);
    return check_sync();
  }

  bool reset_to_cart(const blob& cart_blob) {
    gba.reset_to_cart(cart_blob);
    gbb.reset_to_cart(cart_blob);
    return check_sync();
  }

  bool from_blob(const blob& b) {
    gba.from_blob(b);
    gbb.from_blob(b);
    return check_sync();
  }

  bool next_phase(const blob& cart_blob) {
    bool result = true;
    result &= gba.next_phase(cart_blob);
    if (config_regression) {
      result &= gbb.next_phase(cart_blob);
    }
    result &= check_sync();
    return result;
  }

  bool run_phases(const blob& cart_blob, int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      if (!next_phase(cart_blob)) return false;
    }
    return true;
  }

  bool dbg_write(const blob& cart_blob, uint16_t addr, uint8_t data_in) {
    bool result = true;
    //result &= gba.dbg_write(cart_blob, addr, data_in);
    //result &= gbb.dbg_write(cart_blob, addr, data_in);
    //result &= check_sync();

    result &= check_sync();

    CHECK_P((gba.phase_total & 7) == 0);

    Req old_req = gba.bus_req_new;
    bool old_cpu_en = gba.sys_cpu_en;

    Req req_new;
    req_new.addr = uint16_t(addr);
    req_new.data = data_in;
    req_new.read = 0;
    req_new.write = 1;

    gba.bus_req_new = req_new;
    gbb.bus_req_new = req_new;
    gba.sys_cpu_en = false;
    gbb.sys_cpu_en = false;

    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);

    gba.bus_req_new = old_req;
    gbb.bus_req_new = old_req;
    gba.sys_cpu_en = old_cpu_en;
    gbb.sys_cpu_en = old_cpu_en;

    return result;
  }

  bool dbg_read(const blob& cart_blob, uint16_t addr, uint8_t& out) {
    bool result = true;

    result &= check_sync();

    CHECK_P((gba.phase_total & 7) == 0);

    Req old_req = gba.bus_req_new;
    bool old_cpu_en = gba.sys_cpu_en;

    Req req_new;
    req_new.addr = uint16_t(addr);
    req_new.data = 0;
    req_new.read = 1;
    req_new.write = 0;

    gba.bus_req_new = req_new;
    gbb.bus_req_new = req_new;
    gba.sys_cpu_en = false;
    gbb.sys_cpu_en = false;

    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);

    gba.bus_req_new = old_req;
    gbb.bus_req_new = old_req;
    gba.sys_cpu_en = old_cpu_en;
    gbb.sys_cpu_en = old_cpu_en;

    out = gba.cpu_data_latch;
    return result;
  }

  bool check_sync() {
    if (config_regression) {
      if (gba.hash_regression() != gbb.hash_regression()) {
        LOG_R("Regression test mismatch @ phase %lld!\n", gba.phase_total);
        diff_gb(&gba, &gbb, 0x01);
        __debugbreak();
        return false;
      }
    }
    return true;
  }
};

//-----------------------------------------------------------------------------

struct GateBoyTests {

  TestResults test_regs();

  TestResults test_fastboot_vs_slowboot();
  TestResults test_reset_cart_vs_dump();

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

  GateBoyPair create_debug_gb(const blob& cart_blob);

  bool verbose = false;
};

//-----------------------------------------------------------------------------
