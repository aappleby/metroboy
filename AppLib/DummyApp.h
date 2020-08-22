#pragma once

#include "AppLib/App.h"
#include "AppLib/GridPainter.h"
#include "AppLib/TextPainter.h"
#include "AppLib/Blitter.h"

//-----------------------------------------------------------------------------

class DummyApp : public App {
public:
  ~DummyApp() override;

  const char* app_get_title() override;
  void app_init() override;
  void app_close() override;
  void app_update(double delta) override;
  void app_render_frame(Viewport view) override;
  void app_render_ui(Viewport view) override;

  GridPainter grid_painter;
  TextPainter text_painter;
  Blitter     blitter;
  int tex = 0;
};

//-----------------------------------------------------------------------------