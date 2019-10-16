// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool BEDO;
extern bool T1T2n;
extern bool RESET;
extern bool RESET2;
extern bool CPU_WR_RAW;
extern bool T1; // these are special-function-control pins on the cpu
extern bool T2;
extern bool P10_B;

extern bool NET02;
extern bool ANAP;

extern bool WR_IN;
extern bool CPU_RD_SYNC;
extern bool T1nT2;
extern bool RD_B;
extern bool CPU_RAW_RD;
extern bool FROM_CPU6;

//----------
// outputs

bool PIN_NC;
bool CPU_RD;
bool CPU_RD2;

bool CPU_WR;
bool CPU_WR2;

bool BOOT_CS;
bool FF0F_RD;
bool FF0F_WR;
bool HRAM_CS;

bool FF60_D0;
bool FF60_D1;

bool FEXXFFXXn;
bool FFXX;
bool FFXXn;
bool SARO;

bool T1nT2n;
bool T1nT2;
bool T1T2n;

bool BOOTROM_A7n;
bool BOOTROM_A6n;
bool BOOTROM_A5nA4n;
bool BOOTROM_A5nA4;
bool BOOTROM_A5A4n;
bool BOOTROM_A5AA4;
bool BOOTROM_A3n;
bool BOOTROM_A2n;
bool BOOTROM_A1nA0n;
bool BOOTROM_A1nA0;
bool BOOTROM_A1A0n;
bool BOOTROM_A1A0;

//----------
// registers

static bool TEPU_Q, TEPU_CLK;
static bool AMUT_Q, AMUT_CLK;
static bool BURO_Q, BURO_CLK;

//-----------------------------------------------------------------------------

