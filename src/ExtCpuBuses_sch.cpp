// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool ABUZ;
extern bool FEXXFFXXN;
extern bool CPU_WR_WAT;
extern bool NET02;
extern bool A15_C;
extern bool LUMA;
extern bool MATE;

extern bool DMA_A8;
extern bool DMA_A9;
extern bool DMA_A10;
extern bool DMA_A11;
extern bool DMA_A12;
extern bool DMA_A13;
extern bool DMA_A14;
extern bool DMA_A15;

extern bool T1N_T2;

//----------
// outputs

bool A8_D;
bool A8_A;
bool A9_D;
bool A9_A;
bool A10_D;
bool A10_A;
bool A11_D;
bool A11_A;
bool A12_D;
bool A12_A;
bool A13_D;
bool A13_A;
bool A14_D;
bool A14_A;
bool A15;
bool A15_D;
bool A15_A;
bool NET01;
bool CS_OUT;

//----------
// registers

bool NYRE_L;
bool LONU_L;
bool LOBU_L;
bool LUMY_L;
bool PATE_L;
bool LYSA_L;
bool LUNO_L;

//-----------------------------------------------------------------------------

void tick_extbus() {
  bool SOGY = not(A14);
  bool TUMA = and(A13, SOGY, A15);
  bool TYNU = unk3(A15, A14, TUMA);
  bool TOZA = and(TYNU, ABUZ, FEXXFFXXN);
  bool SOBY = nor(A15, CPU_WR_WAT); // schematic has a question mark?
  bool SEPY = nand(ABUZ, SOBY);

  bool TYHO = mux2(DMA_A15, TOZA, LUMA);
  CS_OUT = TYHO;

  bool TAZY = mux2(DMA_A15, SEPY, LUMA);

  bool RYCA = not(NET02);
  bool RAZA = not(A15_C);
  bool SYZU = not(RAZA);
  A15 = SYZU;
  bool RULO = nor(TAZY, NET02);
  bool SUZE = nand(TAZY, RYCA);

  A15_D = RULO;
  A15_A = SUZE;

  if (MATE) {
    NYRE_L = A14;
    LONU_L = A13;
    LOBU_L = A12;
    LUMY_L = A11;
    PATE_L = A10;
    LYSA_L = A9;
    LUNO_L = A8;
  }

  bool PEGE = mux2(DMA_A14, NYRE_L, LUMA);
  bool MUCE = mux2(DMA_A13, LONU_L, LUMA);
  bool MOJY = mux2(DMA_A12, LOBU_L, LUMA);
  bool MALE = mux2(DMA_A11, LUMY_L, LUMA);
  bool PAMY = mux2(DMA_A10, PATE_L, LUMA);
  bool MASU = mux2(DMA_A9, LYSA_L, LUMA);
  bool MANO = mux2(DMA_A8, LUNO_L, LUMA);

  bool TOVA = not(T1N_T2);
  NET01 = TOVA;

  bool PAHY = nor(T1N_T2, PEGE);
  bool PUHE = nand(PEGE, TOVA);

  bool LEVA = nor(T1N_T2, MUCE);
  bool LABE = nand(MUCE, TOVA);

  bool LOSO = nor(T1N_T2, MOJY);
  bool LUCE = nand(MOJY, TOVA);

  bool LYNY = nor(T1N_T2, MALE);
  bool LEPY = nand(MALE, TOVA);

  bool RORE = nor(T1N_T2, PAMY);
  bool ROXU = nand(PAMY, TOVA);

  bool MENY = nor(T1N_T2, MASU);
  bool MUNE = nand(MASU, TOVA);

  bool MEGO = nor(T1N_T2, MANO);
  bool MYNY = nand(MANO, TOVA);

  A14_D = PAHY;
  A14_A = PUHE;
  A13_D = LEVA;
  A13_A = LABE;
  A12_D = LOSO;
  A12_A = LUCE;
  A11_D = LYNY;
  A11_A = LEPY;
  A10_D = RORE;
  A10_A = ROXU;
  A9_D = MENY;
  A9_A = MUNE;
  A8_D = MEGO;
  A8_A = MYNY;
}
