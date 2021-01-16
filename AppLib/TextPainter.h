#pragma once
#include <string>

#include "AppLib/Viewport.h"

struct TextUniforms {
  vec4 viewport = {};
  vec4 origin = {};
  vec4 bg_col = {};
  //vec4 palette[16] = {};
};

class TextPainter {
public:

  void init();

  void add_char(const char c, double scale = 1.0);
  //void add_char(const char c, const char d);

  void add_text(const char* text, double scale = 1.0);
  void add_text(const char* text, int len, double scale = 1.0);

  void add_text_at(const char* s, uint32_t color, double x, double y, double scale);
  void add_text_at(const char* s, double x, double y, double scale = 1.0);

  void add_string(const std::string& text, double scale = 1.0);
  void dprintf(const char* format, ...);

  void render_buf(Viewport view, double x, double y);

  void render_string(Viewport view, const std::string& text, double x, double y, double scale = 1.0) {
    add_string(text, scale);
    render_buf(view, x, y);
  }

  void newline() { add_char(1, '\n'); }

  void set_pal(int index, double r, double g, double b, double a);

private:

  void push_char(double x, double y, double s, int c, uint32_t color);

  const int max_text_bytes = 1024 * 1024 * 4;
  const int bytes_per_glyph = 20;
  const int max_glyphs = max_text_bytes / bytes_per_glyph;

  vec4 bg_col;

  uint32_t text_prog = 0;
  uint32_t font_tex = 0;
  uint64_t font_ptr = 0;

  uint32_t dump_tex = 0;

  double text_x = 0;
  double text_y = 0;

  int current_color = 0;

  int buf_cursor = 0;
  int glyph_count = 0;

  uint32_t  text_vao = 0;
  uint32_t  text_vbo = 0;
  uint32_t  text_ubo = 0;

  uint32_t* text_data_u32 = 0;
  float*    text_data_f32 = 0;

  int fg_pal = 1;
  uint32_t palette[256];

  TextUniforms text_uniforms = {};
};
