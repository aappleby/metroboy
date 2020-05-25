#include "gateboy_main.h"

#ifdef _MSC_VER
#include <include/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

//-----------------------------------------------------------------------------

uint8_t GateboyMain::read_cycle(uint16_t addr) {
  Schematics::TestGB* gb = state_manager.state();

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

void GateboyMain::write_cycle(uint16_t addr, uint8_t data) {
  Schematics::TestGB* gb = state_manager.state();

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

void GateboyMain::pass_cycle() {
  Schematics::TestGB* gb = state_manager.state();

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

//-----------------------------------------------------------------------------

void GateboyMain::init() {
  Schematics::TestGB* gb = state_manager.state();
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

  auto gb_step = [this](Schematics::TestGB* gb){
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

  state_manager.set_step(gb_step);
}

//-----------------------------------------------------------------------------

/*
void GateboyMain::update(double delta) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) switch (event.key.keysym.sym) {
    case SDLK_RIGHT:
      state_manager.step();
      break;
    case SDLK_LEFT:
      state_manager.unstep();
      break;
    case SDLK_UP:
      state_manager.step(8);
      break;
    case SDLK_DOWN:
      state_manager.unstep(8);
      break;
    }
  }
}
*/

//-----------------------------------------------------------------------------

void GateboyMain::render_frame(int /*screen_w*/, int /*screen_h*/, TextPainter& text_painter) {
  //uint64_t begin = SDL_GetPerformanceCounter();

  text_painter.dprintf(" ----- SYS_REG -----\n");
  text_painter.dprintf("PHASE    %08d\n", state_manager.state()->phase_counter);

  Schematics::TestGB& gb = *state_manager.state();

  int p = gb.phase_counter & 7;
  text_painter.dprintf("PHASE    %c%c%c%c%c%c%c%c\n",
               p == 0 ? 'A' : '_',
               p == 1 ? 'B' : '_',
               p == 2 ? 'C' : '_',
               p == 3 ? 'D' : '_',
               p == 4 ? 'E' : '_',
               p == 5 ? 'F' : '_',
               p == 6 ? 'G' : '_',
               p == 7 ? 'H' : '_');

  float cx = 4;
  float cy = -800;

  text_painter.newline();
  gb.sys_pins.dump_pins(text_painter);
  gb.rst_reg.dump_regs(text_painter);
  gb.clk_reg.dump_regs(text_painter);
  gb.cpu_pins.dump_pins(text_painter);
  gb.bus_reg.dump_regs(text_painter);
  gb.ext_pins.dump_pins(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 192;

  gb.joy_reg.dump_regs(text_painter);
  gb.dbg_reg.dump_regs(text_painter);
  gb.dma_reg.dump_regs(text_painter);
  gb.int_reg.dump_regs(text_painter);
  gb.ser_reg.dump_regs(text_painter);
  gb.joy_pins.dump_pins(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 192;

  gb.tim_reg.dump_regs(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 192;

  gb.cfg_reg.dump_regs(text_painter);
  gb.lcd_reg.dump_regs(text_painter);
  gb.pxp_reg.dump_regs(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 192;

  gb.spr_reg.dump_regs(text_painter);
  gb.sst_reg.dump_regs(text_painter);
  gb.vid_reg.dump_regs(text_painter);
  gb.vclk_reg.dump_regs(text_painter);
  gb.oam_reg.dump_regs(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 192;

  gb.oam_pins.dump_pins(text_painter);
  gb.vram_pins.dump_pins(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 192;

  gb.lcd_pins.dump_pins(text_painter);
  gb.wave_pins.dump_pins(text_painter);
  gb.ser_pins.dump_pins(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 192;

  /*
  uint64_t end = SDL_GetPerformanceCounter();
  double delta = double(end - begin) / double(SDL_GetPerformanceFrequency());
  static double accum = 0;
  accum = accum * 0.99 + delta * 0.01;
  text_painter.dprintf("%f usec\n", accum * 1.0e6);
  text_painter.render(cx, cy, 1.0);
  cx += 192;
  */
}

//-----------------------------------------------------------------------------
