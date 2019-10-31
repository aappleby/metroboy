#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string>
#include <vector>

#include "Timer.h"
#include "TraceViewer.h"
#include "../src/TextPainter.h"
#include "pages/P01_ClocksReset.h"
#include "pages/P03_Timer.h"

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
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

#if 0
struct Pages {
  int64_t timestamp;
  P01_ClocksReset page01;
};

const std::vector<SignalData> P01_ClocksReset::signals =
{
  SignalData("timestamp", offsetof(Pages, timestamp), 0, 1),
  SignalData("page01",    offsetof(Pages, page01),    P01_ClocksReset::signals),
};
#endif

//-----------------------------------------------------------------------------

#if 0
struct Sample {
  int64_t timestamp;
  Timer timer;
};

void step_forwards(void* blob) {
  Sample* sample = (Sample*)blob;

  Timer::Input in = {};

  in.CPU_RD    = true;
  in.RESET     = false;
  in.CLK_GOOD  = true;
  in.CLKIN_B   = (sample->timestamp & 1);
  in.CPU_RESET = false;
  in.FROM_CPU3 = true;
  in.FROM_CPU4 = false;
  in.CPU_WR    = false;

  sample->timer.tock1(in);
  sample->timestamp++;
}

const std::vector<SignalData> wrapper_signals =
{
  SignalData("timer1------------------------------", offsetof(Sample, timer), Timer::signals1()),
  SignalData("timer2------------------------------", offsetof(Sample, timer), Timer::signals2()),
};
#endif


struct Sample {
  int64_t timestamp;
  P01_ClocksReset page01;
  P03_Timer       page03;
};

void step_forwards(void* blobA, void* blobB) {
  Sample* sampleA = (Sample*)blobA;
  Sample* sampleB = (Sample*)blobB;

  sampleB->timestamp             = sampleA->timestamp + 1;
  sampleB->page01.in_CPU_RD      = false;
  sampleB->page01.in_CPU_WR      = false;
  //sampleB->page01.in_RESET       = sampleB->timestamp < 17;
  sampleB->page01.in_RESET       = false;
  sampleB->page01.in_CLKIN_A     = true;
  sampleB->page01.in_CLKIN_B     = (sampleB->timestamp & 1);
  sampleB->page01.in_ABOL        = false;
  //sampleB->page01.in_AJER_2M   = false;
  sampleB->page01.in_FROM_CPU3   = true;
  sampleB->page01.in_FROM_CPU4   = true;
  sampleB->page01.in_APU_RESET   = false;
  sampleB->page01.in_APU_RESET5n = true;
  sampleB->page01.in_FERO_Q      = false;
  sampleB->page01.in_FF04_FF07   = false;
  sampleB->page01.in_TOLA_A1n    = false;
  sampleB->page01.in_TOVY_A0n    = false;
  sampleB->page01.in_T1T2n       = false;
  sampleB->page01.in_T1nT2       = false;
  sampleB->page01.in_T1nT2n      = true;
  sampleB->page01.in_FF40_D7     = true;
  sampleB->page01.in_FF60_D1     = false;

  sampleB->page01.tick(sampleA->page01);

  /*
  struct Input {
  bool RESET2;

  bool BOGA_1M;
  bool CLK_256k;
  bool CLK_64k;
  bool CLK_16k;

  bool CPU_RD;
  bool CPU_WR;

  bool FROM_CPU5;

  bool A00_07;
  bool FFXX;
  bool TOLA_A1n;
  bool FF04_D1n;

  bool INT_TIMER;

  bool A0,A1,A2,A3,A4,A5,A6,A7;
  bool D0,D1,D2,D3,D4,D5,D6,D7;

  };
  */

  sampleB->page03.in.RESET2 = sampleB->page01.out_RESET2;

  sampleB->page03.in.BOGA_1M  = sampleB->page01.out_BOGA_1M;
  sampleB->page03.in.CLK_256K = sampleB->page01.out_CLK_256K;
  sampleB->page03.in.CLK_64K  = sampleB->page01.out_CLK_64K;
  sampleB->page03.in.CLK_16K  = sampleB->page01.out_CLK_16K;
  sampleB->page03.in.FF04_D1n = sampleB->page01.out_FF04_D1n; // this is the 4k clock

  sampleB->page03.in.CPU_RD = false;
  sampleB->page03.in.CPU_WR = false;

  sampleB->page03.in.FROM_CPU5 = false;
  sampleB->page03.in.A00_07 = false;
  sampleB->page03.in.FFXX = false;
  sampleB->page03.in.TOLA_A1n = false;

  sampleB->page03.in.A0 = 0;
  sampleB->page03.in.A1 = 0;
  sampleB->page03.in.A2 = 0;
  sampleB->page03.in.A3 = 0;
  sampleB->page03.in.A4 = 0;
  sampleB->page03.in.A5 = 0;
  sampleB->page03.in.A6 = 0;
  sampleB->page03.in.A7 = 0;

  sampleB->page03.in.D0 = 0;
  sampleB->page03.in.D1 = 0;
  sampleB->page03.in.D2 = 0;
  sampleB->page03.in.D3 = 0;
  sampleB->page03.in.D4 = 0;
  sampleB->page03.in.D5 = 0;
  sampleB->page03.in.D6 = 0;
  sampleB->page03.in.D7 = 0;

  // probably need to fix this
  sampleB->page03.in.INT_TIMER = sampleA->page03.out.INT_TIMER;

  sampleB->page03.tick(sampleA->page03);
}

