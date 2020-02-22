#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <string>
#include <vector>
#include <math.h>

#include <GL/gl3w.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <SDL.h>
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>

using namespace Schematics;

//-----------------------------------------------------------------------------

uint8_t Main::read_cycle(uint16_t addr) {
  TestGB* gb = gbs.state();

  for (int pass_phase = 0; pass_phase < 8; pass_phase++) {
    gb->phase_counter++;

    for (int pass = 0; pass < 256; pass++) {
      int phase = gb->phase_counter;
      gb->sys_pins.CLK_IN.preset(true, !(phase & 1));

      gb->cpu_pins.CPU_RAW_RD.preset(true, 1);
      gb->cpu_pins.CPU_RAW_WR.preset(true, 0);
      gb->cpu_pins.ADDR_VALIDn.preset(true, 1);
      gb->cpu_pins.preset_addr(true, addr);

      // FIXME still don't know who drives these, so we always set them to 0.
      gb->joy_pins.P10_B.set(0);
      gb->joy_pins.P11_B.set(0);
      gb->joy_pins.P12_B.set(0);
      gb->joy_pins.P13_B.set(0);

      gb->tick_everything();
      bool changed = gb->commit_everything();
      if (!changed) break;
      if (pass == 199) printf("stuck!\n");
      if (pass == 200) __debugbreak();
    }
  }

  return (uint8_t)gb->cpu_pins.get_data();
}

void Main::write_cycle(uint16_t addr, uint8_t data) {
  TestGB* gb = gbs.state();

  for (int pass_phase = 0; pass_phase < 8; pass_phase++) {
    gb->phase_counter++;

    for (int pass = 0; pass < 256; pass++) {
      int phase = gb->phase_counter;
      gb->sys_pins.CLK_IN.preset(true, !(phase & 1));

      gb->cpu_pins.CPU_RAW_RD.preset(true, 0);
      gb->cpu_pins.CPU_RAW_WR.preset(true, 1);
      gb->cpu_pins.ADDR_VALIDn.preset(true, 1);
      gb->cpu_pins.preset_addr(true, addr);
      gb->cpu_pins.set_data(true, data);

      // FIXME still don't know who drives these, so we always set them to 0.
      gb->joy_pins.P10_B.set(0);
      gb->joy_pins.P11_B.set(0);
      gb->joy_pins.P12_B.set(0);
      gb->joy_pins.P13_B.set(0);

      gb->tick_everything();
      bool changed = gb->commit_everything();
      if (!changed) break;
      if (pass == 199) printf("stuck!\n");
      if (pass == 200) __debugbreak();
    }
  }
}

void Main::pass_cycle() {
  TestGB* gb = gbs.state();

  for (int pass_phase = 0; pass_phase < 8; pass_phase++) {
    gb->phase_counter++;

    for (int pass = 0; pass < 256; pass++) {
      int phase = gb->phase_counter;
      gb->sys_pins.CLK_IN.preset(true, !(phase & 1));

      gb->cpu_pins.CPU_RAW_RD.preset(true, 0);
      gb->cpu_pins.CPU_RAW_WR.preset(true, 0);
      gb->cpu_pins.ADDR_VALIDn.preset(true, 0);
      gb->cpu_pins.preset_addr(true, 0x0000);

      // FIXME still don't know who drives these, so we always set them to 0.
      gb->joy_pins.P10_B.set(0);
      gb->joy_pins.P11_B.set(0);
      gb->joy_pins.P12_B.set(0);
      gb->joy_pins.P13_B.set(0);

      gb->tick_everything();
      bool changed = gb->commit_everything();
      if (!changed) break;
      if (pass == 199) printf("stuck!\n");
      if (pass == 200) __debugbreak();
    }
  }
}

uint8_t Main::rw_cycle(uint16_t addr, uint8_t data) {
  write_cycle(addr, data);
  return read_cycle(addr);
}

//-----------------------------------------------------------------------------

