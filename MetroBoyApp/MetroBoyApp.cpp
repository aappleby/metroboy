#include "MetroBoyApp/MetroBoyApp.h"

#include "AppLib/AppHost.h"
#include "AppLib/Audio.h"
#include "AppLib/GLBase.h"
#include "CoreLib/Debug.h"

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {

  //App* app = new GateBoyApp();
  //App* app = new DummyApp();
  App* app = new MetroBoyApp();

  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
}


#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

//-----------------------------------------------------------------------------

const char* MetroBoyApp::app_get_title() {
  return "MetroBoy";
}

//-----------------------------------------------------------------------------

void MetroBoyApp::app_init(int /*screen_w*/, int /*screen_h*/) {
  app_start = timestamp();

  blitter.init();
  grid_painter.init();
  text_painter.init();

  keyboard_state = SDL_GetKeyboardState(nullptr);
  //audio_init();

  gb_tex = create_texture_u8(160, 144);
  trace_tex = create_texture_u32(456, 154);
  ram_tex = create_texture_u8(256, 256);

  gb_blitter.init();
  dump_painter.init();
  load_rom("roms/tetris.gb");

  //load_rom("roms/gb-test-roms/cpu_instrs/individual/01-special.gb");            // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/02-interrupts.gb");         // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/03-op sp,hl.gb");           // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/04-op r,imm.gb");           // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/05-op rp.gb");              // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/06-ld r,r.gb");             // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb"); // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/08-misc instrs.gb");        // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/09-op r,r.gb");             // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/10-bit ops.gb");            // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/11-op a,(hl).gb");          // pass
  load_rom("roms/cpu_instrs.gb"); // pass

  //load_rom("microtests/build/dmg/timer_int_halt_a.gb");
  //load_rom("microtests/build/dmg/halt_bug.gb");

  //load_rom("microtests/build/dmg/hblank_int_if_a.gb");
  //load_rom("microtests/build/dmg/hblank_int_if_b.gb");

  //load_rom("microtests/build/dmg/int_hblank_halt_scx1.gb");

  //load_rom("microtests/build/dmg/flood_vram.gb");

  //load_rom("roms/oh.gb"); // broken eye
  //load_rom("roms/pocket.gb");
  //load_rom("roms/gejmboj.gb");
  //load_rom("roms/LinksAwakening");
  //load_rom("roms/Prehistorik Man (U).gb");
  //load_rom("roms/SML.gb");

  //runmode = RUN_FAST;

  //load_memdump("roms", "LinksAwakening_house");
  //load_memdump("roms", "LinksAwakening_dog");
};

//----------------------------------------

void MetroBoyApp::app_close() {
  //audio_stop();
};

//-----------------------------------------------------------------------------

void MetroBoyApp::load_memdump(const std::string& prefix, const std::string& name) {
  load_rom("microtests/build/dmg", "minimal");

  std::string filename = prefix + "/" + name + ".dump";
  blob buf;
  load_array(filename, buf);

  memcpy(gb->vram.ram, &buf[0x8000], 8192);

  gb->ppu.reg_lcdc = buf[0xFF40];
  gb->ppu.stat = buf[0xFF41];
  gb->ppu.scy  = buf[0xFF42];
  gb->ppu.scx  = buf[0xFF43];
  gb->ppu.ly   = buf[0xFF44];
  gb->ppu.reg_lyc  = buf[0xFF45];
  //metroboy->ppu.dma  = buf[0xFF46];
  gb->ppu.bgp  = buf[0xFF47];
  gb->ppu.obp0 = buf[0xFF48];
  gb->ppu.obp1 = buf[0xFF49];
  gb->ppu.wy   = buf[0xFF4A];
  gb->ppu.wx   = buf[0xFF4B];
  gb->ppu.update_palettes();

  uint8_t* oam_flat = (uint8_t*)gb->oam.ram;

  for (int i = 0; i < 256; i++) {
    oam_flat[i] = buf[ADDR_OAM_BEGIN + i];
  }
}

//-----------------------------------------------------------------------------

void MetroBoyApp::load_rom(const std::string& prefix, const std::string& name) {
  std::string gb_filename = prefix + "/" + name;
  printf("Loading rom %s\n", gb_filename.c_str());

  load_array(gb_filename.c_str(), rom);

  gb.clear_history();
  gb->reset_to_cart(rom.data(), rom.size());

  rom_loaded = true;
  runmode = RUN_STEP;
}

//-----------------------------------------------------------------------------

