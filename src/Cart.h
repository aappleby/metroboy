#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Cart {
  void set_rom(uint8_t* new_rom, size_t new_rom_size);
  void reset();

  void tock(int phase, const Req& req);
  void tick(int phase, const Req& req, Ack& ebus_ack) const;

  void dump(std::string& d);

  uint8_t* get_flat_ptr(uint16_t addr);
  
  uint8_t* get_rom() const { return rom; }
  size_t get_rom_size() const { return rom_size; }

  uint8_t* get_cram() { return ram_buf; }

//private:
  uint8_t* rom = 0;
  size_t rom_size = 0;
  int rom_bank_count = 0;
  int ram_bank_count = 0;
  bool ram_enable = 0;
  int mode = 0;
  int bank_latch1 = 0;
  int bank_latch2 = 0;

  uint8_t ram_buf[8 * 1024] = {};
};

//-----------------------------------------------------------------------------
