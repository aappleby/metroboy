#include "GateBoyApp/GateBoyApp.h"

#include "CoreLib/Constants.h"
#include "CoreLib/Debug.h"
#include "CoreLib/File.h"

#include "AppLib/AppHost.h"
#include "AppLib/GLBase.h"

#include "GateBoyLib/Probe.h"

#define SDL_MAIN_HANDLED
#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

int main(int argc, char** argv) {
  App* app = new GateBoyApp();
  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
}

using namespace Schematics;

//-----------------------------------------------------------------------------

GateBoyApp::GateBoyApp() {
  auto top_step = [this](GateBoy* gateboy) { 
    gateboy->next_phase();
  };
  auto top_unstep = [this](GateBoy* gateboy) {
    // Run a logic pass after unstep to update our probes
    int old_phase = (gateboy->phase_total - 1) & 7;
    int new_phase = (gateboy->phase_total - 0) & 7;
    gateboy->next_pass(old_phase, new_phase);
  };
  state_manager.init(top_step, top_unstep);
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
  overlay_tex = create_texture_u32(160, 144);
  keyboard_state = SDL_GetKeyboardState(nullptr);

  auto& gb = *state_manager.state();
  gb.cpu.reset(0x0000);
  gb.reset();

  gb.set_boot_bit();

  const char* filename = "roms/LinksAwakening_dog.dump";
  gb.load_dump(filename);

  //gb.load_rom("microtests/build/dmg/cpu_bus_1.gb");

#if 0
  // ld (hl),a; jr -2;
  gb.mem[0x0155] = 0x77;
  gb.mem[0x0156] = 0x18;
  gb.mem[0x0157] = 0xFD;

  gb.script = new Req[] {
    { .addr = 0x0155, .data = 0x00, .read = 1, .write = 0}, // read "ld (hl), a" opcode
    { .addr = 0xC003, .data = 0x00, .read = 0, .write = 1}, // write to 0xC003
    { .addr = 0x0156, .data = 0x00, .read = 1, .write = 0}, // read "jr -2" opcode
    { .addr = 0x0157, .data = 0x00, .read = 1, .write = 0}, // read "jr -2" param
    { .addr = 0x0157, .data = 0x00, .read = 0, .write = 0}, // idle cycle
  };
  gb.script_len = 5;
  gb.phase_total = 0;
#endif

  //gb.cpu_en = true;

  //gb.sys_buttons = 0x0F;
  //gb.sys_buttons = 0x00;
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
  float col_width = 256 - 32;
  float cursor = 0;

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
  dumper("dbg_req ");
  dump_req(dumper, gateboy->dbg_req);
  dumper("cpu_req ");
  dump_req(dumper, gateboy->cpu_req);

  dumper("\n");

  dumper("----------   CPU    ----------\n");
  gateboy->cpu.dump(dumper);
  top.tim_reg.dump(dumper);
  top.int_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += col_width;
  dumper.clear();

  wire CLK = gateboy->phase_total & 1;

  top.clk_reg.dump(dumper, CLK);
  top.joypad.dump(dumper);
  top.ser_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += col_width;
  dumper.clear();

  top.cpu_bus.dump(dumper);
  top.ext_bus.dump(dumper);
  top.vram_bus.dump(dumper, top);
  top.oam_bus.dump(dumper);
  top.dma_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += col_width;
  dumper.clear();

  top.lcd_reg.dump(dumper, top);
  top.pix_pipe.dump(dumper, top);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += col_width;
  dumper.clear();

  /*
  dump_probes(dumper);
  top.sprite_fetcher.dump(dumper);
  top.sprite_scanner.dump(dumper, top);
  top.sprite_store.dump(dumper);
  top.tile_fetcher.dump(dumper, top);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += col_width;
  dumper.clear();
  */

  //dump_painter.render(view, cursor, 512,      16, 16, DMG_ROM_bin);

  //dump_painter.render(view, cursor, 512, 16, 16, gateboy->oam_ram);
  //dump_painter.render(view, cursor, 768, 16,  8, gateboy->zero_ram);

  //dump_painter.render(view, col_width * 4 + 128, 0, 4, 64, gateboy->mem + 0x0000);

  gb_blitter.blit_screen(view, 1024, 32, 2, gateboy->fb);

  //update_texture_u32(trace_tex, 912, 154, trace);
  //blitter.blit(view, trace_tex, 0, 0, 912, 154);

  memset(overlay, 0, sizeof(overlay));

  int fb_y = top.lcd_reg.get_y();
  if (fb_y >= 0 && fb_y < 144) {
    for (int i = 0; i < 160; i++) {
      overlay[i + fb_y * 160] = 0x33FFFF00;
    }

    int fb_x = top.pix_pipe.get_pix_count() - 8;
    if (fb_x >= 0 && fb_x < 160 && fb_y >= 0 && fb_y < 144) {
      overlay[fb_x + fb_y * 160] = 0x8000FFFF;
    }
  }

  update_texture_u32(overlay_tex, 160, 144, overlay);
  blitter.blit(view, overlay_tex, 1024, 32, 160 * 2, 144 * 2);

  gb_blitter.blit_tiles (view, 1376, 32,     gateboy->vid_ram);
  gb_blitter.blit_map   (view, 1376, 448, 1, gateboy->vid_ram, 0, 1);
  gb_blitter.blit_map   (view, 1376, 736, 1, gateboy->vid_ram, 1, 1);
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
