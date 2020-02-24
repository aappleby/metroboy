#pragma once
#include <stdint.h>
#include <string>

struct TextPainter {

  void init();
  void begin_frame();
  void end_frame();

  void add_char(const char c);
  void add_char(const char c, const char d);
  void add_text(const char* text);
  void add_text(const char* text, int len);
  void add_string(const std::string& text);
  void dprintf(const char* format, ...);

  void render(float x, float y, float scale = 1.0f);

  void render(const std::string& text, float x, float y, float scale = 1.0f) {
    add_string(text);
    render(x, y, scale);
  }

  void render(const std::string& text, int x, int y) {
    add_string(text);
    render(float(x), float(y), 1.0f);
  }

  void newline() { add_char(1, '\n'); }

  void set_pal(int index, float r, float g, float b, float a);

  uint32_t text_prog;
  uint32_t font_tex;
  
  int text_x = 0;
  int text_y = 0;

  int bg_pal = 0;
  int fg_pal = 1;

  int pal_locs[16];

  int inst_begin = 0;
  int inst_end = 0;

  uint32_t  quad_vbo;

  int buf_idx = 0;
  uint32_t  text_vaos[3];
  uint32_t  inst_vbos[3];
  uint32_t* inst_maps[3];
};

#if 0

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

#endif