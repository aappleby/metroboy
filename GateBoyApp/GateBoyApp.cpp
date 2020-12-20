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

#if 1
  // regenerate post-bootrom dump
  /*
  gb_thread.load_cart(DMG_ROM_blob, load_blob("roms/tetris.gb"));
  gb_thread.reset_to_bootrom();
  for (int i = 0; i < 8192; i++) {
    gb_thread.gb->vid_ram[i] = (uint8_t)rand();
  }
  */

  gb_thread.load_cart(DMG_ROM_blob, load_blob("microtests/build/dmg/poweron_000_stat.gb"));
  gb_thread.reset_to_cart();
  gb_thread.gb->sys_cpu_en = 0;
  memset(gb_thread.gb->framebuffer, 3, sizeof(gb_thread.gb->framebuffer));

  /*
  for (int i = 0; i < 1024 * 6; i++) {
    gb_thread.gb->vid_ram[i] = (uint8_t)rand();
  }
  for (int i = 1024 * 6; i < 8192; i++) {
    gb_thread.gb->vid_ram[i] = 0x00;
  }

  gb_thread.gb->dbg_write(ADDR_BGP, 0b11100100);
  */
  {
    uint8_t* vram = gb_thread.gb->vid_ram;
    int cursor = 0;

    for (int i = 0; i < 768; i++) {
      for (int j = 0; j < 8; j++) vram[cursor++] = (uint8_t)i;
    }
    for (int i = 0; i < 2048; i++) {
      vram[cursor++] = 0;
    }


  }

#endif


#if 0
  // run tiny app
  if (1) {
    /*
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
    */

    std::string app = R"(
    0150:
      ld a, ($FF40)
      ld a, ($FF40)
      ld a, ($FF40)
      ld a, ($FF40)
      ld a, ($FF40)
      ld a, ($FF40)
      ld a, ($FF40)
      jp $0150
    )";

    Assembler as;
    as.assemble(app.c_str());
    blob rom = as.link();

    gb_thread.reset_cart(DMG_ROM_blob, rom);
  }
#endif

#if 0
  load_flat_dump("roms/LinksAwakening_dog.dump");
  gb_thread.gb->sys_cpu_en = false;
  gb_thread.gb->phase_total = 0;

  gb_thread.gb->dbg_write(ADDR_WY, 113);
  gb_thread.gb->dbg_write(ADDR_WX, 13 + 7);

  //gb_thread.gb->dbg_write(ADDR_SCX, 3);

#endif

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

  //load_rom("microtests/build/dmg/win10_scx3_a.gb");

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
  memcpy(gb_thread.gb->ext_ram,  gb_thread.cart.data() + 0xC000, 8192);
  memcpy(gb_thread.gb->oam_ram,  gb_thread.cart.data() + 0xFE00, 256);
  memcpy(gb_thread.gb->zero_ram, gb_thread.cart.data() + 0xFF80, 128);

#if 0
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
#endif
}


//-----------------------------------------------------------------------------

