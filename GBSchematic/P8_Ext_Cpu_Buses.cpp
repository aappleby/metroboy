#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P8_ExtCpuBuses {
  struct Input {
    bool CPU_RD;
    bool CPU_RD_SYNC;
    bool CPU_RAW_RD;
    bool CPU_WRQ;

    bool FROM_CPU3;
    bool FROM_CPU4;
    bool FROM_CPU5;

    bool T1T2n;
    bool T1nT2;
    bool FEXXFFXXn;
    bool NET02;

    bool LUMA;
    bool ABUZ;

    // wat?
    bool A15_C;

    bool A0_IN,A1_IN,A2_IN,A3_IN,A4_IN,A5_IN,A6_IN,A7_IN,A8_IN,A9_IN,A10_IN,A11_IN,A12_IN,A13_IN,A14_IN,A15_IN;
    bool D0_IN,D1_IN,D2_IN,D3_IN,D4_IN,D5_IN,D6_IN,D7_IN;

    bool A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    bool DMA_A0,DMA_A1,DMA_A2,DMA_A3,DMA_A4,DMA_A5,DMA_A6,DMA_A7,DMA_A8,DMA_A9,DMA_A10,DMA_A11,DMA_A12,DMA_A13,DMA_A14,DMA_A15;
  };

  struct Output {
    bool CS_OUT;
    bool WR_A;
    bool WR_C;
    bool RD_A;
    bool RD_C;

    bool NET01;

    bool TOLA_A1n;

    bool A_OE;
    bool A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    // signals to physical address pins
    bool A0_A,A1_A,A2_A,A3_A,A4_A,A5_A,A6_A,A7_A,A8_A,A9_A,A10_A,A11_A,A12_A,A13_A,A14_A,A15_A;
    bool A0_D,A1_D,A2_D,A3_D,A4_D,A5_D,A6_D,A7_D,A8_D,A9_D,A10_D,A11_D,A12_D,A13_D,A14_D,A15_D;

    // signals to physical data pins
    bool D0_D,D1_D,D2_D,D3_D,D4_D,D5_D,D6_D,D7_D;
  };

  // High 7 bits of address latch
  reg LUNO,LYSA,PATE,LUMY,LOBU,LONU,NYRE;

  // Low 8 bits of address latch
  reg ALOR,APUR,ALYR,ARET,AVYS,ATEV,AROS,ARYM;

  // Data input latch
  reg SOMA,RONY,RAXY,SELO,SODY,SAGO,RUPA,SAZY;

  void tick(const Input& in, Output& out) {

    wire TOVA = not(in.T1nT2);
    out.NET01 = TOVA;

    //----------
    // center right

    wire SORE = not(in.A15);
    wire TEVY = and(in.A13, in.A14, SORE);
    wire TEXO = and(in.FROM_CPU4, TEVY);
    wire LEVO = not(TEXO);
    wire LAGU = unk3(in.CPU_RAW_RD, LEVO, in.FROM_CPU3);
    wire LYWE = not(LAGU);

    wire MOCA = nor(TEXO, in.T1T2n);
    wire MEXO = not(in.CPU_RD_SYNC);
    wire NEVY = or(MEXO, MOCA);
    wire MOTY = or(MOCA, LYWE);
    wire PUVA = or(NEVY, in.LUMA);
    wire TYMU = or(in.LUMA, MOTY);
    wire USUF = nor(in.T1nT2, PUVA);
    wire UVER = nand(PUVA, out.NET01);
    wire UGAC = nand(out.NET01, TYMU);
    wire URUN = nor(TYMU, in.T1nT2);

    out.WR_A = UVER;
    out.WR_C = USUF;
    out.RD_A = UGAC;
    out.RD_C = URUN;

    //----------
    // top center

    wire TOLA = not(in.A1);
    out.TOLA_A1n = TOLA;

    wire MULE = not(in.T1T2n);
    wire LOXO = unk3(MULE, TEXO, in.T1T2n);
    wire LASY = not(LOXO);
    wire MATE = not(LASY);

    //----------
    // left center

    wire SOGY = not(in.A14);
    wire TUMA = and(in.A13, SOGY, in.A15);
    wire TYNU = unk3(in.A15, in.A14, TUMA);
    wire TOZA = and(TYNU, in.ABUZ, in.FEXXFFXXn);
    wire SOBY = nor(in.A15, in.CPU_WRQ); // schematic has a question mark?
    wire SEPY = nand(in.ABUZ, SOBY);

    wire TYHO = mux2(in.DMA_A15, TOZA, in.LUMA);
    out.CS_OUT = TYHO;

    wire TAZY = mux2(in.DMA_A15, SEPY, in.LUMA);

    wire RYCA = not(in.NET02);
    wire RAZA = not(in.A15_C);
    wire SYZU = not(RAZA);
    out.A15 = SYZU;
    wire RULO = nor(TAZY, in.NET02);
    wire SUZE = nand(TAZY, RYCA);

    out.A15_D = RULO;
    out.A15_A = SUZE;

    //----------
    // bottom left

    wire LUNO_Q = LUNO.latch(MATE, in.A8);
    wire LYSA_Q = LYSA.latch(MATE, in.A9);
    wire PATE_Q = PATE.latch(MATE, in.A10);
    wire LUMY_Q = LUMY.latch(MATE, in.A11);
    wire LOBU_Q = LOBU.latch(MATE, in.A12);
    wire LONU_Q = LONU.latch(MATE, in.A13);
    wire NYRE_Q = NYRE.latch(MATE, in.A14);

    wire MANO = mux2(in.DMA_A8,  LUNO_Q, in.LUMA);
    wire MASU = mux2(in.DMA_A9,  LYSA_Q, in.LUMA);
    wire PAMY = mux2(in.DMA_A10, PATE_Q, in.LUMA);
    wire MALE = mux2(in.DMA_A11, LUMY_Q, in.LUMA);
    wire MOJY = mux2(in.DMA_A12, LOBU_Q, in.LUMA);
    wire MUCE = mux2(in.DMA_A13, LONU_Q, in.LUMA);
    wire PEGE = mux2(in.DMA_A14, NYRE_Q, in.LUMA);

    wire MEGO = nor(in.T1nT2, MANO);
    wire MENY = nor(in.T1nT2, MASU);
    wire RORE = nor(in.T1nT2, PAMY);
    wire LYNY = nor(in.T1nT2, MALE);
    wire LOSO = nor(in.T1nT2, MOJY);
    wire LEVA = nor(in.T1nT2, MUCE);
    wire PAHY = nor(in.T1nT2, PEGE);

    wire MYNY = nand(MANO, TOVA);
    wire MUNE = nand(MASU, TOVA);
    wire ROXU = nand(PAMY, TOVA);
    wire LEPY = nand(MALE, TOVA);
    wire LUCE = nand(MOJY, TOVA);
    wire LABE = nand(MUCE, TOVA);
    wire PUHE = nand(PEGE, TOVA);

    out.A8_D = MEGO;
    out.A9_D = MENY;
    out.A10_D = RORE;
    out.A11_D = LYNY;
    out.A12_D = LOSO;
    out.A13_D = LEVA;
    out.A14_D = PAHY;

    out.A8_A = MYNY;
    out.A9_A = MUNE;
    out.A10_A = ROXU;
    out.A11_A = LEPY;
    out.A12_A = LUCE;
    out.A13_A = LABE;
    out.A14_A = PUHE;

    //----------
    // the rest of the address latch, center

    wire ALOR_Q = ALOR.latch(MATE, in.A0);
    wire APUR_Q = APUR.latch(MATE, in.A1);
    wire ALYR_Q = ALYR.latch(MATE, in.A2);
    wire ARET_Q = ARET.latch(MATE, in.A3);
    wire AVYS_Q = AVYS.latch(MATE, in.A4);
    wire ATEV_Q = ATEV.latch(MATE, in.A5);
    wire AROS_Q = AROS.latch(MATE, in.A6);
    wire ARYM_Q = ARYM.latch(MATE, in.A7);

    wire AMET = mux2(in.DMA_A0, ALOR_Q, in.LUMA);
    wire ATOL = mux2(in.DMA_A1, APUR_Q, in.LUMA);
    wire APOK = mux2(in.DMA_A2, ALYR_Q, in.LUMA);
    wire AMER = mux2(in.DMA_A3, ARET_Q, in.LUMA);
    wire ATEM = mux2(in.DMA_A4, AVYS_Q, in.LUMA);
    wire ATOV = mux2(in.DMA_A5, ATEV_Q, in.LUMA);
    wire ATYR = mux2(in.DMA_A6, AROS_Q, in.LUMA);
    wire ASUR = mux2(in.DMA_A7, ARYM_Q, in.LUMA);

    wire KUPO = nand(out.NET01, AMET);
    wire CABA = nand(out.NET01, ATOL);
    wire BOKU = nand(out.NET01, APOK);
    wire BOTY = nand(out.NET01, AMER);
    wire BYLA = nand(out.NET01, ATEM);
    wire BADU = nand(out.NET01, ATOV);
    wire CEPU = nand(out.NET01, ATYR);
    wire DEFY = nand(out.NET01, ASUR);

    out.A0_A = KUPO;
    out.A1_A = CABA;
    out.A2_A = BOKU;
    out.A3_A = BOTY;
    out.A4_A = BYLA;
    out.A5_A = BADU;
    out.A6_A = CEPU;
    out.A7_A = DEFY;

    wire KOTY = nor(in.NET02, AMET);
    wire COTU = nor(in.NET02, ATOL);
    wire BAJO = nor(in.NET02, APOK);
    wire BOLA = nor(in.NET02, AMER);
    wire BEVO = nor(in.NET02, ATEM);
    wire AJAV = nor(in.NET02, ATOV);
    wire CYKA = nor(in.NET02, ATYR);
    wire COLO = nor(in.NET02, ASUR);

    out.A0_D = KOTY;
    out.A1_D = COTU;
    out.A2_D = BAJO;
    out.A3_D = BOLA;
    out.A4_D = BEVO;
    out.A5_D = AJAV;
    out.A6_D = CYKA;
    out.A7_D = COLO;

    //----------
    // bottom right

    wire REDU = not(in.CPU_RD);
    wire RORU = mux2(REDU, MOTY, in.T1nT2);
    wire LULA = not(RORU);

    wire ROGY = nor(RORU, in.D6);
    wire RYDA = nor(RORU, in.D7);
    wire RUNE = nor(RORU, in.D0);
    wire RESY = nor(RORU, in.D4);
    wire RYPU = nor(RORU, in.D1);
    wire SULY = nor(RORU, in.D2);
    wire SEZE = nor(RORU, in.D3);
    wire TAMU = nor(RORU, in.D5);

    out.D0_D = RUNE;
    out.D1_D = RYPU;
    out.D2_D = SULY;
    out.D3_D = SEZE;
    out.D4_D = RESY;
    out.D5_D = TAMU;
    out.D6_D = ROGY;
    out.D7_D = RYDA; // was D7_A on schematic, probably a typo?

    //----------
    // DX_IN latch, bottom right

    wire LAVO = nand(in.CPU_RAW_RD, TEXO, in.FROM_CPU5);

    wire SOMA_Q = SOMA.latch(in.D0_IN, LAVO);
    wire RONY_Q = RONY.latch(in.D1_IN, LAVO);
    wire RAXY_Q = RAXY.latch(in.D2_IN, LAVO);
    wire SELO_Q = SELO.latch(in.D3_IN, LAVO);
    wire SODY_Q = SODY.latch(in.D4_IN, LAVO);
    wire SAGO_Q = SAGO.latch(in.D5_IN, LAVO);
    wire RUPA_Q = RUPA.latch(in.D6_IN, LAVO);
    wire SAZY_Q = SAZY.latch(in.D7_IN, LAVO); // schematic mislabeled PIN_D7

    // looks non-inverting?
    wire TEPE = SODY_Q;
    wire TAVO = SELO_Q;
    wire RUVO = RONY_Q;
    wire RYMA = SOMA_Q;
    wire RYKO = RAXY_Q;
    wire SEVU = RUPA_Q;
    wire SAFO = SAGO_Q;
    wire TAJU = SAZY_Q;

    // why would you latch and buffer with the same signal?
    if (LAVO) {
      out.D_OE = true;
      out.D0 = RYMA;
      out.D1 = RUVO;
      out.D2 = RYKO;
      out.D3 = TAVO;
      out.D4 = TEPE;
      out.D5 = SAFO;
      out.D6 = SEVU;
      out.D7 = TAJU;
    }

    //----------
    // if NET01 high, drive external address bus onto internal address bus for debugging.

    if (out.NET01) {
      out.A_OE= true;
      wire KOVA = not(in.A0_IN);  wire KEJO = not(KOVA); out.A0  = KEJO;
      wire CAMU = not(in.A1_IN);  wire BYXE = not(CAMU); out.A1  = BYXE;
      wire BUXU = not(in.A2_IN);  wire AKAN = not(BUXU); out.A2  = AKAN;
      wire BASE = not(in.A3_IN);  wire ANAR = not(BASE); out.A3  = ANAR;
      wire AFEC = not(in.A4_IN);  wire AZUV = not(AFEC); out.A4  = AZUV;
      wire ABUP = not(in.A5_IN);  wire AJOV = not(ABUP); out.A5  = AJOV;
      wire CYGU = not(in.A6_IN);  wire BYNE = not(CYGU); out.A6  = BYNE;
      wire COGO = not(in.A7_IN);  wire BYNA = not(COGO); out.A7  = BYNA;
      wire MUJY = not(in.A8_IN);  wire LOFA = not(MUJY); out.A8  = LOFA;
      wire NENA = not(in.A9_IN);  wire MAPU = not(NENA); out.A9  = MAPU;
      wire SURA = not(in.A10_IN); wire RALA = not(SURA); out.A10 = RALA;
      wire MADY = not(in.A11_IN); wire LORA = not(MADY); out.A11 = LORA;
      wire LAHE = not(in.A12_IN); wire LYNA = not(LAHE); out.A12 = LYNA;
      wire LURA = not(in.A13_IN); wire LEFY = not(LURA); out.A13 = LEFY;
      wire PEVO = not(in.A14_IN); wire NEFE = not(PEVO); out.A14 = NEFE;
    }
  }
};
