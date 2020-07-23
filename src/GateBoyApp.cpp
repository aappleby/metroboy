#include "GateBoyApp.h"

#include "File.h"
#include "Debug.h"
#include "BusDump.h"
#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

extern BusDump poweron_004_div[];

using namespace Schematics;

#pragma warning(disable:4702)

//-----------------------------------------------------------------------------

GateBoyApp::GateBoyApp() {
  auto top_step = [this](GateBoy* gateboy) { 
    bool verbose = false;
    bool use_fast_impl = true;
    gateboy->run(1, get_req(), verbose, use_fast_impl);
  };
  state_manager.init(top_step);

  auto gateboy = state_manager.state();
  load_blob("microtests/build/dmg/poweron_000_div.gb", gateboy->mem, 65536);
}

GateBoyApp::~GateBoyApp() {
}


//-----------------------------------------------------------------------------

void GateBoyApp::reset(uint16_t /*new_pc*/) {
  state_manager.reset();
}

//-----------------------------------------------------------------------------

Req GateBoyApp::get_req() {
  Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };
  return req;
}

//-----------------------------------------------------------------------------

const char* GateBoyApp::app_get_title() {
  return "GateBoyApp";
}

//----------------------------------------

void GateBoyApp::app_init() {
  grid_painter.init();
  text_painter.init();
  dump_painter.init();
  keyboard_state = SDL_GetKeyboardState(nullptr);

  auto gateboy = state_manager.state();

  bool verbose = false;
  bool use_fast_impl = true;
  gateboy->run_reset_sequence(verbose, use_fast_impl);
}

void GateBoyApp::app_close() {
}

void GateBoyApp::app_update(double delta) {
  SDL_Event event;

  int  step_forward = 0;
  int  step_backward = 0;
  bool step_up = false;
  bool step_down = false;
  bool load_dump = false;
  bool save_dump = false;
  bool reset_sim = false;

  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_KEYDOWN) switch (event.key.keysym.sym) {
    case SDLK_f:      runmode = RUN_FAST; break;
    case SDLK_v:      runmode = RUN_VSYNC; break;
    case SDLK_s:      runmode = STEP_FRAME; break;
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

    case SDLK_r:      reset_sim = true; break;
    case SDLK_F1:     load_dump = true; break;
    case SDLK_F4:     save_dump = true; break;
    }
  }

  while(step_forward--) {
    state_manager.step(1);
  }

  while(step_backward--) {
    state_manager.unstep(1);
  }

  //get_top()->phase_count += step_forward;
  //get_top()->phase_count -= step_backward;

  frame_count++;
  (void)delta;
}

void GateBoyApp::app_render_frame(Viewport view) {
  grid_painter.render(view);

  auto gateboy = state_manager.state();

  StringDumper dumper;
  float col_width = 224;

  dumper("----------   Top    ----------\n");

  size_t state_size = state_manager.state_size_bytes();

  if (state_size < 1024 * 1024) {
    dumper("State size          %d K\n",      state_size / 1024);
  }
  else {
    dumper("State size          %d M\n",      state_size / (1024 * 1024));
  }
  dumper("Phase count %05d\n",    gateboy->phase_total);
  dumper("Pass count  %05d\n",    gateboy->pass_total);
  dumper("Phase hash  %016llx\n", gateboy->phase_hash);
  dumper("Total hash  %016llx\n", gateboy->total_hash);
  dumper("\n");

  gateboy->top.clk_reg.dump(dumper);
  gateboy->top.cpu_bus.dump(dumper);
  gateboy->top.ext_bus.dump(dumper);
  gateboy->top.vram_bus.dump(dumper);
  gateboy->top.oam_bus.dump(dumper);

  text_painter.render(view, dumper.s.c_str(), col_width * 0, 0);
  dumper.clear();

  gateboy->top.dma_reg.dump(dumper);
  gateboy->top.int_reg.dump(dumper);
  gateboy->top.joypad.dump(dumper);
  gateboy->top.lcd_reg.dump(dumper);

  text_painter.render(view, dumper.s.c_str(), col_width * 1, 0);
  dumper.clear();

  gateboy->top.pix_pipe.dump(dumper);
  gateboy->top.sprite_fetcher.dump(dumper);
  gateboy->top.sprite_scanner.dump(dumper);
  gateboy->top.sprite_store.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), col_width * 2, 0);
  dumper.clear();

  gateboy->top.tile_fetcher.dump(dumper);
  gateboy->top.tim_reg.dump(dumper);
  gateboy->top.ser_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), col_width * 3, 0);
  dumper.clear();

  /*
  int replay_time = frame_count / 20;
  BusDump* dump = poweron_004_div;

  while(dump[replay_cursor].phase != -1 && dump[replay_cursor].phase < replay_time) {
    replay_cursor++;
  }
  */

  dump_bus_dump(dumper, poweron_004_div, replay_cursor, 3200);
  text_painter.render(view, dumper.s.c_str(), col_width * 6, 64);
  dumper.clear();

  dump_painter.render(view, 1024, 512, 16, 32, gateboy->mem);
}

void GateBoyApp::app_render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
