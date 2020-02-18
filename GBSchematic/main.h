#pragma once

#include "AppBase.h"
#include "../src/TextPainter.h"

#include "tests/TestGB.h"

#include "StateManager.h"

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

  StateManager<TestGB> gbs;

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