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

void dump(void* blob, int size) {
  uint8_t* src = (uint8_t*)blob;
  for (int i = 0; i < size; i++) {
    printf("%c", src[i] ? '+' : '-');
  }
  printf("\n");
}

//-----------------------------------------------------------------------------

/*
struct ClocksIn {
  bool CLKIN_A;
  bool CLKIN_B;
  bool MODE_PROD;
  bool CPUCLK_REQ;
};
*/

void step(int phase) {
  (void)phase;
  //Clocks clocks;
}




//-----------------------------------------------------------------------------



#if 0
int step(LcdIn& in, LCD& a, LCD& b, LCD& next) {
  for (int prop = 0; prop < 2000; prop++) {
    LCD_tick(in, a, b, next);
    if (memcmp(&b, &next, sizeof(LCD)) == 0) return prop;
    a = b;
    b = next;

  }
  return 20;
}

void LCD_test() {
  LcdIn in = {
    .ROOTCLK_xBxDxFxH = 0,
    .VID_RESETn = 0,
    .LCDC_EN = 1,
    .DIV_06n = 0,
    .DIV_07n = 0
  };

  LCD a = {};
  LCD b = {};
  LCD next = {};

  int64_t phase = 0;

  int delay = 4001;

  for (int i = 0; i < delay - 100; i++) {
    in.VID_RESETn = phase > delay;
    in.ROOTCLK_xBxDxFxH = phase & 1;
    step(in, a, b, next);
    phase++;
  }


  for (int i = 0; i < 200; i++) {
    in.VID_RESETn = phase > delay;
    in.ROOTCLK_xBxDxFxH = phase & 1;
    step(in, a, b, next);
    printf("%lld %c: %d%d%d%d %d %d%d%d%d %3d %3d %d %d %d %d\n",
           phase,
           char('A' + (phase % 8)),
           b.PHASE_ABCDxxxx,
           b.PHASE_xBCDExxx,
           b.PHASE_xxCDEFxx,
           b.PHASE_xxxDEFGx,
           b.XOTA_xBxDxFxH,
           b.WUVU_AxxDExxH,
           b.VENA_AxxxxFGH,
           b.TALU_AxxxxFGH,
           b.SONO_xBCDExxx,
           b.CNT,
           b.V,
           b.LINE_ENDo,
           b.LINE_ENDp,
           b.LINE_153_SYNC,
           b.REG_VBLANK);
    phase++;

    if ((phase % 8) == 0) printf("\n");
  }
}

//-----------------------------------------------------------------------------

};

int main(int /*argc*/, char** /*argv*/) {
  Schematics::LCD_test();
}
#endif
















#if 0

namespace Schematics {
  void System_tick(const Gameboy& a, const Gameboy& b, Gameboy& c);
  void P21_VideoControl_tick(const Gameboy& a, const Gameboy& b, Gameboy& c);
};

void step_forwards(Schematics::Gameboy& gbIn, Schematics::Gameboy& gbOut) {
  //----------
  // old state

  int64_t old_timestamp = gbIn.timestamp;
  Schematics::Gameboy a = gbIn;

  //----------
  // old state + new inputs

  Schematics::Gameboy b = a;

  /*
  Schematics::CpuIn cpu_in = {};

  cpu_in.CPU_RAW_RD = true;
  cpu_in.CPU_RAW_WR = false;
  cpu_in.ADDR_VALID = true;
  cpu_in.FROM_CPU5 = false;
  cpu_in.CPUCLK_REQ = true;
  */

  /*
  Schematics::ChipIn chip_in = {};

  chip_in.RST     = false;
  chip_in.CLKIN_A = true;
  chip_in.CLKIN_B = (old_timestamp + 1) & 1;
  chip_in.T1 = false;
  chip_in.T2 = false;
  */

  //----------
  // unmerged signals

  //----------
  // destination state
  
  Schematics::Gameboy c;

  Schematics::Gameboy* pa = &a;
  Schematics::Gameboy* pb = &b;
  Schematics::Gameboy* pc = &c;

  for (int rep = 0; rep < 40; rep++) {
    pa->A = 0xA000;
    pb->A = 0xA000;
    pc->A = 0xA000;

    System_tick(*pa, *pb, *pc);
    //P21_VideoControl_tick(cpu_in, chip_in, *pa, *pb, *pc);

    if (memcmp(pb, pc, sizeof(Schematics::Gameboy)) == 0) {
      //printf("%d %d\n", old_timestamp, rep);
      break;
    }

    Schematics::Gameboy* pt = pa; pa = pb; pb = pc; pc = pt;
  }

  gbOut = *pb;
  gbOut.timestamp = old_timestamp + 1;
}
#endif

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
