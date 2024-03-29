#include "GateBoyApp/GateBoyApp.h"
#include "GateBoyLib/GateBoyDumper.h"
#include "GateBoyLib/Utils.h"

#include "metrolib/core/Constants.h"
#include "metrolib/core/Dumper.h" // for StringDumper
#include "metrolib/core/Tests.h"

#include "metrolib/appbase/AppHost.h"
#include "metrolib/appbase/GLBase.h"
#include "metrolib/audio/Audio.h"

#define SDL_MAIN_HANDLED
#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#include <windows.h>
#else
#include <SDL2/SDL.h>
#endif

#include "GateBoyLib/GateBoyState.h"
#include "GateBoyLib/LogicBoy.h"

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  App* app = new GateBoyApp();
  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  LOG("Shutting down\n");
  return ret;

#if 0
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    return 1;
  }

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  printf("display mode %d %d %d\n", display_mode.w, display_mode.h, display_mode.refresh_rate);

  window = SDL_CreateWindow(
                            "hello_sdl2",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            640, 480,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
                            );
  if (window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return 1;
  }

  SDL_GL_SetAttribute(
  SDL_GL_CONTEXT_FLAGS,
  SDL_GL_CONTEXT_DEBUG_FLAG
  // | SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG  // doesn't work in wslg
  // | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG // doesn't work in wslg
  );

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_GLContext gl_context = SDL_GL_CreateContext((SDL_Window*)window);

  printf("gl_context %p\n", gl_context);

  gladLoadGLLoader(SDL_GL_GetProcAddress);

  glViewport(0, 0, display_mode.w, display_mode.h);

  glDisable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.1f, 0.1f, 0.2f, 0.f);
  glClearDepth(1.0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //screenSurface = SDL_GetWindowSurface(window);
  //SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
  //SDL_UpdateWindowSurface(window);

  SDL_GL_SwapWindow((SDL_Window*)window);

  SDL_Delay(2000);
  SDL_DestroyWindow(window);
  SDL_Quit();
#endif
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_init(int screen_w, int screen_h) {
  dvec2 screen_size(screen_w, screen_h);

  LOG_G("GateBoyApp::app_init()\n");
  LOG_INDENT();

#ifdef SIM_AUDIO
  audio_init();
#endif

  view_control.init(screen_size);

  grid_painter.init(65536, 65536);
  text_painter.init();
  dump_painter.init_ascii();
  gb_blitter.init();
  blitter.init();

  trace_tex = create_texture_u32(912, 154, nullptr, false);
  ram_tex = create_texture_u8(256, 256, nullptr, false);
  overlay_tex = create_texture_u32(160, 144, nullptr, false);
  keyboard_state = SDL_GetKeyboardState(nullptr);
  wave_tex = create_texture_u8(256, 256, nullptr, false);

  //gb_thread = new GateBoyThread(new GateBoyPair(new GateBoy(), new LogicBoy()));
  gb_thread = new GateBoyThread(new GateBoy());
  //gb_thread = new GateBoyThread(new LogicBoy());

  //gb_thread->poweron(true);
  gb_thread->reset();

  /*
  BlobStream bs;
  ::load_blob("gateboy.raw.dump", bs.b);
  gb_thread->load_raw_dump(bs);
  */

#if 0
  const char* app = R"(
  0150:
    jr -2
  )";

  //gb_thread->poweron(true);
  gb_thread->load_program(app);
  gb_thread->reset();

  /*
  {
    auto gb = (GateBoyPair*)gb_thread->gb.state();
    gb->run_phases(gb_thread->get_cart(), 504);
    gb->dbg_write(gb_thread->get_cart(), 0xFF46, 0x80);
  }
  */

#endif

#if 1
  // oh is about 125 seconds
  // gejmboj also around 120
  // pocket around 140

  blob cart;

  //load_blob("symlinks/GBMicrotests/bin/sprite4_0_b.gb", cart);
  //load_blob("symlinks/GBMicrotests/bin/line_153_lyc0_int_inc_sled.gb", cart);
  //load_blob("symlinks/GBMicrotests/bin/oam_read_l0_d.gb", cart);
  load_blob("../LinksAwakening.gb", cart);
  //load_blob("tetris.gb", cart);
  //load_blob("SML.gb", cart);
  //load_blob("pman.gb", cart);
  //load_blob("tests/instr_timing.gb", cart);
  //load_blob("tests/cpu_instrs/individual/10-bit ops.gb", cart);
  //load_blob("symlinks/GBMicrotests/bin/timer_tma_write_a.gb", cart);

  gb_thread->load_cart_blob(cart);
  gb_thread->reset();

  //gb_thread->run_to(49582984);

#endif

  gb_thread->start();

  LOG_DEDENT();
  LOG_G("GateBoyApp::app_init() done\n");
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_close() {
  LOG_B("Stopping gb thread\n");
  gb_thread->stop();
  LOG_B("Deleting gb thread\n");
  delete gb_thread;
  gb_thread = nullptr;

#ifdef SIM_AUDIO
  audio_stop();
#endif

  LOG_B("app_close() done\n");
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_update(dvec2 screen_size, double delta) {
  (void)delta;

  frame_begin = timestamp();

  SDL_Event event;

  gb_thread->pause();

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
      break;
    }

    case SDLK_f: {
      if (app_paused) {
        app_paused = false;
        gb_thread->resume();
      }
      gb_thread->clear_steps();
      gb_thread->add_steps(INT_MAX);
      break;
    }
    case SDLK_s: {
      if (app_paused) {
        app_paused = false;
        gb_thread->resume();
      }
      gb_thread->clear_steps();
      break;
    }


    // Run to end of bootrom
    case SDLK_b: {
      gb_thread->clear_steps();
      gb_thread->add_steps(46880727);
      if (app_paused) {
        app_paused = false;
        gb_thread->resume();
      }
      break;
    }

    case SDLK_n: {
      gb_thread->clear_steps();
      //gb_thread->add_steps(52231120 - 46880727);
      gb_thread->add_steps(185482600 - 46880727 - 1);
      if (app_paused) {
        app_paused = false;
        gb_thread->resume();
      }
      break;
    }

    case SDLK_F1: {
      const char* filename = "gateboy.raw.dump";
      LOG_B("Loading raw dump from %s\n", filename);
      BlobStream bs;
      ::load_blob(filename, bs.b);
      gb_thread->load_raw_dump(bs);
      break;
    }
    case SDLK_F4: {
      const char* filename = "gateboy.raw.dump";
      LOG_B("Saving raw dump to %s\n", filename);
      BlobStream bs;
      gb_thread->save_raw_dump(bs);
      save_blob(filename, bs.b);
      break;
    }
    case SDLK_r: {
      if (app_paused) {
        app_paused = false;
        gb_thread->resume();
      }
      gb_thread->reset(); break;
    }
    case SDLK_d:    show_diff   = !show_diff; break;
    case SDLK_g:    show_golden = !show_golden; break;

    case SDLK_t: {
      show_gb_ab = !show_gb_ab;
      break;
    }

    case SDLK_LEFT:   {
      if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        gb_thread->rewind(8);
      }
      else {
        gb_thread->rewind(1);
      }
      break;
    }

    case SDLK_RIGHT:  {
      if (keyboard_state[SDL_SCANCODE_LCTRL] && keyboard_state[SDL_SCANCODE_LALT]) {
        gb_thread->add_steps(114 * 154 * 8);
        //gb_thread->add_steps(128 * 1024);
      }
      else if (keyboard_state[SDL_SCANCODE_LALT]) {
        gb_thread->add_steps(114 * 8);
        gb_thread->run_sync();
      }
      else if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        gb_thread->add_steps(8);
        gb_thread->run_sync();
      }
      else {
        gb_thread->add_steps(1);
        gb_thread->run_sync();
      }

      break;
    }
    }

    if (event.type == SDL_DROPFILE) {
      std::string filename = event.drop.file;
      if (filename.ends_with("gb")) {
        LOG_B("Loading cart rom from %s\n", filename.c_str());
        blob rom;
        load_blob(event.drop.file, rom);
        gb_thread->load_cart_blob(rom);
        gb_thread->reset();
      }
      else if (filename.ends_with("dump")) {
        LOG_B("Loading raw dump from %s\n", filename.c_str());
        BlobStream bs;
        load_blob(event.drop.file, bs.b);
        gb_thread->load_raw_dump(bs);
      }
      SDL_free(event.drop.file);
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
  gb_thread->set_buttons(buttons);

  view_control.update(delta);

  //----------------------------------------

  {
    IGateBoy* gb2 = show_gb_ab ? gb_thread->gb.top().get_a() : gb_thread->gb.top().get_b();

    temp_gb.cpu = gb2->get_cpu();
    temp_gb.mem = gb2->get_mem();
    temp_gb.gb_state = gb2->get_state();
    temp_gb.sys = gb2->get_sys();
    temp_gb.pins = gb2->get_pins();

    blob& cart = gb_thread->get_cart();

    int pc = temp_gb.cpu.core.get_op_addr();
    gb2->get_flat_blob(cart, pc, 64, disasm_buf);

    int sp = temp_gb.cpu.core.get_sp();
    gb2->get_flat_blob(gb_thread->get_cart(), sp, 8, stack_buf);

    temp_probes = gb2->get_probes();
    temp_gb_id = gb2->get_id();

    d.clear();
    d("\002===== Thread =====\001\n");
    gb_thread->dump(d);
  }


  if (!app_paused) gb_thread->resume();
}

