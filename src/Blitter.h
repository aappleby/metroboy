#pragma once

#include "Viewport.h"
#include <stdint.h>

//-----------------------------------------------------------------------------

class Blitter {
public:
  void init();

  void blit(Viewport view,
            uint32_t tex, int tw, int th,
            int sx, int sy, int sw, int sh,
            int dx, int dy, int dw, int dh);

  Viewport view_snap;

  uint32_t blit_vao;
  uint32_t blit_vbo;
  uint32_t blit_ubo;
  uint32_t blit_prog;
};

//-----------------------------------------------------------------------------