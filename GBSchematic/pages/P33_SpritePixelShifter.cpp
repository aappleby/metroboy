#include "P33_SpritePixelShifter.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P33_SpritePixelShifter::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  // sprite x flip
  c.p33.POBE = mux2(b.MD4, b.MD3, b.p29.FLIP_X);
  c.p33.PACY = mux2(b.MD3, b.MD4, b.p29.FLIP_X);
  c.p33.PONO = mux2(b.MD5, b.MD2, b.p29.FLIP_X);
  c.p33.PUGU = mux2(b.MD2, b.MD5, b.p29.FLIP_X);
  c.p33.PUTE = mux2(b.MD7, b.MD0, b.p29.FLIP_X);
  c.p33.PULY = mux2(b.MD0, b.MD7, b.p29.FLIP_X);
  c.p33.PELO = mux2(b.MD6, b.MD1, b.p29.FLIP_X);
  c.p33.PAWE = mux2(b.MD1, b.MD6, b.p29.FLIP_X);

  // the schematic layout is slightly inconsistent, might have some of these swapped

  c.p33.PUDU = latch_pos(b.p29.XADO, b.p33.PUDU, b.p33.POBE);
  c.p33.RAMU = latch_pos(b.p29.PUCO, b.p33.RAMU, b.p33.POBE);
  c.p33.SELE = latch_pos(b.p29.PUCO, b.p33.SELE, b.p33.PACY);
  c.p33.SAJA = latch_pos(b.p29.XADO, b.p33.SAJA, b.p33.PACY);
  c.p33.MYTU = latch_pos(b.p29.XADO, b.p33.MYTU, b.p33.PONO);
  c.p33.MOFO = latch_pos(b.p29.PUCO, b.p33.MOFO, b.p33.PONO);
  c.p33.SUNY = latch_pos(b.p29.XADO, b.p33.SUNY, b.p33.PUGU);
  c.p33.SUTO = latch_pos(b.p29.PUCO, b.p33.SUTO, b.p33.PUGU);
  c.p33.PEFO = latch_pos(b.p29.XADO, b.p33.PEFO, b.p33.PUTE);
  c.p33.REWO = latch_pos(b.p29.PUCO, b.p33.REWO, b.p33.PUTE);
  c.p33.RYDU = latch_pos(b.p29.PUCO, b.p33.RYDU, b.p33.PULY);
  c.p33.SEGA = latch_pos(b.p29.XADO, b.p33.SEGA, b.p33.PULY);
  c.p33.PEBA = latch_pos(b.p29.XADO, b.p33.PEBA, b.p33.PELO);
  c.p33.ROKA = latch_pos(b.p29.PUCO, b.p33.ROKA, b.p33.PELO);
  c.p33.RAMA = latch_pos(b.p29.PUCO, b.p33.RAMA, b.p33.PAWE);
  c.p33.SEMO = latch_pos(b.p29.XADO, b.p33.SEMO, b.p33.PAWE);

  c.p33.LUBO = not(b.p33.PUDU);
  c.p33.SOLO = not(b.p33.RAMU);
  c.p33.VOBY = not(b.p33.SELE);
  c.p33.WERY = not(b.p33.SAJA);
  c.p33.LUMO = not(b.p33.MYTU);
  c.p33.LASE = not(b.p33.MOFO);
  c.p33.WURA = not(b.p33.SUNY);
  c.p33.WYCO = not(b.p33.SUTO);
  c.p33.LOZA = not(b.p33.PEFO);
  c.p33.RATA = not(b.p33.REWO);
  c.p33.SELU = not(b.p33.RYDU);
  c.p33.WAMY = not(b.p33.SEGA);
  c.p33.NUCA = not(b.p33.PEBA);
  c.p33.SYBO = not(b.p33.ROKA);
  c.p33.SERY = not(b.p33.RAMA);
  c.p33.SULU = not(b.p33.SEMO);

  c.p33.LUFY = nand(b.p33.LUBO, b.p34.ROBY);
  c.p33.MAME = nand(b.p33.PUDU, b.p34.ROBY);
  c.p33.REHU = nand(b.p33.SOLO, b.p34.ROBY);
  c.p33.RANO = nand(b.p33.RAMU, b.p34.ROBY);

  c.p33.WAXO = nand(b.p33.VOBY, b.p34.TYTA);
  c.p33.TYGA = nand(b.p33.SELE, b.p34.TYTA);
  c.p33.XATO = nand(b.p33.WERY, b.p34.TYTA);
  c.p33.VEXU = nand(b.p33.SAJA, b.p34.TYTA);

  c.p33.MAJO = nand(b.p33.LUMO, b.p34.LYKU);
  c.p33.MYXA = nand(b.p33.MYTU, b.p34.LYKU);
  c.p33.LYDE = nand(b.p33.LASE, b.p34.LYKU);
  c.p33.LELA = nand(b.p33.MOFO, b.p34.LYKU);

  c.p33.XEXU = nand(b.p33.WURA, b.p34.TYCO);
  c.p33.VABY = nand(b.p33.SUNY, b.p34.TYCO);
  c.p33.XOLE = nand(b.p33.WYCO, b.p34.TYCO);
  c.p33.VUME = nand(b.p33.SUTO, b.p34.TYCO);

  c.p33.MOFY = nand(b.p33.LOZA, b.p34.LESY);
  c.p33.MEZU = nand(b.p33.PEFO, b.p34.LESY);
  c.p33.PYZU = nand(b.p33.RATA, b.p34.LESY);
  c.p33.PABE = nand(b.p33.REWO, b.p34.LESY);

  c.p33.TULA = nand(b.p33.SELU, b.p34.XOVU);
  c.p33.TESO = nand(b.p33.RYDU, b.p34.XOVU);
  c.p33.XYVE = nand(b.p33.WAMY, b.p34.XOVU);
  c.p33.VUNE = nand(b.p33.SEGA, b.p34.XOVU);

  c.p33.MADA = nand(b.p33.NUCA, b.p34.LOTA);
  c.p33.MYTO = nand(b.p33.PEBA, b.p34.LOTA);
  c.p33.RUCA = nand(b.p33.SYBO, b.p34.LOTA);
  c.p33.RUSY = nand(b.p33.ROKA, b.p34.LOTA);

  c.p33.TABY = nand(b.p33.SERY, b.p34.SOKA);
  c.p33.TAPO = nand(b.p33.RAMA, b.p34.SOKA);
  c.p33.TUPE = nand(b.p33.SULU, b.p34.SOKA);
  c.p33.TUXA = nand(b.p33.SEMO, b.p34.SOKA);

  c.p33.NURO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.PABE, b.p33.PYZU, b.p33.NURO, b.chip.P10_B);
  c.p33.MASO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.MYTO, b.p33.MADA, b.p33.MASO, b.p33.NURO);
  c.p33.LEFE = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.LELA, b.p33.LYDE, b.p33.LEFE, b.p33.MASO);
  c.p33.LESU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.MAME, b.p33.LUFY, b.p33.LESU, b.p33.LEFE);
  c.p33.WYHO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.VEXU, b.p33.XATO, b.p33.WYHO, b.p33.LESU);
  c.p33.WORA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.VABY, b.p33.XEXU, b.p33.WORA, b.p33.WYHO);
  c.p33.VAFO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.TUXA, b.p33.TUPE, b.p33.VAFO, b.p33.WORA);
  c.p33.WUFY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.VUNE, b.p33.XYVE, b.p33.WUFY, b.p33.VAFO);

  c.p33.NYLU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.MEZU, b.p33.MOFY, b.p33.NYLU, b.chip.P10_B);
  c.p33.PEFU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.RUSY, b.p33.RUCA, b.p33.PEFU, b.p33.NYLU);
  c.p33.NATY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.MYXA, b.p33.MAJO, b.p33.NATY, b.p33.PEFU);
  c.p33.PYJO = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.RANO, b.p33.REHU, b.p33.PYJO, b.p33.NATY);
  c.p33.VARE = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.TYGA, b.p33.WAXO, b.p33.VARE, b.p33.PYJO);
  c.p33.WEBA = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.VUME, b.p33.XOLE, b.p33.WEBA, b.p33.VARE);
  c.p33.VANU = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.TAPO, b.p33.TABY, b.p33.VANU, b.p33.WEBA);
  c.p33.VUPY = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p33.TESO, b.p33.TULA, b.p33.VUPY, b.p33.VANU);
}
