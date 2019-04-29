#include "Platform.h"
#include "test_wpol.h"

#include "Gameboy.h"

//---------
// wpol generic

static const std::string generic_tests[] = {
  "add_sp_e_timing.gb",
  "boot_hwio-G.gb",    // X
  "boot_regs-dmg.gb",
  "call_cc_timing.gb",
  "call_cc_timing2.gb",
  "call_timing.gb",
  "call_timing2.gb",
  "di_timing-GS.gb",
  "div_timing.gb",
  "ei_timing.gb",
  "halt_ime0_ei.gb",
  "halt_ime0_nointr_timing.gb",
  "halt_ime1_timing.gb",
  "halt_ime1_timing2-GS.gb",
  "if_ie_registers.gb",
  "intr_timing.gb",
  "jp_cc_timing.gb",
  "jp_timing.gb",
  "ld_hl_sp_e_timing.gb",
  "oam_dma_restart.gb",
  "oam_dma_start.gb",
  "oam_dma_timing.gb",
  "pop_timing.gb",
  "push_timing.gb",
  "rapid_di_ei.gb",
  "ret_cc_timing.gb",
  "ret_timing.gb",
  "reti_intr_timing.gb",
  "reti_timing.gb",
  "rst_timing.gb", 

  "bits/mem_oam.gb",
  "bits/reg_f.gb",
  "bits/unused_hwio-GS.gb",
};

//---------
// wpol ppu

static const std::string ppu_tests[] = {
  // dmg pass
  "hblank_ly_scx_timing-GS.gb",
  "hblank_ly_scx_timing_nops.gb",
  "hblank_ly_scx_timing_variant_nops.gb",
  "intr_0_timing.gb",
  "intr_1_2_timing-GS.gb",
  "intr_1_timing.gb",
  "intr_2_0_timing.gb",
  "intr_2_mode0_scx1_timing_nops.gb",
  "intr_2_mode0_scx2_timing_nops.gb",
  "intr_2_mode0_scx3_timing_nops.gb",
  "intr_2_mode0_scx4_timing_nops.gb",
  "intr_2_mode0_scx5_timing_nops.gb",
  "intr_2_mode0_scx6_timing_nops.gb",
  "intr_2_mode0_scx7_timing_nops.gb",
  "intr_2_mode0_scx8_timing_nops.gb",
  "intr_2_mode0_timing.gb",
  "intr_2_mode0_timing_sprites.gb",
  "intr_2_mode0_timing_sprites_nops.gb",
  "intr_2_mode0_timing_sprites_scx1_nops.gb",
  "intr_2_mode0_timing_sprites_scx2_nops.gb",
  "intr_2_mode0_timing_sprites_scx3_nops.gb",
  "intr_2_mode0_timing_sprites_scx4_nops.gb",
  "intr_2_mode3_timing.gb",
  "intr_2_oam_ok_timing.gb",
  "intr_2_timing.gb",
  "lcdon_mode_timing.gb",
  "ly00_01_mode0_2.gb",
  "ly00_mode0_2-GS.gb",
  "ly00_mode1_0-GS.gb",
  "ly00_mode2_3.gb",
  "ly00_mode3_0.gb",
  "ly143_144_145.gb",
  "ly143_144_152_153.gb",
  "ly143_144_mode0_1.gb",
  "ly143_144_mode3_0.gb",
  "ly_lyc-GS.gb",
  "ly_lyc_0-GS.gb",
  "ly_lyc_0_write-GS.gb",
  "ly_lyc_153-GS.gb",
  "ly_lyc_144-GS.gb",
  "ly_lyc_153_write-GS.gb",
  "ly_lyc_write-GS.gb",
  "ly_new_frame-GS.gb",
  "stat_irq_blocking.gb",
  "stat_write_if-GS.gb",
  "vblank_if_timing.gb",
  "vblank_stat_intr-GS.gb",

  // dmg fail
  /*
  "hblank_ly_scx_timing-C.gb",
  "ly00_mode1_2-C.gb",
  "ly_lyc-C.gb",
  "ly_lyc_0-C.gb",
  "ly_lyc_0_write-C.gb",
  "ly_lyc_144-C.gb",
  "ly_lyc_153-C.gb",
  "ly_lyc_153_write-C.gb",
  "ly_lyc_write-C.gb",
  "ly_new_frame-C.gb",
  "stat_write_if-C.gb",
  */
};

//---------
// wpol timer

static const std::string timer_tests[] = {
  "",
};

//---------
// wpol mbc

static const std::string mbc1_tests[] = {
  "",
};

//---------
// wpol misc

static const char* misc_tests[] = {
  "",
};

//-----------------------------------------------------------------------------

extern uint8_t rom_buf[1024 * 1024];

void run_wpol(const char* filename) {
  FILE* rom_file = NULL;
  rom_file = fopen(filename, "rb");
  fseek(rom_file, 0, SEEK_END);
  size_t rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  rom_size = fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);


  Gameboy gameboy;
  gameboy.reset(MODEL_DMG, rom_size, 0x100);

  bool pass = false;
  int i = 0;
  const int ticks = 25000000;  // bits_ram_en needs lots of tcycles
  for (; i < ticks; i++) {
    gameboy.tick();
    gameboy.tock();
    if (gameboy.get_op() == 0x40) {
      pass = gameboy.get_reg_a() == 0x00;
      break;
    }
  }

  if (i == ticks) {
    printf("? TIMEOUT @ %d\n", i);
  }
  else if (pass) {
    printf("  PASS @ %d\n", i);
  }
  else {
    printf("X FAIL @ %d\n", i);
  }
}

void run_wpol_generic() {
  printf("-----Generic tests-----\n");

  std::string prefix = "wpol-gb/tests/build/acceptance/";

  for (auto name : generic_tests) {
    printf("%-50s ", name.c_str());
    run_wpol((prefix + name).c_str());
  }
  printf("\n");
}

void run_wpol_timer() {
  printf("-----Timer tests-----\n");

  std::string prefix = "wpol-gb/tests/build/acceptance/timer/";

  for (auto name : timer_tests) {
    printf("%-50s ", name.c_str());
    run_wpol((prefix + name).c_str());
  }
  printf("\n");
}

void run_wpol_mbc1() {
  printf("-----MBC1 tests-----\n");

  std::string prefix = "wpol-gb/tests/build/emulator-only/mbc1/";

  for (auto name : mbc1_tests) {
    printf("%-50s ", name.c_str());
    run_wpol((prefix + name).c_str());
  }
  printf("\n");
}

void run_wpol_ppu() {
  printf("-----PPU tests-----\n");

  std::string prefix = "wpol-gb/tests/build/acceptance/gpu/";

  for (auto name : ppu_tests) {
    printf("%-50s ", name.c_str());
    run_wpol((prefix + name).c_str());
  }
  printf("\n");
}

void run_wpol_acceptance() {
  run_wpol_generic();
  //run_wpol_timer();
  //run_wpol_mbc1();
  run_wpol_ppu();
}
