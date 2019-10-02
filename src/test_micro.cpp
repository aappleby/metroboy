#include "Gameboy.h"

#include <string>

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#pragma warning(disable : 4996)
extern uint8_t rom_buf[];

static const std::string micro_tests[] = {
  "timer_int_inc_sled",

  "timer_div_phase_a",
  "timer_div_phase_b",
  "timer_div_phase_c",
  "timer_div_phase_d",

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
};

static const std::string micro_tests2[] = {
  "004-tima_boot_phase",

  //"ppu_sprite0_scx0_a", // where did this one go?
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


  "win0_a", "win0_b",
  "win1_a", "win1_b",
  "win2_a", "win2_b",
  "win3_a", "win3_b",
  "win4_a", "win4_b",
  "win5_a", "win5_b",
  "win6_a", "win6_b",
  "win7_a", "win7_b",
  "win8_a", "win8_b",
  "win9_a", "win9_b",
  "win10_a", "win10_b",
  "win11_a", "win11_b",
  "win12_a", "win12_b",
  "win13_a", "win13_b",
  "win14_a", "win14_b",
  "win15_a", "win15_b",

  "win0_scx3_a", "win0_scx3_b",
  "win10_scx3_a", "win10_scx3_b",


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

  "-----",
  "oam_read_l0_a",
  "oam_read_l0_b",
  "oam_read_l0_c",
  "oam_read_l0_d",

  "-----",
  "oam_read_l1_a",
  "oam_read_l1_b",
  "oam_read_l1_c",
  "oam_read_l1_d",
  "oam_read_l1_e",
  "oam_read_l1_f",

  "-----",
  "oam_write_l0_a",
  "oam_write_l0_b",
  "oam_write_l0_c",
  "oam_write_l0_d", 
  "oam_write_l0_e",

  "-----",
  "oam_write_l1_a",
  "oam_write_l1_b",
  "oam_write_l1_c",
  "oam_write_l1_d",
  "oam_write_l1_e",
  "oam_write_l1_f",

  "-----",
  "vram_read_l0_a",
  "vram_read_l0_b",
  "vram_read_l0_c",
  "vram_read_l0_d",

  "-----",
  "vram_read_l1_a",
  "vram_read_l1_b",
  "vram_read_l1_c",
  "vram_read_l1_d",

  "-----",
  "vram_write_l0_a",
  "vram_write_l0_b",
  "vram_write_l0_c",
  "vram_write_l0_d",

  "-----",
  "vram_write_l1_a",
  "vram_write_l1_b",
  "vram_write_l1_c",
  "vram_write_l1_d",

  "-----",
  "hblank_int_scx0_halt_a",
  "hblank_int_scx1_halt_a",
  "hblank_int_scx2_halt_a",
  "hblank_int_scx3_halt_a",
  "hblank_int_scx4_halt_a",
  "hblank_int_scx5_halt_a",
  "hblank_int_scx6_halt_a",
  "hblank_int_scx7_halt_a",
  "-----",
  "hblank_int_scx0_halt_b",
  "hblank_int_scx1_halt_b",
  "hblank_int_scx2_halt_b",
  "hblank_int_scx3_halt_b",
  "hblank_int_scx4_halt_b",
  "hblank_int_scx5_halt_b",
  "hblank_int_scx6_halt_b",
  "hblank_int_scx7_halt_b",
  "-----",
  "hblank_int_scx0_nops_a",
  "hblank_int_scx1_nops_a",
  "hblank_int_scx2_nops_a",
  "hblank_int_scx3_nops_a",
  "hblank_int_scx4_nops_a",
  "hblank_int_scx5_nops_a",
  "hblank_int_scx6_nops_a",
  "hblank_int_scx7_nops_a",
  "-----",
  "hblank_int_scx0_nops_b",
  "hblank_int_scx1_nops_b",
  "hblank_int_scx2_nops_b",
  "hblank_int_scx3_nops_b",
  "hblank_int_scx4_nops_b",
  "hblank_int_scx5_nops_b",
  "hblank_int_scx6_nops_b",
  "hblank_int_scx7_nops_b",
  "-----",
  "hblank_int_scx0_if_a",
  "hblank_int_scx1_if_a",
  "hblank_int_scx2_if_a",
  "hblank_int_scx3_if_a",
  "hblank_int_scx4_if_a",
  "hblank_int_scx5_if_a",
  "hblank_int_scx6_if_a",
  "hblank_int_scx7_if_a",
  "-----",
  "hblank_int_scx0_if_b",
  "hblank_int_scx1_if_b",
  "hblank_int_scx2_if_b",
  "hblank_int_scx3_if_b",
  "hblank_int_scx4_if_b",
  "hblank_int_scx5_if_b",
  "hblank_int_scx6_if_b",
  "hblank_int_scx7_if_b",
  "-----",
  "hblank_int_scx0_if_c",
  "hblank_int_scx1_if_c",
  "hblank_int_scx2_if_c",
  "hblank_int_scx3_if_c",
  "hblank_int_scx4_if_c",
  "hblank_int_scx5_if_c",
  "hblank_int_scx6_if_c",
  "hblank_int_scx7_if_c",
  "-----",
  "hblank_int_scx0_if_d",
  "hblank_int_scx1_if_d",
  "hblank_int_scx2_if_d",
  "hblank_int_scx3_if_d",
  "hblank_int_scx4_if_d",
  "hblank_int_scx5_if_d",
  "hblank_int_scx6_if_d",
  "hblank_int_scx7_if_d",

  "-----",
  "poweron_ly_a",
  "poweron_ly_b",

  "-----",
  "oam_int_inc_sled",
  "oam_int_halt_a",
  "oam_int_halt_b",
  "oam_int_nops_a",
  "oam_int_nops_b",
  "oam_int_if_edge_a",
  "oam_int_if_edge_b",
  "oam_int_if_edge_c",
  "oam_int_if_edge_d",
  "oam_int_if_level_a",
  "oam_int_if_level_b",
  "oam_int_if_level_c",
  "oam_int_if_level_d",

  "-----",
  "hblank_int_inc_sled",
  "hblank_int_halt_a",
  "hblank_int_halt_b",
  "hblank_int_nops_a",
  "hblank_int_nops_b",
  "hblank_int_if_a",
  "hblank_int_if_b",
  "hblank_int_if_c",
  "hblank_int_if_d",
  "hblank_scx2_if_a",
  "hblank_scx3_if_a",
  "hblank_scx3_if_b",
  "hblank_scx3_if_c",
  "hblank_scx3_if_d",

  "-----",
  "vblank_int_inc_sled",
  "vblank_int_halt_a",
  "vblank_int_halt_b",
  "vblank_int_nops_a",
  "vblank_int_nops_b",
  "vblank_int_if_a",
  "vblank_int_if_b",
  "vblank_int_if_c",
  "vblank_int_if_d",

  "-----",
  "vblank2_int_inc_sled",
  "vblank2_int_halt_a",
  "vblank2_int_halt_b",
  "vblank2_int_nops_a",
  "vblank2_int_nops_b",
  "vblank2_int_if_a",
  "vblank2_int_if_b",
  "vblank2_int_if_c",
  "vblank2_int_if_d",

  "-----",
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

  "lcdon_to_hblank_di_timing_a",
  "lcdon_to_hblank_di_timing_b",

  "-----",
  "lcdon_to_ly1_a",
  "lcdon_to_ly1_b",
  "lcdon_to_ly2_a",
  "lcdon_to_ly2_b",
  "lcdon_to_ly3_a",
  "lcdon_to_ly3_b",
  "lcdon_to_lyc1_int",
  "lcdon_to_lyc2_int",
  "lcdon_to_lyc3_int",

  "-----",
  "lcdon_to_oam_int_l0",
  "lcdon_to_oam_int_l1",
  "lcdon_to_oam_int_l2",

  "-----",
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

  "-----",
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

  "-----",
  "lyc1_int_halt_a",
  "lyc1_int_halt_b",
  "lyc1_int_nops_a",
  "lyc1_int_nops_b",
  "lyc1_int_if_edge_a",
  "lyc1_int_if_edge_b",
  "lyc1_int_if_edge_c",
  "lyc1_int_if_edge_d",

  "lyc1_write_timing_a",
  "lyc1_write_timing_b",
  "lyc1_write_timing_c",
  "lyc1_write_timing_d",

  "lyc2_int_halt_a",
  "lyc2_int_halt_b",

  "-----",
  "stat_write_glitch_l0_a",
  "stat_write_glitch_l0_b",
  "stat_write_glitch_l0_c",
  "stat_write_glitch_l143_a",
  "stat_write_glitch_l143_b",
  "stat_write_glitch_l143_c",
  "stat_write_glitch_l143_d",
  "stat_write_glitch_l1_a",
  "stat_write_glitch_l1_b",
  "stat_write_glitch_l1_c",
  "stat_write_glitch_l1_d",
};

