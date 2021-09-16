#include "GateBoyTests/GateBoyTests.h"


#include "CoreLib/Assembler.h"
#include "GateBoyLib/GateBoy.h"
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

//#define TEST_MOONEYE

#pragma optimize("", off)

//-----------------------------------------------------------------------------

TestResults test_regression_cart(blob cart_blob, int cycles, bool from_bootrom) {
  TEST_INIT();
  if (cart_blob.empty()) TEST_FAIL();

  auto gb = make_unique<GateBoy>();

  if (from_bootrom) {
    gb->reset_to_bootrom(cart_blob, true);
  }
  else {
    gb->reset_to_cart(cart_blob);
  }

  for (int i = 0; i < cycles; i++) {
    if (i && (i % 1000000) == 0) LOG_G("Phase %d\n", i);
    if (!gb->next_phase(cart_blob)) TEST_FAIL();
  }

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults test_regression_dump(blob raw_dump, int cycles) {
  TEST_INIT();
  if (raw_dump.empty()) TEST_FAIL();

  auto gb = make_unique<GateBoy>();

  gb->load_raw_dump(raw_dump);
  
  blob cart_blob;
  int cart_size = (int)raw_dump.size() - sizeof(GateBoy);
  cart_blob.resize(cart_size);
  memcpy(cart_blob.data(), raw_dump.data() + sizeof(GateBoy), cart_size);

  for (int i = 0; i < cycles; i++) {
    if (i && (i % 1000000) == 0) LOG_G("Phase %d\n", i);
    if (!gb->next_phase(cart_blob)) TEST_FAIL();
  }

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults fake_test() {
  TEST_INIT();
  LOG_B("Begin\n");
  EXPECT_EQ(0, 0, "Should pass");
  EXPECT_EQ(1, 0, "Should fail");
  LOG_B("End\n");
  TEST_DONE();
}

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  TestResults results;
  GateBoyTests t;

  //results += t.test_reset_to_bootrom();

#if 1
  LOG_G("Regression testing bootrom start\n");
  results += test_regression_cart(Assembler::create_dummy_cart(), 1000000, true);
  
  LOG_G("Regression testing Zelda startup\n");
  blob b;
  load_blob("LinksAwakening.gb", b);
  results += test_regression_cart(b, 1000000, false);
  
  //LOG_G("Regression testing Zelda intro dump\n");
  //load_blob("zelda_intro.dump", b);
  //results += test_regression_dump(b, 1000000);
  //
  //LOG_G("Regression testing SML intro dump\n");
  //load_blob("sml_intro.dump", b);
  //results += test_regression_dump(b, 1000000);

#if 0
  results += t.test_reset_cart_vs_dump();
  results += t.test_fastboot_vs_slowboot();
#endif

  results += t.test_bootrom();
  results += t.test_clk();
  results += t.test_regs();
  results += t.test_dma();

  results += t.test_mem();
  results += t.test_init();

  if (config_drive_flags) {
    results += t.test_ext_bus();
  }

  results += t.test_ppu();
  results += t.test_timer();

  results += t.test_micro_poweron();
  results += t.test_micro_lcden();
  results += t.test_micro_timer();

  if (run_slow_tests) {
    results += t.test_micro_int_vblank();
  }
 
  results += t.test_micro_int_stat();
  results += t.test_micro_int_timer();
  results += t.test_micro_int_serial();
  results += t.test_micro_int_joypad();
  results += t.test_micro_lock_oam();
  results += t.test_micro_lock_vram();
  results += t.test_micro_window();
  results += t.test_micro_ppu();
  results += t.test_micro_dma();
  results += t.test_micro_mbc1();

#if 0
  t.verbose = true;
  results += t.test_mooneye_generic(); // some failing
  results += t.test_mooneye_mbc1();    // pass
  results += t.test_mooneye_timer();   // pass
  results += t.test_mooneye_ppu();     // 3 fails
#endif

#endif

  LOG_G("%s: %6d expect pass\n", __FUNCTION__, results.expect_pass);
  LOG_R("%s: %6d expect fail\n", __FUNCTION__, results.expect_fail);
  LOG_G("%s: %6d test pass\n", __FUNCTION__,   results.test_pass);
  LOG_R("%s: %6d test fail\n", __FUNCTION__,   results.test_fail);


  if (results.test_fail > 20) {
    LOG_R("\n");
    LOG_R("########################################\n");
    LOG_R("##               FAIL                 ##\n");
    LOG_R("########################################\n");
    LOG_R("\n");
  }

  return results.test_fail != 0;
}

//-----------------------------------------------------------------------------

std::unique_ptr<IGateBoy> GateBoyTests::create_debug_gb(const blob& cart_blob, bool cpu_en) {
  auto gb = std::make_unique<GateBoy>();
  gb->reset_to_bootrom(cart_blob, true);
  gb->sys.cpu_en = cpu_en;
  return gb;
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_reg(const char* tag, uint16_t addr, uint8_t mask) {
  TEST_INIT("%-4s @ 0x%04x, mask 0x%02x", tag, addr, mask);

  blob cart_blob = Assembler::create_dummy_cart();
  auto gb = create_debug_gb(cart_blob, false);

  for (int i = 0; i < 256; i++) {
    uint8_t data_in = uint8_t(i & mask);
    gb->dbg_write(cart_blob, addr, data_in).unwrap();
    
    uint8_t data_out = gb->dbg_read(cart_blob, addr).unwrap() & mask;
    ASSERT_EQ(data_in, data_out, "reg %s @ 0x%04x: wrote 0x%02x, read 0x%02x", tag, addr, data_in, data_out);
  }

  TEST_DONE();
}

TestResults GateBoyTests::test_spu_reg(const char* tag, uint16_t addr, uint8_t mask) {
  TEST_INIT("%-4s @ 0x%04x, mask 0x%02x", tag, addr, mask);

  blob cart_blob = Assembler::create_dummy_cart();
  auto gb = create_debug_gb(cart_blob, false);
  gb->dbg_write(cart_blob, ADDR_NR52, 0x80).unwrap();

  for (int i = 0; i < 256; i++) {
    uint8_t data_in = uint8_t(i & mask);
    gb->dbg_write(cart_blob, addr, data_in).unwrap();
    uint8_t data_out = gb->dbg_read(cart_blob, addr);
    data_out &= mask;
    ASSERT_EQ(data_in, data_out, "reg %s @ 0x%04x: wrote 0x%02x, read 0x%02x", tag, addr, data_in, data_out);
  }

  TEST_DONE();
}

TestResults GateBoyTests::test_regs() {
  TEST_INIT();

  results += test_reg("P1",   ADDR_P1,   0b00110000);
  //results += test_reg("SB",   ADDR_SB,   0b11111111);
  //results += test_reg("SC",   ADDR_SC,   0b10000001);
  results += test_reg("TIMA", ADDR_TIMA, 0b11111111);
  results += test_reg("TMA",  ADDR_TMA,  0b11111111);
  results += test_reg("TAC",  ADDR_TAC,  0b00000111);
  results += test_reg("IF",   ADDR_IF,   0b00011111);
  results += test_reg("LCDC", ADDR_LCDC, 0b11111111);
  results += test_reg("STAT", ADDR_STAT, 0b01111000);
  results += test_reg("SCY",  ADDR_SCY,  0b11111111);
  results += test_reg("SCX",  ADDR_SCX,  0b11111111);
  //results += test_reg("LY",   ADDR_LY,   177); not writable
  results += test_reg("LYC",  ADDR_LYC,  0b11111111);
  results += test_reg("DMA",  ADDR_DMA,  0b11111111);
  results += test_reg("BGP",  ADDR_BGP,  0b11111111);
  results += test_reg("OBP0", ADDR_OBP0, 0b11111111);
  results += test_reg("OBP1", ADDR_OBP1, 0b11111111);
  results += test_reg("WY",   ADDR_WY,   0b11111111);
  results += test_reg("WX",   ADDR_WX,   0b11111111);
  results += test_reg("IE",   ADDR_IE,   0b00011111);

  //results += test_spu_reg("NR50", ADDR_NR50, 0b11111111);
  //results += test_spu_reg("NR51", ADDR_NR51, 0b11111111);
  //results += test_reg    ("NR52", ADDR_NR52, 0b10000000);

  TEST_DONE();
}


//-----------------------------------------------------------------------------
// Power-on reset state should be stable

TestResults GateBoyTests::test_fastboot_vs_slowboot() {
  TEST_INIT();

  blob cart_blob = Assembler::create_dummy_cart();

  LOG_B("reset_to_bootrom with fastboot = true\n");
  GateBoy gb1;
  gb1.reset_to_bootrom(cart_blob, true);
  LOG_G("reset_to_bootrom with fastboot = true done\n");

  LOG_B("reset_to_bootrom with fastboot = false\n");
  GateBoy gb2;
  gb2.reset_to_bootrom(cart_blob, false);
  LOG_G("reset_to_bootrom with fastboot = false done\n");

  // Clear the fastboot bit on the first gameboy, since that obviously won't match
  gb1.sys.fastboot = 0;
  gb2.sys.fastboot = 0;

  uint8_t mask = BIT_DATA | BIT_CLOCK | BIT_PULLED | BIT_DRIVEN | BIT_OLD | BIT_NEW;

  EXPECT_EQ(true, gb1.gb_state.diff(gb2.gb_state, mask));

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_reset_to_bootrom() {
  TEST_INIT();

  blob cart_blob = Assembler::create_dummy_cart();

  LOG_B("reset_to_bootrom1\n");
  GateBoy gb1;
  gb1.reset_to_bootrom(cart_blob, true);
  LOG_G("reset_to_bootrom1\n");

  LOG_B("reset_to_bootrom2\n");
  GateBoy gb2;
  gb2.reset_to_bootrom2(cart_blob, true);
  LOG_G("reset_to_bootrom2 done\n");


  uint8_t mask = BIT_DATA | BIT_CLOCK | BIT_PULLED | BIT_DRIVEN | BIT_OLD | BIT_NEW;

  EXPECT_EQ(true, gb1.gb_state.diff(gb2.gb_state, mask));

  TEST_DONE();
}

//-----------------------------------------------------------------------------
// reset_cart() should match dumped reset state.

TestResults GateBoyTests::test_reset_cart_vs_dump() {
  TEST_INIT();

  auto gb1 = create_debug_gb(Assembler::create_dummy_cart(), false);
  auto gb2 = create_debug_gb(Assembler::create_dummy_cart(), false);

  blob dump;
  load_blob("gateboy_post_bootrom.raw.dump", dump);

  if(!GateBoy::check_sentinel(dump)) {
    LOG_Y("Warning : gateboy_post_bootrom_raw.dump not valid\n");
    TEST_DONE();
  }
  LOG_B("gateboy_post_bootrom.raw.dump\n");
  gb1->load_raw_dump(dump);
  LOG_G("gateboy_post_bootrom.raw.dump done\n");

  LOG_B("reset_to_cart with fastboot = true\n");
  gb2->reset_to_cart(Assembler::create_dummy_cart());
  LOG_G("reset_cart done\n");

  uint8_t mask = BIT_DATA | BIT_CLOCK | BIT_PULLED | BIT_DRIVEN | BIT_OLD | BIT_NEW;

  blob blob_a;
  blob blob_b;

  gb1->get_state(blob_a);
  gb2->get_state(blob_b);

  EXPECT_EQ(true, diff_blobs(blob_a.data(), blob_b.data(), blob_a.size(), mask, gb1->get_field_info()));

  //EXPECT_EQ(true, gb1->gb_state.diff(gb2->gb_state, mask));

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_poweron() {
  TEST_INIT();

  LOG_B("===== Stat boot state =====\n");

  results += run_microtest("poweron_stat_000.gb");
  results += run_microtest("poweron_stat_005.gb");
  results += run_microtest("poweron_stat_006.gb"); // probably due to unstable data latch
  results += run_microtest("poweron_stat_007.gb");
  results += run_microtest("poweron_stat_026.gb");
  results += run_microtest("poweron_stat_027.gb");
  results += run_microtest("poweron_stat_069.gb");
  results += run_microtest("poweron_stat_070.gb");
  results += run_microtest("poweron_stat_119.gb");
  results += run_microtest("poweron_stat_120.gb"); // failing
  results += run_microtest("poweron_stat_121.gb");
  results += run_microtest("poweron_stat_140.gb");
  results += run_microtest("poweron_stat_141.gb");
  results += run_microtest("poweron_stat_183.gb");
  results += run_microtest("poweron_stat_184.gb");
  results += run_microtest("poweron_stat_234.gb");

  results += run_microtest("poweron_stat_235.gb");

  LOG_B("===== Div state =====\n");

  results += run_microtest("poweron_div_000.gb");
  results += run_microtest("poweron_div_004.gb");
  results += run_microtest("poweron_div_005.gb");

  LOG_B("===== LY boot state =====\n");

  results += run_microtest("poweron_ly_000.gb");
  results += run_microtest("poweron_ly_119.gb");
  results += run_microtest("poweron_ly_120.gb");
  results += run_microtest("poweron_ly_233.gb");
  results += run_microtest("poweron_ly_234.gb");

  LOG_B("===== Other reg initial values =====\n");
  results += run_microtest("poweron_bgp_000.gb");
  results += run_microtest("poweron_dma_000.gb");
  results += run_microtest("poweron_if_000.gb");
  results += run_microtest("poweron_joy_000.gb");
  results += run_microtest("poweron_lcdc_000.gb");
  results += run_microtest("poweron_lyc_000.gb");
  results += run_microtest("poweron_obp0_000.gb");
  results += run_microtest("poweron_obp1_000.gb");
  //results += run_microtest("poweron_sb_000.gb");
  //results += run_microtest("poweron_sc_000.gb");
  results += run_microtest("poweron_scx_000.gb");
  results += run_microtest("poweron_scy_000.gb");
  results += run_microtest("poweron_tac_000.gb");
  results += run_microtest("poweron_tima_000.gb");
  results += run_microtest("poweron_tma_000.gb");
  results += run_microtest("poweron_wx_000.gb");
  results += run_microtest("poweron_wy_000.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_int_vblank() {
  TEST_INIT();

  results += run_microtest("lcdon_halt_to_vblank_int_a.gb");
  results += run_microtest("lcdon_halt_to_vblank_int_b.gb");
  results += run_microtest("lcdon_nops_to_vblank_int_a.gb");
  results += run_microtest("lcdon_nops_to_vblank_int_b.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_int_stat() {
  TEST_INIT();

  results += run_microtest("int_hblank_halt_scx0.gb"); // int fires on 822 G
  results += run_microtest("int_hblank_halt_scx1.gb"); // int fires on 824 A
  results += run_microtest("int_hblank_halt_scx2.gb"); // int fires on 826 C
  results += run_microtest("int_hblank_halt_scx3.gb"); // int fires on 828 E
  results += run_microtest("int_hblank_halt_scx4.gb"); // int fires on 830 G
  results += run_microtest("int_hblank_halt_scx5.gb"); // int fires on 832 A
  results += run_microtest("int_hblank_halt_scx6.gb"); // int fires on 834 C
  results += run_microtest("int_hblank_halt_scx7.gb"); // int fires on 836 E

  results += run_microtest("int_hblank_incs_scx0.gb"); // int fires on 822 G
  results += run_microtest("int_hblank_incs_scx1.gb"); // int fires on 824 A
  results += run_microtest("int_hblank_incs_scx2.gb"); // int fires on 826 C
  results += run_microtest("int_hblank_incs_scx3.gb"); // int fires on 828 E
  results += run_microtest("int_hblank_incs_scx4.gb"); // int fires on 830 G
  results += run_microtest("int_hblank_incs_scx5.gb"); // int fires on 832 A
  results += run_microtest("int_hblank_incs_scx6.gb"); // int fires on 834 C
  results += run_microtest("int_hblank_incs_scx7.gb"); // int fires on 836 E

  results += run_microtest("int_hblank_nops_scx0.gb"); // int fires on 822 G
  results += run_microtest("int_hblank_nops_scx1.gb"); // int fires on 824 A
  results += run_microtest("int_hblank_nops_scx2.gb"); // int fires on 826 C
  results += run_microtest("int_hblank_nops_scx3.gb"); // int fires on 828 E
  results += run_microtest("int_hblank_nops_scx4.gb"); // int fires on 830 G
  results += run_microtest("int_hblank_nops_scx5.gb"); // int fires on 832 A
  results += run_microtest("int_hblank_nops_scx6.gb"); // int fires on 834 C
  results += run_microtest("int_hblank_nops_scx7.gb"); // int fires on 836 E

  if (run_slow_tests) {
    results += run_microtest("int_vblank1_halt.gb"); // int fires on 131602 C
    results += run_microtest("int_vblank1_incs.gb");
    results += run_microtest("int_vblank1_nops.gb");

    results += run_microtest("int_vblank2_halt.gb"); // int fires on 131562 C
    results += run_microtest("int_vblank2_incs.gb");
    results += run_microtest("int_vblank2_nops.gb");
  }

  results += run_microtest("int_lyc_halt.gb"); // int fires on 1226 C
  results += run_microtest("int_lyc_incs.gb");
  results += run_microtest("int_lyc_nops.gb");

  results += run_microtest("int_oam_halt.gb"); // int fires on 1182 G
  results += run_microtest("int_oam_incs.gb");
  results += run_microtest("int_oam_nops.gb");

  // broken and slow
  if (run_slow_tests) {
    results += run_microtest("int_hblank_halt_bug_a.gb"); // failing
    results += run_microtest("int_hblank_halt_bug_b.gb"); // failing
  }

  results += run_microtest("hblank_int_if_a.gb");
  results += run_microtest("hblank_int_if_b.gb");

  results += run_microtest("hblank_int_scx0_if_a.gb");
  results += run_microtest("hblank_int_scx0_if_b.gb");
  results += run_microtest("hblank_int_scx0_if_c.gb");
  results += run_microtest("hblank_int_scx0_if_d.gb");

  results += run_microtest("hblank_int_scx1_if_a.gb");
  results += run_microtest("hblank_int_scx1_if_b.gb");
  results += run_microtest("hblank_int_scx1_if_c.gb");
  results += run_microtest("hblank_int_scx1_if_d.gb");
  results += run_microtest("hblank_int_scx1_nops_a.gb");
  results += run_microtest("hblank_int_scx1_nops_b.gb");

  results += run_microtest("hblank_int_scx2_if_a.gb");
  results += run_microtest("hblank_int_scx2_if_b.gb");
  results += run_microtest("hblank_int_scx2_if_c.gb");
  results += run_microtest("hblank_int_scx2_if_d.gb");
  results += run_microtest("hblank_int_scx2_nops_a.gb");
  results += run_microtest("hblank_int_scx2_nops_b.gb");

  results += run_microtest("hblank_int_scx3_if_a.gb");
  results += run_microtest("hblank_int_scx3_if_b.gb");
  results += run_microtest("hblank_int_scx3_if_c.gb");
  results += run_microtest("hblank_int_scx3_if_d.gb");
  results += run_microtest("hblank_int_scx3_nops_a.gb");
  results += run_microtest("hblank_int_scx3_nops_b.gb");

  results += run_microtest("hblank_int_scx4_if_a.gb");
  results += run_microtest("hblank_int_scx4_if_b.gb");
  results += run_microtest("hblank_int_scx4_if_c.gb");
  results += run_microtest("hblank_int_scx4_if_d.gb");
  results += run_microtest("hblank_int_scx4_nops_a.gb");
  results += run_microtest("hblank_int_scx4_nops_b.gb");

  results += run_microtest("hblank_int_scx5_if_a.gb");
  results += run_microtest("hblank_int_scx5_if_b.gb");
  results += run_microtest("hblank_int_scx5_if_c.gb");
  results += run_microtest("hblank_int_scx5_if_d.gb");
  results += run_microtest("hblank_int_scx5_nops_a.gb");
  results += run_microtest("hblank_int_scx5_nops_b.gb");

  results += run_microtest("hblank_int_scx6_if_a.gb");
  results += run_microtest("hblank_int_scx6_if_b.gb");
  results += run_microtest("hblank_int_scx6_if_c.gb");
  results += run_microtest("hblank_int_scx6_if_d.gb");
  results += run_microtest("hblank_int_scx6_nops_a.gb");
  results += run_microtest("hblank_int_scx6_nops_b.gb");

  results += run_microtest("hblank_int_scx7_if_a.gb");
  results += run_microtest("hblank_int_scx7_if_b.gb");
  results += run_microtest("hblank_int_scx7_if_c.gb");
  results += run_microtest("hblank_int_scx7_if_d.gb");
  results += run_microtest("hblank_int_scx7_nops_a.gb");
  results += run_microtest("hblank_int_scx7_nops_b.gb");

  results += run_microtest("int_hblank_halt_scx0.gb");
  results += run_microtest("int_hblank_halt_scx1.gb");
  results += run_microtest("int_hblank_halt_scx2.gb");
  results += run_microtest("int_hblank_halt_scx3.gb");
  results += run_microtest("int_hblank_halt_scx4.gb");
  results += run_microtest("int_hblank_halt_scx5.gb");
  results += run_microtest("int_hblank_halt_scx6.gb");
  results += run_microtest("int_hblank_halt_scx7.gb");

  results += run_microtest("int_hblank_incs_scx0.gb");
  results += run_microtest("int_hblank_incs_scx1.gb");
  results += run_microtest("int_hblank_incs_scx2.gb");
  results += run_microtest("int_hblank_incs_scx3.gb");
  results += run_microtest("int_hblank_incs_scx4.gb");
  results += run_microtest("int_hblank_incs_scx5.gb");
  results += run_microtest("int_hblank_incs_scx6.gb");
  results += run_microtest("int_hblank_incs_scx7.gb");

  if (run_slow_tests) {
    results += run_microtest("vblank2_int_if_a.gb");
    results += run_microtest("vblank2_int_if_b.gb");
    results += run_microtest("vblank2_int_if_c.gb");
    results += run_microtest("vblank2_int_if_d.gb");
    results += run_microtest("vblank2_int_inc_sled.gb");
    results += run_microtest("vblank2_int_nops_a.gb");
    results += run_microtest("vblank2_int_nops_b.gb");

    results += run_microtest("vblank_int_if_a.gb");
    results += run_microtest("vblank_int_if_b.gb");
    results += run_microtest("vblank_int_if_c.gb");
    results += run_microtest("vblank_int_if_d.gb");
    results += run_microtest("vblank_int_inc_sled.gb");
    results += run_microtest("vblank_int_nops_a.gb");
    results += run_microtest("vblank_int_nops_b.gb");
  }

  results += run_microtest("lcdon_to_oam_int_l0.gb");
  results += run_microtest("lcdon_to_oam_int_l1.gb");
  results += run_microtest("lcdon_to_oam_int_l2.gb");

  if (run_slow_tests) {
    results += run_microtest("line_144_oam_int_a.gb"); // pass
    results += run_microtest("line_144_oam_int_b.gb"); // pass
    results += run_microtest("line_144_oam_int_c.gb"); // pass
    results += run_microtest("line_144_oam_int_d.gb"); // pass
  }

  results += run_microtest("oam_int_if_edge_a.gb"); // pass
  results += run_microtest("oam_int_if_edge_b.gb"); // pass
  results += run_microtest("oam_int_if_edge_c.gb"); // pass
  results += run_microtest("oam_int_if_edge_d.gb"); // pass
  results += run_microtest("oam_int_if_level_c.gb"); // pass
  results += run_microtest("oam_int_if_level_d.gb"); // pass
  results += run_microtest("oam_int_inc_sled.gb"); // pass
  results += run_microtest("oam_int_nops_a.gb"); // pass
  results += run_microtest("oam_int_nops_b.gb"); // pass

  results += run_microtest("lcdon_to_lyc1_int.gb");
  results += run_microtest("lcdon_to_lyc2_int.gb");
  results += run_microtest("lcdon_to_lyc3_int.gb");

  results += run_microtest("lyc1_int_if_edge_a.gb");  // pass
  results += run_microtest("lyc1_int_if_edge_b.gb");  // pass
  results += run_microtest("lyc1_int_if_edge_c.gb");  // pass
  results += run_microtest("lyc1_int_if_edge_d.gb");  // pass
  results += run_microtest("lyc1_int_nops_a.gb");     // pass
  results += run_microtest("lyc1_int_nops_b.gb");     // pass

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_int_timer() {
  TEST_INIT();
  results += run_microtest("int_timer_halt.gb"); // int fires on 296 A
  results += run_microtest("int_timer_halt_div_a.gb");
  results += run_microtest("int_timer_halt_div_b.gb");
  results += run_microtest("int_timer_incs.gb");
  results += run_microtest("int_timer_nops.gb");
  results += run_microtest("int_timer_nops_div_a.gb");
  results += run_microtest("int_timer_nops_div_b.gb");
  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_int_serial() {
  TEST_INIT();
  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_int_joypad() {
  TEST_INIT();
  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_lcden() {
  TEST_INIT();

  results += run_microtest("lcdon_to_ly1_a.gb");
  results += run_microtest("lcdon_to_ly1_b.gb");
  results += run_microtest("lcdon_to_ly2_a.gb");
  results += run_microtest("lcdon_to_ly2_b.gb");
  results += run_microtest("lcdon_to_ly3_a.gb");
  results += run_microtest("lcdon_to_ly3_b.gb");

  results += run_microtest("lcdon_to_stat0_a.gb");
  results += run_microtest("lcdon_to_stat0_b.gb");
  results += run_microtest("lcdon_to_stat0_c.gb");
  results += run_microtest("lcdon_to_stat0_d.gb");

  if (run_slow_tests) {
    results += run_microtest("lcdon_to_stat1_a.gb");
    results += run_microtest("lcdon_to_stat1_b.gb");
    results += run_microtest("lcdon_to_stat1_c.gb");
    results += run_microtest("lcdon_to_stat1_d.gb"); // failing
    results += run_microtest("lcdon_to_stat1_e.gb");
  }

  results += run_microtest("lcdon_to_stat2_a.gb"); // failing
  results += run_microtest("lcdon_to_stat2_b.gb");
  results += run_microtest("lcdon_to_stat2_c.gb");
  results += run_microtest("lcdon_to_stat2_d.gb");
  results += run_microtest("lcdon_to_stat3_a.gb");
  results += run_microtest("lcdon_to_stat3_b.gb");
  results += run_microtest("lcdon_to_stat3_c.gb");
  results += run_microtest("lcdon_to_stat3_d.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_dma() {
  TEST_INIT();
  LOG_B("===== DMA =====\n");
  results += run_microtest("dma_0x1000.gb");
  results += run_microtest("dma_0x9000.gb");
  results += run_microtest("dma_0xA000.gb");
  results += run_microtest("dma_0xC000.gb");
  results += run_microtest("dma_0xE000.gb");
  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_lock_oam() {
  TEST_INIT();

  results += run_microtest("oam_read_l0_a.gb");
  results += run_microtest("oam_read_l0_b.gb");
  results += run_microtest("oam_read_l0_c.gb");
  results += run_microtest("oam_read_l0_d.gb"); // failing
  results += run_microtest("oam_read_l1_a.gb"); // failing
  results += run_microtest("oam_read_l1_b.gb");
  results += run_microtest("oam_read_l1_c.gb");
  results += run_microtest("oam_read_l1_d.gb");
  results += run_microtest("oam_read_l1_e.gb");
  results += run_microtest("oam_read_l1_f.gb"); // failing

  results += run_microtest("oam_write_l0_a.gb");
  results += run_microtest("oam_write_l0_b.gb");
  results += run_microtest("oam_write_l0_c.gb");
  results += run_microtest("oam_write_l0_d.gb"); // failing
  results += run_microtest("oam_write_l0_e.gb");

  results += run_microtest("oam_write_l1_a.gb");
  results += run_microtest("oam_write_l1_b.gb");
  results += run_microtest("oam_write_l1_c.gb"); // failing
  results += run_microtest("oam_write_l1_d.gb");
  results += run_microtest("oam_write_l1_e.gb");
  results += run_microtest("oam_write_l1_f.gb");

  results += run_microtest("lcdon_to_oam_unlock_a.gb");
  results += run_microtest("lcdon_to_oam_unlock_b.gb");
  results += run_microtest("lcdon_to_oam_unlock_c.gb");
  results += run_microtest("lcdon_to_oam_unlock_d.gb"); // failing

  results += run_microtest("poweron_oam_000.gb");
  results += run_microtest("poweron_oam_005.gb");
  results += run_microtest("poweron_oam_006.gb"); // fail, this is in the gap between hblank and scan
  results += run_microtest("poweron_oam_069.gb");
  results += run_microtest("poweron_oam_070.gb");
  results += run_microtest("poweron_oam_119.gb");
  results += run_microtest("poweron_oam_120.gb"); // fail, this is in the gap between hblank and scan
  results += run_microtest("poweron_oam_121.gb");
  results += run_microtest("poweron_oam_183.gb");
  results += run_microtest("poweron_oam_184.gb");
  results += run_microtest("poweron_oam_233.gb");
  results += run_microtest("poweron_oam_234.gb"); // fail, this is in the gap between hblank and scan
  results += run_microtest("poweron_oam_235.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_lock_vram() {
  TEST_INIT();

  results += run_microtest("poweron_vram_000.gb");
  results += run_microtest("poweron_vram_025.gb");
  results += run_microtest("poweron_vram_026.gb"); // fail, this is in the gap between scan and render
  results += run_microtest("poweron_vram_069.gb");
  results += run_microtest("poweron_vram_070.gb");
  results += run_microtest("poweron_vram_139.gb");
  results += run_microtest("poweron_vram_140.gb"); // fail, this is in the gap between scan and render
  results += run_microtest("poweron_vram_183.gb");
  results += run_microtest("poweron_vram_184.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_timer() {
  TEST_INIT();

  results += run_microtest("timer_tima_inc_256k_a.gb");
  results += run_microtest("timer_tima_inc_256k_b.gb");
  results += run_microtest("timer_tima_inc_256k_c.gb");
  results += run_microtest("timer_tima_inc_256k_d.gb");
  results += run_microtest("timer_tima_inc_256k_e.gb");
  results += run_microtest("timer_tima_inc_256k_f.gb");
  results += run_microtest("timer_tima_inc_256k_g.gb");
  results += run_microtest("timer_tima_inc_256k_h.gb");
  results += run_microtest("timer_tima_inc_256k_i.gb");
  results += run_microtest("timer_tima_inc_256k_j.gb");
  results += run_microtest("timer_tima_inc_256k_k.gb");

  results += run_microtest("timer_tima_reload_256k_a.gb");
  results += run_microtest("timer_tima_reload_256k_b.gb");
  results += run_microtest("timer_tima_reload_256k_c.gb");
  results += run_microtest("timer_tima_reload_256k_d.gb");
  results += run_microtest("timer_tima_reload_256k_e.gb");
  results += run_microtest("timer_tima_reload_256k_f.gb");
  results += run_microtest("timer_tima_reload_256k_g.gb");
  results += run_microtest("timer_tima_reload_256k_h.gb");
  results += run_microtest("timer_tima_reload_256k_i.gb");
  results += run_microtest("timer_tima_reload_256k_j.gb");
  results += run_microtest("timer_tima_reload_256k_k.gb");

  results += run_microtest("timer_tima_phase_a.gb");
  results += run_microtest("timer_tima_phase_b.gb");
  results += run_microtest("timer_tima_phase_c.gb");
  results += run_microtest("timer_tima_phase_d.gb");
  results += run_microtest("timer_tima_phase_e.gb");
  results += run_microtest("timer_tima_phase_f.gb");
  results += run_microtest("timer_tima_phase_g.gb");
  results += run_microtest("timer_tima_phase_h.gb");
  results += run_microtest("timer_tima_phase_i.gb");
  results += run_microtest("timer_tima_phase_j.gb");

  results += run_microtest("timer_tima_write_a.gb");
  results += run_microtest("timer_tima_write_b.gb");
  results += run_microtest("timer_tima_write_c.gb");
  results += run_microtest("timer_tima_write_d.gb");
  results += run_microtest("timer_tima_write_e.gb");
  results += run_microtest("timer_tima_write_f.gb");

  results += run_microtest("timer_div_phase_c.gb");
  results += run_microtest("timer_div_phase_d.gb");

  results += run_microtest("timer_tma_write_a.gb");
  results += run_microtest("timer_tma_write_b.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_ppu() {
  TEST_INIT();

  if (run_slow_tests) {
    results += run_microtest("line_153_ly_a.gb");
    results += run_microtest("line_153_ly_b.gb");
    results += run_microtest("line_153_ly_c.gb");
    results += run_microtest("line_153_ly_d.gb");
    results += run_microtest("line_153_ly_e.gb");
    results += run_microtest("line_153_ly_f.gb");
    results += run_microtest("line_153_lyc0_int_inc_sled.gb");  // failing
  }

  results += run_microtest("lyc1_write_timing_a.gb");
  results += run_microtest("lyc1_write_timing_b.gb");
  results += run_microtest("lyc1_write_timing_c.gb");
  results += run_microtest("lyc1_write_timing_d.gb");

  results += run_microtest("stat_write_glitch_l0_a.gb"); // failing
  results += run_microtest("stat_write_glitch_l0_b.gb"); // failing
  results += run_microtest("stat_write_glitch_l0_c.gb");
  results += run_microtest("stat_write_glitch_l1_a.gb");
  results += run_microtest("stat_write_glitch_l1_b.gb"); // failing
  results += run_microtest("stat_write_glitch_l1_c.gb"); // failing
  results += run_microtest("stat_write_glitch_l1_d.gb");

  results += run_microtest("ppu_sprite0_scx0_b.gb");
  results += run_microtest("ppu_sprite0_scx2_a.gb");
  results += run_microtest("ppu_sprite0_scx2_b.gb");
  results += run_microtest("ppu_sprite0_scx3_a.gb");
  results += run_microtest("ppu_sprite0_scx3_b.gb"); // failing
  results += run_microtest("ppu_sprite0_scx4_a.gb");
  results += run_microtest("ppu_sprite0_scx4_b.gb");
  results += run_microtest("ppu_sprite0_scx5_a.gb");
  results += run_microtest("ppu_sprite0_scx5_b.gb");
  results += run_microtest("ppu_sprite0_scx6_a.gb");
  results += run_microtest("ppu_sprite0_scx6_b.gb");
  results += run_microtest("ppu_sprite0_scx7_a.gb");
  results += run_microtest("ppu_sprite0_scx7_b.gb"); // failing

  results += run_microtest("sprite4_0_a.gb");
  results += run_microtest("sprite4_0_b.gb");
  results += run_microtest("sprite4_1_a.gb");
  results += run_microtest("sprite4_1_b.gb");
  results += run_microtest("sprite4_2_a.gb");
  results += run_microtest("sprite4_2_b.gb");
  results += run_microtest("sprite4_3_a.gb");
  results += run_microtest("sprite4_3_b.gb");
  results += run_microtest("sprite4_4_a.gb");
  results += run_microtest("sprite4_4_b.gb");
  results += run_microtest("sprite4_5_a.gb");
  results += run_microtest("sprite4_5_b.gb");
  results += run_microtest("sprite4_6_a.gb");
  results += run_microtest("sprite4_6_b.gb");
  results += run_microtest("sprite4_7_a.gb");
  results += run_microtest("sprite4_7_b.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_window() {
  TEST_INIT();

  results += run_microtest("win0_scx3_a.gb");
  results += run_microtest("win0_scx3_b.gb");
  results += run_microtest("win10_scx3_a.gb");
  results += run_microtest("win10_scx3_b.gb");

  results += run_microtest("win0_a.gb");
  results += run_microtest("win0_b.gb");
  results += run_microtest("win1_a.gb");
  results += run_microtest("win1_b.gb");
  results += run_microtest("win2_a.gb");
  results += run_microtest("win2_b.gb");
  results += run_microtest("win3_a.gb");
  results += run_microtest("win3_b.gb");
  results += run_microtest("win4_a.gb");
  results += run_microtest("win4_b.gb");
  results += run_microtest("win5_a.gb");
  results += run_microtest("win5_b.gb");
  results += run_microtest("win6_a.gb");
  results += run_microtest("win6_b.gb");
  results += run_microtest("win7_a.gb");
  results += run_microtest("win7_b.gb");
  results += run_microtest("win8_a.gb");
  results += run_microtest("win8_b.gb");
  results += run_microtest("win9_a.gb");
  results += run_microtest("win9_b.gb");

  results += run_microtest("win10_a.gb");
  results += run_microtest("win10_b.gb");
  results += run_microtest("win11_a.gb");
  results += run_microtest("win11_b.gb");
  results += run_microtest("win12_a.gb");
  results += run_microtest("win12_b.gb");
  results += run_microtest("win13_a.gb");
  results += run_microtest("win13_b.gb");
  results += run_microtest("win14_a.gb");
  results += run_microtest("win14_b.gb");
  results += run_microtest("win15_a.gb");
  results += run_microtest("win15_b.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_micro_mbc1() {
  TEST_INIT();

  results += run_microtest("mbc1_ram_banks.gb");
  results += run_microtest("mbc1_rom_banks.gb");

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::run_microtest(const char* filename) {
  TestResults results;

  blob cart_blob;
  load_blob((std::string("tests/microtests/DMG/") + filename).c_str(), cart_blob);

  if (cart_blob.empty()) {
    LOG_B("%-30s ", filename);
    LOG_Y("FILE NOT FOUND\n");
    TEST_FAIL();
  }

  if (verbose) LOG_B("%-30s ", filename);

  auto gb = create_debug_gb(cart_blob, true);
  gb->reset_to_cart(cart_blob);

  int timeout = 150000 * 8;

  while(timeout--) {
    if (!gb->next_phase(cart_blob)) break;
    if (gb->peek(cart_blob, 0xFF82).unwrap()) break;
  }

  uint8_t result_a = gb->peek(cart_blob, 0xFF80); // actual
  uint8_t result_b = gb->peek(cart_blob, 0xFF81); // expected
  uint8_t result_c = gb->peek(cart_blob, 0xFF82); // 0x01 if test passes, 0xFF if test fails

  bool pass = (result_c == 0x01) && (timeout > 0);

  if (pass) {
    if (verbose) LOG_G("%4d %4d %4d %4d PASS @ %d\n", result_a, result_b, (result_a - result_b), result_c, gb->phase_total());
    results.test_pass++;
    return results;
  }
  else {
    if (!verbose) LOG_B("%-30s ", filename);

    const char* reason = "ERROR";
    if      (timeout == 0)         reason = "TIMEOUT";
    else if (result_a != result_b) reason = "MISMATCH";
    else if (result_c == 0xFF)     reason = "FAIL";

    LOG_R("%4d %4d %4d %4d %s @ %d\n", result_a, result_b, (result_a - result_b), result_c, reason, gb->phase_total());
    results.test_fail++;
    return results;
  }
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_init() {
  TEST_INIT();

  blob cart_blob = Assembler::create_dummy_cart();
  auto gb = create_debug_gb(cart_blob, false);

  LOG_G("Checking reg flags\n");

  LOG_G("Checking mem\n");
  // Mem should be clear
  //for (int i = 0; i < 8192; i++)  ASSERT_EQ(0, gb->mem.cart_ram[i]);
  //for (int i = 0; i < 8192; i++)  ASSERT_EQ(0, gb->mem.int_ram[i]);

  for (int i = 0; i < 8192; i++) {
    ASSERT_EQ(0, gb->peek(cart_blob, 0xA000).unwrap());
    ASSERT_EQ(0, gb->peek(cart_blob, 0xC000).unwrap());
  }

  // Framebuffer should be 0x04 (yellow) except for the first pixel, which
  // always gets written to because XONA_LCDCENn is 0 at boot

  LOG_G("Checking framebuffer\n");
  auto fb = gb->get_framebuffer();
  for (int i = 1; i < 160*144; i++) {
    ASSERT_EQ(4, fb[i], "bad framebuffer at %d\n", i);
  }

  LOG_G("Checking reg values\n");
  
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_P1  ).unwrap(), 0xCF, "Bad P1 reset_states value");   // CF after bootrom
  //EXPECT_EQ(gbp.dbg_read(cart_blob, ADDR_SB  ).unwrap(), 0x00, "Bad SB reset_states value");   // 00 after bootrom
  //EXPECT_EQ(gbp.dbg_read(cart_blob, ADDR_SC  ).unwrap(), 0x7E, "Bad SC reset_states value");   // 7E after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_DIV ).unwrap(), 0x00, "Bad DIV reset_states value");  // AB after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_TIMA).unwrap(), 0x00, "Bad TIMA reset_states value"); // 00 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_TMA ).unwrap(), 0x00, "Bad TMA reset_states value");  // 00 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_TAC ).unwrap(), 0xF8, "Bad TAC reset_states value");  // F8 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_IF  ).unwrap(), 0xE0, "Bad IF reset_states value");   // E1 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_LCDC).unwrap(), 0x00, "Bad LCDC reset_states value"); // 91 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_STAT).unwrap(), 0x80, "Bad STAT reset value");        // 85 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_SCY ).unwrap(), 0x00, "Bad SCY reset_states value");  // 00 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_SCX ).unwrap(), 0x00, "Bad SCX reset_states value");  // 00 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_LY  ).unwrap(), 0x00, "Bad LY reset_states value");   // 00 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_LYC ).unwrap(), 0x00, "Bad LYC reset_states value");  // 00 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_DMA ).unwrap(), 0xFF, "Bad DMA reset_states value");  // FF after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_BGP ).unwrap(), 0xFF, "Bad BGP reset_states value");  // FC after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_OBP0).unwrap(), 0xFF, "Bad OBP0 reset_states value"); // 9F after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_OBP1).unwrap(), 0xFF, "Bad OBP1 reset_states value"); // FF after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_WY  ).unwrap(), 0x00, "Bad WY reset_states value");   // 00 after bootrom
  EXPECT_EQ(gb->dbg_read(cart_blob, ADDR_WX  ).unwrap(), 0x00, "Bad WX reset_states value");   // 00 after bootrom

  TEST_DONE();
}

//-----------------------------------------------------------------------------

#define EXPECT_CLK(A, B) EXPECT_EQ(bit(A), get_bit(B, 7 - phase), "Clock phase mismatch, %s at phase %d", #A, phase);

TestResults GateBoyTests::test_clk() {
  TEST_INIT();

  blob cart_blob = Assembler::create_dummy_cart();

  auto gb = new GateBoy();
  gb->reset_to_bootrom(cart_blob, true);
  gb->sys.cpu_en = false;
  gb->dbg_write(cart_blob, ADDR_LCDC, 0x80);
  gb->run_phases(cart_blob, 8);

  auto& clk = gb->gb_state.sys_clk;

  for (int i = 0; i < 32; i++) {
    int phase = gb->sys.phase_total & 7;
    EXPECT_CLK(clk.AFUR_xxxxEFGH.qp_old(), (uint8_t)0b00001111);
    EXPECT_CLK(clk.ALEF_AxxxxFGH.qp_old(), (uint8_t)0b10000111);
    EXPECT_CLK(clk.APUK_ABxxxxGH.qp_old(), (uint8_t)0b11000011);
    EXPECT_CLK(clk.ADYK_ABCxxxxH.qp_old(), (uint8_t)0b11100001);

    EXPECT_CLK(clk.WUVU_ABxxEFxx.qp_old(), (uint8_t)0b11001100);
    EXPECT_CLK(clk.VENA_xxCDEFxx.qp_old(), (uint8_t)0b00111100);
    EXPECT_CLK(clk.WOSU_AxxDExxH.qp_old(), (uint8_t)0b10011001);

    EXPECT_CLK(gb->gb_state.sys_clk.SIG_CPU_BOWA_Axxxxxxx.out_old(), 0b10000000);
    EXPECT_CLK(gb->gb_state.sys_clk.SIG_CPU_BEDO_xBCDEFGH.out_old(), 0b01111111);
    EXPECT_CLK(gb->gb_state.sys_clk.SIG_CPU_BEKO_ABCDxxxx.out_old(), 0b11110000);
    EXPECT_CLK(gb->gb_state.sys_clk.SIG_CPU_BUDE_xxxxEFGH.out_old(), 0b00001111);
    EXPECT_CLK(gb->gb_state.sys_clk.SIG_CPU_BOLO_ABCDEFxx.out_old(), 0b11111100);
    EXPECT_CLK(gb->gb_state.sys_clk.SIG_CPU_BUKE_AxxxxxGH.out_old(), 0b10000011);
    EXPECT_CLK(gb->gb_state.sys_clk.SIG_CPU_BOMA_xBCDEFGH.out_old(), 0b01111111);
    EXPECT_CLK(gb->gb_state.sys_clk.SIG_CPU_BOGA_Axxxxxxx.out_old(), 0b10000000);
    EXPECT_CLK(gb->gb_state.sys_clk.PIN_75_CLK_OUT.qp_ext_old(),    0b11110000);
    gb->next_phase(cart_blob);
  }

  TEST_DONE();
}

//-----------------------------------------------------------------------------

#pragma warning(disable : 4189) // unref var

char cp_ext(uint8_t state) {
  if ((state & (BIT_DRIVEN | BIT_PULLED)) == BIT_DRIVEN) return bit(state) ? '0' : '1';
  if ((state & (BIT_DRIVEN | BIT_PULLED)) == BIT_PULLED) return bit(state) ? 'v' : '^';
  return 'X';
}

TestResults GateBoyTests::test_ext_bus() {
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

    GateBoyPair gbp;
    gbp.reset_to_cart(cart_blob);
    gbp.run_phases(cart_blob, 120);

#if 1
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
      char CLK = cp_ext(gbp.gba.gb_state.sys_clk.PIN_75_CLK_OUT .state);
      char WRn = cp_ext(gbp.gba.gb_state.ext_ctrl.PIN_78_WRn.state);
      char RDn = cp_ext(gbp.gba.gb_state.ext_ctrl.PIN_79_RDn.state);
      char CSn = cp_ext(gbp.gba.gb_state.ext_ctrl.PIN_80_CSn.state);

      char A00 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_01_A00.state);
      char A01 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_02_A01.state);
      char A02 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_03_A02.state);
      char A03 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_04_A03.state);
      char A04 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_05_A04.state);
      char A05 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_06_A05.state);
      char A06 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_07_A06.state);
      char A07 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_08_A07.state);
      char A08 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_09_A08.state);
      char A09 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_10_A09.state);
      char A10 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_11_A10.state);
      char A11 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_12_A11.state);
      char A12 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_13_A12.state);
      char A13 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_14_A13.state);
      char A14 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_15_A14.state);
      char A15 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_16_A15.state);

      char D00 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_17_D00.state);
      char D01 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_18_D01.state);
      char D02 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_19_D02.state);
      char D03 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_20_D03.state);
      char D04 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_21_D04.state);
      char D05 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_22_D05.state);
      char D06 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_23_D06.state);
      char D07 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_24_D07.state);

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

      gbp.next_phase(cart_blob);
    }
