#pragma once
#include <string>

#include "AppLib/Viewport.h"

class DumpPainter {
public:

  void init_hex();
  void init_ascii();
  void begin_frame(Viewport view);
  void end_frame();

  void dump(Viewport view, double world_x, double world_y, double scale_x, double scale_y, int text_w, int text_h, const uint8_t* dump);
  void dump_ascii(Viewport view, double x, double y, int w, int h, const uint8_t* dump);

private:

  uint32_t dump_prog = 0;
  uint32_t dump_vao = 0;

  uint32_t dump_tab = 0;
  uint32_t glyph_tex = 0;

  uint32_t ruler_x_tab = 0;
  uint32_t ruler_y_tab = 0;

  int highlight_x = -1;
  int highlight_y = -1;

  int tile_w = 14;
  int tile_h = 12;
  int cell_w = 6;
  int cell_h = 12;

  //int tile_w = 6;
  //int tile_h = 10;
  //int cell_w = 6;
  //int cell_h = 10;

  uint32_t masks[16];
  uint32_t shifts[16];

};
