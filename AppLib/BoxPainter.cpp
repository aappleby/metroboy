#include "AppLib/BoxPainter.h"

#include "AppLib/GLBase.h"

#include <stdio.h>
#include "SDL/include/SDL.h"
#include "glad/glad.h"

//-----------------------------------------------------------------------------

const char* box_glsl = R"(
layout(std140) uniform BoxUniforms {
  vec4  viewport;
  vec4  origin;
};

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout(location = 0) in vec4 box_pos;
layout(location = 1) in vec4 box_col;

out vec4 frag_col;

void main() {
  float corner_x = float((gl_VertexID >> 0) & 1);
  float corner_y = float((gl_VertexID >> 1) & 1);

  float quad_x = box_pos.x + box_pos.z * corner_x;
  float quad_y = box_pos.y + box_pos.w * corner_y;

  quad_x = quad_x * origin.z + origin.x;
  quad_y = quad_y * origin.w + origin.y;

  gl_Position = vec4(remap(quad_x, viewport.x, viewport.z, -1.0,  1.0),
                     remap(quad_y, viewport.y, viewport.w,  1.0, -1.0),
                     0.0,
                     1.0);
  frag_col = box_col;
}

#else

in vec4 frag_col;
out vec4 fs_out;

void main() {
  fs_out = frag_col;
}

#endif
)";

//-----------------------------------------------------------------------------

void BoxPainter::init() {
  box_prog = create_shader("box_glsl", box_glsl);

  box_data_u32 = new uint32_t[65536];
  box_data_f32 = reinterpret_cast<float*>(box_data_u32);

  box_vao = create_vao();
  box_vbo = create_vbo(65536 * 4);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 4, GL_FLOAT,         GL_FALSE, 20, 0);
  glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,  20, (void*)16);
  glVertexAttribDivisor(0, 1);
  glVertexAttribDivisor(1, 1);

  box_ubo = create_ubo(sizeof(BoxUniforms));
}

//-----------------------------------------------------------------------------

void BoxPainter::push(float x, float y, float w, float h, uint32_t col) {
  box_data_f32[box_cursor++] = x;
  box_data_f32[box_cursor++] = y;
  box_data_f32[box_cursor++] = w;
  box_data_f32[box_cursor++] = h;
  box_data_u32[box_cursor++] = col;
}

//-----------------------------------------------------------------------------

void BoxPainter::render(Viewport view, double x, double y, float scale) {

  bind_shader(box_prog);

  box_uniforms.viewport = { (float)view.min.x, (float)view.min.y, (float)view.max.x, (float)view.max.y };
  box_uniforms.origin = { x, y, scale, scale };
  update_ubo(box_ubo, sizeof(box_uniforms), &box_uniforms);
  bind_ubo(box_prog, "BoxUniforms", 0, box_ubo);

  bind_vao(box_vao);

  int box_count = box_cursor / 5;
  update_vbo(box_vbo, box_count * bytes_per_box, box_data_u32);

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, box_count);
  box_cursor = 0;
}

//-----------------------------------------------------------------------------
