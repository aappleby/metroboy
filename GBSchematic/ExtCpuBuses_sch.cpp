// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "CpuBus.h"
#include "ExtBus.h"
#include "MemBus.h"
#include "DmaBus.h"
#include "AddressDecoder.h"

//----------
// inputs

extern bool ABUZ; // from clocksreset
extern bool LUMA; // from dma?

//----------
// outputs

bool LULA;
bool RORU;
bool TEXO; // used by vram interface

//----------
// registers

reg NYRE;
reg LONU;
reg LOBU;
reg LUMY;
reg PATE;
reg LYSA;
reg LUNO;
reg ARYM;
reg AROS;
reg ATEV;
reg AVYS;
reg ARET;
reg ALYR;
reg APUR;
reg ALOR;

// DX_IN latch
reg SODY;
reg SELO;
reg RONY;
reg SOMA;
reg RAXY;
reg RUPA;
reg SAGO;
reg SAZY;

//-----------------------------------------------------------------------------

void tick_extbus() {

  //----------
  // center right

  wire SORE = not(mem.A15);
  wire TEVY = and(mem.A13, mem.A14, SORE);
  TEXO = and(cpu.FROM_CPU4, TEVY);
  wire LEVO = not(TEXO);
  wire LAGU = unk3(CPU_RAW_RD, LEVO, cpu.FROM_CPU3);
  wire LYWE = not(LAGU);

  wire MOCA = nor(TEXO, T1T2n);
  wire MEXO = not(CPU_RD_SYNC);
  wire NEVY = or(MEXO, MOCA);
  wire MOTY = or(MOCA, LYWE);
  wire PUVA = or(NEVY, LUMA);
  wire TYMU = or(LUMA, MOTY);
  wire USUF = nor(T1nT2, PUVA);
  wire UVER = nand(PUVA, NET01);
  wire UGAC = nand(NET01, TYMU);
  wire URUN = nor(TYMU, T1nT2);

  WR_C = USUF;
  WR_A = UVER;
  RD_A = UGAC;
  RD_C = URUN;

  //----------
  // top center

  wire TOLA = not(mem.A1);
  TOLA_A1n = TOLA;

  wire MULE = not(T1T2n);
  wire LOXO = unk3(MULE, TEXO, T1T2n);
  wire LASY = not(LOXO);
  wire MATE = not(LASY);

  //----------
  // left center

  wire SOGY = not(mem.A14);
  wire TUMA = and(mem.A13, SOGY, mem.A15);
  wire TYNU = unk3(mem.A15, mem.A14, TUMA);
  wire TOZA = and(TYNU, ABUZ, FEXXFFXXn);
  wire SOBY = nor(mem.A15, CPU_WRQ); // schematic has a question mark?
  wire SEPY = nand(ABUZ, SOBY);

  wire TYHO = mux2(dma.DMA_A15, TOZA, LUMA);
  CS_OUT = TYHO;

  wire TAZY = mux2(dma.DMA_A15, SEPY, LUMA);

  wire RYCA = not(NET02);
  wire RAZA = not(ext.A15_C);
  wire SYZU = not(RAZA);
  mem.A15 = SYZU;
  wire RULO = nor(TAZY, NET02);
  wire SUZE = nand(TAZY, RYCA);

  ext.A15_D = RULO;
  ext.A15_A = SUZE;

  //----------
  // bottom left

  wire TOVA = not(T1nT2);
  NET01 = TOVA;

  wire NYRE_Q = NYRE.latch(MATE, mem.A14);
  wire LONU_Q = LONU.latch(MATE, mem.A13);
  wire LOBU_Q = LOBU.latch(MATE, mem.A12);
  wire LUMY_Q = LUMY.latch(MATE, mem.A11);
  wire PATE_Q = PATE.latch(MATE, mem.A10);
  wire LYSA_Q = LYSA.latch(MATE, mem.A9);
  wire LUNO_Q = LUNO.latch(MATE, mem.A8);

  wire PEGE = mux2(dma.DMA_A14, NYRE_Q, LUMA);
  wire MUCE = mux2(dma.DMA_A13, LONU_Q, LUMA);
  wire MOJY = mux2(dma.DMA_A12, LOBU_Q, LUMA);
  wire MALE = mux2(dma.DMA_A11, LUMY_Q, LUMA);
  wire PAMY = mux2(dma.DMA_A10, PATE_Q, LUMA);
  wire MASU = mux2(dma.DMA_A9,  LYSA_Q, LUMA);
  wire MANO = mux2(dma.DMA_A8,  LUNO_Q, LUMA);

  wire PAHY = nor(T1nT2, PEGE);
  wire LEVA = nor(T1nT2, MUCE);
  wire LOSO = nor(T1nT2, MOJY);
  wire LYNY = nor(T1nT2, MALE);
  wire RORE = nor(T1nT2, PAMY);
  wire MENY = nor(T1nT2, MASU);
  wire MEGO = nor(T1nT2, MANO);

  wire PUHE = nand(PEGE, TOVA);
  wire LABE = nand(MUCE, TOVA);
  wire LUCE = nand(MOJY, TOVA);
  wire LEPY = nand(MALE, TOVA);
  wire ROXU = nand(PAMY, TOVA);
  wire MUNE = nand(MASU, TOVA);
  wire MYNY = nand(MANO, TOVA);

  ext.A14_D = PAHY;
  ext.A13_D = LEVA;
  ext.A12_D = LOSO;
  ext.A11_D = LYNY;
  ext.A10_D = RORE;
  ext.A9_D = MENY;
  ext.A8_D = MEGO;

  ext.A14_A = PUHE;
  ext.A13_A = LABE;
  ext.A12_A = LUCE;
  ext.A11_A = LEPY;
  ext.A10_A = ROXU;
  ext.A9_A = MUNE;
  ext.A8_A = MYNY;

  //----------
  // the rest of the address latch, center

  wire ARYM_Q = ARYM.latch(MATE, mem.A7);
  wire AROS_Q = ARYM.latch(MATE, mem.A6);
  wire ATEV_Q = ARYM.latch(MATE, mem.A5);
  wire AVYS_Q = ARYM.latch(MATE, mem.A4);
  wire ARET_Q = ARYM.latch(MATE, mem.A3);
  wire ALYR_Q = ARYM.latch(MATE, mem.A2);
  wire APUR_Q = ARYM.latch(MATE, mem.A1);
  wire ALOR_Q = ARYM.latch(MATE, mem.A0);

  wire ASUR = mux2(dma.DMA_A7, ARYM_Q, LUMA);
  wire ATYR = mux2(dma.DMA_A6, AROS_Q, LUMA);
  wire ATOV = mux2(dma.DMA_A5, ATEV_Q, LUMA);
  wire ATEM = mux2(dma.DMA_A4, AVYS_Q, LUMA);
  wire AMER = mux2(dma.DMA_A3, ARET_Q, LUMA);
  wire APOK = mux2(dma.DMA_A2, ALYR_Q, LUMA);
  wire ATOL = mux2(dma.DMA_A1, APUR_Q, LUMA);
  wire AMET = mux2(dma.DMA_A0, ALOR_Q, LUMA);

  wire COLO = nor (NET02, ASUR);
  wire DEFY = nand(NET01, ASUR);
  wire CYKA = nor (NET02, ATYR);
  wire CEPU = nand(NET01, ATYR);
  wire AJAV = nor (NET02, ATOV);
  wire BADU = nand(NET01, ATOV);
  wire BEVO = nor (NET02, ATEM);
  wire BYLA = nand(NET01, ATEM);
  wire BOLA = nor (NET02, AMER);
  wire BOTY = nand(NET01, AMER);
  wire BAJO = nor (NET02, APOK);
  wire BOKU = nand(NET01, APOK);
  wire COTU = nor (NET02, ATOL);
  wire CABA = nand(NET01, ATOL);
  wire KOTY = nor (NET02, AMET);
  wire KUPO = nand(NET01, AMET);

  ext.A7_D = COLO;
  ext.A7_A = DEFY;
  ext.A6_D = CYKA;
  ext.A6_A = CEPU;
  ext.A5_D = AJAV;
  ext.A5_A = BADU;
  ext.A4_D = BEVO;
  ext.A4_A = BYLA;
  ext.A3_D = BOLA;
  ext.A3_A = BOTY;
  ext.A2_D = BAJO;
  ext.A2_A = BOKU;
  ext.A1_D = COTU;
  ext.A1_A = CABA;
  ext.A0_D = KOTY;
  ext.A0_A = KUPO;

  //----------
  // bottom right

  wire REDU = not(CPU_RD);
  RORU = mux2(REDU, MOTY, T1nT2);
  LULA = not(RORU);

  wire ROGY = nor(RORU, mem.D6);
  wire RYDA = nor(RORU, mem.D7);
  wire RUNE = nor(RORU, mem.D0);
  wire RESY = nor(RORU, mem.D4);
  wire RYPU = nor(RORU, mem.D1);
  wire SULY = nor(RORU, mem.D2);
  wire SEZE = nor(RORU, mem.D3);
  wire TAMU = nor(RORU, mem.D5);

  ext.D6_D = ROGY;
  ext.D7_D = RYDA; // was D7_A on schematic, probably a typo?
  ext.D0_D = RUNE;
  ext.D4_D = RESY;
  ext.D1_D = RYPU;
  ext.D2_D = SULY;
  ext.D3_D = SEZE;
  ext.D5_D = TAMU;

  //----------
  // DX_IN latch, bottom right

  wire LAVO = nand(CPU_RAW_RD, TEXO, cpu.FROM_CPU5);

  wire SODY_Q = SODY.latch(ext.D4_IN, LAVO);
  wire SELO_Q = SELO.latch(ext.D3_IN, LAVO);
  wire RONY_Q = RONY.latch(ext.D1_IN, LAVO);
  wire SOMA_Q = SOMA.latch(ext.D0_IN, LAVO);
  wire RAXY_Q = RAXY.latch(ext.D2_IN, LAVO);
  wire RUPA_Q = RUPA.latch(ext.D6_IN, LAVO);
  wire SAGO_Q = SAGO.latch(ext.D5_IN, LAVO);
  wire SAZY_Q = SAZY.latch(ext.D7_IN, LAVO); // schematic mislabeled PIN_D7

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
    mem.D4 = TEPE;
    mem.D3 = TAVO;
    mem.D1 = RUVO;
    mem.D0 = RYMA;
    mem.D2 = RYKO;
    mem.D6 = SEVU;
    mem.D5 = SAFO;
    mem.D7 = TAJU;
  }

  //----------
  // some debugging mode thing to drive the internal address bus from the external pins?

  bool BASE = not(ext.A3_IN);
  bool AFEC = not(ext.A4_IN);
  bool BUXU = not(ext.A2_IN);
  bool CAMU = not(ext.A1_IN);
  bool CYGU = not(ext.A6_IN);
  bool COGO = not(ext.A7_IN);
  bool KOVA = not(ext.A0_IN);
  bool LAHE = not(ext.A12_IN);
  bool LURA = not(ext.A13_IN);
  bool MUJY = not(ext.A8_IN);
  bool PEVO = not(ext.A14_IN);
  bool MADY = not(ext.A11_IN);
  bool NENA = not(ext.A9_IN);
  bool SURA = not(ext.A10_IN);
  bool ABUP = not(ext.A5_IN);

  bool ANAR = not(BASE);
  bool AZUV = not(AFEC);
  bool AKAN = not(BUXU);
  bool BYXE = not(CAMU);
  bool BYNE = not(CYGU);
  bool BYNA = not(COGO);
  bool KEJO = not(KOVA);
  bool LYNA = not(LAHE);
  bool LEFY = not(LURA);
  bool LOFA = not(MUJY);
  bool NEFE = not(PEVO);
  bool LORA = not(MADY);
  bool MAPU = not(NENA);
  bool RALA = not(SURA);
  bool AJOV = not(ABUP);

  // if NET01 high, drive external address bus onto internal address bus (?)
  if (NET01) {
    mem.A3  = ANAR;
    mem.A4  = AZUV;
    mem.A2  = AKAN;
    mem.A1  = BYXE;
    mem.A6  = BYNE;
    mem.A7  = BYNA;
    mem.A0  = KEJO;
    mem.A12 = LYNA;
    mem.A13 = LEFY;
    mem.A8  = LOFA;
    mem.A14 = NEFE;
    mem.A11 = LORA;
    mem.A9  = MAPU;
    mem.A10 = RALA;
    mem.A5  = AJOV;
  }
}
