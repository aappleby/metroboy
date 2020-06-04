#pragma once

#include "Joypad.h"
#include "IRAM.h"
#include "Cart.h"
#include "DMA.h"
#include "PPU.h"
#include "Serial.h"
#include "SPU.h"
#include "Timer.h"
#include "VRAM.h"
#include "ZRAM.h"
#include "Z80.h"
#include "Types.h"
#include "Bootrom.h"

#include <assert.h>

// 0x0000 - rom
// 0x2000 - rom
// 0x4000 - rom
// 0x6000 - rom
// 0x8000 - vram
// 0xA000 - cram
// 0xC000 - iram
// 0xE000 - eram

//-----------------------------------------------------------------------------

struct Gameboy {
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

  void    set_rom(uint8_t* new_rom, size_t new_rom_size);
  void    reset(uint16_t new_pc);
  HostOut get_host_data() const { return gb_to_host; }

  void sync_to_mcycle() {
    switch(phase & 7) {
    case 0: return;
    case 1: tick2();
    case 2: tock2();
    case 3: tick2();
    case 4: tock2();
    case 5: tick2();
    case 6: tock2();
    case 7: tick2();
    }
  }
  
  void mcycle() {
    assert((phase & 7) == 0);
    tock2();
    tick2();
    tock2();
    tick2();
    tock2();
    tick2();
    tock2();
    tick2();
  }

  void halfcycle() {
    phase & 1 ? tick2() : tock2();
  }

  void tock_req(const Req& ibus_req);
  void tick_ack(Ack& ibus_ack) const;

  void tick2();
  void tock2();

  void dump_cpu   (std::string& out);
  void dump_bus   (std::string& out);
  void dump_zram  (std::string& out);
  void dump_timer (std::string& out);
  void dump_cart  (std::string& out);
  void dump_oam   (std::string& out);
  void dump_joypad(std::string& out);
  void dump_serial(std::string& out);
  void dump_disasm(std::string& out);

  const Z80& get_cpu() const { return z80; }
  const SPU& get_spu() const { return spu; }
  const PPU& get_ppu() const { return ppu; }

  uint8_t* get_rom()    { return cart.get_rom(); }
  uint8_t* get_vram()   { return vram.get(); }
  uint8_t* get_cram()   { return cart.get_cram(); }
  uint8_t* get_iram()   { return iram.get(); }
  uint8_t* get_eram()   { return iram.get(); }

  uint8_t* get_zram()   { return zram.get(); }

  void set_joypad(uint8_t v) { joypad.set(v); }

  void check_sentinel() { assert(sentinel == 0xDEADBEEF); }

//private:

  //----------

  Z80     z80;
  Timer   timer;
  Cart    cart;
  OAM     oam;
  VRAM    vram;
  IRAM    iram;
  ZRAM    zram;
  Joypad  joypad;
  Serial  serial;
  PPU     ppu;
  SPU     spu;
  DMA     dma;
  Bootrom boot;

  //----------

  int64_t phase = 0;

  HostOut gb_to_host;
  uint32_t trace_val = 0;

  uint8_t intf = 0;
  uint8_t imask = 0;
  Ack ack;

  //----------

  Req cpu_req;
  Ack cpu_ack;

  Req dma_ebus_req;
  Req dma_vbus_req;
  Req dma_obus_req;
  Req ppu_vbus_req;
  Req ppu_obus_req;

  Req ibus_req;
  Ack ibus_ack;

  Req ebus_req;
  Ack ebus_ack;

  Req obus_req;
  Ack obus_ack;

  Req vbus_req;
  Ack vbus_ack;

  //----------

  uint8_t fb[160*144] = {0};

  uint32_t sentinel = 0xDEADBEEF;
};

//-----------------------------------------------------------------------------
