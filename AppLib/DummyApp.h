#pragma once

#include "AppLib/App.h"
#include "AppLib/GridPainter.h"
#include "AppLib/TextPainter.h"
#include "AppLib/Blitter.h"
#include "AppLib/Viewport.h"

//-----------------------------------------------------------------------------

class DummyApp : public App {
public:
  ~DummyApp() override;

  const char* app_get_title() override;
  void app_init(int screen_w, int screen_h) override;
  void app_close() override;
  void app_update(double delta) override;
  void app_render_frame() override;
  void app_render_ui() override;

  GridPainter grid_painter;
  TextPainter text_painter;
  Blitter     blitter;
  ViewController view_control;

  int screen_w = 0;
  int screen_h = 0;
  int tex = 0;
};

//-----------------------------------------------------------------------------