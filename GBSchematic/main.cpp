#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string>
#include <vector>

#include "TraceViewer.h"
#include "../src/TextPainter.h"
#include "pages/Sch_Gameboy.h"

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

using namespace Schematics;

void TestLCD();
void TestClocks();
void TestSpriteSeq();


struct StructTest {
  int a;
  int b;
  int c;
};


#include <math.h>

int main(int /*argc*/, char** /*argv*/) {
  TestClocks();
  TestLCD();
  //TestSpriteSeq();

#if 0
  for(int i = 0; i < 8; i++) {
    double t2 = double(i) / 8.0;
    double t1 = t2 - 1.0/3.0;
    double t3 = t2 + 1.0/3.0;

    double s1 = cos(t1 * 2.0 * 3.14159265358979) * 0.5 + 0.5;
    double s2 = cos(t2 * 2.0 * 3.14159265358979) * 0.5 + 0.5;
    double s3 = cos(t3 * 2.0 * 3.14159265358979) * 0.5 + 0.5;

    int c1 = int(s1 * (255-64) + 64);
    int c2 = int(s2 * (255-64) + 64);
    int c3 = int(s3 * (255-64) + 64);

    printf("0xFF%02x%02x%02x\n", c1, c2, c3);
  }
#endif

  return 0;
}

#if 0
uint32_t phase_to_color[8] = {
  0xFF808080,
  0xFFB0B070,
  0xFFFF8080,
  0xFFFFC080,

  0xFFFFFF80,
  0xFF80FF80,
  0xFF8080FF,
  0xFFD080D0,

  /*
  0xFF6fff6f,
  0xFFb8e343,
  0xFFf29f4c,
  0xFFfb5b86,
  0xFFcf40cf,
  0xFF865bfb,
  0xFF4c9ff2,
  0xFF43e3b8,
  */
};


  void sdl_run() {

    TestGB gb;
    gb.reset();

    const int fb_width = 1900;
    const int fb_height = 1000;
    int scale = 2;

    SDL_Window* window = SDL_CreateWindow("MetroBoy Trace Debugger", 4, 35, fb_width, fb_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* fb_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, fb_width, fb_height);

    uint32_t* background = new uint32_t[fb_width * fb_height];
    for (int y = 0; y < fb_height; y++) {
      for (int x = 0; x < fb_width; x++) {
        int c = ((x ^ y) & 0x20) ? 0x10101010 : 0x15151515;
        background[x + y * fb_width] = c;
        //background[x + y * fb_width] = 0;
      }
    }

    //----------

    int frame = 0;

    bool quit = false;
    while (!quit) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) quit = true;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) quit = true;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) scale++;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) scale--;

        if (scale < 1) scale = 1;
      }

      //----------------------------------------
      // Clear screen

      uint32_t* framebuffer = nullptr;
      int pitch = 0;
      SDL_LockTexture(fb_tex, NULL, (void**)(&framebuffer), &pitch);
      memcpy(framebuffer, background, fb_width * fb_height * 4);

      //----------------------------------------

      for (int y = 0; y < 154; y++) {
        for (int x = 0; x < TCYCLES_LINE * 2; x++) {
          gb.sim_fast(1);
          
          uint32_t color = 0;

          //if (gb1.spr.SCAN_DONE_d0)       color = 0x8080FF;
          //if (spr.SCAN_DONE_d4) color = 0xFF8080;
          //if (spr.SCAN_DONE_d5) color = 0x80FF80;
          //if (gb1.spr.SCAN_DONE_d0_TRIG)  color = 0xFFFFFF;

          //color = (gb.lcd.x() << 8) | (gb.lcd.y() << 0);

          //color = phase_to_color[gb.clk_phase & 7];

          if (scale == 1) {
            int px = x * scale + 10;
            int py = y * scale + 10;
            if (px < fb_width && py < fb_height) {
              framebuffer[px + py * (pitch / 4)] = color;
            }
          }
          else {
            for (int dy = 0; dy < scale - 1; dy++) {
              for (int dx = 0; dx < scale - 1; dx++) {
                int px = x * scale + dx + 10;
                int py = y * scale + dy + 10;
                if (px < fb_width && py < fb_height) {
                  framebuffer[px + py * (pitch / 4)] = color;
                }
              }
            }
          }

        }
      }

      if (frame & 1) {
        for (int y = 0; y < 10; y++) {
          for (int x = 0; x < 10; x++) {
            framebuffer[(x + 0) + (y + 0) * (pitch / 4)] = 0xFFAAAA00;
          }
        }
      }

      //----------------------------------------
      // Swap

      SDL_UnlockTexture(fb_tex);
      SDL_RenderCopy(renderer, fb_tex, NULL, NULL);
      SDL_RenderPresent(renderer);
      frame++;
    }
  }
