#pragma once
#include "Platform.h"

//-----------------------------------------------------------------------------

struct MMU {
  MMU();

  void reset(size_t new_rom_size, uint16_t new_pc);
  void reset(uint16_t new_pc);
  
  void tock_t3(uint16_t addr, uint8_t data, bool read, bool write);

  uint8_t* get_flat_ptr(uint16_t addr);

  size_t rom_size = 0;
  uint8_t bus_out = 0;
  bool bus_oe = false;

private:

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
