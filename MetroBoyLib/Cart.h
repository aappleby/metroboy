#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

struct Cart {
  void set_rom(uint8_t* new_rom, size_t new_rom_size);
  void reset();

  void tock(const Req& req);
  void tick(const Req& req, Ack& ebus_ack) const;

  void dump(Dumper& dump) const;

  uint8_t* get_cart_rom() { return cart_rom; }
  uint8_t* get_main_ram() { return main_ram; }
  uint8_t* get_cart_ram() { return cart_ram; }

  uint8_t* get_flat_ptr(uint16_t addr);

//private:
  uint8_t* cart_rom = 0;
  size_t   cart_size = 0;
  int rom_bank_count = 0;
  int ram_bank_count = 0;
  bool ram_enable = 0;
  int mode = 0;
  int bank_latch1 = 0;
  int bank_latch2 = 0;

  uint8_t main_ram[8192] = {};
  uint8_t cart_ram[8192] = {};
};

//-----------------------------------------------------------------------------
