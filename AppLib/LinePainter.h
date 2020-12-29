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
  void push(int ax, int ay, uint32_t ac, int bx, int by, uint32_t bc);
  void render(Viewport view, double x, double y, float scale = 1.0f);

  void push(float ax, float ay, float bx, float by) {
    push(int(ax), int(ay), 0xFF0000FF, int(bx), int(by), 0xFF00FF00);
  }

  void push(dvec2 a, uint32_t ac, dvec2 b, uint32_t bc) {
    push(int(a.x), int(a.y), ac,
         int(b.x), int(b.y), bc);
  }

  void push_box(dvec2 a, dvec2 b, uint32_t c) {
    push(int(a.x), int(a.y), c, int(b.x), int(a.y), c);
    push(int(b.x), int(a.y), c, int(b.x), int(b.y), c);
    push(int(b.x), int(b.y), c, int(a.x), int(b.y), c);
    push(int(a.x), int(b.y), c, int(a.x), int(a.y), c);
  }

private:

  const int max_line_bytes = 65536 * 4;

  uint32_t line_prog = 0;

  int inst_begin = 0;
  int line_end = 0;

  uint32_t  line_vao = 0;
  uint32_t  line_vbo = 0;
  uint32_t* line_data = 0;

  uint32_t line_ubo = 0;
  LineUniforms line_uniforms = {};
};
