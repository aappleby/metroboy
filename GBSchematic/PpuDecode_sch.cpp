// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "MemBus.h"

//----------
// inputs

//----------
// outputs

//----------
// registers

//-----------------------------------------------------------------------------

void tick_ppudecode() {
  bool XALY = nor(mem.A7, mem.A5, mem.A4);
  bool WUTU = nand(FFXX, mem.A6, XALY);
  bool WERO = not(WUTU);

  bool XOLA = not(mem.A0);
  bool XENO = not(mem.A1);
  bool XUSY = not(mem.A2);
  bool XERA = not(mem.A3);
  bool WADO = not(XOLA);
  bool WESA = not(XENO);
  bool WALO = not(XUSY);
  bool WEPO = not(XERA);

  //                     XOLA  XENO  XUSY  XERA
  //                     WADO  WESA  WALO  WEPO

  bool WETA = nand(WERO, XOLA, XENO, XUSY, WEPO); // FF48
  bool WYLE = nand(WERO, XOLA, XENO, WALO, XERA); // FF44
  bool WORU = nand(WERO, XOLA, XENO, XUSY, XERA); // FF40
  bool WATE = nand(WERO, XOLA, WESA, WALO, XERA); // FF46
  bool WYVO = nand(WERO, XOLA, WESA, XUSY, WEPO); // FF4A
  bool WETY = nand(WERO, WADO, XENO, WALO, XERA); // FF45
  bool WOFA = nand(WERO, WADO, XENO, XUSY, XERA); // FF41
  bool WAVU = nand(WERO, WADO, WESA, XUSY, XERA); // FF43
  bool WAGE = nand(WERO, WADO, WESA, XUSY, WEPO); // FF4B
  bool WEBU = nand(WERO, XOLA, WESA, XUSY, XERA); // FF42
  bool VAMA = nand(WERO, WADO, XENO, XUSY, WEPO); // FF49
  bool WYBO = nand(WERO, WADO, WESA, WALO, XERA); // FF47

  bool XAYO = not(WETA);
  bool XOGY = not(WYLE);
  bool VOCA = not(WORU);
  bool XEDA = not(WATE);
  bool VYGA = not(WYVO);
  bool XAYU = not(WETY);
  bool VARY = not(WOFA);
  bool XAVY = not(WAVU);
  bool VUMY = not(WAGE);
  bool XARO = not(WEBU);
  bool TEGO = not(VAMA);
  bool WERA = not(WYBO);

  dec.FF48 = XAYO;
  dec.FF44 = XOGY;
  dec.FF40 = VOCA;
  dec.FF46 = XEDA;
  dec.FF4A = VYGA;
  dec.FF45 = XAYU;
  dec.FF41 = VARY;
  dec.FF43 = XAVY;
  dec.FF4B = VUMY;
  dec.FF42 = XARO;
  dec.FF49 = TEGO;
  dec.FF47 = WERA;
}