void tick_sysdecode() {
  //----------

  bool TYRO = nor(A7, A5, A3, A2, A1, A0);
  bool TUFA = and(A4, A6);
  bool TUGE = nand(TYRO, TUFA, FFXX, CPU_WR);
  bool SATO = or(D0, TEPU_Q);
  bool TEXE = and(CPU_RD, FFXX, TUFA, TYRO);
  bool SYPU = not(TEPU_Q);

  if (TEXE) {
    D0 = SYPU;
  }

  bool TERA = not(TEPU_Q);
  bool TULO = nor(A15, A14, A13, A12, A11, A10, A9, A8);
  bool TUTU = and(TERA, TULO);
  bool YAZA = not(T1T2n);
  bool ZORO = nor(A15, A14, A13, A12);
  bool ZADU = nor(A11, A10, A9, A8);
  bool YULA = and(YAZA, TUTU, CPU_RD);
  bool ZUFA = and(ZORO, ZADU);
  bool ZADO = nand(YULA, ZUFA);
  bool ZERY = not(ZADO);

  BOOT_CS = ZERY;

  //----------

  bool SEMY = nor(A7, A6, A5, A4);
  bool SAPA = and(A0, A1, A2, A3);
  bool ROLO = and(SEMY, SAPA, FFXX, CPU_RD);
  bool REFA = and(SEMY, SAPA, FFXX, CPU_WR_RAW);

  FF0F_RD = ROLO;
  FF0F_WR = REFA;

  //----------

  bool WALE = nand(A0, A1, A2, A3, A4, A5, A6);
  bool WOLY = nand(WALE, A7, FFXX);
  bool WUTA = not(WOLY);

  HRAM_CS = WUTA;

  //----------

  bool UBET = not(T1);
  bool UVAR = not(T2);
  bool UPOJ = nand(UBET, UVAR, RESET);
  bool UNOR = and(T2, UBET);
  bool UMUT = and(T1, UVAR);

  T1nT2n = UPOJ;
  T1nT2 = UNOR;
  T1T2n = UMUT;

  //----------

  bool LECO = nor(BEDO, T1nT2);

  bool RARU = not(P10_B);
  bool ROWE = not(P10_B);
  bool RYKE = not(P10_B);
  bool RYNE = not(P10_B);
  bool RASE = not(P10_B);
  bool REJY = not(P10_B);
  bool REKA = not(P10_B);
  bool ROMY = not(P10_B);

  if (LECO) {
    D7 = RARU;
    D5 = ROWE;
    D6 = RYKE;
    D1 = RYNE;
    D3 = RASE;
    D2 = REJY;
    D4 = REKA;
    D0 = ROMY;
  }

  //----------

  bool TONA = not(A8);
  bool TUNA = nand(A15, A14, A13, A12, A11, A10, A9);
  FEXXFFXXn = TUNA;
  bool SYKE = nor(TONA, TUNA);
  bool RYCU = not(TUNA);
  bool SOHA = not(FFXX);
  bool ROPE = nand(RYCU, SOHA);
  FFXX = SYKE;
  bool BAKO = not(SYKE);
  FFXXn = BAKO;
  SARO = not(ROPE);

  //----------

  bool ZYRA = not(A7);
  bool ZAGE = not(A6);
  bool ZABU = not(A3);
  bool ZOKE = not(A2);

  BOOTROM_A7n = ZYRA;
  BOOTROM_A6n = ZAGE;
  BOOTROM_A3n = ZABU;
  BOOTROM_A2n = ZOKE;

  //----------

  bool ZERA = not(A5);
  bool ZUFY = not(A4);
  bool ZYKY = and(ZERA, ZUFY);
  bool ZYGA = and(ZERA, A4);
  bool ZOVY = and(A5, ZUFY);
  bool ZUKO = and(A5, A4);

  BOOTROM_A5nA4n = ZYKY;
  BOOTROM_A5nA4 = ZYGA;
  BOOTROM_A5A4n = ZOVY;
  BOOTROM_A5AA4 = ZUKO;

  //----------

  bool ZUVY = not(A1);
  bool ZYBA = not(A0);
  bool ZOLE = and(ZUVY, ZYBA);
  bool ZAJE = and(ZUVY, A0);
  bool ZUBU = and(ZYBA, A1);
  bool ZAPY = and(A1, A0);

  bool ZETE = not(ZOLE);
  bool ZEFU = not(ZAJE);
  bool ZYRO = not(ZUBU);
  bool ZAPA = not(ZAPY);

  BOOTROM_A1nA0n = ZETE;
  BOOTROM_A1nA0 = ZEFU;
  BOOTROM_A1A0n = ZYRO;
  BOOTROM_A1A0 = ZAPA;

  //----------

  bool APET = or(NET02, T1T2n);
  bool APER = nand(APET, A5, A6, CPU_WR, ANAP);

  //----------

  bool UBAL = mux2(WR_IN, CPU_RD_SYNC, T1nT2);
  bool TAPU = not(UBAL);
  CPU_WR = TAPU;
  bool DYKY = not(TAPU);
  bool CUPA = not(DYKY);
  CPU_WR2 = CUPA;

  bool UJYV = mux2(RD_B, CPU_RAW_RD, T1nT2);
  bool TEDO = not(UJYV);
  CPU_RD = TEDO;
  bool AJAS = not(TEDO);
  bool ASOT = not(AJAS);
  CPU_RD2 = ASOT;

  bool LEXY = not(FROM_CPU6);
  PIN_NC = LEXY;

  //----------
  // registers

  bool TEPU_Q_ = TEPU_Q;
  if (TEPU_CLK && !TUGE) TEPU_Q_ = SATO;
  if (!RESET2) TEPU_Q_ = 0;
  TEPU_Q = TEPU_Q_;
  TEPU_CLK = TUGE;

  //----------

  bool AMUT_Q_ = AMUT_Q;
  bool BURO_Q_ = BURO_Q;

  if (AMUT_CLK && !APER) AMUT_Q_ = D1;
  if (BURO_CLK && !APER) BURO_Q_ = D0;
  if (!RESET2) AMUT_Q_ = 0;
  if (!RESET2) BURO_Q_ = 0;

  AMUT_CLK = APER;
  BURO_CLK = APER;

  AMUT_Q = AMUT_Q_;
  BURO_Q = BURO_Q_;

}