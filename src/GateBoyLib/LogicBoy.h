#pragma once

#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"
#include "GateBoyLib/LogicBoyState.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct LogicBoyCpu {
  MetroBoyCPU core;
  Req      bus_req_new = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  imask_latch = 0;
  uint8_t  intf_latch = 0;
  uint8_t  intf_latch_delay = 0;
  uint8_t  intf_halt_latch = 0;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct LogicBoyMem {
  uint8_t vid_ram [8192];
  uint8_t cart_ram[32768];
  uint8_t int_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];
  uint8_t framebuffer[160*144];
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
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
  uint64_t phase_origin = 0;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct LogicBoy {
  void reset_to_bootrom(const blob& cart_blob, bool fastboot);
  void reset_to_cart(const blob& cart_blob);

  //----------------------------------------

  static bool check_sentinel(const blob& b) {
    if (b.size() < sizeof(LogicBoy)) return false;

    LogicBoy* gb = (LogicBoy*)b.data();
    if (gb->sentinel1 != SENTINEL1) return false;
    if (gb->sentinel2 != SENTINEL2) return false;
    return true;
  }

  void from_blob(const blob& b) {
    CHECK_P(b.size() >= sizeof(LogicBoy));
    memcpy(this, b.data(), sizeof(LogicBoy));
    CHECK_P(sentinel1 == SENTINEL1);
    CHECK_P(sentinel2 == SENTINEL2);
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
  void tock_logic(const blob& cart_blob);
  void update_framebuffer(int lcd_x, int lcd_y, wire DATA0, wire DATA1);

  void wipe() {
    // FIXME probably don't need this memset
    memset(this, 0, sizeof(*this));

    //lb_state.wipe();

    sentinel1 = SENTINEL1;
    sentinel2 = SENTINEL2;
  }

  //-----------------------------------------------------------------------------

  void tock_spu_logic();
  void tock_serial_logic(bool cpu_wr_old, bool cpu_wr_new, uint16_t cpu_addr_old, uint16_t cpu_addr_new, uint16_t div_old, uint16_t div_new);

  //----------------------------------------

  wire gen_clk_old(uint8_t mask) {
    uint8_t phase_mask_old = 1 << (7 - ((sys.phase_total + 0) & 7));
    return !!(phase_mask_old & mask);
  }

  wire gen_clk_new(uint8_t mask) {
    uint8_t phase_mask_new = 1 << (7 - ((sys.phase_total + 1) & 7));
    return !!(phase_mask_new & mask);
  }

  //-----------------------------------------------------------------------------

  uint64_t sentinel1 = SENTINEL1;

  LogicBoyState lb_state;
  LogicBoyCpu cpu;
  LogicBoyMem mem;
  LogicBoySys sys;

  uint64_t sentinel2 = SENTINEL2;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
