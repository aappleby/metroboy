#include "MetroBoyApp.h"

#include "MetroBoy.h"
#include "Audio.h"
#include "GLBase.h"
#include "Debug.h"

#include "test_codegen.h"
#include "test_micro.h"
#include "test_mooneye.h"
#include "test_wpol.h"
#include "test_screenshot.h"

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <imgui.h>
#include <imgui_internal.h>
#include <vector>
#include <typeinfo>

extern const uint32_t gb_colors[];

void run_wpol_test(const std::string& prefix, const std::string& name);

//-----------------------------------------------------------------------------

MetroBoyApp::MetroBoyApp() {
  post();
}

//-----------------------------------------------------------------------------

void MetroBoyApp::post() {
  double freq = (double)SDL_GetPerformanceFrequency();
  double begin = (double)SDL_GetPerformanceCounter();
  bool pass = true;

  printf("\n");
  printf("---------- POST begin ----------\n");

  pass &= run_microtest("microtests/build/dmg", "poweron_000_div");

#if 0
  printf("Timer\n");
  pass &= run_microtest("microtests/build/dmg", "div_inc_timing_a");
  pass &= run_microtest("microtests/build/dmg", "div_inc_timing_b");
  pass &= run_microtest("microtests/build/dmg", "poweron_000_div");
  pass &= run_microtest("microtests/build/dmg", "poweron_004_div");
  pass &= run_microtest("microtests/build/dmg", "poweron_005_div");
  pass &= run_microtest("microtests/build/dmg", "timer_div_phase_c");
  pass &= run_microtest("microtests/build/dmg", "timer_div_phase_d");
  printf("\n");
#endif

#if 0
  printf("Reset\n");
  pass &= run_microtest("micro_cpu/build/dmg", "rst_0x00");
  pass &= run_microtest("micro_cpu/build/dmg", "rst_0x08");
  pass &= run_microtest("micro_cpu/build/dmg", "rst_0x10");
  pass &= run_microtest("micro_cpu/build/dmg", "rst_0x18");
  pass &= run_microtest("micro_cpu/build/dmg", "rst_0x20");
  pass &= run_microtest("micro_cpu/build/dmg", "rst_0x28");
  pass &= run_microtest("micro_cpu/build/dmg", "rst_0x30");
  pass &= run_microtest("micro_cpu/build/dmg", "rst_0x38");
  printf("\n");
#endif

  if (!pass) exit(1);

  double end = (double)SDL_GetPerformanceCounter();
  printf("---------- POST done in %f seconds ----------\n", (end - begin) / freq);
  printf("\n");
}

//-----------------------------------------------------------------------------

const char* MetroBoyApp::app_get_title() {
  return "MetroBoy";
}

//-----------------------------------------------------------------------------

void MetroBoyApp::app_init() {

  app_start = SDL_GetPerformanceCounter();

  blitter.init();
  grid_painter.init();
  text_painter.init();

  keyboard_state = SDL_GetKeyboardState(nullptr);
  audio_init();

  gb_tex = create_texture_u8(160, 144);
  trace_tex = create_texture_u32(456, 154);
  ram_tex = create_texture_u8(256, 256);

  gb_blitter.init();
  dump_painter.init();

  //run_microtests();
  //run_screenshot_tests();
  //run_mooneye_acceptance();
  //run_wpol_acceptance();
  //run_mealybug_tests();
  //return 0;

  //---------

  //load("oh"); // broken eye
  //load("pocket");
  //load("gejmboj");
  //load("LinksAwakening");
  //load("Prehistorik Man (U)");
  //load("roms/", "SML");
  //load("roms/", "tetris");

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

  //load_memdump("roms", "LinksAwakening_house");
  load_memdump("roms", "LinksAwakening_dog");
  //load_memdump("roms", "tetris");

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
};

//----------------------------------------

void MetroBoyApp::app_close() {
  audio_stop();
};

//-----------------------------------------------------------------------------

