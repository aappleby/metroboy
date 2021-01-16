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
  // regenerate post-bootrom dump
  gb_thread.load_cart(DMG_ROM_blob, load_blob("microtests/build/dmg/poweron_div_004.gb"));
  gb_thread.reset_to_bootrom();
  for (int i = 0; i < 8192; i++) {
    gb_thread.gb->vid_ram[i] = (uint8_t)rand();
  }
#endif

#if 0
  {
    /*
    m3_lcdc_obj_size_change.gb - small fail
    m3_lcdc_obj_size_change_scx.gb - small fail
    m3_lcdc_bg_en_change.gb - something off by one
    m3_bgp_change.gb - off by one
    m3_lcdc_obj_en_change.gb - off by one
    m3_lcdc_obj_en_change_variant.gb - off by one
    m3_lcdc_win_en_change_multiple_wx.gb -  fail, chunks off. image from mealybug wrong
    m3_bgp_change_sprites.gb - off by one
    m3_obp0_change.gb
    m3_lcdc_bg_map_change.gb
    m3_lcdc_tile_sel_change.gb
    m3_lcdc_tile_sel_win_change.gb
    m3_lcdc_win_en_change_multiple.gb  - fail, chunks off
    m3_lcdc_win_map_change.gb
    m3_scx_low_3_bits.gb
    m3_window_timing.gb
    m3_window_timing_wx_0.gb
    m3_wx_4_change.gb
    m3_wx_4_change_sprites.gb
    m3_wx_5_change.gb
    m3_wx_6_change.gb
    */

    //load_rom("microtests/build/dmg/lcdon_to_stat2_a.gb"));
    load_rom   ("roms/mealybug/m3_bgp_change_sprites.gb");
    load_golden("roms/mealybug/m3_bgp_change_sprites.bmp");
  }
#endif


#if 0
  const char* app = R"(
  0150:
    ld a, $55
    ld hl, $c003
    ld (hl), a
    jr -3
  )";

  Assembler as;
  as.assemble(app);

  gb_thread.load_cart(DMG_ROM_blob, as.link());
  gb_thread.reset_to_cart();
  gb_thread.gb->run_phases(120);
#endif

