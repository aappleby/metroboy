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

//-----------------------------------------------------------------------------

MMU::Out MMU::tick() const {
  return {
    mmu_to_bus,
    mmu_to_dma
  };
}

void MMU::tock(int tcycle_, Bus bus_to_mmu_, Bus dma_to_mmu_) {
  tphase = tcycle_ & 3;
  bus_to_mmu = bus_to_mmu_;
  dma_to_mmu = dma_to_mmu_;
  mmu_to_bus = {};
  mmu_to_dma = {};

  bool bus_hit = (bus_to_mmu.addr <= 0x7FFF) | (0xA000 <= bus_to_mmu.addr && bus_to_mmu.addr <= 0xBFFF);
  bool dma_hit = (dma_to_mmu.addr <= 0x7FFF) | (0xA000 <= dma_to_mmu.addr && dma_to_mmu.addr <= 0xBFFF);

  bus_hit &= (bus_to_mmu.read || bus_to_mmu.write);
  dma_hit &= (dma_to_mmu.read != 0);

  uint16_t addr = dma_hit ? dma_to_mmu.addr : bus_to_mmu.addr;
  uint16_t data = dma_hit ? dma_to_mmu.data : bus_to_mmu.data;
  bool read     = dma_hit ? dma_to_mmu.read : bus_to_mmu.read;
  bool write    = dma_hit ? dma_to_mmu.write : bus_to_mmu.write;

  if (write) {
    if (0x0000 <= addr && addr <= 0x1FFF) {
      ram_enable = (data & 0x0F) == 0x0A;
    }
    else if (0x2000 <= addr && addr <= 0x3FFF) {
      bank_latch1 = data & 0b00011111;

      int bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        bank |= 0b00000001;
      }
    }
    else if (0x4000 <= addr && addr <= 0x5FFF) {
      bank_latch2 = data & 0b00000011;

      int bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        bank |= 0b00000001;
      }
    }
    else if (0x6000 <= addr && addr <= 0x7FFF) {
      mode = data & 1;
    }
    else if (0xA000 <= addr && addr <= 0xBFFF) {
      if (ram_enable && ram_bank_count) {
        int bank = mode ? bank_latch2 : 0;
        bank &= (ram_bank_count - 1);
        ram_buf[(addr - 0xA000) | (bank << 13)] = (uint8_t)data;
        ram_dirty = true;
      }
    }
    else if (addr == ADDR_DISABLE_BOOTROM) {
      disable_boot_rom |= (data != 0);
    }
  }
  else if (read) {
    if (addr <= 0x00FF && !disable_boot_rom) {
      data = DMG_ROM_bin[addr];
    }
    else if (0x0000 <= addr && addr <= 0x3FFF) {
      if (mode == 0) {
        data = rom_buf[addr];
      }
      else {
        int bank = (bank_latch2 << 5);
        bank &= (rom_bank_count - 1);
        if (rom_bank_count == 0) bank = 0;
        data = rom_buf[(addr & 0x3FFF) | (bank << 14)];
      }
    }
    else if (0x4000 <= addr && addr <= 0x7FFF) {
      // so one doc says this should ignore bank_latch2 if mode == 1, but that breaks the mooneye test...
      int bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) {
        bank |= 0b00000001;
      }

      bank &= (rom_bank_count - 1);
      data = rom_buf[(addr & 0x3FFF) | (bank << 14)];
    }
    else if (0xA000 <= addr && addr <= 0xBFFF) {
      if (ram_enable && ram_bank_count) {
        int bank = mode ? bank_latch2 : 0;
        bank &= (ram_bank_count - 1);
        if (rom_bank_count == 0) bank = 0;
        data = ram_buf[(addr - 0xA000) | (bank << 13)];
      }
    }
  }

  if (dma_hit) {
    mmu_to_dma = dma_to_mmu;
    mmu_to_dma.data = data;
    mmu_to_dma.ack = true;
  }
  else if (bus_hit) {
    mmu_to_bus = bus_to_mmu;
    mmu_to_bus.data = data;
    mmu_to_bus.ack = true;
  }
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
  print_bus(d, "bus_to_mmu", bus_to_mmu);
  print_bus(d, "mmu_to_bus", mmu_to_bus);
  sprintf(d, "\n");
  print_bus(d, "dma_to_mmu", dma_to_mmu);
  print_bus(d, "mmu_to_dma", mmu_to_dma);
}

//-----------------------------------------------------------------------------
