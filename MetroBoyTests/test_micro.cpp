#include "MetroBoyLib/MetroBoy.h"
#include "CoreLib/File.h"
#include "CoreLib/Tests.h"

static std::string all_microtests[] = {
  "microtests/build/dmg/div_inc_timing_a.gb",
  "microtests/build/dmg/div_inc_timing_b.gb",
};

//------------------------------------------------------------------------------

int run_metroboy_microtest(std::string filename) {
  LOG_B("%-60s ", filename.c_str());

  blob rom;
  load_array(filename, rom);

  MetroBoy gb_local;
  MetroBoy* gameboy = &gb_local;

  gameboy->set_rom(rom.data(), rom.size());
  gameboy->reset(0x100);

  uint8_t result = 0xFF;
  int i = 0;
  const int mcycles = 2000;
  for (; i < mcycles; i++) {
    gameboy->mcycle();
    result = gameboy->get_vram()[0];
    if (result) break;
  }

  if (i == mcycles) {
    LOG_R("TIMEOUT\n");
    return 1;
  }

  if (result != 0x55) {
    LOG_R("FAIL @ %d\n", i);
    return 1;
  }

  LOG_G("pass @ %d\n", i);

  return 0;
}

//------------------------------------------------------------------------------

int run_metroboy_microtests() {
  TEST_START();

  double begin = timestamp();

  int fails = 0;
  for (auto name : all_microtests) {
    if (name == "break") break;
    if (name[0] == '-') continue;
    fails += run_metroboy_microtest(name.c_str());
  }
  double end = timestamp();
  LOG_Y("---------- Microtests took %f seconds, %d failures ----------\n", (end - begin), fails);

  TEST_END();
}

//------------------------------------------------------------------------------
