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
};

uniform sampler2D font_tex;

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout(location = 0) in vec3 glyph_pos;
layout(location = 1) in int  glyph_idx;
layout(location = 2) in vec4 glyph_col;

out vec2  tc_glyph;
out vec4  fg_col;

void main() {
  float corner_x = float((gl_VertexID >> 0) & 1);
  float corner_y = float((gl_VertexID >> 1) & 1);

  float col     = float((glyph_idx >> 0) & 0x1F);
  float row     = float((glyph_idx >> 5) & 0x07);

  float glyph_tcx = (col * atlas_stride_x) + (corner_x * glyph_size_x);
  float glyph_tcy = (row * atlas_stride_y) + (corner_y * glyph_size_y);

  glyph_tcx = remap(glyph_tcx, 0.0, atlas_width,  0.0, 1.0);
  glyph_tcy = remap(glyph_tcy, 0.0, atlas_height, 0.0, 1.0);

  tc_glyph = vec2(glyph_tcx, glyph_tcy);
  fg_col = glyph_col;


  //----------

  float glyph_scale_x = glyph_pos.z;
  float glyph_scale_y = glyph_pos.z;

  float glyph_x = glyph_pos.x;
  float glyph_y = glyph_pos.y;
  float quad_x = glyph_x + (corner_x * glyph_size_x) * glyph_scale_x + origin.x;
  float quad_y = glyph_y + (corner_y * glyph_size_y) * glyph_scale_y + origin.y;

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

static uint32_t text_prog = 0;

//-----------------------------------------------------------------------------

void TextPainter::init() {
  if (!text_prog) {
    text_prog = create_shader("text_glsl", text_glsl);
  }

  set_pal(0, 0.4f, 0.4f, 0.4f, 1.0f); // grey
  set_pal(1, 0.8f, 0.8f, 0.8f, 1.0f); // white
  set_pal(2, 0.6f, 1.0f, 0.6f, 1.0f); // lo-z out = green
  set_pal(3, 1.0f, 0.6f, 0.6f, 1.0f); // hi-z out = red
  set_pal(4, 0.6f, 0.6f, 1.0f, 1.0f); // lo-z in = blue
  set_pal(5, 1.0f, 1.0f, 0.6f, 1.0f); // hi-z in = yellow
  set_pal(6, 1.0f, 0.6f, 1.0f, 1.0f); // error magenta
  set_pal(7, 0.4f, 0.4f, 0.4f, 1.0f); // grey

  text_data_u32 = new uint32_t[max_text_bytes / sizeof(text_data_u32[0])];
  text_data_f32 = reinterpret_cast<float*>(text_data_u32);

  text_vao = create_vao();
  text_vbo = create_vbo(max_text_bytes, nullptr);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glVertexAttribPointer (0, 3, GL_FLOAT,         GL_FALSE, bytes_per_glyph, 0);
  glVertexAttribIPointer(1, 1, GL_INT,                     bytes_per_glyph, (void*)12);
  glVertexAttribPointer (2, 4, GL_UNSIGNED_BYTE, GL_TRUE,  bytes_per_glyph, (void*)16);

  glVertexAttribDivisor(0, 1);
  glVertexAttribDivisor(1, 1);
  glVertexAttribDivisor(2, 1);

  uint8_t* dst_pix = new uint8_t[32768];
  for (int i = 0; i < 32768; i++) dst_pix[i] = terminus[i] == '#' ? 0xFF : 0x00;
  font_tex = create_texture_u8(256, 128, dst_pix, false);
  delete [] dst_pix;

  text_ubo = create_ubo();
}

//-----------------------------------------------------------------------------

void TextPainter::render(Viewport view, dvec2 screen_size, double x, double y) {
  update_vbo(text_vbo, glyph_count * bytes_per_glyph, text_data_u32);

  if (glyph_count == 0) return;

  bind_shader(text_prog);

  text_uniforms.viewport = {
    (float)view.screen_min(screen_size).x,
    (float)view.screen_min(screen_size).y,
    (float)view.screen_max(screen_size).x,
    (float)view.screen_max(screen_size).y,
  };
  text_uniforms.origin = { x, y, 1, 1 };
  text_uniforms.bg_col = bg_col;

  update_ubo(text_ubo, sizeof(text_uniforms), &text_uniforms);
  bind_ubo(text_prog, "TextUniforms", 0, text_ubo);

  bind_texture(text_prog, "font_tex", 0, font_tex);

  bind_vao(text_vao);

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, glyph_count);

  text_x = 0;
  text_y = 0;
  glyph_count = 0;
  buf_cursor = 0;
}

