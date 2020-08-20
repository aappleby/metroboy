#pragma once

#include "App.h"
#include "GateBoy.h"
#include "StateManager2.h"
#include "GridPainter.h"
#include "TextPainter.h"
#include "DumpPainter.h"
#include "GBBlitter.h"
#include "Blitter.h"

class GateBoyApp : public App {
public:

  GateBoyApp();
  ~GateBoyApp() override;

  //----------

  void reset(uint16_t new_pc);

  //----------

  const char* app_get_title() override;
  void app_init() override;
  void app_close() override;
  void app_update(double delta) override;
  void app_render_frame(Viewport view) override;
  void app_render_ui(Viewport view) override;

  //----------

  enum RunMode {
    RUN_FAST,
    RUN_VSYNC,
    STEP_FRAME,
    STEP_LINE,
    STEP_PHASE
  };

  RunMode runmode = STEP_PHASE;
  const uint8_t* keyboard_state = nullptr;

  StateManager2<GateBoy> state_manager;

  GridPainter grid_painter;
  TextPainter text_painter;
  DumpPainter dump_painter;
  GBBlitter   gb_blitter;
  Blitter     blitter;

  int frame_count = 0;
  int replay_cursor = 0;

  uint32_t trace[912 * 154];
  int trace_tex;

  uint32_t overlay[160 * 144];
  int overlay_tex;

};