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

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  App* app = new GateBoyApp();
  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_init() {
  LOG_G("GateBoyApp::app_init()\n");
  LOG_SCOPE_INDENT();

  grid_painter.init();
  text_painter.init();
  dump_painter.init();
  gb_blitter.init();
  blitter.init();

  trace_tex = create_texture_u32(912, 154);
  ram_tex = create_texture_u8(256, 256);
  overlay_tex = create_texture_u32(160, 144);
  keyboard_state = SDL_GetKeyboardState(nullptr);

#if 0
  const char* app = R"(
  0150:
    ld a, $55
    ld hl, $8000
    ld (hl), a
    jr -3
  )";

  Assembler as;
  as.assemble(app);
  blob boot = as.link();

  gb_thread.reset_cart(DMG_ROM_blob, boot);
#endif

#if 0
  // regenerate post-bootrom dump
  gb_thread.reset_boot(DMG_ROM_blob, load_blob("roms/tetris.gb"));
  for (int i = 0; i < 8192; i++) {
    gb_thread.gb->vid_ram[i] = (uint8_t)rand();
  }
#endif


#if 0
  // run tiny app
  if (1) {
    std::string app = R"(
    0150:
      ld a, $00
      ldh ($40), a
      ld a, $73
      ld hl, $8000
      ld (hl), a
      ld hl, $809F
      ld (hl), a

      ld hl, $FF80
      ld a, $E0
      ld (hl+), a
      ld a, $46
      ld (hl+), a
      ld a, $3E
      ld (hl+), a
      ld a, $28
      ld (hl+), a
      ld a, $3D
      ld (hl+), a
      ld a, $20
      ld (hl+), a
      ld a, $FD
      ld (hl+), a
      ld a, $C9
      ld (hl+), a

      ld a, $80
      call $ff80

      ld a, $00
      ld hl, $8000
      add (hl)
      jr -2
    )";

    Assembler as;
    as.assemble(app.c_str());
    blob rom = as.link();

    gb->reset_post_bootrom(rom.data(), rom.size());
  }
#endif

  load_flat_dump("roms/LinksAwakening_dog.dump");
  gb_thread.gb->sys_cpu_en = false;
  gb_thread.gb->phase_total = 0;
  gb_thread.gb->pass_count = 0;
  gb_thread.gb->pass_total = 0;

  /*

  {
    memset(gb->oam_ram, 0, 160);
    memset(gb->vid_ram, 0, 8192);
    uint8_t* cursor = gb->vid_ram;
    for (int i = 0; i < 384; i++) {
      *cursor++ = 0b11111111;
      *cursor++ = 0b11111111;
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;f
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;
      *cursor++ = 0b10101011;
      *cursor++ = 0b10000001;
      *cursor++ = 0b11111111;
      *cursor++ = 0b11111111;
    }
  }
  */

  /*
  for (int i = 0; i < 2048; i += 2) {
    gb->vid_ram[i + 0] = 0xFF;
    gb->vid_ram[i + 1] = 0x00;
  }
  memset(&gb->vid_ram[1024 * 2], 0x00, 1024 * 4);

  for (int i = 0; i < 160; i+= 4) {
    gb->oam_ram[i + 0] = 0xFF;
    gb->oam_ram[i + 1] = 0xFF;
  }

  gb->oam_ram[0] = 17;
  gb->oam_ram[1] = 8;
  */


  //gb->pix_pipe.set_wx(7);
  //gb->pix_pipe.set_wy(16);

  // run rom

  //load_rom   ("roms/mealybug/m3_lcdc_win_en_change_multiple_wx.gb");
  //load_golden("roms/mealybug/m3_lcdc_win_en_change_multiple_wx.bmp");

  //load_rom("microtests/build/dmg/oam_read_l0_d.gb");

  LOG_G("GateBoyApp::app_init() done\n");
  gb_thread.start();
}

//-----------------------------------------------------------------------------

void GateBoyApp::app_close() {
  gb_thread.stop();
}

//------------------------------------------------------------------------------

void GateBoyApp::load_raw_dump() {
  printf("Loading raw dump from %s\n", "gateboy.raw.dump");
  gb_thread.gb->load_dump("gateboy.raw.dump");
  gb_thread.reset_cart(DMG_ROM_blob, gb_thread.cart);
}

void GateBoyApp::save_raw_dump() {
  printf("Saving raw dump to %s\n", "gateboy.raw.dump");
  gb_thread.gb->save_dump("gateboy.raw.dump");
}

//------------------------------------------------------------------------------
// Load a standard GB rom

void GateBoyApp::load_rom(const char* filename) {
  printf("Loading %s\n", filename);

  gb_thread.reset_cart(DMG_ROM_blob, load_blob(filename));
  gb_thread.gb->phase_total = 0;
  gb_thread.gb->pass_count = 0;
  gb_thread.gb->pass_total = 0;

  printf("Loaded %zd bytes from rom %s\n", gb_thread.cart.size(), filename);
}


//-----------------------------------------------------------------------------
// Load a flat (just raw contents of memory addresses, no individual regs) dump
// and copy it into the various regs and memory chunks.

void GateBoyApp::load_flat_dump(const char* filename) {

  gb_thread.reset_cart(DMG_ROM_blob, load_blob(filename));

  memcpy(gb_thread.gb->vid_ram,  gb_thread.cart.data() + 0x8000, 8192);
  memcpy(gb_thread.gb->cart_ram, gb_thread.cart.data() + 0xA000, 8192);
  memcpy(gb_thread.gb->ext_ram,  gb_thread.cart.data() + 0xC000, 8192);
  memcpy(gb_thread.gb->oam_ram,  gb_thread.cart.data() + 0xFE00, 256);
  memcpy(gb_thread.gb->zero_ram, gb_thread.cart.data() + 0xFF80, 128);

  gb_thread.gb->dbg_write(ADDR_BGP,  gb_thread.cart[ADDR_BGP]);
  gb_thread.gb->dbg_write(ADDR_OBP0, gb_thread.cart[ADDR_OBP0]);
  gb_thread.gb->dbg_write(ADDR_OBP1, gb_thread.cart[ADDR_OBP1]);
  gb_thread.gb->dbg_write(ADDR_SCY,  gb_thread.cart[ADDR_SCY]);
  gb_thread.gb->dbg_write(ADDR_SCX,  gb_thread.cart[ADDR_SCX]);
  gb_thread.gb->dbg_write(ADDR_WY,   gb_thread.cart[ADDR_WY]);
  gb_thread.gb->dbg_write(ADDR_WX,   gb_thread.cart[ADDR_WX]);

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

  gb_thread.gb->dbg_write(ADDR_LCDC, gb_thread.cart[ADDR_LCDC]);
}


//-----------------------------------------------------------------------------

