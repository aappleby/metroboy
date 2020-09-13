#include "MetroBoyLib/Gameboy.h"
#include "CoreLib/File.h"
#include "CoreLib/Tests.h"


static const std::string all_microtests[] = {
  //"000-oam_lock",
  //"000-write_to_x8000",
  //"001-vram_unlocked",
  //"002-vram_locked",
  //"004-tima_boot_phase",
  //"004-tima_cycle_timer",
  //"007-lcd_on_stat",
  //"400-dma",
  //"500-scx-timing",
  //"800-ppu-latch-scx",
  //"801-ppu-latch-scy",
  //"802-ppu-latch-tileselect",
  //"803-ppu-latch-bgdisplay",
  //"audio1",
  //"audio2",
  //"audio3",
  //"audio4",

  "div_inc_timing_a",
  "div_inc_timing_b",
  
  // pass
  //"dma_0x8000",
  //"dma_0xA000",
  //"dma_0xE000",
  //"dma_0xFE00",
  //"dma_0xFF00",
  //"dma_basic",
  //"dma_timing_a", ?

  //"halt_op_dupe",
  //"halt_op_dupe_delay",

#if 0
  "hblank_int_halt_a",
  "hblank_int_halt_b",
  "hblank_int_if_a",
  "hblank_int_if_b",
  "hblank_int_if_c",
  "hblank_int_if_d",
  "hblank_int_inc_sled",
  "hblank_int_inc_sled2",
  "hblank_int_nops_a",
  "hblank_int_nops_b",
  "hblank_int_scx0_halt_a",
  "hblank_int_scx0_halt_b",
  "hblank_int_scx0_if_a",
  "hblank_int_scx0_if_b",
  "hblank_int_scx0_if_c",
  "hblank_int_scx0_if_d",
  "hblank_int_scx0_nops_a",
  "hblank_int_scx0_nops_b",
  "hblank_int_scx1_halt_a",
  "hblank_int_scx1_halt_b",
  "hblank_int_scx1_if_a",
  "hblank_int_scx1_if_b",
  "hblank_int_scx1_if_c",
  "hblank_int_scx1_if_d",
  "hblank_int_scx1_nops_a",
  "hblank_int_scx1_nops_b",
  "hblank_int_scx2_halt_a",
  "hblank_int_scx2_halt_b",
  "hblank_int_scx2_if_a",
  "hblank_int_scx2_if_b",
  "hblank_int_scx2_if_c",
  "hblank_int_scx2_if_d",
  "hblank_int_scx2_nops_a",
  "hblank_int_scx2_nops_b",
  "hblank_int_scx3_halt_a",
  "hblank_int_scx3_halt_b",
  "hblank_int_scx3_if_a",
  "hblank_int_scx3_if_b",
  "hblank_int_scx3_if_c",
  "hblank_int_scx3_if_d",
  "hblank_int_scx3_nops_a",
  "hblank_int_scx3_nops_b",
  "hblank_int_scx4_halt_a",
  "hblank_int_scx4_halt_b",
  "hblank_int_scx4_if_a",
  "hblank_int_scx4_if_b",
  "hblank_int_scx4_if_c",
  "hblank_int_scx4_if_d",
  "hblank_int_scx4_nops_a",
  "hblank_int_scx4_nops_b",
  "hblank_int_scx5_halt_a",
  "hblank_int_scx5_halt_b",
  "hblank_int_scx5_if_a",
  "hblank_int_scx5_if_b",
  "hblank_int_scx5_if_c",
  "hblank_int_scx5_if_d",
  "hblank_int_scx5_nops_a",
  "hblank_int_scx5_nops_b",
  "hblank_int_scx6_halt_a",
  "hblank_int_scx6_halt_b",
  "hblank_int_scx6_if_a",
  "hblank_int_scx6_if_b",
  "hblank_int_scx6_if_c",
  "hblank_int_scx6_if_d",
  "hblank_int_scx6_nops_a",
  "hblank_int_scx6_nops_b",
  "hblank_int_scx7_halt_a",
  "hblank_int_scx7_halt_b",
  "hblank_int_scx7_if_a",
  "hblank_int_scx7_if_b",
  "hblank_int_scx7_if_c",
  "hblank_int_scx7_if_d",
  "hblank_int_scx7_nops_a",
  "hblank_int_scx7_nops_b",
  "hblank_scx2_if_a",
  "hblank_scx3_if_a",
  "hblank_scx3_if_b",
  "hblank_scx3_if_c",
  "hblank_scx3_if_d",
  "hblank_scx3_int_a",
  "hblank_scx3_int_b",
#endif

  //"is_if_set_during_ime0",

#if 0
  "lcdon_halt_to_vblank_int_a",
  "lcdon_halt_to_vblank_int_b",
  "lcdon_nops_to_vblank_int_a",
  "lcdon_nops_to_vblank_int_b",
  "lcdon_to_hblank_di_timing_a",
  "lcdon_to_hblank_di_timing_b",
  "lcdon_to_hblank_int_l0",
  "lcdon_to_hblank_int_l1",
  "lcdon_to_hblank_int_l2",
  "lcdon_to_hblank_int_scx0",
  "lcdon_to_hblank_int_scx1",
  "lcdon_to_hblank_int_scx2",
  "lcdon_to_hblank_int_scx3",
  "lcdon_to_hblank_int_scx4",
  "lcdon_to_hblank_int_scx5",
  "lcdon_to_hblank_int_scx6",
  "lcdon_to_hblank_int_scx7",
  "lcdon_to_ly1_a",
  "lcdon_to_ly1_b",
  "lcdon_to_ly2_a",
  "lcdon_to_ly2_b",
  "lcdon_to_ly3_a",
  "lcdon_to_ly3_b",
  "lcdon_to_lyc1_int",
  "lcdon_to_lyc2_int",
  "lcdon_to_lyc3_int",
  "lcdon_to_oam_int_l0",
  "lcdon_to_oam_int_l1",
  "lcdon_to_oam_int_l2",
  "lcdon_to_oam_unlock_a",
  "lcdon_to_oam_unlock_b",
  "lcdon_to_stat0_a",
  "lcdon_to_stat0_b",
  "lcdon_to_stat0_c",
  "lcdon_to_stat0_d",
  "lcdon_to_stat1_a",
  "lcdon_to_stat1_b",
  "lcdon_to_stat1_c",
  "lcdon_to_stat1_d",
  "lcdon_to_stat1_e",
  "lcdon_to_stat2_a",
  "lcdon_to_stat2_b",
  "lcdon_to_stat2_c",
  "lcdon_to_stat2_d",
  "lcdon_to_stat3_a",
  "lcdon_to_stat3_b",
  "lcdon_to_stat3_c",
  "lcdon_to_stat3_d",
  "lcdon_write_timing",
#endif

#if 0
  "line_144_oam_int_a",
  "line_144_oam_int_b",
  "line_144_oam_int_c",
  "line_144_oam_int_d",
#endif
  
  "line_153_ly_a",
  "line_153_ly_b",
  "line_153_ly_c",
  "line_153_ly_d",
  "line_153_ly_e",
  "line_153_ly_f",
  "line_153_lyc_a",
  "line_153_lyc_b",
  "line_153_lyc_c",
  "line_153_lyc_int_a",
  "line_153_lyc_int_b",
  "line_153_lyc0_int_inc_sled",
  "line_153_lyc0_stat_timing_a",
  "line_153_lyc0_stat_timing_b",
  "line_153_lyc0_stat_timing_c",
  "line_153_lyc0_stat_timing_d",
  "line_153_lyc0_stat_timing_e",
  "line_153_lyc0_stat_timing_f",
  "line_153_lyc0_stat_timing_g",
  "line_153_lyc0_stat_timing_h",
  "line_153_lyc0_stat_timing_i",
  "line_153_lyc0_stat_timing_j",
  "line_153_lyc0_stat_timing_k",
  "line_153_lyc0_stat_timing_l",
  "line_153_lyc0_stat_timing_m",
  "line_153_lyc0_stat_timing_n",
  "line_153_lyc153_stat_timing_a",
  "line_153_lyc153_stat_timing_b",
  "line_153_lyc153_stat_timing_c",
  "line_153_lyc153_stat_timing_d",
  "line_153_lyc153_stat_timing_e",
  "line_153_lyc153_stat_timing_f",

  "line_65_ly",

  /*
  "lyc1_int_halt_a",
  "lyc1_int_halt_b",
  "lyc1_int_if_edge_a",
  "lyc1_int_if_edge_b",
  "lyc1_int_if_edge_c",
  "lyc1_int_if_edge_d",
  "lyc1_int_nops_a",
  "lyc1_int_nops_b",
  "lyc1_write_timing_a",
  "lyc1_write_timing_b",
  "lyc1_write_timing_c",
  "lyc1_write_timing_d",
  "lyc2_int_halt_a",
  "lyc2_int_halt_b",
  */

  //"mode2_stat_int_to_oam_unlock",

#if 0
  "oam_int_halt_a",
  "oam_int_halt_b",
  "oam_int_if_edge_a",
  "oam_int_if_edge_b",
  "oam_int_if_edge_c",
  "oam_int_if_edge_d",
  "oam_int_if_level_a",
  "oam_int_if_level_b",
  "oam_int_if_level_c",
  "oam_int_if_level_d",
  "oam_int_inc_sled",
  "oam_int_nops_a",
  "oam_int_nops_b",
#endif

#if 0
  "oam_read_l0_a",
  "oam_read_l0_b",
  "oam_read_l0_c",
  "oam_read_l0_d",
  "oam_read_l1_a",
  "oam_read_l1_b",
  "oam_read_l1_c",
  "oam_read_l1_d",
  "oam_read_l1_e",
  "oam_read_l1_f",
  "oam_sprite_trashing",
  "oam_write_l0_a",
  "oam_write_l0_b",
  "oam_write_l0_c",
  "oam_write_l0_d",
  "oam_write_l0_e",
  "oam_write_l1_a",
  "oam_write_l1_b",
  "oam_write_l1_c",
  "oam_write_l1_d",
  "oam_write_l1_e",
  "oam_write_l1_f",
#endif

#if 1
  "poweron_000_div",
  "poweron_000_if",
  "poweron_000_ly",
  "poweron_000_oam",
  "poweron_000_stat",
  "poweron_000_vram",
  "poweron_004_div",
  "poweron_005_div",
  "poweron_005_oam",
  "poweron_005_stat",
  "poweron_006_oam",   // x
  "poweron_006_stat",
  "poweron_007_stat",
  "poweron_025_vram",
  "poweron_026_stat",
  "poweron_026_vram",  // x
  "poweron_027_stat",
  "poweron_069_oam",   // x
  "poweron_069_stat",
  "poweron_069_vram",  // x
  "poweron_070_oam",
  "poweron_070_stat",
  "poweron_070_vram",
  "poweron_119_ly",
  "poweron_119_oam",
  "poweron_119_stat",
  "poweron_120_ly",
  "poweron_120_oam",   // x
  "poweron_120_stat",
  "poweron_121_oam",   // x
  "poweron_121_stat",
  "poweron_139_vram",
  "poweron_140_stat",
  "poweron_140_vram",  // x
  "poweron_141_stat",
  "poweron_183_oam",   // x
  "poweron_183_stat",
  "poweron_183_vram",  // x
  "poweron_184_oam",
  "poweron_184_stat",
  "poweron_184_vram",
  "poweron_233_ly",
  "poweron_233_oam",
  "poweron_234_ly",
  "poweron_234_oam",   // x
  "poweron_234_stat",
  "poweron_235_oam",   // x
  "poweron_235_stat",
  "poweron_oam_read_a",  // x
  "poweron_oam_read_b",
  "poweron_oam_read_c",
  "poweron_oam_read_d",  // x
#endif

#if 0
  "ppu_scx_vs_bgp",
  //"ppu_sprite_testbench",

  "ppu_sprite0_scx0_b",
  "ppu_sprite0_scx1_a",
  "ppu_sprite0_scx1_b",
  "ppu_sprite0_scx2_a",
  "ppu_sprite0_scx2_b",
  "ppu_sprite0_scx3_a",
  "ppu_sprite0_scx3_b",
  "ppu_sprite0_scx4_a",
  "ppu_sprite0_scx4_b",
  "ppu_sprite0_scx5_a",
  "ppu_sprite0_scx5_b",
  "ppu_sprite0_scx6_a",
  "ppu_sprite0_scx6_b",
  "ppu_sprite0_scx7_a",
  "ppu_sprite0_scx7_b",
  "ppu_spritex_vs_scx",
  "ppu_win_vs_wx",
  "ppu_wx_early",
#endif

#if 0
  "sprite_0_a",
  "sprite_0_b",
  "sprite_1_a",
  "sprite_1_b",
  "sprite4_0_a",
  "sprite4_0_b",
  "sprite4_1_a",
  "sprite4_1_b",
  "sprite4_2_a",
  "sprite4_2_b",
  "sprite4_3_a",
  "sprite4_3_b",
  "sprite4_4_a",
  "sprite4_4_b",
  "sprite4_5_a",
  "sprite4_5_b",
  "sprite4_6_a",
  "sprite4_6_b",
  "sprite4_7_a",
  "sprite4_7_b",
#endif

#if 0
  "stat_write_glitch_l0_a",
  "stat_write_glitch_l0_b",
  "stat_write_glitch_l0_c",
  "stat_write_glitch_l1_a",
  "stat_write_glitch_l1_b",
  "stat_write_glitch_l1_c",
  "stat_write_glitch_l1_d",
  "stat_write_glitch_l143_a",
  "stat_write_glitch_l143_b",
  "stat_write_glitch_l143_c",
  "stat_write_glitch_l143_d",
  "stat_write_glitch_l154_a",
  "stat_write_glitch_l154_b",
  "stat_write_glitch_l154_c",
  "stat_write_glitch_l154_d",
#endif

  "timer_div_phase_c",
  "timer_div_phase_d",
  "timer_int_inc_sled",
  "timer_int_inc_sled_a",
  "timer_int_inc_sled_b",
  "timer_tac_toggle",
  "timer_tima_phase_a",
  "timer_tima_phase_b",
  "timer_tima_phase_c",
  "timer_tima_phase_d",
  "timer_tima_phase_e",
  "timer_tima_phase_f",
  "timer_tima_phase_g",
  "timer_tima_phase_h",
  "timer_tima_phase_i",
  "timer_tima_phase_j",
  "timer_tima_write_a",
  "timer_tima_write_b",
  "timer_tima_write_c",
  "timer_tima_write_d",
  "timer_tima_write_e",
  "timer_tima_write_f",
  "timer_tma_load_a",
  "timer_tma_load_b",
  "timer_tma_load_c",
  "timer_tma_write_a",
  "timer_tma_write_b",

#if 0
  "vblank_int_halt_a",
  "vblank_int_halt_b",
  "vblank_int_if_a",
  "vblank_int_if_b",
  "vblank_int_if_c",
  "vblank_int_if_d",
  "vblank_int_inc_sled",
  "vblank_int_nops_a",
  "vblank_int_nops_b",
#endif

#if 0
  "vblank2_int_halt_a",
  "vblank2_int_halt_b",
  "vblank2_int_if_a",
  "vblank2_int_if_b",
  "vblank2_int_if_c",
  "vblank2_int_if_d",
  "vblank2_int_inc_sled",
  "vblank2_int_nops_a",
  "vblank2_int_nops_b",
#endif

#if 0
  "vram_read_l0_a",
  "vram_read_l0_b",
  "vram_read_l0_c",
  "vram_read_l0_d",
  "vram_read_l1_a",
  "vram_read_l1_b",
  "vram_read_l1_c",
  "vram_read_l1_d",
  "vram_write_l0_a",
  "vram_write_l0_b",
  "vram_write_l0_c",
  "vram_write_l0_d",
  "vram_write_l1_a",
  "vram_write_l1_b",
  "vram_write_l1_c",
  "vram_write_l1_d",
#endif

#if 0
  "win0_a",
  "win0_b",
  "win0_scx3_a",
  "win0_scx3_b",
  "win1_a",
  "win1_b",
  "win10_a",
  "win10_b",
  "win10_scx3_a",
  "win10_scx3_b",
  "win11_a",
  "win11_b",
  "win12_a",
  "win12_b",
  "win13_a",
  "win13_b",
  "win14_a",
  "win14_b",
  "win15_a",
  "win15_b",
  "win2_a",
  "win2_b",
  "win3_a",
  "win3_b",
  "win4_a",
  "win4_b",
  "win5_a",
  "win5_b",
  "win6_a",
  "win6_b",
  "win7_a",
  "win7_b",
  "win8_a",
  "win8_b",
  "win9_a",
  "win9_b",
#endif
};
























