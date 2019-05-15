#include "Platform.h"

#include "Common.h"
#include "Gameboy.h"

#include <string>

static const char* screenshot_tests[] = {
  "sprite_0_a"
};


bool load_bmp(const std::string& prefix, const std::string& name, uint8_t* out_golden) {
  std::string golden_filename = prefix + "/" + name + ".bmp";
  SDL_Surface* golden_surface = SDL_LoadBMP(golden_filename.c_str());

  if (!golden_surface) return false;

  if (golden_surface->format->format == SDL_PIXELFORMAT_INDEX8) {
    uint8_t* src = (uint8_t*)golden_surface->pixels;
    uint32_t* pal = (uint32_t*)golden_surface->format->palette->colors;
    for (int y = 0; y < 144; y++) {
      for (int x = 0; x < 160; x++) {
        uint8_t a = pal[src[x + y * 160]] & 0xFF;

        if (a < 40) a = 3;
        else if (a < 128) a = 2;
        else if (a < 210) a = 1;
        else a = 0;

        out_golden[x + y * 160] = a;
      }
    }
    return true;
  }

  else if (golden_surface->format->format == SDL_PIXELFORMAT_BGR24) {
    uint8_t* src = (uint8_t*)golden_surface->pixels;
    for (int y = 0; y < 144; y++) {
      for (int x = 0; x < 160; x++) {
        uint8_t a = src[x * 3 + y * golden_surface->pitch];

        if (a < 40) a = 3;
        else if (a < 128) a = 2;
        else if (a < 210) a = 1;
        else a = 0;

        out_golden[x + y * 160] = a;
      }
    }
    return true;
  }

  return false;
}

void run_screenshot_test(int model, const std::string& prefix, const std::string& name) {
  std::string filename = prefix + name + ".gb";

  uint8_t golden[160 * 144];
  if (!load_bmp(prefix, name, golden)) {
    printf("Couldn't load golden image for %s\n", name.c_str());
    return;
  }

  FILE* rom_file = NULL;
  rom_file = fopen(filename.c_str(), "rb");
  fseek(rom_file, 0, SEEK_END);
  size_t rom_size = ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  rom_size = fread(rom_buf, 1, rom_size, rom_file);
  fclose(rom_file);

  Gameboy gameboy;
  gameboy.reset(model, rom_size, 0x100);

  int i = 0;
  const int ticks = 100000;
  for (; i < ticks; i++) {
    gameboy.tick();
    gameboy.tock();
    if (gameboy.is_frame_done()) break;
  }

  int result = 0;

  // diff

  // print result

  if (i == ticks) {
    printf("%-50s ", name.c_str());
    printf("? TIMEOUT @ %d\n", i);
  }
  else if (result == 0x55) {
    printf(".");
    //printf("  0x%02x PASS @ %d\n", result, i);
  }
  else {
    printf("\n");
    printf("%-50s ", name.c_str());
    printf("X 0x%02x FAIL @ %d\n", result, i);
  }
}

void run_screenshot_tests() {
  double freq = (double)SDL_GetPerformanceFrequency();
  double begin = (double)SDL_GetPerformanceCounter();

  int model = MODEL_DMG;
  std::string model_string = (model == MODEL_DMG ? "dmg" : "ags");
  std::string prefix = "microtests/build/" + model_string + "/";

  printf("---------- Screenshot tests in %s: ----------\n", prefix.c_str());

  for (auto name : screenshot_tests) {
    if (name[0] == '-') continue;
    run_screenshot_test(model, prefix, name);
  }
  printf("\n");

  double end = (double)SDL_GetPerformanceCounter();
  printf("---------- Screenshot tests took %f seconds ----------\n", (end - begin) / freq);
}
