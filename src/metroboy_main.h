#pragma once

#include "TextPainter.h"
#include "AppBase.h"
#include "StateManager.h"
#include "GridPainter.h"

#include <string>

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class MetroBoyApp : public AppBase {
public:

  MetroBoyApp() {}

  virtual void init();
  virtual void close();
  virtual void begin_frame();
  virtual void update();
  virtual void render_frame();
  virtual void render_ui();
  virtual void end_frame();

  void load(const std::string& prefix, const std::string& name);
  void load(const std::string& name) { load("./", name); }

  void blit_map();

  enum RunMode {
    RUN_FAST,
    RUN_VSYNC,
    STEP_FRAME,
    STEP_LINE,
    STEP_CYCLE
  };

  //----------
  // controls

  int  step_forward = 0;
  int  step_backward = 0;
  bool step_up = false;
  bool step_down = false;
  bool reset = false;
  bool load_dump = false;
  bool save_dump = false;
  double fast_cycles = 114 * 154;
  RunMode runmode = STEP_CYCLE;
  bool rom_loaded = false;
  int overlay_mode = 0;
  const uint8_t* keyboard_state = nullptr;
  uint8_t buttons = 0;

  //----------
  // viz

  GridPainter grid_painter;

  uint32_t blit_map_prog;
  uint32_t blit_map_ubo;
  
  uint32_t gb_tex;
  uint32_t vram_ubo;
  uint32_t trace_tex;

  Viewport view;
  Viewport view_smooth;
  Viewport view_snap;

  //----------
  // gb state

  StateManager metroboy;
  int64_t cycles_begin, cycles_end;

  //----------
  // debug

  uint8_t golden[160 * 144];
  std::string text_buf;
};
