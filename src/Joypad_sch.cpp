// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool RESET2;
extern bool FF00WR;
extern bool FF00RD;
extern bool BURO_Q;
extern bool BEDO;
extern bool FROM_CPU;

extern bool FF60_D0;


//----------
// outputs

//----------
// registers

bool KYME_Q, KYME_CLK;
bool KUKO_Q, KUKO_CLK;
bool KERU_Q, KERU_CLK;
bool JALE_Q, JALE_CLK;
bool KENA_Q, KENA_CLK;
bool KOLO_L;
bool KEJA_L;
bool KEVU_L;
bool KAPA_L;
bool KELY_Q, KELY_CLK;
bool COFY_Q, COFY_CLK;
bool JUTE_Q, JUTE_CLK;
bool KECY_Q, KECY_CLK;

//-----------------------------------------------------------------------------

void tick_joypad() {
  bool JEVA = not(BURO_Q);
  bool KORE = nand(KERU_Q, BURO_Q);
  bool KYWE = nor(JEVA, KERU_Q);
  bool KORY = nand(KYME_Q, FF60_D0);
  bool KURA = not(FF60_D0);
  bool KALE = nor(KYME_Q, KURA);
  bool KASY = nor(FF60_D0, KURA);
  bool KYHU = nand(FF60_D0, JALE_Q);
  bool BYZO = not(FF00RD);

  if (BYZO) {
    KOLO_L = P13_C;
    KEJA_L = P12_C;
    KEVU_L = P10_C;
    KAPA_L = P11_C;
  }

  bool JEKU = not(KOLO_L);
  bool KUVE = not(KEJA_L);
  bool KEMA = not(KEVU_L);
  bool KURO = not(KAPA_L);
  bool KOCE = not(!KELY_Q);
  bool CUDY = not(!COFY_Q);

  if (BYZO) {
    D3 = JEKU;
    D2 = KUVE;
    D0 = KEMA;
    D1 = KURO;
    D4 = KOCE;
    D5 = CUDY;
  }

  bool KARU = or(KURA, !KELY_Q);

  bool CELA = or(!COFY_Q, KURA);
  bool KOLE = nand(JUTE_Q, FF60_D0);
  bool KYBU = nor(JUTE_Q, KURA);
  bool KYTO = nand(KECY_Q, FF60_D0);
  bool KABU = nor(KECY_Q, KURA);
  
  P10_B = KOLE;
  P10_D = KYBU;

  P11_B = KYTO;
  P11_D = KABU;

  P12_A = KYHU;
  P12_D = KASY;

  P13_A = KORY;
  P13_C = KALE;

  P14_A = KARU;
  P14_B = !KELY_Q;

  P15_A = CELA;

  bool AXYN = not(BEDO);
  bool ADYR = not(AXYN);
  bool APYS = nor(FROM_CPU, ADYR);
  bool AFOP = not(APYS);

  bool ANOC = not(P10_B);
  bool AJEC = not(P10_B);
  bool ARAR = not(P10_B);
  bool BENU = not(P10_B);
  bool AKAJ = not(P10_B);
  bool ASUZ = not(P10_B);
  bool ATAJ = not(P10_B);
  bool BEDA = not(P10_B);

  if (AFOP) {
    D0 = ANOC;
    D2 = AJEC;
    D6 = ARAR;
    D4 = BENU;
    D5 = AKAJ;
    D3 = ASUZ;
    D1 = ATAJ;
    D7 = BEDA;
  }

  // unused
  (void)KORE;
  (void)KYWE;

}