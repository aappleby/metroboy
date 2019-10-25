#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string>

#include "Timer.h"
#include "../src/TextPainter.h"

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* fb_tex = nullptr;
SDL_Surface* terminus_surface = nullptr;
uint8_t* terminus_font = nullptr;
const uint8_t* keyboard_state = nullptr;
uint64_t freq;

const int fb_width = 1900;
const int fb_height = 1000;

const int gb_width = 160 * 2;
const int gb_height = 144 * 2;

const int glyph_width = 6;
const int glyph_height = 12;
const int glyph_stride = 256;

static const int console_width = 80;
static const int console_height = 10;
bool quit = false;
int pitch = 0;
uint32_t* background = nullptr;
uint32_t* framebuffer = nullptr;
uint64_t frame_begin, frame_end, frame_time = 0;
int frame_count = 0;
TextPainter tp;

void loop();

int main(int /*argc*/, char** /*argv*/) {
  window = SDL_CreateWindow("MetroBoy Trace Debugger", 100, 100, fb_width, fb_height, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  fb_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, fb_width, fb_height);
  terminus_surface = SDL_LoadBMP("terminus2.bmp");
  terminus_font = (uint8_t*)terminus_surface->pixels;
  keyboard_state = SDL_GetKeyboardState(nullptr);
  freq = SDL_GetPerformanceFrequency();

  background = new uint32_t[fb_width * fb_height];
  //framebuffer = new uint32_t[fb_width * fb_height];

  for (int y = 0; y < fb_height; y++) {
    for (int x = 0; x < fb_width; x++) {
      int c = ((x ^ y) & 0x20) ? 0x10101010 : 0x15151515;
      background[x + y * fb_width] = c;
    }
  }

  tp.glyph_width = glyph_width;
  tp.glyph_height = glyph_height;
  tp.glyph_stride = glyph_stride;
  tp.fb_width = fb_width;
  tp.fb_height = fb_height;
  tp.framebuffer = framebuffer;
  tp.font = terminus_font;

  while (!quit) loop();

  return 0;
}

int highlight_row = 0;
int highlight_col = 7;

template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}

void loop() {
  frame_begin = SDL_GetPerformanceCounter();

  //----------------------------------------
  // Process events

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_RIGHT:  {
        if (highlight_col < 200) highlight_col++;
        break;
      }
      case SDLK_LEFT:   {
        if (highlight_col > 0) highlight_col--;
        break;
      }
      case SDLK_UP: {
        if (highlight_row > 0) highlight_row--;
        break;
      }
      case SDLK_DOWN: {
        if (highlight_row < 20) highlight_row++;
        break;
      }
      }
    }
    if (event.type == SDL_QUIT) quit = true;
  }

  //----------------------------------------
  // Clear screen

  SDL_LockTexture(fb_tex, NULL, (void**)(&framebuffer), &pitch);
  memcpy(framebuffer, background, fb_width * fb_height * 4);
  tp.framebuffer = framebuffer;

  //----------------------------------------
  // Run sim

  tp.frame_count = frame_count;

  Timer timer;

  for (int i = 0; i < 256; i++) {
    tp.clock = i;
    tp.trace_x = 100;
    tp.trace_y = 32;

    tp.row = 0;
    tp.col = i;
    tp.highlight_row = highlight_row;
    tp.highlight_col = highlight_col;

    Timer::Input in = {};

    /*
    // physical pins
    bool CPU_RD;
    bool CPU_WR;
    bool RESET;       // active high!
    bool CLKIN_A;     // this is actually CLOCK_GOOD or similar
    bool CLKIN_B;     // the master 4mhz clock

    bool ABOL_1MHZ;   // this is actually CPU_RESET or similar
    bool AJER_2MHZ;   // on apu sheet
    bool FROM_CPU3;
    bool FROM_CPU4;
    bool APU_RESET;
    bool APU_RESET5n;
    bool FERO_Q;      // something debug-related
    bool FF04_FF07;
    bool TOLA_A1n;
    bool TOVY_A0n;
    bool T1T2n;       // true for debugging
    bool T1nT2;       // true for debugging
    bool T1nT2n;      // true for normal mode
    bool FF40_D7;     // lcd on
    bool FF60_D1;     // debugging
    */

    in.CPU_RD = true;
    in.RESET = false;
    in.CLKIN_A = true;
    in.CLKIN_B = i & 1;

    in.ABOL_1MHZ = false;
    in.AJER_2MHZ = false;
    in.FROM_CPU3 = true;
    in.FROM_CPU4 = false;
    in.APU_RESET = false;
    in.APU_RESET5n = false;
    in.FERO_Q = false;

    in.CPU_WR = false;
    in.FF04_FF07 = true;
    in.TOLA_A1n = true;
    in.TOVY_A0n = true;

    in.T1nT2 = false;
    in.T1T2n = false;
    in.T1nT2n = true;
    in.FF40_D7 = false;
    in.FF60_D1 = false;

    timer.tock(in, tp);
  }

  std::string temp;
  sprintf(temp, "Cycle %d", highlight_col);
  tp.render_text(3,3, temp.c_str());

  //----------------------------------------
  // Swap

  SDL_UnlockTexture(fb_tex);
  framebuffer = nullptr;
  SDL_RenderCopy(renderer, fb_tex, NULL, NULL);

  frame_end = SDL_GetPerformanceCounter();
  frame_time = frame_end - frame_begin;

  SDL_RenderPresent(renderer);
  frame_count++;
}
