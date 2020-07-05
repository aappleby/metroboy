#pragma once

#include "Sch_Top.h"
#include "StateManager2.h"

class GateBoy {
public:

  void init();
  void reset(uint16_t new_pc);
  
  void step(int count) {
    state_manager.step(count);
  }

  void unstep(int count) {
    state_manager.unstep(count);
  }

  Schematics::SignalHash cycle(Schematics::SchematicTop* top, uint16_t addr, uint8_t data, bool read, bool write);
  Schematics::SignalHash phase(Schematics::SchematicTop* top);
  Schematics::SignalHash pass (Schematics::SchematicTop* top);

  StateManager2<Schematics::SchematicTop> state_manager;

  static int main(int argc, char** argv);
  //void render_frame(int screen_w, int screen_h, TextPainter& text_painter);
};