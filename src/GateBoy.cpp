#include "GateBoy.h"

#include <chrono>

using namespace Schematics;

#pragma warning(disable:4702)

SignalHash phase(SchematicTop* top, Req req, bool verbose) {
  SignalHash hash;
  int pass_count = 0;
  for (; pass_count < 256; pass_count++) {
    top->cpu_bus.set_cpu_req(req);
    top->vram_bus.set_vram_data(0);
    top->oam_bus.set_oam_data(0, 0);
    top->ext_bus.set_ext_data(0);
    top->joypad.set_buttons(0);
    
    SignalHash new_hash = top->tick();
    
    if (new_hash.h == hash.h) break;
    hash = new_hash;
    if (pass_count == 199) printf("stuck!\n");
    if (pass_count == 200) __debugbreak();
  }

  if (verbose) {
    printf("Phase %08d %c pass %02d CLK_GOOD %d CLK %d RST %d phz %d%d%d%d vid %d%d%d %d CPU_START %d CPU_RDY %d DIV %05d AFER %d ASOL %d\n",
      top->phase_counter,
      'A' + (top->phase_counter & 7),
      pass_count,
      top->clk_reg.get_clk_a(),
      top->clk_reg.get_clk_b(),
      top->clk_reg.SYS_PIN_RSTp(),
      top->clk_reg.AFUR_ABCDxxxx(),
      top->clk_reg.ALEF_xBCDExxx(),
      top->clk_reg.APUK_xxCDEFxx(),
      top->clk_reg.ADYK_xxxDEFGx(),
      top->clk_reg.WUVU_xxCDxxGH(),
      top->clk_reg.VENA_xxxxEFGH(),
      top->clk_reg.WOSU_xBCxxFGx(),
      top->clk_reg.BOMA_Axxxxxxx(),
      top->clk_reg.CPU_PIN_STARTp(),
      top->clk_reg.CPU_PIN_READYp(),
      top->tim_reg.get_div(),
      top->clk_reg.AFER_SYS_RSTp(),
      top->clk_reg._ASOL_POR_DONEn.q()
      //hash.h);
      );
  }

  return hash;
}


SignalHash run(SchematicTop* top, int phase_count, Req req, bool verbose) {
  SignalHash hash;
  for (int i = 0; i < phase_count; i++) {
    top->phase_counter++;
    wire CLK = (top->phase_counter & 1) & (top->clk_reg.get_clk_a());
    top->clk_reg.set_clk_b(CLK);
    hash = phase(top, req, verbose);
  }
  return hash;
}

//----------------------------------------

//-----------------------------------------------------------------------------

void test_reset_timing(int phase_a, int phase_b, int phase_c, int phase_d) {
  SchematicTop* top = new SchematicTop();

  top->clk_reg.set_cpu_ready(0);
  top->ext_bus.set_ext_rdwr(0, 0);

  top->clk_reg.set_t1t2(0,0);

  SignalHash hash;

  // Just read DIV forever.
  Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };

  // 8 phases w/ reset high, clock not running.
  top->clk_reg.set_rst(1);
  top->clk_reg.set_clk_a(0);
  run(top, phase_a, req, false);

  // 8 phases w/ reset high, clock running.
  top->clk_reg.set_rst(1);
  top->clk_reg.set_clk_a(1);
  run(top, phase_b, req, false);

  // 8 phases w/ reset low, clock running.
  top->clk_reg.set_rst(0);
  top->clk_reg.set_clk_a(1);
  run(top, phase_c, req, false);

  // Force LCDC_EN on and run until we get the CPU start request (~32k mcycles)

  while(!top->clk_reg.CPU_PIN_STARTp()) {
    run(top, 1, req, false);
  }

  // Ack the start request and run another 24 phases.
  // We should see AFER (global reset) clear and the video clocks start up.
  // FIXME why are the video clocks not running...

  top->clk_reg.set_cpu_ready(1);
  run(top, phase_d, req, false);

  top->clk_reg.set_cpu_ready(0);
  if (top->clk_reg.AFER_SYS_RSTp() || top->clk_reg._ASOL_POR_DONEn.q()) {
    printf("\nX %d %d %d %d\n", phase_a, phase_b, phase_c, phase_d);
  }
  else {
    printf(".");
  }

  //run(top, 1, req, true);
}

//-----------------------------------------------------------------------------

