#include "GateBoyApp/GateBoyApp.h"

#include "CoreLib/Constants.h"
#include "CoreLib/Debug.h" // for StringDumper

#include "AppLib/AppHost.h"
#include "AppLib/GLBase.h"

#define SDL_MAIN_HANDLED
#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

using namespace Schematics;

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  App* app = new GateBoyApp();
  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_init() {
  printf("GateBoyApp::app_init()\n");

  grid_painter.init();
  text_painter.init();
  dump_painter.init();
  gb_blitter.init();
  blitter.init();

  trace_tex = create_texture_u32(912, 154);
  ram_tex = create_texture_u8(256, 256);
  overlay_tex = create_texture_u32(160, 144);
  keyboard_state = SDL_GetKeyboardState(nullptr);

  gb_thread.init();
  gb_thread.start();
}

void GateBoyApp::app_close() {
  gb_thread.stop();
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_update(double /*delta*/) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_KEYDOWN)
    switch (event.key.keysym.sym) {

    case SDLK_SPACE: { gb_thread.sig_pause ? gb_thread.resume() : gb_thread.pause(); break; }

    case SDLK_f: {
      gb_thread.clear_work();
      if (runmode != RUN_FAST) {
        gb_thread.step_phase(1024 * 1024 * 1024);
        gb_thread.resume();
        runmode = RUN_FAST;
      }
      break;
    }
    case SDLK_v: {
      gb_thread.clear_work();
      runmode = RUN_SYNC;
      break;
    }
    case SDLK_s: {
      gb_thread.clear_work();
      runmode = RUN_STEP;
      break;
    }

    case SDLK_F1:   gb_thread.load_raw_dump();  break;
    case SDLK_F4:   gb_thread.save_raw_dump();  break;
    case SDLK_r:    gb_thread.reset();          break;
    case SDLK_d:    show_diff   = !show_diff;   break;
    case SDLK_g:    show_golden = !show_golden; break;
    case SDLK_o:    draw_passes = !draw_passes; break;
    case SDLK_UP:   stepmode = STEP_PHASE;      break;
    case SDLK_DOWN: stepmode = STEP_PASS;       break;
    //case SDLK_c:  { gb_thread.toggle_cpu(); break; }

    case SDLK_LEFT:   {
      if (runmode == RUN_STEP) {
        if (keyboard_state[SDL_SCANCODE_LCTRL]) {
          gb_thread.step_back(8);
        } else {
          gb_thread.step_back(1);
        }
      }
      break;
    }

    case SDLK_RIGHT:  {
      if (runmode == RUN_STEP) {
        if (keyboard_state[SDL_SCANCODE_LCTRL] && keyboard_state[SDL_SCANCODE_LALT]) {
          gb_thread.step_phase(114 * 8 * 8);
        }
        else if (keyboard_state[SDL_SCANCODE_LALT]) {
          gb_thread.step_phase(114 * 8);
        } else if (keyboard_state[SDL_SCANCODE_LCTRL]) {
          gb_thread.step_phase(8);
        } else {
          gb_thread.step_phase(1);
        }
      }
      break;
    }
    }

    if (event.type == SDL_DROPFILE) {
      gb_thread.load_rom(event.drop.file);
      SDL_free(event.drop.file);
    }
  }
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_render_frame(Viewport view) {
  gb_thread.pause();

  grid_painter.render(view);

  const auto& top = gb_thread.gb->top;
  uint8_t* framebuffer = gb_thread.gb->framebuffer;
  uint8_t* vid_ram = gb_thread.gb->vid_ram;
  int fb_x = gb_thread.gb->screen_x;
  int fb_y = gb_thread.gb->screen_y;
  int64_t phase_total = gb_thread.gb->phase_total;
  bool sim_stable = gb_thread.gb->sim_stable;

  StringDumper dumper;
  float cursor = 0;

  gb_thread.dump1(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += 224 - 32;
  dumper.clear();

  //----------

  gb_thread.dump2(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += 224 - 64;
  dumper.clear();

  //----------

  gb_thread.dump3(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += 224;
  dumper.clear();

  //----------

  gb_thread.dump4(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += 224;
  dumper.clear();

  //----------

  gb_thread.dump5(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += 224 - 32;
  dumper.clear();

  //----------

  gb_thread.dump6(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  dumper.clear();

  //update_texture_u32(trace_tex, 912, 154, trace);
  //blitter.blit(view, trace_tex, 0, 0, 912, 154);

  // Draw flat memory view
  /*
  {
    //printf("rom_buf.data() %p\n", rom_buf.data());
    //printf("gb->rom_buf %p\n", gb->rom_buf);

    update_texture_u8(ram_tex, 0x00, 0x00, 256, 128, gb->rom_buf);
    update_texture_u8(ram_tex, 0x00, 0x80, 256,  32, gb->vid_ram);
    update_texture_u8(ram_tex, 0x00, 0xA0, 256,  32, gb->cart_ram);
    update_texture_u8(ram_tex, 0x00, 0xC0, 256,  32, gb->ext_ram);
    update_texture_u8(ram_tex, 0x00, 0xFE, 256,   1, gb->oam_ram);
    update_texture_u8(ram_tex, 0x80, 0xFF, 128,   1, gb->zero_ram);
    blitter.blit_mono(view, ram_tex, 256, 256,
                      0, 0, 256, 256,
                      960 + 96 - 64, 640 + 96, 256, 256);
  }
  */

  // Draw screen and vid ram contents

  int gb_x = 1216;
  int gb_y = 32;

  if (has_golden && show_diff) {
    gb_blitter.blit_diff(view, gb_x, gb_y,  2, framebuffer, golden_u8);
  } else if (show_golden) {
    gb_blitter.blit_screen(view, gb_x, gb_y,  2, golden_u8);
  } else {
    gb_blitter.blit_screen(view, gb_x, gb_y,  2, framebuffer);
  }

  gb_blitter.blit_tiles (view, 1632, 32,  1, vid_ram);
  gb_blitter.blit_map   (view, 1344, 448, 1, vid_ram, 0, 0);
  gb_blitter.blit_map   (view, 1632, 448, 1, vid_ram, 0, 1);
  gb_blitter.blit_map   (view, 1344, 736, 1, vid_ram, 1, 0);
  gb_blitter.blit_map   (view, 1632, 736, 1, vid_ram, 1, 1);

  // Draw screen overlay
  if (fb_y >= 0 && fb_y < 144 && fb_x >= 0 && fb_x < 160) {
    memset(overlay, 0, sizeof(overlay));

    for (int x = 0; x < fb_x; x++) {
      uint8_t p0 = top.lcd_pipe_lo[159 - fb_x + x + 1].qp();
      uint8_t p1 = top.lcd_pipe_hi[159 - fb_x + x + 1].qp();

      int r = (3 - (p0 + p1 * 2)) * 30 + 50;
      int g = (3 - (p0 + p1 * 2)) * 30 + 50;
      int b = (3 - (p0 + p1 * 2)) * 30 + 30;

      overlay[x + fb_y * 160] = 0xFF000000 | (b << 16) | (g << 8) | (r << 0);
    }
    {
      uint8_t p0 = top.lcd_pix_lo.qp04();
      uint8_t p1 = top.lcd_pix_hi.qp04();

      int c = (3 - (p0 + p1 * 2)) * 85;

      overlay[fb_x + fb_y * 160] = 0xFF000000 | (c << 16) | (c << 8) | (c << 0);
    }

    update_texture_u32(overlay_tex, 160, 144, overlay);
    blitter.blit(view, overlay_tex, gb_x, gb_y, 160 * 2, 144 * 2);
  }

  // Status bar under screen

  //double phases_per_frame = 114 * 154 * 60 * 8;
  //double sim_ratio = sim_rate / phases_per_frame;
  double sim_ratio = 0.0;
  double sim_time_smooth = 0.0;

  dumper("%s %s Sim clock %8.3f %s %c %s\n",
    runmode_names[runmode],
    stepmode_names[stepmode],
    double(phase_total) / (4194304.0 * 2),
    phase_names[phase_total & 7],
    sim_stable ? ' ' : '*',
    show_golden ? "GOLDEN IMAGE " : "");
  dumper("Sim time %f, sim ratio %f, frame time %f\n", sim_time_smooth, sim_ratio, frame_time_smooth);
  text_painter.render(view, dumper.s, gb_x, gb_y + 144 * 2);
  dumper.clear();

  // Probe dump

  gb_thread.gb->probes.dump(dumper, draw_passes);
  text_painter.render(view, dumper.s, 640 - 64, 640 + 128);
  dumper.clear();

  frame_count++;
  gb_thread.resume();
}

//------------------------------------------------------------------------------

void GateBoyApp::load_golden(const char* filename) {
  SDL_Surface* golden_surface = SDL_LoadBMP(filename);

  if (!golden_surface) {
    printf("Failed to load golden %s\n", filename);
    memset(golden_u8, 0, 160 * 144);
    return;
  }

  if (golden_surface && golden_surface->format->format == SDL_PIXELFORMAT_INDEX8) {
    printf("Loaded i8 golden %s\n", filename);
    uint8_t* src = (uint8_t*)golden_surface->pixels;
    uint32_t* pal = (uint32_t*)golden_surface->format->palette->colors;
    for (int y = 0; y < 144; y++) {
      for (int x = 0; x < 160; x++) {
        uint8_t a = pal[src[x + y * 160]] & 0xFF;

        if (a < 40) a = 3;
        else if (a < 128) a = 2;
        else if (a < 210) a = 1;
        else a = 0;

        golden_u8[x + y * 160] = a;
      }
    }
  }

  else if (golden_surface && golden_surface->format->format == SDL_PIXELFORMAT_BGR24) {
    printf("Loaded bgr24 golden %s\n", filename);
    uint8_t* src = (uint8_t*)golden_surface->pixels;
    (void)src;
    for (int y = 0; y < 144; y++) {
      for (int x = 0; x < 160; x++) {
        uint8_t a = src[x * 3 + y * golden_surface->pitch];

        if (a < 40) a = 3;
        else if (a < 128) a = 2;
        else if (a < 210) a = 1;
        else a = 0;

        golden_u8[x + y * 160] = a;
      }
    }
  }

  has_golden = true;
  show_diff = true;
}

//-----------------------------------------------------------------------------
