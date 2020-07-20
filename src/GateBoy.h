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

  Schematics::SchematicTop* top() {
    return state_manager.state();
  }

  void run   (Schematics::SchematicTop* top, int phase_count, Req req, bool verbose);
  void phase (Schematics::SchematicTop* top, Req req, bool verbose);

  StateManager2<Schematics::SchematicTop> state_manager;

  static int main(int argc, char** argv);
  //void render_frame(int screen_w, int screen_h, TextPainter& text_painter);
};