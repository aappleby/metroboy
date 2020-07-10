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

  SignalHash mcycle(
    Schematics::SchematicTop* top,
    bool RST,
    bool CLK_GOOD,
    bool T1,
    bool T2,
    uint16_t addr,
    uint8_t data,
    bool read,
    bool write
  );

  SignalHash tcycle(
    Schematics::SchematicTop* top,
    bool RST,
    bool CLK_GOOD,
    bool T1,
    bool T2,
    uint16_t addr,
    uint8_t data,
    bool read,
    bool write
  );

  SignalHash phase(
    Schematics::SchematicTop* top,
    bool RST,
    bool CLK_GOOD,
    bool CLK,
    bool T1,
    bool T2,
    uint16_t addr,
    uint8_t data,
    bool read,
    bool write
  );

  SignalHash pass(
    Schematics::SchematicTop* top,
    bool CLK_GOOD,
    bool CLK,
    bool RST,
    bool T1,
    bool T2,
    uint16_t addr,
    uint8_t data,
    bool read,
    bool write
  );

  StateManager2<Schematics::SchematicTop> state_manager;

  static int main(int argc, char** argv);
  //void render_frame(int screen_w, int screen_h, TextPainter& text_painter);
};