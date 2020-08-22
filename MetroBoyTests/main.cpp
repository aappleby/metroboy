#include <stdio.h>

#include "CoreLib/Types.h"

#include "MetroBoyTests/test_codegen.h"
#include "MetroBoyTests/test_micro.h"
#include "MetroBoyTests/test_mooneye.h"
#include "MetroBoyTests/test_wpol.h"
#include "MetroBoyTests/test_screenshot.h"

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  printf("Hello World %f\n", timestamp());

  //run_microtests();
  //run_screenshot_tests();
  //run_mooneye_acceptance();
  //run_wpol_acceptance();
  //run_mealybug_tests();
  //return 0;

  //---------

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

  //runmode = STEP_PHASE;
  //runmode = RUN_FAST;
  //runmode = RUN_VSYNC;

  return 0;
}