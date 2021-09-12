
#include "GateBoyApp/GateBoyApp.h"

#include "CoreLib/Constants.h"
#include "CoreLib/Debug.h" // for StringDumper
#include "CoreLib/Tests.h"

#include "AppLib/AppHost.h"
#include "AppLib/GLBase.h"

#define SDL_MAIN_HANDLED
#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#include <windows.h>
#else
#include <SDL2/SDL.h>
#endif


//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
#ifdef _MSC_VER
  SetPriorityClass(GetCurrentProcess(), 0x00000080);
#endif

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

  gb_thread.start();
  gb_thread.pause();

  /*
  {
    const char* app = R"(
    0150:
      ld a, $40
      ld hl, $FF46
      ld (hl), a
      jr -2
    )";

    Assembler as;
    as.assemble(app);
    auto blob = as.link();

    for (size_t i = 0; i < 256; i++) {
      blob[i + 0x4000] = 0xDA;
    }

    gb_thread.load_cart_blob(blob);
    gb_thread.reset_to_cart();
  }
  */

  blob cart;
  load_blob("tests/microtests/dmg/win0_scx3_a.gb", cart);
  gb_thread.load_cart_blob(cart);
  gb_thread.reset_to_cart();

  gb_thread.add_steps(409);
  gb_thread.run_steps();

  //gb_thread.load_blob(Assembler::create_dummy_cart());
  //gb_thread.reset_to_bootrom();

  //gb_thread.load_raw_dump("zelda_overworld.dump");
  //gb_thread.load_rom("LinksAwakening.gb");
  gb_thread.resume();

  //load_rom("tests/mooneye-gb/tests/build/acceptance/" "ppu/lcdon_write_timing-GS.gb"); // dmg pass, gateboy fail
  //load_rom("tests/mooneye-gb/tests/build/acceptance/ppu/lcdon_timing-GS.gb"); // dmg pass, gateboy fail
  //load_rom("tests/mooneye-gb/tests/build/acceptance/ppu/intr_2_mode0_timing_sprites.gb"); // dmg pass, gateboy fail

  //load_rom("tests/gb-test-roms/cpu_instrs/cpu_instrs.gb"); // pass
  //load_rom("tests/gb-test-roms/instr_timing/instr_timing.gb"); // pass
  //load_rom("tests/gb-test-roms/mem_timing/mem_timing.gb"); // pass
  //load_rom("tests/gb-test-roms/mem_timing-2/mem_timing.gb"); // pass
  //load_rom("tests/gb-test-roms/halt_bug.gb"); // fail
  //load_rom("tests/gb-test-roms/oam_bug/oam_bug.gb"); // fail

  //load_rom("tests/scribbltests/lycscx/lycscx.gb"); // pass
  //load_rom("tests/scribbltests/lycscy/lycscy.gb"); // pass
  //load_rom("tests/scribbltests/palettely/palettely.gb");
  //load_rom("tests/scribbltests/scxly/scxly.gb");
  //load_rom("tests/scribbltests/statcount/statcount-auto.gb"); // pass

  //load_rom("tests/microtests/DMG/poweron_stat_006.gb"); // stat low nibble goes 5-7-6, but it's supposed to read 4 - SADU cleared too late?
  //load_rom("tests/microtests/DMG/poweron_stat_120.gb"); // stat low nibble goes 4-6-2 but it's supposed to read 0 - RUPO cleared too late?
  //load_rom("tests/microtests/DMG/poweron_oam_006.gb"); // should be locked but wasn't - scan started late - oam locked the A after the read
  //load_rom("tests/microtests/DMG/poweron_oam_120.gb"); // should be locked but wasn't - scan started late - oam locked the A after the read
  //load_rom("tests/microtests/DMG/poweron_oam_234.gb"); // should be locked but wasn't - scan started late - oam locked the A after the read

  //load_rom("tests/microtests/DMG/lcdon_to_stat2_a.gb"); // RUPO cleared too late?
  //load_rom("tests/microtests/DMG/lcdon_to_oam_unlock_d.gb"); // hblank late

  // SIG_OAM_OEn goes back to 1 on E after the read starts, so we have to latch before that?
  // but the oam address doesn't show up until H?
  // sig_oam_clkn goes low EFGH during read
  //load_rom("tests/microtests/DMG/oam_read_l0_d.gb");

  //load_rom("tests/microtests/DMG/oam_write_l0_d.gb");   // should be unlocked but wasn't - hblank started late?
  //load_rom("tests/microtests/DMG/oam_write_l1_c.gb"); // this is the scan/render gap
  //load_rom("tests/microtests/DMG/oam_read_l1_a.gb");    // should be locked but wasn't - scan started late?
  //load_rom("tests/microtests/DMG/oam_read_l1_f.gb");    // should be locked but wasn't - scan started late?

  //load_rom("tests/microtests/DMG/poweron_vram_026.gb"); // should be locked but wasn't - rendering started late?
  //load_rom("tests/microtests/DMG/poweron_vram_140.gb"); // should be locked but wasn't - rendering started late?

  //load_rom("tests/microtests/DMG/ppu_sprite0_scx0_b.gb"); // pix 167 on c
  //load_rom("tests/microtests/DMG/ppu_sprite0_scx1_b.gb"); // pix 167 on e
  //load_rom("tests/microtests/DMG/ppu_sprite0_scx2_b.gb"); // pix 167 on g
  //load_rom("tests/microtests/DMG/ppu_sprite0_scx3_b.gb"); // pix 167 on a, fail - so hblank is like 1 or 2 phases late

  // if it was a data latch issue reading stat, it wouldn't also affect oam read...?

  LOG_DEDENT();
  LOG_G("GateBoyApp::app_init() done\n");
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_close() {
  gb_thread.stop();
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_update(dvec2 screen_size, double delta) {
  (void)delta;

  frame_begin = timestamp();

  SDL_Event event;

  gb_thread.pause();

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
        gb_thread.resume();
      }
      gb_thread.clear_steps();
      gb_thread.add_steps(INT_MAX);
      break;
    }
    case SDLK_s: {
      if (app_paused) {
        app_paused = false;
        gb_thread.resume();
      }
      gb_thread.clear_steps();
      break;
    }

    // Run to end of bootrom
    case SDLK_b: {
      if (app_paused) {
        app_paused = false;
        gb_thread.resume();
      }
      gb_thread.clear_steps();
      gb_thread.add_steps(46880640);
      break;
    }

    case SDLK_F1: {
      const char* filename = "gateboy.raw.dump";
      LOG_B("Loading raw dump from %s\n", filename);
      blob raw_dump;
      ::load_blob(filename, raw_dump);
      if (!raw_dump.empty()) {
        gb_thread.load_raw_dump(raw_dump);
      }
      break;
    }
    case SDLK_F4: {
      const char* filename = "gateboy.raw.dump";
      LOG_B("Saving raw dump to %s\n", filename);
      blob raw_dump;
      gb_thread.save_raw_dump(raw_dump);
      save_blob(filename, raw_dump);
      break;
    }
    case SDLK_r: {
      if (app_paused) {
        app_paused = false;
        gb_thread.resume();
      }
      gb_thread.reset_to_cart(); break;
    }
    case SDLK_d:    show_diff   = !show_diff; break;
    case SDLK_g:    show_golden = !show_golden; break;

    case SDLK_t: show_gb_ab = !show_gb_ab; break;

    case SDLK_LEFT:   {
      if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        gb_thread.rewind(8);
      }
      else {
        gb_thread.rewind(1);
      }
      break;
    }

    case SDLK_RIGHT:  {
      if (keyboard_state[SDL_SCANCODE_LCTRL] && keyboard_state[SDL_SCANCODE_LALT]) {
        gb_thread.add_steps(114 * 8 * 8);
      }
      else if (keyboard_state[SDL_SCANCODE_LALT]) {
        gb_thread.add_steps(114 * 8);
      }
      else if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        gb_thread.add_steps(8);
      }
      else {
        gb_thread.add_steps(1);
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
        gb_thread.load_cart_blob(rom);
        gb_thread.reset_to_cart();
      }
      else if (filename.ends_with("dump")) {
        LOG_B("Loading raw dump from %s\n", filename.c_str());
        blob dump;
        load_blob(event.drop.file, dump);
        gb_thread.load_raw_dump(dump);
      }
      SDL_free(event.drop.file);
    }
  }

  //----------------------------------------
  // Button input

  uint8_t buttons = 0;
  if (keyboard_state[SDL_SCANCODE_KP_6])   buttons |= 0x01; // RIGHT
  if (keyboard_state[SDL_SCANCODE_KP_4])   buttons |= 0x02; // LEFT
  if (keyboard_state[SDL_SCANCODE_KP_8])   buttons |= 0x04; // UP
  if (keyboard_state[SDL_SCANCODE_KP_2])   buttons |= 0x08; // DOWN

  if (keyboard_state[SDL_SCANCODE_X])      buttons |= 0x10; // A
  if (keyboard_state[SDL_SCANCODE_Z])      buttons |= 0x20; // B
  if (keyboard_state[SDL_SCANCODE_RSHIFT]) buttons |= 0x40; // SELECT
  if (keyboard_state[SDL_SCANCODE_RETURN]) buttons |= 0x80; // START
  gb_thread.set_buttons(buttons);

  view_control.update(delta);

  //----------------------------------------

  if (!app_paused) gb_thread.resume();
}

