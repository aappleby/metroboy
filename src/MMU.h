#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct MMU {
  void reset(size_t new_rom_size, uint16_t new_pc);
  void reset(uint16_t new_pc);

  BusOut tick() const;
  void tock(CpuBus bus);

  uint8_t* get_flat_ptr(uint16_t addr);
  size_t get_rom_size() const { return rom_size; }

private:
  BusOut out;
  size_t rom_size;
  int rom_bank_count;
  int ram_bank_count;
  bool disable_boot_rom;
  bool ram_enable;
  bool ram_dirty;
  int mode;
  int bank_latch1;
  int bank_latch2;

  uint8_t ram_buf[8 * 1024];
};

//-----------------------------------------------------------------------------
