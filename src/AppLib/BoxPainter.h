#pragma once
#include <string>

#include "AppLib/Viewport.h"

struct BoxUniforms {
  vec4 viewport = {};
  vec4 origin = {};
};

class BoxPainter {
public:

  void init();

  void push_corner_corner(float x, float y, float w, float h, uint32_t color);
  void push_corner_size  (float x, float y, float w, float h, uint32_t color);

  void push_corner_size(dvec2 pos, dvec2 size, uint32_t color) {
    push_corner_size(float(pos.x), float(pos.y), float(size.x), float(size.y), color);
  }

  void push_center_size(dvec2 center, dvec2 size, uint32_t color) {
    push_corner_corner(float(center.x - (size.x / 2)),
                       float(center.y - (size.y / 2)),
                       float(center.x + (size.x / 2)),
                       float(center.y + (size.y / 2)),
                       color);
  }

  void render(Viewport view, dvec2 screen_size, double x, double y, float scale = 1.0f);

private:

  int gate_tex;

  const int max_box_bytes = 65536 * 16;

  uint32_t  box_ubo = 0;
  uint32_t  box_vao = 0;
  uint32_t  box_vbo = 0;
  uint32_t* box_data_u32 = nullptr;
  float*    box_data_f32 = nullptr;
  int       box_cursor = 0;

  const int bytes_per_box = 20;

  BoxUniforms box_uniforms = {};
};
