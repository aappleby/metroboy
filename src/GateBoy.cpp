#include "GateBoy.h"

//-----------------------------------------------------------------------------

int GateBoy::main(int /*argc*/, char** /*argv*/) {
  printf("GateBoy sim starting\n");
  GateBoy gb;

  return 0;
}

//-----------------------------------------------------------------------------

uint8_t GateBoy::read_cycle(uint16_t /*addr*/) {
  Schematics::SchematicTop* gb = state_manager.state();

  for (int pass_phase = 0; pass_phase < 8; pass_phase++) {
    gb->phase_counter++;

    Schematics::SignalHash old_hash;
    for (int pass = 0; pass < 256; pass++) {
      //gb->ext_bus.PIN_CLK_IN_xBxDxFxH.preset(true, (gb->phase_counter & 1));

      //gb->cpu_pins.preset_rd(1);
      //gb->cpu_pins.preset_wr(0);
      //gb->cpu_pins.preset_addr_valid(1);
      //gb->cpu_bus.preset_addr(true, addr);

      //gb->joy_reg.clear_dir();

      gb->tick_everything();
      Schematics::SignalHash hash = gb->commit_everything();
      if (hash == old_hash) break;
      if (pass == 199) printf("stuck!\n");
      if (pass == 200) __debugbreak();
    }
  }

  return (uint8_t)gb->cpu_bus.get_data();
}

//-----------------------------------------------------------------------------

void GateBoy::write_cycle(uint16_t /*addr*/, uint8_t /*data*/) {
  Schematics::SchematicTop* gb = state_manager.state();

  for (int pass_phase = 0; pass_phase < 8; pass_phase++) {
    gb->phase_counter++;

    for (int pass = 0; pass < 256; pass++) {
      //gb->ext_bus.PIN_CLK_IN_xBxDxFxH.preset(true, (gb->phase_counter & 1));

      //gb->cpu_bus.PIN_CPU_RAW_RD.preset(true, 0);
      //gb->cpu_bus.PIN_CPU_RAW_WR.preset(true, 1);
      //gb->cpu_bus.PIN_ADDR_VALID.preset(true, 1);
      //gb->cpu_bus.preset_addr(true, addr);
      //gb->cpu_bus.set_data(true, data);

      //gb->joy_reg.clear_dir();

      gb->tick_everything();
      bool changed = gb->commit_everything();
      if (!changed) break;
      if (pass == 199) printf("stuck!\n");
      if (pass == 200) __debugbreak();
    }
  }
}

//-----------------------------------------------------------------------------

void GateBoy::pass_cycle() {
  Schematics::SchematicTop* gb = state_manager.state();

  for (int pass_phase = 0; pass_phase < 8; pass_phase++) {
    gb->phase_counter++;

    for (int pass = 0; pass < 256; pass++) {
      //gb->ext_pins_in.PIN_CLK_IN_xBxDxFxH.preset(true, (gb->phase_counter & 1));

      gb->cpu_bus.PIN_CPU_RAW_RD.preset(true, 0);
      gb->cpu_bus.PIN_CPU_RAW_WR.preset(true, 0);
      gb->cpu_bus.PIN_ADDR_VALID.preset(true, 0);
      gb->cpu_bus.preset_addr(true, 0x0000);

      //gb->joy_reg.clear_dir();

      gb->tick_everything();
      bool changed = gb->commit_everything();
      if (!changed) break;
      if (pass == 199) printf("stuck!\n");
      if (pass == 200) __debugbreak();
    }
  }
}

//-----------------------------------------------------------------------------

void GateBoy::init() {
  reset(0x0100);

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
}

//-----------------------------------------------------------------------------

