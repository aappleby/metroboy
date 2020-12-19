#include "MetroBoyLib/MetroBoyCart.h"

#include <assert.h>

#include "CoreLib/Constants.h"

#include <string.h>

// 0 0x0000 - 8k rom
// 1 0x2000 - 8k rom
// 2 0x4000 - 8k rom
// 3 0x6000 - 8k rom
// 4 0x8000 - 8k video ram
// 5 0xA000 - 8k cart ram
// 6 0xC000 - 8k sys ram
// 7 0xE000 - 8k echo ram

//-----------------------------------------------------------------------------

void MetroBoyCart::set_rom(uint8_t* new_rom, size_t new_rom_size) {
  cart_rom  = new_rom;
  cart_size = new_rom_size;
}

//-----------------------------------------------------------------------------

void MetroBoyCart::reset_to_cart() {
  ram_enable = false;
  mode = 0;
  bank_latch1 = 0;
  bank_latch2 = 0;

  const int rom_banks = cart_rom[0x0148];
  rom_bank_count = -1;
  switch (rom_banks) {
  case 0x00: rom_bank_count = 0; break;
  case 0x01: rom_bank_count = 4; break;
  case 0x02: rom_bank_count = 8; break;
  case 0x03: rom_bank_count = 16; break;
  case 0x04: rom_bank_count = 32; break;
  case 0x05: rom_bank_count = 64; break;
  case 0x06: rom_bank_count = 128; break;
  case 0x52: assert(false); rom_bank_count = 72; break;
  case 0x53: assert(false); rom_bank_count = 80; break;
  case 0x54: assert(false); rom_bank_count = 96; break;
  default:   rom_bank_count = 0; break;
  }
  assert(rom_bank_count >= 0 && rom_bank_count <= 32);

  const int ram_banks = cart_rom[0x0149];
  ram_bank_count = -1;
  switch (ram_banks) {
  case 0x01: ram_bank_count = 1; break;
  case 0x02: ram_bank_count = 1; break;
  case 0x03: ram_bank_count = 4; break;
  case 0x04: ram_bank_count = 16; break;
  case 0x05: ram_bank_count = 8; break;
  default:   ram_bank_count = 0; break;
  }
  assert(ram_bank_count >= 0 && ram_bank_count <= 1);

  memset(cart_ram, 0, 8 * 1024);
  memset(main_ram, 0, 8 * 1024);
}

//-----------------------------------------------------------------------------

void MetroBoyCart::tick(int phase_total, const Req& req, Ack& ack) const {
  (void)phase_total;

  const int region = req.addr >> 13;
  const uint16_t ram_addr = req.addr & 0x1FFF;
  const uint16_t rom_addr = req.addr & 0x7FFF;

  if (req.read) {
    if (region == 0 || region == 1) {
      // cart_rom
      ack.addr = req.addr;
      ack.read++;
      if (mode == 0) {
        ack.data_lo = cart_rom[rom_addr];
      }
      else {
        const int rom_bank = rom_bank_count ? (bank_latch2 << 5) & (rom_bank_count - 1) : 0;
        ack.data_lo = cart_rom[(rom_addr & 0x3FFF) | (rom_bank << 14)];
      }
    }
    else if (region == 2 || region == 3) {
      // banked cart_rom
      ack.addr = req.addr;
      ack.read++;
      // so one doc says this should ignore bank_latch2 if mode == 1, but that breaks the mooneye test...
      int rom_bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        rom_bank |= 0b00000001;
      }
      rom_bank &= (rom_bank_count - 1);

      ack.data_lo = cart_rom[(rom_addr & 0x3FFF) | (rom_bank << 14)];
    }
    else if (region == 4) {
      // vram
    }
    else if (region == 5) {
      // cart_ram
      ack.addr = req.addr;
      ack.read++;
      if (ram_enable && ram_bank_count) {
        int ram_bank = mode ? bank_latch2 : 0;
        ram_bank &= (ram_bank_count - 1);
        if (ram_bank_count == 0) ram_bank = 0;
        ack.data_lo = cart_ram[(ram_bank << 13) | ram_addr];
      }
    }
    else if (region == 6 || region == 7) {
      // main_ram
      ack.addr = req.addr;
      ack.read++;
      ack.data_lo = main_ram[ram_addr];
    }
  }
}

//-----------------------------------------------------------------------------

void MetroBoyCart::tock(int phase_total, const Req& req) {
  const int region = req.addr >> 13;
  const uint16_t ram_addr = req.addr & 0x1FFF;
  const uint16_t rom_addr = req.addr & 0x7FFF;

  if (DELTA_GH && req.write_sync) {
    if (region == 0) {
      // cart_rom
      ram_enable = (req.data_lo & 0x0F) == 0x0A;
    }
    else if (region == 1) {
      // cart_rom
      bank_latch1 = req.data_lo & 0b00011111;
    }
    else if (region == 2) {
      // banked cart_rom
      bank_latch2 = req.data_lo & 0b00000011;
    }
    else if (region == 3) {
      // banked cart_rom
      mode = req.data_lo & 1;
    }
    else if (region == 4) {
      // vram
    }
    else if (region == 5) {
      // cart_ram
      if (ram_enable && ram_bank_count) {
        int ram_bank = mode ? bank_latch2 : 0;
        ram_bank &= (ram_bank_count - 1);
        cart_ram[(ram_bank << 13) | ram_addr] = static_cast<uint8_t>(req.data_lo);
      }
    }
    else if (region == 6 || region == 7) {
      // main_ram
      main_ram[ram_addr] = static_cast<uint8_t>(req.data_lo);
    }
  }
}

//-----------------------------------------------------------------------------

uint8_t* MetroBoyCart::get_flat_ptr(uint16_t addr) {
  const int region = addr >> 13;
  const uint16_t ram_addr = addr & 0x1FFF;
  const uint16_t rom_addr = addr & 0x7FFF;

  if (region == 0 || region == 1) {
    // cart_rom
    return &cart_rom[rom_addr];
  }
  else if (region == 2 || region == 3) {
    // banked cart_rom
    int rom_bank = ((bank_latch2 << 5) | bank_latch1);
    if (bank_latch1 == 0) rom_bank |= 0b00000001;
    rom_bank &= (rom_bank_count - 1);
    return &cart_rom[(rom_bank << 14) | (rom_addr & 0x3FFF)];
  }
  else if (region == 4) {
    // vram
    return nullptr;
  }
  else if (region == 5) {
    // cart_ram
    if (ram_enable && ram_bank_count) {
      int ram_bank = mode ? bank_latch2 : 0;
      ram_bank &= (ram_bank_count - 1);
      return &cart_ram[(ram_bank << 13) | ram_addr];
    }
  }
  else if (region == 6 || region == 7) {
    // main_ram
    return &main_ram[ram_addr];
  }

  return nullptr;
}

//-----------------------------------------------------------------------------

void MetroBoyCart::dump(Dumper& d) const {
  d("\002--------------CART--------------\001\n");

  d("cart_rom       %p\n", cart_rom);
  d("cart_size      %d\n", cart_size);
  d("rom_bank_count %d\n", rom_bank_count);
  d("ram_bank_count %d\n", ram_bank_count);
  d("ram_enable     %d\n", ram_enable);
  d("mode           %d\n", mode);
  d("bank_latch1    %d\n", bank_latch1);
  d("bank_latch2    %d\n", bank_latch2);
}

//-----------------------------------------------------------------------------
