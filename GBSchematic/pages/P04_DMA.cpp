#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P4_DMA {
  struct Input {
    bool FROM_CPU5;
    bool AMAB;
    bool CPU_WR2;
    bool PHI_OUTn;
    bool CLK1;
    bool CPU_RD2;
    bool FF46;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
    bool RESET6;
  };

  struct Output {
    bool CATY;
    bool WYJA;
    bool MOPA_PHI;
    bool LUMA;    // This is the "DMA READING NOW" signal, it overrides the cpu on the cart wr/rd pins
    bool OAM_ADDR_DMA;
    bool VRAM_TO_OAM;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    bool MA_OE;
    bool MA0,MA1,MA2,MA3,MA4,MA5,MA6,MA7,MA8,MA9,MA10,MA11,MA12;

    bool DMA_A0,DMA_A1,DMA_A2,DMA_A3,DMA_A4,DMA_A5,DMA_A6,DMA_A7,DMA_A8,DMA_A9,DMA_A10,DMA_A11,DMA_A12,DMA_A13,DMA_A14,DMA_A15;
  };

  // FF46 DMA
  reg NAFA,NYGY,PARA,PYNE,PULA,NYDO,POKU,MARU;

  // DMA counter
  reg NAKY,PYRO,NEFY,MUTY,NYKO,PYLO,NUTO,MUGU;

  // Misc state
  reg MAKA;
  reg LUVY;
  reg MYTE;
  reg LENE;
  reg MATU;

  void tick(const Input& in, Output& out) {
    wire NAKY_Q = NAKY.q();
    wire PYRO_Q = PYRO.q();
    wire NEFY_Q = NEFY.q();
    wire MUTY_Q = MUTY.q();
    wire NYKO_Q = NYKO.q();
    wire PYLO_Q = PYLO.q();
    wire NUTO_Q = NUTO.q();
    wire MUGU_Q = MUGU.q();
    wire NAFA_Q = NAFA.q();
    wire PYNE_Q = PYNE.q();
    wire PARA_Q = PARA.q();
    wire NYDO_Q = NYDO.q();
    wire NYGY_Q = NYGY.q();
    wire PULA_Q = PULA.q();
    wire POKU_Q = POKU.q();
    wire MARU_Q = MARU.q();

    out.DMA_A0  = NAKY_Q;
    out.DMA_A1  = PYRO_Q;
    out.DMA_A2  = NEFY_Q;
    out.DMA_A3  = MUTY_Q;
    out.DMA_A4  = NYKO_Q;
    out.DMA_A5  = PYLO_Q;
    out.DMA_A6  = NUTO_Q;
    out.DMA_A7  = MUGU_Q;
    out.DMA_A8  = NAFA_Q;
    out.DMA_A9  = PYNE_Q;
    out.DMA_A10 = PARA_Q;
    out.DMA_A11 = NYDO_Q;
    out.DMA_A12 = NYGY_Q;
    out.DMA_A13 = PULA_Q;
    out.DMA_A14 = POKU_Q;
    out.DMA_A15 = MARU_Q;

    wire LENE_Q = LENE.q();
    wire LOKO = nand(in.RESET6, !LENE_Q);
    wire MOLU = nand(in.FF46, in.CPU_RD2);
    wire LAVY = nand(in.FF46, in.CPU_WR2);
    wire NYGO = not(MOLU);
    wire LORU = not(LAVY);
    wire LYXE = unk2(LORU, LOKO);
    wire LUPA = nor(LAVY, LYXE);

    wire DECY = not(in.FROM_CPU5);
    wire CATY = not(DECY);
    wire MAKA_Q = MAKA.tock(in.CLK1, in.RESET6, CATY);
    wire LUVY_Q = LUVY.tock(in.PHI_OUTn, in.RESET6, LUPA);
    wire NAXY = nor(MAKA_Q, LUVY_Q);
    wire MATU_Q = MATU.q();
    wire POWU = and(MATU_Q, NAXY);
    wire WYJA = unk3(in.AMAB, in.CPU_WR2, POWU);
    wire MOPA = not(in.PHI_OUTn);
    LENE.tock(MOPA, in.RESET6, LUVY_Q);

    wire LAPA = not(LOKO);
    wire NAVO = nand(nand(out.DMA_A0, out.DMA_A1, out.DMA_A2), nand(out.DMA_A3, out.DMA_A4, out.DMA_A7));
    wire NOLO = not(NAVO);
    wire MYTE_Q = MYTE.tock(MOPA, LAPA,   NOLO);
    // FIXME loopy thing, glitch filter? def broken.
    wire LARA = nand(/*LOKY,*/ !MYTE_Q, in.RESET6);
    wire LOKY = nand(LARA, !LENE_Q);
    MATU.tock(in.PHI_OUTn, in.RESET6, LOKY);

    wire META = and(in.PHI_OUTn, LOKY);
    wire LEBU = not(out.DMA_A15);
    wire MUDA = nor(out.DMA_A13, out.DMA_A14, LEBU);
    wire LOGO = not(MUDA);
    wire MORY = nand(MATU_Q, LOGO);
    wire LUMA = not(MORY);
    wire MUHO = nand(MATU_Q, MUDA);
    wire DUGA = not(MATU_Q);
    wire LUFA = not(MUHO);

    out.CATY = CATY;
    out.WYJA = WYJA;
    out.MOPA_PHI = MOPA;
    out.LUMA = LUMA;
    out.OAM_ADDR_DMA = DUGA;
    out.VRAM_TO_OAM = LUFA;

    NAFA.tock(LORU, 0, in.D0);
    NYGY.tock(LORU, 0, in.D4);
    PARA.tock(LORU, 0, in.D2);
    PYNE.tock(LORU, 0, in.D1);
    PULA.tock(LORU, 0, in.D5);
    NYDO.tock(LORU, 0, in.D3);
    POKU.tock(LORU, 0, in.D6);
    MARU.tock(LORU, 0, in.D7);

    bool PUSY = not(NYGO);
    wire POLY = not(!NAFA_Q);
    wire PARE = not(!NYGY_Q);
    wire REMA = not(!PARA_Q);
    wire ROFO = not(!PYNE_Q);
    wire RALY = not(!PULA_Q);
    wire PANE = not(!NYDO_Q);
    wire RESU = not(!POKU_Q);
    wire NUVY = not(!MARU_Q);

    if (PUSY) {
      out.D_OE = true;
      out.D0 = POLY;
      out.D4 = PARE;
      out.D2 = REMA;
      out.D1 = ROFO;
      out.D5 = RALY;
      out.D3 = PANE;
      out.D6 = RESU;
      out.D7 = NUVY;
    }

    NAKY.flip(META,    LAPA);
    PYRO.flip(!NAKY_Q, LAPA);
    NEFY.flip(!PYRO_Q, LAPA);
    MUTY.flip(!NEFY_Q, LAPA);
    NYKO.flip(!MUTY_Q, LAPA);
    PYLO.flip(!NYKO_Q, LAPA);
    NUTO.flip(!PYLO_Q, LAPA);
    MUGU.flip(!NUTO_Q, LAPA);

    bool AHOC = not(out.VRAM_TO_OAM);
    // tribuffer, not inverter? FIXME - check this elsewhere, we could be driving inverted signals onto the tribus
    bool ECAL = NAKY_Q;
    bool EGEZ = PYRO_Q;
    bool FUHE = NEFY_Q;
    bool FYZY = MUTY_Q;
    bool DAMU = NYKO_Q;
    bool DAVA = PYLO_Q;
    bool ETEG = NUTO_Q;
    bool EREW = MUGU_Q;
    wire EVAX = NAFA_Q;
    wire EXYF = NYGY_Q;
    wire ERAF = PARA_Q;
    wire DUVE = PYNE_Q;
    wire FUSY = NYDO_Q;

    if (AHOC) {
      out.MA_OE = true;
      out.MA0 = ECAL;
      out.MA1 = EGEZ;
      out.MA2 = FUHE;
      out.MA3 = FYZY;
      out.MA4 = DAMU;
      out.MA5 = DAVA;
      out.MA6 = ETEG;
      out.MA7 = EREW;
      out.MA8 = EVAX;
      out.MA9 = DUVE;
      out.MA10 = ERAF;
      out.MA11 = FUSY;
      out.MA12 = EXYF;
    }
  }
};
