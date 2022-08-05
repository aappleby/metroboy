#include "GateBoyTests/GateBoyTests.h"

#include "CoreLib/Assembler.h"
#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/LogicBoy.h"
#include "GateBoyLib/Utils.h"
#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "CoreLib/File.h"

#include <stddef.h>
#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <math.h>

#ifdef _MSC_VER
#include <windows.h>
#endif

using namespace std;

const bool run_slow_tests = false;

//#define TEST_AUDIO
//#define TEST_MOONEYE

//#pragma optimize("", off)

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  LOG_G("GateBoyTests::main()\n");

  if (config_fastmode) {
    LOG_R("Don't run tests in fast mode!\n");
    return 0;
  }

  if (0) {
    TestResults results;
    GateBoyTests a;

    LOG_B("========== GateBoy tests ==========\n");
    const auto proto = make_unique<GateBoy>();
    results += a.test_reset   (proto.get(), 0xFF);
    results += a.test_generic         (proto.get());
    LOG_B("\n");

    LOG_G("%s: %6d expect pass\n", __FUNCTION__, results.expect_pass);
    LOG_R("%s: %6d expect fail\n", __FUNCTION__, results.expect_fail);
    LOG_G("%s: %6d test pass\n", __FUNCTION__,   results.test_pass);
    LOG_R("%s: %6d test fail\n", __FUNCTION__,   results.test_fail);

    // FIXME 20 w/o long tests, 23 with

    if (results.test_fail > 23) {
      LOG_R("\n");
      LOG_R("########################################\n");
      LOG_R("##               FAIL                 ##\n");
      LOG_R("########################################\n");
      LOG_R("\n");
    }
  }

  if (1) {
    LOG_B("========== LogicBoy tests ==========\n");
    TestResults results;
    GateBoyTests t;

    const auto proto = make_unique<LogicBoy>();
    results += t.test_reset(proto.get(), 0x01); // OK
    results += t.test_generic(proto.get());

    LOG_G("%s: %6d expect pass\n", __FUNCTION__, results.expect_pass);
    LOG_R("%s: %6d expect fail\n", __FUNCTION__, results.expect_fail);
    LOG_G("%s: %6d test pass\n", __FUNCTION__,   results.test_pass);
    LOG_R("%s: %6d test fail\n", __FUNCTION__,   results.test_fail);
    LOG_B("\n");

    if (results.test_fail > 20) {
      LOG_R("\n");
      LOG_R("########################################\n");
      LOG_R("##               FAIL                 ##\n");
      LOG_R("########################################\n");
      LOG_R("\n");
      return -1;
    }
  }

  if (0) {
    LOG_B("========== GateBoy vs LogicBoy regression tests ==========\n");
    TestResults results;
    GateBoyTests t;

    const auto proto = make_unique<GateBoyPair>(new GateBoy(), new LogicBoy());
    results += t.test_reset(proto.get(), 0x01); // OK
    results += t.test_generic(proto.get());

    LOG_G("%s: %6d expect pass\n", __FUNCTION__, results.expect_pass);
    LOG_R("%s: %6d expect fail\n", __FUNCTION__, results.expect_fail);
    LOG_G("%s: %6d test pass\n", __FUNCTION__,   results.test_pass);
    LOG_R("%s: %6d test fail\n", __FUNCTION__,   results.test_fail);
    LOG_B("\n");

    if (results.test_fail > 20) {
      LOG_R("\n");
      LOG_R("########################################\n");
      LOG_R("##               FAIL                 ##\n");
      LOG_R("########################################\n");
      LOG_R("\n");
      return -1;
    }
  }


  if (0) {
    LOG_B("========== LogicBoy regression dumps ==========\n");

    TestResults results;
    GateBoyTests t;

    auto phases = 114 * 154 * 8 * 6;

    results += t.test_regression_cart("tests/microtests/DMG/minimal.gb", phases);
    results += t.test_regression_cart("LinksAwakening.gb", phases);

    results += t.test_regression_dump("oh_jelly.dump", phases);
    results += t.test_regression_dump("oh_eyes.dump", phases);
    //results += t.test_regression_dump("sprites.dump", phases);
    //results += t.test_regression_dump("zoomer.dump", phases);
    //results += t.test_regression_dump("eyes.dump", phases); // broken because sprite mask
    //results += t.test_regression_dump("scroller.dump", phases);

    LOG_G("%s: %6d expect pass\n", __FUNCTION__, results.expect_pass);
    LOG_R("%s: %6d expect fail\n", __FUNCTION__, results.expect_fail);
    LOG_G("%s: %6d test pass\n", __FUNCTION__,   results.test_pass);
    LOG_R("%s: %6d test fail\n", __FUNCTION__,   results.test_fail);
    LOG_B("\n");

    if (results.test_fail) {
      LOG_R("\n");
      LOG_R("########################################\n");
      LOG_R("##               FAIL                 ##\n");
      LOG_R("########################################\n");
      LOG_R("\n");
      return -1;
    }
  }

  if (0) {
    TestResults results;
    GateBoyTests t;
    const auto proto = make_unique<GateBoyPair>(new GateBoy(), new LogicBoy());

    LOG_B("========== Register fuzz tests ==========\n");

    const int reps = 100;

    results += t.test_fuzz_reg(proto.get(), ADDR_LCDC, reps); // hey this actually found some mismatches

    results += t.test_fuzz_reg(proto.get(), ADDR_P1  , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_SB  , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_SC  , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_DIV , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_TIMA, reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_TMA , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_TAC , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_IF  , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_STAT, reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_SCY , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_SCX , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_LY  , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_LYC , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_DMA , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_BGP , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_OBP0, reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_OBP1, reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_WY  , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_WX  , reps);
    results += t.test_fuzz_reg(proto.get(), ADDR_IE  , reps);

    LOG_G("%s: %6d expect pass\n", __FUNCTION__, results.expect_pass);
    LOG_R("%s: %6d expect fail\n", __FUNCTION__, results.expect_fail);
    LOG_G("%s: %6d test pass\n", __FUNCTION__,   results.test_pass);
    LOG_R("%s: %6d test fail\n", __FUNCTION__,   results.test_fail);
    LOG_B("\n");

    if (results.test_fail) {
      LOG_R("\n");
      LOG_R("########################################\n");
      LOG_R("##               FAIL                 ##\n");
      LOG_R("########################################\n");
      LOG_R("\n");
      return -1;
    }
  }

  return 0;
}

//-----------------------------------------------------------------------------

