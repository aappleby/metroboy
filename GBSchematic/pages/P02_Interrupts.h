#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P02_Interrupts {

  union { bool ASOK; bool INT_JP; };

  static const std::vector<SignalData> signals();
  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

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
  bool ACEF,AGEM,APUG,BATU,AWOB;

  // FF0F IF reg
  bool LOPE,UBUL,ULAK,LALU,NYBO;
  bool MATY,NEJY,NUTY,MOPO,PAVY;
};

