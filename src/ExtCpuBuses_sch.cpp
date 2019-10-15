// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool PIN_A0;

extern bool ABUZ;
extern bool FEXXFFXXn;
extern bool CPU_WR_WAT;
extern bool CPU_RAW_RD;
extern bool CPU_RD_SYNC;
extern bool NET02;
extern bool A15_C;
extern bool LUMA;

extern bool FROM_CPU3;
extern bool FROM_CPU4;
extern bool FROM_CPU5;

extern bool DMA_A8;
extern bool DMA_A9;
extern bool DMA_A10;
extern bool DMA_A11;
extern bool DMA_A12;
extern bool DMA_A13;
extern bool DMA_A14;
extern bool DMA_A15;

extern bool T1nT2;
extern bool T1T2n;

//----------
// outputs

bool LULA;
bool RORU;

bool RD_A;
bool RD_C;
bool WR_A;
bool WR_C;

bool A0_D;
bool A0_A;
bool A1_D;
bool A1_A;
bool A2_D;
bool A2_A;
bool A3_D;
bool A3_A;
bool A4_D;
bool A4_A;
bool A5_D;
bool A5_A;
bool A6_D;
bool A6_A;
bool A7_D;
bool A7_A;
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
bool A15_D;
bool A15_A;

bool D6_D;
bool D7_A; // why A?
bool D0_D;
bool D4_D;
bool D1_D;
bool D2_D;
bool D3_D;
bool D5_D;

bool A15;
bool NET01;
bool CS_OUT;
bool TOLA_A1n;

//----------
// registers

static bool NYRE_L;
static bool LONU_L;
static bool LOBU_L;
static bool LUMY_L;
static bool PATE_L;
static bool LYSA_L;
static bool LUNO_L;
static bool ARYM_L;
static bool AROS_L;
static bool ATEV_L;
static bool AVYS_L;
static bool ARET_L;
static bool ALYR_L;
static bool APUR_L;
static bool ALOR_L;

//-----------------------------------------------------------------------------

void tick_extbus() {
  bool SORE = not(A15);
  bool TEVY = and(A13, A14, SORE);
  bool TEXO = and(FROM_CPU4, TEVY);
  bool MULE = not(T1T2n);
  bool LOXO = unk3(MULE, TEXO, T1T2n);
  bool LASY = not(LOXO);
  bool MATE = not(LASY);
  bool TOLA = not(A1);

  TOLA_A1n = TOLA;

  bool LEVO = not(TEXO);
  bool LAGU = unk3(CPU_RAW_RD, LEVO, FROM_CPU3);
  bool LYWE = not(LAGU);
  bool MOCA = nor(TEXO, T1T2n);
  bool MEXO = not(CPU_RD_SYNC);
  bool NEVY = or(MEXO, MOCA);
  bool MOTY = or(MOCA, LYWE);
  bool PUVA = or(NEVY, LUMA);
  bool TYMU = or(LUMA, MOTY);
  bool USUF = nor(T1nT2, PUVA);
  bool UVER = nand(PUVA, NET01);
  bool UGAC = nand(NET01, TYMU);
  bool URUN = nor(TYMU, T1nT2);

  WR_C = USUF;
  WR_A = UVER;
  RD_A = UGAC;
  RD_C = URUN;


  bool SOGY = not(A14);
  bool TUMA = and(A13, SOGY, A15);
  bool TYNU = unk3(A15, A14, TUMA);
  bool TOZA = and(TYNU, ABUZ, FEXXFFXXn);
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

  // FIXME what trigger kind of latch
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

  bool TOVA = not(T1nT2);
  NET01 = TOVA;

  bool PAHY = nor(T1nT2, PEGE);
  bool PUHE = nand(PEGE, TOVA);

  bool LEVA = nor(T1nT2, MUCE);
  bool LABE = nand(MUCE, TOVA);

  bool LOSO = nor(T1nT2, MOJY);
  bool LUCE = nand(MOJY, TOVA);

  bool LYNY = nor(T1nT2, MALE);
  bool LEPY = nand(MALE, TOVA);

  bool RORE = nor(T1nT2, PAMY);
  bool ROXU = nand(PAMY, TOVA);

  bool MENY = nor(T1nT2, MASU);
  bool MUNE = nand(MASU, TOVA);

  bool MEGO = nor(T1nT2, MANO);
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

  if (MATE) {
    ARYM_L = A7;
    AROS_L = A6;
    ATEV_L = A5;
    AVYS_L = A4;
    ARET_L = A3;
    ALYR_L = A2;
    APUR_L = A1;
    ALOR_L = A0;
  }

  bool ASUR = mux2(DMA_A7, ARYM_L, LUMA);
  bool ATYR = mux2(DMA_A6, AROS_L, LUMA);
  bool ATOV = mux2(DMA_A5, ATEV_L, LUMA);
  bool ATEM = mux2(DMA_A4, AVYS_L, LUMA);
  bool AMER = mux2(DMA_A3, ARET_L, LUMA);
  bool APOK = mux2(DMA_A2, ALYR_L, LUMA);
  bool ATOL = mux2(DMA_A1, APUR_L, LUMA);
  bool AMET = mux2(DMA_A0, ALOR_L, LUMA);

  bool COLO = nor (NET02, ASUR);
  bool DEFY = nand(NET01, ASUR);
  bool CYKA = nor (NET02, ATYR);
  bool CEPU = nand(NET01, ATYR);
  bool AJAV = nor (NET02, ATOV);
  bool BADU = nand(NET01, ATOV);
  bool BEVO = nor (NET02, ATEM);
  bool BYLA = nand(NET01, ATEM);
  bool BOLA = nor (NET02, AMER);
  bool BOTY = nand(NET01, AMER);
  bool BAJO = nor (NET02, APOK);
  bool BOKU = nand(NET01, APOK);
  bool COTU = nor (NET02, ATOL);
  bool CABA = nand(NET01, ATOL);
  bool KOTY = nor (NET02, AMET);
  bool KUPO = nand(NET01, AMET);

  A7_D = COLO;
  A7_A = DEFY;
  A6_D = CYKA;
  A6_A = CEPU;
  A5_D = AJAV;
  A5_A = BADU;
  A4_D = BEVO;
  A4_A = BYLA;
  A3_D = BOLA;
  A3_A = BOTY;
  A2_D = BAJO;
  A2_A = BOKU;
  A1_D = COTU;
  A1_A = CABA;
  A0_D = KOTY;
  A0_A = KUPO;

  bool REDU = not(CPU_RD);
  RORU = mux2(REDU, MOTY, T1nT2);
  LULA = not(RORU);

  bool ROGY = nor(RORU, D6);
  bool RYDA = nor(RORU, D7);
  bool RUNE = nor(RORU, D0);
  bool RESY = nor(RORU, D4);
  bool RYPU = nor(RORU, D1);
  bool SULY = nor(RORU, D2);
  bool SEZE = nor(RORU, D3);
  bool TAMU = nor(RORU, D5);

  D6_D = ROGY;
  D7_A = RYDA;
  D0_D = RUNE;
  D4_D = RESY;
  D1_D = RYPU;
  D2_D = SULY;
  D3_D = SEZE;
  D5_D = TAMU;

  bool LAVO = nand(CPU_RAW_RD, TEXO, FROM_CPU5);

  // the rest of this is bus muxes...
  // A0-A7 tristate driven by external pins when NET01 is high? but a5 driven off a5_c?
  // and A8-A14 but not A15?

  (void)LAVO;
}
