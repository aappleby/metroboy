#include "TestGB.h"

using namespace Schematics;

TestGB gb;

//-----------------------------------------------------------------------------

void TestGB::tick_everything() {
  auto rst_sig = rst_reg.sig(*this);
  auto clk_sig = clk_reg.sig(*this);
  auto cpu_sig = cpu_bus.sig(*this);

  clk_reg.tick(*this);
  dma_reg.tick(*this);
  tim_reg.tick(*this);
  ser_reg.tick(*this);


  joy_reg.tick(rst_sig, clk_sig, cpu_bus, cpu_sig);

  ppu_reg.tick(*this);
  sst_reg.tick(*this);
  lcd_reg.tick(*this);
  pxp_reg.tick(*this);
  //cpu_sig.tick(*this);
  cpu_pins_out.tick(*this);
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
  changed |= lcd_reg.commit();
  changed |= ser_reg.commit();

  /* PIN_71 */ changed |= EXT_PIN_RST.clear_preset();
  /* PIN_72 */ /*GND*/
               /* PIN_73 */ /*CLKOUT*/
  /* PIN_74 */ changed |= EXT_PIN_CLK_GOOD.clear_preset();

  /* PIN_17 */ changed |= EXT_PIN_D0_C.clear_preset();       // -> TOVO,SOMA
  /* PIN_18 */ changed |= EXT_PIN_D1_C.clear_preset();       // -> RUZY,RONY
  /* PIN_19 */ changed |= EXT_PIN_D2_C.clear_preset();       // -> ROME,RAXY
  /* PIN_20 */ changed |= EXT_PIN_D3_C.clear_preset();       // -> SAZA,SELO
  /* PIN_21 */ changed |= EXT_PIN_D4_C.clear_preset();       // -> TEHE,SODY
  /* PIN_22 */ changed |= EXT_PIN_D5_C.clear_preset();       // -> RATU,SAGO
  /* PIN_23 */ changed |= EXT_PIN_D6_C.clear_preset();       // -> SOCA,RUPA
  /* PIN_24 */ changed |= EXT_PIN_D7_C.clear_preset();       // -> RYBA,SAZY

  return changed;
}

//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------

//void preset(bool PIN_RD_A, bool PIN_CS_A);

/*
void preset_d(bool oe, uint8_t d) {
PIN_D0_C.preset(oe, d & 0x01);
PIN_D1_C.preset(oe, d & 0x02);
PIN_D2_C.preset(oe, d & 0x04);
PIN_D3_C.preset(oe, d & 0x08);
PIN_D4_C.preset(oe, d & 0x10);
PIN_D5_C.preset(oe, d & 0x20);
PIN_D6_C.preset(oe, d & 0x40);
PIN_D7_C.preset(oe, d & 0x80);
}
*/


#if 0
void ExtPinsOut::preset() {
  EXT_PIN_WR_C.preset(true, 0);   // -> P07.UBAL
  EXT_PIN_RD_C.preset(true, 0);   // -> P07.UJYV
  EXT_PIN_A00_C.preset(true, 0);   // -> P08.KOVA
  EXT_PIN_A01_C.preset(true, 0);   // -> P08.CAMU
  EXT_PIN_A02_C.preset(true, 0);   // -> P08.BUXU
  EXT_PIN_A03_C.preset(true, 0);   // -> P08.BASE
  EXT_PIN_A04_C.preset(true, 0);   // -> P08.AFEC
  EXT_PIN_A05_C.preset(true, 0);   // -> P08.ABUP
  EXT_PIN_A06_C.preset(true, 0);   // -> P08.CYGU
  EXT_PIN_A07_C.preset(true, 0);   // -> P08.COGO
  EXT_PIN_A08_C.preset(true, 0);   // -> P08.MUJY
  EXT_PIN_A09_C.preset(true, 0);   // -> P08.NENA
  EXT_PIN_A10_C.preset(true, 0);   // -> P08.SURA
  EXT_PIN_A11_C.preset(true, 0);   // -> P08.MADY
  EXT_PIN_A12_C.preset(true, 0);   // -> P08.LAHE
  EXT_PIN_A13_C.preset(true, 0);   // -> P08.LURA
  EXT_PIN_A14_C.preset(true, 0);   // -> P08.PEVO
  EXT_PIN_A15_C.preset(true, 0);   // -> P08.RAZA

  if (!PIN_RD_A && !PIN_CS_A) {
    uint16_t ext_addr = get_addr();

    if (0x0000 <= ext_addr && ext_addr <= 0x7FFF) {
      // Cart rom
      //uint8_t d = rom[ext_addr];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else if (0xC000 <= ext_addr && ext_addr <= 0xDFFF) {
      // Main ram
      //uint8_t d = ram[ext_addr - 0xC000];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else if (0xE000 <= ext_addr && ext_addr <= 0xFFFF) {
      // Echo ram
      //uint8_t d = ram[ext_addr - 0xE000];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else {
      printf("Bad address?\n");
      __debugbreak();
    }
  }
  else {
    preset_d(false, 0);
  }
}
#endif





#if 0
inline void dump_pins(TextPainter& text_painter, const PinOut& a, const PinOut& d) {
  dump_pin(text_painter, a.a, d.a);
}

inline void dump_pins(TextPainter& text_painter, const PinOut& a, const PinOut& b, const PinIn& c, const PinOut& d) {
  dump_pin(text_painter, a.a, b.a, c.a, d.a);
}

