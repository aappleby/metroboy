#pragma once
#include "Platform.h"

struct Gameboy;

class MetroBoyApp {
public:

  int main_(int argc, char** argv);

  void render_text(uint32_t* framebuffer, int dst_x, int dst_y, uint8_t* font, const char* text);
  void draw_bbox(int sx, int sy, int w, int h, uint32_t* buf);
  void render_console(uint32_t* framebuffer, int sx, int sy, uint8_t* font);
  void printf_console(const char* format, ...);

  const int fb_width = 1024;
  const int fb_height = 1024;

  const int gb_width = 160 * 2;
  const int gb_height = 144 * 2;

  const int glyph_width = 6;
  const int glyph_height = 12;
  const int glyph_stride = 256;

  const int console_width = 80;
  const int console_height = 10;

  char console_buf[80 * 10];

  int cursor_x = 0;
  int cursor_y = 0;

  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* fb_tex;
  SDL_Surface* terminus_surface;
  uint8_t* terminus_font;
  const uint8_t* keyboard_state;
};
