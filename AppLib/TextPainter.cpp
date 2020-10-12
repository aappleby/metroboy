#include "AppLib/TextPainter.h"

#include "AppLib/GLBase.h"

#include <stdio.h>
#include "SDL/include/SDL.h"
#include "glad/glad.h"

extern const char* terminus;

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
  vec4  text_pos;
  vec4  bg_palette;
  vec4  palette[16];
};

uniform sampler2D font_tex;

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout(location = 0) in uint glyph2;

out vec2  tc_glyph;
out vec4  fg_color;
out vec4  bg_color;

void main() {
  float corner_x = float((gl_VertexID >> 0) & 1);
  float corner_y = float((gl_VertexID >> 1) & 1);

  uint glyph_packed = glyph2;
  float glyph_x = float((glyph_packed >>  0) & 0xFFu);
  float glyph_y = float((glyph_packed >>  8) & 0xFFu);
  int col       = int  ((glyph_packed >> 16) & 0x1Fu);
  int row       = int  ((glyph_packed >> 21) & 0x07u);
  int fg_style  = int  ((glyph_packed >> 24) & 0xFFu);

  float glyph_tcx = (float(col) * atlas_stride_x) + (corner_x * glyph_size_x);
  float glyph_tcy = (float(row) * atlas_stride_y) + (corner_y * glyph_size_y);

  glyph_tcx = remap(glyph_tcx, 0.0, atlas_width,  0.0, 1.0);
  glyph_tcy = remap(glyph_tcy, 0.0, atlas_height, 0.0, 1.0);

  tc_glyph = vec2(glyph_tcx, glyph_tcy);
  fg_color = palette[fg_style];
  bg_color = bg_palette;

  //----------

  float quad_x = (glyph_x + corner_x) * glyph_size_x * text_pos.z + text_pos.x;
  float quad_y = (glyph_y + corner_y) * glyph_size_y * text_pos.w + text_pos.y;

  gl_Position = vec4(remap(quad_x, viewport.x, viewport.z, -1.0,  1.0),
                     remap(quad_y, viewport.y, viewport.w,  1.0, -1.0),
                     0.0,
                     1.0);
}

#else

in vec2 tc_glyph;
in vec4 fg_color;
in vec4 bg_color;

out vec4 fs_out;

void main() {
  float p = texture(font_tex, tc_glyph).r;

  //p = ((p - 0.5) * 10.0) + 0.5;
  //p = clamp(p, 0.0, 1.0);

  //float p = texelFetch(font_tex, ivec2(tc_glyph.x, tc_glyph.y), 0).r;
  fs_out = mix(bg_color, fg_color, p);
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

  inst_data = new uint32_t[65536];

  dummy_vao = create_vao();

  inst_vbo = create_vbo(65536 * 4);
  glEnableVertexAttribArray(0);
  glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 4, 0);
  glVertexAttribDivisor(0, 1);

  uint8_t* dst_pix = new uint8_t[32768];
  for (int i = 0; i < 32768; i++) dst_pix[i] = terminus[i] == '#' ? 0xFF : 0x00;
  font_tex = create_texture_u8(256, 128, dst_pix);
  delete [] dst_pix;

  text_ubo = create_ubo(sizeof(TextUniforms));
  text_uniforms.bg_palette = vec4(0.0, 0.0, 0.0, 0.5);
}

//-----------------------------------------------------------------------------

void TextPainter::render(Viewport view, double x, double y, float scale) {

  bind_shader(text_prog);

  text_uniforms.text_pos = {x, y, scale, scale};
  text_uniforms.viewport = {
    (float)view.min.x,
    (float)view.min.y,
    (float)view.max.x,
    (float)view.max.y,
  };
  update_ubo(text_ubo, sizeof(text_uniforms), &text_uniforms);
  bind_ubo(text_prog, "TextUniforms", 0, text_ubo);

  bind_texture(text_prog, "font_tex", 0, font_tex);

  bind_vao(dummy_vao);

  update_vbo(inst_vbo, (inst_end - inst_begin)*4, inst_data + inst_begin);

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, inst_end - inst_begin);

  text_x = 0;
  text_y = 0;
  inst_begin = inst_end = 0;
}

//-----------------------------------------------------------------------------

void TextPainter::add_char(const char c) {
  if (c < 10) {
    fg_pal = c;
  }
  else if (c == '\n') {
    text_x = 0;
    text_y++;
  }
  else {
    uint32_t packed = (text_x << 0) | (text_y << 8) | (c << 16) | (fg_pal << 24);
    inst_data[inst_end] = packed;

    inst_end++;
    text_x++;
  }
}

void TextPainter::add_char(const char c, const char d) {
  add_char(c);
  add_char(d);
}

void TextPainter::add_text(const char* text_painter) {
  for(; *text_painter; text_painter++) {
    add_char(*text_painter);
  }
}

void TextPainter::add_text(const char* text_painter, int len) {
  for (int i = 0; i < len; i++) {
    add_char(text_painter[i]);
  }
}

void TextPainter::add_string(const std::string& text_painter) {
  for (auto c : text_painter) add_char(c);
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
