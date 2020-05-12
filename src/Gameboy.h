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

//-----------------------------------------------------------------------------

struct Gameboy {
  struct HostOut {
    int x;
    int y;
    int counter;
    uint8_t pix;
    bool pix_oe;
    sample_t out_r;
    sample_t out_l;
    uint32_t trace;
  };

  void    reset(size_t new_rom_size, uint16_t new_pc);
  void    reset(uint16_t new_pc);
  HostOut get_host_data() const { return gb_to_host; }


  void ibus_req2(Req ibus_req);
  void ibus_ack2(Ack& ibus_ack) const;

  void tock();

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

  int64_t  get_tcycle() const { return tcycle; }

  // 0x0000 - rom
  // 0x2000 - rom
  // 0x4000 - rom
  // 0x6000 - rom
  // 0x8000 - vram
  // 0xA000 - cram
  // 0xC000 - iram
  // 0xE000 - eram

  uint8_t* get_vram()   { return vram.get(); }
  uint8_t* get_cram()   { return cart.get_cram(); }
  uint8_t* get_iram()   { return iram.get(); }
  uint8_t* get_eram()   { return iram.get(); }

  uint8_t* get_zram()   { return zram.get(); }

  void set_joypad(uint8_t v) { joypad.set(v); }

  void check_sentinel() { assert(sentinel == 0xDEADBEEF); }

//private:

  int64_t tcycle;

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

  HostOut gb_to_host;
  uint32_t trace_val;

  //----------

  Req cpu_req;
  Ack cpu_ack;

  Req dma_req;
  Ack dma_ack;

  Req ppu_req;
  Ack ppu_ack;

  Req ibus_req;
  Ack ibus_ack;

  Req ebus_req;
  Ack ebus_ack;

  Req obus_req;
  Ack obus_ack;

  Req vbus_req;
  Ack vbus_ack;

  //----------
  // interrupts

  uint8_t intf;
  uint8_t imask;

  Ack ack;

  //----------

  uint32_t sentinel;
};

//-----------------------------------------------------------------------------
