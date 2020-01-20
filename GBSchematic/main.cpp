#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string>
#include <vector>
#include <math.h>

#include "TraceViewer.h"
#include "../src/TextPainter.h"

#include "tests/TestGB.h"

#ifdef _MSC_VER
#include <include/SDL.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <SDL2/SDL.h>
#endif

using namespace Schematics;

#pragma warning(disable:4189)
#pragma warning(disable:4702)

//-----------------------------------------------------------------------------

const int fb_width = 1888;
const int fb_height = 992;

bool quit = false;
int frame_count = 0;

SDL_Window* window = nullptr;
SDL_GLContext gl_context = nullptr;
const uint8_t* keyboard_state = nullptr;

uint64_t timer_freq, frame_begin, frame_end, frame_time = 0;
uint64_t app_begin = 0;

//-----------------------------------------------------------------------------

void sim_phase(TestGB& gb, bool RST, bool CLKIN_A, bool CLKIN_B, bool CLKREQ) {
  const int pass_count = 20;
  for (int j = 0; j < pass_count; j++) {
    gb.pass_init(RST, CLKIN_A, CLKIN_B);
    gb.cpu_init(CLKREQ);  
    gb.pin_init();
    gb.tick_everything();
    bool changed = gb.commit_everything();
    printf(changed ? "x" : ".");
  }
}

void sim_phase2(TestGB& gb, bool RST, bool CLKIN_A, bool CLKIN_B, bool CLKREQ) {
  const int pass_count = 20;
  for (int j = 0; j < pass_count; j++) {
    gb.pass_init(RST, CLKIN_A, CLKIN_B);
    gb.cpu_init(CLKREQ);  
    gb.pin_init();
    gb.tick_everything();
    gb.commit_everything();
  }
}

int main2(int /*argc*/, char** /*argv*/) {
  //test_clock_phases();
  //test_timer();

  TestGB gb;
  memset(&gb, 0, sizeof(gb));

  uint64_t timeA = SDL_GetPerformanceCounter();

  int phase_counter = 0;

  bool RST = 1;
  bool CLKIN_A = 0;
  bool CLKIN_B = 0;
  bool CLKREQ = 0;

  printf("reset\n");
  RST = 1;
  for (int i = 0; i < 16; i++) {
    CLKIN_B = phase_counter & 1;
    sim_phase(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
    printf("\n");
    phase_counter++;
  }

  printf("!reset\n");
  RST = 0;
  for (int i = 0; i < 16; i++) {
    CLKIN_B = phase_counter & 1;
    sim_phase(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
    printf("\n");
    phase_counter++;
  }

  printf("clkgood\n");
  CLKIN_A = 1;
  for (int i = 0; i < 16; i++) {
    CLKIN_B = phase_counter & 1;
    sim_phase(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
    printf("\n");
    phase_counter++;
  }

  printf("clkreq\n");
  CLKREQ = 1;
  for (int i = 0; i < 16; i++) {
    CLKIN_B = phase_counter & 1;
    sim_phase(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
    printf("\n");
    phase_counter++;
  }

  printf("lcd %d %d\n", gb.lcd_reg.x(), gb.lcd_reg.y());

  for (int i = 0; i < 1024; i++) {
    CLKIN_B = phase_counter & 1;
    sim_phase2(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
    phase_counter++;
  }

  for (int i = 0; i < 1024; i++) {
    CLKIN_B = phase_counter & 1;
    sim_phase2(gb, RST, CLKIN_A, CLKIN_B, CLKREQ);
    phase_counter++;
  }

  printf("lcd %d %d\n", gb.lcd_reg.x(), gb.lcd_reg.y());

  uint64_t timeB = SDL_GetPerformanceCounter();
  //gb.commit_everything();

  printf("end\n");

  double elapsed = double(timeB - timeA) / SDL_GetPerformanceFrequency();

  printf("freq %lld\n", SDL_GetPerformanceFrequency());
  printf("time %lld\n", (timeB - timeA));
  printf("time %f\n", 1000.0 * elapsed);
  printf("freq %f\n", phase_counter / elapsed);

#if 0

  TestGB gb;
  gb.reset();

  //gb.sys_reg.CPU_RAW_RD = 1;

  //gb.sim(456*2*152);

  for (int i = 0; i < 912*2; i++) {
    gb.sim(1);

    ClkSignals clk_sig1 = gb.clk_reg1.signals(gb.sys_reg);
    RstSignals rst_sig1 = gb.rst_reg.rst_signals(gb.sys_reg, clk_sig1);
    VclkSignals vid_clk_sig = gb.vclk_reg.signals();
    LcdSignals lcd_sig = gb.lcd_reg.signals(vid_rst);
    BusSignals bus_ctl = BusSignals::tick(gb.sys_reg, clk_sig1);

    Decoder dcd = Decoder::tick(gb.bus, clk_sig1, gb.sys_reg.BOOT_BIT, gb.sys_reg.MODE_DBG2, gb.sys_reg.ADDR_VALID);


    printf("%03d:%03d:%03d %03d %d %03d %03d %d%d%d%d%d%d %d%d%d%d%d%d %d%d%d%d\n",
           gb.sys_reg.phase_count / (912*154),
           (gb.sys_reg.phase_count / 912) % 154,
           gb.sys_reg.phase_count % 912,
           gb.sys_reg.phase(),
           vid_clk_sig.TALU_xBCDExxx,
           gb.lcd_reg.x(),
           gb.lcd_reg.y(),
           (bool)gb.lcd_reg.NEW_LINE_d0a,
           (bool)gb.lcd_reg.VID_LINE_d4,
           (bool)gb.lcd_reg.NEW_LINE_d4a,
           (bool)gb.lcd_reg.VID_LINE_d6,
           (bool)gb.lcd_reg.LINE_153_d4,
           (bool)gb.lcd_reg.VBLANK_d4,
           lcd_sig.VBLANK_d4b,
           lcd_sig.VID_LINE_TRIG_d4n,
           lcd_sig.VID_LINE_TRIG_d4a,
           lcd_sig.VID_LINE_TRIG_d4p,
           lcd_sig.VID_LINE_TRIG_d4c,
           lcd_sig.XAHY_VID_LINE_TRIG_d4n,
           bus_ctl.TEDO_CPURD,
           bus_ctl.TAPU_CPUWR,
           bus_ctl.ASOT_CPURD,
           bus_ctl.CUPA_CPUWR);


 

    
    if ((i % 8) == 7) printf("\n");

  }
#endif

  return 0;
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




















void sdl_run() {

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




//-----------------------------------------------------------------------------

int main(int, char**)
{
  init();

  while (!quit)
  {
    frame_begin = SDL_GetPerformanceCounter();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) quit = true;
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) quit = true;
    }
    update();
    render_frame();
    render_ui();
    frame_end = SDL_GetPerformanceCounter();
    frame_time = frame_end - frame_begin;
    frame_count++;
    SDL_GL_SwapWindow(window);
  }

  close();
  return 0;
}

//-----------------------------------------------------------------------------