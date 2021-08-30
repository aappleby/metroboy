#pragma once
#include <stdint.h>

#if 0
#include "CoreLib/SignalTree.h"

class TraceViewer {
public:

  uint32_t* framebuffer;
  int fb_width;
  int fb_height;
 

  int screen_x;
  int screen_y;

  void render(void* blob, int count, int size, const std::vector<SignalData>& signals, double center, double span);

  int render_signals(const std::vector<SignalData>& signals, void* blob, int x, int y);
};
#endif