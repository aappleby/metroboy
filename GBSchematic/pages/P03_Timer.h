#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P03_Timer {

  // random decoder
  bool RYFO;

  // FF07 TAC
  bool SOPU_0,SAMY_1,SABO_2;
  bool SARA,SORA,SUPE,ROTE,RYLA;

  // FF06 TMA
  bool SABU_0,NYKE_1,MURU_2,TYVA_3,TYRU_4,SUFY_5,PETO_6,SETA_7;
  bool TOVY,TYJU,TUBY,REVA,NOLA,PYRE,SAPU,SETE,SUPO,SOTU,SALU;

  // TIMA clock mux
  bool UVYR,UKAP,TECY,UBOT,TEKO,SOGU;

  // TIMA reload signal
  bool MUZU,MEKE,MEXU;

  // TIMA reload mux
  bool TOPE,REFU,NYKU,PETU,RATO,ROKE,SALA,SYRU,SOCE;
  bool MULO,PYMA,NADA,NERO,PAGU,PUXY,ROLU,RUGY,REPA;

  // FF05 TIMA
  bool REGA_0,POVY_1,PERU_2,RATE_3,RUBY_4,RAGE_5,PEDA_6,NUGA_7;
  bool TEDA;
  bool SOKU_0,RACY_1,RAVY_2,SOSY_3,SOMU_4,SURO_5,ROWU_6,PUSO_7;

  // INT_TIMER delay
  bool NYDU,MOBA;
  bool MUGY,MERY;

  static const std::vector<SignalData> signals();

  //----------------------------------------

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);
};

