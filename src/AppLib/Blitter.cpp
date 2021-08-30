#include "AppLib/Blitter.h"
#include "AppLib/GLBase.h"
#include "glm/glm/glm.hpp"
#include "glad/glad.h"

//-----------------------------------------------------------------------------

struct BlitUniforms {
  vec4 viewport;

  vec4 blit_src_rect;
  vec4 blit_dst_rect;
  vec4 blit_tex_size;
  vec4 blit_col;
  int  solid;
  int  mono;
};

//-----------------------------------------------------------------------------

const char* blit_glsl = R"(

layout(std140) uniform BlitUniforms
{
  vec4 viewport;

  vec4 blit_src_rect;
  vec4 blit_dst_rect;
  vec4 blit_tex_size;
  vec4 blit_col;
  int  solid;
  int  mono;
};

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout(location = 0) in  vec2 vpos;
out vec2 ftex;

void main() {

  float src_x = remap(vpos.x, 0.0, 1.0, blit_src_rect.x, blit_src_rect.z);
  float src_y = remap(vpos.y, 0.0, 1.0, blit_src_rect.y, blit_src_rect.w);

  ftex.x = remap(src_x, blit_tex_size.x, blit_tex_size.z, 0.0, 1.0);
  ftex.y = remap(src_y, blit_tex_size.y, blit_tex_size.w, 0.0, 1.0);

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

uniform sampler2D tex;

void main() {
  if (bool(solid)) {
    frag = blit_col;
  }
  else if (bool(mono)) {
    frag = vec4(texture(tex, ftex).rrr, 1.0) * blit_col;
  }
  else {
    frag = texture(tex, ftex) * blit_col;
  }
}

#endif

)";

static uint32_t blit_prog = 0;

//-----------------------------------------------------------------------------

void Blitter::init() {

  float unit_quad[] = {
    0, 0,
    1, 0,
    0, 1,
    1, 1,
  };

  blit_ubo = create_ubo();
  blit_vao = create_vao();
  blit_vbo = create_vbo(sizeof(unit_quad), unit_quad);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  if (!blit_prog) {
    blit_prog = create_shader("blit_glsl", blit_glsl);
  }
}

//-----------------------------------------------------------------------------

void Blitter::blit(Viewport view, dvec2 screen_size,
                   uint32_t tex, int tw, int th,
                   int sx, int sy, int sw, int sh,
                   int dx, int dy, int dw, int dh) {
  BlitUniforms blit_uniforms;

  blit_uniforms.viewport = {
    (float)view.screen_min(screen_size).x,
    (float)view.screen_min(screen_size).y,
    (float)view.screen_max(screen_size).x,
    (float)view.screen_max(screen_size).y,
  };

  blit_uniforms.blit_src_rect = {sx, sy, sx+sw, sy+sh};
  blit_uniforms.blit_dst_rect = {dx, dy, dx+dw, dy+dh};
  blit_uniforms.blit_tex_size = {0, 0, tw, th};
  blit_uniforms.blit_col      = {1,1,1,1};

  blit_uniforms.solid = 0;
  blit_uniforms.mono = 0;

  update_ubo(blit_ubo, sizeof(blit_uniforms), &blit_uniforms);

  bind_shader(blit_prog);
  bind_vao(blit_vao);
  bind_ubo(blit_prog, "BlitUniforms", 0, blit_ubo);
  bind_texture(blit_prog, "tex", 0, tex);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//-----------------------------------------------------------------------------

void Blitter::blit_mono(Viewport view, dvec2 screen_size,
                        uint32_t tex, int tw, int th,
                        int sx, int sy, int sw, int sh,
                        int dx, int dy, int dw, int dh) {
  BlitUniforms blit_uniforms;

  blit_uniforms.viewport = {
    (float)view.screen_min(screen_size).x,
    (float)view.screen_min(screen_size).y,
    (float)view.screen_max(screen_size).x,
    (float)view.screen_max(screen_size).y,
  };

  blit_uniforms.blit_src_rect = {sx, sy, sx+sw, sy+sh};
  blit_uniforms.blit_dst_rect = {dx, dy, dx+dw, dy+dh};
  blit_uniforms.blit_tex_size = {0, 0, tw, th};
  blit_uniforms.blit_col      = {1,1,1,1};

  blit_uniforms.solid = 0;
  blit_uniforms.mono = 1;

  update_ubo(blit_ubo, sizeof(blit_uniforms), &blit_uniforms);

  bind_shader(blit_prog);
  bind_vao(blit_vao);
  bind_ubo(blit_prog, "BlitUniforms", 0, blit_ubo);
  bind_texture(blit_prog, "tex", 0, tex);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//-----------------------------------------------------------------------------
