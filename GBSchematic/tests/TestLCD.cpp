#include "../pages/Sch_Clocks.h"
#include "../pages/Sch_BusControl.h"
#include "../pages/Sch_Resets.h"
#include "../pages/Sch_Decoder.h"
#include "../pages/Sch_Video.h"
#include "../pages/Sch_Sprites.h"
#include "../pages/Sch_Registers.h"
#include "../pages/Sch_Timer.h"
#include "../pages/Sch_LCD.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

struct TestGB {

  TestGB() {
    pwron();
  }

  void pwron() {
    memset(this, 0, sizeof(*this));
    sys_sig.pwron();
  }

  //----------------------------------------
  // Boots the chip. Currently no idea if this is actually what the boot
  // sequence really looks like, but it's vaguely plausible.

  void boot(bool fast) {
    sim(16, 20, fast);
    sys_sig.set_rst(false);
    sim(16, 20, fast);
    sys_sig.set_clk_good(true);
    sim(16, 20, fast);
    sys_sig.set_clk_req(true);
    sim(16, 20, fast);
    sys_sig.set_lcdc_en(true);
    sim(456*2*154 - 7, 20, fast);
  }

  //----------------------------------------
  // This should put the GB in the same state as the boot sequence.

  void reset() {
    pwron();

    sys_sig.reset();
    clk_sig1.reset();
    clk_sig2.reset();
    clk_reg.reset();

    rst_sig1.reset();
    rst_sig2.reset();
    rst_reg.reset();

    lcd_sig.reset();
    lcd_reg.reset();

    //bus.reset();
    //spr.reset();
    //dec.reset();
    //vid.reset();
  }

  //----------------------------------------

  // currently matches at 11 passes
  void sim_slow(int phases, int passes) {
    for (int p = 0; p < phases; p++) {
      sys_sig.next_phase();

      for (int pass = 0; pass < passes; pass++) {
        
        clk_sig1 = ClockSignals1::tick_slow(sys_sig, clk_reg);
        rst_sig1 = ResetSignals1::tick_slow(sys_sig, clk_sig1, rst_reg);
        rst_sig2 = ResetSignals2::tick_slow(sys_sig, rst_reg);
        clk_sig2 = ClockSignals2::tick_slow(rst_sig2, clk_reg);
        lcd_sig  = LCDSignals::tick_slow(clk_sig2, rst_sig2, lcd_reg);

        //----------
        TestGB prev = *this;

        ClockRegisters::tock_slow1(sys_sig, clk_sig1, clk_reg);
        ClockRegisters::tock_slow2(sys_sig, clk_sig1, clk_sig2, rst_sig2, clk_reg);
        ResetRegisters::tock_slow(sys_sig, clk_sig1, prev.rst_reg, rst_reg);
        LCDRegisters::tock_slow(clk_sig2, rst_sig2, lcd_sig, prev.lcd_reg, lcd_reg);
      }
    }
  }

  // currently matches at 4 passes
  void sim_fast(int phases, int passes) {
    for (int p = 0; p < phases; p++) {
      sys_sig.next_phase();

      for (int pass = 0; pass < passes; pass++) {
        clk_sig1 = ClockSignals1::tick_fast(sys_sig, clk_reg);
        rst_sig1 = ResetSignals1::tick_fast(sys_sig, clk_sig1, rst_reg);
        rst_sig2 = ResetSignals2::tick_fast(sys_sig, rst_reg);
        clk_sig2 = ClockSignals2::tick_fast(sys_sig, rst_sig2, clk_reg);
        lcd_sig  = LCDSignals::tick_fast(clk_sig2, rst_sig2, lcd_reg);

        //----------

        ClockRegisters::tock_fast1(sys_sig, clk_sig1, clk_reg);
        ClockRegisters::tock_fast2(sys_sig, clk_sig1, clk_sig2, rst_sig2, clk_reg);
        ResetRegisters::tock_fast(sys_sig, clk_sig1, rst_reg, rst_reg);
        LCDRegisters::tock_fast(clk_sig2, rst_sig2, lcd_sig, lcd_reg, lcd_reg);
      }
    }
  }

  void sim(int phases, int passes, bool fast) {
    fast ? sim_fast(phases, passes) : sim_slow(phases, passes);
  }

  //----------------------------------------

  SystemSignals  sys_sig;
  ClockSignals1  clk_sig1;
  ClockSignals2  clk_sig2;
  ClockRegisters clk_reg;

