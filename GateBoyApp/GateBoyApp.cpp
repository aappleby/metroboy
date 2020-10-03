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

GateBoyApp::GateBoyApp() {

  auto top_step = [](GateBoy* gb, StepSize step_size) {
    (void)step_size;
    if (step_size == STEP_PASS) {
      gb->next_pass();
    }
    else if (step_size == STEP_PHASE) {
      gb->next_phase();
    }
    else if (step_size == STEP_CYCLE) {
      gb->next_mcycle();
    }
    else if (step_size == STEP_LINE) {
      gb->next_line();
    }
  };

  auto top_unstep = [](GateBoy* gateboy, StepSize /*step_size*/) {
    (void)gateboy;

    // Run a logic pass after unstep to update our probes
    // This still doesn't work :P
    /*
    uint64_t pass_hash_old = gb->pass_hash;
    gb->update_logic();
    uint64_t pass_hash_new = revert_and_hash(gb->top);
    if (pass_hash_old != pass_hash_new) printf("UNSTEP FAIL\n");
    */

    clear_probes();
  };

  gb.init(top_step, top_unstep);
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
  gb->reset_to_bootrom(rom_buf.data(), rom_buf.size());

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


#if 1
  // run rom

  //load_rom("microtests/build/dmg/int_hblank_halt_bug_a.gb");
  //load_rom("microtests/build/dmg/int_hblank_halt_bug_b.gb");

  //load_rom("microtests/build/dmg/int_hblank_halt_scx0.gb"); // int fires on BC 626, int starts on 680, expected 4E FAIL
  //load_rom("microtests/build/dmg/int_hblank_halt_scx1.gb"); // int fires on DE 628, int starts on 680, expected 4F
  //load_rom("microtests/build/dmg/int_hblank_halt_scx2.gb"); // int fires on FG 630, int starts on 680, expected 4F
  //load_rom("microtests/build/dmg/int_hblank_halt_scx3.gb"); // int fires on HA 632, int starts on 688, expected 4F FAIL
  //load_rom("microtests/build/dmg/int_hblank_halt_scx4.gb"); // int fires on BC 634, int starts on 688, expected 4F FAIL
  //load_rom("microtests/build/dmg/int_hblank_halt_scx5.gb"); // int fires on DE 636, int starts on 688, expected 50
  //load_rom("microtests/build/dmg/int_hblank_halt_scx6.gb"); // int fires on FG 638, int starts on 688, expected 50
  //load_rom("microtests/build/dmg/int_hblank_halt_scx7.gb"); // int fires on HA 640, int starts on 696, expected 50 FAIL

  //load_rom("microtests/build/dmg/int_hblank_incs_scx0.gb"); // int fires on BC 626, int starts on 672
  //load_rom("microtests/build/dmg/int_hblank_incs_scx1.gb"); // int fires on DE 628, int starts on 672
  //load_rom("microtests/build/dmg/int_hblank_incs_scx2.gb"); // int fires on FG 630, int starts on 672
  //load_rom("microtests/build/dmg/int_hblank_incs_scx3.gb"); // int fires on HA 632, int starts on 680
  //load_rom("microtests/build/dmg/int_hblank_incs_scx4.gb"); // int fires on BC 634, int starts on 680
  //load_rom("microtests/build/dmg/int_hblank_incs_scx5.gb"); // int fires on DE 636, int starts on 680
  //load_rom("microtests/build/dmg/int_hblank_incs_scx6.gb"); // int fires on FG 638, int starts on 680
  //load_rom("microtests/build/dmg/int_hblank_incs_scx7.gb"); // int fires on HA 640, int starts on 688

  /*
  oam_int_halt_a.gb              0x01 0x01 0x31 PASS @ 322
  oam_int_halt_b.gb              0x02 0x02 0x31 PASS @ 323
  2038 : int fires on phase G
  2048 : isr starts

  vblank2_int_halt_a.gb          0x02 0x01 0x31 FAIL @ 16486
  vblank2_int_halt_b.gb          0x02 0x02 0x31 PASS @ 16488
  131450 : int fires on phase C
  131464 : isr starts

  vblank_int_halt_a.gb           0x02 0x01 0x31 FAIL @ 16491
  vblank_int_halt_b.gb           0x02 0x02 0x31 PASS @ 16493
  int fires on 131450, phase C

  lyc_int_halt_a.gb              0x05 0x04 0x31 FAIL @ 357
  lyc_int_halt_b.gb              0x05 0x05 0x31 PASS @ 359
  2090 : int fires on phase C
  2104 : isr starts
  passes w/ nop sled, fails w/ halt

  timer_int_halt_a.gb            0x02 0x02 0x31 PASS @ 247
  timer_int_halt_b.gb            0x03 0x03 0x31 PASS @ 241
  1200 : int fires on phase A
  1216 : isr starts

  */

  //load_rom("microtests/build/dmg/int_hblank_nops_scx0.gb"); // int fires on 822, phase G
  //load_rom("microtests/build/dmg/int_hblank_nops_scx1.gb"); // int fires on 824, phase A
  //load_rom("microtests/build/dmg/int_hblank_nops_scx2.gb"); // int fires on 826, phase C
  //load_rom("microtests/build/dmg/int_hblank_nops_scx3.gb"); // int fires on 828, phase E
  //load_rom("microtests/build/dmg/int_hblank_nops_scx4.gb"); // int fires on 830, phase G
  //load_rom("microtests/build/dmg/int_hblank_nops_scx5.gb"); // int fires on 832, phase A
  //load_rom("microtests/build/dmg/int_hblank_nops_scx6.gb"); // int fires on 834, phase C
  //load_rom("microtests/build/dmg/int_hblank_nops_scx7.gb"); // int fires on 836, phase E

                                                            //runmode = RUN_FAST;

  //load_flat_dump("roms/LinksAwakening_dog.dump");
  //gb->sys_cpu_en = false;

  //load_rom("microtests/build/dmg/int_hblank_nops_scx1.gb");

#endif
}

