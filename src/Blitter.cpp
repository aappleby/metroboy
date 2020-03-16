#include "Blitter.h"
#include "GLBase.h"
#include <glm/glm.hpp>

//-----------------------------------------------------------------------------

struct BlitUniforms {
  vec4 viewport;

  vec4 blit_src_rect;
  vec4 blit_dst_rect;
  vec4 blit_tex_size;
  vec4 blit_col;
  int  solid;
  int  mono;
  int  palette;
  int  pad2;
};

//-----------------------------------------------------------------------------

const char* blit_glsl = R"(
uniform sampler2D tex;


layout(std140) uniform BlitUniforms
{
  vec4 viewport;

  vec4 blit_src_rect;
  vec4 blit_dst_rect;
  vec4 blit_tex_size;
  vec4 blit_col;
  int  solid;
  int  mono;
  int  palette;
  int  pad2;
};

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout(location = 0) in  vec2 vpos;
layout(location = 0) out vec2 vtex;

void main() {

  float src_x = remap(vpos.x, 0.0, 1.0, blit_src_rect.x, blit_src_rect.z);
  float src_y = remap(vpos.y, 0.0, 1.0, blit_src_rect.y, blit_src_rect.w);

  vtex.x = remap(src_x, blit_tex_size.x, blit_tex_size.z, 0.0, 1.0);
  vtex.y = remap(src_y, blit_tex_size.y, blit_tex_size.w, 0.0, 1.0);

  float dst_x = remap(vpos.x, 0.0, 1.0, blit_dst_rect.x, blit_dst_rect.z);
  float dst_y = remap(vpos.y, 0.0, 1.0, blit_dst_rect.y, blit_dst_rect.w);

  gl_Position.x = remap(dst_x, viewport.x, viewport.z, -1.0,  1.0);
  gl_Position.y = remap(dst_y, viewport.y, viewport.w,  1.0, -1.0);
  gl_Position.z = 0.0;
  gl_Position.w = 1.0;
}

#else

layout(location = 0) in  vec2 ftex;
layout(location = 0) out vec4 frag;

void main() {
  if (bool(solid)) {
    frag = vec4(1.0, 0.0, 1.0, 1.0);
  }
  else if (bool(mono)) {
    frag = vec4(texture(tex, ftex).rrr, 1.0) * blit_col;
  }
  else if (bool(palette)) {
    frag = vec4(1.0, 0.0, 1.0, 1.0);
  }
  else {
    frag = texture(tex, ftex) * blit_col;
  }
}

#endif

)";

//-----------------------------------------------------------------------------

void Blitter::init() {

  float unit_quad[] = {
    0, 0,
    1, 0,
    0, 1,
    1, 1,
  };

  blit_vao = create_vao();
  blit_vbo = create_vbo(sizeof(unit_quad), unit_quad);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  blit_ubo = create_ubo(sizeof(BlitUniforms));
  blit_prog = create_shader("blit_glsl", blit_glsl);
}

//-----------------------------------------------------------------------------

void Blitter::blit(Viewport view,
                   uint32_t tex, int tw, int th,
                   int sx, int sy, int sw, int sh,
                   int dx, int dy, int dw, int dh) {
  BlitUniforms blit_uniforms = {
    .viewport = {
      (float)view.min.x,
      (float)view.min.y,
      (float)view.max.x,
      (float)view.max.y,
    },

    .blit_src_rect = {sx, sy, sx+sw, sy+sh},
    .blit_dst_rect = {dx, dy, dx+dw, dy+dh},
    .blit_tex_size = {0, 0, tw, th},
    .blit_col = {1,1,1,1},

    .solid = 0,
    .mono = 0,
    .palette = 0,
    .pad2 = 0,
  };
  update_ubo(blit_ubo, sizeof(blit_uniforms), &blit_uniforms);

  bind_shader(blit_prog);
  bind_vao(blit_vao);
  bind_ubo(blit_prog, "BlitUniforms", 0, blit_ubo);
  bind_texture(blit_prog, "tex", 0, tex);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//-----------------------------------------------------------------------------
