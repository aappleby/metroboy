#pragma once
#include <stdint.h>

struct TextPainter {
  int glyph_width;
  int glyph_height;
  int glyph_stride;
  int fb_width;
  int fb_height;
  uint32_t* framebuffer;
  uint8_t* font;
  int frame_count;

  void begin_frame(int x, int y) {
    trace_x = x;
    trace_y = y;
    row = 0;
    col = 0;
  }

  int clock;
  int trace_x;
  int trace_y;
  int highlight_row = 0;
  int highlight_col = 0;

  int row = 0;
  int col = 0;

  void render_glyph(int dst_x, int dst_y, char c, uint32_t color);
  void trace(const char* name, bool c);
  void render_text(int dst_x, int dst_y, const char* text);
  void draw_bbox(int sx, int sy, int w, int h, uint32_t color);
};
