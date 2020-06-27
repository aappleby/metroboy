#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

/*
0b000xxxxx_xxxxxxxx - 0x0000 to 0x1FFF (cart rom)
0b001xxxxx_xxxxxxxx - 0x2000 to 0x3FFF (cart rom)
0b010xxxxx_xxxxxxxx - 0x4000 to 0x5FFF (cart rom banked)
0b011xxxxx_xxxxxxxx - 0x6000 to 0x7FFF (cart rom banked)
0b100xxxxx_xxxxxxxx - 0x8000 to 0x9FFF (vram)
0b101xxxxx_xxxxxxxx - 0xA000 to 0xBFFF (cart ram)

0b110xxxxx_xxxxxxxx - 0xC000 to 0xDFFF (internal ram)
0b111xxxxx_xxxxxxxx - 0xE000 to 0xFFFF (echo ram, oam, high ram, io)

0b11111110_00000000 - 0xFE00 - OAM begin
0b11111110_10011111 - 0xFE9F - OAM end

0b11111111_00000000 - 0xFF00 - IO begin
0b11111111_01111111 - 0xFF7F - IO end

0b11111111_10000000 - 0xFF80 - Zeropage begin
0b11111111_11111110 - 0xFFFE - Zeropage end
0b11111111_11111111 - 0xFFFF - Interrupt enable
*/

TestGB gb;

//-----------------------------------------------------------------------------

#if 0

//Die trace:
ABOL = not(PIN_CLKREQ)
ATEZ = not(CLKIN_A)

ADAR = not(ADYK.qn)
ATYP = not(AFUR.q);
AROV = not(APUK.qn);
AFEP = not(ALEF.qn);

AFAS = nor(ADAR, ATYP)
AREV = nand(AFAS, PIN_CPU_RAW_WR)
APOV = not(AREV) // 4 rung inverter

AGUT = ? ? ? (AROV, AJAX) - gate unused ?
AJAX = not(ATYP)
ALUR = not(AVOR) // 3 rung inverter
AVOR = or(PIN_AFER, ASOL)

PIN_AFER = reg, gap above it or something.starting at the first connected rung

// Because this has BOGA and BOMA both as inputs and two UPOJ inputs, maybe
// it's supposed to be a clock-crossing synchronizer?

AFER00 << UPOJ
AFER01 << ASOL
AFER02 nc
AFER03 << BOGA
AFER04 nc
AFER05 nc
AFER06 << BOMA
AFER06 << UPOJ
AFER07 nc
AFER08 nc
AFER09 >> nc     // QN
AFER10 >> AVOR   // Q

ASOL = nor_latch(<< AFAR, nc, >> AVOR, nc, nc, << PIN_RST) - output inverted

AFAR = nor(PIN_RST, ALYP)
ALYP = not(TABA)
ADAR = not(ADYK)


// Hax

/*p01.ADYK*/ clk_reg.ADYK_PHAZ_xxxxEFGH
/*p01.AFUR*/ clk_reg.AFUR_PHAZ_xBCDExxx

ABOL = not(PIN_CLKREQ)
ATEZ = not(CLKIN_A)
APOV = and(ADYK_xxxxEFGH, AFUR_xBCDExxx, PIN_CPU_RAW_WR) // cpu write is _only_ on E?

ABUZ = or(UNOR, !CPU_ADDR_VALID)

AGUT = ? ? ? (AROV, AJAX) - gate unused ?
AJAX = AFUR
ALUR = nor(AFER_RST, ASOL_RST_LATCH)



#endif

void TestGB::tick_everything() {
  auto clk_sig = clk_reg.sig(*this);

  clk_reg.tick(*this);
  dma_reg.tick(*this);
  tim_reg.tick(*this);
  ser_reg.tick(*this);
  joy_reg.tick(*this);
  ppu_reg.tick(*this);
  sst_reg.tick(*this);
  lcd_reg.tick(*this);
  pxp_reg.tick(*this);
  cpu_reg.tick(*this);
  vram_pins.tick(*this);
  cart_reg.tick(*this);
}

//-----------------------------------------------------------------------------
