// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool FROM_CPU5;
extern bool CLK1;
extern bool RESET6;
extern bool FF46;
extern bool CPU_RD2;
extern bool CPU_WR2;
extern bool PHI_OUTN;
extern bool AMAB;

//----------
// outputs

bool CATY;
bool MOPA_PHI;

bool DMA_A8;
bool DMA_A12;
bool DMA_A10;
bool DMA_A9;
bool DMA_A13;
bool DMA_A11;
bool DMA_A14;
bool DMA_A15;

bool LUMA;
bool OAM_ADDR_DMA;
bool VRAM_TO_OAM;

//----------
// mystery signals

//----------
// registers

bool MAKA = 0;
bool MAKA_CLK =  0;

bool LUVY = 0;
bool LUVY_CLK = 0;

bool LENE = 0;
bool LENE_CLK = 0;

bool MATU = 0;
bool MATU_CLK = 0;

bool MYTE = 0;
bool MYTE_CLK = 0;

// DMA base reg
bool NAFA = 0;
bool NYGY = 0;
bool PARA = 0;
bool PYNE = 0;
bool PULA = 0;
bool NYDO = 0;
bool POKU = 0;
bool MARU = 0;
bool DMA_CLK1 = 0;

// DMA counter reg
bool NAKY = 0;
bool PYRO = 0;
bool NEFY = 0;
bool MUTY = 0;
bool NYKO = 0;
bool PYLO = 0;
bool NUTO = 0;
bool MUGU = 0;
bool DMA_CLK2 = 0;

//-----------------------------------------------------------------------------
// 4_DMA.png