#if 1
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

  d("\002===== Thread =====\001\n");
  gb_thread.dump(d);
  d("\n");

  d("\002===== GateBoy Top =====\001\n");
  gb->dump(d);
  d("\n");

  d("\002===== Reset/Debug =====\001\n");
  gb->rst.dump(d);
  d("\n");

  d("\002===== Clocks =====\001\n");
  gb->clk.dump(d);
  d("\n");

  d("\002===== Timer =====\001\n");
  gb->div.dump(d);
  gb->timer.dump(d);
  d("\n");

  d("\002===== Joypad =====\001\n");
  gb->joypad.dump(d);
  d("\n");

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------

  d("\002===== CPU =====\001\n");
  gb->cpu.dump(d);

  d("\002===== CPU Bus =====\001\n");
  gb->cpu_bus.dump(d);
  d("\n");

  d("\002===== Interrupts =====\001\n");
  gb->interrupts.dump(d);
  d("\n");

  d("\002===== DMA =====\001\n");
  gb->dma.dump(d);
  d("\n");

  /*
  d("\002===== Serial =====\001\n");
  d.dump_bitp   ("ETAF_SER_RUNNING : ", gb->serial.ETAF_SER_RUNNING.state);
  d.dump_bitp   ("CULY_XFER_DIR    : ", gb->serial.CULY_SER_DIR.state);
  d.dump_bitp   ("COTY_SER_CLK     : ", gb->serial.COTY_SER_CLK.state);
  d.dump_bitp   ("ELYS_SER_OUT     : ", gb->serial.ELYS_SER_OUT.state);
  d.dump_slice2p("CAFA_SER_CNT     : ", &gb->serial.CAFA_SER_CNT0, 4);
  d.dump_slice2p("CUBA_SER_DATA    : ", &gb->serial.CUBA_SER_DATA0, 8);
  d("\n");
  */

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------

  d("\002===== LCD =====\001\n");
  gb->lcd.dump(d);
  d.dump_slice2p("LX         : ", &gb->lcd.reg_lx.SAXO_LX0p.state,  7);
  d.dump_slice2p("FF44 LY    : ", &gb->lcd.reg_ly.MUWY_LY0p.state,  8);
  d.dump_slice2n("FF45 LYC   : ", &gb->lcd.reg_lyc.SYRY_LYC0n.state, 8);
  d.dump_bitp   ("MYTA_y153p          : ", gb->lcd.reg_ly.MYTA_y153p.state);
  d.dump_bitp   ("RUTU_LINE_P910p     : ", gb->lcd.reg_lx.RUTU_x113p.state);
  d.dump_bitp   ("NYPE_LINE_P002p     : ", gb->lcd.reg_lx.NYPE_x113p.state);
  d.dump_bitp   ("ROPO_LY_MATCH_SYNCp : ", gb->lcd.reg_lyc.ROPO_LY_MATCH_SYNCp.state);
  d("\n");

  d("\002===== EXT Bus =====\001\n");
  gb->ext_bus.dump(d);
  d("\n");

  d("\002===== VRAM Bus =====\001\n");
  gb->vram_bus.dump(d);
  d("\n");

  d("\002===== OAM Bus =====\001\n");
  gb->oam_bus.dump(d);
  d("\n");

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------

  d("\002===== PPU Regs=====\001\n");
  d.dump_slice2n("FF40 LCDC  : ", &gb->reg_lcdc.VYXE_LCDC_BGENn, 8);
  gb->reg_stat.dump(d);
  d.dump_slice2n("FF42 SCY   : ", &gb->reg_scy.GAVE_SCY0n, 8);
  d.dump_slice2n("FF43 SCX   : ", &gb->reg_scx.DATY_SCX0n, 8);
  d.dump_slice2n("FF47 BGP   : ", &gb->pix_pipes.reg_bgp.PAVO_BGP_D0n, 8);
  d.dump_slice2n("FF48 OBP0  : ", &gb->pix_pipes.reg_obp0.XUFU_OBP0_D0n, 8);
  d.dump_slice2n("FF49 OBP1  : ", &gb->pix_pipes.reg_obp1.MOXY_OBP1_D0n, 8);
  d.dump_slice2n("FF4A WY    : ", &gb->reg_wy.NESO_WY0n, 8);
  d.dump_slice2n("FF4B WX    : ", &gb->reg_wx.MYPA_WX0n, 8);
  d.dump_slice2p("WIN MAP X  : ", &gb->win_map_x.WYKA_WIN_X3, 5);
  d.dump_slice2p("WIN LINE Y : ", &gb->win_line_y.VYNO_WIN_Y0, 8);
  d.dump_bitp   ("RUPO_LYC_MATCHn     : ", gb->reg_stat.RUPO_LYC_MATCHn.state);
  d("\n");

  d("\002===== Pix Pipe =====\001\n");
  d.dump_slice2p("PIX COUNT  : ", &gb->pix_count.XEHO_PX0p, 8);
  d.dump_slice2p("BG PIPE A  : ", &gb->pix_pipes.MYDE_BGW_PIPE_A0, 8);
  d.dump_slice2p("BG PIPE B  : ", &gb->pix_pipes.TOMY_BGW_PIPE_B0, 8);
  d.dump_slice2p("SPR PIPE A : ", &gb->pix_pipes.NURO_SPR_PIPE_A0, 8);
  d.dump_slice2p("SPR PIPE B : ", &gb->pix_pipes.NYLU_SPR_PIPE_B0, 8);
  d.dump_slice2p("PAL PIPE   : ", &gb->pix_pipes.RUGO_PAL_PIPE_D0, 8);
  d.dump_slice2p("MASK PIPE  : ", &gb->pix_pipes.VEZO_MASK_PIPE_0, 8);
  d.dump_bitn   ("REMY_LD0n  : ", gb->pix_pipes.REMY_LD0n.get_state());
  d.dump_bitn   ("RAVO_LD1n  : ", gb->pix_pipes.RAVO_LD1n.get_state());

  d("\n");
  d.dump_bitp("XYMU_RENDERINGn        : ", gb->ppu_reg.XYMU_RENDERINGn.state);
  d.dump_bitp("PYNU_WIN_MODE_Ap       : ", gb->win_reg.PYNU_WIN_MODE_Ap.state);
  d.dump_bitp("PUKU_WIN_HITn          : ", gb->win_reg.PUKU_WIN_HITn.get_state());
  d.dump_bitp("RYDY_WIN_HITp          : ", gb->win_reg.RYDY_WIN_HITp.get_state());
  d.dump_bitp("SOVY_WIN_FIRST_TILE_B  : ", gb->win_reg.SOVY_WIN_HITp.state);
  d.dump_bitp("NOPA_WIN_MODE_B        : ", gb->win_reg.NOPA_WIN_MODE_Bp.state);
  d.dump_bitp("PYCO_WX_MATCH_A        : ", gb->win_reg.PYCO_WIN_MATCHp.state);
  d.dump_bitp("NUNU_WX_MATCH_B        : ", gb->win_reg.NUNU_WIN_MATCHp.state);
  d.dump_bitp("REJO_WY_MATCH_LATCH    : ", gb->win_reg.REJO_WY_MATCH_LATCHp.state);
  d.dump_bitp("SARY_WY_MATCH          : ", gb->win_reg.SARY_WY_MATCHp.state);
  d.dump_bitp("RYFA_FETCHn_A          : ", gb->win_reg.RYFA_WIN_FETCHn_A.state);
  d.dump_bitp("RENE_FETCHn_B          : ", gb->win_reg.RENE_WIN_FETCHn_B.state);
  d.dump_bitp("RYKU_FINE_CNT0         : ", gb->fine_scroll.RYKU_FINE_CNT0.state);
  d.dump_bitp("ROGA_FINE_CNT1         : ", gb->fine_scroll.ROGA_FINE_CNT1.state);
  d.dump_bitp("RUBU_FINE_CNT2         : ", gb->fine_scroll.RUBU_FINE_CNT2.state);
  d.dump_bitp("PUXA_FINE_MATCH_A      : ", gb->fine_scroll.PUXA_SCX_FINE_MATCH_A.state);
  d.dump_bitp("NYZE_FINE_MATCH_B      : ", gb->fine_scroll.NYZE_SCX_FINE_MATCH_B.state);
  d.dump_bitp("ROXY_FINE_SCROLL_DONEn : ", gb->fine_scroll.ROXY_FINE_SCROLL_DONEn.state);
  d.dump_bitp("RUPO_LYC_MATCH_LATCHn  : ", gb->reg_stat.RUPO_LYC_MATCHn.state);
  d.dump_bitp("VOGA_HBLANKp           : ", gb->ppu_reg.VOGA_HBLANKp.state);
  d("\n");

  d("\002===== Tile Fetch =====\001\n");
  gb->tile_fetcher.dump(d);
  d("\n");

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------

  d("\002===== Sprite Fetch =====\001\n");
  gb->sprite_fetcher.dump(d);
  d("\n");

  d("\002===== Sprite Scan =====\001\n");
  gb->sprite_scanner.dump(d);
  d("\n");

  const auto& ss = gb->sprite_store;
  d("\002===== Sprite Store =====\001\n");
  gb->sprite_store.dump(d);
  d("\n");

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------

  d("\002===== Disasm =====\001\n");
  {
    uint16_t pc = gb->cpu.op_addr;
    const uint8_t* code = nullptr;
    uint16_t code_size = 0;
    uint16_t code_base = 0;

    if (!bit(gb->cpu_bus.TEPU_BOOT_BITn_h.qp_old())) {
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

  text_painter.render_string(view, d.s.c_str(), cursor_x, cursor_y);
  cursor_x += col_spacing;
  d.clear();

  //----------------------------------------

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
  text_painter.render_string(view, d.s.c_str(), 42 * 32 - 16, 10 * 32);
  d.clear();

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
    int fb_y = gb->lcd.reg_ly.get_old();

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
  text_painter.render_string(view, d.s, 42 * 32 - 16, 19 * 32 - 24);
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
