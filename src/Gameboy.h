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

  void    reset(int new_model, size_t new_rom_size, uint16_t new_pc);
  void    reset(uint16_t new_pc);
  Bus     tick() const;
  HostOut get_host_data() const { return gb_to_host; }
  void    tock();
  void    dump1(std::string& out);
  void    dump2(std::string& out);
  void    dump3(std::string& out);
  void    dump4(std::string& out);

  const Z80& get_cpu() const { return z80; }
  const SPU& get_spu() const { return spu; }
  const PPU& get_ppu() const { return ppu; }

  int64_t        get_tcycle()     const { return tcycle; }
  const uint8_t* get_vram()       const { return vram.get_ram(); }

  void set_buttons(uint8_t v) { buttons.set(v); }

  void check_sentinel() { assert(sentinel == 0xDEADBEEF); }

private:
  Bus cpu_to_bus;
  Bus gb_to_bus;
  Bus iram_to_bus;
  Bus mmu_to_bus;
  Bus pad_to_bus;
  Bus serial_to_bus;
  Bus zram_to_bus;
  Bus spu_to_bus;  
  Bus timer_to_bus;
  Bus ppu_to_bus;
  Bus vram_to_bus;
  Bus oam_to_bus;

  Bus bus_to_cpu;

  Bus dma_to_bus;
  Bus mmu_to_dma;
  Bus iram_to_dma;
  Bus vram_to_dma;
  Bus dma_to_oam;

  Bus ppu_to_vram;
  Bus vram_to_ppu;

  Bus ppu_to_oam;
  Bus oam_to_ppu;

  HostOut gb_to_host;

  enum DMAMode {
    DMA_NONE,
    DMA_VRAM,
    DMA_IRAM,
    DMA_CART
  };

  Z80 z80;
  Timer timer;
  MMU mmu;
  OAM oam;
  VRAM vram;
  IRAM iram;
  ZRAM zram;
  Buttons buttons;
  Serial serial;

  PPU ppu;
  SPU spu;

  int model;
  int tcycle;
  uint32_t trace_val;

  DMAMode  dma_mode_x;
  uint8_t  dma_count_x;
  uint16_t dma_source_x;

  DMAMode  dma_mode_a;
  uint8_t  dma_count_a;
  uint16_t dma_source_a;

  DMAMode  dma_mode_b;
  uint8_t  dma_count_b;
  uint8_t  dma_data_b;

  uint8_t intf;
  uint8_t imask;

  uint32_t sentinel;
};

//-----------------------------------------------------------------------------
