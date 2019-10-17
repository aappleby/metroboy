// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool CLK_16k;
extern bool CLK_64k;
extern bool CLK_256k;
extern bool BOGA1MHZ;

extern bool FF04_D1n;
extern bool A00_07;
extern bool FFXX;
extern bool TOLA_A1n;

extern bool FROM_CPU5;
extern bool RESET2;

//----------
// outputs

bool INT_TIMER;
bool TOVY_A0n;

//----------
// registers

// tac
static bool SABO_Q, SABO_CLK;
static bool SAMY_Q, SAMY_CLK;
static bool SOPU_Q, SOPU_CLK;

// tima counter
static bool REGA_Q, REGA_CLK;
static bool POVY_Q, POVY_CLK;
static bool PERU_Q, PERU_CLK;
static bool RATE_Q, RATE_CLK;
static bool RUBY_Q, RUBY_CLK;
static bool RAGE_Q, RAGE_CLK;
static bool PEDA_Q, PEDA_CLK;
static bool NUGA_Q, NUGA_CLK;

bool NYDU_Q, NYDU_CLK;
bool MOBA_Q, MOBA_CLK;

// tma
bool TMA_CLK;
bool PETO_Q;
bool MURU_Q;
bool NYKE_Q;
bool SETA_Q;
bool SABU_Q;
bool TYRU_Q;
bool SUFY_Q;
bool TYVA_Q;

//-----------------------------------------------------------------------------

