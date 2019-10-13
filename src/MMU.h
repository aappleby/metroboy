#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct MMU {
  struct Out {
    Bus mmu_to_bus;
    Bus mmu_to_dma;
  };

  void reset(size_t new_rom_size, uint16_t new_pc);
  void reset(uint16_t new_pc);

  Out  tick() const;
  void tock(int tcycle_, Bus bus_to_mmus_, Bus dma_to_mmu_);
  void dump(std::string& d);

  uint8_t* get_flat_ptr(uint16_t addr);
  size_t get_rom_size() const { return rom_size; }

private:
  int tphase;
  Bus bus_to_mmu;
  Bus dma_to_mmu;
  Bus mmu_to_bus;
  Bus mmu_to_dma;

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
