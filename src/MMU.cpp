#include "MMU.h"

#include <assert.h>

#include "Constants.h"

extern uint8_t rom_buf[];
extern const uint8_t DMG_ROM_bin[];

//-----------------------------------------------------------------------------

void MMU::reset(size_t new_rom_size, uint16_t new_pc) {
  *this = {};

  rom_size = new_rom_size;

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
  assert(rom_bank_count <= 32);

  int ram_banks = rom_buf[0x0149];
  switch (ram_banks) {
  case 0x01: ram_bank_count = 1; break;
  case 0x02: ram_bank_count = 1; break;
  case 0x03: ram_bank_count = 4; break;
  case 0x04: ram_bank_count = 16; break;
  case 0x05: ram_bank_count = 8; break;
  default:   ram_bank_count = 0; break;
  }
  assert(ram_bank_count <= 1);

  disable_boot_rom = new_pc != 0x0000;
}

void MMU::reset(uint16_t new_pc) {
  reset(rom_size, new_pc);
}

//-------------------------------------

BusOut MMU::tick() const {
  return out;
}

void MMU::tock(CpuBus bus) {
  if (bus.write) {
    if (bus.addr <= 0x1FFF) {
      ram_enable = (bus.data & 0x0F) == 0x0A;
    }
    else if (bus.addr >= 0x2000 && bus.addr <= 0x3FFF) {
      bank_latch1 = bus.data & 0b00011111;

      int bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        bank |= 0b00000001;
      }
    }
    else if (bus.addr >= 0x4000 && bus.addr <= 0x5FFF) {
      bank_latch2 = bus.data & 0b00000011;

      int bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        bank |= 0b00000001;
      }
    }
    else if (bus.addr >= 0x6000 && bus.addr <= 0x7FFF) {
      mode = bus.data & 1;
    }
    else if (bus.addr >= 0xA000 && bus.addr <= 0xBFFF) {
      if (ram_enable && ram_bank_count) {
        int bank = mode ? bank_latch2 : 0;
        bank &= (ram_bank_count - 1);
        ram_buf[(bus.addr - 0xA000) | (bank << 13)] = bus.data;
        ram_dirty = true;
        //printf("ram_dirty 0x%04x 0x%02x\n", addr, data);
      }
    }
    else if (bus.addr == ADDR_DISABLE_BOOTROM) {
      disable_boot_rom |= (bus.data != 0);
    }
  }

  uint8_t _bus_out = 0x00;
  bool _bus_oe = false;

  if (bus.read) {
    if (bus.addr <= 0x00FF && !disable_boot_rom) {
      _bus_out = DMG_ROM_bin[bus.addr];
      _bus_oe = true;
    }
    else if (bus.addr <= 0x3FFF) {
      if (mode == 0) {
        _bus_out = rom_buf[bus.addr];
        _bus_oe = true;
      }
      else {
        int bank = (bank_latch2 << 5);
        bank &= (rom_bank_count - 1);
        if (rom_bank_count == 0) bank = 0;
        _bus_out = rom_buf[(bus.addr & 0x3FFF) | (bank << 14)];
        _bus_oe = true;
      }
    }
    else if (bus.addr >= 0x4000 && bus.addr <= 0x7FFF) {
      // so one doc says this should ignore bank_latch2 if mode == 1, but that breaks the mooneye test...
      int bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        bank |= 0b00000001;
      }

      bank &= (rom_bank_count - 1);

      _bus_out = rom_buf[(bus.addr & 0x3FFF) | (bank << 14)];
      _bus_oe = true;
    }
    else if (bus.addr >= 0xA000 && bus.addr <= 0xBFFF) {
      if (ram_enable && ram_bank_count) {
        int bank = mode ? bank_latch2 : 0;
        bank &= (ram_bank_count - 1);
        if (rom_bank_count == 0) bank = 0;
        _bus_out = ram_buf[(bus.addr - 0xA000) | (bank << 13)];
        _bus_oe = true;
      }
    }
  }

  out = { _bus_out, _bus_oe };
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

void MMU::dump(std::string& d) {
  dumpit(out.addr, "0x%04x");
  dumpit(out.data, "0x%02x");
  dumpit(out.oe,   "%d");
}

//-----------------------------------------------------------------------------
