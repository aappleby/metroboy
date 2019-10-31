#pragma once
#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P02_Interrupts {

  struct Input {
    bool P10;
    bool P10_B;
    bool P11_C;
    bool P12;
    bool P13_C;

    bool FF0F_RD;
    bool FF0F_WR;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
    bool FF0F;
    bool CPU; // !?!?!?! this has gotta be FROM_CPU11
    bool BOGA1MHZ;
    bool RESET2;

    bool INT_VBL_BUF;
    bool INT_SERIAL;
    bool INT_JP;
    bool INT_STAT;
    bool INT_TIMER;

    bool FROM_CPU7;
    bool FROM_CPU8;
    bool FROM_CPU9;
    bool FROM_CPU10;
    bool FROM_CPU11;
  };

  struct Output {
    bool TO_CPU2;  // for unhalt?
    bool INT_JP;
    bool CPU_INT_VBLANK;
    bool CPU_INT_SERIAL;
    bool CPU_INT_JP;
    bool CPU_INT_STAT;
    bool CPU_INT_TIMER;

    bool OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // glitch filter for joypad interrupt
  reg ACEF,AGEM,APUG,BATU,AWOB;

  // FF0F IF reg
  reg LOPE,UBUL,ULAK,LALU,NYBO;
  reg MATY,NEJY,NUTY,MOPO,PAVY;

  Output tick(const Input& in);
};

