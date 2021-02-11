#include "GateBoyApp/GateBoyApp.h"

#include "CoreLib/Constants.h"
#include "CoreLib/Debug.h" // for StringDumper
#include "CoreLib/Tests.h"

#include "AppLib/AppHost.h"
#include "AppLib/GLBase.h"

#define SDL_MAIN_HANDLED
#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include <windows.h>

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  SetPriorityClass(GetCurrentProcess(), 0x00000080);

  App* app = new GateBoyApp();
  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_init(int _screen_w, int _screen_h) {
  LOG_G("GateBoyApp::app_init()\n");
  LOG_SCOPE_INDENT();

  screen_w = _screen_w;
  screen_h = _screen_h;

  grid_painter.init(65536, 65536);
  text_painter.init();
  dump_painter.init();
  gb_blitter.init();
  blitter.init();

  trace_tex = create_texture_u32(912, 154);
  ram_tex = create_texture_u8(256, 256);
  overlay_tex = create_texture_u32(160, 144);
  keyboard_state = SDL_GetKeyboardState(nullptr);

#if 0
  // regenerate post-bootrom dump
  gb_thread.load_cart(DMG_ROM_blob, load_blob("microtests/build/dmg/poweron_div_004.gb"));
  gb_thread.reset_to_bootrom();
  for (int i = 0; i < 8192; i++) {
    gb_thread.gb->vid_ram[i] = (uint8_t)rand();
  }
#endif

  //load_rom("roms/wpol-gb/tests/build/acceptance/gpu/"   "hblank_ly_scx_timing-GS.gb");
  //load_rom("roms/naughtyemu/build/naughtyemu.gb"); // broken
  //load_rom("roms/LinksAwakening.gb");
  //load_rom("roms/Prehistorik Man (U).gb"); // seems to work

  load_rom("roms/mooneye-gb/tests/build/acceptance/" "interrupts/ie_push.gb");





#if 0
  const char* app = R"(
  0150:
    ld a, $80
    ld ($FF26), a

    ld hl, $FF24
    ld a, $0F
    ld (hl), a
    nop
    nop
    jr -3
  )";

  Assembler as;
  as.assemble(app);

  gb_thread.load_cart(DMG_ROM_blob, as.link());
  gb_thread.reset_to_cart();
#endif

#if 0
  load_flat_dump("roms/LinksAwakening_dog.dump");
  gb_thread.gb->sys_cpu_en = false;
  gb_thread.gb->phase_total = 0;

  gb_thread.gb->dbg_write(ADDR_WY, 113);
  gb_thread.gb->dbg_write(ADDR_WX, 13 + 7);

  //gb_thread.gb->dbg_write(ADDR_SCX, 7);
  //gb_thread.gb->dbg_write(ADDR_SCY, 7);
#endif

  LOG_G("GateBoyApp::app_init() done\n");
  gb_thread.start();
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_close() {
  gb_thread.stop();
}

//------------------------------------------------------------------------------

void GateBoyApp::load_raw_dump() {
  const char* filename = "gateboy.raw.dump";
  printf("Loading raw dump from %s\n", filename);
  blob dump = load_blob(filename);
  gb_thread.gb->from_blob(dump);
  gb_thread.load_cart(DMG_ROM_blob, gb_thread.cart);
  //gb_thread.gb->reset_to_cart();
}

void GateBoyApp::save_raw_dump() {
  const char* filename = "gateboy.raw.dump";
  printf("Saving raw dump to %s\n", filename);
  blob dump;
  gb_thread.gb->to_blob(dump);
  save_blob(filename, dump);
}

//------------------------------------------------------------------------------
// Load a standard GB rom

void GateBoyApp::load_rom(const char* filename) {
  printf("Loading %s\n", filename);

  gb_thread.load_cart(DMG_ROM_blob, load_blob(filename));
  gb_thread.reset_to_cart();

  printf("Loaded %zd bytes from rom %s\n", gb_thread.cart.size(), filename);
}


//-----------------------------------------------------------------------------
// Load a flat (just raw contents of memory addresses, no individual regs) dump
// and copy it into the various regs and memory chunks.

