#include "GateBoyApp/GateBoyApp.h"

#include "CoreLib/Assembler.h"
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

#include <stdarg.h>

using namespace Schematics;

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  App* app = new GateBoyApp();
  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
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
  ram_tex = create_texture_u8(256, 256);
  overlay_tex = create_texture_u32(160, 144);
  keyboard_state = SDL_GetKeyboardState(nullptr);

  // regenerate post-bootrom dump
#if 1
  rom_buf = load_blob("roms/tetris.gb");
  gb->set_rom(rom_buf.data(), rom_buf.size());
  gb->reset_boot();
  gb->run_reset_sequence();

  for (int i = 0; i < 8192; i++) {
    gb->vid_ram[i] = (uint8_t)rand();
  }
#endif

#if 0
  // run tiny app
  if (1) {
    std::string app;
    app += "0150:\n";

    app += "ld a, $00\n";
    app += "ldh ($40), a\n";
    app += "ld a, $73\n";
    app += "ld hl, $8000\n";
    app += "ld (hl), a\n";
    app += "ld hl, $809F\n";
    app += "ld (hl), a\n";

    app += "ld hl, $FF80\n";
    app += "ld a, $E0\n"; app += "ld (hl+), a\n";
    app += "ld a, $46\n"; app += "ld (hl+), a\n";
    app += "ld a, $3E\n"; app += "ld (hl+), a\n";
    app += "ld a, $28\n"; app += "ld (hl+), a\n";
    app += "ld a, $3D\n"; app += "ld (hl+), a\n";
    app += "ld a, $20\n"; app += "ld (hl+), a\n";
    app += "ld a, $FD\n"; app += "ld (hl+), a\n";
    app += "ld a, $C9\n"; app += "ld (hl+), a\n";

    app += "ld a, $80\n";
    app += "call $ff80\n";

    app += "ld a, $00\n";
    app += "ld hl, $8000\n";
    app += "add (hl)\n";
    app += "jr -2\n";

    Assembler as;
    as.assemble(app.c_str());
    blob rom = as.link();

    gb->reset_post_bootrom(rom.data(), rom.size());
  }
#endif


  //load_flat_dump("roms/LinksAwakening_dog.dump");
  //gb->sys_cpu_en = false;

  // run rom
  //load_rom("microtests/build/dmg/ppu_sprite0_scx7_a.gb");
  //load_rom   ("roms/mealybug/m3_lcdc_bg_en_change.gb");
  //load_golden("roms/mealybug/m3_lcdc_bg_en_change.bmp");

  //load_rom("roms/tetris.gb");
}

//----------------------------------------

void GateBoyApp::app_close() {
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_update(double delta) {
  (void)delta;

  SDL_Event event;
  int step_forward = 0;
  int step_backward = 0;

  //----------------------------------------

  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_KEYDOWN)
    switch (event.key.keysym.sym) {

    case SDLK_F1: {
      printf("Loading raw dump from %s\n", "gateboy.raw.dump");
      gb.reset_states();
      gb->load_dump("gateboy.raw.dump");
      break;
    }

    case SDLK_F4: {
      printf("Saving raw dump to %s\n", "gateboy.raw.dump");
      gb->save_dump("gateboy.raw.dump");
      break;
    }

    case SDLK_r:
    case SDLK_F5: {
      printf("Resetting sim\n");
      gb.reset_states();
      gb->reset_cart();
      gb->set_rom(rom_buf.data(), rom_buf.size());
      break;
    }

    case SDLK_c: {
      if (gb->sys_cpu_en) {
        printf("Disabling CPU\n");
        gb->sys_cpu_en = 0;
      }
      else {
        printf("Enabling CPU\n");
        gb->sys_cpu_en = 1;
      }
      break;
    }

    case SDLK_f: runmode = RUN_FAST; break;
    case SDLK_v: runmode = RUN_VSYNC; break;
    case SDLK_s: runmode = RUN_STEP; break;

    case SDLK_UP: {
      stepmode = clamp_val(stepmode + 1, STEP_MIN, STEP_MAX);
      break;
    }

    case SDLK_DOWN: {
      stepmode = clamp_val(stepmode - 1, STEP_MIN, STEP_MAX);
      break;
    }

    case SDLK_LEFT:   {
      if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        step_backward += 8;
      } else {
        step_backward += 1;
      }
      break;
    }

    case SDLK_RIGHT:  {
      if (keyboard_state[SDL_SCANCODE_LCTRL] && keyboard_state[SDL_SCANCODE_LALT]) {
        step_forward += 114 * 8 * 8;
      }
      else if (keyboard_state[SDL_SCANCODE_LALT]) {
        step_forward += 114 * 8;
      } else if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        step_forward += 8;
      } else {
        step_forward += 1;
      }
      break;
    }
    }

    if (event.type == SDL_DROPFILE) {
      load_rom(event.drop.file);
      SDL_free(event.drop.file);
    }
  }

  //----------------------------------------

  if (gb->rom_buf != rom_buf.data()) __debugbreak();

  double sim_begin = timestamp();
  int64_t phase_begin = gb->phase_total;

  if (runmode == RUN_FAST) {
    gb.push();
    for (int i = 0; i < 114 * 8 * 8; i++) {
      gb->next_phase();
    }
  }
  else if (runmode == RUN_STEP && step_forward) {
    gb.push();
    for (int i = 0; i < step_forward; i++) {
      switch(stepmode) {
      case STEP_PASS:  gb->next_pass();   break;
      case STEP_PHASE: gb->next_phase();  break;
      case STEP_CYCLE: gb->next_mcycle(); break;
      case STEP_LINE:  gb->next_line();   break;
      }
    }
  }
  else if (runmode == RUN_STEP && step_backward) {
    for (int i  = 0; i < step_backward; i++) {
      gb.pop();
    }
    clear_probes();
  }

  double sim_end = timestamp();
  int64_t phase_end = gb->phase_total;
  sim_time = sim_end - sim_begin;
  sim_time_smooth = sim_time_smooth * 0.9 + sim_time * 0.1;
  sim_rate = (phase_end - phase_begin) / sim_time_smooth;

  frame_count++;
}

