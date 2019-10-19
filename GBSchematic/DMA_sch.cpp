// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "MemBus.h"
#include "VramBus.h"
#include "DmaBus.h"
#include "OamBus.h"
#include "CpuBus.h"
#include "AddressDecoder.h"
#include "Clocks.h"

//----------
// inputs

extern bool PHI_OUTn;
extern bool AMAB; // from oam

//----------
// outputs

bool CATY;
bool LUMA;
bool WYJA;
bool MOPA_PHI;

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
  bool NAVO = nand(nand(dma.DMA_A0, dma.DMA_A1, dma.DMA_A2), nand(dma.DMA_A3, dma.DMA_A4, dma.DMA_A7));

  bool NOLO = not(NAVO);

  WYJA = unk3(AMAB, CPU_WR2, POWU);

  bool NAFA_Q = NAFA.tock(LORU, 0, mem.D0);
  bool NYGY_Q = NYGY.tock(LORU, 0, mem.D4);
  bool PARA_Q = PARA.tock(LORU, 0, mem.D2);
  bool PYNE_Q = PYNE.tock(LORU, 0, mem.D1);
  bool PULA_Q = PULA.tock(LORU, 0, mem.D5);
  bool NYDO_Q = NYDO.tock(LORU, 0, mem.D3);
  bool POKU_Q = POKU.tock(LORU, 0, mem.D6);
  bool MARU_Q = MARU.tock(LORU, 0, mem.D7);

  bool POLY = not(!NAFA_Q);
  bool PARE = not(!NYGY_Q);
  bool REMA = not(!PARA_Q);
  bool ROFO = not(!PYNE_Q);
  bool RALY = not(!PULA_Q);
  bool PANE = not(!NYDO_Q);
  bool RESU = not(!POKU_Q);
  bool NUVY = not(!MARU_Q);

  dma.DMA_A8  = NAFA_Q;
  dma.DMA_A12 = NYGY_Q;
  dma.DMA_A10 = PARA_Q;
  dma.DMA_A9  = PYNE_Q;
  dma.DMA_A13 = PULA_Q;
  dma.DMA_A11 = NYDO_Q;
  dma.DMA_A14 = POKU_Q;
  dma.DMA_A15 = MARU_Q;

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

  dma.DMA_A0 = NAKY_Q;
  dma.DMA_A1 = PYRO_Q;
  dma.DMA_A2 = NEFY_Q;
  dma.DMA_A3 = MUTY_Q;
  dma.DMA_A4 = NYKO_Q;
  dma.DMA_A5 = PYLO_Q;
  dma.DMA_A6 = NUTO_Q;
  dma.DMA_A7 = MUGU_Q;

  // tribuffer, not inverter? FIXME - check this elsewhere, we could be driving inverted signals onto the tribus
  bool ECAL = NAKY_Q;
  bool EGEZ = PYRO_Q;
  bool FUHE = NEFY_Q;
  bool FYZY = MUTY_Q;
  bool DAMU = NYKO_Q;
  bool DAVA = PYLO_Q;
  bool ETEG = NUTO_Q;
  bool EREW = MUGU_Q;

  bool LEBU = not(dma.DMA_A15);
  bool MUDA = nor(dma.DMA_A13, dma.DMA_A14, LEBU);
  bool LOGO = not(MUDA);
  bool MUHO = nand(MATU_Q, MUDA);
  bool MORY = nand(MATU_Q, LOGO);
  bool DUGA = not(MATU_Q);
  bool LUFA = not(MUHO);
  LUMA = not(MORY);
  oam.OAM_ADDR_DMA = DUGA;
  VRAM_TO_OAM = LUFA;

  bool PUSY = not(NYGO);
  if (PUSY) {
    mem.D0 = POLY;
    mem.D4 = PARE;
    mem.D2 = REMA;
    mem.D1 = ROFO;
    mem.D5 = RALY;
    mem.D3 = PANE;
    mem.D6 = RESU;
    mem.D7 = NUVY;
  }

  bool AHOC = not(VRAM_TO_OAM);
  if (AHOC) {
    vram.MA0 = ECAL;
    vram.MA1 = EGEZ;
    vram.MA2 = FUHE;
    vram.MA3 = FYZY;
    vram.MA4 = DAMU;
    vram.MA5 = DAVA;
    vram.MA6 = ETEG;
    vram.MA7 = EREW;
    vram.MA8 = EVAX;
    vram.MA9 = DUVE;
    vram.MA10 = ERAF;
    vram.MA11 = FUSY;
    vram.MA12 = EXYF;
  }

  //----------
  // registers

  MAKA.tock(CLK1,     RESET6, CATY);
  LUVY.tock(PHI_OUTn, RESET6, LUPA);
  LENE.tock(MOPA,     RESET6, LUVY_Q);
  MATU.tock(PHI_OUTn, RESET6, LOKY);
  MYTE.tock(MOPA,     LAPA,   NOLO);
}