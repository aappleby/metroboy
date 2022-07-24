#include "AppLib/GBBlitter.h"
#include "AppLib/GLBase.h"
#include "glm/glm/glm.hpp"
#include "glad/glad.h"

//-----------------------------------------------------------------------------

const char* gb_blit_trace_glsl = R"(
uniform sampler2D tex;
uniform vec4 viewport;
uniform vec4 blit_dst_rect;

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout(location = 0) in  vec2 vpos;
out vec2 ftex;

void main() {
  ftex.x = vpos.x;
  ftex.y = vpos.y;

  float dst_x = remap(vpos.x, 0.0, 1.0, blit_dst_rect.x, blit_dst_rect.z);
  float dst_y = remap(vpos.y, 0.0, 1.0, blit_dst_rect.y, blit_dst_rect.w);

  gl_Position.x = remap(dst_x, viewport.x, viewport.z, -1.0,  1.0);
  gl_Position.y = remap(dst_y, viewport.y, viewport.w,  1.0, -1.0);
  gl_Position.z = 0.0;
  gl_Position.w = 1.0;
}

#else

in  vec2 ftex;
out vec4 frag;

void main() {
  frag = texture(tex, ftex);
  frag.a = 1.0;
}

#endif

)";

//-----------------------------------------------------------------------------

const char* gb_blit_screen_glsl = R"(
uniform sampler2D tex;
uniform vec4 viewport;
uniform vec4 blit_dst_rect;
uniform vec4 palette[16];

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout(location = 0) in  vec2 vpos;
out vec2 ftex;

void main() {
  ftex.x = vpos.x;
  ftex.y = vpos.y;

  float dst_x = remap(vpos.x, 0.0, 1.0, blit_dst_rect.x, blit_dst_rect.z);
  float dst_y = remap(vpos.y, 0.0, 1.0, blit_dst_rect.y, blit_dst_rect.w);

  gl_Position.x = remap(dst_x, viewport.x, viewport.z, -1.0,  1.0);
  gl_Position.y = remap(dst_y, viewport.y, viewport.w,  1.0, -1.0);
  gl_Position.z = 0.0;
  gl_Position.w = 1.0;
}

#else

in  vec2 ftex;
out vec4 frag;

void main() {
  int pal_index = int(round(texture(tex, ftex).r * 255.0));
  frag = palette[pal_index];
}

#endif

)";

//-----------------------------------------------------------------------------

const char* gb_blit_diff_glsl = R"(
uniform sampler2D test_tex;
uniform sampler2D gold_tex;
uniform vec4 viewport;
uniform vec4 blit_dst_rect;
uniform vec4 palette[5];

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout(location = 0) in  vec2 vpos;
out vec2 ftex;

void main() {
  ftex.x = vpos.x;
  ftex.y = vpos.y;

  float dst_x = remap(vpos.x, 0.0, 1.0, blit_dst_rect.x, blit_dst_rect.z);
  float dst_y = remap(vpos.y, 0.0, 1.0, blit_dst_rect.y, blit_dst_rect.w);

  gl_Position.x = remap(dst_x, viewport.x, viewport.z, -1.0,  1.0);
  gl_Position.y = remap(dst_y, viewport.y, viewport.w,  1.0, -1.0);
  gl_Position.z = 0.0;
  gl_Position.w = 1.0;
}

#else

in  vec2 ftex;
out vec4 frag;

void main() {
  int pal_index1 = int(round(texture(test_tex, ftex).r * 255.0));
  int pal_index2 = int(round(texture(gold_tex, ftex).r * 255.0));

  frag = palette[pal_index1];
  if (pal_index1 != pal_index2) {
    frag.r += 0.5;
    frag.g += 0.5;
  }
}

#endif

)";

//-----------------------------------------------------------------------------

struct GBBlitMapUniforms {
  vec4 viewport;

  vec4 quad_pos;
  vec4 quad_tex;
  vec4 palette[5];
  int  which_map;
  int  alt_map;
  int  pad1;
  int  pad2;
};

static const char* gb_blit_map_glsl = R"(

layout(std140) uniform BlitMapUniforms {
  vec4 viewport;

  vec4 quad_pos;
  vec4 quad_tex;
  vec4 palette[5];
  int  which_map;
  int  alt_map;
};

layout(std140) uniform vramBuffer {
  ivec4 tiles[384]; // 6k of tile data
  ivec4 maps[128];   // 2k of map data
};

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

