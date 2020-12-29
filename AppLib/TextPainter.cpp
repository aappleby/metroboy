#include "AppLib/TextPainter.h"

#include "AppLib/GLBase.h"

#include <stdio.h>
#include "SDL/include/SDL.h"
#include "glad/glad.h"

extern const char* terminus;

const int glyph_size_x = 6;
const int glyph_size_y = 12;

//-----------------------------------------------------------------------------

const char* text_glsl = R"(
const float glyph_size_x = 6.0;
const float glyph_size_y = 12.0;

const float atlas_stride_x = 8.0;
const float atlas_stride_y = 16.0;

const float atlas_width = 256.0;
const float atlas_height = 128.0;

const float inv_atlas_width = 1.0 / atlas_width;
const float inv_atlas_height = 1.0 / atlas_height;

layout(std140) uniform TextUniforms {
  vec4  viewport;
  vec4  origin;
  vec4  bg_col;
  vec4  palette[16];
};

uniform sampler2D font_tex;

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout(location = 0) in ivec4 glyph2;

out vec2  tc_glyph;
out vec4  fg_col;

void main() {
  float corner_x = float((gl_VertexID >> 0) & 1);
  float corner_y = float((gl_VertexID >> 1) & 1);

  float glyph_x = float(glyph2.x);
  float glyph_y = float(glyph2.y);
  float col     = float((glyph2.z >> 0) & 0x1F);
  float row     = float((glyph2.z >> 5) & 0x07);
  int fg_style  = glyph2.w;

  float glyph_tcx = (col * atlas_stride_x) + (corner_x * glyph_size_x);
  float glyph_tcy = (row * atlas_stride_y) + (corner_y * glyph_size_y);

  glyph_tcx = remap(glyph_tcx, 0.0, atlas_width,  0.0, 1.0);
  glyph_tcy = remap(glyph_tcy, 0.0, atlas_height, 0.0, 1.0);

  tc_glyph = vec2(glyph_tcx, glyph_tcy);
  fg_col = palette[fg_style];

  //----------

  float quad_x = glyph_x * origin.z + corner_x * glyph_size_x * origin.z + origin.x;
  float quad_y = glyph_y * origin.w + corner_y * glyph_size_y * origin.w + origin.y;

  gl_Position = vec4(remap(quad_x, viewport.x, viewport.z, -1.0,  1.0),
                     remap(quad_y, viewport.y, viewport.w,  1.0, -1.0),
                     0.0,
                     1.0);
}

#else

in vec2 tc_glyph;
in vec4 fg_col;

out vec4 fs_out;

void main() {
  float p = texture(font_tex, tc_glyph).r;

  fs_out = mix(bg_col, fg_col, p);
}

#endif
)";

//-----------------------------------------------------------------------------

void TextPainter::init() {
  text_prog = create_shader("text_glsl", text_glsl);

  set_pal(0, 0.4f, 0.4f, 0.4f, 1.0f); // grey
  set_pal(1, 0.8f, 0.8f, 0.8f, 1.0f); // white
  set_pal(2, 0.6f, 1.0f, 0.6f, 1.0f); // lo-z out = green
  set_pal(3, 1.0f, 0.6f, 0.6f, 1.0f); // hi-z out = red
  set_pal(4, 0.6f, 0.6f, 1.0f, 1.0f); // lo-z in = blue
  set_pal(5, 1.0f, 1.0f, 0.6f, 1.0f); // hi-z in = yellow
  set_pal(6, 1.0f, 0.6f, 1.0f, 1.0f); // error magenta
  set_pal(7, 0.4f, 0.4f, 0.4f, 1.0f); // grey

  text_data = new uint16_t[max_text_bytes / sizeof(uint16_t)];
  text_vao = create_vao();
  text_vbo = create_vbo(max_text_bytes);
  glEnableVertexAttribArray(0);
  glVertexAttribIPointer(0, 4, GL_SHORT, 8, 0);
  glVertexAttribDivisor(0, 1);

  uint8_t* dst_pix = new uint8_t[32768];
  for (int i = 0; i < 32768; i++) dst_pix[i] = terminus[i] == '#' ? 0xFF : 0x00;
  font_tex = create_texture_u8(256, 128, dst_pix);
  delete [] dst_pix;

  text_ubo = create_ubo(sizeof(TextUniforms));
}

//-----------------------------------------------------------------------------

void TextPainter::render(Viewport view, double x, double y, float scale) {

  bind_shader(text_prog);

  text_uniforms.viewport = {
    (float)view.min.x,
    (float)view.min.y,
    (float)view.max.x,
    (float)view.max.y,
  };
  text_uniforms.origin = {x, y, scale, scale};
  text_uniforms.bg_col = bg_col;
  update_ubo(text_ubo, sizeof(text_uniforms), &text_uniforms);
  bind_ubo(text_prog, "TextUniforms", 0, text_ubo);

  bind_texture(text_prog, "font_tex", 0, font_tex);

  bind_vao(text_vao);

  int glyph_count = (inst_end - inst_begin) / 4;
  int bytes_per_glyph = 8;

  update_vbo(text_vbo, glyph_count * bytes_per_glyph, text_data + inst_begin);

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, glyph_count);

  text_x = 0;
  text_y = 0;
  inst_begin = inst_end = 0;
}

//-----------------------------------------------------------------------------

void TextPainter::push_char(int x, int y, int c, int pal) {
  text_data[inst_end++] = uint16_t(x);
  text_data[inst_end++] = uint16_t(y);
  text_data[inst_end++] = uint16_t(c);
  text_data[inst_end++] = uint16_t(pal);
  CHECK_P((inst_end * sizeof(uint16_t)) < max_text_bytes);
}

void TextPainter::add_char(const char c) {
  if (c == '\n') {
    text_x = 0;
    text_y += glyph_size_y;
  }
  else if (c < 10) {
    fg_pal = c;
  }
  else {
    push_char(text_x, text_y, c, fg_pal);
    text_x += glyph_size_x;
  }
}

void TextPainter::add_char(const char c, const char d) {
  add_char(c);
  add_char(d);
}

void TextPainter::add_text(const char* s) {
  for(; *s; s++) {
    add_char(*s);
  }
}

void TextPainter::add_text(const char* s, int len) {
  for (int i = 0; i < len; i++) {
    add_char(s[i]);
  }
}

void TextPainter::add_text_at(const char* s, int x, int y) {
  int cursor_x = x;
  int cursor_y = y;

  for(; *s; s++) {
    int c = *s;
    if (c == '\n') {
      cursor_x = x;
      cursor_y += glyph_size_y;
    }
    else if (c < 10) {
      fg_pal = c;
    }
    else {
      push_char(cursor_x, cursor_y, c, fg_pal);
      cursor_x += glyph_size_x;
    }
  }
}

void TextPainter::add_string(const std::string& s) {
  for (auto c : s) add_char(c);
}

void TextPainter::dprintf(const char* format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  int count = vsnprintf(buffer, 256, format, args);
  va_end(args);
  add_text(buffer, count);
}

void TextPainter::set_pal(int index, float r, float g, float b, float a) {
  text_uniforms.palette[index] = {r,g,b,a};
}

//-----------------------------------------------------------------------------