void GateBoyApp::load_flat_dump(const char* filename) {

  gb_thread.load_cart(DMG_ROM_blob, load_blob(filename));
  gb_thread.reset_to_cart();

  memcpy(gb_thread.gb->vid_ram,  gb_thread.cart.data() + 0x8000, 8192);
  memcpy(gb_thread.gb->cart_ram, gb_thread.cart.data() + 0xA000, 8192);
  memcpy(gb_thread.gb->int_ram,  gb_thread.cart.data() + 0xC000, 8192);
  memcpy(gb_thread.gb->oam_ram,  gb_thread.cart.data() + 0xFE00, 256);
  memcpy(gb_thread.gb->zero_ram, gb_thread.cart.data() + 0xFF80, 128);

  gb_thread.gb->dbg_write(ADDR_BGP,  gb_thread.cart[ADDR_BGP]);
  gb_thread.gb->dbg_write(ADDR_OBP0, gb_thread.cart[ADDR_OBP0]);
  gb_thread.gb->dbg_write(ADDR_OBP1, gb_thread.cart[ADDR_OBP1]);
  gb_thread.gb->dbg_write(ADDR_SCY,  gb_thread.cart[ADDR_SCY]);
  gb_thread.gb->dbg_write(ADDR_SCX,  gb_thread.cart[ADDR_SCX]);
  gb_thread.gb->dbg_write(ADDR_WY,   gb_thread.cart[ADDR_WY]);
  gb_thread.gb->dbg_write(ADDR_WX,   gb_thread.cart[ADDR_WX]);
  gb_thread.gb->dbg_write(ADDR_LCDC, gb_thread.cart[ADDR_LCDC]);
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_update(double /*delta*/) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_KEYDOWN)
    switch (event.key.keysym.sym) {

    case SDLK_SPACE: { gb_thread.paused() ? gb_thread.resume() : gb_thread.pause(); break; }

    case SDLK_f: {
      gb_thread.clear_work();
      if (runmode != RUN_FAST) {
        gb_thread.step_phase(1024 * 1024 * 1024);
        gb_thread.resume();
        runmode = RUN_FAST;
      }
      break;
    }
    case SDLK_v: {
      gb_thread.clear_work();
      runmode = RUN_SYNC;
      break;
    }
    case SDLK_s: {
      gb_thread.clear_work();
      runmode = RUN_STEP;
      break;
    }

    case SDLK_F1:   load_raw_dump();            break;
    case SDLK_F4:   save_raw_dump();            break;
    case SDLK_r:    gb_thread.reset_to_cart();          break;
    case SDLK_d:    show_diff   = !show_diff;   break;
    case SDLK_g:    show_golden = !show_golden; break;
    case SDLK_o:    draw_passes = !draw_passes; break;
    //case SDLK_c:  { gb_thread.toggle_cpu(); break; }

    case SDLK_LEFT:   {
      if (runmode == RUN_STEP) {
        if (keyboard_state[SDL_SCANCODE_LCTRL]) {
          gb_thread.step_back(8);
        } else {
          gb_thread.step_back(1);
        }
      }
      break;
    }

    case SDLK_RIGHT:  {
      if (runmode == RUN_STEP) {
        if (keyboard_state[SDL_SCANCODE_LCTRL] && keyboard_state[SDL_SCANCODE_LALT]) {
          gb_thread.step_phase(114 * 8 * 8);
        }
        else if (keyboard_state[SDL_SCANCODE_LALT]) {
          gb_thread.step_phase(114 * 8);
        } else if (keyboard_state[SDL_SCANCODE_LCTRL]) {
          gb_thread.step_phase(8);
        } else {
          gb_thread.step_phase(1);
        }
      }
      break;
    }
    }

    if (event.type == SDL_DROPFILE) {
      load_rom(event.drop.file);
      SDL_free(event.drop.file);
    }
  }
}

//-----------------------------------------------------------------------------

#pragma warning(disable:4189)

