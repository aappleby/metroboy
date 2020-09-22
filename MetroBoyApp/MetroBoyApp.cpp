#include "MetroBoyApp/MetroBoyApp.h"

#include "AppLib/Audio.h"
#include "AppLib/GLBase.h"
#include "CoreLib/Debug.h"

#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
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
  double begin = timestamp();

  printf("\n");
  printf("---------- POST begin ----------\n");

  int err = 0;
#if 0
  printf("Timer\n");
  err += run_microtest("microtests/build/dmg/div_inc_timing_a.gb");
  err += run_microtest("microtests/build/dmg/div_inc_timing_b.gb");
  err += run_microtest("microtests/build/dmg/poweron_000_div.gb");
  err += run_microtest("microtests/build/dmg/poweron_004_div.gb");
  err += run_microtest("microtests/build/dmg/poweron_005_div.gb");
  err += run_microtest("microtests/build/dmg/timer_div_phase_c.gb");
  err += run_microtest("microtests/build/dmg/timer_div_phase_d.gb");
  printf("\n");
#endif

#if 0
  printf("Reset\n");
  err += run_microtest("micro_cpu/build/dmg/rst_0x00.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x08.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x10.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x18.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x20.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x28.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x30.gb");
  err += run_microtest("micro_cpu/build/dmg/rst_0x38.gb");
  printf("\n");
#endif
  if (err) exit(1);

  double end = timestamp();
  printf("---------- POST done in %f seconds ----------\n", (end - begin));
  printf("\n");
}

//-----------------------------------------------------------------------------

const char* MetroBoyApp::app_get_title() {
  return "MetroBoy";
}

//-----------------------------------------------------------------------------

void MetroBoyApp::app_init() {
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

  //load_rom("roms/cpu_instrs.gb");
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/01-special.gb");            // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/02-interrupts.gb");         // fail
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/03-op sp,hl.gb");           // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/04-op r,imm.gb");           // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/05-op rp.gb");              // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/06-ld r,r.gb");             // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb"); // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/08-misc instrs.gb");        // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/09-op r,r.gb");             // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/10-bit ops.gb");            // pass
  //load_rom("roms/gb-test-roms/cpu_instrs/individual/11-op a,(hl).gb");          // pass

  //load_rom("roms/oh.gb"); // broken eye
  //load_rom("roms/pocket.gb");
  //load_rom("roms/gejmboj.gb");
  //load_rom("roms/LinksAwakening");
  //load_rom("roms/Prehistorik Man (U).gb");
  //load_rom("roms/SML.gb");
  load_rom("roms/tetris.gb");

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

  memcpy(gb->get_vram(), &buf[0x8000], 8192);

  gb->ppu.lcdc = buf[0xFF40];
  gb->ppu.stat = buf[0xFF41];
  gb->ppu.scy  = buf[0xFF42];
  gb->ppu.scx  = buf[0xFF43];
  gb->ppu.ly   = buf[0xFF44];
  gb->ppu.lyc  = buf[0xFF45];
  //metroboy->ppu.dma  = buf[0xFF46];
  gb->ppu.bgp  = buf[0xFF47];
  gb->ppu.obp0 = buf[0xFF48];
  gb->ppu.obp1 = buf[0xFF49];
  gb->ppu.wy   = buf[0xFF4A];
  gb->ppu.wx   = buf[0xFF4B];
  gb->ppu.update_palettes();

  uint8_t* oam_flat = (uint8_t*)gb->oam.get();

  for (int i = 0; i < 256; i++) {
    oam_flat[i] = buf[ADDR_OAM_BEGIN + i];
  }
}

//-----------------------------------------------------------------------------

