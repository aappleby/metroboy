// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "MemBus.h"
#include "ExtBus.h"
#include "Clocks.h"
#include "CpuBus.h"

//----------
// inputs

extern bool FF60_D0;
extern bool SER_OUT;
extern bool BEDO;

extern reg BURO;

//----------
// outputs

bool SOUT; // unused?

//----------
// registers

bool KYME_Q, KYME_CLK;
bool KUKO_Q, KUKO_CLK;
bool KERU_Q, KERU_CLK;
bool JALE_Q, JALE_CLK;
bool KELY_Q, KELY_CLK;
bool COFY_Q, COFY_CLK;
bool JUTE_Q, JUTE_CLK;
bool KECY_Q, KECY_CLK;

bool KOLO_L;
bool KEJA_L;
bool KEVU_L;
bool KAPA_L;

//-----------------------------------------------------------------------------

void tick_joypad() {
  bool JEVA = not(BURO.q());
  bool KORE = nand(KERU_Q, BURO.q());
  bool KYWE = nor(JEVA, KERU_Q);
  bool KORY = nand(KYME_Q, FF60_D0);
  bool KURA = not(FF60_D0);
  bool KALE = nor(KYME_Q, KURA);
  bool KASY = nor(FF60_D0, KURA);
  bool KYHU = nand(FF60_D0, JALE_Q);
  bool BYZO = not(FF00RD);

  bool KENA = mux2(KUKO_Q, SER_OUT, BURO.q());
  SOUT = KENA;

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

 
  (void)KORE; // unused in schematic
  (void)KYWE; // unused in schematic

  //----------
  // registers

  bool KYME_Q_ = KYME_Q;
  bool KUKO_Q_ = KUKO_Q;
  bool KERU_Q_ = KERU_Q;
  bool JALE_Q_ = JALE_Q;
  bool KELY_Q_ = KELY_Q;
  bool COFY_Q_ = COFY_Q;
  bool JUTE_Q_ = JUTE_Q;
  bool KECY_Q_ = KECY_Q;

  if (KYME_CLK && !FF00WR) KYME_Q_ = D3;
  if (KUKO_CLK && !FF00WR) KUKO_Q_ = D6;
  if (KERU_CLK && !FF00WR) KERU_Q_ = D7;
  if (JALE_CLK && !FF00WR) JALE_Q_ = D2;
  if (KELY_CLK && !FF00WR) KELY_Q_ = D4;
  if (COFY_CLK && !FF00WR) COFY_Q_ = D6;
  if (JUTE_CLK && !FF00WR) JUTE_Q_ = D0;
  if (KECY_CLK && !FF00WR) KECY_Q_ = D1;
  
  if (!RESET2) KYME_Q_ = 0;
  if (!RESET2) KUKO_Q_ = 0;
  if (!RESET2) KERU_Q_ = 0;
  if (!RESET2) JALE_Q_ = 0;
  if (!RESET2) KELY_Q_ = 0;
  if (!RESET2) COFY_Q_ = 0;
  if (!RESET2) JUTE_Q_ = 0;
  if (!RESET2) KECY_Q_ = 0;

  KYME_Q = KYME_Q_;
  KUKO_Q = KUKO_Q_;
  KERU_Q = KERU_Q_;
  JALE_Q = JALE_Q_;
  KELY_Q = KELY_Q_;
  COFY_Q = COFY_Q_;
  JUTE_Q = JUTE_Q_;
  KECY_Q = KECY_Q_;

  KYME_CLK = FF00WR;
  KUKO_CLK = FF00WR;
  KERU_CLK = FF00WR;
  JALE_CLK = FF00WR;
  KELY_CLK = FF00WR;
  COFY_CLK = FF00WR;
  JUTE_CLK = FF00WR;
  KECY_CLK = FF00WR;

}