void MetroBoyApp::load_memdump(const std::string& prefix, const std::string& name) {
  load("microtests/build/dmg", "minimal");

  std::string filename = prefix + "/" + name + ".dump";
  blob buf;
  load_array(filename, buf);

  memcpy(metroboy.gb().get_vram(), &buf[0x8000], 8192);

  metroboy.gb().ppu.lcdc = buf[0xFF40];
  metroboy.gb().ppu.stat = buf[0xFF41];
  metroboy.gb().ppu.scy  = buf[0xFF42];
  metroboy.gb().ppu.scx  = buf[0xFF43];
  metroboy.gb().ppu.ly   = buf[0xFF44];
  metroboy.gb().ppu.lyc  = buf[0xFF45];
  //metroboy.gb().ppu.dma  = buf[0xFF46];
  metroboy.gb().ppu.bgp  = buf[0xFF47];
  metroboy.gb().ppu.obp0 = buf[0xFF48];
  metroboy.gb().ppu.obp1 = buf[0xFF49];
  metroboy.gb().ppu.wy   = buf[0xFF4A];
  metroboy.gb().ppu.wx   = buf[0xFF4B];
  metroboy.gb().ppu.update_palettes();

  uint8_t* oam_flat = (uint8_t*)metroboy.gb().oam.get();

  for (int i = 0; i < 256; i++) {
    oam_flat[i] = buf[ADDR_OAM_BEGIN + i];
  }
}

//-----------------------------------------------------------------------------