/*
AF=$01B0
BC=$0013
DE=$00D8
HL=$014D
Stack Pointer=$FFFE
[$FF05] = $00   ; TIMA
[$FF06] = $00   ; TMA
[$FF07] = $00   ; TAC
[$FF40] = $91   ; LCDC
[$FF42] = $00   ; SCY
[$FF43] = $00   ; SCX
[$FF45] = $00   ; LYC
[$FF47] = $FC   ; BGP
[$FF48] = $FF   ; OBP0
[$FF49] = $FF   ; OBP1
[$FF4A] = $00   ; WY
[$FF4B] = $00   ; WX
[$FFFF] = $00   ; IE
*/

void Main::init() {
  base::init();

  text.init(fb_width, fb_height);

  gb_tex = create_texture(64, 64, 2);

  TestGB* gb = gbs.state();
  gb->sys_pins.RST.preset(true, 1);
  gb->sys_pins.CLK_GOOD.preset(true, 0);
  gb->sys_pins.T2.preset(true, 0);
  gb->sys_pins.T1.preset(true, 0);

  gb->cpu_pins.CLKREQ.preset(true, 0);
  gb->cpu_pins.CPU_RAW_RD.preset(true, 0);
  gb->cpu_pins.CPU_RAW_WR.preset(true, 0);
  gb->cpu_pins.ADDR_VALIDn.preset(true, 1);

  gb->cpu_pins.FROM_CPU5.preset(true, 0);
  gb->cpu_pins.FROM_CPU6.preset(true, 0);
  gb->cpu_pins.ACK_SERIAL.preset(true, 0);
  gb->cpu_pins.ACK_STAT.preset(true, 0);
  gb->cpu_pins.ACK_VBLANK.preset(true, 0);
  gb->cpu_pins.ACK_TIMER.preset(true, 0);
  gb->cpu_pins.ACK_JOYPAD.preset(true, 0);

  gb->ext_preset();

  pass_cycle();
  gb->sys_pins.RST.preset(true, 0);
  pass_cycle();
  gb->sys_pins.CLK_GOOD.preset(true, 1);
  pass_cycle();
  gb->cpu_pins.CLKREQ.preset(true, 1);
  pass_cycle();

  /*
  printf("joyp 0x%02x 0x%02x\n", rw_cycle(0xFF00, 0x00), rw_cycle(0xFF00, 0xFF));
  printf("sb   0x%02x 0x%02x\n", rw_cycle(0xFF01, 0x00), rw_cycle(0xFF01, 0xFF));
  printf("sc   0x%02x 0x%02x\n", rw_cycle(0xFF02, 0x00), rw_cycle(0xFF02, 0xFF));
  printf("nc   0x%02x 0x%02x\n", rw_cycle(0xFF03, 0x00), rw_cycle(0xFF03, 0xFF));

  printf("div  0x%02x 0x%02x\n", rw_cycle(0xFF04, 0x00), rw_cycle(0xFF04, 0xFF));
  printf("tima 0x%02x 0x%02x\n", rw_cycle(0xFF05, 0x00), rw_cycle(0xFF05, 0xFF));
  printf("tma  0x%02x 0x%02x\n", rw_cycle(0xFF06, 0x00), rw_cycle(0xFF06, 0xFF));
  printf("tac  0x%02x 0x%02x\n", rw_cycle(0xFF07, 0x00), rw_cycle(0xFF07, 0xFF));
  printf("if   0x%02x 0x%02x\n", rw_cycle(0xFF0F, 0x00), rw_cycle(0xFF0F, 0xFF));

  printf("lcdc 0x%02x 0x%02x\n", rw_cycle(0xFF40, 0x00), rw_cycle(0xFF40, 0xFF));
  printf("stat 0x%02x 0x%02x\n", rw_cycle(0xFF41, 0x00), rw_cycle(0xFF41, 0xFF));
  printf("scy  0x%02x 0x%02x\n", rw_cycle(0xFF42, 0x00), rw_cycle(0xFF42, 0xFF));
  printf("scx  0x%02x 0x%02x\n", rw_cycle(0xFF43, 0x00), rw_cycle(0xFF43, 0xFF));
  printf("ly   0x%02x 0x%02x\n", rw_cycle(0xFF44, 0x00), rw_cycle(0xFF44, 0xFF));
  printf("lyc  0x%02x 0x%02x\n", rw_cycle(0xFF45, 0x00), rw_cycle(0xFF45, 0xFF));
  printf("lyc  0x%02x 0x%02x\n", rw_cycle(0xFF46, 0x00), rw_cycle(0xFF46, 0xFF));
  printf("bgp  0x%02x 0x%02x\n", rw_cycle(0xFF47, 0x00), rw_cycle(0xFF47, 0xFF));
  printf("obp0 0x%02x 0x%02x\n", rw_cycle(0xFF48, 0x00), rw_cycle(0xFF48, 0xFF));
  printf("obp1 0x%02x 0x%02x\n", rw_cycle(0xFF49, 0x00), rw_cycle(0xFF49, 0xFF));
  printf("wy   0x%02x 0x%02x\n", rw_cycle(0xFF4A, 0x00), rw_cycle(0xFF4A, 0xFF));
  printf("wx   0x%02x 0x%02x\n", rw_cycle(0xFF4B, 0x00), rw_cycle(0xFF4B, 0xFF));
  */

  /*
  for(int addr = 0xFF00; addr <= 0xFFFF; addr++) {
    printf("0x%04x    0x%02x 0x%02x\n", addr, rw_cycle((uint16_t)addr, 0x00), rw_cycle((uint16_t)addr, 0xFF));
  }
  */

  auto gb_step = [this](TestGB* gb){
    gb->phase_counter++;

    for (int pass = 0; pass < 256; pass++) {
      int phase = gb->phase_counter;
      gb->sys_pins.CLK_IN.preset(true, !(phase & 1));

      /*
      if (phase >= 32 && phase < 40) {
        gb->cpu_pins.CPU_RAW_RD.preset(true, 0);
        gb->cpu_pins.CPU_RAW_WR.preset(true, 1);
        gb->cpu_pins.ADDR_VALIDn.preset(true, 1);
        gb->cpu_pins.preset_addr(true, 0xFF42);
        gb->cpu_pins.set_data(true, 0x55);
      }
      else if (phase >= 40 && phase < 48) {
        gb->cpu_pins.CPU_RAW_RD.preset(true, 0);
        gb->cpu_pins.CPU_RAW_WR.preset(true, 0);
        gb->cpu_pins.ADDR_VALIDn.preset(true, 0);
        gb->cpu_pins.preset_addr(true, 0x0000);
      }
      */

      // FIXME still don't know who drives these, so we always set them to 0.
      gb->joy_pins.P10_B.set(0);
      gb->joy_pins.P11_B.set(0);
      gb->joy_pins.P12_B.set(0);
      gb->joy_pins.P13_B.set(0);

      gb->tick_everything();
      bool changed = gb->commit_everything();
      if (!changed) break;
      if (pass == 199) printf("stuck!\n");
      if (pass == 200) __debugbreak();
    }
  };

  gbs.set_step(gb_step);
}

