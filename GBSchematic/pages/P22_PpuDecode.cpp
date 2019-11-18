#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P22_PpuDecode_tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {
  /*p22.XALY*/ c.p22.ADDR_0x00xxxx = nor(b.A07, b.A05, b.A04);
  /*p22.WUTU*/ c.p22.FF4Xn         = nand(b.sys.ADDR_FFXX, b.A06, b.p22.ADDR_0x00xxxx);
  /*p22.WERO*/ c.p22.FF4X          = not(b.p22.FF4Xn);
  
  /*p22.XOLA*/ c.p22.A00n = not(b.A00);
  /*p22.XENO*/ c.p22.A01n = not(b.A01);
  /*p22.XUSY*/ c.p22.A02n = not(b.A02);
  /*p22.XERA*/ c.p22.A03n = not(b.A03);

  /*p22.WADO*/ c.p22.A00 = not(b.p22.A00n);
  /*p22.WESA*/ c.p22.A01 = not(b.p22.A01n);
  /*p22.WALO*/ c.p22.A02 = not(b.p22.A02n);
  /*p22.WEPO*/ c.p22.A03 = not(b.p22.A03n);

  /*p22.WORU*/ c.p22.FF40n = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01n, b.p22.A02n, b.p22.A03n);
  /*p22.WOFA*/ c.p22.FF41n = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01n, b.p22.A02n, b.p22.A03n);
  /*p22.WEBU*/ c.p22.FF42n = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01,  b.p22.A02n, b.p22.A03n);
  /*p22.WAVU*/ c.p22.FF43n = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01,  b.p22.A02n, b.p22.A03n);
  /*p22.WYLE*/ c.p22.FF44n = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01n, b.p22.A02,  b.p22.A03n);
  /*p22.WETY*/ c.p22.FF45n = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01n, b.p22.A02,  b.p22.A03n);
  /*p22.WATE*/ c.p22.FF46n = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01,  b.p22.A02,  b.p22.A03n);
  /*p22.WYBO*/ c.p22.FF47n = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01,  b.p22.A02,  b.p22.A03n);
  /*p22.WETA*/ c.p22.FF48n = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01n, b.p22.A02n, b.p22.A03);
  /*p22.VAMA*/ c.p22.FF49n = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01n, b.p22.A02n, b.p22.A03);
  /*p22.WYVO*/ c.p22.FF4An = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01,  b.p22.A02n, b.p22.A03);
  /*p22.WAGE*/ c.p22.FF4Bn = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01,  b.p22.A02n, b.p22.A03);

  /*p22.VOCA*/ c.p22.FF40 = not(b.p22.FF40n);
  /*p22.VARY*/ c.p22.FF41 = not(b.p22.FF41n);
  /*p22.XARO*/ c.p22.FF42 = not(b.p22.FF42n);
  /*p22.XAVY*/ c.p22.FF43 = not(b.p22.FF43n);
  /*p22.XOGY*/ c.p22.FF44 = not(b.p22.FF44n);
  /*p22.XAYU*/ c.p22.FF45 = not(b.p22.FF45n);
  /*p22.XEDA*/ c.p22.FF46 = not(b.p22.FF46n);
  /*p22.WERA*/ c.p22.FF47 = not(b.p22.FF47n);
  /*p22.XAYO*/ c.p22.FF48 = not(b.p22.FF48n);
  /*p22.TEGO*/ c.p22.FF49 = not(b.p22.FF49n);
  /*p22.VYGA*/ c.p22.FF4A = not(b.p22.FF4An);
  /*p22.VUMY*/ c.p22.FF4B = not(b.p22.FF4Bn);
}