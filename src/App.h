#pragma once

#include "Viewport.h"

//-----------------------------------------------------------------------------

class App {
public:
  virtual ~App() = default;
  virtual const char* get_title() = 0;
  virtual void init() = 0;
  virtual void close() = 0;
  virtual void update(double delta) = 0;
  virtual void render_frame(Viewport view) = 0;
  virtual void render_ui(Viewport view) = 0;
};

//-----------------------------------------------------------------------------