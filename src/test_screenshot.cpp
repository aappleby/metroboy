#include "Gameboy.h"
#include "File.h"

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#pragma warning(disable : 4996)

static const char* mealybug_tests[] = {
  "m3_window_timing",
  "m3_bgp_change",
  "m3_bgp_change_sprites",
  "m3_obp0_change",
  "m3_lcdc_tile_sel_change",
  "m3_lcdc_tile_sel_win_change",
  "m3_lcdc_win_en_change_multiple",
  "m3_lcdc_bg_map_change",
  "m3_lcdc_win_map_change",
  "m3_window_timing_wx_0",
  "m3_wx_6_change",

  "m3_lcdc_bg_en_change",
  "m3_lcdc_obj_en_change",
  "m3_scx_low_3_bits",
  "m3_lcdc_obj_en_change_variant",
  "m3_lcdc_obj_size_change",
  "m3_lcdc_obj_size_change_scx",
  "m3_lcdc_win_en_change_multiple_wx",
  "m3_wx_4_change",
  "m3_wx_4_change_sprites",
  "m3_wx_5_change",
};

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

int run_screenshot_test(const std::string& prefix, const std::string& name) {
  std::string filename = prefix + name + ".gb";

  uint8_t golden[160 * 144];
  if (!load_bmp(prefix, name, golden)) {
    printf("Couldn't load golden image for %s\n", name.c_str());
    return -1;
  }

  blob rom;
  load_array(filename, rom);

  Gameboy gameboy;
  uint8_t fb[160*144] = {};
  gameboy.set_rom(rom.data(), rom.size());
  gameboy.reset(0x100);

  int i = 0;
  const int mcycles = 400000;
  for (; i < mcycles; i++) {
    gameboy.mcycle();
    Gameboy::HostOut gb_out = gameboy.get_host_data();

    if (gb_out.pix_oe) {
      int x = gb_out.x;
      int y = gb_out.y;

      if (x >= 0 && x < 160 && y >= 0 && y < 144) {
        fb[x + y * 160] = gb_out.pix;
      }
    }
  }

  int diff = 0;

  for (int y = 0; y < 144; y++) {
    for (int x = 0; x < 160; x++) {
      if (fb[x + y * 160] != golden[x + y * 160]) {
        diff++;
      }
    }
  }

  return diff;
}

void run_mealybug_tests() {
  double freq = (double)SDL_GetPerformanceFrequency();
  double begin = (double)SDL_GetPerformanceCounter();

  printf("---------- Mealybug tests ----------\n");

  bool dots = true;
  for (auto name : mealybug_tests) {
    if (name[0] == '-') continue;
    int diff = run_screenshot_test("mealybug/", name);
    if (diff == 0) {
      printf(".");
      dots = true;
    }
    else {
      if (dots) {
        printf("\n");
        dots = false;
      }
      printf("%-50s ", name);
      printf("X FAIL diff %d\n", diff);
    }

  }
  printf("\n");

  double end = (double)SDL_GetPerformanceCounter();
  printf("---------- Mealybug tests took %f seconds ----------\n", (end - begin) / freq);
}

void run_screenshot_tests() {
  /*
  double freq = (double)SDL_GetPerformanceFrequency();
  double begin = (double)SDL_GetPerformanceCounter();

  std::string prefix = "microtests/build/dmg/";

  printf("---------- Screenshot tests in %s: ----------\n", prefix.c_str());

  for (auto name : screenshot_tests) {
    if (name[0] == '-') continue;
    run_screenshot_test(model, prefix, name);
  }
  printf("\n");

  double end = (double)SDL_GetPerformanceCounter();
  printf("---------- Screenshot tests took %f seconds ----------\n", (end - begin) / freq);
  */
}
