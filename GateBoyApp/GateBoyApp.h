#pragma once

#include "CoreLib/Assembler.h"
#include "CoreLib/StateManager2.h"
#include "CoreLib/SmoothTimer.h"

#include "AppLib/App.h"
#include "AppLib/GridPainter.h"
#include "AppLib/TextPainter.h"
#include "AppLib/DumpPainter.h"
#include "AppLib/GBBlitter.h"
#include "AppLib/Blitter.h"

#include "GateBoyLib/GateBoy.h"

class GateBoyApp : public App {
public:

  const char* app_get_title() override;

  void app_init() override;
  void app_close() override;

  void begin_frame() { frame_begin = timestamp(); }
  void app_update(double delta) override;
  void app_render_frame(Viewport view) override;
  void app_render_ui(Viewport view) override;

  void end_frame() {
    frame_end = timestamp();
    frame_time = frame_end - frame_begin;
    frame_time_smooth = frame_time_smooth * 0.99 + frame_time * 0.01;
  }

  //----------

private:

  void load_rom(const char* filename);
  void load_golden(const char* filename);
  void load_flat_dump(const char* filename);
  void save_flat_dump(const char* filename);

  enum RunMode {
    RUN_FAST,
    RUN_VSYNC,
    RUN_STEP,
  };

  RunMode runmode = RUN_STEP;
  StepSize stepmode = STEP_PHASE;

  const uint8_t* keyboard_state = nullptr;

  StateStack<GateBoy> gb;

  GridPainter grid_painter;
  TextPainter text_painter;
  DumpPainter dump_painter;
  GBBlitter   gb_blitter;
  Blitter     blitter;

  int frame_count = 0;
  int replay_cursor = 0;

  double sim_time = 0;
  double sim_time_smooth = 0;
  double sim_rate = 0;

  double frame_begin = 0;
  double frame_end = 0;
  double frame_time = 0;
  double frame_time_smooth = 0;

  uint32_t trace[912 * 154];
  int trace_tex;

  uint32_t overlay[160 * 144];
  int overlay_tex;

  uint8_t golden_u8[160 * 144];
  bool has_golden = false;
  bool show_diff = false;
  bool show_golden = false;
  bool draw_passes = false;

  int ram_tex;

  Assembler assembler;

  blob rom_buf;
};