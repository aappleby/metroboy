#include "AppLib/DumpPainter.h"

#include "AppLib/GLBase.h"
#include "SDL/include/SDL.h"
#include "glad/glad.h"

#include <vector>
#include <stdio.h>

extern const char* terminus;
extern const char* terminus_hex;

//-----------------------------------------------------------------------------

const char* dump_glsl = R"(
uniform vec4  fg_color;
uniform vec4  bg_color;
uniform vec4  highlight_color;

uniform vec4  screen_to_win;
uniform vec4  corner_to_clip;

uniform uvec4 highlight;

uniform sampler2D  atlas_tex;
uniform usampler2D dump_tab;
uniform usampler2D ruler_x;
uniform usampler2D ruler_y;

uniform uint shifts[16];
uniform uint masks[16];

const float inv_atlas_w = 1.0 / 256.0;
const float inv_atlas_h = 1.0 / 128.0;

uniform vec4 blah;

#ifdef _VERTEX_

void main() {
  float unit_corner_x = float((gl_VertexID >> 0) & 1);
  float unit_corner_y = float((gl_VertexID >> 1) & 1);

  float clip_corner_x = unit_corner_x * corner_to_clip.x + corner_to_clip.z;
  float clip_corner_y = unit_corner_y * corner_to_clip.y + corner_to_clip.w;

  gl_Position = vec4(clip_corner_x, clip_corner_y, 0.0, 1.0);
}

#else

out vec4 fs_out;

void main() {

  // Convert from fragment coordinates to window coordinates and split into int/fract parts.

  float win_x = gl_FragCoord.x * screen_to_win.x + screen_to_win.z;
  float win_y = gl_FragCoord.y * screen_to_win.y + screen_to_win.w;

  float win_xi = floor(win_x);
  float win_yi = floor(win_y);
  float win_xf = fract(win_x);
  float win_yf = fract(win_y);

  // Use the integer window coordinates to look up tile/cell coordinates from the rulers.
  uvec4 rule_x = texelFetch(ruler_x, ivec2(win_xi, 0), 0);
  uvec4 rule_y = texelFetch(ruler_y, ivec2(win_yi, 0), 0);

  uint tile_xi = rule_x.r;
  uint tile_yi = rule_y.r;

  uint cell_xi = rule_x.g;
  uint cell_yi = rule_y.g;

  uint cell_xf = rule_x.b;
  uint cell_yf = rule_y.b;

  // Look up glyph based on tile coordinates and extract the bits we care about for the current cell.
  uint glyph = texelFetch(dump_tab, ivec2(tile_xi, tile_yi), 0).r;
  uint shift = shifts[cell_xi];
  uint mask  = masks [cell_xi];
  if (mask == 0u) {
    fs_out = bg_color;
    return;
  }
  glyph = (glyph & mask) >> shift;

  // Convert glyph index to atlas cell coordinates
  uint atlas_tex_xi = (glyph & 0x1Fu) << 3; // 8 texels per glyph column
  uint atlas_tex_yi = (glyph & 0xE0u) >> 1; // 16 texels per glyph row

  // Combine atlas cell, cell offset, and subpixel offset into atlas coordinates.
  float atlas_tex_x = float(atlas_tex_xi + cell_xf) + win_xf;
  float atlas_tex_y = float(atlas_tex_yi + cell_yf) + win_yf;

  // Read atlas and emit color.
  float pix = texelFetch(atlas_tex, ivec2(atlas_tex_x, atlas_tex_y), 0).r;

  if (tile_xi == highlight.x && tile_yi == highlight.y) {
    fs_out = mix(bg_color, highlight_color, pix);
  }
  else {
    fs_out = mix(bg_color, fg_color, pix);
  }
}

#endif
)";

//-----------------------------------------------------------------------------

void DumpPainter::init_hex() {
  tile_w = 14;
  tile_h = 12;
  cell_w = 6;
  cell_h = 12;

  masks[0] = 0xF0;
  masks[1] = 0x0F;
  shifts[0] = 0x04;
  shifts[1] = 0x00;

  dump_prog = create_shader("dump_glsl", dump_glsl);
  dump_vao  = create_vao();
  dump_tab  = create_table_u8(256, 256, nullptr);

  {
    std::vector<uint8_t> hexa_pix(2048);
    for (int i = 0; i < 2048; i++) hexa_pix[i] = terminus_hex[i] == '#' ? 0xFF : 0x00;
    glyph_tex = create_texture_u8(128, 16, hexa_pix.data(), false);
  }

  ruler_x_tab = create_table_u32(4096, 1, nullptr);
  ruler_y_tab = create_table_u32(4096, 1, nullptr);

  {

    std::vector<uint32_t> ruler_x(4096);
    for (int i = 0; i < 4096; i++) {
      int tile_xi = i / tile_w;
      int tile_xf = i % tile_w;
      int cell_xi = tile_xf / cell_w;
      int cell_xf = tile_xf % cell_w;
      ruler_x[i] = (tile_xi << 0) | (cell_xi << 8) | (cell_xf << 16);
    }
    update_table_u32(ruler_x_tab, 4096, 1, ruler_x.data());
  }

  {
    std::vector<uint32_t> ruler_y(4096);
    for (int i = 0; i < 4096; i++) {
      int tile_yi = i / tile_h;
      int tile_yf = i % tile_h;
      int cell_yi = tile_yf / cell_h;
      int cell_yf = tile_yf % cell_h;
      ruler_y[i] = (tile_yi << 0) | (cell_yi << 8) | (cell_yf << 16);
    }
    update_table_u32(ruler_y_tab, 4096, 1, ruler_y.data());
  }
}

