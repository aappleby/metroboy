#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P02_Interrupts {

  union { bool ASOK; bool INT_JP; };
  union { bool AWOB; bool TO_CPU2; };

  // FF0F IF reg
  union { bool LOPE; bool FF0F_0; bool TO_CPU3; };
  union { bool UBUL; bool FF0F_1; bool TO_CPU4; };
  union { bool ULAK; bool FF0F_2; bool TO_CPU5; };
  union { bool LALU; bool FF0F_3; bool TO_CPU6; };
  union { bool NYBO; bool FF0F_4; bool TO_CPU7; };

  static const std::vector<SignalData> signals();
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

private:

  bool KERY;
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
  bool ACEF,AGEM,APUG,BATU;

  // FF0F IF reg
  bool MATY,NEJY,NUTY,MOPO,PAVY;
};