//-----------------------------------------------------------------------------
// Load a flat (just raw contents of memory addresses, no individual regs) dump
// and copy it into the various regs and memory chunks.

void GateBoyApp::load_flat_dump(const char* filename) {
  printf("Loading flat dump %s\n", filename);

  rom_buf = load_blob(filename);

  gb.reset_states();
  gb->reset_cart();
  gb->set_rom(rom_buf.data(), rom_buf.size());

  memcpy(gb->vid_ram,  rom_buf.data() + 0x8000, 8192);
  memcpy(gb->cart_ram, rom_buf.data() + 0xA000, 8192);
  memcpy(gb->ext_ram,  rom_buf.data() + 0xC000, 8192);
  memcpy(gb->oam_ram,  rom_buf.data() + 0xFE00, 256);
  memcpy(gb->zero_ram, rom_buf.data() + 0xFF80, 128);

  gb->dbg_write(ADDR_BGP,  rom_buf[ADDR_BGP]);
  gb->dbg_write(ADDR_OBP0, rom_buf[ADDR_OBP0]);
  gb->dbg_write(ADDR_OBP1, rom_buf[ADDR_OBP1]);
  gb->dbg_write(ADDR_SCY,  rom_buf[ADDR_SCY]);
  gb->dbg_write(ADDR_SCX,  rom_buf[ADDR_SCX]);
  gb->dbg_write(ADDR_WY,   rom_buf[ADDR_WY]);
  gb->dbg_write(ADDR_WX,   rom_buf[ADDR_WX]);

  // Bit 7 - LCD Display Enable             (0=Off, 1=On)
  // Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
  // Bit 5 - Window Display Enable          (0=Off, 1=On)
  // Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
  // Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
  // Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
  // Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
  // Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)

  // #define FLAG_BG_ON        0x01
  // #define FLAG_OBJ_ON       0x02
  // #define FLAG_TALL_SPRITES 0x04
  // #define FLAG_BG_MAP_1     0x08
  // #define FLAG_TILE_0       0x10
  // #define FLAG_WIN_ON       0x20
  // #define FLAG_WIN_MAP_1    0x40
  // #define FLAG_LCD_ON       0x80

  gb->dbg_write(ADDR_LCDC, rom_buf[ADDR_LCDC]);

  printf("Loaded %zd bytes from dump %s\n", rom_buf.size(), filename);
}

//----------

void GateBoyApp::save_flat_dump(const char* filename) {
  printf("TODO %s\n", filename);
}

//------------------------------------------------------------------------------
// Load a standard GB rom

