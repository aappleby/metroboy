#include "Platform.h"
#include "test_mooneye.h"

#include "Gameboy.h"

//---------
// mooneye generic

static const char* generic_tests[] = {
  "mooneye-gb/tests/build/acceptance/add_sp_e_timing.gb",
  //"mooneye-gb/tests/build/acceptance/boot_div-dmg0.gb",
  //"mooneye-gb/tests/build/acceptance/boot_div-dmgABCmgb.gb",
  //"mooneye-gb/tests/build/acceptance/boot_div-S.gb",
  //"mooneye-gb/tests/build/acceptance/boot_div2-S.gb",
  //"mooneye-gb/tests/build/acceptance/boot_hwio-dmg0.gb",
  //"mooneye-gb/tests/build/acceptance/boot_hwio-dmgABCmgb.gb",
  //"mooneye-gb/tests/build/acceptance/boot_hwio-S.gb",
  //"mooneye-gb/tests/build/acceptance/boot_regs-dmg0.gb",
  //"mooneye-gb/tests/build/acceptance/boot_regs-dmgABC.gb",
  //"mooneye-gb/tests/build/acceptance/boot_regs-mgb.gb",
  //"mooneye-gb/tests/build/acceptance/boot_regs-sgb.gb",
  //"mooneye-gb/tests/build/acceptance/boot_regs-sgb2.gb",
  "mooneye-gb/tests/build/acceptance/call_cc_timing.gb",
  "mooneye-gb/tests/build/acceptance/call_cc_timing2.gb",
  "mooneye-gb/tests/build/acceptance/call_timing.gb",
  "mooneye-gb/tests/build/acceptance/call_timing2.gb",
  //"mooneye-gb/tests/build/acceptance/di_timing-GS.gb",   // ?
  "mooneye-gb/tests/build/acceptance/div_timing.gb",
  "mooneye-gb/tests/build/acceptance/ei_sequence.gb",
  "mooneye-gb/tests/build/acceptance/ei_timing.gb",
  "mooneye-gb/tests/build/acceptance/halt_ime0_ei.gb",
  "mooneye-gb/tests/build/acceptance/halt_ime0_nointr_timing.gb",
  "mooneye-gb/tests/build/acceptance/halt_ime1_timing.gb",
  //"mooneye-gb/tests/build/acceptance/halt_ime1_timing2-GS.gb",
  "mooneye-gb/tests/build/acceptance/if_ie_registers.gb",
  "mooneye-gb/tests/build/acceptance/intr_timing.gb",
  "mooneye-gb/tests/build/acceptance/jp_cc_timing.gb",
  "mooneye-gb/tests/build/acceptance/jp_timing.gb",
  "mooneye-gb/tests/build/acceptance/ld_hl_sp_e_timing.gb",
  "mooneye-gb/tests/build/acceptance/oam_dma_restart.gb",
  "mooneye-gb/tests/build/acceptance/oam_dma_start.gb",
  "mooneye-gb/tests/build/acceptance/oam_dma_timing.gb",
  "mooneye-gb/tests/build/acceptance/pop_timing.gb",
  "mooneye-gb/tests/build/acceptance/push_timing.gb",
  "mooneye-gb/tests/build/acceptance/rapid_di_ei.gb",
  "mooneye-gb/tests/build/acceptance/ret_cc_timing.gb",
  "mooneye-gb/tests/build/acceptance/ret_timing.gb",
  "mooneye-gb/tests/build/acceptance/reti_intr_timing.gb",
  "mooneye-gb/tests/build/acceptance/reti_timing.gb",
  "mooneye-gb/tests/build/acceptance/rst_timing.gb",
 
  "mooneye-gb/tests/build/acceptance/bits/mem_oam.gb",
  "mooneye-gb/tests/build/acceptance/bits/reg_f.gb",
  //"mooneye-gb/tests/build/acceptance/bits/unused_hwio-GS.gb",

  "mooneye-gb/tests/build/acceptance/instr/daa.gb",

  "mooneye-gb/tests/build/acceptance/interrupts/ie_push.gb",

  "mooneye-gb/tests/build/acceptance/oam_dma/basic.gb",
  "mooneye-gb/tests/build/acceptance/oam_dma/reg_read.gb",
  //"mooneye-gb/tests/build/acceptance/oam_dma/sources-dmgABCmgbS.gb",
};

//---------
// mooneye ppu

