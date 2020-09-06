#include "GateBoyTests/GateBoyTests.h"
#include "GateBoyLib/GateBoy.h"
#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "CoreLib/File.h"
#include <chrono>
#include <stddef.h>

//-----------------------------------------------------------------------------

int GateBoyTests::test_main(int argc, char** argv) {
  TEST_START("Maaaaaain");

  (void)argc;
  (void)argv;

  auto start = std::chrono::high_resolution_clock::now();

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

  auto finish = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed = finish - start;
  LOG_G("Tests took %f seconds\n", elapsed.count());

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_init() {
  TEST_START("Init");

  GateBoy gb;
  gb.reset();

  uint64_t top_hash = hash_states(&gb.top, sizeof(gb.top));
  LOG_Y("Top state hash after reset is 0x%016llx\n", top_hash);
  EXPECT_EQ(0xd89b445082824d87, top_hash, "Top hash mismatch");

  // All unlocked regs should have no delta
  for (int i = 0; i < sizeof(gb.top); i++) {
    uint8_t delta = ((uint8_t*)&gb.top)[i] >> 4;
    ASSERT_EQ(1, (delta == DELTA_NONE) || (delta == DELTA_LOCK));
    //ASSERT_EQ(delta, DELTA_NONE);
  }

  // Mem should be clear
  for (int i = 0; i < 32768; i++) ASSERT_EQ(0, gb.cart_rom[i]);
  for (int i = 0; i < 8192; i++)  ASSERT_EQ(0, gb.cart_ram[i]);
  for (int i = 0; i < 8192; i++)  ASSERT_EQ(0, gb.ext_ram[i]);

  // Framebuffer should be 0x04 (yellow)
  for (int i = 0; i < 160*144; i++) {
    ASSERT_EQ(4, gb.framebuffer[i]);
  }

  EXPECT_EQ(0xCF, gb.dbg_read(ADDR_P1),   "Bad P1 reset value");
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_SB),   "Bad SB reset value");
  EXPECT_EQ(0x7E, gb.dbg_read(ADDR_SC),   "Bad SC reset value"); // double-check this

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

  // Button signals should be pulled high
  EXPECT_EQ('^', gb.top.joypad.PIN_JOY_P10.c());
  EXPECT_EQ('^', gb.top.joypad.PIN_JOY_P11.c());
  EXPECT_EQ('^', gb.top.joypad.PIN_JOY_P12.c());
  EXPECT_EQ('^', gb.top.joypad.PIN_JOY_P13.c());

  // Button scan signals should be driven low
  EXPECT_EQ('0', gb.top.joypad.PIN_JOY_P14.c());
  EXPECT_EQ('0', gb.top.joypad.PIN_JOY_P15.c());

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

    EXPECT_CLK(top.cpu_bus.PIN_CPU_BOWA_xBCDEFGH.tp(), 0b01111111);
    EXPECT_CLK(top.cpu_bus.PIN_CPU_BEDO_Axxxxxxx.tp(), 0b10000000);
    EXPECT_CLK(top.cpu_bus.PIN_CPU_BEKO_ABCDxxxx.tp(), 0b11110000);
    EXPECT_CLK(top.cpu_bus.PIN_CPU_BUDE_xxxxEFGH.tp(), 0b00001111);
    EXPECT_CLK(top.cpu_bus.PIN_CPU_BOLO_ABCDEFxx.tp(), 0b11111100);
    EXPECT_CLK(top.cpu_bus.PIN_CPU_BUKE_AxxxxxGH.tp(), 0b10000011);
    EXPECT_CLK(top.cpu_bus.PIN_CPU_BOMA_Axxxxxxx.tp(), 0b10000000);
    EXPECT_CLK(top.cpu_bus.PIN_CPU_BOGA_xBCDEFGH.tp(), 0b01111111);
    EXPECT_CLK(top.ext_bus.PIN_EXT_CLK.qp(),           0b11110000);
    gb.next_phase();
  }

  TEST_END();
}

