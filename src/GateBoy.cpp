#include "GateBoy.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

int GateBoy::main(int /*argc*/, char** /*argv*/) {
  printf("GateBoy sim starting\n");

  GateBoy gateboy;
  gateboy.init();
  //gateboy.reset(0x100);

  auto top = gateboy.top();

  top->CPU_PIN_READYp.set(0);
  top->EXT_PIN_WRp_C.set(0);
  top->EXT_PIN_RDn_C.set(0);
  top->CPU_PIN5.set(0);

  top->SYS_PIN_T1n.set(1);
  top->SYS_PIN_T2n.set(1);

  SignalHash hash;

  for (int i = 0; i < 2; i++) {
    hash = gateboy.mcycle(
      top,
      /*RST*/   1,
      /*CLK_GOOD*/ 0,
      /*addr*/  0,
      /*data*/  0,
      /*read*/  0,
      /*write*/ 0
    );
  }

  printf("\n");

  for (int i = 0; i < 2; i++) {
    hash = gateboy.mcycle(
      top,
      /*RST*/   1,
      /*CLK_GOOD*/ 1,
      /*addr*/  0,
      /*data*/  0,
      /*read*/  0,
      /*write*/ 0
    );
  }

  printf("\n");

  for (int i = 0; i < 2; i++) {
    hash = gateboy.mcycle(
      top,
      /*RST*/   0,
      /*CLK_GOOD*/ 1,
      /*addr*/  0,
      /*data*/  0,
      /*read*/  0,
      /*write*/ 0
    );
  }

  printf("\n");

  top->XONA_LCDC_EN.preset(1);

  gateboy.verbose = false;

  while(1) {
    wire RST = 0;
    wire CLK_GOOD = 1;
    wire CLK = (top->phase_counter & 1) & CLK_GOOD;
    uint16_t addr = 0;
    uint8_t data = 0;
    bool read = false;
    bool write = false;

    hash = gateboy.phase(top, RST, CLK_GOOD, CLK, addr, data, read, write);
    top->phase_counter++;

    if (top->tim_reg.get_div() == 65535) break;
  }

  gateboy.verbose = true;

  for (int phase = 0; phase < 24; phase++) {
    wire RST = 0;
    wire CLK_GOOD = 1;
    wire CLK = (top->phase_counter & 1) & CLK_GOOD;
    uint16_t addr = 0;
    uint8_t data = 0;
    bool read = false;
    bool write = false;

    if (top->CPU_PIN_STARTp.q()) {
      top->CPU_PIN_READYp.set(1);
    }

    hash = gateboy.phase(top, RST, CLK_GOOD, CLK, addr, data, read, write);
    top->phase_counter++;
  }

  return 0;
}

//-----------------------------------------------------------------------------

SignalHash GateBoy::mcycle(
  SchematicTop* top,
  bool RST,
  bool CLK_GOOD,
  uint16_t addr,
  uint8_t data,
  bool read,
  bool write)
{
  SignalHash hash;
  for (int i = 0; i < 8; i++) {
    wire CLK = (top->phase_counter & 1) & CLK_GOOD;
    hash = phase(top, RST, CLK_GOOD, CLK, addr, data, read, write);
    top->phase_counter++;
  }
  return hash;
}

//----------------------------------------

SignalHash GateBoy::tcycle(
  SchematicTop* top,
  bool RST,
  bool CLK_GOOD,
  uint16_t addr,
  uint8_t data,
  bool read,
  bool write)
{
  SignalHash hash;
  for (int i = 0; i < 2; i++) {
    wire CLK = (top->phase_counter & 1) & CLK_GOOD;
    hash = phase(top, RST, CLK_GOOD, CLK, addr, data, read, write);
    top->phase_counter++;
  }
  return hash;
}

//----------------------------------------

SignalHash GateBoy::phase(
  SchematicTop* top,
  bool RST,
  bool CLK_GOOD,
  bool CLK,
  uint16_t addr,
  uint8_t data,
  bool read,
  bool write)
{
  SignalHash hash;
  int pass_count = 0;
  for (; pass_count < 256; pass_count++) {
    SignalHash new_hash = pass(top, RST, CLK_GOOD, CLK, addr, data, read, write);
    if (new_hash.h == hash.h) break;
    hash = new_hash;
    if (pass_count == 199) printf("stuck!\n");
    if (pass_count == 200) __debugbreak();
  }

  if (verbose) {
    printf("Phase %08d %c pass %02d CLK_GOOD %d CLK %d RST %d phz %d%d%d%d vid %d%d%d BELE %d CPU_RDY %d DIV %05d TUBO %d ASOL %d AFER %d\n",  //hash 0x%016llx\n",
      top->phase_counter,
      'A' + (top->phase_counter & 7),
      pass_count,
      CLK_GOOD,
      CLK,
      RST,
      top->clk_reg.AFUR_ABCDxxxx.q(),
      top->clk_reg.ALEF_xBCDExxx.q(),
      top->clk_reg.APUK_xxCDEFxx.q(),
      top->clk_reg.ADYK_xxxDEFGx.q(),
      top->clk_reg.WUVU_xxCDxxGH.q(),
      top->clk_reg.VENA_xxxxEFGH.q(),
      top->clk_reg.WOSU_xBCxxFGx.q(),
      top->BELE_Axxxxxxx(),
      top->CPU_PIN_READYp.a.val,
      top->tim_reg.get_div(),
      top->rst_reg._TUBO_CPU_READYn.q(),
      top->rst_reg.ASOL_POR_DONEn.q(),
      top->rst_reg.AFER_SYS_RSTp.q()
      //hash.h);
      );
  }

  return hash;
}

//----------------------------------------

SignalHash GateBoy::pass(
  SchematicTop* top,
  bool RST,
  bool CLK_GOOD,
  bool CLK,
  uint16_t addr,
  uint8_t data,
  bool read,
  bool write)
{
  top->SYS_PIN_RSTp.set(RST);
  top->SYS_PIN_CLK_A.set(CLK_GOOD);
  top->SYS_PIN_CLK_B.set(CLK);

  top->set_cpu(addr, data, read, write);
  top->set_ext();
  top->set_joy(0);
  top->set_vram(0, 0);
  top->set_oam();
    
  SignalHash hash = top->tick();

  //printf("PASS COMPLETE\n");

  return hash;
}

//-----------------------------------------------------------------------------

void GateBoy::init() {
  auto top_step = [this](Schematics::SchematicTop* top) {
    bool CLK_GOOD = 1;
    bool CLK = top->phase_counter & 1;
    bool RST = 0;
    uint16_t addr = 0;
    uint8_t data = 0;
    bool read = 0;
    bool write = 0;
    phase(top, CLK_GOOD, CLK, RST, addr, data, read, write);
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
  //gb.rst_reg.dump_regs(text_painter);
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
#endif

//-----------------------------------------------------------------------------
