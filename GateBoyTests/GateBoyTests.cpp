#include "GateBoyTests/GateBoyTests.h"
#include "GateBoyLib/GateBoy.h"
#include "CoreLib/Assembler.h"
#include "CoreLib/Constants.h"
#include "CoreLib/Tests.h"
#include "CoreLib/File.h"
#include <stddef.h>

#include <windows.h>

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  SetPriorityClass(GetCurrentProcess(), 0x00000080);

  TEST_START("Maaaaaain");

  (void)argc;
  (void)argv;

  static const bool skip_passing_tests = true;

  GateBoyTests t;
  //t.verbose = true;
  t.cart_rom.resize(32768, 0);

  auto start = timestamp();

#ifdef USE_COMMIT
  failures += t.test_reset_cart_vs_dump();
#endif

  //failures += t.test_fastboot_vs_slowboot();
  //failures += t.test_bootrom();

  //failures += t.test_clk();
  //failures += t.test_regs();
  //failures += t.test_mem();
  //failures += t.test_dma();
  failures += t.test_init();
  failures += t.test_ext_bus();
  failures += t.test_ppu();
  failures += t.test_timer();

  failures += t.test_micro_poweron();
  failures += t.test_micro_lcden();
  failures += t.test_micro_timer();
  failures += t.test_micro_int_vblank();
  failures += t.test_micro_int_stat();
  failures += t.test_micro_int_timer();
  failures += t.test_micro_int_serial();
  failures += t.test_micro_int_joypad();
  failures += t.test_micro_lock_oam();
  failures += t.test_micro_lock_vram();
  failures += t.test_micro_window();
  failures += t.test_micro_ppu();
  failures += t.test_micro_dma();

  auto finish = timestamp();

  if (!failures) LOG_G("Everything passed!\n");

  LOG_G("Tests took %f seconds\n", finish - start);
  LOG_G("%d failures\n", failures);

  TEST_END();
}

//-----------------------------------------------------------------------------

int diff(const char* name_a, void* blob_a, int start_a, int end_a,
         const char* name_b, void* blob_b, int start_b, int end_b) {
  TEST_START();
  int size_a = end_a - start_a;
  int size_b = end_b - start_b;

  if (size_a != size_b) {
    LOG_R("diff() : Size mismatch %d vs %d\n", size_a, size_b);
    return false;
  }

  uint8_t* bytes_a = (uint8_t*)blob_a;
  uint8_t* bytes_b = (uint8_t*)blob_b;

  for (int i = 0; i < size_a; i++) {
    int ia = start_a + i;
    int ib = start_b + i;

    int byte_a = bytes_a[ia];
    int byte_b = bytes_b[ib];

    EXPECT_EQ(byte_a, byte_b,
              "%s != %s @ %5d : %s[%5d] = 0x%02x, %s[%5d] = 0x%02x\n",
              name_a, name_b, i,
              name_a, ia, bytes_a[ia],
              name_b, ib, bytes_b[ib]);
  }

  TEST_END();
}

//-----------------------------------------------------------------------------

GateBoy GateBoyTests::create_gb_poweron() {
  GateBoy gb;
  gb.load_cart(DMG_ROM_blob.data(), DMG_ROM_blob.size(), cart_rom.data(), cart_rom.size());
  gb.reset_to_bootrom(true);
  gb.sys_cpu_en = 0;
  return gb;
}


//-----------------------------------------------------------------------------

int GateBoyTests::test_regs() {
  TEST_START();

  failures += test_reg("P1",   ADDR_P1,   0b00110000);
  failures += test_reg("SB",   ADDR_SB,   0b11111111);
  failures += test_reg("SC",   ADDR_SC,   0b10000001);
  failures += test_reg("TIMA", ADDR_TIMA, 0b11111111);
  failures += test_reg("TMA",  ADDR_TMA,  0b11111111);
  failures += test_reg("TAC",  ADDR_TAC,  0b00000111);
  failures += test_reg("IF",   ADDR_IF,   0b00011111);
  failures += test_reg("LCDC", ADDR_LCDC, 0b11111111);
  failures += test_reg("STAT", ADDR_STAT, 0b01111000);
  failures += test_reg("SCY",  ADDR_SCY,  0b11111111);
  failures += test_reg("SCX",  ADDR_SCX,  0b11111111);
  //failures += test_reg("LY",   ADDR_LY,   177); not writable
  failures += test_reg("LYC",  ADDR_LYC,  0b11111111);
  failures += test_reg("DMA",  ADDR_DMA,  0b11111111);
  failures += test_reg("BGP",  ADDR_BGP,  0b11111111);
  failures += test_reg("OBP0", ADDR_OBP0, 0b11111111);
  failures += test_reg("OBP1", ADDR_OBP1, 0b11111111);
  failures += test_reg("WY",   ADDR_WY,   0b11111111);
  failures += test_reg("WX",   ADDR_WX,   0b11111111);
  failures += test_reg("IE",   ADDR_IE,   0b00011111);

  TEST_END();
}

//-----------------------------------------------------------------------------
// Power-on reset state should be stable

int GateBoyTests::test_fastboot_vs_slowboot() {
  TEST_START();

  LOG_B("reset_to_bootrom with fastboot = true\n");
  GateBoy gb1;
  gb1.load_cart(DMG_ROM_blob.data(), DMG_ROM_blob.size(), cart_rom.data(), cart_rom.size());
  gb1.reset_to_bootrom(true);
  LOG_G("reset_to_bootrom with fastboot = true done\n");

  LOG_B("reset_to_bootrom with fastboot = false\n");
  GateBoy gb2;
  gb2.load_cart(DMG_ROM_blob.data(), DMG_ROM_blob.size(), cart_rom.data(), cart_rom.size());
  gb2.reset_to_bootrom(false);
  LOG_G("reset_to_bootrom with fastboot = false done\n");

  // Clear the fastboot bit on the first gameboy, since that obviously won't match
  gb1.sys_fastboot = 0;

  int start = 0;
  int end   = offsetof(GateBoy, sentinel3);

  failures += diff("fastboot", &gb1, start, end, "slowboot", &gb2, start, end);

  TEST_END();
}

//-----------------------------------------------------------------------------
// reset_cart() should match dumped reset state.