//-----------------------------------------------------------------------------

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
#if 1
  {
    LOG_Y("Testing \"ld (hl), a; jr -2;\" the hard way - hl = 0xC003, a = 0x55\n");

    GateBoy gb;
    gb.reset();
    gb.set_boot_bit();

    gb.cart_rom[0x0155] = 0x77;
    gb.cart_rom[0x0156] = 0x18;
    gb.cart_rom[0x0157] = 0xFD;

    Req* script = new Req[5];
    script[0].addr = 0x0155; script[0].data = 0x00; script[0].read = 1; script[0].write = 0; // read "ld (hl), a" opcode
    script[1].addr = 0xC003; script[1].data = 0x55; script[1].read = 0; script[1].write = 1; // write to 0xC003
    script[2].addr = 0x0156; script[2].data = 0x00; script[2].read = 1; script[2].write = 0; // read "jr -2" opcode
    script[3].addr = 0x0157; script[3].data = 0x00; script[3].read = 1; script[3].write = 0; // read "jr -2" param
    script[4].addr = 0x0157; script[4].data = 0x00; script[4].read = 0; script[4].write = 0; // idle cycle

    // Run the script once without checking phases
    gb.dbg_req = script[0]; gb.run(8);
    gb.dbg_req = script[1]; gb.run(8);
    gb.dbg_req = script[2]; gb.run(8);
    gb.dbg_req = script[3]; gb.run(8);
    gb.dbg_req = script[4]; gb.run(8);

    // Start checking each phase

    const char* CLK_WAVE = "11110000 11110000 11110000 11110000 11110000";
    const char* WRn_WAVE = "11111111 11110001 11111111 11111111 11111111";
    const char* RDn_WAVE = "00000000 01111111 00000000 00000000 00000000";
    const char* CSn_WAVE = "11111111 11000000 11111111 11111111 11111111";
                                                       
    const char* A00_WAVE = "11111111 11111111 10000000 01111111 11111111";
    const char* A01_WAVE = "10000000 01111111 11111111 11111111 11111111";
    const char* A02_WAVE = "11111111 10000000 01111111 11111111 11111111";
    const char* A03_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A04_WAVE = "11111111 10000000 01111111 11111111 11111111";
    const char* A05_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A06_WAVE = "11111111 10000000 01111111 11111111 11111111";
    const char* A07_WAVE = "00000000 00000000 00000000 00000000 00000000";

    const char* A08_WAVE = "01111111 00000000 01111111 01111111 00000000";
    const char* A09_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A10_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A11_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A12_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A13_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A14_WAVE = "00000000 01111111 00000000 00000000 00000000";
    const char* A15_WAVE = "11000000 11111111 11000000 11000000 11111111";
                                                          
    const char* D00_WAVE = "^^111111 ^1111111 ^^000000 ^^111111 ^^^^^^^^"; // Scope trace w/ extra pulldown resistor patched in so we can see when
    const char* D01_WAVE = "^^111111 ^1110000 ^^000000 ^^000000 ^^^^^^^^"; // the pullups are the only thing driving the bus.
    const char* D02_WAVE = "^^111111 ^1111111 ^^000000 ^^111111 ^^^^^^^^"; // #
    const char* D03_WAVE = "^^000000 ^1110000 ^^111111 ^^111111 ^^^^^^^^"; // #
    const char* D04_WAVE = "^^111111 ^1111111 ^^111111 ^^111111 ^^^^^^^^"; // #
    const char* D05_WAVE = "^^111111 ^1110000 ^^000000 ^^111111 ^^^^^^^^"; // #
    const char* D06_WAVE = "^^111111 ^1111111 ^^000000 ^^111111 ^^^^^^^^"; // #
    const char* D07_WAVE = "^^000000 ^1110000 ^^000000 ^^111111 ^^^^^^^^"; // #

    for (int i = 0; i < 40; i++) {
      if ((i % 8) == 0) gb.dbg_req = script[i / 8];

      char CLK = gb.top.ext_bus.PIN_EXT_CLK.c();
      char WRn = gb.top.ext_bus.PIN_EXT_WRn.c();
      char RDn = gb.top.ext_bus.PIN_EXT_RDn.c();
      char CSn = gb.top.ext_bus.PIN_EXT_CSn.c();

      char A00 = gb.top.ext_bus.PIN_EXT_A00p.c();
      char A01 = gb.top.ext_bus.PIN_EXT_A01p.c();
      char A02 = gb.top.ext_bus.PIN_EXT_A02p.c();
      char A03 = gb.top.ext_bus.PIN_EXT_A03p.c();
      char A04 = gb.top.ext_bus.PIN_EXT_A04p.c();
      char A05 = gb.top.ext_bus.PIN_EXT_A05p.c();
      char A06 = gb.top.ext_bus.PIN_EXT_A06p.c();
      char A07 = gb.top.ext_bus.PIN_EXT_A07p.c();
      char A08 = gb.top.ext_bus.PIN_EXT_A08p.c();
      char A09 = gb.top.ext_bus.PIN_EXT_A09p.c();
      char A10 = gb.top.ext_bus.PIN_EXT_A10p.c();
      char A11 = gb.top.ext_bus.PIN_EXT_A11p.c();
      char A12 = gb.top.ext_bus.PIN_EXT_A12p.c();
      char A13 = gb.top.ext_bus.PIN_EXT_A13p.c();
      char A14 = gb.top.ext_bus.PIN_EXT_A14p.c();
      char A15 = gb.top.ext_bus.PIN_EXT_A15p.c();

      char D00 = gb.top.ext_bus.PIN_EXT_D00p.c();
      char D01 = gb.top.ext_bus.PIN_EXT_D01p.c();
      char D02 = gb.top.ext_bus.PIN_EXT_D02p.c();
      char D03 = gb.top.ext_bus.PIN_EXT_D03p.c();
      char D04 = gb.top.ext_bus.PIN_EXT_D04p.c();
      char D05 = gb.top.ext_bus.PIN_EXT_D05p.c();
      char D06 = gb.top.ext_bus.PIN_EXT_D06p.c();
      char D07 = gb.top.ext_bus.PIN_EXT_D07p.c();

      int wave_idx = ((i / 8) * 9) + (i % 8);

      EXPECT_EQ(CLK, CLK_WAVE[wave_idx], "CLK failure at phase %d - expected %c, got %c\n", i, CLK_WAVE[wave_idx], CLK);
      EXPECT_EQ(WRn, WRn_WAVE[wave_idx], "WRn failure at phase %d - expected %c, got %c\n", i, WRn_WAVE[wave_idx], WRn);
      EXPECT_EQ(RDn, RDn_WAVE[wave_idx], "RDn failure at phase %d - expected %c, got %c\n", i, RDn_WAVE[wave_idx], RDn);
      EXPECT_EQ(CSn, CSn_WAVE[wave_idx], "CSn failure at phase %d - expected %c, got %c\n", i, CSn_WAVE[wave_idx], CSn);

      EXPECT_EQ(A00, A00_WAVE[wave_idx], "A00 failure at phase %d - expected %c, got %c\n", i, A00_WAVE[wave_idx], A00);
      EXPECT_EQ(A01, A01_WAVE[wave_idx], "A01 failure at phase %d - expected %c, got %c\n", i, A01_WAVE[wave_idx], A01);
      EXPECT_EQ(A02, A02_WAVE[wave_idx], "A02 failure at phase %d - expected %c, got %c\n", i, A02_WAVE[wave_idx], A02);
      EXPECT_EQ(A03, A03_WAVE[wave_idx], "A03 failure at phase %d - expected %c, got %c\n", i, A03_WAVE[wave_idx], A03);
      EXPECT_EQ(A04, A04_WAVE[wave_idx], "A04 failure at phase %d - expected %c, got %c\n", i, A04_WAVE[wave_idx], A04);
      EXPECT_EQ(A05, A05_WAVE[wave_idx], "A05 failure at phase %d - expected %c, got %c\n", i, A05_WAVE[wave_idx], A05);
      EXPECT_EQ(A06, A06_WAVE[wave_idx], "A06 failure at phase %d - expected %c, got %c\n", i, A06_WAVE[wave_idx], A06);
      EXPECT_EQ(A07, A07_WAVE[wave_idx], "A07 failure at phase %d - expected %c, got %c\n", i, A07_WAVE[wave_idx], A07);
      EXPECT_EQ(A08, A08_WAVE[wave_idx], "A08 failure at phase %d - expected %c, got %c\n", i, A08_WAVE[wave_idx], A08);
      EXPECT_EQ(A09, A09_WAVE[wave_idx], "A09 failure at phase %d - expected %c, got %c\n", i, A09_WAVE[wave_idx], A09);
      EXPECT_EQ(A10, A10_WAVE[wave_idx], "A10 failure at phase %d - expected %c, got %c\n", i, A10_WAVE[wave_idx], A10);
      EXPECT_EQ(A11, A11_WAVE[wave_idx], "A11 failure at phase %d - expected %c, got %c\n", i, A11_WAVE[wave_idx], A11);
      EXPECT_EQ(A12, A12_WAVE[wave_idx], "A12 failure at phase %d - expected %c, got %c\n", i, A12_WAVE[wave_idx], A12);
      EXPECT_EQ(A13, A13_WAVE[wave_idx], "A13 failure at phase %d - expected %c, got %c\n", i, A13_WAVE[wave_idx], A13);
      EXPECT_EQ(A14, A14_WAVE[wave_idx], "A14 failure at phase %d - expected %c, got %c\n", i, A14_WAVE[wave_idx], A14);
      EXPECT_EQ(A15, A15_WAVE[wave_idx], "A15 failure at phase %d - expected %c, got %c\n", i, A15_WAVE[wave_idx], A15);

      EXPECT_EQ(D00, D00_WAVE[wave_idx], "D00 failure at phase %d - expected %c, got %c\n", i, D00_WAVE[wave_idx], D00);
      EXPECT_EQ(D01, D01_WAVE[wave_idx], "D01 failure at phase %d - expected %c, got %c\n", i, D01_WAVE[wave_idx], D01);
      EXPECT_EQ(D02, D02_WAVE[wave_idx], "D02 failure at phase %d - expected %c, got %c\n", i, D02_WAVE[wave_idx], D02);
      EXPECT_EQ(D03, D03_WAVE[wave_idx], "D03 failure at phase %d - expected %c, got %c\n", i, D03_WAVE[wave_idx], D03);
      EXPECT_EQ(D04, D04_WAVE[wave_idx], "D04 failure at phase %d - expected %c, got %c\n", i, D04_WAVE[wave_idx], D04);
      EXPECT_EQ(D05, D05_WAVE[wave_idx], "D05 failure at phase %d - expected %c, got %c\n", i, D05_WAVE[wave_idx], D05);
      EXPECT_EQ(D06, D06_WAVE[wave_idx], "D06 failure at phase %d - expected %c, got %c\n", i, D06_WAVE[wave_idx], D06);
      EXPECT_EQ(D07, D07_WAVE[wave_idx], "D07 failure at phase %d - expected %c, got %c\n", i, D07_WAVE[wave_idx], D07);

      gb.next_phase();
    }
  }
