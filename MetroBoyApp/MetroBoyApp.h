#pragma once

#include "CoreLib/StateManager.h"
#include "CoreLib/File.h"
#include "CoreLib/SmoothTimer.h"
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

  virtual void begin_frame() { frame_begin = timestamp(); }
  virtual void app_update(double delta);
  virtual void app_render_frame(Viewport view);
  virtual void app_render_ui(Viewport view);
  virtual void end_frame()   { frame_end = timestamp(); frame_time = frame_end - frame_begin; }

  void post();
  void load_rom(const std::string& prefix, const std::string& name);
  void load_rom(const std::string& name) { load_rom("./", name); }

  void load_memdump(const std::string& prefix, const std::string& name);

  //----------------------------------------

  void sync_to_vblank() {
    while(gb->ppu.line != 144) step_cycle();
  }

  void run_to_breakpoint(uint16_t breakpoint) {
    while (gb->cpu.op_addr != breakpoint) step_cycle();
  }

  void step_phase(int count = 1) {
    for (int i = 0; i < count; i++) {
      gb->next_phase();
    }
  }

  void step_cycle(int count = 1) {
    for (int i = 0; i < count; i++) {
      step_phase();
      while(gb->phase_total & 7) step_phase();
    }
  }

  void step_line(int count = 1) {
    for (int i = 0; i < count; i++) {
      int old_line = gb->ppu.line;
      while(gb->ppu.line == old_line) step_cycle();
    }
  }

  void step_frame(int count = 1) {
    for (int i = 0; i < count; i++) {
      while (gb->ppu.line == 144) step_cycle();
      while (gb->ppu.line != 144) step_cycle();
    }
  }

  //----------------------------------------

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

  double sim_time = 0;
  double sim_rate = 0;

  double frame_begin = 0;
  double frame_end = 0;
  double frame_time = 0;

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

  StateStack<MetroBoy> gb;

  //----------
  // debug

  blob rom;
  uint8_t golden[160 * 144] = {0};
  uint32_t tracebuffer[456 * 154] = {0};
  int tracecursor = 0;
};