//----------------------------------------

void GateBoyApp::app_close() {
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_update(double delta) {
  (void)delta;

  SDL_Event event;

  bool load_dump = false;
  bool save_dump = false;
  bool _load_megadump = false;
  bool _save_megadump = false;
  bool reset_sim = false;
  bool toggle_cpu = false;
  int  step_forward = 0;
  int  step_backward = 0;

  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_KEYDOWN) switch (event.key.keysym.sym) {
    case SDLK_F1:     load_dump = true; break;
    case SDLK_F4:     save_dump = true; break;
    case SDLK_F5:     reset_sim = true; break;

    case SDLK_F9:     _load_megadump = true; break;
    case SDLK_F12:    _save_megadump = true; break;

    case SDLK_f:      runmode = RUN_FAST; break;
    case SDLK_v:      runmode = RUN_VSYNC; break;
    case SDLK_s:      runmode = RUN_STEP; break;
    case SDLK_r:      reset_sim = true; break;
    case SDLK_c:      toggle_cpu = true; break;

    case SDLK_UP: {
      stepmode = clamp_val(stepmode + 1, STEP_MIN, STEP_MAX);
      break;
    }

    case SDLK_DOWN: {
      stepmode = clamp_val(stepmode - 1, STEP_MIN, STEP_MAX);
      break;
    }

    case SDLK_LEFT:   {
      step_backward += 1;
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

  if (toggle_cpu) {
    if (gb->sys_cpu_en) {
      printf("Disabling CPU\n");
      gb->sys_cpu_en = 0;
    }
    else {
      printf("Enabling CPU\n");
      gb->sys_cpu_en = 1;
    }
  }

  if (reset_sim) {
    printf("Resetting sim\n");
    gb.reset();
    gb->reset_post_bootrom(rom_buf.data(), rom_buf.size());
  }

  if (_load_megadump) {
    load_megadump("gateboy.megadump");
    _load_megadump = false;
  }

  if (_save_megadump) {
    save_megadump("gateboy.megadump");
    _save_megadump = false;
  }

  if (load_dump) {
    load_raw_dump("gateboy.raw.dump");
    load_dump = false;
  }

  if (save_dump) {
    save_raw_dump("gateboy.raw.dump");
    save_dump = false;
  }

  //----------------------------------------

  if (gb->rom_buf != rom_buf.data()) __debugbreak();

  if (runmode == RUN_FAST) {
    //auto gb = state_manager.state();
    //gb->run(114 * 8 * 8);
    gb.step(114 * 8 * 8, STEP_PHASE);
  }

  else if (runmode == RUN_STEP && step_forward) {
    gb.step(step_forward, stepmode);
    step_forward = 0;
  }

  while(step_backward--) {
    gb.unstep(1, stepmode);
  }

  frame_count++;
}

//-----------------------------------------------------------------------------
// Load/save raw object dump

void GateBoyApp::load_raw_dump(const char* filename) {
  printf("Loading raw dump from %s\n", filename);
  gb.reset();
  gb->load_dump(filename);
}

//----------

void GateBoyApp::save_raw_dump(const char* filename) {
  printf("Saving raw dump to %s\n", filename);
  gb->save_dump(filename);
}

//-----------------------------------------------------------------------------
// Load/save _all_ states in the state manager

void GateBoyApp::load_megadump(const char* filename) {
  printf("Loading megadump %s\n", filename);

  FILE* f = nullptr;
  fopen_s(&f, filename, "rb");
  if (!f) {
    printf("GateBoyApp::load_megadump :: couldn't open %s\n", filename);
    return;
  }

  gb.load_megadump(f);
  gb.scan([](const GateBoy* gb) {
    gb->check_sentinel();
    return true;
  });

  fclose(f);
}

//----------

void GateBoyApp::save_megadump(const char* filename) {
  printf("Saving megadump %s\n", filename);

  FILE* f = nullptr;
  fopen_s(&f, filename, "wb");
  if (!f) {
    printf("GateBoyApp::save_megadump :: couldn't open %s\n", filename);
    return;
  }

  gb.save_megadump(f);
  fclose(f);
}

//-----------------------------------------------------------------------------
// Load a flat (just raw contents of memory addresses, no individual regs) dump
// and copy it into the various regs and memory chunks.

void GateBoyApp::load_flat_dump(const char* filename) {
  printf("Loading flat dump %s\n", filename);

  rom_buf = load_blob(filename);

  gb.reset();
  gb->reset_post_bootrom(rom_buf.data(), rom_buf.size());

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

  gb.reset();
  gb->reset_post_bootrom(rom_buf.data(), rom_buf.size());
  gb->phase_total = 0;
  gb->pass_count = 0;
  gb->pass_total = 0;

  printf("Loaded %zd bytes from rom %s\n", rom_buf.size(), filename);
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_render_frame(Viewport view) {
  grid_painter.render(view);

  const auto& top = gb->top;

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

  dumper("phase %s\n", phases[gb->phase_total & 7]);

  size_t state_size = gb.state_size_bytes();

  if (state_size < 1024 * 1024) {
    dumper("State size          %d K\n",      state_size / 1024);
  }
  else {
    dumper("State size          %d M\n",      state_size / (1024 * 1024));
  }
  dumper("Phase count %8d\n",     gb->phase_total);
  dumper("Pass count  %8d\n",     gb->pass_count);
  dumper("Pass total  %8d\n",     gb->pass_total);
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

  dumper("\n");

  dump_probes(dumper);
  dumper("----------   CPU    ----------\n");
  gb->cpu_blah.dump(dumper);
  top.tim_reg.dump(dumper);
  top.int_reg.dump(dumper, top);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += col_width;
  dumper.clear();

  wire CLK = gb->phase_total & 1;
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

  top.sprite_fetcher.dump(dumper);
  top.sprite_scanner.dump(dumper, top);
  top.sprite_store.dump(dumper);
  top.tile_fetcher.dump(dumper, top);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += col_width;
  dumper.clear();

  dumper("---------- DISASM ----------\n");
  {
    uint16_t pc = gb->cpu_blah.op_addr;
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

    assembler.disassemble(code, code_size, code_base, pc, 30, dumper, /*collapse_nops*/ false);
    text_painter.render(view, dumper.s.c_str(), cursor, 0);
  }
  dumper.clear();

  dump_painter.render(view, cursor, 416, 16, 16, gb->oam_ram);

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
                      960 + 96, 640 + 96, 256, 256);
  }

  // Draw screen and vid ram contents
  if (1) {
    gb_blitter.blit_screen(view, 1280, 32,  2, gb->framebuffer);
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
    blitter.blit(view, overlay_tex, 1024 + 256, 32, 160 * 2, 144 * 2);
  }

  switch(runmode) {
  case RUN_FAST:  dumper("RUN_FAST  ");  break;
  case RUN_VSYNC: dumper("RUN_VSYNC "); break;
  case RUN_STEP:  dumper("RUN_STEP  "); break;
  }
  switch(stepmode) {
  case STEP_PASS:   dumper("STEP_PASS   ");   break;
  case STEP_PHASE:  dumper("STEP_PHASE  ");  break;
  case STEP_CYCLE: dumper("STEP_MCYCLE "); break;
  case STEP_LINE:   dumper("STEP_LINE   ");   break;
  }
  dumper("Sim clock %8.3f ",      double(gb->phase_total) / (4194304.0 * 2));
  dumper("%s", phases[gb->phase_total & 7]);
  dumper("%c", gb->sim_stable ? ' ' : '*');
  dumper("\n");

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


  text_painter.render(view, dumper.s.c_str(), 1280, 32 + 144 * 2);
  dumper.clear();

}

//-----------------------------------------------------------------------------

void GateBoyApp::app_render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