#endif
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

    GateBoyPair gbp;
    gbp.reset_to_cart(cart_blob);
    gbp.run_phases(cart_blob, 120);

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
      char CLK = cp_ext(gbp.gba.gb_state.sys_clk.PIN_75_CLK_OUT. state);
      char WRn = cp_ext(gbp.gba.gb_state.ext_ctrl.PIN_78_WRn.state);
      char RDn = cp_ext(gbp.gba.gb_state.ext_ctrl.PIN_79_RDn.state);
      char CSn = cp_ext(gbp.gba.gb_state.ext_ctrl.PIN_80_CSn.state);

      char A00 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_01_A00.state);
      char A01 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_02_A01.state);
      char A02 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_03_A02.state);
      char A03 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_04_A03.state);
      char A04 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_05_A04.state);
      char A05 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_06_A05.state);
      char A06 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_07_A06.state);
      char A07 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_08_A07.state);
      char A08 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_09_A08.state);
      char A09 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_10_A09.state);
      char A10 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_11_A10.state);
      char A11 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_12_A11.state);
      char A12 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_13_A12.state);
      char A13 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_14_A13.state);
      char A14 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_15_A14.state);
      char A15 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_16_A15.state);

      char D00 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_17_D00.state);
      char D01 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_18_D01.state);
      char D02 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_19_D02.state);
      char D03 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_20_D03.state);
      char D04 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_21_D04.state);
      char D05 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_22_D05.state);
      char D06 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_23_D06.state);
      char D07 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_24_D07.state);

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

      gbp.next_phase(cart_blob);
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

    GateBoyPair gbp;
    gbp.reset_to_cart(cart_blob);
    gbp.run_phases(cart_blob, 120);

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
      char CLK = cp_ext(gbp.gba.gb_state.sys_clk.PIN_75_CLK_OUT .state);
      char WRn = cp_ext(gbp.gba.gb_state.ext_ctrl.PIN_78_WRn.state);
      char RDn = cp_ext(gbp.gba.gb_state.ext_ctrl.PIN_79_RDn.state);
      char CSn = cp_ext(gbp.gba.gb_state.ext_ctrl.PIN_80_CSn.state);

      char A00 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_01_A00.state);
      char A01 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_02_A01.state);
      char A02 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_03_A02.state);
      char A03 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_04_A03.state);
      char A04 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_05_A04.state);
      char A05 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_06_A05.state);
      char A06 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_07_A06.state);
      char A07 = cp_ext(gbp.gba.gb_state.ext_abus.lo.PIN_08_A07.state);
      char A08 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_09_A08.state);
      char A09 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_10_A09.state);
      char A10 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_11_A10.state);
      char A11 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_12_A11.state);
      char A12 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_13_A12.state);
      char A13 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_14_A13.state);
      char A14 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_15_A14.state);
      char A15 = cp_ext(gbp.gba.gb_state.ext_abus.hi.PIN_16_A15.state);

      char D00 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_17_D00.state);
      char D01 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_18_D01.state);
      char D02 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_19_D02.state);
      char D03 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_20_D03.state);
      char D04 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_21_D04.state);
      char D05 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_22_D05.state);
      char D06 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_23_D06.state);
      char D07 = cp_ext(gbp.gba.gb_state.ext_dbus.PIN_24_D07.state);

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

      gbp.next_phase(cart_blob);
    }
  }
