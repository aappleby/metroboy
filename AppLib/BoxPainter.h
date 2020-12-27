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

  void push(float x, float y, float w, float h, uint32_t col);

  void render(Viewport view, double x, double y, float scale = 1.0f);

private:

  uint32_t  box_prog = 0;
  uint32_t  box_ubo = 0;
  uint32_t  box_vao = 0;
  uint32_t  box_vbo = 0;
  uint32_t* box_data_u32 = nullptr;
  float*    box_data_f32 = nullptr;
  int       box_cursor = 0;

  const int bytes_per_box = 20;

  BoxUniforms box_uniforms = {};
};
