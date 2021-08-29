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
#include "GateBoyLib/GateBoyThread.h"

class GateBoyApp : public App {
public:

  const char* app_get_title() override { return "GateBoyApp"; }
  void app_init(int screen_w, int screen_h) override;
  void app_close() override;
  bool pause_when_idle() override { return false; }
  void reset_to_bootrom();
  void load_raw_dump(const char* filename);
  void save_raw_dump(const char* filename);
  void load_flat_dump(const char* filename);
  void load_rom(const char* filename);

  void begin_frame() { frame_begin = timestamp(); }
  void app_update(double delta) override;
  void app_render_frame() override;
  void app_render_ui() override {};

  void end_frame() {
    frame_end = timestamp();
    frame_time = frame_end - frame_begin;
    frame_time_smooth = frame_time_smooth * 0.99 + frame_time * 0.01;
  }

  //----------

private:

  void load_golden(const char* filename);

  int screen_w = 1920;
  int screen_h = 1080;

  const uint8_t* keyboard_state = nullptr;

  GateBoyThread gb_thread;

  GridPainter grid_painter;
  TextPainter text_painter;
  DumpPainter dump_painter;
  GBBlitter   gb_blitter;
  Blitter     blitter;

  int frame_count = 0;
  int replay_cursor = 0;
  int phase_origin = 0;

  double delta = 0;
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

  bool app_paused = false;

  int ram_tex;

  Assembler assembler;
};