#endif



//-----------------------------------------------------------------------------

template<typename ... Args>
void sprintf(std::string& out, const char* format, Args ... args)
{
  char source_buf[1024];
  snprintf(source_buf, 1024, format, args ...);
  out.append(source_buf);
}

//-----------------------------------------------------------------------------

int render_labels(TextPainter& tp, int x, int y, const std::vector<SignalData>& signals, int depth) {
  int cx = x;
  int cy = y;

  for (const SignalData& s : signals) {

    if (!s.children.empty()) {
      tp.render_text(cx + 6 * depth, cy, s.name);
      cy += 12;
      cy += render_labels(tp, cx, cy, s.children, depth + 1);
    }
    else {
      tp.render_text(cx + 6 * depth, cy, s.name);
      if (s.bit_width == 0) {
        cy += 12;
      }
      else {
        cy += s.bit_width * 12;
      }
    }

  }

  return cy - y;
}
















//-----------------------------------------------------------------------------













//-----------------------------------------------------------------------------
#if 0

int main(int /*argc*/, char** /*argv*/) {
  printf("hello world\n");

  const int fb_width = 1900;
  const int fb_height = 1000;
  const int gb_width = 160 * 2;
  const int gb_height = 144 * 2;

  SDL_Window* window = SDL_CreateWindow("MetroBoy Trace Debugger", 4, 35, fb_width, fb_height, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_Texture* fb_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, fb_width, fb_height);
  const uint8_t* keyboard_state = SDL_GetKeyboardState(nullptr);

  uint32_t* background = new uint32_t[fb_width * fb_height];
  for (int y = 0; y < fb_height; y++) {
    for (int x = 0; x < fb_width; x++) {
      int c = ((x ^ y) & 0x20) ? 0x10101010 : 0x15151515;
      background[x + y * fb_width] = c;
    }
  }

  TextPainter tp;

  //----------
  // Generate trace

#if 0
  const int timer_count = 1024;
  Schematics::Gameboy* samples = new Schematics::Gameboy[timer_count];
  memset(samples, 0xCD, timer_count * sizeof(Schematics::Gameboy));

  Schematics::Gameboy reset_sample = {};
  reset_sample.timestamp = -1;

  uint64_t timeA = SDL_GetPerformanceCounter();


  step_forwards(reset_sample, samples[0]);

  for (int i = 1; i < timer_count; i++) {
    samples[i] = samples[i-1];
    step_forwards(samples[i-1], samples[i]);
  }

  uint64_t timeB = SDL_GetPerformanceCounter();

  printf("%f\n", double(timeB - timeA) / double(SDL_GetPerformanceFrequency()));
#endif

  //----------

  int64_t cycle_cursor = 0;
  uint64_t frame_begin, frame_end, frame_time = 0;
  uint64_t freq = SDL_GetPerformanceFrequency();

  //double center = 512.0;
  //int zoom = -8;
  //double center = 8191;
  //int zoom = -8;

  double center = 10;
  int zoom = -36;

  bool quit = false;
  while (!quit) {
    frame_begin = SDL_GetPerformanceCounter();

    int step_count = 0;
    int step_size = 1;

    //----------------------------------------
    // Process events

    if (keyboard_state[SDL_SCANCODE_LCTRL]) {
      step_size = 16;
    }

    double span = 4096.0 * pow(2.0, float(zoom) / 4);

    bool debug_cycle = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_RIGHT:  {
          int delta = int(span / 8.0f);
          if (delta == 0) delta = 1;
          center += delta;
          //if (center > timer_count) center = timer_count;
          break;
        }
        case SDLK_LEFT:   {
          int delta = int(span / 8.0f);
          if (delta == 0) delta = 1;
          center -= delta;
          if (center < 0) center = 0;
          break;
        }
        case SDLK_UP:   {
          zoom--;
          if (zoom < -40) zoom = -40;
          break;
        }
        case SDLK_DOWN: {
          zoom++;
          if (zoom > 40) zoom = 40;
          break;
        }
        case SDLK_ESCAPE: {
          quit = true;
          break;
        }
        case SDLK_d: debug_cycle = true; break;
        }
      }
      if (event.type == SDL_QUIT) quit = true;
    }

    span = 4096.0 * pow(2.0, float(zoom) / 4);

    //----------------------------------------
    // Clear screen

    uint32_t* framebuffer = nullptr;
    int pitch = 0;
    SDL_LockTexture(fb_tex, NULL, (void**)(&framebuffer), &pitch);
    memcpy(framebuffer, background, fb_width * fb_height * 4);
    tp.begin_frame(framebuffer, fb_width, fb_height);

    //----------------------------------------
    // Run sim

    /*
    if (center > 0 && center < timer_count) {
      Sample sample = samples[int(center) - 1];
      step_forwards(&sample);
    }
    */

    /*
    if (debug_cycle) {
      int cursor = (int)floor(center);
      if (cursor > 0) {
        printf("debugging sample %d\n", cursor);
        Schematics::Gameboy sampleA = samples[cursor-1];
        Schematics::Gameboy sampleB = samples[cursor];

        Schematics::Gameboy temp = {};
        step_forwards(sampleA, temp);
      }

      //Sample sampleA = 
    }
    */

    //----------------------------------------
    // Render trace

    cycle_cursor += step_count;
    step_count = 0;

    /*
    TraceViewer tv;

    tv.framebuffer = framebuffer;
    tv.fb_width = fb_width;
    tv.fb_height = fb_height;

    tv.screen_x = 200;
    tv.screen_y = 16;

    render_labels(tp, 100, 16, sample_signals, 0);

    tv.render(samples, sizeof(Schematics::Gameboy), timer_count, sample_signals, center, span);
    */

    std::string temp;
    temp.clear();
    sprintf(temp, "Cycle %lld, center %f, zoom %d, span %f", cycle_cursor, center, zoom, span);
    tp.render_text(3,3, temp.c_str());

    //----------------------------------------
    // Render stats

    static double smoothed_frame_time = 0;
    smoothed_frame_time *= 0.98;
    smoothed_frame_time += (1000.0 * double(frame_time) / double(freq)) * 0.02;

    {
      std::string temp2;
      sprintf(temp2, "frame time %2.2f msec, %6d cyc/frame\n", (double)smoothed_frame_time, 0);
      tp.render_text(fb_width - 256, fb_height - 12, temp2.c_str());
    }
  
    //----------------------------------------
    // Swap

    SDL_UnlockTexture(fb_tex);
    SDL_RenderCopy(renderer, fb_tex, NULL, NULL);

    frame_end = SDL_GetPerformanceCounter();
    frame_time = frame_end - frame_begin;

    SDL_RenderPresent(renderer);

  }

  return 0;
}

#endif

//-----------------------------------------------------------------------------