void GateBoyApp::app_update(double /*delta*/) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_KEYDOWN)
    switch (event.key.keysym.sym) {

    case SDLK_SPACE: { gb_thread.sig_pause ? gb_thread.resume() : gb_thread.pause(); break; }

    case SDLK_d: {
      gb_thread.gb->sys_statediff = !gb_thread.gb->sys_statediff;
      break;
    }

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
    case SDLK_r:    gb_thread.reset_cart(gb_thread.boot, gb_thread.cart);          break;
    //case SDLK_d:    show_diff   = !show_diff;   break;
    case SDLK_g:    show_golden = !show_golden; break;
    case SDLK_o:    draw_passes = !draw_passes; break;
    case SDLK_UP:   stepmode = STEP_PHASE;      break;
    case SDLK_DOWN: stepmode = STEP_PASS;       break;
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

#if 0
void dump_cpu_bus(Dumper& d, const CpuBus& /*cpu_bus*/) {
  d("\002===== CPU Bus =====\001\n");
  d("BUS ADDR 0x%04x  %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    pack_u16(16, &cpu_bus.BUS_CPU_A00),
    cpu_bus.BUS_CPU_A15.c(), cpu_bus.BUS_CPU_A14.c(), cpu_bus.BUS_CPU_A13.c(), cpu_bus.BUS_CPU_A12.c(),
    cpu_bus.BUS_CPU_A11.c(), cpu_bus.BUS_CPU_A10.c(), cpu_bus.BUS_CPU_A09.c(), cpu_bus.BUS_CPU_A08.c(),
    cpu_bus.BUS_CPU_A07.c(), cpu_bus.BUS_CPU_A06.c(), cpu_bus.BUS_CPU_A05.c(), cpu_bus.BUS_CPU_A04.c(),
    cpu_bus.BUS_CPU_A03.c(), cpu_bus.BUS_CPU_A02.c(), cpu_bus.BUS_CPU_A01.c(), cpu_bus.BUS_CPU_A00.c());
  d("BUS DATA 0x%02x    %c%c%c%c%c%c%c%c\n",
    pack_u8(8, &cpu_bus.BUS_CPU_D0p),
    cpu_bus.BUS_CPU_D7p.c(), cpu_bus.BUS_CPU_D6p.c(), cpu_bus.BUS_CPU_D5p.c(), cpu_bus.BUS_CPU_D4p.c(),
    cpu_bus.BUS_CPU_D3p.c(), cpu_bus.BUS_CPU_D2p.c(), cpu_bus.BUS_CPU_D1p.c(), cpu_bus.BUS_CPU_D0p.c());
  d("\n");

  d("PIN_CPU_6         %c\n", cpu_bus.PIN_CPU_6.c());
  d("PIN_CPU_LATCH_EXT %c\n", cpu_bus.PIN_CPU_LATCH_EXT.c());
  d("PIN_CPU_RDp       %c\n", cpu_bus.PIN_CPU_RDp.c());
  d("PIN_CPU_WRp       %c\n", cpu_bus.PIN_CPU_WRp.c());
  d("PIN_CPU_EXT_BUSp  %c\n", cpu_bus.PIN_CPU_EXT_BUSp.c());
  d("\n");

  d("PIN_CPU_BOOTp     %c\n", cpu_bus.PIN_CPU_BOOTp.c());
  d("PIN_CPU_ADDR_HIp  %c\n", cpu_bus.PIN_CPU_ADDR_HIp.c());
  d("PIN STARTp        %d\n", cpu_bus.PIN_CPU_STARTp.qp());
  d("PIN SYS_RSTp      %d\n", cpu_bus.PIN_CPU_SYS_RSTp.qp());
  d("PIN EXT_RST       %d\n", cpu_bus.PIN_CPU_EXT_RST.qp());
  d("PIN UNOR_DBG      %d\n", cpu_bus.PIN_CPU_UNOR_DBG.qp());
  d("PIN UMUT_DBG      %d\n", cpu_bus.PIN_CPU_UMUT_DBG.qp());
  d("PIN EXT_CLKGOOD   %d\n", cpu_bus.PIN_CPU_EXT_CLKGOOD.qp());
  d("\n");

  d("PIN BOWA_xBCDEFGH %d\n", cpu_bus.PIN_CPU_BOWA_Axxxxxxx.qp());
  d("PIN BEDO_Axxxxxxx %d\n", cpu_bus.PIN_CPU_BEDO_xBCDEFGH.qp());
  d("PIN BEKO_ABCDxxxx %d\n", cpu_bus.PIN_CPU_BEKO_ABCDxxxx.qp());
  d("PIN BUDE_xxxxEFGH %d\n", cpu_bus.PIN_CPU_BUDE_xxxxEFGH.qp());
  d("PIN BOLO_ABCDEFxx %d\n", cpu_bus.PIN_CPU_BOLO_ABCDEFxx.qp());
  d("PIN BUKE_AxxxxxGH %d\n", cpu_bus.PIN_CPU_BUKE_AxxxxxGH.qp());
  d("PIN BOMA_Axxxxxxx %d\n", cpu_bus.PIN_CPU_BOMA_xBCDEFGH.qp());
  d("PIN BOGA_xBCDEFGH %d\n", cpu_bus.PIN_CPU_BOGA_Axxxxxxx.qp());
  d("\n");
}
#endif

//-----------------------------------------------------------------------------

#if 0
void dump_ext_bus(Dumper& d, const ExtBus& ext_bus) {
  d("\002===== Ext Bus =====\001\n");
  d("PIN CLK    : %c\n", ext_bus.PIN_EXT_CLK.c());
  d("PIN RDn    : %c\n", ext_bus.PIN_EXT_RDn.c());
  d("PIN WRn    : %c\n", ext_bus.PIN_EXT_WRn.c());
  d("PIN CSn    : %c\n", ext_bus.PIN_EXT_CSn.c());
  d("\n");

  d("PIN ADDR   : 0x%04x %c%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    pack_u16(16, &ext_bus.PIN_EXT_A00p),
    ext_bus.PIN_EXT_A15p.c(), ext_bus.PIN_EXT_A14p.c(), ext_bus.PIN_EXT_A13p.c(), ext_bus.PIN_EXT_A12p.c(),
    ext_bus.PIN_EXT_A11p.c(), ext_bus.PIN_EXT_A10p.c(), ext_bus.PIN_EXT_A09p.c(), ext_bus.PIN_EXT_A08p.c(),
    ext_bus.PIN_EXT_A07p.c(), ext_bus.PIN_EXT_A06p.c(), ext_bus.PIN_EXT_A05p.c(), ext_bus.PIN_EXT_A04p.c(),
    ext_bus.PIN_EXT_A03p.c(), ext_bus.PIN_EXT_A02p.c(), ext_bus.PIN_EXT_A01p.c(), ext_bus.PIN_EXT_A00p.c());
  d("PIN DATA   : 0x%02x %c%c%c%c%c%c%c%c\n",
    pack_u8(8, &ext_bus.PIN_EXT_D00p),
    ext_bus.PIN_EXT_D07p.c(), ext_bus.PIN_EXT_D06p.c(), ext_bus.PIN_EXT_D05p.c(), ext_bus.PIN_EXT_D04p.c(),
    ext_bus.PIN_EXT_D03p.c(), ext_bus.PIN_EXT_D02p.c(), ext_bus.PIN_EXT_D01p.c(), ext_bus.PIN_EXT_D00p.c());
  d("\n");

  d("ADDR LATCH : 0x%04x _%c%c%c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    pack_u16(15, &ext_bus.ALOR_EXT_ADDR_LATCH_00p),
    ext_bus.NYRE_EXT_ADDR_LATCH_14p.c(), ext_bus.LONU_EXT_ADDR_LATCH_13p.c(), ext_bus.LOBU_EXT_ADDR_LATCH_12p.c(), ext_bus.LUMY_EXT_ADDR_LATCH_11p.c(),
    ext_bus.PATE_EXT_ADDR_LATCH_10p.c(), ext_bus.LYSA_EXT_ADDR_LATCH_09p.c(), ext_bus.LUNO_EXT_ADDR_LATCH_08p.c(), ext_bus.ARYM_EXT_ADDR_LATCH_07p.c(),
    ext_bus.AROS_EXT_ADDR_LATCH_06p.c(), ext_bus.ATEV_EXT_ADDR_LATCH_05p.c(), ext_bus.AVYS_EXT_ADDR_LATCH_04p.c(), ext_bus.ARET_EXT_ADDR_LATCH_03p.c(),
    ext_bus.ALYR_EXT_ADDR_LATCH_02p.c(), ext_bus.APUR_EXT_ADDR_LATCH_01p.c(), ext_bus.ALOR_EXT_ADDR_LATCH_00p.c());
  d("DATA LATCH : 0x%02x %c%c%c%c%c%c%c%c\n",
    pack_u8n(8, &ext_bus.SOMA_EXT_DATA_LATCH_D0n),
    ext_bus.SAZY_EXT_DATA_LATCH_D7n.c(), ext_bus.RUPA_EXT_DATA_LATCH_D6n.c(), ext_bus.SAGO_EXT_DATA_LATCH_D5n.c(), ext_bus.SODY_EXT_DATA_LATCH_D4n.c(),
    ext_bus.SELO_EXT_DATA_LATCH_D3n.c(), ext_bus.RAXY_EXT_DATA_LATCH_D2n.c(), ext_bus.RONY_EXT_DATA_LATCH_D1n.c(), ext_bus.SOMA_EXT_DATA_LATCH_D0n.c());
  d("\n");
}
#endif

//-----------------------------------------------------------------------------

void dump_vram_bus(Dumper& d, const VramBus& vram_bus) {
  d("\002===== VRAM Bus =====\001\n");

  /*
  d("VRAM BUS ADDR : %04x %c%c%c%c%c:%c%c%c%c%c%c%c%c\n",
    pack_u16n(13, &vram_bus.BUS_VRAM_A00n) | 0x8000,
    vram_bus.BUS_VRAM_A12n.c(), vram_bus.BUS_VRAM_A11n.c(), vram_bus.BUS_VRAM_A10n.c(), vram_bus.BUS_VRAM_A09n.c(),
    vram_bus.BUS_VRAM_A08n.c(), vram_bus.BUS_VRAM_A07n.c(), vram_bus.BUS_VRAM_A06n.c(), vram_bus.BUS_VRAM_A05n.c(),
    vram_bus.BUS_VRAM_A04n.c(), vram_bus.BUS_VRAM_A03n.c(), vram_bus.BUS_VRAM_A02n.c(), vram_bus.BUS_VRAM_A01n.c(),
    vram_bus.BUS_VRAM_A00n.c());
  d("VRAM BUS DATA : %02x %c%c%c%c%c%c%c%c\n",
    pack_u8(8, &vram_bus.BUS_VRAM_D0p),
    vram_bus.BUS_VRAM_D7p.c(), vram_bus.BUS_VRAM_D6p.c(), vram_bus.BUS_VRAM_D5p.c(), vram_bus.BUS_VRAM_D4p.c(),
    vram_bus.BUS_VRAM_D3p.c(), vram_bus.BUS_VRAM_D2p.c(), vram_bus.BUS_VRAM_D1p.c(), vram_bus.BUS_VRAM_D0p.c());
  d("\n");

  d("VRAM PIN MCSn : %c\n", vram_bus.PIN_VRAM_CSn.c());
  d("VRAM PIN MOEn : %c\n", vram_bus.PIN_VRAM_OEn.c());
  d("VRAM PIN MWRn : %c\n", vram_bus.PIN_VRAM_WRn.c());

  d("VRAM PIN ADDR : 0x%04x\n", pack_u16(13, &vram_bus.PIN_VRAM_A00p) | 0x8000);
  d("VRAM PIN DATA : %02x %c%c%c%c%c%c%c%c\n",
    pack_u8(8, &vram_bus.PIN_VRAM_D00p),
    vram_bus.PIN_VRAM_D07p.c(), vram_bus.PIN_VRAM_D06p.c(), vram_bus.PIN_VRAM_D05p.c(), vram_bus.PIN_VRAM_D04p.c(),
    vram_bus.PIN_VRAM_D03p.c(), vram_bus.PIN_VRAM_D02p.c(), vram_bus.PIN_VRAM_D01p.c(), vram_bus.PIN_VRAM_D00p.c());
  d("\n");
  */

  d("TILE_DA       : 0x%02x\n", pack_u8n(8, &vram_bus.LEGU_TILE_DA0n));
  d("TILE_DB       : 0x%02x\n", pack_u8 (8, &vram_bus.RAWU_TILE_DB0p));
  d("SPRITE_DA     : 0x%02x\n", pack_u8n(8, &vram_bus.REWO_SPRITE_DA0n));
  d("SPRITE_DB     : 0x%02x\n", pack_u8n(8, &vram_bus.PEFO_SPRITE_DB0n));
  d("\n");
}

//-----------------------------------------------------------------------------

void dump_oam_bus(Dumper& d, const OamBus& oam_bus) {
  d("\002===== OAM Bus =====\001\n");

#if 0
  d("PIN_OAM_CLK    %c\n", oam_bus.PIN_OAM_CLK.c());
  d("PIN_OAM_OE     %c\n", oam_bus.PIN_OAM_OEn.c());
  d("PIN_OAM_WR_A   %c\n", oam_bus.PIN_OAM_WR_A.c());
  d("PIN_OAM_WR_B   %c\n", oam_bus.PIN_OAM_WR_B.c());

  d("OAM TRI ADDR   %03d %02x %c%c%c%c%c%c%c%c\n",
    pack_u8(8, &oam_bus.BUS_OAM_A0n),
    pack_u8(8, &oam_bus.BUS_OAM_A0n),
    oam_bus.BUS_OAM_A7n.cn(), oam_bus.BUS_OAM_A6n.cn(), oam_bus.BUS_OAM_A5n.cn(), oam_bus.BUS_OAM_A4n.cn(),
    oam_bus.BUS_OAM_A3n.cn(), oam_bus.BUS_OAM_A2n.cn(), oam_bus.BUS_OAM_A1n.cn(), oam_bus.BUS_OAM_A0n.cn());

  d("OAM BUS ADDR   %03d %02x -%c%c%c%c%c%c%c\n",
    pack_u8n(7, &oam_bus.BUS_OAM_A1n),
    pack_u8n(7, &oam_bus.BUS_OAM_A1n),
    oam_bus.BUS_OAM_A7n.cn(), oam_bus.BUS_OAM_A6n.cn(), oam_bus.BUS_OAM_A5n.cn(), oam_bus.BUS_OAM_A4n.cn(),
    oam_bus.BUS_OAM_A3n.cn(), oam_bus.BUS_OAM_A2n.cn(), oam_bus.BUS_OAM_A1n.cn());

  d("OAM BUS DATA A %03d %02x %c%c%c%c%c%c%c%c\n",
    pack_u8n(8, &oam_bus.BUS_OAM_DA0n),
    pack_u8n(8, &oam_bus.BUS_OAM_DA0n),
    oam_bus.BUS_OAM_DA7n.c(), oam_bus.BUS_OAM_DA6n.c(), oam_bus.BUS_OAM_DA5n.c(), oam_bus.BUS_OAM_DA4n.c(),
    oam_bus.BUS_OAM_DA3n.c(), oam_bus.BUS_OAM_DA2n.c(), oam_bus.BUS_OAM_DA1n.c(), oam_bus.BUS_OAM_DA0n.c());

  d("OAM BUS DATA B %03d %02x %c%c%c%c%c%c%c%c\n",
    pack_u8n(8, &oam_bus.BUS_OAM_DB0n),
    pack_u8n(8, &oam_bus.BUS_OAM_DB0n),
    oam_bus.BUS_OAM_DB7n.c(), oam_bus.BUS_OAM_DB6n.c(), oam_bus.BUS_OAM_DB5n.c(), oam_bus.BUS_OAM_DB4n.c(),
    oam_bus.BUS_OAM_DB3n.c(), oam_bus.BUS_OAM_DB2n.c(), oam_bus.BUS_OAM_DB1n.c(), oam_bus.BUS_OAM_DB0n.c());
#endif

  d("MAKA_HOLD_MEMp   %c\n", oam_bus.MAKA_HOLD_MEMp.c());
  d("WUJE_CPU_OAM_WRn %c\n", oam_bus.WUJE_CPU_OAM_WRn.c());

  d("OAM LATCH A    %03d %02x %c%c%c%c%c%c%c%c\n",
    pack_u8(8, &oam_bus.YDYV_OAM_LATCH_DA0n),
    pack_u8(8, &oam_bus.YDYV_OAM_LATCH_DA0n),
    oam_bus.ZECA_OAM_LATCH_DA7n.c(), oam_bus.YSES_OAM_LATCH_DA6n.c(), oam_bus.XAFU_OAM_LATCH_DA5n.c(), oam_bus.ZAXE_OAM_LATCH_DA4n.c(),
    oam_bus.WONE_OAM_LATCH_DA3n.c(), oam_bus.ZUCA_OAM_LATCH_DA2n.c(), oam_bus.YCEB_OAM_LATCH_DA1n.c(), oam_bus.YDYV_OAM_LATCH_DA0n.c());

  d("OAM LATCH B    %03d %02x %c%c%c%c%c%c%c%c\n",
    pack_u8(8, &oam_bus.XYKY_OAM_LATCH_DB0n),
    pack_u8(8, &oam_bus.XYKY_OAM_LATCH_DB0n),
    oam_bus.ECED_OAM_LATCH_DB7n.c(), oam_bus.ZUVE_OAM_LATCH_DB6n.c(), oam_bus.CYRA_OAM_LATCH_DB5n.c(), oam_bus.WYNO_OAM_LATCH_DB4n.c(),
    oam_bus.YVEL_OAM_LATCH_DB3n.c(), oam_bus.YSEX_OAM_LATCH_DB2n.c(), oam_bus.YRUM_OAM_LATCH_DB1n.c(), oam_bus.XYKY_OAM_LATCH_DB0n.c());

  d("OAM TEMP A     %03d %02x %c%c%c%c%c%c%c%c\n",
    pack_u8(8, &oam_bus.XUSO_OAM_DA0p),
    pack_u8(8, &oam_bus.XUSO_OAM_DA0p),
    oam_bus.YZAB_OAM_DA7p.c(), oam_bus.XOTE_OAM_DA6p.c(), oam_bus.WYSO_OAM_DA5p.c(), oam_bus.YBOG_OAM_DA4p.c(),
    oam_bus.XYJU_OAM_DA3p.c(), oam_bus.YJEX_OAM_DA2p.c(), oam_bus.XEGU_OAM_DA1p.c(), oam_bus.XUSO_OAM_DA0p.c());

  d("OAM TEMP B     %03d %2x %c%c%c%c%c%c%c%c\n",
    pack_u8(8, &oam_bus.YLOR_OAM_DB0p),
    pack_u8(8, &oam_bus.YLOR_OAM_DB0p),
    oam_bus.DEPO_OAM_DB7p.c(), oam_bus.YZOS_OAM_DB6p.c(), oam_bus.BAXO_OAM_DB5p.c(), oam_bus.GOMO_OAM_DB4p.c(),
    oam_bus.ZEZY_OAM_DB3p.c(), oam_bus.ZYVE_OAM_DB2p.c(), oam_bus.ZYTY_OAM_DB1p.c(), oam_bus.YLOR_OAM_DB0p.c());

  d("\n");
}

//-----------------------------------------------------------------------------

#pragma warning(disable:4189)

void GateBoyApp::app_render_frame(Viewport view) {
  gb_thread.pause();

  grid_painter.render(view);

  const auto gb = gb_thread.gb.state();

  uint8_t* framebuffer = gb->framebuffer;
  uint8_t* vid_ram = gb->vid_ram;
  int64_t phase_total = gb->phase_total;
  bool sim_stable = gb->sim_stable;

  StringDumper d;
  float cursor = 0;

  //----------------------------------------

  d("\002===== Top =====\001\n");

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

  d("phase %s\n", phases[gb->phase_total & 7]);

  d("State count %d\n", gb_thread.gb.state_count());
  size_t state_size = gb_thread.gb.state_size_bytes();
  if (state_size < 1024 * 1024) {
    d("State size  %d K\n", state_size / 1024);
  }
  else {
    d("State size  %d M\n", state_size / (1024 * 1024));
  }
  d("Phase count %d\n",      gb->phase_total);
  d("Pass count  %d\n",      gb->pass_count);
  d("Pass total  %d\n",      gb->pass_total);
  d("Pass avg    %4.2f\n",   float(gb->pass_total) / float(gb->phase_total));
  d("Pass hash   %016llx\n", gb->pass_hash);
  d("Total hash  %016llx\n", gb->total_hash);
  d("BGB cycle   0x%08x\n",  (gb->phase_total / 4) - 0x10000);
  d("Sim clock   %f\n",      double(gb->phase_total) / (4194304.0 * 2));

  d("Commands left %d\n",    uint8_t(gb_thread.cursor_w - gb_thread.cursor_r));
  d("Steps left    %d\n",    gb_thread.command.count);
  d("Sim time      %f\n",    gb->sim_time);
  d("Waiting       %d\n",    gb_thread.sig_waiting.load());

  double phase_rate = (gb->phase_total - gb_thread.old_phase_total) / (gb->sim_time - gb_thread.old_sim_time);

  if (gb->sim_time == gb_thread.old_sim_time) {
    phase_rate = 0;
  }

  gb_thread.phase_rate_smooth = (gb_thread.phase_rate_smooth * 0.99) + (phase_rate * 0.01);

  d("Phase rate    %f\n",      gb_thread.phase_rate_smooth);
  gb_thread.old_phase_total = gb->phase_total;
  gb_thread.old_sim_time = gb->sim_time;

  /*
  d("Pass count  %lld\n",    pass_count);
  d("Pass rate   %f\n",      double(pass_count) / sim_time);
  d("Phase count %lld\n",    phase_count);
  d("Phase rate  %f\n",      double(phase_count) / sim_time);
  */

  d("\n");
  d("dbg_req ");
  dump_req(d, gb->dbg_req);
  d("cpu_req ");
  dump_req(d, gb->cpu_req);
  d("bus_req ");
  dump_req(d, gb->bus_req);
  d("cpu_data_latch %d 0x%02x\n", gb->cpu_data_latch, gb->cpu_data_latch);
  d("\n");

  d("\002===== GateBoy =====\001\n");

  d("sys_rst        %d\n", gb->sys_rst);
  d("sys_t1         %d\n", gb->sys_t1);
  d("sys_t2         %d\n", gb->sys_t2);
  d("sys_clken      %d\n", gb->sys_clken);
  d("sys_clkgood    %d\n", gb->sys_clkgood);
  d("sys_cpuready   %d\n", gb->sys_cpuready);
  d("sys_cpu_en     %d\n", gb->sys_cpu_en);
  d("sys_buttons    %d\n", gb->sys_buttons);
  d("sys_fastboot   %d\n", gb->sys_fastboot);


  d("screen_x       %d\n", gb->gb_screen_x);
  d("screen_y       %d\n", gb->gb_screen_y);
  d("lcd_data_latch %d\n", gb->lcd_data_latch);
  d("lcd_pix_lo     %c\n",  gb->lcd_pix_lo.c());
  d("lcd_pix_hi     %c\n",  gb->lcd_pix_hi.c());

  d("lcd_pipe_lo    %c%c%c%c%c%c%c%c\n",
    gb->lcd_pipe_lo[0].c(), gb->lcd_pipe_lo[1].c(), gb->lcd_pipe_lo[2].c(), gb->lcd_pipe_lo[3].c(),
    gb->lcd_pipe_lo[4].c(), gb->lcd_pipe_lo[5].c(), gb->lcd_pipe_lo[6].c(), gb->lcd_pipe_lo[7].c());
  d("lcd_pipe_hi    %c%c%c%c%c%c%c%c\n",
    gb->lcd_pipe_hi[0].c(), gb->lcd_pipe_hi[1].c(), gb->lcd_pipe_hi[2].c(), gb->lcd_pipe_hi[3].c(),
    gb->lcd_pipe_hi[4].c(), gb->lcd_pipe_hi[5].c(), gb->lcd_pipe_hi[6].c(), gb->lcd_pipe_hi[7].c());

  d("\n");

  d("\002===== CPU =====\001\n");
  d("state    %d\n", gb->cpu.state);
  d("state_   %d\n", gb->cpu.state_);
  d("op addr  0x%04x\n", gb->cpu.op_addr);
  d("opcode   0x%02x\n", gb->cpu.op);
  d("opname   '%s' @ %d\n", op_strings2[gb->cpu.op], gb->cpu.state);
  d("CB       0x%02x\n", gb->cpu.cb);
  d("in       0x%02x\n", gb->cpu.in);
  d("out      0x%02x\n", gb->cpu.out);
  d("\n");
  d("bus req   ");
  dump_req(d, gb->cpu.bus_req);
  d("PC        0x%04x 0x%02x 0x%02x\n", gb->cpu.pc, gb->cpu.pcl, gb->cpu.pch);
  d("SP        0x%04x 0x%02x 0x%02x\n", gb->cpu.sp, gb->cpu.sph, gb->cpu.spl);
  d("XY        0x%04x 0x%02x 0x%02x\n", gb->cpu.xy, gb->cpu.xyh, gb->cpu.xyl);
  d("BC        0x%04x 0x%02x 0x%02x\n", gb->cpu.bc, gb->cpu.b,   gb->cpu.c);
  d("DE        0x%04x 0x%02x 0x%02x\n", gb->cpu.de, gb->cpu.d,   gb->cpu.e);
  d("HL        0x%04x 0x%02x 0x%02x\n", gb->cpu.hl, gb->cpu.h,   gb->cpu.l);
  d("AF        0x%04x 0x%02x 0x%02x\n", gb->cpu.af, gb->cpu.a,   gb->cpu.f);
  d("alu_f     0x%02x\n", gb->cpu.alu_f);
  d("\n");
  d("IME       %d\n", gb->cpu.ime);
  d("IME_      %d\n", gb->cpu.ime_delay);
  d("interrupt %d\n", gb->cpu.op == 0xF4);
  d("int_ack   0x%02x\n", gb->cpu.int_ack);
  d("\n");

  d("\002===== Ints =====\001\n");
  d("IE_D0              %c\n", gb->IE_D0.c());
  d("IE_D1              %c\n", gb->IE_D1.c());
  d("IE_D2              %c\n", gb->IE_D2.c());
  d("IE_D3              %c\n", gb->IE_D3.c());
  d("IE_D4              %c\n", gb->IE_D4.c());
  d("\n");

  d("LOPE_FF0F_0        %c\n", gb->int_reg.LOPE_FF0F_D0p.c());
  d("LALU_FF0F_1        %c\n", gb->int_reg.LALU_FF0F_D1p.c());
  d("NYBO_FF0F_2        %c\n", gb->int_reg.NYBO_FF0F_D2p.c());
  d("UBUL_FF0F_3        %c\n", gb->int_reg.UBUL_FF0F_D3p.c());
  d("ULAK_FF0F_4        %c\n", gb->int_reg.ULAK_FF0F_D4p.c());
  d("\n");
  d("MATY_FF0F_L0p      %c\n", gb->int_reg.MATY_FF0F_L0p.c());
  d("MOPO_FF0F_L1p      %c\n", gb->int_reg.MOPO_FF0F_L1p.c());
  d("PAVY_FF0F_L2p      %c\n", gb->int_reg.PAVY_FF0F_L2p.c());
  d("NEJY_FF0F_L3p      %c\n", gb->int_reg.NEJY_FF0F_L3p.c());
  d("NUTY_FF0F_L4p      %c\n", gb->int_reg.NUTY_FF0F_L4p.c());
  d("\n");
#if 0
  d("PIN_CPU_INT_VBLANK %c\n", gb->int_reg.PIN_CPU_INT_VBLANK.c());
  d("PIN_CPU_INT_STAT   %c\n", gb->int_reg.PIN_CPU_INT_STAT.c());
  d("PIN_CPU_INT_TIMER  %c\n", gb->int_reg.PIN_CPU_INT_TIMER.c());
  d("PIN_CPU_INT_SERIAL %c\n", gb->int_reg.PIN_CPU_INT_SERIAL.c());
  d("PIN_CPU_INT_JOYPAD %c\n", gb->int_reg.PIN_CPU_INT_JOYPAD.c());
  d("\n");
  d("PIN_CPU_ACK_VBLANK %c\n", gb->int_reg.PIN_CPU_ACK_VBLANK.c());
  d("PIN_CPU_ACK_STAT   %c\n", gb->int_reg.PIN_CPU_ACK_STAT.c());
  d("PIN_CPU_ACK_TIMER  %c\n", gb->int_reg.PIN_CPU_ACK_TIMER.c());
  d("PIN_CPU_ACK_SERIAL %c\n", gb->int_reg.PIN_CPU_ACK_SERIAL.c());
  d("PIN_CPU_ACK_JOYPAD %c\n", gb->int_reg.PIN_CPU_ACK_JOYPAD.c());
  d("\n");
#endif

  text_painter.render(view, d.s.c_str(), cursor, 0);
  cursor += 224 - 32;
  d.clear();

  //----------------------------------------

  d("\002===== Clocks =====\001\n");
  d("PHASE %c%c%c%c\n", gb->clk_reg.AFUR_xxxxEFGH.c(), gb->clk_reg.ALEF_AxxxxFGH.c(), gb->clk_reg.APUK_ABxxxxGH.c(), gb->clk_reg.ADYK_ABCxxxxH.c());
  d("\n");
  d("AFUR_xxxxEFGH %c\n", gb->clk_reg.AFUR_xxxxEFGH.c());
  d("ALEF_AxxxxFGH %c\n", gb->clk_reg.ALEF_AxxxxFGH.c());
  d("APUK_ABxxxxGH %c\n", gb->clk_reg.APUK_ABxxxxGH.c());
  d("ADYK_ABCxxxxH %c\n", gb->clk_reg.ADYK_ABCxxxxH.c());
  d("VENA_ABCDxxxx %c\n", gb->clk_reg.VENA_xxCDEFxx.c());
  d("WOSU_xBCxxFGx %c\n", gb->clk_reg.WOSU_AxxDExxH.c());
  d("WUVU_xxCDxxGH %c\n", gb->clk_reg.WUVU_ABxxEFxx.c());
  d("\n");

  d("\002===== Reset =====\001\n");
  d("TUBO_WAITINGp  %c\n", gb->clk_reg.TUBO_WAITINGp.c());
  d("ASOL_POR_DONEn %c\n", gb->clk_reg.ASOL_POR_DONEn.c());
  d("AFER_SYS_RSTp  %c\n", gb->clk_reg.AFER_SYS_RSTp.c());
  d("\n");

  d("\002===== Timer =====\001\n");
  d("DIV    : 0x%04x %d\n",      gb->tim_reg.get_div(),    gb->tim_reg.get_div());
  d("TIMA   : 0x%02x %d\n",      gb->tim_reg.get_tima(),   gb->tim_reg.get_tima());
  d("TMA    : 0x%02x %d\n",      gb->tim_reg.get_tma(),    gb->tim_reg.get_tma());
  d("TAC    : 0x%02x %d\n",      gb->tim_reg.get_tac(),    gb->tim_reg.get_tac());
  d("NYDU_TIMA_D7_DELAY   %c\n", gb->tim_reg.NYDU_TIMA_D7_DELAY.c());
  d("MOBA_TIMER_OVERFLOWp %c\n", gb->tim_reg.MOBA_TIMER_OVERFLOWp.c());
  d("\n");

  d("\002===== Joypad =====\001\n");
  d("PIN_JOY_P10      %c\n", gb->joypad.PIN_JOY_P10.c());
  d("PIN_JOY_P11      %c\n", gb->joypad.PIN_JOY_P11.c());
  d("PIN_JOY_P12      %c\n", gb->joypad.PIN_JOY_P12.c());
  d("PIN_JOY_P13      %c\n", gb->joypad.PIN_JOY_P13.c());
  d("PIN_JOY_P14      %c\n", gb->joypad.PIN_JOY_P14.c());
  d("PIN_JOY_P15      %c\n", gb->joypad.PIN_JOY_P15.c());
  //d("PIN_CPU_WAKE     %c\n", gb->joypad.PIN_CPU_WAKE .c());
  d("AWOB_WAKE_CPU    %c\n", gb->joypad.AWOB_WAKE_CPU.c());
  d("BATU_JP_GLITCH0  %c\n", gb->joypad.BATU_JP_GLITCH0.c());
  d("ACEF_JP_GLITCH1  %c\n", gb->joypad.ACEF_JP_GLITCH1.c());
  d("AGEM_JP_GLITCH2  %c\n", gb->joypad.AGEM_JP_GLITCH2.c());
  d("APUG_JP_GLITCH3  %c\n", gb->joypad.APUG_JP_GLITCH3.c());
  d("JUTE_JOYP_RA     %c\n", gb->joypad.JUTE_JOYP_RA.c());
  d("KECY_JOYP_LB     %c\n", gb->joypad.KECY_JOYP_LB.c());
  d("JALE_JOYP_UC     %c\n", gb->joypad.JALE_JOYP_UC.c());
  d("KYME_JOYP_DS     %c\n", gb->joypad.KYME_JOYP_DS.c());
  d("KELY_JOYP_UDLR   %c\n", gb->joypad.KELY_JOYP_UDLR.c());
  d("COFY_JOYP_ABCS   %c\n", gb->joypad.COFY_JOYP_ABCS.c());
  d("KUKO_DBG_FF00_D6 %c\n", gb->joypad.KUKO_DBG_FF00_D6.c());
  d("KERU_DBG_FF00_D7 %c\n", gb->joypad.KERU_DBG_FF00_D7.c());
  d("KEVU_JOYP_L0     %c\n", gb->joypad.KEVU_JOYP_L0.c());
  d("KAPA_JOYP_L1     %c\n", gb->joypad.KAPA_JOYP_L1.c());
  d("KEJA_JOYP_L2     %c\n", gb->joypad.KEJA_JOYP_L2.c());
  d("KOLO_JOYP_L3     %c\n", gb->joypad.KOLO_JOYP_L3.c());
  d("\n");

  d("\002===== Serial =====\001\n");
  d("XFER_START  %c\n", gb->ser_reg.ETAF_SER_RUNNING.c());
  d("XFER_DIR    %c\n", gb->ser_reg.CULY_XFER_DIR.c());
  d("SER_CLK     %c\n", gb->ser_reg.COTY_SER_CLK.c());
  d("SER_CNT     %d\n", pack_u8(3, &gb->ser_reg.CAFA_SER_CNT0));
  d("SER_DATA    0x%02x\n", gb->ser_reg.get_data());
  d("SER_OUT     %c\n", gb->ser_reg.ELYS_SER_OUT.c());
  d("SCK         %c\n", gb->ser_reg.PIN_SCK.c());
  d("SIN         %c\n", gb->ser_reg.PIN_SIN.c());
  d("SOUT        %c\n", gb->ser_reg.PIN_SOUT.c());
  d("_CALY_SER_INTp %c\n", gb->ser_reg.CALY_SER_CNT3.c());
  d("\n");

  text_painter.render(view, d.s.c_str(), cursor, 0);
  cursor += 224 - 64;
  d.clear();

  //----------------------------------------

  //dump_cpu_bus(d, gb->cpu_bus);
  //dump_ext_bus(d, gb->ext_bus);
  dump_vram_bus(d, gb->vram_bus);
  dump_oam_bus(d, gb->oam_bus);


  d("\002===== DMA Reg =====\001\n");
  const auto& dma_reg = gb->dma_reg;

  uint8_t dma_addr_lo =  pack_u8(8, &dma_reg.NAKY_DMA_A00p);
  uint8_t dma_addr_hi = ~pack_u8(8, &dma_reg.NAFA_DMA_A08n);

  d("DMA Addr 0x%02x:%02x\n", dma_addr_hi, dma_addr_lo);
  d("MATU_DMA_RUNNINGp   %d\n",  dma_reg.MATU_DMA_RUNNINGp.qp17());
  d("LYXE_DMA_LATCHn     %d\n",  dma_reg.LYXE_DMA_LATCHp);
  d("MYTE_DMA_DONE       %d\n", !dma_reg.MYTE_DMA_DONE.qn16());
  d("LUVY_DMA_TRIG_d0    %d\n",  dma_reg.LUVY_DMA_TRIG_d0.qp17());
  d("LENE_DMA_TRIG_d4    %d\n", !dma_reg.LENE_DMA_TRIG_d4.qn16());
  d("LOKY_DMA_LATCHp     %d\n",  dma_reg.LOKY_DMA_LATCHp.qp03());
  d("\n");

  text_painter.render(view, d.s.c_str(), cursor, 0);
  cursor += 224;
  d.clear();

  //----------------------------------------

  d("\002===== LCD =====\001\n");
  d("PIX COUNT : %03d\n", gb->pix_pipe.get_pix_count());
  d("LCD X     : %03d\n", gb->lcd_reg.get_lx());
  d("LCD Y     : %03d\n", gb->lcd_reg.get_ly());
  d("LYC       : %03d\n", gb->lcd_reg.get_lyc());
  d("\n");

  d("lcd_pix_lo      : %c\n", gb->lcd_pix_lo.c());
  d("lcd_pix_hi      : %c\n", gb->lcd_pix_hi.c());
  /*
  d("PIN_LCD_CLOCK   : %c\n", gb->PIN_LCD_CLOCK.c());
  d("PIN_LCD_HSYNC   : %c\n", gb->PIN_LCD_HSYNC.c());
  d("PIN_LCD_VSYNC   : %c\n", gb->PIN_LCD_VSYNC.c());
  d("PIN_LCD_DATA1   : %c\n", gb->PIN_LCD_DATA1.c());
  d("PIN_LCD_DATA0   : %c\n", gb->PIN_LCD_DATA0.c());
  d("PIN_LCD_CNTRL   : %c\n", gb->PIN_LCD_CNTRL.c());
  d("PIN_LCD_DATALCH : %c\n", gb->PIN_LCD_LATCH.c());
  d("PIN_LCD_ALTSIGL : %c\n", gb->PIN_LCD_FLIPS.c());
  */
  d("\n");

  d("CATU_LINE_P000      %c\n", gb->lcd_reg.CATU_LINE_P000.c());
  d("NYPE_LINE_P002      %c\n", gb->lcd_reg.NYPE_LINE_P002.c());
  d("ANEL_LINE_P002      %c\n", gb->lcd_reg.ANEL_LINE_P002.c());
  d("RUTU_LINE_P910      %c\n", gb->lcd_reg.RUTU_LINE_P910.c());
  d("MYTA_LINE_153p      %c\n", gb->lcd_reg.MYTA_LINE_153p     .c());
  d("POPU_IN_VBLANKp     %c\n", gb->lcd_reg.POPU_IN_VBLANKp    .c());
  d("ROPO_LY_MATCH_SYNCp %c\n", gb->lcd_reg.ROPO_LY_MATCH_SYNCp.c());
  d("\n");

  d("\002===== Pix Pipe =====\001\n");

  d("PIX COUNT  0x%02x\n", pack_u8(8, &gb->pix_pipe.XEHO_X0p));
  d("FF40 LCDC  0x%02x\n", pack_u8n(8, &gb->pix_pipe.VYXE_LCDC_BGENn));

  // FIXME plumb sadu/xaty in here somehow
  /*
  d.dump_reg("FF41 STAT",
    pack_u8(
      0, //!gb->pix_pipe.SADU_STAT_MODE0n,
      0, //!gb->pix_pipe.XATY_STAT_MODE1n,
      gb->pix_pipe.RUPO_LYC_MATCH_LATCHn.qn03(),
      gb->pix_pipe.ROXE_STAT_HBI_ENn.qn08(),
      gb->pix_pipe.RUFO_STAT_VBI_ENn.qn08(),
      gb->pix_pipe.REFE_STAT_OAI_ENn.qn08(),
      gb->pix_pipe.RUGU_STAT_LYI_ENn.qn08(),
      1));
  */

  d("FF42 SCY   0x%02x\n", pack_u8n(8, &gb->pix_pipe.GAVE_SCY0n));
  d("FF43 SCX   0x%02x\n", pack_u8n(8, &gb->pix_pipe.DATY_SCX0n));
  d("FF47 BGP   0x%02x\n", pack_u8n(8, &gb->pix_pipe.PAVO_BGP_D0n));
  d("FF48 OBP0  0x%02x\n", pack_u8n(8, &gb->pix_pipe.XUFU_OBP0_D0n));
  d("FF49 OBP1  0x%02x\n", pack_u8n(8, &gb->pix_pipe.MOXY_OBP1_D0n));
  d("FF4A WY    0x%02x\n", pack_u8n(8, &gb->pix_pipe.NESO_WY0n));
  d("FF4B WX    0x%02x\n", pack_u8n(8, &gb->pix_pipe.MYPA_WX0n));
  d("BG_PIPE_A  0x%02x\n", pack_u8(8, &gb->pix_pipe.MYDE_BG_PIPE_A0));
  d("BG_PIPE_B  0x%02x\n", pack_u8(8, &gb->pix_pipe.TOMY_BG_PIPE_B0));
  d("SPR_PIPE_A 0x%02x\n", pack_u8(8, &gb->pix_pipe.NYLU_SPR_PIPE_B0));
  d("SPR_PIPE_B 0x%02x\n", pack_u8(8, &gb->pix_pipe.NURO_SPR_PIPE_A0));
  d("PAL_PIPE   0x%02x\n", pack_u8(8, &gb->pix_pipe.RUGO_PAL_PIPE_0));
  d("MASK_PIPE  0x%02x\n", pack_u8(8, &gb->pix_pipe.VEZO_MASK_PIPE_0));

  d("WIN X      0x%02x\n", pack_u8(5, &gb->pix_pipe.WYKA_WIN_X3));
  d("WIN Y      0x%02x\n", pack_u8(8, &gb->pix_pipe.VYNO_WIN_Y0));

  d("\n");

  d("ROXY_FINE_SCROLL_DONEn %c\n", gb->pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.c());
  d("RYKU_FINE_CNT0         %c\n", gb->pix_pipe.RYKU_FINE_CNT0.c());
  d("ROGA_FINE_CNT1         %c\n", gb->pix_pipe.ROGA_FINE_CNT1.c());
  d("RUBU_FINE_CNT2         %c\n", gb->pix_pipe.RUBU_FINE_CNT2.c());
  d("XYMU_RENDERINGp        %c\n", gb->pix_pipe.XYMU_RENDERINGn.c());
  d("RUPO_LYC_MATCH_LATCHn  %c\n", gb->pix_pipe.RUPO_LYC_MATCH_LATCHn.c());
  d("WUSA_LCD_CLOCK_GATE    %c\n", gb->pix_pipe.WUSA_LCD_CLOCK_GATE.c());
  d("VOGA_RENDER_DONE_SYNC  %c\n", gb->pix_pipe.VOGA_HBLANKp.c());
  d("PUXA_FINE_MATCH_A      %c\n", gb->pix_pipe.PUXA_SCX_FINE_MATCH_A.c());
  d("NYZE_FINE_MATCH_B      %c\n", gb->pix_pipe.NYZE_SCX_FINE_MATCH_B.c());
  d("PAHO_X_8_SYNC          %c\n", gb->pix_pipe.PAHO_X_8_SYNC.c());
  d("POFY_HSYNCp            %c\n", gb->pix_pipe.POFY.c());
  d("\n");


  d("\002===== Window =====\001\n");
  d("PYNU_WIN_MODE_A       : %c\n", gb->pix_pipe.PYNU_WIN_MODE_A.c());
  d("RYDY_WIN_FIRST_TILE_A : %c\n", gb->pix_pipe.RYDY.c());
  d("NOPA_WIN_MODE_B       : %c\n", gb->pix_pipe.NOPA_WIN_MODE_B.c());
  d("SOVY_WIN_FIRST_TILE_B : %c\n", gb->pix_pipe.SOVY_WIN_FIRST_TILE_B.c());
  d("REJO_WY_MATCH_LATCH   : %c\n", gb->pix_pipe.REJO_WY_MATCH_LATCH.c());
  d("SARY_WY_MATCH         : %c\n", gb->pix_pipe.SARY_WY_MATCH.c());
  d("RYFA_FETCHn_A         : %c\n", gb->pix_pipe.RYFA_FETCHn_A.c());
  d("RENE_FETCHn_B         : %c\n", gb->pix_pipe.RENE_FETCHn_B.c());
  d("PYCO_WX_MATCH_A       : %c\n", gb->pix_pipe.PYCO_WX_MATCH_A.c());
  d("NUNU_WX_MATCH_B       : %c\n", gb->pix_pipe.NUNU_WX_MATCH_B.c());
  d("\n");

  text_painter.render(view, d.s.c_str(), cursor, 0);
  cursor += 224;
  d.clear();

  //----------------------------------------

  d("\002===== SpriteFetch =====\001\n");
  d("SOBU_SFETCH_REQp     %c\n", gb->sprite_fetcher.SOBU_SFETCH_REQp    .c());
  d("SUDA_SFETCH_REQp     %c\n", gb->sprite_fetcher.SUDA_SFETCH_REQp    .c());
  d("\n");
  d("TAKA_SFETCH_RUNNINGp %c\n", gb->sprite_fetcher.TAKA_SFETCH_RUNNINGp.c());
  d("\n");
  d("TOXE_SFETCH_S0       %c\n", gb->sprite_fetcher.TOXE_SFETCH_S0      .c());
  d("TYFO_SFETCH_S0_D1    %c\n", gb->sprite_fetcher.TYFO_SFETCH_S0_D1   .c());
  d("\n");
  d("TULY_SFETCH_S1       %c\n", gb->sprite_fetcher.TULY_SFETCH_S1      .c());
  d("TOBU_SFETCH_S1_D2    %c\n", gb->sprite_fetcher.TOBU_SFETCH_S1_D2   .c());
  d("VONU_SFETCH_S1_D4    %c\n", gb->sprite_fetcher.VONU_SFETCH_S1_D4   .c());
  d("SEBA_SFETCH_S1_D5    %c\n", gb->sprite_fetcher.SEBA_SFETCH_S1_D5   .c());
  d("\n");
  d("TESE_SFETCH_S2       %c\n", gb->sprite_fetcher.TESE_SFETCH_S2      .c());
  d("\n");

  //----------------------------------------

  d("\002===== SpriteScan =====\001\n");

  d("SCAN INDEX       %02d\n", pack_u8(6, &gb->sprite_scanner.YFEL_SCAN0));
  d("SPRITE INDEX     %02d\n", pack_u8(6, &gb->sprite_scanner.XADU_SPRITE_IDX0p));

  d("BESU_SCANNINGp   %c\n", gb->sprite_scanner.BESU_SCANNINGp  .c());
  d("CENO_SCANNINGp   %c\n", gb->sprite_scanner.CENO_SCANNINGp  .c());
  d("BYBA_SCAN_DONE_A %c\n", gb->sprite_scanner.BYBA_SCAN_DONE_A.c());
  d("DOBA_SCAN_DONE_B %c\n", gb->sprite_scanner.DOBA_SCAN_DONE_B.c());
  d("\n");
  d("LCD Y      %03d\n", gb->lcd_reg.get_ly());

  d("\n");

  //----------------------------------------

  const auto& ss = gb->sprite_store;
  d("\002===== SpriteStore =====\001\n");
  d("DEZY_STORE_ENn %c\n", ss.DEZY_STORE_ENn.c());

  d("SPRITE COUNT %02d\n", pack_u8(4, &ss.BESE_SPRITE_COUNT0));

#if 0
  int spr_tri_idx  = pack_u8(6, &ss.SPR_TRI_I0p);
  int spr_tri_line = pack_u8(4, &ss.SPR_TRI_L0);

  d("SPR_TRI_IDX  = %2d %c%c%c%c%c%c\n", spr_tri_idx,  ss.SPR_TRI_I5p.c(), ss.SPR_TRI_I4p.c(), ss.SPR_TRI_I3p.c(), ss.SPR_TRI_I2p.c(), ss.SPR_TRI_I1p.c(), ss.SPR_TRI_I0p.c());
  d("SPR_TRI_LINE = %2d %c%c%c%c\n",     spr_tri_line, ss.SPR_TRI_L0.c(),  ss.SPR_TRI_L1.c(),  ss.SPR_TRI_L2.c(),  ss.SPR_TRI_L3.c());
#endif

  d("STORE0 R%d I%02d L%02d X%03d\n", ss.EBOJ_STORE0_RSTp.qp17(), pack_u8n(6, &ss.YGUS_STORE0_I0n), pack_u8n(4, &ss.GYHO_STORE0_L0n), pack_u8n(8, &ss.XEPE_STORE0_X0p));
  d("STORE1 R%d I%02d L%02d X%03d\n", ss.CEDY_STORE1_RSTp.qp17(), pack_u8n(6, &ss.CADU_STORE1_I0n), pack_u8n(4, &ss.AMES_STORE1_L0n), pack_u8n(8, &ss.DANY_STORE1_X0p));
  d("STORE2 R%d I%02d L%02d X%03d\n", ss.EGAV_STORE2_RSTp.qp17(), pack_u8n(6, &ss.BUHE_STORE2_I0n), pack_u8n(4, &ss.YLOV_STORE2_L0n), pack_u8n(8, &ss.FOKA_STORE2_X0p));
  d("STORE3 R%d I%02d L%02d X%03d\n", ss.GOTA_STORE3_RSTp.qp17(), pack_u8n(6, &ss.DEVY_STORE3_I0n), pack_u8n(4, &ss.ZURO_STORE3_L0n), pack_u8n(8, &ss.XOLY_STORE3_X0p));
  d("STORE4 R%d I%02d L%02d X%03d\n", ss.XUDY_STORE4_RSTp.qp17(), pack_u8n(6, &ss.XAVE_STORE4_I0n), pack_u8n(4, &ss.CAPO_STORE4_L0n), pack_u8n(8, &ss.WEDU_STORE4_X0p));
  d("STORE5 R%d I%02d L%02d X%03d\n", ss.WAFY_STORE5_RSTp.qp17(), pack_u8n(6, &ss.EKOP_STORE5_I0n), pack_u8n(4, &ss.ACEP_STORE5_L0n), pack_u8n(8, &ss.FUSA_STORE5_X0p));
  d("STORE6 R%d I%02d L%02d X%03d\n", ss.WOMY_STORE6_RSTp.qp17(), pack_u8n(6, &ss.GABO_STORE6_I0n), pack_u8n(4, &ss.ZUMY_STORE6_L0n), pack_u8n(8, &ss.YCOL_STORE6_X0p));
  d("STORE7 R%d I%02d L%02d X%03d\n", ss.WAPO_STORE7_RSTp.qp17(), pack_u8n(6, &ss.GULE_STORE7_I0n), pack_u8n(4, &ss.XYNA_STORE7_L0n), pack_u8n(8, &ss.ERAZ_STORE7_X0p));
  d("STORE8 R%d I%02d L%02d X%03d\n", ss.EXUQ_STORE8_RSTp.qp17(), pack_u8n(6, &ss.AXUV_STORE8_I0n), pack_u8n(4, &ss.AZAP_STORE8_L0n), pack_u8n(8, &ss.EZUF_STORE8_X0p));
  d("STORE9 R%d I%02d L%02d X%03d\n", ss.FONO_STORE9_RSTp.qp17(), pack_u8n(6, &ss.YBER_STORE9_I0n), pack_u8n(4, &ss.CANA_STORE9_L0n), pack_u8n(8, &ss.XUVY_STORE9_X0p));
  d("\n");

  //----------------------------------------

  d("\002=====TileFetcher=====\001\n");
  d("LAXU_BFETCH_S0           %c\n", gb->tile_fetcher.LAXU_BFETCH_S0.c());
  d("MESU_BFETCH_S1           %c\n", gb->tile_fetcher.MESU_BFETCH_S1.c());
  d("NYVA_BFETCH_S2           %c\n", gb->tile_fetcher.NYVA_BFETCH_S2.c());
  d("LYZU_BFETCH_S0_D1        %c\n", gb->tile_fetcher.LYZU_BFETCH_S0_D1.c());
  d("\n");
  d("NYKA_FETCH_DONE_P11      %c\n", gb->tile_fetcher.NYKA_FETCH_DONE_P11.c());
  d("PORY_FETCH_DONE_P12      %c\n", gb->tile_fetcher.PORY_FETCH_DONE_P12.c());
  d("PYGO_FETCH_DONE_P13      %c\n", gb->tile_fetcher.PYGO_FETCH_DONE_P13.c());
  d("POKY_PRELOAD_DONEp       %c\n", gb->tile_fetcher.POKY_PRELOAD_LATCHp.c());
  d("\n");
  d("LONY_FETCH_RUNNINGp      %c\n", gb->tile_fetcher.LONY_BG_FETCH_RUNNINGp.c()); // 1 for phases 0-11, 0 for 12-15
  d("LOVY_FETCH_DONEp         %c\n", gb->tile_fetcher.LOVY_BG_FETCH_DONEp.c());    // 0 for phases 0-11, 1 for 12-15
  d("\n");

  text_painter.render(view, d.s.c_str(), cursor, 0);
  cursor += 224 - 32;
  d.clear();

  //----------------------------------------

  d("\002===== Disasm =====\001\n");
  {
    uint16_t pc = gb->cpu.op_addr;
    const uint8_t* code = nullptr;
    uint16_t code_size = 0;
    uint16_t code_base = 0;

    if (!gb->BOOT_BITn.qp17()) {
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
  text_painter.render(view, d.s.c_str(), cursor, 0);
  d.clear();

  //----------------------------------------

  //update_texture_u32(trace_tex, 912, 154, trace);
  //blitter.blit(view, trace_tex, 0, 0, 912, 154);

  // Draw flat memory view
  /*
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
  */

  // Draw screen and vid ram contents

  int gb_x = 1216;
  int gb_y = 32;

  if (has_golden && show_diff) {
    gb_blitter.blit_diff(view, gb_x, gb_y,  2, framebuffer, golden_u8);
  } else if (show_golden) {
    gb_blitter.blit_screen(view, gb_x, gb_y,  2, golden_u8);
  } else {
    gb_blitter.blit_screen(view, gb_x, gb_y,  2, framebuffer);
  }

  gb_blitter.blit_tiles (view, 1632, 32,  1, vid_ram);
  gb_blitter.blit_map   (view, 1344, 448, 1, vid_ram, 0, 0);
  gb_blitter.blit_map   (view, 1632, 448, 1, vid_ram, 0, 1);
  gb_blitter.blit_map   (view, 1344, 736, 1, vid_ram, 1, 0);
  gb_blitter.blit_map   (view, 1632, 736, 1, vid_ram, 1, 1);

  // Draw screen overlay
  {
    int fb_x = gb->gb_screen_x;
    int fb_y = gb->gb_screen_y;
    if (fb_y >= 0 && fb_y < 144 && fb_x >= 0 && fb_x < 160) {
      memset(overlay, 0, sizeof(overlay));

      for (int x = 0; x < fb_x; x++) {
        uint8_t p0 = gb->lcd_pipe_lo[159 - fb_x + x + 1].qp();
        uint8_t p1 = gb->lcd_pipe_hi[159 - fb_x + x + 1].qp();

        int r = (3 - (p0 + p1 * 2)) * 30 + 50;
        int g = (3 - (p0 + p1 * 2)) * 30 + 50;
        int b = (3 - (p0 + p1 * 2)) * 30 + 30;

        overlay[x + fb_y * 160] = 0xFF000000 | (b << 16) | (g << 8) | (r << 0);
      }
      {
        uint8_t p0 = gb->lcd_pix_lo.qp04();
        uint8_t p1 = gb->lcd_pix_hi.qp04();

        int c = (3 - (p0 + p1 * 2)) * 85;

        overlay[fb_x + fb_y * 160] = 0xFF000000 | (c << 16) | (c << 8) | (c << 0);
      }

      update_texture_u32(overlay_tex, 160, 144, overlay);
      blitter.blit(view, overlay_tex, gb_x, gb_y, 160 * 2, 144 * 2);
    }
  }

  // Status bar under screen

  //double phases_per_frame = 114 * 154 * 60 * 8;
  //double sim_ratio = sim_rate / phases_per_frame;
  double sim_ratio = 0.0;
  double sim_time_smooth = 0.0;

  d("%s %s Sim clock %8.3f %s %c %s\n",
    runmode_names[runmode],
    stepmode_names[stepmode],
    double(phase_total) / (4194304.0 * 2),
    phase_names[phase_total & 7],
    sim_stable ? ' ' : '*',
    show_golden ? "GOLDEN IMAGE " : "");
  d("Sim time %f, sim ratio %f, frame time %f\n", sim_time_smooth, sim_ratio, frame_time_smooth);
  text_painter.render(view, d.s, gb_x, gb_y + 144 * 2);
  d.clear();

  // Probe dump

  gb->probes.dump(d, draw_passes);
  text_painter.render(view, d.s, 640 - 64, 640 + 128);
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
