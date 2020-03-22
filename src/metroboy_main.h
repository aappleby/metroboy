#pragma once

#include "AppBase.h"
#include "MetroBoy.h"
#include "GridPainter.h"
#include "GBBlitter.h"
#include "DumpPainter.h"

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
  virtual void update(double delta);
  virtual void render_frame();
  virtual void render_ui();
  virtual void end_frame();
  virtual Viewport get_viewport() { return view_snap; }

  void load(const std::string& prefix, const std::string& name);
  void load(const std::string& name) { load("./", name); }

  void load_memdump(const std::string& prefix, const std::string& name);

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
  GBBlitter gb_blitter;
  DumpPainter dump_painter;

  uint32_t gb_tex;
  uint32_t vram_ubo;
  uint32_t trace_tex;

  uint32_t ram_tex;

  Viewport view;
  Viewport view_smooth;

  //----------
  // gb state

  const int gb_screen_x = 32*32;
  const int gb_screen_y = 32*11;

  MetroBoy metroboy;
  int64_t cycles_begin, cycles_end;

  //----------
  // debug

  uint8_t golden[160 * 144];
  std::string text_buf;
};
