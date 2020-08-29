#include "GateBoyTests/GateBoyTests.h"
#include "GateBoyLib/GateBoy.h"
#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "CoreLib/File.h"
#include <chrono>
#include <stddef.h>

//-----------------------------------------------------------------------------

int GateBoyTests::test_main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  TEST_START("Maaaaaain");

  err += test_init();
  err += test_clk();
  err += test_ext_bus();
  err += test_mem();
  err += test_interrupts();
  err += test_bootrom();
  err += test_dma();
  err += test_joypad();
  err += test_ppu();
  err += test_serial();
  err += test_timer();

  if (!err) LOG_G("Everything passed!\n");

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_init() {
  TEST_START("Init");

  GateBoy gb;
  gb.reset();

  uint64_t top_hash = hash(&gb.top, sizeof(gb.top));
  LOG_Y("Top hash after reset is 0x%016llx\n", top_hash);
  EXPECT_EQ(0x109151db44405d2c, top_hash, "Top hash mismatch");

  uint8_t* cursor = (uint8_t*)(&gb.top);

  // All regs should have no delta
  for (int i = 0; i < sizeof(gb.top); i++) {
    uint8_t d = *cursor++;
    ASSERT_EQ(0, (d & 0xF0));
  }

  // Mem should be clear
  for (int i = 0; i < 32768; i++) ASSERT_EQ(0, gb.cart_rom[i]);
  for (int i = 0; i < 8192; i++)  ASSERT_EQ(0, gb.cart_ram[i]);
  for (int i = 0; i < 8192; i++)  ASSERT_EQ(0, gb.ext_ram[i]);

  // Framebuffer should be 0x04 (yellow)
  for (int i = 0; i < 160*144; i++) {
    ASSERT_EQ(4, gb.fb[i]);
  }

  EXPECT_EQ(0xCF, gb.dbg_read(ADDR_P1),   "Bad P1 reset value");
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_SB),   "Bad SB reset value");
  EXPECT_EQ(0xFC, gb.dbg_read(ADDR_SC),   "Bad SC reset value"); // double-check this

  EXPECT_EQ(0x00, gb.dbg_read(ADDR_TIMA), "Bad TIMA reset value");
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_TMA),  "Bad TMA reset value");
  EXPECT_EQ(0xF8, gb.dbg_read(ADDR_TAC),  "Bad TAC reset value");

  EXPECT_EQ(0x00, gb.dbg_read(ADDR_SCY),  "Bad SCY reset value");
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_SCX),  "Bad SCX reset value");
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_LY),   "Bad LY reset value");
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_LYC),  "Bad LYC reset value");
  EXPECT_EQ(0xFF, gb.dbg_read(ADDR_DMA),  "Bad DMA reset value"); // double-check this
  EXPECT_EQ(0xFF, gb.dbg_read(ADDR_BGP),  "Bad BGP reset value");
  EXPECT_EQ(0xFF, gb.dbg_read(ADDR_OBP0), "Bad OBP0 reset value");
  EXPECT_EQ(0xFF, gb.dbg_read(ADDR_OBP1), "Bad OBP1 reset value");
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_WY),   "Bad WY reset value");
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_WX),   "Bad WX reset value");

  TEST_END();
}

//-----------------------------------------------------------------------------

#define EXPECT_CLK(A, B) EXPECT_EQ(wire(A), wire((B) & (1 << (7 - phase))), "Clock phase mismatch, %s at phase %d", #A, phase);