//-----------------------------------------------------------------------------

#pragma warning(disable:4189)

double ease(double a, double b, double delta);

void GateBoyApp::app_render_frame(dvec2 screen_size, double delta) {
  (void)delta;

  auto& cpu = temp_gb.cpu;
  auto& mem = temp_gb.mem;
  auto& state = temp_gb.gb_state;
  auto& sys = temp_gb.sys;
  auto& pins = temp_gb.pins;

  blob& cart = gb_thread->get_cart();

  auto& view = view_control.view_smooth_snap;

  uint64_t phase_total_old = sys.gb_phase_total_old;
  //uint64_t phase_total_new = sys.gb_phase_total_new;

  //gb_thread->pause();

  grid_painter.render(view, screen_size);

  //if (!app_paused) gb_thread->resume();

  const int row1 = 4;

  const int col1 = 32 *  0 + 8;
  const int col2 = 32 *  7 + 8;
  const int col3 = 32 * 14 + 8;
  const int col4 = 32 * 21 + 8;
  const int col5 = 32 * 29 + 8 - 32; // scoot it a bit because i have long probe names
  const int col6 = 32 * 40;
  const int col7 = 32 * 51;

  GateBoyDumper dumper;

  //----------------------------------------
  // Column 1

  /*
  d("\002===== HASHES =====\001\n");
  {
    d("cpu       0x%016llx\n", gb->get_cpu().core.get_hash());
    d("vid_ram   0x%016llx\n", hash_blob(gb->get_mem().vid_ram,     sizeof(gb->get_mem().vid_ram)));
    d("cart_ram  0x%016llx\n", hash_blob(gb->get_mem().cart_ram,    sizeof(gb->get_mem().cart_ram)));
    d("int_ram   0x%016llx\n", hash_blob(gb->get_mem().int_ram,     sizeof(gb->get_mem().int_ram)));
    d("oam_ram   0x%016llx\n", hash_blob(gb->get_mem().oam_ram,     sizeof(gb->get_mem().oam_ram)));
    d("zero_ram  0x%016llx\n", hash_blob(gb->get_mem().zero_ram,    sizeof(gb->get_mem().zero_ram)));
    d("framebuf  0x%016llx\n", hash_blob(gb->get_mem().framebuffer, sizeof(gb->get_mem().framebuffer)));
  }
  d("\n");
  */


  double fps = 1.0f / delta;
  static double smooth_fps = 0.0;
  smooth_fps = ease(smooth_fps, fps, delta / 2.0);
  d("App fps       : %d\n", (int)floor(smooth_fps + 0.5));

  if (app_paused) {
    d("\003GB_THREAD IS PAUSED\001\n");
  }
  else {
    d("\n");
  }

  d("\n");

  d("\002===== Sys =====\001\n");
  dumper.dump_sys(sys, d);
  d("\n");

  d("\002===== CPU =====\001\n");
  cpu.core.dump(d);
  d("\n");

  /*
  d("\002===== STACK =====\001\n");
  {
    for (auto i = 0; i < temp_buf.size(); i += 2) {
      d("0x%04x: 0x%04x\n", sp + i, t[i / 2]);
    }
  }
  d("\n");
  */

  d("\002===== Pins =====\001\n");
  dumper.dump_pins(pins, d);
  d("\n");

  text_painter.render_string(view, screen_size, d.s.c_str(), col1, row1);
  d.clear();

  //----------------------------------------
  // Column 2

  d("\002===== Clocks =====\001\n");
  dumper.dump_clocks(state, d);
  d("\n");

  d("\002===== Resets =====\001\n");
  dumper.dump_resets(state, d);
  d("\n");

  d("\002===== Interrupts =====\001\n");
  dumper.dump_interrupts(state, d);
  d("\n");

  d("\002===== DMA =====\001\n");
  dumper.dump_dma(state, d);
  d("\n");

  d("\002===== CPU Bus =====\001\n");
  dumper.dump_cpu_bus(state, d);
  d("\n");

  d("\002===== EXT Bus =====\001\n");
  dumper.dump_ext_bus(state, d);
  d("\n");

  d("\002===== OAM Bus =====\001\n");
  dumper.dump_oam_bus(state, d);
  d("\n");

  d("\002===== VRAM Bus =====\001\n");
  dumper.dump_vram_bus(state, d);
  d("\n");

  text_painter.render_string(view, screen_size, d.s.c_str(), col2, row1);
  d.clear();

  //----------------------------------------
  // Column 3

  d("\002===== TileFetcher =====\001\n");
  dumper.dump_tile_fetcher(state, d);
  d("\n");

  d("\002===== Sprite Fetch =====\001\n");
  dumper.dump_sprite_fetcher(state, d);
  d("\n");

  d("\002===== SpriteStore =====\001\n");
  dumper.dump_sprite_store(state, d);
  d("\n");

  d("\002===== Sprite Scan =====\001\n");
  dumper.dump_sprite_scanner(state, d);
  d("\n");

  d("\002===== Joypad =====\001\n");
  dumper.dump_joypad(state, d);
  d("\n");

  d("\002===== Timer =====\001\n");
  dumper.dump_timer(state, d);
  d("\n");

  text_painter.render_string(view, screen_size, d.s.c_str(), col3, row1);
  d.clear();


  //----------------------------------------
  // Column 4

#if 0
  d("\002===== PPU =====\001\n");
  dumper.dump_ppu(state, d);
  d("\n");

  d("\002===== LCD =====\001\n");
  dumper.dump_lcd(state, d);
  d("\n");

  d("\002===== MBC1 =====\001\n");
  dumper.dump_mbc1(state, d);
  d("\n");
#endif

  d("\002===== SPU =====\001\n");
  dumper.dump_spu(state, d);
  d("\n");

#if 0
  d("\002===== Serial =====\001\n");
  dumper.dump_serial(state, d);
  d("\n");
#endif

  d("\002========== Disassembly ==========\001\n");
  {
    auto pc = cpu.core.get_op_addr();
    assembler.disassemble(disasm_buf, 64, pc, pc, 16, d, /*collapse_nops*/ false);
  }
  d("\n");


  // Probe dump
  d("\002========== Debug Probes ==========\001\n");
  temp_probes.dump(d);
  d("\n");


  text_painter.render_string(view, screen_size, d.s.c_str(), col4, row1);
  d.clear();


  //----------------------------------------
  // Column 5

  // Help

#if 0
  d("\004========== GATEBOY INSTRUCTIONS ==========\001\n");
  d(R"(
Drag and drop rom files onto the window to load.

Game Boy controls:
  Dpad   = Arrows
  A      = Z
  B      = X
  Select = Right Shift
  Start  = Enter

UI controls:
  Mouse drag   : Pan view
  Mouse wheel  : Zoom view
  Escape       : Reset view
  Shift-Escape : Quit GateBoy

Sim controls:
  Space : pause / unpause
  F     : Run full speed (which isn't very fast)
  B     : Run 46880640 phases, (bootrom duration)
  S     : Run in single-stepping mode.
  R     : Reset to the start of game execution.
  G     : Show golden image (for render tests)
  D     : Show golden diff (for render tests)
  F1    : Load state from "gateboy.raw.dump"
  F4    : Save state to "gateboy.raw.dump"

Step controls:
  Right arrow - step forward 1 phase
  Ctrl+R      - step forward 8 phases
  Alt+R       - step forward 1 line (144*8 phases)
  Ctrl+Alt+R  - step forward 8 lines (114*8*8 phases)
  Left arrow  - rewind history 1 step
  Ctrl+L      - rewind history 8 steps

)");
#endif

  text_painter.render_string(view, screen_size, d.s.c_str(), col5, row1);
  d.clear();

  //----------------------------------------
  // Column 6

  int gb_screen_y = row1 + 16;

  text_painter.render_string(view, screen_size, "\002========== Game Boy Screen ==========\001", col6, row1);

  if (has_golden && show_diff) {
    gb_blitter.blit_diff(view, screen_size, col6, gb_screen_y, 2, mem.framebuffer, golden_u8);
  }
  else if (show_golden) {
    gb_blitter.blit_screen(view, screen_size, col6, gb_screen_y, 2, golden_u8);
  }
  else {
    gb_blitter.blit_screen(view, screen_size, col6, gb_screen_y, 2, mem.framebuffer);
  }

  // Status bar under screen

  static const char* phase_names[] = {
    "\002A_______\001",
    "\003_B______\001",
    "\002__C_____\001",
    "\003___D____\001",
    "\002____E___\001",
    "\003_____F__\001",
    "\002______G_\001",
    "\003_______H\001",
  };

  d("Viewing sim %s, Sim clock %8.3f %s %s\n",
    temp_gb_id,
    double(phase_total_old) / (4194304.0 * 2),
    phase_names[phase_total_old & 7],
    show_golden ? "GOLDEN IMAGE " : "");

  d("%c %c %c %c %c %c %c %c",
    sys.buttons & 0x01 ? 'R' : '-',
    sys.buttons & 0x02 ? 'L' : '-',
    sys.buttons & 0x04 ? 'U' : '-',
    sys.buttons & 0x08 ? 'D' : '-',
    sys.buttons & 0x10 ? 'A' : '-',
    sys.buttons & 0x20 ? 'B' : '-',
    sys.buttons & 0x40 ? 'E' : '-',
    sys.buttons & 0x80 ? 'S' : '-');
  d("\n");

  text_painter.render_string(view, screen_size, d.s, col6, gb_screen_y + 144 * 2);
  d.clear();

  // Draw screen overlay
  if (!config_fastmode) {
    memset(overlay, 0, sizeof(overlay));

    int fb_x = bit_pack(state.pix_count) - 8;
    int fb_y = bit_pack(state.reg_ly);

    if (fb_y >= 0 && fb_y < 144) {
      for (int x = 0; x < 160; x++) {
        uint32_t c = (x == fb_x) ? 0x22FF0000 : 0x2200FFFF;
        overlay[x + fb_y * 160] = c; //0xFF000000 | (b << 16) | (g << 8) | (r << 0);
      }
    }

    update_texture_u32(overlay_tex, 160, 144, overlay);
    blitter.blit(view, screen_size, overlay_tex, col6, gb_screen_y, 160 * 2, 144 * 2);
  }

  // Draw flat memory view

  if (cart.data())
  {
    text_painter.render_string(view, screen_size, "\002========== Flat memory view ==========\001", col6, 768);
    update_texture_u8(ram_tex, 0x00, 0x00, 256, 128, cart.data());
    update_texture_u8(ram_tex, 0x00, 0x80, 256, 32,  mem.vid_ram);
    update_texture_u8(ram_tex, 0x00, 0xA0, 256, 32,  mem.cart_ram);
    update_texture_u8(ram_tex, 0x00, 0xC0, 256, 32,  mem.int_ram);
    update_texture_u8(ram_tex, 0x00, 0xFE, 256, 1,   mem.oam_ram);
    update_texture_u8(ram_tex, 0x80, 0xFF, 128, 1,   mem.zero_ram);
    blitter.blit_mono(view, screen_size, ram_tex, 256, 256, 0, 0, 256, 256, col6, 784, 256, 256);
  }

  d("\002========== OAM ==========\001\n");
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", mem.oam_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n\n");

  d("\002========== Ram (first 128 bytes) ==========\001\n");
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", mem.int_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n\n");

  d("\002========== ZRAM ==========\001\n");
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", mem.zero_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n\n");


  text_painter.render_string(view, screen_size, d.s, col6, 352);
  d.clear();


  //----------------------------------------
  // Column 7

  int row2 = 320;
  int row3 = 640;

  text_painter.render_string(view, screen_size, "\002========== VRAM Map 0 ==========\001", col7, row1);
  gb_blitter.blit_map   (view, screen_size, col7, row1 + 16,  1, mem.vid_ram, 0, (int)bit0(state.reg_lcdc.WEXU_LCDC_BGTILEp.state));

  text_painter.render_string(view, screen_size, "\002========== VRAM Map 1 ==========\001", col7, row2);
  gb_blitter.blit_map   (view, screen_size, col7, row2 + 16, 1, mem.vid_ram, 1, (int)bit0(state.reg_lcdc.WEXU_LCDC_BGTILEp.state));

  text_painter.render_string(view, screen_size, "\002========== VRAM Tiles ==========\001", col7, row3);
  gb_blitter.blit_tiles (view, screen_size, col7, row3 + 16, 1, mem.vid_ram);

  if (1) {
    static uint8_t buf[256*256];
    //bool scroll = false;
    //bool raw_sound = true;

    //auto audio_l = mem.audio_l;
    //auto audio_r = mem.audio_r;
    auto audio_1 = mem.audio_1;
    auto audio_2 = mem.audio_2;
    auto audio_3 = mem.audio_3;
    auto audio_4 = mem.audio_4;

    auto plot = [&](const sample_t* audio, int smin, int smax, int dmin, int dmax) {
      for (int i = 0; i < 255; i++) {
        int y1, y2;

        int x = int(sys.gb_phase_total_old >> 9);

        y1 = audio[(x + i + 1) & 0xFF];
        y2 = audio[(x + i + 2) & 0xFF];

        y1 = remap_clamp(y1, smin, smax, dmin, dmax);
        y2 = remap_clamp(y2, smin, smax, dmin, dmax);

        sort(y1, y2);
        for (int y = y1; y <= y2; y++) buf[i + y * 256] = 0xFF;
      }
    };

    if (!app_paused) {
      memset(buf, 0, 65536);

      plot(audio_1, 15, 0,  0,   63);
      plot(audio_2, 15, 0,  64, 127);
      plot(audio_3, 15, 0, 128, 191);
      plot(audio_4, 15, 0, 192, 255);

      /*
      for (int i = 0; i < 255; i++) {
        int offset = scroll ? (i + spu_ring_cursor) & 0xFF : i;
        int l1, l2, r1, r2;

        if (raw_sound) {
          l1 = audio_l[(i + 0) & 0xFF];
          l2 = audio_l[(i + 1) & 0xFF];
          r1 = audio_r[(i + 0) & 0xFF];
          r2 = audio_r[(i + 1) & 0xFF];

          l1 = remap_clamp(l1,   0, 511, 127,   0);
          l2 = remap_clamp(l2,   0, 511, 127,   0);
          r1 = remap_clamp(r1,   0, 511, 255, 128);
          r2 = remap_clamp(r2,   0, 511, 255, 128);
        }
        else {
          l1 = spu_ring_buffer[(2 * offset + 0) & 0x1FF];
          r1 = spu_ring_buffer[(2 * offset + 1) & 0x1FF];
          l2 = spu_ring_buffer[(2 * offset + 2) & 0x1FF];
          r2 = spu_ring_buffer[(2 * offset + 3) & 0x1FF];

          l1 = remap_clamp(l1, -256, 255, 127,   0);
          l2 = remap_clamp(l2, -256, 255, 127,   0);
          r1 = remap_clamp(r1, -256, 255, 255, 128);
          r2 = remap_clamp(r2, -256, 255, 255, 128);

          if (!scroll && i == (spu_write_cursor/2) - 1) {
            l2 = l1;
            r2 = r1;
          }
        }

        sort(l1, l2);
        sort(r1, r2);

        for (int y = l1; y <= l2; y++) buf[i + y * 256] = 0xFF;
        for (int y = r1; y <= r2; y++) buf[i + y * 256] = 0xFF;
      }
      */

      update_texture_u8(wave_tex, 0x00, 0x00, 256, 256,  buf);
    }

    blitter.blit_mono(view, screen_size,
      wave_tex, 256, 256,
      0, 0, 256, 256,
      32*29, 32, 256, 256);
  }

  frame_count++;
  frame_end = timestamp();
  frame_time = frame_end - frame_begin;
  frame_time_smooth = frame_time_smooth * 0.99 + frame_time * 0.01;
}

//------------------------------------------------------------------------------

void GateBoyApp::load_golden(const char* filename) {
  SDL_Surface* golden_surface = SDL_LoadBMP(filename);

  if (!golden_surface) {
    LOG_R("Failed to load golden %s\n", filename);
    memset(golden_u8, 0, 160 * 144);
    return;
  }

  if (golden_surface && golden_surface->format->format == SDL_PIXELFORMAT_INDEX8) {
    LOG_B("Loaded i8 golden %s\n", filename);
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
    LOG_B("Loaded bgr24 golden %s\n", filename);
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
