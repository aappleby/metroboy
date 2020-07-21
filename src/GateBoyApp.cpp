#include "GateBoyApp.h"

#include "Debug.h"

#include <chrono>

using namespace Schematics;

#pragma warning(disable:4702)

//-----------------------------------------------------------------------------

GateBoyApp::~GateBoyApp() {
}

const char* GateBoyApp::get_title() {
  return "GateBoyApp";
}

//----------------------------------------

void GateBoyApp::init() {
  grid_painter.init();
  text_painter.init();

  auto top_step = [this](Schematics::SchematicTop* top) {
    top->clk_reg.set_clk_a(1);
    top->clk_reg.set_clk_b(top->phase_count & 1);
    phase(top, {0}, false);
  };
  state_manager.init(top_step);
}

void GateBoyApp::close() {
}

void GateBoyApp::update(double delta) {
  (void)delta;
}

void GateBoyApp::render_frame(Viewport view) {
  grid_painter.render(view);

  auto top = get_top();

  StringDumper dumper;

  top->clk_reg.dump(dumper);
  top->cpu_bus.dump(dumper);
  top->ext_bus.dump(dumper);
  top->vram_bus.dump(dumper);
  top->oam_bus.dump(dumper);

  text_painter.render(view, dumper.s.c_str(), 0, 0);

  dumper.clear();
  top->dma_reg.dump(dumper);
  top->int_reg.dump(dumper);

  text_painter.render(view, dumper.s.c_str(), 256, 0);

#if 0
  //uint64_t begin = SDL_GetPerformanceCounter();

  Schematics::SchematicTop& top = *state_manager.state();

  text_painter.dprintf(" ----- SYS_REG -----\n");
  text_painter.dprintf("PHASE    %08d\n", top.phase_rate_n);

  int p = top.phase_rate_n & 7;
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
#endif

}

void GateBoyApp::render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------


#if 0
uint64_t phase(SchematicTop* top, Req req, bool verbose) {
  uint64_t phase_hash;

  for (top->pass_count = 0; top->pass_count < 256; top->pass_count++) {
    top->cpu_bus.set_cpu_req(req);
    top->vram_bus.set_vram_data(0);
    top->oam_bus.set_oam_data(0, 0);
    top->ext_bus.set_ext_data(0);
    top->joypad.set_buttons(0);
    
    top->tick();
    uint64_t pass_hash = top->commit();
    
    if (pass_hash.h == phase_hash.h) break;
    phase_hash = pass_hash;
    if (top->pass_count == 199) printf("stuck!\n");
    CHECK_N(top->pass_count == 200);
  }

  if (verbose) {
    printf("Phase %08d %c pass %02d CLK_GOOD %d CLK %d RST %d phz %d%d%d%d vid %d%d%d %d CPU_START %d CPU_RDY %d DIV %05d AFER %d ASOL %d\n",
      top->phase_count,
      'A' + (top->phase_count & 7),
      top->pass_count,
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
      top->clk_reg._CPU_PIN_STARTp(),
      top->clk_reg.CPU_PIN_READYp(),
      top->tim_reg.get_div(),
      top->clk_reg.AFER_SYS_RSTp(),
      top->clk_reg.ASOL_POR_DONEn()
      //hash.h);
      );
  }

  return phase_hash;
}


uint64_t run(SchematicTop* top, int phase_count, Req req, bool verbose) {
  uint64_t phase_hash;
  for (int i = 0; i < phase_count; i++) {
    top->phase_count++;
    wire CLK = (top->phase_count & 1) & (top->clk_reg.get_clk_a());
    top->clk_reg.set_clk_b(CLK);
    phase_hash = phase(top, req, verbose);
  }
  return phase_hash;
}

//-----------------------------------------------------------------------------

