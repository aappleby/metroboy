#include "LCD.h"

#include "ExtBus.h"
#include "PPU.h"
#include "Clocks.h"
#include "Sprites.h"
#include "Window.h"
#include "LCD.h"

//-----------------------------------------------------------------------------

void LCD::tick(const Resets& rst, const Window& win, Clocks& clk) {
  wire NYKA_Q = NYKA.q();
  wire PORY_Q = PORY.q();
  wire PYGO_Q = PYGO.q();
  wire PAHO_Q = PAHO.q();
  wire LUCA_Q = LUCA.q();
  wire LEBE_Q = LEBE.q();
  wire VSYNC_Q = VSYNC.q();

  SEGU = not(and(win.SYLO, unk2(PYGO_Q, not(ppu.XYMU)), nor(spr.FEPO, ppu.WODU, win.MYVO)));
  ROXO = not(SEGU);

  ext.PIN_S = not(VSYNC_Q);

  // polarity?
  ext.PIN_CPL = amux2(ppu.LCDC_LCDEN, not(ppu.PURE),             not(ppu.LCDC_LCDEN), not(clk.FF04_D0n));
  ext.PIN_FR  = amux2(ppu.LCDC_LCDEN, xor(ppu.NAPO_OUT, LUCA_Q), not(ppu.LCDC_LCDEN), not(clk.FF04_D1n));

  // FIXME another logic loop...
  TOFU = not(rst.RESET_VIDEO);
  bool POME = nor(spr.AVAP, /*POFY*/false);
  bool RUJU = or(PAHO_Q, TOFU, POME);
  bool POFY = not(RUJU);
  bool RUZE = not(POFY);

  ext.PIN_ST = RUZE;
  // move to clocks.cpp
  clk.CLKPIPE = nor(SEGU, win.ROXY);

  bool NAFY = nor(win.MOSU, not(ppu.XYMU));
  NYKA.tock(clk.CLK_0246, NAFY, win.LYRY);
  PORY.tock(win.MYVO, NAFY, NYKA_Q);
  PYGO.tock(clk.CLK_0246, ppu.XYMU, PORY_Q);

  bool XYDO_Q = ppu.X_R3.q();
  PAHO.tock(ROXO, ppu.XYMU, XYDO_Q);

  bool RUTU_Q = ppu.RUTU.q();
  LUCA.tock(!RUTU_Q, rst.RESET_VIDEO, !LUCA_Q);
  LEBE.tock(!LUCA_Q, rst.RESET_VIDEO, !LEBE_Q);

  bool NYPE_Q = ppu.NYPE.q();

  bool NERU = !(ppu.V7 || ppu.V6 || ppu.V5 || ppu.V4 || ppu.V3 || ppu.V2 || ppu.V1); // biiig nor
  VSYNC.tock(NYPE_Q,    rst.RESET_VIDEO, NERU);
}