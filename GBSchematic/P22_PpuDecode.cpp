#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P22_PpuDecode {
  struct Input {
    bool FFXX;
    bool A0,A1,A2,A3,A4,A5,A6,A7;
  };

  struct Output {
    bool FF40;
    bool FF41;
    bool FF42;
    bool FF43;
    bool FF44;
    bool FF45;
    bool FF46;
    bool FF47;
    bool FF48;
    bool FF49;
    bool FF4A;
    bool FF4B;
  };

  void tick_ppudecode(const Input& in, Output& out) {
    bool XALY = nor(in.A7, in.A5, in.A4);
    bool WUTU = nand(in.FFXX, in.A6, XALY);
    bool WERO = not(WUTU);

    bool XOLA = not(in.A0);
    bool XENO = not(in.A1);
    bool XUSY = not(in.A2);
    bool XERA = not(in.A3);
    bool WADO = not(XOLA);
    bool WESA = not(XENO);
    bool WALO = not(XUSY);
    bool WEPO = not(XERA);

    bool WORU = nand(WERO, XOLA, XENO, XUSY, XERA); // FF40
    bool WOFA = nand(WERO, WADO, XENO, XUSY, XERA); // FF41
    bool WEBU = nand(WERO, XOLA, WESA, XUSY, XERA); // FF42
    bool WAVU = nand(WERO, WADO, WESA, XUSY, XERA); // FF43
    bool WYLE = nand(WERO, XOLA, XENO, WALO, XERA); // FF44
    bool WETY = nand(WERO, WADO, XENO, WALO, XERA); // FF45
    bool WATE = nand(WERO, XOLA, WESA, WALO, XERA); // FF46
    bool WYBO = nand(WERO, WADO, WESA, WALO, XERA); // FF47
    bool WETA = nand(WERO, XOLA, XENO, XUSY, WEPO); // FF48
    bool VAMA = nand(WERO, WADO, XENO, XUSY, WEPO); // FF49
    bool WYVO = nand(WERO, XOLA, WESA, XUSY, WEPO); // FF4A
    bool WAGE = nand(WERO, WADO, WESA, XUSY, WEPO); // FF4B

    bool VOCA = not(WORU);
    bool VARY = not(WOFA);
    bool XARO = not(WEBU);
    bool XAVY = not(WAVU);
    bool XOGY = not(WYLE);
    bool XAYU = not(WETY);
    bool XEDA = not(WATE);
    bool WERA = not(WYBO);
    bool XAYO = not(WETA);
    bool TEGO = not(VAMA);
    bool VYGA = not(WYVO);
    bool VUMY = not(WAGE);

    out.FF40 = VOCA;
    out.FF41 = VARY;
    out.FF42 = XARO;
    out.FF43 = XAVY;
    out.FF44 = XOGY;
    out.FF45 = XAYU;
    out.FF46 = XEDA;
    out.FF47 = WERA;
    out.FF48 = XAYO;
    out.FF49 = TEGO;
    out.FF4A = VYGA;
    out.FF4B = VUMY;
  }
};