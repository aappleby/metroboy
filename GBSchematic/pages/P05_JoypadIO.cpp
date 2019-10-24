#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P5_Joypad_IO {
  struct Input {
    bool BEDO;
    bool BURO_Q;
    bool RESET2;
    bool FROM_CPU;
    bool SER_OUT;

    bool FF00RD;
    bool FF00WR;
    bool FF60_D0;

    bool P10_C;
    bool P11_C;
    bool P12_C;
    bool P13_C;

    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool SOUT;
    bool P10_B; // this signal is weeeeird
    bool P10_D;
    bool P11_B;
    bool P11_D;
    bool P12_A;
    bool P12_D;
    bool P13_A;
    bool P13_C;
    bool P14_A;
    bool P14_B;
    bool P15_A;
    
    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // FF00 JOYP
  reg JUTE,KECY,JALE,KYME,KELY,COFY,KUKO,KERU;

  reg KOLO,KEJA,KEVU,KAPA;

  void tick(const Input& in, Output& out) {
    wire JUTE_Q = JUTE.tock(in.FF00WR, in.RESET2, in.D0);
    wire KECY_Q = JUTE.tock(in.FF00WR, in.RESET2, in.D1);
    wire JALE_Q = JUTE.tock(in.FF00WR, in.RESET2, in.D2);
    wire KYME_Q = JUTE.tock(in.FF00WR, in.RESET2, in.D3);
    wire KELY_Q = KELY.tock(in.FF00WR, in.RESET2, in.D4);
    wire COFY_Q = COFY.tock(in.FF00WR, in.RESET2, in.D5);
    wire KUKO_Q = JUTE.tock(in.FF00WR, in.RESET2, in.D6);
    wire KERU_Q = JUTE.tock(in.FF00WR, in.RESET2, in.D7);

    wire KURA = not(in.FF60_D0);

    wire KOLE = nand(JUTE_Q, in.FF60_D0);
    wire KYBU = nor(JUTE_Q, KURA);
    out.P10_B = KOLE;
    out.P10_D = KYBU;

    wire KYTO = nand(KECY_Q, in.FF60_D0);
    wire KABU = nor(KECY_Q, KURA);
    out.P11_B = KYTO;
    out.P11_D = KABU;

    wire KYHU = nand(in.FF60_D0, JALE_Q);
    wire KASY = nor(in.FF60_D0, KURA);
    out.P12_A = KYHU;
    out.P12_D = KASY;

    wire KORY = nand(KYME_Q, in.FF60_D0);
    wire KALE = nor(KYME_Q, KURA);
    out.P13_A = KORY;
    out.P13_C = KALE;

    wire KARU = or(KURA, !KELY_Q);
    out.P14_A = KARU;
    out.P14_B = !KELY_Q;

    wire CELA = or(!COFY_Q, KURA);
    out.P15_A = CELA;

    wire KENA = mux2(KUKO_Q, in.SER_OUT, in.BURO_Q);
    out.SOUT = KENA;

    wire KORE = nand(KERU_Q, in.BURO_Q);
    wire JEVA = not(in.BURO_Q);
    wire KYWE = nor(JEVA, KERU_Q);
    (void)KORE; // unused in schematic
    (void)KYWE; // unused in schematic

    //----------

    wire BYZO   = not(in.FF00RD);
    wire KOLO_Q = KOLO.latch(BYZO, in.P13_C);
    wire KEJA_Q = KEJA.latch(BYZO, in.P12_C);
    wire KEVU_Q = KEVU.latch(BYZO, in.P10_C);
    wire KAPA_Q = KAPA.latch(BYZO, in.P11_C);

    wire JEKU = not(KOLO_Q); // inversion?
    wire KUVE = not(KEJA_Q);
    wire KEMA = not(KEVU_Q);
    wire KURO = not(KAPA_Q);
    wire KOCE = not(!KELY_Q);
    wire CUDY = not(!COFY_Q);

    if (BYZO) {
      out.D_OE = true;
      out.D0 = KEMA;
      out.D1 = KURO;
      out.D2 = KUVE;
      out.D3 = JEKU;
      out.D4 = KOCE;
      out.D5 = CUDY;
    }

    //----------

    wire AXYN = not(in.BEDO);
    wire ADYR = not(AXYN);
    wire APYS = nor(in.FROM_CPU, ADYR);
    wire AFOP = not(APYS);

    wire ANOC = not(out.P10_B);
    wire AJEC = not(out.P10_B);
    wire ARAR = not(out.P10_B);
    wire BENU = not(out.P10_B);
    wire AKAJ = not(out.P10_B);
    wire ASUZ = not(out.P10_B);
    wire ATAJ = not(out.P10_B);
    wire BEDA = not(out.P10_B);

    if (AFOP) {
      out.D_OE = true;
      out.D0 = ANOC;
      out.D2 = AJEC;
      out.D6 = ARAR;
      out.D4 = BENU;
      out.D5 = AKAJ;
      out.D3 = ASUZ;
      out.D1 = ATAJ;
      out.D7 = BEDA;
    }
  }
};