int decode_tile2(int tile_index, int tile_x, int tile_y) {
  int  flat_index    = tile_index * 16 + tile_y * 2;
  ivec4 packed_ivec4 = tiles[flat_index / 16];
  int   packed_tile  = packed_ivec4[(flat_index >> 2) & 3];
  //vec4  temp         = unpackUnorm4x8(uint(packed_tile)) * 255.0;
  int temp_x = (packed_tile >>  0) & 0xFF;
  int temp_y = (packed_tile >>  8) & 0xFF;
  int temp_z = (packed_tile >> 16) & 0xFF;
  int temp_w = (packed_tile >> 24) & 0xFF;

  int pix_a = ((tile_y & 1) == 0) ? int(temp_x) : int(temp_z);
  int pix_b = ((tile_y & 1) == 0) ? int(temp_y) : int(temp_w);

  pix_a = (pix_a >> (7 - tile_x)) & 1;
  pix_b = (pix_b >> (7 - tile_x)) & 1;

  return pix_a + (2 * pix_b);
}

//----------------------------------------

#ifdef _VERTEX_

layout(location = 0) in  vec2 vpos;
out vec2 ftex;

void main() {

  float x = remap(vpos.x, 0.0, 1.0, quad_pos.x, quad_pos.z);
  float y = remap(vpos.y, 0.0, 1.0, quad_pos.y, quad_pos.w);

  gl_Position = vec4(remap(x, viewport.x, viewport.z, -1.0,  1.0),
                     remap(y, viewport.y, viewport.w,  1.0, -1.0),
                     1.0,
                     1.0);

  ftex.x = remap(vpos.x, 0.0, 1.0, quad_tex.x, quad_tex.z);
  ftex.y = remap(vpos.y, 0.0, 1.0, quad_tex.y, quad_tex.w);
}

#endif

//----------------------------------------

#ifdef _FRAGMENT_

in  vec2 ftex;
out vec4 frag;

void main() {
  int pix_x = int(ftex.x) & 0xFF;
  int pix_y = int(ftex.y) & 0xFF;

  int map_x = pix_x >> 3;
  int map_y = pix_y >> 3;

  int tile_x = pix_x & 7;
  int tile_y = pix_y & 7;

  int flat_map_index = map_y * 32 + map_x;
  if (bool(which_map)) flat_map_index += 1024;

  ivec4 packed_map_ivec4 = maps[flat_map_index >> 4];
  int   packed_map_int   = packed_map_ivec4[(flat_map_index >> 2) & 3];
  //vec4  unpacked_map     = unpackUnorm4x8(uint(packed_map_int));

  ivec4 unpacked_map;
  unpacked_map.x = (packed_map_int >>  0) & 0xFF;
  unpacked_map.y = (packed_map_int >>  8) & 0xFF;
  unpacked_map.z = (packed_map_int >> 16) & 0xFF;
  unpacked_map.w = (packed_map_int >> 24) & 0xFF;

  int  tile_index = unpacked_map[flat_map_index & 3];

  if (bool(alt_map) && (tile_index < 128)) tile_index += 256;

  frag = palette[decode_tile2(tile_index, tile_x, tile_y)];
}

#endif

)";

//-----------------------------------------------------------------------------

struct GBBlitTileUniforms {
  vec4 viewport;
  vec4 quad_pos;
  vec4 quad_tex;
  vec4 palette[5];
};

static const char* gb_blit_tile_glsl = R"(

layout(std140) uniform BlitTileUniforms {
  vec4 viewport;

  vec4 quad_pos;
  vec4 quad_tex;
  vec4 palette[5];
};

layout(std140) uniform vramBuffer {
  ivec4 tiles[384]; // 6k of tile data
  ivec4 maps[128];   // 2k of map data
};

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

int decode_tile2(int tile_index, int tile_x, int tile_y) {
  int  flat_index    = tile_index * 16 + tile_y * 2;
  ivec4 packed_ivec4 = tiles[flat_index / 16];
  int   packed_tile  = packed_ivec4[(flat_index >> 2) & 3];
  //vec4  temp         = unpackUnorm4x8(uint(packed_tile)) * 255.0;
  int temp_x = (packed_tile >>  0) & 0xFF;
  int temp_y = (packed_tile >>  8) & 0xFF;
  int temp_z = (packed_tile >> 16) & 0xFF;
  int temp_w = (packed_tile >> 24) & 0xFF;

  int pix_a = ((tile_y & 1) == 0) ? int(temp_x) : int(temp_z);
  int pix_b = ((tile_y & 1) == 0) ? int(temp_y) : int(temp_w);

  pix_a = (pix_a >> (7 - tile_x)) & 1;
  pix_b = (pix_b >> (7 - tile_x)) & 1;

  return pix_a + (2 * pix_b);
}

//----------------------------------------

#ifdef _VERTEX_

