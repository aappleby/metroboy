#pragma once

#include "Blitter.h"
#include "GridPainter.h"
#include "TextPainter.h"
#include "Viewport.h"

//-----------------------------------------------------------------------------

class AppBase {
public:
  virtual ~AppBase() = default;

  int app_main(int argc, char** argv);

  virtual const char* get_title() { return "MetroBoy Game Boy Simulator"; }
  virtual void init() {};
  virtual void close() {};
  virtual void update(double /*delta*/) {}
  virtual void render_frame(int /*screen_w*/, int /*screen_h*/) {}
  virtual void render_ui(int /*screen_w*/, int /*screen_h*/) {}
  virtual Viewport get_viewport() { return view_snap; }

  //----------

  int redraw_count = 500;

  void redraw() {
    redraw_count++;
  }

protected:

  uint64_t app_start = 0;

  int frame_count = 0;
  //int screen_w = 0;
  //int screen_h = 0;

  Blitter     blitter;
  GridPainter grid_painter;
  TextPainter text_painter;

private:

  Viewport view_raw;
  Viewport view_smooth;
  Viewport view_snap;

  bool quit = false;
  void* window = nullptr;
  void* gl_context = nullptr;
  const uint8_t* keyboard_state = nullptr;

  uint32_t imgui_prog = 0;
  uint32_t imgui_tex = 0;
  uint32_t imgui_vao = 0;
  uint32_t imgui_vbo = 0;
  uint32_t imgui_ibo = 0;
};

//-----------------------------------------------------------------------------