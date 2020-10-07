#include "MetroBoyLib/MetroBoy.h"
#include "CoreLib/File.h"

//---------
// mooneye generic

static const std::string generic_tests[] = {

  "boot_div-dmgABCmgb.gb",
  "boot_hwio-dmgABCmgb.gb",
  "boot_regs-dmgABC.gb",

  "add_sp_e_timing.gb",
  "call_cc_timing.gb",
  "call_cc_timing2.gb",
  "call_timing.gb",
  "call_timing2.gb",
  "di_timing-GS.gb",             // hangs
  "div_timing.gb",
  "ei_sequence.gb",
  "ei_timing.gb",
  "halt_ime0_ei.gb",             // hangs
  "halt_ime0_nointr_timing.gb",  // hangs
  "halt_ime1_timing.gb",
  "halt_ime1_timing2-GS.gb",     // hangs
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

  "instr/daa.gb",

  "interrupts/ie_push.gb",

  "oam_dma/basic.gb",
  "oam_dma/reg_read.gb",
  "oam_dma/sources-dmgABCmgbS.gb",
};

//---------
// mooneye ppu

static const std::string ppu_tests[] = {
  "hblank_ly_scx_timing-GS.gb",
  "intr_1_2_timing-GS.gb",
  "intr_2_0_timing.gb",
  "intr_2_mode0_timing.gb",
  "intr_2_mode0_timing_sprites.gb",
  "intr_2_mode3_timing.gb",
  "intr_2_oam_ok_timing.gb",
  "lcdon_timing-dmgABCmgbS.gb",
  "lcdon_write_timing-GS.gb",
  "stat_irq_blocking.gb",
  "stat_lyc_onoff.gb",
  "vblank_stat_intr-GS.gb",
};

//---------
// mooneye timer

static const std::string timer_tests[] = {
  // this one is a tiny bit slow
  "div_write.gb",
  "rapid_toggle.gb",
  "tim00.gb",
  "tim00_div_trigger.gb",
  "tim01.gb",
  "tim01_div_trigger.gb",
  "tim10.gb",
  "tim10_div_trigger.gb",
  "tim11.gb",
  "tim11_div_trigger.gb",
  "tima_reload.gb",
  "tima_write_reloading.gb",
  "tma_write_reloading.gb",
};

//---------
// mooneye mbc

static const std::string mbc1_tests[] = {
  "bits_ram_en.gb",
  "ram_256Kb.gb",
  "ram_64Kb.gb",
  "rom_16Mb.gb",
  "rom_1Mb.gb",
  "rom_2Mb.gb",
  "rom_4Mb.gb",
  "rom_512Kb.gb",
  "rom_8Mb.gb",

  // not going to bother with multicart support for now
  //"multicart_rom_8Mb.gb",
};

//---------
// mooneye misc

static const char* misc_tests[] = {
  "bits/unused_hwio-C.gb",
  "ppu/vblank_stat_intr-C.gb",
  "boot_div-A.gb",
  "boot_div-cgb0.gb",
  "boot_div-cgbABCDE.gb",
  "boot_hwio-C.gb",
  "boot_regs-A.gb",
  "boot_regs-cgb.gb",

  //"boot_div-dmg0.gb",
  //"boot_div-S.gb",
  //"boot_div2-S.gb",
  //"boot_hwio-dmg0.gb",
  //"boot_hwio-S.gb",
  //"boot_regs-dmg0.gb",
  //"boot_regs-mgb.gb",
  //"boot_regs-sgb.gb",
  //"boot_regs-sgb2.gb",
};

//-----------------------------------------------------------------------------

void run_mooneye_test(const std::string& prefix, const std::string& name) {
  std::string filename = prefix + name;
  blob rom;
  load_array(filename, rom);

  MetroBoy gameboy;
  gameboy.reset_cart(rom.data(), rom.size());

  uint8_t result = 0xFF;
  int phase = 0;
  int timeout = 1000000 * 8;

  // bits_ram_en needs lots of phases
  if (name == "bits_ram_en.gb") timeout = 25000000 * 8;

  for (; phase < timeout; phase++) {
    gameboy.next_phase();
    if (gameboy.z80.op == 0x40) {
      //printf("\ntest %s end @ %d\n", name.c_str(), i);
      result = gameboy.z80.a;
      break;
    }
  }

  if (phase == timeout) {
    printf("\n");
    printf("%-50s ", name.c_str());
    printf("? TIMEOUT @ %d\n", phase);
  }
  else if (result == 0x00) {
    printf(".");
  }
  else {
    printf("\n");
    printf("%-50s ", name.c_str());
    printf("X 0x%02x FAIL @ %d\n", result, phase);
  }
}

void run_mooneye_acceptance() {
  double begin = timestamp();

  printf("\n");
  printf("---------- Mooneye tests =====\001\n");

  printf("Generic");
  for (auto name : generic_tests) {
    std::string prefix = "roms/mooneye-gb/tests/build/acceptance/";
    run_mooneye_test(prefix, name);
  }
  printf("\n");

  printf("Timer");
  for (auto name : timer_tests) {
    std::string prefix = "roms/mooneye-gb/tests/build/acceptance/timer/";
    run_mooneye_test(prefix, name);
  }
  printf("\n");

  printf("MBC1");
  for (auto name : mbc1_tests) {
    std::string prefix = "roms/mooneye-gb/tests/build/emulator-only/mbc1/";
    run_mooneye_test(prefix, name);
  }
  printf("\n");

  /*
  printf("-----PPU tests-----\n");
  for (auto name : ppu_tests) {
    std::string prefix = "roms/mooneye-gb/tests/build/acceptance/ppu/";
    run_mooneye_test(prefix, name);
  }
  printf("\n");
  */

  double end = timestamp();
  printf("---------- Mooneye took %f seconds ----------\n", (end - begin));
}
