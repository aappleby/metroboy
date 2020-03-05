#pragma once
#include "Viewport.h"

#include <stdint.h>

struct Rect {
  float x;
  float y;
  float w;
  float h;
};

struct GridUniforms {
  Rect viewport;
};

class GridPainter {
public:

  void init();
  //void update(double delta, double zoom_level, double origin_x, double origin_y);
  void render();

  void set_viewport(Viewport vp) { viewport = vp; }

private:

  Viewport viewport;

  uint64_t last_frame_time = 0;

  double zoom_level_smooth = 0;
  double origin_x_smooth = 0;
  double origin_y_smooth = 0;

  uint32_t grid_vao;
  uint32_t grid_vbo;
  uint32_t grid_prog;
  uint32_t grid_ubo;
  GridUniforms grid_uniforms;
};