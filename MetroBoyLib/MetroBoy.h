#pragma once

#include "CoreLib/Types.h"
#include "CoreLib/MetroBoyCPU.h"

#include "MetroBoyLib/MetroBoyBootrom.h"
#include "MetroBoyLib/MetroBoyCart.h"
#include "MetroBoyLib/MetroBoyDMA.h"
#include "MetroBoyLib/MetroBoyInterrupts.h"
#include "MetroBoyLib/MetroBoyJoypad.h"
#include "MetroBoyLib/MetroBoyOAM.h"
#include "MetroBoyLib/MetroBoyPPU.h"
#include "MetroBoyLib/MetroBoySerial.h"
#include "MetroBoyLib/MetroBoySPU.h"
#include "MetroBoyLib/MetroBoyTimer.h"
#include "MetroBoyLib/MetroBoyVRAM.h"
#include "MetroBoyLib/MetroBoyZRAM.h"

//-----------------------------------------------------------------------------

struct MetroBoy {
  MetroBoy() = default;

  void reset_app(uint8_t* new_rom, size_t new_rom_size);
  void reset_boot(uint8_t* new_rom, size_t new_rom_size);

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

  MetroBoyCPU     cpu;
  MetroBoyTimer   timer;
  MetroBoyCart    cart;
  MetroBoyVRAM    vram;
  MetroBoyOAM     oam;
  MetroBoyZRAM    zram;
  MetroBoyJoypad  joypad;
  MetroBoySerial  serial;
  MetroBoyPPU     ppu;
  MetroBoySPU     spu;
  MetroBoyDMA     dma;
  MetroBoyBootrom boot;
  MetroBoyInterrupts ints;

  uint8_t intf_delay = 0;
  uint8_t imask_delay = 0;

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
