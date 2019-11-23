#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string>
#include <vector>

#include "TraceViewer.h"
#include "../src/TextPainter.h"
#include "pages/Gameboy.h"

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

void System_tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

void step_forwards(Gameboy& gbIn, Gameboy& gbOut) {
  //----------
  // old state

  Gameboy a = gbIn;

  //----------
  // old state + new inputs

  Gameboy b = a;
  b.timestamp = a.timestamp + 1;

  b.cpu.CPU_RAW_RD = false;
  b.cpu.CPU_RAW_WR = false;
  b.cpu.ADDR_VALID = true;

  b.cpu.FROM_CPU5 = false;
  b.cpu.CPUCLK_REQ = true;

  b.chip.RST     = false;
  b.chip.CLKIN_A = true;
  b.chip.CLKIN_B = b.timestamp & 1;
  b.chip.T1 = false;
  b.chip.T2 = false;

  //----------
  // unmerged signals

  //----------
  // destination state
  
  Gameboy c;

  Gameboy* pa = &a;
  Gameboy* pb = &b;
  Gameboy* pc = &c;

  for (int rep = 0; rep < 40; rep++) {
    pc->timestamp = pb->timestamp;
    pc->cpu = pb->cpu;
    pc->chip = pb->chip;

    System_tick(*pa, *pb, *pc);
    //P09_ApuControl_tick(*pa, *pb, *pc);

    if (memcmp(pb, pc, sizeof(Gameboy)) == 0) {
      printf("%d\n", rep);
      break;
    }

    Gameboy* pt = pa; pa = pb; pb = pc; pc = pt;
  }

  gbOut = *pb;
}

#if 0
const std::vector<SignalData> P01_ClocksReset::signals() {
  return
  {
    SignalData("-----Clocks-----"),
    SignalData("PHASE_ABCD", offsetof(P01_ClocksReset, CLK_ABCDxxxx1)),
    SignalData("PHASE_BCDE", offsetof(P01_ClocksReset, CLK_xBCDExxx1)),
    SignalData("PHASE_CDEF", offsetof(P01_ClocksReset, CLK_xxCDEFxx1)),
    SignalData("PHASE_DEFG", offsetof(P01_ClocksReset, CLK_xxxDEFGx1)),
    SignalData("COKE", offsetof(P01_ClocksReset, COKE)),

    /*
    SignalData("-----DIV-----"),
    SignalData("DIV_00",    offsetof(P01_ClocksReset, DIV_00)),
    SignalData("DIV_01",    offsetof(P01_ClocksReset, DIV_01)),
    SignalData("DIV_02",    offsetof(P01_ClocksReset, DIV_02)),
    SignalData("DIV_03",    offsetof(P01_ClocksReset, DIV_03)),
    SignalData("DIV_04",    offsetof(P01_ClocksReset, DIV_04)),
    SignalData("DIV_05",    offsetof(P01_ClocksReset, DIV_05)),
    SignalData("DIV_06",    offsetof(P01_ClocksReset, DIV_06)),
    SignalData("DIV_07",    offsetof(P01_ClocksReset, DIV_07)),
    SignalData("DIV_08",    offsetof(P01_ClocksReset, DIV_08)),
    SignalData("DIV_09",    offsetof(P01_ClocksReset, DIV_09)),
    SignalData("DIV_10",    offsetof(P01_ClocksReset, DIV_10)),
    SignalData("DIV_11",    offsetof(P01_ClocksReset, DIV_11)),
    SignalData("DIV_12",    offsetof(P01_ClocksReset, DIV_12)),
    SignalData("DIV_13",    offsetof(P01_ClocksReset, DIV_13)),
    SignalData("DIV_14",    offsetof(P01_ClocksReset, DIV_14)),
    SignalData("DIV_15",    offsetof(P01_ClocksReset, DIV_15)),
    */

    /*
    SignalData("----------"),
    SignalData("BARA",       offsetof(P01_ClocksReset, BARA)),
    SignalData("CARU",       offsetof(P01_ClocksReset, CARU)),
    SignalData("BYLU",       offsetof(P01_ClocksReset, BYLU)),

    SignalData("----------"),
    SignalData("ATYK",       offsetof(P01_ClocksReset, ATYK)),
    SignalData("AVOK",       offsetof(P01_ClocksReset, AVOK)),
    SignalData("JESO",       offsetof(P01_ClocksReset, JESO)),
    */
  };
}
#endif

