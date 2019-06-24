#include "Platform.h"
#include "main.h"

#include "Audio.h"
#include "Common.h"
#include "MetroBoy.h"
#include "Assembler.h"

#include "test_codegen.h"
#include "test_micro.h"
#include "test_mooneye.h"
#include "test_wpol.h"
#include "test_screenshot.h"

void run_test(const std::string& prefix, const std::string& name);

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  MetroBoyApp app;
  return app.main_(argc, argv);
}

int MetroBoyApp::main_(int /*argc*/, char** /*argv*/) {
  //test_codegen();
  //return 0;

  //metroboy.reset(0x100);

  //run_microtests();
  //run_screenshot_tests();

  //run_test("mooneye-gb/tests/build/acceptance/ppu/", "vblank_stat_intr-GS");
  //run_test("mooneye-gb/tests/build/acceptance/ppu/", "lcdon_write_timing-GS");
  //run_test("wpol-gb/tests/build/acceptance/gpu/",    "intr_2_timing");
  //run_test("wpol-gb/tests/build/acceptance/gpu/",    "vblank_stat_intr-GS");
  //run_test("microtests/build/dmg/", "dma_timing_a");

  //run_mooneye_acceptance();
  //run_wpol_acceptance();
  //run_mealybug_tests();
  //return 0;

  //---------

  //load("wpol-gb/tests/build/acceptance/gpu", "intr_2_mode0_timing_sprites");
  //load("wpol-gb/tests/build/acceptance/gpu", "lcdon_write_timing-GS");
  //load("wpol-gb/tests/build/acceptance/gpu", "intr_2_mode0_timing_sprites_nops");

  //load("oh"); // broken eye
  //load("pocket");
  //load("gejmboj");
  //load("LinksAwakening");
  load("Prehistorik Man (U)");
  //load("SML");

  //load("microtests/build/dmg", "spu_env_change");

  //load("microtests/build/dmg", "oam_sprite_trashing");
  //load("microtests/build/dmg", "oam_write_l0_e");
  //load("microtests/build/dmg", "stat_write_glitch_l1_a");

  //load("microtests/build/dmg", "ppu_spritex_vs_scx");

  //load("microtests/build/dmg", "win13_a");

  //load("microtests/build/dmg", "ppu_scx_vs_bgp");
  //load("microtests/build/dmg", "ppu_win_vs_wx");
  //load("microtests/build/dmg", "ppu_sprite_testbench");
  //load("microtests/build/dmg", "ppu_wx_early");
  
  //load("microtests/build/dmg", "ppu_sprite0_scx1_a");

  //load("microtests/build/dmg", "temp");

  //load("mooneye-gb/tests/build/acceptance/", "boot_hwio-dmgABCmgb");

  //load("mealybug", "m3_lcdc_bg_en_change");              // tiny error top left

  //load("mealybug", "m3_lcdc_obj_en_change_variant");     // tiny fail top left, black bar bottom right, something about bgp
  //load("mealybug", "m3_lcdc_obj_size_change");           // nope
  //load("mealybug", "m3_lcdc_obj_size_change_scx");       // nope
  
  //load("mealybug", "m3_lcdc_win_en_change_multiple_wx"); // off by one-ish, missing dots
  //load("mealybug", "m3_wx_4_change");                    // a few wrong pixels, window retriggering?
  //load("mealybug", "m3_wx_4_change_sprites");            // no dots
  //load("mealybug", "m3_wx_5_change");                    // a few wrong pixels now

  //----------

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  window = SDL_CreateWindow("MetroBoy Gameboy Simulator", 4, 34, fb_width, fb_height, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  fb_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, fb_width, fb_height);
  terminus_surface = SDL_LoadBMP("terminus2.bmp");
  terminus_font = (uint8_t*)terminus_surface->pixels;
  keyboard_state = SDL_GetKeyboardState(nullptr);
  freq = SDL_GetPerformanceFrequency();

  audio_init();

  while (!quit) loop();

  audio_stop();

  SDL_Quit();
  return 0;
}

//-----------------------------------------------------------------------------

