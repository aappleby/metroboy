#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P02_Interrupts {

#if 0
  struct Input {
    bool FF0F_RD;
    bool FF0F_WR;
    bool FF0F;
    bool CPU; // !?!?!?! this has gotta be FROM_CPU11
    bool BOGA1MHZ;
    bool RESET2;

    bool INT_VBL_BUF;
    bool INT_SERIAL;
    bool INT_JP;
    bool INT_STAT;
    bool INT_TIMER;
  };

  struct Output {
    bool INT_JP;
  };

  Input in;
  Output out;
#endif
  
  bool KERY;
  bool ASOK;
  bool ROTU;
  bool LETY;
  bool MUXE;
  bool LUFE;
  bool SULO;
  bool LAMO;
  bool SEME;
  bool LEJA;
  bool NABE;
  bool LESA;
  bool RAKE;
  bool MYZU;
  bool LYTA;
  bool TOME;
  bool TUNY;
  bool TOGA;
  bool TYME;
  bool MODY;
  bool MOVU;
  bool PYHU;
  bool PYGA;
  bool PESU;
  bool POLA;
  bool NELA;
  bool PADO;
  bool PEGY;
  bool NABO;
  bool ROVA;
  
  // glitch filter for joypad interrupt
  bool ACEF,AGEM,APUG,BATU,AWOB;

  // FF0F IF reg
  bool LOPE,UBUL,ULAK,LALU,NYBO;
  bool MATY,NEJY,NUTY,MOPO,PAVY;

  static const std::vector<SignalData> signals();

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};

