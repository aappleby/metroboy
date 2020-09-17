#pragma once
#include "GateBoyLib/Sch_Top.h"
#include "CoreLib/Z80.h"
#include "CoreLib/File.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoy {
  GateBoy();

  void     reset_to_bootrom();
  void     reset_post_bootrom();
  uint8_t  dbg_read (int addr);
  void     dbg_write(int addr, uint8_t data);

  void     next_phase();
  void     next_pass();
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

  void check_sentinel() {
    if (sentinel1 != 0xDEADBEEFBAADF00D) *reinterpret_cast<int*>(0xDEADC0DEDEADC0DE) = 1;
    if (sentinel2 != 0xF00DCAFEBAADC0DE) *reinterpret_cast<int*>(0xDEADC0DEDEADC0DE) = 1;
  }

  uint64_t sentinel1 = 0xDEADBEEFBAADF00D;

  Z80 cpu;

  Schematics::SchematicTop top;

  uint8_t  cart_rom[32768];
  uint8_t  vid_ram [8192];
  uint8_t  cart_ram[8192];
  uint8_t  ext_ram [8192];
  uint8_t  oam_ram [256];
  uint8_t  zero_ram[128];
  uint8_t  framebuffer[160*144];

  Req      bus_req = {0};
  Req      dbg_req = {0};

  int32_t  phase_total = 0;
  int32_t  pass_count = 0;
  int32_t  pass_total = 0;
  uint64_t pass_hash = HASH_INIT;
  uint64_t total_hash = HASH_INIT;

  int32_t  sys_rst = 1;
  int32_t  sys_t1 = 0;
  int32_t  sys_t2 = 0;
  int32_t  sys_clken = 0;
  int32_t  sys_clkgood = 0;
  int32_t  sys_cpuready = 0;
  int32_t  sys_cpu_en = 0;
  uint8_t  sys_buttons = 0;
  uint8_t  sys_cart_loaded = 0;

  // FIXME delete these
  int32_t  ack_vblank = 0;
  int32_t  ack_stat = 0;
  int32_t  ack_timer = 0;
  int32_t  ack_serial = 0;
  int32_t  ack_joypad = 0;

  uint64_t sentinel2 = 0xF00DCAFEBAADC0DE;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