layout(location = 0) in  vec2 vpos;
out vec2 ftex;

void main() {

  float x = remap(vpos.x, 0.0, 1.0, quad_pos.x, quad_pos.z);
  float y = remap(vpos.y, 0.0, 1.0, quad_pos.y, quad_pos.w);

  gl_Position = vec4(remap(x, viewport.x, viewport.z, -1.0,  1.0),
                     remap(y, viewport.y, viewport.w,  1.0, -1.0),
                     1.0,
                     1.0);

  ftex.x = remap(vpos.x, 0.0, 1.0, quad_tex.x, quad_tex.z);
  ftex.y = remap(vpos.y, 0.0, 1.0, quad_tex.y, quad_tex.w);
}

#endif

//----------------------------------------

#ifdef _FRAGMENT_

in  vec2 ftex;
out vec4 frag;

void main() {
  int pix_x = int(ftex.x) & 0xFF;
  int pix_y = int(ftex.y) & 0xFF;

  int map_x = pix_x >> 3;
  int map_y = pix_y >> 3;

  int tile_x = pix_x & 7;
  int tile_y = pix_y & 7;

  int tile_index = map_y * 16 + map_x;
  frag = palette[decode_tile2(tile_index, tile_x, tile_y)];
}

#endif

)";

//-----------------------------------------------------------------------------

void GBBlitter::init() {

  float unit_quad[] = { 0, 0, 1, 0, 0, 1, 1, 1 };
  quad_vao = create_vao();
  quad_vbo = create_vbo(sizeof(unit_quad), unit_quad);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  blit_map_ubo = create_ubo();
  blit_map_prog = create_shader("gb_blit_map_glsl", gb_blit_map_glsl);

  blit_tile_ubo = create_ubo();
  blit_tile_prog = create_shader("gb_blit_tile_glsl", gb_blit_tile_glsl);

  vram_ubo = create_ubo();

  blit_screen_prog = create_shader("gb_blit_screen_glsl", gb_blit_screen_glsl);
  blit_screen_tex = create_texture_u8(160, 144, nullptr, false);

  blit_diff_prog = create_shader("gb_blit_diff_glsl", gb_blit_diff_glsl);
  blit_diff_tex = create_texture_u8(160, 144, nullptr, false);

  blit_trace_prog = create_shader("gb_blit_trace_glsl", gb_blit_trace_glsl);
  blit_trace_tex = create_texture_u32(456, 154, nullptr, false);
}

//-----------------------------------------------------------------------------

static vec4 default_pal[5] = {
  {0.48, 0.48, 0.48, 1.0},
  {0.36, 0.36, 0.36, 1.0},
  {0.24, 0.24, 0.24, 1.0},
  {0.12, 0.12, 0.12, 1.0},
  {0.80, 0.80, 0.00, 1.0},
};

