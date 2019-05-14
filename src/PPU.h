#pragma once
#include "OAM.h"
#include "Constants.h"

//-----------------------------------------------------------------------------

struct PPU {
  void reset(bool run_bootrom, int new_model);
  int model = MODEL_DMG;

  void tick(int tphase, ubit16_t cpu_addr, ubit8_t cpu_data, bool cpu_read, bool cpu_write);
  void tock_lcdoff(int tphase, ubit16_t cpu_addr_, ubit8_t cpu_data_, bool cpu_read_, bool cpu_write_, uint8_t vram_in, uint8_t oam_in);
  void tock(int tphase, ubit16_t cpu_addr_, ubit8_t cpu_data_, bool cpu_read_, bool cpu_write_, uint8_t vram_in, uint8_t oam_in);
  char* dump(char* cursor);

  bool is_frame_start() { return frame_start; }
  bool is_frame_done()  { return frame_done; }
  uint8_t get_line()    { return line; }
  uint8_t get_lcdc()    { return lcdc; }
  int get_pix_count()   { return pix_count2; }

  //bool stat_int_oam;
  //bool stat_int_vblank;
  //bool stat_int_glitch;

  ubit8_t bus_out;
  bool bus_oe;

  ubit8_t pix_out;
  bool pix_oe;

  // HAAACK
  ubit16_t vram_addr;

  bool oam_lock;
  bool vram_lock;

//private:

  void merge_sprite(int tphase);
  void check_sprite_hit(int tphase);
  void emit_pixel(int tphase);
  void merge_tile(int tphase);

  void bus_read_early(uint16_t cpu_addr);
  void bus_read_late(uint16_t cpu_addr);
  void bus_write_early(uint16_t cpu_addr, uint8_t cpu_data);
  void bus_write_late (uint16_t cpu_addr, uint8_t cpu_data);

  void dump_tiles(uint32_t* framebuffer, int stride, int sx, int sy, int scale, const uint8_t* vram) const;
  void draw_sprite(OAM& oam, uint32_t* framebuffer, int stride, int sx, int sy, int scale, const uint8_t* vram, int sprite_index) const;
  void draw_map(uint32_t* framebuffer, int stride, int sx, int sy, int scale, const uint8_t* map, const uint8_t* tiles, uint8_t map_flip) const;
  void draw_bg_map(uint32_t* framebuffer, int stride, int sx, int sy, int scale, const uint8_t* vram) const;
  void draw_wm_map(uint32_t* framebuffer, int stride, int sx, int sy, int scale, const uint8_t* vram) const;

  //----------
  // Registers

  ubit8_t lcdc; // FF40
  ubit8_t stat; // FF41
  ubit8_t scy;  // FF42
  ubit8_t scx;  // FF43
  ubit8_t ly;   // FF44
  ubit8_t lyc;  // FF45
  ubit8_t dma;  // FF46
  ubit8_t bgp;  // FF47
  ubit8_t obp0; // FF48
  ubit8_t obp1; // FF49
  ubit8_t wy;   // FF4A
  ubit8_t wx;   // FF4B

  uint8_t wx_delay;
  uint8_t bgp_early;

  uint8_t palettes[4];

  ubit8_t scy_latch;

  uint8_t lcdc_delay;

  uint8_t win_x_latch = 0;
  uint8_t win_y_latch = 0;
  uint8_t win_y_counter = 0;

  //----------
  // Timers and states

  int state;

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

  int old_stat_int = 0;
  int stat_int;
  int compare_line;

  int new_stat_int = 0;

  //----------
  // Sprites

  uint16_t oam_addr;
  uint8_t oam_data;
  bool oam_read;

  ubit4_t sprite_count;
  sbit8_t sprite_index;
  bool    sprite_latched;
  uint8_t sprite_x[10]; // 80 bits
  uint8_t sprite_y[10]; // 80 bits
  uint8_t sprite_i[10]; // 60 bits?

  ubit8_t spriteY;
  ubit8_t spriteX;
  ubit8_t spriteP;
  ubit8_t spriteF;

  ubit8_t sprite_lo;
  ubit8_t sprite_hi;

  //----------
  // Vram Fetcher

  enum FetchType {
    FETCH_BACKGROUND,
    FETCH_WINDOW,
    FETCH_SPRITE,
    FETCH_NONE
  };

  enum FetchState {
    FETCH_MAP,
    FETCH_LO,
    FETCH_HI,
    FETCH_IDLE,
  };

  FetchType fetch_type = FETCH_NONE;
  bool fetch_delay = false;
  FetchState fetch_state;
  bool    window_hit;
  bool    fetch_restarted;

  //----------
  // Pixel pipe

  ubit8_t map_x;
  ubit8_t map_y;

  ubit8_t tile_map;
  ubit8_t tile_lo;
  ubit8_t tile_hi;
  bool    tile_latched;

  int pix_count2;
  int pix_discard;
  int next_pix;
  ubit4_t pipe_count;

  ubit8_t bg_pix_lo;
  ubit8_t bg_pix_hi;
  ubit8_t bg_pal_lo;
  ubit8_t bg_pal_hi;

  ubit8_t ob_pix_lo;
  ubit8_t ob_pix_hi;
  ubit8_t ob_pal_lo;
  ubit8_t ob_pal_hi;
};

//-----------------------------------------------------------------------------