void MetroBoyApp::loop() {
  frame_begin = SDL_GetPerformanceCounter();

  //----------------------------------------
  // Process events

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) switch (event.key.keysym.sym) {
    case SDLK_f:      runmode = RUN_FAST; break;
    case SDLK_v:      runmode = RUN_VSYNC; break;
    case SDLK_s:      runmode = STEP_FRAME; break;
    case SDLK_o:      overlay_mode = (overlay_mode + 1) % 3; break;
    case SDLK_RIGHT:  step_forward++; break;
    case SDLK_LEFT:   step_backward++; break;
    case SDLK_UP:     step_up = true; break;
    case SDLK_DOWN:   step_down = true; break;

    case SDLK_r:      reset = true; break;
    case SDLK_F1:     load_dump = true; break;
    case SDLK_F4:     save_dump = true; break;
    case SDLK_ESCAPE: quit = true; break;
    }

    if (event.type == SDL_QUIT) quit = true;

    if (event.type == SDL_DROPFILE) {
      metroboy.load_rom(MODEL_DMG, event.drop.file, false);
      rom_loaded = true;
      runmode = RUN_VSYNC;
      SDL_free(event.drop.file);
    }
  }

  //----------------------------------------
  // Button input

  buttons = 0;
  if (keyboard_state[SDL_SCANCODE_RIGHT])  buttons |= 0x01;
  if (keyboard_state[SDL_SCANCODE_LEFT])   buttons |= 0x02;
  if (keyboard_state[SDL_SCANCODE_UP])     buttons |= 0x04;
  if (keyboard_state[SDL_SCANCODE_DOWN])   buttons |= 0x08;
  if (keyboard_state[SDL_SCANCODE_X])      buttons |= 0x10;
  if (keyboard_state[SDL_SCANCODE_Z])      buttons |= 0x20;
  if (keyboard_state[SDL_SCANCODE_RSHIFT]) buttons |= 0x40;
  if (keyboard_state[SDL_SCANCODE_RETURN]) buttons |= 0x80;

  // probably too fast for debugging
  //if (keyboard_state[SDL_SCANCODE_RIGHT])  step_forward++;
  //if (keyboard_state[SDL_SCANCODE_LEFT])   step_backward++;

  //----------------------------------------
  // Handle sim controls

  if (reset) {
    metroboy.reset(0x0100);
    reset = false;
  }

  if (load_dump) {
    metroboy.load_dump();
    load_dump = false;
    rom_loaded = true;
  }

  if (save_dump) {
    metroboy.save_dump();
    save_dump = false;
  }

  if (step_up) {
    if (runmode == STEP_CYCLE) runmode = STEP_LINE;
    else if (runmode == STEP_LINE) runmode = STEP_FRAME;
  }

  if (step_down) {
    if (runmode == STEP_FRAME) runmode = STEP_LINE;
    else if (runmode == STEP_LINE) runmode = STEP_CYCLE;
  }

  //----------------------------------------
  // Run simulation

  int64_t cycles_begin = metroboy.total_tcycles();

  if (runmode == RUN_FAST) {
    fast_cycles += (16.0 - 1000 * (double(frame_time) / double(freq))) * 100;
    metroboy.run_fast(buttons, (int)fast_cycles);
  }
  else if (runmode == RUN_VSYNC) {
    //printf("%d --------\n", frame_count);
    metroboy.run_vsync(buttons);
  }
  else if (runmode == STEP_CYCLE) {
    while (step_forward--) {
      if (keyboard_state[SDL_SCANCODE_LSHIFT]) {
        metroboy.step_over();
      }
      else {
        metroboy.step_cycle();
      }
    }
    while (step_backward--) {
      metroboy.unstep_cycle();
    }
  }
  else if (runmode == STEP_FRAME) {
    while (step_forward--) metroboy.step_frame();
    while (step_backward--) metroboy.unstep_frame();
  }
  else if (runmode == STEP_LINE) {
    while (step_forward--) metroboy.step_line();
    while (step_backward--) metroboy.unstep_line();
  }

  step_forward = 0;
  step_backward = 0;
  step_up = false;
  step_down = false;

  int64_t cycles_end = metroboy.total_tcycles();

  //----------------------------------------
  // Clear screen

  SDL_LockTexture(fb_tex, NULL, (void**)(&framebuffer), &pitch);

  for (int y = 0; y < 1024; y++) {
    for (int x = 0; x < 1024; x++) {
      int c = ((x ^ y) & 0x20) ? 0x10101010 : 0x15151515;
      framebuffer[x + y * 1024] = c;
    }
  }

  //----------------------------------------
  // Left column text

  Gameboy& gameboy = metroboy.gb();

  gameboy.dump(text_buf);
  gameboy.oam.dump(text_buf);
  render_text(4, 4, text_buf.c_str());
  text_buf.clear();

  gameboy.dump_disasm(text_buf);
  render_text(140, 4, text_buf.c_str());
  text_buf.clear();

  gameboy.spu.dump(text_buf);
  render_text(280, 4, text_buf.c_str());
  text_buf.clear();

  //----------------------------------------
  // Gameboy screen

  const int gb_screenx = (fb_width / 2) - 160;
  const int gb_screeny = (fb_height / 2) - 128;

  if (overlay_mode == 0 || overlay_mode == 1) {
    for (int y = 0; y < 144; y++) {
      uint32_t* line1 = &framebuffer[(y * 2 + gb_screeny + 0) * fb_width + gb_screenx];
      uint32_t* lineM2 = &framebuffer[(y * 2 + gb_screeny + 1) * fb_width + gb_screenx];
      for (int x = 0; x < 160; x++) {
        uint32_t c = gb_colors[gameboy.framebuffer[x + (y * 160)] & 7];
        *line1++ = c; *line1++ = c;
        *lineM2++ = c; *lineM2++ = c;
      }
    }
  }

  draw_bbox(gb_screenx - 2, gb_screeny - 2, 320 + 3, 288 + 3, 0x505050);
  draw_bbox(gb_screenx - 1, gb_screeny - 1, 320+1, 288+1, 0x101010);

  //----------------------------------------
  // Reference image

  if (overlay_mode == 2) {
    for (int y = 0; y < 144; y++) {
      uint32_t* line1 = &framebuffer[(y * 2 + gb_screeny + 0) * fb_width + gb_screenx];
      uint32_t* lineM2 = &framebuffer[(y * 2 + gb_screeny + 1) * fb_width + gb_screenx];
      for (int x = 0; x < 160; x++) {
        uint32_t c = gb_colors[golden[x + y * 160]];
        c += 0x100000;
        *line1++ = c; *line1++ = c;
        *lineM2++ = c; *lineM2++ = c;
      }
    }
  }

  //----------------------------------------
  // Diff overlay

  if (overlay_mode == 1) {
    for (int y = 0; y < 144; y++) {
      uint32_t* line1 = &framebuffer[(y * 2 + gb_screeny + 0) * fb_width + gb_screenx];
      uint32_t* lineM2 = &framebuffer[(y * 2 + gb_screeny + 1) * fb_width + gb_screenx];
      for (int x = 0; x < 160; x++) {
        int c = gameboy.framebuffer[x + (y * 160)];
        if (c != golden[x + y * 160]) {
          *line1++ += 0x808000;
          *line1++ += 0x808000;
          *lineM2++ += 0x808000;
          *lineM2++ += 0x808000;
        }
        else {
          line1++;
          line1++;
          lineM2++;
          lineM2++;
        }
      }
    }
  }

  //----------------------------------------
  // Step highlight

  if (runmode == STEP_LINE || runmode == STEP_CYCLE) {

    int px = gameboy.get_pix_count();
    int py = gameboy.get_line();

    for (int x = 0; x < 320; x++) {
      int color = (px == (x / 2)) ? 0x00606000 : 0x00600000;
      framebuffer[(gb_screenx + x) + (gb_screeny + py * 2 + 0) * fb_width] += color;
      framebuffer[(gb_screenx + x) + (gb_screeny + py * 2 + 1) * fb_width] += color;
    }
  }

  //----------------------------------------
  // Stat bar

  const char* mode_names[] = {
    "RUN_FAST",
    "RUN_VSYNC",
    "STEP_FRAME",
    "STEP_LINE",
    "STEP_CYCLE",
  };

  sprintf(text_buf, "%s %d", mode_names[runmode], (int)(metroboy.current_gameboy->get_tcycle() & 3));
  render_text(32 * 11, 32 * 11 + 18, text_buf.c_str());
  text_buf.clear();

  //----------------------------------------
  // VRAM dump

  gameboy.get_ppu().dump_tiles(framebuffer, fb_width, 736, 32, 2, gameboy.get_vram());
  gameboy.get_ppu().draw_bg_map(framebuffer, fb_width, 736, 448, 1, gameboy.get_vram());
  gameboy.get_ppu().draw_wm_map(framebuffer, fb_width, 736, 736, 1, gameboy.get_vram());

  //----------------------------------------
  // Trace buffer

  int trace_sx = 32 * 8;
  int trace_sy = 32 * 22;

  for (int y = 0; y < 154; y++) {
    for (int x = 0; x < 456; x++) {
      framebuffer[(trace_sx + x) + (trace_sy + y) * fb_width] = metroboy.tracebuffer[x + y * 456];
    }
  }

  //----------------------------------------
  // Perf timer

  static double smoothed_frame_time = 0;
  smoothed_frame_time *= 0.98;
  smoothed_frame_time += (1000.0 * double(frame_time) / double(freq)) * 0.02;

  sprintf(text_buf, "frame time %2.2f msec, %6d cyc/frame\n", (double)smoothed_frame_time, (int)(cycles_end - cycles_begin) / 4);
  render_text(736, 1024 - 12 - 4, text_buf.c_str());
  text_buf.clear();

  //----------------------------------------
  // Console

  //render_console(256 - 32, 1024 - glyph_height * console_height - 4, terminus_font);

  //----------------------------------------
  // Swap

  SDL_UnlockTexture(fb_tex);
  framebuffer = nullptr;
  SDL_RenderCopy(renderer, fb_tex, NULL, NULL);

  frame_end = SDL_GetPerformanceCounter();
  frame_time = frame_end - frame_begin;

  SDL_RenderPresent(renderer);
  frame_count++;
}