#if 0
const std::vector<SignalData> P03_Timer::signals() {
  return
  {
    SignalData("-----TAC-----"),
    SignalData("TAC_0", offsetof(P03_Timer, TAC_0)),
    SignalData("TAC_1", offsetof(P03_Timer, TAC_1)),
    SignalData("TAC_2", offsetof(P03_Timer, TAC_2)),

    SignalData("-----TMA-----"),
    SignalData("TMA_0", offsetof(P03_Timer, TMA_0)),
    SignalData("TMA_1", offsetof(P03_Timer, TMA_1)),
    SignalData("TMA_2", offsetof(P03_Timer, TMA_2)),
    SignalData("TMA_3", offsetof(P03_Timer, TMA_3)),
    SignalData("TMA_4", offsetof(P03_Timer, TMA_4)),
    SignalData("TMA_5", offsetof(P03_Timer, TMA_5)),
    SignalData("TMA_6", offsetof(P03_Timer, TMA_6)),
    SignalData("TMA_7", offsetof(P03_Timer, TMA_7)),

    SignalData("-----TIMA-----"),
    SignalData("TIMA_0", offsetof(P03_Timer, TIMA_0)),
    SignalData("TIMA_1", offsetof(P03_Timer, TIMA_1)),
    SignalData("TIMA_2", offsetof(P03_Timer, TIMA_2)),
    SignalData("TIMA_3", offsetof(P03_Timer, TIMA_3)),
    SignalData("TIMA_4", offsetof(P03_Timer, TIMA_4)),
    SignalData("TIMA_5", offsetof(P03_Timer, TIMA_5)),
    SignalData("TIMA_6", offsetof(P03_Timer, TIMA_6)),
    SignalData("TIMA_7", offsetof(P03_Timer, TIMA_7)),

    SignalData("-----Int-----"),
    SignalData("NYDU",   offsetof(P03_Timer, NYDU)),
    SignalData("INT_TIMER",   offsetof(P03_Timer, INT_TIMER)),

    SignalData("-----Clock mux-----"),
    SignalData("UVYR",   offsetof(P03_Timer, UVYR)),
    SignalData("UKAP",   offsetof(P03_Timer, UKAP)),
    SignalData("UBOT",   offsetof(P03_Timer, UBOT)),
    SignalData("TEKO",   offsetof(P03_Timer, TEKO)),
    SignalData("TECY",   offsetof(P03_Timer, TECY)),
    SignalData("SOGU",   offsetof(P03_Timer, SOGU)),
  };
}
#endif

const std::vector<SignalData> sample_signals =
{
  SignalData("RESET",    offsetof(Gameboy, chip.RST)),
  SignalData("CLKIN_B",  offsetof(Gameboy, chip.CLKIN_B)),
  SignalData("AJER_2M",  offsetof(Gameboy, sys.AJER_2M)),
};

//-----------------------------------------------------------------------------

#if 0
int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  printf("Hello World Again\n");

  return 0;
}
#endif

//-----------------------------------------------------------------------------

#if 1
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

  const int timer_count = 1024;
  Gameboy* samples = new Gameboy[timer_count];
  memset(samples, 0xCD, timer_count * sizeof(Gameboy));

  Gameboy reset_sample = {};
  reset_sample.timestamp = -1;

  uint64_t timeA = SDL_GetPerformanceCounter();


  step_forwards(reset_sample, samples[0]);

  for (int i = 1; i < timer_count; i++) {
    samples[i] = samples[i-1];
    step_forwards(samples[i-1], samples[i]);
  }

  uint64_t timeB = SDL_GetPerformanceCounter();

  printf("%f\n", double(timeB - timeA) / double(SDL_GetPerformanceFrequency()));

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
        Gameboy sampleA = samples[cursor-1];
        Gameboy sampleB = samples[cursor];

        Gameboy temp = {};
        step_forwards(sampleA, temp);
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

    tv.render(samples, sizeof(Gameboy), timer_count, sample_signals, center, span);

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