void GBBlitter::blit_tiles(Viewport view, dvec2 screen_size, int screen_x, int screen_y, int scale, const uint8_t* vram) {
  (void)scale;
  update_ubo(vram_ubo, 8192, vram);

  {
    GBBlitTileUniforms uniforms;
    uniforms.viewport = {
      (float)view.screen_min(screen_size).x,
      (float)view.screen_min(screen_size).y,
      (float)view.screen_max(screen_size).x,
      (float)view.screen_max(screen_size).y,
    };
    uniforms.quad_pos = {};
    uniforms.quad_tex = {};
    uniforms.palette[0] = default_pal[0];
    uniforms.palette[1] = default_pal[1];
    uniforms.palette[2] = default_pal[2];
    uniforms.palette[3] = default_pal[3];
    uniforms.palette[4] = default_pal[4];

    bind_shader(blit_tile_prog);
    bind_vao(quad_vao);
    bind_ubo(blit_tile_prog, "BlitTileUniforms", 0, blit_tile_ubo);
    bind_ubo(blit_tile_prog, "vramBuffer", 1, vram_ubo);

    float x = float(screen_x);
    float y = float(screen_y);

    uniforms.quad_pos = {x, y, x + 256, y + 128};
    uniforms.quad_tex = {0, 0, 128, 64};
    update_ubo(blit_tile_ubo, sizeof(uniforms), &uniforms);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    uniforms.quad_pos = {x, y + 128, x + 256, y + 256};
    uniforms.quad_tex = {0, 64, 128, 128};
    update_ubo(blit_tile_ubo, sizeof(uniforms), &uniforms);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    uniforms.quad_pos = {x, y + 256, x + 256, y + 384};
    uniforms.quad_tex = {0, 128, 128, 192};
    update_ubo(blit_tile_ubo, sizeof(uniforms), &uniforms);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

//-----------------------------------------------------------------------------

void GBBlitter::blit_map(Viewport view, dvec2 screen_size, int screen_x, int screen_y, int scale, const uint8_t* vram, int which_map, int alt_map) {
  update_ubo(vram_ubo, 8192, vram);

  {
    GBBlitMapUniforms uniforms;
    uniforms.viewport = {
      (float)view.screen_min(screen_size).x,
      (float)view.screen_min(screen_size).y,
      (float)view.screen_max(screen_size).x,
      (float)view.screen_max(screen_size).y,
    };
    uniforms.quad_pos = {screen_x, screen_y, screen_x + 256 * scale, screen_y + 256 * scale};
    uniforms.quad_tex = {0, 0, 256, 256};

    uniforms.palette[0] = default_pal[0];
    uniforms.palette[1] = default_pal[1];
    uniforms.palette[2] = default_pal[2];
    uniforms.palette[3] = default_pal[3];
    uniforms.palette[4] = default_pal[4];

    uniforms.which_map = which_map;
    uniforms.alt_map = !alt_map;
    update_ubo(blit_map_ubo, sizeof(uniforms), &uniforms);

    bind_shader(blit_map_prog);
    bind_vao(quad_vao);
    bind_ubo(blit_map_prog, "BlitMapUniforms", 0, blit_map_ubo);
    bind_ubo(blit_map_prog, "vramBuffer", 1, vram_ubo);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

//-----------------------------------------------------------------------------

void GBBlitter::blit_screen(Viewport view, dvec2 screen_size, int screen_x, int screen_y, int scale, const uint8_t* framebuffer) {
  update_texture_u8(blit_screen_tex, 160, 144, framebuffer);

  bind_shader(blit_screen_prog);
  bind_vao(quad_vao);
  bind_texture(blit_screen_prog, "tex", 0, blit_screen_tex);

  glUniform4f(glGetUniformLocation(blit_screen_prog, "viewport"),
      (float)view.screen_min(screen_size).x,
      (float)view.screen_min(screen_size).y,
      (float)view.screen_max(screen_size).x,
      (float)view.screen_max(screen_size).y);
  glUniform4fv(glGetUniformLocation(blit_screen_prog, "palette"), 16, (float*)default_pal);
  glUniform4f(glGetUniformLocation(blit_screen_prog, "blit_dst_rect"),
              float(screen_x), float(screen_y), float(screen_x + 160 * scale), float(screen_y + 144 * scale));

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//-----------------------------------------------------------------------------

void GBBlitter::blit_diff(Viewport view, dvec2 screen_size, int screen_x, int screen_y, int scale, const uint8_t* test_tex, const uint8_t* gold_tex) {
  update_texture_u8(blit_screen_tex, 160, 144, test_tex);
  update_texture_u8(blit_diff_tex,   160, 144, gold_tex);

  bind_shader(blit_diff_prog);
  bind_vao(quad_vao);
  bind_texture(blit_diff_prog, "test_tex", 0, blit_screen_tex);
  bind_texture(blit_diff_prog, "gold_tex", 1, blit_diff_tex);

  glUniform4f(glGetUniformLocation(blit_diff_prog, "viewport"),
      (float)view.screen_min(screen_size).x,
      (float)view.screen_min(screen_size).y,
      (float)view.screen_max(screen_size).x,
      (float)view.screen_max(screen_size).y);
  glUniform4fv(glGetUniformLocation(blit_diff_prog, "palette"), 16, (float*)default_pal);
  glUniform4f(glGetUniformLocation(blit_diff_prog, "blit_dst_rect"),
              float(screen_x), float(screen_y), float(screen_x + 160 * scale), float(screen_y + 144 * scale));

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//-----------------------------------------------------------------------------

void GBBlitter::blit_trace(Viewport view, dvec2 screen_size, int screen_x, int screen_y, const uint32_t* trace) {
  update_texture_u32(blit_trace_tex, 456, 154, trace);

  bind_shader(blit_trace_prog);
  bind_vao(quad_vao);
  bind_texture(blit_trace_prog, "tex", 0, blit_trace_tex);

  glUniform4f(glGetUniformLocation(blit_trace_prog, "viewport"),
      (float)view.screen_min(screen_size).x,
      (float)view.screen_min(screen_size).y,
      (float)view.screen_max(screen_size).x,
      (float)view.screen_max(screen_size).y);
  glUniform4f(glGetUniformLocation(blit_trace_prog, "blit_dst_rect"),
              float(screen_x), float(screen_y), float(screen_x + 456), float(screen_y + 154));

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//-----------------------------------------------------------------------------