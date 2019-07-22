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
#include "Types.h"

#include <assert.h>

//-----------------------------------------------------------------------------

struct Gameboy {
  Gameboy();

  void reset(int new_model, size_t new_rom_size, uint16_t new_pc);
  void reset(uint16_t new_pc);

  void tick();
  GameboyOut tock();

  uint32_t trace();

  const Z80& get_cpu() const { return z80; }
  const SPU& get_spu() const { return spu; }
  const PPU& get_ppu() const { return ppu; }
  const OAM& get_oam() const { return oam; }

  int64_t        get_tcycle()     const { return tcycle; }
  const uint8_t* get_vram()       const { return vram.ram; }

  void set_buttons(uint8_t v) { buttons.set(v); }

  void check_sentinel() { assert(sentinel == 0xDEADBEEF); }

  void dump(std::string& out);
  void dump_disasm(std::string& out);

  uint8_t framebuffer[160 * 144];

  PpuOut get_ppu_out() { return ppu_out; }

private:

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

  CpuBus cpu_bus2;

  CpuOut cpu_out;
  PpuOut ppu_out;

  SpuOut spu_out;
  ButtonsOut buttons_out;
  BusOut iram_out;
  BusOut serial_out;
  BusOut vram_out;
  BusOut mmu_out;
  BusOut zram_out;
  TimerOut timer_out;
  BusOut oam_out;

  int model = MODEL_DMG;
  int64_t tcycle = -1;

  bool new_stat_int;
  bool stat_int_delay;

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
  uint8_t bus_in = 0;

  uint8_t intf = 0;
  uint8_t imask = 0;

  uint32_t sentinel = 0xDEADBEEF;
};

//-----------------------------------------------------------------------------