void test_reset_timing(int phase_a, int phase_b, int phase_c, int phase_d) {
  SchematicTop* top = new SchematicTop();

  top->clk_reg.set_cpu_ready(0);
  top->ext_bus.set_ext_rdwr(0, 0);

  top->clk_reg.set_t1t2(0,0);

  uint64_t phase_hash;

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

  while(!top->clk_reg._CPU_PIN_STARTp()) {
    run(top, 1, req, false);
  }

  // Ack the start request and run another 24 phases.
  // We should see AFER (global reset) clear and the video clocks start up.
  // FIXME why are the video clocks not running...

  top->clk_reg.set_cpu_ready(1);
  run(top, phase_d, req, false);

  top->clk_reg.set_cpu_ready(0);
  if (top->clk_reg.AFER_SYS_RSTp() || top->clk_reg.ASOL_POR_DONEn()) {
    printf("\nX %d %d %d %d\n", phase_a, phase_b, phase_c, phase_d);
  }
  else {
    printf(".");
  }

  //run(top, 1, req, true);
}

/*
for (int phase_a = 0; phase_a <= 8; phase_a++) 
for (int phase_b = 1; phase_b <= 8; phase_b++)
for (int phase_c = 0; phase_c <= 8; phase_c++)
for (int phase_d = 8; phase_d <= 16; phase_d++)
  test_reset_timing(phase_a, phase_b, phase_c, phase_d);
return 0;
*/

#endif

//-----------------------------------------------------------------------------

int GateBoyApp::main(int /*argc*/, char** /*argv*/) {
  printf("GateBoyApp sim starting!\n");

  GateBoyApp gateboy;
  gateboy.init();
  //gateboy.reset(0x100);

  auto top = gateboy.get_top();

  top->clk_reg.set_cpu_ready(0);
  top->ext_bus.set_ext_rdwr(0, 0);

  top->clk_reg.set_t1t2(0,0);

  // Just read DIV forever.
  Req req = {.addr = 0xFF04, .data = 0, .read = 1, .write = 0 };

  //Req req = {.addr = 0x9456, .data = 0x45, .read = 0, .write = 0 };

  //----------
  // 8 phases w/ reset high, clock not running.

  top->clk_reg.set_rst(1);
  top->clk_reg.set_clk_a(0);
  gateboy.run(top, 8, req, true);
  printf("\n");

  //----------
  // 8 phases w/ reset high, clock running.

  top->clk_reg.set_rst(1);
  top->clk_reg.set_clk_a(1);
  gateboy.run(top, 8, req, true);
  printf("\n");

  //----------
  // 8 phases w/ reset low, clock running.

  top->clk_reg.set_rst(0);
  top->clk_reg.set_clk_a(1);
  gateboy.run(top, 8, req, true);
  printf("\n");

  //----------
  // Force LCDC_EN on and run until we get the CPU start request (~32k mcycles)

  //gateboy.verbose = false;
  while(!top->clk_reg.CPU_PIN_STARTp()) {
    gateboy.run(top, 8, req, true);
    printf("\n");
  }
  //gateboy.verbose = true;

  //----------
  // Ack the start request and run another 24 phases.
  // We should see AFER (global reset) clear and the video clocks start up.
  // FIXME why are the video clocks not running...

  gateboy.run(top, 8, req, true);
  top->clk_reg.set_cpu_ready(1);
  gateboy.run(top, 16, req, true);
  printf("\n");

  //----------
  // Dump buses

#if 0
  for (int i = 0; i < 8; i++) {
    StringDumper d;
    
    top->cpu_bus.dump(d);
    top->vram_bus.dump(d);
    
    d.print();
    gateboy.run(top, 1, req, true);
  }
#endif

  //----------

  printf("Commit phase_hash   %016llx\n", top->phase_hash);
  printf("Combined phase_hash %016llx\n", top->combined_hash);
  printf("\n");

#if 0
#if _DEBUG
  const int iter_count = 16;
  const int phase_per_iter = 1024;
  const int warmup = 0;
#else
  const int iter_count = 74;
  const int phase_per_iter = 8192;
  const int warmup = 10;
#endif

  double phase_rate_sum1 = 0;
  double phase_rate_sum2 = 0;
  double phase_rate_n = 0;

  double pass_rate_sum1 = 0;
  double pass_rate_sum2 = 0;
  double pass_rate_n = 0;

  printf("Running perf test");
  for (int iter = 0; iter < iter_count; iter++) {
    top->phase_count = 0;
    top->pass_count = 0;

    auto start = std::chrono::high_resolution_clock::now();
    gateboy.run(top, phase_per_iter, req, false);
    auto finish = std::chrono::high_resolution_clock::now();

    if (iter >= warmup) {
      std::chrono::duration<double> elapsed = finish - start;
      double time = elapsed.count();

      double phase_rate = double(top->phase_count) / time;
      phase_rate_sum1 += phase_rate;
      phase_rate_sum2 += phase_rate * phase_rate;
      phase_rate_n++;

      double pass_rate = double(top->pass_count) / time;
      pass_rate_sum1 += pass_rate;
      pass_rate_sum2 += pass_rate * pass_rate;
      pass_rate_n++;
    }
    printf(".");
  }
  printf("Done\n");

  double phase_rate_mean     = phase_rate_sum1 / phase_rate_n;
  double phase_rate_variance = (phase_rate_sum2 / phase_rate_n) - (phase_rate_mean * phase_rate_mean);
  double phase_rate_sigma    = sqrt(phase_rate_variance);
  printf("Mean phase/sec %f sigma %f\n", phase_rate_mean, phase_rate_sigma);

  double pass_rate_mean     = pass_rate_sum1 / pass_rate_n;
  double pass_rate_variance = (pass_rate_sum2 / pass_rate_n) - (pass_rate_mean * pass_rate_mean);
  double pass_rate_sigma    = sqrt(pass_rate_variance);
  printf("Mean pass/sec %f sigma %f\n", pass_rate_mean, pass_rate_sigma);

  printf("Commit phase_hash   %016llx\n", top->phase_hash);
  printf("Combined phase_hash %016llx\n", top->combined_hash);

#endif

  return 0;
}

