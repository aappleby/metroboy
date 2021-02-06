#include "MetroBoyLib/MetroBoy.h"
#include "CoreLib/File.h"

//---------
// mooneye generic

static const std::string mooneye_generic_tests[] = {
  "boot_div-dmgABCmgb.gb",         // gateboy ok
  "boot_hwio-dmgABCmgb.gb",        // XXX gateboy fail at ff10
  "boot_regs-dmgABC.gb",           // gateboy ok
  "add_sp_e_timing.gb",            // gateboy ok
  "call_cc_timing.gb",             // gateboy ok
  "call_cc_timing2.gb",            // gateboy ok
  "call_timing.gb",                // gateboy ok
  "call_timing2.gb",               // gateboy ok
  "di_timing-GS.gb",               // gateboy ok
  "div_timing.gb",                 // gateboy ok
  "ei_sequence.gb",                // XXX gateboy fail
  "ei_timing.gb",                  // XXX gateboy fail
  "halt_ime0_ei.gb",               // gateboy pass
  "halt_ime0_nointr_timing.gb",    // XXX gateboy hang
  "halt_ime1_timing.gb",           // gateboy pass
  "halt_ime1_timing2-GS.gb",       // gateboy pass
  "if_ie_registers.gb",            // gateboy pass
  "intr_timing.gb",                // gateboy pass
  "jp_cc_timing.gb",               // gateboy pass
  "jp_timing.gb",                  // gateboy pass
  "ld_hl_sp_e_timing.gb",          // gateboy pass
  "oam_dma_restart.gb",            // gateboy pass
  "oam_dma_start.gb",              // gateboy pass
  "oam_dma_timing.gb",             // gateboy pass
  "pop_timing.gb",                 // gateboy pass
  "push_timing.gb",                // gateboy pass
  "rapid_di_ei.gb",                // XXX gateboy FAIL
  "ret_cc_timing.gb",              // gateboy pass
  "ret_timing.gb",                 // gateboy pass
  "reti_intr_timing.gb",           // XXX gateboy FAIL
  "reti_timing.gb",                // gateboy pass
  "rst_timing.gb",                 // gateboy pass
  "bits/mem_oam.gb",               // gateboy pass
  "bits/reg_f.gb",                 // gateboy pass
  "bits/unused_hwio-GS.gb",        // XXX gateboy FAIL FFFF
  "instr/daa.gb",                  // gateboy pass
  "interrupts/ie_push.gb",         // XXX gateboy FAIL R3 - unwanted cancel
  "oam_dma/basic.gb",              // gateboy pass
  "oam_dma/reg_read.gb",           // XXX gateboy hang? wtf?
  "oam_dma/sources-dmgABCmgbS.gb", // XXX gateboy hang? wtf?
};

//---------
// mooneye ppu

static const std::string mooneye_ppu_tests[] = {
  "hblank_ly_scx_timing-GS.gb",      // gateboy pass
  "intr_1_2_timing-GS.gb",           // gateboy pass
  "intr_2_0_timing.gb",              // gateboy pass
  "intr_2_mode0_timing.gb",          // gateboy pass
  "intr_2_mode0_timing_sprites.gb",  // XXX gateboy FAIL test 00 fail
  "intr_2_mode3_timing.gb",          // gateboy pass
  "intr_2_oam_ok_timing.gb",         // gateboy pass
  "lcdon_timing-dmgABCmgbS.gb",      // XXX gateboy FAIL stat lyc
  "lcdon_write_timing-GS.gb",        // XXX gateboy FAIL
  "stat_irq_blocking.gb",            // gateboy pass
  "stat_lyc_onoff.gb",               // gateboy pass
  "vblank_stat_intr-GS.gb",          // gateboy pass
};

//---------
// mooneye timer

static const std::string mooneye_timer_tests[] = {
  // this one is a tiny bit slow
  "div_write.gb",               // gateboy pass
  "rapid_toggle.gb",            // gateboy pass
  "tim00.gb",                   // gateboy pass
  "tim00_div_trigger.gb",       // gateboy pass
  "tim01.gb",                   // gateboy pass
  "tim01_div_trigger.gb",       // gateboy pass
  "tim10.gb",                   // gateboy pass
  "tim10_div_trigger.gb",       // gateboy pass
  "tim11.gb",                   // gateboy pass
  "tim11_div_trigger.gb",       // gateboy pass
  "tima_reload.gb",             // gateboy pass
  "tima_write_reloading.gb",    // gateboy pass
  "tma_write_reloading.gb",     // gateboy pass
};

//---------
// mooneye mbc

static const std::string mooneye_mbc1_tests[] = {
  "bits_ram_en.gb", // XXX gateboy fail ram not disabled
  "ram_256Kb.gb",   // XXX gateboy fail round 1
  "ram_64Kb.gb",    // XXX gateboy fail round 1
  "rom_16Mb.gb",    // XXX gateboy fail
  "rom_1Mb.gb",
  "rom_2Mb.gb",
  "rom_4Mb.gb",
  "rom_512Kb.gb",
  "rom_8Mb.gb",

  // not going to bother with multicart support for now
  //"multicart_rom_8Mb.gb",
};

//-----------------------------------------------------------------------------

void run_mooneye_test(const std::string& prefix, const std::string& name) {
  std::string filename = prefix + name;
  blob rom;
  load_array(filename, rom);

  MetroBoy gameboy;
  gameboy.reset_to_cart(rom.data(), rom.size());

  uint8_t result = 0xFF;
  int phase = 0;
  int timeout = 1000000 * 8;

  // bits_ram_en needs lots of phases
  if (name == "bits_ram_en.gb") timeout = 25000000 * 8;

  for (; phase < timeout; phase++) {
    gameboy.next_phase();
    if (gameboy.cpu.op == 0x40) {
      //printf("\ntest %s end @ %d\n", name.c_str(), i);
      result = gameboy.cpu.a;
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
  for (auto name : mooneye_generic_tests) {
    std::string prefix = "roms/mooneye-gb/tests/build/acceptance/";
    run_mooneye_test(prefix, name);
  }
  printf("\n");

  printf("Timer");
  for (auto name : mooneye_timer_tests) {
    std::string prefix = "roms/mooneye-gb/tests/build/acceptance/timer/";
    run_mooneye_test(prefix, name);
  }
  printf("\n");

  printf("MBC1");
  for (auto name : mooneye_mbc1_tests) {
    std::string prefix = "roms/mooneye-gb/tests/build/emulator-only/mbc1/";
    run_mooneye_test(prefix, name);
  }
  printf("\n");

  /*
  printf("-----PPU tests-----\n");
  for (auto name : mooneye_ppu_tests) {
    std::string prefix = "roms/mooneye-gb/tests/build/acceptance/ppu/";
    run_mooneye_test(prefix, name);
  }
  printf("\n");
  */

  double end = timestamp();
  printf("---------- Mooneye took %f seconds ----------\n", (end - begin));
}
