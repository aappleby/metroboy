#include "AppLib/BoxPainter.h"

#include "AppLib/GLBase.h"

#include <stdio.h>
#include "SDL/include/SDL.h"
#include "glad/glad.h"

//-----------------------------------------------------------------------------

const char* box_glsl = R"(
layout(std140) uniform BoxUniforms {
  vec4  viewport;
  vec4  origin;
};

float remap(float x, float a1, float a2, float b1, float b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

#ifdef _VERTEX_

layout(location = 0) in vec4 box_pos;
layout(location = 1) in vec4 box_col;

out vec2 box_tc;
out vec4 frag_col;

void main() {
  float corner_x = float((gl_VertexID >> 0) & 1);
  float corner_y = float((gl_VertexID >> 1) & 1);

  int symbol = int(box_col.a);

  box_tc = vec2(corner_x, corner_y);

  box_tc.y *= 1.0 / 32.0;

  box_tc.y += (1.0 / 32.0) * float(symbol);

  float quad_x = box_pos.x + box_pos.z * corner_x;
  float quad_y = box_pos.y + box_pos.w * corner_y;

  quad_x = quad_x * origin.z + origin.x;
  quad_y = quad_y * origin.w + origin.y;

  gl_Position = vec4(remap(quad_x, viewport.x, viewport.z, -1.0,  1.0),
                     remap(quad_y, viewport.y, viewport.w,  1.0, -1.0),
                     0.0,
                     1.0);
  frag_col = box_col * (1.0 / 255.0);
}

#else

in vec2 box_tc;
in vec4 frag_col;
out vec4 frag_out;

uniform sampler2D tex;

void main() {
  //frag_out = frag_col;
  //frag_out = vec4(box_tc, 0.0, 1.0);
  frag_out = vec4(texture(tex, box_tc).rrr, 1.0) * vec4(frag_col.rgb, 1.0);
}

#endif
)";

static uint32_t box_prog = 0;

//-----------------------------------------------------------------------------

extern const char* gate_pix;

void BoxPainter::init() {
  if (!box_prog) {
    box_prog = create_shader("box_glsl", box_glsl);
  }

  box_data_u32 = new uint32_t[max_box_bytes / sizeof(uint32_t)];
  box_data_f32 = reinterpret_cast<float*>(box_data_u32);

  box_vao = create_vao();
  box_vbo = create_vbo(max_box_bytes, nullptr);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 4, GL_FLOAT,         GL_FALSE, 20, 0);
  glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 20, (void*)16);
  glVertexAttribDivisor(0, 1);
  glVertexAttribDivisor(1, 1);

  box_ubo = create_ubo();

  uint8_t* dst_pix = new uint8_t[16 * 512];
  for (int i = 0; i < 16 * 512; i++) {
    uint8_t c = gate_pix[i];
    if      (c == '#') c = 0x00;
    else if (c == '=') c = 0x80;
    else               c = 0xFF;
    dst_pix[i] = c;
  }
  gate_tex = create_texture_u8(16, 512, dst_pix, false);
  delete [] dst_pix;
}

//-----------------------------------------------------------------------------

void BoxPainter::push_corner_corner(float ax, float ay, float bx, float by, uint32_t col) {

  //col &= 0x00FFFFFF;
  //col |= rand() << 24;

  box_data_f32[box_cursor++] = ax;
  box_data_f32[box_cursor++] = ay;
  box_data_f32[box_cursor++] = bx - ax;
  box_data_f32[box_cursor++] = by - ay;
  box_data_u32[box_cursor++] = col;

  CHECK_P(box_cursor * sizeof(uint32_t) < max_box_bytes);
}

void BoxPainter::push_corner_size(float x, float y, float w, float h, uint32_t col) {
  //col &= 0x00FFFFFF;
  //col |= rand() << 24;


  box_data_f32[box_cursor++] = x;
  box_data_f32[box_cursor++] = y;
  box_data_f32[box_cursor++] = w;
  box_data_f32[box_cursor++] = h;
  box_data_u32[box_cursor++] = col;

  CHECK_P(box_cursor * sizeof(uint32_t) < max_box_bytes);
}

//-----------------------------------------------------------------------------

void BoxPainter::render(Viewport view, dvec2 screen_size, double x, double y, float scale) {
  if (box_cursor == 0) return;
  int box_count = box_cursor / 5;
  update_vbo(box_vbo, box_count * bytes_per_box, box_data_u32);

  if (box_cursor == 0) return;

  bind_shader(box_prog);

  box_uniforms.viewport = {
    (float)view.screen_min(screen_size).x,
    (float)view.screen_min(screen_size).y,
    (float)view.screen_max(screen_size).x,
    (float)view.screen_max(screen_size).y
  };
  box_uniforms.origin = { x, y, scale, scale };
  update_ubo(box_ubo, sizeof(box_uniforms), &box_uniforms);
  bind_ubo(box_prog, "BoxUniforms", 0, box_ubo);

  bind_vao(box_vao);

  bind_texture(box_prog, "tex", 0, gate_tex);

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, box_count);

  box_cursor = 0;
}

//-----------------------------------------------------------------------------
