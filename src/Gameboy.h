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
  
  
  Ack     on_ibus_req(Req ibus_req);

  void    tock();

  void dump_cpu   (std::string& out);
  void dump_bus   (std::string& out);
  void dump_zram  (std::string& out);
  void dump_timer (std::string& out);
  void dump_cart  (std::string& out);
  void dump_vram  (std::string& out);
  void dump_iram  (std::string& out);
  void dump_oam   (std::string& out);
  void dump_joypad(std::string& out);
  void dump_serial(std::string& out);
  void dump_disasm(std::string& out);

  const Z80& get_cpu() const { return z80; }
  const SPU& get_spu() const { return spu; }
  const PPU& get_ppu() const { return ppu; }

  int64_t  get_tcycle() const { return tcycle; }
  uint8_t* get_vram()   { return vram.get(); }
  uint8_t* get_zram()   { return zram.get(); }
  uint8_t* get_iram()   { return iram.get(); }

  void set_joypad(uint8_t v) { joypad.set(v); }

  void check_sentinel() { assert(sentinel == 0xDEADBEEF); }

//private:

  int tcycle;

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

  Req bus_req_cpu;
  Ack bus_ack_cpu;

  Req ibus_req_cpu;
  Ack ibus_ack_gb;
  Ack ibus_ack_tim;
  Ack ibus_ack_zram;
  Ack ibus_ack_joy;
  Ack ibus_ack_ser;
  Ack ibus_ack_ppu;
  Ack ibus_ack_spu;
  Ack ibus_ack_dma;
  Ack ibus_ack_boot;
  Ack ibus_ack_cpu;

  Req ebus_req_cpu;
  Req ebus_req_dma;
  Ack ebus_ack_cpu;
  Ack ebus_ack_dma;

  Req obus_req_cpu;
  Req obus_req_ppu;
  Req obus_req_dma;
  Ack obus_ack_cpu;
  Ack obus_ack_ppu;
  Ack obus_ack_dma;

  Req vbus_req_cpu;
  Req vbus_req_ppu;
  Req vbus_req_dma;
  Ack vbus_ack_cpu;
  Ack vbus_ack_ppu;
  Ack vbus_ack_dma;

  //----------
  // interrupts

  uint8_t intf;
  uint8_t imask;

  //----------

  uint32_t sentinel;
};

//-----------------------------------------------------------------------------