int GateBoyTests::test_reset_cart_vs_dump() {
  TEST_START();

  auto blob = load_blob("gateboy_post_bootrom.raw.dump");
  if(!GateBoy::check_sentinel(blob)) {
    LOG_Y("Warning : gateboy_post_bootrom_raw.dump not valid\n");
    TEST_END();
  }

  LOG_B("gateboy_post_bootrom.raw.dump\n");
  GateBoy gb1;
  gb1.from_blob(blob);
  LOG_G("gateboy_post_bootrom.raw.dump done\n");

  LOG_B("reset_to_cart with fastboot = true\n");
  GateBoy gb2;
  gb2.load_cart(DMG_ROM_blob.data(), DMG_ROM_blob.size(), cart_rom.data(), cart_rom.size());
  gb2.reset_to_cart();
  LOG_G("reset_cart done\n");

  int start = 0;
  int end   = offsetof(GateBoy, sentinel3);

  failures += diff("dump", &gb1, start, end, "reset_to_cart", &gb2, start, end);

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_poweron() {
  TEST_START();

  LOG_B("===== Stat boot state =====\n");

  failures += run_microtest("poweron_stat_000.gb");
  failures += run_microtest("poweron_stat_005.gb");
  failures += run_microtest("poweron_stat_006.gb"); // probably due to unstable data latch
  failures += run_microtest("poweron_stat_007.gb");
  failures += run_microtest("poweron_stat_026.gb");
  failures += run_microtest("poweron_stat_027.gb");
  failures += run_microtest("poweron_stat_069.gb");
  failures += run_microtest("poweron_stat_070.gb");
  failures += run_microtest("poweron_stat_119.gb");
  failures += run_microtest("poweron_stat_120.gb"); // failing
  failures += run_microtest("poweron_stat_121.gb");
  failures += run_microtest("poweron_stat_140.gb");
  failures += run_microtest("poweron_stat_141.gb");
  failures += run_microtest("poweron_stat_183.gb");
  failures += run_microtest("poweron_stat_184.gb");
  failures += run_microtest("poweron_stat_234.gb");

  failures += run_microtest("poweron_stat_235.gb");

  LOG_B("===== Div state =====\n");

  failures += run_microtest("poweron_div_000.gb");
  failures += run_microtest("poweron_div_004.gb");
  failures += run_microtest("poweron_div_005.gb");

  LOG_B("===== LY boot state =====\n");

  failures += run_microtest("poweron_ly_000.gb");
  failures += run_microtest("poweron_ly_119.gb");
  failures += run_microtest("poweron_ly_120.gb");
  failures += run_microtest("poweron_ly_233.gb");
  failures += run_microtest("poweron_ly_234.gb");

  LOG_B("===== Other reg initial values =====\n");
  failures += run_microtest("poweron_bgp_000.gb");
  failures += run_microtest("poweron_dma_000.gb");
  failures += run_microtest("poweron_if_000.gb");
  failures += run_microtest("poweron_joy_000.gb");
  failures += run_microtest("poweron_lcdc_000.gb");
  failures += run_microtest("poweron_lyc_000.gb");
  failures += run_microtest("poweron_obp0_000.gb");
  failures += run_microtest("poweron_obp1_000.gb");
  failures += run_microtest("poweron_sb_000.gb");
  failures += run_microtest("poweron_sc_000.gb");
  failures += run_microtest("poweron_scx_000.gb");
  failures += run_microtest("poweron_scy_000.gb");
  failures += run_microtest("poweron_tac_000.gb");
  failures += run_microtest("poweron_tima_000.gb");
  failures += run_microtest("poweron_tma_000.gb");
  failures += run_microtest("poweron_wx_000.gb");
  failures += run_microtest("poweron_wy_000.gb");

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_int_vblank() {
  TEST_START();

#ifdef RUN_SLOW_TESTS
  failures += run_microtest("lcdon_halt_to_vblank_int_a.gb");
  failures += run_microtest("lcdon_halt_to_vblank_int_b.gb");
  failures += run_microtest("lcdon_nops_to_vblank_int_a.gb");
  failures += run_microtest("lcdon_nops_to_vblank_int_b.gb");
#endif

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_int_stat() {
  TEST_START();

  failures += run_microtest("int_hblank_halt_scx0.gb"); // int fires on 822 G
  failures += run_microtest("int_hblank_halt_scx1.gb"); // int fires on 824 A
  failures += run_microtest("int_hblank_halt_scx2.gb"); // int fires on 826 C
  failures += run_microtest("int_hblank_halt_scx3.gb"); // int fires on 828 E
  failures += run_microtest("int_hblank_halt_scx4.gb"); // int fires on 830 G
  failures += run_microtest("int_hblank_halt_scx5.gb"); // int fires on 832 A
  failures += run_microtest("int_hblank_halt_scx6.gb"); // int fires on 834 C
  failures += run_microtest("int_hblank_halt_scx7.gb"); // int fires on 836 E

  failures += run_microtest("int_hblank_incs_scx0.gb"); // int fires on 822 G
  failures += run_microtest("int_hblank_incs_scx1.gb"); // int fires on 824 A
  failures += run_microtest("int_hblank_incs_scx2.gb"); // int fires on 826 C
  failures += run_microtest("int_hblank_incs_scx3.gb"); // int fires on 828 E
  failures += run_microtest("int_hblank_incs_scx4.gb"); // int fires on 830 G
  failures += run_microtest("int_hblank_incs_scx5.gb"); // int fires on 832 A
  failures += run_microtest("int_hblank_incs_scx6.gb"); // int fires on 834 C
  failures += run_microtest("int_hblank_incs_scx7.gb"); // int fires on 836 E

  failures += run_microtest("int_hblank_nops_scx0.gb"); // int fires on 822 G
  failures += run_microtest("int_hblank_nops_scx1.gb"); // int fires on 824 A
  failures += run_microtest("int_hblank_nops_scx2.gb"); // int fires on 826 C
  failures += run_microtest("int_hblank_nops_scx3.gb"); // int fires on 828 E
  failures += run_microtest("int_hblank_nops_scx4.gb"); // int fires on 830 G
  failures += run_microtest("int_hblank_nops_scx5.gb"); // int fires on 832 A
  failures += run_microtest("int_hblank_nops_scx6.gb"); // int fires on 834 C
  failures += run_microtest("int_hblank_nops_scx7.gb"); // int fires on 836 E

#ifdef RUN_SLOW_TESTS
  failures += run_microtest("int_vblank1_halt.gb"); // int fires on 131602 C
  failures += run_microtest("int_vblank1_incs.gb");
  failures += run_microtest("int_vblank1_nops.gb");

  failures += run_microtest("int_vblank2_halt.gb"); // int fires on 131562 C
  failures += run_microtest("int_vblank2_incs.gb");
  failures += run_microtest("int_vblank2_nops.gb");
#endif

  failures += run_microtest("int_lyc_halt.gb"); // int fires on 1226 C
  failures += run_microtest("int_lyc_incs.gb");
  failures += run_microtest("int_lyc_nops.gb");

  failures += run_microtest("int_oam_halt.gb"); // int fires on 1182 G
  failures += run_microtest("int_oam_incs.gb");
  failures += run_microtest("int_oam_nops.gb");

  // broken and slow
#ifdef RUN_SLOW_TESTS
  failures += run_microtest("int_hblank_halt_bug_a.gb"); // failing
  failures += run_microtest("int_hblank_halt_bug_b.gb"); // failing
#endif

  failures += run_microtest("hblank_int_if_a.gb");
  failures += run_microtest("hblank_int_if_b.gb");

  failures += run_microtest("hblank_int_scx0_if_a.gb");
  failures += run_microtest("hblank_int_scx0_if_b.gb");
  failures += run_microtest("hblank_int_scx0_if_c.gb");
  failures += run_microtest("hblank_int_scx0_if_d.gb");

  failures += run_microtest("hblank_int_scx1_if_a.gb");
  failures += run_microtest("hblank_int_scx1_if_b.gb");
  failures += run_microtest("hblank_int_scx1_if_c.gb");
  failures += run_microtest("hblank_int_scx1_if_d.gb");
  failures += run_microtest("hblank_int_scx1_nops_a.gb");
  failures += run_microtest("hblank_int_scx1_nops_b.gb");

  failures += run_microtest("hblank_int_scx2_if_a.gb");
  failures += run_microtest("hblank_int_scx2_if_b.gb");
  failures += run_microtest("hblank_int_scx2_if_c.gb");
  failures += run_microtest("hblank_int_scx2_if_d.gb");
  failures += run_microtest("hblank_int_scx2_nops_a.gb");
  failures += run_microtest("hblank_int_scx2_nops_b.gb");

  failures += run_microtest("hblank_int_scx3_if_a.gb");
  failures += run_microtest("hblank_int_scx3_if_b.gb");
  failures += run_microtest("hblank_int_scx3_if_c.gb");
  failures += run_microtest("hblank_int_scx3_if_d.gb");
  failures += run_microtest("hblank_int_scx3_nops_a.gb");
  failures += run_microtest("hblank_int_scx3_nops_b.gb");

  failures += run_microtest("hblank_int_scx4_if_a.gb");
  failures += run_microtest("hblank_int_scx4_if_b.gb");
  failures += run_microtest("hblank_int_scx4_if_c.gb");
  failures += run_microtest("hblank_int_scx4_if_d.gb");
  failures += run_microtest("hblank_int_scx4_nops_a.gb");
  failures += run_microtest("hblank_int_scx4_nops_b.gb");

  failures += run_microtest("hblank_int_scx5_if_a.gb");
  failures += run_microtest("hblank_int_scx5_if_b.gb");
  failures += run_microtest("hblank_int_scx5_if_c.gb");
  failures += run_microtest("hblank_int_scx5_if_d.gb");
  failures += run_microtest("hblank_int_scx5_nops_a.gb");
  failures += run_microtest("hblank_int_scx5_nops_b.gb");

  failures += run_microtest("hblank_int_scx6_if_a.gb");
  failures += run_microtest("hblank_int_scx6_if_b.gb");
  failures += run_microtest("hblank_int_scx6_if_c.gb");
  failures += run_microtest("hblank_int_scx6_if_d.gb");
  failures += run_microtest("hblank_int_scx6_nops_a.gb");
  failures += run_microtest("hblank_int_scx6_nops_b.gb");

  failures += run_microtest("hblank_int_scx7_if_a.gb");
  failures += run_microtest("hblank_int_scx7_if_b.gb");
  failures += run_microtest("hblank_int_scx7_if_c.gb");
  failures += run_microtest("hblank_int_scx7_if_d.gb");
  failures += run_microtest("hblank_int_scx7_nops_a.gb");
  failures += run_microtest("hblank_int_scx7_nops_b.gb");

  failures += run_microtest("int_hblank_halt_scx0.gb");
  failures += run_microtest("int_hblank_halt_scx1.gb");
  failures += run_microtest("int_hblank_halt_scx2.gb");
  failures += run_microtest("int_hblank_halt_scx3.gb");
  failures += run_microtest("int_hblank_halt_scx4.gb");
  failures += run_microtest("int_hblank_halt_scx5.gb");
  failures += run_microtest("int_hblank_halt_scx6.gb");
  failures += run_microtest("int_hblank_halt_scx7.gb");

  failures += run_microtest("int_hblank_incs_scx0.gb");
  failures += run_microtest("int_hblank_incs_scx1.gb");
  failures += run_microtest("int_hblank_incs_scx2.gb");
  failures += run_microtest("int_hblank_incs_scx3.gb");
  failures += run_microtest("int_hblank_incs_scx4.gb");
  failures += run_microtest("int_hblank_incs_scx5.gb");
  failures += run_microtest("int_hblank_incs_scx6.gb");
  failures += run_microtest("int_hblank_incs_scx7.gb");

#ifdef RUN_SLOW_TESTS
  failures += run_microtest("vblank2_int_if_a.gb");
  failures += run_microtest("vblank2_int_if_b.gb");
  failures += run_microtest("vblank2_int_if_c.gb");
  failures += run_microtest("vblank2_int_if_d.gb");
  failures += run_microtest("vblank2_int_inc_sled.gb");
  failures += run_microtest("vblank2_int_nops_a.gb");
  failures += run_microtest("vblank2_int_nops_b.gb");

  failures += run_microtest("vblank_int_if_a.gb");
  failures += run_microtest("vblank_int_if_b.gb");
  failures += run_microtest("vblank_int_if_c.gb");
  failures += run_microtest("vblank_int_if_d.gb");
  failures += run_microtest("vblank_int_inc_sled.gb");
  failures += run_microtest("vblank_int_nops_a.gb");
  failures += run_microtest("vblank_int_nops_b.gb");
#endif

  failures += run_microtest("lcdon_to_oam_int_l0.gb");
  failures += run_microtest("lcdon_to_oam_int_l1.gb");
  failures += run_microtest("lcdon_to_oam_int_l2.gb");

#ifdef RUN_SLOW_TESTS
  failures += run_microtest("line_144_oam_int_a.gb"); // pass
  failures += run_microtest("line_144_oam_int_b.gb"); // pass
  failures += run_microtest("line_144_oam_int_c.gb"); // pass
  failures += run_microtest("line_144_oam_int_d.gb"); // pass
#endif

  failures += run_microtest("oam_int_if_edge_a.gb"); // pass
  failures += run_microtest("oam_int_if_edge_b.gb"); // pass
  failures += run_microtest("oam_int_if_edge_c.gb"); // pass
  failures += run_microtest("oam_int_if_edge_d.gb"); // pass
  failures += run_microtest("oam_int_if_level_c.gb"); // pass
  failures += run_microtest("oam_int_if_level_d.gb"); // pass
  failures += run_microtest("oam_int_inc_sled.gb"); // pass
  failures += run_microtest("oam_int_nops_a.gb"); // pass
  failures += run_microtest("oam_int_nops_b.gb"); // pass

  failures += run_microtest("lcdon_to_lyc1_int.gb");
  failures += run_microtest("lcdon_to_lyc2_int.gb");
  failures += run_microtest("lcdon_to_lyc3_int.gb");

  failures += run_microtest("lyc1_int_if_edge_a.gb");  // pass
  failures += run_microtest("lyc1_int_if_edge_b.gb");  // pass
  failures += run_microtest("lyc1_int_if_edge_c.gb");  // pass
  failures += run_microtest("lyc1_int_if_edge_d.gb");  // pass
  failures += run_microtest("lyc1_int_nops_a.gb");     // pass
  failures += run_microtest("lyc1_int_nops_b.gb");     // pass

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_int_timer() {
  TEST_START();
  failures += run_microtest("int_timer_halt.gb"); // int fires on 296 A
  failures += run_microtest("int_timer_halt_div_a.gb");
  failures += run_microtest("int_timer_halt_div_b.gb");
  failures += run_microtest("int_timer_incs.gb");
  failures += run_microtest("int_timer_nops.gb");
  failures += run_microtest("int_timer_nops_div_a.gb");
  failures += run_microtest("int_timer_nops_div_b.gb");
  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_int_serial() {
  TEST_START();
  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_int_joypad() {
  TEST_START();
  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_lcden() {
  TEST_START();

  failures += run_microtest("lcdon_to_ly1_a.gb");
  failures += run_microtest("lcdon_to_ly1_b.gb");
  failures += run_microtest("lcdon_to_ly2_a.gb");
  failures += run_microtest("lcdon_to_ly2_b.gb");
  failures += run_microtest("lcdon_to_ly3_a.gb");
  failures += run_microtest("lcdon_to_ly3_b.gb");

  failures += run_microtest("lcdon_to_stat0_a.gb");
  failures += run_microtest("lcdon_to_stat0_b.gb");
  failures += run_microtest("lcdon_to_stat0_c.gb");
  failures += run_microtest("lcdon_to_stat0_d.gb");

#ifdef RUN_SLOW_TESTS
  failures += run_microtest("lcdon_to_stat1_a.gb");
  failures += run_microtest("lcdon_to_stat1_b.gb");
  failures += run_microtest("lcdon_to_stat1_c.gb");
  failures += run_microtest("lcdon_to_stat1_d.gb"); // failing
  failures += run_microtest("lcdon_to_stat1_e.gb");
#endif

  failures += run_microtest("lcdon_to_stat2_a.gb"); // failing
  failures += run_microtest("lcdon_to_stat2_b.gb");
  failures += run_microtest("lcdon_to_stat2_c.gb");
  failures += run_microtest("lcdon_to_stat2_d.gb");
  failures += run_microtest("lcdon_to_stat3_a.gb");
  failures += run_microtest("lcdon_to_stat3_b.gb");
  failures += run_microtest("lcdon_to_stat3_c.gb");
  failures += run_microtest("lcdon_to_stat3_d.gb");

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_dma() {
  TEST_START();
  LOG_B("===== DMA =====\n");
  failures += run_microtest("dma_0x1000.gb");
  failures += run_microtest("dma_0x9000.gb");
  failures += run_microtest("dma_0xA000.gb");
  failures += run_microtest("dma_0xC000.gb");
  failures += run_microtest("dma_0xE000.gb");
  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_lock_oam() {
  TEST_START();

  failures += run_microtest("oam_read_l0_a.gb");
  failures += run_microtest("oam_read_l0_b.gb");
  failures += run_microtest("oam_read_l0_c.gb");
  failures += run_microtest("oam_read_l0_d.gb"); // failing
  failures += run_microtest("oam_read_l1_a.gb"); // failing
  failures += run_microtest("oam_read_l1_b.gb");
  failures += run_microtest("oam_read_l1_c.gb");
  failures += run_microtest("oam_read_l1_d.gb");
  failures += run_microtest("oam_read_l1_e.gb");
  failures += run_microtest("oam_read_l1_f.gb"); // failing

  failures += run_microtest("oam_write_l0_a.gb");
  failures += run_microtest("oam_write_l0_b.gb");
  failures += run_microtest("oam_write_l0_c.gb");
  failures += run_microtest("oam_write_l0_d.gb"); // failing
  failures += run_microtest("oam_write_l0_e.gb");

  failures += run_microtest("oam_write_l1_a.gb");
  failures += run_microtest("oam_write_l1_b.gb");
  failures += run_microtest("oam_write_l1_c.gb"); // failing
  failures += run_microtest("oam_write_l1_d.gb");
  failures += run_microtest("oam_write_l1_e.gb");
  failures += run_microtest("oam_write_l1_f.gb");

  failures += run_microtest("lcdon_to_oam_unlock_a.gb");
  failures += run_microtest("lcdon_to_oam_unlock_b.gb");
  failures += run_microtest("lcdon_to_oam_unlock_c.gb");
  failures += run_microtest("lcdon_to_oam_unlock_d.gb"); // failing

  failures += run_microtest("poweron_oam_000.gb");
  failures += run_microtest("poweron_oam_005.gb");
  failures += run_microtest("poweron_oam_006.gb"); // fail, this is in the gap between hblank and scan
  failures += run_microtest("poweron_oam_069.gb");
  failures += run_microtest("poweron_oam_070.gb");
  failures += run_microtest("poweron_oam_119.gb");
  failures += run_microtest("poweron_oam_120.gb"); // fail, this is in the gap between hblank and scan
  failures += run_microtest("poweron_oam_121.gb");
  failures += run_microtest("poweron_oam_183.gb");
  failures += run_microtest("poweron_oam_184.gb");
  failures += run_microtest("poweron_oam_233.gb");
  failures += run_microtest("poweron_oam_234.gb"); // fail, this is in the gap between hblank and scan
  failures += run_microtest("poweron_oam_235.gb");

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_lock_vram() {
  TEST_START();

  failures += run_microtest("poweron_vram_000.gb");
  failures += run_microtest("poweron_vram_025.gb");
  failures += run_microtest("poweron_vram_026.gb"); // fail, this is in the gap between scan and render
  failures += run_microtest("poweron_vram_069.gb");
  failures += run_microtest("poweron_vram_070.gb");
  failures += run_microtest("poweron_vram_139.gb");
  failures += run_microtest("poweron_vram_140.gb"); // fail, this is in the gap between scan and render
  failures += run_microtest("poweron_vram_183.gb");
  failures += run_microtest("poweron_vram_184.gb");

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_timer() {
  TEST_START();

  failures += run_microtest("timer_tima_inc_256k_a.gb");
  failures += run_microtest("timer_tima_inc_256k_b.gb");
  failures += run_microtest("timer_tima_inc_256k_c.gb");
  failures += run_microtest("timer_tima_inc_256k_d.gb");
  failures += run_microtest("timer_tima_inc_256k_e.gb");
  failures += run_microtest("timer_tima_inc_256k_f.gb");
  failures += run_microtest("timer_tima_inc_256k_g.gb");
  failures += run_microtest("timer_tima_inc_256k_h.gb");
  failures += run_microtest("timer_tima_inc_256k_i.gb");
  failures += run_microtest("timer_tima_inc_256k_j.gb");
  failures += run_microtest("timer_tima_inc_256k_k.gb");

  failures += run_microtest("timer_tima_reload_256k_a.gb");
  failures += run_microtest("timer_tima_reload_256k_b.gb");
  failures += run_microtest("timer_tima_reload_256k_c.gb");
  failures += run_microtest("timer_tima_reload_256k_d.gb");
  failures += run_microtest("timer_tima_reload_256k_e.gb");
  failures += run_microtest("timer_tima_reload_256k_f.gb");
  failures += run_microtest("timer_tima_reload_256k_g.gb");
  failures += run_microtest("timer_tima_reload_256k_h.gb");
  failures += run_microtest("timer_tima_reload_256k_i.gb");
  failures += run_microtest("timer_tima_reload_256k_j.gb");
  failures += run_microtest("timer_tima_reload_256k_k.gb");

  failures += run_microtest("timer_tima_phase_a.gb");
  failures += run_microtest("timer_tima_phase_b.gb");
  failures += run_microtest("timer_tima_phase_c.gb");
  failures += run_microtest("timer_tima_phase_d.gb");
  failures += run_microtest("timer_tima_phase_e.gb");
  failures += run_microtest("timer_tima_phase_f.gb");
  failures += run_microtest("timer_tima_phase_g.gb");
  failures += run_microtest("timer_tima_phase_h.gb");
  failures += run_microtest("timer_tima_phase_i.gb");
  failures += run_microtest("timer_tima_phase_j.gb");

  failures += run_microtest("timer_tima_write_a.gb");
  failures += run_microtest("timer_tima_write_b.gb");
  failures += run_microtest("timer_tima_write_c.gb");
  failures += run_microtest("timer_tima_write_d.gb");
  failures += run_microtest("timer_tima_write_e.gb");
  failures += run_microtest("timer_tima_write_f.gb");

  failures += run_microtest("timer_div_phase_c.gb");
  failures += run_microtest("timer_div_phase_d.gb");

  failures += run_microtest("timer_tma_write_a.gb");
  failures += run_microtest("timer_tma_write_b.gb");

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_ppu() {
  TEST_START();

#ifdef RUN_SLOW_TESTS
  failures += run_microtest("line_153_ly_a.gb");
  failures += run_microtest("line_153_ly_b.gb");
  failures += run_microtest("line_153_ly_c.gb");
  failures += run_microtest("line_153_ly_d.gb");
  failures += run_microtest("line_153_ly_e.gb");
  failures += run_microtest("line_153_ly_f.gb");
  failures += run_microtest("line_153_lyc0_int_inc_sled.gb");  // failing
#endif

  failures += run_microtest("lyc1_write_timing_a.gb");
  failures += run_microtest("lyc1_write_timing_b.gb");
  failures += run_microtest("lyc1_write_timing_c.gb");
  failures += run_microtest("lyc1_write_timing_d.gb");

  failures += run_microtest("stat_write_glitch_l0_a.gb"); // failing
  failures += run_microtest("stat_write_glitch_l0_b.gb"); // failing
  failures += run_microtest("stat_write_glitch_l0_c.gb");
  failures += run_microtest("stat_write_glitch_l1_a.gb");
  failures += run_microtest("stat_write_glitch_l1_b.gb"); // failing
  failures += run_microtest("stat_write_glitch_l1_c.gb"); // failing
  failures += run_microtest("stat_write_glitch_l1_d.gb");

  failures += run_microtest("ppu_sprite0_scx0_b.gb");
  failures += run_microtest("ppu_sprite0_scx2_a.gb");
  failures += run_microtest("ppu_sprite0_scx2_b.gb");
  failures += run_microtest("ppu_sprite0_scx3_a.gb"); // failing
  failures += run_microtest("ppu_sprite0_scx3_b.gb");
  failures += run_microtest("ppu_sprite0_scx4_a.gb");
  failures += run_microtest("ppu_sprite0_scx4_b.gb");
  failures += run_microtest("ppu_sprite0_scx5_a.gb");
  failures += run_microtest("ppu_sprite0_scx5_b.gb");
  failures += run_microtest("ppu_sprite0_scx6_a.gb");
  failures += run_microtest("ppu_sprite0_scx6_b.gb");
  failures += run_microtest("ppu_sprite0_scx7_a.gb"); // failing
  failures += run_microtest("ppu_sprite0_scx7_b.gb");

  failures += run_microtest("sprite4_0_a.gb");
  failures += run_microtest("sprite4_0_b.gb");
  failures += run_microtest("sprite4_1_a.gb");
  failures += run_microtest("sprite4_1_b.gb");
  failures += run_microtest("sprite4_2_a.gb");
  failures += run_microtest("sprite4_2_b.gb");
  failures += run_microtest("sprite4_3_a.gb");
  failures += run_microtest("sprite4_3_b.gb");
  failures += run_microtest("sprite4_4_a.gb");
  failures += run_microtest("sprite4_4_b.gb");
  failures += run_microtest("sprite4_5_a.gb");
  failures += run_microtest("sprite4_5_b.gb");
  failures += run_microtest("sprite4_6_a.gb");
  failures += run_microtest("sprite4_6_b.gb");
  failures += run_microtest("sprite4_7_a.gb");
  failures += run_microtest("sprite4_7_b.gb");

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_micro_window() {
  TEST_START();

  failures += run_microtest("win0_scx3_a.gb");
  failures += run_microtest("win0_scx3_b.gb");
  failures += run_microtest("win10_scx3_a.gb");
  failures += run_microtest("win10_scx3_b.gb");

  failures += run_microtest("win0_a.gb");
  failures += run_microtest("win0_b.gb");
  failures += run_microtest("win1_a.gb");
  failures += run_microtest("win1_b.gb");
  failures += run_microtest("win2_a.gb");
  failures += run_microtest("win2_b.gb");
  failures += run_microtest("win3_a.gb");
  failures += run_microtest("win3_b.gb");
  failures += run_microtest("win4_a.gb");
  failures += run_microtest("win4_b.gb");
  failures += run_microtest("win5_a.gb");
  failures += run_microtest("win5_b.gb");
  failures += run_microtest("win6_a.gb");
  failures += run_microtest("win6_b.gb");
  failures += run_microtest("win7_a.gb");
  failures += run_microtest("win7_b.gb");
  failures += run_microtest("win8_a.gb");
  failures += run_microtest("win8_b.gb");
  failures += run_microtest("win9_a.gb");
  failures += run_microtest("win9_b.gb");

  failures += run_microtest("win10_a.gb");
  failures += run_microtest("win10_b.gb");
  failures += run_microtest("win11_a.gb");
  failures += run_microtest("win11_b.gb");
  failures += run_microtest("win12_a.gb");
  failures += run_microtest("win12_b.gb");
  failures += run_microtest("win13_a.gb");
  failures += run_microtest("win13_b.gb");
  failures += run_microtest("win14_a.gb");
  failures += run_microtest("win14_b.gb");
  failures += run_microtest("win15_a.gb");
  failures += run_microtest("win15_b.gb");

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::run_microtest(const char* filename) {
  cart_rom = load_blob(std::string("microtests/build/dmg/") + filename);

  if (cart_rom.empty()) {
    LOG_B("%-30s ", filename);
    LOG_Y("FILE NOT FOUND\n");
    return 1;
  }

  if (verbose) LOG_B("%-30s ", filename);

  GateBoy gb;
  gb.load_cart(DMG_ROM_blob.data(), DMG_ROM_blob.size(), cart_rom.data(), cart_rom.size());
  gb.reset_to_cart();
  gb.phase_total = 0;

  //int timeout = 500; // All our "fast" microtests take under 500 cycles
  int timeout = 150000; // All our "fast" microtests take under 500 cycles
  int mcycle = 0;
  for (; mcycle < timeout; mcycle++) {
    gb.run_phases(8);
    if (gb.zero_ram[2]) break;
  }

  uint8_t result_a = gb.zero_ram[0]; // actual
  uint8_t result_b = gb.zero_ram[1]; // expected
  uint8_t result_c = gb.zero_ram[2]; // 0x01 if test passes, 0xFF if test fails

  bool pass = (result_c == 0x01) && (mcycle < timeout);

  if (pass) {
    if (verbose) LOG_G("%4d %4d %4d %4d PASS @ %d\n", result_a, result_b, (result_a - result_b), result_c, mcycle);
    return 0;
  }
  else {
    if (!verbose) LOG_B("%-30s ", filename);

    const char* reason = "ERROR";
    if      (mcycle == timeout)    reason = "TIMEOUT";
    else if (result_a != result_b) reason = "MISMATCH";
    else if (result_c == 0xFF)     reason = "FAIL";

    LOG_R("%4d %4d %4d %4d %s @ %d\n", result_a, result_b, (result_a - result_b), result_c, reason, gb.phase_total);
    return 1;
  }
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_init() {
  TEST_START();

  GateBoy gb = create_gb_poweron();

  LOG_G("Checking reg flags\n");

  LOG_G("Checking mem\n");
  // Mem should be clear
  for (int i = 0; i < 8192; i++)  ASSERT_EQ(0, gb.cart_ram[i]);
  for (int i = 0; i < 8192; i++)  ASSERT_EQ(0, gb.int_ram[i]);

  // Framebuffer should be 0x04 (yellow) except for the first pixel, which
  // always gets written to because XONA_LCDCENn is 0 at boot

  LOG_G("Checking framebuffer\n");
  for (int i = 1; i < 160*144; i++) {
    ASSERT_EQ(4, gb.framebuffer[i], "bad framebuffer at %d\n", i);
  }

  LOG_G("Checking reg values\n");
  EXPECT_EQ(0xCF, gb.dbg_read(ADDR_P1),   "Bad P1 reset_states value");   // CF after bootrom
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_SB),   "Bad SB reset_states value");   // 00 after bootrom
  EXPECT_EQ(0x7E, gb.dbg_read(ADDR_SC),   "Bad SC reset_states value");   // 7E after bootrom
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_DIV),  "Bad DIV reset_states value");  // AB after bootrom
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_TIMA), "Bad TIMA reset_states value"); // 00 after bootrom
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_TMA),  "Bad TMA reset_states value");  // 00 after bootrom
  EXPECT_EQ(0xF8, gb.dbg_read(ADDR_TAC),  "Bad TAC reset_states value");  // F8 after bootrom
  EXPECT_EQ(0xE0, gb.dbg_read(ADDR_IF),   "Bad IF reset_states value");   // E1 after bootrom
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_LCDC), "Bad LCDC reset_states value"); // 91 after bootrom
  EXPECT_EQ(0x80, gb.dbg_read(ADDR_STAT), "Bad STAT reset value");        // 85 after bootrom
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_SCY),  "Bad SCY reset_states value");  // 00 after bootrom
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_SCX),  "Bad SCX reset_states value");  // 00 after bootrom
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_LY),   "Bad LY reset_states value");   // 00 after bootrom
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_LYC),  "Bad LYC reset_states value");  // 00 after bootrom

  EXPECT_EQ(0xFF, gb.dbg_read(ADDR_DMA),  "Bad DMA reset_states value");  // FF after bootrom
  EXPECT_EQ(0xFF, gb.dbg_read(ADDR_BGP),  "Bad BGP reset_states value");  // FC after bootrom
  EXPECT_EQ(0xFF, gb.dbg_read(ADDR_OBP0), "Bad OBP0 reset_states value"); // 9F after bootrom
  EXPECT_EQ(0xFF, gb.dbg_read(ADDR_OBP1), "Bad OBP1 reset_states value"); // FF after bootrom

  EXPECT_EQ(0x00, gb.dbg_read(ADDR_WY),   "Bad WY reset_states value");   // 00 after bootrom
  EXPECT_EQ(0x00, gb.dbg_read(ADDR_WX),   "Bad WX reset_states value");   // 00 after bootrom

  TEST_END();
}