void tick_dma() {
  bool DECY = not(FROM_CPU5);
  CATY = not(DECY);
  bool NAXY = nor(MAKA, LUVY);

  bool MOPA = not(PHI_OUTN);
  MOPA_PHI = MOPA;

  bool LENE_N = not(LENE);
  bool LOKO = nand(RESET6, LENE_N);

  bool MOLU = nand(FF46, CPU_RD2);
  bool LAVY = nand(FF46, CPU_WR2);
  bool NYGO = not(MOLU);
  bool LORU = not(LAVY);

  bool LYXE = unk2(LORU, LOKO);

  bool LUPA = nor(LAVY, LYXE);

  bool POWU = and(MATU, NAXY);

  bool MYTE_N = not(MYTE);

  // FIXME loopy thing, glitch filter? def broken.
  bool LARA = nand(/*LOKY,*/ MYTE_N, RESET6);
  bool LOKY = nand(LARA, LENE_N);

  // dma_a == 159
  bool NAVO = nand(nand(DMA_A0, DMA_A1, DMA_A2), nand(DMA_A3, DMA_A4, DMA_A7));

  bool NOLO = not(NAVO);

  bool WYJA = unk3(AMAB, CPU_WR2, POWU);

  bool PUSY = not(NYGO);

  bool POLY = not(!NAFA);
  bool PARE = not(!NYGY);
  bool REMA = not(!PARA);
  bool ROFO = not(!PYNE);
  bool RALY = not(!PULA);
  bool PANE = not(!NYDO);
  bool RESU = not(!POKU);
  bool NUVY = not(!MARU);

  if (PUSY) {
    D0 = POLY;
    D4 = PARE;
    D2 = REMA;
    D1 = ROFO;
    D5 = RALY;
    D3 = PANE;
    D6 = RESU;
    D7 = NUVY;
  }

  DMA_A8 = NAFA;
  DMA_A12 = NYGY;
  DMA_A10 = PARA;
  DMA_A9 = PYNE;
  DMA_A13 = PULA;
  DMA_A11 = NYDO;
  DMA_A14 = POKU;
  DMA_A15 = MARU;

  bool EVAX = not(NAFA);
  bool EXYF = not(NYGY);
  bool ERAF = not(PARA);
  bool DUVE = not(PYNE);
  bool FUSY = not(NYDO);

  bool AHOC = not(VRAM_TO_OAM);

  if (AHOC) {
    MA8 = EVAX;
    MA12 = EXYF;
    MA10 = ERAF;
    MA9 = DUVE;
    MA11 = FUSY;
  }

  bool LAPA = not(LOKO);
  bool META = and(PHI_OUTN, LOKY);

  DMA_A0 = NAKY;
  DMA_A1 = PYRO;
  DMA_A2 = NEFY;
  DMA_A3 = MUTY;
  DMA_A4 = NYKO;
  DMA_A5 = PYLO;
  DMA_A6 = NUTO;
  DMA_A7 = MUGU;

  bool ECAL = not(NAKY);
  bool EGEZ = not(PYRO);
  bool FUHE = not(NEFY);
  bool FYZY = not(MUTY);
  bool DAMU = not(NYKO);
  bool DAVA = not(PYLO);
  bool ETEG = not(NUTO);
  bool EREW = not(MUGU);

  if (AHOC) {
    MA0 = ECAL;
    MA1 = EGEZ;
    MA2 = FUHE;
    MA3 = FYZY;
    MA4 = DAMU;
    MA5 = DAVA;
    MA6 = ETEG;
    MA7 = EREW;
  }

  bool LEBU = not(DMA_A15);
  bool MUDA = nor(DMA_A13, DMA_A14, LEBU);
  bool LOGO = not(MUDA);
  bool MUHO = nand(MATU, MUDA);
  bool MORY = nand(MATU, LOGO);
  bool DUGA = not(MATU);
  bool LUFA = not(MUHO);
  LUMA = not(MORY);
  OAM_ADDR_DMA = DUGA;
  VRAM_TO_OAM = LUFA;

  //----------
  // registers

  bool MAKA_ = MAKA;
  bool MAKA_CLK_ = CLK1;
  if (MAKA_CLK && !MAKA_CLK_) {
    MAKA_ = CATY;
  }
  if(!RESET6) {
    MAKA_ = 0;
  }

  bool LUVY_ = LUVY;
  bool LUVY_CLK_ = PHI_OUTN;
  if (LUVY_CLK && !LUVY_CLK_) {
    LUVY_ = LUPA;
  }
  if (!RESET6) {
    LUVY_ = 0;
  }

  bool LENE_ = LENE;
  bool LENE_CLK_ = MOPA;
  if (LENE_CLK && !LENE_CLK_) {
    LENE_ = LUVY;
  }
  if (!RESET6) {
    LENE_ = 0;
  }

  bool MATU_ = MATU;
  bool MATU_CLK_ = PHI_OUTN;
  if (MATU_CLK && !MATU_CLK_) {
    MATU_ = LOKY;
  }
  if (!RESET6) {
    MATU_ = 0;
  }

  bool MYTE_ = MYTE;
  bool MYTE_CLK_ = MOPA;
  if (MYTE_CLK && !MYTE_CLK_) {
    MYTE_ = NOLO;
  }
  if (!RESET6) {
    MYTE_ = 0;
  }

  bool DMA_CLK1_ = LORU;
  if (DMA_CLK1 && !DMA_CLK1_) {
    NAFA = D0;
    NYGY = D4;
    PARA = D2;
    PYNE = D1;
    PULA = D5;
    NYDO = D3;
    POKU = D6;
    MARU = D7;
  }
  DMA_CLK1 = DMA_CLK1_;

  bool DMA_CLK2_ = META;
  if (DMA_CLK2 && !DMA_CLK2_) {
    // increment dma counter
  }
  if (!LAPA) {
    // clear dma counter
    NAKY = 0;
    PYRO = 0;
    NEFY = 0;
    MUTY = 0;
    NYKO = 0;
    PYLO = 0;
    NUTO = 0;
    MUGU = 0;
  }
  DMA_CLK2 = DMA_CLK2_;

  //----------
  // writebacks

  LUVY = LUVY_;
  LUVY_CLK =  LUVY_CLK_;

  MAKA = MAKA_;
  MAKA_CLK = MAKA_CLK_;

  LENE = LENE_;
  LENE_CLK = LENE_CLK_;

  MATU = MATU_;
  MATU_CLK = MATU_CLK_;

  MYTE = MYTE_;
  MYTE_CLK = MYTE_CLK_;

  //----------
  // unsunk signals

  (void)NYGO;
  (void)POWU;
  (void)WYJA;
}