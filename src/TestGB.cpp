#include "TestGB.h"

using namespace Schematics;

TestGB gb;

//-----------------------------------------------------------------------------

void TestGB::ext_preset() {
  ext_bus.preset();

  if (!ext_bus.PIN_RD_A && !ext_bus.PIN_CS_A) {
    uint16_t ext_addr = ext_bus.get_addr();

    if (0x0000 <= ext_addr && ext_addr <= 0x7FFF) {
      // Cart rom
      //uint8_t d = rom[ext_addr];
      uint8_t d = 0;
      ext_bus.preset_d(true, d);
    }
    else if (0xC000 <= ext_addr && ext_addr <= 0xDFFF) {
      // Main ram
      //uint8_t d = ram[ext_addr - 0xC000];
      uint8_t d = 0;
      ext_bus.preset_d(true, d);
    }
    else if (0xE000 <= ext_addr && ext_addr <= 0xFFFF) {
      // Echo ram
      //uint8_t d = ram[ext_addr - 0xE000];
      uint8_t d = 0;
      ext_bus.preset_d(true, d);
    }
    else {
      printf("Bad address?\n");
      __debugbreak();
    }
  }
  else {
    ext_bus.preset_d(false, 0);
  }
}

//-----------------------------------------------------------------------------

void TestGB::tick_everything() {
  auto rst_sig = rst_reg.sig(*this);
  auto clk_sig = clk_reg.sig(*this);
  auto adr_sig = adr_reg.sig(cpu_bus);
  auto cpu_sig = cpu_bus.sig(*this);

  clk_reg.tick(*this);
  dma_reg.tick(*this);
  tim_reg.tick(*this);
  ser_reg.tick(*this);


  joy_reg.tick(rst_sig, clk_sig, adr_sig, cpu_sig, cpu_bus);

  ppu_reg.tick(*this);
  sst_reg.tick(*this);
  lcd_reg.tick(*this);
  pxp_reg.tick(*this);
  cpu_bus.tick(*this);
  vram_pins.tick(*this);
}

//-----------------------------------------------------------------------------

bool TestGB::commit_everything() {
  bool changed = false;

  changed |= lcd_reg.commit();
  changed |= pxp_reg.commit();
  changed |= sst_reg.commit();
  changed |= tim_reg.commit();
  changed |= ppu_reg.commit();
  changed |= win_reg.commit();
  changed |= ext_bus.commit();
  changed |= lcd_reg.commit_pins();
  changed |= ser_reg.commit();

  return changed;
}

//-----------------------------------------------------------------------------
