#include "metroboy_main.h"

#include "MetroBoy.h"
#include "Audio.h"
#include "GLBase.h"

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
#include <examples/imgui_impl_sdl.h>
#include <vector>

extern const uint32_t gb_colors[];
extern uint8_t rom_buf[];

void run_test(const std::string& prefix, const std::string& name);

std::string junk(65536, 0);

//-----------------------------------------------------------------------------

void MetroBoyApp::init() {
  AppBase::init();
  keyboard_state = SDL_GetKeyboardState(nullptr);
  audio_init();

  gb_tex = create_texture_u8(160, 144);
  trace_tex = create_texture_u32(456, 154);
  ram_tex = create_texture_u8(256, 256);

  gb_blitter.init();
  grid_painter.init();
  dump_painter.init();

  view = view.reset(screen_w, screen_h);
  view_smooth = view;
  view_snap = view;

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

  //load("cpu_instrs");
  //load("instr_timing");

  //load("microtests/build/dmg", "poweron_000_div");
  //load("microtests/build/dmg", "minimal");
  //load("micro_cpu/build/dmg", "cpu_mov");

  //load("microtests/build/dmg", "400-dma");

  //load("roms/gb-test-roms/cpu_instrs/individual", "01-special");
  //load("roms/gb-test-roms/cpu_instrs/individual", "02-interrupts");
  //load("roms/gb-test-roms/cpu_instrs/individual", "03-op sp,hl");
  //load("roms/gb-test-roms/cpu_instrs/individual", "04-op r,imm");
  //load("roms/gb-test-roms/cpu_instrs/individual", "05-op rp");
  //load("roms/gb-test-roms/cpu_instrs/individual", "06-ld r,r");
  //load("roms/gb-test-roms/cpu_instrs/individual", "07-jr,jp,call,ret,rst");
  //load("roms/gb-test-roms/cpu_instrs/individual", "08-misc instrs");
  //load("roms/gb-test-roms/cpu_instrs/individual", "09-op r,r");
  //load("roms/gb-test-roms/cpu_instrs/individual", "10-bit ops");
  //load("roms/gb-test-roms/cpu_instrs/individual", "11-op a,(hl)");

  load("roms/gb-test-roms/cpu_instrs", "cpu_instrs");


  //load_memdump("roms", "LinksAwakening_house");

  runmode = STEP_CYCLE;
  //runmode = RUN_FAST;
  //runmode = RUN_VSYNC;
};

//-----------------------------------------------------------------------------

void MetroBoyApp::load_memdump(const std::string& prefix, const std::string& name) {
  std::string filename = prefix + "/" + name + ".dump";

  FILE* file = nullptr;
  fopen_s(&file, filename.c_str(), "rb");

  std::vector<uint8_t> buf(65536, 0);
  size_t size = fread(buf.data(), 1, 65536, file);
  if(size != 65536) assert(false);

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

  uint8_t* oam_flat = (uint8_t*)metroboy.gb().oam.ram;

  for (int i = 0; i < 256; i++) {
    oam_flat[i] = buf[ADDR_OAM_BEGIN + i];
  }
}

//-----------------------------------------------------------------------------

void MetroBoyApp::load(const std::string& prefix, const std::string& name) {
  std::string gb_filename = prefix + "/" + name + ".gb";

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

  printf("Loading rom %s\n", gb_filename.c_str());
  memset(rom_buf, 0, 1024 * 1024);
  metroboy.load_rom(gb_filename.c_str(), false);
  rom_loaded = true;
  runmode = STEP_CYCLE;
}

//-----------------------------------------------------------------------------

void MetroBoyApp::close() {
  audio_stop();
  AppBase::close();
};

//-----------------------------------------------------------------------------