void GateBoyApp::app_update(double /*delta*/) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_KEYDOWN)
    switch (event.key.keysym.sym) {

    case SDLK_SPACE: { gb_thread.sig_pause ? gb_thread.resume() : gb_thread.pause(); break; }

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
        if (stepmode == STEP_PHASE) {
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
        else {
          gb_thread.step_pass(1);
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

void GateBoyApp::app_render_frame(Viewport view) {
  gb_thread.pause();

  grid_painter.render(view);

  const auto gb = gb_thread.gb.state();

  uint8_t* framebuffer = gb->framebuffer;
  uint8_t* vid_ram = gb->vid_ram;
  uint64_t phase_total = gb->phase_total;

  StringDumper d;
  float cursor = 0;
  float col_spacing = 6.5 * 32;

  //----------------------------------------

  d("\002===== Thread =====\001\n");

  d("State count   : %d\n", gb_thread.gb.state_count());
  size_t state_size = gb_thread.gb.state_size_bytes();
  if (state_size < 1024 * 1024) {
    d("State size    : %d K\n", state_size / 1024);
  }
  else {
    d("State size    : %d M\n", state_size / (1024 * 1024));
  }
  d("BGB cycle     : 0x%08x\n",  (gb->phase_total / 4) - 0x10000);
  d("Sim clock     : %f\n",      double(gb->phase_total) / (4194304.0 * 2));

  d("Commands left : %d\n",    uint8_t(gb_thread.cursor_w - gb_thread.cursor_r));
  d("Steps left    : %d\n",    gb_thread.command.count);
  d("Waiting       : %d\n",    gb_thread.sig_waiting.load());

  double phase_rate = (gb->phase_total - gb_thread.old_phase_total) / (gb->sim_time - gb_thread.old_sim_time);

  if (gb->sim_time == gb_thread.old_sim_time) {
    phase_rate = 0;
  }

  gb_thread.phase_rate_smooth = (gb_thread.phase_rate_smooth * 0.99) + (phase_rate * 0.01);

  d("Phase rate    : %f\n",      gb_thread.phase_rate_smooth);
  d("Sim fps       : %f\n",      60.0 * gb_thread.phase_rate_smooth / PHASES_PER_SECOND);

  gb_thread.old_phase_total = gb->phase_total;
  gb_thread.old_sim_time = gb->sim_time;

  /*
  d("Pass count  %lld\n",    pass_count);
  d("Pass rate   %f\n",      double(pass_count) / sim_time);
  d("Phase count %lld\n",    phase_count);
  d("Phase rate  %f\n",      double(phase_count) / sim_time);
  */
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

  text_painter.render(view, d.s.c_str(), cursor, 0);
  cursor += col_spacing;
  d.clear();

  //----------------------------------------

  d("\002===== CPU =====\001\n");
  gb->cpu.dump(d);

  d("\002===== Interrupts =====\001\n");
  gb->interrupts.dump(d);
  d("\n");

  d("\002===== Disasm =====\001\n");
  {
    uint16_t pc = gb->cpu.op_addr;
    const uint8_t* code = nullptr;
    uint16_t code_size = 0;
    uint16_t code_base = 0;

    if (!gb->cpu_bus.BOOT_BITn_h.qp_old()) {
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

  /*
  d("\002===== Joypad =====\001\n");
  gb->joypad.dump(d);
  d("\n");
  */

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

  text_painter.render(view, d.s.c_str(), cursor, 0);
  cursor += col_spacing;
  d.clear();

  //----------------------------------------

  d("\002===== CPU Bus =====\001\n");
  gb->cpu_bus.dump(d);
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

  d("\002===== DMA =====\001\n");
  gb->dma.dump(d);
  d("\n");

  text_painter.render(view, d.s.c_str(), cursor, 0);
  cursor += col_spacing;
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

  d("\002===== PPU Regs=====\001\n");
  d.dump_slice2n("FF40 LCDC  : ", &gb->reg_lcdc.VYXE_LCDC_BGENn, 8);
  d.dump_slice2n("FF41 STAT  : ", &gb->reg_stat.ROXE_STAT_HBI_ENn, 4);
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
  d.dump_bitn   ("REMY_LD0n  : ", gb->pix_pipes.REMY_LD0n.state);
  d.dump_bitn   ("RAVO_LD1n  : ", gb->pix_pipes.RAVO_LD1n.state);

  d("\n");
  d.dump_bitp("XYMU_RENDERINGn        : ", gb->ppu_reg.XYMU_RENDERINGn.state);
  d.dump_bitp("PYNU_WIN_MODE_Ap       : ", gb->win_reg.PYNU_WIN_MODE_Ap.state);
  d.dump_bitp("PUKU_WIN_HITn          : ", gb->win_reg.PUKU_WIN_HITn.state);
  d.dump_bitp("RYDY_WIN_HITp          : ", gb->win_reg.RYDY_WIN_HITp.state);
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

  text_painter.render(view, d.s.c_str(), cursor, 0);
  cursor += col_spacing;
  d.clear();

  //----------------------------------------

  d("\002===== Tile Fetch =====\001\n");
  gb->tile_fetcher.dump(d);
  d("\n");

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

  text_painter.render(view, d.s.c_str(), cursor, 0);
  cursor += col_spacing;
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

  // Draw screen overlay
  {
    memset(overlay, 0, sizeof(overlay));

#if 0
    int fb_x = gb->gb_screen_x;
    int fb_y = gb->gb_screen_y;
    if (fb_y >= 0 && fb_y < 144 && fb_x >= 0 && fb_x < 160) {
      for (int x = 0; x < fb_x; x++) {
        uint8_t p0 = gb->lcd.lcd_pipe_lo[159 - fb_x + x + 1].qp_old();
        uint8_t p1 = gb->lcd.lcd_pipe_hi[159 - fb_x + x + 1].qp_old();
        int r = (3 - (p0 + p1 * 2)) * 30 + 50;
        int g = (3 - (p0 + p1 * 2)) * 30 + 50;
        int b = (3 - (p0 + p1 * 2)) * 30 + 30;
        overlay[x + fb_y * 160] = 0xFF000000 | (b << 16) | (g << 8) | (r << 0);
      }
      {
        uint8_t p0 = gb->lcd.lcd_pix_lo.qp_old();
        uint8_t p1 = gb->lcd.lcd_pix_hi.qp_old();
        int c = (3 - (p0 + p1 * 2)) * 85;
        overlay[fb_x + fb_y * 160] = 0xFF000000 | (c << 16) | (c << 8) | (c << 0);
      }
    }
#endif

    update_texture_u32(overlay_tex, 160, 144, overlay);
  }

  int gb_x = 42 * 32;
  int gb_y = 1 * 32;

  if (has_golden && show_diff) {
    gb_blitter.blit_diff(view,   gb_x, gb_y,  2, framebuffer, golden_u8);
  } else if (show_golden) {
    gb_blitter.blit_screen(view, gb_x, gb_y,  2, golden_u8);
  } else {
    gb_blitter.blit_screen(view, gb_x, gb_y,  2, framebuffer);
  }
  gb_blitter.blit_tiles (view, 33 * 32, 16 * 32, 1, vid_ram);
  gb_blitter.blit_map   (view, 42 * 32, 16 * 32, 1, vid_ram, 0, 0);
  gb_blitter.blit_map   (view, 51 * 32, 16 * 32, 1, vid_ram, 0, 1);
  gb_blitter.blit_map   (view, 42 * 32, 25 * 32, 1, vid_ram, 1, 0);
  gb_blitter.blit_map   (view, 51 * 32, 25 * 32, 1, vid_ram, 1, 1);

  blitter.blit(view, overlay_tex, gb_x, gb_y, 160 * 2, 144 * 2);

  // Status bar under screen

  //double phases_per_frame = 114 * 154 * 60 * 8;
  //double sim_ratio = sim_rate / phases_per_frame;
  double sim_ratio = 0.0;
  double sim_time_smooth = 0.0;

  d("%s %s Sim clock %8.3f %s %s\n",
    runmode_names[runmode],
    stepmode_names[stepmode],
    double(phase_total) / (4194304.0 * 2),
    phase_names[phase_total & 7],
    show_golden ? "GOLDEN IMAGE " : "");
  //d("Sim time %f, sim ratio %f, frame time %f\n", sim_time_smooth, sim_ratio, frame_time_smooth);
  text_painter.render(view, d.s, gb_x, gb_y + 144 * 2);
  d.clear();

  // Probe dump

  gb->probes.dump(d, draw_passes);
  text_painter.render(view, d.s, 34 * 32, 11 * 32);
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
