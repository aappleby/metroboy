#pragma once

#include "TextPainter.h"

#include <stdint.h>
#include <glm/glm.hpp>

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

struct BlitUniforms {
  uint64_t tex_ptr;
  uint64_t pad1;

  vec4 quad_pos;
  vec4 viewport;
  vec4 screen_size;
  vec4 quad_col;
  int  solid;
  int  mono;
  int  palette;
  int  pad2;
};

class AppBase {
public:
  int main(int argc, char** argv);
  virtual void init() = 0;
  virtual void close() = 0;

protected:

  virtual void update() = 0;
  virtual void begin_frame() = 0;
  virtual void render_frame() = 0;
  virtual void render_ui() = 0;
  virtual void end_frame() = 0;

  //----------

  virtual void quad_init() final;
  virtual void blit_init() final;
  virtual int compile_shader(const char* hdr, const char* vert_src, const char* frag_src) final;
  
  virtual uint32_t create_texture(int width, int height) final;
  virtual void     update_texture(uint32_t tex, int width, int height, void* pix) final;

  virtual void blit(uint32_t tex, int x, int y, int w, int h) final;

  uint32_t create_vao();
  uint32_t create_vbo(int size_bytes);
  uint32_t create_vbo(void* data, int size_bytes);
  void     update_vbo(uint32_t buf, void* data, int size_bytes);

  //----------

  bool quit = false;
  int frame_count = 0;
  uint64_t timer_freq;
  uint64_t frame_begin, frame_end, last_frame_time;
  double last_update_time_smooth = 0;
  uint64_t update_begin, update_end, last_update_time;
  double last_frame_time_smooth = 0;

  uint64_t app_begin = 0;
  float now;

  const int fb_width = 1856;
  const int fb_height = 1024;

  SDL_Window* window = nullptr;
  SDL_GLContext gl_context = nullptr;
  const uint8_t* keyboard_state = nullptr;

  TextPainter text_painter;

  //Buf quad_buf;
  uint32_t quad_vao;
  uint32_t quad_vbo;
  uint32_t blit_prog;
  uint32_t checker_tex;
  uint32_t bg_tex;

  uint32_t     blit_ubo;
  BlitUniforms blit_uniforms;
};