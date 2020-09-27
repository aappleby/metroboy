#pragma once

#include "CoreLib/Types.h"
#include "CoreLib/Z80.h"

#include "MetroBoyLib/Bootrom.h"
#include "MetroBoyLib/Cart.h"
#include "MetroBoyLib/DMA.h"
#include "MetroBoyLib/Interrupts.h"
#include "MetroBoyLib/Joypad.h"
#include "MetroBoyLib/LCD.h"
#include "MetroBoyLib/PPU.h"
#include "MetroBoyLib/Serial.h"
#include "MetroBoyLib/SPU.h"
#include "MetroBoyLib/Timer.h"
#include "MetroBoyLib/VRAM.h"
#include "MetroBoyLib/ZRAM.h"

//-----------------------------------------------------------------------------

struct MetroBoy {
  MetroBoy() = default;
  MetroBoy(uint16_t new_pc, uint8_t* new_rom, size_t new_rom_size) {
    reset(new_pc, new_rom, new_rom_size);
  }

  void reset(uint16_t new_pc, uint8_t* new_rom, size_t new_rom_size);

  void next_phase();

  void dump_bus   (Dumper& d);
  void dump_disasm(Dumper& d);

  //----------------------------------------

  void check_sentinel() const {
    if (sentinel1 != 0xDEADBEEFBAADF00D) *reinterpret_cast<int*>(0xDEADC0DEDEADC0DE) = 1;
    if (sentinel2 != 0xF00DCAFEBAADC0DE) *reinterpret_cast<int*>(0xDEADC0DEDEADC0DE) = 1;
  }

  uint64_t sentinel1 = 0xDEADBEEFBAADF00D;

  //----------

  Z80     z80;
  Timer   timer;
  Cart    cart;
  VRAM    vram;
  OAM     oam;
  ZRAM    zram;
  Joypad  joypad;
  Serial  serial;
  PPU     ppu;
  SPU     spu;
  DMA     dma;
  Bootrom boot;
  LCD     lcd;
  Interrupts ints;

  int64_t phase_total = 0;

  Req cpu_req  = {0};

  Req ebus_req = {0}; // 0x0000 - 0x7FFF, 0xA000 - 0xFFFF
  Ack ebus_ack = {0};

  Req vbus_req = {0}; // 0x8000 - 0x9FFF
  Ack vbus_ack = {0};

  Req obus_req = {0}; // 0xFE00 - 0xFEFF
  Ack obus_ack = {0};

  Req ibus_req = {0}; // 0xFF00 - 0xFFFF
  Ack ibus_ack = {0};

  uint8_t dma_data_latch;

  uint8_t framebuffer[160*144] = {0};

  //----------

  uint64_t sentinel2 = 0xF00DCAFEBAADC0DE;
};

//-----------------------------------------------------------------------------
