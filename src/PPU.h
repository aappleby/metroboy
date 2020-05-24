#pragma once
#include <stdint.h>
#include "OAM.h"
#include "VRAM.h"
#include "Constants.h"

//-----------------------------------------------------------------------------

struct PPU {
  /*
  struct Out {
    int x;
    int y;
    int counter;
    uint8_t pix_out;
    bool pix_oe;

    bool stat1;
    bool stat2;
    bool vblank1;
    bool vblank2;
  };
  */

  void reset(bool run_bootrom);

  void tock_req(const Req& ibus_req);
  void tick_ack(Ack& ibus_ack) const;

  void get_vbus_req(Req& r) const;
  void get_obus_req(Req& r) const;

  void on_vbus_ack(const Ack& vbus_ack);
  void on_obus_ack(const Ack& obus_ack);

  void tock(const int tcycle);

  void dump(std::string& out) const;

  uint8_t get_stat()       const { return stat; }

  bool get_old_stat_int1() const { return old_stat_int2; }
  bool get_old_stat_int2() const { return old_stat_int2; }

//private:

  void tock_lcdoff();
  void emit_pixel(int tphase);
  void merge_tile(int tphase);

  Ack ack;

  //----------
  // Timers and states

  int  frame_count = 0;
  bool frame_start = 0;
  bool frame_done = 0;

  uint8_t line = 0;
  uint8_t line_delay1 = 0;
  uint8_t line_delay2 = 0;
  uint8_t line_delay3 = 0;

  int state = 0;

  uint16_t counter = 0;
  uint16_t counter_delay1 = 0;
  uint16_t counter_delay2 = 0;
  uint16_t counter_delay3 = 0;

  uint8_t hblank_delay2 = 0;

  //----------
  // Registers

  uint8_t lcdc = 0; // FF40
  uint8_t stat = 0; // FF41
  uint8_t scy = 0;  // FF42
  uint8_t scx = 0;  // FF43
  uint8_t ly = 0;   // FF44
  uint8_t lyc = 0;  // FF45
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
  // interrupt stuff

  int compare_line = 0;

  uint8_t stat_int1 = 0;
  uint8_t stat_int2 = 0;

  bool old_stat_int1 = 0;
  bool old_stat_int2 = 0;

  uint8_t scx_latch = 0;
  uint8_t win_y_latch = 0;
  uint8_t win_y_counter = 0;

  //----------
  // Sprites

  uint8_t sprite_count = 0;
  int8_t sprite_index = 0;
  int sprite_hit = 0;

  uint8_t spriteY = 0;
  uint8_t spriteX = 0;
  uint8_t spriteP = 0;
  uint8_t spriteF = 0;

  uint8_t sprite_lo = 0;
  uint8_t sprite_hi = 0;

  uint8_t sprite_x[10] = {0}; // 80 bits
  uint8_t sprite_y[10] = {0}; // 80 bits
  uint8_t sprite_i[10] = {0}; // 60 bits?

  //----------
  // Vram Fetcher

  enum FetchType {
    FETCH_NONE,
    FETCH_BACKGROUND,
    FETCH_WINDOW,
    FETCH_SPRITE,
  };

  enum FetchState {
    FETCH_IDLE,
    
    FETCH_TILE_MAP,
    FETCH_TILE_LO,
    FETCH_TILE_HI,

    FETCH_SPRITE_MAP,
    FETCH_SPRITE_LO,
    FETCH_SPRITE_HI,
  };

  FetchType fetch_type = FETCH_NONE;
  FetchState fetch_state = FETCH_IDLE;
  bool fetch_delay = 0;

  bool in_window_old = 0;
  bool in_window_new = 0;
  bool in_window_early = 0;
  bool in_window_late = 0;

  bool win_retrig_old = 0;
  bool win_retrig_new = 0;

  //----------
  // Pixel pipe

  uint8_t map_x = 0;

  uint8_t tile_map = 0;
  uint8_t tile_lo = 0;
  uint8_t tile_hi = 0;
  bool    tile_latched = 0;

  uint8_t pix_count = 0;
  uint8_t pix_discard_scx = 0;
  uint8_t pix_discard_pad = 0;
  uint8_t pipe_count = 0;

  uint8_t bg_pix_lo = 0;
  uint8_t bg_pix_hi = 0;
  uint8_t bg_pal_lo = 0;
  uint8_t bg_pal_hi = 0;

  uint8_t ob_pix_lo = 0;
  uint8_t ob_pix_hi = 0;
  uint8_t ob_pal_lo = 0;
  uint8_t ob_pal_hi = 0;

  uint8_t pix_out = 0;
  bool pix_oe = 0;

  bool stat1 = 0;
  bool stat2 = 0;
  bool vblank1 = 0;
  bool vblank2 = 0;
};

//-----------------------------------------------------------------------------