#pragma once
#include <string>

#include "AppLib/Viewport.h"

class DumpPainter {
public:

  void init();
  void begin_frame(Viewport view);
  void end_frame();

  void render(Viewport view, double x, double y, int w, int h, const uint8_t* dump);

private:

  uint32_t dump_prog = 0;
  uint32_t dump_vao = 0;

  uint32_t dump_tab = 0;
  uint32_t atlas_tex = 0;
  uint32_t hexa_tex = 0;

  uint32_t ruler_x_tab = 0;
  uint32_t ruler_y_tab = 0;

  int highlight_x = 0;
  int highlight_y = 0;

  static const int tile_w = 14;
  static const int tile_h = 12;
  static const int cell_w = 6;
  static const int cell_h = 12;
};