void MetroBoyApp::update(double delta) {

  int mouse_x = 0, mouse_y = 0;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
      if (event.type == SDL_MOUSEWHEEL) {
        view = view.zoom({mouse_x, mouse_y}, double(event.wheel.y) * 0.25);
      }

      if (event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON_LMASK)) {
        view = view.pan({event.motion.xrel, event.motion.yrel});
      }
    }

    if (!io.WantCaptureKeyboard) {
      if (event.type == SDL_KEYDOWN) switch (event.key.keysym.sym) {
      case SDLK_f:      runmode = RUN_FAST; break;
      case SDLK_v:      runmode = RUN_VSYNC; break;
      case SDLK_s:      runmode = STEP_FRAME; break;
      case SDLK_o:      overlay_mode = (overlay_mode + 1) % 3; break;
      case SDLK_RIGHT:  {
        if (keyboard_state[SDL_SCANCODE_LCTRL]) {
          step_forward += 10;
        } else {
          step_forward++;
        }
        break;
      }
      case SDLK_LEFT:   {
        if (keyboard_state[SDL_SCANCODE_LCTRL]) {
          step_backward += 10;
        } else {
          step_backward++;
        }
        break;
      }
      case SDLK_UP:     step_up = true; break;
      case SDLK_DOWN:   step_down = true; break;

      case SDLK_r:      reset = true; break;
      case SDLK_F1:     load_dump = true; break;
      case SDLK_F4:     save_dump = true; break;
      case SDLK_ESCAPE: {
        view = view.reset(screen_w, screen_h);
        break;
      }
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
    if (runmode == STEP_CYCLE) runmode = STEP_LINE;
    else if (runmode == STEP_LINE) runmode = STEP_FRAME;
  }

  if (step_down) {
    if (runmode == STEP_FRAME) runmode = STEP_LINE;
    else if (runmode == STEP_LINE) runmode = STEP_CYCLE;
  }

  //----------------------------------------
  // Run simulation

  cycles_begin = metroboy.total_tcycles();

  if (runmode == RUN_FAST) {
    int64_t cycle_begin = metroboy.total_tcycles();
    double begin = double(SDL_GetPerformanceCounter() - app_start);
    metroboy.run_fast(buttons, (int)fast_cycles);
    double end = double(SDL_GetPerformanceCounter() - app_start);
    int64 cycle_end = metroboy.total_tcycles();
    last_cycles = int(cycle_end - cycle_begin) / 4;
    sim_time_msec = 1000.0 * double(end - begin) / double(perf_freq);
    
    //printf("run_fast %f\n", elapsed_ms);
    double new_cycles = fast_cycles * (sim_budget_msec / sim_time_msec);
    //printf("run_fast %f\n", new_cycles);
    
    double a = 0.95;
    fast_cycles = fast_cycles * a + new_cycles * (1.0 - a);
  }
  else if (runmode == RUN_VSYNC) {
    //printf("%d --------\n", frame_count);
    int64_t cycle_begin = metroboy.total_tcycles();
    double begin = double(SDL_GetPerformanceCounter() - app_start);
    metroboy.run_vsync(buttons);
    double end = double(SDL_GetPerformanceCounter() - app_start);
    int64 cycle_end = metroboy.total_tcycles();
    last_cycles = int(cycle_end - cycle_begin) / 4;
    sim_time_msec = 1000.0 * double(end - begin) / double(perf_freq);
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
      metroboy.pop_cycle();
    }
  }
  else if (runmode == STEP_FRAME) {
    while (step_forward--)  metroboy.step_frame();
    while (step_backward--) metroboy.pop_frame();
  }
  else if (runmode == STEP_LINE) {
    while (step_forward--)  metroboy.step_line();
    while (step_backward--) metroboy.pop_line();
  }

  step_forward = 0;
  step_backward = 0;
  step_up = false;
  step_down = false;

  cycles_end = metroboy.total_tcycles();

  Viewport snapped = view.snap();
  view_smooth = view_smooth.ease(view, delta);
  view_snap = view_snap.ease(snapped, delta);

  if (view_snap == snapped) {
    view = view_snap;
    view_smooth = view_snap;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyApp::begin_frame() {
}

//-----------------------------------------------------------------------------

void MetroBoyApp::render_frame() {

  grid_painter.set_viewport(view_snap);
  grid_painter.render();

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

  //const int gb_screen_x = 32 * 32;
  //const int gb_screen_y = 32 * 11;

  const int gb_screen_x = 960;
  const int gb_screen_y = 320;
  gb_blitter.blit_screen(view_snap, gb_screen_x, gb_screen_y, 2, metroboy.fb());

  const int gb_map_x = screen_w - 256 - 32;
  const int gb_map_y = 32;

  gb_blitter.blit_map   (view_snap, gb_map_x, gb_map_y, metroboy.get_vram());

  const int gb_trace_x = 960;
  const int gb_trace_y = 320 + 320;
  gb_blitter.blit_trace (view_snap, gb_trace_x, gb_trace_y, metroboy.get_trace());

  /*
  update_texture_u8(ram_tex, 0, 0*32, 256, 128, rom_buf);
  update_texture_u8(ram_tex, 0, 4*32, 256,  32, metroboy.get_vram());
  update_texture_u8(ram_tex, 0, 5*32, 256,  32, metroboy.get_cram());
  update_texture_u8(ram_tex, 0, 6*32, 256,  32, metroboy.get_iram());
  update_texture_u8(ram_tex, 0, 7*32, 256,  32, metroboy.get_iram());
  blitter.blit_mono(view_snap, ram_tex, 256, 256,
                    0, 0, 256, 256,
                    gb_screen_x, 32, 256, 256);
  */


  //dump_painter.render(view_snap, 900, 100, 16, 8, metroboy.gb().get_zram());
  //dump_painter.render(view_snap, 900, 300, 64, 128, metroboy.gb().get_iram());

  /*
  if ((frame_count % 10) == 0) {
    for (int i = 0; i < 65536; i++) {
      static uint32_t c = 1;
      c *= 0x1234567;
      c ^= c >> 16;
      junk[i] = uint8_t(c);
    }

    char* cursor = junk.data();
    for (int i = 0; i < 200; i++) {
      snprintf(cursor, 256, "line %03d hello world %05d\n", i, frame_count);
      cursor += 256;
    }
  }

  dump_painter.render(view_snap, 900, 100, 256, 256, (const uint8_t*)junk.data());
  */


#if 0
  /*
  if (overlay_mode == 0 || overlay_mode == 1) {
    for (int y = 0; y < 144; y++) {
      uint32_t* line1 = &framebuffer[(y * 2 + gb_screen_y + 0) * fb_width + gb_screen_x];
      uint32_t* lineM2 = &framebuffer[(y * 2 + gb_screen_y + 1) * fb_width + gb_screen_x];
      for (int x = 0; x < 160; x++) {
        uint32_t c = gb_colors[fb.buf[x + (y * 160)] & 7];
        *line1++ = c; *line1++ = c;
        *lineM2++ = c; *lineM2++ = c;
      }
    }
  }
  */
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

void MetroBoyApp::render_ui() {

  //----------------------------------------
  // Left column text

  int column = 0;

  Gameboy& gameboy = metroboy.gb();

  gameboy.dump_bus(text_buf);
  sprintf(text_buf, "\n");

  gameboy.dump_cpu(text_buf);
  sprintf(text_buf, "\n");

  gameboy.dump_timer(text_buf);
  sprintf(text_buf, "\n");

  gameboy.dma.dump(text_buf);
  sprintf(text_buf, "\n");

  text_painter.render(text_buf, column, 0);
  text_buf.clear();
  column += 32 * 7;

  gameboy.dump_zram(text_buf);
  sprintf(text_buf, "\n");

  gameboy.dump_cart(text_buf);
  sprintf(text_buf, "\n");

  gameboy.dump_oam(text_buf);
  sprintf(text_buf, "\n");

  gameboy.dump_joypad(text_buf);
  sprintf(text_buf, "\n");

  gameboy.dump_serial(text_buf);
  sprintf(text_buf, "\n");

  text_painter.render(text_buf, column, 0);
  text_buf.clear();
  column += 32 * 7;

  gameboy.get_ppu().dump(text_buf);
  sprintf(text_buf, "\n");

  text_painter.render(text_buf, column, 0);
  text_buf.clear();
  column += 32 * 7;

  gameboy.dump_disasm(text_buf);
  sprintf(text_buf, "\n");

  gameboy.get_spu().dump(text_buf);
  sprintf(text_buf, "\n");

  text_painter.render(text_buf, column, 0);
  text_buf.clear();
  column += 32 * 7;

  {
    ImGuiIO& io = ImGui::GetIO();

    double sim_cycles_per_sec = double(last_cycles) * (1000.0 / sim_budget_msec);
    double rt_cycles_per_sec = (114*154*60);

    double app_cycles_per_sec = fast_cycles * io.Framerate;

    const char* mode_names[] = {
        "RUN_FAST", "RUN_VSYNC", "STEP_FRAME", "STEP_LINE", "STEP_CYCLE",
    };

    text_painter.dprintf("%s %d\n", mode_names[runmode], (int)(metroboy.gb().get_tcycle() & 3));
    text_painter.dprintf("sim budget %2.2f msec/frame\n", sim_budget_msec);
    text_painter.dprintf("sim time   %02.2f msec/frame\n", sim_time_msec);
    //text_painter.dprintf("sim rate   %07d cycles/frame\n", int(fast_cycles));
    text_painter.dprintf("sim rate   %07d cycles/frame\n", last_cycles);
    text_painter.dprintf("sim speed  %1.2fx realtime\n", sim_cycles_per_sec / rt_cycles_per_sec);
    text_painter.dprintf("app speed  %1.2fx realtime (%.1f FPS)\n", app_cycles_per_sec / rt_cycles_per_sec, io.Framerate);
    
    text_painter.render(960, 800);
  }

  //----------------------------------------
  // Perf timer

  /*
  sprintf(text_buf, "view        zoom %f view_x %f view_y %f\n", view.get_zoom(),        view.min.x, view.min.y);
  sprintf(text_buf, "view_smooth zoom %f view_x %f view_y %f\n", view_smooth.get_zoom(), view_smooth.min.x, view_smooth.min.y);
  sprintf(text_buf, "view_snap   zoom %f view_x %f view_y %f\n", view_snap.get_zoom(),   view_snap.min.x, view_snap.min.y);
  //sprintf(text_buf, "frame time %2.2f msec, %6d cyc/frame\n", last_frame_time_smooth, (int)(cycles_end - cycles_begin) / 4);
  text_painter.render(text_buf, 0, 1024 - 48);
  text_buf.clear();
  */
}

//-----------------------------------------------------------------------------

void MetroBoyApp::end_frame() {
}

//-----------------------------------------------------------------------------