//------------------------------------------------------------------------------

void GateBoyApp::run(SchematicTop* top, int phase_count, Req req, bool verbose) {
  for (int i = 0; i < phase_count; i++) {
    top->phase_count++;
    wire CLK = (top->phase_count & 1) & (top->clk_reg.get_clk_a());
    top->clk_reg.set_clk_b(CLK);
    phase(top, req, verbose);
  }
}

//------------------------------------------------------------------------------

void GateBoyApp::phase(SchematicTop* top, Req req, bool verbose) {
  //printf("phase\n");

  uint64_t phase_hash = 0;
  int pass_count = 0;
  for (; pass_count < 256; pass_count++) {
    top->cpu_bus.set_cpu_req(req);
    top->vram_bus.set_vram_data(0);
    top->oam_bus.set_oam_data(0, 0);
    top->ext_bus.set_ext_data(0);
    top->joypad.set_buttons(0);
    
    top->tick();
    uint64_t pass_hash = top->commit();

    //printf("hash 0x%016llx\n", new_hash.h);
    
    if (pass_hash == phase_hash) break;
    phase_hash = pass_hash;
    if (pass_count == 199) printf("stuck!\n");
    CHECK_N(pass_count == 200);
  }

  top->phase_hash = phase_hash;
  combine_hash(top->combined_hash, phase_hash);

  if (verbose) {
    printf("Phase %c @ %08d:%02d phase_hash %016llx CLK_GOOD %d CLK %d RST %d phz %d%d%d%d vid %d%d%d BOMA %d CPU_START %d CPU_RDY %d DIV %05d TUBO %d AFER %d ASOL %d\n",
      'A' + (top->phase_count & 7),
      top->phase_count,
      pass_count,
      phase_hash,
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
      top->clk_reg.TUBO_CPU_READYn(),
      top->clk_reg.AFER_SYS_RSTp(),
      top->clk_reg.ASOL_POR_DONEn()
      //hash.h);
      );
  }
}

//-----------------------------------------------------------------------------

void GateBoyApp::reset(uint16_t /*new_pc*/) {
  state_manager.reset();
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
