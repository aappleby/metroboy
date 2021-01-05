#pragma once

#include "AppLib/Viewport.h"

//-----------------------------------------------------------------------------

class App {
public:
  virtual ~App() = default;
  virtual const char* app_get_title() = 0;
  virtual void app_init(int screen_w, int screen_h) = 0;
  virtual void app_close() = 0;

  virtual void begin_frame(int /*screen_w*/, int /*screen_h*/) {};
  virtual void app_update(double delta) = 0;
  virtual void app_render_frame() = 0;
  virtual void app_render_ui() = 0;
  virtual void end_frame() {};
};

//-----------------------------------------------------------------------------