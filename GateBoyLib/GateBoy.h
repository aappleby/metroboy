#pragma once
#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"
#include "CoreLib/Z80.h"
#include "CoreLib/File.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoy {

  void dump(Dumper& d) const;

  void     set_rom(uint8_t* rom_buf, size_t rom_size);
  void     load_post_bootrom_state();
  void     reset_cart();

  void     reset_boot();
  void     run_reset_sequence();

  uint8_t  dbg_read (int addr);
  void     dbg_write(int addr, uint8_t data);

  void next_pass();

  void next_phase() {
    do {
      next_pass();
    } while(!sim_stable);
  }

  void run(int phase_count) {
    for (int i = 0; i < phase_count; i++) {
      next_phase();
    }
  }

  void set_boot_bit() {
    dbg_write(0xFF50, 0xFF);
  }

  //----------------------------------------

  void load_dump(const char* filename) {
    load_obj(filename, *this);
    check_sentinel();
  }

  void save_dump(const char* filename) {
    save_obj(filename, *this);
  }

  //----------------------------------------

  void check_div() const {
    int div = top.tim_reg.get_div();
    if (div != 0xEAF3) {
      printf("div fail!\n");
      *reinterpret_cast<int*>(0xDEADC0DEDEADC0DE) = 1;
    }
  }

  void check_sentinel() const {
    if (sentinel1 != 0xDEADBEEFBAADF00D) {
      printf("sentinel1 fail!\n");
      *reinterpret_cast<int*>(0xDEADC0DEDEADC0DE) = 1;
    }
    if (sentinel2 != 0xF00DCAFEBAADC0DE) {
      printf("sentinel2 fail!\n");
      *reinterpret_cast<int*>(0xDEADC0DEDEADC0DE) = 1;
    }
  }

  //----------------------------------------

  uint64_t sentinel1 = 0xDEADBEEFBAADF00D;

  // CPU

  Z80      cpu;
  Req      cpu_req = {0};
  Req      dbg_req = {0};
  Req      bus_req = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  imask_latch = 0;

  bool int_vblank = 0;
  bool int_vblank_halt = 0;

  bool int_stat = 0;
  bool int_stat_halt = 0;

  bool int_timer = 0;
  bool int_timer_halt = 0;

  bool int_serial = 0;
  bool int_serial_halt = 0;

  bool int_joypad = 0;
  bool int_joypad_halt = 0;

  // SOC

  Schematics::SchematicTop top;
  uint8_t  sim_stable = 0;

  double   sim_time = 0;
  int32_t  phase_total = 0;
  int32_t  pass_count = 0;
  int32_t  pass_total = 0;
  uint64_t pass_hash = HASH_INIT;
  uint64_t total_hash = HASH_INIT;

  // Sys flags

  int32_t sys_rst = 1;
  int32_t sys_t1 = 0;
  int32_t sys_t2 = 0;
  int32_t sys_clken = 0;
  int32_t sys_clkgood = 0;
  int32_t sys_cpuready = 0;
  int32_t sys_cpu_en = 0;
  uint8_t sys_buttons = 0;

  // Memory

  uint8_t vid_ram [8192];
  uint8_t cart_ram[8192];
  uint8_t ext_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];

  bool old_lcd_clock = 0;
  bool old_lcd_latch = 0;

  uint8_t framebuffer[160*144];
  uint8_t screen_x = 0;
  uint8_t screen_y = 0;
  uint8_t lcd_data_latch = 0;

  uint8_t* rom_buf = nullptr;
  size_t   rom_size = 0;

  Probes probes;

  uint64_t sentinel2 = 0xF00DCAFEBAADC0DE;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
