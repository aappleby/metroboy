#pragma once

#include "AppLib/Viewport.h"

//-----------------------------------------------------------------------------

class GBBlitter {
public:
  void init();

  void blit_tiles (Viewport view, dvec2 screen_size, int screen_x, int screen_y, int scale, const uint8_t* vram);
  void blit_map   (Viewport view, dvec2 screen_size, int screen_x, int screen_y, int scale, const uint8_t* vram, int which_map, int alt_map);
  void blit_screen(Viewport view, dvec2 screen_size, int screen_x, int screen_y, int scale, const uint8_t* framebuffer);
  void blit_diff  (Viewport view, dvec2 screen_size, int screen_x, int screen_y, int scale, const uint8_t* test_tex, const uint8_t* gold_tex);
  void blit_trace (Viewport view, dvec2 screen_size, int screen_x, int screen_y, const uint32_t* trace);

  uint32_t vram_ubo = 0;
  uint32_t quad_vao = 0;
  uint32_t quad_vbo = 0;

  uint32_t blit_map_ubo = 0;
  uint32_t blit_map_prog = 0;

  uint32_t blit_tile_ubo = 0;
  uint32_t blit_tile_prog = 0;

  uint32_t blit_screen_prog = 0;
  uint32_t blit_screen_tex = 0;

  uint32_t blit_diff_prog = 0;
  uint32_t blit_diff_tex = 0;

  uint32_t blit_trace_prog = 0;
  uint32_t blit_trace_tex = 0;
};

//-----------------------------------------------------------------------------