void MetroBoyApp::app_update(double /*delta*/) {
  int  step_forward = 0;
  int  step_backward = 0;

  //----------------------------------------
  // Handle keys

  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_KEYDOWN) switch (event.key.keysym.sym) {
    case SDLK_f:      runmode = RUN_FAST; break;
    case SDLK_v:      runmode = RUN_SYNC; break;
    case SDLK_s:      runmode = RUN_STEP; break;
    case SDLK_o:      overlay_mode = (overlay_mode + 1) % 3; break;
    //case SDLK_UP:     stepsize = clamp_val(stepsize + 1, STEP_MIN, STEP_MAX); break;
    //case SDLK_DOWN:   stepsize = clamp_val(stepsize - 1, STEP_MIN, STEP_MAX); break;

    case SDLK_r: {
      gb.clear_history();
      gb->reset_to_cart(rom.data(), rom.size());
      break;
    }
    case SDLK_F1: {
      load_obj("dump.MetroBoy", *gb.top());
      rom_loaded = true;
      break;
    }
    case SDLK_F4: {
      save_obj("dump.MetroBoy", *gb.top());
      break;
    }

    case SDLK_RIGHT:  {
      step_forward = keyboard_state[SDL_SCANCODE_LCTRL] ? 8 : 1;
      break;
    }

    case SDLK_LEFT: {
      step_backward = keyboard_state[SDL_SCANCODE_LCTRL] ? 8 : 1;
      break;
    }
    }

    if (event.type == SDL_DROPFILE) {
      load_array(event.drop.file, rom);
      gb.clear_history();
      gb->reset_to_cart(rom.data(), rom.size());
      rom_loaded = true;
      runmode = RUN_SYNC;
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

  //----------------------------------------
  // Run simulation

  double sim_begin = timestamp();
  int64_t phase_begin = gb->phase_total;

  if (runmode == RUN_FAST) {
    gb.clear_history();
    gb->joy.set(~buttons);
    step_cycle(MCYCLES_PER_FRAME * 8);
  }
  else if (runmode == RUN_SYNC) {
    gb.clear_history();
    gb->joy.set(~buttons);
    sync_to_vblank();

    //audio_begin();
    for (int i = 0; i < MCYCLES_PER_FRAME; i++) {
      step_cycle(1);
      //audio_post(gb->get_host_data().out_l, gb->get_host_data().out_r);
    }
    //audio_end();
  }
  else if (runmode == RUN_STEP) {
    if (step_forward) {
      gb.push();
      step_phase(step_forward);
    }
    if (step_backward) {
      gb.pop();
    }
  }

  double sim_end = timestamp();
  int64_t phase_end = gb->phase_total;
  sim_time = sim_end - sim_begin;
  sim_rate = (phase_end - phase_begin) / sim_time;
}

//-----------------------------------------------------------------------------

void MetroBoyApp::app_render_frame() {
  Viewport view = Viewport::screenspace(1920, 1080);

  grid_painter.render(view);

  //----------------------------------------
  // Flat memory view

  update_texture_u8(ram_tex, 0x00, 0x00, 256, 128, rom.data());
  update_texture_u8(ram_tex, 0x00, 0x80, 256,  32, gb->vram.ram);
  update_texture_u8(ram_tex, 0x00, 0xA0, 256,  32, gb->cart.cart_ram);
  update_texture_u8(ram_tex, 0x00, 0xC0, 256,  32, gb->cart.main_ram);
  update_texture_u8(ram_tex, 0x00, 0xFE, 256,   1, gb->oam.ram);
  update_texture_u8(ram_tex, 0x80, 0xFF, 128,   1, gb->zram.ram);

  blitter.blit_mono(view, ram_tex, 256, 256,
                    0, 0, 256, 256,
                    960 + 96, 640 + 96, 256, 256);

  //----------------------------------------
  // Gameboy screen

  //gb_blitter.blit_map   (view, 1280, 32,  2, gb->vram.ram, 0, 0);

  gb_blitter.blit_screen(view, 1280, 32,  2, gb->framebuffer);
  gb_blitter.blit_tiles (view, 1632, 32,  1, gb->vram.ram);
  gb_blitter.blit_map   (view, 1344, 448, 1, gb->vram.ram, 0, 0);
  gb_blitter.blit_map   (view, 1632, 448, 1, gb->vram.ram, 0, 1);
  gb_blitter.blit_map   (view, 1344, 736, 1, gb->vram.ram, 1, 0);
  gb_blitter.blit_map   (view, 1632, 736, 1, gb->vram.ram, 1, 1);

  //----------------------------------------
  // Trace view

  //const int gb_trace_x = int(view.sx()) - 32 * 17;
  //const int gb_trace_y = int(view.sy()) - 32 * 7;
  //gb_blitter.blit_trace (view, gb_trace_x, gb_trace_y, metroboy.get_trace());

  //----------------------------------------
  // Memory debugger

  //dump_painter.render(view, 900, 100, 16, 8, metroboy->get_zram());
  //dump_painter.render(view, 900, 300, 64, 128, metroboy->get_main_ram());
}

