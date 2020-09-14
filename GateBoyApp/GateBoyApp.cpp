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
  /*
  {
    state_manager.reset();
    auto& gb = *state_manager.state();
    gb.reset_to_bootrom();
    load_blob("roms/tetris.gb", gb.cart_rom, 32768);
    gb.sys_cart_loaded = 1;

    for (int i = 0; i < 8192; i++) {
      gb.vid_ram[i] = (uint8_t)rand();
    }
  }
  */

  state_manager.reset();
  state_manager.state()->reset_post_bootrom();
  load_rom("microtests/build/dmg/timer_int_inc_sled.gb");

  /*
  for (int i = 0; i < 8192; i++) {
    auto gb = state_manager.state();
    gb->vid_ram[i] = (uint8_t)rand();
  }
  */

  //load_flat_dump("roms/LinksAwakening_dog.dump");

  /*
  auto gb = state_manager.state();
  gb->sys_cpu_en = 0;
  gb->run(576);
  gb->sys_cpu_en = 1;
  */
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
    case SDLK_s:      runmode = STEP_FRAME; break;
    case SDLK_r:      reset_sim = true; break;
    case SDLK_c:      toggle_cpu = true; break;

    case SDLK_RIGHT:  {
      if (keyboard_state[SDL_SCANCODE_LCTRL] && keyboard_state[SDL_SCANCODE_LALT]) {
        step_forward += 114 * 8 * 8;
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
      if (keyboard_state[SDL_SCANCODE_LCTRL]) {
        step_backward += 8;
      } else {
        step_backward += 1;
      }
      break;
    }
    }

    if (event.type == SDL_DROPFILE) {
      load_rom(event.drop.file);
      SDL_free(event.drop.file);
    }
  }

  if (runmode == RUN_FAST) {
    step_forward = 114 * 8 * 8;
  }

  if (toggle_cpu) {
    auto gb = state_manager.state();
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
    state_manager.reset();
    state_manager.state()->reset_post_bootrom();
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
// Load/save raw object dump

void GateBoyApp::load_raw_dump(const char* filename) {
  printf("Loading raw dump from %s\n", filename);
  state_manager.reset();
  state_manager.state()->load_dump(filename);
}

//----------

void GateBoyApp::save_raw_dump(const char* filename) {
  printf("Saving raw dump to %s\n", filename);
  state_manager.state()->save_dump(filename);
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

  size_t state_count = state_manager.state_count();
  fread(&state_count, 1, sizeof(state_count), f);

  for (auto s : state_manager.states) {
    delete s;
  }
  state_manager.states.resize(state_count);
  for (size_t i = 0; i < state_count; i++) {
    state_manager.states[i] = new GateBoy();
    fread(state_manager.states[i], 1, sizeof(GateBoy), f);
    state_manager.states[i]->check_sentinel();
  }

  printf("Loaded %zd states\n", state_count);
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

  size_t state_count = state_manager.state_count();
  fwrite(&state_count, 1, sizeof(state_count), f);

  for (size_t i = 0; i < state_count; i++) {
    fwrite(state_manager.states[i], 1, sizeof(GateBoy), f);
  }

  printf("Saved %zd states\n", state_count);
  fclose(f);
}

//-----------------------------------------------------------------------------
// Load a flat (just raw contents of memory addresses, no individual regs) dump
// and copy it into the various regs and memory chunks.

void GateBoyApp::load_flat_dump(const char* filename) {
  printf("Loading flat dump %s\n", filename);

  state_manager.reset();
  state_manager.state()->reset_post_bootrom();

  uint8_t* dump = new uint8_t[65536];
  memset(dump, 0, 65536);
  size_t size = load_blob(filename, dump, 65536);
  
  auto gb = state_manager.state();

  memcpy(gb->cart_rom, dump + 0x0000, 32768);
  memcpy(gb->vid_ram,  dump + 0x8000, 8192);
  memcpy(gb->cart_ram, dump + 0xA000, 8192);
  memcpy(gb->ext_ram,  dump + 0xC000, 8192);
  memcpy(gb->oam_ram,  dump + 0xFE00, 256);
  memcpy(gb->zero_ram, dump + 0xFF80, 128);

  gb->dbg_write(ADDR_BGP,  dump[ADDR_BGP]);
  gb->dbg_write(ADDR_OBP0, dump[ADDR_OBP0]);
  gb->dbg_write(ADDR_OBP1, dump[ADDR_OBP1]);
  gb->dbg_write(ADDR_SCY,  dump[ADDR_SCY]);
  gb->dbg_write(ADDR_SCX,  dump[ADDR_SCX]);
  gb->dbg_write(ADDR_WY,   dump[ADDR_WY]);
  gb->dbg_write(ADDR_WX,   dump[ADDR_WX]);

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

  gb->dbg_write(ADDR_LCDC, dump[ADDR_LCDC]);

  printf("Loaded %zd bytes from dump %s\n", size, filename);
  delete [] dump;
}

//----------

void GateBoyApp::save_flat_dump(const char* filename) {
  printf("TODO %s\n", filename);
}

//------------------------------------------------------------------------------
// Load a standard GB rom

void GateBoyApp::load_rom(const char* filename) {
  printf("Loading %s\n", filename);

  state_manager.reset();
  state_manager.state()->reset_post_bootrom();

  auto gb = state_manager.state();
  size_t size = load_blob(filename, gb->cart_rom, 32768);
  gb->sys_cart_loaded = 1;

  printf("Loaded %zd bytes from rom %s\n", size, filename);
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
  dumper("BGB cycle   0x%08x\n",  gateboy->phase_total / 4);

  dumper("\n");
  dumper("dbg_req ");
  dump_req(dumper, gateboy->dbg_req);
  dumper("cpu_req ");
  dump_req(dumper, gateboy->bus_req);

  dumper("\n");

  dump_probes(dumper);
  dumper("----------   CPU    ----------\n");
  gateboy->cpu.dump(dumper);
  top.tim_reg.dump(dumper);
  top.int_reg.dump(dumper, top);
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

  top.sprite_fetcher.dump(dumper);
  top.sprite_scanner.dump(dumper, top);
  top.sprite_store.dump(dumper);
  top.tile_fetcher.dump(dumper, top);
  text_painter.render(view, dumper.s.c_str(), cursor, 0);
  cursor += col_width;
  dumper.clear();

  dumper("---------- DISASM ----------\n");
  if (gateboy->top.bootrom.BOOT_BITn.qp()) {
    uint16_t pc = gateboy->cpu.op_addr;
    Assembler a;
    a.disassemble(gateboy->cart_rom, 32768,
                  ADDR_CART_ROM_BEGIN, pc,
                  30, dumper, false);
  }
  else {
    uint16_t pc = gateboy->cpu.op_addr;
    Assembler a;
    a.disassemble(DMG_ROM_bin, 256,
                  ADDR_CART_ROM_BEGIN, pc,
                  30, dumper, false);
  }
  text_painter.render(view, dumper.s.c_str(), cursor, 512 - 160);
  cursor += col_width;
  dumper.clear();

  //dump_painter.render(view, cursor, 512,      16, 16, DMG_ROM_bin);

  //dump_painter.render(view, cursor, 512, 16, 16, gateboy->oam_ram);
  //dump_painter.render(view, cursor, 768, 16,  8, gateboy->zero_ram);

  //dump_painter.render(view, col_width * 4 + 128, 0, 4, 64, gateboy->mem + 0x0000);

  gb_blitter.blit_screen(view, 1024 + 256, 32, 2, gateboy->framebuffer);

  //update_texture_u32(trace_tex, 912, 154, trace);
  //blitter.blit(view, trace_tex, 0, 0, 912, 154);

  /*
  $FFFF	Interrupt Enable Flag
  $FF80-$FFFE	Zero Page - 127 bytes
  $FF00-$FF7F	Hardware I/O Registers
  $FEA0-$FEFF	Unusable Memory
  $FE00-$FE9F	OAM - Object Attribute Memory
  $E000-$FDFF	Echo RAM - Reserved, Do Not Use
  $D000-$DFFF	Internal RAM - Bank 1-7 (switchable - CGB only)
  $C000-$CFFF	Internal RAM - Bank 0 (fixed)

  */

  update_texture_u8(ram_tex, 0x00, 0x00, 256, 128, gateboy->cart_rom);
  update_texture_u8(ram_tex, 0x00, 0x80, 256,  32, gateboy->vid_ram);
  update_texture_u8(ram_tex, 0x00, 0xA0, 256,  32, gateboy->cart_ram);
  update_texture_u8(ram_tex, 0x00, 0xC0, 256,  32, gateboy->ext_ram);
  update_texture_u8(ram_tex, 0x00, 0xFE, 256,   1, gateboy->oam_ram);
  update_texture_u8(ram_tex, 0x80, 0xFF, 128,   1, gateboy->zero_ram);

  //void update_texture_u8(int tex, int dx, int dy, int dw, int dh, const uint8_t* pix) {
  blitter.blit_mono(view, ram_tex, 256, 256,
                    0, 0, 256, 256,
                    1056, 736, 256, 256);


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

  gb_blitter.blit_tiles (view, 1632, 32,     gateboy->vid_ram);
  gb_blitter.blit_map   (view, 1344, 448, 1, gateboy->vid_ram, 0, 0);
  gb_blitter.blit_map   (view, 1632, 448, 1, gateboy->vid_ram, 0, 1);
  gb_blitter.blit_map   (view, 1344, 736, 1, gateboy->vid_ram, 1, 0);
  gb_blitter.blit_map   (view, 1632, 736, 1, gateboy->vid_ram, 1, 1);
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
