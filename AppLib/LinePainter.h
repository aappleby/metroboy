#pragma once
#include <vector>

#include "AppLib/Viewport.h"

class LinePainter {
public:

  void init();
  void render(Viewport view, dvec2 screen_size, double x, double y, float scale = 1.0f);

  void push(float ax, float ay, uint32_t ac, float bx, float by, uint32_t bc) {
    line_data.push_back(*(uint32_t*)&ax);
    line_data.push_back(*(uint32_t*)&ay);
    line_data.push_back(*(uint32_t*)&ac);
    line_data.push_back(*(uint32_t*)&bx);
    line_data.push_back(*(uint32_t*)&by);
    line_data.push_back(*(uint32_t*)&bc);
  }

  void push(dvec2 a, uint32_t ac, dvec2 b, uint32_t bc) {
    push(float(a.x), float(a.y), ac, float(b.x), float(b.y), bc);
  }

  void push_box(dvec2 a, dvec2 b, uint32_t c) {
    push(float(a.x), float(a.y), c, float(b.x), float(a.y), c);
    push(float(b.x), float(a.y), c, float(b.x), float(b.y), c);
    push(float(b.x), float(b.y), c, float(a.x), float(b.y), c);
    push(float(a.x), float(b.y), c, float(a.x), float(a.y), c);
  }

  void push_corner_size(dvec2 a, dvec2 s, uint32_t c) {
    push(float(a.x),       float(a.y),       c, float(a.x + s.x), float(a.y),       c);
    push(float(a.x + s.x), float(a.y),       c, float(a.x + s.x), float(a.y + s.y), c);
    push(float(a.x + s.x), float(a.y + s.y), c, float(a.x),       float(a.y + s.y), c);
    push(float(a.x),       float(a.y + s.y), c, float(a.x),       float(a.y),       c);
  }

private:

  uint32_t line_prog = 0;
  uint32_t line_ubo = 0;
  uint32_t line_vao = 0;
  uint32_t line_vbo = 0;

  std::vector<uint32_t> line_data;
};
