#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P22_PpuDecode::tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {
  c.p22.XALY = nor(b.A07, b.A05, b.A04);
  c.p22.WUTU = nand(b.p07.ADDR_FFXX, b.A06, b.p22.XALY);
  c.p22.WERO = not(b.p22.WUTU);

  c.p22.XOLA = not(b.A00);
  c.p22.XENO = not(b.A01);
  c.p22.XUSY = not(b.A02);
  c.p22.XERA = not(b.A03);
  c.p22.WADO = not(b.p22.XOLA);
  c.p22.WESA = not(b.p22.XENO);
  c.p22.WALO = not(b.p22.XUSY);
  c.p22.WEPO = not(b.p22.XERA);

  c.p22.WORU = nand(b.p22.WERO, b.p22.XOLA, b.p22.XENO, b.p22.XUSY, b.p22.XERA); // FF40
  c.p22.WOFA = nand(b.p22.WERO, b.p22.WADO, b.p22.XENO, b.p22.XUSY, b.p22.XERA); // FF41
  c.p22.WEBU = nand(b.p22.WERO, b.p22.XOLA, b.p22.WESA, b.p22.XUSY, b.p22.XERA); // FF42
  c.p22.WAVU = nand(b.p22.WERO, b.p22.WADO, b.p22.WESA, b.p22.XUSY, b.p22.XERA); // FF43
  c.p22.WYLE = nand(b.p22.WERO, b.p22.XOLA, b.p22.XENO, b.p22.WALO, b.p22.XERA); // FF44
  c.p22.WETY = nand(b.p22.WERO, b.p22.WADO, b.p22.XENO, b.p22.WALO, b.p22.XERA); // FF45
  c.p22.WATE = nand(b.p22.WERO, b.p22.XOLA, b.p22.WESA, b.p22.WALO, b.p22.XERA); // FF46
  c.p22.WYBO = nand(b.p22.WERO, b.p22.WADO, b.p22.WESA, b.p22.WALO, b.p22.XERA); // FF47
  c.p22.WETA = nand(b.p22.WERO, b.p22.XOLA, b.p22.XENO, b.p22.XUSY, b.p22.WEPO); // FF48
  c.p22.VAMA = nand(b.p22.WERO, b.p22.WADO, b.p22.XENO, b.p22.XUSY, b.p22.WEPO); // FF49
  c.p22.WYVO = nand(b.p22.WERO, b.p22.XOLA, b.p22.WESA, b.p22.XUSY, b.p22.WEPO); // FF4A
  c.p22.WAGE = nand(b.p22.WERO, b.p22.WADO, b.p22.WESA, b.p22.XUSY, b.p22.WEPO); // FF4B

  c.p22.VOCA = not(b.p22.WORU);
  c.p22.VARY = not(b.p22.WOFA);
  c.p22.XARO = not(b.p22.WEBU);
  c.p22.XAVY = not(b.p22.WAVU);
  c.p22.XOGY = not(b.p22.WYLE);
  c.p22.XAYU = not(b.p22.WETY);
  c.p22.XEDA = not(b.p22.WATE);
  c.p22.WERA = not(b.p22.WYBO);
  c.p22.XAYO = not(b.p22.WETA);
  c.p22.TEGO = not(b.p22.VAMA);
  c.p22.VYGA = not(b.p22.WYVO);
  c.p22.VUMY = not(b.p22.WAGE);
}