void dump_regs(TextPainter& /*text_painter*/) {
  /*
  text_painter.dprintf(" ----- EXT_BUS -----\n");

  text_painter.dprintf("ADDR_LATCH ");
  dump2(text_painter, EXT_ADDR_LATCH_14.a);
  dump2(text_painter, EXT_ADDR_LATCH_13.a);
  dump2(text_painter, EXT_ADDR_LATCH_12.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_11.a);
  dump2(text_painter, EXT_ADDR_LATCH_10.a);
  dump2(text_painter, EXT_ADDR_LATCH_09.a);
  dump2(text_painter, EXT_ADDR_LATCH_08.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_07.a);
  dump2(text_painter, EXT_ADDR_LATCH_06.a);
  dump2(text_painter, EXT_ADDR_LATCH_05.a);
  dump2(text_painter, EXT_ADDR_LATCH_04.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_03.a);
  dump2(text_painter, EXT_ADDR_LATCH_02.a);
  dump2(text_painter, EXT_ADDR_LATCH_01.a);
  dump2(text_painter, EXT_ADDR_LATCH_00.a);
  text_painter.newline();

  text_painter.dprintf("DATA_LATCH ");
  dump2(text_painter, CART_DATA_LATCH_07.a);
  dump2(text_painter, CART_DATA_LATCH_06.a);
  dump2(text_painter, CART_DATA_LATCH_05.a);
  dump2(text_painter, CART_DATA_LATCH_04.a);
  text_painter.add_char(':');
  dump2(text_painter, CART_DATA_LATCH_03.a);
  dump2(text_painter, CART_DATA_LATCH_02.a);
  dump2(text_painter, CART_DATA_LATCH_01.a);
  dump2(text_painter, CART_DATA_LATCH_00.a);
  text_painter.newline();
  */
}

void dump_pins(TextPainter& /*text_painter*/) {
  /*
  text_painter.dprintf("----- EXT_PINS -----\n");

  text_painter.dprintf("PIN_PHI %d\n", PIN_PHI.a.val);
  text_painter.dprintf("WR  %d\n", PIN_WR_A.a.val);
  text_painter.dprintf("RD  %d\n", PIN_RD_A.a.val);
  text_painter.dprintf("CS  %d\n", PIN_CS_A.a.val);

  text_painter.add_text("Axx ");
  dump_pins(text_painter, PIN_A15_A, PIN_A15_D);
  dump_pins(text_painter, PIN_A14_A, PIN_A14_D);
  dump_pins(text_painter, PIN_A13_A, PIN_A13_D);
  dump_pins(text_painter, PIN_A12_A, PIN_A12_D);
  text_painter.add_char(':');
  dump_pins(text_painter, PIN_A11_A, PIN_A11_D);
  dump_pins(text_painter, PIN_A10_A, PIN_A10_D);
  dump_pins(text_painter, PIN_A09_A, PIN_A09_D);
  dump_pins(text_painter, PIN_A08_A, PIN_A08_D);
  text_painter.add_char(':');
  dump_pins(text_painter, PIN_A07_A, PIN_A07_D);
  dump_pins(text_painter, PIN_A06_A, PIN_A06_D);
  dump_pins(text_painter, PIN_A05_A, PIN_A05_D);
  dump_pins(text_painter, PIN_A04_A, PIN_A04_D);
  text_painter.add_char(':');
  dump_pins(text_painter, PIN_A03_A, PIN_A03_D);
  dump_pins(text_painter, PIN_A02_A, PIN_A02_D);
  dump_pins(text_painter, PIN_A01_A, PIN_A01_D);
  dump_pins(text_painter, PIN_A00_A, PIN_A00_D);
  text_painter.newline();

  text_painter.add_text("Dxx ");
  dump_pins(text_painter, PIN_D7_A, PIN_D7_B, PIN_D7_C, PIN_D7_D);
  dump_pins(text_painter, PIN_D6_A, PIN_D6_B, PIN_D6_C, PIN_D6_D);
  dump_pins(text_painter, PIN_D5_A, PIN_D5_B, PIN_D5_C, PIN_D5_D);
  dump_pins(text_painter, PIN_D4_A, PIN_D4_B, PIN_D4_C, PIN_D4_D);
  dump_pins(text_painter, PIN_D3_A, PIN_D3_B, PIN_D3_C, PIN_D3_D);
  dump_pins(text_painter, PIN_D2_A, PIN_D2_B, PIN_D2_C, PIN_D2_D);
  dump_pins(text_painter, PIN_D1_A, PIN_D1_B, PIN_D1_C, PIN_D1_D);
  dump_pins(text_painter, PIN_D0_A, PIN_D0_B, PIN_D0_C, PIN_D0_D);
  text_painter.newline();
  */
}

uint16_t get_addr() const {
  uint16_t ext_addr = (uint16_t)pack(
    PIN_A00_A, PIN_A01_A, PIN_A02_A, PIN_A03_A,
    PIN_A04_A, PIN_A05_A, PIN_A06_A, PIN_A07_A,
    PIN_A08_A, PIN_A09_A, PIN_A10_A, PIN_A11_A,
    PIN_A12_A, PIN_A13_A, PIN_A14_A, PIN_A15_A);

  return ext_addr;
}

/*
void dump_pins(TextPainter& text_painter) {
text_painter.dprintf("----- SYS_PINS -----\n");
text_painter.dprintf("PIN_RST      %d\n", PIN_RST.a.val);
text_painter.dprintf("PIN_CLK_GOOD %d\n", PIN_CLK_GOOD.a.val);
text_painter.dprintf("PIN_CLK_IN_xBxDxFxH   %d\n", PIN_CLK_IN_xBxDxFxH.a.val);
text_painter.dprintf("PIN_T1       %d\n", PIN_T1.a.val);
text_painter.dprintf("PIN_T2       %d\n", PIN_T2.a.val);
text_painter.newline();
}
*/

#endif