//-----------------------------------------------------------------------------

#define EXPECT_CLK(A, B) EXPECT_EQ(bit(A), ((B) >> (7 - phase)) & 1, "Clock phase mismatch, %s at phase %d", #A, phase);

int GateBoyTests::test_clk() {
  TEST_START();

  GateBoy gb = create_gb_poweron();
  gb.dbg_write(ADDR_LCDC, 0x80);
  gb.run_phases(8);

  auto& top = gb;
  auto& clk = top.clk;

  for (int i = 0; i < 32; i++) {
    int phase = gb.phase_total & 7;
    EXPECT_CLK(clk.AFUR_xxxxEFGHp.qp_old(), 0b00001111);
    EXPECT_CLK(clk.ALEF_AxxxxFGHp.qp_old(), 0b10000111);
    EXPECT_CLK(clk.APUK_ABxxxxGHp.qp_old(), 0b11000011);
    EXPECT_CLK(clk.ADYK_ABCxxxxHp.qp_old(), 0b11100001);

    EXPECT_CLK(clk.WUVU_ABxxEFxx.qp_old(), 0b11001100);
    EXPECT_CLK(clk.VENA_xxCDEFxx.qp_old(), 0b00111100);
    EXPECT_CLK(clk.WOSU_AxxDExxH.qp_old(), 0b10011001);

    EXPECT_CLK(top.clk.SIG_CPU_BOWA_Axxxxxxx.qp_old(), 0b10000000);
    EXPECT_CLK(top.clk.SIG_CPU_BEDO_xBCDEFGH.qp_old(), 0b01111111);
    EXPECT_CLK(top.clk.SIG_CPU_BEKO_ABCDxxxx.qp_old(), 0b11110000);
    EXPECT_CLK(top.clk.SIG_CPU_BUDE_xxxxEFGH.qp_old(), 0b00001111);
    EXPECT_CLK(top.clk.SIG_CPU_BOLO_ABCDEFxx.qp_old(), 0b11111100);
    EXPECT_CLK(top.clk.SIG_CPU_BUKE_AxxxxxGH.qp_old(), 0b10000011);
    EXPECT_CLK(top.clk.SIG_CPU_BOMA_xBCDEFGH.qp_old(), 0b01111111);
    EXPECT_CLK(top.clk.SIG_CPU_BOGA_Axxxxxxx.qp_old(), 0b10000000);
    EXPECT_CLK(top.pins.PIN_75_CLK_OUT.qp_ext_old(),    0b11110000);
    gb.next_phase();
  }

  TEST_END();
}

