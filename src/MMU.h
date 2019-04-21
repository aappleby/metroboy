#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

//-----------------------------------------------------------------------------

struct MMU {
  MMU();

  void reset(int new_rom_size, uint16_t new_pc);
  void reset(uint16_t new_pc);
  
  void tock_t3(uint16_t addr, uint8_t data, bool read, bool write);

  uint8_t* get_flat_ptr(uint16_t addr);

  int rom_size;
  uint8_t bus_out;
  bool bus_oe;

private:

  bool disable_boot_rom;

  int rom_bank_count;

  uint8_t ram_buf[8 * 1024];
  int ram_bank_count;
  bool ram_enable;
  bool ram_dirty;

  int mode;
  int bank_latch1;
  int bank_latch2;
};
