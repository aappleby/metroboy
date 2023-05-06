#pragma once

//#include "metrolib/core/Assembler.h"
//#include "metrolib/core/StateManager2.h"
//#include "metrolib/core/SmoothTimer.h"

#include "metrolib/appbase/App.h"
#include "metrolib/appbase/GridPainter.h"
#include "metrolib/appbase/TextPainter.h"
#include "metrolib/appbase/DumpPainter.h"
//#include "metrolib/appbase/GBBlitter.h"
#include "metrolib/appbase/Blitter.h"

//#include "GateBoyLib/GateBoy.h"
//#include "GateBoyLib/GateBoyThread.h"

class MetronicaApp : public App {
public:

  const char* app_get_title() override { return "Metronica"; }
  void app_init(int screen_w, int screen_h) override;
  void app_close() override;
  bool pause_when_idle() override { return false; }

  void app_update(dvec2 screen_size, double delta) override;
  void app_render_frame(dvec2 screen_size, double delta) override;
  void app_render_ui(dvec2 /*screen_size*/, double /*delta*/) override {};

  //----------

private:

  ViewController view_control;

  const uint8_t* keyboard_state = nullptr;

  GridPainter grid_painter = {};
  TextPainter text_painter = {};
  Blitter     blitter = {};

  int frame_count = 0;
  int replay_cursor = 0;

  double frame_begin = 0;
  double frame_end = 0;
  double frame_time = 0;
  double frame_time_smooth = 0;

  bool app_paused = false;

  uint64_t counter_start = 0;
  uint64_t counter_per_second = 0;
  uint64_t counter_old = 0;
  uint64_t counter_new = 0;
  uint64_t time_bucket = 0;
  uint64_t phase_old = 0;
  uint64_t phase_new = 0;

  int ram_tex = 0;
};
