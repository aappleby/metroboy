#pragma once
#include <string>

#include "AppLib/Viewport.h"

struct TextUniforms {
  vec4 viewport = {};
  vec4 origin = {};
  vec4 bg_col = {};
  vec4 palette[16] = {};
};

class TextPainter {
public:

  void init();

  void push_char(float x, float y, int c, int pal);

  void add_char(const char c);
  void add_char(const char c, const char d);

  void add_text(const char* text_painter);
  void add_text(const char* text_painter, int len);
  void add_text_at(const char* s, float x, float y);

  void add_string(const std::string& text_painter);
  void dprintf(const char* format, ...);

  void render(Viewport view, double x, double y, float scale = 1.0f);

  void render(Viewport view, const std::string& text, double x, double y, float scale = 1.0f) {
    add_string(text);
    render(view, x, y, scale);
  }

  void newline() { add_char(1, '\n'); }

  void set_pal(int index, float r, float g, float b, float a);

//private:

  const int max_text_bytes = 1024 * 1024;
  const int bytes_per_glyph = 16;
  const int max_glyphs = max_text_bytes / bytes_per_glyph;

  vec4 bg_col;

  uint32_t text_prog = 0;
  uint32_t font_tex = 0;
  uint64_t font_ptr = 0;

  uint32_t dump_tex = 0;

  float text_x = 0;
  float text_y = 0;

  int fg_pal = 1;

  int pal_locs[16];

  int glyph_count = 0;

  uint32_t  text_vao = 0;
  uint32_t  text_vbo = 0;
  uint32_t  text_ubo = 0;

  uint32_t* text_data_u32 = 0;
  float*    text_data_f32 = 0;

  TextUniforms text_uniforms = {};
};
