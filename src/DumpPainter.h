#pragma once
#include <stdint.h>
#include <string>

#include "Viewport.h"

class DumpPainter {
public:

  void init();
  void begin_frame(Viewport view);
  void end_frame();

  void render(Viewport view, double x, double y, int w, int h, const uint8_t* dump);

private:

  uint32_t dump_prog;
  uint32_t dump_vao;

  uint32_t dump_tab;
  uint32_t atlas_tex;
  uint32_t hexa_tex;

  uint32_t ruler_x_tab;
  uint32_t ruler_y_tab;

  static const int tile_w = 16;
  static const int tile_h = 12;
  static const int cell_w = 6;
  static const int cell_h = 12;
};
