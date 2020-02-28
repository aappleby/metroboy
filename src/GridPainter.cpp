#include "GridPainter.h"

#include <stdio.h>
#include <math.h>
#include <GL/gl3w.h>
#include <include/SDL.h>

//-----------------------------------------------------------------------------

static const GLchar* vert_source = R"(
#version 460

layout(location = 0) in  vec2 vert_pos;

void main() {
  gl_Position = vec4(vert_pos.x, vert_pos.y, 1.0, 1.0);
}

)";

//-----------------------------------------------------------------------------

static const GLchar* frag_source = R"(
#version 460

uniform vec4 screen_size = vec4(1920.0, 1080.0, 1.0 / 1920.0, 1.0 / 1080.0);
layout(location = 0) out vec4 frag_col;

uniform dvec3 origin = dvec3(0, 0, 0);

void main() {
  double screen_offset_x = gl_FragCoord.x - screen_size.x * 0.5;
  double screen_offset_y = gl_FragCoord.y - screen_size.y * 0.5;

  double scale = pow(2.0, float(origin.z));

  double world_x = origin.x + screen_offset_x / scale;
  double world_y = origin.y + screen_offset_y / scale;

  if (world_x >= 0 && world_x <= 32 && world_y >= 0 && world_y <= 32) {
    frag_col = vec4(1.0, 0.0, 1.0, 1.0);
    return;
  }

  frag_col.r = float(world_x);
  frag_col.g = float(world_y);
  frag_col.b = 0.5;
  frag_col.a = 1.0;

  /*
  double x = (gl_FragCoord.x - screen_size.x * 0.5 - origin.x) * scale;
  double y = (gl_FragCoord.y - screen_size.y * 0.5 + origin.y) * scale;


  frag_col.r = fract(x / 64.0) > 0.5 ? 1.0 : 0.0;
  frag_col.g = fract(y / 64.0) > 0.5 ? 1.0 : 0.0;
  frag_col.b = 0.5;
  frag_col.a = 1.0;
  */
}

)";

//-----------------------------------------------------------------------------

void GridPainter::init() {
  float quad[] = {
    -1, -1,  1, -1,  1,  1,
    -1, -1,  1,  1, -1,  1,
  };

  glGenVertexArrays(1, &grid_vao);
  glBindVertexArray(grid_vao);

  glGenBuffers(1, &grid_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
  glBufferStorage(GL_ARRAY_BUFFER, sizeof(quad), quad, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  const char* name = "grid_painter";
  char buf[1024];
  int len = 0;

  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vert_source, NULL);
  glCompileShader(vertexShader);

  glGetShaderInfoLog(vertexShader, 1024, &len, buf);
  printf("%s vert shader log:\n%s", name, buf);

  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &frag_source, NULL);
  glCompileShader(fragmentShader);

  glGetShaderInfoLog(fragmentShader, 1024, &len, buf);
  printf("%s frag shader log:\n%s", name, buf);

  grid_prog = glCreateProgram();
  glAttachShader(grid_prog, vertexShader);
  glAttachShader(grid_prog, fragmentShader);
  glLinkProgram(grid_prog);
  glUseProgram(grid_prog);

  glGetProgramInfoLog(grid_prog, 1024, &len, buf);
  printf("%s shader prog log:\n%s", name, buf);
  printf("glGetError %d\n", glGetError());

  last_frame_time = SDL_GetPerformanceCounter();
}

//-----------------------------------------------------------------------------

/*
void GridPainter::update(double delta, double zoom_level, double origin_x, double origin_y) {
  //origin_x = sin(frame_count * 0.01) * 200;
  //origin_y = cos(frame_count * 0.01) * 200;

  double factor = pow(0.1, delta / 0.4);

  zoom_level_smooth = zoom_level_smooth * factor + zoom_level * (1.0 - factor);

  origin_x_smooth = origin_x_smooth * factor + origin_x * (1.0 - factor);
  origin_y_smooth = origin_y_smooth * factor + origin_y * (1.0 - factor);
}
*/

//-----------------------------------------------------------------------------

#pragma warning(disable:4189)

void GridPainter::render(Viewport vp) {
  
  glUseProgram(grid_prog);

  double origin_x = (vp.min.x + vp.max.x) * 0.5;
  double origin_y = (vp.min.y + vp.max.y) * 0.5;
  double scale = vp.screen_size.x / (vp.max.x - vp.min.x);
  double zoom = log2(scale);

  glUniform3d(glGetUniformLocation(grid_prog, "origin"), origin_x, origin_y, zoom);

  int temp[4] = {0};
  glGetIntegerv(GL_VIEWPORT, temp);
  int fb_width  = temp[2];
  int fb_height = temp[3];
  glUniform4f(glGetUniformLocation(grid_prog, "screen_size"),
              (float)fb_width, (float)fb_height, 1.0f / fb_width, 1.0f / fb_height);


  glBindVertexArray(grid_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

//-----------------------------------------------------------------------------
