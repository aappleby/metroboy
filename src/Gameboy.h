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
  void tock();

  uint32_t trace();

  int64_t        get_tcycle()     const { return tcycle; }
  bool           is_frame_start() const { return ppu.is_frame_start(); }
  bool           is_frame_done()  const { return ppu.is_frame_done(); }
  int            get_line()       const { return ppu.get_line(); }
  int            get_pix_count()  const { return ppu.get_pix_count(); }
  const SPU&     get_spu()        const { return spu; }
  const PPU&     get_ppu()        const { return ppu; }
  const uint8_t* get_vram()       const { return vram.ram; }
  const OAM&     get_oam()        const { return oam; }
  bool           get_pix_oe()     const { return ppu.pix_oe; }
  uint8_t        get_pix_out()    const { return ppu.pix_out; }
  sample_t       get_audio_r()    const { return spu_out.out_r; }
  sample_t       get_audio_l()    const { return spu_out.out_l; }
  uint16_t       get_pc()         const { return z80.get_pc(); }
  uint8_t        get_op()         const { return z80.get_op(); }
  uint8_t        get_reg_a()      const { return z80.get_a(); }
                 
  void set_buttons(uint8_t v) { buttons.set(v); }

  // utils
  void check_sentinel() { assert(sentinel == 0xDEADBEEF); }


  void dump(std::string& out);
  void dump_disasm(std::string& out);

  uint8_t framebuffer[160 * 144];

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

  SpuOut spu_out;
  BusOut buttons_out;
  BusOut iram_out;
  BusOut serial_out;
  BusOut vram_out;
  BusOut mmu_out;
  BusOut zram_out;
  BusOut timer_out;
  BusOut oam_out;

  int model = MODEL_DMG;
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
  uint8_t bus_in = 0;

  uint8_t intf = 0;
  uint8_t imask = 0;

  uint32_t sentinel = 0xDEADBEEF;
};

//-----------------------------------------------------------------------------
