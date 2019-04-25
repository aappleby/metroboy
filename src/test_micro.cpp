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
  "100_int_timing_1a.gb",
  "100_int_timing_1b.gb",
  "-----",
  "100_int_timing_2a.gb",
  "100_int_timing_2b.gb",
  "-----",

  "100_int_timing_3a.gb",
  "-----",

  "100_int_timing_4a.gb",
  "100_int_timing_4b.gb",

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
};

void run_microtest(const char* filename) {
  FILE* rom_file = NULL;
  rom_file = fopen(filename, "rb");
  fseek(rom_file, 0, SEEK_END);
  size_t rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  rom_size = fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);

  Gameboy gameboy;
  gameboy.reset(MODEL_DMG, rom_size, 0x100);

  uint8_t result = 0;
  int i = 0;
  const int ticks = 25000000;  // bits_ram_en needs lots of tcycles
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
  std::string prefix = "microtests/build/dmg/";

  printf("Microtests in %s:\n", prefix.c_str());

  for (auto name : micro_tests) {
    if (name[0] == '-') {
      printf("%s\n", name.c_str());
      continue;
    }
    printf("%-30s ", name.c_str());
    run_microtest((prefix + name).c_str());
  }
}
