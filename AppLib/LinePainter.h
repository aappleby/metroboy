#pragma once
#include <string>

#include "AppLib/Viewport.h"

struct LineUniforms {
  vec4 viewport = {};
  vec4 origin = {};
};

class LinePainter {
public:

  void init();
  void push(int ax, int ay, int bx, int by);
  void render(Viewport view, double x, double y, float scale = 1.0f);

  void push(float ax, float ay, float bx, float by) {
    push(int(ax), int(ay), int(bx), int(by));
  }

private:

  uint32_t line_prog = 0;

  int inst_begin = 0;
  int inst_end = 0;

  uint32_t  line_vao = 0;
  uint32_t  line_vbo = 0;
  uint32_t* line_data = 0;

  uint32_t line_ubo = 0;
  LineUniforms line_uniforms = {};
};
