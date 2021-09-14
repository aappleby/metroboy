#pragma once
#include "GateBoyLib/GateBoy.h"

void print_field_at(int offset);

//---------------------------------------------------------------------------------------------------------------------

struct GateBoyPair {
  GateBoyPair() {
    gba.sys.logic_mode = false;
    gbb.sys.logic_mode = true;
  }

  GateBoyPair(bool logic_a, bool logic_b) {
    gba.sys.logic_mode = logic_a;
    gbb.sys.logic_mode = logic_b;
  }

  GateBoy gba;
  GateBoy gbb;

  //void wipe_flags() {
  //  if (gba.sys.logic_mode) gba.wipe_flags();
  //  if (gbb.sys.logic_mode) gbb.wipe_flags();
  //}

  bool reset_to_bootrom(const blob& cart_blob, bool fastboot) {
    gba.reset_to_bootrom(cart_blob, fastboot);
    gbb.reset_to_bootrom(cart_blob, fastboot);

    //wipe_flags();
    return check_sync();
  }

  bool reset_to_cart(const blob& cart_blob) {
    gba.reset_to_cart(cart_blob);
    gbb.reset_to_cart(cart_blob);

    //wipe_flags();
    return check_sync();
  }

  bool from_blob(const blob& b) {
    gba.from_blob(b);
    gbb.from_blob(b);

    //wipe_flags();
    return check_sync();
  }

  bool next_phase(const blob& cart_blob) {

    //if (gba.sys.logic_mode) gba.check_no_flags();
    //if (gbb.sys.logic_mode) gbb.check_no_flags();

    bool result = true;
    result &= gba.next_phase(cart_blob);
    if (config_regression) {
      result &= gbb.next_phase(cart_blob);
    }

    //if (gba.sys.logic_mode) gba.check_no_flags();
    //if (gbb.sys.logic_mode) gbb.check_no_flags();

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

    CHECK_P((gba.sys.phase_total & 7) == 0);

    Req old_req = gba.cpu.bus_req_new;
    bool old_cpu_en = gba.sys.cpu_en;

    Req req_new;
    req_new.addr = uint16_t(addr);
    req_new.data = data_in;
    req_new.read = 0;
    req_new.write = 1;

    gba.cpu.bus_req_new = req_new;
    gbb.cpu.bus_req_new = req_new;
    gba.sys.cpu_en = false;
    gbb.sys.cpu_en = false;

    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);

    gba.cpu.bus_req_new = old_req;
    gbb.cpu.bus_req_new = old_req;
    gba.sys.cpu_en = old_cpu_en;
    gbb.sys.cpu_en = old_cpu_en;

    return result;
  }

  bool dbg_read(const blob& cart_blob, uint16_t addr, uint8_t& out) {
    bool result = true;

    result &= check_sync();

    CHECK_P((gba.sys.phase_total & 7) == 0);

    Req old_req = gba.cpu.bus_req_new;
    bool old_cpu_en = gba.sys.cpu_en;

    Req req_new;
    req_new.addr = uint16_t(addr);
    req_new.data = 0;
    req_new.read = 1;
    req_new.write = 0;

    gba.cpu.bus_req_new = req_new;
    gbb.cpu.bus_req_new = req_new;
    gba.sys.cpu_en = false;
    gbb.sys.cpu_en = false;

    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);
    result &= next_phase(cart_blob);

    gba.cpu.bus_req_new = old_req;
    gbb.cpu.bus_req_new = old_req;
    gba.sys.cpu_en = old_cpu_en;
    gbb.sys.cpu_en = old_cpu_en;

    out = gba.cpu.cpu_data_latch;
    return result;
  }

  bool check_sync() {
    if (config_regression) {
      if (gba.hash_regression() != gbb.hash_regression()) {
        LOG_R("Regression test mismatch @ phase %lld!\n", gba.sys.phase_total);
        diff_gb(&gba, &gbb, 0x01);
        //__debugbreak();
        return false;
      }
    }

    return true;
  }
};

//---------------------------------------------------------------------------------------------------------------------
