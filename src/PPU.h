#pragma once

#include <stdint.h>
#include "OAM.h"
#include "Utils.h"

#pragma warning(disable : 4201)

//-----------------------------------------------------------------------------

#define ADDR_TILE0        0x8000
#define ADDR_TILE1        0x8800
#define ADDR_MAP0         0x9800
#define ADDR_MAP1         0x9C00

#define ADDR_OAM_BEGIN    0xFE00
#define ADDR_OAM_END      0xFE9F

#define ADDR_GPU_BEGIN    0xFF40
#define ADDR_LCDC         0xFF40
#define ADDR_STAT         0xFF41
#define ADDR_SCY          0xFF42
#define ADDR_SCX          0xFF43
#define ADDR_LY           0xFF44
#define ADDR_LYC          0xFF45
#define ADDR_DMA          0xFF46
#define ADDR_BGP          0xFF47
#define ADDR_OBP0         0xFF48
#define ADDR_OBP1         0xFF49
#define ADDR_WY           0xFF4A
#define ADDR_WX           0xFF4B
#define ADDR_GPU_END      0xFF4B

#define FLAG_BG_ON        0x01
#define FLAG_OBJ_ON       0x02
#define FLAG_TALL_SPRITES 0x04
#define FLAG_BG_MAP_1     0x08
#define FLAG_TILE_0       0x10
#define FLAG_WIN_ON       0x20
#define FLAG_WIN_MAP_1    0x40
#define FLAG_LCD_ON       0x80

#define TCYCLES_LINE      456
#define TCYCLES_OAM       80
#define TCYCLES_VRAM      172
#define TCYCLES_HBLANK    204

#define HBLANK_DELAY_START  4

#define HBLANK_DELAY_PHASE  3 // 3 = we pass sprite timings?
#define HBLANK_DELAY_LOCK   1
#define HBLANK_DELAY_STATE  1
#define HBLANK_DELAY_INT    1

#define SPRITE_PRI    0x80
#define SPRITE_FLIP_Y 0x40
#define SPRITE_FLIP_X 0x20
#define SPRITE_PAL    0x10

#define PPU_STATE_HBLANK 0
#define PPU_STATE_VBLANK 1
#define PPU_STATE_OAM    2
#define PPU_STATE_VRAM   3

//-----------------------------------------------------------------------------

const static uint32_t gb_colors[8] = {
  0xFF7F7F7F,
  0xFF5F5F5F,
  0xFF3F3F3F,
  0xFF1F1F1F,
  0xFF9F7F7F,
  0xFF7F5F5F,
  0xFF5F3F3F,
  0xFF3F1F1F,
};

//-----------------------------------------------------------------------------

enum FetchState {
  FETCH_TILE_MAP,
  FETCH_TILE_LO,
  FETCH_TILE_HI,

  FETCH_SPRITE_MAP,
  FETCH_SPRITE_LO,
  FETCH_SPRITE_HI,

  FETCH_IDLE,
  FETCH_DONE,
};

//-----------------------------------------------------------------------------
// PPU ticks at 4 mhz

struct PPU {
  void reset();

  void tick(ubit16_t cpu_addr, ubit8_t cpu_data, bool cpu_read, bool cpu_write, uint8_t vram_in, uint8_t oam_in);
  void tock(ubit16_t cpu_addr_, ubit8_t cpu_data_, bool cpu_read_, bool cpu_write_, uint8_t vram_in, uint8_t oam_in);
  char* dump(char* cursor);

  bool is_frame_start() { return frame_start; }
  bool is_frame_done()  { return frame_done; }
  uint8_t get_line()    { return line2; }
  uint8_t get_lcdc()    { return lcdc; }
  int get_pix_count()   { return pix_count; }

  bool vblank_int;
  bool stat_int;

  bool stat_int_lyc;
  bool stat_int_oam;
  bool stat_int_hblank;
  bool stat_int_vblank;
  bool stat_int_glitch;

  ubit8_t bus_out;
  bool bus_oe;

  ubit8_t pix_out;
  bool pix_oe;

  // HAAACK
  ubit16_t vram_addr;
  bool vram_delay;

  bool oam_lock;
  bool vram_lock;

//private:

  void handle_lcd_off(ubit16_t cpu_addr, ubit8_t cpu_data, bool cpu_read, bool cpu_write);
  void merge_sprite();
  void check_sprite_hit();
  void emit_pixel();
  void merge_tile();

  void bus_read(uint16_t cpu_addr);
  void bus_write(uint16_t cpu_addr, uint8_t cpu_data);

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
  ubit8_t wy;   // FF4A
  ubit8_t wx;   // FF4B

  uint8_t palettes[4];

  ubit8_t scy_latch;

  bool lyc_match;

  //----------
  // Timers and states

  uint16_t counter2;
  uint8_t line2;

  bool frame_start;
  bool frame_done;
  int frame_count;

  bool oam_phase;
  bool render_phase;
  bool hblank_phase;

  uint8_t hblank_delay;

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

  FetchState fetch_state;
  bool    window_hit;

  //----------
  // Pixel pipe

  ubit8_t map_x;
  ubit8_t map_y;

  ubit8_t tile_map;
  ubit8_t tile_lo;
  ubit8_t tile_hi;
  bool    tile_latched;

  int pix_count;
  ubit4_t pipe_count;
  int pix_discard;

  ubit8_t bg_pix_lo;
  ubit8_t bg_pix_hi;
  ubit8_t bg_pal_lo;
  ubit8_t bg_pal_hi;

  ubit8_t ob_pix_lo;
  ubit8_t ob_pix_hi;
  ubit8_t ob_pal_lo;
  ubit8_t ob_pal_hi;
};

uint16_t pack_map_addr(uint16_t base, uint8_t map_x, uint8_t map_y);
uint16_t pack_tile_addr(uint16_t base, uint8_t tile, uint8_t ty);
uint16_t tile_map_address(uint8_t lcdc, uint8_t map_x, uint8_t map_y);
uint16_t win_map_address(uint8_t lcdc, uint8_t map_x, uint8_t map_y);
uint16_t tile_base_address(uint8_t lcdc, uint8_t scy, uint8_t line, uint8_t map);
uint16_t win_base_address(uint8_t lcdc, uint8_t wy, uint8_t line, uint8_t map);
uint16_t sprite_base_address(uint8_t lcdc, uint8_t line, uint8_t sprite_y, uint8_t map, uint8_t flags);


//-----------------------------------------------------------------------------