#endif

  {
    // VRAM access
    // ld (hl), a
    // jr -2

    LOG_Y("Testing \"ld (hl), a; jr -2;\" the hard way - hl = 0x9777, a = 0x55\n");

    GateBoy gb;
    gb.reset();
    gb.set_boot_bit();

    gb.cart_rom[0x0155] = 0x77;
    gb.cart_rom[0x0156] = 0x18;
    gb.cart_rom[0x0157] = 0xFD;

    Req* script = new Req[5];
    script[0].addr = 0x0155; script[0].data = 0x00; script[0].read = 1; script[0].write = 0; // read "ld (hl), a" opcode
    script[1].addr = 0x9777; script[1].data = 0x55; script[1].read = 0; script[1].write = 1; // write to 0xC003
    script[2].addr = 0x0156; script[2].data = 0x00; script[2].read = 1; script[2].write = 0; // read "jr -2" opcode
    script[3].addr = 0x0157; script[3].data = 0x00; script[3].read = 1; script[3].write = 0; // read "jr -2" param
    script[4].addr = 0x0157; script[4].data = 0x00; script[4].read = 0; script[4].write = 0; // idle cycle

    // Run the script once without checking phases
    gb.dbg_req = script[0]; gb.run(8);
    gb.dbg_req = script[1]; gb.run(8);
    gb.dbg_req = script[2]; gb.run(8);
    gb.dbg_req = script[3]; gb.run(8);
    gb.dbg_req = script[4]; gb.run(8);

    // Start checking each phase

    const char* CLK_WAVE = "11110000 11110000 11110000 11110000 11110000";
    const char* WRn_WAVE = "11111111 11111111 11111111 11111111 11111111";
    const char* RDn_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* CSn_WAVE = "11111111 11111111 11111111 11111111 11111111";

    const char* A00_WAVE = "11111111 11111111 10000000 01111111 11111111";
    const char* A01_WAVE = "10000000 00000000 01111111 11111111 11111111";
    const char* A02_WAVE = "11111111 11111111 11111111 11111111 11111111";
    const char* A03_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A04_WAVE = "11111111 11111111 11111111 11111111 11111111";
    const char* A05_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A06_WAVE = "11111111 11111111 11111111 11111111 11111111";
    const char* A07_WAVE = "00000000 00000000 00000000 00000000 00000000";

    const char* A08_WAVE = "01111111 00000000 01111111 01111111 00000000";
    const char* A09_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A10_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A11_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A12_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A13_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A14_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A15_WAVE = "11000000 11111111 11000000 11000000 11111111";

    const char* D00_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D01_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^000000 ^^^^^^^^";
    const char* D02_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D03_WAVE = "^^000000 ^^^^^^^^ ^^111111 ^^111111 ^^^^^^^^";
    const char* D04_WAVE = "^^111111 ^^^^^^^^ ^^111111 ^^111111 ^^^^^^^^";
    const char* D05_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D06_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D07_WAVE = "^^000000 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";

    for (int i = 0; i < 40; i++) {
      if ((i % 8) == 0) gb.dbg_req = script[i / 8];

      char CLK = gb.top.ext_bus.PIN_EXT_CLK.c();
      char WRn = gb.top.ext_bus.PIN_EXT_WRn.c();
      char RDn = gb.top.ext_bus.PIN_EXT_RDn.c();
      char CSn = gb.top.ext_bus.PIN_EXT_CSn.c();

      char A00 = gb.top.ext_bus.PIN_EXT_A00p.c();
      char A01 = gb.top.ext_bus.PIN_EXT_A01p.c();
      char A02 = gb.top.ext_bus.PIN_EXT_A02p.c();
      char A03 = gb.top.ext_bus.PIN_EXT_A03p.c();
      char A04 = gb.top.ext_bus.PIN_EXT_A04p.c();
      char A05 = gb.top.ext_bus.PIN_EXT_A05p.c();
      char A06 = gb.top.ext_bus.PIN_EXT_A06p.c();
      char A07 = gb.top.ext_bus.PIN_EXT_A07p.c();
      char A08 = gb.top.ext_bus.PIN_EXT_A08p.c();
      char A09 = gb.top.ext_bus.PIN_EXT_A09p.c();
      char A10 = gb.top.ext_bus.PIN_EXT_A10p.c();
      char A11 = gb.top.ext_bus.PIN_EXT_A11p.c();
      char A12 = gb.top.ext_bus.PIN_EXT_A12p.c();
      char A13 = gb.top.ext_bus.PIN_EXT_A13p.c();
      char A14 = gb.top.ext_bus.PIN_EXT_A14p.c();
      char A15 = gb.top.ext_bus.PIN_EXT_A15p.c();

      char D00 = gb.top.ext_bus.PIN_EXT_D00p.c();
      char D01 = gb.top.ext_bus.PIN_EXT_D01p.c();
      char D02 = gb.top.ext_bus.PIN_EXT_D02p.c();
      char D03 = gb.top.ext_bus.PIN_EXT_D03p.c();
      char D04 = gb.top.ext_bus.PIN_EXT_D04p.c();
      char D05 = gb.top.ext_bus.PIN_EXT_D05p.c();
      char D06 = gb.top.ext_bus.PIN_EXT_D06p.c();
      char D07 = gb.top.ext_bus.PIN_EXT_D07p.c();

      
      int wave_idx = ((i / 8) * 9) + (i % 8);

      EXPECT_EQ(CLK, CLK_WAVE[wave_idx], "CLK failure at phase %d - expected %c, got %c\n", i, CLK_WAVE[wave_idx], CLK);
      EXPECT_EQ(WRn, WRn_WAVE[wave_idx], "WRn failure at phase %d - expected %c, got %c\n", i, WRn_WAVE[wave_idx], WRn);
      EXPECT_EQ(RDn, RDn_WAVE[wave_idx], "RDn failure at phase %d - expected %c, got %c\n", i, RDn_WAVE[wave_idx], RDn);
      EXPECT_EQ(CSn, CSn_WAVE[wave_idx], "CSn failure at phase %d - expected %c, got %c\n", i, CSn_WAVE[wave_idx], CSn);

      EXPECT_EQ(A00, A00_WAVE[wave_idx], "A00 failure at phase %d - expected %c, got %c\n", i, A00_WAVE[wave_idx], A00);
      EXPECT_EQ(A01, A01_WAVE[wave_idx], "A01 failure at phase %d - expected %c, got %c\n", i, A01_WAVE[wave_idx], A01);
      EXPECT_EQ(A02, A02_WAVE[wave_idx], "A02 failure at phase %d - expected %c, got %c\n", i, A02_WAVE[wave_idx], A02);
      EXPECT_EQ(A03, A03_WAVE[wave_idx], "A03 failure at phase %d - expected %c, got %c\n", i, A03_WAVE[wave_idx], A03);

      EXPECT_EQ(A04, A04_WAVE[wave_idx], "A04 failure at phase %d - expected %c, got %c\n", i, A04_WAVE[wave_idx], A04);
      EXPECT_EQ(A05, A05_WAVE[wave_idx], "A05 failure at phase %d - expected %c, got %c\n", i, A05_WAVE[wave_idx], A05);
      EXPECT_EQ(A06, A06_WAVE[wave_idx], "A06 failure at phase %d - expected %c, got %c\n", i, A06_WAVE[wave_idx], A06);
      EXPECT_EQ(A07, A07_WAVE[wave_idx], "A07 failure at phase %d - expected %c, got %c\n", i, A07_WAVE[wave_idx], A07);

      EXPECT_EQ(A08, A08_WAVE[wave_idx], "A08 failure at phase %d - expected %c, got %c\n", i, A08_WAVE[wave_idx], A08);
      EXPECT_EQ(A09, A09_WAVE[wave_idx], "A09 failure at phase %d - expected %c, got %c\n", i, A09_WAVE[wave_idx], A09);
      EXPECT_EQ(A10, A10_WAVE[wave_idx], "A10 failure at phase %d - expected %c, got %c\n", i, A10_WAVE[wave_idx], A10);
      EXPECT_EQ(A11, A11_WAVE[wave_idx], "A11 failure at phase %d - expected %c, got %c\n", i, A11_WAVE[wave_idx], A11);

      EXPECT_EQ(A12, A12_WAVE[wave_idx], "A12 failure at phase %d - expected %c, got %c\n", i, A12_WAVE[wave_idx], A12);
      EXPECT_EQ(A13, A13_WAVE[wave_idx], "A13 failure at phase %d - expected %c, got %c\n", i, A13_WAVE[wave_idx], A13);
      EXPECT_EQ(A14, A14_WAVE[wave_idx], "A14 failure at phase %d - expected %c, got %c\n", i, A14_WAVE[wave_idx], A14);
      EXPECT_EQ(A15, A15_WAVE[wave_idx], "A15 failure at phase %d - expected %c, got %c\n", i, A15_WAVE[wave_idx], A15);

      EXPECT_EQ(D00, D00_WAVE[wave_idx], "D00 failure at phase %d - expected %c, got %c\n", i, D00_WAVE[wave_idx], D00);
      EXPECT_EQ(D01, D01_WAVE[wave_idx], "D01 failure at phase %d - expected %c, got %c\n", i, D01_WAVE[wave_idx], D01);
      EXPECT_EQ(D02, D02_WAVE[wave_idx], "D02 failure at phase %d - expected %c, got %c\n", i, D02_WAVE[wave_idx], D02);
      EXPECT_EQ(D03, D03_WAVE[wave_idx], "D03 failure at phase %d - expected %c, got %c\n", i, D03_WAVE[wave_idx], D03);
      EXPECT_EQ(D04, D04_WAVE[wave_idx], "D04 failure at phase %d - expected %c, got %c\n", i, D04_WAVE[wave_idx], D04);
      EXPECT_EQ(D05, D05_WAVE[wave_idx], "D05 failure at phase %d - expected %c, got %c\n", i, D05_WAVE[wave_idx], D05);
      EXPECT_EQ(D06, D06_WAVE[wave_idx], "D06 failure at phase %d - expected %c, got %c\n", i, D06_WAVE[wave_idx], D06);
      EXPECT_EQ(D07, D07_WAVE[wave_idx], "D07 failure at phase %d - expected %c, got %c\n", i, D07_WAVE[wave_idx], D07);

      gb.next_phase();
    }
  }

  {
    // ZRAM access
    // ld (hl), a
    // jr -2

    LOG_Y("Testing \"ld (hl), a; jr -2;\" the hard way - hl = 0xFF80, a = 0x55\n");

    GateBoy gb;
    gb.reset();
    gb.set_boot_bit();

    gb.cart_rom[0x0155] = 0x77;
    gb.cart_rom[0x0156] = 0x18;
    gb.cart_rom[0x0157] = 0xFD;

    Req* script = new Req[5];
    script[0].addr = 0x0155; script[0].data = 0x00; script[0].read = 1; script[0].write = 0; // read "ld (hl), a" opcode
    script[1].addr = 0xFF80; script[1].data = 0x55; script[1].read = 0; script[1].write = 1; // write to 0xFF80
    script[2].addr = 0x0156; script[2].data = 0x00; script[2].read = 1; script[2].write = 0; // read "jr -2" opcode
    script[3].addr = 0x0157; script[3].data = 0x00; script[3].read = 1; script[3].write = 0; // read "jr -2" param
    script[4].addr = 0x0157; script[4].data = 0x00; script[4].read = 0; script[4].write = 0; // idle cycle

    // Run the script once without checking phases
    gb.dbg_req = script[0]; gb.run(8);
    gb.dbg_req = script[1]; gb.run(8);
    gb.dbg_req = script[2]; gb.run(8);
    gb.dbg_req = script[3]; gb.run(8);
    gb.dbg_req = script[4]; gb.run(8);

    // Start checking each phase

      
    const char* CLK_WAVE = "11110000 11110000 11110000 11110000 11110000";
    const char* WRn_WAVE = "11111111 11111111 11111111 11111111 11111111";
    const char* RDn_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* CSn_WAVE = "11111111 11111111 11111111 11111111 11111111";

#if 0
    // PIN_CPU_ADDR_EXTp has a GLITCH!
    {
      // PIN_EXT_WRn = nand(PIN_CPU_WRp, xxxxEFGx, PIN_CPU_ADDR_EXTp, !ADDR_VRAM);
      // PIN_EXT_RDn = (PIN_CPU_WR && PIN_CPU_ADDR_EXTp);
      // PIN_EXT_ANNp = tp_latch(and2(PIN_CPU_ADDR_EXTp, !ADDR_VRAM), BUS_CPU_ANN);

      // WRn = 1 = nand(PIN_CPU_WRp, xxxxEFGx, PIN_CPU_ADDR_EXTp);
      
      // RDn = 0 = (PIN_CPU_WR && PIN_CPU_ADDR_EXTp);
      // A00 = 1 = tp_latch(and2(PIN_CPU_ADDR_EXTp, !ADDR_VRAM), BUS_CPU_ANN);

      //const char* WRn_WAVE = "11111111 11111111 11111111 11111111 11111111"; // good
      //const char* RDn_WAVE = "00000000 00000000 00000000 00000000 00000000";
      //const char* A00_WAVE = "11111111 10000000 00000000 01111111 11111111"; // good
    }

    // The glitch pulse causes the ext address latch to latch even though the
    // 0xFFxx addresses are _not_ external, so the address shows up on the bus.

    //                                xxxxxxx x
    const char* A00_WAVE = "11111111 10000000 00000000 01111111 11111111";
    const char* A01_WAVE = "10000000 00000000 01111111 11111111 11111111";
    const char* A02_WAVE = "11111111 10000000 01111111 11111111 11111111";
    const char* A03_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A04_WAVE = "11111111 10000000 01111111 11111111 11111111";
    const char* A05_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A06_WAVE = "11111111 10000000 01111111 11111111 11111111";
    const char* A07_WAVE = "00000000 01111111 10000000 00000000 00000000";

    const char* A08_WAVE = "01111111 01111111 11111111 01111111 00000000";
    const char* A09_WAVE = "00000000 01111111 10000000 00000000 00000000";
    const char* A10_WAVE = "00000000 01111111 10000000 00000000 00000000";
    const char* A11_WAVE = "00000000 01111111 10000000 00000000 00000000";
    const char* A12_WAVE = "00000000 01111111 10000000 00000000 00000000";
    const char* A13_WAVE = "00000000 01111111 10000000 00000000 00000000";
    const char* A14_WAVE = "00000000 01111111 10000000 00000000 00000000";
    const char* A15_WAVE = "11000000 11111111 11000000 11000000 11111111";
#endif

    // We are not going to bother simulating the glitch as it has no effect
    // on the operation of the Game Boy

    const char* A00_WAVE = "11111111 11111111 10000000 01111111 11111111";
    const char* A01_WAVE = "10000000 00000000 01111111 11111111 11111111";
    const char* A02_WAVE = "11111111 11111111 11111111 11111111 11111111";
    const char* A03_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A04_WAVE = "11111111 11111111 11111111 11111111 11111111";
    const char* A05_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A06_WAVE = "11111111 11111111 11111111 11111111 11111111";
    const char* A07_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A08_WAVE = "01111111 00000000 01111111 01111111 00000000";
    const char* A09_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A10_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A11_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A12_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A13_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A14_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* A15_WAVE = "11000000 11111111 11000000 11000000 11111111";

    const char* D00_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D01_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^000000 ^^^^^^^^";
    const char* D02_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D03_WAVE = "^^000000 ^^^^^^^^ ^^111111 ^^111111 ^^^^^^^^";
    const char* D04_WAVE = "^^111111 ^^^^^^^^ ^^111111 ^^111111 ^^^^^^^^";
    const char* D05_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D06_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D07_WAVE = "^^000000 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";

    for (int i = 0; i < 40; i++) {
      if ((i % 8) == 0) gb.dbg_req = script[i / 8];

      char CLK = gb.top.ext_bus.PIN_EXT_CLK.c();
      char WRn = gb.top.ext_bus.PIN_EXT_WRn.c();
      char RDn = gb.top.ext_bus.PIN_EXT_RDn.c();
      char CSn = gb.top.ext_bus.PIN_EXT_CSn.c();

      char A00 = gb.top.ext_bus.PIN_EXT_A00p.c();
      char A01 = gb.top.ext_bus.PIN_EXT_A01p.c();
      char A02 = gb.top.ext_bus.PIN_EXT_A02p.c();
      char A03 = gb.top.ext_bus.PIN_EXT_A03p.c();
      char A04 = gb.top.ext_bus.PIN_EXT_A04p.c();
      char A05 = gb.top.ext_bus.PIN_EXT_A05p.c();
      char A06 = gb.top.ext_bus.PIN_EXT_A06p.c();
      char A07 = gb.top.ext_bus.PIN_EXT_A07p.c();
      char A08 = gb.top.ext_bus.PIN_EXT_A08p.c();
      char A09 = gb.top.ext_bus.PIN_EXT_A09p.c();
      char A10 = gb.top.ext_bus.PIN_EXT_A10p.c();
      char A11 = gb.top.ext_bus.PIN_EXT_A11p.c();
      char A12 = gb.top.ext_bus.PIN_EXT_A12p.c();
      char A13 = gb.top.ext_bus.PIN_EXT_A13p.c();
      char A14 = gb.top.ext_bus.PIN_EXT_A14p.c();
      char A15 = gb.top.ext_bus.PIN_EXT_A15p.c();

      char D00 = gb.top.ext_bus.PIN_EXT_D00p.c();
      char D01 = gb.top.ext_bus.PIN_EXT_D01p.c();
      char D02 = gb.top.ext_bus.PIN_EXT_D02p.c();
      char D03 = gb.top.ext_bus.PIN_EXT_D03p.c();
      char D04 = gb.top.ext_bus.PIN_EXT_D04p.c();
      char D05 = gb.top.ext_bus.PIN_EXT_D05p.c();
      char D06 = gb.top.ext_bus.PIN_EXT_D06p.c();
      char D07 = gb.top.ext_bus.PIN_EXT_D07p.c();

      
      int wave_idx = ((i / 8) * 9) + (i % 8);

      EXPECT_EQ(CLK, CLK_WAVE[wave_idx], "CLK failure at phase %d - expected %c, got %c\n", i, CLK_WAVE[wave_idx], CLK);
      EXPECT_EQ(WRn, WRn_WAVE[wave_idx], "WRn failure at phase %d - expected %c, got %c\n", i, WRn_WAVE[wave_idx], WRn);
      EXPECT_EQ(RDn, RDn_WAVE[wave_idx], "RDn failure at phase %d - expected %c, got %c\n", i, RDn_WAVE[wave_idx], RDn);
      EXPECT_EQ(CSn, CSn_WAVE[wave_idx], "CSn failure at phase %d - expected %c, got %c\n", i, CSn_WAVE[wave_idx], CSn);

      EXPECT_EQ(A00, A00_WAVE[wave_idx], "A00 failure at phase %d - expected %c, got %c\n", i, A00_WAVE[wave_idx], A00);
      EXPECT_EQ(A01, A01_WAVE[wave_idx], "A01 failure at phase %d - expected %c, got %c\n", i, A01_WAVE[wave_idx], A01);
      EXPECT_EQ(A02, A02_WAVE[wave_idx], "A02 failure at phase %d - expected %c, got %c\n", i, A02_WAVE[wave_idx], A02);
      EXPECT_EQ(A03, A03_WAVE[wave_idx], "A03 failure at phase %d - expected %c, got %c\n", i, A03_WAVE[wave_idx], A03);
      EXPECT_EQ(A04, A04_WAVE[wave_idx], "A04 failure at phase %d - expected %c, got %c\n", i, A04_WAVE[wave_idx], A04);
      EXPECT_EQ(A05, A05_WAVE[wave_idx], "A05 failure at phase %d - expected %c, got %c\n", i, A05_WAVE[wave_idx], A05);
      EXPECT_EQ(A06, A06_WAVE[wave_idx], "A06 failure at phase %d - expected %c, got %c\n", i, A06_WAVE[wave_idx], A06);
      EXPECT_EQ(A07, A07_WAVE[wave_idx], "A07 failure at phase %d - expected %c, got %c\n", i, A07_WAVE[wave_idx], A07);
      EXPECT_EQ(A08, A08_WAVE[wave_idx], "A08 failure at phase %d - expected %c, got %c\n", i, A08_WAVE[wave_idx], A08);
      EXPECT_EQ(A09, A09_WAVE[wave_idx], "A09 failure at phase %d - expected %c, got %c\n", i, A09_WAVE[wave_idx], A09);
      EXPECT_EQ(A10, A10_WAVE[wave_idx], "A10 failure at phase %d - expected %c, got %c\n", i, A10_WAVE[wave_idx], A10);
      EXPECT_EQ(A11, A11_WAVE[wave_idx], "A11 failure at phase %d - expected %c, got %c\n", i, A11_WAVE[wave_idx], A11);
      EXPECT_EQ(A12, A12_WAVE[wave_idx], "A12 failure at phase %d - expected %c, got %c\n", i, A12_WAVE[wave_idx], A12);
      EXPECT_EQ(A13, A13_WAVE[wave_idx], "A13 failure at phase %d - expected %c, got %c\n", i, A13_WAVE[wave_idx], A13);
      EXPECT_EQ(A14, A14_WAVE[wave_idx], "A14 failure at phase %d - expected %c, got %c\n", i, A14_WAVE[wave_idx], A14);
      EXPECT_EQ(A15, A15_WAVE[wave_idx], "A15 failure at phase %d - expected %c, got %c\n", i, A15_WAVE[wave_idx], A15);

      EXPECT_EQ(D00, D00_WAVE[wave_idx], "D00 failure at phase %d - expected %c, got %c\n", i, D00_WAVE[wave_idx], D00);
      EXPECT_EQ(D01, D01_WAVE[wave_idx], "D01 failure at phase %d - expected %c, got %c\n", i, D01_WAVE[wave_idx], D01);
      EXPECT_EQ(D02, D02_WAVE[wave_idx], "D02 failure at phase %d - expected %c, got %c\n", i, D02_WAVE[wave_idx], D02);
      EXPECT_EQ(D03, D03_WAVE[wave_idx], "D03 failure at phase %d - expected %c, got %c\n", i, D03_WAVE[wave_idx], D03);
      EXPECT_EQ(D04, D04_WAVE[wave_idx], "D04 failure at phase %d - expected %c, got %c\n", i, D04_WAVE[wave_idx], D04);
      EXPECT_EQ(D05, D05_WAVE[wave_idx], "D05 failure at phase %d - expected %c, got %c\n", i, D05_WAVE[wave_idx], D05);
      EXPECT_EQ(D06, D06_WAVE[wave_idx], "D06 failure at phase %d - expected %c, got %c\n", i, D06_WAVE[wave_idx], D06);
      EXPECT_EQ(D07, D07_WAVE[wave_idx], "D07 failure at phase %d - expected %c, got %c\n", i, D07_WAVE[wave_idx], D07);

      gb.next_phase();
    }
  }

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

  // TAC 100 - 2048 phases per TIMA tick
  // TAC 101 - 32 phases per TIMA tick
  // TAC 110 - 128 phases per TIMA tick
  // TAC 111 - 512 phases per TIMA tick

  LOG("Testing TIMA tick rate and reset to TMA... ");
  {
    GateBoy gb;
    gb.reset();

    gb.dbg_write(ADDR_TMA, 0x80);
    gb.dbg_write(ADDR_TIMA,0xFD);
    gb.dbg_write(ADDR_DIV, 0x00);
    gb.dbg_write(ADDR_TAC, 0b00000100);

    EXPECT_EQ(0xFD, gb.top.tim_reg.get_tima());
    gb.run(2048);
    EXPECT_EQ(0xFE, gb.top.tim_reg.get_tima());
    gb.run(2048);
    EXPECT_EQ(0xFF, gb.top.tim_reg.get_tima());
    gb.run(2048);
    EXPECT_EQ(0x80, gb.top.tim_reg.get_tima());
    gb.run(2048);
    EXPECT_EQ(0x81, gb.top.tim_reg.get_tima());
    gb.run(2048);
    if (!err) LOG_B("TAC 0b100 pass ");
  }
  {
    GateBoy gb;
    gb.reset();

    gb.dbg_write(ADDR_TMA, 0x80);
    gb.dbg_write(ADDR_TIMA,0xFD);
    gb.dbg_write(ADDR_DIV, 0x00);
    gb.dbg_write(ADDR_TAC, 0b00000101);

    EXPECT_EQ(0xFD, gb.top.tim_reg.get_tima());
    gb.run(32);
    EXPECT_EQ(0xFE, gb.top.tim_reg.get_tima());
    gb.run(32);
    EXPECT_EQ(0xFF, gb.top.tim_reg.get_tima());
    gb.run(32);
    EXPECT_EQ(0x80, gb.top.tim_reg.get_tima());
    gb.run(32);
    EXPECT_EQ(0x81, gb.top.tim_reg.get_tima());
    gb.run(32);
    if (!err) LOG_B("TAC 0b101 pass ");
  }
  {
    GateBoy gb;
    gb.reset();

    gb.dbg_write(ADDR_TMA, 0x80);
    gb.dbg_write(ADDR_TIMA,0xFD);
    gb.dbg_write(ADDR_DIV, 0x00);
    gb.dbg_write(ADDR_TAC, 0b00000110);

    EXPECT_EQ(0xFD, gb.top.tim_reg.get_tima());
    gb.run(128);
    EXPECT_EQ(0xFE, gb.top.tim_reg.get_tima());
    gb.run(128);
    EXPECT_EQ(0xFF, gb.top.tim_reg.get_tima());
    gb.run(128);
    EXPECT_EQ(0x80, gb.top.tim_reg.get_tima());
    gb.run(128);
    EXPECT_EQ(0x81, gb.top.tim_reg.get_tima());
    gb.run(128);
    if (!err) LOG_B("TAC 0b110 pass ");
  }
  {
    GateBoy gb;
    gb.reset();

    gb.dbg_write(ADDR_TMA, 0x80);
    gb.dbg_write(ADDR_TIMA,0xFD);
    gb.dbg_write(ADDR_DIV, 0x00);
    gb.dbg_write(ADDR_TAC, 0b00000111);

    EXPECT_EQ(0xFD, gb.top.tim_reg.get_tima());
    gb.run(512);
    EXPECT_EQ(0xFE, gb.top.tim_reg.get_tima());
    gb.run(512);
    EXPECT_EQ(0xFF, gb.top.tim_reg.get_tima());
    gb.run(512);
    EXPECT_EQ(0x80, gb.top.tim_reg.get_tima());
    gb.run(512);
    EXPECT_EQ(0x81, gb.top.tim_reg.get_tima());
    gb.run(512);
    if (!err) LOG_B("TAC 0b111 pass ");
  }
  if (!err) LOG("\n");


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

  uint8_t blob[256];
  for (int i = 0; i < 256; i++) {
    blob[i] = uint8_t(rand());
  }

  if (src < 0x8000) {
    uint8_t* src_data = get_flat_ptr(gb, src);
    for (int i = 0; i < 256; i++) {
      src_data[i] = blob[i];
    }
  }
  else {
    for (int i = 0; i < 256; i++) {
      gb.dbg_write(src + i, blob[i]);
    }
  }

  memset(gb.oam_ram, 0xFF, 256);

  gb.dbg_write(0xFF46, uint8_t(src >> 8));

  for (int i = 0; i < 1288; i++) gb.next_phase();

  for (int i = 0; i < 160; i++) {
    uint8_t a = blob[i];
    uint8_t b = gb.dbg_read(0xFE00 + i);
    ASSERT_EQ(a, b, "dma mismatch @ 0x%04x : expected 0x%02x, got 0x%02x", src + i, a, b);
  }

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_serial() {
  TEST_START();

  GateBoy gb;
  err += test_reg("SB", ADDR_SB, 0b11111111);
  err += test_reg("SC", ADDR_SC, 0b10000001);

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_ppu() {
  TEST_START();

  // Mucking with LCDC causes bus collisions somewhere - figure out why later
  //err += test_reg("LCDC", ADDR_LCDC, 0b11111111);

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

  {
    LOG("Checking LY increment rate... ");
    GateBoy gb;
    gb.reset();
    gb.dbg_write(ADDR_LCDC, 0x80);

    // LY should increment every 114*8 phases after LCD enable
    for (int i = 0; i < 153; i++) {
      EXPECT_EQ(i, gb.top.lcd_reg.get_ly());
      gb.run(114 * 8);
    }

    // LY is reset early on the last line, we should be at 0 now.
    EXPECT_EQ(0, gb.top.lcd_reg.get_ly());
    gb.run(114 * 8);

    // And we should be at 0 again
    EXPECT_EQ(0, gb.top.lcd_reg.get_ly());
    gb.run(114 * 8);

    // And now we should be at 1.
    EXPECT_EQ(1, gb.top.lcd_reg.get_ly());

    if (!err) LOG_B("Pass");
  }
  LOG("\n");

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

  Req req;
  req.addr = 0xFF04, req.data = 0, req.read = 1, req.write = 0;

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

  gateboy.dbg_req.addr = 0x0150;
  gateboy.dbg_req.data = 0;
  gateboy.dbg_req.read = 1;
  gateboy.dbg_req.write = 0;
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

