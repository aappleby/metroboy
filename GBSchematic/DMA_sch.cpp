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
extern bool PHI_OUTn;
extern bool AMAB;

//----------
// outputs

bool CATY;
bool LUMA;
bool WYJA;
bool MOPA_PHI;

bool DMA_A8;
bool DMA_A12;
bool DMA_A10;
bool DMA_A9;
bool DMA_A13;
bool DMA_A11;
bool DMA_A14;
bool DMA_A15;

bool OAM_ADDR_DMA;
bool VRAM_TO_OAM;

//----------
// registers

reg MAKA;
reg LUVY;
reg MYTE;
reg LENE;
reg MATU;

// DMA base reg
reg NAFA;
reg NYGY;
reg PARA;
reg PYNE;
reg PULA;
reg NYDO;
reg POKU;
reg MARU;

// DMA counter reg
reg NAKY;
reg PYRO;
reg NEFY;
reg MUTY;
reg NYKO;
reg PYLO;
reg NUTO;
reg MUGU;

//-----------------------------------------------------------------------------
// 4_DMA.png

void tick_dma() {
  bool MAKA_Q = MAKA.q();
  bool LUVY_Q = LUVY.q();
  bool MYTE_Q = MYTE.q();
  bool LENE_Q = LENE.q();
  bool MATU_Q = MATU.q();

  bool DECY = not(FROM_CPU5);
  CATY = not(DECY);
  bool NAXY = nor(MAKA_Q, LUVY_Q);

  bool MOPA = not(PHI_OUTn);
  MOPA_PHI = MOPA;

  bool LENE_Qn = not(LENE_Q);
  bool LOKO = nand(RESET6, LENE_Qn);

  bool MOLU = nand(FF46, CPU_RD2);
  bool LAVY = nand(FF46, CPU_WR2);
  bool NYGO = not(MOLU);
  bool LORU = not(LAVY);

  bool LYXE = unk2(LORU, LOKO);

  bool LUPA = nor(LAVY, LYXE);

  bool POWU = and(MATU_Q, NAXY);

  // FIXME loopy thing, glitch filter? def broken.
  bool LARA = nand(/*LOKY,*/ !MYTE_Q, RESET6);
  bool LOKY = nand(LARA, LENE_Qn);

  // dma_a == 159
  bool NAVO = nand(nand(DMA_A0, DMA_A1, DMA_A2), nand(DMA_A3, DMA_A4, DMA_A7));

  bool NOLO = not(NAVO);

  WYJA = unk3(AMAB, CPU_WR2, POWU);

  bool NAFA_Q = NAFA.tock(LORU, 0, D0);
  bool NYGY_Q = NYGY.tock(LORU, 0, D4);
  bool PARA_Q = PARA.tock(LORU, 0, D2);
  bool PYNE_Q = PYNE.tock(LORU, 0, D1);
  bool PULA_Q = PULA.tock(LORU, 0, D5);
  bool NYDO_Q = NYDO.tock(LORU, 0, D3);
  bool POKU_Q = POKU.tock(LORU, 0, D6);
  bool MARU_Q = MARU.tock(LORU, 0, D7);

  bool POLY = not(!NAFA_Q);
  bool PARE = not(!NYGY_Q);
  bool REMA = not(!PARA_Q);
  bool ROFO = not(!PYNE_Q);
  bool RALY = not(!PULA_Q);
  bool PANE = not(!NYDO_Q);
  bool RESU = not(!POKU_Q);
  bool NUVY = not(!MARU_Q);

  DMA_A8  = NAFA_Q;
  DMA_A12 = NYGY_Q;
  DMA_A10 = PARA_Q;
  DMA_A9  = PYNE_Q;
  DMA_A13 = PULA_Q;
  DMA_A11 = NYDO_Q;
  DMA_A14 = POKU_Q;
  DMA_A15 = MARU_Q;

  bool EVAX = not(NAFA_Q);
  bool EXYF = not(NYGY_Q);
  bool ERAF = not(PARA_Q);
  bool DUVE = not(PYNE_Q);
  bool FUSY = not(NYDO_Q);


  bool LAPA = not(LOKO);
  bool META = and(PHI_OUTn, LOKY);

  bool NAKY_Q = NAKY.flip(META,    LAPA);
  bool PYRO_Q = PYRO.flip(!NAKY_Q, LAPA);
  bool NEFY_Q = NEFY.flip(!PYRO_Q, LAPA);
  bool MUTY_Q = MUTY.flip(!NEFY_Q, LAPA);
  bool NYKO_Q = NYKO.flip(!MUTY_Q, LAPA);
  bool PYLO_Q = PYLO.flip(!NYKO_Q, LAPA);
  bool NUTO_Q = NUTO.flip(!PYLO_Q, LAPA);
  bool MUGU_Q = MUGU.flip(!NUTO_Q, LAPA);

  DMA_A0 = NAKY_Q;
  DMA_A1 = PYRO_Q;
  DMA_A2 = NEFY_Q;
  DMA_A3 = MUTY_Q;
  DMA_A4 = NYKO_Q;
  DMA_A5 = PYLO_Q;
  DMA_A6 = NUTO_Q;
  DMA_A7 = MUGU_Q;

  // tribuffer, not inverter? FIXME - check this elsewhere, we could be driving inverted signals onto the tribus
  bool ECAL = NAKY_Q;
  bool EGEZ = PYRO_Q;
  bool FUHE = NEFY_Q;
  bool FYZY = MUTY_Q;
  bool DAMU = NYKO_Q;
  bool DAVA = PYLO_Q;
  bool ETEG = NUTO_Q;
  bool EREW = MUGU_Q;

  bool LEBU = not(DMA_A15);
  bool MUDA = nor(DMA_A13, DMA_A14, LEBU);
  bool LOGO = not(MUDA);
  bool MUHO = nand(MATU_Q, MUDA);
  bool MORY = nand(MATU_Q, LOGO);
  bool DUGA = not(MATU_Q);
  bool LUFA = not(MUHO);
  LUMA = not(MORY);
  OAM_ADDR_DMA = DUGA;
  VRAM_TO_OAM = LUFA;

  bool PUSY = not(NYGO);
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

  bool AHOC = not(VRAM_TO_OAM);
  if (AHOC) {
    MA0 = ECAL;
    MA1 = EGEZ;
    MA2 = FUHE;
    MA3 = FYZY;
    MA4 = DAMU;
    MA5 = DAVA;
    MA6 = ETEG;
    MA7 = EREW;
    MA8 = EVAX;
    MA9 = DUVE;
    MA10 = ERAF;
    MA11 = FUSY;
    MA12 = EXYF;
  }

  //----------
  // registers

  MAKA.tock(CLK1,     RESET6, CATY);
  LUVY.tock(PHI_OUTn, RESET6, LUPA);
  LENE.tock(MOPA,     RESET6, LUVY_Q);
  MATU.tock(PHI_OUTn, RESET6, LOKY);
  MYTE.tock(MOPA,     LAPA,   NOLO);
}