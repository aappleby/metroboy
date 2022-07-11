#include "GateBoyApp/GateBoyApp.h"
#include "GateBoyLib/GateBoyDumper.h"
#include "GateBoyLib/Utils.h"

#include "CoreLib/Constants.h"
#include "CoreLib/Debug.h" // for StringDumper
#include "CoreLib/Tests.h"

#include "AppLib/AppHost.h"
#include "AppLib/Audio.h"
#include "AppLib/GLBase.h"

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
#ifdef _MSC_VER
  SetPriorityClass(GetCurrentProcess(), 0x00000080);
#endif

  printf("%d %d\n", (int)sizeof(GateBoyState), (int)sizeof(LogicBoyState));

  App* app = new GateBoyApp();
  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_init(int screen_w, int screen_h) {
  dvec2 screen_size(screen_w, screen_h);

  LOG_G("GateBoyApp::app_init()\n");
  LOG_INDENT();

  audio_init();

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
  //gb_thread = new GateBoyThread(new LogicBoy());
  gb_thread = new GateBoyThread(new GateBoy());
  gb_thread->start();

  /*
  NR10 FF10 -PPP NSSS Sweep period, negate, shift
  NR11 FF11 DDLL LLLL Duty, Length load (64-L)
  NR12 FF12 VVVV APPP Starting volume, Envelope add mode, period
  NR13 FF13 FFFF FFFF Frequency LSB
  NR14 FF14 TL-- -FFF Trigger, Length enable, Frequency MSB
  */

  // baBING
  // 0x000700c0 0xff26 0x80 // apu on
  // 0x00070108 0xff25 0xf3 // left en 0b1111 right en 0b0011
  // 0x00070128 0xff24 0x77 // l vol 7 r vol 7

  // 0x000700d0 0xff11 0x80 // ch1 duty 0b10
  // 0x000700f8 0xff12 0xf3 // ch1 vol 15 env- period 3

  // 0x02150f98 0xff13 0x83 // ch1      freq lo 0b10000011
  // 0x02150fc0 0xff14 0x87 // ch1 trig freq hi 0b0000011110000011

  // 0x021fc700 0xff13 0xc1 // ch1      freq lo 0b11000001
  // 0x021fc728 0xff14 0x87 // ch1 trig freq hi 0b0000011111000001

  gb_thread->load_program(R"(
    0150:
      ld a, $00
      ld ($FF26), a
      ld a, $80
      ld ($FF26), a
      ld a, $F3
      ld ($FF25), a
      ld a, $55
      ld ($FF24), a

      ld a, $19
      ld ($FF10), a
      ld a, $80
      ld ($FF11), a
      ld a, $F8
      ld ($FF12), a

      ld a, $AA
      ld ($FF13), a
      ld a, $85
      ld ($FF14), a

      jr -2
  )");


#if 0
  // test_fuzz_reg failed at 1871:0268 - write 0xe5 to 0xff40
  {
    gb_thread->gb->set_cpu_en(false);

    auto gb = gb_thread->gb.state();
    auto addr = ADDR_LCDC;
    auto& dummy_cart = gb_thread->get_cart();


    uint32_t r = xorshift32(1);

    for (int i = 0; i < 203; i++) {
      r = xorshift32(r);
      //if (r & 1) {
      if ((r % 100) >= 90) {
        r = xorshift32(r);
        auto res = gb->dbg_write(dummy_cart, addr, uint8_t(r));
        if (res.is_err()) printf("%d\n", i);
      }
      else {
        auto res = gb->dbg_read(dummy_cart, addr);
        if (res.is_err()) printf("%d\n", i);
      }
    }

    r = xorshift32(r);
    if (r & 1) {
      r = xorshift32(r);
      auto res = gb->dbg_req((uint16_t)addr, uint8_t(r), 1);
    }
    else {
      gb->dbg_req((uint16_t)addr, 0, 0);
    }
  }

  gb_thread->run_to(1707 - 2);

#endif

#if 1
  // oh is about 125 seconds
  // gejmboj also around 120
  // pocket around 140

  //blob cart;

  //load_blob("tests/microtests/DMG/line_153_lyc0_int_inc_sled.gb", cart);
  //load_blob("tests/microtests/DMG/oam_read_l0_d.gb", cart);
  //load_blob("LinksAwakening.gb", cart);     // broken
  //load_blob("tetris.gb", cart);             // broken
  //load_blob("SML.gb", cart); // reboot loop
  //load_blob("pman.gb", cart); // title screen funkd up
  //load_blob("tests/instr_timing.gb", cart);
  //load_blob("tests/cpu_instrs/individual/10-bit ops.gb", cart);
  //load_blob("tests/microtests/DMG/timer_tma_write_a.gb", cart);

  //gb_thread->load_cart_blob(cart);
  //gb_thread->reset_to_cart();
#endif

  gb_thread->resume();

  LOG_DEDENT();
  LOG_G("GateBoyApp::app_init() done\n");
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_close() {
  gb_thread->stop();
  delete gb_thread;
  gb_thread = nullptr;
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
      if (app_paused) {
        app_paused = false;
        gb_thread->resume();
      }
      gb_thread->clear_steps();
      gb_thread->add_steps(46880727);
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
      gb_thread->reset_to_cart(); break;
    }
    case SDLK_d:    show_diff   = !show_diff; break;
    case SDLK_g:    show_golden = !show_golden; break;

    case SDLK_t: {
      show_gb_ab = !show_gb_ab;
      gb_thread->gb->dbg_flip();
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
        gb_thread->reset_to_cart();
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

  if (!app_paused) gb_thread->resume();
}

//-----------------------------------------------------------------------------

#pragma warning(disable:4189)

double ease(double a, double b, double delta);

void GateBoyApp::app_render_frame(dvec2 screen_size, double delta) {
  (void)delta;

  auto& view = view_control.view_smooth_snap;

  gb_thread->pause();

  //auto& gb = show_gb_ab ? gb_thread.gbp->gbb : gb_thread.gbp->gba;
  auto gb = gb_thread->gb.state();

  uint64_t phase_total = gb->get_sys().gb_phase_total;

  auto& cpu = gb_thread->gb->get_cpu();
  auto& mem = gb_thread->gb->get_mem();
  auto& state = gb_thread->gb->get_state();
  auto& sys = gb_thread->gb->get_sys();
  auto& pins = gb_thread->gb->get_pins();
  auto& probes = gb_thread->gb->get_probes();

  StringDumper d;

  float col_spacing = 220;

  grid_painter.render(view, screen_size);

  const int row1 = 4;

  const int col1 = 32 *  0 + 8;
  const int col2 = 32 *  7 + 8;
  const int col3 = 32 * 14 + 8;
  const int col4 = 32 * 21 + 8;
  const int col5 = 32 * 29 + 8;
  const int col6 = 32 * 40;
  const int col7 = 32 * 51;

  GateBoyDumper dumper;

  //----------------------------------------
  // Column 1

  d("\002===== HASHES =====\001\n");
  {
    d("phase    %lld\n",      gb->get_sys().gb_phase_total);
    d("cpu      0x%016llx\n", gb->get_cpu().core.get_hash());
    d("vid_ram  0x%016llx\n", hash_blob(gb->get_mem().vid_ram,     sizeof(gb->get_mem().vid_ram)));
    d("cart_ram 0x%016llx\n", hash_blob(gb->get_mem().cart_ram,    sizeof(gb->get_mem().cart_ram)));
    d("int_ram  0x%016llx\n", hash_blob(gb->get_mem().int_ram,     sizeof(gb->get_mem().int_ram)));
    d("oam_ram  0x%016llx\n", hash_blob(gb->get_mem().oam_ram,     sizeof(gb->get_mem().oam_ram)));
    d("zero_ram 0x%016llx\n", hash_blob(gb->get_mem().zero_ram,    sizeof(gb->get_mem().zero_ram)));
    d("framebuf 0x%016llx\n", hash_blob(gb->get_mem().framebuffer, sizeof(gb->get_mem().framebuffer)));
  }
  d("\n");

  d("\002===== Thread =====\001\n");
  gb_thread->dump(d);

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

  d("\002===== STACK =====\001\n");
  {
    int sp = gb->get_cpu().core.get_sp();
    gb->get_flat_blob(gb_thread->get_cart(), sp, 8, temp_buf);
    uint16_t* t = (uint16_t*)temp_buf.data();
    for (auto i = 0; i < temp_buf.size(); i += 2) {
      d("0x%04x: 0x%04x\n", sp + i, t[i / 2]);
    }
  }
  d("\n");

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

  // Probe dump
  d("\002========== Debug Probes ==========\001\n");
  gb->get_probes().dump(d);
  d("\n");

  d("\002========== Disassembly ==========\001\n");
  {
    int pc = gb->get_cpu().core.get_op_addr();

    temp_buf.resize(64);
    gb->get_flat_blob(gb_thread->get_cart(), pc, 64, temp_buf);
    assembler.disassemble(temp_buf.data(), 64, pc, pc, 16, d, /*collapse_nops*/ false);

#if 0
    const uint8_t* code = nullptr;
    int code_size = 0;
    int code_base = 0;
  
    if (!bit(state.cpu_signals.TEPU_BOOT_BITn.state)) {
      code      = DMG_ROM_blob.data();
      code_size = (int)DMG_ROM_blob.size();
      code_base = ADDR_BOOT_ROM_BEGIN;
    }
    else if (pc >= 0x0000 && pc <= 0x7FFF) {
      // FIXME needs to account for mbc1 mem mapping
      code      = gb_thread->get_cart().data();
      code_size = (int)gb_thread->get_cart().size();
      code_base = ADDR_CART_ROM_BEGIN;
    }
    else if (pc >= 0xFF80 && pc <= 0xFFFE) {
      code      = mem.zero_ram;
      code_size = 127;
      code_base = ADDR_ZEROPAGE_BEGIN;
    }
  
    assembler.disassemble(code, code_size, code_base, pc, 16, d, /*collapse_nops*/ false);
#endif
  }
  d("\n");

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

  double sim_ratio = 0.0;
  double sim_time_smooth = 0.0;

  d("Viewing sim %c, Sim clock %8.3f %s %s\n",
    show_gb_ab ? 'B' : 'A',
    double(phase_total) / (4194304.0 * 2),
    phase_names[phase_total & 7],
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
        uint32_t c = (x == fb_x) ? 0x77FFFFFF : 0x2200FFFF;
        overlay[x + fb_y * 160] = c; //0xFF000000 | (b << 16) | (g << 8) | (r << 0);
      }
    }

    update_texture_u32(overlay_tex, 160, 144, overlay);
    blitter.blit(view, screen_size, overlay_tex, col6, gb_screen_y, 160 * 2, 144 * 2);
  }

  // Draw flat memory view

  if (gb_thread->get_cart().data())
  {
    text_painter.render_string(view, screen_size, "\002========== Flat memory view ==========\001", col6, 768);
    update_texture_u8(ram_tex, 0x00, 0x00, 256, 128, gb_thread->get_cart().data());
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
  gb_blitter.blit_map   (view, screen_size, col7, row1 + 16,  1, mem.vid_ram, 0, (int)!bit0(state.reg_lcdc.WEXU_LCDC_BGTILEp.get_state() ^ 1));

  text_painter.render_string(view, screen_size, "\002========== VRAM Map 1 ==========\001", col7, row2);
  gb_blitter.blit_map   (view, screen_size, col7, row2 + 16, 1, mem.vid_ram, 1, (int)!bit0(state.reg_lcdc.WEXU_LCDC_BGTILEp.get_state() ^ 1));

  text_painter.render_string(view, screen_size, "\002========== VRAM Tiles ==========\001", col7, row3);
  gb_blitter.blit_tiles (view, screen_size, col7, row3 + 16, 1, mem.vid_ram);

  if (!app_paused) gb_thread->resume();



  {
    static uint8_t buf[256*256];

    if (spu_buffer && !app_paused) {
      for (int i = 0; i < 65536; i++) {
        buf[i] = buf[i] >> 1;
      }
      for (int i = 0; i < 255; i++) {
        int x = i;
     
        int y1 = (spu_buffer[(2 * i + 0 + spu_write_cursor) & 0x1FF]);
        int y2 = (spu_buffer[(2 * i + 2 + spu_write_cursor) & 0x1FF]);

        y1 += 64;
        y2 += 64;

        if (y1 <   0) y1 = 0;
        if (y1 > 127) y1 = 127;
        if (y2 <   0) y2 = 0;
        if (y2 > 127) y2 = 127;

        if (y1 > y2) {
          auto t = y1;
          y1 = y2;
          y2 = t;
        }

        for (int y = y1; y <= y2; y++) {
          buf[x + y * 256] = 0xFF;
        }
      }

      for (int i = 0; i < 255; i++) {
        int x = i;
        int y1 = (spu_buffer[(2 * i + 1 + spu_write_cursor) & 0x1FF]);
        int y2 = (spu_buffer[(2 * i + 3 + spu_write_cursor) & 0x1FF]);

        y1 += 64;
        y2 += 64;

        if (y1 <   0) y1 = 0;
        if (y1 > 127) y1 = 127;
        if (y2 <   0) y2 = 0;
        if (y2 > 127) y2 = 127;

        y1 += 128;
        y2 += 128;

        if (y1 > y2) {
          auto t = y1;
          y1 = y2;
          y2 = t;
        }
        for (int y = y1; y <= y2; y++) {
          buf[x + y * 256] = 0xFF;
        }
      }
      update_texture_u8(wave_tex, 0x00, 0x00, 256, 256,  buf);
    }

    blitter.blit_mono(view, screen_size,
      wave_tex, 256, 256,
      0, 0, 256, 256,
      32*29, 784, 256, 256);
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
