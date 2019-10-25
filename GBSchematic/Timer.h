#pragma once
#include "Schematics.h"
#include "../src/TextPainter.h"

struct Timer {

  struct Input {
    bool RESET;       // active high!
    bool APU_RESET;
    bool APU_RESET5n;

    bool CLKIN_A;     // this is actually CLOCK_GOOD or similar
    bool CLKIN_B;     // the master 4mhz clock

    bool INT_TIMER;

    bool CPU_RD;
    bool CPU_WR;

    bool ABOL_1MHZ;   // this is actually CPU_RESET or similar
    bool AJER_2MHZ;   // on apu sheet

    bool FROM_CPU3;   // controls CPU_RD_SYNC?
    bool FROM_CPU4;
    bool FROM_CPU5;

    bool FERO_Q;      // something debug-related
    
    bool A00_07;
    bool FFXX;
    bool FF04_FF07;
    bool TOLA_A1n;
    bool TOVY_A0n;
    
    bool T1T2n;       // true for debugging
    bool T1nT2;       // true for debugging
    bool T1nT2n;      // true for normal mode
    
    bool FF04_D1n;
    bool FF40_D7;     // lcd on
    bool FF60_D1;     // debugging

    bool A0,A1,A2,A3,A4,A5,A6,A7;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool RESET2;      // active low!
    bool RESET6;
    bool RESET7;
    bool RESET7n;
    bool RESET8;
    bool RESET9;
    bool RESET_VIDEO;
    bool RESET_DIVn;

    bool INT_TIMER;

    bool CPU_RD_SYNC; 

    bool CLK1;
    bool CLK2;
    bool BYFE_128Hz;
    bool JESO_512K;
    bool ATAL_4MHZ;
    bool AMUK_4MHZ;
    bool ARYF_4MHZ;
    bool APUV_4MHZ;
    bool BAVU_1MHZ;
    bool BOGA1MHZ;
    bool TAMA16384;
    bool CLK_16K;
    bool CLK_64K;
    bool CLK_256K;
    bool HAMA_512Kn;
    bool HORU_512Hz;
    bool BUFY_256Hz;

    bool PHI_OUT;     // cartridge clock
    bool PHIn;

    bool TOVY_A0n;
    bool FF04_FF07;
    bool FF04_D0n;
    bool FF04_D1n;

    bool TO_CPU;
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

  // FF05 TIMA
  reg REGA,POVY,PERU,RATE,RUBY,RAGE,PEDA,NUGA;

  // FF06 TMA
  reg PETO,MURU,NYKE,SETA,SABU,TYRU,SUFY,TYVA;

  // FF07 TAC
  reg SABO,SAMY,SOPU;

  // INT_TIMER delay
  reg NYDU,MOBA;

  Output tock1(const Input& in, TextPainter& tp);
  Output tock2(const Input& in, TextPainter& tp);
};
