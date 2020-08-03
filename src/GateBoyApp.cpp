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

  //auto gateboy = state_manager.state();
  //load_blob("microtests/build/dmg/poweron_000_div.gb", gateboy->mem, 65536);
}

GateBoyApp::~GateBoyApp() {
}


//-----------------------------------------------------------------------------

void GateBoyApp::reset(uint16_t /*new_pc*/) {
  state_manager.reset();
}

//-----------------------------------------------------------------------------

Req GateBoyApp::get_req() {
  //Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };
  //Req req = {.addr = 0x0100, .data = 0, .read = 1, .write = 0 };

  /*
  auto gateboy = state_manager.state();
  uint16_t addr = (uint16_t)((gateboy->phase_total - 87) >> 3);
  Req req = {.addr = addr, .data = 0, .read = 1, .write = 0 };
  */

  Req req = {.addr = 0, .data = 0, .read = 0, .write = 0};
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

  printf("\n");
  printf("//----------------------------------------\n");
  printf("// GateBoy starting...\n");
  printf("\n");

  auto gateboy = state_manager.state();

  bool verbose = false;
  bool use_fast_impl = false;
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
      if (keyboard_state[SDL_SCANCODE_LCTRL] && keyboard_state[SDL_SCANCODE_LALT]) {
        step_forward += 16384;
      } else if (keyboard_state[SDL_SCANCODE_LALT]) {
        step_forward += 1024;
      } else if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        step_forward += 8;
      } else {
        step_forward += 1;
      }
      break;
    }
    case SDLK_LEFT:   {
      step_backward += 1;
      break;
    }
    case SDLK_UP:     step_up = true; break;
    case SDLK_DOWN:   step_down = true; break;

    case SDLK_r:      reset_sim = true; break;
    case SDLK_F1:     load_dump = true; break;
    case SDLK_F4:     save_dump = true; break;
    }
  }

  if (step_forward) {
    state_manager.step(step_forward);
    step_forward = 0;
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
  float col_width = 256;

  dumper("----------   Top    ----------\n");

  const char* phases[] = {
    "\002A_______\001",
    "\003_B______\001",
    "\002__C_____\001",
    "\003___D____\001",
    "\002____E___\001",
    "\003_____F__\001",
    "\002______G_\001",
    "\003_______H\001",
  };

  dumper("phase %s\n", phases[gateboy->phase_total & 7]);

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

  const auto& top = gateboy->top;

  top.clk_reg.dump(dumper, top);
  top.tim_reg.dump(dumper);
  top.int_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), col_width * 0, 0);
  dumper.clear();

  top.cpu_bus.dump(dumper);
  top.ext_bus.dump(dumper);
  top.vram_bus.dump(dumper, top);
  top.oam_bus.dump(dumper);
  top.dma_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), col_width * 1, 0);
  dumper.clear();

  top.lcd_reg.dump(dumper);
  top.pix_pipe.dump(dumper, top);
  text_painter.render(view, dumper.s.c_str(), col_width * 2, 0);
  dumper.clear();

  top.sprite_fetcher.dump(dumper);
  top.sprite_scanner.dump(dumper, top);
  top.sprite_store.dump(dumper);
  top.tile_fetcher.dump(dumper, top);
  top.joypad.dump(dumper);
  top.ser_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), col_width * 3, 0);
  dumper.clear();

  dump_painter.render(view, col_width * 4, 0, 4, 40, gateboy->mem + 0xFE00);

  dump_bus_dump(dumper, poweron_004_div, replay_cursor, 3200);
  text_painter.render(view, dumper.s.c_str(), col_width * 5, 0);
  dumper.clear();

}

void GateBoyApp::app_render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