void Main::close() {
  base::close();
}

//-----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  Main m;
  return m.main(argc, argv);
}

//-----------------------------------------------------------------------------

void Main::begin_frame() {
  base::begin_frame();
}

//-----------------------------------------------------------------------------

void Main::update() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT) quit = true;
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) quit = true;

    if (event.type == SDL_KEYDOWN) switch (event.key.keysym.sym) {
    case SDLK_RIGHT:
      gbs.step();
      break;
    case SDLK_LEFT:
      gbs.unstep();
      break;
    case SDLK_UP:
      for (int i = 0; i < 8; i++) gbs.step();
      break;
    case SDLK_DOWN:
      for (int i = 0; i < 8; i++) gbs.unstep();
      break;
    }
  }
  
  base::update();
}

//-----------------------------------------------------------------------------

void Main::render_frame() {
  base::render_frame();

  uint64_t begin = SDL_GetPerformanceCounter();

  text.begin_frame();
  
  text.set_pal(0, 0.4, 0.4, 0.4, 1.0); // grey
  text.set_pal(1, 0.8, 0.8, 0.8, 1.0); // white 
  
  text.set_pal(2, 0.6, 1.0, 0.6, 1.0); // lo-z out = green
  text.set_pal(3, 1.0, 0.6, 0.6, 1.0); // hi-z out = red

  text.set_pal(4, 0.6, 0.6, 1.0, 1.0); // lo-z in = blue
  text.set_pal(5, 1.0, 1.0, 0.6, 1.0); // hi-z in = yellow

  text.set_pal(6, 1.0, 0.6, 1.0, 1.0); // error magenta

  text.dprintf(" ----- SYS_REG -----\n");
  text.dprintf("PHASE    %08d\n", gbs.state()->phase_counter);

  TestGB& gb = *gbs.state();

  int p = gb.phase_counter & 7;
  text.dprintf("PHASE    %c%c%c%c%c%c%c%c\n",
               p == 0 ? 'A' : '_',
               p == 1 ? 'B' : '_',
               p == 2 ? 'C' : '_',
               p == 3 ? 'D' : '_',
               p == 4 ? 'E' : '_',
               p == 5 ? 'F' : '_',
               p == 6 ? 'G' : '_',
               p == 7 ? 'H' : '_');

  float cx = 4;
  float cy = 4;

  text.newline();
  gb.sys_pins.dump_pins(text);
  gb.rst_reg.dump_regs(text);
  gb.clk_reg.dump_regs(text);
  gb.cpu_pins.dump_pins(text);
  gb.bus_reg.dump_regs(text);
  gb.ext_pins.dump_pins(text);
  text.render(cx, cy, 1.0);
  cx += 192;

  gb.joy_reg.dump_regs(text);
  gb.dbg_reg.dump_regs(text);
  gb.dma_reg.dump_regs(text);
  gb.int_reg.dump_regs(text);
  gb.ser_reg.dump_regs(text);
  gb.joy_pins.dump_pins(text);
  text.render(cx, cy, 1.0);
  cx += 192;

  gb.tim_reg.dump_regs(text);
  text.render(cx, cy, 1.0);
  cx += 192;

  gb.cfg_reg.dump_regs(text);
  gb.lcd_reg.dump_regs(text);
  gb.pxp_reg.dump_regs(text);
  text.render(cx, cy, 1.0);
  cx += 192;

  gb.spr_reg.dump_regs(text);
  gb.sst_reg.dump_regs(text);
  gb.vid_reg.dump_regs(text);
  gb.vclk_reg.dump_regs(text);
  gb.oam_reg.dump_regs(text);
  text.render(cx, cy, 1.0);
  cx += 192;

  gb.oam_pins.dump_pins(text);
  gb.vram_pins.dump_pins(text);
  text.render(cx, cy, 1.0);
  cx += 192;

  gb.lcd_pins.dump_pins(text);
  gb.wave_pins.dump_pins(text);
  gb.ser_pins.dump_pins(text);
  text.render(cx, cy, 1.0);
  cx += 192;

  uint64_t end = SDL_GetPerformanceCounter();
  double delta = double(end - begin) / double(SDL_GetPerformanceFrequency());
  static double accum = 0;
  accum = accum * 0.99 + delta * 0.01;
  text.dprintf("%f usec\n", accum * 1.0e6);
  text.render(cx, cy, 1.0);
  cx += 192;
}

//-----------------------------------------------------------------------------

void Main::render_ui() {
  ImGui::Begin("GB Sim Stats");
  ImGui::Text("now   %f\n", now);
  ImGui::Text("phase %d\n", gbs.state()->phase_counter);
  ImGui::Text("freq  %f\n", gbs.state()->phase_counter / now);
  ImGui::End();

  base::render_ui();
}

//-----------------------------------------------------------------------------

void Main::end_frame() {
  base::end_frame();
}

//-----------------------------------------------------------------------------
