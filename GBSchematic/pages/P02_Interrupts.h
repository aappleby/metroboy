#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P02_Interrupts {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);
  static const std::vector<SignalData> signals();

  /*p02.ASOK*/ bool INT_JP;
  /*p02.AWOB*/ bool TO_CPU2;

  // FF0F IF reg
  /*p02.LOPE*/ union { bool FF0F_0; bool TO_CPU3; };
  /*p02.UBUL*/ union { bool FF0F_1; bool TO_CPU4; };
  /*p02.ULAK*/ union { bool FF0F_2; bool TO_CPU5; };
  /*p02.LALU*/ union { bool FF0F_3; bool TO_CPU6; };
  /*p02.NYBO*/ union { bool FF0F_4; bool TO_CPU7; };

private:

  /*p02.KERY*/ bool KERY;
  /*p02.ROTU*/ bool ROTU;
  /*p02.LETY*/ bool LETY;
  /*p02.MUXE*/ bool MUXE;
  /*p02.LUFE*/ bool LUFE;
  /*p02.SULO*/ bool SULO;
  /*p02.LAMO*/ bool LAMO;
  /*p02.SEME*/ bool SEME;
  /*p02.LEJA*/ bool LEJA;
  /*p02.NABE*/ bool NABE;
  /*p02.LESA*/ bool LESA;
  /*p02.RAKE*/ bool RAKE;
  /*p02.MYZU*/ bool MYZU;
  /*p02.LYTA*/ bool LYTA;
  /*p02.TOME*/ bool TOME;
  /*p02.TUNY*/ bool TUNY;
  /*p02.TOGA*/ bool TOGA;
  /*p02.TYME*/ bool TYME;
  /*p02.MODY*/ bool MODY;
  /*p02.MOVU*/ bool MOVU;
  /*p02.PYHU*/ bool PYHU;
  /*p02.PYGA*/ bool PYGA;
  /*p02.PESU*/ bool PESU;
  /*p02.POLA*/ bool POLA;
  /*p02.NELA*/ bool NELA;
  /*p02.PADO*/ bool PADO;
  /*p02.PEGY*/ bool PEGY;
  /*p02.NABO*/ bool NABO;
  /*p02.ROVA*/ bool ROVA;
  
  // glitch filter for joypad interrupt
  /*p02.ACEF*/ bool ACEF;
  /*p02.AGEM*/ bool AGEM;
  /*p02.APUG*/ bool APUG;
  /*p02.BATU*/ bool BATU;

  // FF0F IF reg
  /*p02.MATY*/ bool MATY;
  /*p02.NEJY*/ bool NEJY;
  /*p02.NUTY*/ bool NUTY;
  /*p02.MOPO*/ bool MOPO;
  /*p02.PAVY*/ bool PAVY;
};

