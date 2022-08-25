#pragma once
#include "AppLib/Viewport.h"

struct Rect {
  float x = 0;
  float y = 0;
  float w = 0;
  float h = 0;
};

struct GridUniforms {
  Rect viewport = {};
  float world_width = 0;
  float world_height = 0;
};

class GridPainter {
public:

  void init(float world_width, float world_height);
  void render(Viewport view, dvec2 screen_size);

private:

  double zoom_level_smooth = 0;
  double origin_x_smooth = 0;
  double origin_y_smooth = 0;

  uint32_t grid_vao = 0;
  uint32_t grid_vbo = 0;
  uint32_t grid_ubo = 0;
  GridUniforms grid_uniforms;
};