int GateBoy::main(int /*argc*/, char** /*argv*/) {
  printf("GateBoy sim starting\n");

  for (int phase_a = 0; phase_a <= 8; phase_a++) 
  for (int phase_b = 1; phase_b <= 8; phase_b++)
  for (int phase_c = 0; phase_c <= 8; phase_c++)
  for (int phase_d = 8; phase_d <= 16; phase_d++)
    test_reset_timing(phase_a, phase_b, phase_c, phase_d);
  return 0;

  GateBoy gateboy;
  gateboy.init();
  //gateboy.reset(0x100);

  auto top = gateboy.top();

  top->clk_reg.set_cpu_ready(0);
  top->ext_bus.set_ext_rdwr(0, 0);

  top->clk_reg.set_t1t2(0,0);

  SignalHash hash;

  gateboy.verbose = true;

  // Just read DIV forever.
  Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };

  // 8 phases w/ reset high, clock not running.
  top->clk_reg.set_rst(1);
  top->clk_reg.set_clk_a(0);
  gateboy.run(top, 8, req);
  printf("\n");

  // 8 phases w/ reset high, clock running.
  top->clk_reg.set_rst(1);
  top->clk_reg.set_clk_a(1);
  gateboy.run(top, 8, req);
  printf("\n");

  // 8 phases w/ reset low, clock running.
  top->clk_reg.set_rst(0);
  top->clk_reg.set_clk_a(1);
  gateboy.run(top, 8, req);
  printf("\n");

  // Force LCDC_EN on and run until we get the CPU start request (~32k mcycles)

  while(!top->clk_reg.CPU_PIN_STARTp()) {
    gateboy.run(top, 1, req);
  }

  // Ack the start request and run another 24 phases.
  // We should see AFER (global reset) clear and the video clocks start up.
  // FIXME why are the video clocks not running...

  top->clk_reg.set_cpu_ready(1);
  gateboy.run(top, 8, req);
  printf("\n");

  top->pix_pipe.XONA_LCDC_EN.preset(1);
  gateboy.run(top, 24, req);
  printf("\n");

  /*
  printf("Running a bunch of phases for perf test\n");
  gateboy.verbose = false;

  const int phase_count = 31415;
  auto start = std::chrono::high_resolution_clock::now();
  gateboy.run(top, phase_count, req);
  auto finish = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed = finish - start;
  printf("Done - %f sec, %f phases/sec\n", elapsed.count(), double(phase_count) / elapsed.count());
  printf("\n");
  */

  /*
  printf("DIV  %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
         top->tim_reg.UPOF_DIV_15.as_char(),
         top->tim_reg.UKET_DIV_14.as_char(),
         top->tim_reg.TEKA_DIV_13.as_char(),
         top->tim_reg.SUBU_DIV_12.as_char(),
         top->tim_reg.SOLA_DIV_11.as_char(),
         top->tim_reg.TERU_DIV_10.as_char(),
         top->tim_reg.TOFE_DIV_09.as_char(),
         top->tim_reg.TUGO_DIV_08.as_char(),
         top->tim_reg.TULU_DIV_07.as_char(),
         top->tim_reg.UGOT_DIV_06.as_char(),
         top->tim_reg.TAMA_DIV_05.as_char(),
         top->tim_reg.UNYK_DIV_04.as_char(),
         top->tim_reg.TERO_DIV_03.as_char(),
         top->tim_reg.UNER_DIV_02.as_char(),
         top->tim_reg.UFOR_DIV_01.as_char(),
         top->tim_reg.UKUP_DIV_00.as_char());


  printf("CPUD   %c%c%c%c%c%c%c%c\n",
         top->CPU_TRI_D7.as_char(),
         top->CPU_TRI_D6.as_char(),
         top->CPU_TRI_D5.as_char(),
         top->CPU_TRI_D4.as_char(),
         top->CPU_TRI_D3.as_char(),
         top->CPU_TRI_D2.as_char(),
         top->CPU_TRI_D1.as_char(),
         top->CPU_TRI_D0.as_char());
  */

  return 0;
}

//----------------------------------------

SignalHash GateBoy::run(SchematicTop* top, int phase_count, Req req) {
  SignalHash hash;
  for (int i = 0; i < phase_count; i++) {
    top->phase_counter++;
    wire CLK = (top->phase_counter & 1) & (top->clk_reg.get_clk_a());
    top->clk_reg.set_clk_b(CLK);
    hash = phase(top, req);
  }
  return hash;
}

//----------------------------------------

