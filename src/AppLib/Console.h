#pragma once
#include <stdint.h>

struct Console {
  void init();
  void begin_frame();
  void end_frame();

  void plot(int x, int y, int color, char c);
  void print_at(int x, int y, int color, const char* format, ...);

  short screen_w = 0;
  short screen_h = 0;
  void* hin = nullptr;
  void* hout = nullptr;
  unsigned int* back = nullptr;
  int frame = 0;
  int mouse_x = 0;
  int mouse_y = 0;
};