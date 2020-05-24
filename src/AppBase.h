#pragma once

#include "Blitter.h"
#include "TextPainter.h"
#include "Viewport.h"

#include <stdint.h>
#include <glm/glm.hpp>

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

//-----------------------------------------------------------------------------

class AppBase {
public:
  virtual ~AppBase() = default;

  int main(int argc, char** argv);
  virtual void init() {};
  virtual void close() {};
  virtual void update(double /*delta*/) {}
  virtual void begin_frame() {}
  virtual void render_frame() {}
  virtual void render_ui() {}
  virtual void end_frame() {}
  virtual Viewport get_viewport() { return {{0,0},{1920,1080},{1920,1080}}; }

  //----------

  int redraw_count = 500;

  void redraw() {
    redraw_count++;
  }

protected:

  uint64_t app_start = 0;
  uint64_t perf_freq = 0;

  int frame_count = 0;
  int screen_w = 0;
  int screen_h = 0;

  Viewport view_snap;
  Blitter blitter;
  TextPainter text_painter;

private:

  bool quit = false;
  SDL_Window* window = nullptr;
  SDL_GLContext gl_context = nullptr;
  const uint8_t* keyboard_state = nullptr;

  uint32_t imgui_prog = 0;
  uint32_t imgui_tex = 0;
  uint32_t imgui_vao = 0;
  uint32_t imgui_vbo = 0;
  uint32_t imgui_ibo = 0;
};

//-----------------------------------------------------------------------------