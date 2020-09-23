#include "CoreLib/File.h"
#include "CoreLib/Tests.h"
#include "CoreLib/Types.h"
#include "MetroBoyLib/MetroBoy.h"

#include <stdio.h>
#include <string>

void run_wpol_acceptance();
void run_mooneye_acceptance();
void run_screenshot_tests();
void run_mealybug_tests();
int  run_microtests();
int  run_microtest(std::string name);
void test_codegen();

//------------------------------------------------------------------------------

#if 0
void MetroBoyApp::post() {
  double begin = timestamp();

  printf("\n");
  printf("---------- POST begin ----------\n");

  int err = 0;

  printf("Reset\n");
  err += run_microtest("micro_cpu/build/dmg/rst_0x00.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x08.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x10.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x18.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x20.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x28.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x30.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x38.gb");
  printf("\n");
  if (err) exit(1);

  double end = timestamp();
  printf("---------- POST done in %f seconds ----------\n", (end - begin));
  printf("\n");
}

  //load("roms/gb-test-roms/cpu_instrs/cpu_instrs");
  //load("roms/gb-test-roms/instr_timing/instr_timing");
  //load("roms/gb-test-roms/cpu_instrs/individual", "08-misc instrs");
  //load("roms/gb-test-roms/cpu_instrs/individual", "11-op a,(hl)");

  //load("roms/tetris"); // tetris brokennnn

  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma/basic");
  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma/reg_read");
  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma/sources-dmgABCmgbS");
  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma_restart");
  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma_start");
  /*pass*/ //load("roms/mooneye-gb/tests/build/acceptance", "oam_dma_timing");

  //load("micro_cpu/build/dmg", "cpu_zeropage");

  //load("microtests/build/dmg", "dma_basic");
  //load("microtests/build/dmg", "dma_0x8000");
  //load("microtests/build/dmg", "dma_0xA000");
  //load("microtests/build/dmg", "dma_0xE000");
  //load("microtests/build/dmg", "dma_0xFF00");

  //load("microtests/build/dmg", "poweron_006_oam");

#endif

//------------------------------------------------------------------------------

int run_microtest(std::string filename) {
  std::string path = "microtests/build/dmg/" + filename;

  blob rom = load_blob(path);
  MetroBoy gb(0x0100, rom.data(), rom.size());

  int timeout = 500; // All our "fast" microtests take under 500 cycles
  int mcycle = 0;
  for (; mcycle < timeout; mcycle++) {
    gb.mcycle();
    if (gb.vram.ram[0]) break;
  }

  uint8_t result_a = gb.zram.ram[0]; // actual
  uint8_t result_b = gb.zram.ram[1]; // expected
  uint8_t result_c = gb.zram.ram[2]; // sanity (should be 0x31)

  if (mcycle == timeout) {
    LOG_B("%-30s ", filename.c_str());
    LOG_Y("TIMEOUT\n");
    return 1;
  }
  else if (result_c != 0x31) {
    LOG_B("%-30s ", filename.c_str());
    LOG_Y("0x%02x 0x%02x 0x%02x ERROR @ %d\n", result_a, result_b, result_c, mcycle);
    return 1;
  }
  else if (result_a == result_b) {
    LOG_B("%-30s ", filename.c_str());
    LOG_G("0x%02x 0x%02x 0x%02x PASS @ %d\n", result_a, result_b, result_c, mcycle);
    return 0;
  }
  else {
    LOG_B("%-30s ", filename.c_str());
    LOG_R("0x%02x 0x%02x 0x%02x FAIL @ %d\n", result_a, result_b, result_c, mcycle);
    return 1;
  }
}

//------------------------------------------------------------------------------

int run_microtests() {
  TEST_START();

  double begin = timestamp();

  LOG_Y("Timer microtests\n");
  err += run_microtest("div_inc_timing_a.gb");
  err += run_microtest("div_inc_timing_b.gb");
  err += run_microtest("poweron_000_div.gb");
  err += run_microtest("poweron_004_div.gb");
  err += run_microtest("poweron_005_div.gb");
  err += run_microtest("timer_div_phase_c.gb");
  err += run_microtest("timer_div_phase_d.gb");

  double end = timestamp();
  LOG_Y("---------- Microtests took %f seconds, %d failures ----------\n", (end - begin), err);

  TEST_END();
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  run_microtests();
  //run_screenshot_tests();
  //run_mooneye_acceptance();
  //run_wpol_acceptance();
  //run_mealybug_tests();
  //return 0;

  //---------


  return 0;
}

//------------------------------------------------------------------------------
