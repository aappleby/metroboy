#pragma once

#include "App.h"
#include "GridPainter.h"
#include "TextPainter.h"
#include "Blitter.h"

//-----------------------------------------------------------------------------

class DummyApp : public App {
public:
  ~DummyApp() override;
  const char* get_title() override;

  void init() override;
  void close() override;
  void update(double delta) override;
  void render_frame(Viewport view) override;
  void render_ui(Viewport view) override;

  GridPainter grid_painter;
  TextPainter text_painter;
  Blitter     blitter;
  int tex = 0;
};

//-----------------------------------------------------------------------------