void MetroBoyApp::load_rom(const std::string& prefix, const std::string& name) {
  std::string gb_filename = prefix + "/" + name;
  printf("Loading rom %s\n", gb_filename.c_str());
  
  load_array(gb_filename.c_str(), rom);
  gb->set_rom(rom.data(), rom.size());
  gb->reset(0x0100);
  
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
      load_array(event.drop.file, rom);
      gb->set_rom(rom.data(), rom.size());
      gb->reset(0x0100);

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

  //----------------------------------------
  // Handle sim controls

  if (reset) {
    gb.clear();
    memset(tracebuffer, 0, sizeof(tracebuffer));
    gb->set_rom(rom.data(), rom.size());
    gb->reset(0x0100);
    reset = false;
  }

  if (load_dump) {
    load_obj("dump.MetroBoy", *gb.state());
    load_dump = false;
    rom_loaded = true;
  }

  if (save_dump) {
    save_obj("dump.MetroBoy", *gb.state());
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
    // 114 * 154 = 17556 mcycles/frame
    // Simulate 8 frames per real frame.

    gb.clear();
    gb->set_joypad(~buttons);
    gb->step_mcycle(114 * 154 * 8);
  }
  else if (runmode == RUN_VSYNC) {
    //printf("%d --------\n", frame_count);
    int64_t phase_begin = gb->phase_total;
    double time_begin = timestamp();

    //metroboy.run_vsync(buttons);

    gb.clear();
    gb->set_joypad(~buttons);
    gb->sync_to_mcycle();

    while(gb->get_host_data().y != 144) {
      gb->mcycle();
    }

    audio_begin();

    for (int i = 0; i < 154 * 114; i++) {
      gb->mcycle();
      //audio_post(current->gb.get_host_data().out_l,
      //           current->gb.get_host_data().out_r);
    }

    audio_end();

    double time_end = timestamp();
    int64 phase_end = gb->phase_total;
    last_mcycles = int(phase_end - phase_begin) / 4;
    sim_time_msec = 1000.0 * (time_end - time_begin);
  }
  else if (runmode == STEP_PHASE) {
    if (step_forward) {
      gb.push_cycle();
      gb->step_phase(step_forward);
    }
    if (step_backward) {
      gb.pop_cycle();
    }
  }
  else if (runmode == STEP_FRAME) {
    if (step_forward)  {
      gb.push_frame();
      gb->step_frame(step_forward);
    }
    if (step_backward) {
      gb.pop_frame();
    }
  }
  else if (runmode == STEP_LINE) {
    if (step_forward) {
      gb.push_line();
      gb->step_line(step_forward);
    }
    if (step_backward) {
      gb.pop_line();
    }
  }

  step_forward = 0;
  step_backward = 0;
  step_up = false;
  step_down = false;
}

/*
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
*/

//-----------------------------------------------------------------------------

void MetroBoyApp::app_render_frame(Viewport view) {

  grid_painter.render(view);

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
  // Flat memory view

  {
    update_texture_u8(ram_tex, 0x00, 0x00, 256, 128, gb->get_cart_rom());
    update_texture_u8(ram_tex, 0x00, 0x80, 256,  32, gb->get_vram());
    update_texture_u8(ram_tex, 0x00, 0xA0, 256,  32, gb->get_cart_ram());
    update_texture_u8(ram_tex, 0x00, 0xC0, 256,  32, gb->get_main_ram());
    update_texture_u8(ram_tex, 0x00, 0xFE, 256,   1, gb->get_oam_ram());
    update_texture_u8(ram_tex, 0x80, 0xFF, 128,   1, gb->get_zram());

    blitter.blit_mono(view, ram_tex, 256, 256,
                      0, 0, 256, 256,
                      960 + 96, 640 + 96, 256, 256);
  }

  //----------------------------------------
  // Gameboy screen

  gb_blitter.blit_screen(view, 1280, 32,  2, gb->fb);
  gb_blitter.blit_tiles (view, 1632, 32,  1, gb->get_vram());
  gb_blitter.blit_map   (view, 1344, 448, 1, gb->get_vram(), 0, 0);
  gb_blitter.blit_map   (view, 1632, 448, 1, gb->get_vram(), 0, 1);
  gb_blitter.blit_map   (view, 1344, 736, 1, gb->get_vram(), 1, 0);
  gb_blitter.blit_map   (view, 1632, 736, 1, gb->get_vram(), 1, 1);

  //----------------------------------------
  // Trace view

  //const int gb_trace_x = int(view.sx()) - 32 * 17;
  //const int gb_trace_y = int(view.sy()) - 32 * 7;
  //gb_blitter.blit_trace (view, gb_trace_x, gb_trace_y, metroboy.get_trace());


  //dump_painter.render(view, 900, 100, 16, 8, metroboy->get_zram());
  //dump_painter.render(view, 900, 300, 64, 128, metroboy->get_main_ram());

  //----------------------------------------
  // Reference image

#if 0

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
#endif

  //----------------------------------------
  // Diff overlay

#if 0
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
#endif

  //----------------------------------------
  // Screen overlay

#if 0
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

}

//-----------------------------------------------------------------------------

void MetroBoyApp::app_render_ui(Viewport view) {
  StringDumper d;

  //----------------------------------------
  // Left column text

  int column = 0;

  if (1) {
    gb->dump_bus(d);
    gb->z80.dump(d);
    gb->timer2.dump(d);
    gb->dma2.dump(d);
    gb->cart.dump(d);
    gb->joypad.dump(d);
    gb->serial.dump(d);
    text_painter.render(view, d.s, (float)column, 0);
    d.clear();
    column += 32 * 7;
  }

  if (1) {
    gb->lcd.dump(d);
    gb->get_spu().dump(d);
    gb->zram.dump(d);
    gb->oam.dump(d);
    text_painter.render(view, d.s, (float)column, 0);
    d.clear();
    column += 32 * 7;
  }

  if (1) {
    gb->ppu.dump(d);
    text_painter.render(view, d.s, (float)column, 0);
    d.clear();
    column += 32 * 7;
  }

  if (1) {
    gb->dump_disasm(d);
    text_painter.render(view, d.s, (float)column, 0);
    d.clear();
    column += 32 * 7;
  }

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
}

//-----------------------------------------------------------------------------