//-----------------------------------------------------------------------------

#pragma warning(disable:4189)

double ease(double a, double b, double delta);

void GateBoyApp::app_render_frame(dvec2 screen_size, double delta) {
  (void)delta;

  auto& view = view_control.view_smooth_snap;

  gb_thread.pause();

  auto& gb = show_gb_ab ? gb_thread.gbp->gbb : gb_thread.gbp->gba;

  uint8_t* framebuffer = gb.gbm.framebuffer;
  uint8_t* vid_ram = gb.gbm.vid_ram;
  uint64_t phase_total = gb.gbs.phase_total;


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

  //----------------------------------------
  // Column 1

  d("\002===== Thread =====\001\n");
  gb_thread.dump(d);

  double fps = 1.0f / delta;
  static double smooth_fps = 0.0;
  smooth_fps = ease(smooth_fps, fps, delta);
  d("App fps       : %d\n", (int)round(smooth_fps));
  d("Logic mode    : %d\n", gb.gbs.logic_mode);

  if (app_paused) {
    d("\003GB_THREAD IS PAUSED\001\n");
  }
  else {
    d("\n");
  }

  d("\n");

  d("\002===== GateBoy Top =====\001\n");
  gb.dump_sys(d);
  d("\n");

  d("\002===== CPU =====\001\n");
  gb.gbc.gb_cpu.dump(d);
  d("\n");

  d("\002===== Clocks =====\001\n");
  gb.dump_clocks(d);
  d("\n");

  d("\002===== Resets =====\001\n");
  gb.dump_resets(d);
  d("\n");


  d("\002===== Interrupts =====\001\n");
  gb.dump_interrupts(d);
  d("\n");

  text_painter.render_string(view, screen_size, d.s.c_str(), col1, row1);
  d.clear();

  //----------------------------------------
  // Column 2

  d("\002===== DMA =====\001\n");
  gb.dump_dma(d);
  d("\n");

  d("\002===== CPU Bus =====\001\n");
  gb.dump_cpu_bus(d);
  d("\n");

  d("\002===== EXT Bus =====\001\n");
  gb.dump_ext_bus(d);
  d("\n");

  d("\002===== OAM Bus =====\001\n");
  gb.dump_oam_bus(d);
  d("\n");

  d("\002===== VRAM Bus =====\001\n");
  gb.dump_vram_bus(d);
  d("\n");

  d("\002===== MBC1 =====\001\n");
  gb.dump_mbc1(d);
  d("\n");

  d("\002===== Timer =====\001\n");
  gb.dump_timer(d);
  d("\n");

  d("\002===== SPU =====\001\n");
  gb.dump_spu(d);
  d("\n");

  text_painter.render_string(view, screen_size, d.s.c_str(), col2, row1);
  d.clear();

  //----------------------------------------
  // Column 3

  d("\002===== TileFetcher =====\001\n");
  gb.dump_tile_fetcher(d);
  d("\n");

  d("\002===== Sprite Fetch =====\001\n");
  gb.dump_sprite_fetcher(d);
  d("\n");

  d("\002===== SpriteStore =====\001\n");
  gb.dump_sprite_store(d);
  d("\n");

  d("\002===== Sprite Scan =====\001\n");
  gb.dump_sprite_scanner(d);
  d("\n");

  d("\002===== Joypad =====\001\n");
  gb.dump_joypad(d);
  d("\n");

  d("\002===== Serial =====\001\n");
  gb.dump_serial(d);
  d("\n");

  text_painter.render_string(view, screen_size, d.s.c_str(), col3, row1);
  d.clear();

  //----------------------------------------
  // Column 4

  d("\002===== PPU =====\001\n");
  gb.dump_ppu(d);
  d("\n");

  d("\002===== LCD =====\001\n");
  gb.dump_lcd(d);
  d("\n");

  text_painter.render_string(view, screen_size, d.s.c_str(), col4, row1);
  d.clear();

  //----------------------------------------
  // Column 5

  // Help

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

  // Probe dump
  d("\002========== Debug Probes ==========\001\n");
  gb.gbs.probes.dump(d);
  d("\n");

  d("\002========== Disassembly ==========\001\n");
  {
    int pc = gb.gbc.gb_cpu.op_addr;
    const uint8_t* code = nullptr;
    int code_size = 0;
    int code_base = 0;

    if (!bit(gb.gbr.cpu_signals.TEPU_BOOT_BITn.qp_old())) {
      code      = DMG_ROM_blob.data();
      code_size = (int)DMG_ROM_blob.size();
      code_base = ADDR_BOOT_ROM_BEGIN;
    }
    else if (pc >= 0x0000 && pc <= 0x7FFF) {
      // FIXME needs to account for mbc1 mem mapping
      code      = gb_thread.get_cart().data();
      code_size = (int)gb_thread.get_cart().size();
      code_base = ADDR_CART_ROM_BEGIN;
    }
    else if (pc >= 0xFF80 && pc <= 0xFFFE) {
      code      = gb.gbm.zero_ram;
      code_size = 127;
      code_base = ADDR_ZEROPAGE_BEGIN;
    }

    assembler.disassemble(code, code_size, code_base, pc, 16, d, /*collapse_nops*/ false);
  }
  d("\n");

  text_painter.render_string(view, screen_size, d.s.c_str(), col5, row1);
  d.clear();

  //----------------------------------------
  // Column 6

  int gb_screen_y = row1 + 16;

  text_painter.render_string(view, screen_size, "\002========== Game Boy Screen ==========\001", col6, row1);

  if (has_golden && show_diff) {
    gb_blitter.blit_diff(view, screen_size, col6, gb_screen_y, 2, framebuffer, golden_u8);
  }
  else if (show_golden) {
    gb_blitter.blit_screen(view, screen_size, col6, gb_screen_y, 2, golden_u8);
  }
  else {
    gb_blitter.blit_screen(view, screen_size, col6, gb_screen_y, 2, framebuffer);
  }

  // Status bar under screen

  double sim_ratio = 0.0;
  double sim_time_smooth = 0.0;

  d("Viewing sim %c, Sim clock %8.3f %s %s\n",
    show_gb_ab ? 'B' : 'A',
    double(phase_total) / (4194304.0 * 2),
    phase_names[phase_total & 7],
    show_golden ? "GOLDEN IMAGE " : "");

  d("%c %c %c %c %c %c %c %c\n",
    gb.gbs.sys_buttons & 0x01 ? 'R' : '-',
    gb.gbs.sys_buttons & 0x02 ? 'L' : '-',
    gb.gbs.sys_buttons & 0x04 ? 'U' : '-',
    gb.gbs.sys_buttons & 0x08 ? 'D' : '-',
    gb.gbs.sys_buttons & 0x10 ? 'A' : '-',
    gb.gbs.sys_buttons & 0x20 ? 'B' : '-',
    gb.gbs.sys_buttons & 0x40 ? 'E' : '-',
    gb.gbs.sys_buttons & 0x80 ? 'S' : '-');


  text_painter.render_string(view, screen_size, d.s, col6, gb_screen_y + 144 * 2);
  d.clear();


  // Draw screen overlay
  {
    memset(overlay, 0, sizeof(overlay));

    int fb_x = bit_pack(gb.gbr.pix_count) - 8;
    int fb_y = bit_pack(gb.gbr.reg_ly);

    if (fb_y >= 0 && fb_y < 144) {
      for (int x = 0; x < 160; x++) {
        //uint8_t p0 = gb->lcd.lcd_pipe_lo[159 - fb_x + x + 1].qp_old();
        //uint8_t p1 = gb->lcd.lcd_pipe_hi[159 - fb_x + x + 1].qp_old();
        //int r = (3 - (p0 + p1 * 2)) * 30 + 50;
        //int g = (3 - (p0 + p1 * 2)) * 30 + 50;
        //int b = (3 - (p0 + p1 * 2)) * 30 + 30;

        uint32_t c = (x == fb_x) ? 0x77FFFFFF : 0x2200FFFF;
        overlay[x + fb_y * 160] = c; //0xFF000000 | (b << 16) | (g << 8) | (r << 0);
      }
      /*
      {
        uint8_t p0 = gb->lcd.lcd_pix_lo.qp_old();
        uint8_t p1 = gb->lcd.lcd_pix_hi.qp_old();
        int c = (3 - (p0 + p1 * 2)) * 85;
        overlay[fb_x + fb_y * 160] = 0xFF000000 | (c << 16) | (c << 8) | (c << 0);
      }
      */
    }

    update_texture_u32(overlay_tex, 160, 144, overlay);
  }
  blitter.blit(view, screen_size, overlay_tex, col6, gb_screen_y, 160 * 2, 144 * 2);

  // Draw flat memory view
  {
    text_painter.render_string(view, screen_size, "\002========== Flat memory view ==========\001", col6, 768);
    update_texture_u8(ram_tex, 0x00, 0x00, 256, 128, gb_thread.get_cart().data());
    update_texture_u8(ram_tex, 0x00, 0x80, 256, 32,  gb.gbm.vid_ram);
    update_texture_u8(ram_tex, 0x00, 0xA0, 256, 32,  gb.gbm.cart_ram);
    update_texture_u8(ram_tex, 0x00, 0xC0, 256, 32,  gb.gbm.int_ram);
    update_texture_u8(ram_tex, 0x00, 0xFE, 256, 1,   gb.gbm.oam_ram);
    update_texture_u8(ram_tex, 0x80, 0xFF, 128, 1,   gb.gbm.zero_ram);
    blitter.blit_mono(view, screen_size, ram_tex, 256, 256, 0, 0, 256, 256, col6, 784, 256, 256);
  }

  d("\002========== OAM ==========\001\n");
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", gb.gbm.oam_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n\n");

  d("\002========== Ram (first 128 bytes) ==========\001\n");
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", gb.gbm.int_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n\n");

  d("\002========== ZRAM ==========\001\n");
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", gb.gbm.zero_ram[x + y * 16]);
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
  gb_blitter.blit_map   (view, screen_size, col7, row1 + 16,  1, vid_ram, 0,  (int)bit(gb.gbr.reg_lcdc.WEXU_LCDC_BGTILEn.qn_old()));
  //gb_blitter.blit_map   (view, screen_size, col7, row1 + 16,  1, vid_ram, (int)bit(gb.reg_lcdc.XAFO_LCDC_BGMAPn.qn_old()),  (int)bit(gb.reg_lcdc.WEXU_LCDC_BGTILEn.qn_old()));

  text_painter.render_string(view, screen_size, "\002========== VRAM Map 1 ==========\001", col7, row2);
  gb_blitter.blit_map   (view, screen_size, col7, row2 + 16, 1, vid_ram, 1, (int)bit(gb.gbr.reg_lcdc.WEXU_LCDC_BGTILEn.qn_old()));
  //gb_blitter.blit_map   (view, screen_size, col7, row2 + 16, 1, vid_ram, (int)bit(gb.reg_lcdc.WOKY_LCDC_WINMAPn.qn_old()), (int)bit(gb.reg_lcdc.WEXU_LCDC_BGTILEn.qn_old()));

  text_painter.render_string(view, screen_size, "\002========== VRAM Tiles ==========\001", col7, row3);
  gb_blitter.blit_tiles (view, screen_size, col7, row3 + 16, 1, vid_ram);

  if (!app_paused) gb_thread.resume();

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
