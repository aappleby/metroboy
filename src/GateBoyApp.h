#pragma once

#include "App.h"
#include "Sch_Top.h"
#include "StateManager2.h"
#include "GridPainter.h"
#include "TextPainter.h"

class GateBoyApp : public App {
public:

  //----------

  ~GateBoyApp() override;
  const char* get_title() override;
  void init() override;
  void close() override;
  void update(double delta) override;
  void render_frame(Viewport view) override;
  void render_ui(Viewport view) override;

  //----------

  void reset(uint16_t new_pc);
  
  void step(int count) {
    state_manager.step(count);
  }

  void unstep(int count) {
    state_manager.unstep(count);
  }

  Schematics::SchematicTop* get_top() {
    return state_manager.state();
  }

  void run   (Schematics::SchematicTop* top, int phase_count, Req req, bool verbose);
  void phase (Schematics::SchematicTop* top, Req req, bool verbose);

  StateManager2<Schematics::SchematicTop> state_manager;

  static int main(int argc, char** argv);

  GridPainter grid_painter;
  TextPainter text_painter;
};