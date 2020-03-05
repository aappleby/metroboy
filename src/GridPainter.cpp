#include "GridPainter.h"

#include <stdio.h>
#include <math.h>
#include <GL/gl3w.h>
#include <include/SDL.h>

//-----------------------------------------------------------------------------

static const GLchar* vert_hdr = R"(
#version 460

struct Rect {
  float x;
  float y;
  float w;
  float h;
};

layout(std140, binding = 0) uniform GridUniforms
{
  Rect viewport;
};

)";

//-----------------------------------------------------------------------------

static const GLchar* vert_src = R"(

layout(location = 0) in  vec2 vert_pos;
layout(location = 0) out vec2 world_pos; // interpolating this is going to lose precision...

void main() {
  world_pos.x = vert_pos.x * viewport.w + viewport.x;
  world_pos.y = vert_pos.y * viewport.h + viewport.y;

  gl_Position = vec4(2.0 * vert_pos.x - 1.0, -2.0 * vert_pos.y + 1.0, 1.0, 1.0);
}

)";

//-----------------------------------------------------------------------------

static const GLchar* frag_src = R"(

layout(location = 0) in  vec2 world_pos;
layout(location = 0) out vec4 frag_col;

void main() {
  bool bx = fract(world_pos.x * (1.0 / 64.0)) > 0.5;
  bool by = fract(world_pos.y * (1.0 / 64.0)) > 0.5;
  bool b = bx ^^ by;

  float ga = 14.0 / 255.0;
  float gb = 19.0 / 255.0;

  frag_col.r = b ? ga : gb;
  frag_col.g = b ? ga : gb;
  frag_col.b = b ? ga : gb;
  frag_col.a = 1.0;
}

)";

//-----------------------------------------------------------------------------

void GridPainter::init() {
  float quad[] = {
     0,  0,  1,  0,  1,  1,
     0,  0,  1,  1,  0,  1,
  };

  glGenVertexArrays(1, &grid_vao);
  glBindVertexArray(grid_vao);

  glGenBuffers(1, &grid_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
  glBufferStorage(GL_ARRAY_BUFFER, sizeof(quad), quad, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  const GLchar* vert_srcs[] = {
    vert_hdr,
    vert_src
  };

  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 2, vert_srcs, NULL);
  glCompileShader(vertexShader);

  const GLchar* frag_srcs[] = {
    vert_hdr,
    frag_src
  };

  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 2, frag_srcs, NULL);
  glCompileShader(fragmentShader);

  grid_prog = glCreateProgram();
  glAttachShader(grid_prog, vertexShader);
  glAttachShader(grid_prog, fragmentShader);
  glLinkProgram(grid_prog);
  glUseProgram(grid_prog);

  last_frame_time = SDL_GetPerformanceCounter();

  glGenBuffers(1, &grid_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, grid_ubo);
  glNamedBufferStorage(grid_ubo, sizeof(GridUniforms), nullptr, GL_DYNAMIC_STORAGE_BIT);
}


//-----------------------------------------------------------------------------

#pragma warning(disable:4189)

void GridPainter::render() {
  grid_uniforms.viewport = {(float)viewport.mx(), (float)viewport.my(), (float)viewport.dx(), (float)viewport.dy() };
  glNamedBufferSubData(grid_ubo, 0, sizeof(grid_uniforms), &grid_uniforms);
  
  glUseProgram(grid_prog);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, grid_ubo);
  glBindVertexArray(grid_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

//-----------------------------------------------------------------------------