const std::vector<SignalData> sample_signals =
{
  SignalData("stamp0", offsetof(Sample, timestamp), 0, 1),
  SignalData("stamp1", offsetof(Sample, timestamp), 1, 1),
  SignalData("stamp2", offsetof(Sample, timestamp), 2, 1),
  SignalData("page01", offsetof(Sample, page01), P01_ClocksReset::signals()),
  SignalData("page03", offsetof(Sample, page03), P03_Timer::signals()),
};

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

int main(int /*argc*/, char** /*argv*/) {
  const int fb_width = 1900;
  const int fb_height = 1000;
  const int gb_width = 160 * 2;
  const int gb_height = 144 * 2;

  SDL_Window* window = SDL_CreateWindow("MetroBoy Trace Debugger", 100, 100, fb_width, fb_height, SDL_WINDOW_SHOWN);
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

  const int timer_count = 1024 * 1024;
  Sample* samples = new Sample[timer_count];
  memset(samples, 0xCD, timer_count * sizeof(Sample));

  Sample reset_sample = {};
  reset_sample.timestamp = -1;

  //bool SABO,SAMY,SOPU;

  reset_sample.page03.SOPU_0 = true;
  reset_sample.page03.SAMY_1 = true;
  reset_sample.page03.SABO_2 = true;

  uint64_t timeA = SDL_GetPerformanceCounter();


  step_forwards(&reset_sample, &samples[0]);
  for (int i = 1; i < timer_count; i++) {
    samples[i] = samples[i-1];
    step_forwards(&samples[i-1], &samples[i]);
  }

  uint64_t timeB = SDL_GetPerformanceCounter();

  printf("%f\n", double(timeB - timeA) / double(SDL_GetPerformanceFrequency()));

  //----------

  int64_t cycle_cursor = 0;
  uint64_t frame_begin, frame_end, frame_time = 0;
  uint64_t freq = SDL_GetPerformanceFrequency();

  //double center = 2500.0;
  //int zoom = 0;
  double center = 130822;
  int zoom = -40;

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
          if (center > timer_count) center = timer_count;
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

    if (debug_cycle) {
      int cursor = (int)floor(center);
      if (cursor > 0) {
        printf("debugging sample %d\n", cursor);
        Sample sampleA = samples[cursor-1];
        Sample sampleB = samples[cursor];

        Sample temp = {};
        step_forwards(&sampleA, &temp);
      }

      //Sample sampleA = 
    }

    //----------------------------------------
    // Render trace

    cycle_cursor += step_count;
    step_count = 0;

    TraceViewer tv;

    tv.framebuffer = framebuffer;
    tv.fb_width = fb_width;
    tv.fb_height = fb_height;

    tv.screen_x = 200;
    tv.screen_y = 16;

    render_labels(tp, 100, 16, sample_signals, 0);

    tv.render(samples, sizeof(Sample), timer_count, sample_signals, center, span);

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
