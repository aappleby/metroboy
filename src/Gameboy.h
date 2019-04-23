#pragma once

#include "Buttons.h"
#include "IRAM.h"
#include "MMU.h"
#include "PPU.h"
#include "Serial.h"
#include "SPU.h"
#include "Timer.h"
#include "VRAM.h"
#include "ZRAM.h"
#include "Z80.h"

//-----------------------------------------------------------------------------

struct Gameboy {
  Gameboy();

  void reset(size_t new_rom_size, uint16_t new_pc);
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

  int64_t tcycle = -1;

  bool cpu_read_oam = false;
  bool cpu_read_vram = false;
  bool cpu_read_iram = false;
  bool cpu_read_cart = false;

  DMAMode  dma_mode_x = DMA_NONE;
  uint8_t  dma_count_x = 0;
  uint16_t dma_source_x = 0;

  DMAMode  dma_mode_a = DMA_NONE;
  uint8_t  dma_count_a = 0;
  uint16_t dma_source_a = 0;

  DMAMode  dma_mode_b = DMA_NONE;
  uint8_t  dma_count_b = 0;
  uint8_t  dma_data_b = 0;

  uint8_t oam_bus_out = 0;
  bool oam_bus_oe = false;

  uint8_t bus_out = 0;
  bool bus_oe = false;

  uint8_t bus_out_latch = 0;
  bool bus_oe_latch = false;

  bool old_stat_int = false;

  uint8_t intf = 0;
  uint8_t imask = 0;

  uint32_t sentinel = 0xDEADBEEF;
};

//-----------------------------------------------------------------------------
