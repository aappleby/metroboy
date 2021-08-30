#include "AppLib/LinePainter.h"

#include "AppLib/GLBase.h"
#include "glad/glad.h"

//-----------------------------------------------------------------------------

const char* line_glsl = R"(
//----------------------------------------

layout(std140) uniform LineUniforms {
  vec4  viewport;
  vec4  origin;
};

//----------------------------------------
#ifdef _VERTEX_

layout(location = 0) in vec2 line_vtx;
layout(location = 1) in vec4 line_col;

out vec4  fg_color;

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  fg_color = line_col;
  return x;
}

void main() {
  float lx = line_vtx.x + origin.x;
  float ly = line_vtx.y + origin.y;

  gl_Position = vec4(remap(lx, viewport.x, viewport.z, -1.0,  1.0),
                     remap(ly, viewport.y, viewport.w,  1.0, -1.0),
                     0.0,
                     1.0);
}
#endif

//----------------------------------------
#ifdef _FRAGMENT_

in vec4 fg_color;
out vec4 fs_out;

void main() {
  fs_out = fg_color;
}

#endif

//----------------------------------------
)";

//-----------------------------------------------------------------------------

void LinePainter::init() {
  line_prog = create_shader("line_glsl", line_glsl);
  line_ubo = create_ubo();
  line_vao = create_vao();
  line_vbo = create_vbo();

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT,         GL_FALSE, 12, 0);
  glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,  12, (void*)8);
}

//-----------------------------------------------------------------------------

void LinePainter::render(Viewport view, dvec2 screen_size, double x, double y, float scale) {
  if (line_data.empty()) return;

  bind_shader(line_prog);

  float line_uniforms[] = {
    (float)view.screen_min(screen_size).x,
    (float)view.screen_min(screen_size).y,
    (float)view.screen_max(screen_size).x,
    (float)view.screen_max(screen_size).y,
    (float)x,
    (float)y,
    scale,
    scale
  };

  bind_ubo(line_prog, "LineUniforms", 0, line_ubo);
  bind_vao(line_vao);

  update_ubo(line_ubo, sizeof(line_uniforms), line_uniforms);
  update_vbo(line_vbo, (int)line_data.size() * 4, line_data.data());

  glDrawArrays(GL_LINES, 0, (int)line_data.size() / 3);

  line_data.clear();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
