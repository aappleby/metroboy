#pragma once
#include "glm/glm/glm.hpp"

using namespace glm;

//-----------------------------------------------------------------------------

class App {
public:
  virtual ~App() = default;
  virtual const char* app_get_title() = 0;
  virtual void app_init(int screen_w, int screen_h) = 0;
  virtual void app_close() = 0;
  virtual bool pause_when_idle() { return true; }

  virtual void app_update(dvec2 screen_size, double delta) = 0;
  virtual void app_render_frame(dvec2 screen_size, double delta) = 0;
  virtual void app_render_ui(dvec2 screen_size, double delta) = 0;
};

//-----------------------------------------------------------------------------