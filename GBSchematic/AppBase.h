#pragma once
#include <stdint.h>
#include <SDL.h>

struct Buf{
  int len;
  uint32_t vao;
  uint32_t vbo[8];
};

struct Tex {
  int width;
  int height;
  int gl_tex;
};

class AppBase {
public:
  int main(int argc, char** argv);
  virtual void init() = 0;
  virtual void close() = 0;

protected:

  virtual void begin_frame() = 0;
  virtual void update() = 0;
  virtual void render_frame() = 0;
  virtual void render_ui() = 0;
  virtual void end_frame() = 0;

  //----------

  virtual void quad_init() final;
  virtual void blit_init() final;
  virtual void check_gl_error() final;
  virtual int compile_shader(const char* name, const char* vert_source, const char* frag_source) final;
  
  virtual uint32_t create_texture(int width, int height, int channels, bool filter = false) final;
  virtual void     update_texture(uint32_t tex, int width, int height, int channels, void* pix) final;

  virtual void blit(uint32_t tex, int x, int y, int w, int h) final;
  virtual void blit_mono(uint32_t tex, int x, int y, int w, int h) final;

  uint32_t create_vao();
  uint32_t create_vbo(int size_bytes);
  uint32_t create_vbo(void* data, int size_bytes);
  void     update_vbo(uint32_t buf, void* data, int size_bytes);

  //----------

  bool quit = false;
  int frame_count = 0;
  uint64_t timer_freq, frame_begin, frame_end;
  uint64_t app_begin = 0;
  float now;

  const int fb_width = 1888;
  const int fb_height = 1024;

  SDL_Window* window = nullptr;
  SDL_GLContext gl_context = nullptr;
  const uint8_t* keyboard_state = nullptr;

  //Buf quad_buf;
  uint32_t quad_vao;
  uint32_t quad_vbo;
  uint32_t blit_prog;
  uint32_t checker_tex;
  uint32_t bg_tex;
};