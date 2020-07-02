#include "Sch_ExtBus.h"

#include "TestGB.h"

using namespace Schematics;

//------------------------------------------------------------------------------

bool ExtPinsIn::commit() {
  bool changed = false;
  /* PIN_71 */ changed |= PIN_RST.clear_preset();
  /* PIN_72 */ /*GND*/
  /* PIN_73 */ /*CLKOUT*/
  /* PIN_74 */ changed |= PIN_CLK_GOOD.clear_preset();
  /* PIN_74 */ changed |= PIN_CLK_IN_xBxDxFxH.clear_preset();

  /* PIN_76 */ changed |= PIN_T2.clear_preset();
  /* PIN_77 */ changed |= PIN_T1.clear_preset();
  /* PIN_78 */ changed |= PIN_WR_C.clear_preset();     // -> UBAL
  /* PIN_79 */ changed |= PIN_RD_C.clear_preset();     // -> UJYV

  /* PIN_01 */ changed |= PIN_A00_C.clear_preset();      // -> KOVA
  /* PIN_02 */ changed |= PIN_A01_C.clear_preset();      // -> CAMU
  /* PIN_03 */ changed |= PIN_A02_C.clear_preset();      // -> BUXU
  /* PIN_04 */ changed |= PIN_A03_C.clear_preset();      // -> BASE
  /* PIN_05 */ changed |= PIN_A04_C.clear_preset();      // -> AFEC
  /* PIN_06 */ changed |= PIN_A05_C.clear_preset();      // -> ABUP
  /* PIN_07 */ changed |= PIN_A06_C.clear_preset();      // -> CYGU
  /* PIN_08 */ changed |= PIN_A07_C.clear_preset();      // -> COGO
  /* PIN_09 */ changed |= PIN_A08_C.clear_preset();      // -> MUJY
  /* PIN_10 */ changed |= PIN_A09_C.clear_preset();      // -> NENA
  /* PIN_11 */ changed |= PIN_A10_C.clear_preset();      // -> SURA
  /* PIN_12 */ changed |= PIN_A11_C.clear_preset();      // -> MADY
  /* PIN_13 */ changed |= PIN_A12_C.clear_preset();      // -> LAHE
  /* PIN_14 */ changed |= PIN_A13_C.clear_preset();      // -> LURA
  /* PIN_15 */ changed |= PIN_A14_C.clear_preset();      // -> PEVO
  /* PIN_16 */ changed |= PIN_A15_C.clear_preset();      // -> RAZA

  /* PIN_17 */ changed |= PIN_D0_C.clear_preset();       // -> TOVO,SOMA
  /* PIN_18 */ changed |= PIN_D1_C.clear_preset();       // -> RUZY,RONY
  /* PIN_19 */ changed |= PIN_D2_C.clear_preset();       // -> ROME,RAXY
  /* PIN_20 */ changed |= PIN_D3_C.clear_preset();       // -> SAZA,SELO
  /* PIN_21 */ changed |= PIN_D4_C.clear_preset();       // -> TEHE,SODY
  /* PIN_22 */ changed |= PIN_D5_C.clear_preset();       // -> RATU,SAGO
  /* PIN_23 */ changed |= PIN_D6_C.clear_preset();       // -> SOCA,RUPA
  /* PIN_24 */ changed |= PIN_D7_C.clear_preset();       // -> RYBA,SAZY
  return changed;
}

//------------------------------------------------------------------------------

