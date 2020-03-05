#pragma once
#include <stdint.h>
#include <string>

#include "Viewport.h"

struct TextUniforms {
  vec4 screen_size;
  vec4 viewport;
  vec4 text_pos;
  vec4 bg_palette;
  vec4 palette[16];
};

class TextPainter {
public:

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

  void set_viewport(Viewport vp) { viewport = vp; }

private:

  Viewport viewport;

  uint32_t text_prog;
  uint32_t font_tex;
  uint64_t font_ptr;

  int text_x = 0;
  int text_y = 0;

  int fg_pal = 1;

  int pal_locs[16];

  int inst_begin = 0;
  int inst_end = 0;

  uint32_t dummy_vao;

  uint32_t  inst_vbo;
  uint32_t* inst_data;

  uint32_t text_ubo;
  TextUniforms text_uniforms;
};
