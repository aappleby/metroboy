// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "AddressDecoder.h"
#include "MemBus.h"
#include "ExtBus.h"
#include "Clocks.h"
#include "CpuBus.h"
#include "System.h"

//----------
// inputs

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
  bool JEVA = not(sys.BURO.q());
  bool KORE = nand(KERU_Q, sys.BURO.q());
  bool KYWE = nor(JEVA, KERU_Q);
  bool KORY = nand(KYME_Q, sys.FF60_D0);
  bool KURA = not(sys.FF60_D0);
  bool KALE = nor(KYME_Q, KURA);
  bool KASY = nor(sys.FF60_D0, KURA);
  bool KYHU = nand(sys.FF60_D0, JALE_Q);
  bool BYZO = not(FF00RD);

  bool KENA = mux2(KUKO_Q, ext.SER_OUT, sys.BURO.q());
  SOUT = KENA;

  if (BYZO) {
    KOLO_L = ext.P13_C;
    KEJA_L = ext.P12_C;
    KEVU_L = ext.P10_C;
    KAPA_L = ext.P11_C;
  }

  bool JEKU = not(KOLO_L);
  bool KUVE = not(KEJA_L);
  bool KEMA = not(KEVU_L);
  bool KURO = not(KAPA_L);
  bool KOCE = not(!KELY_Q);
  bool CUDY = not(!COFY_Q);

  if (BYZO) {
    mem.D3 = JEKU;
    mem.D2 = KUVE;
    mem.D0 = KEMA;
    mem.D1 = KURO;
    mem.D4 = KOCE;
    mem.D5 = CUDY;
  }

  bool KARU = or(KURA, !KELY_Q);

  bool CELA = or(!COFY_Q, KURA);
  bool KOLE = nand(JUTE_Q, sys.FF60_D0);
  bool KYBU = nor(JUTE_Q, KURA);
  bool KYTO = nand(KECY_Q, sys.FF60_D0);
  bool KABU = nor(KECY_Q, KURA);
  
  ext.P10_B = KOLE;
  ext.P10_D = KYBU;

  ext.P11_B = KYTO;
  ext.P11_D = KABU;

  ext.P12_A = KYHU;
  ext.P12_D = KASY;

  ext.P13_A = KORY;
  ext.P13_C = KALE;

  ext.P14_A = KARU;
  ext.P14_B = !KELY_Q;

  ext.P15_A = CELA;

  bool AXYN = not(clk.BEDO);
  bool ADYR = not(AXYN);
  bool APYS = nor(cpu.FROM_CPU, ADYR);
  bool AFOP = not(APYS);

  bool ANOC = not(ext.P10_B);
  bool AJEC = not(ext.P10_B);
  bool ARAR = not(ext.P10_B);
  bool BENU = not(ext.P10_B);
  bool AKAJ = not(ext.P10_B);
  bool ASUZ = not(ext.P10_B);
  bool ATAJ = not(ext.P10_B);
  bool BEDA = not(ext.P10_B);

  if (AFOP) {
    mem.D0 = ANOC;
    mem.D2 = AJEC;
    mem.D6 = ARAR;
    mem.D4 = BENU;
    mem.D5 = AKAJ;
    mem.D3 = ASUZ;
    mem.D1 = ATAJ;
    mem.D7 = BEDA;
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

  if (KYME_CLK && !FF00WR) KYME_Q_ = mem.D3;
  if (KUKO_CLK && !FF00WR) KUKO_Q_ = mem.D6;
  if (KERU_CLK && !FF00WR) KERU_Q_ = mem.D7;
  if (JALE_CLK && !FF00WR) JALE_Q_ = mem.D2;
  if (KELY_CLK && !FF00WR) KELY_Q_ = mem.D4;
  if (COFY_CLK && !FF00WR) COFY_Q_ = mem.D6;
  if (JUTE_CLK && !FF00WR) JUTE_Q_ = mem.D0;
  if (KECY_CLK && !FF00WR) KECY_Q_ = mem.D1;
  
  if (!rst.RESET2) KYME_Q_ = 0;
  if (!rst.RESET2) KUKO_Q_ = 0;
  if (!rst.RESET2) KERU_Q_ = 0;
  if (!rst.RESET2) JALE_Q_ = 0;
  if (!rst.RESET2) KELY_Q_ = 0;
  if (!rst.RESET2) COFY_Q_ = 0;
  if (!rst.RESET2) JUTE_Q_ = 0;
  if (!rst.RESET2) KECY_Q_ = 0;

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