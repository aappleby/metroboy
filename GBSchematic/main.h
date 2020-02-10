#pragma once

#include "AppBase.h"
#include "../src/TextPainter.h"

#include "tests/TestGB.h"

using namespace Schematics;

class Main : public AppBase {
public:

  void init();
  void close();

private:

  typedef AppBase base;

  TextPainter text;

  void print(SignalState s);

  //----------

  void sim_phase();

  TestGB gb;
  int phase_counter = 0;
  bool RST = 1;
  bool CLKIN_A = 0;
  bool CLKIN_B = 0;
  bool CLKREQ = 0;

  //----------

  uint32_t gb_tex;
  uint32_t viz_prog;

  //----------
  // AppBase::

  void begin_frame();
  void update();
  void render_frame();
  void render_ui();
  void end_frame();
};