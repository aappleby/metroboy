#pragma once

#include "Viewport.h"
#include <stdint.h>

//-----------------------------------------------------------------------------

class GBBlitter {
public:
  void init();

  void blit_tiles (Viewport view, int screen_x, int screen_y, const uint8_t* vram);
  void blit_map   (Viewport view, int screen_x, int screen_y, int scale, const uint8_t* vram);
  void blit_screen(Viewport view, int screen_x, int screen_y, int scale, const uint8_t* framebuffer);
  void blit_trace (Viewport view, int screen_x, int screen_y, const uint32_t* trace);

  uint32_t vram_ubo;
  uint32_t quad_vao;
  uint32_t quad_vbo;

  uint32_t blit_map_ubo;
  uint32_t blit_map_prog;

  uint32_t blit_tile_ubo;
  uint32_t blit_tile_prog;

  uint32_t blit_screen_prog;
  uint32_t blit_screen_tex;

  uint32_t blit_trace_prog;
  uint32_t blit_trace_tex;
};

//-----------------------------------------------------------------------------
