#pragma once
#include "MetroBoy.h"

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

struct Gameboy;

class MetroBoyApp {
public:

  MetroBoyApp() {}

  int main_(int argc, char** argv);

  void render_text(int dst_x, int dst_y, const char* text);
  void draw_bbox(int sx, int sy, int w, int h, uint32_t color);
  void render_console(int sx, int sy, uint8_t* font);
  void printf_console(const char* format, ...);
  
  void load(const std::string& prefix, const std::string& name);
  void load(const std::string& name) { load("./", name); }

  void loop();

  MetroBoy metroboy;
  bool rom_loaded = false;
  const char* filename = nullptr;
  std::string text_buf;
  uint64_t frame_begin, frame_end, frame_time = 0;
  double fast_cycles = 114 * 154;

  uint32_t* background = nullptr;
  uint32_t* framebuffer = nullptr;
  
  int pitch = 0;

  uint64_t freq;

  enum RunMode {
    RUN_FAST,
    RUN_VSYNC,
    STEP_FRAME,
    STEP_LINE,
    STEP_CYCLE
  };
  RunMode runmode = STEP_CYCLE;
  bool quit = false;
  int step_forward = 0;
  int step_backward = 0;
  bool step_up = false;
  bool step_down = false;
  bool load_dump = false;
  bool save_dump = false;
  bool reset = false;

  int frame_count = 0;
  uint8_t buttons = 0;
  int overlay_mode = 0;

  const int fb_width = 1900;
  const int fb_height = 1000;

  const int gb_width = 160 * 2;
  const int gb_height = 144 * 2;

  const int glyph_width = 6;
  const int glyph_height = 12;
  const int glyph_stride = 256;

  static const int console_width = 80;
  static const int console_height = 10;

  char console_buf[console_width * console_height];

  int cursor_x = 0;
  int cursor_y = 0;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Texture* fb_tex = nullptr;
  SDL_Surface* terminus_surface = nullptr;
  uint8_t* terminus_font = nullptr;
  const uint8_t* keyboard_state = nullptr;

  uint8_t golden[160 * 144];
};