//-----------------------------------------------------------------------------

void TextPainter::push_char(double x, double y, double scale, int c, uint32_t color) {
  text_data_f32[buf_cursor++] = float(x);
  text_data_f32[buf_cursor++] = float(y);
  text_data_f32[buf_cursor++] = float(scale);
  text_data_u32[buf_cursor++] = c;
  text_data_u32[buf_cursor++] = color;
  glyph_count++;
  CHECK_P(glyph_count <= max_glyphs);
}

void TextPainter::add_char(const char c, double scale) {
  if (c == '\n') {
    text_x = 0;
    text_y += glyph_size_y;
  }
  else if (c < 10) {
    fg_pal = c;
  }
  else {
    push_char(text_x, text_y, scale, c, palette[fg_pal]);
    text_x += glyph_size_x * scale;
  }
}

void TextPainter::add_text(const char* s, double scale) {
  for(; *s; s++) {
    add_char(*s, scale);
  }
}

void TextPainter::add_text(const char* s, int len, double scale) {
  for (int i = 0; i < len; i++) {
    add_char(s[i], scale);
  }
}

void TextPainter::add_text_at(const char* s, uint32_t color, double x, double y, double scale) {
  double cursor_x = x;
  double cursor_y = y;

  for(; *s; s++) {
    int c = *s;
    if (c == '\n') {
      cursor_x = x;
      cursor_y += glyph_size_y * scale;
    }
    else {
      push_char(cursor_x, cursor_y, scale, c, color);
      cursor_x += glyph_size_x * scale;
    }
  }
}

void TextPainter::add_text_at(const char* s, double x, double y, double scale) {
  double cursor_x = x;
  double cursor_y = y;

  for(; *s; s++) {
    int c = *s;
    if (c == '\n') {
      cursor_x = x;
      cursor_y += glyph_size_y * scale;
    }
    else if (c < 10) {
      fg_pal = c;
    }
    else {
      push_char(cursor_x, cursor_y, scale, c, palette[fg_pal]);
      cursor_x += glyph_size_x * scale;
    }
  }
}

void TextPainter::add_text_at_simple(const char* s, double x, double y) {
  double cursor_x = x;
  double cursor_y = y;

  uint32_t color = palette[fg_pal];

  for(; *s; s++) {
    int c = *s;
    push_char(cursor_x, cursor_y, 1.0, c, color);
    cursor_x += glyph_size_x;
  }
}

void TextPainter::add_string(const std::string& s, double scale) {
  for (auto c : s) add_char(c, scale);
}

void TextPainter::dprintf(const char* format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  int count = vsnprintf(buffer, 256, format, args);
  va_end(args);
  add_text(buffer, count, 1.0);
}

void TextPainter::set_pal(int index, double r, double g, double b, double a) {
  uint8_t r2 = uint8_t(r * 255.0);
  uint8_t g2 = uint8_t(g * 255.0);
  uint8_t b2 = uint8_t(b * 255.0);
  uint8_t a2 = uint8_t(a * 255.0);

  palette[index] = uint32_t((r2 << 0) | (g2 << 8) | (b2 << 16) | (a2 << 24));
}

//-----------------------------------------------------------------------------