//-----------------------------------------------------------------------------

void MetroBoyApp::load(const std::string& prefix, const std::string& name) {
  std::string gb_filename = prefix + "/" + name + ".gb";
  std::string golden_filename = prefix + "/" + name + ".bmp";
  SDL_Surface* golden_surface = SDL_LoadBMP(golden_filename.c_str());

  if (!golden_surface) {
    overlay_mode = 0;
    memset(golden, 0, 160 * 144);
  }

  if (golden_surface && golden_surface->format->format == SDL_PIXELFORMAT_INDEX8) {
    uint8_t* src = (uint8_t*)golden_surface->pixels;
    uint32_t* pal = (uint32_t*)golden_surface->format->palette->colors;
    for (int y = 0; y < 144; y++) {
      for (int x = 0; x < 160; x++) {
        uint8_t a = pal[src[x + y * 160]] & 0xFF;

        if (a < 40) a = 3;
        else if (a < 128) a = 2;
        else if (a < 210) a = 1;
        else a = 0;

        golden[x + y * 160] = a;
      }
    }
    overlay_mode = 1;
  }

  else if (golden_surface && golden_surface->format->format == SDL_PIXELFORMAT_BGR24) {
    uint8_t* src = (uint8_t*)golden_surface->pixels;
    for (int y = 0; y < 144; y++) {
      for (int x = 0; x < 160; x++) {
        uint8_t a = src[x * 3 + y * golden_surface->pitch];

        if (a < 40) a = 3;
        else if (a < 128) a = 2;
        else if (a < 210) a = 1;
        else a = 0;

        golden[x + y * 160] = a;
      }
    }
    overlay_mode = 1;
  }

  memset(rom_buf, 0, 1024 * 1024);
  metroboy.load_rom(MODEL_DMG, gb_filename.c_str(), false);
  rom_loaded = true;
  runmode = RUN_VSYNC;
}