void tick_timer() {
  bool RYFO = and(A2, A00_07, FFXX);
  bool FF04_FF07 = RYFO;
  bool TOPE = nand(A0, TOLA_A1n, CPU_WR, FF04_FF07);

  //----------
  // tac

  bool SARA = nand(A0, A1, CPU_WR, FF04_FF07);
  bool UVYR = not(CLK_64k);
  bool UBOT = not(CLK_256k);
  bool SORA = and(FF04_FF07, CPU_RD, A1, A0);
  bool UKAP = mux2(CLK_16k, UVYR, SOPU_Q);
  bool TEKO = mux2(UBOT, FF04_D1n, SOPU_Q);
  bool SUPE = not(!SABO_Q);
  bool ROTE = not(!SAMY_Q);
  bool RYLA = not(!SOPU_Q);

  if (SORA) {
    D2 = SUPE;
    D1 = ROTE;
    D0 = RYLA;
  }

  bool TECY = mux2(UKAP, TEKO, SAMY_Q);
  bool SOGU = nor(TECY, !SABO_Q);

  //----------
  // tima

  bool MUZU = or(FROM_CPU5, TOPE);
  bool MEKE = not(INT_TIMER);
  bool MEXU = nand(MUZU, RESET2, MEKE);
  
  bool TEDA = and(FF04_FF07, CPU_RD, TOLA_A1n, A0);
  
  bool SOKU = not(!REGA_Q);
  bool RACY = not(!POVY_Q);
  bool RAVY = not(!PERU_Q);
  bool SOSY = not(!RATE_Q);
  bool SOMU = not(!RUBY_Q);
  bool SURO = not(!RAGE_Q);
  bool ROWU = not(!PEDA_Q);
  bool PUSO = not(!NUGA_Q);

  if (TEDA) {
    D0 = SOKU;
    D1 = RACY;
    D2 = RAVY;
    D3 = SOSY;
    D4 = SOMU;
    D5 = SURO;
    D6 = ROWU; // schematic missing annotation
    D7 = PUSO;
  }

  bool MUGY = not(MEXU);
  bool MERY = nor(!NYDU_Q, NUGA_Q);

  //----------
  // tma

  bool TOVY = not(A0);
  bool TUBY = and(FF04_FF07, CPU_RD, A1, TOVY);
  bool TYJU = nand(TOVY, A1, CPU_WR, FF04_FF07);

  TOVY_A0n = TOVY;

  bool REVA = not(!PETO_Q);
  bool NOLA = not(!MURU_Q);
  bool PYRE = not(!NYKE_Q);
  bool SAPU = not(!SETA_Q);
  bool SETE = not(!SABU_Q);
  bool SUPO = not(!TYRU_Q);
  bool SOTU = not(!SUFY_Q);
  bool SALU = not(!TYVA_Q);

  if (TUBY) {
    D6 = REVA;
    D2 = NOLA;
    D1 = PYRE;
    D7 = SAPU;
    D0 = SETE;
    D4 = SUPO;
    D5 = SOTU;
    D3 = SALU;
  }

  bool REFU = mux2(PETO_Q, D6, TOPE);
  bool NYKU = mux2(MURU_Q, D2, TOPE);
  bool PETU = mux2(NYKE_Q, D1, TOPE);
  bool RATO = mux2(SETA_Q, D7, TOPE);
  bool ROKE = mux2(SABU_Q, D0, TOPE);
  bool SALA = mux2(TYRU_Q, D4, TOPE);
  bool SYRU = mux2(SUFY_Q, D5, TOPE);
  bool SOCE = mux2(TYVA_Q, D3, TOPE);

  bool MULO = not(RESET2);

  bool PYMA = nor(MULO, REFU);
  bool NADA = nor(MULO, NYKU);
  bool NERO = nor(MULO, PETU);
  bool PAGU = nor(MULO, RATO);
  bool PUXY = nor(MULO, ROKE);
  bool ROLU = nor(MULO, SALA);
  bool RUGY = nor(MULO, SYRU);
  bool REPA = nor(MULO, SOCE);

  // FIXME trigger? async?
  if (MEXU) {
    REGA_Q = PUXY;
    POVY_Q = NERO;
    PERU_Q = NADA;
    RATE_Q = REPA;
    RUBY_Q = ROLU;
    RAGE_Q = RUGY;
    PEDA_Q = PYMA;
    NUGA_Q = PAGU;
  }

  //----------
  // registers

  // TAC
  bool SABO_Q_ = SABO_Q;
  bool SAMY_Q_ = SAMY_Q;
  bool SOPU_Q_ = SOPU_Q;

  if (SABO_CLK && !SARA) SABO_Q_ = D2;
  if (SAMY_CLK && !SARA) SAMY_Q_ = D1;
  if (SOPU_CLK && !SARA) SOPU_Q_ = D0;

  if (!RESET2) SABO_Q_ = 0;
  if (!RESET2) SAMY_Q_ = 0;
  if (!RESET2) SOPU_Q_ = 0;

  SABO_CLK = SARA;
  SAMY_CLK = SARA;
  SOPU_CLK = SARA;

  SABO_Q = SABO_Q_;
  SAMY_Q = SAMY_Q_;
  SOPU_Q = SOPU_Q_;

  // TIMA
  bool REGA_Q_ = REGA_Q;
  bool POVY_Q_ = POVY_Q;
  bool PERU_Q_ = PERU_Q;
  bool RATE_Q_ = RATE_Q;
  bool RUBY_Q_ = RUBY_Q;
  bool RAGE_Q_ = RAGE_Q;
  bool PEDA_Q_ = PEDA_Q;
  bool NUGA_Q_ = NUGA_Q;

  if (REGA_CLK && !SOGU)   REGA_Q_ = !REGA_Q;
  if (POVY_CLK && !REGA_Q) POVY_Q_ = !POVY_Q;
  if (PERU_CLK && !POVY_Q) PERU_Q_ = !PERU_Q;
  if (RATE_CLK && !PERU_Q) RATE_Q_ = !RATE_Q;
  if (RUBY_CLK && !RATE_Q) RUBY_Q_ = !RUBY_Q;
  if (RAGE_CLK && !RUBY_Q) RAGE_Q_ = !RAGE_Q;
  if (PEDA_CLK && !RAGE_Q) PEDA_Q_ = !PEDA_Q;
  if (NUGA_CLK && !PEDA_Q) NUGA_Q_ = !NUGA_Q;

  REGA_CLK = SOGU;
  POVY_CLK = REGA_Q;
  PERU_CLK = POVY_Q;
  RATE_CLK = PERU_Q;
  RUBY_CLK = RATE_Q;
  RAGE_CLK = RUBY_Q;
  PEDA_CLK = RAGE_Q;
  NUGA_CLK = PEDA_Q;

  REGA_Q = REGA_Q_;
  POVY_Q = POVY_Q_;
  PERU_Q = PERU_Q_;
  RATE_Q = RATE_Q_;
  RUBY_Q = RUBY_Q_;
  RAGE_Q = RAGE_Q_;
  PEDA_Q = PEDA_Q_;
  NUGA_Q = NUGA_Q_;

  bool NYDU_Q_ = NYDU_Q;
  bool MOBA_Q_ = MOBA_Q;

  if (NYDU_CLK && !BOGA1MHZ) NYDU_Q_ = NUGA_Q;
  if (MOBA_CLK && !BOGA1MHZ) MOBA_Q_ = MERY;

  if (!MUGY)   NYDU_Q_ = 0;
  if (!RESET2) MOBA_Q_ = 0;

  NYDU_CLK = BOGA1MHZ;
  MOBA_CLK = BOGA1MHZ;

  NYDU_Q = NYDU_Q_;
  MOBA_Q = MOBA_Q_;

  // TMA

  bool PETO_Q_ = PETO_Q;
  bool MURU_Q_ = MURU_Q;
  bool NYKE_Q_ = NYKE_Q;
  bool SETA_Q_ = SETA_Q;
  bool SABU_Q_ = SABU_Q;
  bool TYRU_Q_ = TYRU_Q;
  bool SUFY_Q_ = SUFY_Q;
  bool TYVA_Q_ = TYVA_Q;

  if (TMA_CLK && !TYJU) {
    PETO_Q_ = D6;
    MURU_Q_ = D2;
    NYKE_Q_ = D1;
    SETA_Q_ = D7;
    SABU_Q_ = D0;
    TYRU_Q_ = D4;
    SUFY_Q_ = D5;
    TYVA_Q_ = D3;
  }

  if (!RESET2) {
    PETO_Q_ = 0;
    MURU_Q_ = 0;
    NYKE_Q_ = 0;
    SETA_Q_ = 0;
    SABU_Q_ = 0;
    TYRU_Q_ = 0;
    SUFY_Q_ = 0;
    TYVA_Q_ = 0;
  }

  TMA_CLK = TYJU;

  PETO_Q = PETO_Q_;
  MURU_Q = MURU_Q_;
  NYKE_Q = NYKE_Q_;
  SETA_Q = SETA_Q_;
  SABU_Q = SABU_Q_;
  TYRU_Q = TYRU_Q_;
  SUFY_Q = SUFY_Q_;
  TYVA_Q = TYVA_Q_;
}