void GateBoyApp::app_render_frame() {
  Viewport view = Viewport::screenspace(screen_w, screen_h);

  gb_thread.pause();

  grid_painter.render(view);

  const auto gb = gb_thread.gb.state();

  uint8_t* framebuffer = gb->framebuffer;
  uint8_t* vid_ram = gb->vid_ram;
  uint64_t phase_total = gb->phase_total;


  StringDumper d;

  float cursor_x = 8;
  float cursor_y = 4;
  float col_spacing = 220;

  //----------------------------------------
  // dump column 1

  d("\002===== Thread =====\001\n");
  gb_thread.dump(d);
  d("\n");

  d("\002===== GateBoy Top =====\001\n");
  gb->dump_sys(d);
  d("\n");

  d("\002===== CPU =====\001\n");
  gb->cpu.dump(d);
  d("\n");

  d("\002===== Clocks =====\001\n");
  gb->dump_clocks(d);
  d("\n");

  d("\002===== Resets =====\001\n");
  gb->dump_resets(d);
  d("\n");


  d("\002===== Interrupts =====\001\n");
  gb->dump_interrupts(d);
  d("\n");

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------
  // dump column 2

  d("\002===== DMA =====\001\n");
  gb->dump_dma(d);
  d("\n");

  d("\002===== CPU Bus =====\001\n");
  gb->dump_cpu_bus(d);
  d("\n");

  d("\002===== EXT Bus =====\001\n");
  gb->dump_ext_bus(d);
  d("\n");

  d("\002===== OAM Bus =====\001\n");
  gb->dump_oam_bus(d);
  d("\n");

  d("\002===== VRAM Bus =====\001\n");
  gb->dump_vram_bus(d);
  d("\n");

  d("\002===== MBC1 =====\001\n");
  gb->dump_mbc1(d);
  d("\n");

  d("\002===== Timer =====\001\n");
  gb->dump_timer(d);
  d("\n");

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------
  // dump column 3

  d("\002===== TileFetcher =====\001\n");
  gb->dump_tile_fetcher(d);
  d("\n");

  d("\002===== Sprite Fetch =====\001\n");
  gb->dump_sprite_fetcher(d);
  d("\n");

  d("\002===== SpriteStore =====\001\n");
  gb->dump_sprite_store(d);
  d("\n");

  d("\002===== Sprite Scan =====\001\n");
  gb->dump_sprite_scanner(d);
  d("\n");

  d("\002===== CRAM =====\001\n");
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", gb->cart_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n");

  d("\002===== OAM =====\001\n");
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", gb->oam_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n");

  d("\002===== ZRAM =====\001\n");
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 16; x++) {
      d("%02x ", gb->zero_ram[x + y * 16]);
    }
    d("\n");
  }
  d("\n");

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------
  // dump column 4

  d("\002===== LCD =====\001\n");
  gb->dump_lcd(d);
  d("\n");

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------
  // dump column 5

  d("\002===== PPU =====\001\n");
  gb->dump_ppu(d);
  d("\n");

  d("\002===== SPU =====\001\n");
  gb->dump_spu(d);
  d("\n");

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------
  // dump column 6

  d("\002===== Disasm =====\001\n");
  {
    uint16_t pc = gb->cpu.op_addr;
    const uint8_t* code = nullptr;
    uint16_t code_size = 0;
    uint16_t code_base = 0;

    if (!bit(gb->cpu_signals.TEPU_BOOT_BITn_h.qp_old())) {
      code = gb_thread.boot.data();
      code_size = 256;
      code_base = ADDR_BOOT_ROM_BEGIN;
    }
    else if (pc >= 0x0000 && pc <= 0x7FFF) {
      // FIXME needs to account for mbc1 mem mapping
      code = gb_thread.cart.data();
      code_size = 32768;
      code_base = ADDR_CART_ROM_BEGIN;
    }
    else if (pc >= 0xFF80 && pc <= 0xFFFE) {
      code = gb->zero_ram;
      code_size = 127;
      code_base = ADDR_ZEROPAGE_BEGIN;
    }

    assembler.disassemble(code, code_size, code_base, pc, 34, d, /*collapse_nops*/ false);
  }
  d("\n");

  d("\002===== Joypad =====\001\n");
  gb->dump_joypad(d);
  d("\n");

  d("\002===== Serial =====\001\n");
  gb->dump_serial(d);
  d("\n");

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------
  // dump column 7

  /*
  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();
  */

  //----------------------------------------

  //update_texture_u32(trace_tex, 912, 154, trace);
  //blitter.blit(view, trace_tex, 0, 0, 912, 154);

  // Draw flat memory view
  {
    //printf("rom_buf.data() %p\n", rom_buf.data());
    //printf("gb->rom_buf %p\n", gb->rom_buf);

    update_texture_u8(ram_tex, 0x00, 0x00, 256, 128, gb_thread.cart.data());
    update_texture_u8(ram_tex, 0x00, 0x80, 256,  32, gb->vid_ram);
    update_texture_u8(ram_tex, 0x00, 0xA0, 256,  32, gb->cart_ram);
    update_texture_u8(ram_tex, 0x00, 0xC0, 256,  32, gb->int_ram);
    update_texture_u8(ram_tex, 0x00, 0xFE, 256,   1, gb->oam_ram);
    update_texture_u8(ram_tex, 0x80, 0xFF, 128,   1, gb->zero_ram);
    blitter.blit_mono(view, ram_tex, 256, 256,
                      0, 0, 256, 256,
                      42 * 32 - 16, 640 + 96 + 32 + 32, 256, 256);
  }

  // Draw screen and vid ram contents

  // Draw screen overlay
  {
    memset(overlay, 0, sizeof(overlay));

    int fb_x = gb->pix_count.get_old() - 8;
    int fb_y = gb->reg_ly.get_old();

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

  int gb_x = 42 * 32 - 16;
  int gb_y = 8;

  if (has_golden && show_diff) {
    gb_blitter.blit_diff(view,   gb_x, gb_y,  2, framebuffer, golden_u8);
  } else if (show_golden) {
    gb_blitter.blit_screen(view, gb_x, gb_y,  2, golden_u8);
  } else {
    gb_blitter.blit_screen(view, gb_x, gb_y,  2, framebuffer);
  }
  gb_blitter.blit_map   (view, 52 * 32 - 8,   0 * 32 + 8,  1, vid_ram, bit(gb->reg_lcdc.XAFO_LCDC_BGMAPn.qn_old()),  bit(gb->reg_lcdc.WEXU_LCDC_BGTILEn.qn_old()));
  gb_blitter.blit_map   (view, 52 * 32 - 8,   8 * 32 + 16, 1, vid_ram, bit(gb->reg_lcdc.WOKY_LCDC_WINMAPn.qn_old()), bit(gb->reg_lcdc.WEXU_LCDC_BGTILEn.qn_old()));
  gb_blitter.blit_tiles (view, 52 * 32 - 8,  16 * 32 + 24, 1, vid_ram);

  blitter.blit(view, overlay_tex, gb_x, gb_y, 160 * 2, 144 * 2);

  // Status bar under screen

  //double phases_per_frame = 114 * 154 * 60 * 8;
  //double sim_ratio = sim_rate / phases_per_frame;
  double sim_ratio = 0.0;
  double sim_time_smooth = 0.0;

  d("%s Sim clock %8.3f %s %s\n",
    runmode_names[runmode],
    double(phase_total) / (4194304.0 * 2),
    phase_names[phase_total & 7],
    show_golden ? "GOLDEN IMAGE " : "");
  //d("Sim time %f, sim ratio %f, frame time %f\n", sim_time_smooth, sim_ratio, frame_time_smooth);
  text_painter.render_string(view, d.s, gb_x, gb_y + 144 * 2);
  d.clear();

  // Probe dump

  gb->probes.dump(d, draw_passes);
  text_painter.render_string(view, d.s, 42 * 32 - 16, 19 * 20 - 24);
  d.clear();

  frame_count++;
  gb_thread.resume();
}

//------------------------------------------------------------------------------

void GateBoyApp::load_golden(const char* filename) {
  SDL_Surface* golden_surface = SDL_LoadBMP(filename);

  if (!golden_surface) {
    printf("Failed to load golden %s\n", filename);
    memset(golden_u8, 0, 160 * 144);
    return;
  }

  if (golden_surface && golden_surface->format->format == SDL_PIXELFORMAT_INDEX8) {
    printf("Loaded i8 golden %s\n", filename);
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
    printf("Loaded bgr24 golden %s\n", filename);
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
