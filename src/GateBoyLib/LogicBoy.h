#pragma once

#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"
#include "GateBoyLib/LogicBoyState.h"

//-----------------------------------------------------------------------------

struct LogicBoyCpu {
  MetroBoyCPU core;
  Req      bus_req_new = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  imask_latch = 0;
  uint8_t  intf_latch = 0;
  uint8_t  intf_latch_delay = 0;
  uint8_t  intf_halt_latch = 0;
};

//-----------------------------------------------------------------------------

struct LogicBoyMem {
  uint8_t vid_ram [8192];
  uint8_t cart_ram[32768];
  uint8_t int_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];
  uint8_t framebuffer[160*144];
};

//-----------------------------------------------------------------------------

struct LogicBoySys {
  // External signals

  uint8_t rst = 0;
  uint8_t t1 = 0;
  uint8_t t2 = 0;
  uint8_t clk_en = 0;
  uint8_t clk_good = 0;
  uint8_t clk_req = 0;
  uint8_t cpu_en = 0;
  uint8_t fastboot = 0;
  uint8_t buttons = 0;

  // Debug stuff
  uint64_t phase_total = 0;
  double   sim_time = 0;
};

//-----------------------------------------------------------------------------

struct LogicBoy {
  void reset_to_bootrom(const blob& cart_blob, bool fastboot);
  void reset_to_cart(const blob& cart_blob);

  //----------------------------------------

  void from_blob(const blob& b) {
    CHECK_P(b.size() >= sizeof(LogicBoy));
    memcpy(this, b.data(), sizeof(LogicBoy));
  }

  void to_blob(blob& b) {
    uint8_t* bytes = (uint8_t*)this;
    b.insert(b.end(), bytes, bytes + sizeof(*this));
  }

  //----------------------------------------

  bool dbg_read (const blob& cart_blob, int addr, uint8_t& out);
  bool dbg_write(const blob& cart_blob, int addr, uint8_t data);

  void set_boot_bit(const blob& cart_blob) {
    dbg_write(cart_blob, 0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

  void run_phases(const blob& cart_blob, int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      next_phase(cart_blob);
    }
  }

  bool next_phase(const blob& cart_blob);
  void tock_cpu();
  void tock_logic(const blob& cart_blob, int64_t phase_total);
  void update_framebuffer();

  void wipe() {
    lb_state.wipe();
  }

  //-----------------------------------------------------------------------------

  void tock_spu_logic();
  void tock_serial_logic(bool cpu_wr_old, bool cpu_wr_new, uint16_t cpu_addr_old, uint16_t cpu_addr_new, uint16_t div_old, uint16_t div_new);

  //-----------------------------------------------------------------------------

  LogicBoyState lb_state;
  LogicBoyCpu cpu;
  LogicBoyMem mem;
  LogicBoySys sys;
};

//-----------------------------------------------------------------------------
