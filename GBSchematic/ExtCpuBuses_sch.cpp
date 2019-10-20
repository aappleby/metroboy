// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "ExtCpuBuses_sch.h"
#include "CpuBus.h"
#include "ExtBus.h"
#include "MemBus.h"
#include "DmaBus.h"
#include "AddressDecoder.h"
#include "Debug.h"

//-----------------------------------------------------------------------------
// ABUZ is a funny clock
// LUMA is some dma signal

void ExtCpuBuses_sch::tick(bool ABUZ, bool LUMA, const AddressDecoder& dec, MemBus& mem) {

  //----------
  // center right

  TEXO = and(cpu.FROM_CPU4, mem.A13, mem.A14, not(mem.A15));
  wire LEVO = not(TEXO);
  wire LAGU = unk3(cpu.CPU_RAW_RD, LEVO, cpu.FROM_CPU3);
  wire LYWE = not(LAGU);

  wire MOCA = nor(TEXO, dbg.T1T2n);
  wire MEXO = not(cpu.CPU_RD_SYNC);
  wire MOTY = or(MOCA, LYWE);

  ext.WR_C = nor(dbg.T1nT2, or(or(MEXO, MOCA), LUMA));
  ext.WR_A = nand(or(or(MEXO, MOCA), LUMA), dbg.NET01);
  ext.RD_A = nand(dbg.NET01, or(LUMA, MOTY));
  ext.RD_C = nor(or(LUMA, MOTY), dbg.T1nT2);

  //----------
  // top center

  wire TOLA = not(mem.A1);
  mem.TOLA_A1n = TOLA;

  wire MULE = not(dbg.T1T2n);
  wire LOXO = unk3(MULE, TEXO, dbg.T1T2n);
  wire LASY = not(LOXO);
  wire MATE = not(LASY);

  //----------
  // left center

  wire TYNU = unk3(mem.A15, mem.A14, and(mem.A13, not(mem.A14), mem.A15));
  wire SOBY = nor(mem.A15, cpu.CPU_WRQ); // schematic has a question mark?

  wire TOZA = and(TYNU, ABUZ, dec.FEXXFFXXn);
  wire SEPY = nand(ABUZ, nor(mem.A15, cpu.CPU_WRQ));

  wire TYHO = mux2(dma.DMA_A15, TOZA, LUMA);
  wire TAZY = mux2(dma.DMA_A15, SEPY, LUMA);

  ext.CS_OUT = TYHO;
  mem.A15 = ext.A15_C;
  ext.A15_A = nand(TAZY, not(dbg.NET02));
  ext.A15_D = nor(TAZY, dbg.NET02);

  //----------
  // bottom left

  wire NYRE_Q = NYRE.latch(MATE, mem.A14);
  wire LONU_Q = LONU.latch(MATE, mem.A13);
  wire LOBU_Q = LOBU.latch(MATE, mem.A12);
  wire LUMY_Q = LUMY.latch(MATE, mem.A11);
  wire PATE_Q = PATE.latch(MATE, mem.A10);
  wire LYSA_Q = LYSA.latch(MATE, mem.A9);
  wire LUNO_Q = LUNO.latch(MATE, mem.A8);

  wire MANO = mux2(dma.DMA_A8,  LUNO_Q, LUMA);
  wire MASU = mux2(dma.DMA_A9,  LYSA_Q, LUMA);
  wire PAMY = mux2(dma.DMA_A10, PATE_Q, LUMA);
  wire MALE = mux2(dma.DMA_A11, LUMY_Q, LUMA);
  wire MOJY = mux2(dma.DMA_A12, LOBU_Q, LUMA);
  wire MUCE = mux2(dma.DMA_A13, LONU_Q, LUMA);
  wire PEGE = mux2(dma.DMA_A14, NYRE_Q, LUMA);

  ext.A8_D  = nor(dbg.T1nT2, MANO);
  ext.A9_D  = nor(dbg.T1nT2, MASU);
  ext.A10_D = nor(dbg.T1nT2, PAMY);
  ext.A11_D = nor(dbg.T1nT2, MALE);
  ext.A12_D = nor(dbg.T1nT2, MOJY);
  ext.A13_D = nor(dbg.T1nT2, MUCE);
  ext.A14_D = nor(dbg.T1nT2, PEGE);

  ext.A8_A  = nand(MANO, not(dbg.T1nT2));
  ext.A9_A  = nand(MASU, not(dbg.T1nT2));
  ext.A10_A = nand(PAMY, not(dbg.T1nT2));
  ext.A14_A = nand(PEGE, not(dbg.T1nT2));
  ext.A11_A = nand(MALE, not(dbg.T1nT2));
  ext.A12_A = nand(MOJY, not(dbg.T1nT2));
  ext.A13_A = nand(MUCE, not(dbg.T1nT2));

  //----------
  // the rest of the address latch, center

  wire ALOR_Q = ARYM.latch(MATE, mem.A0);
  wire APUR_Q = ARYM.latch(MATE, mem.A1);
  wire ALYR_Q = ARYM.latch(MATE, mem.A2);
  wire ARET_Q = ARYM.latch(MATE, mem.A3);
  wire AVYS_Q = ARYM.latch(MATE, mem.A4);
  wire ATEV_Q = ARYM.latch(MATE, mem.A5);
  wire AROS_Q = ARYM.latch(MATE, mem.A6);
  wire ARYM_Q = ARYM.latch(MATE, mem.A7);

  wire AMET = mux2(dma.DMA_A0, ALOR_Q, LUMA);
  wire ATOL = mux2(dma.DMA_A1, APUR_Q, LUMA);
  wire APOK = mux2(dma.DMA_A2, ALYR_Q, LUMA);
  wire AMER = mux2(dma.DMA_A3, ARET_Q, LUMA);
  wire ATEM = mux2(dma.DMA_A4, AVYS_Q, LUMA);
  wire ATOV = mux2(dma.DMA_A5, ATEV_Q, LUMA);
  wire ATYR = mux2(dma.DMA_A6, AROS_Q, LUMA);
  wire ASUR = mux2(dma.DMA_A7, ARYM_Q, LUMA);

  ext.A0_A = nand(dbg.NET01, AMET);
  ext.A1_A = nand(dbg.NET01, ATOL);
  ext.A2_A = nand(dbg.NET01, APOK);
  ext.A3_A = nand(dbg.NET01, AMER);
  ext.A4_A = nand(dbg.NET01, ATEM);
  ext.A5_A = nand(dbg.NET01, ATOV);
  ext.A6_A = nand(dbg.NET01, ATYR);
  ext.A7_A = nand(dbg.NET01, ASUR);

  ext.A0_D = nor (dbg.NET02, AMET);
  ext.A1_D = nor (dbg.NET02, ATOL);
  ext.A2_D = nor (dbg.NET02, APOK);
  ext.A3_D = nor (dbg.NET02, AMER);
  ext.A4_D = nor (dbg.NET02, ATEM);
  ext.A5_D = nor (dbg.NET02, ATOV);
  ext.A6_D = nor (dbg.NET02, ATYR);
  ext.A7_D = nor (dbg.NET02, ASUR);

  //----------
  // bottom right

  wire REDU = not(cpu.CPU_RD);
  RORU = mux2(REDU, MOTY, dbg.T1nT2);
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

  wire LAVO = nand(cpu.CPU_RAW_RD, TEXO, cpu.FROM_CPU5);

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
  // if NET01 high, drive external address bus onto internal address bus for debugging.

  if (dbg.NET01) {
    wire KOVA = not(ext.A0_IN);  wire KEJO = not(KOVA); mem.A0  = KEJO;
    wire CAMU = not(ext.A1_IN);  wire BYXE = not(CAMU); mem.A1  = BYXE;
    wire BUXU = not(ext.A2_IN);  wire AKAN = not(BUXU); mem.A2  = AKAN;
    wire BASE = not(ext.A3_IN);  wire ANAR = not(BASE); mem.A3  = ANAR;
    wire AFEC = not(ext.A4_IN);  wire AZUV = not(AFEC); mem.A4  = AZUV;
    wire ABUP = not(ext.A5_IN);  wire AJOV = not(ABUP); mem.A5  = AJOV;
    wire CYGU = not(ext.A6_IN);  wire BYNE = not(CYGU); mem.A6  = BYNE;
    wire COGO = not(ext.A7_IN);  wire BYNA = not(COGO); mem.A7  = BYNA;
    wire MUJY = not(ext.A8_IN);  wire LOFA = not(MUJY); mem.A8  = LOFA;
    wire NENA = not(ext.A9_IN);  wire MAPU = not(NENA); mem.A9  = MAPU;
    wire SURA = not(ext.A10_IN); wire RALA = not(SURA); mem.A10 = RALA;
    wire MADY = not(ext.A11_IN); wire LORA = not(MADY); mem.A11 = LORA;
    wire LAHE = not(ext.A12_IN); wire LYNA = not(LAHE); mem.A12 = LYNA;
    wire LURA = not(ext.A13_IN); wire LEFY = not(LURA); mem.A13 = LEFY;
    wire PEVO = not(ext.A14_IN); wire NEFE = not(PEVO); mem.A14 = NEFE;
  }
}