GateBoyTests::GateBoyTests() : dummy_cart(Assembler::create_dummy_cart()) {
};

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_regression_cart(const char* filename, int cycles) {
  TEST_INIT("%s", filename);
  blob cart_blob;
  load_blob(filename, cart_blob);
  if (cart_blob.empty()) {
    LOG_Y("Could not load %s!\n", filename);
    TEST_DONE();
  }

  auto gb = make_unique<GateBoyPair>();
  gb->reset();

  for (int i = 0; i < cycles; i++) {
    if (i && (i % 1000000) == 0) LOG_G("Phase %d\n", i);
    if (gb->next_phase(cart_blob).is_err()) TEST_FAIL();
  }

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_regression_dump(const char* filename, int cycles) {
  TEST_INIT("%s", filename);

  BlobStream bs;
  load_blob(filename, bs.b);
  if (bs.b.empty()) {
    LOG_Y("Could not load %s!\n", filename);
    TEST_DONE();
  }

  auto gb = make_unique<GateBoyPair>();
  gb->load_raw_dump(bs);
  
  blob cart_blob = bs.rest();

  for (int i = 0; i < cycles; i++) {
    if (i && (i % 1000000) == 0) LOG_G("Phase %d\n", i);
    if (gb->next_phase(cart_blob).is_err()) TEST_FAIL();
  }

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_generic(const IGateBoy* proto) {
  TEST_INIT();

  results += test_first_op(proto);
  //results += test_bootrom(proto);
  results += test_clk(proto);
  results += test_regs(proto);
  results += test_spu_regs(proto);
  results += test_dma(proto);

  results += test_mem(proto);

  // Ext bus test only passes if flags are on and we're using the driven/pulled falgs
  if ((proto->get_flags().unwrap() & (BIT_DRIVEN | BIT_PULLED)) == (BIT_DRIVEN | BIT_PULLED)) {
    results += test_ext_bus(proto);
  }

  results += test_micro_poweron(proto);
  results += test_micro_lcden(proto);
  results += test_micro_timer(proto);

  if (run_slow_tests) {
    results += test_micro_int_vblank(proto);
  }
 
  results += test_micro_int_stat(proto);
  results += test_micro_int_timer(proto);
  results += test_micro_int_serial(proto);
  results += test_micro_int_joypad(proto);
  results += test_micro_lock_oam(proto);
  results += test_micro_lock_vram(proto);
  results += test_micro_window(proto);
  results += test_micro_ppu(proto);
  results += test_micro_dma(proto);
  results += test_micro_mbc1(proto);

#if 0
  verbose = true;
  results += test_mooneye_generic(); // some failing
  results += test_mooneye_mbc1();    // pass
  results += test_mooneye_timer();   // pass
  results += test_mooneye_ppu();     // 3 fails
#endif

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_fuzz_reg(const IGateBoy* proto, uint16_t addr, int reps) {
  TEST_INIT("test_fuzz_reg 0x%04x", addr);

  for (int j = 0; j < reps; j++) {
    uint32_t r = xorshift32(j);
    LOG_B(".");
    unique_ptr<IGateBoy> gb(proto->clone());
    gb->reset();
    for (int i = 0; i < 1000; i++) {
      r = xorshift32(r);
      if ((r % 100) >= 90) {
        r = xorshift32(r);
        auto res = gb->dbg_write(dummy_cart, addr, uint8_t(r));
        if (res.is_err()) {
          LOG_R("\ntest_fuzz_reg failed at %04d:%04d - write 0x%02x to 0x%04x\n", j, i, uint8_t(r), addr);
          //TEST_FAIL();
          break;
        }
      }
      else {
        auto res = gb->dbg_read(dummy_cart, addr);
        if (res.is_err()) {
          LOG_R("\ntest_fuzz_reg failed at %04d:%04d - read 0x%04x\n", j, i, addr);
          //TEST_FAIL();
          break;
        }
      }
    }
  }
  LOG_B("\n");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_reg(const IGateBoy* proto, const char* tag, uint16_t addr, uint8_t mask) {
  TEST_INIT("%-4s @ 0x%04x, mask 0x%02x", tag, addr, mask);

  unique_ptr<IGateBoy> gb(proto->clone());

  for (int i = 0; i < 256; i++) {
    gb->reset();

    uint8_t data_in = uint8_t(i & mask);
    auto res1 = gb->dbg_write(dummy_cart, addr, data_in);
    ASSERT_EQ(true, res1.is_ok(), "dbg_write failed @ %d", i);

    auto res2 = gb->dbg_read(dummy_cart, addr);
    ASSERT_EQ(true, res2.is_ok(), "dbg_read failed @ %d", i);
    uint8_t data_out = res2.unwrap() & mask;
    ASSERT_EQ(data_in, data_out, "reg %s @ 0x%04x: wrote 0x%02x, read 0x%02x", tag, addr, data_in, data_out);
  }

  TEST_DONE();
}

TestResults GateBoyTests::test_spu_reg(const IGateBoy* proto, const char* tag, uint16_t addr, uint8_t mask) {
  TEST_INIT("%-4s @ 0x%04x, mask 0x%02x", tag, addr, mask);

  unique_ptr<IGateBoy> gb(proto->clone());
  gb->reset();

  gb->dbg_write(dummy_cart, ADDR_NR52, 0x80).unwrap();

  for (int i = 0; i < 256; i++) {
    uint8_t data_in = uint8_t(i & mask);
    auto res1 = gb->dbg_write(dummy_cart, addr, data_in);
    ASSERT_EQ(true, res1.is_ok(), "dbg_write failed @ %d", i);

    auto res2 = gb->dbg_read(dummy_cart, addr);
    ASSERT_EQ(true, res2.is_ok(), "dbg_read failed @ %d", i);
    uint8_t data_out = res2.unwrap() & mask;
    ASSERT_EQ(data_in, data_out, "reg %s @ 0x%04x: wrote 0x%02x, read 0x%02x", tag, addr, data_in, data_out);
  }

  TEST_DONE();
}

TestResults GateBoyTests::test_regs(const IGateBoy* proto) {
  TEST_INIT();

  results += test_reg(proto, "P1",   ADDR_P1,   0b00110000);
  //results += test_reg(proto, "SB",   ADDR_SB,   0b11111111);
  //results += test_reg(proto, "SC",   ADDR_SC,   0b10000001);
  results += test_reg(proto, "TIMA", ADDR_TIMA, 0b11111111);
  results += test_reg(proto, "TMA",  ADDR_TMA,  0b11111111);
  results += test_reg(proto, "TAC",  ADDR_TAC,  0b00000111);
  results += test_reg(proto, "IF",   ADDR_IF,   0b00011111);
  results += test_reg(proto, "LCDC", ADDR_LCDC, 0b11111111);
  results += test_reg(proto, "STAT", ADDR_STAT, 0b01111000);
  results += test_reg(proto, "SCY",  ADDR_SCY,  0b11111111);
  results += test_reg(proto, "SCX",  ADDR_SCX,  0b11111111);
  //results += test_reg(proto, "LY",   ADDR_LY,   177); not writable
  results += test_reg(proto, "LYC",  ADDR_LYC,  0b11111111);
  results += test_reg(proto, "DMA",  ADDR_DMA,  0b11111111);
  results += test_reg(proto, "BGP",  ADDR_BGP,  0b11111111);
  results += test_reg(proto, "OBP0", ADDR_OBP0, 0b11111111);
  results += test_reg(proto, "OBP1", ADDR_OBP1, 0b11111111);
  results += test_reg(proto, "WY",   ADDR_WY,   0b11111111);
  results += test_reg(proto, "WX",   ADDR_WX,   0b11111111);
  results += test_reg(proto, "IE",   ADDR_IE,   0b00011111);

  TEST_DONE();
}

TestResults GateBoyTests::test_spu_regs(const IGateBoy* proto) {
  TEST_INIT();

#ifdef TEST_AUDIO

  results += test_spu_reg(proto, "NR50", ADDR_NR50, 0b11111111);
  results += test_spu_reg(proto, "NR51", ADDR_NR51, 0b11111111);
  // since this controls power to the whole sound section, we have to test it as a regular reg
  results += test_reg(proto, "NR52", ADDR_NR52, 0b10000000);

  results += test_spu_reg(proto, "NR10", ADDR_NR10, 0b01111111);
  results += test_spu_reg(proto, "NR11", ADDR_NR11, 0b11000000);
  results += test_spu_reg(proto, "NR12", ADDR_NR12, 0b11111111);
  results += test_spu_reg(proto, "NR13", ADDR_NR13, 0b00000000); // NR13 can't be read but we'll try anyway
  results += test_spu_reg(proto, "NR14", ADDR_NR14, 0b01000000);

  results += test_spu_reg(proto, "NR21", ADDR_NR21, 0b11000000);
  results += test_spu_reg(proto, "NR22", ADDR_NR22, 0b11111111);
  results += test_spu_reg(proto, "NR23", ADDR_NR23, 0b00000000); // NR23 can't be read but we'll try anyway
  results += test_spu_reg(proto, "NR24", ADDR_NR24, 0b01000000);

  results += test_spu_reg(proto, "NR30", ADDR_NR30, 0b10000000);
  results += test_spu_reg(proto, "NR31", ADDR_NR31, 0b00000000); // NR31 can't be read but we'll try anyway
  results += test_spu_reg(proto, "NR32", ADDR_NR32, 0b01100000);
  results += test_spu_reg(proto, "NR33", ADDR_NR33, 0b00000000); // NR33 can't be read but we'll try anyway
  results += test_spu_reg(proto, "NR34", ADDR_NR34, 0b01000000);

  results += test_spu_reg(proto, "NR41", ADDR_NR41, 0b00000000); // NR41 can't be read but we'll try anyway
  results += test_spu_reg(proto, "NR42", ADDR_NR42, 0b11111111);
  results += test_spu_reg(proto, "NR43", ADDR_NR43, 0b11111111);
  results += test_spu_reg(proto, "NR44", ADDR_NR44, 0b01000000);

#endif

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::diff_gb(IGateBoy* gb1, IGateBoy* gb2, uint8_t mask) {
  TEST_INIT();
  bool pass = true;
  LOG_G("State:\n")
  pass &= bit_cmp(gb1->get_state(), gb2->get_state(), mask, GateBoyState::fields);
  LOG_G("CPU:\n")
  pass &= bit_cmp(gb1->get_cpu(),   gb2->get_cpu(),   mask, GateBoyCpu::fields);
  LOG_G("Mem:\n")
  pass &= bit_cmp(gb1->get_mem(),   gb2->get_mem(),   mask);
  LOG_G("Sys:\n")
  pass &= bit_cmp(gb1->get_sys(),   gb2->get_sys(),   mask);
  LOG_G("Pins:\n")
  pass &= bit_cmp(gb1->get_pins(),  gb2->get_pins(),  mask);
  EXPECT_EQ(true, pass, "diff fail");

  TEST_DONE();
}

//-----------------------------------------------------------------------------
// reset_cart() should match dumped reset state.

TestResults GateBoyTests::test_reset(const IGateBoy* /*proto*/, uint8_t mask) {
  TEST_INIT();

  GateBoy* gb1 =  new GateBoy();
  GateBoy* gb2 =  new GateBoy();

  LOG_B("load gateboy_reset_to_cart.raw.dump\n");
  BlobStream bs;
  if (!load_blob("gateboy_reset_to_cart.raw.dump", bs.b)) {
    LOG_Y("Warning : can't load gateboy_reset_to_cart_raw.dump\n");
    TEST_DONE();
  }

  if (gb1->load_raw_dump(bs).is_err()) {
    LOG_Y("Warning : gateboy_reset_to_cart_raw.dump not valid\n");
    TEST_DONE();
  }
  LOG_G("load gateboy_reset_to_cart.raw.dump done\n");

  LOG_B("reset()\n");
  gb2->reset();
  LOG_G("reset() done\n");

  results += diff_gb(gb1, gb2, mask);

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_poweron(const IGateBoy* proto) {
  TEST_INIT();

  LOG_B("===== Stat boot state =====\n");

  results += run_microtest(proto, "poweron_stat_000.gb");
  results += run_microtest(proto, "poweron_stat_005.gb");
  results += run_microtest(proto, "poweron_stat_006.gb"); // probably due to unstable data latch
  results += run_microtest(proto, "poweron_stat_007.gb");
  results += run_microtest(proto, "poweron_stat_026.gb");
  results += run_microtest(proto, "poweron_stat_027.gb");
  results += run_microtest(proto, "poweron_stat_069.gb");
  results += run_microtest(proto, "poweron_stat_070.gb");
  results += run_microtest(proto, "poweron_stat_119.gb");
  results += run_microtest(proto, "poweron_stat_120.gb"); // failing
  results += run_microtest(proto, "poweron_stat_121.gb");
  results += run_microtest(proto, "poweron_stat_140.gb");
  results += run_microtest(proto, "poweron_stat_141.gb");
  results += run_microtest(proto, "poweron_stat_183.gb");
  results += run_microtest(proto, "poweron_stat_184.gb");
  results += run_microtest(proto, "poweron_stat_234.gb");

  results += run_microtest(proto, "poweron_stat_235.gb");

  LOG_B("===== Div state =====\n");

  results += run_microtest(proto, "poweron_div_000.gb");
  results += run_microtest(proto, "poweron_div_004.gb");
  results += run_microtest(proto, "poweron_div_005.gb");

  LOG_B("===== LY boot state =====\n");

  results += run_microtest(proto, "poweron_ly_000.gb");
  results += run_microtest(proto, "poweron_ly_119.gb");
  results += run_microtest(proto, "poweron_ly_120.gb");
  results += run_microtest(proto, "poweron_ly_233.gb");
  results += run_microtest(proto, "poweron_ly_234.gb");

  LOG_B("===== Other reg initial values =====\n");
  results += run_microtest(proto, "poweron_bgp_000.gb");
  results += run_microtest(proto, "poweron_dma_000.gb");
  results += run_microtest(proto, "poweron_if_000.gb");
  results += run_microtest(proto, "poweron_joy_000.gb");
  results += run_microtest(proto, "poweron_lcdc_000.gb");
  results += run_microtest(proto, "poweron_lyc_000.gb");
  results += run_microtest(proto, "poweron_obp0_000.gb");
  results += run_microtest(proto, "poweron_obp1_000.gb");
  //results += run_microtest(proto, "poweron_sb_000.gb");
  //results += run_microtest(proto, "poweron_sc_000.gb");
  results += run_microtest(proto, "poweron_scx_000.gb");
  results += run_microtest(proto, "poweron_scy_000.gb");
  results += run_microtest(proto, "poweron_tac_000.gb");
  results += run_microtest(proto, "poweron_tima_000.gb");
  results += run_microtest(proto, "poweron_tma_000.gb");
  results += run_microtest(proto, "poweron_wx_000.gb");
  results += run_microtest(proto, "poweron_wy_000.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_int_vblank(const IGateBoy* proto) {
  TEST_INIT();

  results += run_microtest(proto, "lcdon_halt_to_vblank_int_a.gb", true);
  results += run_microtest(proto, "lcdon_halt_to_vblank_int_b.gb", true);
  results += run_microtest(proto, "lcdon_nops_to_vblank_int_a.gb", true);
  results += run_microtest(proto, "lcdon_nops_to_vblank_int_b.gb", true);

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_int_stat(const IGateBoy* proto) {
  TEST_INIT();

  results += run_microtest(proto, "int_hblank_halt_scx0.gb"); // int fires on 822 G
  results += run_microtest(proto, "int_hblank_halt_scx1.gb"); // int fires on 824 A
  results += run_microtest(proto, "int_hblank_halt_scx2.gb"); // int fires on 826 C
  results += run_microtest(proto, "int_hblank_halt_scx3.gb"); // int fires on 828 E
  results += run_microtest(proto, "int_hblank_halt_scx4.gb"); // int fires on 830 G
  results += run_microtest(proto, "int_hblank_halt_scx5.gb"); // int fires on 832 A
  results += run_microtest(proto, "int_hblank_halt_scx6.gb"); // int fires on 834 C
  results += run_microtest(proto, "int_hblank_halt_scx7.gb"); // int fires on 836 E

  results += run_microtest(proto, "int_hblank_incs_scx0.gb"); // int fires on 822 G
  results += run_microtest(proto, "int_hblank_incs_scx1.gb"); // int fires on 824 A
  results += run_microtest(proto, "int_hblank_incs_scx2.gb"); // int fires on 826 C
  results += run_microtest(proto, "int_hblank_incs_scx3.gb"); // int fires on 828 E
  results += run_microtest(proto, "int_hblank_incs_scx4.gb"); // int fires on 830 G
  results += run_microtest(proto, "int_hblank_incs_scx5.gb"); // int fires on 832 A
  results += run_microtest(proto, "int_hblank_incs_scx6.gb"); // int fires on 834 C
  results += run_microtest(proto, "int_hblank_incs_scx7.gb"); // int fires on 836 E

  results += run_microtest(proto, "int_hblank_nops_scx0.gb"); // int fires on 822 G
  results += run_microtest(proto, "int_hblank_nops_scx1.gb"); // int fires on 824 A
  results += run_microtest(proto, "int_hblank_nops_scx2.gb"); // int fires on 826 C
  results += run_microtest(proto, "int_hblank_nops_scx3.gb"); // int fires on 828 E
  results += run_microtest(proto, "int_hblank_nops_scx4.gb"); // int fires on 830 G
  results += run_microtest(proto, "int_hblank_nops_scx5.gb"); // int fires on 832 A
  results += run_microtest(proto, "int_hblank_nops_scx6.gb"); // int fires on 834 C
  results += run_microtest(proto, "int_hblank_nops_scx7.gb"); // int fires on 836 E

  if (run_slow_tests) {
    results += run_microtest(proto, "int_vblank1_halt.gb", true); // int fires on 131602 C
    results += run_microtest(proto, "int_vblank1_incs.gb", true);
    results += run_microtest(proto, "int_vblank1_nops.gb", true);

    results += run_microtest(proto, "int_vblank2_halt.gb", true); // int fires on 131562 C
    results += run_microtest(proto, "int_vblank2_incs.gb", true);
    results += run_microtest(proto, "int_vblank2_nops.gb", true);
  }

  results += run_microtest(proto, "int_lyc_halt.gb"); // int fires on 1226 C
  results += run_microtest(proto, "int_lyc_incs.gb");
  results += run_microtest(proto, "int_lyc_nops.gb");

  results += run_microtest(proto, "int_oam_halt.gb"); // int fires on 1182 G
  results += run_microtest(proto, "int_oam_incs.gb");
  results += run_microtest(proto, "int_oam_nops.gb");

  // broken and slow
  if (run_slow_tests) {
    results += run_microtest(proto, "int_hblank_halt_bug_a.gb", true);
    results += run_microtest(proto, "int_hblank_halt_bug_b.gb", true); // failing
  }

  results += run_microtest(proto, "hblank_int_if_a.gb");
  results += run_microtest(proto, "hblank_int_if_b.gb");

  results += run_microtest(proto, "hblank_int_scx0_if_a.gb");
  results += run_microtest(proto, "hblank_int_scx0_if_b.gb");
  results += run_microtest(proto, "hblank_int_scx0_if_c.gb");
  results += run_microtest(proto, "hblank_int_scx0_if_d.gb");

  results += run_microtest(proto, "hblank_int_scx1_if_a.gb");
  results += run_microtest(proto, "hblank_int_scx1_if_b.gb");
  results += run_microtest(proto, "hblank_int_scx1_if_c.gb");
  results += run_microtest(proto, "hblank_int_scx1_if_d.gb");
  results += run_microtest(proto, "hblank_int_scx1_nops_a.gb");
  results += run_microtest(proto, "hblank_int_scx1_nops_b.gb");

  results += run_microtest(proto, "hblank_int_scx2_if_a.gb");
  results += run_microtest(proto, "hblank_int_scx2_if_b.gb");
  results += run_microtest(proto, "hblank_int_scx2_if_c.gb");
  results += run_microtest(proto, "hblank_int_scx2_if_d.gb");
  results += run_microtest(proto, "hblank_int_scx2_nops_a.gb");
  results += run_microtest(proto, "hblank_int_scx2_nops_b.gb");

  results += run_microtest(proto, "hblank_int_scx3_if_a.gb");
  results += run_microtest(proto, "hblank_int_scx3_if_b.gb");
  results += run_microtest(proto, "hblank_int_scx3_if_c.gb");
  results += run_microtest(proto, "hblank_int_scx3_if_d.gb");
  results += run_microtest(proto, "hblank_int_scx3_nops_a.gb");
  results += run_microtest(proto, "hblank_int_scx3_nops_b.gb");

  results += run_microtest(proto, "hblank_int_scx4_if_a.gb");
  results += run_microtest(proto, "hblank_int_scx4_if_b.gb");
  results += run_microtest(proto, "hblank_int_scx4_if_c.gb");
  results += run_microtest(proto, "hblank_int_scx4_if_d.gb");
  results += run_microtest(proto, "hblank_int_scx4_nops_a.gb");
  results += run_microtest(proto, "hblank_int_scx4_nops_b.gb");

  results += run_microtest(proto, "hblank_int_scx5_if_a.gb");
  results += run_microtest(proto, "hblank_int_scx5_if_b.gb");
  results += run_microtest(proto, "hblank_int_scx5_if_c.gb");
  results += run_microtest(proto, "hblank_int_scx5_if_d.gb");
  results += run_microtest(proto, "hblank_int_scx5_nops_a.gb");
  results += run_microtest(proto, "hblank_int_scx5_nops_b.gb");

  results += run_microtest(proto, "hblank_int_scx6_if_a.gb");
  results += run_microtest(proto, "hblank_int_scx6_if_b.gb");
  results += run_microtest(proto, "hblank_int_scx6_if_c.gb");
  results += run_microtest(proto, "hblank_int_scx6_if_d.gb");
  results += run_microtest(proto, "hblank_int_scx6_nops_a.gb");
  results += run_microtest(proto, "hblank_int_scx6_nops_b.gb");

  results += run_microtest(proto, "hblank_int_scx7_if_a.gb");
  results += run_microtest(proto, "hblank_int_scx7_if_b.gb");
  results += run_microtest(proto, "hblank_int_scx7_if_c.gb");
  results += run_microtest(proto, "hblank_int_scx7_if_d.gb");
  results += run_microtest(proto, "hblank_int_scx7_nops_a.gb");
  results += run_microtest(proto, "hblank_int_scx7_nops_b.gb");

  results += run_microtest(proto, "int_hblank_halt_scx0.gb");
  results += run_microtest(proto, "int_hblank_halt_scx1.gb");
  results += run_microtest(proto, "int_hblank_halt_scx2.gb");
  results += run_microtest(proto, "int_hblank_halt_scx3.gb");
  results += run_microtest(proto, "int_hblank_halt_scx4.gb");
  results += run_microtest(proto, "int_hblank_halt_scx5.gb");
  results += run_microtest(proto, "int_hblank_halt_scx6.gb");
  results += run_microtest(proto, "int_hblank_halt_scx7.gb");

  results += run_microtest(proto, "int_hblank_incs_scx0.gb");
  results += run_microtest(proto, "int_hblank_incs_scx1.gb");
  results += run_microtest(proto, "int_hblank_incs_scx2.gb");
  results += run_microtest(proto, "int_hblank_incs_scx3.gb");
  results += run_microtest(proto, "int_hblank_incs_scx4.gb");
  results += run_microtest(proto, "int_hblank_incs_scx5.gb");
  results += run_microtest(proto, "int_hblank_incs_scx6.gb");
  results += run_microtest(proto, "int_hblank_incs_scx7.gb");

  if (run_slow_tests) {
    results += run_microtest(proto, "vblank2_int_if_a.gb", true);
    results += run_microtest(proto, "vblank2_int_if_b.gb", true);
    results += run_microtest(proto, "vblank2_int_if_c.gb", true);
    results += run_microtest(proto, "vblank2_int_if_d.gb", true);
    results += run_microtest(proto, "vblank2_int_inc_sled.gb", true);
    results += run_microtest(proto, "vblank2_int_nops_a.gb", true);
    results += run_microtest(proto, "vblank2_int_nops_b.gb", true);

    results += run_microtest(proto, "vblank_int_if_a.gb", true);
    results += run_microtest(proto, "vblank_int_if_b.gb", true);
    results += run_microtest(proto, "vblank_int_if_c.gb", true);
    results += run_microtest(proto, "vblank_int_if_d.gb", true);
    results += run_microtest(proto, "vblank_int_inc_sled.gb", true);
    results += run_microtest(proto, "vblank_int_nops_a.gb", true);
    results += run_microtest(proto, "vblank_int_nops_b.gb", true);
  }

  results += run_microtest(proto, "lcdon_to_oam_int_l0.gb");
  results += run_microtest(proto, "lcdon_to_oam_int_l1.gb");
  results += run_microtest(proto, "lcdon_to_oam_int_l2.gb");

  if (run_slow_tests) {
    results += run_microtest(proto, "line_144_oam_int_a.gb", true); // pass
    results += run_microtest(proto, "line_144_oam_int_b.gb", true); // pass
    results += run_microtest(proto, "line_144_oam_int_c.gb", true); // pass
    results += run_microtest(proto, "line_144_oam_int_d.gb", true); // pass
  }

  results += run_microtest(proto, "oam_int_if_edge_a.gb"); // pass
  results += run_microtest(proto, "oam_int_if_edge_b.gb"); // pass
  results += run_microtest(proto, "oam_int_if_edge_c.gb"); // pass
  results += run_microtest(proto, "oam_int_if_edge_d.gb"); // pass
  results += run_microtest(proto, "oam_int_if_level_c.gb"); // pass
  results += run_microtest(proto, "oam_int_if_level_d.gb"); // pass
  results += run_microtest(proto, "oam_int_inc_sled.gb"); // pass
  results += run_microtest(proto, "oam_int_nops_a.gb"); // pass
  results += run_microtest(proto, "oam_int_nops_b.gb"); // pass

  results += run_microtest(proto, "lcdon_to_lyc1_int.gb");
  results += run_microtest(proto, "lcdon_to_lyc2_int.gb");
  results += run_microtest(proto, "lcdon_to_lyc3_int.gb");

  results += run_microtest(proto, "lyc1_int_if_edge_a.gb");  // pass
  results += run_microtest(proto, "lyc1_int_if_edge_b.gb");  // pass
  results += run_microtest(proto, "lyc1_int_if_edge_c.gb");  // pass
  results += run_microtest(proto, "lyc1_int_if_edge_d.gb");  // pass
  results += run_microtest(proto, "lyc1_int_nops_a.gb");     // pass
  results += run_microtest(proto, "lyc1_int_nops_b.gb");     // pass

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_int_timer(const IGateBoy* proto) {
  TEST_INIT();
  results += run_microtest(proto, "int_timer_halt.gb"); // int fires on 296 A
  results += run_microtest(proto, "int_timer_halt_div_a.gb");
  results += run_microtest(proto, "int_timer_halt_div_b.gb");
  results += run_microtest(proto, "int_timer_incs.gb");
  results += run_microtest(proto, "int_timer_nops.gb");
  results += run_microtest(proto, "int_timer_nops_div_a.gb");
  results += run_microtest(proto, "int_timer_nops_div_b.gb");
  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_int_serial(const IGateBoy* proto) {
  TEST_INIT();
  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_int_joypad(const IGateBoy* proto) {
  TEST_INIT();
  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_lcden(const IGateBoy* proto) {
  TEST_INIT();

  results += run_microtest(proto, "lcdon_to_ly1_a.gb");
  results += run_microtest(proto, "lcdon_to_ly1_b.gb");
  results += run_microtest(proto, "lcdon_to_ly2_a.gb");
  results += run_microtest(proto, "lcdon_to_ly2_b.gb");
  results += run_microtest(proto, "lcdon_to_ly3_a.gb");
  results += run_microtest(proto, "lcdon_to_ly3_b.gb");

  results += run_microtest(proto, "lcdon_to_stat0_a.gb");
  results += run_microtest(proto, "lcdon_to_stat0_b.gb");
  results += run_microtest(proto, "lcdon_to_stat0_c.gb");
  results += run_microtest(proto, "lcdon_to_stat0_d.gb");

  if (run_slow_tests) {
    results += run_microtest(proto, "lcdon_to_stat1_a.gb", true);
    results += run_microtest(proto, "lcdon_to_stat1_b.gb", true);
    results += run_microtest(proto, "lcdon_to_stat1_c.gb", true);
    results += run_microtest(proto, "lcdon_to_stat1_d.gb", true); // failing
    results += run_microtest(proto, "lcdon_to_stat1_e.gb", true);
  }

  results += run_microtest(proto, "lcdon_to_stat2_a.gb"); // failing
  results += run_microtest(proto, "lcdon_to_stat2_b.gb");
  results += run_microtest(proto, "lcdon_to_stat2_c.gb");
  results += run_microtest(proto, "lcdon_to_stat2_d.gb");
  results += run_microtest(proto, "lcdon_to_stat3_a.gb");
  results += run_microtest(proto, "lcdon_to_stat3_b.gb");
  results += run_microtest(proto, "lcdon_to_stat3_c.gb");
  results += run_microtest(proto, "lcdon_to_stat3_d.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_dma(const IGateBoy* proto) {
  TEST_INIT();
  LOG_B("===== DMA =====\n");
  results += run_microtest(proto, "dma_0x1000.gb");
  results += run_microtest(proto, "dma_0x9000.gb");
  results += run_microtest(proto, "dma_0xA000.gb");
  results += run_microtest(proto, "dma_0xC000.gb");
  results += run_microtest(proto, "dma_0xE000.gb");
  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_lock_oam(const IGateBoy* proto) {
  TEST_INIT();

  results += run_microtest(proto, "oam_read_l0_a.gb");
  results += run_microtest(proto, "oam_read_l0_b.gb");
  results += run_microtest(proto, "oam_read_l0_c.gb");
  results += run_microtest(proto, "oam_read_l0_d.gb"); // failing
  results += run_microtest(proto, "oam_read_l1_a.gb"); // failing
  results += run_microtest(proto, "oam_read_l1_b.gb");
  results += run_microtest(proto, "oam_read_l1_c.gb");
  results += run_microtest(proto, "oam_read_l1_d.gb");
  results += run_microtest(proto, "oam_read_l1_e.gb");
  results += run_microtest(proto, "oam_read_l1_f.gb"); // failing

  results += run_microtest(proto, "oam_write_l0_a.gb");
  results += run_microtest(proto, "oam_write_l0_b.gb");
  results += run_microtest(proto, "oam_write_l0_c.gb");
  results += run_microtest(proto, "oam_write_l0_d.gb"); // failing
  results += run_microtest(proto, "oam_write_l0_e.gb");

  results += run_microtest(proto, "oam_write_l1_a.gb");
  results += run_microtest(proto, "oam_write_l1_b.gb");
  results += run_microtest(proto, "oam_write_l1_c.gb"); // failing
  results += run_microtest(proto, "oam_write_l1_d.gb");
  results += run_microtest(proto, "oam_write_l1_e.gb");
  results += run_microtest(proto, "oam_write_l1_f.gb");

  results += run_microtest(proto, "lcdon_to_oam_unlock_a.gb");
  results += run_microtest(proto, "lcdon_to_oam_unlock_b.gb");
  results += run_microtest(proto, "lcdon_to_oam_unlock_c.gb");
  results += run_microtest(proto, "lcdon_to_oam_unlock_d.gb"); // failing

  results += run_microtest(proto, "poweron_oam_000.gb");
  results += run_microtest(proto, "poweron_oam_005.gb");
  results += run_microtest(proto, "poweron_oam_006.gb"); // fail, this is in the gap between hblank and scan
  results += run_microtest(proto, "poweron_oam_069.gb");
  results += run_microtest(proto, "poweron_oam_070.gb");
  results += run_microtest(proto, "poweron_oam_119.gb");
  results += run_microtest(proto, "poweron_oam_120.gb"); // fail, this is in the gap between hblank and scan
  results += run_microtest(proto, "poweron_oam_121.gb");
  results += run_microtest(proto, "poweron_oam_183.gb");
  results += run_microtest(proto, "poweron_oam_184.gb");
  results += run_microtest(proto, "poweron_oam_233.gb");
  results += run_microtest(proto, "poweron_oam_234.gb"); // fail, this is in the gap between hblank and scan
  results += run_microtest(proto, "poweron_oam_235.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_lock_vram(const IGateBoy* proto) {
  TEST_INIT();

  results += run_microtest(proto, "poweron_vram_000.gb");
  results += run_microtest(proto, "poweron_vram_025.gb");
  results += run_microtest(proto, "poweron_vram_026.gb"); // fail, this is in the gap between scan and render
  results += run_microtest(proto, "poweron_vram_069.gb");
  results += run_microtest(proto, "poweron_vram_070.gb");
  results += run_microtest(proto, "poweron_vram_139.gb");
  results += run_microtest(proto, "poweron_vram_140.gb"); // fail, this is in the gap between scan and render
  results += run_microtest(proto, "poweron_vram_183.gb");
  results += run_microtest(proto, "poweron_vram_184.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_timer(const IGateBoy* proto) {
  TEST_INIT();

  results += run_microtest(proto, "timer_tima_inc_256k_a.gb");
  results += run_microtest(proto, "timer_tima_inc_256k_b.gb");
  results += run_microtest(proto, "timer_tima_inc_256k_c.gb");
  results += run_microtest(proto, "timer_tima_inc_256k_d.gb");
  results += run_microtest(proto, "timer_tima_inc_256k_e.gb");
  results += run_microtest(proto, "timer_tima_inc_256k_f.gb");
  results += run_microtest(proto, "timer_tima_inc_256k_g.gb");
  results += run_microtest(proto, "timer_tima_inc_256k_h.gb");
  results += run_microtest(proto, "timer_tima_inc_256k_i.gb");
  results += run_microtest(proto, "timer_tima_inc_256k_j.gb");
  results += run_microtest(proto, "timer_tima_inc_256k_k.gb");

  results += run_microtest(proto, "timer_tima_reload_256k_a.gb");
  results += run_microtest(proto, "timer_tima_reload_256k_b.gb");
  results += run_microtest(proto, "timer_tima_reload_256k_c.gb");
  results += run_microtest(proto, "timer_tima_reload_256k_d.gb");
  results += run_microtest(proto, "timer_tima_reload_256k_e.gb");
  results += run_microtest(proto, "timer_tima_reload_256k_f.gb");
  results += run_microtest(proto, "timer_tima_reload_256k_g.gb");
  results += run_microtest(proto, "timer_tima_reload_256k_h.gb");
  results += run_microtest(proto, "timer_tima_reload_256k_i.gb");
  results += run_microtest(proto, "timer_tima_reload_256k_j.gb");
  results += run_microtest(proto, "timer_tima_reload_256k_k.gb");

  results += run_microtest(proto, "timer_tima_phase_a.gb");
  results += run_microtest(proto, "timer_tima_phase_b.gb");
  results += run_microtest(proto, "timer_tima_phase_c.gb");
  results += run_microtest(proto, "timer_tima_phase_d.gb");
  results += run_microtest(proto, "timer_tima_phase_e.gb");
  results += run_microtest(proto, "timer_tima_phase_f.gb");
  results += run_microtest(proto, "timer_tima_phase_g.gb");
  results += run_microtest(proto, "timer_tima_phase_h.gb");
  results += run_microtest(proto, "timer_tima_phase_i.gb");
  results += run_microtest(proto, "timer_tima_phase_j.gb");

  results += run_microtest(proto, "timer_tima_write_a.gb");
  results += run_microtest(proto, "timer_tima_write_b.gb");
  results += run_microtest(proto, "timer_tima_write_c.gb");
  results += run_microtest(proto, "timer_tima_write_d.gb");
  results += run_microtest(proto, "timer_tima_write_e.gb");
  results += run_microtest(proto, "timer_tima_write_f.gb");

  results += run_microtest(proto, "timer_div_phase_c.gb");
  results += run_microtest(proto, "timer_div_phase_d.gb");

  results += run_microtest(proto, "timer_tma_write_a.gb");
  results += run_microtest(proto, "timer_tma_write_b.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_ppu(const IGateBoy* proto) {
  TEST_INIT();

  if (run_slow_tests) {
    results += run_microtest(proto, "line_153_ly_a.gb", true);
    results += run_microtest(proto, "line_153_ly_b.gb", true);
    results += run_microtest(proto, "line_153_ly_c.gb", true);
    results += run_microtest(proto, "line_153_ly_d.gb", true);
    results += run_microtest(proto, "line_153_ly_e.gb", true);
    results += run_microtest(proto, "line_153_ly_f.gb", true);
    results += run_microtest(proto, "line_153_lyc0_int_inc_sled.gb", true);  // failing
  }

  results += run_microtest(proto, "lyc1_write_timing_a.gb");
  results += run_microtest(proto, "lyc1_write_timing_b.gb");
  results += run_microtest(proto, "lyc1_write_timing_c.gb");
  results += run_microtest(proto, "lyc1_write_timing_d.gb");

  results += run_microtest(proto, "stat_write_glitch_l0_a.gb"); // failing
  results += run_microtest(proto, "stat_write_glitch_l0_b.gb"); // failing
  results += run_microtest(proto, "stat_write_glitch_l0_c.gb");
  results += run_microtest(proto, "stat_write_glitch_l1_a.gb");
  results += run_microtest(proto, "stat_write_glitch_l1_b.gb"); // failing
  results += run_microtest(proto, "stat_write_glitch_l1_c.gb"); // failing
  results += run_microtest(proto, "stat_write_glitch_l1_d.gb");

  results += run_microtest(proto, "ppu_sprite0_scx0_b.gb");
  results += run_microtest(proto, "ppu_sprite0_scx2_a.gb");
  results += run_microtest(proto, "ppu_sprite0_scx2_b.gb");
  results += run_microtest(proto, "ppu_sprite0_scx3_a.gb");
  results += run_microtest(proto, "ppu_sprite0_scx3_b.gb"); // failing
  results += run_microtest(proto, "ppu_sprite0_scx4_a.gb");
  results += run_microtest(proto, "ppu_sprite0_scx4_b.gb");
  results += run_microtest(proto, "ppu_sprite0_scx5_a.gb");
  results += run_microtest(proto, "ppu_sprite0_scx5_b.gb");
  results += run_microtest(proto, "ppu_sprite0_scx6_a.gb");
  results += run_microtest(proto, "ppu_sprite0_scx6_b.gb");
  results += run_microtest(proto, "ppu_sprite0_scx7_a.gb");
  results += run_microtest(proto, "ppu_sprite0_scx7_b.gb"); // failing

  results += run_microtest(proto, "sprite4_0_a.gb");
  results += run_microtest(proto, "sprite4_0_b.gb");
  results += run_microtest(proto, "sprite4_1_a.gb");
  results += run_microtest(proto, "sprite4_1_b.gb");
  results += run_microtest(proto, "sprite4_2_a.gb");
  results += run_microtest(proto, "sprite4_2_b.gb");
  results += run_microtest(proto, "sprite4_3_a.gb");
  results += run_microtest(proto, "sprite4_3_b.gb");
  results += run_microtest(proto, "sprite4_4_a.gb");
  results += run_microtest(proto, "sprite4_4_b.gb");
  results += run_microtest(proto, "sprite4_5_a.gb");
  results += run_microtest(proto, "sprite4_5_b.gb");
  results += run_microtest(proto, "sprite4_6_a.gb");
  results += run_microtest(proto, "sprite4_6_b.gb");
  results += run_microtest(proto, "sprite4_7_a.gb");
  results += run_microtest(proto, "sprite4_7_b.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_window(const IGateBoy* proto) {
  TEST_INIT();

  results += run_microtest(proto, "win0_scx3_a.gb");
  results += run_microtest(proto, "win0_scx3_b.gb");
  results += run_microtest(proto, "win10_scx3_a.gb");
  results += run_microtest(proto, "win10_scx3_b.gb");

  results += run_microtest(proto, "win0_a.gb");
  results += run_microtest(proto, "win0_b.gb");
  results += run_microtest(proto, "win1_a.gb");
  results += run_microtest(proto, "win1_b.gb");
  results += run_microtest(proto, "win2_a.gb");
  results += run_microtest(proto, "win2_b.gb");
  results += run_microtest(proto, "win3_a.gb");
  results += run_microtest(proto, "win3_b.gb");
  results += run_microtest(proto, "win4_a.gb");
  results += run_microtest(proto, "win4_b.gb");
  results += run_microtest(proto, "win5_a.gb");
  results += run_microtest(proto, "win5_b.gb");
  results += run_microtest(proto, "win6_a.gb");
  results += run_microtest(proto, "win6_b.gb");
  results += run_microtest(proto, "win7_a.gb");
  results += run_microtest(proto, "win7_b.gb");
  results += run_microtest(proto, "win8_a.gb");
  results += run_microtest(proto, "win8_b.gb");
  results += run_microtest(proto, "win9_a.gb");
  results += run_microtest(proto, "win9_b.gb");

  results += run_microtest(proto, "win10_a.gb");
  results += run_microtest(proto, "win10_b.gb");
  results += run_microtest(proto, "win11_a.gb");
  results += run_microtest(proto, "win11_b.gb");
  results += run_microtest(proto, "win12_a.gb");
  results += run_microtest(proto, "win12_b.gb");
  results += run_microtest(proto, "win13_a.gb");
  results += run_microtest(proto, "win13_b.gb");
  results += run_microtest(proto, "win14_a.gb");
  results += run_microtest(proto, "win14_b.gb");
  results += run_microtest(proto, "win15_a.gb");
  results += run_microtest(proto, "win15_b.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_mbc1(const IGateBoy* proto) {
  TEST_INIT();

  results += run_microtest(proto, "mbc1_ram_banks.gb");
  results += run_microtest(proto, "mbc1_rom_banks.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::run_microtest(const IGateBoy* proto, const char* filename, bool verbose) {
  TestResults results;
  //LOG_B("- %s\n", filename);

  blob cart_blob;
  load_blob((std::string("tests/microtests/DMG/") + filename).c_str(), cart_blob);

  if (cart_blob.empty()) {
    LOG_B("%-30s ", filename);
    LOG_Y("FILE NOT FOUND\n");
    TEST_FAIL();
  }

  if (verbose) LOG_B("*%-30s ", filename);

  unique_ptr<IGateBoy> gb(proto->clone());
  gb->reset();

  int timeout = 150000 * 8;

  while(timeout--) {
    if (gb->next_phase(cart_blob).is_err()) break;
    if (gb->peek(0xFF82).unwrap()) break;
  }

  uint8_t result_a = gb->peek(0xFF80).unwrap(); // actual
  uint8_t result_b = gb->peek(0xFF81).unwrap(); // expected
  uint8_t result_c = gb->peek(0xFF82).unwrap(); // 0x01 if test passes, 0xFF if test fails

  bool pass = (result_c == 0x01) && (timeout > 0);

  if (pass) {
    if (verbose) LOG_G("%4d %4d %4d %4d PASS @ %d\n", result_a, result_b, (result_a - result_b), result_c, gb->get_sys().gb_phase_total_old);
    results.test_pass++;
    return results;
  }
  else {
    if (!verbose) LOG_B("%-30s ", filename);

    const char* reason = "ERROR";
    if      (timeout == 0)         reason = "TIMEOUT";
    else if (result_a != result_b) reason = "FAIL";
    else if (result_c == 0xFF)     reason = "FAIL";

    LOG_R("%4d %4d %4d %4d %s @ %d\n", result_a, result_b, (result_a - result_b), result_c, reason, gb->get_sys().gb_phase_total_old);
    results.test_fail++;
    return results;
  }
}

//-----------------------------------------------------------------------------
// At phase 97, the stack pointer should be 0xFFFE (top of zram)

TestResults GateBoyTests::test_first_op(const IGateBoy* proto) {
  TEST_INIT();

  unique_ptr<IGateBoy> gb(proto->clone());
  gb->reset();
  gb->run_to(dummy_cart, 105);

  EXPECT_EQ(gb->get_cpu().core.reg.sp, 0xFFFE, "first op sp wrong");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

#define EXPECT_CLK(A, B) EXPECT_EQ(bit0(A), bit(B, 7 - phase), "Clock phase mismatch, %s at phase %d", #A, phase);

TestResults GateBoyTests::test_clk(const IGateBoy* proto) {
  TEST_INIT();

  unique_ptr<IGateBoy> gb(proto->clone());
  gb->reset();
  gb->dbg_write(dummy_cart, ADDR_LCDC, 0x80);
  gb->run_phases(dummy_cart, 8);


  for (int i = 0; i < 32; i++) {
    int phase = gb->get_sys().gb_phase_total_old & 7;
    auto& clk = gb->get_state().sys_clk;
    EXPECT_CLK(clk.AFUR_ABCDxxxx.state, (uint8_t)0b11110000);
    EXPECT_CLK(clk.ALEF_xBCDExxx.state, (uint8_t)0b01111000);
    EXPECT_CLK(clk.APUK_xxCDEFxx.state, (uint8_t)0b00111100);
    EXPECT_CLK(clk.ADYK_xxxDEFGx.state, (uint8_t)0b00011110);

    EXPECT_CLK(clk.WUVU_ABxxEFxx.state, (uint8_t)0b11001100);
    EXPECT_CLK(clk.VENA_xxCDEFxx.state, (uint8_t)0b00111100);
    EXPECT_CLK(clk.WOSU_AxxDExxH.state, (uint8_t)0b10011001);

    EXPECT_CLK(clk.SIG_CPU_BOWA_Axxxxxxx.state, 0b10000000);
    EXPECT_CLK(clk.SIG_CPU_BEDO_xBCDEFGH.state, 0b01111111);
    EXPECT_CLK(clk.SIG_CPU_BEKO_ABCDxxxx.state, 0b11110000);
    EXPECT_CLK(clk.SIG_CPU_BUDE_xxxxEFGH.state, 0b00001111);
    EXPECT_CLK(clk.SIG_CPU_BOLO_ABCDEFxx.state, 0b11111100);
    EXPECT_CLK(clk.SIG_CPU_BUKE_AxxxxxGH.state, 0b10000011);
    EXPECT_CLK(clk.SIG_CPU_BOMA_xBCDEFGH.state, 0b01111111);
    EXPECT_CLK(clk.SIG_CPU_BOGA_Axxxxxxx.state, 0b10000000);
    
    // external signals are inverted
    EXPECT_CLK(gb->get_pins().sys.PIN_75_CLK_OUT.state, 0b00001111);
    
    gb->next_phase(dummy_cart);
  }

  TEST_DONE();
}

//-----------------------------------------------------------------------------

#pragma warning(disable : 4189) // unref var

char cp_ext(uint8_t state) {
  if ((state & (BIT_DRIVEN | BIT_PULLED)) == BIT_DRIVEN) return bit0(state) ? '0' : '1';
  if ((state & (BIT_DRIVEN | BIT_PULLED)) == BIT_PULLED) return bit0(state) ? 'v' : '^';
  return 'X';
}

TestResults GateBoyTests::test_ext_bus(const IGateBoy* proto) {
  TEST_INIT();

  // Check all signals for all phases of "ld (hl), a; jr -2;" with hl = 0xC003 and a = 0x55
  if (1) {
    LOG_B("Testing cram write external bus waves\n");

    const char* app = R"(
    0150:
      ld a, $55
      ld hl, $c003
      ld (hl), a
      jr -3
    )";

    Assembler as;
    as.assemble(app);
    blob cart_blob = as.link();

    unique_ptr<IGateBoy> gb(proto->clone());
    gb->reset();
    gb->run_phases(cart_blob, 121);

    // FIXME i broke this at some point...

    // Start checking each phase
    const char* CLK_WAVE = "11110000 11110000 11110000 11110000 11110000";
    const char* WRn_WAVE = "11111111 11110001 11111111 11111111 11111111";
    const char* RDn_WAVE = "00000000 01111111 00000000 00000000 00000000";
    const char* CSn_WAVE = "11111111 11000000 11111111 11111111 11111111";

    //                       0x0155   0xC003   0x0156   0x0157   0x8057
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
      const auto& pins = gb->get_pins();

      char CLK = cp_ext(pins.sys.PIN_75_CLK_OUT .state);
      char WRn = cp_ext(pins.ctrl.PIN_78_WRn.state);
      char RDn = cp_ext(pins.ctrl.PIN_79_RDn.state);
      char CSn = cp_ext(pins.ctrl.PIN_80_CSn.state);

      char A00 = cp_ext(pins.abus_lo.PIN_01_A00.state);
      char A01 = cp_ext(pins.abus_lo.PIN_02_A01.state);
      char A02 = cp_ext(pins.abus_lo.PIN_03_A02.state);
      char A03 = cp_ext(pins.abus_lo.PIN_04_A03.state);
      char A04 = cp_ext(pins.abus_lo.PIN_05_A04.state);
      char A05 = cp_ext(pins.abus_lo.PIN_06_A05.state);
      char A06 = cp_ext(pins.abus_lo.PIN_07_A06.state);
      char A07 = cp_ext(pins.abus_lo.PIN_08_A07.state);
      char A08 = cp_ext(pins.abus_hi.PIN_09_A08.state);
      char A09 = cp_ext(pins.abus_hi.PIN_10_A09.state);
      char A10 = cp_ext(pins.abus_hi.PIN_11_A10.state);
      char A11 = cp_ext(pins.abus_hi.PIN_12_A11.state);
      char A12 = cp_ext(pins.abus_hi.PIN_13_A12.state);
      char A13 = cp_ext(pins.abus_hi.PIN_14_A13.state);
      char A14 = cp_ext(pins.abus_hi.PIN_15_A14.state);
      char A15 = cp_ext(pins.abus_hi.PIN_16_A15.state);

      char D00 = cp_ext(pins.dbus.PIN_17_D00.state);
      char D01 = cp_ext(pins.dbus.PIN_18_D01.state);
      char D02 = cp_ext(pins.dbus.PIN_19_D02.state);
      char D03 = cp_ext(pins.dbus.PIN_20_D03.state);
      char D04 = cp_ext(pins.dbus.PIN_21_D04.state);
      char D05 = cp_ext(pins.dbus.PIN_22_D05.state);
      char D06 = cp_ext(pins.dbus.PIN_23_D06.state);
      char D07 = cp_ext(pins.dbus.PIN_24_D07.state);

      int wave_idx = ((i / 8) * 9) + (i % 8);

      EXPECT_EQ(CLK, CLK_WAVE[wave_idx], "CLK failure at phase %d - expected %c, got %c", i, CLK_WAVE[wave_idx], CLK);
      EXPECT_EQ(WRn, WRn_WAVE[wave_idx], "WRn failure at phase %d - expected %c, got %c", i, WRn_WAVE[wave_idx], WRn);
      EXPECT_EQ(RDn, RDn_WAVE[wave_idx], "RDn failure at phase %d - expected %c, got %c", i, RDn_WAVE[wave_idx], RDn);
      EXPECT_EQ(CSn, CSn_WAVE[wave_idx], "CSn failure at phase %d - expected %c, got %c", i, CSn_WAVE[wave_idx], CSn);

      EXPECT_EQ(A00, A00_WAVE[wave_idx], "A00 failure at phase %d - expected %c, got %c", i, A00_WAVE[wave_idx], A00);
      EXPECT_EQ(A01, A01_WAVE[wave_idx], "A01 failure at phase %d - expected %c, got %c", i, A01_WAVE[wave_idx], A01);
      EXPECT_EQ(A02, A02_WAVE[wave_idx], "A02 failure at phase %d - expected %c, got %c", i, A02_WAVE[wave_idx], A02);
      EXPECT_EQ(A03, A03_WAVE[wave_idx], "A03 failure at phase %d - expected %c, got %c", i, A03_WAVE[wave_idx], A03);
      EXPECT_EQ(A04, A04_WAVE[wave_idx], "A04 failure at phase %d - expected %c, got %c", i, A04_WAVE[wave_idx], A04);
      EXPECT_EQ(A05, A05_WAVE[wave_idx], "A05 failure at phase %d - expected %c, got %c", i, A05_WAVE[wave_idx], A05);
      EXPECT_EQ(A06, A06_WAVE[wave_idx], "A06 failure at phase %d - expected %c, got %c", i, A06_WAVE[wave_idx], A06);
      EXPECT_EQ(A07, A07_WAVE[wave_idx], "A07 failure at phase %d - expected %c, got %c", i, A07_WAVE[wave_idx], A07);
      EXPECT_EQ(A08, A08_WAVE[wave_idx], "A08 failure at phase %d - expected %c, got %c", i, A08_WAVE[wave_idx], A08);
      EXPECT_EQ(A09, A09_WAVE[wave_idx], "A09 failure at phase %d - expected %c, got %c", i, A09_WAVE[wave_idx], A09);
      EXPECT_EQ(A10, A10_WAVE[wave_idx], "A10 failure at phase %d - expected %c, got %c", i, A10_WAVE[wave_idx], A10);
      EXPECT_EQ(A11, A11_WAVE[wave_idx], "A11 failure at phase %d - expected %c, got %c", i, A11_WAVE[wave_idx], A11);
      EXPECT_EQ(A12, A12_WAVE[wave_idx], "A12 failure at phase %d - expected %c, got %c", i, A12_WAVE[wave_idx], A12);
      EXPECT_EQ(A13, A13_WAVE[wave_idx], "A13 failure at phase %d - expected %c, got %c", i, A13_WAVE[wave_idx], A13);
      EXPECT_EQ(A14, A14_WAVE[wave_idx], "A14 failure at phase %d - expected %c, got %c", i, A14_WAVE[wave_idx], A14);
      EXPECT_EQ(A15, A15_WAVE[wave_idx], "A15 failure at phase %d - expected %c, got %c", i, A15_WAVE[wave_idx], A15);

      EXPECT_EQ(D00, D00_WAVE[wave_idx], "D00 failure at phase %d - expected %c, got %c", i, D00_WAVE[wave_idx], D00);
      EXPECT_EQ(D01, D01_WAVE[wave_idx], "D01 failure at phase %d - expected %c, got %c", i, D01_WAVE[wave_idx], D01);
      EXPECT_EQ(D02, D02_WAVE[wave_idx], "D02 failure at phase %d - expected %c, got %c", i, D02_WAVE[wave_idx], D02);
      EXPECT_EQ(D03, D03_WAVE[wave_idx], "D03 failure at phase %d - expected %c, got %c", i, D03_WAVE[wave_idx], D03);
      EXPECT_EQ(D04, D04_WAVE[wave_idx], "D04 failure at phase %d - expected %c, got %c", i, D04_WAVE[wave_idx], D04);
      EXPECT_EQ(D05, D05_WAVE[wave_idx], "D05 failure at phase %d - expected %c, got %c", i, D05_WAVE[wave_idx], D05);
      EXPECT_EQ(D06, D06_WAVE[wave_idx], "D06 failure at phase %d - expected %c, got %c", i, D06_WAVE[wave_idx], D06);
      EXPECT_EQ(D07, D07_WAVE[wave_idx], "D07 failure at phase %d - expected %c, got %c", i, D07_WAVE[wave_idx], D07);

      gb->next_phase(cart_blob);
    }
  }

#if 1
  if (1) {
    LOG_B("Testing vram write external bus waves\n");

    const char* app = R"(
    0150:
      ld a, $55
      ld hl, $9777
      ld (hl), a
      jr -3
    )";

    Assembler as;
    as.assemble(app);
    blob cart_blob = as.link();

    unique_ptr<IGateBoy> gb(proto->clone());
    gb->reset();
    gb->run_phases(cart_blob, 121);

    const char* CLK_WAVE = "11110000 11110000 11110000 11110000 11110000";
    const char* WRn_WAVE = "11111111 11111111 11111111 11111111 11111111";
    const char* RDn_WAVE = "00000000 00000000 00000000 00000000 00000000";
    const char* CSn_WAVE = "11111111 11111111 11111111 11111111 11111111";

    //                       0x0155  0x8055    0x0156   0x0157  0x8057
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
      const auto& pins = gb->get_pins();

      char CLK = cp_ext(pins.sys.PIN_75_CLK_OUT. state);
      char WRn = cp_ext(pins.ctrl.PIN_78_WRn.state);
      char RDn = cp_ext(pins.ctrl.PIN_79_RDn.state);
      char CSn = cp_ext(pins.ctrl.PIN_80_CSn.state);

      char A00 = cp_ext(pins.abus_lo.PIN_01_A00.state);
      char A01 = cp_ext(pins.abus_lo.PIN_02_A01.state);
      char A02 = cp_ext(pins.abus_lo.PIN_03_A02.state);
      char A03 = cp_ext(pins.abus_lo.PIN_04_A03.state);
      char A04 = cp_ext(pins.abus_lo.PIN_05_A04.state);
      char A05 = cp_ext(pins.abus_lo.PIN_06_A05.state);
      char A06 = cp_ext(pins.abus_lo.PIN_07_A06.state);
      char A07 = cp_ext(pins.abus_lo.PIN_08_A07.state);
      char A08 = cp_ext(pins.abus_hi.PIN_09_A08.state);
      char A09 = cp_ext(pins.abus_hi.PIN_10_A09.state);
      char A10 = cp_ext(pins.abus_hi.PIN_11_A10.state);
      char A11 = cp_ext(pins.abus_hi.PIN_12_A11.state);
      char A12 = cp_ext(pins.abus_hi.PIN_13_A12.state);
      char A13 = cp_ext(pins.abus_hi.PIN_14_A13.state);
      char A14 = cp_ext(pins.abus_hi.PIN_15_A14.state);
      char A15 = cp_ext(pins.abus_hi.PIN_16_A15.state);

      char D00 = cp_ext(pins.dbus.PIN_17_D00.state);
      char D01 = cp_ext(pins.dbus.PIN_18_D01.state);
      char D02 = cp_ext(pins.dbus.PIN_19_D02.state);
      char D03 = cp_ext(pins.dbus.PIN_20_D03.state);
      char D04 = cp_ext(pins.dbus.PIN_21_D04.state);
      char D05 = cp_ext(pins.dbus.PIN_22_D05.state);
      char D06 = cp_ext(pins.dbus.PIN_23_D06.state);
      char D07 = cp_ext(pins.dbus.PIN_24_D07.state);

      int wave_idx = ((i / 8) * 9) + (i % 8);

      EXPECT_EQ(CLK, CLK_WAVE[wave_idx], "CLK failure at phase %d - expected %c, got %c", i, CLK_WAVE[wave_idx], CLK);
      EXPECT_EQ(WRn, WRn_WAVE[wave_idx], "WRn failure at phase %d - expected %c, got %c", i, WRn_WAVE[wave_idx], WRn);
      EXPECT_EQ(RDn, RDn_WAVE[wave_idx], "RDn failure at phase %d - expected %c, got %c", i, RDn_WAVE[wave_idx], RDn);
      EXPECT_EQ(CSn, CSn_WAVE[wave_idx], "CSn failure at phase %d - expected %c, got %c", i, CSn_WAVE[wave_idx], CSn);

      EXPECT_EQ(A00, A00_WAVE[wave_idx], "A00 failure at phase %d - expected %c, got %c", i, A00_WAVE[wave_idx], A00);
      EXPECT_EQ(A01, A01_WAVE[wave_idx], "A01 failure at phase %d - expected %c, got %c", i, A01_WAVE[wave_idx], A01);
      EXPECT_EQ(A02, A02_WAVE[wave_idx], "A02 failure at phase %d - expected %c, got %c", i, A02_WAVE[wave_idx], A02);
      EXPECT_EQ(A03, A03_WAVE[wave_idx], "A03 failure at phase %d - expected %c, got %c", i, A03_WAVE[wave_idx], A03);

      EXPECT_EQ(A04, A04_WAVE[wave_idx], "A04 failure at phase %d - expected %c, got %c", i, A04_WAVE[wave_idx], A04);
      EXPECT_EQ(A05, A05_WAVE[wave_idx], "A05 failure at phase %d - expected %c, got %c", i, A05_WAVE[wave_idx], A05);
      EXPECT_EQ(A06, A06_WAVE[wave_idx], "A06 failure at phase %d - expected %c, got %c", i, A06_WAVE[wave_idx], A06);
      EXPECT_EQ(A07, A07_WAVE[wave_idx], "A07 failure at phase %d - expected %c, got %c", i, A07_WAVE[wave_idx], A07);

      EXPECT_EQ(A08, A08_WAVE[wave_idx], "A08 failure at phase %d - expected %c, got %c", i, A08_WAVE[wave_idx], A08);
      EXPECT_EQ(A09, A09_WAVE[wave_idx], "A09 failure at phase %d - expected %c, got %c", i, A09_WAVE[wave_idx], A09);
      EXPECT_EQ(A10, A10_WAVE[wave_idx], "A10 failure at phase %d - expected %c, got %c", i, A10_WAVE[wave_idx], A10);
      EXPECT_EQ(A11, A11_WAVE[wave_idx], "A11 failure at phase %d - expected %c, got %c", i, A11_WAVE[wave_idx], A11);

      EXPECT_EQ(A12, A12_WAVE[wave_idx], "A12 failure at phase %d - expected %c, got %c", i, A12_WAVE[wave_idx], A12);
      EXPECT_EQ(A13, A13_WAVE[wave_idx], "A13 failure at phase %d - expected %c, got %c", i, A13_WAVE[wave_idx], A13);
      EXPECT_EQ(A14, A14_WAVE[wave_idx], "A14 failure at phase %d - expected %c, got %c", i, A14_WAVE[wave_idx], A14);
      EXPECT_EQ(A15, A15_WAVE[wave_idx], "A15 failure at phase %d - expected %c, got %c", i, A15_WAVE[wave_idx], A15);

      EXPECT_EQ(D00, D00_WAVE[wave_idx], "D00 failure at phase %d - expected %c, got %c", i, D00_WAVE[wave_idx], D00);
      EXPECT_EQ(D01, D01_WAVE[wave_idx], "D01 failure at phase %d - expected %c, got %c", i, D01_WAVE[wave_idx], D01);
      EXPECT_EQ(D02, D02_WAVE[wave_idx], "D02 failure at phase %d - expected %c, got %c", i, D02_WAVE[wave_idx], D02);
      EXPECT_EQ(D03, D03_WAVE[wave_idx], "D03 failure at phase %d - expected %c, got %c", i, D03_WAVE[wave_idx], D03);
      EXPECT_EQ(D04, D04_WAVE[wave_idx], "D04 failure at phase %d - expected %c, got %c", i, D04_WAVE[wave_idx], D04);
      EXPECT_EQ(D05, D05_WAVE[wave_idx], "D05 failure at phase %d - expected %c, got %c", i, D05_WAVE[wave_idx], D05);
      EXPECT_EQ(D06, D06_WAVE[wave_idx], "D06 failure at phase %d - expected %c, got %c", i, D06_WAVE[wave_idx], D06);
      EXPECT_EQ(D07, D07_WAVE[wave_idx], "D07 failure at phase %d - expected %c, got %c", i, D07_WAVE[wave_idx], D07);

      gb->next_phase(cart_blob);
    }
  }
#endif

#if 1
  if (1) {
    LOG_B("Testing zram write external bus waves\n");

    const char* app = R"(
    0150:
      ld a, $55
      ld hl, $FF80
      ld (hl), a
      jr -3
    )";

    Assembler as;
    as.assemble(app);
    blob cart_blob = as.link();

    unique_ptr<IGateBoy> gb(proto->clone());
    gb->reset();
    gb->run_phases(cart_blob, 121);

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
      // PIN_EXT_A00p = tp_latch(and2(PIN_CPU_ADDR_EXTp, !ADDR_VRAM), BUS_CPU_A00);

      // WRn = 1 = nand(PIN_CPU_WRp, xxxxEFGx, PIN_CPU_ADDR_EXTp);

      // RDn = 0 = (PIN_CPU_WR && PIN_CPU_ADDR_EXTp);
      // A00 = 1 = tp_latch(and2(PIN_CPU_ADDR_EXTp, !ADDR_VRAM), BUS_CPU_A00);

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
#else

    // We are not going to bother simulating the glitch as it has no effect
    // on the operation of the Game Boy

    // correct
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

#endif

    const char* D00_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D01_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^000000 ^^^^^^^^";
    const char* D02_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D03_WAVE = "^^000000 ^^^^^^^^ ^^111111 ^^111111 ^^^^^^^^";
    const char* D04_WAVE = "^^111111 ^^^^^^^^ ^^111111 ^^111111 ^^^^^^^^";
    const char* D05_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D06_WAVE = "^^111111 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";
    const char* D07_WAVE = "^^000000 ^^^^^^^^ ^^000000 ^^111111 ^^^^^^^^";

    for (int i = 0; i < 40; i++) {
      const auto& pins = gb->get_pins();

      char CLK = cp_ext(pins.sys.PIN_75_CLK_OUT .state);
      char WRn = cp_ext(pins.ctrl.PIN_78_WRn.state);
      char RDn = cp_ext(pins.ctrl.PIN_79_RDn.state);
      char CSn = cp_ext(pins.ctrl.PIN_80_CSn.state);

      char A00 = cp_ext(pins.abus_lo.PIN_01_A00.state);
      char A01 = cp_ext(pins.abus_lo.PIN_02_A01.state);
      char A02 = cp_ext(pins.abus_lo.PIN_03_A02.state);
      char A03 = cp_ext(pins.abus_lo.PIN_04_A03.state);
      char A04 = cp_ext(pins.abus_lo.PIN_05_A04.state);
      char A05 = cp_ext(pins.abus_lo.PIN_06_A05.state);
      char A06 = cp_ext(pins.abus_lo.PIN_07_A06.state);
      char A07 = cp_ext(pins.abus_lo.PIN_08_A07.state);
      char A08 = cp_ext(pins.abus_hi.PIN_09_A08.state);
      char A09 = cp_ext(pins.abus_hi.PIN_10_A09.state);
      char A10 = cp_ext(pins.abus_hi.PIN_11_A10.state);
      char A11 = cp_ext(pins.abus_hi.PIN_12_A11.state);
      char A12 = cp_ext(pins.abus_hi.PIN_13_A12.state);
      char A13 = cp_ext(pins.abus_hi.PIN_14_A13.state);
      char A14 = cp_ext(pins.abus_hi.PIN_15_A14.state);
      char A15 = cp_ext(pins.abus_hi.PIN_16_A15.state);

      char D00 = cp_ext(pins.dbus.PIN_17_D00.state);
      char D01 = cp_ext(pins.dbus.PIN_18_D01.state);
      char D02 = cp_ext(pins.dbus.PIN_19_D02.state);
      char D03 = cp_ext(pins.dbus.PIN_20_D03.state);
      char D04 = cp_ext(pins.dbus.PIN_21_D04.state);
      char D05 = cp_ext(pins.dbus.PIN_22_D05.state);
      char D06 = cp_ext(pins.dbus.PIN_23_D06.state);
      char D07 = cp_ext(pins.dbus.PIN_24_D07.state);

      int wave_idx = ((i / 8) * 9) + (i % 8);

      EXPECT_EQ(CLK, CLK_WAVE[wave_idx], "CLK failure at phase %d - expected %c, got %c", i, CLK_WAVE[wave_idx], CLK);
      EXPECT_EQ(WRn, WRn_WAVE[wave_idx], "WRn failure at phase %d - expected %c, got %c", i, WRn_WAVE[wave_idx], WRn);
      EXPECT_EQ(RDn, RDn_WAVE[wave_idx], "RDn failure at phase %d - expected %c, got %c", i, RDn_WAVE[wave_idx], RDn);
      EXPECT_EQ(CSn, CSn_WAVE[wave_idx], "CSn failure at phase %d - expected %c, got %c", i, CSn_WAVE[wave_idx], CSn);

      EXPECT_EQ(A00, A00_WAVE[wave_idx], "A00 failure at phase %d - expected %c, got %c", i, A00_WAVE[wave_idx], A00);
      EXPECT_EQ(A01, A01_WAVE[wave_idx], "A01 failure at phase %d - expected %c, got %c", i, A01_WAVE[wave_idx], A01);
      EXPECT_EQ(A02, A02_WAVE[wave_idx], "A02 failure at phase %d - expected %c, got %c", i, A02_WAVE[wave_idx], A02);
      EXPECT_EQ(A03, A03_WAVE[wave_idx], "A03 failure at phase %d - expected %c, got %c", i, A03_WAVE[wave_idx], A03);
      EXPECT_EQ(A04, A04_WAVE[wave_idx], "A04 failure at phase %d - expected %c, got %c", i, A04_WAVE[wave_idx], A04);
      EXPECT_EQ(A05, A05_WAVE[wave_idx], "A05 failure at phase %d - expected %c, got %c", i, A05_WAVE[wave_idx], A05);
      EXPECT_EQ(A06, A06_WAVE[wave_idx], "A06 failure at phase %d - expected %c, got %c", i, A06_WAVE[wave_idx], A06);
      EXPECT_EQ(A07, A07_WAVE[wave_idx], "A07 failure at phase %d - expected %c, got %c", i, A07_WAVE[wave_idx], A07);
      EXPECT_EQ(A08, A08_WAVE[wave_idx], "A08 failure at phase %d - expected %c, got %c", i, A08_WAVE[wave_idx], A08);
      EXPECT_EQ(A09, A09_WAVE[wave_idx], "A09 failure at phase %d - expected %c, got %c", i, A09_WAVE[wave_idx], A09);
      EXPECT_EQ(A10, A10_WAVE[wave_idx], "A10 failure at phase %d - expected %c, got %c", i, A10_WAVE[wave_idx], A10);
      EXPECT_EQ(A11, A11_WAVE[wave_idx], "A11 failure at phase %d - expected %c, got %c", i, A11_WAVE[wave_idx], A11);
      EXPECT_EQ(A12, A12_WAVE[wave_idx], "A12 failure at phase %d - expected %c, got %c", i, A12_WAVE[wave_idx], A12);
      EXPECT_EQ(A13, A13_WAVE[wave_idx], "A13 failure at phase %d - expected %c, got %c", i, A13_WAVE[wave_idx], A13);
      EXPECT_EQ(A14, A14_WAVE[wave_idx], "A14 failure at phase %d - expected %c, got %c", i, A14_WAVE[wave_idx], A14);
      EXPECT_EQ(A15, A15_WAVE[wave_idx], "A15 failure at phase %d - expected %c, got %c", i, A15_WAVE[wave_idx], A15);

      EXPECT_EQ(D00, D00_WAVE[wave_idx], "D00 failure at phase %d - expected %c, got %c", i, D00_WAVE[wave_idx], D00);
      EXPECT_EQ(D01, D01_WAVE[wave_idx], "D01 failure at phase %d - expected %c, got %c", i, D01_WAVE[wave_idx], D01);
      EXPECT_EQ(D02, D02_WAVE[wave_idx], "D02 failure at phase %d - expected %c, got %c", i, D02_WAVE[wave_idx], D02);
      EXPECT_EQ(D03, D03_WAVE[wave_idx], "D03 failure at phase %d - expected %c, got %c", i, D03_WAVE[wave_idx], D03);
      EXPECT_EQ(D04, D04_WAVE[wave_idx], "D04 failure at phase %d - expected %c, got %c", i, D04_WAVE[wave_idx], D04);
      EXPECT_EQ(D05, D05_WAVE[wave_idx], "D05 failure at phase %d - expected %c, got %c", i, D05_WAVE[wave_idx], D05);
      EXPECT_EQ(D06, D06_WAVE[wave_idx], "D06 failure at phase %d - expected %c, got %c", i, D06_WAVE[wave_idx], D06);
      EXPECT_EQ(D07, D07_WAVE[wave_idx], "D07 failure at phase %d - expected %c, got %c", i, D07_WAVE[wave_idx], D07);

      gb->next_phase(cart_blob);
    }
  }
#endif

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_mem(const IGateBoy* proto) {
  TEST_INIT();

  results += test_mem(proto, "ROM",  0x0000, 0x7FFF, 31,  false);
  results += test_mem(proto, "VRAM", 0x8000, 0x9FFF, 31,  true);
  results += test_mem(proto, "CRAM", 0xA000, 0xBFFF, 31,  true);
  results += test_mem(proto, "IRAM", 0xC000, 0xDFFF, 31,  true);
  results += test_mem(proto, "ERAM", 0xE000, 0xFDFF, 31,  true);
  results += test_mem(proto, "OAM",  0xFE00, 0xFEFF, 1,   true);
  results += test_mem(proto, "ZRAM", 0xFF80, 0xFFFE, 1,   true);

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults GateBoyTests::test_bootrom(const IGateBoy* proto) {
  TEST_INIT();

  unique_ptr<IGateBoy> gb(proto->clone());
  gb->poweron(true);
  gb->run_to(dummy_cart, 87);

  for (int i = 0; i < 16; i++) {
    uint8_t data_out = gb->dbg_read(dummy_cart, i).unwrap();
    EXPECT_EQ(data_out, DMG_ROM_bin[i], "bootrom @ 0x%04x = 0x%02x, expected 0x%02x", i, data_out, DMG_ROM_bin[i]);
  }

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults GateBoyTests::test_dma(const IGateBoy* proto) {
  TEST_INIT();

  for (int src = 0x0000; src < 0xFE00; src += 0x1000) {
    results += test_dma(proto, uint16_t(src));
  }

  TEST_DONE();
}

//----------------------------------------

TestResults GateBoyTests::test_dma(const IGateBoy* proto, uint16_t src) {
  TEST_INIT("0x%04x", src);

  auto test_cart = dummy_cart;

  unique_ptr<IGateBoy> gb(proto->clone());
  gb->reset();
  
  gb->set_cpu_en(false);
  
  gb->dbg_write(test_cart, ADDR_LCDC, 0);
  gb->dbg_write(test_cart, 0x0000, 0x0A); // enable mbc1 ram

  for (int i = 0; i < 256; i++) {
    uint8_t r = (uint8_t)rand();
    //uint8_t r = (uint8_t)i;
    if (src < 0x8000) {
      test_cart[src + i] = r;
    }
    else {
      gb->poke(src + i, r);
    }
    gb->poke(ADDR_OAM_BEGIN + i, 0xFF);
  }

  gb->dbg_write(test_cart, ADDR_DMA, uint8_t(src >> 8));
  gb->run_phases(test_cart, 644);

  gb->run_phases(test_cart, 644);

  for (int i = 0; i < 160; i++) {
    uint8_t src_a = src < 0x8000 ? test_cart[src + i] : gb->peek(src + i).unwrap();
    uint8_t dst_a = gb->peek(ADDR_OAM_BEGIN + i).unwrap();
    ASSERT_EQ(src_a, dst_a, "dma mismatch @ 0x%04x : expected 0x%02x, got 0x%02x", src + i, src_a, dst_a);
  }

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults GateBoyTests::test_mem(const IGateBoy* proto, const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write) {
  TEST_INIT("%-4s @ [0x%04x,0x%04x], step %3d write %d", tag, addr_start, addr_end, step, test_write);

  auto test_cart = dummy_cart;

  unique_ptr<IGateBoy> gb(proto->clone());
  
  gb->reset();
  gb->set_cpu_en(false);


  gb->dbg_write(test_cart, 0xFF50, 0x01); // disable bootrom
  gb->dbg_write(test_cart, 0xFF40, 0x00); // disable video
  gb->dbg_write(test_cart, 0x0000, 0x0A); // enable mbc1 ram

  for (uint16_t addr = addr_start; addr < addr_end; addr += step) {
    uint8_t data_wr = 0x55;
    if (test_write) {
      if (addr < 0x8000) {
        test_cart[addr] = 0;
      }
      else {
        gb->poke(addr, 0);
      }
      gb->dbg_write(test_cart, addr, data_wr);
    }
    else {
      if (addr < 0x8000) {
        test_cart[addr] = data_wr;
      }
      else {
        gb->poke(addr, data_wr);
      }
    }

    uint8_t data_rd = addr < 0x8000 ? test_cart[addr] : gb->peek(addr).unwrap();
    ASSERT_EQ(data_rd, data_wr, "WRITE FAIL addr 0x%04x : wrote 0x%02x, read 0x%02x", addr, data_wr, data_rd);
    data_rd = gb->dbg_read(test_cart, addr).unwrap();
    ASSERT_EQ(data_rd, data_wr, "READ FAIL  addr 0x%04x : wrote 0x%02x, read 0x%02x", addr, data_wr, data_rd);
  }

  for (uint16_t addr = addr_start; addr < addr_end; addr += step) {
    uint8_t data_wr = 0xAA;
    if (test_write) {
      if (addr < 0x8000) {
        test_cart[addr] = 0;
      }
      else {
        gb->poke(addr, 0);
      }
      gb->dbg_write(test_cart, addr, data_wr);
    }
    else {
      if (addr < 0x8000) {
        test_cart[addr] = data_wr;
      }
      else {
        gb->poke(addr, data_wr);
      }
    }

    uint8_t data_rd = addr < 0x8000 ? test_cart[addr] : gb->peek(addr).unwrap();
    ASSERT_EQ(data_rd, data_wr, "WRITE FAIL addr 0x%04x : wrote 0x%02x, read 0x%02x", addr, data_wr, data_rd);
    data_rd = gb->dbg_read(test_cart, addr).unwrap();
    ASSERT_EQ(data_rd, data_wr, "READ FAIL  addr 0x%04x : wrote 0x%02x, read 0x%02x", addr, data_wr, data_rd);
  }

  TEST_DONE();
}

//-----------------------------------------------------------------------------
// takes a couple minutes

TestResults GateBoyTests::test_mooneye_generic(const IGateBoy* proto) {
  TEST_INIT();

  const char* path = "tests/mooneye-gb/tests/build/acceptance/";

  results += run_mooneye_test(proto, path, "boot_div-dmgABCmgb.gb");         // p
  results += run_mooneye_test(proto, path, "boot_hwio-dmgABCmgb.gb");        // XXX sound regs
  results += run_mooneye_test(proto, path, "boot_regs-dmgABC.gb");           // p
  results += run_mooneye_test(proto, path, "add_sp_e_timing.gb");            // p
  results += run_mooneye_test(proto, path, "call_cc_timing.gb");             // p
  results += run_mooneye_test(proto, path, "call_cc_timing2.gb");            // p
  results += run_mooneye_test(proto, path, "call_timing.gb");                // p
  results += run_mooneye_test(proto, path, "call_timing2.gb");               // p
  results += run_mooneye_test(proto, path, "di_timing-GS.gb");               // p
  results += run_mooneye_test(proto, path, "div_timing.gb");                 // p
  results += run_mooneye_test(proto, path, "ei_sequence.gb");                // p
  results += run_mooneye_test(proto, path, "ei_timing.gb");                  // p
  results += run_mooneye_test(proto, path, "halt_ime0_ei.gb");               // p
  results += run_mooneye_test(proto, path, "halt_ime0_nointr_timing.gb");    // p
  results += run_mooneye_test(proto, path, "halt_ime1_timing.gb");           // p
  results += run_mooneye_test(proto, path, "halt_ime1_timing2-GS.gb");       // p
  results += run_mooneye_test(proto, path, "if_ie_registers.gb");            // p
  results += run_mooneye_test(proto, path, "intr_timing.gb");                // p
  results += run_mooneye_test(proto, path, "jp_cc_timing.gb");               // p
  results += run_mooneye_test(proto, path, "jp_timing.gb");                  // p
  results += run_mooneye_test(proto, path, "ld_hl_sp_e_timing.gb");          // p
  results += run_mooneye_test(proto, path, "oam_dma_restart.gb");            // p
  results += run_mooneye_test(proto, path, "oam_dma_start.gb");              // p
  results += run_mooneye_test(proto, path, "oam_dma_timing.gb");             // p
  results += run_mooneye_test(proto, path, "pop_timing.gb");                 // p
  results += run_mooneye_test(proto, path, "push_timing.gb");                // p
  results += run_mooneye_test(proto, path, "rapid_di_ei.gb");                // p
  results += run_mooneye_test(proto, path, "ret_cc_timing.gb");              // p
  results += run_mooneye_test(proto, path, "ret_timing.gb");                 // p
  results += run_mooneye_test(proto, path, "reti_intr_timing.gb");           // p
  results += run_mooneye_test(proto, path, "reti_timing.gb");                // p
  results += run_mooneye_test(proto, path, "rst_timing.gb");                 // p
  results += run_mooneye_test(proto, path, "bits/mem_oam.gb");               // p
  results += run_mooneye_test(proto, path, "bits/reg_f.gb");                 // p
  results += run_mooneye_test(proto, path, "bits/unused_hwio-GS.gb");        // XXX sound regs
  results += run_mooneye_test(proto, path, "instr/daa.gb");                  // p
  results += run_mooneye_test(proto, path, "interrupts/ie_push.gb");         // p
  results += run_mooneye_test(proto, path, "oam_dma/basic.gb");              // p
  results += run_mooneye_test(proto, path, "oam_dma/basic.gb");              // p
  results += run_mooneye_test(proto, path, "oam_dma/reg_read.gb");           // p
  results += run_mooneye_test(proto, path, "oam_dma/sources-GS.gb");         // p

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_mooneye_mbc1(const IGateBoy* proto) {
  TEST_INIT();

  const char* path = "tests/mooneye-gb/tests/build/emulator-only/mbc1/";

  results += run_mooneye_test(proto, path, "bits_bank1.gb"); // pass, but very slow (3 sim-sec)
  results += run_mooneye_test(proto, path, "bits_bank2.gb"); // pass, but very slow (3 sim-sec)
  results += run_mooneye_test(proto, path, "bits_mode.gb");  // pass, but very slow (3 sim-sec)
  results += run_mooneye_test(proto, path, "bits_ramg.gb");  // pass, but very slow (6 sim-sec)

  // not going to bother with multicart support for now
  //"multicart_rom_8Mb.gb",

  results += run_mooneye_test(proto, path, "ram_256Kb.gb");  // pass
  results += run_mooneye_test(proto, path, "ram_64Kb.gb");   // pass
  results += run_mooneye_test(proto, path, "rom_16Mb.gb");   // pass
  results += run_mooneye_test(proto, path, "rom_1Mb.gb");    // pass
  results += run_mooneye_test(proto, path, "rom_2Mb.gb");    // pass
  results += run_mooneye_test(proto, path, "rom_4Mb.gb");    // pass
  results += run_mooneye_test(proto, path, "rom_512Kb.gb");  // pass
  results += run_mooneye_test(proto, path, "rom_8Mb.gb");    // pass

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_mooneye_timer(const IGateBoy* proto) {
  TEST_INIT();

  const char* path = "tests/mooneye-gb/tests/build/acceptance/timer/";

  results += run_mooneye_test(proto, path, "div_write.gb");            // pass
  results += run_mooneye_test(proto, path, "rapid_toggle.gb");         // pass
  results += run_mooneye_test(proto, path, "tim00.gb");                // pass
  results += run_mooneye_test(proto, path, "tim00_div_trigger.gb");    // pass
  results += run_mooneye_test(proto, path, "tim01.gb");                // pass
  results += run_mooneye_test(proto, path, "tim01_div_trigger.gb");    // pass
  results += run_mooneye_test(proto, path, "tim10.gb");                // pass
  results += run_mooneye_test(proto, path, "tim10_div_trigger.gb");    // pass
  results += run_mooneye_test(proto, path, "tim11.gb");                // pass
  results += run_mooneye_test(proto, path, "tim11_div_trigger.gb");    // pass
  results += run_mooneye_test(proto, path, "tima_reload.gb");          // pass
  results += run_mooneye_test(proto, path, "tima_write_reloading.gb"); // pass
  results += run_mooneye_test(proto, path, "tma_write_reloading.gb");  // pass

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_mooneye_ppu(const IGateBoy* proto) {
  TEST_INIT();

  const char* path = "tests/mooneye-gb/tests/build/acceptance/ppu/";

  results += run_mooneye_test(proto, path, "hblank_ly_scx_timing-GS.gb");      // p
  results += run_mooneye_test(proto, path, "intr_1_2_timing-GS.gb");           // p
  results += run_mooneye_test(proto, path, "intr_2_0_timing.gb");              // p
  results += run_mooneye_test(proto, path, "intr_2_mode0_timing.gb");          // p
  results += run_mooneye_test(proto, path, "intr_2_mode0_timing_sprites.gb");  // dmg pass, gateboy fail
  results += run_mooneye_test(proto, path, "intr_2_mode3_timing.gb");          // p
  results += run_mooneye_test(proto, path, "intr_2_oam_ok_timing.gb");         // p
  results += run_mooneye_test(proto, path, "lcdon_timing-GS.gb");              // dmg pass, gateboy fail - this is the same as lcdon_to_stat2_a
  results += run_mooneye_test(proto, path, "lcdon_write_timing-GS.gb");        // dmg pass, gateboy fail - probably the same as lcdon_to_oam_unlock_d
  results += run_mooneye_test(proto, path, "stat_irq_blocking.gb");            // p
  results += run_mooneye_test(proto, path, "stat_lyc_onoff.gb");               // p
  results += run_mooneye_test(proto, path, "vblank_stat_intr-GS.gb");          // p

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::run_mooneye_test(const IGateBoy* proto, const char* path, const char* filename) {
  TEST_INIT();

  blob cart_blob;
  load_blob((std::string(path) + std::string(filename)).c_str(), cart_blob);

  if (cart_blob.empty()) {
    LOG_B("%-30s ", filename);
    LOG_Y("FILE NOT FOUND\n");
    TEST_FAIL();
  }

  //if (verbose) LOG_B("%-50s ", filename);

  unique_ptr<IGateBoy> gb(proto->clone());
  gb->reset();

  int timeout = 6400000; // bits_ramg is super slow

  int mcycle = 0;
  for (; mcycle < timeout; mcycle++) {
    gb->run_phases(cart_blob, 8);
    LOG_R("FIXME\n");
    debugbreak();
    //if (gb->get_state().gba.cpu.core.op == 0x40) break;
  }

  /*
  if ((gbp.gba.cpu.core.a == 0x00) && (mcycle != timeout)) {
    if (verbose) LOG_G("PASS @ %d\n", mcycle);
  }
  else {
    if (!verbose) LOG_B("%-50s ", filename);
    LOG_R("%s @ %d\n", (mcycle == timeout) ? "TIMEOUT" : "FAIL", mcycle);
    TEST_FAIL();
  }
  */

  TEST_DONE();
}

//-----------------------------------------------------------------------------


#if 0

./cgb_sound/cgb_sound.gb
./cgb_sound/rom_singles/01-registers.gb
./cgb_sound/rom_singles/02-len ctr.gb
./cgb_sound/rom_singles/03-trigger.gb
./cgb_sound/rom_singles/04-sweep.gb
./cgb_sound/rom_singles/05-sweep details.gb
./cgb_sound/rom_singles/06-overflow on trigger.gb
./cgb_sound/rom_singles/07-len sweep period sync.gb
./cgb_sound/rom_singles/08-len ctr during power.gb
./cgb_sound/rom_singles/09-wave read while on.gb
./cgb_sound/rom_singles/10-wave trigger while on.gb
./cgb_sound/rom_singles/11-regs after power.gb
./cgb_sound/rom_singles/12-wave.gb
./cpu_instrs/cpu_instrs.gb
./cpu_instrs/individual/01-special.gb
./cpu_instrs/individual/02-interrupts.gb
./cpu_instrs/individual/03-op sp,hl.gb
./cpu_instrs/individual/04-op r,imm.gb
./cpu_instrs/individual/05-op rp.gb
./cpu_instrs/individual/06-ld r,r.gb
./cpu_instrs/individual/07-jr,jp,call,ret,rst.gb
./cpu_instrs/individual/08-misc instrs.gb
./cpu_instrs/individual/09-op r,r.gb
./cpu_instrs/individual/10-bit ops.gb
./cpu_instrs/individual/11-op a,(hl).gb
./dmg_sound/dmg_sound.gb
./dmg_sound/rom_singles/01-registers.gb
./dmg_sound/rom_singles/02-len ctr.gb
./dmg_sound/rom_singles/03-trigger.gb
./dmg_sound/rom_singles/04-sweep.gb
./dmg_sound/rom_singles/05-sweep details.gb
./dmg_sound/rom_singles/06-overflow on trigger.gb
./dmg_sound/rom_singles/07-len sweep period sync.gb
./dmg_sound/rom_singles/08-len ctr during power.gb
./dmg_sound/rom_singles/09-wave read while on.gb
./dmg_sound/rom_singles/10-wave trigger while on.gb
./dmg_sound/rom_singles/11-regs after power.gb
./dmg_sound/rom_singles/12-wave write while on.gb
./halt_bug.gb
./instr_timing/instr_timing.gb
./interrupt_time/interrupt_time.gb
./mem_timing/individual/01-read_timing.gb
./mem_timing/individual/02-write_timing.gb
./mem_timing/individual/03-modify_timing.gb
./mem_timing/mem_timing.gb
./mem_timing-2/mem_timing.gb
./mem_timing-2/rom_singles/01-read_timing.gb
./mem_timing-2/rom_singles/02-write_timing.gb
./mem_timing-2/rom_singles/03-modify_timing.gb
./oam_bug/oam_bug.gb
./oam_bug/rom_singles/1-lcd_sync.gb
./oam_bug/rom_singles/2-causes.gb
./oam_bug/rom_singles/3-non_causes.gb
./oam_bug/rom_singles/4-scanline_timing.gb
./oam_bug/rom_singles/5-timing_bug.gb
./oam_bug/rom_singles/6-timing_no_bug.gb
./oam_bug/rom_singles/7-timing_effect.gb
./oam_bug/rom_singles/8-instr_effect.gb

#endif

//-----------------------------------------------------------------------------
