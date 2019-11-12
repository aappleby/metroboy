`ifndef PPU_SV
`define PPU_SV

`include "Constants.sv"
`include "types.sv"

import constants::*;
import types::*;

//-----------------------------------------------------------------------------

module PPU (
  input bool clk,
  input bool rst,
  input CpuBus bus
);

  uint8_t stat_int1;
  uint8_t stat_int2;

  uint8_t bus_out;
  bool bus_oe;

  uint16_t vram_addr;

  uint16_t oam_addr;
  bool oam_read;

  uint8_t pix_out;
  bool pix_oe;

  //----------
  // Registers

  uint8_t lcdc; // FF40
  uint8_t stat; // FF41
  uint8_t scy;  // FF42
  uint8_t scx;  // FF43
  uint8_t ly;   // FF44
  uint8_t lyc;  // FF45
  uint8_t dma;  // FF46
  uint8_t bgp;  // FF47
  uint8_t obp0; // FF48
  uint8_t obp1; // FF49
  uint8_t wy;   // FF4A
  uint8_t wx;   // FF4B

  uint8_t bgp_early;

  uint8_t palettes[4];


  uint8_t scx_latch;
  uint8_t win_y_latch = 0;
  uint8_t win_y_counter = 0;

  //----------
  // Timers and states

  uint8_t state;

  uint16_t counter;
  uint8_t line;

  uint16_t counter_delay1;
  uint8_t line_delay1;

  uint16_t counter_delay2;
  uint8_t line_delay2;

  uint16_t counter_delay3;
  uint8_t line_delay3;

  bool frame_start;
  bool frame_done;
  int frame_count;

  uint8_t hblank_delay2;

  int compare_line;

  //----------
  // Sprites

  uint8_t sprite_count;
  int8_t sprite_index;
  uint8_t sprite_x[10]; // 80 bits
  uint8_t sprite_y[10]; // 80 bits
  uint8_t sprite_i[10]; // 60 bits?

  uint8_t spriteY;
  uint8_t spriteX;
  uint8_t spriteP;
  uint8_t spriteF;

  uint8_t sprite_lo;
  uint8_t sprite_hi;

  //----------
  // Vram Fetcher

  typedef enum {
    FETCH_BACKGROUND,
    FETCH_WINDOW,
    FETCH_SPRITE,
    FETCH_NONE
  } FetchType;

  typedef enum {
    FETCH_MAP,
    FETCH_LO,
    FETCH_HI,
    FETCH_IDLE
  } FetchState;

  FetchType fetch_type = FETCH_NONE;
  bool fetch_delay = false;
  FetchState fetch_state;
  int sprite_hit;

  bool in_window_old;
  bool in_window_new;
  bool in_window_new_early;
  bool in_window_late;

  bool window_retrigger_old;
  bool window_retrigger_new;

  //----------
  // Pixel pipe

  uint8_t map_x;
  uint8_t map_y;

  uint8_t tile_map;
  uint8_t tile_lo;
  uint8_t tile_hi;
  bool    tile_latched;

  int pix_count2;
  int pix_discard_scx;
  int pix_discard_pad;
  uint8_t pipe_count;

  uint8_t bg_pix_lo;
  uint8_t bg_pix_hi;
  uint8_t bg_pal_lo;
  uint8_t bg_pal_hi;

  uint8_t ob_pix_lo;
  uint8_t ob_pix_hi;
  uint8_t ob_pal_lo;
  uint8_t ob_pal_hi;

  //-----------------------------------------------------------------------------

  task reset;
    bus_out = 0;
    bus_oe = 0;

    pix_out = 0;
    pix_oe = 0;

    vram_addr = 0;

    //----------
    // Registers

    lcdc = 0;
    stat = 8'h80;
    scy = 0;
    scx = 0;
    ly = 0;
    lyc = 0;
    dma = 0;
    bgp = 0;
    obp0 = 0;
    obp1 = 0;
    wy = 0;
    wx = 0;

    palettes[0] = 0;
    palettes[1] = 0;
    palettes[2] = 0;
    palettes[3] = 0;

    scx_latch = 0;
    win_y_latch = 0;
    win_y_counter = 0;

    //----------
    // Timers and states

    state = PPU_STATE_HBLANK;

    frame_start = 0;
    frame_done = 0;
    frame_count = 0;

    line = 0;
    line_delay1 = 0;
    line_delay2 = 0;
    line_delay3 = 0;

    counter = 3;
    counter_delay1 = 2;
    counter_delay2 = 1;
    counter_delay3 = 0;

    hblank_delay2 = HBLANK_DELAY_START;

    stat_int1 = 0;
    stat_int2 = 0;
    compare_line = 0;

    //----------
    // Sprites

    oam_addr = 0;
    oam_read = false;

    sprite_count = 0;
    sprite_index = -1;
    foreach(sprite_x[i]) sprite_x[i] = 8'hFF;
    foreach(sprite_y[i]) sprite_y[i] = 8'hFF;
    foreach(sprite_i[i]) sprite_i[i] = 8'hFF;

    spriteY = 0;
    spriteX = 0;
    spriteP = 0;
    spriteF = 0;

    sprite_lo = 0;
    sprite_hi = 0;

    //----------
    // Pixel pipe

    fetch_state = FETCH_IDLE;
    fetch_delay = false;
    sprite_hit = 15;

    in_window_old = false;
    in_window_new = false;
    in_window_new_early = false;
    window_retrigger_old = false;
    window_retrigger_new = false;

    tile_map = 0;
    tile_lo = 0;
    tile_hi = 0;
    tile_latched = 0;

    pix_count2 = 0;
    pipe_count = 0;
    pix_discard_scx = 0;
    pix_discard_pad = 0;

    bg_pix_lo = 0;
    bg_pix_hi = 0;
    bg_pal_lo = 0;
    bg_pal_hi = 0;

    ob_pix_lo = 0;
    ob_pix_hi = 0;
    ob_pal_lo = 0;
    ob_pal_hi = 0;

    //----------
    // Fixup if we're not running the bootrom

    dma = 8'hFF;
    bgp = 8'hFC;
    bgp_early = 8'hFC;
    obp0 = 8'hFF;
    obp1 = 8'hFF;

    line = 153;
    line_delay1 = 153;
    line_delay2 = 153;
    line_delay3 = 153;

    counter = 403;
    counter_delay1 = 402;
    counter_delay2 = 401;
    counter_delay3 = 400;

    lcdc = 8'h91;
    palettes[0] = 8'hfc;
    pix_count2 = 160;
    pix_discard_scx = 0;
    pix_discard_pad = 8;

  endtask;

  //----------

  function tick;
  endfunction

  //----------

  task tock(CpuBus bus);
  endtask;

  //----------

  always @(posedge clk) begin
    if (rst) reset();
    else tock(bus);
  end

endmodule;


//-----------------------------------------------------------------------------

`endif