int GateBoyTests::test_clk() {
  TEST_START();

  GateBoy gb;
  gb.reset();

  auto& top = gb.top;
  auto& clk_reg = top.clk_reg;

  for (int i = 0; i < 32; i++) {
    int phase = gb.phase_total & 7;
    //wire CLK = phase & 1;
    //wire CLKGOOD = 1;

    EXPECT_CLK(clk_reg.ZAXY_xBxDxFxH,   0b01010101);
    
    EXPECT_CLK(clk_reg.AFUR_xxxxEFGH.qp(), 0b00001111);
    EXPECT_CLK(clk_reg.ALEF_AxxxxFGH.qp(), 0b10000111);
    EXPECT_CLK(clk_reg.APUK_ABxxxxGH.qp(), 0b11000011);
    EXPECT_CLK(clk_reg.ADYK_ABCxxxxH.qp(), 0b11100001);

    EXPECT_CLK(clk_reg.AROV_xxCDEFxx,   0b00111100);
    EXPECT_CLK(clk_reg.ATYP_ABCDxxxx,   0b11110000);
    EXPECT_CLK(clk_reg.AJAX_xxxxEFGH,   0b00001111);
    EXPECT_CLK(clk_reg.ADAR_ABCxxxxH,   0b11100001);
    EXPECT_CLK(clk_reg.BUDE_xxxxEFGH,   0b00001111);
    EXPECT_CLK(clk_reg.BALY_Axxxxxxx,   0b10000000);

    EXPECT_CLK(top.cpu_bus.CPU_PIN_BOWA_xBCDEFGH.tp(), 0b01111111);
    EXPECT_CLK(top.cpu_bus.CPU_PIN_BEDO_Axxxxxxx.tp(), 0b10000000);
    EXPECT_CLK(top.cpu_bus.CPU_PIN_BEKO_ABCDxxxx.tp(), 0b11110000);
    EXPECT_CLK(top.cpu_bus.CPU_PIN_BUDE_xxxxEFGH.tp(), 0b00001111);
    EXPECT_CLK(top.cpu_bus.CPU_PIN_BOLO_ABCDEFxx.tp(), 0b11111100);
    EXPECT_CLK(top.cpu_bus.CPU_PIN_BUKE_AxxxxxGH.tp(), 0b10000011);
    EXPECT_CLK(top.cpu_bus.CPU_PIN_BOMA_Axxxxxxx.tp(), 0b10000000);
    EXPECT_CLK(top.cpu_bus.CPU_PIN_BOGA_xBCDEFGH.tp(), 0b01111111);
    EXPECT_CLK(top.ext_bus.EXT_PIN_CLK.qp(),           0b11110000);
    gb.next_phase();
  }

  TEST_END();
}

//-----------------------------------------------------------------------------

/*
Code in ROM, write 0x55 (0b01010101) to 0xC003 (0b1100000000000011)
0x0155 0x77 ld (hl), A   : 2 mcycles, 16 phases
0x0156 0x18 jr           : 3 mcycles, 24 phases
0x0157 0xFD -2

CLK ABCDxxxx ABCDxxxx ABCDxxxx ABCDxxxx ABCDxxxx
WRn ABCDEFGH ABCDxxxH ABCDEFGH ABCDEFGH ABCDEFGH
RDn xxxxxxxx xBCDEFGH xxxxxxxx xxxxxxxx xxxxxxxx
CSn ABCDEFGH ABxxxxxx ABCDEFGH ABCDEFGH ABCDEFGH
              
     05       03       06       07       07
A00 ABCDEFGH ABCDEFGH Axxxxxxx xBCDEFGH ABCDEFGH
A01 Axxxxxxx xBCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH
A02 ABCDEFGH Axxxxxxx xBCDEFGH ABCDEFGH ABCDEFGH
A03 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
      
     00       C0       00       00       80
A12 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
A13 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
A14 xxxxxxxx xBCDEFGH xxxxxxxx xxxxxxxx xxxxxxxx
A15 ABxxxxxx ABCDEFGH ABxxxxxx ABxxxxxx ABCDEFGH

    ^^0x77-- -^^^0x55 ^^0x18-- --0xFD-- -----^^^
D0  ABCDEFGH ABCDEFGH ABxxxxxx xxCDEFGH ABCDEFGH
D1  ABCDEFGH ABCDxxxx ABxxxxxx xxxxxxxx xxxxxFGH
D2  ABCDEFGH ABCDEFGH ABxxxxxx xxCDEFGH ABCDEFGH
D3  ABxxxxxx xBCDxxxx ABCDEFGH ABCDEFGH ABCDEFGH
D4  ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH ABCDEFGH
D5  ABCDEFGH ABCDxxxx ABxxxxxx xxCDEFGH ABCDEFGH
D6  ABCDEFGH ABCDEFGH ABxxxxxx xxCDEFGH ABCDEFGH
D7  ABxxxxxx xBCDxxxx ABxxxxxx xxCDEFGH ABCDEFGH
*/

#pragma warning(disable : 4189) // unref var