bool run_microtest(int model, const std::string& prefix, const std::string& name) {
  if (name[0] == '-') {
    return true;
  }

  std::string filename = prefix + name + ".gb";

  FILE* rom_file = NULL;
  rom_file = fopen(filename.c_str(), "rb");

  if (rom_file == NULL) {
    printf("?");
    return false;
  }

  fseek(rom_file, 0, SEEK_END);
  size_t rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  rom_size = fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);

  Gameboy gameboy;
  gameboy.reset(model, rom_size, 0x100);

  uint8_t result = 0xFF;
  int i = 0;
  const int ticks = 100000;  // bits_ram_en needs lots of tcycles
  for (; i < ticks; i++) {
    gameboy.tick();
    //assert(!gameboy.ppu.vram_delay == !gameboy.ppu.fetch_delay);

    gameboy.tock();
    //assert(!gameboy.ppu.vram_delay == !gameboy.ppu.fetch_delay);


    result = gameboy.get_vram()[0];
    if (result) break;
  }

  if (i == ticks) {
    printf("%-50s ", name.c_str());
    printf("? TIMEOUT @ %d\n", i);
    return false;
  }
  else if (result == 0x55) {
    printf(".");
    return true;
    //printf("  0x%02x PASS @ %d\n", result, i);
  }
  else {
    printf("\n");
    printf("%-50s ", name.c_str());
    printf("X 0x%02x FAIL @ %d\n", result, i);
    return false;
  }
}

void run_microtests() {
  double freq = (double)SDL_GetPerformanceFrequency();
  double begin = (double)SDL_GetPerformanceCounter();

  int model = MODEL_DMG;
  std::string model_string = (model == MODEL_DMG ? "dmg" : "ags");
  std::string prefix = "microtests/build/" + model_string + "/";

  printf("---------- Microtests in %s: ----------\n", prefix.c_str());

  int fails = 0;
  for (auto name : micro_tests) {
    bool pass = run_microtest(model, prefix, name);
    if (!pass) fails++;
  }
  printf("\n");

  double end = (double)SDL_GetPerformanceCounter();
  printf("---------- Microtests took %f seconds, %d failures ----------\n", (end - begin) / freq, fails);
}
