#pragma once

#include "Sch_Top.h"
#include "StateManager2.h"

class GateBoy {
public:

  void init();
  void reset(uint16_t new_pc);
  void render_frame(int screen_w, int screen_h, TextPainter& text_painter);

  void step(int count) {
    state_manager.step(count);
  }

  void unstep(int count) {
    state_manager.unstep(count);
  }

  uint8_t read_cycle (uint16_t addr);
  void    write_cycle(uint16_t addr, uint8_t data);
  void    pass_cycle ();

  StateManager2<Schematics::SchematicTop> state_manager;

  static int main(int argc, char** argv);
};