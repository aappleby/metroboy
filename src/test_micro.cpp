#include "Platform.h"

#include "Common.h"
#include "Gameboy.h"

#include <string>

static const std::string micro_tests[] = {
  "-----",
  "oam_int_inc_sled.gb",
  "oam_int_halt_a.gb",
  "oam_int_halt_b.gb",
  "oam_int_nops_a.gb",
  "oam_int_nops_b.gb",
  "oam_int_if_edge_a.gb",
  "oam_int_if_edge_b.gb",
  "oam_int_if_edge_c.gb",
  "oam_int_if_edge_d.gb",
  "oam_int_if_level_a.gb",
  "oam_int_if_level_b.gb",
  "oam_int_if_level_c.gb",
  "oam_int_if_level_d.gb",

  "-----",
  "hblank_int_inc_sled.gb",
  "hblank_int_halt_a.gb",
  "hblank_int_halt_b.gb",
  "hblank_int_nops_a.gb",
  "hblank_int_nops_b.gb",
  "hblank_int_if_a.gb",
  "hblank_int_if_b.gb",
  "hblank_int_if_c.gb",
  "hblank_int_if_d.gb",

  "-----",
  "vblank_int_inc_sled.gb",
  "vblank_int_halt_a.gb",
  "vblank_int_halt_b.gb",
  "vblank_int_nops_a.gb",
  "vblank_int_nops_b.gb",
  "vblank_int_if_a.gb",
  "vblank_int_if_b.gb",
  "vblank_int_if_c.gb",
  "vblank_int_if_d.gb",

  "-----",
  "vblank2_int_inc_sled.gb",
  "vblank2_int_halt_a.gb",
  "vblank2_int_halt_b.gb",
  "vblank2_int_nops_a.gb",
  "vblank2_int_nops_b.gb",
  "vblank2_int_if_a.gb",
  "vblank2_int_if_b.gb",
  "vblank2_int_if_c.gb",
  "vblank2_int_if_d.gb",

  "-----",
  "lcdon_to_hblank_int_l0.gb",
  "lcdon_to_hblank_int_l1.gb",
  "lcdon_to_hblank_int_l2.gb",
  "lcdon_to_hblank_int_scx0.gb",
  "lcdon_to_hblank_int_scx1.gb",
  "lcdon_to_hblank_int_scx2.gb",
  "lcdon_to_hblank_int_scx3.gb",
  "lcdon_to_hblank_int_scx4.gb",
  "lcdon_to_hblank_int_scx5.gb",
  "lcdon_to_hblank_int_scx6.gb",
  "lcdon_to_hblank_int_scx7.gb",

  "lcdon_to_hblank_di_timing_a.gb",
  "lcdon_to_hblank_di_timing_b.gb",

  "-----",
  "lcdon_to_ly1_a.gb",
  "lcdon_to_ly1_b.gb",
  "lcdon_to_ly2_a.gb",
  "lcdon_to_ly2_b.gb",
  "lcdon_to_ly3_a.gb",
  "lcdon_to_ly3_b.gb",
  "lcdon_to_lyc1_int.gb",
  "lcdon_to_lyc2_int.gb",
  "lcdon_to_lyc3_int.gb",

  "-----",
  "lcdon_to_oam_int_l0.gb",
  "lcdon_to_oam_int_l1.gb",
  "lcdon_to_oam_int_l2.gb",

  "-----",
  "lcdon_to_stat0_a.gb",
  "lcdon_to_stat0_b.gb",
  "lcdon_to_stat0_c.gb",
  "lcdon_to_stat0_d.gb",
  "lcdon_to_stat2_a.gb",
  "lcdon_to_stat2_b.gb",
  "lcdon_to_stat2_c.gb",
  "lcdon_to_stat2_d.gb",
  "lcdon_to_stat3_a.gb",
  "lcdon_to_stat3_b.gb",
  "lcdon_to_stat3_c.gb",
  "lcdon_to_stat3_d.gb",

  "-----",
  "line_153_ly_a.gb",
  "line_153_ly_b.gb",
  "line_153_ly_c.gb",
  "line_153_ly_d.gb",
  "line_153_ly_e.gb",
  "line_153_ly_f.gb",

  "line_153_lyc_a.gb",
  "line_153_lyc_b.gb",
  "line_153_lyc_c.gb",

  "line_153_lyc_int.gb",

  "line_153_lyc0_stat_timing_a.gb",
  "line_153_lyc0_stat_timing_b.gb",
  "line_153_lyc0_stat_timing_c.gb",
  "line_153_lyc0_stat_timing_d.gb",
  "line_153_lyc0_stat_timing_e.gb",
  "line_153_lyc0_stat_timing_f.gb",
  "line_153_lyc0_stat_timing_g.gb",
  "line_153_lyc0_stat_timing_h.gb",
  "line_153_lyc0_stat_timing_i.gb",
  "line_153_lyc0_stat_timing_j.gb",
  "line_153_lyc0_stat_timing_k.gb",
  "line_153_lyc0_stat_timing_l.gb",
  "line_153_lyc0_stat_timing_m.gb",
  "line_153_lyc0_stat_timing_n.gb",

  "line_153_lyc153_stat_timing_a.gb",
  "line_153_lyc153_stat_timing_b.gb",
  "line_153_lyc153_stat_timing_c.gb",
  "line_153_lyc153_stat_timing_d.gb",
  "line_153_lyc153_stat_timing_e.gb",
  "line_153_lyc153_stat_timing_f.gb",

  "-----",
  "lyc1_int_halt_a.gb",
  "lyc1_int_halt_b.gb",
  "lyc1_int_if_edge_a.gb",
  "lyc1_int_if_edge_b.gb",
  "lyc1_int_if_edge_c.gb",
  "lyc1_int_if_edge_d.gb",

  "lyc1_write_timing_a.gb",
  "lyc1_write_timing_b.gb",
  "lyc1_write_timing_c.gb",
  "lyc1_write_timing_d.gb",

  "-----",
  "stat_write_glitch_l0_a.gb",
  "stat_write_glitch_l0_b.gb",
  "stat_write_glitch_l0_c.gb",
  "stat_write_glitch_l143_a.gb",
  "stat_write_glitch_l143_b.gb",
  "stat_write_glitch_l143_c.gb",
  "stat_write_glitch_l143_d.gb",
  "stat_write_glitch_l1_a.gb",
  "stat_write_glitch_l1_b.gb",
  "stat_write_glitch_l1_c.gb",
  "stat_write_glitch_l1_d.gb",
};