void GateBoyApp::load_rom(const char* filename) {
  printf("Loading %s\n", filename);

  rom_buf = load_blob(filename);

  gb.reset_states();
  gb->load_post_bootrom_state();
  gb->reset_cart();
  gb->set_rom(rom_buf.data(), rom_buf.size());
  gb->phase_total = 0;
  gb->pass_count = 0;
  gb->pass_total = 0;

  printf("Loaded %zd bytes from rom %s\n", rom_buf.size(), filename);
}

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
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_render_frame(Viewport view) {
  grid_painter.render(view);

  const auto& top = gb->top;

  StringDumper dumper;
  float cursor = 0;

  dumper("\002===== Top =====\001\n");

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

  dumper("phase %s\n", phases[gb->phase_total & 7]);

  dumper("State count %d\n", gb.state_count());
  size_t state_size = gb.state_size_bytes();
  if (state_size < 1024 * 1024) {
    dumper("State size  %d K\n", state_size / 1024);
  }
  else {
    dumper("State size  %d M\n", state_size / (1024 * 1024));
  }
  dumper("Phase count %d\n",     gb->phase_total);
  dumper("Pass count  %d\n",     gb->pass_count);
  dumper("Pass total  %d\n",     gb->pass_total);
  dumper("Pass avg    %4.2f\n",   float(gb->pass_total) / float(gb->phase_total));
  dumper("Pass hash   %016llx\n", gb->pass_hash);
  dumper("Total hash  %016llx\n", gb->total_hash);
  dumper("BGB cycle   0x%08x\n",  (gb->phase_total / 4) - 0x10000);
  dumper("Sim clock   %f\n",      double(gb->phase_total) / (4194304.0 * 2));

  dumper("\n");
  dumper("dbg_req ");
  dump_req(dumper, gb->dbg_req);
  dumper("cpu_req ");
  dump_req(dumper, gb->cpu_req);
  dumper("bus_req ");
  dump_req(dumper, gb->bus_req);
  dumper("cpu_data_latch %d 0x%02x\n", gb->cpu_data_latch, gb->cpu_data_latch);

  dumper("\n");

  dump_probes(dumper);
  gb->cpu.dump(dumper);
  top.tim_reg.dump(dumper);
  top.int_reg.dump(dumper, top);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += 224 - 32;
  dumper.clear();

  //----------

  wire CLK = gb->phase_total & 1;
  top.clk_reg.dump(dumper, CLK);
  top.joypad.dump(dumper);
  top.ser_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += 224 - 64;
  dumper.clear();

  //----------

  top.cpu_bus.dump(dumper);
  top.ext_bus.dump(dumper);
  top.vram_bus.dump(dumper, top);
  top.oam_bus.dump(dumper);
  top.dma_reg.dump(dumper);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += 224;
  dumper.clear();

  //----------

  top.lcd_reg.dump(dumper, top);
  top.pix_pipe.dump(dumper, top);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += 224;
  dumper.clear();

  //----------

  top.sprite_fetcher.dump(dumper);
  top.sprite_scanner.dump(dumper, top);
  top.sprite_store.dump(dumper);
  top.tile_fetcher.dump(dumper, top);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += 224 - 32;
  dumper.clear();

  //----------

  dumper("\002===== Disasm =====\001\n");
  {
    uint16_t pc = gb->cpu.op_addr;
    const uint8_t* code = nullptr;
    uint16_t code_size = 0;
    uint16_t code_base = 0;

    if (!gb->top.bootrom.BOOT_BITn.qp()) {
      code = DMG_ROM_bin;
      code_size = 256;
      code_base = ADDR_BOOT_ROM_BEGIN;
    }
    else if (pc >= 0x0000 && pc <= 0x7FFF) {
      // FIXME needs to account for mbc1 mem mapping
      code = gb->rom_buf;
      code_size = 32768;
      code_base = ADDR_CART_ROM_BEGIN;
    }
    else if (pc >= 0xFF80 && pc <= 0xFFFE) {
      code = gb->zero_ram;
      code_size = 127;
      code_base = ADDR_ZEROPAGE_BEGIN;
    }

    assembler.disassemble(code, code_size, code_base, pc, 34, dumper, /*collapse_nops*/ false);
  }
  dumper("\n");

  dumper("\002===== OAM =====\001\n");
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 16; x++) {
      dumper("%02x ", gb->oam_ram[x + y * 16]);
    }
    dumper("\n");
  }
  dumper("\n");
  dumper("\002===== ZRAM =====\001\n");
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 16; x++) {
      dumper("%02x ", gb->zero_ram[x + y * 16]);
    }
    dumper("\n");
  }
  dumper("\n");

  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  dumper.clear();

  //cursor += col_width;
  //dumper.clear();

  //dump_painter.render(view, cursor, 512,      16, 16, DMG_ROM_bin);
  //dump_painter.render(view, cursor, 768, 16,  8, gb->zero_ram);
  //dump_painter.render(view, col_width * 4 + 128, 0, 4, 64, gb->mem + 0x0000);

  //update_texture_u32(trace_tex, 912, 154, trace);
  //blitter.blit(view, trace_tex, 0, 0, 912, 154);

  // Draw flat memory view
  {
    //printf("rom_buf.data() %p\n", rom_buf.data());
    //printf("gb->rom_buf %p\n", gb->rom_buf);

    update_texture_u8(ram_tex, 0x00, 0x00, 256, 128, gb->rom_buf);
    update_texture_u8(ram_tex, 0x00, 0x80, 256,  32, gb->vid_ram);
    update_texture_u8(ram_tex, 0x00, 0xA0, 256,  32, gb->cart_ram);
    update_texture_u8(ram_tex, 0x00, 0xC0, 256,  32, gb->ext_ram);
    update_texture_u8(ram_tex, 0x00, 0xFE, 256,   1, gb->oam_ram);
    update_texture_u8(ram_tex, 0x80, 0xFF, 128,   1, gb->zero_ram);
    blitter.blit_mono(view, ram_tex, 256, 256,
                      0, 0, 256, 256,
                      960 + 96 - 64, 640 + 96, 256, 256);
  }

  // Draw screen and vid ram contents
  if (1) {
    if (has_golden) {
      gb_blitter.blit_diff(view, 1280, 64,  2, gb->framebuffer, golden_u8);
    }
    else {
      gb_blitter.blit_screen(view, 1280, 64,  2, gb->framebuffer);
    }

    gb_blitter.blit_tiles (view, 1632, 32,  1, gb->vid_ram);
    gb_blitter.blit_map   (view, 1344, 448, 1, gb->vid_ram, 0, 0);
    gb_blitter.blit_map   (view, 1632, 448, 1, gb->vid_ram, 0, 1);
    gb_blitter.blit_map   (view, 1344, 736, 1, gb->vid_ram, 1, 0);
    gb_blitter.blit_map   (view, 1632, 736, 1, gb->vid_ram, 1, 1);
  }

  // Draw screen overlay
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
    blitter.blit(view, overlay_tex, 1280, 64, 160 * 2, 144 * 2);
  }

  switch(runmode) {
  case RUN_FAST:  dumper("RUN_FAST  "); break;
  case RUN_VSYNC: dumper("RUN_VSYNC "); break;
  case RUN_STEP:  dumper("RUN_STEP  "); break;
  }
  switch(stepmode) {
  case STEP_PASS:   dumper("STEP_PASS   "); break;
  case STEP_PHASE:  dumper("STEP_PHASE  "); break;
  case STEP_CYCLE:  dumper("STEP_CYCLE  "); break;
  case STEP_LINE:   dumper("STEP_LINE   "); break;
  }
  dumper("Sim clock %8.3f ",      double(gb->phase_total) / (4194304.0 * 2));
  dumper("%s", phases[gb->phase_total & 7]);
  dumper("%c", gb->sim_stable ? ' ' : '*');
  dumper("\n");

  /*
  int count = 0;
  gb.rev_scan([&](const GateBoy* gb) {
    switch(gb->get_step_size()) {
    case STEP_LINE:  dumper("L"); break;
    case STEP_CYCLE: dumper("C"); break;
    case STEP_PHASE: dumper("P"); break;
    case STEP_PASS:  dumper("S"); break;
    }
    count++;
    return count < 30;
  });
  dumper("\n");
  */


  text_painter.render(view, dumper.s.c_str(), 1280, 64 + 144 * 2);
  dumper.clear();

  double phases_per_frame = 114 * 154 * 60 * 8;
  double sim_ratio = sim_rate / phases_per_frame;

  text_painter.dprintf("Sim time %f, sim ratio %f\n", sim_time_smooth, sim_ratio);
  text_painter.dprintf("Frame time %f\n", frame_time_smooth);
  text_painter.render(view, float(view.screen_size.x - 320), float(view.screen_size.y - 64));
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
