#include "AppLib/LinePainter.h"

#include "AppLib/GLBase.h"

#include <stdio.h>
#include "SDL/include/SDL.h"
#include "glad/glad.h"

//-----------------------------------------------------------------------------

const char* line_glsl = R"(
layout(std140) uniform LineUniforms {
  vec4  viewport;
  vec4  origin;
};

#ifdef _VERTEX_

layout(location = 0) in ivec2 line_vtx;
layout(location = 1) in vec4  line_col;

out vec4  fg_color;

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  fg_color = line_col;
  return x;
}

void main() {
  gl_Position = vec4(remap(float(line_vtx.x), viewport.x, viewport.z, -1.0,  1.0),
                     remap(float(line_vtx.y), viewport.y, viewport.w,  1.0, -1.0),
                     0.0,
                     1.0);
}

#else

in vec4 fg_color;
out vec4 fs_out;

void main() {
  //fs_out = vec4(1.0, 1.0, 1.0, 0.3);
  fs_out = fg_color;
}

#endif
)";

//-----------------------------------------------------------------------------

void LinePainter::init() {
  line_prog = create_shader("line_glsl", line_glsl);

  line_data = new uint32_t[65536];
  line_ubo = create_ubo(sizeof(LineUniforms));
  line_vao = create_vao();
  line_vbo = create_vbo(65536 * 4);

  glEnableVertexAttribArray(0);
  glVertexAttribIPointer(0, 2, GL_INT, 12, 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 12, (void*)8);
}

//-----------------------------------------------------------------------------

void LinePainter::render(Viewport view, double x, double y, float scale) {
  bind_shader(line_prog);

  line_uniforms.origin = {x, y, scale, scale};
  line_uniforms.viewport = {
    (float)view.min.x,
    (float)view.min.y,
    (float)view.max.x,
    (float)view.max.y,
  };
  update_ubo(line_ubo, sizeof(line_uniforms), &line_uniforms);
  bind_ubo(line_prog, "LineUniforms", 0, line_ubo);

  bind_vao(line_vao);

  int vert_count = inst_end / 3;

  update_vbo(line_vbo, inst_end * 4, line_data);
  glDrawArrays(GL_LINES, 0, vert_count);
  inst_begin = inst_end = 0;
}

//-----------------------------------------------------------------------------

void LinePainter::push(int ax, int ay, int bx, int by) {
  line_data[inst_end++] = ax;
  line_data[inst_end++] = ay;
  line_data[inst_end++] = 0x7777FFFF;
  line_data[inst_end++] = bx;
  line_data[inst_end++] = by;
  line_data[inst_end++] = 0x77FF77FF;
}

//-----------------------------------------------------------------------------