int GateBoyTests::test_ext_bus() {
  TEST_START();
#if 0
  // The low bits of the address bus _must_ change on phase B.
  {
    LOG_Y("Testing when bus changes take effect\n");
    GateBoy gb;
    gb.reset();
    gb.set_boot_bit();

    // Preset address to 0xFF and run for a mcycle.
    gb.dbg_req = { .addr = 0x00FF, .data = 0x00, .read = 1, .write = 0 };
    gb.run(8);

    // We should see the address on the bus
    ASSERT_EQ(gb.phase_total & 7, 0);
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x80FF, "Phase A fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x80FF, "Phase B fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase C fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase D fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase E fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase F fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase G fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase H fail"); gb.next_phase();

    // Change the debug request in state B. It should _not_ take effect yet.
    ASSERT_EQ(gb.phase_total & 7, 0);
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x80FF, "Phase A fail"); gb.next_phase();
    gb.dbg_req = { .addr = 0x0000, .data = 0x00, .read = 1, .write = 0 };
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x80FF, "Phase B fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase C fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase D fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase E fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase F fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase G fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase H fail"); gb.next_phase();

    // Now we're back at phase A again.
    // The address bus change should take effect on state B
    ASSERT_EQ(gb.phase_total & 7, 0);
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x80FF, "Phase A fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x8000, "Phase B fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x0000, "Phase C fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x0000, "Phase D fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x0000, "Phase E fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x0000, "Phase F fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x0000, "Phase G fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x0000, "Phase H fail"); gb.next_phase();

    // Now change the address back in state A. It should take effect in state B.
    gb.dbg_req = { .addr = 0x00FF, .data = 0x00, .read = 1, .write = 0 };
    ASSERT_EQ(gb.phase_total & 7, 0);
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x8000, "Phase A fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x80FF, "Phase B fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase C fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase D fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase E fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase F fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase G fail"); gb.next_phase();
    ASSERT_EQ(gb.top.ext_bus.get_pin_addr(), 0x00FF, "Phase H fail"); gb.next_phase();
  }
#endif

  // Check all signals for all phases of "ld (hl), a; jr -2;" with hl = 0xC003 and a = 0x55
