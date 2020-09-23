#pragma once

#include "CoreLib/Types.h"
#include "CoreLib/Z80.h"

#include "MetroBoyLib/Joypad.h"
#include "MetroBoyLib/Cart.h"
#include "MetroBoyLib/DMA.h"
#include "MetroBoyLib/LCD.h"
#include "MetroBoyLib/PPU.h"
#include "MetroBoyLib/Serial.h"
#include "MetroBoyLib/SPU.h"
#include "MetroBoyLib/Timer.h"
#include "MetroBoyLib/VRAM.h"
#include "MetroBoyLib/ZRAM.h"
#include "MetroBoyLib/Bootrom.h"

#include <assert.h>

//-----------------------------------------------------------------------------

struct MetroBoy {
  MetroBoy() = default;
  MetroBoy(uint16_t new_pc, uint8_t* new_rom, size_t new_rom_size) {
    reset(new_pc, new_rom, new_rom_size);
  }

  struct HostOut {
    int x = 0;
    int y = 0;
    int counter = 0;
    uint8_t pix = 0;
    bool pix_oe = 0;
    sample_t out_r = 0;
    sample_t out_l = 0;
    uint32_t trace = 0;
  };

  void reset(uint16_t new_pc, uint8_t* new_rom, size_t new_rom_size);

  HostOut get_host_data() const { return gb_to_host; }

  void sync_to_vblank() {
    sync_to_mcycle();
    while(get_host_data().y != 144) {
      mcycle();
    }
  }

  void step_phase(int count) {
    for (int i = 0; i < count; i++) {
      halfcycle();
    }
  }

  void step_mcycle(int count) {
    sync_to_mcycle();
    for (int i = 0; i < count; i++) {
      mcycle();
    }
  }

  void step_line(int count) {
    sync_to_mcycle();

    for (int i = 0; i < count; i++) {
      int old_line = get_host_data().y;

      while(1) {
        mcycle();
        int new_line = get_host_data().y;
        if (old_line != new_line) {
          break;
        }
      }
    }
  }

  void run_to_breakpoint(uint16_t breakpoint) {
    sync_to_mcycle();

    while (1) {
      uint16_t op_addr = get_cpu().get_op_addr();
      if (op_addr == breakpoint) break;
      mcycle();
    }
  }

  void step_frame(int count) {
    sync_to_mcycle();

    for (int i = 0; i < count; i++) {
      do {
        mcycle();
      } while (get_host_data().y == 144);

      do {
        mcycle();
      } while (get_host_data().y != 144);
    }
  }


  void sync_to_mcycle() {
    int old_phase = (phase_total + 0) & 7;
    switch(old_phase) {
    case 0: return;
    case 1: halfcycle();
    case 2: halfcycle();
    case 3: halfcycle();
    case 4: halfcycle();
    case 5: halfcycle();
    case 6: halfcycle();
    case 7: halfcycle();
    }
  }
  
  void mcycle() {
    //assert(((phase_total + 0) & 7) == 7);

    for (int i = 0; i < 8; i++) {
      halfcycle();
    }
  }

  void halfcycle() {
    int old_phase = (phase_total + 0) & 7;
    int new_phase = (phase_total + 1) & 7;

    tick_gb(); tock_gb(old_phase, new_phase);
  }

  void tick_gb();
  void tock_gb(int old_phase, int new_phase);

  void tock(int old_phase, int new_phase, const Req& req);
  void tick(const Req& req, Ack& ibus_ack) const;

  void dump_bus   (Dumper& d);
  void dump_disasm(Dumper& d);

  const Z80& get_cpu() const { return z80; }
  const SPU& get_spu() const { return spu; }
  const PPU& get_ppu() const { return ppu; }

  // 0x0000 - rom
  // 0x2000 - rom
  // 0x4000 - rom
  // 0x6000 - rom
  // 0x8000 - vram
  // 0xA000 - cram
  // 0xC000 - iram
  // 0xE000 - eram
  // 0xFF80 - zram

  uint8_t* get_cart_rom() { return cart.get_cart_rom(); }
  uint8_t* get_vram()     { return vram.ram; }
  uint8_t* get_cart_ram() { return cart.get_cart_ram(); }
  uint8_t* get_main_ram() { return cart.get_main_ram(); }
  uint8_t* get_oam_ram()  { return (uint8_t*)oam.get(); }
  uint8_t* get_zram()     { return zram.ram; }

  void set_joypad(uint8_t v) { joypad.set(v); }

  void check_sentinel() { assert(sentinel == 0xDEADBEEF); }

  //----------

  Z80     z80;
  NewTimer timer2;

  Cart    cart;
  VRAM    vram;
  OAM     oam;
  ZRAM    zram;
  Joypad  joypad;
  Serial  serial;
  PPU     ppu;
  SPU     spu;
  DMA2    dma2;
  Bootrom boot;
  LCD     lcd;

  //----------

  int64_t phase_total = -1;

  HostOut gb_to_host;
  uint32_t trace_val = 0;

  uint8_t intf = 0;
  uint8_t imask = 0;

  //----------

  Req cpu_req;
  Ack cpu_ack;

  Req ebus_req; // 0x0000 - 0x7FFF, 0xA000 - 0xFFFF
  Ack ebus_ack;

  Req vbus_req; // 0x8000 - 0x9FFF
  Ack vbus_ack;

  Req obus_req; // 0xFE00 - 0xFEFF
  Ack obus_ack;

  Req ibus_req; // 0xFF00 - 0xFFFF
  Ack ibus_ack;

  uint8_t dma_data_latch;

  //----------

  uint8_t fb[160*144] = {0};

  uint32_t sentinel = 0xDEADBEEF;
};

//-----------------------------------------------------------------------------
