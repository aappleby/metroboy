#pragma once

#include "CoreLib/StateManager.h"
#include "CoreLib/File.h"
#include "AppLib/App.h"
#include "AppLib/GridPainter.h"
#include "AppLib/GBBlitter.h"
#include "AppLib/DumpPainter.h"
#include "AppLib/Blitter.h"
#include "AppLib/GridPainter.h"
#include "AppLib/TextPainter.h"
#include "MetroBoyLib/MetroBoy.h"

#include <string>

class MetroBoyApp : public App {
public:

  MetroBoyApp();

  virtual const char* app_get_title();
  virtual void app_init();
  virtual void app_close();
  virtual void app_update(double delta);
  virtual void app_render_frame(Viewport view);
  virtual void app_render_ui(Viewport view);

  void post();
  void load_rom(const std::string& prefix, const std::string& name);
  void load_rom(const std::string& name) { load_rom("./", name); }

  void load_memdump(const std::string& prefix, const std::string& name);

  enum RunMode {
    RUN_FAST,
    RUN_VSYNC,
    STEP_FRAME,
    STEP_LINE,
    STEP_PHASE
  };

  double app_start = 0;

  Blitter     blitter;
  GridPainter grid_painter;
  TextPainter text_painter;

  //----------
  // controls

  int  step_forward = 0;
  int  step_backward = 0;
  bool step_up = false;
  bool step_down = false;
  bool reset = false;
  bool load_dump = false;
  bool save_dump = false;
  
  double mcycles_per_sec_1x = 114 * 154 * 60; // 1.05 mhz
  double sim_speed = 1.06;
  double sim_budget_msec = 60.0;
  double fast_mcycles = mcycles_per_sec_1x * sim_speed * sim_budget_msec / 1000.0;
  int last_mcycles = 0;

  double sim_time_msec = 0.0;

  RunMode runmode = STEP_PHASE;
  bool rom_loaded = false;
  int overlay_mode = 0;
  const uint8_t* keyboard_state = nullptr;
  uint8_t buttons = 0;

  //----------
  // viz

  GBBlitter gb_blitter;
  DumpPainter dump_painter;

  uint32_t gb_tex = 0;
  uint32_t vram_ubo = 0;
  uint32_t trace_tex = 0;

  uint32_t ram_tex = 0;

  //----------
  // gb state

  StateManagerBase<MetroBoy> gb;

  //----------
  // debug

  uint8_t golden[160 * 144] = {};

  blob rom;
  uint32_t tracebuffer[456 * 154] = {0};
  int tracecursor = 0;
};