#if 0
  {
    LOG_Y("Testing \"ld (hl), a; jr -2;\" the hard way\n");

    GateBoy gb;
    gb.reset();
    gb.set_boot_bit();

    Req* script = new Req[]{
      { .addr = 0x0155, .data = 0x00, .read = 1, .write = 0}, // read "ld (hl), a" opcode
      { .addr = 0xC003, .data = 0x00, .read = 0, .write = 1}, // write to 0xC003
      { .addr = 0x0156, .data = 0x00, .read = 1, .write = 0}, // read "jr -2" opcode
      { .addr = 0x0157, .data = 0x00, .read = 1, .write = 0}, // read "jr -2" param
      { .addr = 0x0157, .data = 0x00, .read = 0, .write = 0}, // idle cycle
    };

    // Run the script once without checking phases
    gb.dbg_req = script[0]; gb.run(8);
    gb.dbg_req = script[1]; gb.run(8);
    gb.dbg_req = script[2]; gb.run(8);
    gb.dbg_req = script[3]; gb.run(8);
    gb.dbg_req = script[4]; gb.run(8);

    // Start checking each phase

    const char* CLK_WAVE = "ABCDxxxxABCDxxxxABCDxxxxABCDxxxxABCDxxxx";
    const char* WRn_WAVE = "ABCDEFGHABCDxxxHABCDEFGHABCDEFGHABCDEFGH";
    const char* RDn_WAVE = "xxxxxxxxxBCDEFGHxxxxxxxxxxxxxxxxxxxxxxxx";
    const char* CSn_WAVE = "ABCDEFGHABxxxxxxABCDEFGHABCDEFGHABCDEFGH";

    const char* A00_WAVE = "11111111" "11111111" "10000000" "01111111" "11111111";
    const char* A01_WAVE = "10000000" "01111111" "11111111" "11111111" "11111111";
    const char* A02_WAVE = "11111111" "10000000" "01111111" "11111111" "11111111";
    const char* A03_WAVE = "00000000" "00000000" "00000000" "00000000" "00000000";
    const char* A12_WAVE = "00000000" "00000000" "00000000" "00000000" "00000000";
    const char* A13_WAVE = "00000000" "00000000" "00000000" "00000000" "00000000";
    const char* A14_WAVE = "00000000" "01111111" "00000000" "00000000" "00000000";
    const char* A15_WAVE = "11000000" "11111111" "11000000" "11000000" "11111111";

    /*
    const char* D00_WAVE = "ABCDEFGHABCDEFGHABxxxxxxxxCDEFGHABCDEFGH";
    const char* D01_WAVE = "ABCDEFGHABCDxxxxABxxxxxxxxxxxxxxxxxxxFGH";
    const char* D02_WAVE = "ABCDEFGHABCDEFGHABxxxxxxxxCDEFGHABCDEFGH";
    const char* D03_WAVE = "ABxxxxxxxBCDxxxxABCDEFGHABCDEFGHABCDEFGH";
    const char* D04_WAVE = "ABCDEFGHABCDEFGHABCDEFGHABCDEFGHABCDEFGH";
    const char* D05_WAVE = "ABCDEFGHABCDxxxxABxxxxxxxxCDEFGHABCDEFGH";
    const char* D06_WAVE = "ABCDEFGHABCDEFGHABxxxxxxxxCDEFGHABCDEFGH";
    const char* D07_WAVE = "ABxxxxxxxBCDxxxxABxxxxxxxxCDEFGHABCDEFGH";
    */

    //                     "AB      AB      AB      AB      AB      ";
    const char* D00_WAVE = "HHHHHHHHHHHHHHHHHHxxxxxx^^HHHHHHHHHHHHHH";
    const char* D01_WAVE = "^^HHHHHHHHHHxxxxHHxxxxxx^^xxxxxx^^^^^^^^";
    const char* D02_WAVE = "HHHHHHHHHHHHHHHHHHxxxxxx^^HHHHHHHHHHHHHH";
    const char* D03_WAVE = "HHxxxxxx^HHHxxxx??HHHHHHHHHHHHHHHHHHHHHH";
    const char* D04_WAVE = "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
    const char* D05_WAVE = "HHHHHHHHHHHHxxxx??xxxxxx^^HHHHHHHHHHHHHH";
    const char* D06_WAVE = "HHHHHHHHHHHHHHHHHHxxxxxx^^HHHHHHHHHHHHHH";
    const char* D07_WAVE = "HHxxxxxx^HHHxxxx??xxxxxx^^HHHHHHHHHHHHHH";

    for (int i = 0; i < 40; i++) {
      if ((i % 8) == 0) gb.dbg_req = script[i / 8];

      wire CLK = gb.top.ext_bus.EXT_PIN_CLK.qp();
      wire WRn = gb.top.ext_bus.EXT_PIN_WRn.qp();
      wire RDn = gb.top.ext_bus.EXT_PIN_RDn.qp();
      wire CSn = gb.top.ext_bus.EXT_PIN_CSn.qp();

      char A00 = gb.top.ext_bus.EXT_PIN_A00p.c();
      char A01 = gb.top.ext_bus.EXT_PIN_A01p.c();
      char A02 = gb.top.ext_bus.EXT_PIN_A02p.c();
      char A03 = gb.top.ext_bus.EXT_PIN_A03p.c();
      char A12 = gb.top.ext_bus.EXT_PIN_A12p.c();
      char A13 = gb.top.ext_bus.EXT_PIN_A13p.c();
      char A14 = gb.top.ext_bus.EXT_PIN_A14p.c();
      char A15 = gb.top.ext_bus.EXT_PIN_A15p.c();

      wire D00 = gb.top.ext_bus.EXT_PIN_D00p.qp();
      wire D01 = gb.top.ext_bus.EXT_PIN_D01p.qp();
      wire D02 = gb.top.ext_bus.EXT_PIN_D02p.qp();
      wire D03 = gb.top.ext_bus.EXT_PIN_D03p.qp();
      wire D04 = gb.top.ext_bus.EXT_PIN_D04p.qp();
      wire D05 = gb.top.ext_bus.EXT_PIN_D05p.qp();
      wire D06 = gb.top.ext_bus.EXT_PIN_D06p.qp();
      wire D07 = gb.top.ext_bus.EXT_PIN_D07p.qp();

      ASSERT_EQ(CLK, CLK_WAVE[i] != 'x', "CLK failure at phase %d\n", i);
      ASSERT_EQ(WRn, WRn_WAVE[i] != 'x', "WRn failure at phase %d\n", i);
      //ASSERT_EQ(RDn, RDn_WAVE[i] != 'x', "RDn failure at phase %d\n", i); // broken
      ASSERT_EQ(CSn, CSn_WAVE[i] != 'x', "CSn failure at phase %d\n", i);

      ASSERT_EQ(A00, A00_WAVE[i], "A00 failure at phase %d\n", i);

      ASSERT_EQ(A01, A01_WAVE[i], "A01 failure at phase %d\n", i);
      ASSERT_EQ(A02, A02_WAVE[i], "A02 failure at phase %d\n", i);
      ASSERT_EQ(A03, A03_WAVE[i], "A03 failure at phase %d\n", i);
      ASSERT_EQ(A12, A12_WAVE[i], "A12 failure at phase %d\n", i);
      ASSERT_EQ(A13, A13_WAVE[i], "A13 failure at phase %d\n", i);
      //ASSERT_EQ(A14, A14_WAVE[i], "A14 failure at phase %d\n", i);
      ASSERT_EQ(A15, A15_WAVE[i], "A15 failure at phase %d\n", i);

      //ASSERT_EQ(D00, D00_WAVE[i] != 'x', "D00 failure at phase %d\n", i); // broken, fixing it now

      gb.next_phase();
    }
  }
