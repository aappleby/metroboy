#pragma once
#include <string>

#include "AppLib/Viewport.h"

struct TextUniforms {
  vec4 viewport = {};
  vec4 text_pos = {};
  vec4 bg_palette = {};
  vec4 palette[16] = {};
};

class TextPainter {
public:

  void init();

  void add_char(const char c);
  void add_char(const char c, const char d);
  void add_text(const char* text_painter);
  void add_text(const char* text_painter, int len);
  void add_string(const std::string& text_painter);
  void dprintf(const char* format, ...);

  void render(Viewport view, double x, double y, float scale = 1.0f);

  void render(Viewport view, const std::string& text, double x, double y, float scale = 1.0f) {
    add_string(text);
    render(view, x, y, scale);
  }

  void newline() { add_char(1, '\n'); }

  void set_pal(int index, float r, float g, float b, float a);

private:

  uint32_t text_prog = 0;
  uint32_t font_tex = 0;
  uint64_t font_ptr = 0;

  uint32_t dump_tex = 0;

  int text_x = 0;
  int text_y = 0;

  int fg_pal = 1;

  int pal_locs[16];

  int inst_begin = 0;
  int inst_end = 0;

  uint32_t dummy_vao = 0;

  uint32_t  inst_vbo = 0;
  uint32_t* inst_data = 0;

  uint32_t text_ubo = 0;
  TextUniforms text_uniforms = {};
};
