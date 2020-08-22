#pragma once

#include "AppLib/Viewport.h"

//-----------------------------------------------------------------------------

class App {
public:
  virtual ~App() = default;
  virtual const char* app_get_title() = 0;
  virtual void app_init() = 0;
  virtual void app_close() = 0;
  virtual void app_update(double delta) = 0;
  virtual void app_render_frame(Viewport view) = 0;
  virtual void app_render_ui(Viewport view) = 0;
};

//-----------------------------------------------------------------------------