#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P03_Timer {

  //uint8_t get_tima() const { return (uint8_t)pack(REGA_0,POVY_1,PERU_2,RATE_3,RUBY_4,RAGE_5,PEDA_6,NUGA_7); }
  //uint8_t get_tma() const  { return (uint8_t)pack(SABU_0,NYKE_1,MURU_2,TYVA_3,TYRU_4,SUFY_5,PETO_6,SETA_7); }
  //uint8_t get_tac() const  { return (uint8_t)pack(SOPU_0,SAMY_1,SABO_2); }
  //void set_tima(uint8_t x) { unpack(x, REGA_0,POVY_1,PERU_2,RATE_3,RUBY_4,RAGE_5,PEDA_6,NUGA_7); }
  //void set_tma(uint8_t x)  { unpack(x, SABU_0,NYKE_1,MURU_2,TYVA_3,TYRU_4,SUFY_5,PETO_6,SETA_7); }
  //void set_tac(uint8_t x)  { unpack(x, SOPU_0,SAMY_1,SABO_2); }

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);
  static const std::vector<SignalData> signals();

  //----------------------------------------

  /*p03.RYFO*/ union { bool FF04_FF07; };
  /*p03.TOVY*/ union { bool A0n; };

  //----------
  // FF07 TAC

  /*p03.SORA*/ bool FF07_RD;
  /*p03.SARA*/ bool FF07_WR;

  /*p03.SOPU*/ bool TAC_0;
  /*p03.SAMY*/ bool TAC_1;
  /*p03.SABO*/ bool TAC_2;

  /*p03.SUPE*/ bool FF07_D0;
  /*p03.ROTE*/ bool FF07_D1;
  /*p03.RYLA*/ bool FF07_D2;

  //----------
  // FF06 TMA

  /*p03.TUBY*/ bool FF06_RD;
  /*p03.TYJU*/ bool FF06_WR;

  /*p03.SABU*/ bool TMA_0;
  /*p03.NYKE*/ bool TMA_1;
  /*p03.MURU*/ bool TMA_2;
  /*p03.TYVA*/ bool TMA_3;
  /*p03.TYRU*/ bool TMA_4;
  /*p03.SUFY*/ bool TMA_5;
  /*p03.PETO*/ bool TMA_6;
  /*p03.SETA*/ bool TMA_7;

  /*p03.SETE*/ bool FF06_D0;
  /*p03.PYRE*/ bool FF06_D1;
  /*p03.NOLA*/ bool FF06_D2;
  /*p03.SALU*/ bool FF06_D3;
  /*p03.SUPO*/ bool FF06_D4;
  /*p03.SOTU*/ bool FF06_D5;
  /*p03.REVA*/ bool FF06_D6;
  /*p03.SAPU*/ bool FF06_D7;

  //----------
  // TIMA clock mux

  /*p03.UVYR*/ bool UVYR;
  /*p03.UKAP*/ bool UKAP;
  /*p03.TECY*/ bool TECY;
  /*p03.UBOT*/ bool UBOT;
  /*p03.TEKO*/ bool TEKO;
  /*p03.SOGU*/ bool SOGU;

  // TIMA reload signal
  /*p03.MUZU*/ bool MUZU;
  /*p03.MEKE*/ bool MEKE;
  /*p03.MEXU*/ bool MEXU;

  // TIMA reload mux

  /*p03.ROKE*/ bool TIMA_MUX_0;
  /*p03.PETU*/ bool TIMA_MUX_1;
  /*p03.NYKU*/ bool TIMA_MUX_2;
  /*p03.SOCE*/ bool TIMA_MUX_3;
  /*p03.SALA*/ bool TIMA_MUX_4;
  /*p03.SYRU*/ bool TIMA_MUX_5;
  /*p03.REFU*/ bool TIMA_MUX_6;
  /*p03.RATO*/ bool TIMA_MUX_7;

  /*p03.MULO*/ bool TIMA_RST;

  /*p03.PUXY*/ bool TIMA_LD_0;
  /*p03.NERO*/ bool TIMA_LD_1;
  /*p03.NADA*/ bool TIMA_LD_2;
  /*p03.REPA*/ bool TIMA_LD_3;
  /*p03.ROLU*/ bool TIMA_LD_4;
  /*p03.RUGY*/ bool TIMA_LD_5;
  /*p03.PYMA*/ bool TIMA_LD_6;
  /*p03.PAGU*/ bool TIMA_LD_7;

  //----------
  // FF05 TIMA

  /*p03.TEDA*/ bool FF05_RD;
  /*p03.TOPE*/ bool FF05_WRn;

  /*p03.REGA*/ bool TIMA_0;
  /*p03.POVY*/ bool TIMA_1;
  /*p03.PERU*/ bool TIMA_2;
  /*p03.RATE*/ bool TIMA_3;
  /*p03.RUBY*/ bool TIMA_4;
  /*p03.RAGE*/ bool TIMA_5;
  /*p03.PEDA*/ bool TIMA_6;
  /*p03.NUGA*/ bool TIMA_7;

  /*p03.SOKU*/ bool FF05_D0;
  /*p03.RACY*/ bool FF05_D1;
  /*p03.RAVY*/ bool FF05_D2;
  /*p03.SOSY*/ bool FF05_D3;
  /*p03.SOMU*/ bool FF05_D4;
  /*p03.SURO*/ bool FF05_D5;
  /*p03.ROWU*/ bool FF05_D6;
  /*p03.PUSO*/ bool FF05_D7;

  //----------
  // INT_TIMER delay

  /*p03.MUGY*/ bool MUGY;
  /*p03.NYDU*/ bool NYDU;
  /*p03.MERY*/ bool MERY;
  /*p03.MOBA*/ bool INT_TIMER;
};

