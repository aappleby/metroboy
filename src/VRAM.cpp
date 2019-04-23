#include "VRAM.h"
#include "Constants.h"

void VRAM::reset() {
  bus_out = 0x00;
  memset(ram, 0, sizeof(ram));
}

void VRAM::tock(uint16_t addr, uint8_t data, bool read, bool write) {
  if ((addr & 0xE000) == 0x8000) {
    if (read)  bus_out = ram[addr - ADDR_VRAM_BEGIN];
    if (write) ram[addr - ADDR_VRAM_BEGIN] = data;
  }
}
