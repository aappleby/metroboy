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

  double app_start = 0;

  Blitter     blitter;
  GridPainter grid_painter;
  TextPainter text_painter;

  //----------
  // controls

  RunMode runmode = RUN_STEP;
  StepSize stepsize = STEP_PHASE;

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

  blob rom;
  uint8_t golden[160 * 144] = {0};
  uint32_t tracebuffer[456 * 154] = {0};
  int tracecursor = 0;
};
