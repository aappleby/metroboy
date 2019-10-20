// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "Clocks.h"
#include "ExtBus.h"
#include "VramBus.h"
#include "PPU.h"
#include "Sprites.h"

//-----------------------------------------------------------------------------

void Sprites::tick_pixelshifter() {
  // sprite x flip
  bool POBE = mux2(vram.MD4, vram.MD3, spr.XONO);
  bool PACY = mux2(vram.MD3, vram.MD4, spr.XONO);
  bool PONO = mux2(vram.MD5, vram.MD2, spr.XONO);
  bool PUGU = mux2(vram.MD2, vram.MD5, spr.XONO);
  bool PUTE = mux2(vram.MD7, vram.MD0, spr.XONO);
  bool PULY = mux2(vram.MD0, vram.MD7, spr.XONO);
  bool PELO = mux2(vram.MD6, vram.MD1, spr.XONO);
  bool PAWE = mux2(vram.MD1, vram.MD6, spr.XONO);

  // the schematic layout is slightly inconsistent, might have some of these swapped

  bool PUDU_Q = PUDU.latch(spr.XADO, POBE);
  bool RAMU_Q = RAMU.latch(spr.PUCO, POBE);
  bool SELE_Q = SELE.latch(spr.PUCO, PACY);
  bool SAJA_Q = SAJA.latch(spr.XADO, PACY);
  bool MYTU_Q = MYTU.latch(spr.XADO, PONO);
  bool MOFO_Q = MOFO.latch(spr.PUCO, PONO);
  bool SUNY_Q = SUNY.latch(spr.XADO, PUGU);
  bool SUTO_Q = SUTO.latch(spr.PUCO, PUGU);
  bool PEFO_Q = PEFO.latch(spr.XADO, PUTE);
  bool REWO_Q = REWO.latch(spr.PUCO, PUTE);
  bool RYDU_Q = RYDU.latch(spr.PUCO, PULY);
  bool SEGA_Q = RYDU.latch(spr.XADO, PULY);
  bool PEBA_Q = PEBA.latch(spr.XADO, PELO);
  bool ROKA_Q = ROKA.latch(spr.PUCO, PELO);
  bool RAMA_Q = RAMA.latch(spr.PUCO, PAWE);
  bool SEMO_Q = SEMO.latch(spr.XADO, PAWE);

  bool LUBO = not(PUDU_Q);
  bool SOLO = not(RAMU_Q);
  bool VOBY = not(RAMU_Q);
  bool WERY = not(RAMU_Q);
  bool LUMO = not(RAMU_Q);
  bool LASE = not(RAMU_Q);
  bool WURA = not(RAMU_Q);
  bool WYCO = not(RAMU_Q);
  bool LOZA = not(RAMU_Q);
  bool RATA = not(RAMU_Q);
  bool SELU = not(RAMU_Q);
  bool WAMY = not(RAMU_Q);
  bool NUCA = not(RAMU_Q);
  bool SYBO = not(RAMU_Q);
  bool SERY = not(RAMU_Q);
  bool SULU = not(RAMU_Q);

  bool LUFY = nand(LUBO,   ppu.ROBY);
  bool MAME = nand(PUDU_Q, ppu.ROBY);
  bool REHU = nand(SOLO,   ppu.ROBY);
  bool RANO = nand(RAMU_Q, ppu.ROBY);

  bool WAXO = nand(VOBY,   ppu.TYTA);
  bool TYGA = nand(SELE_Q, ppu.TYTA);
  bool XATO = nand(WERY,   ppu.TYTA);
  bool VEXU = nand(SAJA_Q, ppu.TYTA);

  bool MAJO = nand(LUMO,   ppu.LYKU);
  bool MYXA = nand(MYTU_Q, ppu.LYKU);
  bool LYDE = nand(LASE,   ppu.LYKU);
  bool LELA = nand(MOFO_Q, ppu.LYKU);

  bool XEXU = nand(WURA,   ppu.TYCO);
  bool VABY = nand(SUNY_Q, ppu.TYCO);
  bool XOLE = nand(WYCO,   ppu.TYCO);
  bool VUME = nand(SUTO_Q, ppu.TYCO);

  bool MOFY = nand(LOZA,   ppu.LESY);
  bool MEZU = nand(PEFO_Q, ppu.LESY);
  bool PYZU = nand(RATA,   ppu.LESY);
  bool PABE = nand(REWO_Q, ppu.LESY);

  bool TULA = nand(SELU,   ppu.XOVU);
  bool TESO = nand(RYDU_Q, ppu.XOVU);
  bool XYVE = nand(WAMY,   ppu.XOVU);
  bool VUNE = nand(SEGA_Q, ppu.XOVU);

  bool MADA = nand(NUCA,   ppu.LOTA);
  bool MYTO = nand(PEBA_Q, ppu.LOTA);
  bool RUCA = nand(SYBO,   ppu.LOTA);
  bool RUSY = nand(ROKA_Q, ppu.LOTA);

  bool TABY = nand(SERY,   ppu.SOKA);
  bool TAPO = nand(RAMA_Q, ppu.SOKA);
  bool TUPE = nand(SULU,   ppu.SOKA);
  bool TUXA = nand(SEMO_Q, ppu.SOKA);

  bool NURO_Q = NURO.srtock(clk.CLKPIPE, PABE, PYZU, ext.P10_B);
  bool MASO_Q = MASO.srtock(clk.CLKPIPE, MYTO, MADA, NURO_Q);
  bool LEFE_Q = LEFE.srtock(clk.CLKPIPE, LELA, LYDE, MASO_Q);
  bool LESU_Q = LESU.srtock(clk.CLKPIPE, MAME, LUFY, LEFE_Q);
  bool WYHO_Q = WYHO.srtock(clk.CLKPIPE, VEXU, XATO, LESU_Q);
  bool WORA_Q = WORA.srtock(clk.CLKPIPE, VABY, XEXU, WYHO_Q);
  bool VAFO_Q = VAFO.srtock(clk.CLKPIPE, TUXA, TUPE, WORA_Q);
  bool WUFY_Q = WUFY.srtock(clk.CLKPIPE, VUNE, XYVE, VAFO_Q);

  bool NYLU_Q = NYLU.srtock(clk.CLKPIPE, MEZU, MOFY, ext.P10_B);
  bool PEFU_Q = PEFU.srtock(clk.CLKPIPE, RUSY, RUCA, NYLU_Q);
  bool NATY_Q = NATY.srtock(clk.CLKPIPE, MYXA, MAJO, PEFU_Q);
  bool PYJO_Q = PYJO.srtock(clk.CLKPIPE, RANO, REHU, NATY_Q);
  bool VARE_Q = VARE.srtock(clk.CLKPIPE, TYGA, WAXO, PYJO_Q);
  bool WEBA_Q = WEBA.srtock(clk.CLKPIPE, VUME, XOLE, VARE_Q);
  bool VANU_Q = VANU.srtock(clk.CLKPIPE, TAPO, TABY, WEBA_Q);
  bool VUPY_Q = VUPY.srtock(clk.CLKPIPE, TESO, TULA, VANU_Q);

  ppu.SPR_PIX_B_0 = NURO_Q;
  ppu.SPR_PIX_B_1 = MASO_Q;
  ppu.SPR_PIX_B_2 = LEFE_Q;
  ppu.SPR_PIX_B_3 = LESU_Q;
  ppu.SPR_PIX_B_4 = WYHO_Q;
  ppu.SPR_PIX_B_5 = WORA_Q;
  ppu.SPR_PIX_B_6 = VAFO_Q;
  ppu.SPR_PIX_B_7 = WUFY_Q;

  ppu.SPR_PIX_A_0 = NYLU_Q;
  ppu.SPR_PIX_A_1 = PEFU_Q;
  ppu.SPR_PIX_A_2 = NATY_Q;
  ppu.SPR_PIX_A_3 = PYJO_Q;
  ppu.SPR_PIX_A_4 = VARE_Q;
  ppu.SPR_PIX_A_5 = WEBA_Q;
  ppu.SPR_PIX_A_6 = VANU_Q;
  ppu.SPR_PIX_A_7 = VUPY_Q;
}