//------------------------------------------------------------------------------

int run_microtest(const std::string& prefix, const std::string& name) {
  std::string filename = prefix + name + ".gb";
  LOG_B("%-60s ", filename.c_str());

  //TEST_START(filename.c_str());

  blob rom;
  load_array(filename, rom);

  Gameboy gb_local;
  Gameboy* gameboy = &gb_local;

  gameboy->set_rom(rom.data(), rom.size());
  gameboy->reset(0x100);

  uint8_t result = 0xFF;
  int i = 0;
  const int mcycles = 100000;  // bits_ram_en needs lots of tcycles
  for (; i < mcycles; i++) {
    gameboy->mcycle();
    result = gameboy->get_vram()[0];
    if (result) break;
  }

  if (i == mcycles) {
    LOG_R("TIMEOUT\n");
    return 1;
  }

  if (result != 0x55) {
    LOG_R("FAIL @ %d\n", i);
    return 1;
  }

  //EXPECT_NE(i, mcycles,   "TIMEOUT @ %d", i);
  //EXPECT_EQ(0x55, result, "%s FAIL @ %d", name.c_str(), i);

  //TEST_END();

  LOG_G("pass @ %d\n", i);

  return 0;
}

//------------------------------------------------------------------------------

int run_microtests() {
  std::string prefix = "microtests/build/dmg/";
  TEST_START("Microtests in %s:", prefix.c_str());

  double begin = timestamp();

  int fails = 0;
  for (auto name : all_microtests) {
    if (name == "break") break;
    if (name[0] == '-') continue;
    fails += run_microtest(prefix, name);
  }
  double end = timestamp();
  LOG_Y("---------- Microtests took %f seconds, %d failures ----------\n", (end - begin), fails);

  TEST_END();
}

//------------------------------------------------------------------------------
