#include "Gameboy.h"

#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>

static const char* micro_tests[] = {
  "-----",
  "microtests/build/oam_int_inc_sled.gb",
  "microtests/build/oam_int_halt_a.gb",
  "microtests/build/oam_int_halt_b.gb",
  "microtests/build/oam_int_nops_a.gb",
  "microtests/build/oam_int_nops_b.gb",
  "microtests/build/oam_int_if_a.gb",
  "microtests/build/oam_int_if_b.gb",
  "microtests/build/oam_int_if_c.gb",
  "microtests/build/oam_int_if_d.gb",

  "-----",
  "microtests/build/hblank_int_inc_sled.gb",
  "microtests/build/hblank_int_halt_a.gb",
  "microtests/build/hblank_int_halt_b.gb",
  "microtests/build/hblank_int_nops_a.gb",
  "microtests/build/hblank_int_nops_b.gb",
  "microtests/build/hblank_int_if_a.gb",
  "microtests/build/hblank_int_if_b.gb",
  "microtests/build/hblank_int_if_c.gb",
  "microtests/build/hblank_int_if_d.gb",

  "-----",
  "microtests/build/vblank_int_inc_sled.gb",
  "microtests/build/vblank_int_halt_a.gb",
  "microtests/build/vblank_int_halt_b.gb",
  "microtests/build/vblank_int_nops_a.gb",
  "microtests/build/vblank_int_nops_b.gb",
  "microtests/build/vblank_int_if_a.gb",
  "microtests/build/vblank_int_if_b.gb",
  "microtests/build/vblank_int_if_c.gb",
  "microtests/build/vblank_int_if_d.gb",
  "microtests/build/vblank_int_if_e.gb",
  "microtests/build/vblank_int_if_f.gb",

  "-----",
  "microtests/build/vblank2_int_inc_sled.gb",
  "microtests/build/vblank2_int_halt_a.gb",
  "microtests/build/vblank2_int_halt_b.gb",
  "microtests/build/vblank2_int_nops_a.gb",
  "microtests/build/vblank2_int_nops_b.gb",
  "microtests/build/vblank2_int_if_a.gb",
  "microtests/build/vblank2_int_if_b.gb",
  "microtests/build/vblank2_int_if_c.gb",
  "microtests/build/vblank2_int_if_d.gb",

  "-----",

  //"microtests/build/is_if_set_during_ime0.gb",
  "microtests/build/enable_oam_int_during_oam_a.gb",
  "microtests/build/enable_oam_int_during_oam_b.gb",
  "microtests/build/enable_oam_int_during_oam_c.gb",
  "microtests/build/enable_oam_int_during_oam_d.gb",

  //"microtests/build/lcdon_halt_to_vblank_int_a.gb",
  //"microtests/build/lcdon_halt_to_vblank_int_b.gb",
  //"microtests/build/lcdon_nops_to_vblank_int_a.gb",
  //"microtests/build/lcdon_nops_to_vblank_int_b.gb",
  
  "-----",
  "microtests/build/100_int_timing_1a.gb",
  "microtests/build/100_int_timing_1b.gb", // broken, timer update timing off?
  "-----",
  "microtests/build/100_int_timing_2a.gb",
  "microtests/build/100_int_timing_2b.gb",
  "-----",

  "microtests/build/100_int_timing_3a.gb",
  "-----",

  "microtests/build/100_int_timing_4a.gb",
  "microtests/build/100_int_timing_4b.gb",

  "-----",
  "microtests/build/lcdon_to_hblank_int_l0.gb",
  "microtests/build/lcdon_to_hblank_int_l1.gb",
  "microtests/build/lcdon_to_hblank_int_l2.gb",
  "microtests/build/lcdon_to_hblank_int_scx0.gb",
  "microtests/build/lcdon_to_hblank_int_scx1.gb",
  "microtests/build/lcdon_to_hblank_int_scx2.gb",
  "microtests/build/lcdon_to_hblank_int_scx3.gb",
  "microtests/build/lcdon_to_hblank_int_scx4.gb",
  "microtests/build/lcdon_to_hblank_int_scx5.gb",
  "microtests/build/lcdon_to_hblank_int_scx6.gb",
  "microtests/build/lcdon_to_hblank_int_scx7.gb",

  "microtests/build/lcdon_to_hblank_di_timing_a.gb",
  "microtests/build/lcdon_to_hblank_di_timing_b.gb",

  "-----",
  "microtests/build/lcdon_to_ly1_a.gb",
  "microtests/build/lcdon_to_ly1_b.gb",
  "microtests/build/lcdon_to_ly2_a.gb",
  "microtests/build/lcdon_to_ly2_b.gb",
  "microtests/build/lcdon_to_ly3_a.gb",
  "microtests/build/lcdon_to_ly3_b.gb",
  "microtests/build/lcdon_to_lyc1_int.gb",
  "microtests/build/lcdon_to_lyc2_int.gb",
  "microtests/build/lcdon_to_lyc3_int.gb",

  "-----",
  "microtests/build/lcdon_to_oam_int_l0.gb",
  "microtests/build/lcdon_to_oam_int_l1.gb",
  "microtests/build/lcdon_to_oam_int_l2.gb",

  "-----",
  "microtests/build/lcdon_to_stat0_a.gb",
  "microtests/build/lcdon_to_stat0_b.gb",
  "microtests/build/lcdon_to_stat0_c.gb",
  "microtests/build/lcdon_to_stat0_d.gb",
  "microtests/build/lcdon_to_stat2_a.gb",
  "microtests/build/lcdon_to_stat2_b.gb",
  "microtests/build/lcdon_to_stat2_c.gb",
  "microtests/build/lcdon_to_stat2_d.gb",
  "microtests/build/lcdon_to_stat3_a.gb",
  "microtests/build/lcdon_to_stat3_b.gb",
  "microtests/build/lcdon_to_stat3_c.gb",
  "microtests/build/lcdon_to_stat3_d.gb",

  "microtests/build/lcdon_to_vblank_int.gb",
  "microtests/build/lcdon_to_stat_int_vblank.gb",
};

extern uint8_t rom_buf[1024 * 1024];

void run_microtest(const char* filename) {
  if (filename[0] == '-') {
    printf("%s\n", filename);
    return;
  }
  printf("%-50s ", filename);

  FILE* rom_file = NULL;
  fopen_s(&rom_file, filename, "rb");
  fseek(rom_file, 0, SEEK_END);
  int rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);

  Gameboy gameboy;
  gameboy.reset(rom_size, 0x100);

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
  for (auto name : micro_tests) run_microtest(name);
}
