#pragma once

#include "GameboyLib/Assembler.h"
#include "CoreLib/StateStack.h"
#include "CoreLib/Dumper.h" // for StringDumper

#include "AppLib/App.h"
#include "AppLib/GridPainter.h"
#include "AppLib/TextPainter.h"
#include "AppLib/DumpPainter.h"
#include "GameboyLib/GBBlitter.h"
#include "AppLib/Blitter.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/GateBoyThread.h"

class GateBoyApp : public App {
public:

  const char* app_get_title() override { return "GateBoyApp"; }
  void app_init(int screen_w, int screen_h) override;
  void app_close() override;
  bool pause_when_idle() override { return false; }

  void app_update(dvec2 screen_size, double delta) override;
  void app_render_frame(dvec2 screen_size, double delta) override;
  void app_render_ui(dvec2 /*screen_size*/, double /*delta*/) override {};

  //----------

private:

  void load_golden(const char* filename);

  ViewController view_control;

  const uint8_t* keyboard_state = nullptr;

  GateBoyThread* gb_thread;

  GateBoy temp_gb;

  GridPainter grid_painter;
  TextPainter text_painter;
  DumpPainter dump_painter;
  GBBlitter   gb_blitter;
  Blitter     blitter;

  int frame_count = 0;
  int replay_cursor = 0;

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
  bool show_gb_ab = false;

  bool app_paused = false;

  int ram_tex;
  int wave_tex;

  Assembler assembler;


  blob temp_buf;
  Probes temp_probes;
  const char* temp_gb_id = nullptr;
  StringDumper d;
  uint8_t disasm_buf[64];
  uint8_t stack_buf[8];
};
