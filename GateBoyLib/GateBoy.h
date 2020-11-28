#pragma once

#include "CoreLib/File.h"
#include "CoreLib/CPU.h"
#include "CoreLib/Constants.h"

#include "GateBoyLib/Probe.h"
#include "GateBoyLib/Sch_Timer.h"
#include "GateBoyLib/Sch_LCD.h"
#include "GateBoyLib/Sch_DMA.h"
#include "GateBoyLib/Sch_SpriteStore.h"
#include "GateBoyLib/Sch_Clocks.h"
#include "GateBoyLib/Sch_PixPipe.h"
#include "GateBoyLib/Sch_Joypad.h"
#include "GateBoyLib/Sch_Serial.h"
#include "GateBoyLib/Sch_Interrupts.h"
#include "GateBoyLib/Sch_SpriteFetcher.h"
#include "GateBoyLib/Sch_TileFetcher.h"
#include "GateBoyLib/Sch_SpriteScanner.h"
#include "GateBoyLib/Sch_ExtBus.h"
#include "GateBoyLib/Sch_OamBus.h"
#include "GateBoyLib/Sch_VramBus.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct GateBoy {

  void dump(Dumper& d) const;

  void reset_boot(uint8_t* _boot_buf, size_t _boot_size,
                  uint8_t* _cart_buf, size_t _cart_size,
                  bool fastboot = true);
  void reset_cart(uint8_t* _boot_buf, size_t _boot_size,
                  uint8_t* _cart_buf, size_t _cart_size);



  void load_post_bootrom_state();
  void run_reset_sequence(bool fastboot = true);

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
    int div = tim_reg.get_div();
    if (div != BOOT_DIV) {
      printf("div fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  void check_sentinel() const {
    if (sentinel1 != SENTINEL1) {
      printf("sentinel1 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
    if (sentinel2 != SENTINEL2) {
      printf("sentinel2 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
    if (sentinel3 != SENTINEL3) {
      printf("sentinel2 fail!\n");
      *reinterpret_cast<int*>(SENTINEL4) = 1;
    }
  }

  uint8_t dbg_read (int addr);
  void dbg_write(int addr, uint8_t data);

  void set_boot_bit() {
    dbg_write(0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

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

  void next_pass();

  void tock_slow();
  void commit_and_hash();

  //-----------------------------------------------------------------------------

  uint8_t* reg_begin() { return (uint8_t*)(&sentinel1) + sizeof(sentinel1); }
  uint8_t* reg_end()   { return (uint8_t*)(&sentinel2); }


  //-----------------------------------------------------------------------------
  // All the SOC registers, pins, buses. Everything in this section should derive
  // from BitBase.

  uint64_t sentinel1 = SENTINEL1;

  OamBus oam_bus;
  ExtBus ext_bus;
  VramBus vram_bus;


  ClockRegisters     clk_reg;
  DmaRegisters       dma_reg;
  InterruptRegisters int_reg;
  JoypadRegisters    joypad;
  LcdRegisters       lcd_reg;
  PixelPipe          pix_pipe;
  SerialRegisters    ser_reg;
  SpriteStore        sprite_store;
  TimerRegisters     tim_reg;
  TileFetcher        tile_fetcher;
  SpriteFetcher      sprite_fetcher;
  SpriteScanner      sprite_scanner;

  // Starts 0, set to 1 by bootrom which blocks reading 0x0000-0x00FF.
  // In run mode, BOOT_BITn must _not_ be reset.
  /*p07.TEPU*/ DFF17 BOOT_BITn_s;

  /*p25.SOTO*/ DFF17 SOTO_DBG_VRAMp;

  //DelayGlitch lcd_data1_delay;
  //DelayGlitch lcd_data0_delay;

  DFF IE_D0;
  DFF IE_D1;
  DFF IE_D2;
  DFF IE_D3;
  DFF IE_D4;

  // ok this is technically part of the lcd, but registers need to go in this section...
  NorLatch lcd_pix_lo;
  NorLatch lcd_pix_hi;

  DFF lcd_pipe_lo[160];
  DFF lcd_pipe_hi[160];

  uint64_t sentinel2 = SENTINEL2;

  //-----------------------------------------------------------------------------
  // Control stuff

  bool sys_rst = 1;
  bool sys_t1 = 0;
  bool sys_t2 = 0;
  bool sys_clken = 0;
  bool sys_clkgood = 0;
  bool sys_cpuready = 0;
  bool sys_cpu_en = 0;
  bool sys_fastboot = 0;
  bool sys_statediff = 1;
  bool sys_cpu_start = 0;

  uint8_t sys_buttons = 0;

  bool     sim_stable = 0;
  double   sim_time = 0;
  int32_t  phase_total = 0;
  int32_t  pass_count = 0;
  int32_t  pass_total = 0;
  uint64_t pass_hash = HASH_INIT;
  uint64_t total_hash = HASH_INIT;
  Probes   probes;

  //-----------------------------------------------------------------------------
  // CPU

  CPU      cpu;
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

  //-----------------------------------------------------------------------------
  // Memory

  uint8_t* boot_buf = nullptr;
  size_t   boot_size = 0;

  uint8_t* cart_buf = nullptr;
  size_t   cart_size = 0;

  uint8_t vid_ram [8192];
  uint8_t cart_ram[8192];
  uint8_t ext_ram [8192];
  uint8_t oam_ram [256];
  uint8_t zero_ram[128];

  //-----------------------------------------------------------------------------
  // LCD and framebuffer

  uint8_t framebuffer[160*144];
  uint8_t gb_screen_x = 0;
  uint8_t gb_screen_y = 0;
  bool old_lcd_clock = 0;
  bool old_lcd_latch = 0;
  uint8_t lcd_data_latch = 0;

  //-----------------------------------------------------------------------------

  uint64_t sentinel3 = SENTINEL3;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------