//-----------------------------------------------------------------------------

void MetroBoyApp::app_render_ui() {
  Viewport view = Viewport::screenspace(1920, 1080);

  StringDumper d;

  int column = 0;
  if (1) {
    gb->dump_bus(d);
    gb->cpu.dump(d);
    gb->ints.dump(d);
    gb->timer.dump(d);
    gb->dma.dump(d);
    gb->cart.dump(d);
    gb->joy.dump(d);
    gb->serial.dump(d);
    text_painter.render_string(view, d.s, (float)column, 0);
    d.clear();
    column += 32 * 7;
  }

  if (1) {
    gb->spu.dump(d);
    gb->zram.dump(d);
    gb->oam.dump(d);
    text_painter.render_string(view, d.s, (float)column, 0);
    d.clear();
    column += 32 * 7;
  }

  if (1) {
    gb->ppu.dump(d);
    text_painter.render_string(view, d.s, (float)column, 0);
    d.clear();
    column += 32 * 7;
  }

  if (1) {
    gb->dump_disasm(d);
    text_painter.render_string(view, d.s, (float)column, 0);
    d.clear();
    column += 32 * 7;
  }

  text_painter.dprintf("%s\n", runmode_names[runmode]);

  size_t state_size = gb.state_size_bytes();
  if (state_size < 1024 * 1024) {
    text_painter.dprintf("State size %d K\n", state_size / 1024);
  }
  else {
    text_painter.dprintf("State size %d M\n", state_size / (1024 * 1024));
  }

  double phases_per_frame = 114 * 154 * 60 * 8;
  double sim_ratio = sim_rate / phases_per_frame;

  text_painter.dprintf("Sim time %f, sim ratio %f\n", sim_time, sim_ratio);
  text_painter.dprintf("Frame time %f\n", frame_time);
  text_painter.render_buf(view, float(view.screen_size.x - 320), float(view.screen_size.y - 64));


    /*
  double mcycles_per_sec_1x = 114 * 154 * 60; // 1.05 mhz
  double sim_speed = 1.06;
  double sim_budget_msec = 60.0;
  double fast_mcycles = mcycles_per_sec_1x * sim_speed * sim_budget_msec / 1000.0;
  int last_mcycles = 0;
  double sim_time_msec = 0.0;

  {
    double sim_mcycles_per_sec = double(last_mcycles) * (1000.0 / sim_budget_msec);
    double rt_mcycles_per_sec = 114.0 * 154.0 * 60.0;

    const char* mode_names[] = {
        "RUN_FAST", "RUN_VSYNC", "STEP_FRAME", "STEP_LINE", "STEP_CYCLE",
    };

    text_painter.dprintf("%s %d\n", mode_names[runmode], (int)(gb->phase_total & 7));
    text_painter.dprintf("sim budget %2.2f msec/frame\n", sim_budget_msec);
    text_painter.dprintf("sim time   %02.2f msec/frame\n", sim_time_msec);
    text_painter.dprintf("sim rate   %7d cycles/frame\n", last_mcycles);
    text_painter.dprintf("sim speed  %1.2fx realtime\n", sim_mcycles_per_sec / rt_mcycles_per_sec);

    text_painter.render(view, float(view.screen_size.x - 300 + 96), float(view.screen_size.y - 64));
  }
  */
}

//-----------------------------------------------------------------------------





#if 0
void MetroBoy::step_over() {
  push_cycle();

  auto& gb = current->gb;

  int op_addr = gb.get_cpu().get_op_addr();
  int op = get_rom()[op_addr];
  int op_size = op_sizes[op];
  if (op == 0xcb) op_size = 2;

  int next_op_addr = op_addr + op_size;

  gb.sync_to_mcycle();
  int i = 0;
  for (; i < 1000000; i++) {
    if (gb.get_cpu().get_op_addr() == next_op_addr) {
      // step succeeded
      return;
    }
    gb.mcycle();
  }

  // step failed
  pop_cycle();
  return;
}
#endif


#if 0
  //----------------------------------------
  // Wave thingy

  for (int i = 0; i < 16; i++) {
    uint8_t a = (gameboy.spu.s3_wave[i] & 0x0F) >> 0;
    uint8_t b = (gameboy.spu.s3_wave[i] & 0xF0) >> 4;
    uint32_t color = 0xFFFFFFFF;

    framebuffer[(512 + 2 * i + 0) + (100 + b) * fb_width] = color;
    framebuffer[(512 + 2 * i + 1) + (100 + a) * fb_width] = color;
  }
#endif




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
  // Screen overlay

  {
    memset(overlay, 0, sizeof(overlay));

    int fb_y = top.lcd_reg.get_ly();
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
    blitter.blit(view, overlay_tex, 1024 + 256, 32, 160 * 2, 144 * 2);
  }
#endif