void MetroBoyApp::load(const std::string& prefix, const std::string& name) {
  std::string gb_filename = prefix + "/" + name + ".gb";
  printf("Loading rom %s\n", gb_filename.c_str());
  metroboy.load_rom(gb_filename.c_str(), false);
  rom_loaded = true;
  runmode = STEP_PHASE;

#if 0
  std::string golden_filename = prefix + "/" + name + ".bmp";
  SDL_Surface* golden_surface = SDL_LoadBMP(golden_filename.c_str());

  if (!golden_surface) {
    overlay_mode = 0;
    memset(golden, 0, 160 * 144);
  }

  if (golden_surface && golden_surface->format->format == SDL_PIXELFORMAT_INDEX8) {
    printf("Loaded i8 golden\n");
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
    printf("Loaded argb golden\n");
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
#endif
}

//-----------------------------------------------------------------------------

void MetroBoyApp::app_update(double /*delta*/) {
  ImGuiIO& io = ImGui::GetIO();

  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    if (!io.WantCaptureKeyboard) {
      if (event.type == SDL_KEYDOWN) switch (event.key.keysym.sym) {
      case SDLK_f:      runmode = RUN_FAST; break;
      case SDLK_v:      runmode = RUN_VSYNC; break;
      case SDLK_s:      runmode = STEP_FRAME; break;
      case SDLK_o:      overlay_mode = (overlay_mode + 1) % 3; break;
      case SDLK_RIGHT:  {
        if (keyboard_state[SDL_SCANCODE_LCTRL]) {
          step_forward = 8;
        } else if (keyboard_state[SDL_SCANCODE_LALT]) {
          step_forward = 1024;
        } else {
          step_forward = 1;
        }
        break;
      }
      case SDLK_LEFT:   {
        step_backward = 1;
        break;
      }
      case SDLK_UP:     step_up = true; break;
      case SDLK_DOWN:   step_down = true; break;

      case SDLK_r:      reset = true; break;
      case SDLK_F1:     load_dump = true; break;
      case SDLK_F4:     save_dump = true; break;
      }
    }

    if (event.type == SDL_DROPFILE) {
      metroboy.load_rom(event.drop.file, false);
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
    if (runmode == STEP_PHASE) runmode = STEP_LINE;
    else if (runmode == STEP_LINE) runmode = STEP_FRAME;
  }

  if (step_down) {
    if (runmode == STEP_FRAME) runmode = STEP_LINE;
    else if (runmode == STEP_LINE) runmode = STEP_PHASE;
  }

  //----------------------------------------
  // Run simulation

  if (runmode == RUN_FAST) {
    int64_t phase_begin = metroboy.gb().phase_total;
    double time_begin = double(SDL_GetPerformanceCounter() - app_start);
    metroboy.run_fast(buttons, (int)fast_mcycles);
    double time_end = double(SDL_GetPerformanceCounter() - app_start);
    int64 phase_end = metroboy.gb().phase_total;
    last_mcycles = int(phase_end - phase_begin) / 8;
    sim_time_msec = 1000.0 * double(time_end - time_begin) / double(SDL_GetPerformanceFrequency());
    
    //printf("run_fast %f\n", elapsed_ms);
    double new_mcycles = fast_mcycles * (sim_budget_msec / sim_time_msec);
    //printf("run_fast %f\n", new_cycles);
    
    double a = 0.95;
    fast_mcycles = fast_mcycles * a + new_mcycles * (1.0 - a);
  }
  else if (runmode == RUN_VSYNC) {
    //printf("%d --------\n", frame_count);
    int64_t phase_begin = metroboy.gb().phase_total;
    double time_begin = double(SDL_GetPerformanceCounter() - app_start);
    metroboy.run_vsync(buttons);
    double time_end = double(SDL_GetPerformanceCounter() - app_start);
    int64 phase_end = metroboy.gb().phase_total;
    last_mcycles = int(phase_end - phase_begin) / 4;
    sim_time_msec = 1000.0 * double(time_end - time_begin) / double(SDL_GetPerformanceFrequency());
  }
  else if (runmode == STEP_PHASE) {
    if (step_forward)  metroboy.step_phase(step_forward);
    if (step_backward) metroboy.unstep_phase();
  }
  else if (runmode == STEP_FRAME) {
    if (step_forward)  metroboy.step_frame(step_forward);
    if (step_backward) metroboy.unstep_frame();
  }
  else if (runmode == STEP_LINE) {
    if (step_forward)  metroboy.step_line(step_forward);
    if (step_backward) metroboy.unstep_line();
  }

  step_forward = 0;
  step_backward = 0;
  step_up = false;
  step_down = false;
}

//-----------------------------------------------------------------------------

void MetroBoyApp::app_render_frame(Viewport view) {

  grid_painter.render(view);

  /*
  static bool CLKIN_A = 0;
  static bool CLKIN_B = 0;

  ImGui::Begin("GB_sys_reset");
  CLKIN_A = ImGui::ButtonEx("CLKIN_A", ImVec2(0,0), ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_Repeat);
  if (CLKIN_A) {
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "CLKIN_A");
  }
  else {
    ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "CLKIN_A");
  }
  ImGui::Checkbox("CLKIN_B", &CLKIN_B);
  ImGui::Text("T1");
  ImGui::Text("T2");
  ImGui::Text("RST");
  ImGui::End();
  */

  //----------------------------------------
  // Wave thingy

  /*
  for (int i = 0; i < 16; i++) {
    uint8_t a = (gameboy.get_spu().get_wave()[i] & 0x0F) >> 0;
    uint8_t b = (gameboy.get_spu().get_wave()[i] & 0xF0) >> 4;
    uint32_t color = 0xFFFFFFFF;

    framebuffer[(512 + 2 * i + 0) + (100 + b) * fb_width] = color;
    framebuffer[(512 + 2 * i + 1) + (100 + a) * fb_width] = color;
  }
  */

  //----------------------------------------
  // Gameboy screen

  update_texture_u8(ram_tex, 0, 0*32, 256, 128, metroboy.get_cart_rom());
  update_texture_u8(ram_tex, 0, 4*32, 256,  32, metroboy.get_vram());
  update_texture_u8(ram_tex, 0, 5*32, 256,  32, metroboy.get_cart_ram());
  update_texture_u8(ram_tex, 0, 6*32, 256,  32, metroboy.get_main_ram());
  update_texture_u8(ram_tex, 0, 7*32, 256,  32, metroboy.get_main_ram());

  gb_blitter.blit_screen(view, 32 * 32, 32 * 1, 2, metroboy.fb());

  /*
  #define FLAG_BG_ON        0x01
  #define FLAG_OBJ_ON       0x02
  #define FLAG_TALL_SPRITES 0x04
  #define FLAG_BG_MAP_1     0x08
  #define FLAG_TILE_0       0x10
  #define FLAG_WIN_ON       0x20
  #define FLAG_WIN_MAP_1    0x40
  #define FLAG_LCD_ON       0x80
  */

  /*
  int bg_map  = (metroboy.gb().get_ppu().lcdc & FLAG_BG_MAP_1)  ? 1 : 0;
  int win_map = (metroboy.gb().get_ppu().lcdc & FLAG_WIN_MAP_1) ? 1 : 0;
  int alt_map = (metroboy.gb().get_ppu().lcdc & FLAG_TILE_0)    ? 0 : 1;

  gb_blitter.blit_tiles (view, 32 * 43, 32 * 1,     metroboy.get_vram());
  gb_blitter.blit_map   (view, 32 * 43, 32 * 14, 1, metroboy.get_vram(), bg_map, alt_map);
  gb_blitter.blit_map   (view, 32 * 43, 32 * 23, 1, metroboy.get_vram(), win_map, alt_map);


  blitter.blit_mono(view, ram_tex, 256, 256, 0, 0, 256, 256, 32 * 32, 32 * 11, 256, 256);
  */

  /*
  const int gb_trace_x = screen_w - 32 * 17;
  const int gb_trace_y = screen_h - 32 * 7;
  gb_blitter.blit_trace (get_viewport(), gb_trace_x, gb_trace_y, metroboy.get_trace());
  */


  //dump_painter.render(view, 900, 100, 16, 8, metroboy.gb().get_zram());
  //dump_painter.render(view, 900, 300, 64, 128, metroboy.gb().get_iram());

#if 0

  //----------------------------------------
  // Reference image

  if (overlay_mode == 2) {
    for (int y = 0; y < 144; y++) {
      uint32_t* line1 = &framebuffer[(y * 2 + gb_screen_y + 0) * fb_width + gb_screen_x];
      uint32_t* lineM2 = &framebuffer[(y * 2 + gb_screen_y + 1) * fb_width + gb_screen_x];
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
      uint32_t* line1 = &framebuffer[(y * 2 + gb_screen_y + 0) * fb_width + gb_screen_x];
      uint32_t* lineM2 = &framebuffer[(y * 2 + gb_screen_y + 1) * fb_width + gb_screen_x];
      for (int x = 0; x < 160; x++) {
        int c = fb.buf[x + (y * 160)];
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

    int px = metroboy.gb_out.x;
    int py = metroboy.gb_out.y;

    for (int x = 0; x < 320; x++) {
      int color = (px == (x / 2)) ? 0x00606000 : 0x00600000;
      framebuffer[(gb_screen_x + x) + (gb_screen_y + py * 2 + 0) * fb_width] += color;
      framebuffer[(gb_screen_x + x) + (gb_screen_y + py * 2 + 1) * fb_width] += color;
    }
  }

#endif

}

//-----------------------------------------------------------------------------

void MetroBoyApp::app_render_ui(Viewport view) {
  StringDumper dump;

  //----------------------------------------
  // Left column text

  int column = 0;

  Gameboy& gameboy = metroboy.gb();

  if (1) {
    gameboy.dump_bus(dump);
    dump("\n");

    gameboy.dump_cpu(dump);
    dump("\n");

    //gameboy.dump_timer(dump);
    //dump("\n");

    gameboy.dma2.dump(dump);
    dump("\n");

    text_painter.render(view, dump.s, (float)column, 0);
    dump.clear();
    column += 32 * 8;
  }

  if (1) {
    gameboy.dump_zram(dump);
    dump("\n");

    gameboy.dump_cart(dump);
    dump("\n");

    gameboy.dump_oam(dump);
    dump("\n");

    gameboy.dump_joypad(dump);
    dump("\n");

    gameboy.dump_serial(dump);
    dump("\n");

    text_painter.render(view, dump.s, (float)column, 0);
    dump.clear();
    column += 32 * 8;
  }

  if (1) {
    gameboy.lcd.dump(dump);
    dump("\n");

    gameboy.get_ppu().dump(dump);
    dump("\n");

    text_painter.render(view, dump.s, (float)column, 0);
    dump.clear();
    column += 32 * 8;
  }

  if (1) {
    gameboy.dump_disasm(dump);
    dump("\n");

    gameboy.get_spu().dump(dump);
    dump("\n");

    text_painter.render(view, dump.s, (float)column, 0);
    dump.clear();
    column += 32 * 8;
  }

  {
    double sim_mcycles_per_sec = double(last_mcycles) * (1000.0 / sim_budget_msec);
    double rt_mcycles_per_sec = 114.0 * 154.0 * 60.0;

    const char* mode_names[] = {
        "RUN_FAST", "RUN_VSYNC", "STEP_FRAME", "STEP_LINE", "STEP_CYCLE",
    };

    text_painter.dprintf("%s %d\n", mode_names[runmode], (int)(metroboy.gb().phase_total & 7));
    text_painter.dprintf("sim budget %2.2f msec/frame\n", sim_budget_msec);
    text_painter.dprintf("sim time   %02.2f msec/frame\n", sim_time_msec);
    text_painter.dprintf("sim rate   %7d cycles/frame\n", last_mcycles);
    text_painter.dprintf("sim speed  %1.2fx realtime\n", sim_mcycles_per_sec / rt_mcycles_per_sec);
    
    text_painter.render(view, float(view.screen_size.x - 300 + 96), float(view.screen_size.y - 64));
  }
}

//-----------------------------------------------------------------------------