SignalHash GateBoy::phase(SchematicTop* top, Req req) {
  SignalHash hash;
  int pass_count = 0;
  for (; pass_count < 256; pass_count++) {
    top->cpu_bus.set_cpu_req(req);
    top->vram_bus.set_vram_data(0);
    top->oam_bus.set_oam_data(0, 0);
    top->ext_bus.set_ext_data(0);
    top->joypad.set_buttons(0);
    
    SignalHash new_hash = top->tick();
    
    if (new_hash.h == hash.h) break;
    hash = new_hash;
    if (pass_count == 199) printf("stuck!\n");
    if (pass_count == 200) __debugbreak();
  }

  if (verbose) {
    printf("Phase %08d %c pass %02d CLK_GOOD %d CLK %d RST %d phz %d%d%d%d vid %d%d%d %d CPU_START %d CPU_RDY %d DIV %05d AFER %d ASOL %d\n",
      top->phase_counter,
      'A' + (top->phase_counter & 7),
      pass_count,
      top->clk_reg.get_clk_a(),
      top->clk_reg.get_clk_b(),
      top->clk_reg.SYS_PIN_RSTp(),
      top->clk_reg.AFUR_ABCDxxxx(),
      top->clk_reg.ALEF_xBCDExxx(),
      top->clk_reg.APUK_xxCDEFxx(),
      top->clk_reg.ADYK_xxxDEFGx(),
      top->clk_reg.WUVU_xxCDxxGH(),
      top->clk_reg.VENA_xxxxEFGH(),
      top->clk_reg.WOSU_xBCxxFGx(),
      top->clk_reg.BOMA_Axxxxxxx(),
      top->clk_reg.CPU_PIN_STARTp(),
      top->clk_reg.CPU_PIN_READYp(),
      top->tim_reg.get_div(),
      top->clk_reg.AFER_SYS_RSTp(),
      top->clk_reg._ASOL_POR_DONEn.q()
      //hash.h);
      );
  }

  return hash;
}

//-----------------------------------------------------------------------------

void GateBoy::init() {
  auto top_step = [this](Schematics::SchematicTop* top) {
    top->clk_reg.set_clk_a(1);
    top->clk_reg.set_clk_b(top->phase_counter & 1);
    phase(top, {0});
  };
  state_manager.init(top_step);

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

  /*
  Schematics::SchematicTop* gb = state_manager.state();
  gb->SYS_PIN_RST.preset(true, 1);
  gb->SYS_PIN_CLK_GOOD.preset(true, 0);
  gb->preset_t1t2(0,0);

  gb->CPU_PIN_CLKREQ.preset(true, 0);
  gb->CPU_PIN_RD.preset(true, 0);
  gb->CPU_PIN_WR.preset(true, 0);
  gb->CPU_PIN_ADDR_VALID.preset(true, 1);
  gb->CPU_PIN5.preset(true, 0);
  gb->CPU_PIN6.preset(true, 0);
  */

  /*
  gb->int_reg.CPU_PIN_ACK_SERIAL.preset(true, 0);
  gb->int_reg.CPU_PIN_ACK_STAT.preset(true, 0);
  gb->int_reg.CPU_PIN_ACK_VBLANK.preset(true, 0);
  gb->int_reg.CPU_PIN_ACK_TIMER.preset(true, 0);
  gb->int_reg.CPU_PIN_ACK_JOYPAD.preset(true, 0);
  */

  //gb->ext_pins_in.preset();

  /*
  cycle();
  gb->SYS_PIN_RST.preset(true, 0);
  cycle();
  gb->SYS_PIN_CLK_GOOD.preset(true, 1);
  cycle();
  gb->CPU_PIN_CLKREQ.preset(true, 1);
  cycle();
  */
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

#if 0
void GateBoy::render_frame(int /*screen_w*/, int /*screen_h*/, TextPainter& text_painter) {
  //uint64_t begin = SDL_GetPerformanceCounter();

  Schematics::SchematicTop& top = *state_manager.state();

  text_painter.dprintf(" ----- SYS_REG -----\n");
  text_painter.dprintf("PHASE    %08d\n", top.phase_counter);

  int p = top.phase_counter & 7;
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
  //gb.clk_reg.dump_regs(text_painter);
  //.dump_regs(text_painter);
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

  //gb.pix_pipe.dump_regs(text_painter);
  //gb.sst_reg.dump_regs(text_painter);
  //gb.pix_pipe.dump_regs(text_painter);
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
#endif

//-----------------------------------------------------------------------------
