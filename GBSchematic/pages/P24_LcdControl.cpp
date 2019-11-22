#include "P24_LcdControl.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P24_LcdControl_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // Main PPU clock, needs untangling

  /*p24.LOBY*/ c.p24.RENDERINGn = not(b.p21.RENDERING);

  /*p24.SACU*/ c.p24.CLKPIPE = nor(b.p24.SEGU, b.p27.ROXY);


  /*p24.SEGU*/ c.p24.SEGU = not(b.p24.TYFA);
  /*p24.TYFA*/   c.p24.TYFA = and(b.p24.SOCY, b.p24.POKY, b.p24.VYBO);
  /*p24.SOCY*/     c.p24.SOCY = not(b.p24.TOMU);
  /*p24.TOMU*/       c.p24.TOMU = not(b.p27.SYLO);
  /*p24.POKY*/     c.p24.POKY = or(b.p24.PYGO, b.p24.RENDERINGn);
  /*p24.VYBO*/     c.p24.VYBO = nor(b.spr.OAM_SCAN, b.p21.RENDER_DONEn, b.p21.CLK_xBxDxFxHc);

  /*p24.ROXO*/ c.p24.ROXO = not(b.p24.SEGU);


  /*p24.PYGO*/ c.p24.PYGO = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p21.RENDERING, b.p24.PYGO, b.p24.PORY);
  /*p24.PORY*/   c.p24.PORY = tock_pos(a.p21.CLK_xBxDxFxHc, b.p21.CLK_xBxDxFxHc, b.p24.NAFY, b.p24.PORY, b.p24.NYKA);
  /*p24.NAFY*/     c.p24.NAFY = nor(b.p27.MOSU, b.p24.RENDERINGn);
  /*p24.NYKA*/     c.p24.NYKA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p24.NAFY, b.p24.NYKA, b.p27.LYRY);

  /*p27.NUNY*/ c.p27.NUNY = and(!b.p27.NOPA, b.p27.PYNU);
  /*p27.NOPA*/ c.p27.NOPA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.sys.VID_RESETn1, b.p27.NOPA, b.p27.PYNU);

  /*p27.NYFO*/ c.p27.NYFO = not(b.p27.NUNY);
  /*p27.MOSU*/ c.p27.MOSU = not(b.p27.NYFO);

  /*p27.ROXY*/ c.p27.ROXY = or(b.p27.PAHA, b.p27.FINE_MATCH_TRIG);
  /*p27.PAHA*/   c.p27.PAHA = not(b.p21.RENDERING);

  /*p27.PUXA*/ c.p27.FINE_MATCH_SYNC1 = tock_pos(a.p24.ROXO,          b.p24.ROXO,          b.p21.RENDERING,  b.p27.FINE_MATCH_SYNC1, b.p27.FINE_MATCHn);
  /*p27.NYZE*/ c.p27.FINE_MATCH_SYNC2 = tock_pos(a.p21.CLK_xBxDxFxHa, b.p21.CLK_xBxDxFxHa, b.p21.RENDERING,  b.p27.FINE_MATCH_SYNC2, b.p27.FINE_MATCH_SYNC1);
  /*p27.POVA*/ c.p27.FINE_MATCH_TRIG  = and(b.p27.FINE_MATCH_SYNC1, !b.p27.FINE_MATCH_SYNC2);



  //----------
  // Control

  /*p21.POGU*/ c.chip.CPG = not(b.p21.RYNO);
  /*p21.RYNO*/   c.p21.RYNO = or(b.p21.SYGU, b.p21.LINE_DONEn);
  /*p21.SYGU*/     c.p21.SYGU = tock_pos(a.p21.SONO_CLK, b.p21.SONO_CLK, b.sys.VID_RESETn2, b.p21.SYGU, b.p21.TEGY);
  /*p21.TEGY*/       c.p21.TEGY = nand(b.p21.CNT_000n, b.p21.CNT_007n, b.p21.CNT_045n, b.p21.CNT_083n);
  /*p21.VOKU*/         c.p21.CNT_000n = nand(b.p21.CNT_6n, b.p21.CNT_5n, b.p21.CNT_4n, b.p21.CNT_3n, b.p21.CNT_2n, b.p21.CNT_1n, b.p21.CNT_0n); // 0000000 == 0
  /*p21.TOZU*/         c.p21.CNT_007n = nand(b.p21.CNT_6n, b.p21.CNT_5n, b.p21.CNT_4n, b.p21.CNT_3n, b.p21.CNT_2,  b.p21.CNT_1,  b.p21.CNT_0); // 0000111 == 7
  /*p21.TECE*/         c.p21.CNT_045n = nand(b.p21.CNT_6n, b.p21.CNT_5,  b.p21.CNT_4n, b.p21.CNT_3,  b.p21.CNT_2,  b.p21.CNT_1n, b.p21.CNT_0); // 0101101 == 45
  /*p21.TEBO*/         c.p21.CNT_083n = nand(b.p21.CNT_6,  b.p21.CNT_4n, b.p21.CNT_4,  b.p21.CNT_6n, b.p21.CNT_2n, b.p21.CNT_1,  b.p21.CNT_0); // 1010011 == 83

  // Clock
  /*p21.RYPO*/ c.p21.CP = not(b.p21.CPn);
  /*p21.SEMU*/   c.p21.CPn = or(b.p21.TOBA, b.p27.FINE_MATCH_TRIG);
  /*p21.TOBA*/     c.p21.TOBA = and(b.p24.CLKPIPE, b.p21.WUSA);
  /*p21.WUSA*/       c.p21.WUSA = or(b.p21.X_009, b.p21.WEGO);

  // Horizontal sync
  /*p24.RUZE*/ c.chip.ST = not(b.p24.POFY);
  /*p24.POFY*/   c.p24.POFY = not(b.p24.RUJU);
  /*p24.RUJU*/     c.p24.RUJU = or(b.p24.PAHO, b.sys.VID_RESET3, b.p24.POME);
  /*p24.PAHO*/        c.p24.PAHO = tock_pos(a.p24.ROXO, b.p24.ROXO, b.p21.RENDERING, b.p24.PAHO, b.p21.X3);
  /*p24.POME*/        c.p24.POME = nor(b.spr.AVAP, b.p24.POFY);


  // Data latch
  /*p24.KYMO*/ c.chip.CPL = not(b.p24.CPLn);
  /*p24.KAHE*/   c.p24.CPLn = amux2(b.p23.LCDC_EN, b.p24.KASA, b.p24.LCDC_ENn, b.p24.UMOB);
  /*p24.KASA*/     c.p24.KASA = not(b.p21.LINE_DONEa);
  /*p24.UMOB*/     c.p24.UMOB = not(b.sys.DIV_06n);

  // Alt signal?
  /*p24.KOFO*/ c.chip.FR  = not(b.p24.FRn);
  /*p24.KUPA*/   c.p24.FRn  = amux2(b.p23.LCDC_EN, b.p24.KEBO, b.p24.LCDC_ENn, b.p24.USEC);
  /*p24.KEBO*/     c.p24.KEBO = not(b.p24.MECO);
  /*p24.MECO*/       c.p24.MECO = not(b.p24.MAGU);
  /*p24.MAGU*/         c.p24.MAGU = xor(b.p21.NAPO, b.p24.LUCA);
  /*p21.NAPO*/           c.p21.NAPO = tock_pos(a.p21.LINE_144_SYNC, b.p21.LINE_144_SYNC, b.sys.VID_RESETn2, b.p21.NAPO, !b.p21.NAPO);
  /*p24.LUCA*/           c.p24.LUCA = tock_pos(a.p24.LINE_DONE,     b.p24.LINE_DONE,     b.sys.VID_RESETn2, b.p24.LUCA, !b.p24.LUCA);
  /*p24.LOFU*/             c.p24.LINE_DONE = not(b.p21.LINE_DONEn);
  /*p24.USEC*/     c.p24.USEC = not(b.sys.DIV_07n);

  // Vertical sync
  /*p24.MURE*/ c.chip.S = not(b.p24.LINE_000_SYNCn);
  /*p24.MEDA*/   c.p24.LINE_000_SYNCn = tock_pos(a.p21.LINE_DONE_DELAYn, b.p21.LINE_DONE_DELAYn, b.sys.VID_RESETn2, b.p24.LINE_000_SYNCn, b.p24.LINE_000n);
  /*p24.NERU*/     c.p24.LINE_000n = nor(b.p21.V0, b.p21.V1, b.p21.V2, b.p21.V3, b.p21.V4, b.p21.V5, b.p21.V6, b.p21.V7);

  /*p24.KEDY*/ c.p24.LCDC_ENn = not(b.p23.LCDC_EN);


  // Unused
  /*p24.LEBE*/ c.p24.LEBE = tock_pos(!a.p24.LUCA, !b.p24.LUCA, b.sys.VID_RESETn2, b.p24.LEBE, !b.p24.LEBE);
}