void ExtPinsOut::tick(TestGB& gb) {
  {
    auto cpu_sig = gb.cpu_bus.sig(gb);
    auto& cpu_bus = gb.cpu_bus;
    auto& ext_pins_in = gb.ext_pins_in;

    // External data bus to internal data bus
    // SOMA01 << LAVO04
    // SOMA02 nc
    // SOMA03 << D0_C
    // SOMA04 nc
    // SOMA05 nc
    // SOMA06 nc
    // SOMA07 >> RYMA04
    // SOMA08 nc
    // SOMA09 == nc

    // Is this actually like a pass gate? We already know the latch cells, and this is bigger than those.

    /*p08.SOMA*/ SOMA_CPU_DATA_LATCH_00.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, ext_pins_in.PIN_D0_C);
    /*p08.RONY*/ RONY_CPU_DATA_LATCH_01.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, ext_pins_in.PIN_D1_C);
    /*p08.RAXY*/ RAXY_CPU_DATA_LATCH_02.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, ext_pins_in.PIN_D2_C);
    /*p08.SELO*/ SELO_CPU_DATA_LATCH_03.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, ext_pins_in.PIN_D3_C);
    /*p08.SODY*/ SODY_CPU_DATA_LATCH_04.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, ext_pins_in.PIN_D4_C);
    /*p08.SAGO*/ SAGO_CPU_DATA_LATCH_05.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, ext_pins_in.PIN_D5_C);
    /*p08.RUPA*/ RUPA_CPU_DATA_LATCH_06.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, ext_pins_in.PIN_D6_C);
    /*p08.SAZY*/ SAZY_CPU_DATA_LATCH_07.tp_latch(cpu_sig.LAVO_LATCH_CPU_DATAp, ext_pins_in.PIN_D7_C);

    // RYMA 6-rung green tribuf

    /*p08.RYMA*/ cpu_bus.TRI_D0.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, SOMA_CPU_DATA_LATCH_00);
    /*p08.RUVO*/ cpu_bus.TRI_D1.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, RONY_CPU_DATA_LATCH_01);
    /*p08.RYKO*/ cpu_bus.TRI_D2.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, RAXY_CPU_DATA_LATCH_02);
    /*p08.TAVO*/ cpu_bus.TRI_D3.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, SELO_CPU_DATA_LATCH_03);
    /*p08.TEPE*/ cpu_bus.TRI_D4.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, SODY_CPU_DATA_LATCH_04);
    /*p08.SAFO*/ cpu_bus.TRI_D5.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, SAGO_CPU_DATA_LATCH_05);
    /*p08.SEVU*/ cpu_bus.TRI_D6.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, RUPA_CPU_DATA_LATCH_06);
    /*p08.TAJU*/ cpu_bus.TRI_D7.set_tribuf(cpu_sig.LAVO_LATCH_CPU_DATAp, SAZY_CPU_DATA_LATCH_07);
  }
}

//------------------------------------------------------------------------------