void GateBoy::reset(uint16_t /*new_pc*/) {
  state_manager.reset();

  auto gb_step = [this](Schematics::SchematicTop* gb){
    gb->phase_counter++;

    for (int pass = 0; pass < 256; pass++) {
      gb->clk_reg.preset_clk(gb->phase_counter & 1);

      /*
      if (phase >= 32 && phase < 40) {
      gb->cpu_pins.CPU_RAW_RD.preset(true, 0);
      gb->cpu_pins.CPU_RAW_WR.preset(true, 1);
      gb->cpu_pins.ADDR_VALIDx.preset(true, 1);
      gb->cpu_pins.preset_addr(true, 0xFF42);
      gb->cpu_pins.set_data(true, 0x55);
      }
      else if (phase >= 40 && phase < 48) {
      gb->cpu_pins.CPU_RAW_RD.preset(true, 0);
      gb->cpu_pins.CPU_RAW_WR.preset(true, 0);
      gb->cpu_pins.ADDR_VALIDx.preset(true, 0);
      gb->cpu_pins.preset_addr(true, 0x0000);
      }
      */

      //gb->joy_reg.clear_dir();

      gb->tick_everything();
      bool changed = gb->commit_everything();
      if (!changed) break;
      if (pass == 199) printf("stuck!\n");
      if (pass == 200) __debugbreak();
    }
  };

  state_manager.set_step(gb_step);

  Schematics::SchematicTop* gb = state_manager.state();

  gb->EXT_PIN_RST.preset(true, 1);
  gb->EXT_PIN_CLK_GOOD.preset(true, 0);
  gb->dbg_reg.preset_t1t2(0,0);

  gb->cpu_bus.CPU_PIN_CLKREQ.preset(true, 0);
  gb->cpu_bus.PIN_CPU_RAW_RD.preset(true, 0);
  gb->cpu_bus.PIN_CPU_RAW_WR.preset(true, 0);
  gb->cpu_bus.PIN_ADDR_VALID.preset(true, 1);
  gb->cpu_bus.CPU_PIN5.preset(true, 0);
  gb->cpu_bus.CPU_PIN6.preset(true, 0);

  /*
  gb->int_reg.PIN_ACK_SERIAL.preset(true, 0);
  gb->int_reg.PIN_ACK_STAT.preset(true, 0);
  gb->int_reg.PIN_ACK_VBLANK.preset(true, 0);
  gb->int_reg.PIN_ACK_TIMER.preset(true, 0);
  gb->int_reg.PIN_ACK_JOYPAD.preset(true, 0);
  */

  //gb->ext_pins_in.preset();

  pass_cycle();
  gb->EXT_PIN_RST.preset(true, 0);
  pass_cycle();
  gb->EXT_PIN_CLK_GOOD.preset(true, 1);
  pass_cycle();
  gb->cpu_bus.CPU_PIN_CLKREQ.preset(true, 1);
  pass_cycle();
}

//-----------------------------------------------------------------------------

/*
void GateBoy::update(double delta) {
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

void GateBoy::render_frame(int /*screen_w*/, int /*screen_h*/, TextPainter& text_painter) {
  //uint64_t begin = SDL_GetPerformanceCounter();

  Schematics::SchematicTop& gb = *state_manager.state();

  text_painter.dprintf(" ----- SYS_REG -----\n");
  text_painter.dprintf("PHASE    %08d\n", gb.phase_counter);

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

  float cx = 32 * 8 * 4;
  float cy = 0;

  text_painter.newline();
  //gb.ext_bus.dump_pins(text_painter);
  //gb.rst_reg.dump_regs(text_painter);
  //gb.clk_reg.dump_regs(text_painter);
  //gb.vck_reg.dump_regs(text_painter);
  //gb.cpu_bus.dump_pins(text_painter);
  //gb.ext_bus.dump_pins(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 32 * 8;

  //gb.joy_reg.dump_regs(text_painter);
  //gb.dbg_reg.dump_regs(text_painter);
  //gb.dma_reg.dump_regs(text_painter);
  //gb.int_reg.dump_regs(text_painter);
  //gb.ser_reg.dump_regs(text_painter);
  //gb.joy_reg.dump_pins(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 32 * 8;

  //gb.tim_reg.dump_regs(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 32 * 8;

  //gb.lcd_reg.dump_regs(text_painter);
  //gb.pxp_reg.dump_regs(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 32 * 8;

  //gb.ppu_reg.dump_regs(text_painter);
  //gb.sst_reg.dump_regs(text_painter);
  //gb.ppu_reg.dump_regs(text_painter);
  //gb.oam_reg.dump_regs(text_painter);
  text_painter.render(cx, cy, 1.0);
  cx += 32 * 8;

  if (1) {
    //gb.oam_bus.dump_pins(text_painter);
    //gb.vram_bus.dump_pins(text_painter);
    text_painter.render(cx, cy, 1.0);
    cx += 32 * 8;
  }

  if (1) {
    //gb.lcd_reg.dump_pins(text_painter);
    //gb.ser_reg.dump_pins(text_painter);
    text_painter.render(cx, cy, 1.0);
    cx += 32 * 8;
  }

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
