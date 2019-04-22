#include "MMU.h"
#include "constants.h"
#include "Utils.h"

#include <stdio.h>
#include <memory.h>

extern uint8_t rom_buf[1024 * 1024];

//-----------------------------------------------------------------------------

MMU::MMU() {
  rom_size = 0;
}

void MMU::reset(int new_rom_size, uint16_t new_pc) {
  rom_size = new_rom_size;
  reset(new_pc);
}

void MMU::reset(uint16_t new_pc) {
  memset(ram_buf, 0, sizeof(ram_buf));
  ram_enable = false;

  mode = 0;
  bank_latch1 = 0;
  bank_latch2 = 0;

  bus_out = rom_buf[new_pc];
  bus_oe = true;

  int cart_type = rom_buf[0x147];

  const char* cart_strings[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1 + RAM",
    "MBC1 + RAM + BATTERY",
    "---",
    "MBC2",
    "MBC2 + BATTERY",
    "---",
    "ROM + RAM",
    "ROM + RAM + BATTERY",
    "---",
    "MMM01",
    "MMM01 + RAM",
    "MMM01 + RAM + BATTERY",
    "---",
    "MBC3 + TIMER + BATTERY",
    "MBC3 + TIMER + RAM + BATTERY",
    "MBC3",
    "MBC3 + RAM",
    "---",
  };

  if (cart_type < 20) {
    //printf_console("MMU: cart type %s\n", cart_strings[cart_type]);
  }


  int rom_banks = rom_buf[0x0148];
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

  SDL_assert_always(rom_bank_count <= 32);

  int ram_banks = rom_buf[0x0149];
  switch (ram_banks) {
  case 0x01: ram_bank_count = 1; break;
  case 0x02: ram_bank_count = 1; break;
  case 0x03: ram_bank_count = 4; break;
  case 0x04: ram_bank_count = 16; break;
  case 0x05: ram_bank_count = 8; break;
  default:   ram_bank_count = 0; break;
  }

  SDL_assert_always(ram_bank_count <= 1);

  //printf_console("MMU: %d rom banks\n", rom_bank_count);
  //printf_console("MMU: %d ram banks\n", ram_bank_count);

  disable_boot_rom = !(new_pc == 0x0000);
}

//-------------------------------------

void MMU::tock_t3(uint16_t addr, uint8_t data, bool read, bool write) {
  if (write) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
      ram_enable = (data & 0x0F) == 0x0A;
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
      bank_latch1 = data & 0b00011111;

      int bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        bank |= 0b00000001;
      }
    }
    else if (addr >= 0x4000 && addr <= 0x5FFF) {
      bank_latch2 = data & 0b00000011;

      int bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        bank |= 0b00000001;
      }
    }
    else if (addr >= 0x6000 && addr <= 0x7FFF) {
      mode = data & 1;
    }
    else if (addr >= 0xA000 && addr <= 0xBFFF) {
      if (ram_enable && ram_bank_count) {
        int bank = mode ? bank_latch2 : 0;
        bank &= (ram_bank_count - 1);
        ram_buf[(addr - 0xA000) | (bank << 13)] = data;
        ram_dirty = true;
        //printf("ram_dirty 0x%04x 0x%02x\n", addr, data);
      }
    }
    else if (addr == ADDR_DISABLE_BOOTROM) {
      disable_boot_rom |= (data != 0);
    }
  }

  uint8_t _bus_out = 0x00;
  bool _bus_oe = false;

  if (read) {
    if (addr <= 0x00FF && !disable_boot_rom) {
      _bus_out = DMG_ROM_bin[addr];
      _bus_oe = true;
    }
    else if (addr <= 0x3FFF) {
      if (mode == 0) {
        _bus_out = rom_buf[addr];
        _bus_oe = true;
      }
      else {
        int bank = (bank_latch2 << 5);
        bank &= (rom_bank_count - 1);
        if (rom_bank_count == 0) bank = 0;
        _bus_out = rom_buf[(addr & 0x3FFF) | (bank << 14)];
        _bus_oe = true;
      }
    }
    else if (addr >= 0x4000 && addr <= 0x7FFF) {
      // so one doc says this should ignore bank_latch2 if mode == 1, but that breaks the mooneye test...
      int bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        bank |= 0b00000001;
      }

      bank &= (rom_bank_count - 1);

      _bus_out = rom_buf[(addr & 0x3FFF) | (bank << 14)];
      _bus_oe = true;
    }
    else if (addr >= 0xA000 && addr <= 0xBFFF) {
      if (ram_enable && ram_bank_count) {
        int bank = mode ? bank_latch2 : 0;
        bank &= (ram_bank_count - 1);
        if (rom_bank_count == 0) bank = 0;
        _bus_out = ram_buf[(addr - 0xA000) | (bank << 13)];
        _bus_oe = true;
      }
    }
  }

  bus_out = _bus_out;
  bus_oe = _bus_oe;
}

//-----------------------------------------------------------------------------

uint8_t* MMU::get_flat_ptr(uint16_t addr) {
  if (addr <= 0x00FF && !disable_boot_rom) {
    return (uint8_t*)(&DMG_ROM_bin[addr]);
  }
  else if (addr <= 0x3FFF) {
    if (mode == 0) {
      return &rom_buf[addr];
    }
    else {
      int bank = (bank_latch2 << 5);
      bank &= (rom_bank_count - 1);
      if (rom_bank_count == 0) bank = 0;
      return &rom_buf[(addr & 0x3FFF) | (bank << 14)];
    }
  }
  else if (addr >= 0x4000 && addr <= 0x7FFF) {
    int bank = ((bank_latch2 << 5) | bank_latch1);
    if (bank_latch1 == 0) {
      bank |= 0b00000001;
    }

    bank &= (rom_bank_count - 1);

    return &rom_buf[(addr & 0x3FFF) | (bank << 14)];
  }
  else if (addr >= 0xA000 && addr <= 0xBFFF) {
    if (ram_enable && ram_bank_count) {
      int bank = mode ? bank_latch2 : 0;
      bank &= (ram_bank_count - 1);
      if (rom_bank_count == 0) bank = 0;
      return &ram_buf[(addr - 0xA000) | (bank << 13)];
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