bool ExtPinsOut::commit() {
  bool changed = false;

  /*p08.SOMA*/ changed |= SOMA_CPU_DATA_LATCH_00.commit_latch();
  /*p08.RONY*/ changed |= RONY_CPU_DATA_LATCH_01.commit_latch();
  /*p08.RAXY*/ changed |= RAXY_CPU_DATA_LATCH_02.commit_latch();
  /*p08.SELO*/ changed |= SELO_CPU_DATA_LATCH_03.commit_latch();
  /*p08.SODY*/ changed |= SODY_CPU_DATA_LATCH_04.commit_latch();
  /*p08.SAGO*/ changed |= SAGO_CPU_DATA_LATCH_05.commit_latch();
  /*p08.RUPA*/ changed |= RUPA_CPU_DATA_LATCH_06.commit_latch();
  /*p08.SAZY*/ changed |= SAZY_CPU_DATA_LATCH_07.commit_latch();

  return changed;
}

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
PIN_WR_C.preset(true, 0);   // -> P07.UBAL
PIN_RD_C.preset(true, 0);   // -> P07.UJYV
PIN_A00_C.preset(true, 0);   // -> P08.KOVA
PIN_A01_C.preset(true, 0);   // -> P08.CAMU
PIN_A02_C.preset(true, 0);   // -> P08.BUXU
PIN_A03_C.preset(true, 0);   // -> P08.BASE
PIN_A04_C.preset(true, 0);   // -> P08.AFEC
PIN_A05_C.preset(true, 0);   // -> P08.ABUP
PIN_A06_C.preset(true, 0);   // -> P08.CYGU
PIN_A07_C.preset(true, 0);   // -> P08.COGO
PIN_A08_C.preset(true, 0);   // -> P08.MUJY
PIN_A09_C.preset(true, 0);   // -> P08.NENA
PIN_A10_C.preset(true, 0);   // -> P08.SURA
PIN_A11_C.preset(true, 0);   // -> P08.MADY
PIN_A12_C.preset(true, 0);   // -> P08.LAHE
PIN_A13_C.preset(true, 0);   // -> P08.LURA
PIN_A14_C.preset(true, 0);   // -> P08.PEVO
PIN_A15_C.preset(true, 0);   // -> P08.RAZA

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
// so the address bus is technically a tribuf, but we're going to ignore
// this debug circuit for now.
{
  // If we're in debug mode 2, drive external address bus onto internal address
  // these may be backwards, probably don't want to drive external address onto bus normally...

  /*p08.KOVA*/ wire A00_Cn = not(PIN_A00_C);
  /*p08.CAMU*/ wire A01_Cn = not(PIN_A01_C);
  /*p08.BUXU*/ wire A02_Cn = not(PIN_A02_C);
  /*p08.BASE*/ wire A03_Cn = not(PIN_A03_C);
  /*p08.AFEC*/ wire A04_Cn = not(PIN_A04_C);
  /*p08.ABUP*/ wire A05_Cn = not(PIN_A05_C);
  /*p08.CYGU*/ wire A06_Cn = not(PIN_A06_C);
  /*p08.COGO*/ wire A07_Cn = not(PIN_A07_C);
  /*p08.MUJY*/ wire A08_Cn = not(PIN_A08_C);
  /*p08.NENA*/ wire A09_Cn = not(PIN_A09_C);
  /*p08.SURA*/ wire A10_Cn = not(PIN_A10_C);
  /*p08.MADY*/ wire A11_Cn = not(PIN_A11_C);
  /*p08.LAHE*/ wire A12_Cn = not(PIN_A12_C);
  /*p08.LURA*/ wire A13_Cn = not(PIN_A13_C);
  /*p08.PEVO*/ wire A14_Cn = not(PIN_A14_C);
  /*p08.RAZA*/ wire A15_Cn = not(PIN_A15_C);

  /*p08.KEJO*/ cpu_bus.PIN_A00.set(!TOVA_MODE_DBG2n, not(A00_Cn));
  /*p08.BYXE*/ cpu_bus.PIN_A01.set(!TOVA_MODE_DBG2n, not(A01_Cn));
  /*p08.AKAN*/ cpu_bus.PIN_A02.set(!TOVA_MODE_DBG2n, not(A02_Cn));
  /*p08.ANAR*/ cpu_bus.PIN_A03.set(!TOVA_MODE_DBG2n, not(A03_Cn));
  /*p08.AZUV*/ cpu_bus.PIN_A04.set(!TOVA_MODE_DBG2n, not(A04_Cn));
  /*p08.AJOV*/ cpu_bus.PIN_A05.set(!TOVA_MODE_DBG2n, not(A05_Cn));
  /*p08.BYNE*/ cpu_bus.PIN_A06.set(!TOVA_MODE_DBG2n, not(A06_Cn));
  /*p08.BYNA*/ cpu_bus.PIN_A07.set(!TOVA_MODE_DBG2n, not(A07_Cn));
  /*p08.LOFA*/ cpu_bus.PIN_A08.set(!TOVA_MODE_DBG2n, not(A08_Cn));
  /*p08.MAPU*/ cpu_bus.PIN_A09.set(!TOVA_MODE_DBG2n, not(A09_Cn));
  /*p08.RALA*/ cpu_bus.PIN_A10.set(!TOVA_MODE_DBG2n, not(A10_Cn));
  /*p08.LORA*/ cpu_bus.PIN_A11.set(!TOVA_MODE_DBG2n, not(A11_Cn));
  /*p08.LYNA*/ cpu_bus.PIN_A12.set(!TOVA_MODE_DBG2n, not(A12_Cn));
  /*p08.LEFY*/ cpu_bus.PIN_A13.set(!TOVA_MODE_DBG2n, not(A13_Cn));
  /*p08.NEFE*/ cpu_bus.PIN_A14.set(!TOVA_MODE_DBG2n, not(A14_Cn));
  /*p08.SYZU*/ cpu_bus.PIN_A15.set(!TOVA_MODE_DBG2n, not(A15_Cn));
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