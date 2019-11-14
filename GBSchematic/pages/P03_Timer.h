#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P03_Timer {

  union { bool RYFO; bool FF04_FF07; };
  union { bool TOVY; bool A0n; };
  union { bool MOBA; bool INT_TIMER; };

  uint8_t get_tima() const { return (uint8_t)pack(REGA_0,POVY_1,PERU_2,RATE_3,RUBY_4,RAGE_5,PEDA_6,NUGA_7); }
  uint8_t get_tma() const  { return (uint8_t)pack(SABU_0,NYKE_1,MURU_2,TYVA_3,TYRU_4,SUFY_5,PETO_6,SETA_7); }
  uint8_t get_tac() const  { return (uint8_t)pack(SOPU_0,SAMY_1,SABO_2); }
  void set_tima(uint8_t x) { unpack(x, REGA_0,POVY_1,PERU_2,RATE_3,RUBY_4,RAGE_5,PEDA_6,NUGA_7); }
  void set_tma(uint8_t x)  { unpack(x, SABU_0,NYKE_1,MURU_2,TYVA_3,TYRU_4,SUFY_5,PETO_6,SETA_7); }
  void set_tac(uint8_t x)  { unpack(x, SOPU_0,SAMY_1,SABO_2); }

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);
  static const std::vector<SignalData> signals();

private:

  // FF07 TAC
  bool SOPU_0;
  bool SAMY_1;
  bool SABO_2;

  // FF06 TMA
  bool SABU_0;
  bool NYKE_1;
  bool MURU_2;
  bool TYVA_3;
  bool TYRU_4;
  bool SUFY_5;
  bool PETO_6;
  bool SETA_7;

  // TIMA clock mux
  bool UVYR,UKAP,TECY,UBOT,TEKO,SOGU;

  // TIMA reload signal
  bool MUZU,MEKE,MEXU;

  // TIMA reload mux
  bool TOPE,REFU,NYKU,PETU,RATO,ROKE,SALA,SYRU,SOCE;
  bool MULO,PYMA,NADA,NERO,PAGU,PUXY,ROLU,RUGY,REPA;

  // FF05 TIMA
  bool REGA_0;
  bool POVY_1;
  bool PERU_2;
  bool RATE_3;
  bool RUBY_4;
  bool RAGE_5;
  bool PEDA_6;
  bool NUGA_7;

  bool TEDA;
  bool SOKU_0,RACY_1,RAVY_2,SOSY_3,SOMU_4,SURO_5,ROWU_6,PUSO_7;

  // INT_TIMER delay
  bool NYDU,MUGY,MERY;

  //----------------------------------------

  bool SARA,SORA,SUPE,ROTE,RYLA;
  bool TYJU,TUBY,REVA,NOLA,PYRE,SAPU,SETE,SUPO,SOTU,SALU;
};

