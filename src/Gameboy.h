#pragma once

#include "utils.h"
#include "Z80.h"
#include "PPU.h"
#include "MMU.h"
#include "VRAM.h"
#include "Timer.h"
#include "IRAM.h"
#include "Buttons.h"
#include "Serial.h"
#include "ZRAM.h"
#include "SPU.h"

//-----------------------------------------------------------------------------

struct Gameboy {
  Gameboy();

  void reset(int new_rom_size, uint16_t new_pc);
  void reset(uint16_t new_pc);

  void tick();
  void tock();

  uint32_t trace();

  // timing
  int64_t get_tcycle()     { return tcycle; }
  bool    is_frame_start() { return ppu.is_frame_start(); }
  bool    is_frame_done()  { return ppu.is_frame_done(); }
  int     get_line()       { return ppu.get_line(); }
  int     get_pix_count()  { return ppu.get_pix_count(); }

  // input
  void set_buttons(uint8_t v) { buttons.set(v); }

  // audio/video
  const PPU& get_ppu()  { return ppu; }
  uint8_t*   get_vram() { return vram.ram; }

  bool    get_pix_oe()     { return ppu.pix_oe; }
  uint8_t get_pix_out()    { return ppu.pix_out; }
  ubit9_t get_audio_r() { return spu.out_r; }
  ubit9_t get_audio_l() { return spu.out_l; }

  uint16_t get_pc() {
    return z80.get_pc();
  }

  uint8_t get_op() {
    return z80.get_op();
  }

  uint8_t get_reg_a() {
    return z80.get_a();
  }

  // utils
  void check_sentinel() {
    SDL_assert_release(sentinel == 0xDEADBEEF);
  }
  char* dump(char* cursor);
  char* dump_disasm(char* cursor);

  int fbX = 0;
  int fbY = 0;
  uint8_t framebuffer[160 * 144];

//private:

  enum DMAMode {
    DMA_NONE,
    DMA_VRAM,
    DMA_IRAM,
    DMA_CART
  };

  Z80 z80;
  MMU mmu;
  PPU ppu;
  OAM oam;
  SPU spu;
  Timer timer;
  VRAM vram;
  IRAM iram;
  Buttons buttons;
  Serial serial;
  ZRAM zram;

  int64_t tcycle;

  bool cpu_read_oam;
  bool cpu_read_vram;
  bool cpu_read_iram;
  bool cpu_read_cart;

  DMAMode  dma_mode_x;
  uint8_t  dma_count_x;
  uint16_t dma_source_x;

  DMAMode  dma_mode_a;
  uint8_t  dma_count_a;
  uint16_t dma_source_a;

  DMAMode  dma_mode_b;
  uint8_t  dma_count_b;
  uint8_t  dma_data_b;

  uint8_t oam_bus_out;
  bool oam_bus_oe;

  uint8_t bus_out;
  bool bus_oe;

  uint8_t bus_out_latch;
  bool bus_oe_latch;

  bool old_stat_int;

  uint8_t intf;
  uint8_t imask;

  uint32_t sentinel = 0xDEADBEEF;
};

//-----------------------------------------------------------------------------