void run_microtest(int model, const char* filename) {
  FILE* rom_file = NULL;
  rom_file = fopen(filename, "rb");
  fseek(rom_file, 0, SEEK_END);
  size_t rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  rom_size = fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);

  Gameboy gameboy;
  gameboy.reset(model, rom_size, 0x100);

  uint8_t result = 0;
  int i = 0;
  const int ticks = 100000;  // bits_ram_en needs lots of tcycles
  for (; i < ticks; i++) {
    gameboy.tick();
    gameboy.tock();

    result = gameboy.vram.ram[0];
    if (result) break;
  }

  if (i == ticks) {
    printf("? TIMEOUT @ %d\n", i);
  }
  else if (result == 0x55) {
    printf("  0x%02x PASS @ %d\n", result, i);
  }
  else {
    printf("X 0x%02x FAIL @ %d\n", result, i);
  }
}

void run_microtests() {
  double freq = (double)SDL_GetPerformanceFrequency();
  double begin = (double)SDL_GetPerformanceCounter();

  int model = MODEL_DMG;
  std::string model_string = (model == MODEL_DMG ? "dmg" : "ags");
  std::string prefix = "microtests/build/" + model_string + "/";

  printf("---------- Microtests in %s: ----------\n", prefix.c_str());

  for (auto name : micro_tests) {
    if (name[0] == '-') {
      printf("%s\n", name.c_str());
      continue;
    }
    printf("%-50s ", name.c_str());
    run_microtest(model, (prefix + name).c_str());
  }

  double end = (double)SDL_GetPerformanceCounter();
  printf("---------- Microtests took %f seconds ----------\n", (end - begin) / freq);
}