  ResetSignals1  rst_sig1;
  ResetSignals2  rst_sig2;
  ResetRegisters rst_reg;

  LCDSignals     lcd_sig;
  LCDRegisters   lcd_reg;

  /*
  //Sprites_tickScanner(clk_sig1, clk_sig2, prev.lcd, rst_sig1, prev.spr, spr);
  //dec.tick(bus, prev.clk_reg, BOOT_BIT, MODE_DBG2, ADDR_VALID);

  Bus     bus;
  Sprites spr;
  Decoder dec;
  Video   vid;
  */

  uint64_t alignment_pad;
};

//-----------------------------------------------------------------------------

#pragma optimize("", off)

template<typename T>
__declspec(noinline) void check_match(const T& a, const T& b) {
  uint8_t* blobA = (uint8_t*)&a;
  uint8_t* blobB = (uint8_t*)&b;

  for (int i = 0; i < sizeof(T); i++) {
    if (blobA[i] != blobB[i]) {
      printf("Mismatch at %d - 0x%02x 0x%02x", i, blobA[i], blobB[i]);
      __debugbreak();
    }
  }
}

__declspec(noinline) void check(bool x) {
  if (!x) {
    printf("check failed\n");
    __debugbreak();
  }
}

#pragma optimize("", on)

//----------------------------------------

#if 0
template<>
void check_match(const TestGB& gb1, const TestGB& gb2) {
  check_match(gb1.sys_sig,    gb2.sys_sig);
  check_match(gb1.clk_sig1,   gb2.clk_sig1);
  check_match(gb1.clk_sig2,   gb2.clk_sig2);
  check_match(gb1.clk_reg,    gb2.clk_reg);

  check_match(gb1.rst_sig1,   gb2.rst_sig1);
  check_match(gb1.rst_reg,    gb2.rst_reg);

  check_match(gb1.lcd_sig, gb2.lcd_sig);
  check_match(gb1.lcd_reg, gb2.lcd_reg);

  //check_match(gb1.bus, gb2.bus);
  //check_match(gb1.spr, gb2.spr);
  //check_match(gb1.vid, gb2.vid);
}
#endif

//-----------------------------------------------------------------------------

void sim_fast_slow(TestGB& gb1, TestGB& gb2, int phases) {
  for (int i = 0; i < phases; i++) {
    gb1.sim_slow(1, 20);
    gb2.sim_fast(1, 20);
    check_match(gb1, gb2);
  }
}

//----------------------------------------
// After boot, we should be in phase 7 and 10 phases from x=1.

void check_boot_phase_alignment(TestGB gb) {
  check(gb.sys_sig.phaseC() == 7);

  check(gb.lcd_reg.x() == 0);
  check(gb.lcd_reg.y() == 0);

  gb.sim_slow(9, 20);

  check(gb.lcd_reg.x() == 0);
  check(gb.lcd_reg.y() == 0);

  gb.sim_slow(1, 20);

  check(gb.lcd_reg.x() == 1);
  check(gb.lcd_reg.y() == 0);
}

//----------------------------------------
// Slow boot, fast boot, and reset should produce matching states

void test_reset() {
  printf("test_reset: ");

  TestGB gb1;
  TestGB gb2;

  sim_fast_slow(gb1, gb2, 16);

  gb1.sys_sig.set_rst(false);
  gb2.sys_sig.set_rst(false);
  sim_fast_slow(gb1, gb2, 16);

  gb1.sys_sig.set_clk_good(true);
  gb2.sys_sig.set_clk_good(true);
  sim_fast_slow(gb1, gb2, 16);

  gb1.sys_sig.set_clk_req(true);
  gb2.sys_sig.set_clk_req(true);
  sim_fast_slow(gb1, gb2, 16);

  gb1.sys_sig.set_lcdc_en(true);
  gb2.sys_sig.set_lcdc_en(true);

  sim_fast_slow(gb1, gb2, 456*2*154 - 7);
  check_boot_phase_alignment(gb1);
  check_boot_phase_alignment(gb2);

  TestGB gb3;
  gb3.reset();
  check_match(gb2, gb3);

  printf("pass\n");
}

//-----------------------------------------------------------------------------

void TestLCD() {

  printf("\n");
  printf("TestGB size %zd\n", sizeof(TestGB));
  test_reset();
  printf("\n");

  return;
}

//-----------------------------------------------------------------------------