static const char* ppu_tests[] = {
  //"mooneye-gb/tests/build/acceptance/ppu/hblank_ly_scx_timing-GS.gb",
  //"mooneye-gb/tests/build/acceptance/ppu/intr_1_2_timing-GS.gb",
  "mooneye-gb/tests/build/acceptance/ppu/intr_2_0_timing.gb",
  "mooneye-gb/tests/build/acceptance/ppu/intr_2_mode0_timing.gb",
  "mooneye-gb/tests/build/acceptance/ppu/intr_2_mode0_timing_sprites.gb",
  "mooneye-gb/tests/build/acceptance/ppu/intr_2_mode3_timing.gb",
  "mooneye-gb/tests/build/acceptance/ppu/intr_2_oam_ok_timing.gb",
  //"mooneye-gb/tests/build/acceptance/ppu/lcdon_timing-dmgABCmgbS.gb",
  //"mooneye-gb/tests/build/acceptance/ppu/lcdon_write_timing-GS.gb",
  "mooneye-gb/tests/build/acceptance/ppu/stat_irq_blocking.gb",
  "mooneye-gb/tests/build/acceptance/ppu/stat_lyc_onoff.gb",
  //"mooneye-gb/tests/build/acceptance/ppu/vblank_stat_intr-GS.gb",

  //"mooneye-gb/tests/build/misc/ppu/vblank_stat_intr-C.gb",  // ?
};

//---------
// mooneye timer

static const char* timer_tests[] = {
  // this one is a tiny bit slow
  "mooneye-gb/tests/build/acceptance/timer/div_write.gb",
  "mooneye-gb/tests/build/acceptance/timer/rapid_toggle.gb",
  "mooneye-gb/tests/build/acceptance/timer/tim00.gb",
  "mooneye-gb/tests/build/acceptance/timer/tim00_div_trigger.gb",
  "mooneye-gb/tests/build/acceptance/timer/tim01.gb",
  "mooneye-gb/tests/build/acceptance/timer/tim01_div_trigger.gb",
  "mooneye-gb/tests/build/acceptance/timer/tim10.gb",
  "mooneye-gb/tests/build/acceptance/timer/tim10_div_trigger.gb",
  "mooneye-gb/tests/build/acceptance/timer/tim11.gb",
  "mooneye-gb/tests/build/acceptance/timer/tim11_div_trigger.gb",
  "mooneye-gb/tests/build/acceptance/timer/tima_reload.gb",
  "mooneye-gb/tests/build/acceptance/timer/tima_write_reloading.gb",
  "mooneye-gb/tests/build/acceptance/timer/tma_write_reloading.gb",
};

//---------
// mooneye mbc

static const char* mbc1_tests[] = {
  "mooneye-gb_hwtests/emulator-only/mbc1/bits_ram_en.gb",
  "mooneye-gb_hwtests/emulator-only/mbc1/ram_256Kb.gb",
  "mooneye-gb_hwtests/emulator-only/mbc1/ram_64Kb.gb",
  "mooneye-gb_hwtests/emulator-only/mbc1/rom_16Mb.gb",
  "mooneye-gb_hwtests/emulator-only/mbc1/rom_1Mb.gb",
  "mooneye-gb_hwtests/emulator-only/mbc1/rom_2Mb.gb",
  "mooneye-gb_hwtests/emulator-only/mbc1/rom_4Mb.gb",
  "mooneye-gb_hwtests/emulator-only/mbc1/rom_512Kb.gb",
  "mooneye-gb_hwtests/emulator-only/mbc1/rom_8Mb.gb",

  // not going to bother with multicart support for now
  //"mooneye-gb_hwtests/emulator-only/mbc1/multicart_rom_8Mb.gb",
};

//---------
// mooneye misc

/*
static const char* misc_tests[] = {
  //"mooneye-gb_hwtests/misc/bits/unused_hwio-C.gb", 
  //"mooneye-gb_hwtests/misc/ppu/vblank_stat_intr-C.gb",
  //"mooneye-gb_hwtests/misc/boot_div-A.gb",
  //"mooneye-gb_hwtests/misc/boot_div-cgb0.gb",
  //"mooneye-gb_hwtests/misc/boot_div-cgbABCDE.gb",
  //"mooneye-gb_hwtests/misc/boot_hwio-C.gb",
  //"mooneye-gb_hwtests/misc/boot_regs-A.gb",
  //"mooneye-gb_hwtests/misc/boot_regs-cgb.gb",
};
*/

//-----------------------------------------------------------------------------

extern uint8_t rom_buf[1024 * 1024];

void run_mooneye(const char* filename) {
  printf("%-70s ", filename);

  FILE* rom_file = NULL;
  fopen_s(&rom_file, filename, "rb");
  fseek(rom_file, 0, SEEK_END);
  int rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);


  Gameboy gameboy;
  gameboy.reset(rom_size, 0x100);

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

void run_mooneye_generic() {
  printf("-----Generic tests-----\n");
  for (auto name : generic_tests) run_mooneye(name);
  printf("\n");
}

void run_mooneye_timer() {
  printf("-----Timer tests-----\n");
  for (auto name : timer_tests) run_mooneye(name);
  printf("\n");
}

void run_mooneye_mbc1() {
  printf("-----MBC1 tests-----\n");
  for (auto name : mbc1_tests) run_mooneye(name);
  printf("\n");
}

void run_mooneye_ppu() {
  printf("-----PPU tests-----\n");
  for (auto name : ppu_tests) run_mooneye(name);
  printf("\n");
}

void run_mooneye_acceptance() {
  run_mooneye_generic();
  run_mooneye_timer();
  //run_mooneye_mbc1();
  run_mooneye_ppu();
}
