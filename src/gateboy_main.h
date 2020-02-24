#pragma once

#include "AppBase.h"
#include "TextPainter.h"

#include "TestGB.h"

#include "StateManager2.h"

using namespace Schematics;

class GateboyMain : public AppBase {
public:

  void init();
  void close();

private:

  uint8_t read_cycle (uint16_t addr);
  void    write_cycle(uint16_t addr, uint8_t data);
  void    pass_cycle ();
  uint8_t rw_cycle(uint16_t addr, uint8_t data);

  typedef AppBase base;

  TextPainter text;

  void print(SignalState s);

  //----------

  StateManager2<TestGB> gbs;

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