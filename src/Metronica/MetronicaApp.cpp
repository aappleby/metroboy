#include "Metronica/MetronicaApp.h"

#include "AppLib/AppHost.h"
#include "AppLib/Audio.h"
#include "AppLib/GLBase.h"
#include "CoreLib/Constants.h"
#include "CoreLib/Debug.h" // for StringDumper
#include "CoreLib/Tests.h"
#include "GateBoyLib/GateBoyDumper.h"
#include "GateBoyLib/GateBoyState.h"
#include "GateBoyLib/LogicBoy.h"
#include "GateBoyLib/Utils.h"
#include "MetroBoyLib/MetroBoySPU.h"

#define SDL_MAIN_HANDLED
#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#include <windows.h>
#else
#include <SDL2/SDL.h>
#endif

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

uint64_t phases_per_second = 114 * 154 * 60 * 8; // 8426880

const uint64_t gb_phase_total = 46880719;

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
#ifdef _MSC_VER
  SetPriorityClass(GetCurrentProcess(), 0x00000080);
#endif

  printf("%d %d\n", (int)sizeof(GateBoyState), (int)sizeof(LogicBoyState));

  App* app = new MetronicaApp();
  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
}

//-----------------------------------------------------------------------------

struct MusicEvent {
  uint64_t phase;
  uint32_t addr;
  uint32_t data;
};

std::vector<MusicEvent> music;
int music_cursor = 0;

MetroBoySPU spu;

void MetronicaApp::app_init(int screen_w, int screen_h) {
  dvec2 screen_size(screen_w, screen_h);

  LOG_G("MetronicaApp::app_init()\n");
  LOG_INDENT();

  counter_start = SDL_GetPerformanceCounter();
  counter_per_second = SDL_GetPerformanceFrequency();
  counter_old = counter_start;
  counter_new = counter_start;
  time_bucket = 0;
  phase_old = gb_phase_total;
  phase_new = gb_phase_total;

  audio_init();

  blob music_blob;
  load_blob("audio_test.txt", music_blob);

  printf("Music blob size %d\n", (int)music_blob.size());
  music_blob.push_back(0);

  const char* cursor = (const char*)music_blob.data();

  while(*cursor) {
    MusicEvent event;
    const char* format = "0x%" SCNx64 " 0x%04x 0x%02x";
    int blah = sscanf(cursor, format, &event.phase, &event.addr, &event.data);
    //printf("0x%08x 0x%04x 0x%02x\n", event.phase, event.addr, event.data);
    while(*cursor != '\n' && *cursor != 0) cursor++;
    if (*cursor == '\n') cursor++;
    music.push_back(event);
  }
  // sentinel
  music.push_back({uint64_t(-1), 0, 0});
  
  spu.reset_to_cart();

  view_control.init(screen_size);

  grid_painter.init(65536, 65536);
  text_painter.init();
  dump_painter.init_ascii();
  gb_blitter.init();
  blitter.init();

  ram_tex = create_texture_u8(256, 256, nullptr, false);
  keyboard_state = SDL_GetKeyboardState(nullptr);

  LOG_DEDENT();
  LOG_G("MetronicaApp::app_init() done\n");
}

//-----------------------------------------------------------------------------

void MetronicaApp::app_close() {
  audio_stop();
}

//-----------------------------------------------------------------------------

bool loud = false;