#endif

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_mem() {
  TEST_INIT();

  results += test_mem("ROM",  0x0000, 0x7FFF, 31,  false);
  results += test_mem("VRAM", 0x8000, 0x9FFF, 31,  true);
  results += test_mem("CRAM", 0xA000, 0xBFFF, 31,  true);
  results += test_mem("IRAM", 0xC000, 0xDFFF, 31,  true);
  results += test_mem("ERAM", 0xE000, 0xFDFF, 31,  true);
  results += test_mem("OAM",  0xFE00, 0xFEFF, 1,   true);
  results += test_mem("ZRAM", 0xFF80, 0xFFFE, 1,   true);

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults GateBoyTests::test_bootrom() {
  TEST_INIT();

  blob cart_blob = Assembler::create_dummy_cart();
  auto gb = create_debug_gb(cart_blob, false);

  for (int i = 0; i < 16; i++) {
    uint8_t data_out = gb->dbg_read(cart_blob, i);
    EXPECT_EQ(data_out, DMG_ROM_blob[i], "bootrom @ 0x%04x = 0x%02x, expected 0x%02x", i, data_out, DMG_ROM_bin[i]);
  }

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults GateBoyTests::test_timer() {
  TEST_INIT();

  // TAC 100 - 2048 phases per TIMA tick
  // TAC 101 - 32 phases per TIMA tick
  // TAC 110 - 128 phases per TIMA tick
  // TAC 111 - 512 phases per TIMA tick

  blob cart_blob = Assembler::create_dummy_cart();

  LOG("Testing TIMA tick rate and reset_states to TMA...\n");
  {
    auto gb = make_unique<GateBoy>();
    gb->reset_to_bootrom(cart_blob, true);
    gb->sys.cpu_en = false;

    gb->dbg_write(cart_blob, ADDR_TMA, 0x80).unwrap();
    gb->dbg_write(cart_blob, ADDR_TIMA,0xFD).unwrap();
    gb->dbg_write(cart_blob, ADDR_DIV, 0x00).unwrap();
    gb->dbg_write(cart_blob, ADDR_TAC, 0b00000100).unwrap();

    EXPECT_EQ(0xFD, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 2048);
    EXPECT_EQ(0xFE, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 2048);
    EXPECT_EQ(0xFF, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 2048);
    EXPECT_EQ(0x80, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 2048);
    EXPECT_EQ(0x81, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 2048);
    if (results.ok()) LOG_B("TAC 0b100 pass\n");
  }

  {
    auto gb = make_unique<GateBoy>();
    gb->reset_to_bootrom(cart_blob, true);
    gb->sys.cpu_en = false;

    gb->dbg_write(cart_blob, ADDR_TMA, 0x80).unwrap();
    gb->dbg_write(cart_blob, ADDR_TIMA,0xFD).unwrap();
    gb->dbg_write(cart_blob, ADDR_DIV, 0x00).unwrap();
    gb->dbg_write(cart_blob, ADDR_TAC, 0b00000101).unwrap();

    EXPECT_EQ(0xFD, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 32);
    EXPECT_EQ(0xFE, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 32);
    EXPECT_EQ(0xFF, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 32);
    EXPECT_EQ(0x80, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 32);
    EXPECT_EQ(0x81, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 32);
    if (results.ok()) LOG_B("TAC 0b101 pass\n");
  }
  {
    auto gb = make_unique<GateBoy>();
    gb->reset_to_bootrom(cart_blob, true);
    gb->sys.cpu_en = false;

    gb->dbg_write(cart_blob, ADDR_TMA, 0x80);
    gb->dbg_write(cart_blob, ADDR_TIMA,0xFD);
    gb->dbg_write(cart_blob, ADDR_DIV, 0x00);
    gb->dbg_write(cart_blob, ADDR_TAC, 0b00000110);

    EXPECT_EQ(0xFD, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 128);
    EXPECT_EQ(0xFE, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 128);
    EXPECT_EQ(0xFF, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 128);
    EXPECT_EQ(0x80, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 128);
    EXPECT_EQ(0x81, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 128);
    if (results.ok()) LOG_B("TAC 0b110 pass\n");
  }
  {
    auto gb = make_unique<GateBoy>();
    gb->reset_to_bootrom(cart_blob, true);
    gb->sys.cpu_en = false;

    gb->dbg_write(cart_blob, ADDR_TMA, 0x80);
    gb->dbg_write(cart_blob, ADDR_TIMA,0xFD);
    gb->dbg_write(cart_blob, ADDR_DIV, 0x00);
    gb->dbg_write(cart_blob, ADDR_TAC, 0b00000111);

    EXPECT_EQ(0xFD, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 512);
    EXPECT_EQ(0xFE, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 512);
    EXPECT_EQ(0xFF, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 512);
    EXPECT_EQ(0x80, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 512);
    EXPECT_EQ(0x81, bit_pack(gb->gb_state.reg_tima));
    gb->run_phases(cart_blob, 512);
    if (results.ok()) LOG_B("TAC 0b111 pass\n");
  }

  if (run_slow_tests) {
    auto gb = make_unique<GateBoy>();
    gb->reset_to_bootrom(cart_blob, true);
    gb->sys.cpu_en = false;

    // passes, but slow :/
    LOG("Testing div reset_states + rollover, this takes a minute...");
    gb->dbg_write(cart_blob, ADDR_DIV, 0);
    for (int i = 1; i < 32768; i++) {
      uint8_t div_a = gb->dbg_read(cart_blob, ADDR_DIV);
      uint8_t div_b = (i >> 6) & 0xFF;
      EXPECT_EQ(div_a, div_b, "div match fail");
    }
    LOG("\n");
  }

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults GateBoyTests::test_dma() {
  TEST_INIT();

  for (int src = 0x0000; src < 0xFE00; src += 0x1000) {
    results += test_dma(uint16_t(src));
  }

  TEST_DONE();
}

//----------------------------------------

TestResults GateBoyTests::test_dma(uint16_t src) {
  TEST_INIT("0x%04x", src);

  blob cart_blob = Assembler::create_dummy_cart();

  auto gb = std::make_unique<GateBoy>();
  gb->reset_to_cart(cart_blob);
  gb->sys.cpu_en = false;

  gb->dbg_write(cart_blob, ADDR_LCDC, 0);
  gb->dbg_write(cart_blob, 0x0000, 0x0A); // enable mbc1 ram

  for (int i = 0; i < 256; i++) {
    //uint8_t r = (uint8_t)rand();
    uint8_t r = (uint8_t)i;
    gb->poke(cart_blob, src + i, r);
    gb->poke(cart_blob, ADDR_OAM_BEGIN + i, 0xFF);
  }

  gb->dbg_write(cart_blob, 0xFF46, uint8_t(src >> 8));
  gb->run_phases(cart_blob, 644);

  gb->run_phases(cart_blob, 644);

  for (int i = 0; i < 160; i++) {
    uint8_t src_a = gb->peek(cart_blob, src + i);
    uint8_t dst_a = gb->peek(cart_blob, ADDR_OAM_BEGIN + i);
    ASSERT_EQ(src_a, dst_a, "dma mismatch @ 0x%04x : expected 0x%02x, got 0x%02x", src + i, src_a, dst_a);
  }

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults GateBoyTests::test_ppu() {
  TEST_INIT();

  // slow
  if (run_slow_tests) {
    LOG("Checking LY increment rate... ");
    blob cart_blob = Assembler::create_dummy_cart();

    auto gb = make_unique<GateBoy>();
    gb->reset_to_bootrom(cart_blob, true);
    gb->sys.cpu_en = false;

    gb->dbg_write(cart_blob, ADDR_LCDC, 0x80);

    // LY should increment every 114*8 phases after LCD enable, except on the last line.
    for (uint32_t i = 0; i < 153; i++) {
      EXPECT_EQ(i, bit_pack(gb->gb_state.reg_ly));
      gb->run_phases(cart_blob, 114 * 8);
    }

    // LY is reset early on the last line, we should be at 0 now.
    EXPECT_EQ(0, bit_pack(gb->gb_state.reg_ly));
    gb->run_phases(cart_blob, 114 * 8);

    // And we should be at 0 again
    EXPECT_EQ(0, bit_pack(gb->gb_state.reg_ly));
    gb->run_phases(cart_blob, 114 * 8);

    // And now we should be at 1.
    EXPECT_EQ(1, bit_pack(gb->gb_state.reg_ly));

    if (results.ok()) LOG_B("Pass");
  }

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults GateBoyTests::test_mem(const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write) {
  TEST_INIT("%-4s @ [0x%04x,0x%04x], step %3d write %d", tag, addr_start, addr_end, step, test_write);

  blob cart_blob = Assembler::create_dummy_cart();
  auto gb = make_unique<GateBoy>();
  gb->reset_to_bootrom(cart_blob, true);
  gb->sys.cpu_en = false;

  gb->dbg_write(cart_blob, 0xFF50, 0x01); // disable bootrom
  gb->dbg_write(cart_blob, 0x0000, 0x0A); // enable mbc1 ram

  for (uint16_t addr = addr_start; addr < addr_end; addr += step) {
    uint8_t data_wr = 0x55;
    if (test_write) {
      gb->poke(cart_blob, addr, 0);
      gb->dbg_write(cart_blob, addr, data_wr);
    }
    else {
      gb->poke(cart_blob, addr, data_wr);
    }

    uint8_t data_rd = gb->peek(cart_blob, addr);
    ASSERT_EQ(data_rd, data_wr, "WRITE FAIL addr 0x%04x : wrote 0x%02x, read 0x%02x", addr, data_wr, data_rd);
    data_rd = gb->dbg_read(cart_blob, addr);
    ASSERT_EQ(data_rd, data_wr, "READ FAIL  addr 0x%04x : wrote 0x%02x, read 0x%02x", addr, data_wr, data_rd);
  }

  for (uint16_t addr = addr_start; addr < addr_end; addr += step) {
    uint8_t data_wr = 0xAA;
    if (test_write) {
      gb->poke(cart_blob, addr, 0);
      gb->dbg_write(cart_blob, addr, data_wr);
    }
    else {
      gb->poke(cart_blob, addr, data_wr);
    }

    uint8_t data_rd = gb->peek(cart_blob, addr);
    ASSERT_EQ(data_rd, data_wr, "WRITE FAIL addr 0x%04x : wrote 0x%02x, read 0x%02x", addr, data_wr, data_rd);
    data_rd = gb->dbg_read(cart_blob, addr);
    ASSERT_EQ(data_rd, data_wr, "READ FAIL  addr 0x%04x : wrote 0x%02x, read 0x%02x", addr, data_wr, data_rd);
  }

  TEST_DONE();
}

//-----------------------------------------------------------------------------

void GateBoyTests::run_benchmark() {
  blob cart_blob = Assembler::create_dummy_cart();
  auto gb = make_unique<GateBoy>();

  const int iter_count = config_debug ? 16 : 74;
  const int phase_per_iter = config_debug ? 128 : 8192;
  const int warmup = config_debug ? 0 : 10;

  double phase_rate_sum1 = 0;
  double phase_rate_sum2 = 0;
  double phase_rate_n = 0;

  LOG("Running perf test");
  for (int iter = 0; iter < iter_count; iter++) {
    // FIXME should probably benchmark something other than the bootrom...
    gb->reset_to_bootrom(cart_blob, true);
    gb->cpu.bus_req_new.addr = 0x0150;
    gb->cpu.bus_req_new.data = 0;
    gb->cpu.bus_req_new.read = 1;
    gb->cpu.bus_req_new.write = 0;
    gb->sys.cpu_en = false;
    gb->sys.phase_total = 0;

    auto start = timestamp();
    gb->run_phases(cart_blob, phase_per_iter);
    auto finish = timestamp();

    if (iter >= warmup) {
      double time = (finish - start);

      double phase_rate = double(gb->sys.phase_total) / time;
      phase_rate_sum1 += phase_rate;
      phase_rate_sum2 += phase_rate * phase_rate;
      phase_rate_n++;
    }
    LOG(".");
  }
  LOG("Done\n");

  LOG("Phase total %d\n", gb->sys.phase_total);

  double phase_rate_mean     = phase_rate_sum1 / phase_rate_n;
  double phase_rate_variance = (phase_rate_sum2 / phase_rate_n) - (phase_rate_mean * phase_rate_mean);
  double phase_rate_sigma    = sqrt(phase_rate_variance);
  LOG("Mean phase/sec %f sigma %f\n", phase_rate_mean, phase_rate_sigma);
}

//-----------------------------------------------------------------------------
// takes a couple minutes

TestResults GateBoyTests::test_mooneye_generic() {
  TEST_INIT();

  const char* path = "tests/mooneye-gb/tests/build/acceptance/";

  results += run_mooneye_test(path, "boot_div-dmgABCmgb.gb");         // p
  results += run_mooneye_test(path, "boot_hwio-dmgABCmgb.gb");        // XXX sound regs
  results += run_mooneye_test(path, "boot_regs-dmgABC.gb");           // p
  results += run_mooneye_test(path, "add_sp_e_timing.gb");            // p
  results += run_mooneye_test(path, "call_cc_timing.gb");             // p
  results += run_mooneye_test(path, "call_cc_timing2.gb");            // p
  results += run_mooneye_test(path, "call_timing.gb");                // p
  results += run_mooneye_test(path, "call_timing2.gb");               // p
  results += run_mooneye_test(path, "di_timing-GS.gb");               // p
  results += run_mooneye_test(path, "div_timing.gb");                 // p
  results += run_mooneye_test(path, "ei_sequence.gb");                // p
  results += run_mooneye_test(path, "ei_timing.gb");                  // p
  results += run_mooneye_test(path, "halt_ime0_ei.gb");               // p
  results += run_mooneye_test(path, "halt_ime0_nointr_timing.gb");    // p
  results += run_mooneye_test(path, "halt_ime1_timing.gb");           // p
  results += run_mooneye_test(path, "halt_ime1_timing2-GS.gb");       // p
  results += run_mooneye_test(path, "if_ie_registers.gb");            // p
  results += run_mooneye_test(path, "intr_timing.gb");                // p
  results += run_mooneye_test(path, "jp_cc_timing.gb");               // p
  results += run_mooneye_test(path, "jp_timing.gb");                  // p
  results += run_mooneye_test(path, "ld_hl_sp_e_timing.gb");          // p
  results += run_mooneye_test(path, "oam_dma_restart.gb");            // p
  results += run_mooneye_test(path, "oam_dma_start.gb");              // p
  results += run_mooneye_test(path, "oam_dma_timing.gb");             // p
  results += run_mooneye_test(path, "pop_timing.gb");                 // p
  results += run_mooneye_test(path, "push_timing.gb");                // p
  results += run_mooneye_test(path, "rapid_di_ei.gb");                // p
  results += run_mooneye_test(path, "ret_cc_timing.gb");              // p
  results += run_mooneye_test(path, "ret_timing.gb");                 // p
  results += run_mooneye_test(path, "reti_intr_timing.gb");           // p
  results += run_mooneye_test(path, "reti_timing.gb");                // p
  results += run_mooneye_test(path, "rst_timing.gb");                 // p
  results += run_mooneye_test(path, "bits/mem_oam.gb");               // p
  results += run_mooneye_test(path, "bits/reg_f.gb");                 // p
  results += run_mooneye_test(path, "bits/unused_hwio-GS.gb");        // XXX sound regs
  results += run_mooneye_test(path, "instr/daa.gb");                  // p
  results += run_mooneye_test(path, "interrupts/ie_push.gb");         // p
  results += run_mooneye_test(path, "oam_dma/basic.gb");              // p
  results += run_mooneye_test(path, "oam_dma/basic.gb");              // p
  results += run_mooneye_test(path, "oam_dma/reg_read.gb");           // p
  results += run_mooneye_test(path, "oam_dma/sources-GS.gb");         // p

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_mooneye_mbc1() {
  TEST_INIT();

  const char* path = "tests/mooneye-gb/tests/build/emulator-only/mbc1/";

  results += run_mooneye_test(path, "bits_bank1.gb"); // pass, but very slow (3 sim-sec)
  results += run_mooneye_test(path, "bits_bank2.gb"); // pass, but very slow (3 sim-sec)
  results += run_mooneye_test(path, "bits_mode.gb");  // pass, but very slow (3 sim-sec)
  results += run_mooneye_test(path, "bits_ramg.gb");  // pass, but very slow (6 sim-sec)

  // not going to bother with multicart support for now
  //"multicart_rom_8Mb.gb",

  results += run_mooneye_test(path, "ram_256Kb.gb");  // pass
  results += run_mooneye_test(path, "ram_64Kb.gb");   // pass
  results += run_mooneye_test(path, "rom_16Mb.gb");   // pass
  results += run_mooneye_test(path, "rom_1Mb.gb");    // pass
  results += run_mooneye_test(path, "rom_2Mb.gb");    // pass
  results += run_mooneye_test(path, "rom_4Mb.gb");    // pass
  results += run_mooneye_test(path, "rom_512Kb.gb");  // pass
  results += run_mooneye_test(path, "rom_8Mb.gb");    // pass

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_mooneye_timer() {
  TEST_INIT();

  const char* path = "tests/mooneye-gb/tests/build/acceptance/timer/";

  results += run_mooneye_test(path, "div_write.gb");            // pass
  results += run_mooneye_test(path, "rapid_toggle.gb");         // pass
  results += run_mooneye_test(path, "tim00.gb");                // pass
  results += run_mooneye_test(path, "tim00_div_trigger.gb");    // pass
  results += run_mooneye_test(path, "tim01.gb");                // pass
  results += run_mooneye_test(path, "tim01_div_trigger.gb");    // pass
  results += run_mooneye_test(path, "tim10.gb");                // pass
  results += run_mooneye_test(path, "tim10_div_trigger.gb");    // pass
  results += run_mooneye_test(path, "tim11.gb");                // pass
  results += run_mooneye_test(path, "tim11_div_trigger.gb");    // pass
  results += run_mooneye_test(path, "tima_reload.gb");          // pass
  results += run_mooneye_test(path, "tima_write_reloading.gb"); // pass
  results += run_mooneye_test(path, "tma_write_reloading.gb");  // pass

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::test_mooneye_ppu() {
  TEST_INIT();

  const char* path = "tests/mooneye-gb/tests/build/acceptance/ppu/";

  results += run_mooneye_test(path, "hblank_ly_scx_timing-GS.gb");      // p
  results += run_mooneye_test(path, "intr_1_2_timing-GS.gb");           // p
  results += run_mooneye_test(path, "intr_2_0_timing.gb");              // p
  results += run_mooneye_test(path, "intr_2_mode0_timing.gb");          // p
  results += run_mooneye_test(path, "intr_2_mode0_timing_sprites.gb");  // dmg pass, gateboy fail
  results += run_mooneye_test(path, "intr_2_mode3_timing.gb");          // p
  results += run_mooneye_test(path, "intr_2_oam_ok_timing.gb");         // p
  results += run_mooneye_test(path, "lcdon_timing-GS.gb");              // dmg pass, gateboy fail - this is the same as lcdon_to_stat2_a
  results += run_mooneye_test(path, "lcdon_write_timing-GS.gb");        // dmg pass, gateboy fail - probably the same as lcdon_to_oam_unlock_d
  results += run_mooneye_test(path, "stat_irq_blocking.gb");            // p
  results += run_mooneye_test(path, "stat_lyc_onoff.gb");               // p
  results += run_mooneye_test(path, "vblank_stat_intr-GS.gb");          // p

  TEST_DONE();
}

//-----------------------------------------------------------------------------

TestResults GateBoyTests::run_mooneye_test(const char* path, const char* filename) {
  TEST_INIT();

  blob rom;
  load_blob((std::string(path) + std::string(filename)).c_str(), rom);

  if (rom.empty()) {
    LOG_B("%-30s ", filename);
    LOG_Y("FILE NOT FOUND\n");
    TEST_FAIL();
  }

  if (verbose) LOG_B("%-50s ", filename);

  GateBoyPair gbp;
  gbp.reset_to_cart(rom);

  int timeout = 6400000; // bits_ramg is super slow

  int mcycle = 0;
  for (; mcycle < timeout; mcycle++) {
    gbp.run_phases(rom, 8);
    if (gbp.gba.cpu.core.op == 0x40) break;
  }

  if ((gbp.gba.cpu.core.a == 0x00) && (mcycle != timeout)) {
    if (verbose) LOG_G("PASS @ %d\n", mcycle);
  }
  else {
    if (!verbose) LOG_B("%-50s ", filename);
    LOG_R("%s @ %d\n", (mcycle == timeout) ? "TIMEOUT" : "FAIL", mcycle);
    TEST_FAIL();
  }

  TEST_DONE();
}

//-----------------------------------------------------------------------------


#if 0

//load_rom("tests/gb-test-roms/instr_timing/instr_timing.gb"); // pass
//load_rom("tests/gb-test-roms/cpu_instrs/cpu_instrs.gb"); // doesn't work yet, probably mbc1 stuff

//load_rom("tests/gb-test-roms/cpu_instrs/individual/01-special.gb"); // pass
//load_rom("tests/gb-test-roms/cpu_instrs/individual/02-interrupts.gb"); // broken
//load_rom("tests/gb-test-roms/cpu_instrs/individual/03-op sp,hl.gb"); // pass
//load_rom("tests/gb-test-roms/cpu_instrs/individual/04-op r,imm.gb"); // pass
//load_rom("tests/gb-test-roms/cpu_instrs/individual/05-op rp.gb"); // pass
//load_rom("tests/gb-test-roms/cpu_instrs/individual/06-ld r,r.gb"); // pass
//load_rom("tests/gb-test-roms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb"); // pass
//load_rom("tests/gb-test-roms/cpu_instrs/individual/08-misc instrs.gb"); // pass
//load_rom("tests/gb-test-roms/cpu_instrs/individual/09-op r,r.gb"); // pass
//load_rom("tests/gb-test-roms/cpu_instrs/individual/10-bit ops.gb"); // pass
//load_rom("tests/gb-test-roms/cpu_instrs/individual/11-op a,(hl).gb");

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
