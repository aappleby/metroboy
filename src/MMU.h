#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct MMU {
  void reset(size_t new_rom_size, uint16_t new_pc);
  void reset(uint16_t new_pc);
  
  BusOut tock_t2(CpuBus bus);

  uint8_t* get_flat_ptr(uint16_t addr);
  size_t get_rom_size() const { return rom_size; }

private:

  size_t rom_size = 0;

  bool disable_boot_rom = false;

  int rom_bank_count = 0;

  uint8_t ram_buf[8 * 1024];
  int ram_bank_count = 0;
  bool ram_enable = false;
  bool ram_dirty = false;

  int mode = 0;
  int bank_latch1 = 0;
  int bank_latch2 = 0;
};

//-----------------------------------------------------------------------------
