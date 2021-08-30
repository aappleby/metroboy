#include "AppLib/GridPainter.h"
#include "AppLib/GLBase.h"

#include <stdio.h>
#include <math.h>
#include "SDL/include/SDL.h"
#include "glad/glad.h"

//-----------------------------------------------------------------------------

static const GLchar* grid_glsl = R"(

struct Rect {
  float x;
  float y;
  float w;
  float h;
};

layout(std140) uniform GridUniforms
{
  Rect viewport;
  float world_width;
  float world_height;
};

#ifdef _VERTEX_

out vec2 world_pos;

void main() {
  float x = gl_VertexID == 1 ? 2.0 : 0.0;
  float y = gl_VertexID == 2 ? 2.0 : 0.0;

  world_pos.x = x * viewport.w + viewport.x;
  world_pos.y = y * viewport.h + viewport.y;

  gl_Position = vec4(2.0 * x - 1.0, -2.0 * y + 1.0, 1.0, 1.0);
}

#endif

#ifdef _FRAGMENT_

in  vec2 world_pos;
out vec4 frag_col;

void main() {

  float half_world_width = world_width * 0.5;
  float half_world_height = world_height * 0.5;

  int world_x = int(floor(world_pos.x));
  int world_y = int(floor(world_pos.y));

  bool bx = (((world_x >> 5) & 1) == 0);
  bool by = (((world_y >> 5) & 1) == 0);
  bool b  = bx ^^ by;


  float ga = 14.0 / 255.0;
  float gb = 19.0 / 255.0;

  frag_col.r = b ? ga : gb;
  frag_col.g = b ? ga : gb;
  frag_col.b = b ? ga : gb;
  frag_col.a = 1.0;

  //if (world_pos.x < -half_world_width)  frag_col.r = 0.2;
  //if (world_pos.x >  half_world_width)  frag_col.r = 0.2;
  //if (world_pos.y < -half_world_height) frag_col.r = 0.2;
  //if (world_pos.y >  half_world_height) frag_col.r = 0.2;

  if (world_pos.x < -half_world_width)  frag_col.rgb *= 0.9;
  if (world_pos.x >  half_world_width)  frag_col.rgb *= 1.1;
  if (world_pos.y < -half_world_height) frag_col.rgb *= 0.9;
  if (world_pos.y >  half_world_height) frag_col.rgb *= 1.1;

  //if (world_pos.x > -16.0 && world_pos.x < 16.0) frag_col.g += 0.2;

  if (world_pos.x > (-half_world_width - 16.0) && world_pos.x < (-half_world_width + 16.0))   frag_col.r += 0.2;
  if (world_pos.x > ( half_world_width - 16.0) && world_pos.x < ( half_world_width + 16.0))   frag_col.r += 0.2;
  if (world_pos.y > (-half_world_height - 16.0) && world_pos.y < (-half_world_height + 16.0)) frag_col.r += 0.2;
  if (world_pos.y > ( half_world_height - 16.0) && world_pos.y < ( half_world_height + 16.0)) frag_col.r += 0.2;
}

#endif

)";

static uint32_t grid_prog = 0;

//-----------------------------------------------------------------------------

void GridPainter::init(float world_width, float world_height) {
  grid_uniforms.world_width  = world_width;
  grid_uniforms.world_height = world_height;

  if (!grid_prog) {
    grid_prog = create_shader("grid_glsl", grid_glsl);
  }

  grid_ubo = create_ubo();
}

//-----------------------------------------------------------------------------

#pragma warning(disable:4189)

void GridPainter::render(Viewport view, dvec2 screen_size) {
  bind_shader(grid_prog);

  dvec2 screen_size_world = screen_size * view.scale_screen_to_world();


  grid_uniforms.viewport = {
    (float)view.screen_min(screen_size).x,
    (float)view.screen_min(screen_size).y,
    (float)screen_size_world.x,
    (float)screen_size_world.y
  };

  update_ubo(grid_ubo, sizeof(grid_uniforms), &grid_uniforms);
  bind_ubo(grid_prog, "GridUniforms", 0, grid_ubo);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

//-----------------------------------------------------------------------------
