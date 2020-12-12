#pragma once
#include "MetroBoyLib/OAM.h"
#include "MetroBoyLib/VRAM.h"
#include "CoreLib/Constants.h"

struct LCD;

//-----------------------------------------------------------------------------

struct PPU {
  void reset_cart();
  void reset_boot();

  bool    read (uint16_t addr, uint8_t& out);
  bool    write(uint16_t addr, uint8_t data);

  uint8_t read(uint16_t addr) {
    uint8_t temp = 0;
    read(addr, temp);
    return temp;
  }

  void tick(int phase_total, const Req& req, Ack& ibus_ack);
  void tock(int phase_total, const Req& ibus_req, const Ack vbus_ack, const Ack obus_ack);
  //void tock_lcdoff();

  void get_vbus_req(Req& r) const;
  void get_obus_req(Req& r) const;

  void emit_pixel();

  void dump(Dumper& dump) const;

  //----------
  // Timers and states

  // 114 * 8 = 912 phases per line

  // CATU_LINE_P000 = 1   @   0  - scan index reset, pix count reset, mode 2
  // BESU_SCANNINGp = 1   @   0
  // NYPE_LINE_P002 = 1   @   2
  // ANEL_LINE_P002 = 1   @   2
  // CENO_SCANNINGp = 1   @   4
  // XYMU_RENDERINGp = 1  @ 160
  // BYBA_SCAN_DONE_A = 1 @ 160
  // DOBA_SCAN_DONE_B = 1 @ 161
  // pix count 1          @ 174
  // pix count 167        @ 506
  // XYMU_RENDERINGp = 0  @ 507
  // RUTU_LINE_P910 = 1   @ 910

  int     phase_count = 0;
  uint8_t line = 0; // this is the actual line, no glitches
  bool    lyc_match = 0;
  int     frame_count = 0;
  uint8_t stat_int = 0;
  uint8_t vblank_int = 0;

  //----------
  // Registers

  uint8_t reg_lcdc = 0; // FF40
  uint8_t stat = 0; // FF41
  uint8_t scy = 0;  // FF42
  uint8_t scx = 0;  // FF43
  uint8_t ly = 0;   // FF44 // this line counter contains glitches
  uint8_t reg_lyc = 0;  // FF45
  uint8_t bgp = 0;  // FF47
  uint8_t obp0 = 0; // FF48
  uint8_t obp1 = 0; // FF49
  uint8_t wy = 0;   // FF4A
  uint8_t wx = 0;   // FF4B


  uint8_t palettes[4] = {0};

  void update_palettes() {
    palettes[0] = bgp;
    palettes[1] = bgp;
    palettes[2] = obp0;
    palettes[3] = obp1;
  }

  //----------
  // Sprite store

  int8_t  sprite_index = 0;
  uint8_t sprite_count = 0;
  int     sprite_hit = 0;

  uint8_t spriteY = 0;
  uint8_t spriteX = 0;
  uint8_t spriteP = 0;
  uint8_t spriteF = 0;

  uint8_t sprite_x[10] = {0}; // 80 bits
  uint8_t sprite_y[10] = {0}; // 80 bits
  uint8_t sprite_i[10] = {0}; // 60 bits?

  //----------
  // Vram Fetcher

  uint8_t bg_fetch_state = 0;
  uint8_t sp_fetch_state = 0;

  //----------
  // Window stuff

#if 0
  uint8_t win_y_latch = 0;
  uint8_t win_y_counter = 0;

  bool in_window_old = 0;
  bool in_window_new = 0;
  bool in_window_early = 0;
  bool in_window_late = 0;

  bool win_retrig_old = 0;
  bool win_retrig_new = 0;
#endif

  //----------
  // Pixel pipe

  //uint8_t map_x = 0;

  uint8_t tile_map = 0;
  uint8_t tile_lo = 0;
  uint8_t tile_hi = 0;
  bool    tile_latched = 0;

  uint8_t pix_count = 0;

  uint8_t pix_discard_scx = 0;
  uint8_t pix_discard_pad = 0;

  uint8_t bg_pipe_lo  = 0;
  uint8_t bg_pipe_hi  = 0;
  uint8_t spr_pipe_lo = 0;
  uint8_t spr_pipe_hi = 0;
  uint8_t pal_pipe    = 0;
  uint8_t mask_pipe   = 0;

  uint8_t pix_out = 0;
  bool    pix_oe = 0;
};

//-----------------------------------------------------------------------------