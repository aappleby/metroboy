#include "Cart.h"

#include <assert.h>

#include "Constants.h"

// 0x0000 - rom
// 0x2000 - rom
// 0x4000 - rom
// 0x6000 - rom
// 0x8000 - vram
// 0xA000 - cram
// 0xC000 - iram
// 0xE000 - eram

//-----------------------------------------------------------------------------

void Cart::set_rom(uint8_t* new_rom, size_t new_rom_size) {
  rom = new_rom;
  rom_size = new_rom_size;
}

//-----------------------------------------------------------------------------

void Cart::reset() {
  ack = {};
  ram_enable = false;
  mode = 0;
  bank_latch1 = 0;
  bank_latch2 = 0;
  memset(ram_buf, 0, 8 * 1024);

  const int rom_banks = rom[0x0148];
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

  const int ram_banks = rom[0x0149];
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
}

//-----------------------------------------------------------------------------

void Cart::tock_req(const Req& req) {
  ack = {0};
  if (!req) return;

  const int region = req.addr >> 13;
  if (region == 4) return;  // vram not on ebus
  if (region >= 6) return;  // iram has its own handler

  const bool rom_hit = (region < 4);
  const bool ram_hit = region == 5;

  const uint16_t ram_addr = req.addr & 0x1FFF;
  const uint16_t rom_addr = req.addr & 0x7FFF;

  ack = {0};

  // should check what phase this happens on...
  if (req.write && (rom_hit || ram_hit)) {
    if (region == 0) {
      ram_enable = (req.data & 0x0F) == 0x0A;
    }
    else if (region == 1) {
      bank_latch1 = req.data & 0b00011111;
    }
    else if (region == 2) {
      bank_latch2 = req.data & 0b00000011;
    }
    else if (region == 3) {
      mode = req.data & 1;
    }
    else if (region == 5) {
      if (ram_enable && ram_bank_count) {
        int ram_bank = mode ? bank_latch2 : 0;
        ram_bank &= (ram_bank_count - 1);
        ram_buf[(ram_addr - 0xA000) | (ram_bank << 13)] = static_cast<uint8_t>(req.data);
      }
    }
    ack = {
      .addr  = req.addr,
      .data  = req.data,
      .read  = 0,
      .write = 1,
    };
  }
  else if (req.read && (rom_hit || ram_hit)) {
    ack = {
      .addr  = req.addr,
      .data  = 0,
      .read  = 1,
      .write = 0,
    };

    if (region == 0 || region == 1) {
      if (mode == 0) {
        ack.data = rom[rom_addr];
      }
      else {
        const int rom_bank = rom_bank_count ? (bank_latch2 << 5) & (rom_bank_count - 1) : 0;
        ack.data = rom[(rom_addr & 0x3FFF) | (rom_bank << 14)];
      }
    }
    else if (region == 2 || region == 3) {
      // so one doc says this should ignore bank_latch2 if mode == 1, but that breaks the mooneye test...
      int rom_bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        rom_bank |= 0b00000001;
      }
      rom_bank &= (rom_bank_count - 1);

      ack.data = rom[(rom_addr & 0x3FFF) | (rom_bank << 14)];
    }
    else if (region == 5) {
      if (ram_enable && ram_bank_count) {
        int ram_bank = mode ? bank_latch2 : 0;
        ram_bank &= (ram_bank_count - 1);
        if (ram_bank_count == 0) ram_bank = 0;

        ack.data = ram_buf[(rom_addr - 0xA000) | (ram_bank << 13)];
      }
    }
  }
  else {
    assert(false);
  }
}

void Cart::tick_ack(Ack& ack_) const {
  ack_.addr  += ack.addr;
  ack_.data  += ack.data;
  ack_.read  += ack.read;
  ack_.write += ack.write;
}

//-----------------------------------------------------------------------------

uint8_t* Cart::get_flat_ptr(uint16_t addr) {
  if (addr <= 0x3FFF) {
    if (mode == 0) {
      return &rom[addr];
    }
    else {
      int region = (bank_latch2 << 5);
      region &= (rom_bank_count - 1);
      if (rom_bank_count == 0) region = 0;
      return &rom[(addr & 0x3FFF) | (region << 14)];
    }
  }
  else if (addr >= 0x4000 && addr <= 0x7FFF) {
    int region = ((bank_latch2 << 5) | bank_latch1);
    if (bank_latch1 == 0) {
      region |= 0b00000001;
    }

    region &= (rom_bank_count - 1);

    return &rom[(addr & 0x3FFF) | (region << 14)];
  }
  else if (addr >= 0xA000 && addr <= 0xBFFF) {
    if (ram_enable && ram_bank_count) {
      int region = mode ? bank_latch2 : 0;
      region &= (ram_bank_count - 1);
      if (rom_bank_count == 0) region = 0;
      return &ram_buf[(addr - 0xA000) | (region << 13)];
    }
    else
    {
      return nullptr;
    }
  }
  else {
    return nullptr;
  }
}

//-----------------------------------------------------------------------------

void Cart::dump(std::string& d) {
  sprintf(d, "\002--------------CART--------------\001\n");

  sprintf(d, "rom_size       %d\n", rom_size);
  sprintf(d, "rom_bank_count %d\n", rom_bank_count);
  sprintf(d, "ram_bank_count %d\n", ram_bank_count);
  sprintf(d, "ram_enable     %d\n", ram_enable);
  sprintf(d, "mode           %d\n", mode);
  sprintf(d, "bank_latch1    %d\n", bank_latch1);
  sprintf(d, "bank_latch2    %d\n", bank_latch2);

  sprintf(d, "bank_latch2    %d\n", bank_latch2);
  sprintf(d, "bank_latch2    %d\n", bank_latch2);
  sprintf(d, "bank_latch2    %d\n", bank_latch2);
  sprintf(d, "bank_latch2    %d\n", bank_latch2);

  sprintf(d, "ack: "); print_ack(d, ack);
}

//-----------------------------------------------------------------------------