//-----------------------------------------------------------------------------

#pragma warning(disable : 4189) // unref var

int GateBoyTests::test_ext_bus() {
  TEST_START();

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
    cart_rom = as.link();

    GateBoy gb;
    gb.load_cart(DMG_ROM_blob.data(), DMG_ROM_blob.size(), cart_rom.data(), cart_rom.size());
    gb.reset_to_cart();
    gb.run_phases(120);

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
    const char* D01_WAVE = "^^111111 ^1110000 ^^000000 ^^000000 ^^^^^^^^"; // the pullups are the only thing driving the new_bus.
    const char* D02_WAVE = "^^111111 ^1111111 ^^000000 ^^111111 ^^^^^^^^"; // #
    const char* D03_WAVE = "^^000000 ^1110000 ^^111111 ^^111111 ^^^^^^^^"; // #
    const char* D04_WAVE = "^^111111 ^1111111 ^^111111 ^^111111 ^^^^^^^^"; // #
    const char* D05_WAVE = "^^111111 ^1110000 ^^000000 ^^111111 ^^^^^^^^"; // #
    const char* D06_WAVE = "^^111111 ^1111111 ^^000000 ^^111111 ^^^^^^^^"; // #
    const char* D07_WAVE = "^^000000 ^1110000 ^^000000 ^^111111 ^^^^^^^^"; // #

    for (int i = 0; i < 40; i++) {
      char CLK = gb.pins.PIN_75_CLK_OUT.ext_c();
      char WRn = gb.ext_bus.PIN_78_WRn.ext_c();
      char RDn = gb.ext_bus.PIN_79_RDn.ext_c();
      char CSn = gb.ext_bus.PIN_80_CSn.ext_c();

      char A00 = gb.ext_bus.PIN_01_A00.ext_c();
      char A01 = gb.ext_bus.PIN_02_A01.ext_c();
      char A02 = gb.ext_bus.PIN_03_A02.ext_c();
      char A03 = gb.ext_bus.PIN_04_A03.ext_c();
      char A04 = gb.ext_bus.PIN_05_A04.ext_c();
      char A05 = gb.ext_bus.PIN_06_A05.ext_c();
      char A06 = gb.ext_bus.PIN_07_A06.ext_c();
      char A07 = gb.ext_bus.PIN_08_A07.ext_c();
      char A08 = gb.ext_bus.PIN_09_A08.ext_c();
      char A09 = gb.ext_bus.PIN_10_A09.ext_c();
      char A10 = gb.ext_bus.PIN_11_A10.ext_c();
      char A11 = gb.ext_bus.PIN_12_A11.ext_c();
      char A12 = gb.ext_bus.PIN_13_A12.ext_c();
      char A13 = gb.ext_bus.PIN_14_A13.ext_c();
      char A14 = gb.ext_bus.PIN_15_A14.ext_c();
      char A15 = gb.ext_bus.PIN_16_A15.ext_c();

      char D00 = gb.ext_bus.PIN_17_D00.ext_c();
      char D01 = gb.ext_bus.PIN_18_D01.ext_c();
      char D02 = gb.ext_bus.PIN_19_D02.ext_c();
      char D03 = gb.ext_bus.PIN_20_D03.ext_c();
      char D04 = gb.ext_bus.PIN_21_D04.ext_c();
      char D05 = gb.ext_bus.PIN_22_D05.ext_c();
      char D06 = gb.ext_bus.PIN_23_D06.ext_c();
      char D07 = gb.ext_bus.PIN_24_D07.ext_c();

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
    cart_rom = as.link();

    GateBoy gb;
    gb.load_cart(DMG_ROM_blob.data(), DMG_ROM_blob.size(), cart_rom.data(), cart_rom.size());
    gb.reset_to_cart();
    gb.run_phases(120);

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
      char CLK = gb.pins.PIN_75_CLK_OUT.ext_c();
      char WRn = gb.ext_bus.PIN_78_WRn.ext_c();
      char RDn = gb.ext_bus.PIN_79_RDn.ext_c();
      char CSn = gb.ext_bus.PIN_80_CSn.ext_c();

      char A00 = gb.ext_bus.PIN_01_A00.ext_c();
      char A01 = gb.ext_bus.PIN_02_A01.ext_c();
      char A02 = gb.ext_bus.PIN_03_A02.ext_c();
      char A03 = gb.ext_bus.PIN_04_A03.ext_c();
      char A04 = gb.ext_bus.PIN_05_A04.ext_c();
      char A05 = gb.ext_bus.PIN_06_A05.ext_c();
      char A06 = gb.ext_bus.PIN_07_A06.ext_c();
      char A07 = gb.ext_bus.PIN_08_A07.ext_c();
      char A08 = gb.ext_bus.PIN_09_A08.ext_c();
      char A09 = gb.ext_bus.PIN_10_A09.ext_c();
      char A10 = gb.ext_bus.PIN_11_A10.ext_c();
      char A11 = gb.ext_bus.PIN_12_A11.ext_c();
      char A12 = gb.ext_bus.PIN_13_A12.ext_c();
      char A13 = gb.ext_bus.PIN_14_A13.ext_c();
      char A14 = gb.ext_bus.PIN_15_A14.ext_c();
      char A15 = gb.ext_bus.PIN_16_A15.ext_c();

      char D00 = gb.ext_bus.PIN_17_D00.ext_c();
      char D01 = gb.ext_bus.PIN_18_D01.ext_c();
      char D02 = gb.ext_bus.PIN_19_D02.ext_c();
      char D03 = gb.ext_bus.PIN_20_D03.ext_c();
      char D04 = gb.ext_bus.PIN_21_D04.ext_c();
      char D05 = gb.ext_bus.PIN_22_D05.ext_c();
      char D06 = gb.ext_bus.PIN_23_D06.ext_c();
      char D07 = gb.ext_bus.PIN_24_D07.ext_c();

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
    cart_rom = as.link();

    GateBoy gb;
    gb.load_cart(DMG_ROM_blob.data(), DMG_ROM_blob.size(), cart_rom.data(), cart_rom.size());
    gb.reset_to_cart();
    gb.run_phases(120);

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
    // 0xFFxx addresses are _not_ external, so the address shows up on the new_bus.

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
      char CLK = gb.pins.PIN_75_CLK_OUT.ext_c();
      char WRn = gb.ext_bus.PIN_78_WRn.ext_c();
      char RDn = gb.ext_bus.PIN_79_RDn.ext_c();
      char CSn = gb.ext_bus.PIN_80_CSn.ext_c();

      char A00 = gb.ext_bus.PIN_01_A00.ext_c();
      char A01 = gb.ext_bus.PIN_02_A01.ext_c();
      char A02 = gb.ext_bus.PIN_03_A02.ext_c();
      char A03 = gb.ext_bus.PIN_04_A03.ext_c();
      char A04 = gb.ext_bus.PIN_05_A04.ext_c();
      char A05 = gb.ext_bus.PIN_06_A05.ext_c();
      char A06 = gb.ext_bus.PIN_07_A06.ext_c();
      char A07 = gb.ext_bus.PIN_08_A07.ext_c();
      char A08 = gb.ext_bus.PIN_09_A08.ext_c();
      char A09 = gb.ext_bus.PIN_10_A09.ext_c();
      char A10 = gb.ext_bus.PIN_11_A10.ext_c();
      char A11 = gb.ext_bus.PIN_12_A11.ext_c();
      char A12 = gb.ext_bus.PIN_13_A12.ext_c();
      char A13 = gb.ext_bus.PIN_14_A13.ext_c();
      char A14 = gb.ext_bus.PIN_15_A14.ext_c();
      char A15 = gb.ext_bus.PIN_16_A15.ext_c();

      char D00 = gb.ext_bus.PIN_17_D00.ext_c();
      char D01 = gb.ext_bus.PIN_18_D01.ext_c();
      char D02 = gb.ext_bus.PIN_19_D02.ext_c();
      char D03 = gb.ext_bus.PIN_20_D03.ext_c();
      char D04 = gb.ext_bus.PIN_21_D04.ext_c();
      char D05 = gb.ext_bus.PIN_22_D05.ext_c();
      char D06 = gb.ext_bus.PIN_23_D06.ext_c();
      char D07 = gb.ext_bus.PIN_24_D07.ext_c();

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

  TEST_END();
}

//-----------------------------------------------------------------------------

int GateBoyTests::test_mem() {
  TEST_START();

  failures += test_mem("OAM",  0xFE00, 0xFEFF, 1,   true);
  failures += test_mem("ROM",  0x0000, 0x7FFF, 256, false);
  failures += test_mem("VRAM", 0x8000, 0x9FFF, 256, true);
  failures += test_mem("CRAM", 0xA000, 0xBFFF, 256, true);
  failures += test_mem("IRAM", 0xC000, 0xDFFF, 256, true);
  failures += test_mem("ERAM", 0xE000, 0xFDFF, 256, true);
  failures += test_mem("ZRAM", 0xFF80, 0xFFFE, 1,   true);

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_bootrom() {
  TEST_START();

  GateBoy gb = create_gb_poweron();

  for (int i = 0; i < 16; i++) {
    uint8_t byte = gb.dbg_read(i);
    EXPECT_EQ(byte, DMG_ROM_blob[i], "bootrom @ 0x%04x = 0x%02x, expected 0x%02x", i, byte, DMG_ROM_bin[i]);
  }

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_timer() {
  TEST_START();

  // TAC 100 - 2048 phases per TIMA tick
  // TAC 101 - 32 phases per TIMA tick
  // TAC 110 - 128 phases per TIMA tick
  // TAC 111 - 512 phases per TIMA tick

  LOG("Testing TIMA tick rate and reset_states to TMA...\n");
  {
    GateBoy gb = create_gb_poweron();

    gb.dbg_write(ADDR_TMA, 0x80);
    gb.dbg_write(ADDR_TIMA,0xFD);
    gb.dbg_write(ADDR_DIV, 0x00);
    gb.dbg_write(ADDR_TAC, 0b00000100);

    EXPECT_EQ(0xFD, gb.timer.get_tima());
    gb.run_phases(2048);
    EXPECT_EQ(0xFE, gb.timer.get_tima());
    gb.run_phases(2048);
    EXPECT_EQ(0xFF, gb.timer.get_tima());
    gb.run_phases(2048);
    EXPECT_EQ(0x80, gb.timer.get_tima());
    gb.run_phases(2048);
    EXPECT_EQ(0x81, gb.timer.get_tima());
    gb.run_phases(2048);
    if (!failures) LOG_B("TAC 0b100 pass\n");
  }

  {
    GateBoy gb = create_gb_poweron();

    gb.dbg_write(ADDR_TMA, 0x80);
    gb.dbg_write(ADDR_TIMA,0xFD);
    gb.dbg_write(ADDR_DIV, 0x00);
    gb.dbg_write(ADDR_TAC, 0b00000101);

    EXPECT_EQ(0xFD, gb.timer.get_tima());
    gb.run_phases(32);
    EXPECT_EQ(0xFE, gb.timer.get_tima());
    gb.run_phases(32);
    EXPECT_EQ(0xFF, gb.timer.get_tima());
    gb.run_phases(32);
    EXPECT_EQ(0x80, gb.timer.get_tima());
    gb.run_phases(32);
    EXPECT_EQ(0x81, gb.timer.get_tima());
    gb.run_phases(32);
    if (!failures) LOG_B("TAC 0b101 pass\n");
  }
  {
    GateBoy gb = create_gb_poweron();

    gb.dbg_write(ADDR_TMA, 0x80);
    gb.dbg_write(ADDR_TIMA,0xFD);
    gb.dbg_write(ADDR_DIV, 0x00);
    gb.dbg_write(ADDR_TAC, 0b00000110);

    EXPECT_EQ(0xFD, gb.timer.get_tima());
    gb.run_phases(128);
    EXPECT_EQ(0xFE, gb.timer.get_tima());
    gb.run_phases(128);
    EXPECT_EQ(0xFF, gb.timer.get_tima());
    gb.run_phases(128);
    EXPECT_EQ(0x80, gb.timer.get_tima());
    gb.run_phases(128);
    EXPECT_EQ(0x81, gb.timer.get_tima());
    gb.run_phases(128);
    if (!failures) LOG_B("TAC 0b110 pass\n");
  }
  {
    GateBoy gb = create_gb_poweron();

    gb.dbg_write(ADDR_TMA, 0x80);
    gb.dbg_write(ADDR_TIMA,0xFD);
    gb.dbg_write(ADDR_DIV, 0x00);
    gb.dbg_write(ADDR_TAC, 0b00000111);

    EXPECT_EQ(0xFD, gb.timer.get_tima());
    gb.run_phases(512);
    EXPECT_EQ(0xFE, gb.timer.get_tima());
    gb.run_phases(512);
    EXPECT_EQ(0xFF, gb.timer.get_tima());
    gb.run_phases(512);
    EXPECT_EQ(0x80, gb.timer.get_tima());
    gb.run_phases(512);
    EXPECT_EQ(0x81, gb.timer.get_tima());
    gb.run_phases(512);
    if (!failures) LOG_B("TAC 0b111 pass\n");
  }
  if (!failures) LOG("\n");

#ifdef RUN_SLOW_TESTS
  {
    GateBoy gb = create_gb_poweron();
    gb.sys_cpu_en = 0;

    // passes, but slow :/
    LOG("Testing div reset_states + rollover, this takes a minute...");
    gb.dbg_write(ADDR_DIV, 0);
    for (int i = 1; i < 32768; i++) {
      int div_a = gb.dbg_read(ADDR_DIV);
      int div_b = (i >> 6) & 0xFF;
      EXPECT_EQ(div_a, div_b, "div match fail");
    }
    LOG("\n");
  }
#endif

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_dma() {
  TEST_START();

  for (int src = 0x0000; src < 0xFE00; src += 0x1000) {
    failures += test_dma(uint16_t(src));
  }

  TEST_END();
}

//----------------------------------------

uint8_t* get_flat_ptr(GateBoy& gb, uint16_t addr) {
  if (addr >= 0x0000 && addr <= 0x7FFF) {
    return gb.cart_buf + (addr & 0x7FFF);
  }
  else if (addr >= 0x8000 && addr <= 0x9FFF) {
    return gb.vid_ram + (addr & 0x1FFF);
  }
  else if (addr >= 0xA000 && addr <= 0xBFFF) {
    return gb.cart_ram + (addr & 0x1FFF);
  }
  else if (addr >= 0xC000 && addr <= 0xDFFF) {
    return gb.int_ram + (addr & 0x1FFF);
  }
  else if (addr >= 0xE000 && addr <= 0xFDFF) {
    return gb.int_ram + (addr & 0x1FFF);
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
  gb.load_cart(DMG_ROM_blob.data(), DMG_ROM_blob.size(), cart_rom.data(), cart_rom.size());
  gb.reset_to_cart();
  gb.sys_cpu_en = 0;
  gb.dbg_write(ADDR_LCDC, 0);

  uint8_t* mem = get_flat_ptr(gb, src);
  for (int i = 0; i < 256; i++) {
    mem[i] = uint8_t(rand());
    gb.oam_ram[i] = 0xFF;
  }

  gb.dbg_write(0xFF46, uint8_t(src >> 8));
  gb.run_phases(1288);

  for (int i = 0; i < 160; i++) {
    uint8_t a = mem[i];
    uint8_t b = gb.oam_ram[i];
    ASSERT_EQ(a, b, "dma mismatch @ 0x%04x : expected 0x%02x, got 0x%02x", src + i, a, b);
  }

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_ppu() {
  TEST_START();

  // slow
#ifdef RUN_SLOW_TESTS
  LOG("Checking LY increment rate... ");
  GateBoy gb = create_gb_poweron();
  gb.dbg_write(ADDR_LCDC, 0x80);

  // LY should increment every 114*8 phases after LCD enable, except on the last line.
  for (int i = 0; i < 153; i++) {
    EXPECT_EQ(i, gb.lcd.reg_ly.get_old());
    gb.run_phases(114 * 8);
  }

  // LY is reset early on the last line, we should be at 0 now.
  EXPECT_EQ(0, gb.lcd.reg_ly.get_old());
  gb.run_phases(114 * 8);

  // And we should be at 0 again
  EXPECT_EQ(0, gb.lcd.reg_ly.get_old());
  gb.run_phases(114 * 8);

  // And now we should be at 1.
  EXPECT_EQ(1, gb.lcd.reg_ly.get_old());

  if (!failures) LOG_B("Pass");
#endif
  LOG("\n");

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_mem(const char* tag, uint16_t addr_start, uint16_t addr_end, uint16_t step, bool test_write) {
  TEST_START("%-4s @ [0x%04x,0x%04x], step %3d write %d", tag, addr_start, addr_end, step, test_write);

  GateBoy gb = create_gb_poweron();
  gb.dbg_write(0xFF50, 1);

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
    ASSERT_EQ(mem[i], data_wr,  "WRITE FAIL addr 0x%04x : wrote 0x%02x, read 0x%02x", addr_start + i, data_wr, mem[i]);
    uint8_t data_rd = gb.dbg_read(addr_start + i);
    ASSERT_EQ(data_rd, data_wr, "READ FAIL  addr 0x%04x : wrote 0x%02x, read 0x%02x", addr_start + i, data_wr, data_rd);
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
    ASSERT_EQ(mem[i], data_wr,  "WRITE FAIL addr 0x%04x : wrote 0x%02x, read 0x%02x", addr_start + i, data_wr, mem[i]);
    uint8_t data_rd = gb.dbg_read(addr_start + i);
    ASSERT_EQ(data_rd, data_wr, "READ FAIL  addr 0x%04x : wrote 0x%02x, read 0x%02x", addr_start + i, data_wr, data_rd);
  }

  TEST_END();
}

//------------------------------------------------------------------------------

int GateBoyTests::test_reg(const char* tag, uint16_t addr, uint8_t mask) {
  TEST_START("%-4s @ 0x%04x, mask 0x%02x", tag, addr, mask);

  GateBoy gb = create_gb_poweron();

  for (int i = 0; i < 256; i++) {
    uint8_t data_in = uint8_t(i & mask);
    gb.dbg_write(addr, uint8_t(data_in));
    uint8_t data_out = gb.dbg_read(addr) & mask;
    ASSERT_EQ(data_in, data_out, "reg %s @ 0x%04x: wrote 0x%02x, read 0x%02x", tag, addr, data_in, data_out);
  }

  TEST_END();
}

//-----------------------------------------------------------------------------

void GateBoyTests::run_benchmark() {
  GateBoy gb;

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

  LOG("Running perf test");
  for (int iter = 0; iter < iter_count; iter++) {
    // FIXME should probably benchmark something other than the bootrom...
    gb.load_cart(DMG_ROM_blob.data(), DMG_ROM_blob.size(), nullptr, 0);
    gb.reset_to_bootrom(true);
    gb.bus_req_new.addr = 0x0150;
    gb.bus_req_new.data = 0;
    gb.bus_req_new.read = 1;
    gb.bus_req_new.write = 0;
    gb.sys_cpu_en = false;
    gb.phase_total = 0;

    auto start = timestamp();
    gb.run_phases(phase_per_iter);
    auto finish = timestamp();

    if (iter >= warmup) {
      double time = (finish - start);

      double phase_rate = double(gb.phase_total) / time;
      phase_rate_sum1 += phase_rate;
      phase_rate_sum2 += phase_rate * phase_rate;
      phase_rate_n++;
    }
    LOG(".");
  }
  LOG("Done\n");

  LOG("Phase total %d\n", gb.phase_total);

  double phase_rate_mean     = phase_rate_sum1 / phase_rate_n;
  double phase_rate_variance = (phase_rate_sum2 / phase_rate_n) - (phase_rate_mean * phase_rate_mean);
  double phase_rate_sigma    = sqrt(phase_rate_variance);
  LOG("Mean phase/sec %f sigma %f\n", phase_rate_mean, phase_rate_sigma);
}

//-----------------------------------------------------------------------------
