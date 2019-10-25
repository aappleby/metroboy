#pragma once
#include "Schematics.h"
#include "../src/TextPainter.h"

struct Timer {

  struct Input {
    // physical pins
    bool CPU_RD;
    bool CPU_WR;
    bool RESET;       // active high!
    bool CLKIN_A;     // this is actually CLOCK_GOOD or similar
    bool CLKIN_B;     // the master 4mhz clock

    bool ABOL_1MHZ;   // this is actually CPU_RESET or similar
    bool AJER_2MHZ;   // on apu sheet
    bool FROM_CPU3;   // controls CPU_RD_SYNC?
    bool FROM_CPU4;
    bool APU_RESET;
    bool APU_RESET5n;
    bool FERO_Q;      // something debug-related
    bool FF04_FF07;
    bool TOLA_A1n;
    bool TOVY_A0n;
    bool T1T2n;       // true for debugging
    bool T1nT2;       // true for debugging
    bool T1nT2n;      // true for normal mode
    bool FF40_D7;     // lcd on
    bool FF60_D1;     // debugging
  };

  struct Output {
    bool FF04_D0n;
    bool FF04_D1n;
    bool ATAL_4MHZ;
    bool AMUK_4MHZ;
    bool ARYF_4MHZ;
    bool APUV_4MHZ;
    bool BAVU_1MHZ;
    bool BOGA1MHZ;
    bool TAMA16384;
    bool CLK_16384;
    bool CLK_65536;
    bool CLK_262144;
    bool HAMA_512Kn;
    bool HORU_512Hz;
    bool BUFY_256Hz;
    bool PHI_OUT;     // cartridge clock
    bool PHIn;
    bool CLK2;
    bool CLK1;
    bool BYFE_128Hz;
    bool JESO_512K;
    bool RESET2;      // active low!
    bool RESET6;
    bool RESET7;
    bool RESET7n;
    bool RESET8;
    bool RESET9;
    bool RESET_VIDEO;
    bool RESET_DIVn;

    bool TO_CPU;
    bool CPU_RD_SYNC; 
    bool BUKE;
    bool ABUZ;
    bool AFAS;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
};

  // 1mhz phase generator
  reg ADYK,AFUR,ALEF,APUK;

  // register for the RESET2 signal, roughly
  reg AFER;

  // div
  reg UKUP,UFOR,UNER,TERO,UNYK,TAMA,UGOT,TULU,TUGO,TOFE,TERU,SOLA,SUBU,TEKA,UKET,UPOF;

  // clock divider for apu sequencer
  reg BARA,CARU,BYLU;

  // clock divider for something else...
  reg ATYK,AVOK,JESO;

  Output tock(const Input& in, TextPainter& tp);

#if 0
  struct Input {
    // physical pins
    bool CPU_WR;
    bool CLKIN_B;     // the master 4mhz clock

    uint16_t addr;
  };

  struct Output {
    bool CLK_16384;
    bool CLK_65536;
    bool CLK_262144;
  };

  Output tock(int clk, const Input& in, TextPainter& tp);

private:

  int tick = 0;

  // 1mhz phase generator
  reg ADYK,AFUR,ALEF,APUK;

  // div
  reg UKUP,UFOR,UNER,TERO,UNYK,TAMA,UGOT,TULU,TUGO,TOFE,TERU,SOLA,SUBU,TEKA,UKET,UPOF;
#endif
};