void MetronicaApp::app_update(dvec2 screen_size, double delta) {
  (void)delta;

  frame_begin = timestamp();

  counter_old = counter_new;
  counter_new = SDL_GetPerformanceCounter();
  time_bucket += (counter_new - counter_old) * phases_per_second;
  phase_old = phase_new;
  phase_new = phase_new + (time_bucket / counter_per_second);
  time_bucket %= counter_per_second;

  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEMOTION) {
      if (event.motion.state & SDL_BUTTON_LMASK) {
        view_control.pan(dvec2(event.motion.xrel, event.motion.yrel));
      }
    }

    if (event.type == SDL_MOUSEWHEEL) {
      int mouse_x = 0, mouse_y = 0;
      SDL_GetMouseState(&mouse_x, &mouse_y);
      view_control.on_mouse_wheel(dvec2(mouse_x, mouse_y), screen_size, double(event.wheel.y) * 0.25);
    }

    if (event.type == SDL_KEYDOWN)
    switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        view_control.view_target      = Viewport::screenspace(screen_size);
        view_control.view_target_snap = Viewport::screenspace(screen_size);
        break;

      case SDLK_SPACE: {
        app_paused = !app_paused;
        loud = !loud;
        break;
      }
    }
  }

  //----------------------------------------
  // Button input

  uint8_t buttons = 0;
  if (keyboard_state[SDL_SCANCODE_L])      buttons |= 0x01; // RIGHT
  if (keyboard_state[SDL_SCANCODE_J])      buttons |= 0x02; // LEFT
  if (keyboard_state[SDL_SCANCODE_I])      buttons |= 0x04; // UP
  if (keyboard_state[SDL_SCANCODE_K])      buttons |= 0x08; // DOWN

  if (keyboard_state[SDL_SCANCODE_KP_6])   buttons |= 0x01; // RIGHT
  if (keyboard_state[SDL_SCANCODE_KP_4])   buttons |= 0x02; // LEFT
  if (keyboard_state[SDL_SCANCODE_KP_8])   buttons |= 0x04; // UP
  if (keyboard_state[SDL_SCANCODE_KP_2])   buttons |= 0x08; // DOWN

  if (keyboard_state[SDL_SCANCODE_X])      buttons |= 0x10; // A
  if (keyboard_state[SDL_SCANCODE_Z])      buttons |= 0x20; // B
  if (keyboard_state[SDL_SCANCODE_RSHIFT]) buttons |= 0x40; // SELECT
  if (keyboard_state[SDL_SCANCODE_RETURN]) buttons |= 0x80; // START

  view_control.update(delta);

  //----------------------------------------
  // Run sim

  for (int i = phase_old; i < phase_new; i++) {
    if (music[music_cursor].phase == i) {
      auto& m = music[music_cursor];
      //printf("0x%08lx 0x%04x 0x%02x\n", m.phase, m.addr, m.data);
      music_cursor++;
    }

    if ((i & 7) == 0) {
      if (loud) {
        audio_post(rand() & 63, rand() & 63);
      }
      else {
        audio_post(rand() & 15, rand() & 15);
      }
    }
  }
}

//-----------------------------------------------------------------------------

void MetronicaApp::app_render_frame(dvec2 screen_size, double /*delta*/) {
  auto& view = view_control.view_smooth_snap;
  grid_painter.render(view, screen_size);

  for (int y = 0; y < 48; y++) {
    text_painter.dprintf("0x%04x    :", 0xFF10 + y);
    text_painter.render(view, screen_size, 0, y * 12);
  }

  for (int i = 0; i < music.size(); i++) {
    auto e = music[i];
    text_painter.dprintf("%02x", e.data);

    auto phase_delta = e.phase - phase_new;
    text_painter.fg_pal = (phase_delta < 800000) ? 2 : 1;
    text_painter.render(view, screen_size, 64 + double(e.phase - phase_new) / phases_per_second * 256.0, (e.addr - 0xFF10) * 12);
  }

  {
    StringDumper d;
    spu.dump(d);
    text_painter.render_string(view, screen_size, d.s, 256, 512);
  }

  /*
  uint8_t buf[256*256];
  for (int i = 0; i < 65536; i++) {
    buf[i] = i + (rand() % 32);
  }
  update_texture_u8(ram_tex, 0x00, 0x00, 256, 256,  buf);

  blitter.blit_mono(view, screen_size,
    ram_tex, 256, 256,
    0, 0, 256, 256,
    32*40, 784, 256, 256);
  */

  frame_count++;
  frame_end = timestamp();
  frame_time = frame_end - frame_begin;
  frame_time_smooth = frame_time_smooth * 0.99 + frame_time * 0.01;
}

//-----------------------------------------------------------------------------
