#include "GateBoyApp.h"
#include "GateBoyTests.h"

#include "File.h"
#include "Debug.h"
#include "BusDump.h"
#include "Probe.h"
#include "GLBase.h"
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
    gateboy->next_phase();
  };
  auto top_unstep = [this](GateBoy* gateboy) {
    // Run a logic pass after unstep to update our probes
    gateboy->next_pass();
  };
  state_manager.init(top_step, top_unstep);

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

const char* GateBoyApp::app_get_title() {
  return "GateBoyApp";
}

//----------------------------------------

void GateBoyApp::app_init() {
  printf("GateBoyApp::app_init()\n");

  grid_painter.init();
  text_painter.init();
  dump_painter.init();
  gb_blitter.init();
  blitter.init();

  trace_tex = create_texture_u32(912, 154);

  keyboard_state = SDL_GetKeyboardState(nullptr);
  printf("\n");

  GateBoyTests::test_all_mem();

  auto gateboy = state_manager.state();
  gateboy->reset();

  //gateboy->run_bootrom();

#if !_DEBUG
  /*
  gateboy->test_all_mem();
  gateboy->test_bootrom();
  gateboy->test_timer();
  gateboy->test_joypad();
  gateboy->test_dma();
  gateboy->test_serial();
  gateboy->test_ppu();
  gateboy->test_interrupts();
  */
#endif

  //const char* filename = "roms/LinksAwakening_dog.dump";
  //gateboy->load(filename);
}

void GateBoyApp::app_close() {
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_update(double delta) {
  (void)delta;

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
        step_forward += 114 * 8;
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

  frame_count++;
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_render_frame(Viewport view) {
  grid_painter.render(view);

  auto gateboy = state_manager.state();
  const auto& top = gateboy->top;

  StringDumper dumper;
  float col_width = 256;
  float cursor_y = 0;

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
  dumper("Phase count %8d\n",    gateboy->phase_total);
  dumper("Pass count  %8d\n",    gateboy->pass_count);
  dumper("Pass total  %8d\n",    gateboy->pass_total);
  dumper("Pass avg    %4.2f\n",  float(gateboy->pass_total) / float(gateboy->phase_total));
  dumper("Phase hash  %016llx\n", gateboy->phase_hash);
  dumper("Total hash  %016llx\n", gateboy->total_hash);
  dumper("\n");

  dumper("----------   CPU    ----------\n");
  gateboy->cpu.dump(dumper);
  top.tim_reg.dump(dumper);
  top.int_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor_y, 0);
  cursor_y += col_width;
  dumper.clear();

  top.clk_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor_y, 0);
  cursor_y += col_width;
  dumper.clear();

  top.cpu_bus.dump(dumper);
  top.ext_bus.dump(dumper);
  top.vram_bus.dump(dumper, top);
  top.oam_bus.dump(dumper);
  top.dma_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor_y, 0);
  cursor_y += col_width;
  dumper.clear();

  top.lcd_reg.dump(dumper, top);
  top.pix_pipe.dump(dumper, top);
  text_painter.render(view, dumper.s.c_str(), cursor_y, 0);
  cursor_y += col_width;
  dumper.clear();

  dump_probes(dumper);
  top.sprite_fetcher.dump(dumper);
  top.sprite_scanner.dump(dumper, top);
  top.sprite_store.dump(dumper);
  top.tile_fetcher.dump(dumper, top);
  top.joypad.dump(dumper);
  top.ser_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor_y, 0);
  cursor_y += col_width;
  dumper.clear();

  /*
  dump_painter.render(view, col_width * 4,       0, 4, 64, gateboy->mem + 0xFE00);
  dump_painter.render(view, col_width * 4 + 128, 0, 4, 64, gateboy->mem + 0x0000);

  dump_bus_dump(dumper, poweron_004_div, replay_cursor, 3200);
  text_painter.render(view, dumper.s.c_str(), col_width * 5, 0);
  dumper.clear();
  */

  gb_blitter.blit_screen(view, int(view.screen_size.x - 320 - 32), 32, 2, gateboy->fb);

  /*
  for (int i = 0; i < 912 * 154; i++) {
    trace[i] = rand() | 0xFF000000;
  }
  */

  //update_texture_u32(trace_tex, 912, 154, trace);
  //blitter.blit(view, trace_tex, 0, 0, 912, 154);

}

//-----------------------------------------------------------------------------

void GateBoyApp::app_render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
