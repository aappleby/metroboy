#pragma once
#include <stdint.h>

struct TextPainter {
  TextPainter();

  void begin_frame(uint32_t* fb, int width, int height);
  void render_glyph(int dst_x, int dst_y, char c, uint32_t color);
  void render_text(int dst_x, int dst_y, const char* text);
  void draw_bbox(int sx, int sy, int w, int h, uint32_t color);

private:
  uint32_t* framebuffer;
  int fb_width;
  int fb_height;
  uint8_t* font;
};