#endif


#if 0
  // A15 should be ABxxxxxx if we're _not_ reading from the ext bus
  {
    GateBoy gb;
    gb.reset();

    gb.dbg_req = { .addr = 0x1000, .data = 0x00, .read = 1, .write = 0 };
    gb.run(8);

    for (int i = 0; i < 32; i++) {
      int phase = i & 7;
      wire A15 = gb.top.ext_bus.EXT_PIN_A15p.qp();

      if (phase == 0 || phase == 1) {
        ASSERT_EQ(1, A15, "A15 not high on phase 0/1");
      }
      else {
        ASSERT_EQ(0, A15, "A15 not low on phase 2/3/4/5/6/7");
      }

      gb.next_phase();
    }
  }

  LOG_G("Pass!\n");
#endif
  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_mem() {
  TEST_START();

  GateBoy gb;
  gb.reset();
  gb.set_boot_bit();

  err += test_mem(gb, "ROM",  0x0000, 0x7FFF, 256, false);
  err += test_mem(gb, "VRAM", 0x8000, 0x9FFF, 256, true);
  err += test_mem(gb, "CRAM", 0xA000, 0xBFFF, 256, true);
  err += test_mem(gb, "IRAM", 0xC000, 0xDFFF, 256, true);
  err += test_mem(gb, "ERAM", 0xE000, 0xFDFF, 256, true);
  err += test_mem(gb, "OAM",  0xFE00, 0xFEFF, 1,   true);
  err += test_mem(gb, "ZRAM", 0xFF80, 0xFFFE, 1,   true);

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_interrupts() {
  TEST_START();

  GateBoy gb;
  gb.reset();

  // hblank no stat int
  // vblank no stat int

  /*
  #define EI_GLITCH    0b10000000
  #define EI_LYC       0b01000000
  #define EI_OAM       0b00100000
  #define EI_VBLANK    0b00010000
  #define EI_HBLANK    0b00001000
  */
  //dbg_write(ADDR_STAT, EI_OAM);
  //dbg_write(ADDR_IE,   0b11111111);

  test_reg("IF",   0xFF0F, 0b00011111); // broken
  //test_reg("IE",   0xFFFF, 0b00011111);

  TEST_END();
}

//------------------------------------------------------------------------------

extern const uint8_t DMG_ROM_bin[];

int GateBoyTests::test_bootrom() {
  TEST_START();

  GateBoy gb;
  gb.reset();

  for (int i = 0; i < 16; i++) {
    uint8_t byte = gb.dbg_read(i);
    EXPECT_EQ(byte, DMG_ROM_bin[i], "bootrom @ 0x%04x = 0x%02x, expected 0x%02x", i, byte, DMG_ROM_bin[i]);
  }

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_timer() {
  TEST_START();

  test_reg("TIMA", ADDR_TIMA, 0b11111111); // works
  test_reg("TMA",  ADDR_TMA,  0b11111111); // works
  test_reg("TAC",  ADDR_TAC,  0b00000111); // works

  // 00 - 2048 phases per TIMA tick (looks ok)
  // 01 - 32 phases per TIMA tick   (looks ok)
  // 10 - 128 phases per TIMA tick  (looks ok)
  // 11 - 512 phases per TIMA tick

  // FF04 DIV
  // FF05 TIMA
  // FF06 TMA
  // FF07 TAC

  {
    //int delay = 0;

    GateBoy gb;
    gb.reset();

    gb.dbg_write(ADDR_TMA, 0x80);
    gb.dbg_write(ADDR_TIMA,0xFD);
    gb.dbg_write(ADDR_DIV, 0x00);

    LOG("DIV  %d\n",     gb.top.tim_reg.get_div());
    LOG("TIMA 0x%02x\n", gb.top.tim_reg.get_tima());
    gb.dbg_write(ADDR_TAC, 0b00000101);
    LOG("DIV  %d\n",     gb.top.tim_reg.get_div());
    LOG("TIMA 0x%02x\n", gb.top.tim_reg.get_tima());
    gb.dbg_write(ADDR_DIV, 0x00);
    LOG("DIV  %d\n",     gb.top.tim_reg.get_div());
    LOG("TIMA 0x%02x\n", gb.top.tim_reg.get_tima());

    /*
    gb.NOP();
    gb.LDH_A8_A(0x06, 0x80);
    gb.NOP();
    gb.LDH_A8_A(0x05, 0xFD);

    // ld a, 0x05
    gb.NOP();
    gb.NOP();

    gb.LDH_A8_A(07, 0x05);
    gb.LDH_A8_A(04, 0x05);
    
    //gb.NOPS(delay);
    */

    //uint8_t a = gb.LDH_A_A8(0x05);
    //LOG("delay %d TIMA 0x%02x\n", delay, a);
  }

  /*
  00:   4096 Hz    (~4194 Hz SGB)
  01: 262144 Hz  (~268400 Hz SGB)
  10:  65536 Hz   (~67110 Hz SGB)
  11:  16384 Hz   (~16780 Hz SGB)
  */

#if 0
  GateBoy gb;
  gb.reset();

  // passes, but slow :/
  LOG("Testing div reset + rollover, this takes a minute...");
  gb.dbg_write(ADDR_DIV, 0);
  for (int i = 1; i < 32768; i++) {
    int div_a = gb.dbg_read(ADDR_DIV);
    int div_b = (i >> 6) & 0xFF;
    EXPECT_EQ(div_a, div_b, "div match fail");
  }
  LOG("\n");
#endif

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_joypad() {
  TEST_START();

  err += test_reg("JOYP", ADDR_P1,   0b00110000);

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_dma() {
  TEST_START();

  GateBoy gb;
  gb.reset();

  for (int src = 0x0000; src < 0xFE00; src += 0x1000) {
    err += test_dma(uint16_t(src));
  }

  TEST_END();
}

//----------------------------------------

uint8_t* get_flat_ptr(GateBoy& gb, uint16_t addr) {
  if (addr >= 0x0000 && addr <= 0x7FFF) {
    return gb.cart_rom + (addr & 0x7FFF);
  }
  else if (addr >= 0x8000 && addr <= 0x9FFF) {
    return gb.vid_ram + (addr & 0x1FFF);
  }
  else if (addr >= 0xA000 && addr <= 0xBFFF) {
    return gb.cart_ram + (addr & 0x1FFF);
  }
  else if (addr >= 0xC000 && addr <= 0xDFFF) {
    return gb.ext_ram + (addr & 0x1FFF);
  }
  else if (addr >= 0xE000 && addr <= 0xFDFF) {
    return gb.ext_ram + (addr & 0x1FFF);
  }
  else if (addr >= 0xFE00 && addr <= 0xFEFF) {
    return gb.oam_ram + (addr & 0x00FF);
  }
  else if (addr >= 0xFF80 && addr <= 0xFFFE) {
    return gb.zero_ram + (addr & 0x007F);
  }
  else {
    __debugbreak();
    return nullptr;
  }
}

//----------------------------------------

int GateBoyTests::test_dma(uint16_t src) {
  TEST_START("0x%04x", src);

  GateBoy gb;
  gb.reset();

  uint8_t* src_data = get_flat_ptr(gb, src);
  for (int i = 0; i < 256; i++) {
    src_data[i] = uint8_t(rand());
  }

  memset(gb.oam_ram, 0xFF, 256);

  gb.dbg_write(0xFF46, uint8_t(src >> 8));

  for (int i = 0; i < 1288; i++) gb.next_phase();

  for (int i = 0; i < 160; i++) {
    uint8_t a = src_data[i];
    uint8_t b = gb.dbg_read(0xFE00 + i);
    ASSERT_EQ(a, b, "dma mismatch @ 0x%04x : expected 0x%02x, got 0x%02x", src + i, a, b);
  }

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_serial() {
  TEST_START();

  GateBoy gb;
  //err += test_reg(gb, "SB",   ADDR_SB,   0b11111111); // something wrong with these ones
  //test_reg("SC",   ADDR_SC, 0b10000011);

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_ppu() {
  TEST_START();

  err += test_reg("LCDC", ADDR_LCDC, 0b11111111);
  err += test_reg("STAT", ADDR_STAT, 0b01111000);
  err += test_reg("SCY",  ADDR_SCY,  0b11111111);
  err += test_reg("SCX",  ADDR_SCX,  0b11111111);
  //err += test_reg("LY",   0xFF44, 177); // not standard reg
  err += test_reg("LYC",  ADDR_LYC,  0b11111111);
  err += test_reg("BGP",  ADDR_BGP,  0b11111111);
  err += test_reg("OBP0", ADDR_OBP0, 0b11111111);
  err += test_reg("OBP1", ADDR_OBP1, 0b11111111);
  err += test_reg("WY",   ADDR_WY,   0b11111111);
  err += test_reg("WX",   ADDR_WX,   0b11111111);

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_mem(GateBoy& gb, const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write) {
  TEST_START("%-4s @ [0x%04x,0x%04x], step %3d write %d", tag, addr_start, addr_end, step, test_write);

  int len = addr_end - addr_start + 1;
  uint8_t* mem = get_flat_ptr(gb, addr_start);

  for (int i = 0; i < len; i += step) {
    uint8_t data_wr = 0x55;
    if (test_write) {
      mem[i] = 0;
      gb.dbg_write(addr_start + i, data_wr);
    }
    else {
      mem[i] = data_wr;
    }
    uint8_t data_rd = gb.dbg_read(addr_start + i);
    ASSERT_EQ(data_rd, data_wr, "addr 0x%04x : expected 0x%02x, was 0x%02x", addr_start + i, data_wr, data_rd);
  }

  for (int i = 0; i < len; i += step) {
    uint8_t data_wr = 0xAA;
    if (test_write) {
      mem[i] = 0;
      gb.dbg_write(addr_start + i, data_wr);
    }
    else {
      mem[i] = data_wr;
    }
    uint8_t data_rd = gb.dbg_read(addr_start + i);
    ASSERT_EQ(data_rd, data_wr, "addr 0x%04x : expected 0x%02x, was 0x%02x", addr_start + i, data_wr, data_rd);
  }

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_reg(const char* tag, uint16_t addr, uint8_t mask) {
  TEST_START("%-4s @ 0x%04x, mask 0x%02x", tag, addr, mask);

  GateBoy gb;
  gb.reset();

  for (int i = 0; i < 256; i++) {
    uint8_t data_in = uint8_t(i & mask);
    gb.dbg_write(addr, uint8_t(data_in));
    uint8_t data_out = gb.dbg_read(addr) & mask;
    ASSERT_EQ(data_in, data_out, "reg %s @ 0x%04x: wrote 0x%02x, read 0x%02x", tag, addr, data_in, data_out);
  }

  TEST_END();
}

//------------------------------------------------------------------------------







//-----------------------------------------------------------------------------

void GateBoyTests::fuzz_reset_sequence(GateBoy& gateboy) {
  uint64_t rng = 1;

  Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };

#ifdef _DEBUG
  const int fuzz_count = 128;
#else
  const int fuzz_count = 65536;
#endif

  for (int i = 0; i < fuzz_count; i++) {
    mix(rng);

    //gateboy.top.clk_reg.preset_rst(wire(rng & 0x01));
    //gateboy.top.clk_reg.preset_clk_a(wire(rng & 0x02));
    //gateboy.top.cpu_bus.preset_cpu_ready(wire(rng & 0x04));
    //gateboy.top.clk_reg.preset_t1t2(wire(rng & 0x08), wire(rng & 0x10));

    int phase_count = (rng >> 8) & 0x0F;
    gateboy.run(phase_count);

    if ((i & 0xFF) == 0xFF) printf(".");
  }
}

//-----------------------------------------------------------------------------

void GateBoyTests::test_reset_sequence() {
  LOG("Running reset fuzz test in slow mode\n");
  GateBoy gateboy1;
  fuzz_reset_sequence(gateboy1);
  LOG("\n");

  LOG("Running reset fuzz test in fast mode\n");
  GateBoy gateboy2;
  fuzz_reset_sequence(gateboy2);
  LOG("\n");

  if (gateboy1.phase_hash != gateboy2.phase_hash) {
    LOG("XXXXXXXXXX FAIL PHASE HASH XXXXXXXXXX\n");
  }
  else {
    LOG("---------- PASS PHASE HASH ----------\n");
  }

  if (gateboy1.total_hash != gateboy2.total_hash) {
    LOG("XXXXXXXXXX FAIL TOTAL HASH XXXXXXXXXX\n");
  }
  else {
    LOG("---------- PASS TOTAL HASH ----------\n");
  }
}

//-----------------------------------------------------------------------------

void GateBoyTests::run_benchmark(GateBoy& gateboy) {

  gateboy.reset();

  LOG("Hash 1 after reset: 0x%016llx\n", gateboy.phase_hash);

#if _DEBUG
  const int iter_count = 16;
  const int phase_per_iter = 128;
  const int warmup = 0;
#else
  const int iter_count = 74;
  const int phase_per_iter = 8192;
  const int warmup = 10;
#endif

  double phase_rate_sum1 = 0;
  double phase_rate_sum2 = 0;
  double phase_rate_n = 0;

  double pass_rate_sum1 = 0;
  double pass_rate_sum2 = 0;
  double pass_rate_n = 0;

  gateboy.dbg_req = {.addr = 0x0150, .data = 0, .read = 1, .write = 0 };
  gateboy.sys_cpu_en = false;

  LOG("Running perf test");
  for (int iter = 0; iter < iter_count; iter++) {
    gateboy.phase_total = 0;
    gateboy.pass_total = 0;

    auto start = std::chrono::high_resolution_clock::now();
    gateboy.run(phase_per_iter);
    auto finish = std::chrono::high_resolution_clock::now();

    if (iter >= warmup) {
      std::chrono::duration<double> elapsed = finish - start;
      double time = elapsed.count();

      double phase_rate = double(gateboy.phase_total) / time;
      phase_rate_sum1 += phase_rate;
      phase_rate_sum2 += phase_rate * phase_rate;
      phase_rate_n++;

      double pass_rate = double(gateboy.pass_total) / time;
      pass_rate_sum1 += pass_rate;
      pass_rate_sum2 += pass_rate * pass_rate;
      pass_rate_n++;
    }
    LOG(".");
  }
  LOG("Done\n");

  LOG("Phase total %d\n", gateboy.phase_total);
  LOG("Pass total %d\n", gateboy.pass_total);

  double phase_rate_mean     = phase_rate_sum1 / phase_rate_n;
  double phase_rate_variance = (phase_rate_sum2 / phase_rate_n) - (phase_rate_mean * phase_rate_mean);
  double phase_rate_sigma    = sqrt(phase_rate_variance);
  LOG("Mean phase/sec %f sigma %f\n", phase_rate_mean, phase_rate_sigma);

  double pass_rate_mean     = pass_rate_sum1 / pass_rate_n;
  double pass_rate_variance = (pass_rate_sum2 / pass_rate_n) - (pass_rate_mean * pass_rate_mean);
  double pass_rate_sigma    = sqrt(pass_rate_variance);
  LOG("Mean pass/sec %f sigma %f\n", pass_rate_mean, pass_rate_sigma);

  LOG("Commit phase_hash   0x%016llx\n", gateboy.phase_hash);
  LOG("Combined phase_hash 0x%016llx\n", gateboy.total_hash);
}

//-----------------------------------------------------------------------------

void GateBoyTests::run_benchmark() {
  LOG("Running benchmark in slow mode\n");
  GateBoy gateboy1;
  run_benchmark(gateboy1);
  LOG("\n");

#if 0
  LOG("Running benchmark in fast mode\n");
  GateBoy gateboy2;
  run_benchmark(gateboy2);
  LOG("\n");

  if (gateboy1.phase_hash != gateboy2.phase_hash) {
    LOG("XXXXXXXXXX FAIL PHASE HASH XXXXXXXXXX\n");
  }
  else {
    LOG("---------- PASS PHASE HASH ----------\n");
  }

  if (gateboy1.total_hash != gateboy2.total_hash) {
    LOG("XXXXXXXXXX FAIL TOTAL HASH XXXXXXXXXX\n");
  }
  else {
    LOG("---------- PASS TOTAL HASH ----------\n");
  }
#endif
}