//-----------------------------------------------------------------------------

void MetroBoyApp::render_text(int dst_x, int dst_y, const char* text) {
  int xcursor = 0;
  int ycursor = 0;
  const char* c = text;

  while (*c) {
    if (*c == '\n') {
      xcursor = 0;
      ycursor += glyph_height;
      c++;
      continue;
    }

    int row = ((*c) >> 5) * 16 + 3;
    int col = ((*c) & 31) * 8;
    int src_cursor = col + (row * glyph_stride);
    int dst_cursor = (xcursor + dst_x) + (ycursor + dst_y) * fb_width;

    for (int y = 0; y < glyph_height; y++) {
      for (int x = 0; x < glyph_width; x++) {
        if (terminus_font[src_cursor]) framebuffer[dst_cursor] = 0xFF00FF00;
        src_cursor++;
        dst_cursor++;
      }
      src_cursor += (glyph_stride - glyph_width);
      dst_cursor += (fb_width - glyph_width);
    }

    xcursor += glyph_width;
    if (xcursor > (fb_width - glyph_width)) {
      xcursor = 0;
      ycursor += glyph_height;
    }
    c++;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyApp::draw_bbox(int sx, int sy, int w, int h, uint32_t color) {
  int ax = sx;
  int bx = sx + w;
  int ay = sy;
  int by = sy + h;
  int x, y;

  for (x = ax, y = ay; x <= bx; x++) {
    if (x >= 0 && x <= fb_width && y >= 0 && y <= fb_height) framebuffer[x + y * fb_width] = color;
  }

  for (x = ax, y = by; x <= bx; x++) {
    if (x >= 0 && x <= fb_width && y >= 0 && y <= fb_height) framebuffer[x + y * fb_width] = color;
  }

  for (x = ax, y = ay + 1; y <= by - 1; y++) {
    if (x >= 0 && x <= fb_width && y >= 0 && y <= fb_height) framebuffer[x + y * fb_width] = color;
  }

  for (x = bx, y = ay + 1; y <= by - 1; y++) {
    if (x >= 0 && x <= fb_width && y >= 0 && y <= fb_height) framebuffer[x + y * fb_width] = color;
  }
}


//-----------------------------------------------------------------------------
// Console

void MetroBoyApp::render_console(int sx, int sy, uint8_t* font) {

  for (int cy = 0; cy < console_height; cy++) {
    char* line = &console_buf[(((cy + cursor_y)) % console_height) * console_width];

    for (int cx = 0; cx < console_width; cx++) {
      char c = *line;
      int row = (c >> 5) * 16 + 3;
      int col = (c & 31) * 8;

      int src_cursor = col + (row * glyph_stride);
      int dst_cursor = (sx + (cx * glyph_width)) + (sy + (cy * glyph_height)) * fb_width;

      for (int y = 0; y < glyph_height; y++) {
        for (int x = 0; x < glyph_width; x++) {
          if (font[src_cursor]) framebuffer[dst_cursor] = 0xFF00FF00;
          src_cursor++;
          dst_cursor++;
        }
        src_cursor += (glyph_stride - glyph_width);
        dst_cursor += (fb_width - glyph_width);
      }

      line++;
    }
  }

  draw_bbox(sx - 2, sy - 2, console_width * glyph_width + 4, console_height * glyph_height + 4, 0xFF00FF00);
}

//-----------------------------------------------------------------------------

void MetroBoyApp::printf_console(const char* format, ...) {

  char buffer[256];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  char* text = buffer;
  while (*text) {
    char c = *text;
    if (c == '\n') {
      for (int i = cursor_x; i < console_width; i++) {
        console_buf[i + cursor_y * console_width] = 0;
      }
      cursor_x = 0;
      cursor_y = (cursor_y + 1) % console_height;
    }
    else {
      console_buf[cursor_x + cursor_y * console_width] = c;
      cursor_x++;
      if (cursor_x == console_width) {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y == console_height) {
          cursor_y = 0;
        }
      }
    }
    text++;
  }
}

//-----------------------------------------------------------------------------