//-----------------------------------------------------------------------------

void DumpPainter::init_ascii() {
  tile_w = 6;
  tile_h = 12;
  cell_w = 6;
  cell_h = 12;

  masks[0] = 0xFF;
  shifts[0] = 0x00;

  dump_prog = create_shader("dump_glsl", dump_glsl);
  dump_vao = create_vao();
  dump_tab = create_table_u8(256, 256, nullptr);

  {
    std::vector<uint8_t> font_pix(65536);
    for (int i = 0; i < 32768; i++) font_pix[i] = terminus[i] == '#' ? 0xFF : 0x00;
    glyph_tex = create_texture_u8(256, 128, font_pix.data(), false);
  }

  ruler_x_tab = create_table_u32(4096, 1, nullptr);
  ruler_y_tab = create_table_u32(4096, 1, nullptr);

  {

    std::vector<uint32_t> ruler_x(4096);
    for (int i = 0; i < 4096; i++) {
      int tile_xi = i / tile_w;
      int tile_xf = i % tile_w;
      int cell_xi = tile_xf / cell_w;
      int cell_xf = tile_xf % cell_w;
      ruler_x[i] = (tile_xi << 0) | (cell_xi << 8) | (cell_xf << 16);
    }
    update_table_u32(ruler_x_tab, 4096, 1, ruler_x.data());
  }

  {
    std::vector<uint32_t> ruler_y(4096);
    for (int i = 0; i < 4096; i++) {
      int tile_yi = i / tile_h;
      int tile_yf = i % tile_h;
      int cell_yi = tile_yf / cell_h;
      int cell_yf = tile_yf % cell_h;
      ruler_y[i] = (tile_yi << 0) | (cell_yi << 8) | (cell_yf << 16);
    }
    update_table_u32(ruler_y_tab, 4096, 1, ruler_y.data());
  }

  bind_shader(dump_prog);
  glUniform1uiv(glGetUniformLocation(dump_prog, "shifts"), 16, shifts);
  glUniform1uiv(glGetUniformLocation(dump_prog, "masks"), 16, masks);
}

//-----------------------------------------------------------------------------

void DumpPainter::dump(Viewport view, dvec2 screen_size, double world_x, double world_y, double scale_x, double scale_y, int text_w, int text_h, vec4 color, const uint8_t* dump) {
  update_table_u8(dump_tab, text_w, text_h, dump);

  bind_shader(dump_prog);
  glUniform1uiv(glGetUniformLocation(dump_prog, "shifts"), 16, shifts);
  glUniform1uiv(glGetUniformLocation(dump_prog, "masks"), 16, masks);


  double view_w = view.screen_max(screen_size).x - view.screen_min(screen_size).x;
  double view_h = view.screen_max(screen_size).y - view.screen_min(screen_size).y;

  double view_dx = (world_x - view.screen_min(screen_size).x);
  double view_dy = (world_y - view.screen_min(screen_size).y);

  {
    double ax = +view_w / (screen_size.x * scale_x);
    double ay = +view_h / (screen_size.y * scale_y);
    double bx = -view_dx / scale_x;
    double by = -view_dy / scale_y;

    ay = -ay;
    by += view_h / scale_y;

    glUniform4f(glGetUniformLocation(dump_prog, "screen_to_win"), float(ax), float(ay), float(bx), float(by));
  }

  {
    double ax = 2.0 * (text_w * tile_w * scale_x) / view_w;
    double ay = 2.0 * (text_h * tile_h * scale_y) / view_h;
    double bx = 2.0 * (world_x - view.screen_min(screen_size).x) / view_w - 1.0;
    double by = 2.0 * (world_y - view.screen_min(screen_size).y) / view_h - 1.0;

    ay = -ay;
    by = -by;

    glUniform4f(glGetUniformLocation(dump_prog, "corner_to_clip"), float(ax), float(ay), float(bx), float(by));
  }

  glUniform4ui(glGetUniformLocation(dump_prog, "highlight"), highlight_x, highlight_y, 0, 0);
  glUniform4f(glGetUniformLocation(dump_prog, "fg_color"), 0.8f, 0.8f, 0.8f, 1.0f);
  glUniform4f(glGetUniformLocation(dump_prog, "bg_color"), color.r, color.g, color.b, color.a);
  glUniform4f(glGetUniformLocation(dump_prog, "highlight_color"), 1.0f, 1.0f, 0.0f, 1.0f);

  bind_texture(dump_prog, "dump_tab", 0, dump_tab);
  bind_texture(dump_prog, "atlas_tex", 1, glyph_tex);
  bind_table(dump_prog, "ruler_x", 2, ruler_x_tab);
  bind_table(dump_prog, "ruler_y", 3, ruler_y_tab);

  bind_vao(dump_vao);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//-----------------------------------------------------------------------------
