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
  bool XALY = nor(A7, A5, A4);
  bool WUTU = nand(FFXX, A6, XALY);
  bool WERO = not(WUTU);

  bool XOLA = not(A0);
  bool XENO = not(A1);
  bool XUSY = not(A2);
  bool XERA = not(A3);
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

  FF48 = XAYO;
  FF44 = XOGY;
  FF40 = VOCA;
  FF46 = XEDA;
  FF4A = VYGA;
  FF45 = XAYU;
  FF41 = VARY;
  FF43 = XAVY;
  FF4B = VUMY;
  FF42 = XARO;
  FF49 = TEGO;
  FF47 = WERA;
}